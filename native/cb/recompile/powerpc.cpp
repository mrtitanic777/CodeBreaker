// cb/recompile/powerpc.cpp -- see powerpc.hpp.

#include "cb/recompile/powerpc.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <format>
#include <memory>
#include <mutex>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/recompile/builders/arith.hpp"
#include "cb/recompile/builders/floating.hpp"
#include "cb/recompile/builders/logical.hpp"
#include "cb/recompile/builders/memory.hpp"
#include "cb/recompile/builders/operands.hpp"
#include "cb/recompile/builders/system.hpp"
#include "cb/recompile/builders/vector.hpp"

namespace cb::recompile::powerpc {

namespace {
namespace ppc {

#include "cb/recompile/powerpc_runtime.inc"

using disassemble::Row;

// ---- operand tokens ---------------------------------------------------------------------------------------------------------

std::optional<std::int64_t> decimal(std::string_view digits) {
    if (digits.empty() || digits.size() > 18) return std::nullopt;
    return text::parse_int<std::int64_t>(digits, 10);
}

// "r5" -> 5 (the letter says which kind of register)
std::optional<int> numbered(std::string_view token, char letter) {
    token = text::trim(token);
    if (token.size() < 2 || token.front() != letter) return std::nullopt;
    const std::optional<std::int64_t> n = decimal(token.substr(1));
    if (!n || *n > 100000) return std::nullopt;
    return static_cast<int>(*n);
}

std::optional<int> gpr(std::string_view token) { return numbered(token, 'r'); }
std::optional<int> fpr(std::string_view token) { return numbered(token, 'f'); }
std::optional<int> vpr(std::string_view token) { return numbered(token, 'v'); }

// A register operand where a bare 0 means the literal zero (rA of the X-form loads and stores: Capstone writes
// `stfiwx f0, 0, r11`): 0, which the builders read as "no base register".
std::optional<int> gpr_or_zero(std::string_view token) { return text::trim(token) == "0" ? std::optional<int>(0) : gpr(token); }

// -5, 0x70, -0x70
std::optional<std::int64_t> number(std::string_view token) {
    token = text::trim(token);
    bool negative = false;
    if (token.starts_with('-')) {
        negative = true;
        token.remove_prefix(1);
    } else if (token.starts_with('+')) {
        token.remove_prefix(1);
    }
    std::optional<std::int64_t> n;
    if (token.starts_with("0x") || token.starts_with("0X")) {
        if (token.size() > 2 + 16) return std::nullopt;
        const std::optional<std::uint64_t> u = text::parse_int<std::uint64_t>(token.substr(2), 16);
        if (u) n = static_cast<std::int64_t>(*u);
    } else {
        n = decimal(token);
    }
    if (!n) return std::nullopt;
    return negative ? -*n : *n;
}

// 'crN' -> the CR field 0-7 (compares, mcrf, mtcrf, branches); the symbolic 'crNlt|gt|eq|un|so' (Capstone's CR-bit operand) ->
// the CR bit 0-31 (the crand/cror/... family, which divides by 4 to recover the field). The two consumers use disjoint token
// shapes, so one reader serves both.
std::optional<int> condition_field(std::string_view token) {
    token = text::trim(token);
    if (token.size() < 3 || !token.starts_with("cr") || token[2] < '0' || token[2] > '9') return std::nullopt;
    const int field = token[2] - '0';
    const std::string_view rest = token.substr(3);
    if (rest.empty()) return field;
    int bit = -1;
    if (rest == "lt") bit = 0;
    else if (rest == "gt") bit = 1;
    else if (rest == "eq") bit = 2;
    else if (rest == "un" || rest == "so") bit = 3;
    return bit < 0 ? std::nullopt : std::optional<int>(field * 4 + bit);
}

// '-0x70(r1)' -> the displacement and the base register ('(0)' is no base).
std::optional<builders::MemoryOperand> memory_operand(std::string_view token) {
    token = text::trim(token);
    const std::size_t open = token.find('(');
    if (open == std::string_view::npos || open == 0 || !token.ends_with(')')) return std::nullopt;
    const std::string_view disp = token.substr(0, open);
    const std::string_view base = token.substr(open + 1, token.size() - open - 2);
    int base_register = 0;
    if (base != "0") {
        const std::optional<int> r = gpr(base);
        if (!r) return std::nullopt;
        base_register = *r;
    }
    // [0-9A-Fa-f]+ after an optional "-" and an optional "0x"; without the 0x the digits must be decimal
    std::string_view digits = disp.starts_with('-') ? disp.substr(1) : disp;
    if (digits.empty()) return std::nullopt;
    for (const char c : digits)
        if (!std::isxdigit(static_cast<unsigned char>(c)) && c != 'x') return std::nullopt;
    const std::optional<std::int64_t> value = number(disp);
    if (!value) return std::nullopt;
    return builders::MemoryOperand{*value, base_register};
}

std::vector<std::string> split_operands(std::string_view operands) {
    std::vector<std::string> out;
    if (text::trim(operands).empty()) return out;
    for (const std::string_view part : text::split(operands, ','))
        out.emplace_back(text::trim(part));
    return out;
}

// ---- what the builders ask of the recompiler --------------------------------------------------------------------------------

// How operand tokens are read and registers spelled in the generated code.
class Operands final : public builders::Operands {
public:
    std::optional<int> reg(std::string_view token) const override { return gpr_or_zero(token); }
    std::optional<int> vreg(std::string_view token) const override { return vpr(token); }
    std::optional<int> freg(std::string_view token) const override { return fpr(token); }
    std::optional<int> cr(std::string_view token) const override { return condition_field(token); }
    std::optional<std::int64_t> imm(std::string_view token) const override { return number(token); }
    std::optional<builders::MemoryOperand> mem(std::string_view token) const override { return memory_operand(token); }
    std::string register_value(int n, std::string_view field) const override { return std::format("ctx.r{}.{}", n, field); }
    std::string base_expression(int base) const override { return base == 0 ? "0" : std::format("ctx.r{}.u32", base); }
    std::string address(std::int64_t displacement, int base) const override {
        return base == 0 ? std::format("{}", displacement) : std::format("ctx.r{}.u32 + {}", base, displacement);
    }
};

const Operands& builder_operands() {
    static const Operands ops;
    return ops;
}

std::string unimplemented(const Row& insn) { return std::format("PPC_UNIMPLEMENTED(\"{} {}\");", insn.mnemonic, insn.operands); }

// ---- one instruction ----------------------------------------------------------------------------------------------------------

// An operand the instruction needs and the text did not give in a form this module reads: the instruction is a trap.
[[noreturn]] void bad_operand(const Row& insn, std::string_view what) {
    throw ParseError("powerpc", 0, 0, std::format("{} {}: {}", insn.mnemonic, insn.operands, what));
}

template <class T>
T needed(const std::optional<T>& value, const Row& insn, std::string_view what) {
    if (!value) bad_operand(insn, what);
    return *value;
}

// The last `0x...` number of the operands: where a branch goes.
std::optional<std::uint64_t> last_hex_number(std::string_view operands) {
    std::optional<std::uint64_t> found;
    for (std::size_t at = operands.find("0x"); at != std::string_view::npos; at = operands.find("0x", at + 2)) {
        std::size_t end = at + 2;
        while (end < operands.size() && std::isxdigit(static_cast<unsigned char>(operands[end]))) ++end;
        if (end == at + 2) continue;
        found = end - at - 2 > 16 ? std::optional<std::uint64_t>(~std::uint64_t{0}) : text::parse_int<std::uint64_t>(operands.substr(at + 2, end - at - 2), 16);
    }
    return found;
}

// A branch's statement: a goto inside the function, a tail call out of it; under a condition when there is one.
std::vector<std::string> goto_or_tail_call(std::uint64_t target, const FunctionBounds& fn, const std::string& condition = {}) {
    const std::string statement = fn.lo <= target && target < fn.hi ? std::format("goto loc_{:08X};", target)
                                                                     : std::format("sub_{:08X}(ctx, base); return;", target);
    if (condition.empty()) return {statement};
    return {std::format("if ({}) {{ {} }}", condition, statement)};
}

// beq, bne ...: the predicate of the CR field, for the mnemonics that test one bit.
std::optional<std::string> branch_predicate(std::string_view mnemonic, std::string_view cr) {
    struct Entry {
        std::string_view mnemonic;
        std::string_view bit;
        bool negated;
    };
    static constexpr std::array<Entry, 10> kRows = {{
        {"beq", "eq", false}, {"bne", "eq", true}, {"blt", "lt", false}, {"bge", "lt", true}, {"bgt", "gt", false},
        {"ble", "gt", true},  {"bso", "so", false}, {"bns", "so", true}, {"bun", "so", false}, {"bnu", "so", true},
    }};
    for (const Entry& r : kRows)
        if (r.mnemonic == mnemonic) return std::format("{}{}.{}", r.negated ? "!" : "", cr, r.bit);
    return std::nullopt;
}

// beqlr, beqctr ...: the CR bit a conditional return or jump through CTR tests, and whether it is negated.
std::optional<std::pair<std::string_view, bool>> condition_of(std::string_view stem) {
    if (stem == "eq") return std::pair<std::string_view, bool>{"eq", false};
    if (stem == "ne") return std::pair<std::string_view, bool>{"eq", true};
    if (stem == "lt") return std::pair<std::string_view, bool>{"lt", false};
    if (stem == "ge") return std::pair<std::string_view, bool>{"lt", true};
    if (stem == "gt") return std::pair<std::string_view, bool>{"gt", false};
    if (stem == "le") return std::pair<std::string_view, bool>{"gt", true};
    return std::nullopt;
}

// The mnemonic without a branch-hint suffix.
std::string_view without_hint(std::string_view mnemonic) {
    while (!mnemonic.empty() && (mnemonic.back() == '+' || mnemonic.back() == '-')) mnemonic.remove_suffix(1);
    return mnemonic;
}

class Emitter {
public:
    Emitter(const Row& insn, const FunctionBounds& fn, const JumpTables& jump_tables) : insn_(insn), fn_(fn), jump_tables_(jump_tables) {
        mn_ = insn.mnemonic;
        ops_ = split_operands(insn.operands);
    }

    std::vector<std::string> run() {
        // Capstone leaves out an implicit cr0 on compares ("cmplwi r10,2"); the builders read [crD, rA, rB|imm], so it is put
        // back ("cmplwi cr0,r10,2").
        if (mn_.starts_with("cmp") && !ops_.empty() && !condition_field(ops_[0])) ops_.insert(ops_.begin(), "cr0");

        std::string_view stem = mn_;   // the mnemonic less trailing dots
        while (stem.ends_with('.')) stem.remove_suffix(1);
        try {
            // ---- atomics: load-reserve / store-conditional. Before the record forms: stwcx. and stdcx. set CR0 as part of
            // their own semantics, and if these are not implemented every guest spinlock / refcount / atomic-increment loop
            // spins forever. The reservation's value is kept in ctx.reserved, and the conditional store is a compare-and-swap.
            if (stem == "lwarx" || stem == "ldarx" || stem == "stwcx" || stem == "stdcx") return atomic(stem);

            // ---- the integer record form (Rc bit / trailing '.'): the base operation through the normal path, then the CR0
            // compare exactly once. Kept ahead of the builders because arith strips the '.' itself (and would otherwise
            // return a base op with no CR0 update, silently dropping the record semantics). FP (CR1) and vector (CR6) manage
            // their own condition field inside their builders, so their dotted forms are not intercepted here.
            if (mn_.size() > 1 && mn_.ends_with('.') && !mn_.starts_with('f') && !mn_.starts_with('v')) return record_form();

            // vsel128's selector is its destination: the builders read it as the fourth operand
            if (mn_ == "vsel128" && ops_.size() == 3) ops_.push_back(ops_[0]);

            // ---- the builders (recompile/builders/): scalar FP, integer arithmetic and compare, logical/rotate/shift,
            // SPR/trap/CR-logical, loads/stores/atomics, and VMX/VMX128. Each returns the statements or says the mnemonic is
            // not its own. Builders that emit their own condition update (vcmp*. via CR6, the FP compares) are reached here
            // directly; base integer ops arrive dotless (the record form above already stripped it and appends CR0). A builder
            // that cannot read an operand (Capstone writes some in another shape than the builders expect) gives way to the
            // next, and in the end to a trap: never a crashed recompile.
            const builders::Instruction instruction{mn_, ops_, static_cast<std::uint32_t>(insn_.address), insn_.word};
            const builders::Operands& operands = builder_operands();
            using Build = std::optional<std::vector<std::string>> (*)(const builders::Instruction&, const builders::Operands&);
            static constexpr std::array<Build, 6> kBuilders = {builders::floating::emit, builders::arith::emit, builders::logical::emit,
                                                               builders::system::emit,   builders::memory::emit, builders::vector::emit};
            for (const Build build : kBuilders) {
                try {
                    if (std::optional<std::vector<std::string>> made = build(instruction, operands)) return std::move(*made);
                } catch (const ParseError&) {
                }
            }
            return by_hand();
        } catch (const ParseError&) {
            return {unimplemented(insn_)};   // (an honest trap, not a guess)
        }
    }

private:
    std::string rv(int n, std::string_view field = "u64") const { return std::format("ctx.r{}.{}", n, field); }
    std::string rv(const std::string& token, std::string_view field = "u64") const { return rv(needed(gpr(token), insn_, "a register"), field); }
    const std::string& op(std::size_t i) const {
        if (i >= ops_.size()) bad_operand(insn_, std::format("operand {} is missing", i + 1));
        return ops_[i];
    }
    int reg_at(std::size_t i) const { return needed(gpr(op(i)), insn_, std::format("operand {} is not a register", i + 1)); }
    int freg_at(std::size_t i) const { return needed(fpr(op(i)), insn_, std::format("operand {} is not a floating-point register", i + 1)); }
    int vreg_at(std::size_t i) const { return needed(vpr(op(i)), insn_, std::format("operand {} is not a vector register", i + 1)); }
    std::int64_t imm_at(std::size_t i) const { return needed(number(op(i)), insn_, std::format("operand {} is not a number", i + 1)); }
    std::string base_expression(int base) const { return builder_operands().base_expression(base); }

    std::vector<std::string> atomic(std::string_view stem) const {
        const int d = reg_at(0);
        const int b = reg_at(2);
        const int a = gpr(op(1)).value_or(0);   // (rA 0 is no base)
        const std::string ea = a == 0 ? rv(b, "u32") : std::format("ctx.r{}.u32 + {}", a, rv(b, "u32"));
        if (stem == "lwarx")
            return {std::format("{{ uint32_t _ea = {}; ctx.reserved.u32 = *(volatile uint32_t*)(base + _ea); ctx.r{}.u64 = __builtin_bswap32(ctx.reserved.u32); }}", ea, d)};
        if (stem == "ldarx")
            return {std::format("{{ uint32_t _ea = {}; ctx.reserved.u64 = *(volatile uint64_t*)(base + _ea); ctx.r{}.u64 = __builtin_bswap64(ctx.reserved.u64); }}", ea, d)};
        if (stem == "stwcx")
            return {std::format("{{ uint32_t _ea = {}; ctx.cr0.lt = ctx.cr0.gt = 0; ctx.cr0.so = ctx.xer.so; ctx.cr0.eq = __sync_bool_compare_and_swap((uint32_t*)(base + _ea), "
                                "ctx.reserved.u32, __builtin_bswap32(ctx.r{}.u32)); }}", ea, d)};
        return {std::format("{{ uint32_t _ea = {}; ctx.cr0.lt = ctx.cr0.gt = 0; ctx.cr0.so = ctx.xer.so; ctx.cr0.eq = __sync_bool_compare_and_swap((uint64_t*)(base + _ea), "
                            "ctx.reserved.u64, __builtin_bswap64(ctx.r{}.u64)); }}", ea, d)};
    }

    std::vector<std::string> record_form() const {
        Row base_row = insn_;
        base_row.mnemonic = mn_.substr(0, mn_.size() - 1);
        std::vector<std::string> base = emit_insn(base_row, fn_, jump_tables_);
        const std::optional<int> d = ops_.empty() ? std::nullopt : gpr(ops_[0]);
        // Don't double-set CR0: some builders (andi./andis.) already emit their own cr0 update. Only append when the base
        // didn't touch cr0.
        const bool has_cr0 = std::ranges::any_of(base, [](const std::string& s) { return s.find("cr0") != std::string::npos; });
        const bool trap = std::ranges::any_of(base, [](const std::string& s) { return s.find("UNIMPLEMENTED") != std::string::npos; });
        if (d && !has_cr0 && !trap) base.push_back(std::format("ctx.cr0.compare<int32_t>(ctx.r{}.s32, 0, ctx.xer);", *d));
        return base;
    }

    // The CR field a conditional branch tests: the first operand when it is one, else cr0.
    int cr_field() const {
        if (!ops_.empty()) {
            if (const std::optional<int> c = condition_field(ops_[0])) return *c;
        }
        return 0;
    }

    std::uint64_t branch_target() const { return needed(last_hex_number(insn_.operands), insn_, "a branch target"); }

    // What the module translates itself: control flow, the shifts by an immediate, a few moves, and the hints.
    std::vector<std::string> by_hand() const {
        const std::string& mn = mn_;
        // ---- cache / sync hints -> no-ops (correct: they are hints/barriers)
        if (mn == "dcbi" || mn == "icbi" || mn == "isync" || mn == "ptesync" || mn == "tlbsync" || mn == "dcbz128")
            return {std::format("/* {} (cache/sync hint) */", mn)};
        // supervisor MSR operations: the MSR is not modelled -- writes do nothing
        if (mn == "mtmsr" || mn == "mtmsree") return {std::format("/* {} (MSR write, not modeled) */", mn)};
        if (mn == "trap" || mn == "twui") return {"/* tw (conditional trap) - elided in PoC */"};

        // ---- a few conversions, shifts and moves the builders leave to this module
        if (mn == "fctiw") return {std::format("ctx.f{}.u64 = (uint32_t)(int32_t)ctx.f{}.f64;", freg_at(0), freg_at(1))};
        if (mn == "vmr") return {std::format("ppc_vmove(ctx.v{}, ctx.v{});", vreg_at(0), vreg_at(1))};
        if (mn == "slwi") return {std::format("ctx.r{}.u64 = (uint32_t)({} << {});", reg_at(0), rv(op(1), "u32"), imm_at(2))};
        if (mn == "srwi") return {std::format("ctx.r{}.u64 = {} >> {};", reg_at(0), rv(op(1), "u32"), imm_at(2))};
        if (mn == "sldi") return {std::format("ctx.r{}.u64 = {} << {};", reg_at(0), rv(op(1), "u64"), imm_at(2))};
        if (mn == "srdi") return {std::format("ctx.r{}.u64 = {} >> {};", reg_at(0), rv(op(1), "u64"), imm_at(2))};
        if (mn == "sub") return {std::format("{} = {} - {};", rv(op(0)), rv(op(1)), rv(op(2)))};   // extended: sub rD,rB,rA
        if (mn == "lvxl") return {vector_load()};
        if (mn == "stvxl") return {vector_store()};

        // ---- branches
        if (mn == "blr") return {"return;"};
        if (mn == "bctr") return switch_or_indirect_jump();
        if (mn == "bctrl")
            return {std::format("ctx.lr = 0x{:08X};", insn_.address + kWord), "PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32);"};
        // call through LR (LR becomes the return address)
        if (mn == "blrl")
            return {std::format("{{ uint32_t _to = (uint32_t)ctx.lr; ctx.lr = 0x{:08X}; PPC_CALL_INDIRECT_FUNC(_to); }}", insn_.address + kWord)};
        const std::string_view hinted = without_hint(mn);
        // conditional jumps through CTR (beqctr ...), and returns that count CTR down (bdzlr, bdnzlr)
        if (hinted.starts_with('b') && hinted.ends_with("ctr")) {
            if (const auto c = condition_of(hinted.substr(1, hinted.size() - 4)))
                return {std::format("if ({}ctx.cr{}.{}) {{ PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return; }}", c->second ? "!" : "", cr_field(), c->first)};
        }
        if (hinted == "bdzlr" || hinted == "bdnzlr")
            return {"ctx.ctr.u64 -= 1;", std::format("if (ctx.ctr.u64 {} 0) return;", mn.starts_with("bdz") ? "==" : "!=")};
        if (mn == "b" || mn == "ba") return goto_or_tail_call(branch_target(), fn_);
        if (mn == "bl" || mn == "bla") {
            const std::uint64_t target = branch_target();
            return {std::format("ctx.lr = 0x{:08X};", insn_.address + kWord), std::format("sub_{:08X}(ctx, base);", target)};
        }
        if (const std::optional<std::string> predicate = branch_predicate(hinted, std::format("ctx.cr{}", cr_field())))
            return goto_or_tail_call(branch_target(), fn_, *predicate);
        // conditional returns: beqlr ...
        if (hinted.starts_with('b') && hinted.ends_with("lr") && hinted.size() == 5) {
            if (const auto c = condition_of(hinted.substr(1, 2))) return {std::format("if ({}ctx.cr{}.{}) return;", c->second ? "!" : "", cr_field(), c->first)};
        }
        // ---- CTR-decrement branches (bdnz/bdz)
        if (hinted == "bdnz" || hinted == "bdz") {
            std::vector<std::string> out = {"ctx.ctr.u64 -= 1;"};
            for (std::string& s : goto_or_tail_call(branch_target(), fn_, hinted == "bdnz" ? "ctx.ctr.u64 != 0" : "ctx.ctr.u64 == 0")) out.push_back(std::move(s));
            return out;
        }
        // ...and with a condition bit too: bdnzf/bdnzt/bdzf/bdzt <bit>, <target>; the bit is written "eq" (in cr0) or "4*crN+eq"
        if ((hinted == "bdnzf" || hinted == "bdnzt" || hinted == "bdzf" || hinted == "bdzt") && ops_.size() == 2) {
            if (const std::optional<std::string> condition = ctr_and_bit(hinted)) {
                std::vector<std::string> out = {"ctx.ctr.u64 -= 1;"};
                const std::uint64_t target = needed(number(ops_[1]), insn_, "a branch target");
                for (std::string& s : goto_or_tail_call(target, fn_, *condition)) out.push_back(std::move(s));
                return out;
            }
        }
        // ---- not translated -- an honest trap (never a silent wrong result)
        return {unimplemented(insn_)};
    }

    // "ctx.ctr.u64 != 0 && ctx.cr0.eq" for bdnzt cr0eq...: the bit is "lt|gt|eq|so|un" or "4*crN+" and one of them.
    std::optional<std::string> ctr_and_bit(std::string_view hinted) const {
        std::string bit_text;
        for (const char c : ops_[0])
            if (c != ' ') bit_text += c;
        std::string_view t = bit_text;
        int field = 0;
        if (t.starts_with("4*cr")) {
            if (t.size() < 6 || t[4] < '0' || t[4] > '9' || t[5] != '+') return std::nullopt;
            field = t[4] - '0';
            t.remove_prefix(6);
        }
        std::string bit;
        if (t == "lt" || t == "gt" || t == "eq" || t == "so") bit = t;
        else if (t == "un") bit = "so";
        else return std::nullopt;
        const std::string ctr = hinted.starts_with("bdnz") ? "ctx.ctr.u64 != 0" : "ctx.ctr.u64 == 0";
        const std::string cond = std::format("{}ctx.cr{}.{}", hinted.ends_with('t') ? "" : "!", field, bit);
        return ctr + " && " + cond;
    }

    // A recovered switch table (recompile/jumptable): a real switch on the index register instead of control flow shattered
    // through an indirect call. Local cases goto the block label; out-of-function cases tail-call. An unknown index is the
    // honest indirect call (the table's default / out-of-range path).
    std::vector<std::string> switch_or_indirect_jump() const {
        const auto table = jump_tables_.find(static_cast<std::uint32_t>(insn_.address));
        if (table == jump_tables_.end()) return {"PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return;"};
        std::vector<std::string> out = {std::format("switch (ctx.r{}.u32) {{", table->second.index_reg)};
        for (std::size_t i = 0; i < table->second.targets.size(); ++i) {
            const std::uint32_t t = table->second.targets[i];
            if (fn_.lo <= t && t < fn_.hi) out.push_back(std::format("case {}u: goto loc_{:08X};", i, t));
            else out.push_back(std::format("case {}u: sub_{:08X}(ctx, base); return;", i, t));
        }
        out.emplace_back("default: PPC_CALL_INDIRECT_FUNC(ctx.ctr.u32); return;");
        out.emplace_back("}");
        return out;
    }

    std::string vector_load() const {
        return std::format("{{ uint32_t _ea = ({} + {}) & ~0xFu; for (int _i=0;_i<16;_i++) ctx.v{}.u8[_i] = PPC_LOAD_U8(_ea+_i); }}",
                           base_expression(reg_at(1)), rv(op(2), "u32"), vreg_at(0));
    }
    std::string vector_store() const {
        return std::format("{{ uint32_t _ea = ({} + {}) & ~0xFu; for (int _i=0;_i<16;_i++) PPC_STORE_U8(_ea+_i, ctx.v{}.u8[_i]); }}",
                           base_expression(reg_at(1)), rv(op(2), "u32"), vreg_at(0));
    }

    const Row& insn_;
    const FunctionBounds& fn_;
    const JumpTables& jump_tables_;
    std::string mn_;
    std::vector<std::string> ops_;
};

// ---- a function ----------------------------------------------------------------------------------------------------------------

// What the jumptable detector reads of the image.
class ImageMemory final : public jumptable::GuestMemory {
public:
    explicit ImageMemory(const GuestImage& image) : image_(image) {}

    std::optional<std::uint8_t> u8(std::uint32_t addr) const override { return read<std::uint8_t>(addr); }
    std::optional<std::uint16_t> u16(std::uint32_t addr) const override { return read<std::uint16_t>(addr); }
    std::optional<std::uint32_t> u32(std::uint32_t addr) const override { return read<std::uint32_t>(addr); }
    bool is_exec(std::uint32_t addr) const override { return image_.is_exec ? image_.is_exec(addr) : true; }

private:
    template <class T>
    std::optional<T> read(std::uint32_t addr) const {
        if (addr < image_.base) return std::nullopt;
        const std::uint64_t offset = addr - image_.base;
        if (offset + sizeof(T) > image_.bytes.size()) return std::nullopt;
        T v = 0;
        for (std::size_t i = 0; i < sizeof(T); ++i) v = static_cast<T>((v << 8) | image_.bytes[static_cast<std::size_t>(offset) + i]);
        return v;
    }
    const GuestImage& image_;
};

// "sub_0082100100(ctx" -> the address (the first call of the statement).
std::optional<std::uint32_t> called_function(std::string_view statement) {
    for (std::size_t at = statement.find("sub_"); at != std::string_view::npos; at = statement.find("sub_", at + 4)) {
        const std::string_view rest = statement.substr(at + 4);
        if (rest.size() < 8 + 4 || !rest.substr(8).starts_with("(ctx")) continue;
        std::uint32_t v = 0;
        bool hex = true;
        for (const char c : rest.substr(0, 8)) {
            const bool digit = c >= '0' && c <= '9';
            const bool upper = c >= 'A' && c <= 'F';
            if (!digit && !upper) {
                hex = false;
                break;
            }
            v = (v << 4) | static_cast<std::uint32_t>(digit ? c - '0' : c - 'A' + 10);
        }
        if (hex) return v;
    }
    return std::nullopt;
}

// A function's Disassembler per thread: Capstone's engine is not safe to share, and an image has thousands of functions.
class DisassemblerPool {
public:
    explicit DisassemblerPool(disassemble::VectorDecoder vector) : vector_(std::move(vector)) {}

    class Lease {
    public:
        Lease(DisassemblerPool& pool, std::unique_ptr<disassemble::Disassembler> dis) : pool_(pool), dis_(std::move(dis)) {}
        Lease(Lease&&) = default;
        ~Lease() {
            if (dis_) pool_.give_back(std::move(dis_));
        }
        disassemble::Disassembler& operator*() { return *dis_; }

    private:
        DisassemblerPool& pool_;
        std::unique_ptr<disassemble::Disassembler> dis_;
    };

    Lease take() {
        {
            std::lock_guard lock(mutex_);
            if (!free_.empty()) {
                auto dis = std::move(free_.back());
                free_.pop_back();
                return Lease(*this, std::move(dis));
            }
        }
        return Lease(*this, std::make_unique<disassemble::Disassembler>(vector_));
    }

private:
    void give_back(std::unique_ptr<disassemble::Disassembler> dis) {
        std::lock_guard lock(mutex_);
        free_.push_back(std::move(dis));
    }
    disassemble::VectorDecoder vector_;
    std::mutex mutex_;
    std::vector<std::unique_ptr<disassemble::Disassembler>> free_;
};

std::uint32_t big_endian_word(ByteView bytes, std::size_t at) {
    return (static_cast<std::uint32_t>(bytes[at]) << 24) | (static_cast<std::uint32_t>(bytes[at + 1]) << 16) | (static_cast<std::uint32_t>(bytes[at + 2]) << 8) | bytes[at + 3];
}

constexpr std::uint32_t kBlr = 0x4E800020;
constexpr std::uint32_t kBctr = 0x4E800420;
constexpr std::uint32_t kMaxFunctionSize = 0x4000;   // a function with no .pdata length runs at most this far
constexpr std::uint32_t kDefaultFunctionSize = 0x100;
constexpr std::size_t kMaxTailRounds = 8;

// The functions of an image sized, and the bounds they are sized against.
class Sizer {
public:
    Sizer(const disassemble::FlatImage& flat, const std::vector<std::uint32_t>& starts) : flat_(flat), bounds_(starts.begin(), starts.end()) {}

    void add_bounds(std::span<const std::uint32_t> more) { bounds_.insert(more.begin(), more.end()); }

    // Bytes from `va` through its first blr / bctr / plain branch. A function with no .pdata length runs at least that far: the
    // register save and restore runs have an entry every word (__savegprlr_14, _15 ...), so the gap to the next start is a
    // single instruction, but each entry runs on to the run's end.
    std::uint32_t to_end(std::uint32_t va) const {
        const ByteView img = flat_.bytes;
        if (va < flat_.base) return kDefaultFunctionSize;
        const std::size_t off = va - flat_.base;
        const std::size_t stop = std::min<std::size_t>(img.size(), off + kMaxFunctionSize);
        for (std::size_t k = off; k + 4 <= stop; k += 4) {
            const std::uint32_t w = big_endian_word(img, k);
            if (w == kBlr || w == kBctr || (w >> 26 == 18 && (w & 1) == 0)) return static_cast<std::uint32_t>(k + 4 - off);
        }
        return kDefaultFunctionSize;
    }

    std::uint32_t size_of(std::uint32_t va) const {
        const auto next = bounds_.upper_bound(va);
        const std::uint32_t gap = std::min<std::uint32_t>(next != bounds_.end() ? *next - va : kDefaultFunctionSize, kMaxFunctionSize);
        return gap <= 0x40 ? std::max(gap, to_end(va)) : gap;
    }

private:
    const disassemble::FlatImage& flat_;
    std::set<std::uint32_t> bounds_;   // the function starts, and the import thunks: a function with no length runs to the next
};

}  // namespace ppc
}  // namespace

std::vector<std::string> emit_insn(const disassemble::Row& insn, const FunctionBounds& function, const JumpTables& jump_tables) {
    return ppc::Emitter(insn, function, jump_tables).run();
}

Recompiled recompile_function(disassemble::Disassembler& dis, ByteView body, std::uint32_t address, std::uint32_t size, const GuestImage* image) {
    const std::uint32_t n = size - size % kWord;
    const FunctionBounds bounds{address, address + n};
    const std::vector<disassemble::Row> insns = dis.listing(body, address, 0, n / kWord);

    // pass 1: collect intra-function branch targets -> labels
    std::set<std::uint32_t> labels;
    for (const disassemble::Row& row : insns) {
        if (!row.mnemonic.starts_with('b')) continue;
        const std::optional<std::uint64_t> target = ppc::last_hex_number(row.operands);
        if (target && bounds.lo <= *target && *target < bounds.hi) labels.insert(static_cast<std::uint32_t>(*target));
    }

    // pass 1b: recover bctr jump tables from the image (recompile/jumptable). Any in-function case target becomes a block
    // label so the switch can goto it.
    JumpTables jump_tables;
    if (image) {
        std::vector<jumptable::Instruction> decoded;
        decoded.reserve(insns.size());
        for (const disassemble::Row& row : insns) decoded.push_back({row.address, row.word});
        const ppc::ImageMemory memory(*image);
        jump_tables = jumptable::detect_jump_tables(decoded, memory);
        for (const auto& [bctr, table] : jump_tables)
            for (const std::uint32_t t : table.targets)
                if (bounds.lo <= t && t < bounds.hi) labels.insert(t);
    }

    // pass 2: emit
    Recompiled out;
    std::vector<std::string> lines;
    // (A weak `__imp__` alias for the host to override would need __attribute__((alias)), an ELF feature; the macro
    // PPC_FUNC_IMPL carries the model instead, so the body is portable.)
    lines.push_back(std::format("PPC_FUNC_IMPL(sub_{:08X}) {{", address));
    lines.emplace_back("\tPPC_FUNC_PROLOGUE();");
    lines.emplace_back("\tuint32_t ea{};");
    lines.emplace_back("\tPPCRegister temp{};");   // scratch for carry/rotate builder sequences
    Stats& stats = out.stats;
    for (const disassemble::Row& row : insns) {
        ++stats.total;
        if (labels.contains(row.address)) lines.push_back(std::format("loc_{:08X}:", row.address));
        std::string comment = "\t// " + row.mnemonic + " " + row.operands;
        while (!comment.empty() && text::is_space(comment.back())) comment.pop_back();
        lines.push_back(std::move(comment));
        for (const std::string& stmt : emit_insn(row, bounds, jump_tables)) {
            lines.push_back("\t" + stmt);
            if (stmt.find("PPC_UNIMPLEMENTED") != std::string::npos) ++stats.unimplemented;
            if (const std::optional<std::uint32_t> callee = ppc::called_function(stmt)) out.called.insert(*callee);
        }
    }
    lines.emplace_back("}");
    if (std::ranges::any_of(lines, [](const std::string& l) { return l.find("vTemp") != std::string::npos; }))   // (the scratch vector some vector builders use)
        lines.insert(lines.begin() + 4, "\tPPCVRegister vTemp{};");
    stats.translated = stats.total - stats.unimplemented;
    stats.labels = labels.size();
    out.cpp = text::join(lines, "\n") + "\n";
    return out;
}

// ---- the runtime ----------------------------------------------------------------------------------------------------------------

std::string_view vector_helpers() { return ppc::kVectorHelpers; }

std::string runtime_header() {
    const auto each = [](const char* fmt_prefix, int count, std::string_view type) {
        std::string out;
        for (int i = 0; i < count; ++i) {
            if (i) out += '\n';
            out += std::format("    {} {}{};", type, fmt_prefix, i);
        }
        return out;
    };
    const std::string regs = each("r", 32, "PPCRegister");
    const std::string fprs = each("f", 32, "PPCRegister");
    const std::string vrs = each("v", 128, "PPCVRegister");
    const std::string crs = each("cr", 8, "PPCCRRegister");
    struct Width {
        const char* name;
        const char* type;
        const char* swap;
    };
    static constexpr std::array<Width, 4> kWidths = {{{"U8", "uint8_t", ""}, {"U16", "uint16_t", "__builtin_bswap16"}, {"U32", "uint32_t", "__builtin_bswap32"}, {"U64", "uint64_t", "__builtin_bswap64"}}};
    std::string loads, stores;
    for (const Width& w : kWidths) {
        if (!loads.empty()) loads += '\n';
        loads += std::format("#define PPC_LOAD_{}(x)  ({}(*(volatile {}*)(base + (uint32_t)(x))))", w.name, w.swap, w.type);
        if (!stores.empty()) stores += '\n';
        stores += std::format("#define PPC_STORE_{}(x,y)  (*(volatile {}*)(base + (uint32_t)(x)) = ({}((({})(y)))))", w.name, w.type, w.swap, w.type);
    }
    // Documentation of the runtime symbols the builders reference (the vector/memory helper prototypes + the whole-vector
    // shuffle tables). In BRIDGE mode these are real definitions from <rex/ppc/context.h|memory.h>; kept here (comment-only) so
    // the standalone contract is self-describing.
    std::string port_helpers;
    for (const std::string_view helpers : {builders::memory::runtime_helpers(), builders::vector::runtime_helpers()}) {
        if (text::trim(helpers).empty()) continue;
        if (!port_helpers.empty()) port_helpers += '\n';
        port_helpers += helpers;
    }
    std::string out(ppc::kRuntimeTemplate);
    const std::pair<std::string_view, const std::string*> slots[] = {{"@regs@", &regs}, {"@fprs@", &fprs}, {"@vrs@", &vrs},         {"@crs@", &crs},
                                                                      {"@loads@", &loads}, {"@stores@", &stores}, {"@port_helpers@", &port_helpers}};
    const std::string vec(ppc::kVectorHelpers);
    out = text::replace_all(out, "@VEC_HELPERS@", vec);
    for (const auto& [name, value] : slots) out = text::replace_all(out, name, *value);
    return out;
}

std::string forward_declarations(const std::set<std::uint32_t>& called) {
    std::string out;
    for (const std::uint32_t a : called) {
        if (!out.empty()) out += '\n';
        out += std::format("extern PPC_FUNC(sub_{:08X});", a);
    }
    return out;
}

// ---- every function of an image ----------------------------------------------------------------------------------------------------

ImageResult recompile_image(const Program& program, const disassemble::VectorDecoder& vector, const ImageOptions& options) {
    const discover::Image& image = program.image;
    const disassemble::FlatImage& flat = program.flat;
    discover::Options discovery_options;
    discovery_options.seeds = options.extra;
    disassemble::Disassembler discovery_dis(vector);
    const discover::Discovery found = discover::discover(image, discovery_dis, discovery_options);
    std::vector<std::uint32_t> addrs;   // (a std::map: in address order)
    for (const auto& [a, size] : found.functions) addrs.push_back(a);

    // a function with no .pdata length runs to the next start; the import thunks are starts too (a static initializer just
    // before the thunk table would otherwise take the table in)
    std::vector<std::uint32_t> bound_starts = addrs;
    bound_starts.insert(bound_starts.end(), image.imports.begin(), image.imports.end());
    ppc::Sizer sizer(flat, bound_starts);
    std::map<std::uint32_t, std::uint32_t> sizes;
    for (const std::uint32_t va : addrs) {
        const std::uint32_t known = found.functions.at(va);
        sizes[va] = known ? known : sizer.size_of(va);
    }
    std::set<std::uint32_t> chosen_set;
    const std::size_t take = options.limit && *options.limit > 0 ? std::min(*options.limit, addrs.size()) : addrs.size();
    chosen_set.insert(addrs.begin(), addrs.begin() + static_cast<std::ptrdiff_t>(take));
    std::vector<std::uint32_t> emit_addrs;
    for (const std::uint32_t a : chosen_set)
        if (!options.skip.contains(a)) emit_addrs.push_back(a);
    const std::uint32_t code_lo = addrs.empty() ? flat.base : addrs.front();
    const std::uint32_t code_hi = addrs.empty() ? flat.base : addrs.back() + sizes.at(addrs.back());
    const GuestImage guest{flat.bytes, flat.base, [code_lo, code_hi](std::uint32_t a) { return (a & 3) == 0 && code_lo <= a && a < code_hi; }};

    ppc::DisassemblerPool pool(vector);
    const unsigned workers = options.jobs ? *options.jobs : static_cast<unsigned>(parallel::workers());
    parallel::ThreadPool threads(std::max(1u, workers));
    const auto recompile_one = [&](std::uint32_t va) -> std::optional<Recompiled> {
        if (va < flat.base || va - flat.base >= flat.bytes.size()) return std::nullopt;
        const std::size_t off = va - flat.base;
        const std::size_t length = std::min<std::size_t>(sizes.at(va), flat.bytes.size() - off);
        if (length == 0) return std::nullopt;
        auto lease = pool.take();   // (each thread decodes with its own Capstone)
        return recompile_function(*lease, ByteView(flat.bytes).subspan(off, length), va, sizes.at(va), &guest);
    };

    // Recompile, then recompile what the code calls that is not yet a function -- the targets of tail jumps (a `b` into
    // another function: the register restore runs are entered that way, one entry per register count) -- until nothing called
    // is missing.
    std::map<std::uint32_t, std::optional<Recompiled>> results;
    std::vector<std::uint32_t> pending = emit_addrs;
    for (std::size_t round = 0; !pending.empty() && round < ppc::kMaxTailRounds; ++round) {
        const std::vector<std::optional<Recompiled>> made = parallel::parallel_map(threads, pending, recompile_one);
        for (std::size_t i = 0; i < pending.size(); ++i) results[pending[i]] = made[i];
        std::set<std::uint32_t> called;
        for (const auto& [va, r] : results)
            if (r) called.insert(r->called.begin(), r->called.end());
        std::vector<std::uint32_t> missing;
        if (!options.limit || *options.limit == 0) {
            for (const std::uint32_t a : called) {
                if (!results.contains(a) && !options.skip.contains(a) && image.code_begin <= a && a < image.code_end && !image.imports.contains(a)) missing.push_back(a);
            }
        }
        if (!missing.empty()) {
            std::vector<std::uint32_t> more(missing);
            for (const auto& [va, r] : results) more.push_back(va);
            sizer.add_bounds(more);
            for (const std::uint32_t a : missing) sizes[a] = sizer.size_of(a);
            chosen_set.insert(missing.begin(), missing.end());
        }
        pending = std::move(missing);
    }

    ImageResult out;
    Aggregate& agg = out.agg;
    const std::vector<std::uint32_t> chosen(chosen_set.begin(), chosen_set.end());
    agg.discovered = found.functions.size();
    agg.tail_entries = chosen.size() - take;
    agg.lo = chosen.empty() ? 0 : chosen.front();
    agg.hi = chosen.empty() ? 0 : chosen.back();
    std::set<std::uint32_t> all_called;
    for (const auto& [va, r] : results) {
        if (!r) continue;
        agg.parts.push_back(r->cpp);
        all_called.insert(r->called.begin(), r->called.end());
        ++agg.funcs;
        agg.total += r->stats.total;
        agg.translated += r->stats.translated;
        agg.unimplemented += r->stats.unimplemented;
    }
    // every function is declared before any is defined (an earlier one calls a later one)
    std::set<std::uint32_t> declared(chosen.begin(), chosen.end());
    declared.insert(all_called.begin(), all_called.end());
    agg.decls.assign(declared.begin(), declared.end());
    agg.chosen = chosen;
    for (const std::uint32_t a : chosen)
        if (options.skip.contains(a)) agg.skipped.push_back(a);

    std::string body = "#include \"rex_ppc_min.h\"\n\n";
    for (std::size_t i = 0; i < agg.decls.size(); ++i) {
        if (i) body += '\n';
        body += std::format("PPC_FUNC(sub_{:08X});", agg.decls[i]);
    }
    body += "\n\n" + text::join(agg.parts, "\n");
    out.body = std::move(body);
    std::string mapping = "#include \"rex_ppc_min.h\"\n\nstruct PPCFuncMapping { uint32_t addr; PPCFunc* func; };\nPPCFuncMapping PPCFuncMappings[] = {\n";
    for (const std::uint32_t va : chosen) mapping += std::format("\t{{ 0x{:08X}, sub_{:08X} }},\n", va, va);
    mapping += "\t{ 0, nullptr },\n};\n";
    out.mapping = std::move(mapping);
    return out;
}

// ---- checking the output compiles ----------------------------------------------------------------------------------------------------

std::optional<std::filesystem::path> find_compiler(const CompilerEnv& env) {
    std::error_code ec;
    if (env.gpp && std::filesystem::is_regular_file(*env.gpp, ec)) return *env.gpp;
    const std::optional<std::string> path = env.path_variable ? env.path_variable : fs::get_env("PATH");
    if (!path) return std::nullopt;
    for (const std::string_view dir : text::split(*path, ';')) {
        if (dir.empty()) continue;
        for (const char* name : {"g++.exe", "g++"}) {
            const std::filesystem::path candidate = fs::from_text(dir) / name;
            if (std::filesystem::is_regular_file(candidate, ec)) return candidate;
        }
    }
    return std::nullopt;
}

std::vector<std::filesystem::path> kernel_includes(const std::filesystem::path& checkout_root) {
    const std::filesystem::path root = checkout_root / "native" / "rexglue" / "kernel";
    std::error_code ec;
    if (!std::filesystem::is_directory(root, ec)) return {};
    std::vector<std::filesystem::path> out;
    for (const char* dir : {"shim", "include", "thirdparty/fmt/include", "thirdparty/simde", "thirdparty", "thirdparty/mspack", "thirdparty/o1heap", "thirdparty/crypto"})
        out.push_back(root / dir);
    return out;
}

CompileCheck check_compiles(std::string_view source, const CompilerEnv& env, bool bridge) {
    const std::optional<std::filesystem::path> gpp = find_compiler(env);
    if (!gpp) return {std::nullopt, "no C++ compiler (settings.json: gpp, or g++ on PATH)"};
    const std::vector<std::filesystem::path> includes = bridge ? kernel_includes(env.checkout_root) : std::vector<std::filesystem::path>{};
    fs::ScopedTempDir dir("cb-recompile");
    fs::write_text(dir.path() / "rex_ppc_min.h", runtime_header());
    const std::filesystem::path cpp = dir.path() / "f.cpp";
    fs::write_text(cpp, source);
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(*gpp), includes.empty() ? "-std=c++20" : "-std=c++23", "-c", "-w", "-I", fs::to_text(dir.path())};
    for (const std::filesystem::path& inc : includes) {
        spec.argv.emplace_back("-I");
        spec.argv.push_back(fs::to_text(inc));
    }
    spec.argv.emplace_back(includes.empty() ? "-DREX_PPC_STANDALONE" : "-DFMT_HEADER_ONLY");
    spec.argv.emplace_back("-o");
    spec.argv.push_back(fs::to_text(dir.path() / "f.o"));
    spec.argv.push_back(fs::to_text(cpp));
    spec.timeout = std::chrono::minutes(10);
    const process::ProcessResult r = process::run(spec);
    std::string message = text::sanitize_utf8(as_text(r.err));
    if (message.size() > 3000) message.resize(3000);
    return {r.exit_code == 0, message};
}

// ---- the command ----------------------------------------------------------------------------------------------------------------------

int main(const Hooks& hooks, std::span<const std::string> args) {
    const cli::Command command{
        "recompile",
        "Xbox 360 PowerPC recompiled to C++ that runs on a PC",
        {cli::Positional{"image", "a PowerPC image or an .xex (not needed with --runtime)", /*required=*/false},
         cli::Option{"--all", "", "recompile every function"},
         cli::Option{"--at", "", "recompile the function at this address", cli::Arg::Value, "ADDR"},
         cli::Option{"--size", "", "its size", cli::Arg::Value, "N", "0x100"},
         cli::Option{"--out", "-o", "write the C++ here (--all without it: the binary's project folder)", cli::Arg::Value, "FILE"},
         cli::Option{"--limit", "", "only the first N functions", cli::Arg::Value, "N"},
         cli::Option{"--jobs", "-j", "recompile on this many threads", cli::Arg::Value, "N"},
         cli::Option{"--check", "", "compile the output with g++ (exit 1 when it does not)"},
         cli::Option{"--runtime", "", "write the runtime header here and stop", cli::Arg::Value, "HEADER"}},
        [&](const cli::Args& a) -> int {
            if (const std::optional<std::string> header = a.value("--runtime")) {
                fs::write_text(fs::from_text(*header), runtime_header());
                cb::out("wrote {}\n", *header);
                return 0;
            }
            const std::optional<std::string> image_arg = a.value("image");
            if (!image_arg) throw UsageError("an image is needed (or --runtime)");
            const std::filesystem::path binary = fs::from_text(*image_arg);
            std::optional<std::filesystem::path> out = a.value("--out") ? std::optional<std::filesystem::path>(fs::from_text(*a.value("--out"))) : std::nullopt;
            if (!a.flag("--all") && !a.value("--at")) throw UsageError("--all, or --at ADDR, says what to recompile");
            if (!hooks.load_image) throw InternalError("recompile: no image reader");
            const LoadedImage loaded = hooks.load_image(binary);
            std::string full;
            if (a.flag("--all")) {
                ImageOptions options;
                if (const auto limit = a.integer<std::size_t>("--limit")) options.limit = *limit;
                if (const auto jobs = a.integer<unsigned>("--jobs")) options.jobs = *jobs;
                const ImageResult made = recompile_image({loaded.image, disassemble::flatten(loaded.facts)}, hooks.vector, options);
                const Aggregate& agg = made.agg;
                cb::out("{} functions ({} found, {} tail-jump entries), {}/{} instructions translated ({:.1f}%), {} not\n", agg.funcs, agg.discovered,
                        agg.tail_entries, agg.translated, agg.total, 100.0 * static_cast<double>(agg.translated) / static_cast<double>(std::max<std::size_t>(1, agg.total)),
                        agg.unimplemented);
                bool in_project = false;
                if (!out) {
                    if (!hooks.project_output) throw UsageError("-o FILE says where to write the C++");
                    out = hooks.project_output(binary);
                    in_project = true;
                }
                std::filesystem::path init = *out;
                init.replace_extension();
                init = fs::from_text(fs::to_text(init) + "_init.cpp");
                fs::write_text(*out, made.body);
                fs::write_text(init, made.mapping);
                if (in_project && hooks.record)
                    hooks.record(binary, {*out, init}, "every function as C++ (one statement per guest instruction) and its mapping table");
                cb::out("wrote {} and its mapping table\n", fs::to_text(*out));
                full = made.body;
            } else {
                const std::uint32_t at = *a.integer<std::uint32_t>("--at");
                const std::uint32_t size = a.integer<std::uint32_t>("--size").value_or(0x100);
                const disassemble::CodeSection section = disassemble::code_section(loaded.facts);
                if (at < section.address || at - section.address > section.code.size()) throw UsageError(std::format("0x{:08X} is not in the code", at));
                const std::size_t off = at - section.address;
                disassemble::Disassembler dis(hooks.vector);
                const Recompiled r = recompile_function(dis, ByteView(section.code).subspan(off, std::min<std::size_t>(size, section.code.size() - off)), at, size);
                full = "#include \"rex_ppc_min.h\"\n\n" + forward_declarations(r.called) + "\n\n" + r.cpp;
                if (out) fs::write_text(*out, full);
                cb::out("sub_{:08X}: {}/{} instructions translated, {} not, {} labels, {} calls\n", at, r.stats.translated, r.stats.total, r.stats.unimplemented,
                        r.stats.labels, r.called.size());
            }
            if (a.flag("--check")) {
                const CompileCheck check = check_compiles(full, hooks.compiler);
                if (check.compiles.value_or(false)) cb::out("compiles\n");
                else if (check.compiles) cb::out("does not compile:\n{}\n", check.message);
                else cb::out("{}\n", check.message);
                return check.compiles.value_or(false) ? 0 : 1;
            }
            return 0;
        },
        {}, {}, {}};
    const cli::Cli program("codebreaker", {command});
    std::vector<std::string> line = {"recompile"};
    line.insert(line.end(), args.begin(), args.end());
    try {
        return program.run(line);
    } catch (const UsageError& e) {   // (the command was misused: say so, exit 2)
        cb::err("{}\n", e.what());
        return 2;
    }
}

}  // namespace cb::recompile::powerpc
