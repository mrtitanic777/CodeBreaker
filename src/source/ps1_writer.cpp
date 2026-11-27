// source/ps1_writer.cpp -- a PlayStation program as assembly source that builds back into it (ps1_writer.hpp).

#include "source/ps1_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <functional>
#include <format>
#include <set>
#include <tuple>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/mips.hpp"
#include "source/isa.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace cb::source::ps1_writer {

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
constexpr unsigned kFunctionReach = 400;       // how many instructions a function is looked at for before it is called data

// An instruction, and whether it is the delay slot of a jump that ends the flow (so the flow ends after it).
struct PsInsn {
    dm::Insn insn;
    bool ends = false;
};

bool ends_flow(const dm::Insn& insn) {
    return insn.delay_slot && (insn.flow == dm::Flow::Jump || insn.flow == dm::Flow::Return || insn.flow == dm::Flow::Indirect);
}

// Flow runs through the program from where the header says, at multiples of four.
class Ps1Flow final : public iw::CodeModel<PsInsn> {
public:
    Ps1Flow(ByteView text, std::uint32_t base) : text_(text), base_(base) {}

    std::optional<PsInsn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        const std::size_t off = pc - base_;
        std::optional<dm::Insn> insn = dm::decode(text_, off, pc);
        if (!insn) return std::nullopt;
        PsInsn out{*insn, false};
        if (off >= 4) {
            const std::optional<dm::Insn> before = dm::decode(text_, off - 4, pc - 4);
            out.ends = before && ends_flow(*before);
        }
        return out;
    }
    std::uint32_t size(const PsInsn&) const override { return 4; }
    std::optional<std::uint32_t> target(const PsInsn& p) const override { return p.insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= base_ && pc % 4 == 0 && pc - base_ + 4 <= text_.size(); }
    bool fits(std::uint32_t pc, const PsInsn&) const override { return inside(pc); }
    bool follows(const PsInsn& p) const override { return p.insn.target && inside(*p.insn.target) && p.insn.flow != dm::Flow::None; }
    bool stops(const PsInsn& p) const override { return p.ends; }
    bool finds_functions() const override { return true; }
    bool starts_function(const PsInsn& p) const override {
        return p.insn.mnemonic == "jal" || (p.insn.mnemonic == "bgezal" && p.insn.rs == 0);
    }

private:
    ByteView text_;
    std::uint32_t base_;
};

std::uint32_t word_at(ByteView text, std::size_t off) {
    return static_cast<std::uint32_t>(text[off]) | static_cast<std::uint32_t>(text[off + 1]) << 8 |
           static_cast<std::uint32_t>(text[off + 2]) << 16 | static_cast<std::uint32_t>(text[off + 3]) << 24;
}

// `addiu $sp, $sp, -N`: how GCC opens a function that calls others or keeps a frame.
bool is_prologue(std::uint32_t word) { return (word >> 16) == 0x27BD && (word & 0x8000) != 0 && (word & 3) == 0; }

// Do the bytes at `at` read as a function: instructions that all decode, whose jumps and branches stay in the program, that end
// in a jump or return (and its delay slot) within a few hundred words? Data does not: random words stop decoding, and zeros
// never end.
bool reads_as_function(ByteView text, std::uint32_t base, std::uint32_t at) {
    const std::uint32_t end = base + static_cast<std::uint32_t>(text.size());
    for (unsigned count = 0; count < kFunctionReach; ++count) {
        const std::uint32_t pc = at + 4 * count;
        if (pc < base || pc + 4 > end) return false;
        const std::optional<dm::Insn> insn = dm::decode(text, pc - base, pc);
        if (!insn) return false;
        if (insn->target && (*insn->target < base || *insn->target >= end || *insn->target % 4 != 0)) {
            if (insn->mnemonic != "jal") return false;   // (a call may go anywhere: the BIOS)
        }
        if (!ends_flow(*insn)) continue;
        if (pc + 8 > end || !dm::decode(text, pc + 4 - base, pc + 4)) return false;
        return count >= 1;
    }
    return false;
}

// Which register an instruction writes (32: none).
unsigned written(const dm::Insn& i) {
    switch (i.form) {
        case dm::Form::RdRsRt: case dm::Form::RdRtSa: case dm::Form::RdRtRs: case dm::Form::Rd: case dm::Form::RdRs: return i.rd;
        case dm::Form::RtRsImm: case dm::Form::RtImm: case dm::Form::RtCop:
            return i.mnemonic == "mtc0" || i.mnemonic == "mtc2" || i.mnemonic == "ctc2" ? 32 : i.rt;
        case dm::Form::RtMem: return i.mnemonic[0] == 'l' && i.mnemonic != "lwc2" ? i.rt : 32;
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
std::vector<Use> address_pairs(const std::map<std::uint32_t, PsInsn>& code, const std::set<std::uint32_t>& starts) {
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
                if (q.mnemonic == "addiu") out.push_back({pc, at, high + static_cast<std::uint32_t>(q.imm), false});
                else if (q.mnemonic == "ori") out.push_back({pc, at, high | static_cast<std::uint32_t>(q.imm), true});
                else if (q.form == dm::Form::RtMem) out.push_back({pc, at, high + static_cast<std::uint32_t>(q.imm), false});
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
    // config/knowledge/ps1/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("ps1", "hardware");
    return table;
}

namespace {

std::uint32_t physical(std::uint32_t a) { return a >= 0xC0000000u ? a : a & 0x1FFFFFFFu; }

// The hardware register at `address` (any of its views), and the name it is written with there.
std::optional<std::string> hardware_name(std::uint32_t address) {
    const auto it = hardware().find(physical(address));
    if (it == hardware().end()) return std::nullopt;
    const std::uint32_t region = address & 0xE0000000u;
    return it->second + (address >= 0xC0000000u || region == 0 ? "" : region == 0x80000000u ? "_K0" : "_K1");
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

iw::Written write(const formats::psx::Exe& exe, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    // (an executable's extension goes; a disc's SLUS_000.01 keeps its whole name, ".01" is no extension)
    const std::filesystem::path named(file_name);
    const bool known = std::ranges::any_of(formats::psx::kExtensions, [&](std::string_view e) { return text::iequals(named.extension().string(), e); });
    const std::string stem = known || !named.has_extension() ? named.stem().string() : named.filename().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const ByteView text = exe.text();
    const std::uint32_t base = exe.header.t_addr;
    const std::uint32_t end = base + static_cast<std::uint32_t>(text.size());
    const Ps1Flow flow(text, base);

    // function starts: what the entry point calls, and the places that begin as a function does after another one's end (a jump and its delay slot)
    std::set<std::uint32_t> entries;
    for (const std::uint32_t a : exe.functions(isa::mips_decoder())) entries.insert(a);
    for (std::size_t off = 8; off + 4 <= text.size(); off += 4) {
        const std::uint32_t at = base + static_cast<std::uint32_t>(off);
        const std::optional<dm::Insn> before = dm::decode(text, off - 8, at - 8);   // (a jump that ends a flow, then its delay slot, then this)
        if (before && ends_flow(*before) && (is_prologue(word_at(text, off)) || reads_as_function(text, base, at))) entries.insert(at);
    }
    if (flow.inside(exe.header.pc0)) entries.insert(exe.header.pc0);

    // walk the code; what the code points at (an address built with `lui`, a word in a table) may be a function it never calls
    iw::Walked<PsInsn> walked;
    std::vector<Use> uses;
    for (unsigned round = 0; round < kRounds; ++round) {
        const std::vector<std::uint32_t> from(entries.begin(), entries.end());
        walked = iw::walk<PsInsn>(flow, from);
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
    const std::map<std::uint32_t, PsInsn>& code = walked.code;
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
        if ((a & 0xDFFFFFFFu) >= 0x80000000u && p < 0x200000) return Kind::Variable;      // RAM, past or before the program
        if (p >= 0x1F800000 && p < 0x1F800400) return Kind::Variable;                       // the scratchpad
        return Kind::None;
    };

    // names: the entry point is `entry`, a function `sub_<address>`
    iw::Names names;
    if (starts.contains(exe.header.pc0)) names[exe.header.pc0] = "entry";
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
    const formats::psx::Header& h = exe.header;
    std::vector<std::string> head = {
        std::format("; {}.s -- written by CodeBreaker from {}", stem, base_name),
        std::format("; PlayStation executable: entry 0x{:08X}, program at 0x{:08X} ({} bytes), {} functions", h.pc0, base, text.size(), functions.size()),
        ""};
    translate::l2::Assembly out(head, "gas", std::string(file_name));
    out.append("        .set noreorder");
    out.append("        .set noat");
    out.append("");
    for (const auto& [name, value] : equates) out.append(std::format("        .equ {}, {}", name, hex8(value)));
    if (!equates.empty()) out.append("");

    // the header, field by field
    out.part("header", "executable header", std::nullopt, formats::psx::kHeaderSize);
    const ByteView file = exe.data;
    const WordNames word_name{&labels};
    const auto field_word = [&](std::size_t at, std::string_view what, bool address) {
        const std::uint32_t v = word_at(file, at);
        out.append(std::format("        .word {}    ; {}", address ? word_name(v) : hex8(v), what));
    };
    out.append("        .ascii \"PS-X EXE\"");
    if (std::ranges::all_of(file.subspan(8, 8), [](std::uint8_t b) { return b == 0; })) out.append("        .space 8");
    else out.append(byte_row(file.subspan(8, 8)));
    field_word(0x10, "pc0: where it starts", true);
    field_word(0x14, "gp0: the initial $gp", false);
    field_word(0x18, "t_addr: where the program is loaded", false);
    field_word(0x1C, "t_size: its length", false);
    field_word(0x20, "data address", false);
    field_word(0x24, "data size", false);
    field_word(0x28, "bss address", false);
    field_word(0x2C, "bss size", false);
    field_word(0x30, "stack base", false);
    field_word(0x34, "stack offset", false);
    if (std::ranges::all_of(file.subspan(0x38, 0x14), [](std::uint8_t b) { return b == 0; })) out.append("        .space 20");
    else out.append(byte_row(file.subspan(0x38, 0x14)));
    // the region marker and the zeros after it (any text: a licensed disc's marker is one string, a homebrew's may be two lines)
    for (std::size_t at = 0x4C; at < formats::psx::kHeaderSize;) {
        const DataItem item = data_item(file, at, formats::psx::kHeaderSize, static_cast<std::uint32_t>(at), [](std::uint32_t) { return std::nullopt; });
        out.append(item.line);
        at += item.took;
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

    const auto text_of = [&](const PsInsn& p) {
        // the description's text, a branch or call by its label, an address half by its expression
        const auto it = immediate.find(p.insn.pc);
        std::map<std::uint32_t, std::string> by_value;
        std::vector<std::uint32_t> operands;
        if (p.insn.target)
            if (const auto named = names_for_text.find(*p.insn.target); named != names_for_text.end()) by_value[*p.insn.target] = named->second, operands.push_back(*p.insn.target);
        return dialect::find("gas-mips").fallback(risc_source::code("mips", text.subspan(p.insn.pc - base, 4), p.insn.pc, by_value, operands, it == immediate.end() ? std::string() : it->second));
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

    std::string source;   // (every line said in GNU as's: `$sp`, decimal offsets, `#` comments)
    for (const std::string& l : out.lines()) source += dialect::find("gas-mips").line(l) + "\n";
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, iw::code_bytes(code, flow), exe.data.size());
    iw::assembled(r, [&] { return risc_source::assemble("mips", dialect::find("gas-mips").parse(source), folder); }, exe.data, folder / (stem + ".built.exe"), "executable");
    return r;
}

iw::Written reconstruct(const std::filesystem::path& exe_file, const std::filesystem::path& folder) {
    const formats::psx::Exe exe = formats::psx::read(exe_file);
    // a disc's program is named as the disc names it (SLUS_007.55.s), not after the image
    const std::string name = exe.boot_file.empty() ? exe_file.filename().string() : std::filesystem::path(exe.boot_file).filename().string();
    return write(exe, name, folder);
}

}  // namespace cb::source::ps1_writer
