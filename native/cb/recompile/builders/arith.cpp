#include "cb/recompile/builders/arith.hpp"

#include <array>
#include <cstddef>

#include "cb/recompile/builders/reading.hpp"

namespace cb::recompile::builders::arith {
namespace {

// Which operands an instruction reads, and what its {i} and {j} stand for.
enum class Form {
    Three,          // rD, rA, rB
    Two,            // rD, rA
    Simm,           // rD, rA, simm:       {i} = the 32-bit signed value, {j} = its unsigned text
    SimmNot,        // rD, rA, simm:       {i} as above, {j} = the complement of the unsigned value
    SimmBase,       // rD, rA|0, simm:     {z} = "ctx.rA.s64 + " unless rA is r0
    SimmShiftedBase,// rD, rA|0, simm:     the same, with {i} = simm << 16
    CompareRegs,    // crD, rA, rB
    CompareSigned,  // crD, rA, simm:      {i} = the 32-bit signed value
    CompareUnsigned,// crD, rA, uimm:      {i} = the 32-bit unsigned value
};

struct Row {
    std::string_view mnemonic;
    Form form;
    std::string_view statements;   // '\n' between statements
};

// In the order the builder groups them: addition, division, multiplication, negation, subtraction, then the compares.
constexpr std::array kRows = std::to_array<Row>({
    {"add", Form::Three, "ctx.r{d}.u64 = ctx.r{a}.u64 + ctx.r{b}.u64;"},
    {"adde", Form::Three,
     "temp.u8 = (ctx.r{a}.u32 + ctx.r{b}.u32 < ctx.r{a}.u32) | (ctx.r{a}.u32 + ctx.r{b}.u32 + ctx.xer.ca < ctx.xer.ca);\n"
     "ctx.r{d}.u64 = ctx.r{a}.u64 + ctx.r{b}.u64 + ctx.xer.ca;\n"
     "ctx.xer.ca = temp.u8;"},
    {"addi", Form::SimmBase, "ctx.r{d}.s64 = {z}{i};"},
    {"addic", Form::SimmNot, "ctx.xer.ca = ctx.r{a}.u32 > {j};\nctx.r{d}.s64 = ctx.r{a}.s64 + {i};"},
    {"addis", Form::SimmShiftedBase, "ctx.r{d}.s64 = {z}{i};"},
    {"addze", Form::Two,
     "temp.s64 = ctx.r{a}.s64 + ctx.xer.ca;\nctx.xer.ca = temp.u32 < ctx.r{a}.u32;\nctx.r{d}.s64 = temp.s64;"},
    {"addme", Form::Two,
     "temp.u8 = (ctx.r{a}.u32 + 0xFFFFFFFFu < ctx.r{a}.u32) | (ctx.r{a}.u32 + 0xFFFFFFFFu + ctx.xer.ca < ctx.xer.ca);\n"
     "ctx.r{d}.u64 = ctx.r{a}.u64 + ctx.xer.ca + 0xFFFFFFFFFFFFFFFFull;\n"
     "ctx.xer.ca = temp.u8;"},
    {"addc", Form::Three, "ctx.xer.ca = ctx.r{a}.u32 + ctx.r{b}.u32 < ctx.r{a}.u32;\nctx.r{d}.u64 = ctx.r{a}.u64 + ctx.r{b}.u64;"},

    {"divd", Form::Three,
     "ctx.r{d}.s64 = (ctx.r{b}.s64 && !(ctx.r{a}.s64 == INT64_MIN && ctx.r{b}.s64 == -1)) ? ctx.r{a}.s64 / ctx.r{b}.s64 : 0;"},
    {"divdu", Form::Three, "ctx.r{d}.u64 = ctx.r{b}.u64 ? ctx.r{a}.u64 / ctx.r{b}.u64 : 0;"},
    {"divw", Form::Three,
     "ctx.r{d}.u64 = uint32_t((ctx.r{b}.s32 && !(ctx.r{a}.s32 == INT32_MIN && ctx.r{b}.s32 == -1)) ? ctx.r{a}.s32 / ctx.r{b}.s32 : 0);"},
    {"divwu", Form::Three, "ctx.r{d}.u64 = uint32_t(ctx.r{b}.u32 ? ctx.r{a}.u32 / ctx.r{b}.u32 : 0);"},

    {"mulhw", Form::Three, "ctx.r{d}.s64 = (int64_t(ctx.r{a}.s32) * int64_t(ctx.r{b}.s32)) >> 32;"},
    {"mulhwu", Form::Three, "ctx.r{d}.u64 = (uint64_t(ctx.r{a}.u32) * uint64_t(ctx.r{b}.u32)) >> 32;"},
    {"mulld", Form::Three, "ctx.r{d}.s64 = static_cast<int64_t>(ctx.r{a}.u64 * ctx.r{b}.u64);"},
    {"mulli", Form::Simm, "ctx.r{d}.s64 = static_cast<int64_t>(ctx.r{a}.u64 * static_cast<uint64_t>({i}));"},
    {"mullw", Form::Three, "ctx.r{d}.s64 = int64_t(ctx.r{a}.s32) * int64_t(ctx.r{b}.s32);"},
    {"mulhd", Form::Three,
     "ctx.r{d}.s64 = static_cast<int64_t>((static_cast<__int128>(static_cast<int64_t>(ctx.r{a}.s64)) * "
     "static_cast<__int128>(static_cast<int64_t>(ctx.r{b}.s64))) >> 64);"},
    {"mulhdu", Form::Three,
     "ctx.r{d}.u64 = static_cast<uint64_t>((static_cast<__uint128_t>(ctx.r{a}.u64) * static_cast<__uint128_t>(ctx.r{b}.u64)) >> 64);"},

    {"neg", Form::Two, "ctx.r{d}.s64 = static_cast<int64_t>(-ctx.r{a}.u64);"},

    // subtract-from: rD = rB - rA
    {"subf", Form::Three, "ctx.r{d}.u64 = ctx.r{b}.u64 - ctx.r{a}.u64;"},
    {"subfc", Form::Three, "ctx.xer.ca = ctx.r{b}.u32 >= ctx.r{a}.u32;\nctx.r{d}.u64 = ctx.r{b}.u64 - ctx.r{a}.u64;"},
    {"subfe", Form::Three,
     "temp.u8 = (~ctx.r{a}.u32 + ctx.r{b}.u32 < ~ctx.r{a}.u32) | (~ctx.r{a}.u32 + ctx.r{b}.u32 + ctx.xer.ca < ctx.xer.ca);\n"
     "ctx.r{d}.u64 = ~ctx.r{a}.u64 + ctx.r{b}.u64 + ctx.xer.ca;\n"
     "ctx.xer.ca = temp.u8;"},
    {"subfic", Form::Simm, "ctx.xer.ca = ctx.r{a}.u32 <= {j};\nctx.r{d}.u64 = static_cast<uint64_t>({i}) - ctx.r{a}.u64;"},
    {"subfze", Form::Two,
     "temp.u8 = ~ctx.r{a}.u32 + ctx.xer.ca < ~ctx.r{a}.u32;\nctx.r{d}.u64 = ~ctx.r{a}.u64 + ctx.xer.ca;\nctx.xer.ca = temp.u8;"},
    {"subfme", Form::Two,
     "temp.u8 = (~ctx.r{a}.u32 + 0xFFFFFFFFu < ~ctx.r{a}.u32) | (~ctx.r{a}.u32 + 0xFFFFFFFFu + ctx.xer.ca < ctx.xer.ca);\n"
     "ctx.r{d}.u64 = ~ctx.r{a}.u64 + ctx.xer.ca + 0xFFFFFFFFFFFFFFFFull;\n"
     "ctx.xer.ca = temp.u8;"},

    // compares: ctx.crD.compare<T>(rA.field, rB.field | imm, ctx.xer)
    {"cmpd", Form::CompareRegs, "ctx.cr{c}.compare<int64_t>(ctx.r{a}.s64, ctx.r{b}.s64, ctx.xer);"},
    {"cmpld", Form::CompareRegs, "ctx.cr{c}.compare<uint64_t>(ctx.r{a}.u64, ctx.r{b}.u64, ctx.xer);"},
    {"cmplw", Form::CompareRegs, "ctx.cr{c}.compare<uint32_t>(ctx.r{a}.u32, ctx.r{b}.u32, ctx.xer);"},
    {"cmpw", Form::CompareRegs, "ctx.cr{c}.compare<int32_t>(ctx.r{a}.s32, ctx.r{b}.s32, ctx.xer);"},
    {"cmpdi", Form::CompareSigned, "ctx.cr{c}.compare<int64_t>(ctx.r{a}.s64, {i}, ctx.xer);"},
    {"cmpldi", Form::CompareUnsigned, "ctx.cr{c}.compare<uint64_t>(ctx.r{a}.u64, {i}, ctx.xer);"},
    {"cmplwi", Form::CompareUnsigned, "ctx.cr{c}.compare<uint32_t>(ctx.r{a}.u32, {i}, ctx.xer);"},
    {"cmpwi", Form::CompareSigned, "ctx.cr{c}.compare<int32_t>(ctx.r{a}.s32, {i}, ctx.xer);"},
});

constexpr std::array kMnemonics = [] {
    std::array<std::string_view, kRows.size()> names{};
    for (std::size_t k = 0; k < kRows.size(); ++k) names[k] = kRows[k].mnemonic;
    return names;
}();

Slots read_slots(const Row& row, const Instruction& insn, const Operands& ops) {
    Slots s;
    switch (row.form) {
        case Form::Three:
            s.d = gpr_at(insn, ops, 0);
            s.a = gpr_at(insn, ops, 1);
            s.b = gpr_at(insn, ops, 2);
            break;
        case Form::Two:
            s.d = gpr_at(insn, ops, 0);
            s.a = gpr_at(insn, ops, 1);
            break;
        case Form::Simm:
        case Form::SimmNot:
        case Form::SimmBase:
        case Form::SimmShiftedBase: {
            s.d = gpr_at(insn, ops, 0);
            s.a = gpr_at(insn, ops, 1);
            const std::int64_t imm = imm_at(insn, ops, 2);
            const std::int64_t simm = as_i32(row.form == Form::SimmShiftedBase ? static_cast<std::int64_t>(static_cast<std::uint64_t>(imm) << 16) : imm);
            s.i = std::to_string(simm);
            s.j = std::to_string(row.form == Form::SimmNot ? as_u32(~simm) : as_u32(simm));
            if (row.form == Form::SimmBase || row.form == Form::SimmShiftedBase) {
                if (s.a != 0) s.z = "ctx.r" + std::to_string(s.a) + ".s64 + ";
            }
            break;
        }
        case Form::CompareRegs:
            s.c = crf_at(insn, ops, 0);
            s.a = gpr_at(insn, ops, 1);
            s.b = gpr_at(insn, ops, 2);
            break;
        case Form::CompareSigned:
        case Form::CompareUnsigned: {
            s.c = crf_at(insn, ops, 0);
            s.a = gpr_at(insn, ops, 1);
            const std::int64_t imm = imm_at(insn, ops, 2);
            s.i = std::to_string(row.form == Form::CompareSigned ? as_i32(imm) : as_u32(imm));
            break;
        }
    }
    return s;
}

}  // namespace

std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands) {
    // a record form's CR0 update is the recompiler's: the base mnemonic is built, the update is not
    std::string_view base = insn.mnemonic;
    if (base.ends_with('.')) base.remove_suffix(1);
    for (const Row& row : kRows) {
        if (row.mnemonic == base) return fill_lines(row.statements, read_slots(row, insn, operands));
    }
    return std::nullopt;
}

std::string_view runtime_helpers() { return ""; }

std::span<const std::string_view> mnemonics() { return kMnemonics; }

}  // namespace cb::recompile::builders::arith
