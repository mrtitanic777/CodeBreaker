// cb-unity: alone (file-local names that other files in a batch also use)
// source/n64_writer.cpp -- a Nintendo 64 ROM as assembly source that builds back into it (n64_writer.hpp).

#include "source/n64_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <set>
#include <tuple>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/mips.hpp"
#include "source/isa_r4300.hpp"
#include "source/risc_source.hpp"

namespace cb::source::n64_writer {

namespace dm = decompile::mips;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kSub{"sub_", 8};
constexpr iw::Spelling kLocal{"L_", 8};
constexpr iw::Spelling kData{"D_", 8};
constexpr unsigned kLookAhead = 8;           // instructions after a `lui` that may use what it loaded
constexpr unsigned kStringMin = 4;           // a run of printable bytes this long is a string
constexpr unsigned kStringMax = 96;
constexpr std::size_t kFillMin = 16;         // a run of one value this long is `.space` / `.fill`
constexpr unsigned kRounds = 4;              // how many times what the code points at may add functions
constexpr unsigned kFunctionReach = 400;     // how many instructions a function is looked at for before it is called data
constexpr std::uint32_t kCartKseg1 = 0xB0000000;   // the cartridge as a program reads it: ROM offset + this

// An instruction, and whether it is the delay slot of a jump that ends the flow (so the flow ends after it).
struct N64Insn {
    dm::Insn insn;
    bool ends = false;
};

bool ends_flow(const dm::Insn& insn) {
    return insn.delay_slot && (insn.flow == dm::Flow::Jump || insn.flow == dm::Flow::Return || insn.flow == dm::Flow::Indirect);
}

// Flow runs through the program from its entry point, at multiples of four.
class N64Flow final : public iw::CodeModel<N64Insn> {
public:
    N64Flow(ByteView text, std::uint32_t base) : text_(text), base_(base) {}

    std::optional<N64Insn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        const std::size_t off = pc - base_;
        std::optional<dm::Insn> insn = dm::decode(text_, off, pc, dm::Variant::R4300);
        if (!insn) return std::nullopt;
        N64Insn out{*insn, false};
        if (off >= 4) {
            const std::optional<dm::Insn> before = dm::decode(text_, off - 4, pc - 4, dm::Variant::R4300);
            out.ends = before && ends_flow(*before);
        }
        return out;
    }
    std::uint32_t size(const N64Insn&) const override { return 4; }
    std::optional<std::uint32_t> target(const N64Insn& p) const override { return p.insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= base_ && pc % 4 == 0 && pc - base_ + 4 <= text_.size(); }
    bool fits(std::uint32_t pc, const N64Insn&) const override { return inside(pc); }
    bool follows(const N64Insn& p) const override { return p.insn.target && inside(*p.insn.target) && p.insn.flow != dm::Flow::None; }
    bool stops(const N64Insn& p) const override { return p.ends || p.insn.mnemonic == "eret"; }
    bool finds_functions() const override { return true; }
    bool starts_function(const N64Insn& p) const override {
        return p.insn.mnemonic == "jal" || ((p.insn.mnemonic == "bgezal" || p.insn.mnemonic == "bgezall") && p.insn.rs == 0);
    }

private:
    ByteView text_;
    std::uint32_t base_;
};

std::uint32_t word_at(ByteView text, std::size_t off) {
    return static_cast<std::uint32_t>(text[off]) << 24 | static_cast<std::uint32_t>(text[off + 1]) << 16 | static_cast<std::uint32_t>(text[off + 2]) << 8 |
           static_cast<std::uint32_t>(text[off + 3]);
}

// `addiu $sp, $sp, -N` or `daddiu $sp, $sp, -N`: how GCC opens a function that calls others or keeps a frame.
bool is_prologue(std::uint32_t word) { return ((word >> 16) == 0x27BD || (word >> 16) == 0x67BD) && (word & 0x8000) != 0 && (word & 3) == 0; }

// Do the bytes at `at` read as a function: instructions that all decode, whose jumps and branches stay in the program, that end
// in a jump or return (and its delay slot) within a few hundred words? Data does not: random words stop decoding, and zeros
// never end.
bool reads_as_function(ByteView text, std::uint32_t base, std::uint32_t at) {
    const std::uint32_t end = base + static_cast<std::uint32_t>(text.size());
    for (unsigned count = 0; count < kFunctionReach; ++count) {
        const std::uint32_t pc = at + 4 * count;
        if (pc < base || pc + 4 > end) return false;
        const std::optional<dm::Insn> insn = dm::decode(text, pc - base, pc, dm::Variant::R4300);
        if (!insn) return false;
        if (insn->target && (*insn->target < base || *insn->target >= end || *insn->target % 4 != 0)) {
            if (insn->mnemonic != "jal") return false;   // (a call may go anywhere: the console's code in ROM, a library)
        }
        if (insn->mnemonic == "eret") return count >= 1;
        if (!ends_flow(*insn)) continue;
        if (pc + 8 > end || !dm::decode(text, pc + 4 - base, pc + 4, dm::Variant::R4300)) return false;
        return count >= 1;
    }
    return false;
}

// Which register an instruction writes (32: none).
unsigned written(const dm::Insn& i) {
    const std::string& m = i.mnemonic;
    switch (i.form) {
        case dm::Form::RdRsRt: case dm::Form::RdRtSa: case dm::Form::RdRtRs: case dm::Form::Rd: case dm::Form::RdRs: return i.rd;
        case dm::Form::RtRsImm: case dm::Form::RtImm: return i.rt;
        case dm::Form::RtCop: return m == "mfc0" || m == "dmfc0" || m == "cfc1" ? i.rt : 32;
        case dm::Form::RtFs: return m == "mfc1" || m == "dmfc1" ? i.rt : 32;
        case dm::Form::RtMem: return m[0] == 'l' || m == "sc" || m == "scd" ? i.rt : 32;
        case dm::Form::Jump: return i.flow == dm::Flow::Call ? 31 : 32;
        case dm::Form::RsBranch: return i.flow == dm::Flow::Call ? 31 : 32;
        default: return 32;
    }
}

// One use of what a `lui` loaded: the instruction that finishes the address.
struct Use {
    std::uint32_t lui;        // the `lui`'s address
    std::uint32_t at;         // the instruction that completes the address
    std::uint32_t address;    // the address they make
    bool ori;                 // by ORI: the high half is not adjusted for the low half's sign
};

// Every `lui` and what it is used for, as far as the code shows (the next few instructions, until the register is written or the flow
// leaves).
std::vector<Use> address_pairs(const std::map<std::uint32_t, N64Insn>& code, const std::set<std::uint32_t>& starts) {
    std::vector<Use> out;
    for (const auto& [pc, p] : code) {
        if (p.insn.mnemonic != "lui") continue;
        const unsigned reg = p.insn.rt;
        if (reg == 0) continue;
        const std::uint32_t high = static_cast<std::uint32_t>(p.insn.imm) << 16;
        bool last = false;   // the instruction being looked at is the delay slot of a flow-ending jump or of a call
        for (unsigned k = 1; k <= kLookAhead; ++k) {
            const std::uint32_t at = pc + 4 * k;
            const auto it = code.find(at);
            if (it == code.end() || starts.contains(at)) break;
            const dm::Insn& q = it->second.insn;
            if (q.rs == reg) {
                if (q.mnemonic == "addiu" || q.mnemonic == "daddiu") out.push_back({pc, at, high + static_cast<std::uint32_t>(q.imm), false});
                else if (q.mnemonic == "ori") out.push_back({pc, at, high | static_cast<std::uint32_t>(q.imm), true});
                else if (q.form == dm::Form::RtMem || q.form == dm::Form::FtMem) out.push_back({pc, at, high + static_cast<std::uint32_t>(q.imm), false});
            }
            if (written(q) == reg || last) break;
            if (q.delay_slot) last = true;   // (the delay slot is the next instruction; then the flow has gone elsewhere)
        }
    }
    return out;
}

// How an address is named: part of the program's text, a hardware register, or a variable in RAM.
enum class Kind { None, Text, Hardware, Variable };

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/n64/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("n64", "hardware");
    return table;
}

namespace {

std::uint32_t physical(std::uint32_t a) { return a >= 0xC0000000u ? a : a & 0x1FFFFFFFu; }

// The register at `address` (any of its views): KSEG1, how programs write them, gives the bare name; the other views a suffix.
std::optional<std::string> hardware_name(std::uint32_t address) {
    const auto it = hardware().find(physical(address));
    if (it == hardware().end()) return std::nullopt;
    const std::uint32_t region = address & 0xE0000000u;
    return it->second + (address >= 0xC0000000u || region == 0xA0000000u ? "" : region == 0x80000000u ? "_K0" : "_PHYS");
}

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }

std::string hex8(std::uint32_t v) { return std::format("0x{:08X}", v); }

std::string byte_row(ByteView bytes) {
    std::vector<std::string> items;
    for (const std::uint8_t b : bytes) items.push_back(std::format("0x{:02X}", b));
    return "        .byte " + text::join(items, ", ");
}

// A word and its meaning in the source: an address in the program is its label.
struct WordNames {
    const iw::Names* labels;
    std::string operator()(std::uint32_t v) const {
        const auto it = labels->find(v);
        return it != labels->end() ? it->second : hex8(v);
    }
};

std::size_t printable_run(ByteView buf, std::size_t off, std::size_t limit) {
    std::size_t run = 0;
    while (off + run < limit && run < kStringMax && printable(buf[off + run])) ++run;
    return run;
}

std::size_t same_run(ByteView buf, std::size_t off, std::size_t limit) {
    std::size_t run = 1;
    while (off + run < limit && buf[off + run] == buf[off]) ++run;
    return run;
}

// Where a string starts inside the word at buf[at] (1 to 3 bytes in), or 0.
std::size_t string_inside_word(ByteView buf, std::size_t at, std::size_t limit) {
    for (std::size_t k = 1; k < 4 && at + k < limit; ++k)
        if (printable_run(buf, at + k, limit) >= kStringMin) return k;
    return 0;
}

// One line of data from buf[off], which is at `addr`, to before `limit`: a string, a run of one value, up to four words (a word that
// `pointer_at` names is written as the name), or a few bytes. How many bytes it took, and the line.
struct DataItem {
    std::string line;
    std::size_t took = 0;
};
DataItem data_item(ByteView buf, std::size_t off, std::size_t limit, std::uint32_t addr,
                   const std::function<std::optional<std::string>(std::uint32_t)>& pointer_at) {
    if (const std::size_t run = printable_run(buf, off, limit); run >= kStringMin) {
        const bool terminated = off + run < limit && buf[off + run] == 0;
        return {std::format("        {} \"{}\"", terminated ? ".asciz" : ".ascii", as_text(buf.subspan(off, run))), run + (terminated ? 1u : 0u)};
    }
    if (const std::size_t same = same_run(buf, off, limit); same >= kFillMin)
        return {buf[off] == 0 ? std::format("        .space {}", same) : std::format("        .fill {}, 1, 0x{:02X}", same, buf[off]), same};
    if (addr % 4 == 0 && off + 4 <= limit) {
        if (const std::size_t lead = string_inside_word(buf, off, limit); lead != 0) return {byte_row(buf.subspan(off, lead)), lead};   // (the bytes before a string)
        std::vector<std::string> items;
        std::size_t took = 0;
        while (items.size() < 4 && off + took + 4 <= limit) {
            const std::size_t at = off + took;
            if (took != 0 && (printable_run(buf, at, limit) >= kStringMin || same_run(buf, at, limit) >= kFillMin || string_inside_word(buf, at, limit) != 0)) break;
            const std::uint32_t v = word_at(buf, at);
            const std::optional<std::string> name = pointer_at(addr + static_cast<std::uint32_t>(took));
            items.push_back(name ? *name : hex8(v));
            took += 4;
        }
        return {"        .word " + text::join(items, ", "), took};
    }
    const std::size_t room = addr % 4 != 0 ? 4 - addr % 4 : limit - off;
    const std::size_t count = std::min<std::size_t>({room, limit - off, 16});
    return {byte_row(buf.subspan(off, count)), count};
}

}  // namespace

iw::Written write(const formats::n64::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const ByteView text = rom.program();
    const std::uint32_t base = rom.load_address();
    const std::uint32_t end = base + static_cast<std::uint32_t>(text.size());
    const N64Flow flow(text, base);

    // function starts: what the entry point calls, and the places that begin as a function does after another one's end (a jump and its delay slot)
    std::set<std::uint32_t> entries;
    for (const std::uint32_t a : rom.functions(isa::r4300_decoder())) entries.insert(a);
    for (std::size_t off = 8; off + 4 <= text.size(); off += 4) {
        const std::uint32_t at = base + static_cast<std::uint32_t>(off);
        const std::optional<dm::Insn> before = dm::decode(text, off - 8, at - 8, dm::Variant::R4300);   // (a jump that ends a flow, then its delay slot, then this)
        if (before && ends_flow(*before) && (is_prologue(word_at(text, off)) || reads_as_function(text, base, at))) entries.insert(at);
    }
    if (flow.inside(base)) entries.insert(base);

    // walk the code; what the code points at (an address built with `lui`, a word in a table) may be a function it never calls
    iw::Walked<N64Insn> walked;
    std::vector<Use> uses;
    for (unsigned round = 0; round < kRounds; ++round) {
        const std::vector<std::uint32_t> from(entries.begin(), entries.end());
        walked = iw::walk<N64Insn>(flow, from);
        const std::set<std::uint32_t> starts(walked.starts.begin(), walked.starts.end());
        uses = address_pairs(walked.code, starts);
        std::set<std::uint32_t> pointed;
        for (const Use& u : uses) pointed.insert(u.address);
        for (std::size_t off = 0; off + 4 <= text.size(); off += 4) {
            const std::uint32_t a = base + static_cast<std::uint32_t>(off);
            if (!walked.code.contains(a)) pointed.insert(word_at(text, off));
        }
        bool added = false;
        for (const std::uint32_t a : pointed) {
            if (a >= base && a < end && a % 4 == 0 && !walked.code.contains(a) && (is_prologue(word_at(text, a - base)) || reads_as_function(text, base, a)))
                added |= entries.insert(a).second;
        }
        if (!added) break;
    }
    const std::map<std::uint32_t, N64Insn>& code = walked.code;
    std::vector<std::uint32_t> function_starts;   // (an entry point whose bytes are not an instruction is not a function)
    for (const std::uint32_t a : walked.starts)
        if (code.contains(a)) function_starts.push_back(a);
    const std::set<std::uint32_t> starts(function_starts.begin(), function_starts.end());
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a >= base && a < end && !inside_insn.contains(a); };
    const auto classify = [&](std::uint32_t a) {
        if (a >= base && a < end) return line_start(a) ? Kind::Text : Kind::None;
        if (hardware_name(a)) return Kind::Hardware;
        const std::uint32_t p = physical(a);
        if ((a & 0xDFFFFFFFu) >= 0x80000000u && p < formats::n64::kRdramSize) return Kind::Variable;   // RDRAM, past or before the program
        if (p >= 0x04000000 && p < 0x04002000) return Kind::Variable;                                // SP memory
        if (p >= 0x10000000 && p < 0x1FC00000) return Kind::Variable;                                // the cartridge
        if (p >= 0x1FC007C0 && p < 0x1FC00800) return Kind::Variable;                                // PIF RAM
        return Kind::None;
    };

    // names: the entry point is `entry`, a function `sub_<address>`
    iw::Names names;
    if (starts.contains(base)) names[base] = "entry";
    const iw::Functions functions = iw::name_functions(function_starts, names, kSub);
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);

    // what the code and the tables point at
    std::map<std::string, std::uint32_t> equates;    // the names an address outside the labels is written with
    std::unordered_map<std::uint32_t, std::string> immediate;   // by the instruction's address: how its immediate is written
    const auto name_of = [&](std::uint32_t a) -> std::optional<std::string> {
        switch (classify(a)) {
            case Kind::Text: {
                const auto it = labels.find(a);
                if (it != labels.end()) return it->second;
                const std::string made = (code.contains(a) ? kLocal : kData)(a);
                labels[a] = made;
                return made;
            }
            case Kind::Hardware: {
                std::string n = *hardware_name(a);
                equates[n] = a;
                return n;
            }
            case Kind::Variable: {
                std::string n = std::format("var_{:08X}", a);
                equates[n] = a;
                return n;
            }
            case Kind::None: break;
        }
        return std::nullopt;
    };
    // a `lui` is written from one of its uses: an addiu, load or store's (the high half is adjusted for the low half's sign) over an ori's, and
    // of those the lowest address
    struct HighHalf {
        std::uint32_t address;
        bool ori;
        std::string expression;
    };
    std::map<std::uint32_t, HighHalf> high_half;   // by the `lui`'s address
    for (const Use& u : uses) {
        const std::optional<std::string> name = name_of(u.address);
        if (!name) continue;
        immediate[u.at] = "%lo(" + *name + ")";
        HighHalf candidate{u.address, u.ori, u.ori ? "%up(" + *name + ")" : "%hi(" + *name + ")"};
        const auto it = high_half.find(u.lui);
        if (it == high_half.end() || std::tie(candidate.ori, candidate.address) < std::tie(it->second.ori, it->second.address)) high_half[u.lui] = std::move(candidate);
    }
    for (const auto& [pc, high] : high_half) immediate[pc] = high.expression;

    // words in the data that hold an address in the program (a jump table, a table of pointers)
    std::set<std::uint32_t> pointer_words;
    for (std::size_t off = 0; off + 4 <= text.size(); off += 4) {
        const std::uint32_t a = base + static_cast<std::uint32_t>(off);
        if (a % 4 != 0 || code.contains(a) || inside_insn.contains(a) || inside_insn.contains(a + 1) || inside_insn.contains(a + 2) || inside_insn.contains(a + 3)) continue;
        const std::uint32_t v = word_at(text, off);
        if (v >= base && v < end && line_start(v) && name_of(v)) pointer_words.insert(a);
    }

    // the source ----------------------------------------------------------------------------------------------------------------
    const formats::n64::Header& h = rom.header;
    const auto sums = rom.checksums();
    std::string crc_note;
    if (!sums) crc_note = "CRCs not checked (the ROM is shorter than the megabyte they cover)";
    else if (rom.checksums_match()) crc_note = "CRCs match the program's";
    else crc_note = std::format("CRCs do not match the program's ({:08X} {:08X}); kept as they are", sums->first, sums->second);
    std::vector<std::string> head = {
        std::format("; {}.s -- written by CodeBreaker from {}", stem, base_name),
        std::format("; Nintendo 64 ROM \"{}\" ({}{}), {}: entry 0x{:08X}, program at 0x{:08X} ({} bytes), {} functions", h.name, h.game_code, h.country, formats::n64::order_text(rom.order), h.entry,
                    base, text.size(), functions.size()),
        std::format("; boot code {}; {}", formats::n64::cic_text(rom.cic), crc_note),
        std::format("; this source is the ROM in the console's byte order (.z64){}", rom.order == formats::n64::ByteOrder::Z64 ? "" : "; the rebuilt file is made in the file's own order"),
        "; (MIPS syntax as the processor's SLEIGH description writes it: build with `assemble --cpu r4300 --sleigh`)", ""};
    translate::l2::Assembly out(head, "sleigh-r4300", std::string(file_name));
    out.append("        .set noreorder");
    out.append("        .set noat");
    out.append("");
    for (const auto& [name, value] : equates) out.append(std::format("        .equ {}, {}", name, hex8(value)));
    if (!equates.empty()) out.append("");

    // the header, field by field
    const ByteView file = rom.data;
    out.part("header", "ROM header", std::nullopt, formats::n64::kHeaderSize);
    const WordNames word_name{&labels};
    const auto field_word = [&](std::size_t at, std::string_view what, bool address) {
        const std::uint32_t v = word_at(file, at);
        out.append(std::format("        .word {}    ; {}", address ? word_name(v) : hex8(v), what));
    };
    field_word(0x00, "PI bus configuration", false);
    field_word(0x04, "clock rate", false);
    field_word(0x08, "entry point", true);
    field_word(0x0C, "release", false);
    field_word(0x10, "CRC1", false);
    field_word(0x14, std::format("CRC2: {}", crc_note), false);
    const auto field_bytes = [&](std::size_t at, std::size_t n, std::string_view what) {
        const ByteView part = file.subspan(at, n);
        if (std::ranges::all_of(part, [](std::uint8_t b) { return b == 0; })) {
            out.append(std::format("        .space {}    ; {}", n, what));
        } else if (std::ranges::all_of(part, printable)) {
            out.append(std::format("        .ascii \"{}\"    ; {}", as_text(part), what));
        } else {
            out.append(byte_row(part) + "    ; " + std::string(what));
        }
    };
    field_bytes(0x18, 8, "reserved");
    field_bytes(0x20, 20, "name");
    field_bytes(0x34, 7, "reserved");
    out.append(std::format("        .byte 0x{:02X}    ; category", file[0x3B]));
    field_bytes(0x3C, 2, "game code");
    out.append(std::format("        .byte 0x{:02X}, 0x{:02X}    ; country, version", file[0x3E], file[0x3F]));

    // the boot code, as data
    out.part("boot", "boot code (IPL3)", std::nullopt, formats::n64::kBootSize - formats::n64::kHeaderSize);
    {
        const ByteView boot = file.subspan(formats::n64::kHeaderSize, formats::n64::kBootSize - formats::n64::kHeaderSize);
        for (std::size_t at = 0; at < boot.size();) {
            const DataItem item = data_item(boot, at, boot.size(), static_cast<std::uint32_t>(formats::n64::kHeaderSize + at), [](std::uint32_t) { return std::nullopt; });
            out.append(item.line);
            at += item.took;
        }
    }

    // the program
    out.part("text", "program", std::nullopt);
    out.append("");
    out.append(std::format("        .org {}", hex8(base)));

    std::set<std::uint32_t> boundaries = {static_cast<std::uint32_t>(text.size())};
    for (const auto& entry : code) boundaries.insert(entry.first - base);
    for (const auto& entry : labels)
        if (line_start(entry.first)) boundaries.insert(entry.first - base);
    decompile::AddressLabels names_for_text;
    for (const auto& [a, label] : labels) names_for_text[a] = label;

    const auto text_of = [&](const N64Insn& p) {
        // the description's text, a branch or call by its label, an address half by its expression
        const auto it = immediate.find(p.insn.pc);
        std::map<std::uint32_t, std::string> by_value;
        std::vector<std::uint32_t> operands;
        if (p.insn.target)
            if (const auto named = names_for_text.find(*p.insn.target); named != names_for_text.end()) by_value[*p.insn.target] = named->second, operands.push_back(*p.insn.target);
        return risc_source::code("r4300", text.subspan(p.insn.pc - base, 4), p.insn.pc, by_value, operands, it == immediate.end() ? std::string() : it->second);
    };
    std::size_t off = 0;
    while (off < text.size()) {
        const std::uint32_t addr = base + static_cast<std::uint32_t>(off);
        if (const auto label = labels.find(addr); label != labels.end() && line_start(addr)) {
            if (const auto fn = functions.find(addr); fn != functions.end()) {
                out.part(fn->second, "function", translate::ir::OriginVa{out.module().value_or(""), addr});   // each function a fragment
                out.append("");
            }
            out.append(label->second + ":");
        }
        if (const auto it = code.find(addr); it != code.end()) {
            out.append("        " + text_of(it->second));
            off += 4;
            continue;
        }
        const std::size_t limit = *boundaries.upper_bound(static_cast<std::uint32_t>(off));
        const DataItem item = data_item(text, off, limit, addr, [&](std::uint32_t a) -> std::optional<std::string> {
            if (!pointer_words.contains(a)) return std::nullopt;
            return word_name(word_at(text, a - base));
        });
        out.append(item.line);
        off += item.took;
    }

    // what is past the first megabyte: data the game reads through the PI
    const std::size_t rest_at = formats::n64::kBootSize + text.size();
    if (rest_at < file.size()) {
        out.part("cartridge", "cartridge data", std::nullopt);
        out.append("");
        out.append(std::format("        .org {}", hex8(kCartKseg1 + static_cast<std::uint32_t>(rest_at))));
        const ByteView rest = file.subspan(rest_at);
        for (std::size_t at = 0; at < rest.size();) {
            const DataItem item = data_item(rest, at, rest.size(), kCartKseg1 + static_cast<std::uint32_t>(rest_at + at), [](std::uint32_t) { return std::nullopt; });
            out.append(item.line);
            at += item.took;
        }
    }

    const std::string source = iw::text(out) + "\n";
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), rom.data.size());
    const std::string built_name = stem + ".built" + std::string(formats::n64::order_text(rom.order));
    iw::assembled(
        r, [&] { return formats::n64::Rom::convert(risc_source::assemble("r4300", source, folder), formats::n64::ByteOrder::Z64, rom.order); }, rom.in_order(), folder / built_name, "ROM");
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(formats::n64::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::n64_writer
