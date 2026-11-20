// Tests of the x86asm types: the operand flag word's bit layout, NASM's register tables, the prefix/size token numbering, the
// instruction-name tokens and the instruction flag set. The values are NASM's (include/opflags.h, x86/regs.dat, insns.dat).
#include "cb/build/x86asm/types.hpp"

#include <algorithm>
#include <cstring>
#include <string>

#include "cb/core/testing.hpp"

namespace {
using namespace cb::build::x86asm;
}

CB_TEST("x86asm.types.opflags_layout", [] {
    // The fields of the word: type 0-3, modifiers 4-6, register class 7-17, subclass 18-31, size 32-47, register set 48-52.
    CB_CHECK_EQ(OPTYPE_SHIFT, 0u);
    CB_CHECK_EQ(MODIFIER_SHIFT, 4u);
    CB_CHECK_EQ(REG_CLASS_SHIFT, 7u);
    CB_CHECK_EQ(SUBCLASS_SHIFT, 18u);
    CB_CHECK_EQ(SIZE_SHIFT, 32u);
    CB_CHECK_EQ(REGSET_SHIFT, 48u);
    CB_CHECK_EQ(OPFLAGS_USED_BITS, 53u);
    CB_CHECK_EQ(OPFLAGS_UNUSED_BITS, 11u);
    CB_CHECK_EQ(OPTYPE_MASK, std::uint64_t{0xF});
    CB_CHECK_EQ(MODIFIER_MASK, std::uint64_t{0x70});
    CB_CHECK_EQ(REG_CLASS_MASK, std::uint64_t{0x3FF80});
    CB_CHECK_EQ(SUBCLASS_MASK, std::uint64_t{0xFFFC0000});
    CB_CHECK_EQ(SIZE_MASK, std::uint64_t{0xFFFF00000000});
    CB_CHECK_EQ(REGSET_MASK, std::uint64_t{0x1F} << 48);

    using namespace opf;
    CB_CHECK_EQ(REGISTER, std::uint64_t{1});
    CB_CHECK_EQ(IMMEDIATE, std::uint64_t{2});
    CB_CHECK_EQ(REGMEM, std::uint64_t{4});
    CB_CHECK_EQ(MEMORY, std::uint64_t{0xC});
    CB_CHECK_EQ(BITS8, std::uint64_t{1} << 32);
    CB_CHECK_EQ(BITS512, std::uint64_t{1} << 39);
    CB_CHECK_EQ(FAR_, std::uint64_t{1} << 40);
    CB_CHECK_EQ(ABS_, std::uint64_t{1} << 43);
    CB_CHECK_EQ(TO, std::uint64_t{0x10});
    CB_CHECK_EQ(COLON, std::uint64_t{0x20});
    CB_CHECK_EQ(STRICT_, std::uint64_t{0x40});
    CB_CHECK_EQ(REG_CLASS_GPR, std::uint64_t{1} << 8);
    CB_CHECK_EQ(REG_GPR, std::uint64_t{0x105});
    CB_CHECK_EQ(REG8, std::uint64_t{0x105} | BITS8);
    CB_CHECK_EQ(RN_L16, std::uint64_t{1} << 28);
    CB_CHECK_EQ(RN_ZERO, (std::uint64_t{1} << 29) | (std::uint64_t{1} << 28));
    CB_CHECK_EQ(RN_NZERO, std::uint64_t{1} << 30);
    CB_CHECK_EQ(UNITY, (std::uint64_t{1} << 19) | IMMEDIATE);
    CB_CHECK_EQ(IMM_KNOWN, (std::uint64_t{1} << 25) | IMMEDIATE);
    CB_CHECK_EQ(MEM_OFFS, (std::uint64_t{1} << 18) | MEMORY);
    CB_CHECK_EQ(RS32, std::uint64_t{1} << 52);
    // A class matches an operand when (class & ~operand) == 0: a memory operand is an r/m, a register is not memory.
    CB_CHECK(is_class(REGMEM, MEMORY));
    CB_CHECK(!is_class(MEMORY, REGMEM));
    CB_CHECK(is_class(REG_GPR, REG_AL));
    CB_CHECK(is_class(REG8, REG_CL));
    CB_CHECK(!is_class(REG_AL, REG_CL));   // AL is the accumulator specifically
});

CB_TEST("x86asm.types.decorator_flags", [] {
    using namespace deco;
    CB_CHECK_EQ(Z, std::uint64_t{0x10});
    CB_CHECK_EQ(B32, std::uint64_t{0x20} | (std::uint64_t{1} << 10));
    CB_CHECK_EQ(B64, std::uint64_t{0x20} | (std::uint64_t{1} << 11));
    CB_CHECK_EQ(ER, std::uint64_t{0x40});
    CB_CHECK_EQ(SAE, std::uint64_t{0x80});
    CB_CHECK_EQ(MASK, std::uint64_t{0xF});
    CB_CHECK_EQ(BR_1TO16, std::uint64_t{3} << 12);
    CB_CHECK_EQ(brsize_to_size(BR_BITS32), opf::BITS32);
    CB_CHECK_EQ(brsize_to_size(BR_BITS64), opf::BITS64);
});

CB_TEST("x86asm.types.register_tables", [] {
    CB_CHECK_EQ(static_cast<int>(R_AH), 1);
    CB_CHECK_EQ(static_cast<int>(R_AL), 2);
    CB_CHECK_EQ(static_cast<int>(R_AX), 3);
    CB_CHECK_EQ(static_cast<int>(R_ZMM9), 344);
    CB_CHECK_EQ(EXPR_REG_START, 1);
    CB_CHECK_EQ(EXPR_REG_END, 344);
    CB_CHECK_EQ(static_cast<int>(REG_ENUM_LIMIT), 345);
    CB_CHECK_EQ(kRegNames.size(), std::size_t{344});
    CB_CHECK_EQ(kRegFlags.size(), std::size_t{345});
    CB_CHECK_EQ(kRegVals.size(), std::size_t{345});

    // The names are in NASM's (sorted) order, and every register's name finds it again.
    CB_CHECK(std::is_sorted(kRegNames.begin(), kRegNames.end(), [](const char* a, const char* b) { return std::strcmp(a, b) < 0; }));
    CB_CHECK_EQ(std::string(register_name(R_AL)), "al");
    CB_CHECK_EQ(std::string(register_name(R_ZMM9)), "zmm9");
    CB_CHECK_EQ(std::string(register_name(R_RAX)), "rax");
    CB_CHECK(register_name(0) == nullptr);
    CB_CHECK(register_name(REG_ENUM_LIMIT) == nullptr);

    // The x86 register numbers (the ModRM/opcode encodings).
    CB_CHECK_EQ(kRegVals[R_zero], -1);
    CB_CHECK_EQ(kRegVals[R_AL], 0);
    CB_CHECK_EQ(kRegVals[R_AH], 4);
    CB_CHECK_EQ(kRegVals[R_BH], 7);
    CB_CHECK_EQ(kRegVals[R_SP], 4);
    CB_CHECK_EQ(kRegVals[R_ESI], 6);
    CB_CHECK_EQ(kRegVals[R_R8], 8);
    CB_CHECK_EQ(kRegVals[R_R15D], 15);
    CB_CHECK_EQ(kRegVals[R_GS], 5);
    CB_CHECK_EQ(kRegVals[R_ES], 0);
    CB_CHECK_EQ(kRegVals[R_ZMM31], 31);
    CB_CHECK_EQ(REG_NUM_ESP, 4);
    CB_CHECK_EQ(REG_NUM_EBP, 5);

    // The flag words.
    using namespace opf;
    CB_CHECK_EQ(kRegFlags[0], std::uint64_t{0});
    CB_CHECK_EQ(kRegFlags[R_AL], rn_flags(0) | REG_AL);
    CB_CHECK_EQ(kRegFlags[R_AH], rn_flags(4) | REG_HIGH);
    CB_CHECK_EQ(kRegFlags[R_R9D], rn_flags(9) | REG32);
    CB_CHECK_EQ(kRegFlags[R_CR20], rn_flags(20) | REG_CREG);
    CB_CHECK_EQ(kRegFlags[R_FS], rn_flags(4) | REG_FS);
    CB_CHECK_EQ(kRegFlags[R_K1], rn_flags(1) | OPMASKREG);
    CB_CHECK_EQ(kRegFlags[R_ZMM31], rn_flags(31) | ZMMREG);
    CB_CHECK_EQ(kRegFlags[R_TMM7], rn_flags(7) | TMMREG);
    CB_CHECK(is_reg_class(REG_GPR, R_RAX));
    CB_CHECK(is_reg_class(REG64, R_R15));
    CB_CHECK(!is_reg_class(REG64, R_EAX));
    CB_CHECK(!is_reg_class(REG_GPR, R_ES));
    CB_CHECK(is_sreg(R_DS));
    CB_CHECK(!is_sreg(R_AX));
    CB_CHECK(is_fsgs(R_GS));
    CB_CHECK(!is_fsgs(R_ES));
    CB_CHECK(!is_reg_class(REG_GPR, 0));
    CB_CHECK(!is_reg_class(REG_GPR, P_LOCK));
});

CB_TEST("x86asm.types.token_numbering", [] {
    // Prefixes start where registers end; sizes after prefixes; specials after sizes; decorators after those.
    CB_CHECK_EQ(static_cast<int>(PREFIX_ENUM_START), static_cast<int>(REG_ENUM_LIMIT));
    CB_CHECK_EQ(static_cast<int>(P_A16), static_cast<int>(REG_ENUM_LIMIT));
    CB_CHECK_EQ(static_cast<int>(P_PN) + 1, static_cast<int>(PREFIX_ENUM_LIMIT));
    CB_CHECK_EQ(static_cast<int>(PREFIX_ENUM_LIMIT) - static_cast<int>(PREFIX_ENUM_START), 29);
    CB_CHECK_EQ(static_cast<int>(S_BYTE), static_cast<int>(PREFIX_ENUM_LIMIT));
    CB_CHECK_EQ(static_cast<int>(S_ABS), static_cast<int>(SIZE_ENUM_LIMIT));
    CB_CHECK_EQ(static_cast<int>(BRC_1TO2), static_cast<int>(SPECIAL_ENUM_LIMIT));
    CB_CHECK_EQ(std::string(prefix_name(P_LOCK)), "lock");
    CB_CHECK_EQ(std::string(prefix_name(P_REPNZ)), "repnz");
    CB_CHECK_EQ(std::string(prefix_name(P_REX2)), "{rex2}");
    CB_CHECK_EQ(std::string(prefix_name(P_PN)), "{pn}");
    CB_CHECK_EQ(std::string(prefix_name(R_FS)), "fs");   // a register can also be a prefix
    CB_CHECK(prefix_name(S_BYTE) == nullptr);

    CB_CHECK_EQ(token_value(TokenType::EOS), 0);
    CB_CHECK_EQ(token_value(TokenType::PLUS), '+');
    CB_CHECK_EQ(token_value(TokenType::LBRACKET), '[');
    CB_CHECK_EQ(token_value(TokenType::SHL), 256);
    CB_CHECK_EQ(token_value(TokenType::DBL_XOR), 267);
    CB_CHECK_EQ(token_value(TokenType::NUM), token_value(TokenType::MAX_OPERATOR) + 1);
    CB_CHECK_EQ(token_value(TokenType::START_PP), token_value(TokenType::END_ASM));
    CB_CHECK_EQ(token_value(TokenType::INVALID), -1);

    CB_CHECK_EQ(EXPR_UNKNOWN, EXPR_REG_END + 1);
    CB_CHECK_EQ(EXPR_SEGBASE, EXPR_REG_END + 5);
    CB_CHECK_EQ(static_cast<int>(MAXPREFIX), 9);
    CB_CHECK_EQ(static_cast<int>(PPS_REX), 6);
    CB_CHECK_EQ(static_cast<int>(T1S), 6);
    CB_CHECK_EQ(static_cast<int>(DUP), 16);
    CB_CHECK_EQ(REX_P, 0x40u);
    CB_CHECK_EQ(REX_rB, REX_B | REX_B1 | REX_BV | REX_H | REX_P);
    CB_CHECK_EQ(EVEX_PP.pos, 16u);
    CB_CHECK_EQ(EVEX_PP.mask, 0x3u << 16);
    CB_CHECK_EQ(EVEX_Z.mask, 1u << 31);
    CB_CHECK_EQ(EVEX_MM.basemask, 7u);
    CB_CHECK_EQ(static_cast<int>(OPTIM_LEVEL_1), 4 | 32 | 1 | 2 | 16);
    CB_CHECK_EQ(tym_type(TY_DWORD | tys_elements(5)), static_cast<unsigned>(TY_DWORD));
    CB_CHECK_EQ(tym_elements(TY_DWORD | tys_elements(5)), 5u);
});

CB_TEST("x86asm.types.instruction_names", [] {
    CB_CHECK_EQ(static_cast<int>(I_DB), 0);
    CB_CHECK_EQ(static_cast<int>(I_RESB), 8);
    CB_CHECK_EQ(static_cast<int>(I_INCBIN), 16);
    CB_CHECK_EQ(static_cast<int>(I_none), -1);
    CB_CHECK_EQ(kInsnNames.size(), std::size_t{2640});
    CB_CHECK_EQ(std::string(kInsnNames[I_DB]), "db");
    CB_CHECK_EQ(std::string(kInsnNames[I_MOV]), "mov");
    CB_CHECK_EQ(std::string(kInsnNames[I_HINT_NOP63]), "hint_nop63");
    CB_CHECK_EQ(MAX_OPERANDS, 5);
    CB_CHECK_EQ(MAX_INSLEN, 17);
    CB_CHECK_EQ(db_bytes(I_DT), 10);
    CB_CHECK_EQ(db_bytes(I_DZ), 64);
    CB_CHECK_EQ(db_bytes(I_RESB), 0);
    CB_CHECK_EQ(db_bytes(I_none), -1);
    CB_CHECK_EQ(resb_bytes(I_RESY), 32);
    CB_CHECK(opcode_is_db(I_DQ));
    CB_CHECK(!opcode_is_db(I_RESB));
    CB_CHECK(opcode_is_resb(I_RESZ));
    CB_CHECK(!opcode_is_resb(I_INCBIN));
    CB_CHECK(is_hint_nop(0x0f1c));
    CB_CHECK(is_hint_nop(0x0d));
    CB_CHECK(!is_hint_nop(0x0f20));
    CB_CHECK(!is_hint_nop(0x10000));
});

CB_TEST("x86asm.types.instruction_flags", [] {
    CB_CHECK_EQ(IF_8086, 192u);
    CB_CHECK_EQ(IF_386, 195u);
    CB_CHECK_EQ(IF_ANY, 209u);
    CB_CHECK_EQ(IF_FIELD_COUNT, 7u);
    CB_CHECK_EQ(IF_CPU_FIELD, 6u);
    CB_CHECK_EQ(IFM_SM0, 1u);
    CB_CHECK_EQ(IFM_ANY, 0x00020000u);

    IFlags f;
    CB_CHECK(!f.test(IF_PRIV));
    f.set(IF_PRIV);
    CB_CHECK(f.test(IF_PRIV));
    CB_CHECK_EQ(f.field[2], if_genbit(IF_PRIV));   // bit 66: field 2
    f.clear(IF_PRIV);
    CB_CHECK(f == IFlags{});

    // The CPU level is "at least": the highest bit set is not below the one asked about.
    IFlags cpu;
    cpu.set(IF_486);
    CB_CHECK(iflag_cpu_level_ok(cpu, IF_386));
    CB_CHECK(iflag_cpu_level_ok(cpu, IF_486));
    CB_CHECK(!iflag_cpu_level_ok(cpu, IF_PENT));
    CB_CHECK_EQ(iflag_cmp_cpu_level(cpu, cpu), 0);
    IFlags cpu2;
    cpu2.set(IF_PENT);
    CB_CHECK_EQ(iflag_cmp_cpu_level(cpu, cpu2), -1);
    CB_CHECK_EQ(compare(cpu2, cpu), 1);
    iflag_set_all_features(cpu);
    CB_CHECK(cpu.test(IF_VEX));
    CB_CHECK(cpu.test(IF_NOP));
    CB_CHECK(!cpu.test(IF_SM0));
    CB_CHECK(iflag_pfmask(cpu) == IFlags{});
    cpu.set(IF_AMD);
    CB_CHECK(iflag_pfmask(cpu).test(IF_AMD));
});

CB_TEST("x86asm.types.records_start_zeroed", [] {
    // NASM zeroes an insn and its operands before the parser fills them.
    Insn insn;
    CB_CHECK_EQ(static_cast<int>(insn.opcode), static_cast<int>(I_none));
    CB_CHECK_EQ(static_cast<int>(insn.operands), 0);
    CB_CHECK_EQ(insn.rex, 0u);
    CB_CHECK(insn.label.empty());
    CB_CHECK(insn.eops.empty());
    CB_CHECK(insn.itemp == nullptr);
    CB_CHECK_EQ(insn.oprs.size(), std::size_t{MAX_OPERANDS});
    CB_CHECK_EQ(insn.oprs[0].type, std::uint64_t{0});
    CB_CHECK_EQ(static_cast<int>(insn.oprs[4].hinttype), static_cast<int>(EAH_NOHINT));
    CB_CHECK_EQ(static_cast<int>(insn.ea.type), static_cast<int>(EA_INVALID));

    OutData out;
    CB_CHECK_EQ(static_cast<int>(out.type), static_cast<int>(OUT_RAWDATA));
    CB_CHECK_EQ(out.size, std::uint64_t{0});

    Expr e{{EXPR_SIMPLE, 1}, {R_EAX, 2}};
    CB_CHECK_EQ(e.size(), std::size_t{2});

    SrcLocation a{"f.asm", 3};
    CB_CHECK(a == SrcLocation{"f.asm", 3});
    CB_CHECK(!(a == SrcLocation{"f.asm", 4}));
});
