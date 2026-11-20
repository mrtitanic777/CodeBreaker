// analyze/dos_functions.cpp -- see dos_functions.hpp.
#include "analyze/dos_functions.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <format>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <iterator>

#include "core/capstone_api.hpp"
#include "core/text.hpp"
#include "platforms/decoder.hpp"
#include "symbols/dos_interrupts.hpp"

namespace cb::analyze::dos_functions {

std::string_view name_of(Evidence evidence) {
    switch (evidence) {
        case Evidence::Entry: return "entry";
        case Evidence::Called: return "called";
        case Evidence::Vector: return "vector";
        case Evidence::Table: return "table";
        case Evidence::Variable: return "variable";
        case Evidence::Pointer: return "pointer";
        case Evidence::Prologue: return "prologue";
        case Evidence::Follows: return "follows";
    }
    return "?";
}

std::string_view name_of(Distance distance) { return distance == Distance::Far ? "far" : "near"; }

namespace {

using Lin = std::int64_t;   // a linear address, signed: it is computed from displacements and may leave the image

constexpr std::size_t kMaxInsnBytes = 16;

// Prefixes real-mode code puts before an opcode.
bool is_prefix(std::uint8_t b) {
    switch (b) {
        case 0x26: case 0x2E: case 0x36: case 0x3E: case 0x64: case 0x65: case 0x66: case 0x67: case 0xF0: case 0xF2: case 0xF3:
            return true;
        default:
            return false;
    }
}

// First bytes of instructions that real-mode programs hardly use and that text and tables decode to.
bool is_odd_opcode(std::uint8_t b) {
    switch (b) {
        case 0x00: case 0x27: case 0x2F: case 0x37: case 0x3F: case 0x5C: case 0x62: case 0x63: case 0x6C: case 0x6D:
        case 0x6E: case 0x6F: case 0xD6: case 0xF1:
            return true;
        default:
            return false;
    }
}

// What an instruction does to control flow.
enum class Flow { Next, Call, Jump, Branch, Stop };

enum class ReturnKind { Near, Far, Iret };
struct Return {
    ReturnKind kind = ReturnKind::Near;
    int removes = 0;
};

// The mnemonics the register tracking reads.
enum class Mnemonic { Other, Push, Pop, Mov, Lea, Xor, Sub };

// One operand as Capstone gives it.
struct Operand {
    enum class Type { Other, Reg, Imm, Mem };
    Type type = Type::Other;
    unsigned reg = 0;
    std::int64_t imm = 0;
    int size = 0;
    unsigned segment = 0, base = 0, index = 0;
    std::int64_t disp = 0;
};

// One instruction, as far as control flow and the register tracking need it.
struct Ins {
    int size = 0;
    Flow flow = Flow::Next;
    std::vector<Lin> targets;
    bool far = false;
    std::optional<Return> ret;
    std::optional<std::uint8_t> number;     // the interrupt number of an `int`
    std::optional<Lin> table;               // `call/jmp [table + index]`: where the table is
    std::optional<Lin> variable;            // `call/jmp [variable]`: where the pointer is
    bool odd = false;
    Lin cs = 0;                             // the segment base it was first decoded under
    Mnemonic mnemonic = Mnemonic::Other;
    int operand_count = 0;
    std::array<Operand, 2> operands{};      // the first two
};

// ---- the constants a straight run of instructions loads into registers --------------------------------------------

enum class Reg : std::size_t { Al, Ah, Bl, Bh, Cl, Ch, Dl, Dh, Si, Di, Bp, Ds, Es, Count };

std::optional<Reg> reg_of(std::string_view name) {
    static const std::map<std::string_view, Reg> table = {
        {"al", Reg::Al}, {"ah", Reg::Ah}, {"bl", Reg::Bl}, {"bh", Reg::Bh}, {"cl", Reg::Cl}, {"ch", Reg::Ch}, {"dl", Reg::Dl},
        {"dh", Reg::Dh}, {"si", Reg::Si}, {"di", Reg::Di}, {"bp", Reg::Bp}, {"ds", Reg::Ds}, {"es", Reg::Es}};
    const auto it = table.find(name);
    return it == table.end() ? std::nullopt : std::optional<Reg>(it->second);
}

// ax, bx, cx, dx: the two halves.
std::optional<std::pair<Reg, Reg>> halves_of(std::string_view name) {
    if (name == "ax") return std::pair{Reg::Al, Reg::Ah};
    if (name == "bx") return std::pair{Reg::Bl, Reg::Bh};
    if (name == "cx") return std::pair{Reg::Cl, Reg::Ch};
    if (name == "dx") return std::pair{Reg::Dl, Reg::Dh};
    return std::nullopt;
}

// Registers a call or an `int` may change.
constexpr std::array<Reg, 12> kVolatile = {Reg::Al, Reg::Ah, Reg::Bl, Reg::Bh, Reg::Cl, Reg::Ch,
                                           Reg::Dl, Reg::Dh, Reg::Si, Reg::Di, Reg::Bp, Reg::Es};

struct Regs {
    std::array<std::optional<std::int64_t>, static_cast<std::size_t>(Reg::Count)> value{};   // known constants
    std::optional<std::int64_t> pushed;                              // what the last `push` pushed
    std::map<std::int64_t, std::optional<std::int64_t>> table;       // stores into the interrupt table: offset -> value

    std::optional<std::int64_t>& at(Reg r) { return value[static_cast<std::size_t>(r)]; }
    const std::optional<std::int64_t>& at(Reg r) const { return value[static_cast<std::size_t>(r)]; }
    // a 16-bit register from its two halves
    std::optional<std::int64_t> word(std::pair<Reg, Reg> halves) const {
        const auto &lo = at(halves.first), &hi = at(halves.second);
        return lo && hi ? std::optional<std::int64_t>(*hi << 8 | *lo) : std::nullopt;
    }
};

// A handler stored into the interrupt table: its address and the interrupt.
struct Vector {
    Lin handler = 0;
    std::uint8_t number = 0;
};

// The one pass over a program.
class Finder {
public:
    explicit Finder(const formats::dos::Program& program)
        : program_(program),
          base_(program.base),
          decoder_(platforms::Decoder::open(platforms::Arch::X86, 16, true)),
          mark_(program.image.size(), 0),
          guess_(program.image.size(), 0),
          relocs_(program.relocations.begin(), program.relocations.end()) {}

    Analysis run() {
        for (const formats::dos::Entry& e : program_.entries) {
            const std::uint32_t* seg = program_.segments.find(e.address);
            const Lin cs = (seg != nullptr ? Lin{*seg} : Lin{(e.address >> 4) & 0xF000}) * 16;
            add(e.address, e.call == formats::dos::Entry::Call::Far ? Distance::Far : Distance::Near, Evidence::Entry, cs, e.name);
        }
        drain();
        guesses();
        Analysis out;
        out.interrupts = name_interrupts();
        for (const auto& [va, f] : functions_) out.functions.emplace(static_cast<std::uint32_t>(va), f);
        for (const auto& [at, n] : tables_) out.tables.emplace(static_cast<std::uint32_t>(at), n);
        for (const auto& [handler, number] : vectors_) out.vectors.emplace(static_cast<std::uint32_t>(handler), number);
        out.census = census();
        out.bytes.reserve(mark_.size());
        for (const std::uint8_t m : mark_) out.bytes.push_back(static_cast<ByteClass>(m));
        out.guessed.assign(guess_.begin(), guess_.end());
        return out;
    }

private:
    // ---- the image -------------------------------------------------------------------------------------------------

    bool inside(Lin va) const { return va >= 0 && program_.inside(static_cast<std::uint64_t>(va)); }
    std::size_t index(Lin va) const { return static_cast<std::size_t>(va - base_); }
    std::uint8_t byte_at(Lin va) const { return program_.image[index(va)]; }
    // The word at `va`; both bytes must be in the image (callers check).
    Lin word(Lin va) const { return byte_at(va) | (Lin{byte_at(va + 1)} << 8); }
    std::uint8_t& mark(Lin va) { return mark_[index(va)]; }
    static bool is_part(std::uint8_t m) { return m == 2 || m == 3; }   // inside an instruction, or a jump table

    // ---- reading instructions ---------------------------------------------------------------------------------------

    // The instruction at `va`, or none when the bytes are not one. Remembered: it was decoded under the segment of
    // its first reader.
    const Ins* decode(Lin va, Lin cs) {
        const auto cached = cache_.find(va);
        if (cached != cache_.end()) return cached->second ? &*cached->second : nullptr;
        std::optional<Ins> ins = decode_now(va, cs);
        const auto it = cache_.emplace(va, std::move(ins)).first;
        return it->second ? &*it->second : nullptr;
    }

    std::optional<Ins> decode_now(Lin va, Lin cs) {
        if (!inside(va)) return std::nullopt;
        const std::size_t from = index(va);
        const std::size_t length = std::min(kMaxInsnBytes, program_.image.size() - from);
        const ByteView raw(program_.image.data() + from, length);
        if (length >= 2 && ((raw[0] == 0x00 && raw[1] == 0x00) || (raw[0] == 0xFF && raw[1] == 0xFF))) return std::nullopt;
        std::optional<Ins> out;
        decoder_.each(raw, static_cast<std::uint64_t>(va) & 0xFFFF, 1, [&](const cs_insn& got) {
            out = classify(raw, va, cs, got);
            return false;
        });
        return out;
    }

    // What control flow needs of one decoded instruction.
    std::optional<Ins> classify(ByteView raw, Lin va, Lin cs, const cs_insn& got) {
        if (!inside(va + got.size - 1)) return std::nullopt;
        Ins ins;
        ins.size = got.size;
        ins.cs = cs;
        std::size_t k = 0;
        while (k + 1 < got.size && is_prefix(raw[k])) ++k;
        const std::uint8_t op = raw[k];
        const Lin next = va + got.size;
        auto at = [&](std::size_t i) -> std::uint8_t { return i < raw.size() ? raw[i] : std::uint8_t{0}; };
        auto s8 = [&](std::size_t i) { return static_cast<std::int64_t>(static_cast<std::int8_t>(at(i))); };
        auto s16 = [&](std::size_t i) { return static_cast<std::int64_t>(static_cast<std::int16_t>(at(i) | (at(i + 1) << 8))); };
        auto u16 = [&](std::size_t i) { return static_cast<Lin>(at(i) | (at(i + 1) << 8)); };
        // a near target stays inside the 64 KB of its segment
        auto near = [&](Lin rel) -> Lin {
            const Lin t = next + rel;
            return (cs <= t && t < cs + 0x10000) ? t : cs + ((t - cs) & 0xFFFF);
        };
        const bool has_0f = op == 0x0F;
        ins.odd = is_odd_opcode(op) || std::find(raw.begin(), raw.begin() + static_cast<std::ptrdiff_t>(k), 0x64) != raw.begin() + static_cast<std::ptrdiff_t>(k) ||
                  std::find(raw.begin(), raw.begin() + static_cast<std::ptrdiff_t>(k), 0x65) != raw.begin() + static_cast<std::ptrdiff_t>(k) ||
                  (has_0f && at(k + 1) < 0x10);
        ins.mnemonic = mnemonic_of(got.mnemonic);
        const cs_x86& x = got.detail->x86;
        ins.operand_count = x.op_count;
        for (int i = 0; i < x.op_count && i < 2; ++i) ins.operands[static_cast<std::size_t>(i)] = operand_of(x.operands[i]);

        if (op == 0xE8) {
            ins.flow = Flow::Call;
            ins.targets = {near(s16(k + 1))};
        } else if (op == 0xE9) {
            ins.flow = Flow::Jump;
            ins.targets = {near(s16(k + 1))};
        } else if (op == 0xEB) {
            ins.flow = Flow::Jump;
            ins.targets = {near(s8(k + 1))};
        } else if ((op >= 0x70 && op <= 0x7F) || (op >= 0xE0 && op <= 0xE3)) {
            ins.flow = Flow::Branch;
            ins.targets = {near(s8(k + 1))};
        } else if (has_0f && at(k + 1) >= 0x80 && at(k + 1) <= 0x8F) {
            ins.flow = Flow::Branch;
            ins.targets = {near(s16(k + 2))};
        } else if (op == 0x9A || op == 0xEA) {
            const Lin off = u16(k + 1), seg = u16(k + 3);
            ins.flow = op == 0x9A ? Flow::Call : Flow::Jump;
            ins.far = true;
            ins.targets = {seg * 16 + off};
            if (!relocs_.empty() && !relocs_.contains(static_cast<std::uint32_t>(va + static_cast<Lin>(k) + 3))) {
                ins.targets.clear();   // an absolute segment: not this program
            }
        } else if (op == 0xC3 || op == 0xC2 || op == 0xCB || op == 0xCA || op == 0xCF) {
            ins.flow = Flow::Stop;
            ins.ret = Return{op == 0xCF ? ReturnKind::Iret : (op == 0xC3 || op == 0xC2) ? ReturnKind::Near : ReturnKind::Far,
                             (op == 0xC2 || op == 0xCA) ? static_cast<int>(u16(k + 1)) : 0};
        } else if (op == 0xCD || op == 0xCC || op == 0xCE) {
            ins.number = op == 0xCD ? at(k + 1) : op == 0xCC ? std::uint8_t{3} : std::uint8_t{4};
        } else if (op == 0xF4) {
            ins.flow = Flow::Stop;
        } else if (op == 0xFF) {
            const unsigned kind = (at(k + 1) >> 3) & 7;
            if (kind >= 2 && kind <= 5) {
                ins.flow = (kind == 2 || kind == 3) ? Flow::Call : Flow::Jump;
                if (x.op_count == 1 && x.operands[0].type == X86_OP_MEM) {
                    const x86_op_mem& m = x.operands[0].mem;
                    const bool through_ds_or_cs = m.segment == 0 || m.segment == X86_REG_CS || m.segment == X86_REG_DS;
                    // a segment's linear address; DS is the program's, and a program with none gets no table or variable
                    const auto segment_base = [&]() -> std::optional<Lin> {
                        if (m.segment == X86_REG_CS) return cs;
                        if (program_.ds) return Lin{*program_.ds} * 16;
                        return std::nullopt;
                    };
                    if (kind == 2 || kind == 4) {
                        const bool indexed = m.index != 0 || m.base == X86_REG_BX || m.base == X86_REG_SI || m.base == X86_REG_DI;
                        if (indexed && m.disp != 0 && through_ds_or_cs) {
                            if (const auto seg = segment_base()) ins.table = *seg + (m.disp & 0xFFFF);
                        }
                    }
                    if (m.index == 0 && m.base == 0 && through_ds_or_cs) {   // through one pointer variable
                        if (const auto seg = segment_base()) {
                            ins.variable = *seg + (m.disp & 0xFFFF);
                            ins.far = kind == 3 || kind == 5;
                        }
                    }
                }
            }
        }
        return ins;
    }

    static Mnemonic mnemonic_of(const char* m) {
        const std::string_view s(m);
        if (s == "push") return Mnemonic::Push;
        if (s == "pop") return Mnemonic::Pop;
        if (s == "mov") return Mnemonic::Mov;
        if (s == "lea") return Mnemonic::Lea;
        if (s == "xor") return Mnemonic::Xor;
        if (s == "sub") return Mnemonic::Sub;
        return Mnemonic::Other;
    }

    static Operand operand_of(const cs_x86_op& o) {
        Operand out;
        out.size = o.size;
        switch (o.type) {
            case X86_OP_REG:
                out.type = Operand::Type::Reg;
                out.reg = static_cast<unsigned>(o.reg);
                break;
            case X86_OP_IMM:
                out.type = Operand::Type::Imm;
                out.imm = o.imm;
                break;
            case X86_OP_MEM:
                out.type = Operand::Type::Mem;
                out.segment = static_cast<unsigned>(o.mem.segment);
                out.base = static_cast<unsigned>(o.mem.base);
                out.index = static_cast<unsigned>(o.mem.index);
                out.disp = o.mem.disp;
                break;
            default:
                break;
        }
        return out;
    }

    // Does decoding from `va` run cleanly into a return, a known instruction or an indirect jump -- without meeting
    // bytes that are no instruction, the middle of a known one, or a jump out of the program? `strict` also refuses
    // instructions real-mode programs hardly use, a start that reads as text, and an end in an indirect jump.
    bool looks_like_code(Lin va, Lin cs, std::size_t limit = 1500, bool strict = false) {
        if (strict) {
            // (the eight bytes of the image from `va`, whatever span they are in; no bytes at all read as text)
            bool text = true;
            for (Lin i = std::max<Lin>(va - base_, 0); i < va - base_ + 8 && i < static_cast<Lin>(program_.image.size()); ++i) {
                text = text && program_.image[static_cast<std::size_t>(i)] >= 0x20 && program_.image[static_cast<std::size_t>(i)] < 0x7F;
            }
            if (text) return false;
        }
        std::unordered_set<Lin> seen;
        std::vector<Lin> work{va};
        bool ended = false;
        while (!work.empty() && seen.size() < limit) {
            Lin a = work.back();
            work.pop_back();
            while (!seen.contains(a)) {
                const Ins* ins = decode(a, cs);
                if (ins == nullptr || is_part(mark(a)) || (strict && ins->odd)) return false;
                seen.insert(a);
                if (mark(a) == 1) {
                    ended = true;
                    break;
                }
                if (ins->flow == Flow::Stop || ends_program(*ins, std::nullopt)) {
                    ended = ended || ins->ret.has_value() || ins->number.has_value();
                    break;
                }
                if ((ins->flow == Flow::Jump || ins->flow == Flow::Branch) && !ins->far) {
                    for (Lin t : ins->targets) {
                        if (!inside(t)) return false;
                        work.push_back(t);
                    }
                }
                if (ins->flow == Flow::Jump) {
                    ended = ended || (ins->targets.empty() && !strict);
                    break;
                }
                a += ins->size;
            }
        }
        return ended && seen.size() >= (strict ? 2u : 3u);
    }

    // `int 20h`, `int 27h`, `int 21h` with AH = 4Ch, 00h or 31h end the program.
    static bool ends_program(const Ins& ins, std::optional<std::int64_t> ah) {
        if (!ins.number) return false;
        if (*ins.number == 0x20 || *ins.number == 0x27) return true;
        return *ins.number == 0x21 && ah && (*ah == 0x4C || *ah == 0x00 || *ah == 0x31);
    }

    // ---- following a function ---------------------------------------------------------------------------------------

    Function* add(Lin va, Distance distance, Evidence evidence, Lin cs, std::string name = {}, bool sure = true) {
        if (functions_.contains(va) || !inside(va) || is_part(mark(va))) return nullptr;
        Function f;
        f.address = static_cast<std::uint32_t>(va);
        f.name = name.empty() ? std::format("sub_{:05X}", va - base_) : std::move(name);
        f.distance = distance;
        f.evidence = evidence;
        f.cs = static_cast<std::uint32_t>(cs);
        f.sure = sure;
        const auto it = functions_.emplace(va, std::move(f)).first;
        queue_.push_back(va);
        return &it->second;
    }

    // The entries of a jump table at `at`: words that are offsets of code in segment `cs`, up to the first that is
    // not, or to where the table would run into its own targets.
    std::vector<Lin> table_entries(Lin at, Lin cs) {
        std::vector<Lin> out;
        Lin stop = at + 512;
        while (at + 2 <= stop && inside(at + 1) && (mark(at) == 0 || mark(at) == 3) && (mark(at + 1) == 0 || mark(at + 1) == 3)) {
            const Lin t = cs + word(at);
            const bool known = labels_.contains(t) || functions_.contains(t) || (inside(t) && mark(t) == 1);
            if (!inside(t) || t == cs || t == at || !(known || looks_like_code(t, cs))) break;
            out.push_back(t);
            if (t > at) stop = std::min(stop, t);
            at += 2;
        }
        return out;
    }

    // Mark everything reached from one function's start; note its returns and its calls.
    void follow(Lin start) {
        // (the function map holds the record; nothing here adds to it before the end)
        const bool sure = functions_.at(start).sure;
        const Lin fcs = functions_.at(start).cs;
        const bool guessing = !sure;
        std::unordered_set<Lin> seen;
        std::vector<Lin> work{start};
        std::vector<Return> returns;
        while (!work.empty()) {
            Lin a = work.back();
            work.pop_back();
            Regs regs;
            while (!seen.contains(a) && inside(a)) {
                const Ins* ins = decode(a, fcs);
                const std::size_t i = index(a);
                if (ins == nullptr || is_part(mark_[i])) break;   // runs into data, or into the middle of code
                seen.insert(a);
                if (mark_[i] == 0) {
                    mark_[i] = 1;
                    for (std::size_t j = i + 1; j < i + static_cast<std::size_t>(ins->size); ++j) mark_[j] = 2;
                    if (guessing) std::fill_n(guess_.begin() + static_cast<std::ptrdiff_t>(i), ins->size, std::uint8_t{1});
                } else if (!guessing && guess_[i] != 0) {
                    std::fill_n(guess_.begin() + static_cast<std::ptrdiff_t>(i), ins->size, std::uint8_t{0});
                }
                bool ends = false;
                if (ins->number) {
                    sites_[a] = *ins->number;
                    const auto ds = regs.at(Reg::Ds);
                    const auto off = regs.word({Reg::Dl, Reg::Dh});
                    if (*ins->number == 0x21 && regs.at(Reg::Ah) == 0x25 && regs.at(Reg::Al) && ds && off) {
                        vectors_.try_emplace(*ds * 16 + *off, static_cast<std::uint8_t>(*regs.at(Reg::Al)));
                    }
                    ends = ends_program(*ins, regs.at(Reg::Ah));
                }
                if (const std::optional<Vector> vector = constants(a, *ins, regs)) vectors_.try_emplace(vector->handler, vector->number);
                if (ins->ret) returns.push_back(*ins->ret);
                if (ins->table && !tables_.contains(*ins->table) && inside(*ins->table)) {
                    const std::vector<Lin> targets = table_entries(*ins->table, fcs);
                    if (targets.size() >= 2) {
                        tables_[*ins->table] = targets.size();
                        for (Lin j = *ins->table; j < *ins->table + 2 * static_cast<Lin>(targets.size()); ++j) mark(j) = 3;
                        if (ins->flow == Flow::Jump) {
                            labels_.insert(targets.begin(), targets.end());
                            work.insert(work.end(), targets.begin(), targets.end());
                        } else {
                            for (Lin t : targets) add(t, Distance::Near, Evidence::Table, fcs, {}, sure);
                        }
                    }
                }
                if (ins->variable && inside(*ins->variable) && inside(*ins->variable + (ins->far ? 3 : 1)) && mark(*ins->variable) == 0) {
                    const Lin at = *ins->variable;
                    Lin t, cs;
                    if (!ins->far) {
                        cs = fcs;
                        t = fcs + word(at);
                    } else {
                        cs = word(at + 2) * 16;
                        t = (relocs_.empty() || relocs_.contains(static_cast<std::uint32_t>(at + 2))) ? cs + word(at) : -1;
                    }
                    if (t != cs && inside(t) && (mark(t) == 1 || looks_like_code(t, cs))) {
                        add(t, ins->far ? Distance::Far : Distance::Near, Evidence::Variable, cs, {}, sure);
                    }
                }
                if (ins->flow == Flow::Call) {
                    for (Lin t : ins->targets) {
                        if (t != a + ins->size) {   // `call $+3` only fetches the address
                            add(t, ins->far ? Distance::Far : Distance::Near, Evidence::Called, ins->far ? far_cs(a, *ins) : fcs, {}, sure);
                        }
                    }
                } else if (ins->flow == Flow::Jump || ins->flow == Flow::Branch) {
                    for (Lin t : ins->targets) {
                        if (ins->far) {
                            add(t, Distance::Far, Evidence::Called, far_cs(a, *ins), {}, sure);
                        } else if (inside(t) && !functions_.contains(t)) {
                            labels_.insert(t);
                            work.push_back(t);
                        }
                    }
                }
                if (ins->flow == Flow::Stop || ins->flow == Flow::Jump || ends) break;
                a += ins->size;
            }
        }
        Function& f = functions_.at(start);
        std::uint32_t size = 0;
        for (Lin a : seen) size += static_cast<std::uint32_t>(cache_.at(a)->size);
        f.size = size;
        const auto far = static_cast<std::ptrdiff_t>(std::count_if(returns.begin(), returns.end(), [](const Return& r) { return r.kind != ReturnKind::Near; }));
        const std::ptrdiff_t near = static_cast<std::ptrdiff_t>(returns.size()) - far;
        if (far != near) f.distance = far > near ? Distance::Far : Distance::Near;
        f.interrupt = near == 0 && std::any_of(returns.begin(), returns.end(), [](const Return& r) { return r.kind == ReturnKind::Iret; });
        std::set<int> removed;
        for (const Return& r : returns) {
            if ((r.kind != ReturnKind::Near) == (f.distance == Distance::Far)) removed.insert(r.removes);
        }
        f.removes = removed.size() == 1 ? *removed.begin() : -1;
    }

    // The segment base of a far call or jump's target: the segment word of the instruction.
    Lin far_cs(Lin va, const Ins& ins) const { return word(va + ins.size - 2) * 16; }

    // ---- constants in registers, along a straight run of instructions ------------------------------------------------

    // The value of a register by name: a half's or a whole one's constant, CS from the segment, nothing for the rest.
    std::optional<std::int64_t> get(const Regs& regs, const Ins& ins, std::string_view name) const {
        if (const auto halves = halves_of(name)) return regs.word(*halves);
        if (name == "cs") return ins.cs >> 4;
        const auto r = reg_of(name);
        return r ? regs.at(*r) : std::nullopt;
    }

    // Set a register by name; one the tracking does not follow is ignored.
    static void put(Regs& regs, std::string_view name, std::optional<std::int64_t> value) {
        if (const auto halves = halves_of(name)) {
            regs.at(halves->first) = value ? std::optional<std::int64_t>(*value & 0xFF) : std::nullopt;
            regs.at(halves->second) = value ? std::optional<std::int64_t>((*value >> 8) & 0xFF) : std::nullopt;
        } else if (const auto r = reg_of(name)) {
            regs.at(*r) = value;
        }
    }

    std::string reg_name(unsigned id) const {
        const char* s = decoder_.register_name(id);
        return s != nullptr ? s : "";
    }

    // Update `regs` across the instruction at `va`. Only the simple ways a program loads a service number or a pointer
    // are understood; anything else that writes a register forgets it. Gives (handler, interrupt) when the instruction
    // completes a pair of stores into the interrupt table at 0000:4n.
    std::optional<Vector> constants(Lin va, const Ins& ins, Regs& regs) {
        const Operand& o0 = ins.operands[0];
        const Operand& o1 = ins.operands[1];
        const int count = ins.operand_count;
        using Type = Operand::Type;
        if (ins.mnemonic == Mnemonic::Push && count == 1) {
            regs.pushed = o0.type == Type::Reg ? get(regs, ins, reg_name(o0.reg))
                          : o0.type == Type::Imm ? std::optional<std::int64_t>(o0.imm & 0xFFFF) : std::nullopt;
            return std::nullopt;
        }
        const std::optional<std::int64_t> pushed = std::exchange(regs.pushed, std::nullopt);
        if (ins.mnemonic == Mnemonic::Pop && count == 1 && o0.type == Type::Reg) {
            put(regs, reg_name(o0.reg), pushed);
            return std::nullopt;
        }
        if (ins.flow == Flow::Call || ins.number) {
            for (Reg r : kVolatile) regs.at(r) = std::nullopt;
            return std::nullopt;
        }
        if (count == 2 && o0.type == Type::Reg) {
            const std::string dst = reg_name(o0.reg);
            if (ins.mnemonic == Mnemonic::Mov && o1.type == Type::Imm) {
                put(regs, dst, o1.imm & (o0.size == 1 ? 0xFF : 0xFFFF));
                return std::nullopt;
            }
            if (ins.mnemonic == Mnemonic::Mov && o1.type == Type::Reg) {
                put(regs, dst, get(regs, ins, reg_name(o1.reg)));
                return std::nullopt;
            }
            if (ins.mnemonic == Mnemonic::Lea && o1.base == 0 && o1.index == 0) {
                put(regs, dst, o1.disp & 0xFFFF);
                return std::nullopt;
            }
            if ((ins.mnemonic == Mnemonic::Xor || ins.mnemonic == Mnemonic::Sub) && o1.type == Type::Reg && o1.reg == o0.reg) {
                put(regs, dst, 0);
                return std::nullopt;
            }
        }
        if (ins.mnemonic == Mnemonic::Mov && count == 2 && o0.type == Type::Mem && o0.size == 2 && o0.base == 0 && o0.index == 0) {
            const std::optional<Reg> seg = o0.segment == X86_REG_ES ? std::optional<Reg>(Reg::Es)
                                           : o0.segment == 0       ? std::optional<Reg>(Reg::Ds) : std::nullopt;
            const std::int64_t at = o0.disp & 0xFFFF;
            if (seg && regs.at(*seg) == 0 && at < 0x400) {
                const std::optional<std::int64_t> value =
                    o1.type == Type::Imm ? std::optional<std::int64_t>(o1.imm & 0xFFFF) : get(regs, ins, reg_name(o1.reg));
                regs.table[at] = value;
                const auto before = regs.table.find(at - 2);
                if (at % 4 == 2 && before != regs.table.end() && before->second && value) {
                    return Vector{*value * 16 + *before->second, static_cast<std::uint8_t>(at / 4)};
                }
            }
            return std::nullopt;
        }
        forget_written(va, ins, regs);
        return std::nullopt;
    }

    // Whatever the instruction writes is no longer a known constant.
    void forget_written(Lin va, const Ins& ins, Regs& regs) {
        const std::size_t from = index(va);
        const ByteView raw(program_.image.data() + from, static_cast<std::size_t>(ins.size));
        decoder_.each(raw, static_cast<std::uint64_t>(va) & 0xFFFF, 1, [&](const cs_insn& got) {
            if (const auto access = decoder_.register_access(got)) {
                for (std::uint16_t r : access->written) {
                    std::string name = reg_name(r);
                    // eax and ax are one register here
                    if (name.size() == 3 && name[0] == 'e' && halves_of(std::string_view(name).substr(1))) name.erase(0, 1);
                    put(regs, name, std::nullopt);
                }
            }
            return false;
        });
    }

    // ---- the whole program ----------------------------------------------------------------------------------------------

    void drain() {
        while (!queue_.empty()) {
            while (!queue_.empty()) {
                const Lin f = queue_.back();
                queue_.pop_back();
                follow(f);
            }
            const std::map<Lin, std::uint8_t> installed = vectors_;   // handlers the code just read installs
            for (const auto& [va, number] : installed) {
                if (!functions_.contains(va) && inside(va)) {
                    const Lin cs = segment_of(va);
                    if (looks_like_code(va, cs)) {
                        add(va, Distance::Far, Evidence::Vector, cs, std::format("int_{:02X}_handler_{:05X}", number, va - base_));
                    }
                }
            }
        }
    }

    // The segment base code at `va` runs with: that of the nearest function before it.
    Lin segment_of(Lin va) const {
        auto it = functions_.upper_bound(va);
        if (it != functions_.begin()) {
            --it;
            const Lin cs = it->second.cs;
            if (va - cs < 0x10000) return cs;
        }
        return va & ~Lin{0xF};
    }

    // Function starts nothing was seen to call: far pointers in data, prologues in bytes nothing reaches, and what
    // stands behind a return.
    void guesses() {
        std::set<Lin> tried;
        for (;;) {
            const std::size_t before = functions_.size();
            std::set<Lin> bases;
            for (const auto& [va, f] : functions_) bases.insert(f.cs);
            for (const std::uint32_t site : program_.relocations) {   // far pointers in data
                const Lin i = static_cast<Lin>(site) - base_;
                // (a relocation site is a word of the image, in a span or not; one that is not is no far pointer)
                if (i < 2 || i + 2 > static_cast<Lin>(mark_.size())) continue;
                const std::size_t at = static_cast<std::size_t>(i);
                if (mark_[at] == 1 || mark_[at] == 2 || mark_[at - 2] == 1 || mark_[at - 2] == 2) continue;
                const Lin seg = word(site), off = word(Lin{site} - 2);
                const Lin t = seg * 16 + off;
                if (bases.contains(seg * 16) && inside(t) && !functions_.contains(t) && mark(t) == 0 && looks_like_code(t, seg * 16)) {
                    add(t, Distance::Far, Evidence::Pointer, seg * 16, {}, false);
                }
            }
            drain();
            for (const formats::dos::Span& span : program_.spans) {   // prologues in bytes nothing reaches
                const std::size_t lo = static_cast<std::size_t>(Lin{span.address} - base_);
                const std::size_t end = std::min(lo + span.length, program_.image.size());
                for (std::size_t j = lo; j < end; ++j) {
                    if (!starts_prologue(j, end)) continue;
                    const Lin va = base_ + static_cast<Lin>(j);
                    if (mark_[j] != 0 || (program_.image[j] == 0xC8 && !plain_enter(j))) continue;
                    const Lin cs = segment_of(va);
                    if (looks_like_code(va, cs)) {
                        add(va, Distance::Near, Evidence::Prologue, cs, {}, false);
                        drain();
                    }
                }
            }
            after_returns(tried);
            if (functions_.size() == before) break;
        }
    }

    // `push bp / mov bp,sp` (either encoding) or the `enter` opcode at image offset j, wholly before `end`.
    bool starts_prologue(std::size_t j, std::size_t end) const {
        const Bytes& image = program_.image;
        if (image[j] == 0xC8) return true;
        return j + 3 <= end && image[j] == 0x55 && ((image[j + 1] == 0x8B && image[j + 2] == 0xEC) || (image[j + 1] == 0x89 && image[j + 2] == 0xE5));
    }

    // `enter n, 0`: an even frame under 4 KB, no nesting.
    bool plain_enter(std::size_t j) const {
        const Bytes& image = program_.image;
        if (j + 3 >= image.size()) return false;
        return image[j + 3] == 0x00 && (image[j + 1] & 1) == 0 && image[j + 2] < 0x10;
    }

    // `follows`: what stands directly behind a return or jump of code already found.
    void after_returns(std::set<Lin>& tried) {
        for (;;) {
            std::vector<Lin> ends;
            for (const auto& [va, ins] : cache_) {
                if (ins && (ins->flow == Flow::Stop || ins->flow == Flow::Jump) && inside(va) && mark(va) == 1) ends.push_back(va + ins->size);
            }
            std::sort(ends.begin(), ends.end());
            bool found = false;
            for (Lin va : ends) {
                const Lin stop = va + 4;
                while (va < stop && inside(va) && mark(va) == 0 && (byte_at(va) == 0x90 || byte_at(va) == 0xCC || byte_at(va) == 0)) ++va;   // padding between routines
                if (tried.contains(va) || !inside(va) || mark(va) != 0 || functions_.contains(va)) continue;
                tried.insert(va);
                const Lin cs = segment_of(va);
                if (looks_like_code(va, cs, 1500, true)) {
                    add(va, Distance::Near, Evidence::Follows, cs, {}, false);
                    drain();
                    found = true;
                }
            }
            if (!found) break;
        }
    }

    // The interrupt calls inside the functions found, with the service number where a constant was loaded in the same
    // straight run of instructions. A run is cut at every address something jumps to, at every call and at every
    // function start, so a number is only reported where no other path can bring a different one.
    std::vector<Interrupt> name_interrupts() {
        std::unordered_map<Lin, Lin> before;   // an instruction -> the one that falls into it
        for (const auto& [va, ins] : cache_) {
            if (ins && ins->flow == Flow::Next && !ins->number && inside(va) && mark(va) == 1) before.emplace(va + ins->size, va);
        }
        std::vector<Interrupt> out;
        for (const auto& [va, number] : sites_) {
            std::vector<Lin> run;
            Lin a = va;
            while (!labels_.contains(a) && !functions_.contains(a) && before.contains(a) && run.size() < 64) {
                a = before.at(a);
                run.push_back(a);
            }
            Regs regs;
            for (auto b = run.rbegin(); b != run.rend(); ++b) constants(*b, *cache_.at(*b), regs);
            Interrupt it;
            it.address = static_cast<std::uint32_t>(va);
            it.number = number;
            if (regs.at(Reg::Ah)) it.ah = static_cast<std::uint8_t>(*regs.at(Reg::Ah));
            if (regs.at(Reg::Al)) it.al = static_cast<std::uint8_t>(*regs.at(Reg::Al));
            const auto owner = functions_.upper_bound(va);
            if (owner != functions_.begin()) it.function = static_cast<std::uint32_t>(std::prev(owner)->first);
            out.push_back(it);
        }
        return out;
    }

    Census census() const {
        Census out;
        for (const formats::dos::Span& span : program_.spans) {
            const std::size_t lo = static_cast<std::size_t>(Lin{span.address} - base_);
            for (std::size_t i = lo; i < lo + span.length && i < mark_.size(); ++i) {
                ++out.total;
                if (mark_[i] == 3) ++out.jump_tables;
                else if (mark_[i] == 0) ++out.not_classified;
                else if (guess_[i] != 0) ++out.guessed_code;
                else ++out.code;
            }
        }
        return out;
    }

    const formats::dos::Program& program_;
    const Lin base_;
    platforms::Decoder decoder_;
    std::unordered_map<Lin, std::optional<Ins>> cache_;     // by address: lookup only, and a scan whose result is sorted
    std::vector<std::uint8_t> mark_;                         // 1 instruction start, 2 inside one, 3 jump table
    std::vector<std::uint8_t> guess_;                        // reached only from a guessed function start
    std::unordered_set<Lin> labels_;                         // addresses jumped to
    std::map<Lin, std::uint8_t> sites_;                      // the address of an `int` -> its number
    std::unordered_set<std::uint32_t> relocs_;
    std::map<Lin, Function> functions_;                      // ordered: the nearest function before an address is asked for
    std::map<Lin, std::size_t> tables_;
    std::map<Lin, std::uint8_t> vectors_;                    // handler -> interrupt; drained in address order
    std::vector<Lin> queue_;                                 // function starts to follow (a stack: the last found is followed first)
};

}  // namespace

Analysis discover(const formats::dos::Program& program) { return Finder(program).run(); }

std::vector<std::string> report(std::string_view name, const formats::dos::Program& program, const Analysis& analysis, bool all,
                                const symbols::dos_interrupts::ServiceTable& table) {
    std::vector<std::string> out;
    std::uint64_t loaded = 0;
    for (const formats::dos::Span& s : program.spans) loaded += s.length;
    std::string stored;
    if (program.packer) {
        stored = std::format(", stored with {} ({})", formats::dos::packer_name(*program.packer),
                             program.unpacked ? "unpacked" : "NOT unpacked");
    }
    out.push_back(std::format("{}: {}{}, {} bytes loaded", name, formats::dos::kind_name(program.kind), stored, loaded));

    std::map<std::string_view, int> by;   // by the evidence's name, so the line lists them alphabetically
    std::size_t guesses = 0;
    for (const auto& [va, f] : analysis.functions) {
        ++by[name_of(f.evidence)];
        if (!f.sure) ++guesses;
    }
    std::vector<std::string> parts;
    for (const auto& [evidence, n] : by) parts.push_back(std::format("{} {}", n, evidence));
    out.push_back(std::format("{} functions: {}; {} of them rest on a guess", analysis.functions.size(), text::join(parts, ", "), guesses));

    const Census& c = analysis.census;
    out.push_back(std::format("bytes: code {}, guessed code {}, jump tables {}, not classified {}, total {}", c.code, c.guessed_code,
                              c.jump_tables, c.not_classified, c.total));

    std::size_t named = 0;
    for (const Interrupt& i : analysis.interrupts) {
        if (table.name(i.number, i.ah, i.al).exact) ++named;
    }
    out.push_back(std::format("{} interrupt calls, {} with the service named", analysis.interrupts.size(), named));
    for (const std::string& note : program.notes) out.push_back("note: " + note);

    if (all) {
        for (const auto& [va, f] : analysis.functions) {
            out.push_back(std::format("  {:05X} {:<26} {:<4} removes {:2}  {:5} bytes  {}{}", va, f.name, name_of(f.distance), f.removes,
                                      f.size, name_of(f.evidence), f.sure ? "" : " (guess)"));
        }
    }
    return out;
}

}  // namespace cb::analyze::dos_functions
