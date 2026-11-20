// emulate/shadow.cpp -- the PowerPC run's symbolic shadow (see shadow.hpp).
// cb-unity: alone
//
// One instruction at a time: its p-code is read from the lifter, then every operation is evaluated on pairs (concrete value, symbolic
// value or nothing). The concrete half comes from the interpreter's registers and memory as they are before the instruction; the
// symbolic half only from the bytes the run already made from an input. A register byte or memory byte that holds an expression is
// kept (as byte k of that expression); one the instruction wrote from concrete values is forgotten.
#include "emulate/shadow.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <format>
#include <map>

#include "core/error.hpp"
#include "core/text.hpp"

namespace cb::emulate::shadow {

namespace {

using native_shadow::PathEntry;
using native_shadow::PathKind;
using native_shadow::Step;
using native_shadow::Var;
using native_shadow::VarKind;
using pcode::Input;
using pcode::Space;
using pcode::Varnode;
using symbolic::Expr;
using SymOp = symbolic::Op;

constexpr std::uint64_t kAddressMask = 0xFFFFFFFFull;   // the interpreter's addresses are 32 bits wide
constexpr std::uint32_t kPageSize = powerpc::kPageSize;

// A register or memory byte's symbolic holder: byte `k` of `expr`.
struct ByteOf {
    const Expr* expr = nullptr;
    std::uint8_t k = 0;
};

// A value of an operation: what it is now, and the expression it is made from the inputs by (none: made of constants).
struct Value {
    std::uint64_t c = 0;
    const Expr* s = nullptr;
};

// A byte an instruction has written but the run has not seen yet.
struct LocalByte {
    std::uint8_t c = 0;
    std::optional<ByteOf> e;
};

// A p-code temporary.
struct Unique {
    std::uint32_t size = 0;
    std::uint64_t c = 0;
    const Expr* s = nullptr;
};

// The registers of the interpreter the shadow can read, by what they are.
enum class RegClass : std::uint8_t { Gpr, Fpr, CrField, Lr, Ctr, Xer, XerCa, Never };
struct KnownReg {
    RegClass cls = RegClass::Never;
    std::uint8_t index = 0;
};

// r0-r31, f0-f31, cr0-cr7, LR, CTR, XER, xer_ca, and xer_so / xer_ov (the interpreter never sets those two: they read 0).
std::optional<KnownReg> shadow_classify_register(std::string_view name) {
    if (name == "LR") return KnownReg{RegClass::Lr, 0};
    if (name == "CTR") return KnownReg{RegClass::Ctr, 0};
    if (name == "XER") return KnownReg{RegClass::Xer, 0};
    if (name == "xer_ca") return KnownReg{RegClass::XerCa, 0};
    if (name == "xer_so" || name == "xer_ov") return KnownReg{RegClass::Never, 0};
    auto numbered = [&](std::string_view prefix, unsigned limit, RegClass cls) -> std::optional<KnownReg> {
        if (!name.starts_with(prefix)) return std::nullopt;
        const std::string_view digits = name.substr(prefix.size());
        if (digits.empty() || (digits.size() > 1 && digits.front() == '0')) return std::nullopt;
        const auto n = text::parse_int<unsigned>(digits, 10);
        if (!n || *n >= limit) return std::nullopt;
        return KnownReg{cls, static_cast<std::uint8_t>(*n)};
    };
    if (auto r = numbered("cr", 8, RegClass::CrField)) return r;
    if (auto r = numbered("r", 32, RegClass::Gpr)) return r;
    return numbered("f", 32, RegClass::Fpr);
}

std::uint64_t shadow_register_value(const powerpc::Machine& m, KnownReg reg) {
    switch (reg.cls) {
        case RegClass::Gpr: return m.r[reg.index];
        case RegClass::Fpr: return m.f[reg.index];
        case RegClass::CrField: return (static_cast<std::uint64_t>(m.cr) >> (28 - 4 * reg.index)) & 0xF;
        case RegClass::Lr: return m.lr;
        case RegClass::Ctr: return m.ctr;
        case RegClass::Xer: return static_cast<std::uint64_t>(m.ca) << 29;
        case RegClass::XerCa: return m.ca ? 1 : 0;
        case RegClass::Never: return 0;
    }
    return 0;
}

// Something the shadow cannot follow (a value wider than its 64 bits, a p-code construct it has no meaning for): it ends the shadow, never
// the run.
class ShadowLimit final : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(ShadowLimit)
};

// A byte of the interpreter's memory as the run sees it, without making the page (a page not made reads as zeros).
std::uint8_t shadow_peek_byte(const powerpc::Machine& m, std::uint64_t address) {
    const std::uint32_t at = static_cast<std::uint32_t>(address & kAddressMask);
    const powerpc::Page* page = m.peek(at);
    return page != nullptr ? (*page)[at & (kPageSize - 1)] : std::uint8_t{0};
}

// ---- operations --------------------------------------------------------------------------------------------------------------

enum class K : std::uint8_t {
    Copy, Load, Store, CallOther, CBranch, Branch, BranchInd, Call,
    IntAdd, IntSub, IntAnd, IntOr, IntXor, IntMult, IntDiv, IntRem, IntSDiv, IntSRem,
    IntEqual, IntNotEqual, IntLess, IntLessEqual, IntSLess, IntSLessEqual,
    IntZext, IntSext, IntCarry, IntSCarry, IntSBorrow, Int2Comp, IntNegate, IntLeft, IntRight, IntSRight,
    BoolNegate, BoolAnd, BoolOr, BoolXor, Piece, Subpiece, PopCount, LzCount,
    Other,
};

K shadow_kind_of(std::string_view opcode) {
    static const std::unordered_map<std::string_view, K> kTable = {
        {"COPY", K::Copy}, {"LOAD", K::Load}, {"STORE", K::Store}, {"CALLOTHER", K::CallOther}, {"CBRANCH", K::CBranch},
        {"BRANCH", K::Branch}, {"BRANCHIND", K::BranchInd}, {"CALLIND", K::BranchInd}, {"RETURN", K::BranchInd}, {"CALL", K::Call},
        {"INT_ADD", K::IntAdd}, {"INT_SUB", K::IntSub}, {"INT_AND", K::IntAnd}, {"INT_OR", K::IntOr}, {"INT_XOR", K::IntXor},
        {"INT_MULT", K::IntMult}, {"INT_DIV", K::IntDiv}, {"INT_REM", K::IntRem}, {"INT_SDIV", K::IntSDiv}, {"INT_SREM", K::IntSRem},
        {"INT_EQUAL", K::IntEqual}, {"INT_NOTEQUAL", K::IntNotEqual}, {"INT_LESS", K::IntLess}, {"INT_LESSEQUAL", K::IntLessEqual},
        {"INT_SLESS", K::IntSLess}, {"INT_SLESSEQUAL", K::IntSLessEqual}, {"INT_ZEXT", K::IntZext}, {"INT_SEXT", K::IntSext},
        {"INT_CARRY", K::IntCarry}, {"INT_SCARRY", K::IntSCarry}, {"INT_SBORROW", K::IntSBorrow}, {"INT_2COMP", K::Int2Comp},
        {"INT_NEGATE", K::IntNegate}, {"INT_LEFT", K::IntLeft}, {"INT_RIGHT", K::IntRight}, {"INT_SRIGHT", K::IntSRight},
        {"BOOL_NEGATE", K::BoolNegate}, {"BOOL_AND", K::BoolAnd}, {"BOOL_OR", K::BoolOr}, {"BOOL_XOR", K::BoolXor},
        {"PIECE", K::Piece}, {"SUBPIECE", K::Subpiece}, {"POPCOUNT", K::PopCount}, {"LZCOUNT", K::LzCount},
    };
    const auto it = kTable.find(opcode);
    return it == kTable.end() ? K::Other : it->second;
}

// A value of `size` bytes can be followed only up to the 64 bits a concrete half holds.
std::uint64_t shadow_mask_of(std::uint32_t size) {
    if (size > 8) throw ShadowLimit("shadow: a value wider than 8 bytes");
    return size == 8 ? ~0ull : ((1ull << (8 * size)) - 1);
}

// `v` as a signed number of `size` bytes.
std::int64_t shadow_sign_extended(std::uint64_t v, std::uint32_t size) {
    if (size == 0 || size > 8) throw ShadowLimit("shadow: a value wider than 8 bytes");
    const unsigned bits = 8 * size;
    if (bits == 64) return static_cast<std::int64_t>(v);
    const std::uint64_t sign = 1ull << (bits - 1);
    return (v & sign) != 0 ? static_cast<std::int64_t>(v) - (std::int64_t{1} << bits) : static_cast<std::int64_t>(v);
}

// The comparison `how` of two bit-vectors of a p-code compare, as a p-code boolean byte (flipped for "not equal").
struct CompareSpec {
    SymOp op;
    bool flip;
};
std::optional<CompareSpec> shadow_compare_of(K k) {
    switch (k) {
        case K::IntEqual: return CompareSpec{SymOp::Eq, false};
        case K::IntNotEqual: return CompareSpec{SymOp::Eq, true};
        case K::IntLess: return CompareSpec{SymOp::ULt, false};
        case K::IntLessEqual: return CompareSpec{SymOp::ULe, false};
        case K::IntSLess: return CompareSpec{SymOp::SLt, false};
        case K::IntSLessEqual: return CompareSpec{SymOp::SLe, false};
        default: return std::nullopt;
    }
}

std::optional<SymOp> shadow_binary_of(K k) {
    switch (k) {
        case K::IntAdd: return SymOp::Add;
        case K::IntSub: return SymOp::Sub;
        case K::IntAnd: return SymOp::And;
        case K::IntOr: return SymOp::Or;
        case K::IntXor: return SymOp::Xor;
        case K::IntMult: return SymOp::Mul;
        case K::IntDiv: return SymOp::UDiv;
        case K::IntRem: return SymOp::URem;
        case K::IntSDiv: return SymOp::SDiv;
        case K::IntSRem: return SymOp::SRem;
        default: return std::nullopt;
    }
}

}  // namespace

// ---- module functions -------------------------------------------------------------------------------------------------------

std::optional<pcode::Mode> lift_mode(std::string_view cpu_language) {
    if (cpu_language.starts_with("x86:LE:16")) return pcode::Mode::X86Real16;
    if (cpu_language.starts_with("x86:LE:32")) return pcode::Mode::X86Flat32;
    if (cpu_language.starts_with("PowerPC:BE")) return pcode::Mode::PpcBe64;
    return std::nullopt;
}

// ---- the shadow -------------------------------------------------------------------------------------------------------------

struct PowerPcShadow::Impl {
    Impl(pcode::Lifter& lifter_, InputFn is_input_, native_shadow::Run& run_)
        : lifter(lifter_), is_input(std::move(is_input_)), run(run_) {
        // Each byte of the lifter's register file that is a register the interpreter has: which register, and the byte's shift in it
        // (big-endian: the first byte is the most significant).
        for (const auto& [name, slot] : lifter.registers()) {
            if (const auto kr = shadow_classify_register(name)) {
                const std::uint16_t id = static_cast<std::uint16_t>(known.size());
                known.push_back(*kr);
                for (std::uint32_t k = 0; k < slot.size; ++k) {
                    places.emplace(slot.offset + k, RegByte{id, static_cast<std::uint8_t>(8 * (slot.size - 1 - k))});
                }
            }
        }
        slot_value.assign(known.size(), 0);
        slot_stamp.assign(known.size(), 0);
    }

    struct RegByte {
        std::uint16_t id = 0;
        std::uint8_t shift = 0;
    };
    struct Pending {   // a conditional jump awaiting its outcome
        std::uint64_t site = 0, target = 0, fall = 0;
        const Expr* entry = nullptr;   // the condition it makes in the path (none: it was made of constants)
        bool guess = false;            // the direction the specification says it goes
    };

    pcode::Lifter& lifter;
    InputFn is_input;
    native_shadow::Run& run;
    std::vector<KnownReg> known;
    std::unordered_map<std::uint64_t, RegByte> places;           // register-file byte -> its register and shift
    std::vector<std::uint64_t> slot_value;                       // each register's value, as this instruction first read it
    std::vector<std::uint32_t> slot_stamp;
    std::uint32_t stamp = 0;
    std::unordered_map<std::uint64_t, ByteOf> reg;               // register byte -> the expression it is byte k of
    std::unordered_map<std::uint64_t, ByteOf> mem;               // memory byte -> the same
    std::unordered_set<std::uint64_t> touched;                   // bytes the run wrote (no longer inputs)
    std::unordered_map<std::string, const Expr*> input_expr;     // an input's name -> its expression
    std::optional<Pending> pending;
    std::unordered_map<std::uint64_t, const pcode::Lifted*> lifted;   // (word, address) -> its p-code (null: does not decode)
    // scratch of the instruction being followed
    std::unordered_map<std::uint64_t, LocalByte> local_reg, local_mem;
    std::unordered_map<std::uint64_t, Unique> uniq;

    symbolic::ExprPool& pool() { return run.pool; }

    // The register `name` of the lifter's file: its offset and size.
    pcode::RegisterSlot slot_of(std::string_view name) const {
        const auto* found = lifter.registers().find(std::string(name));
        if (found == nullptr) throw UsageError(std::format("shadow: the lifter has no register {}", name));
        return *found;
    }

    // The expression of the input `name` (made on first use).
    const Expr* input(const std::string& name, std::uint32_t bits, VarKind kind, std::variant<std::monostate, std::uint64_t, std::string> where,
                      std::uint64_t value) {
        const auto it = input_expr.find(name);
        if (it != input_expr.end()) return it->second;
        run.vars.insert_or_assign(name, Var{kind, std::move(where), bits, value});
        return input_expr.emplace(name, pool().var(name, bits)).first->second;
    }

    const Expr* fit(const Expr* e, std::uint32_t bits) {
        if (e->bits == bits) return e;
        return e->bits > bits ? pool().extract(e, 0, bits) : pool().zext(e, bits);
    }

    // A value of `size` bytes from its bytes (least significant first): each byte k of an expression, or nothing for a concrete byte (of
    // `conc`). All the bytes of one expression in order: that expression.
    const Expr* join(const std::array<std::optional<ByteOf>, 8>& entries, std::uint64_t conc, std::uint32_t size) {
        const std::optional<ByteOf>& first = entries[0];
        bool all = first.has_value();
        for (std::uint32_t k = 0; all && k < size; ++k) {
            const auto& e = entries[k];
            all = e.has_value() && e->expr == first->expr && e->k == first->k + k;
        }
        if (all) return pool().extract(first->expr, 8 * first->k, 8 * size);
        const Expr* out = nullptr;
        for (std::uint32_t k = 0; k < size; ++k) {
            const auto& e = entries[k];
            const Expr* byte = e ? pool().extract(e->expr, 8u * e->k, 8) : pool().constant((conc >> (8 * k)) & 0xFF, 8);
            out = out == nullptr ? byte : pool().concat(byte, out);
        }
        return out;
    }

    const Expr* bit_vector(const Value& v, std::uint32_t size) { return v.s != nullptr ? v.s : pool().constant(v.c, 8 * size); }

    std::uint64_t register_byte(const powerpc::Machine& m, std::uint64_t offset) {
        const auto it = places.find(offset);
        if (it == places.end()) return 0;
        const RegByte& rb = it->second;
        if (slot_stamp[rb.id] != stamp) {
            slot_value[rb.id] = shadow_register_value(m, known[rb.id]);
            slot_stamp[rb.id] = stamp;
        }
        return rb.shift >= 64 ? 0 : (slot_value[rb.id] >> rb.shift) & 0xFF;
    }

    // ---- one instruction -------------------------------------------------------------------------------------------------

    void step(powerpc::Machine& m, std::uint32_t address);
    void run_instruction(powerpc::Machine& m, std::uint64_t address, const pcode::Lifted& ins);
    Value arithmetic(K kind, std::string_view name, std::uint32_t out_size, std::span<const Value> vals, std::span<const std::uint32_t> sizes);
    void forget_register(const pcode::RegisterSlot& slot) {
        for (std::uint32_t b = 0; b < slot.size; ++b) reg.erase(slot.offset + b);
    }
    void lose(std::string why) {
        run.lost.insert(std::move(why));
        run.off = true;
    }
};

PowerPcShadow::PowerPcShadow(pcode::Lifter& lifter, InputFn is_input) : run_(std::make_shared<native_shadow::Run>()) {
    if (lifter.mode() != pcode::Mode::PpcBe64) throw UsageError("PowerPcShadow needs the lifter of the ppc-be-64 mode");
    impl_ = std::make_unique<Impl>(lifter, std::move(is_input), *run_);
}

PowerPcShadow::~PowerPcShadow() = default;

void PowerPcShadow::enter(std::span<const native_shadow::Place> places, int bits) {
    Impl& s = *impl_;
    const std::uint32_t nbytes = static_cast<std::uint32_t>(bits / 8);
    std::int64_t position = 0;
    for (const native_shadow::Place& item : places) {
        const std::variant<std::int64_t, std::string> k = item.input ? *item.input : std::variant<std::int64_t, std::string>(position);
        ++position;
        const Expr* value = nullptr;
        if (const auto* number = std::get_if<std::int64_t>(&k)) {
            value = s.input(std::format("a{}", *number), static_cast<std::uint32_t>(bits), VarKind::Arg, static_cast<std::uint64_t>(*number), item.value);
        } else {
            const std::string& name = std::get<std::string>(k);
            value = s.input(name, static_cast<std::uint32_t>(bits), VarKind::Reg, name, item.value);
        }
        if (const auto* register_name = std::get_if<std::string>(&item.where)) {
            const pcode::RegisterSlot slot = s.slot_of(*register_name);
            for (std::uint32_t j = 0; j < slot.size; ++j) {   // (the register's bytes, least significant first: the last is the first)
                const std::uint64_t b = slot.offset + (slot.size - 1 - j);
                if (j < nbytes) s.reg[b] = ByteOf{value, static_cast<std::uint8_t>(j)};
            }
        } else {
            const std::uint64_t address = std::get<std::uint64_t>(item.where);
            for (std::uint32_t j = 0; j < nbytes; ++j) s.mem[address + (nbytes - 1 - j)] = ByteOf{value, static_cast<std::uint8_t>(j)};
        }
    }
}

void PowerPcShadow::forget(std::span<const std::string> names) {
    Impl& s = *impl_;
    if (s.run.off) return;
    for (const std::string& name : names) {
        if (const auto* slot = s.lifter.registers().find(name)) s.forget_register(*slot);
    }
}

void PowerPcShadow::written(std::uint64_t at, std::uint64_t size) {
    Impl& s = *impl_;
    for (std::uint64_t b = at; b < at + size; ++b) {
        s.mem.erase(b);
        s.touched.insert(b);
    }
}

// What the interpreter does at a call out of the code (the function's own): it does not run it; r3 is its result.
void PowerPcShadow::after_call(powerpc::Machine&, bool symbolic_result, std::uint64_t ret) {
    Impl& s = *impl_;
    s.pending.reset();
    if (s.run.off) return;
    try {
        const pcode::RegisterSlot r3 = s.slot_of("r3");
        s.forget_register(r3);
        if (symbolic_result) {
            const Expr* r = s.input("ret", 64, VarKind::Ret, std::monostate{}, ret);
            for (std::uint32_t k = 0; k < 8 && k < r3.size; ++k) s.reg[r3.offset + (r3.size - 1 - k)] = ByteOf{r, static_cast<std::uint8_t>(k)};
        }
    } catch (const Error& e) {
        s.lose(std::format("shadow: {}", e.what()));
    }
}

void PowerPcShadow::step(powerpc::Machine& m, std::uint32_t pc) {
    Impl& s = *impl_;
    if (s.run.off) return;
    try {
        s.step(m, pc);
    } catch (const Error& e) {
        s.lose(std::format("shadow: {}", e.what()));
    }
}

// ---- one instruction ---------------------------------------------------------------------------------------------------------

void PowerPcShadow::Impl::step(powerpc::Machine& m, std::uint32_t address) {
    if (pending) {
        const Pending pend = *pending;
        pending.reset();
        if (address == pend.target || address == pend.fall) {
            const bool went = address == pend.target;
            run.trail.push_back(Step{pend.site, went});
            if (pend.entry != nullptr) {
                if (went != pend.guess) run.lost.insert("disagree");   // (the specification and the processor differ: the processor's way is kept)
                run.path.push_back(PathEntry{pend.site, went, pend.entry, PathKind::Jump});
            }
        }
    }
    ++run.steps;
    ++stamp;
    std::uint32_t word = 0;   // the instruction, big-endian
    for (unsigned i = 0; i < 4; ++i) word = (word << 8) | shadow_peek_byte(m, std::uint64_t{address} + i);
    const std::uint64_t key = (std::uint64_t{word} << 32) | address;
    const pcode::Lifted* ins = nullptr;
    if (const auto it = lifted.find(key); it != lifted.end()) {
        ins = it->second;
    } else {
        const std::uint8_t raw[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8),
                                     static_cast<std::uint8_t>(word)};
        ins = lifter.lift(ByteView(raw, 4), address);
        lifted.emplace(key, ins);
    }
    if (ins == nullptr) {   // an instruction not followed: what it writes is no longer known to be made from anything
        run.lost.insert("undecoded");
        reg.clear();
        return;
    }
    bool reads_hit = false;
    if (!reg.empty()) {
        for (const std::uint64_t b : ins->reads) {
            if (reg.contains(b)) {
                reads_hit = true;
                break;
            }
        }
    }
    if (!ins->full && !reads_hit) {   // nothing made from an input is read: only what it writes stops being one
        if (!reg.empty()) {
            for (const std::uint64_t b : ins->writes) reg.erase(b);
        }
        return;
    }
    run_instruction(m, address, *ins);
    if (run.path.size() >= kPathCap) {
        run.lost.insert("path-capped");
        run.off = true;
    }
}

void PowerPcShadow::Impl::run_instruction(powerpc::Machine& m, std::uint64_t address, const pcode::Lifted& ins) {
    local_reg.clear();
    local_mem.clear();
    uniq.clear();
    const std::uint32_t fall_through = static_cast<std::uint32_t>(address + ins.length);

    // The bytes of a value at `at`, least significant first: big-endian, so the last byte is the first.
    auto byte_at = [](std::uint64_t at, std::uint32_t size, std::uint32_t k) { return at + (size - 1 - k); };

    auto load = [&](std::uint64_t at, std::uint32_t size) -> Value {
        if (size == 0 || size > 8) throw ShadowLimit("shadow: a value wider than 8 bytes");
        std::uint64_t c = 0;
        for (std::uint32_t i = 0; i < size; ++i) c = (c << 8) | shadow_peek_byte(m, at + i);
        std::array<std::optional<ByteOf>, 8> entries;
        bool any = false;
        for (std::uint32_t k = 0; k < size; ++k) {
            const std::uint64_t b = byte_at(at, size, k);
            std::optional<ByteOf> e;
            if (const auto lm = local_mem.find(b); lm != local_mem.end()) {
                c = (c & ~(0xFFull << (8 * k))) | (std::uint64_t{lm->second.c} << (8 * k));
                e = lm->second.e;
            } else if (const auto it = mem.find(b); it != mem.end()) {
                e = it->second;
            } else if (!touched.contains(b) && is_input(b)) {
                e = ByteOf{input(std::format("m{:x}", b), 8, VarKind::Mem, b, shadow_peek_byte(m, b)), 0};
                mem[b] = *e;
            }
            entries[k] = e;
            any = any || e.has_value();
        }
        return {c, any ? join(entries, c, size) : nullptr};
    };

    auto store = [&](std::uint64_t at, std::uint32_t size, std::uint64_t c, const Expr* s) {
        for (std::uint32_t k = 0; k < size; ++k) {
            local_mem[byte_at(at, size, k)] =
                LocalByte{static_cast<std::uint8_t>(k < 8 ? (c >> (8 * k)) & 0xFF : 0), s != nullptr ? std::optional<ByteOf>(ByteOf{s, static_cast<std::uint8_t>(k)}) : std::nullopt};
        }
    };

    auto read = [&](const Input& in) -> Value {
        const Varnode* vn = std::get_if<Varnode>(&in);
        if (vn == nullptr) throw ShadowLimit("shadow: an operand that is not a varnode");
        switch (vn->space) {
            case Space::Constant: return {vn->offset & shadow_mask_of(vn->size), nullptr};
            case Space::Unique: {
                const auto got = uniq.find(vn->offset);
                if (got == uniq.end()) return {0, nullptr};
                if (got->second.size == vn->size) return {got->second.c, got->second.s};
                if (got->second.size > vn->size)   // (the low part of a wider one)
                    return {got->second.c & shadow_mask_of(vn->size), got->second.s != nullptr ? pool().extract(got->second.s, 0, 8 * vn->size) : nullptr};
                return {0, nullptr};
            }
            case Space::Register: {
                if (vn->size > 8) throw ShadowLimit("shadow: a value wider than 8 bytes");
                std::uint64_t c = 0;
                std::array<std::optional<ByteOf>, 8> entries;
                bool any = false;
                for (std::uint32_t k = 0; k < vn->size; ++k) {
                    const std::uint64_t b = byte_at(vn->offset, vn->size, k);
                    std::uint8_t cb = 0;
                    std::optional<ByteOf> e;
                    if (const auto lr = local_reg.find(b); lr != local_reg.end()) {
                        cb = lr->second.c;
                        e = lr->second.e;
                    } else {
                        cb = static_cast<std::uint8_t>(register_byte(m, b));
                        if (const auto it = reg.find(b); it != reg.end()) e = it->second;
                    }
                    c |= std::uint64_t{cb} << (8 * k);
                    entries[k] = e;
                    any = any || e.has_value();
                }
                return {c, any ? join(entries, c, vn->size) : nullptr};
            }
            case Space::Ram: return load(vn->offset, vn->size);   // (ram named directly)
        }
        return {};
    };

    auto write = [&](const Varnode& vn, std::uint64_t c, const Expr* s) {
        c &= shadow_mask_of(vn.size);
        switch (vn.space) {
            case Space::Unique: uniq[vn.offset] = Unique{vn.size, c, s}; break;
            case Space::Register:
                for (std::uint32_t k = 0; k < vn.size; ++k) {
                    local_reg[byte_at(vn.offset, vn.size, k)] =
                        LocalByte{static_cast<std::uint8_t>(k < 8 ? (c >> (8 * k)) & 0xFF : 0), s != nullptr ? std::optional<ByteOf>(ByteOf{s, static_cast<std::uint8_t>(k)}) : std::nullopt};
                }
                break;
            case Space::Ram: store(vn.offset, vn.size, c, s); break;
            case Space::Constant: break;
        }
    };

    // An address (or any value) held at the value it had: the pointer as given.
    auto pin = [&](std::uint64_t c, const Expr* s) {
        if (s == nullptr) return;
        run.path.push_back(PathEntry{address, true, pool().compare(SymOp::Eq, s, pool().constant(c, s->bits)), PathKind::Pin});
        ++run.pins;
    };

    auto operand = [](const std::vector<Input>& inputs, std::size_t k) -> const Input& {
        if (k >= inputs.size()) throw ShadowLimit("shadow: an operation with too few inputs");
        return inputs[k];
    };
    auto varnode_of = [](const Input& in) -> const Varnode& {
        const Varnode* vn = std::get_if<Varnode>(&in);
        if (vn == nullptr) throw ShadowLimit("shadow: an operand that is not a varnode");
        return *vn;
    };

    const auto& ops = ins.ops;
    std::int64_t i = 0;
    const std::int64_t n = static_cast<std::int64_t>(ops.size());
    bool done = false;
    while (i < n && !done) {
        const pcode::Op& op = ops[static_cast<std::size_t>(i)];
        ++i;
        const auto& args = op.inputs;
        const K kind = shadow_kind_of(op.opcode);
        switch (kind) {
            case K::Copy: {
                const Value v = read(operand(args, 0));
                if (op.out) write(*op.out, v.c, v.s);
                break;
            }
            case K::Load: {
                const Value a = read(operand(args, 1));
                pin(a.c, a.s);
                if (op.out) {
                    const Value v = load(a.c & kAddressMask, op.out->size);
                    write(*op.out, v.c, v.s);
                }
                break;
            }
            case K::Store: {
                const Value a = read(operand(args, 1));
                pin(a.c, a.s);
                const Value v = read(operand(args, 2));
                const std::uint32_t size = varnode_of(operand(args, 2)).size;
                store(a.c & kAddressMask, size, v.c & shadow_mask_of(size), v.s);
                break;
            }
            case K::CallOther: {
                // A user operation: followed no further. Made from an input, it leaves the path's conditions possibly short.
                bool symbolic_input = false;
                for (std::size_t a = 1; a < args.size(); ++a) {
                    if (std::holds_alternative<Varnode>(args[a]) && read(args[a]).s != nullptr) symbolic_input = true;
                }
                if (symbolic_input) {
                    const std::string* name = args.empty() ? nullptr : std::get_if<std::string>(&args[0]);
                    run.lost.insert(std::format("userop:{}", name != nullptr ? *name : std::string()));
                }
                if (op.out) write(*op.out, 0, nullptr);
                break;
            }
            case K::CBranch: {
                const Value cond = read(operand(args, 1));
                const bool taken = cond.c != 0;
                const Varnode& target = varnode_of(operand(args, 0));
                if (target.space == Space::Constant) {   // a branch inside the instruction's p-code
                    if (cond.s != nullptr) run.path.push_back(PathEntry{address, taken, pool().nonzero(cond.s), PathKind::Inner});
                    if (taken) {
                        i = i - 1 + shadow_sign_extended(target.offset, target.size);
                        if (i < 0 || i > n) throw ShadowLimit("shadow: a branch out of the p-code");
                    }
                } else {
                    const std::uint64_t destination = target.offset & kAddressMask;
                    if (destination == fall_through || destination == address) {
                        // (a string instruction's count run out, or its compare: its loop is the instruction's own)
                        if (cond.s != nullptr) run.path.push_back(PathEntry{address, taken, pool().nonzero(cond.s), PathKind::Inner});
                    } else {   // the instruction's jump: which way it went is the next step's to say
                        pending = Pending{address, destination, fall_through, cond.s != nullptr ? pool().nonzero(cond.s) : nullptr, taken};
                    }
                    if (taken) done = true;
                }
                break;
            }
            case K::Branch: {
                const Varnode& target = varnode_of(operand(args, 0));
                if (target.space == Space::Constant) {
                    i = i - 1 + shadow_sign_extended(target.offset, target.size);
                    if (i < 0 || i > n) throw ShadowLimit("shadow: a branch out of the p-code");
                } else {
                    done = true;
                }
                break;
            }
            case K::BranchInd: {
                const Value v = read(operand(args, 0));
                pin(v.c, v.s);
                done = true;
                break;
            }
            case K::Call: done = true; break;
            default: {
                std::array<Value, 3> vals;
                std::array<std::uint32_t, 3> sizes{};
                if (args.size() > 3) throw ShadowLimit("shadow: an operation of more than three inputs");
                for (std::size_t a = 0; a < args.size(); ++a) {
                    vals[a] = read(args[a]);
                    sizes[a] = varnode_of(args[a]).size;
                }
                const Value out = arithmetic(kind, op.opcode, op.out ? op.out->size : 0, std::span<const Value>(vals.data(), args.size()),
                                             std::span<const std::uint32_t>(sizes.data(), args.size()));
                if (op.out) write(*op.out, out.c, out.s);
                break;
            }
        }
    }
    // what the instruction left: kept where it is made from an input, forgotten elsewhere
    for (const auto& [b, v] : local_reg) {
        if (v.e) reg[b] = *v.e;
        else reg.erase(b);
    }
    for (const auto& [b, v] : local_mem) {
        touched.insert(b);
        if (v.e) mem[b] = *v.e;
        else mem.erase(b);
    }
}

// One p-code operation of values: the concrete result, and the expression of it when an input is in it. What is not followed (floating
// point, and the rest) is nothing, and a loss when an input was in it.
Value PowerPcShadow::Impl::arithmetic(K kind, std::string_view name, std::uint32_t out_size, std::span<const Value> vals, std::span<const std::uint32_t> sizes) {
    if (vals.empty()) throw ShadowLimit("shadow: an operation without inputs");
    const std::uint64_t ac = vals[0].c;
    const Expr* const a_s = vals[0].s;
    const std::uint32_t asize = sizes[0];
    const std::uint64_t m = out_size != 0 ? shadow_mask_of(out_size) : 0;
    bool sym = false;
    for (const Value& v : vals) sym = sym || v.s != nullptr;
    const bool binary = vals.size() > 1;
    const std::uint64_t bc = binary ? vals[1].c : 0;
    const Expr* const b_s = binary ? vals[1].s : nullptr;
    const std::uint32_t bsize = binary ? sizes[1] : 0;

    // ---- the concrete half
    std::uint64_t c = 0;
    // signed division and remainder by magnitudes, so the extreme value does not overflow
    auto signed_divide = [&](std::int64_t x, std::int64_t y, bool remainder) -> std::uint64_t {
        const std::uint64_t ux = x < 0 ? 0 - static_cast<std::uint64_t>(x) : static_cast<std::uint64_t>(x);
        const std::uint64_t uy = y < 0 ? 0 - static_cast<std::uint64_t>(y) : static_cast<std::uint64_t>(y);
        if (remainder) {
            const std::uint64_t r = ux % uy;
            return (x >= 0 ? r : 0 - r) & m;
        }
        const std::uint64_t q = ux / uy;
        return ((x < 0) == (y < 0) ? q : 0 - q) & m;
    };
    switch (kind) {
        case K::IntAdd: c = (ac + bc) & m; break;
        case K::IntSub: c = (ac - bc) & m; break;
        case K::IntAnd: c = ac & bc; break;
        case K::IntOr: c = ac | bc; break;
        case K::IntXor: c = ac ^ bc; break;
        case K::IntMult: c = (ac * bc) & m; break;
        case K::IntDiv: c = bc != 0 ? (ac / bc) & m : 0; break;
        case K::IntRem: c = bc != 0 ? (ac % bc) & m : 0; break;
        case K::IntSDiv: {
            const std::int64_t y = shadow_sign_extended(bc, bsize);
            c = y != 0 ? signed_divide(shadow_sign_extended(ac, asize), y, false) : 0;
            break;
        }
        case K::IntSRem: {
            const std::int64_t y = shadow_sign_extended(bc, bsize);
            c = y != 0 ? signed_divide(shadow_sign_extended(ac, asize), y, true) : 0;
            break;
        }
        case K::IntEqual: c = ac == bc; break;
        case K::IntNotEqual: c = ac != bc; break;
        case K::IntLess: c = ac < bc; break;
        case K::IntLessEqual: c = ac <= bc; break;
        case K::IntSLess: c = shadow_sign_extended(ac, asize) < shadow_sign_extended(bc, bsize); break;
        case K::IntSLessEqual: c = shadow_sign_extended(ac, asize) <= shadow_sign_extended(bc, bsize); break;
        case K::IntZext: c = ac; break;
        case K::IntSext: c = static_cast<std::uint64_t>(shadow_sign_extended(ac, asize)) & m; break;
        case K::IntCarry: {
            const std::uint64_t sum = ac + bc;
            c = (sum < ac) || sum > shadow_mask_of(asize);
            break;
        }
        case K::IntSCarry:
        case K::IntSBorrow: {
            const bool carry = kind == K::IntSCarry;
            const std::int64_t x = shadow_sign_extended(ac, asize), y = shadow_sign_extended(bc, asize);
            bool overflow;
            if (asize >= 8) {   // (64 bits: the signed sum or difference wraps exactly when the operands' signs say so)
                const std::int64_t r = static_cast<std::int64_t>(carry ? static_cast<std::uint64_t>(x) + static_cast<std::uint64_t>(y)
                                                                       : static_cast<std::uint64_t>(x) - static_cast<std::uint64_t>(y));
                overflow = carry ? ((x >= 0) == (y >= 0) && (r >= 0) != (x >= 0)) : ((x >= 0) != (y >= 0) && (r >= 0) != (x >= 0));
            } else {
                const std::int64_t r = carry ? x + y : x - y;
                const std::int64_t half = std::int64_t{1} << (8 * asize - 1);
                overflow = !(-half <= r && r < half);
            }
            c = overflow;
            break;
        }
        case K::Int2Comp: c = (0 - ac) & m; break;
        case K::IntNegate: c = (~ac) & m; break;
        case K::IntLeft: c = bc < 8u * out_size ? (ac << bc) & m : 0; break;
        case K::IntRight: c = bc < 8u * asize ? (ac >> bc) & m : 0; break;
        case K::IntSRight: {
            const std::int64_t x = shadow_sign_extended(ac, asize);
            const std::uint64_t sh = std::min<std::uint64_t>(bc, 8u * asize);
            c = static_cast<std::uint64_t>(sh >= 64 ? (x < 0 ? -1 : 0) : (x >> sh)) & m;
            break;
        }
        case K::BoolNegate: c = ac == 0; break;
        case K::BoolAnd: c = ac != 0 && bc != 0; break;
        case K::BoolOr: c = ac != 0 || bc != 0; break;
        case K::BoolXor: c = (ac != 0) != (bc != 0); break;
        case K::Piece:
            if (bsize >= 8) throw ShadowLimit("shadow: a value wider than 8 bytes");
            c = ((ac << (8 * bsize)) | bc) & m;
            break;
        case K::Subpiece: c = (bc * 8 >= 64 ? 0 : (ac >> (8 * bc))) & m; break;
        case K::PopCount: c = static_cast<std::uint64_t>(std::popcount(ac)) & m; break;
        case K::LzCount: c = static_cast<std::uint64_t>(8 * asize - (64 - std::countl_zero(ac))) & m; break;
        default:   // (floating point and the rest: not followed)
            if (sym) run.lost.insert(std::format("op:{}", name));
            return {0, nullptr};
    }
    if (!sym) return {c, nullptr};

    // ---- the symbolic half
    const Expr* A = bit_vector({ac, a_s}, asize);
    const Expr* B = binary ? bit_vector({bc, b_s}, bsize) : nullptr;
    const std::uint32_t out_bits = 8 * out_size;
    const Expr* s = nullptr;
    if (const auto op = shadow_binary_of(kind)) {
        s = pool().binary(*op, A, B);
    } else if (const auto cmp = shadow_compare_of(kind)) {
        const Expr* made = pool().compare(cmp->op, A, B);
        s = pool().to_bool(cmp->flip ? pool().negate(made) : made);
    } else {
        switch (kind) {
            case K::IntZext: s = pool().zext(A, out_bits); break;
            case K::IntSext: s = pool().sext(A, out_bits); break;
            case K::IntCarry: s = pool().to_bool(pool().compare(SymOp::ULt, pool().binary(SymOp::Add, A, B), A)); break;
            case K::IntSCarry:
            case K::IntSBorrow: {
                const bool carry = kind == K::IntSCarry;
                const Expr* r = pool().binary(carry ? SymOp::Add : SymOp::Sub, A, B);
                const Expr* x = carry ? pool().binary(SymOp::And, pool().binary(SymOp::Xor, A, r), pool().binary(SymOp::Xor, B, r))
                                      : pool().binary(SymOp::And, pool().binary(SymOp::Xor, A, B), pool().binary(SymOp::Xor, A, r));
                s = pool().to_bool(pool().nonzero(pool().extract(x, 8 * asize - 1, 1)));
                break;
            }
            case K::Int2Comp: s = pool().unary(SymOp::Neg, A); break;
            case K::IntNegate: s = pool().unary(SymOp::Not, A); break;
            case K::IntLeft:
            case K::IntRight:
            case K::IntSRight: {
                const std::uint32_t width = 8 * asize;
                const SymOp shift = kind == K::IntLeft ? SymOp::Shl : kind == K::IntRight ? SymOp::LShr : SymOp::AShr;
                const Expr* amount = nullptr;
                const Expr* big = nullptr;   // the shift count reaches the width: all bits shifted out
                if (bsize > asize) {
                    amount = pool().extract(B, 0, width);
                    big = pool().compare(SymOp::ULe, pool().constant(width, 8 * bsize), B);
                } else {
                    amount = fit(B, width);
                }
                s = pool().binary(shift, A, amount);
                if (big != nullptr) {
                    const Expr* over = shift == SymOp::AShr ? pool().binary(SymOp::AShr, A, pool().constant(width - 1, width)) : pool().constant(0, width);
                    s = pool().ite(big, over, s);
                }
                break;
            }
            case K::BoolNegate: s = pool().to_bool(pool().negate(pool().nonzero(A))); break;
            case K::BoolAnd:
            case K::BoolOr:
            case K::BoolXor: {
                const SymOp how = kind == K::BoolAnd ? SymOp::BAnd : kind == K::BoolOr ? SymOp::BOr : SymOp::BXor;
                s = pool().to_bool(pool().logic(how, pool().nonzero(A), pool().nonzero(B)));
                break;
            }
            case K::Piece: s = pool().concat(A, B); break;
            case K::Subpiece: {
                const std::int64_t lo = 8 * static_cast<std::int64_t>(bc);
                const std::int64_t width = std::min<std::int64_t>(8 * asize - lo, out_bits);
                s = width > 0 ? fit(pool().extract(A, static_cast<std::uint32_t>(lo), static_cast<std::uint32_t>(width)), out_bits) : pool().constant(0, out_bits);
                break;
            }
            case K::PopCount: s = pool().popcount(A, out_bits); break;
            default:   // (no symbolic form for it: the concrete result stands)
                run.lost.insert(std::format("op:{}", name));
                return {c, nullptr};
        }
    }
    return {c, fit(s, out_bits)};   // (an output narrower or wider than made)
}

}  // namespace cb::emulate::shadow
