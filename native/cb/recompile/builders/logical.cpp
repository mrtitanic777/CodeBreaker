#include "cb/recompile/builders/logical.hpp"

#include <array>
#include <cstddef>

#include "cb/core/error.hpp"
#include "cb/recompile/builders/reading.hpp"

namespace cb::recompile::builders::logical {
namespace {

constexpr std::uint64_t kAllBits = 0xFFFFFFFFFFFFFFFFull;

// Which operands an instruction reads, and what its {i} {j} {k} stand for ("sh" is a shift count operand, "mb" and "me"
// the mask bounds).
enum class Form {
    Three,           // rA, rS, rB
    Two,             // rA, rS
    Imm,             // rA, rS, imm:  {i} = the immediate as written
    ImmShifted,      // {i} = imm << 16
    ImmLowShifted,   // {i} = (imm & 0xFFFFFFFF) << 16
    ClearLeftWord,   // rA, rS, n:    {j} = the mask of the low 32 - n bits
    Rldicl,          // sh, mb:       {j} = mask(mb, 63)
    Rldicr,          // sh, me:       {j} = mask(0, me)
    Rldic,           // sh, mb:       {j} = mask(mb, 63 - sh)
    Rldimi,          // sh, mb:       {j} = mask(mb, ~sh), {k} = its complement
    Rldcl,           // rA, rS, rB, mb: {j} = mask(mb, 63)
    Rldcr,           // rA, rS, rB, me: {j} = mask(0, me)
    Rlwimi,          // sh, mb, me:   {j} = mask(mb + 32, me + 32), {k} = its complement
    Rlwinm,          // sh, mb, me
    Rlwnm,           // rA, rS, rB, mb, me
    ShiftAlgebraic,  // sh:           {j} = mask(64 - sh, 63); `when_zero` is used for sh == 0
};

struct Row {
    std::string_view mnemonic;
    Form form;
    std::string_view statements;     // '\n' between statements
    std::string_view when_zero = {}; // the statements when the shift count is 0 (ShiftAlgebraic, Rldic)
};

constexpr std::array kRows = std::to_array<Row>({
    // AND
    {"and", Form::Three, "ctx.r{d}.u64 = ctx.r{a}.u64 & ctx.r{b}.u64;"},
    {"andc", Form::Three, "ctx.r{d}.u64 = ctx.r{a}.u64 & ~ctx.r{b}.u64;"},
    {"andi", Form::Imm, "ctx.r{d}.u64 = ctx.r{a}.u64 & {i};\nctx.cr0.compare<int32_t>(ctx.r{d}.s32, 0, ctx.xer);"},
    {"andis", Form::ImmShifted, "ctx.r{d}.u64 = ctx.r{a}.u64 & {i};\nctx.cr0.compare<int32_t>(ctx.r{d}.s32, 0, ctx.xer);"},
    // OR, NAND, NOR, NOT
    {"nand", Form::Three, "ctx.r{d}.u64 = ~(ctx.r{a}.u64 & ctx.r{b}.u64);"},
    {"nor", Form::Three, "ctx.r{d}.u64 = ~(ctx.r{a}.u64 | ctx.r{b}.u64);"},
    {"not", Form::Two, "ctx.r{d}.u64 = ~ctx.r{a}.u64;"},
    {"or", Form::Three, "ctx.r{d}.u64 = ctx.r{a}.u64 | ctx.r{b}.u64;"},
    {"orc", Form::Three, "ctx.r{d}.u64 = ctx.r{a}.u64 | ~ctx.r{b}.u64;"},
    {"ori", Form::Imm, "ctx.r{d}.u64 = ctx.r{a}.u64 | {i};"},
    {"oris", Form::ImmLowShifted, "ctx.r{d}.u64 = ctx.r{a}.u64 | {i};"},
    // XOR, EQV
    {"xor", Form::Three, "ctx.r{d}.u64 = ctx.r{a}.u64 ^ ctx.r{b}.u64;"},
    {"xori", Form::Imm, "ctx.r{d}.u64 = ctx.r{a}.u64 ^ {i};"},
    {"xoris", Form::ImmShifted, "ctx.r{d}.u64 = ctx.r{a}.u64 ^ {i};"},
    {"eqv", Form::Three, "ctx.r{d}.u64 = ~(ctx.r{a}.u64 ^ ctx.r{b}.u64);"},
    // count leading zeros
    {"cntlzd", Form::Two, "ctx.r{d}.u64 = ctx.r{a}.u64 == 0 ? 64 : __builtin_clzll(ctx.r{a}.u64);"},
    {"cntlzw", Form::Two, "ctx.r{d}.u64 = ctx.r{a}.u32 == 0 ? 32 : __builtin_clz(ctx.r{a}.u32);"},
    // sign extension
    {"extsb", Form::Two, "ctx.r{d}.s64 = ctx.r{a}.s8;"},
    {"extsh", Form::Two, "ctx.r{d}.s64 = ctx.r{a}.s16;"},
    {"extsw", Form::Two, "ctx.r{d}.s64 = ctx.r{a}.s32;"},
    // clear left word immediate
    {"clrlwi", Form::ClearLeftWord, "ctx.r{d}.u64 = ctx.r{a}.u32 & {j};"},
    // rotate left doubleword
    {"rldicl", Form::Rldicl, "ctx.r{d}.u64 = __builtin_rotateleft64(ctx.r{a}.u64, {i}) & {j};"},
    {"rldicr", Form::Rldicr, "ctx.r{d}.u64 = __builtin_rotateleft64(ctx.r{a}.u64, {i}) & {j};"},
    {"rldic", Form::Rldic, "ctx.r{d}.u64 = __builtin_rotateleft64(ctx.r{a}.u64, {i}) & {j};", "ctx.r{d}.u64 = ctx.r{a}.u64 & {j};"},
    {"rldcl", Form::Rldcl, "ctx.r{d}.u64 = __builtin_rotateleft64(ctx.r{a}.u64, ctx.r{b}.u8 & 0x3F) & {j};"},
    {"rldcr", Form::Rldcr, "ctx.r{d}.u64 = __builtin_rotateleft64(ctx.r{a}.u64, ctx.r{b}.u8 & 0x3F) & {j};"},
    {"rldimi", Form::Rldimi, "ctx.r{d}.u64 = (__builtin_rotateleft64(ctx.r{a}.u64, {i}) & {j}) | (ctx.r{d}.u64 & {k});"},
    {"rotldi", Form::Imm, "ctx.r{d}.u64 = __builtin_rotateleft64(ctx.r{a}.u64, {i});"},
    // rotate left word
    {"rlwimi", Form::Rlwimi,
     "ctx.r{d}.u64 = (__builtin_rotateleft64(ctx.r{a}.u32 | (ctx.r{a}.u64 << 32), {i}) & {j}) | (ctx.r{d}.u64 & {k});"},
    {"rlwinm", Form::Rlwinm, "ctx.r{d}.u64 = __builtin_rotateleft64(ctx.r{a}.u32 | (ctx.r{a}.u64 << 32), {i}) & {j};"},
    {"rlwnm", Form::Rlwnm, "ctx.r{d}.u64 = __builtin_rotateleft64(ctx.r{a}.u32 | (ctx.r{a}.u64 << 32), ctx.r{b}.u8 & 0x1F) & {j};"},
    {"rotlw", Form::Three, "ctx.r{d}.u64 = __builtin_rotateleft32(ctx.r{a}.u32, ctx.r{b}.u8 & 0x1F);"},
    {"rotlwi", Form::Imm, "ctx.r{d}.u64 = __builtin_rotateleft32(ctx.r{a}.u32, {i});"},
    // shift left
    {"sld", Form::Three, "ctx.r{d}.u64 = ctx.r{b}.u8 & 0x40 ? 0 : (ctx.r{a}.u64 << (ctx.r{b}.u8 & 0x7F));"},
    {"slw", Form::Three, "ctx.r{d}.u64 = ctx.r{b}.u8 & 0x20 ? 0 : (ctx.r{a}.u32 << (ctx.r{b}.u8 & 0x3F));"},
    // shift right algebraic
    {"srad", Form::Three,
     "temp.u64 = ctx.r{b}.u64 & 0x7F;\n"
     "if (temp.u64 > 0x3F) temp.u64 = 0x3F;\n"
     "ctx.xer.ca = (ctx.r{a}.s64 < 0) & (((ctx.r{a}.s64 >> temp.u64) << temp.u64) != ctx.r{a}.s64);\n"
     "ctx.r{d}.s64 = ctx.r{a}.s64 >> temp.u64;"},
    {"sradi", Form::ShiftAlgebraic,
     "ctx.xer.ca = (ctx.r{a}.s64 < 0) & ((ctx.r{a}.u64 & {j}) != 0);\nctx.r{d}.s64 = ctx.r{a}.s64 >> {i};",
     "ctx.xer.ca = 0;\nctx.r{d}.s64 = ctx.r{a}.s64;"},
    {"sraw", Form::Three,
     "temp.u32 = ctx.r{b}.u32 & 0x3F;\n"
     "if (temp.u32 > 0x1F) temp.u32 = 0x1F;\n"
     "ctx.xer.ca = (ctx.r{a}.s32 < 0) & (((ctx.r{a}.s32 >> temp.u32) << temp.u32) != ctx.r{a}.s32);\n"
     "ctx.r{d}.s64 = ctx.r{a}.s32 >> temp.u32;"},
    {"srawi", Form::ShiftAlgebraic,
     "ctx.xer.ca = (ctx.r{a}.s32 < 0) & ((ctx.r{a}.u32 & {j}) != 0);\nctx.r{d}.s64 = ctx.r{a}.s32 >> {i};",
     "ctx.xer.ca = 0;\nctx.r{d}.s64 = ctx.r{a}.s32;"},
    // shift right
    {"srd", Form::Three, "ctx.r{d}.u64 = ctx.r{b}.u8 & 0x40 ? 0 : (ctx.r{a}.u64 >> (ctx.r{b}.u8 & 0x7F));"},
    {"srw", Form::Three, "ctx.r{d}.u64 = ctx.r{b}.u8 & 0x20 ? 0 : (ctx.r{a}.u32 >> (ctx.r{b}.u8 & 0x3F));"},
});

constexpr std::array kMnemonics = [] {
    std::array<std::string_view, kRows.size()> names{};
    for (std::size_t k = 0; k < kRows.size(); ++k) names[k] = kRows[k].mnemonic;
    return names;
}();

// Sums and differences that only feed a mask (which keeps six bits) wrap round instead of overflowing.
std::int64_t wrap_add(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(b)); }
std::int64_t wrap_sub(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) - static_cast<std::uint64_t>(b)); }

// The decimal text of `value << 16`; an immediate no real instruction carries (they are 16 bits) is refused when the
// result would not fit.
std::string shifted_left_16(const Instruction& insn, std::int64_t value) {
    constexpr std::int64_t kLimit = std::int64_t{1} << 47;
    if (value < -kLimit || value >= kLimit) throw ParseError(insn.mnemonic, 0, 3, "immediate " + std::to_string(value) + " does not fit when shifted left by 16");
    return std::to_string(value * 65536);
}

Slots read_slots(const Row& row, const Instruction& insn, const Operands& ops) {
    Slots s;
    s.d = gpr_at(insn, ops, 0);
    s.a = gpr_at(insn, ops, 1);
    auto mask_slots = [&](std::int64_t start, std::int64_t stop, bool with_complement) {
        const std::uint64_t mask = rotate_mask(start, stop);
        s.j = hex(mask);
        if (with_complement) s.k = hex(~mask);
    };
    switch (row.form) {
        case Form::Two: break;
        case Form::Three: s.b = gpr_at(insn, ops, 2); break;
        case Form::Imm: s.i = std::to_string(imm_at(insn, ops, 2)); break;
        case Form::ImmShifted: s.i = shifted_left_16(insn, imm_at(insn, ops, 2)); break;
        case Form::ImmLowShifted: s.i = std::to_string((imm_at(insn, ops, 2) & 0xFFFFFFFF) << 16); break;
        case Form::ClearLeftWord: {
            const std::int64_t n = imm_at(insn, ops, 2);
            if (n > 32) throw ParseError(insn.mnemonic, 0, 3, "clear count " + std::to_string(n) + " is above 32");
            const std::uint64_t width = static_cast<std::uint64_t>(32) - static_cast<std::uint64_t>(n);   // a negative count widens it
            s.j = hex(width >= 64 ? kAllBits : (std::uint64_t{1} << width) - 1);
            break;
        }
        case Form::Rldicl:
            s.i = std::to_string(imm_at(insn, ops, 2));
            mask_slots(imm_at(insn, ops, 3), 63, false);
            break;
        case Form::Rldicr:
            s.i = std::to_string(imm_at(insn, ops, 2));
            mask_slots(0, imm_at(insn, ops, 3), false);
            break;
        case Form::Rldic: {
            const std::int64_t sh = imm_at(insn, ops, 2);
            s.i = std::to_string(sh);
            mask_slots(imm_at(insn, ops, 3), wrap_sub(63, sh), false);
            break;
        }
        case Form::Rldimi: {
            const std::int64_t sh = imm_at(insn, ops, 2);
            s.i = std::to_string(sh);
            mask_slots(imm_at(insn, ops, 3), ~sh, true);
            break;
        }
        case Form::Rldcl:
            s.b = gpr_at(insn, ops, 2);
            mask_slots(imm_at(insn, ops, 3), 63, false);
            break;
        case Form::Rldcr:
            s.b = gpr_at(insn, ops, 2);
            mask_slots(0, imm_at(insn, ops, 3), false);
            break;
        case Form::Rlwimi:
        case Form::Rlwinm:
            s.i = std::to_string(imm_at(insn, ops, 2));
            mask_slots(wrap_add(imm_at(insn, ops, 3), 32), wrap_add(imm_at(insn, ops, 4), 32), row.form == Form::Rlwimi);
            break;
        case Form::Rlwnm:
            s.b = gpr_at(insn, ops, 2);
            mask_slots(wrap_add(imm_at(insn, ops, 3), 32), wrap_add(imm_at(insn, ops, 4), 32), false);
            break;
        case Form::ShiftAlgebraic: {
            const std::int64_t n = imm_at(insn, ops, 2);
            s.i = std::to_string(n);
            mask_slots(wrap_sub(64, n), 63, false);
            break;
        }
    }
    return s;
}

}  // namespace

std::uint64_t rotate_mask(std::int64_t start, std::int64_t stop) {
    start &= 0x3F;
    stop &= 0x3F;
    const std::uint64_t value = (kAllBits >> start) ^ (stop >= 63 ? 0 : (kAllBits >> (stop + 1)));
    return start <= stop ? value : ~value;
}

std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands) {
    for (const Row& row : kRows) {
        if (row.mnemonic != insn.mnemonic) continue;
        const Slots slots = read_slots(row, insn, operands);
        const bool zero_count = (row.form == Form::ShiftAlgebraic || row.form == Form::Rldic) && slots.i == "0";
        return fill_lines(zero_count ? row.when_zero : row.statements, slots);
    }
    return std::nullopt;
}

std::string_view runtime_helpers() { return ""; }

std::span<const std::string_view> mnemonics() { return kMnemonics; }

}  // namespace cb::recompile::builders::logical
