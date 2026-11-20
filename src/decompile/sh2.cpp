// decompile/sh2.cpp -- the SH-2's instructions: a table of every encoding, read both ways (sh2.hpp).

#include "decompile/sh2.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <vector>

#include "core/text.hpp"
#include "decompile/mips.hpp"

namespace cb::decompile::sh2 {

namespace {

// ---- the table --------------------------------------------------------------------------------------------------

using O = Operand;

struct Def {
    std::string_view name;
    std::uint16_t mask;    // which bits of the word are fixed ...
    std::uint16_t bits;    // ... and what they are (everything else is an operand)
    O a, b;
    std::uint8_t size;     // 1, 2 or 4: the unit of a displacement; 0 where there is none
    Flow flow;
    bool delay;
};

constexpr std::uint16_t kRm = 0xF00F;   // rn and rm: nnnn mmmm in the middle, the last nibble fixed
constexpr std::uint16_t kRn = 0xF0FF;   // one register in bits 8-11
constexpr std::uint16_t kAll = 0xFFFF;
constexpr std::uint16_t kTop = 0xF000;  // the first nibble fixed: a field of 8 or 12 bits, or a displacement
constexpr std::uint16_t kByte = 0xFF00;

constexpr auto kDefs = std::to_array<Def>({
    // 0xxx
    {"stc", kRn, 0x0002, O::Sr, O::RegN, 0, Flow::None, false},
    {"stc", kRn, 0x0012, O::Gbr, O::RegN, 0, Flow::None, false},
    {"stc", kRn, 0x0022, O::Vbr, O::RegN, 0, Flow::None, false},
    {"bsrf", kRn, 0x0003, O::RegN, O::None, 0, Flow::Call, true},
    {"braf", kRn, 0x0023, O::RegN, O::None, 0, Flow::Indirect, true},
    {"mov.b", kRm, 0x0004, O::RegM, O::R0IndexN, 1, Flow::None, false},
    {"mov.w", kRm, 0x0005, O::RegM, O::R0IndexN, 2, Flow::None, false},
    {"mov.l", kRm, 0x0006, O::RegM, O::R0IndexN, 4, Flow::None, false},
    {"mul.l", kRm, 0x0007, O::RegM, O::RegN, 0, Flow::None, false},
    {"clrt", kAll, 0x0008, O::None, O::None, 0, Flow::None, false},
    {"sett", kAll, 0x0018, O::None, O::None, 0, Flow::None, false},
    {"clrmac", kAll, 0x0028, O::None, O::None, 0, Flow::None, false},
    {"nop", kAll, 0x0009, O::None, O::None, 0, Flow::None, false},
    {"div0u", kAll, 0x0019, O::None, O::None, 0, Flow::None, false},
    {"movt", kRn, 0x0029, O::RegN, O::None, 0, Flow::None, false},
    {"sts", kRn, 0x000A, O::Mach, O::RegN, 0, Flow::None, false},
    {"sts", kRn, 0x001A, O::Macl, O::RegN, 0, Flow::None, false},
    {"sts", kRn, 0x002A, O::Pr, O::RegN, 0, Flow::None, false},
    {"rts", kAll, 0x000B, O::None, O::None, 0, Flow::Return, true},
    {"sleep", kAll, 0x001B, O::None, O::None, 0, Flow::None, false},
    {"rte", kAll, 0x002B, O::None, O::None, 0, Flow::Return, true},
    {"mov.b", kRm, 0x000C, O::R0IndexM, O::RegN, 1, Flow::None, false},
    {"mov.w", kRm, 0x000D, O::R0IndexM, O::RegN, 2, Flow::None, false},
    {"mov.l", kRm, 0x000E, O::R0IndexM, O::RegN, 4, Flow::None, false},
    {"mac.l", kRm, 0x000F, O::PostM, O::PostN, 4, Flow::None, false},
    // 1nmd
    {"mov.l", kTop, 0x1000, O::RegM, O::DispN, 4, Flow::None, false},
    // 2nmx
    {"mov.b", kRm, 0x2000, O::RegM, O::IndN, 1, Flow::None, false},
    {"mov.w", kRm, 0x2001, O::RegM, O::IndN, 2, Flow::None, false},
    {"mov.l", kRm, 0x2002, O::RegM, O::IndN, 4, Flow::None, false},
    {"mov.b", kRm, 0x2004, O::RegM, O::PreN, 1, Flow::None, false},
    {"mov.w", kRm, 0x2005, O::RegM, O::PreN, 2, Flow::None, false},
    {"mov.l", kRm, 0x2006, O::RegM, O::PreN, 4, Flow::None, false},
    {"div0s", kRm, 0x2007, O::RegM, O::RegN, 0, Flow::None, false},
    {"tst", kRm, 0x2008, O::RegM, O::RegN, 0, Flow::None, false},
    {"and", kRm, 0x2009, O::RegM, O::RegN, 0, Flow::None, false},
    {"xor", kRm, 0x200A, O::RegM, O::RegN, 0, Flow::None, false},
    {"or", kRm, 0x200B, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/str", kRm, 0x200C, O::RegM, O::RegN, 0, Flow::None, false},
    {"xtrct", kRm, 0x200D, O::RegM, O::RegN, 0, Flow::None, false},
    {"mulu.w", kRm, 0x200E, O::RegM, O::RegN, 0, Flow::None, false},
    {"muls.w", kRm, 0x200F, O::RegM, O::RegN, 0, Flow::None, false},
    // 3nmx
    {"cmp/eq", kRm, 0x3000, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/hs", kRm, 0x3002, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/ge", kRm, 0x3003, O::RegM, O::RegN, 0, Flow::None, false},
    {"div1", kRm, 0x3004, O::RegM, O::RegN, 0, Flow::None, false},
    {"dmulu.l", kRm, 0x3005, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/hi", kRm, 0x3006, O::RegM, O::RegN, 0, Flow::None, false},
    {"cmp/gt", kRm, 0x3007, O::RegM, O::RegN, 0, Flow::None, false},
    {"sub", kRm, 0x3008, O::RegM, O::RegN, 0, Flow::None, false},
    {"subc", kRm, 0x300A, O::RegM, O::RegN, 0, Flow::None, false},
    {"subv", kRm, 0x300B, O::RegM, O::RegN, 0, Flow::None, false},
    {"add", kRm, 0x300C, O::RegM, O::RegN, 0, Flow::None, false},
    {"dmuls.l", kRm, 0x300D, O::RegM, O::RegN, 0, Flow::None, false},
    {"addc", kRm, 0x300E, O::RegM, O::RegN, 0, Flow::None, false},
    {"addv", kRm, 0x300F, O::RegM, O::RegN, 0, Flow::None, false},
    // 4nxx
    {"shll", kRn, 0x4000, O::RegN, O::None, 0, Flow::None, false},
    {"shlr", kRn, 0x4001, O::RegN, O::None, 0, Flow::None, false},
    {"sts.l", kRn, 0x4002, O::Mach, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x4003, O::Sr, O::PreN, 4, Flow::None, false},
    {"rotl", kRn, 0x4004, O::RegN, O::None, 0, Flow::None, false},
    {"rotr", kRn, 0x4005, O::RegN, O::None, 0, Flow::None, false},
    {"lds.l", kRn, 0x4006, O::PostN, O::Mach, 4, Flow::None, false},
    {"ldc.l", kRn, 0x4007, O::PostN, O::Sr, 4, Flow::None, false},
    {"shll2", kRn, 0x4008, O::RegN, O::None, 0, Flow::None, false},
    {"shlr2", kRn, 0x4009, O::RegN, O::None, 0, Flow::None, false},
    {"lds", kRn, 0x400A, O::RegN, O::Mach, 0, Flow::None, false},
    {"jsr", kRn, 0x400B, O::IndN, O::None, 0, Flow::Call, true},
    {"ldc", kRn, 0x400E, O::RegN, O::Sr, 0, Flow::None, false},
    {"mac.w", kRm, 0x400F, O::PostM, O::PostN, 2, Flow::None, false},
    {"dt", kRn, 0x4010, O::RegN, O::None, 0, Flow::None, false},
    {"cmp/pz", kRn, 0x4011, O::RegN, O::None, 0, Flow::None, false},
    {"sts.l", kRn, 0x4012, O::Macl, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x4013, O::Gbr, O::PreN, 4, Flow::None, false},
    {"cmp/pl", kRn, 0x4015, O::RegN, O::None, 0, Flow::None, false},
    {"lds.l", kRn, 0x4016, O::PostN, O::Macl, 4, Flow::None, false},
    {"ldc.l", kRn, 0x4017, O::PostN, O::Gbr, 4, Flow::None, false},
    {"shll8", kRn, 0x4018, O::RegN, O::None, 0, Flow::None, false},
    {"shlr8", kRn, 0x4019, O::RegN, O::None, 0, Flow::None, false},
    {"lds", kRn, 0x401A, O::RegN, O::Macl, 0, Flow::None, false},
    {"tas.b", kRn, 0x401B, O::IndN, O::None, 1, Flow::None, false},
    {"ldc", kRn, 0x401E, O::RegN, O::Gbr, 0, Flow::None, false},
    {"shal", kRn, 0x4020, O::RegN, O::None, 0, Flow::None, false},
    {"shar", kRn, 0x4021, O::RegN, O::None, 0, Flow::None, false},
    {"sts.l", kRn, 0x4022, O::Pr, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x4023, O::Vbr, O::PreN, 4, Flow::None, false},
    {"rotcl", kRn, 0x4024, O::RegN, O::None, 0, Flow::None, false},
    {"rotcr", kRn, 0x4025, O::RegN, O::None, 0, Flow::None, false},
    {"lds.l", kRn, 0x4026, O::PostN, O::Pr, 4, Flow::None, false},
    {"ldc.l", kRn, 0x4027, O::PostN, O::Vbr, 4, Flow::None, false},
    {"shll16", kRn, 0x4028, O::RegN, O::None, 0, Flow::None, false},
    {"shlr16", kRn, 0x4029, O::RegN, O::None, 0, Flow::None, false},
    {"lds", kRn, 0x402A, O::RegN, O::Pr, 0, Flow::None, false},
    {"jmp", kRn, 0x402B, O::IndN, O::None, 0, Flow::Indirect, true},
    {"ldc", kRn, 0x402E, O::RegN, O::Vbr, 0, Flow::None, false},
    // 5nmd
    {"mov.l", kTop, 0x5000, O::DispM, O::RegN, 4, Flow::None, false},
    // 6nmx
    {"mov.b", kRm, 0x6000, O::IndM, O::RegN, 1, Flow::None, false},
    {"mov.w", kRm, 0x6001, O::IndM, O::RegN, 2, Flow::None, false},
    {"mov.l", kRm, 0x6002, O::IndM, O::RegN, 4, Flow::None, false},
    {"mov", kRm, 0x6003, O::RegM, O::RegN, 0, Flow::None, false},
    {"mov.b", kRm, 0x6004, O::PostM, O::RegN, 1, Flow::None, false},
    {"mov.w", kRm, 0x6005, O::PostM, O::RegN, 2, Flow::None, false},
    {"mov.l", kRm, 0x6006, O::PostM, O::RegN, 4, Flow::None, false},
    {"not", kRm, 0x6007, O::RegM, O::RegN, 0, Flow::None, false},
    {"swap.b", kRm, 0x6008, O::RegM, O::RegN, 0, Flow::None, false},
    {"swap.w", kRm, 0x6009, O::RegM, O::RegN, 0, Flow::None, false},
    {"negc", kRm, 0x600A, O::RegM, O::RegN, 0, Flow::None, false},
    {"neg", kRm, 0x600B, O::RegM, O::RegN, 0, Flow::None, false},
    {"extu.b", kRm, 0x600C, O::RegM, O::RegN, 0, Flow::None, false},
    {"extu.w", kRm, 0x600D, O::RegM, O::RegN, 0, Flow::None, false},
    {"exts.b", kRm, 0x600E, O::RegM, O::RegN, 0, Flow::None, false},
    {"exts.w", kRm, 0x600F, O::RegM, O::RegN, 0, Flow::None, false},
    // 7nii
    {"add", kTop, 0x7000, O::ImmS, O::RegN, 0, Flow::None, false},
    // 8xxx
    {"mov.b", kByte, 0x8000, O::R0, O::DispM, 1, Flow::None, false},
    {"mov.w", kByte, 0x8100, O::R0, O::DispM, 2, Flow::None, false},
    {"mov.b", kByte, 0x8400, O::DispM, O::R0, 1, Flow::None, false},
    {"mov.w", kByte, 0x8500, O::DispM, O::R0, 2, Flow::None, false},
    {"cmp/eq", kByte, 0x8800, O::ImmS, O::R0, 0, Flow::None, false},
    {"bt", kByte, 0x8900, O::Branch8, O::None, 0, Flow::Branch, false},
    {"bf", kByte, 0x8B00, O::Branch8, O::None, 0, Flow::Branch, false},
    {"bt/s", kByte, 0x8D00, O::Branch8, O::None, 0, Flow::Branch, true},
    {"bf/s", kByte, 0x8F00, O::Branch8, O::None, 0, Flow::Branch, true},
    // 9ndd, Axxx, Bxxx
    {"mov.w", kTop, 0x9000, O::PcDisp, O::RegN, 2, Flow::None, false},
    {"bra", kTop, 0xA000, O::Branch12, O::None, 0, Flow::Jump, true},
    {"bsr", kTop, 0xB000, O::Branch12, O::None, 0, Flow::Call, true},
    // Cxxx
    {"mov.b", kByte, 0xC000, O::R0, O::GbrDisp, 1, Flow::None, false},
    {"mov.w", kByte, 0xC100, O::R0, O::GbrDisp, 2, Flow::None, false},
    {"mov.l", kByte, 0xC200, O::R0, O::GbrDisp, 4, Flow::None, false},
    {"trapa", kByte, 0xC300, O::ImmU, O::None, 0, Flow::None, false},
    {"mov.b", kByte, 0xC400, O::GbrDisp, O::R0, 1, Flow::None, false},
    {"mov.w", kByte, 0xC500, O::GbrDisp, O::R0, 2, Flow::None, false},
    {"mov.l", kByte, 0xC600, O::GbrDisp, O::R0, 4, Flow::None, false},
    {"mova", kByte, 0xC700, O::PcDisp, O::R0, 4, Flow::None, false},
    {"tst", kByte, 0xC800, O::ImmU, O::R0, 0, Flow::None, false},
    {"and", kByte, 0xC900, O::ImmU, O::R0, 0, Flow::None, false},
    {"xor", kByte, 0xCA00, O::ImmU, O::R0, 0, Flow::None, false},
    {"or", kByte, 0xCB00, O::ImmU, O::R0, 0, Flow::None, false},
    {"tst.b", kByte, 0xCC00, O::ImmU, O::GbrR0, 1, Flow::None, false},
    {"and.b", kByte, 0xCD00, O::ImmU, O::GbrR0, 1, Flow::None, false},
    {"xor.b", kByte, 0xCE00, O::ImmU, O::GbrR0, 1, Flow::None, false},
    {"or.b", kByte, 0xCF00, O::ImmU, O::GbrR0, 1, Flow::None, false},
    // Dndd, Enii
    {"mov.l", kTop, 0xD000, O::PcDisp, O::RegN, 4, Flow::None, false},
    {"mov", kTop, 0xE000, O::ImmS, O::RegN, 0, Flow::None, false},
});

// What the SH-4 adds (SH7750). Operand names say where the register field is: N is bits 8-11, M bits 4-7.
constexpr std::uint16_t kBankMask = 0xF08F;   // rn in bits 8-11, the bank register in bits 4-6
constexpr std::uint16_t kEven = 0xF1FF;       // a double register: bit 8 clear
constexpr auto kDefs4 = std::to_array<Def>({
    // 0xxx: control registers, cache and TLB instructions
    {"stc", kRn, 0x0032, O::Ssr, O::RegN, 0, Flow::None, false},
    {"stc", kRn, 0x0042, O::Spc, O::RegN, 0, Flow::None, false},
    {"stc", kRn, 0x003A, O::Sgr, O::RegN, 0, Flow::None, false},
    {"stc", kRn, 0x00FA, O::Dbr, O::RegN, 0, Flow::None, false},
    {"stc", kBankMask, 0x0082, O::BankM, O::RegN, 0, Flow::None, false},
    {"pref", kRn, 0x0083, O::IndN, O::None, 0, Flow::None, false},
    {"ocbi", kRn, 0x0093, O::IndN, O::None, 0, Flow::None, false},
    {"ocbp", kRn, 0x00A3, O::IndN, O::None, 0, Flow::None, false},
    {"ocbwb", kRn, 0x00B3, O::IndN, O::None, 0, Flow::None, false},
    {"movca.l", kRn, 0x00C3, O::R0, O::IndN, 4, Flow::None, false},
    {"ldtlb", kAll, 0x0038, O::None, O::None, 0, Flow::None, false},
    {"clrs", kAll, 0x0048, O::None, O::None, 0, Flow::None, false},
    {"sets", kAll, 0x0058, O::None, O::None, 0, Flow::None, false},
    {"sts", kRn, 0x005A, O::Fpul, O::RegN, 0, Flow::None, false},
    {"sts", kRn, 0x006A, O::Fpscr, O::RegN, 0, Flow::None, false},
    // 4nxx
    {"shad", kRm, 0x400C, O::RegM, O::RegN, 0, Flow::None, false},
    {"shld", kRm, 0x400D, O::RegM, O::RegN, 0, Flow::None, false},
    {"stc.l", kRn, 0x4033, O::Ssr, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x4043, O::Spc, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x4032, O::Sgr, O::PreN, 4, Flow::None, false},
    {"stc.l", kRn, 0x40F2, O::Dbr, O::PreN, 4, Flow::None, false},
    {"stc.l", kBankMask, 0x4083, O::BankM, O::PreN, 4, Flow::None, false},
    {"ldc.l", kRn, 0x4037, O::PostN, O::Ssr, 4, Flow::None, false},
    {"ldc.l", kRn, 0x4047, O::PostN, O::Spc, 4, Flow::None, false},
    {"ldc.l", kRn, 0x40F6, O::PostN, O::Dbr, 4, Flow::None, false},
    {"ldc.l", kBankMask, 0x4087, O::PostN, O::BankM, 4, Flow::None, false},
    {"ldc", kRn, 0x403E, O::RegN, O::Ssr, 0, Flow::None, false},
    {"ldc", kRn, 0x404E, O::RegN, O::Spc, 0, Flow::None, false},
    {"ldc", kRn, 0x40FA, O::RegN, O::Dbr, 0, Flow::None, false},
    {"ldc", kBankMask, 0x408E, O::RegN, O::BankM, 0, Flow::None, false},
    {"sts.l", kRn, 0x4052, O::Fpul, O::PreN, 4, Flow::None, false},
    {"sts.l", kRn, 0x4062, O::Fpscr, O::PreN, 4, Flow::None, false},
    {"lds.l", kRn, 0x4056, O::PostN, O::Fpul, 4, Flow::None, false},
    {"lds.l", kRn, 0x4066, O::PostN, O::Fpscr, 4, Flow::None, false},
    {"lds", kRn, 0x405A, O::RegN, O::Fpul, 0, Flow::None, false},
    {"lds", kRn, 0x406A, O::RegN, O::Fpscr, 0, Flow::None, false},
    // Fxxx: the FPU
    {"fadd", kRm, 0xF000, O::FrM, O::FrN, 0, Flow::None, false},
    {"fsub", kRm, 0xF001, O::FrM, O::FrN, 0, Flow::None, false},
    {"fmul", kRm, 0xF002, O::FrM, O::FrN, 0, Flow::None, false},
    {"fdiv", kRm, 0xF003, O::FrM, O::FrN, 0, Flow::None, false},
    {"fcmp/eq", kRm, 0xF004, O::FrM, O::FrN, 0, Flow::None, false},
    {"fcmp/gt", kRm, 0xF005, O::FrM, O::FrN, 0, Flow::None, false},
    {"fmov.s", kRm, 0xF006, O::R0IndexM, O::MovN, 4, Flow::None, false},
    {"fmov.s", kRm, 0xF007, O::MovM, O::R0IndexN, 4, Flow::None, false},
    {"fmov.s", kRm, 0xF008, O::IndM, O::MovN, 4, Flow::None, false},
    {"fmov.s", kRm, 0xF009, O::PostM, O::MovN, 4, Flow::None, false},
    {"fmov.s", kRm, 0xF00A, O::MovM, O::IndN, 4, Flow::None, false},
    {"fmov.s", kRm, 0xF00B, O::MovM, O::PreN, 4, Flow::None, false},
    {"fmov", kRm, 0xF00C, O::MovM, O::MovN, 0, Flow::None, false},
    {"fmac", kRm, 0xF00E, O::FrM, O::FrN, 0, Flow::None, false},   // fmac fr0,frm,frn (fr0 is written in the text, not an operand)
    {"fsts", kRn, 0xF00D, O::Fpul, O::FrN, 0, Flow::None, false},
    {"flds", kRn, 0xF01D, O::FrN, O::Fpul, 0, Flow::None, false},
    {"float", kRn, 0xF02D, O::Fpul, O::FrN, 0, Flow::None, false},
    {"ftrc", kRn, 0xF03D, O::FrN, O::Fpul, 0, Flow::None, false},
    {"fneg", kRn, 0xF04D, O::FrN, O::None, 0, Flow::None, false},
    {"fabs", kRn, 0xF05D, O::FrN, O::None, 0, Flow::None, false},
    {"fsqrt", kRn, 0xF06D, O::FrN, O::None, 0, Flow::None, false},
    {"fsrra", kRn, 0xF07D, O::FrN, O::None, 0, Flow::None, false},
    {"fldi0", kRn, 0xF08D, O::FrN, O::None, 0, Flow::None, false},
    {"fldi1", kRn, 0xF09D, O::FrN, O::None, 0, Flow::None, false},
    {"fcnvsd", kEven, 0xF0AD, O::Fpul, O::DrN, 0, Flow::None, false},
    {"fcnvds", kEven, 0xF0BD, O::DrN, O::Fpul, 0, Flow::None, false},
    {"fipr", kRn, 0xF0ED, O::FvM, O::FvN, 0, Flow::None, false},
    {"ftrv", 0xF3FF, 0xF1FD, O::Xmtrx, O::FvN, 0, Flow::None, false},
    {"fsca", kEven, 0xF0FD, O::Fpul, O::DrN, 0, Flow::None, false},
    {"fschg", kAll, 0xF3FD, O::None, O::None, 0, Flow::None, false},
    {"frchg", kAll, 0xFBFD, O::None, O::None, 0, Flow::None, false},
});

// The definitions whose first nibble is `top`, in table order (the SH-4's are the SH-2's and then its own).
const std::array<std::vector<const Def*>, 16>& by_top_nibble(Variant variant) {
    static const std::array<std::array<std::vector<const Def*>, 16>, 2> tables = [] {
        std::array<std::array<std::vector<const Def*>, 16>, 2> t;
        for (const Def& d : kDefs) {
            t[0][d.bits >> 12].push_back(&d);
            t[1][d.bits >> 12].push_back(&d);
        }
        for (const Def& d : kDefs4) t[1][d.bits >> 12].push_back(&d);
        return t;
    }();
    return tables[variant == Variant::Sh4 ? 1 : 0];
}

std::int32_t sign_extend(std::uint32_t v, unsigned bits) {
    const std::uint32_t top = 1u << (bits - 1);
    return static_cast<std::int32_t>((v ^ top) - top);
}


}  // namespace

std::optional<Insn> decode_word(std::uint16_t word, std::uint32_t pc, Variant variant) {
    for (const Def* d : by_top_nibble(variant)[word >> 12]) {
        if ((word & d->mask) != d->bits) continue;
        Insn i;
        i.word = word;
        i.pc = pc;
        i.mnemonic = std::string(d->name);
        i.a = d->a;
        i.b = d->b;
        i.n = (word >> 8) & 15;
        i.m = (word >> 4) & 15;
        i.size = d->size;
        i.flow = d->flow;
        i.delay_slot = d->delay;
        i.raw = little_endian(variant) ? std::array<std::uint8_t, 2>{static_cast<std::uint8_t>(word), static_cast<std::uint8_t>(word >> 8)}
                                       : std::array<std::uint8_t, 2>{static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
        for (const O o : {d->a, d->b}) {
            switch (o) {
                case O::ImmS: i.imm = sign_extend(word & 0xFF, 8); break;
                case O::ImmU: i.imm = word & 0xFF; break;
                case O::DispN: case O::DispM: i.imm = static_cast<std::int32_t>(word & 15) * d->size; break;
                case O::GbrDisp: i.imm = static_cast<std::int32_t>(word & 0xFF) * d->size; break;
                case O::PcDisp: {
                    const std::uint32_t base = d->size == 4 ? (pc + 4) & ~3u : pc + 4;
                    i.imm = static_cast<std::int32_t>(word & 0xFF) * d->size;
                    i.literal = base + static_cast<std::uint32_t>(i.imm);
                    break;
                }
                case O::Branch8: i.target = pc + 4 + static_cast<std::uint32_t>(sign_extend(word & 0xFF, 8) * 2); break;
                case O::Branch12: i.target = pc + 4 + static_cast<std::uint32_t>(sign_extend(word & 0xFFF, 12) * 2); break;
                default: break;
            }
        }
        return i;
    }
    return std::nullopt;
}

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc, Variant variant) {
    if (at + 2 > code.size()) return std::nullopt;
    const std::uint16_t word = little_endian(variant) ? static_cast<std::uint16_t>(code[at + 1] << 8 | code[at]) : static_cast<std::uint16_t>(code[at] << 8 | code[at + 1]);
    return decode_word(word, pc, variant);
}

// ---- text -------------------------------------------------------------------------------------------------------

std::string_view register_name(unsigned number) {
    static constexpr std::array<std::string_view, 16> names = {"r0", "r1", "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
                                                               "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
    return names.at(number & 15);
}

std::optional<unsigned> register_number(std::string_view t) {
    t = text::trim(t);
    if (t.size() == 2 && (t[0] == 's' || t[0] == 'S') && (t[1] == 'p' || t[1] == 'P')) return 15;
    if (t.size() < 2 || t.size() > 3 || (t[0] != 'r' && t[0] != 'R')) return std::nullopt;
    const auto n = text::parse_int<unsigned>(t.substr(1));
    if (!n || *n > 15) return std::nullopt;
    if (t.size() == 3 && t[1] == '0') return std::nullopt;   // (r05 is not a register's name)
    return *n;
}

namespace {

std::string address_text(std::uint32_t address, const AddressLabels& labels) {
    const auto it = labels.find(address);
    if (it != labels.end() && !it->second.empty()) return it->second;
    return std::format("0x{:08X}", address);
}

std::string operand_text(const Insn& i, O o, const AddressLabels& labels) {
    switch (o) {
        case O::None: return {};
        case O::RegN: return std::string(register_name(i.n));
        case O::RegM: return std::string(register_name(i.m));
        case O::R0: return "r0";
        case O::ImmS: return std::format("#{}", i.imm);
        case O::ImmU: return i.imm > 9 ? std::format("#0x{:X}", i.imm) : std::format("#{}", i.imm);
        case O::IndN: return std::format("@{}", register_name(i.n));
        case O::IndM: return std::format("@{}", register_name(i.m));
        case O::PostN: return std::format("@{}+", register_name(i.n));
        case O::PostM: return std::format("@{}+", register_name(i.m));
        case O::PreN: return std::format("@-{}", register_name(i.n));
        case O::DispN: return std::format("@({},{})", i.imm, register_name(i.n));
        case O::DispM: return std::format("@({},{})", i.imm, register_name(i.m));
        case O::R0IndexN: return std::format("@(r0,{})", register_name(i.n));
        case O::R0IndexM: return std::format("@(r0,{})", register_name(i.m));
        case O::GbrDisp: return std::format("@({},gbr)", i.imm);
        case O::GbrR0: return "@(r0,gbr)";
        case O::PcDisp: {
            const auto it = labels.find(*i.literal);
            if (it != labels.end() && !it->second.empty()) return it->second;
            return std::format("@({},pc)", i.imm);
        }
        case O::Branch8: case O::Branch12: return address_text(*i.target, labels);
        case O::Sr: return "sr";
        case O::Gbr: return "gbr";
        case O::Vbr: return "vbr";
        case O::Mach: return "mach";
        case O::Macl: return "macl";
        case O::Pr: return "pr";
        case O::FrN: case O::MovN: return std::format("fr{}", i.n);
        case O::FrM: case O::MovM: return std::format("fr{}", i.m);
        case O::DrN: return std::format("dr{}", i.n);
        case O::FvN: return std::format("fv{}", 4 * ((i.word >> 10) & 3));
        case O::FvM: return std::format("fv{}", 4 * ((i.word >> 8) & 3));
        case O::Xmtrx: return "xmtrx";
        case O::Fpul: return "fpul";
        case O::Fpscr: return "fpscr";
        case O::Ssr: return "ssr";
        case O::Spc: return "spc";
        case O::Sgr: return "sgr";
        case O::Dbr: return "dbr";
        case O::BankM: return std::format("r{}_bank", (i.word >> 4) & 7);
    }
    return {};
}

}  // namespace

std::string text(const Insn& insn, const AddressLabels& labels) {
    const std::string a = operand_text(insn, insn.a, labels);
    if (a.empty()) return insn.mnemonic;
    const std::string b = operand_text(insn, insn.b, labels);
    if (insn.mnemonic == "fmac") return std::format("{:<8}fr0,{},{}", insn.mnemonic, a, b);   // (fr0 is the third operand: the word has no field for it)
    return std::format("{:<8}{}", insn.mnemonic, b.empty() ? a : a + "," + b);
}

// ---- expressions ------------------------------------------------------------------------------------------------

std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc) { return mips::value(expr, labels, pc); }

// ---- assembling -------------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void fail(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

// One operand of an assembly line, by its shape.
struct Parsed {
    enum class Kind : std::uint8_t { Reg, Imm, Ind, Post, Pre, DispReg, R0Index, GbrDisp, GbrR0, PcDisp, Control, Fp, Bank, Bare };
    Kind kind = Kind::Bare;
    unsigned reg = 0;
    char fclass = 0;   // Fp: 'f' frN, 'd' drN, 'x' xdN, 'v' fvN
    std::string_view expr;
    O control = O::None;
};

std::optional<O> control_register(std::string_view t) {
    const std::string name = text::lower(t);
    if (name == "sr") return O::Sr;
    if (name == "gbr") return O::Gbr;
    if (name == "vbr") return O::Vbr;
    if (name == "mach") return O::Mach;
    if (name == "macl") return O::Macl;
    if (name == "pr") return O::Pr;
    if (name == "ssr") return O::Ssr;
    if (name == "spc") return O::Spc;
    if (name == "sgr") return O::Sgr;
    if (name == "dbr") return O::Dbr;
    if (name == "fpul") return O::Fpul;
    if (name == "fpscr") return O::Fpscr;
    if (name == "xmtrx") return O::Xmtrx;
    return std::nullopt;
}

// Split `text` at its commas that are not inside parentheses.
std::vector<std::string_view> operands_of(std::string_view t) {
    std::vector<std::string_view> out;
    int depth = 0;
    std::size_t from = 0;
    for (std::size_t i = 0; i < t.size(); ++i) {
        if (t[i] == '(') ++depth;
        else if (t[i] == ')') --depth;
        else if (t[i] == ',' && depth == 0) {
            out.push_back(text::trim(t.substr(from, i - from)));
            from = i + 1;
        }
    }
    if (!text::trim(t.substr(from)).empty() || !out.empty()) out.push_back(text::trim(t.substr(from)));
    return out;
}

// fr0-fr15, dr0-dr14, xd0-xd14 and fv0-fv12: the class ('f' 'd' 'x' 'v') and the number.
std::optional<std::pair<char, unsigned>> fp_register(std::string_view t) {
    const std::string name = text::lower(t);
    if (name.size() < 3 || name.size() > 4) return std::nullopt;
    char cls = 0;
    if (name.starts_with("fr")) cls = 'f';
    else if (name.starts_with("dr")) cls = 'd';
    else if (name.starts_with("xd")) cls = 'x';
    else if (name.starts_with("fv")) cls = 'v';
    else return std::nullopt;
    const auto n = text::parse_int<unsigned>(std::string_view(name).substr(2));
    if (!n || *n > 15 || (name.size() == 4 && name[2] == '0')) return std::nullopt;
    return std::pair{cls, *n};
}

// r0_bank .. r7_bank: the number.
std::optional<unsigned> bank_register(std::string_view t) {
    const std::string name = text::lower(t);
    if (name.size() != 7 || name[0] != 'r' || name.substr(2) != "_bank" || name[1] < '0' || name[1] > '7') return std::nullopt;
    return static_cast<unsigned>(name[1] - '0');
}

Parsed parse_operand(std::string_view line, std::string_view t) {
    t = text::trim(t);
    if (t.empty()) fail(line, "an operand is missing");
    Parsed p;
    if (t[0] == '#') {
        p.kind = Parsed::Kind::Imm;
        p.expr = text::trim(t.substr(1));
        return p;
    }
    if (t[0] == '@') {
        const std::string_view rest = text::trim(t.substr(1));
        if (rest.starts_with('-')) {
            const auto r = register_number(rest.substr(1));
            if (!r) fail(line, "a register is missing after @-");
            p.kind = Parsed::Kind::Pre;
            p.reg = *r;
            return p;
        }
        if (rest.starts_with('(')) {
            if (!rest.ends_with(')')) fail(line, "a ')' is missing");
            const std::vector<std::string_view> parts = operands_of(rest.substr(1, rest.size() - 2));
            if (parts.size() != 2) fail(line, "@(disp,register) takes a displacement and a register");
            const auto base = register_number(parts[1]);
            const bool first_r0 = register_number(parts[0]) == 0u;
            if (base) {
                if (first_r0) {
                    p.kind = Parsed::Kind::R0Index;
                } else {
                    p.kind = Parsed::Kind::DispReg;
                    p.expr = parts[0];
                }
                p.reg = *base;
                return p;
            }
            const std::string second = text::lower(parts[1]);
            if (second == "gbr") {
                p.kind = first_r0 ? Parsed::Kind::GbrR0 : Parsed::Kind::GbrDisp;
                p.expr = parts[0];
                return p;
            }
            if (second == "pc") {
                p.kind = Parsed::Kind::PcDisp;
                p.expr = parts[0];
                return p;
            }
            fail(line, "the base of @(disp,base) is a register, gbr or pc");
        }
        const bool post = rest.ends_with('+');
        const auto r = register_number(post ? rest.substr(0, rest.size() - 1) : rest);
        if (!r) fail(line, "a register is missing after @");
        p.kind = post ? Parsed::Kind::Post : Parsed::Kind::Ind;
        p.reg = *r;
        return p;
    }
    if (const auto r = register_number(t)) {
        p.kind = Parsed::Kind::Reg;
        p.reg = *r;
        return p;
    }
    if (const auto fp = fp_register(t)) {
        p.kind = Parsed::Kind::Fp;
        p.fclass = fp->first;
        p.reg = fp->second;
        return p;
    }
    if (const auto bank = bank_register(t)) {
        p.kind = Parsed::Kind::Bank;
        p.reg = *bank;
        return p;
    }
    if (const auto c = control_register(t)) {
        p.kind = Parsed::Kind::Control;
        p.control = *c;
        return p;
    }
    p.kind = Parsed::Kind::Bare;
    p.expr = t;
    return p;
}

// Every definition of a variant, in table order.
const std::vector<const Def*>& all_defs(Variant variant) {
    static const std::array<std::vector<const Def*>, 2> lists = [] {
        std::array<std::vector<const Def*>, 2> l;
        for (const Def& d : kDefs) {
            l[0].push_back(&d);
            l[1].push_back(&d);
        }
        for (const Def& d : kDefs4) l[1].push_back(&d);
        return l;
    }();
    return lists[variant == Variant::Sh4 ? 1 : 0];
}

// Can an operand written like `p` be the table's operand `o`?
bool shape_matches(O o, const Parsed& p) {
    using K = Parsed::Kind;
    switch (o) {
        case O::None: return false;
        case O::RegN: case O::RegM: return p.kind == K::Reg;
        case O::R0: return p.kind == K::Reg && p.reg == 0;
        case O::ImmS: case O::ImmU: return p.kind == K::Imm;
        case O::IndN: case O::IndM: return p.kind == K::Ind;
        case O::PostN: case O::PostM: return p.kind == K::Post;
        case O::PreN: return p.kind == K::Pre;
        case O::DispN: case O::DispM: return p.kind == K::DispReg;
        case O::R0IndexN: case O::R0IndexM: return p.kind == K::R0Index;
        case O::GbrDisp: return p.kind == K::GbrDisp;
        case O::GbrR0: return p.kind == K::GbrR0;
        case O::PcDisp: return p.kind == K::PcDisp || p.kind == K::Bare;
        case O::Branch8: case O::Branch12: return p.kind == K::Bare;
        case O::Sr: case O::Gbr: case O::Vbr: case O::Mach: case O::Macl: case O::Pr: return p.kind == K::Control && p.control == o;
        case O::FrN: case O::FrM: return p.kind == K::Fp && (p.fclass == 'f' || (p.fclass == 'd' && p.reg % 2 == 0));
        case O::MovN: case O::MovM: return p.kind == K::Fp && (p.fclass == 'f' || ((p.fclass == 'd' || p.fclass == 'x') && p.reg % 2 == 0));
        case O::DrN: return p.kind == K::Fp && p.fclass == 'd' && p.reg % 2 == 0;
        case O::FvN: case O::FvM: return p.kind == K::Fp && p.fclass == 'v' && p.reg % 4 == 0;
        case O::Xmtrx: case O::Fpul: case O::Fpscr: case O::Ssr: case O::Spc: case O::Sgr: case O::Dbr: return p.kind == K::Control && p.control == o;
        case O::BankM: return p.kind == K::Bank;
    }
    return false;
}

// A displacement in bytes as its field: a multiple of the unit, 0 up to `limit` units.
std::uint32_t scaled(std::string_view line, std::int64_t bytes, unsigned unit, unsigned limit, std::string_view what) {
    unit = std::max(unit, 1u);
    if (bytes < 0 || bytes % unit != 0 || bytes / unit > static_cast<std::int64_t>(limit))
        fail(line, std::format("{} is not a displacement of {} to {} bytes in steps of {}", bytes, what, limit * unit, unit));
    return static_cast<std::uint32_t>(bytes / unit);
}

}  // namespace

Bytes assemble(std::string_view line_in, std::uint32_t pc, const NameLabels& labels, Variant variant) {
    const std::string_view line = text::trim(line_in);
    std::size_t split = 0;
    while (split < line.size() && !text::is_space(line[split])) ++split;
    std::string mnemonic = text::lower(line.substr(0, split));
    if (mnemonic.empty()) fail(line, "no instruction");
    const std::vector<std::string_view> raw = operands_of(text::trim(line.substr(split)));
    std::vector<Parsed> operands;
    for (const std::string_view r : raw) operands.push_back(parse_operand(line, r));
    if (variant == Variant::Sh4) {
        // other spellings of the same words: `fmov.d` is the pair move `fmov.s` is when SZ=1, and `fmov` of a memory operand is `fmov.s`
        const bool memory = std::ranges::any_of(operands, [](const Parsed& p) { return p.kind != Parsed::Kind::Fp; });
        if (mnemonic == "fmov" || mnemonic == "fmov.s" || mnemonic == "fmov.d") mnemonic = memory ? "fmov.s" : "fmov";
        // fmac fr0,frm,frn: the first operand names a register the word has no field for
        if (mnemonic == "fmac" && operands.size() == 3) {
            if (operands[0].kind != Parsed::Kind::Fp || operands[0].fclass != 'f' || operands[0].reg != 0) fail(line, "fmac multiplies into fr0: its first operand is fr0");
            operands.erase(operands.begin());
        }
    }

    bool known = false;
    for (const Def* def : all_defs(variant)) {
        const Def& d = *def;
        if (d.name != mnemonic) continue;
        known = true;
        const std::size_t wanted = d.a == O::None ? 0 : d.b == O::None ? 1 : 2;
        if (wanted != operands.size()) continue;
        if (wanted >= 1 && !shape_matches(d.a, operands[0])) continue;
        if (wanted == 2 && !shape_matches(d.b, operands[1])) continue;

        std::uint32_t word = d.bits;
        for (std::size_t k = 0; k < wanted; ++k) {
            const O o = k == 0 ? d.a : d.b;
            const Parsed& p = operands[k];
            const auto number = [&](std::string_view expr) { return value(expr, labels, pc); };
            switch (o) {
                case O::RegN: case O::IndN: case O::PostN: case O::PreN: case O::R0IndexN: word |= p.reg << 8; break;
                case O::RegM: case O::IndM: case O::PostM: case O::R0IndexM: word |= p.reg << 4; break;
                case O::ImmS: {
                    const std::int64_t v = number(p.expr);
                    if (v < -128 || v > 255) fail(line, std::format("#{} does not fit in 8 bits", v));
                    word |= static_cast<std::uint32_t>(v) & 0xFF;
                    break;
                }
                case O::ImmU: {
                    const std::int64_t v = number(p.expr);
                    if (v < 0 || v > 255) fail(line, std::format("#{} does not fit in 8 bits", v));
                    word |= static_cast<std::uint32_t>(v);
                    break;
                }
                case O::DispN: word |= p.reg << 8 | scaled(line, number(p.expr), d.size, 15, "@(disp,rn)"); break;
                case O::DispM: word |= p.reg << 4 | scaled(line, number(p.expr), d.size, 15, "@(disp,rm)"); break;
                case O::GbrDisp: word |= scaled(line, number(p.expr), d.size, 255, "@(disp,gbr)"); break;
                case O::PcDisp: {
                    const std::uint32_t base = d.size == 4 ? (pc + 4) & ~3u : pc + 4;
                    const std::int64_t bytes = p.kind == Parsed::Kind::PcDisp ? number(p.expr) : number(p.expr) - static_cast<std::int64_t>(base);
                    word |= scaled(line, bytes, d.size, 255, "@(disp,pc)");
                    break;
                }
                case O::FrN: word |= p.reg << 8; break;
                case O::FrM: word |= p.reg << 4; break;
                case O::MovN: word |= (p.reg | (p.fclass == 'x' ? 1u : 0u)) << 8; break;
                case O::MovM: word |= (p.reg | (p.fclass == 'x' ? 1u : 0u)) << 4; break;
                case O::DrN: word |= p.reg << 8; break;
                case O::FvN: word |= (p.reg / 4) << 10; break;
                case O::FvM: word |= (p.reg / 4) << 8; break;
                case O::BankM: word |= p.reg << 4; break;
                case O::Branch8: case O::Branch12: {
                    const std::int64_t bytes = number(p.expr) - (static_cast<std::int64_t>(pc) + 4);
                    const std::int64_t half = bytes / 2;
                    const std::int64_t limit = o == O::Branch8 ? 128 : 2048;
                    if (bytes % 2 != 0 || half < -limit || half >= limit) fail(line, "the branch target is out of reach");
                    word |= static_cast<std::uint32_t>(half) & (o == O::Branch8 ? 0xFF : 0xFFF);
                    break;
                }
                default: break;
            }
        }
        if (little_endian(variant)) return {static_cast<std::uint8_t>(word), static_cast<std::uint8_t>(word >> 8)};
        return {static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
    }
    fail(line, known ? "the operands do not fit the instruction" : "unknown instruction " + mnemonic);
}

}  // namespace cb::decompile::sh2
