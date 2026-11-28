// cb-unity: alone (file-local names that other files in a batch also use)
// source/console_arm.cpp -- the console writer's `vectored` shape for an ARM7TDMI program (console_writer.hpp): the Game Boy Advance, written as
// GNU as's source (`.syntax unified`): every function in the order it lies in the ROM, with the state it runs in (`.arm` / `.thumb`) said before
// its instructions, and the literal pools that follow a function's code as `.word` lines under a label of their own.
//
// Code is what the program's own flow reaches from the entry branch and from the functions it calls -- the processor's state is part of an
// address (its lowest bit), a branch stays in its state, and the state changes only through `bx`, so a `bx r0` is followed to where it goes when
// the register was just loaded from a literal pool or made from the pc (`ldr r0, =func+1`; `add r0, pc, #1`; `mov lr, pc` before it makes the
// call come back). A function only called through a pointer is found where one begins after another's end (after a return and its literal
// pool, when the bytes read as a function), or where a pointer to it is -- a word of a literal pool, or in a table -- and its bytes read as a
// function. Every other byte is data: strings, words (a word holding an address in the ROM is its label), runs of one value, bytes.
//
// An instruction is written as text only when the text assembles back to its own bytes (the assembler's round trip); the few encodings the
// assembler would write differently stay as `.hword`/`.word` with the instruction as a comment, so the source always builds the ROM it came
// from. Functions are `arm_<address>` and `thumb_<address>` (`entry` for the header's branch), jump targets `L_<address>`, data `D_<address>`;
// the hardware's registers have their names, other addresses outside the ROM are `var_<address>` equates. The header is written field by
// field, as the description lists them.

#include "source/console_risc.hpp"
#include "source/console_shapes.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/arm.hpp"
#include "knowledge/package.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace cb::source::console_writer::arm_code {

namespace da = decompile::arm;
namespace iw = image_writer;

namespace {

constexpr iw::Spelling kLocal{"L_", 8};
constexpr iw::Spelling kData{"D_", 8};
constexpr unsigned kRounds = 6;              // how many times what the code points at may add functions
constexpr unsigned kFunctionReach = 200;     // how many instructions a function is looked at for before it is called data
constexpr std::uint32_t kPoolReach = 160;    // how far past a function's end the next one is looked for (its literal pool lies between)
constexpr unsigned kLookBack = 12;           // instructions before a `bx` that may have loaded its register
constexpr unsigned kTableMax = 256;          // entries of an `add pc, pc, rX, lsl #2` branch table

// An instruction, and where a `bx` through a register goes when the code before it says (an address with the state in its lowest bit).
struct GInsn {
    da::Insn insn;
    std::optional<std::uint32_t> resolved;
    bool call = false;   // `mov lr, pc` then `bx`: a call that comes back
};

bool ends_flow(const da::Insn& insn) { return insn.flow == da::Flow::Jump || insn.flow == da::Flow::Return || insn.flow == da::Flow::Indirect; }

std::uint32_t word_at(ByteView b, std::size_t off) { return risc::word_at(b, off, false); }

std::uint32_t tagged(std::uint32_t real, bool thumb) { return real | (thumb ? 1u : 0u); }

// Does the instruction write the register?
bool writes(const da::Insn& i, unsigned reg) {
    switch (i.op) {
        case da::Op::Tst: case da::Op::Teq: case da::Op::Cmp: case da::Op::Cmn: return false;
        case da::Op::Mul: case da::Op::Mla: case da::Op::Swp: case da::Op::Mrs: return i.rd == reg;
        case da::Op::Umull: case da::Op::Umlal: case da::Op::Smull: case da::Op::Smlal: return i.rd == reg || i.rn == reg;
        case da::Op::Ldr: return i.rd == reg || (i.wb && i.rn == reg);
        case da::Op::Str: return i.wb && i.rn == reg;
        case da::Op::Ldm: return ((i.list >> reg) & 1) || (i.wb && i.rn == reg);
        case da::Op::Stm: return i.wb && i.rn == reg;
        case da::Op::Bl: return reg == 14;
        case da::Op::B: case da::Op::Bx: case da::Op::Swi: case da::Op::Msr: return false;
        default: return i.rd == reg;
    }
}

// The address a PC-relative word load reads.
std::optional<std::uint32_t> literal_address(const da::Insn& i) {
    if (i.op != da::Op::Ldr || i.rn != 15 || !i.imm_form || !i.pre || i.wb || i.user || i.size != 4 || i.sign) return std::nullopt;
    const std::uint32_t base = i.thumb() ? ((i.pc + 4) & ~3u) : i.pc + 8;
    return i.up ? base + i.imm : base - i.imm;
}

// Flow runs through the ROM from the entry points, in the state each address says.
class GbaFlow final : public iw::CodeModel<GInsn> {
public:
    GbaFlow(ByteView rom, std::uint32_t base) : rom_(rom), base_(base) {}

    std::optional<da::Insn> plain(std::uint32_t key) const {
        const std::uint32_t real = key & ~1u;
        const unsigned unit = (key & 1) ? 2 : 4;
        if (real < base_ || real % unit != 0 || real - base_ + unit > rom_.size()) return std::nullopt;
        return da::decode(rom_, real - base_, key);
    }
    std::optional<GInsn> decode(std::uint32_t key) const override {
        const std::optional<da::Insn> insn = plain(key);
        if (!insn) return std::nullopt;
        GInsn g{*insn, std::nullopt, false};
        if (insn->op == da::Op::Bx && insn->rm != 14) {
            g.resolved = resolve(*insn);
            if (!insn->thumb() && insn->pc >= base_ + 4) {
                const std::optional<da::Insn> before = plain(insn->pc - 4);
                g.call = before && before->op == da::Op::Mov && before->rd == 14 && !before->imm_form && before->rm == 15 && before->cond == insn->cond &&
                         !before->shift_by_reg && before->shift == da::Shift::Lsl && before->amount == 0 && !before->rrx;
            }
        }
        return g;
    }
    std::uint32_t size(const GInsn& g) const override { return g.insn.length; }
    std::optional<std::uint32_t> target(const GInsn& g) const override { return g.resolved ? g.resolved : g.insn.target; }
    bool inside(std::uint32_t key) const override {
        const std::uint32_t real = key & ~1u;
        return real >= base_ && real % ((key & 1) ? 2 : 4) == 0 && real - base_ + ((key & 1) ? 2 : 4) <= rom_.size();
    }
    bool fits(std::uint32_t key, const GInsn& g) const override { return (key & ~1u) - base_ + g.insn.length <= rom_.size(); }
    bool follows(const GInsn& g) const override {
        const std::optional<std::uint32_t> to = target(g);
        return to && inside(*to) && (g.insn.flow != da::Flow::None || g.resolved);
    }
    bool stops(const GInsn& g) const override { return ends_flow(g.insn) && !g.call; }
    bool finds_functions() const override { return true; }
    bool starts_function(const GInsn& g) const override { return g.insn.op == da::Op::Bl || g.resolved.has_value(); }

private:
    // Where `bx rm` goes, from the instruction that loaded the register: a literal-pool word, or the pc plus or or'ed with a constant.
    std::optional<std::uint32_t> resolve(const da::Insn& bx) const {
        unsigned reg = bx.rm;
        const bool thumb = bx.thumb();
        const unsigned unit = thumb ? 2 : 4;
        std::uint32_t at = bx.pc;
        for (unsigned k = 0; k < kLookBack; ++k) {
            if (at < base_ + unit) return std::nullopt;
            at -= unit;
            const std::optional<da::Insn> p = plain(tagged(at, thumb));
            if (!p || p->flow != da::Flow::None) return std::nullopt;
            if (!writes(*p, reg)) continue;
            if (p->cond != 14) return std::nullopt;
            std::optional<std::uint32_t> value;
            if (const auto a = literal_address(*p)) {
                if (*a >= base_ && *a + 4 <= base_ + rom_.size()) value = word_at(rom_, *a - base_);
            } else if ((p->op == da::Op::Add || p->op == da::Op::Orr || p->op == da::Op::Sub) && p->rn == 15 && p->imm_form) {
                const std::uint32_t pc = p->thumb() ? ((p->pc + 4) & ~3u) : p->pc + 8;
                value = p->op == da::Op::Add ? pc + p->imm : p->op == da::Op::Orr ? (pc | p->imm) : pc - p->imm;
            } else if (p->op == da::Op::Mov && p->imm_form) {
                value = p->imm;
            } else if (p->op == da::Op::Mov && !p->imm_form && !p->shift_by_reg && p->shift == da::Shift::Lsl && p->amount == 0 && !p->rrx && p->rm != 15) {
                reg = p->rm;   // (the register was copied: follow the copy)
                continue;
            }
            if (!value) return std::nullopt;
            return (*value & 1) ? *value : (*value & ~3u);
        }
        return std::nullopt;
    }

    ByteView rom_;
    std::uint32_t base_;
};

// `stmfd sp!, {..., lr}` and `push {..., lr}`: how a function that calls others opens.
bool is_prologue(ByteView rom, std::uint32_t base, std::uint32_t key) {
    const std::uint32_t real = key & ~1u;
    if (real < base || real - base + ((key & 1) ? 2 : 4) > rom.size()) return false;
    const std::size_t off = real - base;
    if (key & 1) return (static_cast<unsigned>(rom[off] | rom[off + 1] << 8) & 0xFF00) == 0xB500;
    const std::uint32_t w = word_at(rom, off);
    return (w >> 16) == 0xE92D && (w & 0x4000) != 0;
}

// Do the bytes at `key` read as a function: instructions that all decode, whose branches stay in the ROM, that end in a return (an
// unconditional jump, `bx`, `pop {pc}`, `mov pc`) within a few hundred instructions? Data does not: words that are not instructions stop
// decoding, and zeros never end.
bool reads_as_function(ByteView rom, std::uint32_t base, std::uint32_t key) {
    const bool thumb = key & 1;
    const std::uint32_t end = base + static_cast<std::uint32_t>(rom.size());
    std::uint32_t pc = key & ~1u;
    for (unsigned count = 0; count < kFunctionReach; ++count) {
        if (pc < base || pc >= end || pc % (thumb ? 2 : 4) != 0) return false;
        const std::optional<da::Insn> insn = da::decode(rom, pc - base, tagged(pc, thumb));
        if (!insn) return false;
        if (insn->target) {
            const std::uint32_t t = *insn->target & ~1u;
            if (t < base || t >= end) return false;
        }
        if (ends_flow(*insn)) return count >= 1;
        pc += insn->length;
    }
    return false;
}

// How an address is named: part of the ROM, a hardware register, or a variable in RAM.
enum class Kind { None, Text, Hardware, Variable };

using risc::hex8;

}  // namespace

image_writer::Written write(const Description& d, const Loaded& program, Values values, const std::filesystem::path& folder) {
    const Vectored& shape = d.vectored;
    fs::make_dirs(folder);
    const std::string stem = std::get<std::string>(values.at("stem"));
    const std::string file_name = std::get<std::string>(values.at("file"));
    const ByteView image = program.image;
    if (program.segments.pieces.empty()) throw UsageError(std::format("console platform {}: the loader gives no program piece", d.name));
    const std::uint32_t base = program.segments.pieces.front().address;
    // the header: from the first field to the end of the last (written as its fields when no code or label lies in it)
    std::uint32_t header_from = static_cast<std::uint32_t>(image.size()), header_size = 0;
    for (const VectorField& f : shape.fields) {
        header_from = std::min(header_from, f.at);
        header_size = std::max(header_size, f.at + f.size);
    }
    const iw::Names hardware = knowledge::standard().hardware_names(d.platform, shape.registers);
    const std::uint32_t end = base + static_cast<std::uint32_t>(image.size());
    const GbaFlow flow(image, base);

    // function starts: what the entry point calls, and what the code points at or leaves behind
    std::set<std::uint32_t> entries;
    for (const std::uint32_t a : program.starts) entries.insert(a);
    entries.insert(base);

    iw::Walked<GInsn> walked;
    std::vector<bool> covered;   // the ROM's bytes that an instruction takes
    for (unsigned round = 0; round < kRounds; ++round) {
        const std::vector<std::uint32_t> from(entries.begin(), entries.end());
        walked = iw::walk<GInsn>(flow, from);
        covered.assign(image.size(), false);
        for (const auto& [key, g] : walked.code)
            for (unsigned k = 0; k < g.insn.length; ++k) covered[(key & ~1u) - base + k] = true;
        bool added = false;
        const auto candidate = [&](std::uint32_t key, bool prologue_needed) {
            const std::uint32_t real = key & ~1u;
            if (real < base + header_size || real >= end || real % ((key & 1) ? 2 : 4) != 0 || covered[real - base] || entries.contains(key)) return;   // (the header is not code)
            if ((!prologue_needed || is_prologue(image, base, key)) && reads_as_function(image, base, key)) added |= entries.insert(key).second;
        };
        // what follows a function's end: its literal pool, then the next function
        for (const auto& [key, g] : walked.code) {
            if (!flow.stops(g)) continue;
            const std::uint32_t after = (key & ~1u) + g.insn.length;
            for (std::uint32_t p = after; p < end && p - after <= kPoolReach && !covered[p - base]; p += 2) {
                if (is_prologue(image, base, p | 1)) candidate(p | 1, true);
                if (p % 4 == 0 && is_prologue(image, base, p)) candidate(p, true);
            }
        }
        // what a literal pool word or a table word points at
        const auto pointer = [&](std::uint32_t v, bool prologue_needed) {
            if (v < base || v >= end) return;
            if (v & 1) candidate(v, prologue_needed);
            else if (v % 4 == 0) candidate(v, prologue_needed);
        };
        for (const auto& [key, g] : walked.code) {
            if (const auto a = literal_address(g.insn); a && *a >= base && *a + 4 <= end) pointer(word_at(image, *a - base), false);
            // an ARM branch table: `add pc, pc, rX, lsl #2`, a padding word, and a `b` for each case
            if (g.insn.op == da::Op::Add && g.insn.rd == 15 && g.insn.rn == 15 && !g.insn.imm_form && g.insn.shift == da::Shift::Lsl && g.insn.amount == 2 && !g.insn.thumb() &&
                g.insn.cond == 14) {
                for (unsigned n = 0; n < kTableMax; ++n) {
                    const std::uint32_t at = g.insn.pc + 8 + 4 * n;
                    const std::optional<da::Insn> entry = flow.plain(at);
                    if (!entry || entry->op != da::Op::B || entry->cond != 14 || !entry->target || *entry->target < base || *entry->target >= end) break;
                    if (!covered[at - base]) added |= entries.insert(at).second;
                }
            }
        }
        for (std::size_t off = 0; off + 4 <= image.size(); off += 4) {
            if (covered[off] || covered[off + 1] || covered[off + 2] || covered[off + 3]) continue;
            pointer(word_at(image, off), true);
        }
        if (!added) break;
    }
    const std::map<std::uint32_t, GInsn>& code = walked.code;
    std::vector<std::uint32_t> function_starts;   // (an entry point whose bytes are not an instruction is not a function)
    for (const std::uint32_t a : walked.starts)
        if (code.contains(a)) function_starts.push_back(a);

    const auto is_code = [&](std::uint32_t real) { return code.contains(real) || code.contains(real | 1); };
    const auto line_start = [&](std::uint32_t a) { return a >= base && a < end && (!covered[a - base] || is_code(a)); };
    const auto physical_kind = [&](std::uint32_t a) {
        const auto within = [&](std::uint32_t lo, std::uint32_t size) { return a >= lo && a - lo < size; };
        if (hardware.contains(a)) return Kind::Hardware;
        for (const Range& r : shape.variables) {
            if (within(r.from, r.to - r.from)) return Kind::Variable;
        }
        return Kind::None;
    };

    // names: the entry point is `entry`, a function `arm_<address>` or `thumb_<address>`
    iw::Names names;
    iw::Functions functions;
    for (const std::uint32_t key : function_starts) {
        const std::uint32_t real = key & ~1u;
        const std::string name = key == base ? "entry" : std::string((key & 1) ? "thumb_" : "arm_") + std::format("{:08X}", real);
        functions[key] = name;
        names.emplace(real, name);
    }
    std::map<std::uint32_t, std::string> labels(names.begin(), names.end());
    for (const std::uint32_t t : walked.targets)
        if (const std::uint32_t real = t & ~1u; line_start(real) && !labels.contains(real)) labels[real] = kLocal(real);

    // what the code and the tables point at
    std::map<std::string, std::uint32_t> equates;
    std::set<std::uint32_t> pool_words;   // the words PC-relative loads read
    for (const auto& [key, g] : code)
        if (const auto a = literal_address(g.insn); a && *a >= base && *a + 4 <= end && *a % 4 == 0 && !covered[*a - base] && !covered[*a - base + 1] && !covered[*a - base + 2] &&
                                                   !covered[*a - base + 3]) {
            pool_words.insert(*a);
            if (!labels.contains(*a)) labels[*a] = kData(*a);
        }
    // how the value a word holds is written: a label in the ROM (with `+ 1` for a Thumb address), a register's name, a variable's
    const auto value_text = [&](std::uint32_t v) -> std::optional<std::string> {
        if (v >= base && v < end) {
            const std::uint32_t real = v & ~1u;
            if ((v & 1) && code.contains(v)) {
                if (!labels.contains(real)) labels[real] = kLocal(real);
                return labels[real] + " + 1";
            }
            if (!(v & 1) && code.contains(v) && !labels.contains(v)) labels[v] = kLocal(v);
            if (line_start(v) || labels.contains(v)) {
                if (!labels.contains(v)) labels[v] = kData(v);
                return labels[v];
            }
            return std::nullopt;
        }
        switch (physical_kind(v)) {
            case Kind::Hardware: {
                const std::string n = hardware.at(v);
                equates[n] = v;
                return n;
            }
            case Kind::Variable: {
                const std::string n = std::format("var_{:08X}", v);
                equates[n] = v;
                return n;
            }
            default: return std::nullopt;
        }
    };
    std::map<std::uint32_t, std::string> word_names;   // by the word's address: how its value is written
    for (const std::uint32_t a : pool_words)
        if (const auto t = value_text(word_at(image, a - base))) word_names[a] = *t;
    for (std::size_t off = 0; off + 4 <= image.size(); off += 4) {   // words in the data that hold an address in the ROM (a jump table, a table of pointers)
        const std::uint32_t a = base + static_cast<std::uint32_t>(off);
        if (covered[off] || covered[off + 1] || covered[off + 2] || covered[off + 3] || pool_words.contains(a)) continue;
        const std::uint32_t v = word_at(image, off);
        if (v >= base && v < end)
            if (const auto t = value_text(v)) word_names[a] = *t;
    }

    // the text of each instruction: the description's, with the branch's label put in, kept only when it assembles back to the instruction's bytes
    std::set<std::uint32_t> as_data;   // the instructions whose text would not assemble to their bytes
    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [key, g] : code) {
        const char* cpu = g.insn.thumb() ? "thumb" : "arm";
        const std::optional<std::string> said = risc_source::described(cpu, g.insn.bytes(), g.insn.pc);
        std::string t;
        if (said) {
            std::map<std::uint32_t, std::string> by_value;
            std::vector<std::uint32_t> operands;
            if (g.insn.target) {
                const std::uint32_t real = *g.insn.target & ~1u;
                if (const auto named = labels.find(real); named != labels.end()) by_value[real] = named->second, operands.push_back(real);
            }
            if (const auto word = literal_address(g.insn))   // (a PC-relative load names the word it reads)
                if (const auto named = labels.find(*word); named != labels.end()) by_value[*word] = named->second, operands.push_back(*word);
            t = risc_source::code(cpu, g.insn.bytes(), g.insn.pc, by_value, operands);
        } else {
            as_data.insert(key);
            t = risc_source::read_text(cpu, g.insn.bytes(), g.insn.pc).value_or("?");
        }
        instruction_text[key] = std::move(t);
    }

    // an instruction with a label inside it (a branch from other code lands in the middle of it) is written as its bits, so the label has a place
    for (const auto& [a, n] : labels)
        for (std::uint32_t back = 1; back < 4 && a - base >= back; ++back) {
            const std::uint32_t r = a - back;
            for (const std::uint32_t k : {r, r | 1}) {
                const auto it = code.find(k);
                if (it != code.end() && (k & ~1u) == r && r + it->second.insn.length > a) as_data.insert(k);
            }
        }

    // the source ----------------------------------------------------------------------------------------------------------------
    values.insert_or_assign("functions", static_cast<std::int64_t>(functions.size()));
    values.insert_or_assign("base", static_cast<std::int64_t>(base));
    const dialect::Dialect& said = dialect::find(d.dialect);
    translate::l2::Assembly out(head_comment(d, values), shape.assembler.empty() ? d.dialect : shape.assembler, file_name);
    if (!shape.block.empty()) out.extend(said.block(shape.block, {{"cpu", shape.block_cpu}}));
    for (const auto& [name, value] : equates) out.append(std::format("        .equ {}, {}", name, hex8(value)));
    if (!equates.empty()) out.append("");
    out.part(program.segments.pieces.front().segment, "cartridge", std::nullopt);
    out.append(std::format("        .org {}", hex8(base)));

    std::set<std::uint32_t> boundaries = {static_cast<std::uint32_t>(image.size())};
    for (const auto& entry : code) boundaries.insert((entry.first & ~1u) - base);
    for (const auto& entry : labels)
        if (line_start(entry.first)) boundaries.insert(entry.first - base);

    const bool header_free = [&] {
        for (std::uint32_t k = header_from; k < header_size; ++k)
            if (covered[k] || labels.contains(base + k)) return false;
        return true;
    }();
    int mode = -1;   // the state the lines so far are in: 0 ARM, 1 Thumb, -1 not said yet
    std::size_t off = 0;
    while (off < image.size()) {
        const std::uint32_t addr = base + static_cast<std::uint32_t>(off);
        if (off == header_from && header_free && header_size > header_from) {
            std::vector<std::string> lines;
            risc::header_fields(lines, shape, image, values, [](std::uint32_t v) { return risc::hex8(v); });
            out.extend(lines);
            off = header_size;
            continue;
        }
        const auto label = labels.find(addr);
        const bool arm_code = code.contains(addr), thumb_code = code.contains(addr | 1);
        const std::uint32_t key = arm_code ? addr : addr | 1;
        if (label != labels.end() && line_start(addr)) {
            for (const std::uint32_t fk : {addr, addr | 1}) {
                if (const auto fn = functions.find(fk); fn != functions.end() && code.contains(fk)) {
                    out.part(fn->second, "function", translate::ir::OriginVa{out.module().value_or(""), addr});   // each function a fragment
                    out.append("");
                    mode = -1;
                    break;
                }
            }
            out.append(label->second + ":");
        }
        if (arm_code || thumb_code) {
            const da::Insn& insn = code.at(key).insn;
            const int wanted = insn.thumb() ? 1 : 0;
            if (as_data.contains(key)) {
                // the bits, in pieces small enough to put a label between (an instruction another path of the code overlaps)
                const std::string comment = "  ; " + said.instruction(instruction_text.at(key));
                unsigned step = insn.length;
                for (unsigned k = 1; k < insn.length; ++k)
                    if (labels.contains(addr + k)) step = std::min(step, k % 2 ? 1u : 2u);
                for (unsigned k = 0; k < insn.length; k += step) {
                    if (k != 0)
                        if (const auto inner = labels.find(addr + k); inner != labels.end()) out.append(inner->second + ":");
                    const auto raw16 = [&](unsigned at) { return static_cast<unsigned>(insn.raw[at] | insn.raw[at + 1] << 8); };
                    std::string line;
                    if (step == 1) line = std::format("        .byte 0x{:02X}", insn.raw[k]);
                    else if (step == 2) line = std::format("        .half 0x{:04X}", raw16(k));
                    else if (insn.length == 2) line = std::format("        .half 0x{:04X}", raw16(0));
                    else if (insn.thumb()) line = std::format("        .half 0x{:04X}, 0x{:04X}", raw16(0), raw16(2));
                    else line = std::format("        .word 0x{:08X}", insn.word);
                    out.append(k == 0 ? line + comment : line);
                }
            } else {
                if (mode != wanted) {
                    out.append(wanted ? "        .context TMode=1" : "        .context TMode=0");
                    mode = wanted;
                }
                out.append("        " + instruction_text.at(key));
            }
            off += insn.length;
            continue;
        }
        const std::size_t limit = *boundaries.upper_bound(static_cast<std::uint32_t>(off));
        const risc::DataItem item = risc::data_item(image, off, limit, addr, false, [&](std::uint32_t a) -> std::optional<std::string> {
            const auto it = word_names.find(a);
            if (it == word_names.end()) return std::nullopt;
            return it->second;
        });
        out.append(item.line);
        off += item.took;
    }

    const std::string source = said.print(iw::text(out) + "\n");   // (said in GNU as's: config/dialects/gas-arm.json)
    iw::Written r = iw::finish(folder, stem + "." + shape.source, source, functions, iw::code_bytes(code, flow), program.segments.measured);
    iw::assembled(r, [&] { return risc_source::assemble(shape.cpu, said.parse(source), folder); }, image, folder / fill(shape.built, values), shape.built_what);
    return r;
}

}  // namespace cb::source::console_writer::arm_code
