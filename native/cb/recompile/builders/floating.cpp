#include "cb/recompile/builders/floating.hpp"

#include <array>
#include <cstddef>

#include "cb/core/error.hpp"

namespace cb::recompile::builders::floating {
namespace {

constexpr std::string_view kFlush = "ctx.fpscr.disableFlushMode();";

// How an instruction names its registers: which operand token is which of the template's letters.
enum class Form {
    Unary,    // d, a
    Binary,   // d, a, b
    Fused,    // d, a, c, b -- the third operand token is the multiplier `c`, the fourth the addend `b`
    Compare,  // crfD, fA, fB: the template's `c` is the condition field
};

struct Rule {
    std::string_view mnemonic;
    Form form;
    std::string_view statement;   // {d} {a} {b} {c} stand for register numbers
};

// In the order the builder groups them (sign, move and conversion, comparison, then the arithmetic).
constexpr std::array kRules = std::to_array<Rule>({
    {"fabs", Form::Unary, "ctx.f{d}.u64 = ctx.f{a}.u64 & ~0x8000000000000000;"},
    {"fnabs", Form::Unary, "ctx.f{d}.u64 = ctx.f{a}.u64 | 0x8000000000000000;"},
    {"fneg", Form::Unary, "ctx.f{d}.u64 = ctx.f{a}.u64 ^ 0x8000000000000000;"},

    {"fmr", Form::Unary, "ctx.f{d}.f64 = ctx.f{a}.f64;"},
    {"fcfid", Form::Unary, "ctx.f{d}.f64 = double(ctx.f{a}.s64);"},
    {"fctid", Form::Unary,
     "ctx.f{d}.s64 = std::isnan(ctx.f{a}.f64) ? int64_t(0x8000000000000000ULL) : "
     "(ctx.f{a}.f64 > double(LLONG_MAX)) ? LLONG_MAX : "
     "simde_mm_cvtsd_si64(simde_mm_load_sd(&ctx.f{a}.f64));"},
    {"fctidz", Form::Unary,
     "ctx.f{d}.s64 = std::isnan(ctx.f{a}.f64) ? int64_t(0x8000000000000000ULL) : "
     "(ctx.f{a}.f64 > double(LLONG_MAX)) ? LLONG_MAX : "
     "simde_mm_cvttsd_si64(simde_mm_load_sd(&ctx.f{a}.f64));"},
    {"fctiwz", Form::Unary,
     "ctx.f{d}.s64 = std::isnan(ctx.f{a}.f64) ? int64_t(0x80000000U) : "
     "(ctx.f{a}.f64 > double(INT_MAX)) ? INT_MAX : "
     "simde_mm_cvttsd_si32(simde_mm_load_sd(&ctx.f{a}.f64));"},
    {"frsp", Form::Unary, "ctx.f{d}.f64 = double(float(ctx.f{a}.f64));"},

    // fcmpo is translated as fcmpu (the same statements).
    {"fcmpu", Form::Compare, "ctx.cr{c}.compare(ctx.f{a}.f64, ctx.f{b}.f64);"},
    {"fcmpo", Form::Compare, "ctx.cr{c}.compare(ctx.f{a}.f64, ctx.f{b}.f64);"},

    {"fadd", Form::Binary, "ctx.f{d}.f64 = ctx.f{a}.f64 + ctx.f{b}.f64;"},
    {"fadds", Form::Binary, "ctx.f{d}.f64 = double(float(ctx.f{a}.f64 + ctx.f{b}.f64));"},
    {"fsub", Form::Binary, "ctx.f{d}.f64 = ctx.f{a}.f64 - ctx.f{b}.f64;"},
    {"fsubs", Form::Binary, "ctx.f{d}.f64 = double(float(ctx.f{a}.f64 - ctx.f{b}.f64));"},
    {"fmul", Form::Binary, "ctx.f{d}.f64 = ctx.f{a}.f64 * ctx.f{b}.f64;"},
    {"fmuls", Form::Binary, "ctx.f{d}.f64 = double(float(ctx.f{a}.f64 * ctx.f{b}.f64));"},
    {"fdiv", Form::Binary, "ctx.f{d}.f64 = ctx.f{a}.f64 / ctx.f{b}.f64;"},
    {"fdivs", Form::Binary, "ctx.f{d}.f64 = double(float(ctx.f{a}.f64 / ctx.f{b}.f64));"},

    // a * c (+/-) b: the multiplier is the third operand token, the addend the fourth.
    {"fmadd", Form::Fused, "ctx.f{d}.f64 = ctx.f{a}.f64 * ctx.f{c}.f64 + ctx.f{b}.f64;"},
    {"fmadds", Form::Fused, "ctx.f{d}.f64 = double(float(ctx.f{a}.f64 * ctx.f{c}.f64 + ctx.f{b}.f64));"},
    {"fmsub", Form::Fused, "ctx.f{d}.f64 = ctx.f{a}.f64 * ctx.f{c}.f64 - ctx.f{b}.f64;"},
    {"fmsubs", Form::Fused, "ctx.f{d}.f64 = double(float(ctx.f{a}.f64 * ctx.f{c}.f64 - ctx.f{b}.f64));"},
    {"fnmadd", Form::Fused, "ctx.f{d}.f64 = -(ctx.f{a}.f64 * ctx.f{c}.f64 + ctx.f{b}.f64);"},
    {"fnmadds", Form::Fused, "ctx.f{d}.f64 = double(float(-(ctx.f{a}.f64 * ctx.f{c}.f64 + ctx.f{b}.f64)));"},
    {"fnmsub", Form::Fused, "ctx.f{d}.f64 = -(ctx.f{a}.f64 * ctx.f{c}.f64 - ctx.f{b}.f64);"},
    {"fnmsubs", Form::Fused, "ctx.f{d}.f64 = double(float(-(ctx.f{a}.f64 * ctx.f{c}.f64 - ctx.f{b}.f64)));"},

    {"fres", Form::Unary, "ctx.f{d}.f64 = double(float(1.0 / ctx.f{a}.f64));"},
    {"frsqrte", Form::Unary, "ctx.f{d}.f64 = 1.0 / sqrt(ctx.f{a}.f64);"},
    {"fsqrt", Form::Unary, "ctx.f{d}.f64 = sqrt(ctx.f{a}.f64);"},
    {"fsqrts", Form::Unary, "ctx.f{d}.f64 = double(float(sqrt(ctx.f{a}.f64)));"},

    // a >= 0.0 ? c : b
    {"fsel", Form::Fused, "ctx.f{d}.f64 = ctx.f{a}.f64 >= 0.0 ? ctx.f{c}.f64 : ctx.f{b}.f64;"},
});

constexpr std::array kMnemonics = [] {
    std::array<std::string_view, kRules.size()> names{};
    for (std::size_t i = 0; i < kRules.size(); ++i) names[i] = kRules[i].mnemonic;
    return names;
}();

[[noreturn]] void bad_operand(const Instruction& insn, std::size_t index, std::string_view wanted) {
    const std::string what = index < insn.operands.size()
                                 ? "operand '" + insn.operands[index] + "' is not " + std::string(wanted)
                                 : "operand " + std::to_string(index + 1) + " is missing";
    throw ParseError(insn.mnemonic, 0, static_cast<std::uint32_t>(index + 1), what);
}

int freg_at(const Instruction& insn, const Operands& ops, std::size_t index) {
    if (index >= insn.operands.size()) bad_operand(insn, index, "a floating-point register");
    const std::optional<int> n = ops.freg(insn.operands[index]);
    if (!n) bad_operand(insn, index, "a floating-point register");
    return *n;
}

int cr_at(const Instruction& insn, const Operands& ops, std::size_t index) {
    if (index >= insn.operands.size()) bad_operand(insn, index, "a condition register field");
    const std::optional<int> n = ops.cr(insn.operands[index]);
    if (!n) bad_operand(insn, index, "a condition register field");
    return *n;
}

// `statement` with each {d} {a} {b} {c} replaced by its register number.
std::string fill(std::string_view statement, int d, int a, int b, int c) {
    std::string out;
    out.reserve(statement.size() + 16);
    for (std::size_t i = 0; i < statement.size(); ++i) {
        const bool slot = statement[i] == '{' && i + 2 < statement.size() && statement[i + 2] == '}';
        if (!slot) {
            out += statement[i];
            continue;
        }
        switch (statement[i + 1]) {
            case 'd': out += std::to_string(d); break;
            case 'a': out += std::to_string(a); break;
            case 'b': out += std::to_string(b); break;
            default: out += std::to_string(c); break;   // 'c'
        }
        i += 2;
    }
    return out;
}

}  // namespace

std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands) {
    for (const Rule& rule : kRules) {
        if (rule.mnemonic != insn.mnemonic) continue;
        int d = 0, a = 0, b = 0, c = 0;
        switch (rule.form) {
            case Form::Unary:
                d = freg_at(insn, operands, 0);
                a = freg_at(insn, operands, 1);
                break;
            case Form::Binary:
                d = freg_at(insn, operands, 0);
                a = freg_at(insn, operands, 1);
                b = freg_at(insn, operands, 2);
                break;
            case Form::Fused:
                d = freg_at(insn, operands, 0);
                a = freg_at(insn, operands, 1);
                c = freg_at(insn, operands, 2);
                b = freg_at(insn, operands, 3);
                break;
            case Form::Compare:
                c = cr_at(insn, operands, 0);
                a = freg_at(insn, operands, 1);
                b = freg_at(insn, operands, 2);
                break;
        }
        return std::vector<std::string>{std::string(kFlush), fill(rule.statement, d, a, b, c)};
    }
    return std::nullopt;
}

std::string_view runtime_helpers() { return "#include <cmath>\n#include <climits>\n"; }

std::span<const std::string_view> mnemonics() { return kMnemonics; }

}  // namespace cb::recompile::builders::floating
