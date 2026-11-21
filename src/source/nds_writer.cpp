// cb-unity: alone (file-local names that other files in a batch also use)
// source/nds_writer.cpp -- a Nintendo DS cartridge as assembly source that builds back into it (nds_writer.hpp).

#include "source/nds_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <functional>
#include <set>
#include <unordered_map>
#include <vector>


#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/arm.hpp"
#include "source/isa.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace cb::source::nds_writer {

namespace da = decompile::arm;
namespace iw = image_writer;
namespace nds = formats::nds;
using Cpu = nds::Cpu;

namespace {

constexpr unsigned kStringMin = 4;           // a run of printable bytes this long is a string
constexpr unsigned kStringMax = 96;
constexpr std::size_t kFillMin = 16;         // a run of one value this long is `.space` / `.fill`
constexpr unsigned kRounds = 6;              // how many times what the code points at may add functions
constexpr unsigned kFunctionReach = 200;     // how many instructions a function is looked at for before it is called data
constexpr std::uint32_t kPoolReach = 160;    // how far past a function's end the next one is looked for (its literal pool lies between)
constexpr unsigned kLookBack = 12;           // instructions before a `bx` or `blx` that may have loaded its register
constexpr unsigned kTableMax = 256;          // entries of an `add pc, pc, rX, lsl #2` branch table

// An instruction, and where a `bx` or `blx` through a register goes when the code before it says (an address with the state in its lowest bit).
struct GInsn {
    da::Insn insn;
    std::optional<std::uint32_t> resolved;
    bool call = false;   // `mov lr, pc` then `bx`: a call that comes back
};

bool ends_flow(const da::Insn& insn) { return insn.flow == da::Flow::Jump || insn.flow == da::Flow::Return || insn.flow == da::Flow::Indirect; }

std::uint32_t word_at(ByteView b, std::size_t off) {
    return static_cast<std::uint32_t>(b[off]) | static_cast<std::uint32_t>(b[off + 1]) << 8 | static_cast<std::uint32_t>(b[off + 2]) << 16 |
           static_cast<std::uint32_t>(b[off + 3]) << 24;
}

std::uint32_t tagged(std::uint32_t real, bool thumb) { return real | (thumb ? 1u : 0u); }

// Does the instruction write the register?
bool writes(const da::Insn& i, unsigned reg) {
    switch (i.op) {
        case da::Op::Tst: case da::Op::Teq: case da::Op::Cmp: case da::Op::Cmn: case da::Op::Pld: case da::Op::Mcr: case da::Op::Bkpt: return false;
        case da::Op::Mul: case da::Op::Mla: case da::Op::Swp: case da::Op::Mrs: case da::Op::Clz: case da::Op::Mrc: case da::Op::Smla: case da::Op::Smul:
        case da::Op::Smlaw: case da::Op::Smulw: case da::Op::Qadd: case da::Op::Qsub: case da::Op::Qdadd: case da::Op::Qdsub: return i.rd == reg;
        case da::Op::Umull: case da::Op::Umlal: case da::Op::Smull: case da::Op::Smlal: case da::Op::Smlalxy: return i.rd == reg || i.rn == reg;
        case da::Op::Ldr: return i.rd == reg || (i.size == 8 && i.rd + 1u == reg) || (i.wb && i.rn == reg);
        case da::Op::Str: return i.wb && i.rn == reg;
        case da::Op::Ldm: return ((i.list >> reg) & 1) || (i.wb && i.rn == reg);
        case da::Op::Stm: return i.wb && i.rn == reg;
        case da::Op::Bl: case da::Op::Blx: return reg == 14;
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

// Flow runs through one program from its entry points, in the state each address says; the program is `image`, loaded at `base`.
class ArmFlow final : public iw::CodeModel<GInsn> {
public:
    ArmFlow(ByteView image, std::uint32_t base, da::Arch arch) : rom_(image), base_(base), arch_(arch) {}

    std::optional<da::Insn> plain(std::uint32_t key) const {
        const std::uint32_t real = key & ~1u;
        const unsigned unit = (key & 1) ? 2 : 4;
        if (real < base_ || real % unit != 0 || real - base_ + unit > rom_.size()) return std::nullopt;
        return da::decode(rom_, real - base_, key, arch_);
    }
    std::optional<GInsn> decode(std::uint32_t key) const override {
        const std::optional<da::Insn> insn = plain(key);
        if (!insn) return std::nullopt;
        GInsn g{*insn, std::nullopt, false};
        if ((insn->op == da::Op::Bx && insn->rm != 14) || (insn->op == da::Op::Blx && !insn->target)) {
            g.resolved = resolve(*insn);
            if (insn->op == da::Op::Bx && !insn->thumb() && insn->pc >= base_ + 4) {
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
    bool starts_function(const GInsn& g) const override { return g.insn.op == da::Op::Bl || g.insn.op == da::Op::Blx || g.resolved.has_value(); }

private:
    // Where `bx rm` (or `blx rm`) goes, from the instruction that loaded the register: a literal-pool word, or the pc plus or or'ed with a constant.
    std::optional<std::uint32_t> resolve(const da::Insn& bx) const {
        unsigned reg = bx.rm;
        const bool thumb = bx.thumb();
        const unsigned unit = thumb ? 2 : 4;
        std::uint32_t at = bx.pc;
        for (unsigned k = 0; k < kLookBack; ++k) {
            if (at < base_ + unit) return std::nullopt;
            at -= unit;
            const std::optional<da::Insn> p = plain(tagged(at, thumb));
            if (!p || (p->flow != da::Flow::None && p->flow != da::Flow::Call)) return std::nullopt;
            if (!writes(*p, reg)) continue;
            if (p->flow == da::Flow::Call || p->cond != 14) return std::nullopt;
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
    da::Arch arch_;
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

// Do the bytes at `key` read as a function: instructions that all decode, whose branches stay in the program, that end in a return (an
// unconditional jump, `bx`, `pop {pc}`, `mov pc`) within a few hundred instructions? Data does not: words that are not instructions stop
// decoding, and zeros never end.
bool reads_as_function(ByteView rom, std::uint32_t base, da::Arch arch, std::uint32_t key) {
    const bool thumb = key & 1;
    const std::uint32_t end = base + static_cast<std::uint32_t>(rom.size());
    std::uint32_t pc = key & ~1u;
    for (unsigned count = 0; count < kFunctionReach; ++count) {
        if (pc < base || pc >= end || pc % (thumb ? 2 : 4) != 0) return false;
        const std::optional<da::Insn> insn = da::decode(rom, pc - base, tagged(pc, thumb), arch);
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
enum class Kind { None, Hardware, Variable };

std::string hex8(std::uint32_t v) { return std::format("0x{:08X}", v); }

bool printable(std::uint8_t b) { return b >= 32 && b < 127 && b != '"' && b != '\\'; }

std::string byte_row(ByteView bytes) {
    std::vector<std::string> items;
    for (const std::uint8_t b : bytes) items.push_back(std::format("0x{:02X}", b));
    return "        .byte " + text::join(items, ", ");
}

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
// `named` names is written as the name), or a few bytes. How many bytes it took, and the line.
struct DataItem {
    std::string line;
    std::size_t took = 0;
};
DataItem data_item(ByteView buf, std::size_t off, std::size_t limit, std::uint32_t addr,
                   const std::function<std::optional<std::string>(std::uint32_t)>& named) {
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
            const std::optional<std::string> name = named(addr + static_cast<std::uint32_t>(took));
            if (took != 0 && (printable_run(buf, at, limit) >= kStringMin || same_run(buf, at, limit) >= kFillMin || string_inside_word(buf, at, limit) != 0))
                break;
            items.push_back(name ? *name : hex8(word_at(buf, at)));
            took += 4;
        }
        return {"        .word " + text::join(items, ", "), took};
    }
    const std::size_t room = addr % 4 != 0 ? 4 - addr % 4 : limit - off;
    const std::size_t count = std::min<std::size_t>({room, limit - off, 16});
    return {byte_row(buf.subspan(off, count)), count};
}

// The bytes [from, to) of a stretch that holds no code, as data lines (`base_address` is the address of buf[0]).
void data_lines(translate::l2::Assembly& out, ByteView buf, std::size_t from, std::size_t to, std::uint32_t base_address) {
    std::size_t off = from;
    const auto none = [](std::uint32_t) -> std::optional<std::string> { return std::nullopt; };
    while (off < to) {
        const DataItem item = data_item(buf, off, to, base_address + static_cast<std::uint32_t>(off), none);
        out.append(item.line);
        off += item.took;
    }
}

// What the analysis of one program gives: its lines, its functions and the equates its names need.
struct Line {
    std::string text;
    std::string part;            // a fragment of the source begins here (a function), named so
    std::uint32_t part_address = 0;
};
struct Analysis {
    std::vector<Line> lines;
    iw::Functions functions;
    std::size_t code_bytes = 0;
    std::map<std::string, std::uint32_t> equates;
};

struct Program {
    Cpu cpu;
    std::string tag;             // "arm9", "arm7"
    ByteView image;
    std::uint32_t base = 0;
    std::uint32_t entry = 0;
    da::Arch arch = da::Arch::V4T;
    const formats::Decoder* decoder = nullptr;
};

}  // namespace

const std::map<std::uint32_t, std::string>& hardware(Cpu cpu) {
    // config/knowledge/nds/registers.json: the ARM9's and the ARM7's
    static const std::array<std::map<std::uint32_t, std::string>, 2> tables = {knowledge::standard().hardware_names("nds", "arm9"),
                                                                                knowledge::standard().hardware_names("nds", "arm7")};
    return tables[cpu == Cpu::Arm9 ? 0 : 1];
}

namespace {

// Analyse one program: its code from the entry point, the functions, how every address is named, the text of every instruction; the source lines.
Analysis analyse(const Program& p, const nds::Rom& rom) {
    Analysis result;
    const ByteView image = p.image;
    const std::uint32_t base = p.base;
    const std::uint32_t end = base + static_cast<std::uint32_t>(image.size());
    const ArmFlow flow(image, base, p.arch);
    const std::string& tag = p.tag;
    const iw::Spelling local{p.cpu == Cpu::Arm9 ? "L9_" : "L7_", 8};
    const iw::Spelling data_name{p.cpu == Cpu::Arm9 ? "D9_" : "D7_", 8};

    // function starts: what the entry point calls, and what the code points at or leaves behind
    std::set<std::uint32_t> entries;
    for (const std::uint32_t a : rom.functions(p.cpu, *p.decoder)) entries.insert(a);
    if (p.entry >= base && (p.entry & ~1u) < end) entries.insert(p.entry);
    iw::Walked<GInsn> walked;
    std::vector<bool> covered;   // the program's bytes that an instruction takes
    for (unsigned round = 0; round < kRounds; ++round) {
        const std::vector<std::uint32_t> from(entries.begin(), entries.end());
        walked = iw::walk<GInsn>(flow, from);
        covered.assign(image.size(), false);
        for (const auto& [key, g] : walked.code)
            for (unsigned k = 0; k < g.insn.length; ++k) covered[(key & ~1u) - base + k] = true;
        bool added = false;
        const auto candidate = [&](std::uint32_t key, bool prologue_needed) {
            const std::uint32_t real = key & ~1u;
            if (real < base || real >= end || real % ((key & 1) ? 2 : 4) != 0 || covered[real - base] || entries.contains(key)) return;
            if ((!prologue_needed || is_prologue(image, base, key)) && reads_as_function(image, base, p.arch, key)) added |= entries.insert(key).second;
        };
        // what follows a function's end: its literal pool, then the next function
        for (const auto& [key, g] : walked.code) {
            if (!flow.stops(g)) continue;
            const std::uint32_t after = (key & ~1u) + g.insn.length;
            for (std::uint32_t q = after; q < end && q - after <= kPoolReach && !covered[q - base]; q += 2) {
                if (is_prologue(image, base, q | 1)) candidate(q | 1, true);
                if (q % 4 == 0 && is_prologue(image, base, q)) candidate(q, true);
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
        if (hardware(p.cpu).contains(a)) return Kind::Hardware;
        if (within(0x02000000, 0x01000000) || within(0x03000000, 0x01000000) || within(0x05000000, 0x800) || within(0x06000000, 0x01000000) || within(0x07000000, 0x800))
            return Kind::Variable;
        return Kind::None;
    };

    // names: the entry point is `<tag>_entry`, a function `<tag>_arm_<address>` or `<tag>_thumb_<address>`
    iw::Names names;
    for (const std::uint32_t key : function_starts) {
        const std::uint32_t real = key & ~1u;
        const std::string name = (key & ~1u) == (p.entry & ~1u) ? tag + "_entry" : tag + ((key & 1) ? "_thumb_" : "_arm_") + std::format("{:08X}", real);
        result.functions[key] = name;
        names.emplace(real, name);
    }
    std::map<std::uint32_t, std::string> labels(names.begin(), names.end());
    for (const std::uint32_t t : walked.targets)
        if (const std::uint32_t real = t & ~1u; line_start(real) && !labels.contains(real)) labels[real] = local(real);

    // what the code and the tables point at
    std::map<std::string, std::uint32_t>& equates = result.equates;
    std::set<std::uint32_t> pool_words;   // the words PC-relative loads read
    for (const auto& [key, g] : code)
        if (const auto a = literal_address(g.insn); a && *a >= base && *a + 4 <= end && *a % 4 == 0 && !covered[*a - base] && !covered[*a - base + 1] && !covered[*a - base + 2] &&
                                                   !covered[*a - base + 3]) {
            pool_words.insert(*a);
            if (!labels.contains(*a)) labels[*a] = data_name(*a);
        }
    // how the value a word holds is written: a label in the program (with `+ 1` for a Thumb address), a register's name, a variable's
    const auto value_text = [&](std::uint32_t v) -> std::optional<std::string> {
        if (v >= base && v < end) {
            const std::uint32_t real = v & ~1u;
            if ((v & 1) && code.contains(v)) {
                if (!labels.contains(real)) labels[real] = local(real);
                return labels[real] + " + 1";
            }
            if (!(v & 1) && code.contains(v) && !labels.contains(v)) labels[v] = local(v);
            if (line_start(v) || labels.contains(v)) {
                if (!labels.contains(v)) labels[v] = data_name(v);
                return labels[v];
            }
            return std::nullopt;
        }
        switch (physical_kind(v)) {
            case Kind::Hardware: {
                const std::string n = hardware(p.cpu).at(v);
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
    for (std::size_t off = 0; off + 4 <= image.size(); off += 4) {   // words in the data that hold an address in the program (a jump table, a table of pointers)
        const std::uint32_t a = base + static_cast<std::uint32_t>(off);
        if (covered[off] || covered[off + 1] || covered[off + 2] || covered[off + 3] || pool_words.contains(a)) continue;
        const std::uint32_t v = word_at(image, off);
        if (v >= base && v < end)
            if (const auto t = value_text(v)) word_names[a] = *t;
    }

    // the text of each instruction, kept only when it assembles back to the instruction's bytes
    // (both processors are read and built through the ARMv5TE description: the ARM7's ARMv4T instructions are in it, and the text is kept only when it builds back)
    std::set<std::uint32_t> as_data;   // the instructions whose text would not assemble to their bytes
    std::map<std::uint32_t, std::string> instruction_text;
    for (const auto& [key, g] : code) {
        const char* cpu = g.insn.thumb() ? "thumb9" : "arm9";
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

    // the lines ---------------------------------------------------------------------------------------------------------------
    const auto emit = [&](std::string text) { result.lines.push_back({std::move(text), {}, 0}); };
    std::set<std::uint32_t> boundaries = {static_cast<std::uint32_t>(image.size())};
    for (const auto& entry : code) boundaries.insert((entry.first & ~1u) - base);
    for (const auto& entry : labels)
        if (line_start(entry.first)) boundaries.insert(entry.first - base);

    int mode = -1;   // the state the lines so far are in: 0 ARM, 1 Thumb, -1 not said yet
    std::size_t off = 0;
    while (off < image.size()) {
        const std::uint32_t addr = base + static_cast<std::uint32_t>(off);
        const auto label = labels.find(addr);
        const bool arm_code = code.contains(addr), thumb_code = code.contains(addr | 1);
        const std::uint32_t key = arm_code ? addr : addr | 1;
        if (label != labels.end() && line_start(addr)) {
            for (const std::uint32_t fk : {addr, addr | 1}) {
                if (const auto fn = result.functions.find(fk); fn != result.functions.end() && code.contains(fk)) {
                    result.lines.push_back({"", fn->second, addr});   // each function a fragment
                    mode = -1;
                    break;
                }
            }
            emit(label->second + ":");
        }
        if (arm_code || thumb_code) {
            const da::Insn& insn = code.at(key).insn;
            const int wanted = insn.thumb() ? 1 : 0;
            if (as_data.contains(key)) {
                // the bits, in pieces small enough to put a label between (an instruction another path of the code overlaps)
                const std::string comment = "  ; " + dialect::find("gas-arm").instruction(instruction_text.at(key));
                unsigned step = insn.length;
                for (unsigned k = 1; k < insn.length; ++k)
                    if (labels.contains(addr + k)) step = std::min(step, k % 2 ? 1u : 2u);
                for (unsigned k = 0; k < insn.length; k += step) {
                    if (k != 0)
                        if (const auto inner = labels.find(addr + k); inner != labels.end()) emit(inner->second + ":");
                    const auto raw16 = [&](unsigned at) { return static_cast<unsigned>(insn.raw[at] | insn.raw[at + 1] << 8); };
                    std::string line;
                    if (step == 1) line = std::format("        .byte 0x{:02X}", insn.raw[k]);
                    else if (step == 2) line = std::format("        .half 0x{:04X}", raw16(k));
                    else if (insn.length == 2) line = std::format("        .half 0x{:04X}", raw16(0));
                    else if (insn.thumb()) line = std::format("        .half 0x{:04X}, 0x{:04X}", raw16(0), raw16(2));
                    else line = std::format("        .word 0x{:08X}", insn.word);
                    emit(k == 0 ? line + comment : line);
                }
            } else {
                if (mode != wanted) {
                    emit(wanted ? "        .context TMode=1" : "        .context TMode=0");
                    mode = wanted;
                }
                emit("        " + instruction_text.at(key));
            }
            off += insn.length;
            continue;
        }
        const std::size_t limit = *boundaries.upper_bound(static_cast<std::uint32_t>(off));
        const DataItem item = data_item(image, off, limit, addr, [&](std::uint32_t a) -> std::optional<std::string> {
            const auto it = word_names.find(a);
            if (it == word_names.end()) return std::nullopt;
            return it->second;
        });
        emit(item.line);
        off += item.took;
    }
    result.code_bytes = iw::code_bytes(code, flow);
    // the names the header uses: where each function is
    return result;
}

// ---- the file ---------------------------------------------------------------------------------------------------------

std::string label_for(std::string_view prefix, std::uint32_t id) { return std::format("{}_{}", prefix, id); }

// UTF-16 text as `.half` rows with the text in a comment; the zeros after it as `.space`.
void utf16_lines(translate::l2::Assembly& out, ByteView bytes) {
    std::size_t halves = bytes.size() / 2;
    while (halves > 0 && bytes[2 * halves - 2] == 0 && bytes[2 * halves - 1] == 0) --halves;
    for (std::size_t from = 0; from < halves; from += 8) {
        std::vector<std::string> items;
        std::string shown;
        for (std::size_t k = from; k < std::min(halves, from + 8); ++k) {
            const unsigned unit = bytes[2 * k] | bytes[2 * k + 1] << 8;
            items.push_back(std::format("0x{:04X}", unit));
            shown += (unit >= 32 && unit < 127) ? std::string(1, static_cast<char>(unit)) : unit == 10 ? std::string("\\n") : std::string("?");
        }
        out.append("        .half " + text::join(items, ", ") + "   ; " + shown);
    }
    if (halves * 2 < bytes.size()) out.append(std::format("        .space {}", bytes.size() - halves * 2));
}

void zero_or_bytes(translate::l2::Assembly& out, ByteView bytes) {
    if (bytes.empty()) return;
    if (std::ranges::all_of(bytes, [](std::uint8_t b) { return b == 0; })) {
        out.append(std::format("        .space {}", bytes.size()));
        return;
    }
    for (std::size_t at = 0; at < bytes.size();) {
        const std::size_t run = same_run(bytes, at, bytes.size());
        if (run >= kFillMin) {
            out.append(bytes[at] == 0 ? std::format("        .space {}", run) : std::format("        .fill {}, 1, 0x{:02X}", run, bytes[at]));
            at += run;
            continue;
        }
        std::size_t take = std::min<std::size_t>(12, bytes.size() - at);
        for (std::size_t k = 1; k < take; ++k)
            if (same_run(bytes, at + k, bytes.size()) >= kFillMin) {
                take = k;
                break;
            }
        out.append(byte_row(bytes.subspan(at, take)));
        at += take;
    }
}

// A header text field written as a string and the zeros after it, or bytes.
void text_field(translate::l2::Assembly& out, ByteView rom, std::size_t at, std::size_t length) {
    std::size_t text_length = 0;
    while (text_length < length && printable(rom[at + text_length])) ++text_length;
    const bool zero_tail = std::ranges::all_of(rom.subspan(at + text_length, length - text_length), [](std::uint8_t b) { return b == 0; });
    if (!zero_tail) {
        out.append(byte_row(rom.subspan(at, length)));
        return;
    }
    if (text_length != 0) out.append(std::format("        .ascii \"{}\"", as_text(rom.subspan(at, text_length))));
    if (text_length < length) out.append(std::format("        .space {}", length - text_length));
}

}  // namespace

iw::Written write(const nds::Rom& rom, std::string_view file_name, const std::filesystem::path& folder) {
    fs::make_dirs(folder);
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const ByteView image = rom.data;
    const nds::Header& h = rom.header;

    const std::vector<nds::Region> regions = rom.regions();
    const auto region_of = [&](nds::Region::Kind kind) -> const nds::Region* {
        for (const nds::Region& r : regions)
            if (r.kind == kind) return &r;
        return nullptr;
    };

    // the two programs
    struct Part {
        Program program;
        Analysis analysis;
        const nds::Region* region = nullptr;
    };
    std::vector<Part> programs;
    for (const Cpu cpu : {Cpu::Arm9, Cpu::Arm7}) {
        const nds::Region* r = region_of(cpu == Cpu::Arm9 ? nds::Region::Kind::Arm9 : nds::Region::Kind::Arm7);
        if (!r) continue;
        const nds::Binary& b = rom.binary(cpu);
        Program p;
        p.cpu = cpu;
        p.tag = cpu == Cpu::Arm9 ? "arm9" : "arm7";
        p.image = rom.memory(cpu);
        p.base = b.ram_address;
        p.entry = b.entry;
        p.arch = cpu == Cpu::Arm9 ? da::Arch::V5TE : da::Arch::V4T;
        p.decoder = cpu == Cpu::Arm9 ? &isa::arm9_decoder() : &isa::arm_decoder();
        Part part{p, {}, r};
        part.analysis = analyse(part.program, rom);
        programs.push_back(std::move(part));
    }

    iw::Functions functions;
    std::size_t code_bytes = 0;
    std::map<std::string, std::uint32_t> equates;
    for (const Part& part : programs) {
        for (const auto& [key, name] : part.analysis.functions) functions.emplace(key, name);
        code_bytes += part.analysis.code_bytes;
        equates.insert(part.analysis.equates.begin(), part.analysis.equates.end());
    }
    const auto function_text = [&](const Part& part, std::uint32_t address) -> std::optional<std::string> {
        const auto it = part.analysis.functions.find(address);
        if (it == part.analysis.functions.end()) return std::nullopt;
        return it->second + ((address & 1) ? " + 1" : "");
    };

    // labels that stand for places in the file: one at the start of each region and one just after it
    std::map<std::uint32_t, const nds::Region*> by_offset;   // region start -> region
    for (const nds::Region& r : regions) by_offset[r.offset] = &r;
    const auto file_label = [&](const nds::Region& r) -> std::string {
        switch (r.kind) {
            case nds::Region::Kind::Header: return "header";
            case nds::Region::Kind::Arm9: return "arm9_rom";
            case nds::Region::Kind::Arm7: return "arm7_rom";
            case nds::Region::Kind::Fnt: return "fnt";
            case nds::Region::Kind::Fat: return "fat";
            case nds::Region::Kind::Overlays9: return "overlays9";
            case nds::Region::Kind::Overlays7: return "overlays7";
            case nds::Region::Kind::Banner: return "banner";
            case nds::Region::Kind::File: return label_for("file", r.id);
        }
        return {};
    };
    // the two words {offset, size} of a table named by labels, when the region is there as the header says
    const auto offset_expr = [&](nds::Region::Kind kind, std::uint32_t offset, std::uint32_t size, std::string_view end_label) -> std::pair<std::string, std::string> {
        const nds::Region* r = region_of(kind);
        if (r && r->offset == offset && r->size == size) {
            const std::string name = file_label(*r);
            return {name, std::format("{} - {}", end_label.empty() ? name + "_end" : std::string(end_label), name)};
        }
        return {hex8(offset), hex8(size)};
    };

    // the source ----------------------------------------------------------------------------------------------------------------
    std::vector<std::string> head = {
        std::format("; {}.s -- written by CodeBreaker from {}", stem, base_name),
        std::format("; Nintendo DS cartridge \"{}\" ({}): {} bytes, ARM9 {} functions, ARM7 {} functions", h.title, h.game_code, image.size(),
                    programs.empty() ? 0 : programs[0].analysis.functions.size(), programs.size() > 1 ? programs[1].analysis.functions.size() : 0),
        ""};
    translate::l2::Assembly out(head, "gas", std::string(file_name));
    out.append("        .syntax unified");
    out.append("");
    for (const auto& [name, value] : equates) out.append(std::format("        .equ {}, {}", name, hex8(value)));
    if (!equates.empty()) out.append("");

    const auto program_of = [&](Cpu cpu) -> const Part* {
        for (const Part& part : programs)
            if (part.program.cpu == cpu) return &part;
        return nullptr;
    };

    // the header: field by field. Each field is {length, line}; the bytes the table does not name are written in place.
    const auto header_lines = [&] {
        const ByteView b = image.subspan(0, nds::kHeaderSize);
        std::map<std::size_t, std::pair<std::size_t, std::vector<std::string>>> fields;
        const auto field = [&](std::size_t at, std::size_t length, std::vector<std::string> lines) { fields[at] = {length, std::move(lines)}; };
        const auto word = [&](std::size_t at, std::string expr, const char* what) { field(at, 4, {std::format("        .word {}   ; {}", expr, what)}); };
        const auto literal = [&](std::size_t at, const char* what) { word(at, hex8(word_at(b, at)), what); };
        const auto text_lines = [&](std::size_t at, std::size_t length, const char* what) {
            translate::l2::Assembly scratch;
            text_field(scratch, b, at, length);
            std::vector<std::string> lines = {std::string("        ; ") + what};
            lines.insert(lines.end(), scratch.lines().begin(), scratch.lines().end());
            field(at, length, std::move(lines));
        };
        const auto byte_field = [&](std::size_t at, const char* what) { field(at, 1, {std::format("        .byte 0x{:02X}   ; {}", b[at], what)}); };
        text_lines(0x00, 12, "the title");
        text_lines(0x0C, 4, "the game code");
        text_lines(0x10, 2, "the maker code");
        byte_field(0x12, "the unit code");
        byte_field(0x13, "the encryption seed");
        byte_field(0x14, "the device capacity");
        byte_field(0x1D, "the region");
        byte_field(0x1E, "the version");
        byte_field(0x1F, "the autostart flags");
        for (const Cpu cpu : {Cpu::Arm9, Cpu::Arm7}) {
            const std::size_t at = cpu == Cpu::Arm9 ? 0x20 : 0x30;
            const char* who = cpu == Cpu::Arm9 ? "ARM9" : "ARM7";
            const nds::Binary& bin = rom.binary(cpu);
            const Part* part = program_of(cpu);
            if (part && part->region->offset == bin.rom_offset && part->region->size == bin.size) {
                const std::string tag = part->program.tag;
                const std::optional<std::string> entry = function_text(*part, bin.entry);
                word(at, tag + "_rom", cpu == Cpu::Arm9 ? "the ARM9 program's place in the file" : "the ARM7 program's place in the file");
                word(at + 4, entry ? *entry : hex8(bin.entry), cpu == Cpu::Arm9 ? "where the ARM9 starts" : "where the ARM7 starts");
                word(at + 8, tag, cpu == Cpu::Arm9 ? "the ARM9 program's RAM address" : "the ARM7 program's RAM address");
                word(at + 12, tag + "_end - " + tag, cpu == Cpu::Arm9 ? "the ARM9 program's size" : "the ARM7 program's size");
            } else {
                literal(at, who);
                literal(at + 4, who);
                literal(at + 8, who);
                literal(at + 12, who);
            }
        }
        {
            const auto [fnt, fnt_size] = offset_expr(nds::Region::Kind::Fnt, h.fnt_offset, h.fnt_size, "");
            word(0x40, fnt, "the file name table");
            word(0x44, fnt_size, "its size");
            const auto [fat, fat_size] = offset_expr(nds::Region::Kind::Fat, h.fat_offset, h.fat_size, "");
            word(0x48, fat, "the file allocation table");
            word(0x4C, fat_size, "its size");
            const auto [o9, o9_size] = offset_expr(nds::Region::Kind::Overlays9, h.ovl9_offset, h.ovl9_size, "");
            word(0x50, o9, "the ARM9 overlay table");
            word(0x54, o9_size, "its size");
            const auto [o7, o7_size] = offset_expr(nds::Region::Kind::Overlays7, h.ovl7_offset, h.ovl7_size, "");
            word(0x58, o7, "the ARM7 overlay table");
            word(0x5C, o7_size, "its size");
        }
        literal(0x60, "card control, normal commands");
        literal(0x64, "card control, KEY1 commands");
        if (const nds::Region* banner = region_of(nds::Region::Kind::Banner); banner && banner->offset == h.banner_offset) word(0x68, "banner", "the icon and title");
        else literal(0x68, "the icon and title");
        field(0x6C, 2, {std::format("        .half 0x{:04X}   ; the secure area's CRC-16", b[0x6C] | b[0x6D] << 8)});
        field(0x6E, 2, {std::format("        .half 0x{:04X}   ; the secure area delay", b[0x6E] | b[0x6F] << 8)});
        literal(0x70, "the ARM9 auto-load hook");
        literal(0x74, "the ARM7 auto-load hook");
        field(0x78, 8, {byte_row(b.subspan(0x78, 8)) + "   ; secure area disable"});
        literal(0x80, "the total used ROM size");
        literal(0x84, "the header size");
        {
            std::vector<std::string> lines = {"        ; the Nintendo logo"};
            for (std::size_t at = 0xC0; at < 0xC0 + 156; at += 12) lines.push_back(byte_row(b.subspan(at, 12)));
            field(0xC0, 156, std::move(lines));
        }
        field(0x15C, 2, {std::format("        .half 0x{:04X}   ; the logo's CRC-16", b[0x15C] | b[0x15D] << 8)});
        field(0x15E, 2, {std::format("        .half 0x{:04X}   ; the header's CRC-16", b[0x15E] | b[0x15F] << 8)});
        literal(0x160, "the debug ROM's offset");
        literal(0x164, "the debug ROM's size");
        literal(0x168, "the debug ROM's RAM address");
        std::size_t at = 0;
        while (at < nds::kHeaderSize) {
            if (const auto f = fields.find(at); f != fields.end()) {
                for (const std::string& line : f->second.second) out.append(line);
                at += f->second.first;
                continue;
            }
            std::size_t to = at;   // reserved bytes up to the next field
            while (to < nds::kHeaderSize && !fields.contains(to)) ++to;
            zero_or_bytes(out, b.subspan(at, to - at));
            at = to;
        }
    };

    // the file allocation table: a pair of labels for each file, or the numbers
    const auto fat_lines = [&](const nds::Region& r) {
        std::map<std::uint32_t, const nds::Region*> files;
        for (const nds::Region& f : regions)
            if (f.kind == nds::Region::Kind::File) files[f.id] = &f;
        const std::uint32_t count = r.size / 8;
        for (std::uint32_t id = 0; id < count; ++id) {
            const std::uint32_t start = word_at(image, r.offset + id * 8), stop = word_at(image, r.offset + id * 8 + 4);
            const auto f = files.find(id);
            if (f != files.end() && f->second->offset == start && f->second->end() == stop) {
                const std::string name = file_label(*f->second);
                out.append(std::format("        .word {}, {}_end   ; file {}{}", name, name, id, f->second->name.empty() ? "" : " " + f->second->name));
            } else {
                out.append(std::format("        .word {}, {}   ; file {}", hex8(start), hex8(stop), id));
            }
        }
        if (r.size % 8 != 0) zero_or_bytes(out, image.subspan(r.offset + count * 8, r.size % 8));
    };

    const auto overlay_lines = [&](const nds::Region& r) {
        out.append("        ; one entry of eight words for each overlay: id, RAM address, size, bss size, static initialisers start and end, file id, reserved");
        std::size_t at = 0;
        for (; at + 32 <= r.size; at += 32) {
            std::vector<std::string> items;
            for (std::size_t k = 0; k < 32; k += 4) items.push_back(hex8(word_at(image, r.offset + at + k)));
            out.append("        .word " + text::join(items, ", "));
        }
        if (at < r.size) zero_or_bytes(out, image.subspan(r.offset + at, r.size - at));
    };

    const auto banner_lines = [&](const nds::Region& r) {
        const ByteView b = image.subspan(r.offset, r.size);
        out.append(std::format("        .half 0x{:04X}   ; the banner's version", b[0] | b[1] << 8));
        out.append(std::format("        .half 0x{:04X}   ; its CRC-16", b[2] | b[3] << 8));
        out.append("        ; the CRC-16s of the other languages' parts, then reserved bytes");
        zero_or_bytes(out, b.subspan(4, 0x20 - 4));
        out.append("        ; the icon: 32 x 32 pixels, 4 bits each, in 8 x 8 tiles");
        for (std::size_t at = 0x20; at < 0x220; at += 32) {
            std::vector<std::string> items;
            for (std::size_t k = 0; k < 32; k += 4) items.push_back(hex8(word_at(b, at + k)));
            out.append("        .word " + text::join(items, ", "));
        }
        out.append("        ; the icon's palette: 16 colours, BGR555");
        std::vector<std::string> palette;
        for (std::size_t k = 0; k < 32; k += 2) palette.push_back(std::format("0x{:04X}", b[0x220 + k] | b[0x221 + k] << 8));
        out.append("        .half " + text::join(palette, ", "));
        static constexpr const char* languages[] = {"Japanese", "English", "French", "German", "Italian", "Spanish", "Chinese", "Korean"};
        std::size_t at = 0x240;
        for (unsigned k = 0; k < 8 && at + 0x100 <= b.size(); ++k, at += 0x100) {
            out.append(std::format("        ; the title in {}", languages[k]));
            utf16_lines(out, b.subspan(at, 0x100));
        }
        if (at < b.size()) zero_or_bytes(out, b.subspan(at));
    };

    out.part("header", "header", std::nullopt);
    std::uint32_t pos = 0;
    const auto gap_to = [&](std::uint32_t to) {
        if (pos < to) data_lines(out, image, pos, to, pos);
        pos = to;
    };
    for (const nds::Region& r : regions) {
        gap_to(r.offset);
        const std::string name = file_label(r);
        switch (r.kind) {
            case nds::Region::Kind::Header:
                out.append(name + ":");
                header_lines();
                break;
            case nds::Region::Kind::Arm9: case nds::Region::Kind::Arm7: {
                const Part* part = program_of(r.kind == nds::Region::Kind::Arm9 ? Cpu::Arm9 : Cpu::Arm7);
                if (!part) break;
                const std::string& tag = part->program.tag;
                out.part(tag, "program", std::nullopt);
                out.append(name + ":");
                out.append(std::format("        .org {}", hex8(part->program.base)));
                out.append(r.kind == nds::Region::Kind::Arm9 ? "        .cpu arm946e-s" : "        .cpu arm7tdmi");
                out.append(tag + ":");
                for (const Line& line : part->analysis.lines) {
                    if (!line.part.empty()) {
                        out.part(line.part, "function", translate::ir::OriginVa{out.module().value_or(""), line.part_address});
                        out.append("");
                    } else {
                        out.append(line.text);
                    }
                }
                out.append(tag + "_end:");
                out.append(std::format("        .org {}", hex8(r.end())));
                break;
            }
            case nds::Region::Kind::Fnt:
                out.part("filesystem", "section", std::nullopt);
                out.append(name + ":");
                data_lines(out, image, r.offset, r.end(), r.offset);
                break;
            case nds::Region::Kind::Fat:
                out.append(name + ":");
                fat_lines(r);
                break;
            case nds::Region::Kind::Overlays9: case nds::Region::Kind::Overlays7:
                out.append(name + ":");
                overlay_lines(r);
                break;
            case nds::Region::Kind::Banner:
                out.append(name + ":");
                banner_lines(r);
                break;
            case nds::Region::Kind::File:
                out.append(name + ":" + (r.name.empty() ? "" : "   ; " + r.name));
                data_lines(out, image, r.offset, r.end(), r.offset);
                break;
        }
        out.append(name + "_end:");
        pos = r.end();
    }
    gap_to(static_cast<std::uint32_t>(image.size()));

    const std::string source = dialect::find("gas-arm").print(iw::text(out) + "\n");   // (said in GNU as's: config/dialects/gas-arm.json)
    iw::Written r = iw::finish(folder, stem + ".s", source, functions, code_bytes, image.size());
    iw::assembled(r, [&] { return risc_source::assemble("arm9", dialect::find("gas-arm").parse(source), folder); }, image, folder / (stem + ".built.nds"), "ROM");
    return r;
}

iw::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder) {
    return write(nds::read(rom_file), rom_file.filename().string(), folder);
}

}  // namespace cb::source::nds_writer
