// source/console_mips.cpp -- the console writer's `vectored` shape for a MIPS program (console_writer.hpp): the PlayStation's R3000A (a PS-X EXE)
// and the Nintendo 64's R4300 (a ROM), written as GNU as's source with `.set noreorder` and each delay slot on a line of its own.
//
// Code is what the program's own flow reaches from the loader's starts and the entry (decompile/mips knows which instruction ends a flow: a jump
// or return, after its delay slot; the R4300's ERET without one), and from the functions it calls; a function only called through a pointer is
// found where one begins after another's end (the bytes after a jump and its delay slot, when they read as a function: every word an
// instruction, ending in a jump or return) or where a pointer to it is -- built in the code (`lui`/`addiu`) or held in a table. Every byte it
// reaches is an instruction, every other byte data: strings as `.ascii`/`.asciz`, words as `.word` (a word holding an address in the program is
// that label), runs of one value as `.space`/`.fill`, the rest `.byte`. An address the code builds with a pair is the pair's `%hi(name)` and
// `%lo(name)` over a label, an `.equ var_<address>` for a variable outside the program, or the hardware register's name. Functions are `entry`
// or `sub_<address>`, jump targets `L_<address>`, data `D_<address>`. The header is written field by field, as the description lists them.

#include "source/console_risc.hpp"
#include "source/console_shapes.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <set>
#include <tuple>
#include <unordered_map>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/mips.hpp"
#include "knowledge/package.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace cb::source::console_writer::mips_code {

namespace {

namespace dm = decompile::mips;
namespace iw = image_writer;

constexpr iw::Spelling kSub{"sub_", 8};
constexpr iw::Spelling kLocal{"L_", 8};
constexpr iw::Spelling kData{"D_", 8};
constexpr unsigned kLookAhead = 8;           // instructions after a `lui` that may use what it loaded
constexpr unsigned kRounds = 4;              // how many times what the code points at may add functions
constexpr unsigned kFunctionReach = 400;     // how many instructions a function is looked at for before it is called data

// The processor: the R3000A or the R4300 (branch likely, doubleword and FPU instructions, ERET), and the byte order of its words.
struct Cpu {
    bool r4300 = false;
    bool big = false;

    std::optional<dm::Insn> decode(ByteView text, std::size_t off, std::uint32_t pc) const {
        return r4300 ? dm::decode(text, off, pc, dm::Variant::R4300) : dm::decode(text, off, pc);
    }
    std::uint32_t word_at(ByteView text, std::size_t off) const { return risc::word_at(text, off, big); }
    // `addiu $sp, $sp, -N` (or the R4300's `daddiu`): how GCC opens a function that calls others or keeps a frame.
    bool is_prologue(std::uint32_t word) const {
        return ((word >> 16) == 0x27BD || (r4300 && (word >> 16) == 0x67BD)) && (word & 0x8000) != 0 && (word & 3) == 0;
    }
    // Which register an instruction writes (32: none).
    unsigned written(const dm::Insn& i) const {
        const std::string& m = i.mnemonic;
        if (r4300) {
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
        switch (i.form) {
            case dm::Form::RdRsRt: case dm::Form::RdRtSa: case dm::Form::RdRtRs: case dm::Form::Rd: case dm::Form::RdRs: return i.rd;
            case dm::Form::RtRsImm: case dm::Form::RtImm: case dm::Form::RtCop: return m == "mtc0" || m == "mtc2" || m == "ctc2" ? 32 : i.rt;
            case dm::Form::RtMem: return m[0] == 'l' && m != "lwc2" ? i.rt : 32;
            case dm::Form::Jump: return i.flow == dm::Flow::Call ? 31 : 32;
            case dm::Form::RsBranch: return i.flow == dm::Flow::Call ? 31 : 32;
            default: return 32;
        }
    }
};

// An instruction, and whether it is the delay slot of a jump that ends the flow (so the flow ends after it).
struct MInsn {
    dm::Insn insn;
    bool ends = false;
};

bool ends_flow(const dm::Insn& insn) {
    return insn.delay_slot && (insn.flow == dm::Flow::Jump || insn.flow == dm::Flow::Return || insn.flow == dm::Flow::Indirect);
}

// Flow runs through the program at multiples of four.
class MipsFlow final : public iw::CodeModel<MInsn> {
public:
    MipsFlow(Cpu cpu, ByteView text, std::uint32_t base) : cpu_(cpu), text_(text), base_(base) {}

    std::optional<MInsn> decode(std::uint32_t pc) const override {
        if (!inside(pc)) return std::nullopt;
        const std::size_t off = pc - base_;
        std::optional<dm::Insn> insn = cpu_.decode(text_, off, pc);
        if (!insn) return std::nullopt;
        MInsn out{*insn, false};
        if (off >= 4) {
            const std::optional<dm::Insn> before = cpu_.decode(text_, off - 4, pc - 4);
            out.ends = before && ends_flow(*before);
        }
        return out;
    }
    std::uint32_t size(const MInsn&) const override { return 4; }
    std::optional<std::uint32_t> target(const MInsn& p) const override { return p.insn.target; }
    bool inside(std::uint32_t pc) const override { return pc >= base_ && pc % 4 == 0 && pc - base_ + 4 <= text_.size(); }
    bool fits(std::uint32_t pc, const MInsn&) const override { return inside(pc); }
    bool follows(const MInsn& p) const override { return p.insn.target && inside(*p.insn.target) && p.insn.flow != dm::Flow::None; }
    bool stops(const MInsn& p) const override { return p.ends || (cpu_.r4300 && p.insn.mnemonic == "eret"); }
    bool finds_functions() const override { return true; }
    bool starts_function(const MInsn& p) const override {
        return p.insn.mnemonic == "jal" || ((p.insn.mnemonic == "bgezal" || (cpu_.r4300 && p.insn.mnemonic == "bgezall")) && p.insn.rs == 0);
    }

private:
    Cpu cpu_;
    ByteView text_;
    std::uint32_t base_;
};

// Do the bytes at `at` read as a function: instructions that all decode, whose jumps and branches stay in the program, that end in a jump or
// return (and its delay slot) within a few hundred words? Data does not: random words stop decoding, and zeros never end.
bool reads_as_function(const Cpu& cpu, ByteView text, std::uint32_t base, std::uint32_t at) {
    const std::uint32_t end = base + static_cast<std::uint32_t>(text.size());
    for (unsigned count = 0; count < kFunctionReach; ++count) {
        const std::uint32_t pc = at + 4 * count;
        if (pc < base || pc + 4 > end) return false;
        const std::optional<dm::Insn> insn = cpu.decode(text, pc - base, pc);
        if (!insn) return false;
        if (insn->target && (*insn->target < base || *insn->target >= end || *insn->target % 4 != 0)) {
            if (insn->mnemonic != "jal") return false;   // (a call may go anywhere: the BIOS, the console's code in ROM, a library)
        }
        if (cpu.r4300 && insn->mnemonic == "eret") return count >= 1;
        if (!ends_flow(*insn)) continue;
        if (pc + 8 > end || !cpu.decode(text, pc + 4 - base, pc + 4)) return false;
        return count >= 1;
    }
    return false;
}

// One use of what a `lui` loaded: the instruction that finishes the address.
struct Use {
    std::uint32_t lui;        // the `lui`'s address
    std::uint32_t at;         // the instruction that completes the address
    std::uint32_t address;    // the address they make
    bool ori;                 // by ORI: the high half is not adjusted for the low half's sign
};

// Every `lui` and what it is used for, as far as the code shows (the next few instructions, until the register is written or the flow leaves).
std::vector<Use> address_pairs(const Cpu& cpu, const std::map<std::uint32_t, MInsn>& code, const std::set<std::uint32_t>& starts) {
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
                if (q.mnemonic == "addiu" || (cpu.r4300 && q.mnemonic == "daddiu")) out.push_back({pc, at, high + static_cast<std::uint32_t>(q.imm), false});
                else if (q.mnemonic == "ori") out.push_back({pc, at, high | static_cast<std::uint32_t>(q.imm), true});
                else if (q.form == dm::Form::RtMem || (cpu.r4300 && q.form == dm::Form::FtMem)) out.push_back({pc, at, high + static_cast<std::uint32_t>(q.imm), false});
            }
            if (cpu.written(q) == reg || last) break;
            if (q.delay_slot) last = true;   // (the delay slot is the next instruction; then the flow has gone elsewhere)
        }
    }
    return out;
}

// How an address is named: part of the program's text, a hardware register, or a variable.
enum class Kind { None, Text, Hardware, Variable };

std::uint32_t physical(std::uint32_t a) { return a >= 0xC0000000u ? a : a & 0x1FFFFFFFu; }

using risc::byte_row;
using risc::hex8;

const std::string& text_value(const Values& values, const char* name) {
    static const std::string none;
    const auto it = values.find(name);
    return it != values.end() && std::holds_alternative<std::string>(it->second) ? std::get<std::string>(it->second) : none;
}

}  // namespace

image_writer::Written write(const Description& d, const Loaded& program, Values values, const std::filesystem::path& folder) {
    const Vectored& shape = d.vectored;
    const Cpu cpu{shape.cpu == "r4300", shape.big_endian};
    const dialect::Dialect& said = dialect::find(d.dialect);
    fs::make_dirs(folder);
    const std::string stem = text_value(values, "stem");
    const std::string file_name = text_value(values, "file");
    const Piece* text_piece = nullptr;
    for (const Piece& p : program.segments.pieces) {
        if (p.code) text_piece = &p;
    }
    if (!text_piece) throw UsageError(std::format("console platform {}: the loader gives no program piece", d.name));
    const ByteView file = program.image;
    const ByteView text = file.subspan(text_piece->offset, text_piece->size);
    const std::uint32_t base = text_piece->address;
    const std::uint32_t end = base + static_cast<std::uint32_t>(text.size());
    const MipsFlow flow(cpu, text, base);

    // function starts: what the entry point calls, and the places that begin as a function does after another one's end (a jump and its delay slot)
    std::set<std::uint32_t> entries(program.starts.begin(), program.starts.end());
    for (std::size_t off = 8; off + 4 <= text.size(); off += 4) {
        const std::uint32_t at = base + static_cast<std::uint32_t>(off);
        const std::optional<dm::Insn> before = cpu.decode(text, off - 8, at - 8);   // (a jump that ends a flow, then its delay slot, then this)
        if (before && ends_flow(*before) && (cpu.is_prologue(cpu.word_at(text, off)) || reads_as_function(cpu, text, base, at))) entries.insert(at);
    }
    for (const auto& [at, name] : program.entries) {
        if (flow.inside(at)) entries.insert(at);
    }

    // walk the code; what the code points at (an address built with `lui`, a word in a table) may be a function it never calls
    iw::Walked<MInsn> walked;
    std::vector<Use> uses;
    for (unsigned round = 0; round < kRounds; ++round) {
        const std::vector<std::uint32_t> from(entries.begin(), entries.end());
        walked = iw::walk<MInsn>(flow, from);
        const std::set<std::uint32_t> starts(walked.starts.begin(), walked.starts.end());
        uses = address_pairs(cpu, walked.code, starts);
        std::set<std::uint32_t> pointed;
        for (const Use& u : uses) pointed.insert(u.address);
        for (std::size_t off = 0; off + 4 <= text.size(); off += 4) {
            const std::uint32_t a = base + static_cast<std::uint32_t>(off);
            if (!walked.code.contains(a)) pointed.insert(cpu.word_at(text, off));
        }
        bool added = false;
        for (const std::uint32_t a : pointed) {
            if (a >= base && a < end && a % 4 == 0 && !walked.code.contains(a) && (cpu.is_prologue(cpu.word_at(text, a - base)) || reads_as_function(cpu, text, base, a)))
                added |= entries.insert(a).second;
        }
        if (!added) break;
    }
    const std::map<std::uint32_t, MInsn>& code = walked.code;
    std::vector<std::uint32_t> function_starts;   // (an entry point whose bytes are not an instruction is not a function)
    for (const std::uint32_t a : walked.starts)
        if (code.contains(a)) function_starts.push_back(a);
    const std::set<std::uint32_t> starts(function_starts.begin(), function_starts.end());
    const std::set<std::uint32_t> inside_insn = iw::covered(code, flow);
    const auto line_start = [&](std::uint32_t a) { return a >= base && a < end && !inside_insn.contains(a); };
    const iw::Names hardware = knowledge::standard().hardware_names(d.platform, shape.registers);
    // the hardware register at `address` (any of its views), and the name it is written with there
    const auto hardware_name = [&](std::uint32_t address) -> std::optional<std::string> {
        const auto it = hardware.find(physical(address));
        if (it == hardware.end()) return std::nullopt;
        const std::uint32_t region = address & 0xE0000000u;
        return it->second + (address >= 0xC0000000u || region == shape.bare_region ? "" : region == 0x80000000u ? "_K0" : shape.other_suffix);
    };
    const auto classify = [&](std::uint32_t a) {
        if (a >= base && a < end) return line_start(a) ? Kind::Text : Kind::None;
        if (hardware_name(a)) return Kind::Hardware;
        const std::uint32_t p = physical(a);
        if ((a & 0xDFFFFFFFu) >= 0x80000000u && p < shape.ram_size) return Kind::Variable;   // RAM, past or before the program
        for (const Range& r : shape.variables) {
            if (p >= r.from && p < r.to) return Kind::Variable;
        }
        return Kind::None;
    };

    // names: the entry point is `entry`, a function `sub_<address>`
    iw::Names names;
    for (const auto& [at, name] : program.entries) {
        if (starts.contains(at)) names[at] = name;
    }
    const iw::Functions functions = iw::name_functions(function_starts, names, kSub);
    iw::Names labels = iw::make_labels(walked.targets, names, line_start, kLocal);

    // what the code and the tables point at
    std::map<std::string, std::uint32_t> equates;               // the names an address outside the labels is written with
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
        const std::uint32_t v = cpu.word_at(text, off);
        if (v >= base && v < end && line_start(v) && name_of(v)) pointer_words.insert(a);
    }
    const auto word_name = [&](std::uint32_t v) {
        const auto it = labels.find(v);
        return it != labels.end() ? it->second : hex8(v);
    };

    // the source ----------------------------------------------------------------------------------------------------------------
    values.insert_or_assign("functions", static_cast<std::int64_t>(functions.size()));
    values.insert_or_assign("base", static_cast<std::int64_t>(base));
    values.insert_or_assign("text_size", static_cast<std::int64_t>(text.size()));
    translate::l2::Assembly out(head_comment(d, values), shape.assembler.empty() ? d.dialect : shape.assembler, file_name);
    out.extend(shape.prologue);
    for (const auto& [name, value] : equates) out.append(std::format("        .equ {}, {}", name, hex8(value)));
    if (!equates.empty()) out.append("");

    decompile::AddressLabels names_for_text;
    for (const auto& [a, label] : labels) names_for_text[a] = label;
    for (const Piece& piece : program.segments.pieces) {
        out.part(piece.segment, piece.role.empty() ? "program" : piece.role, std::nullopt, piece.code ? std::nullopt : std::optional<std::uint64_t>(piece.size));
        if (piece.org) {
            out.append("");
            out.append(std::format("        .org {}", hex8(piece.address)));
        }
        if (piece.role == "header") {   // the header, field by field
            std::vector<std::string> lines;
            risc::header_fields(lines, shape, file, values, word_name);
            out.extend(lines);
        } else if (piece.code) {
            std::set<std::uint32_t> boundaries = {static_cast<std::uint32_t>(text.size())};
            for (const auto& entry : code) boundaries.insert(entry.first - base);
            for (const auto& entry : labels)
                if (line_start(entry.first)) boundaries.insert(entry.first - base);
            const auto text_of = [&](const MInsn& p) {   // the description's text, a branch or call by its label, an address half by its expression
                const auto it = immediate.find(p.insn.pc);
                std::map<std::uint32_t, std::string> by_value;
                std::vector<std::uint32_t> operands;
                if (p.insn.target)
                    if (const auto named = names_for_text.find(*p.insn.target); named != names_for_text.end()) by_value[*p.insn.target] = named->second, operands.push_back(*p.insn.target);
                return said.fallback(risc_source::code(shape.cpu, text.subspan(p.insn.pc - base, 4), p.insn.pc, by_value, operands, it == immediate.end() ? std::string() : it->second));
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
                const risc::DataItem item = risc::data_item(text, off, limit, addr, cpu.big, [&](std::uint32_t a) -> std::optional<std::string> {
                    if (!pointer_words.contains(a)) return std::nullopt;
                    return word_name(cpu.word_at(text, a - base));
                });
                out.append(item.line);
                off += item.took;
            }
        } else {   // data the program reads (a cartridge's past the first megabyte), each row at its own address
            std::vector<std::string> lines;
            risc::items(lines, file, piece.offset, piece.offset + piece.size, piece.address, cpu.big);
            out.extend(lines);
        }
    }

    std::string source;   // (said in GNU as's: `$sp`, decimal offsets, `#` comments; line by line, or the whole text at the end)
    if (shape.whole) {
        source = said.print(iw::text(out) + "\n");
    } else {
        for (const std::string& l : out.lines()) source += said.line(l) + "\n";
    }
    iw::Written r = iw::finish(folder, stem + "." + shape.source, source, functions, iw::code_bytes(code, flow), program.segments.measured);
    iw::assembled(
        r,
        [&] {
            Bytes image = risc_source::assemble(shape.cpu, said.parse(source), folder);
            return program.segments.rebuilt ? program.segments.rebuilt(image) : image;
        },
        program.segments.original.empty() ? program.image : program.segments.original, folder / fill(shape.built, values), shape.built_what);
    return r;
}

}  // namespace cb::source::console_writer::mips_code
