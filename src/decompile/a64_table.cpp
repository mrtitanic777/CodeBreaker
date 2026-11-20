// cb-unity: alone
// decompile/a64_table.cpp -- the rows of the A64 table for the integer instructions, the loads and stores, and the scalar floating point (decompile/a64_internal.hpp says how a
// row is written). Rows are in preferred-disassembly order: an alias stands before the row it is a special case of.
#include <format>
#include <string>

#include "decompile/a64_internal.hpp"

namespace cb::decompile::a64::detail {

namespace {

// ---- predicates ---------------------------------------------------------------------------------------------------------------------------------------------------

bool sf_of(std::uint32_t w) { return (w >> 31) & 1; }

bool mov_sp(std::uint32_t w) { return bits(w, 0, 5) == 31 || bits(w, 5, 5) == 31; }

bool li_ok(std::uint32_t w) { return decode_bitmask(bits(w, 22, 1), bits(w, 10, 6), bits(w, 16, 6), sf_of(w) ? 64 : 32).has_value(); }

// Can a move-wide instruction (movz, or movn of the complement) make this value? Then `mov` of it is that and not the bitmask form.
bool one_chunk(std::uint64_t v, unsigned size) {
    for (unsigned hw = 0; hw < size / 16; ++hw)
        if ((v & ~(0xFFFFull << (16 * hw))) == 0) return true;
    return false;
}
bool mov_bitmask(std::uint32_t w) {
    const unsigned size = sf_of(w) ? 64 : 32;
    const std::optional<std::uint64_t> v = decode_bitmask(bits(w, 22, 1), bits(w, 10, 6), bits(w, 16, 6), size);
    if (!v) return false;
    const std::uint64_t mask = size == 64 ? ~0ull : 0xFFFFFFFFull;
    return !one_chunk(*v, size) && !one_chunk(~*v & mask, size);
}

bool mw_ok(std::uint32_t w) { return sf_of(w) || bits(w, 21, 2) < 2; }
bool movz_alias(std::uint32_t w) { return mw_ok(w) && !(bits(w, 5, 16) == 0 && bits(w, 21, 2) != 0); }
bool movn_alias(std::uint32_t w) { return movz_alias(w) && !(!sf_of(w) && bits(w, 5, 16) == 0xFFFF); }

bool bf_ok(std::uint32_t w) { return bits(w, 22, 1) == (sf_of(w) ? 1u : 0u) && (sf_of(w) || (!(bits(w, 21, 1)) && !(bits(w, 15, 1)))); }
unsigned bf_r(std::uint32_t w) { return bits(w, 16, 6); }
unsigned bf_u(std::uint32_t w) { return bits(w, 10, 6); }
unsigned bf_last(std::uint32_t w) { return sf_of(w) ? 63 : 31; }
bool sxtb_ok(std::uint32_t w) { return bf_ok(w) && bf_r(w) == 0 && bf_u(w) == 7; }
bool sxth_ok(std::uint32_t w) { return bf_ok(w) && bf_r(w) == 0 && bf_u(w) == 15; }
bool sxtw_ok(std::uint32_t w) { return bf_ok(w) && sf_of(w) && bf_r(w) == 0 && bf_u(w) == 31; }
bool asr_ok(std::uint32_t w) { return bf_ok(w) && bf_u(w) == bf_last(w); }
bool lsl_ok(std::uint32_t w) { return bf_ok(w) && bf_u(w) != bf_last(w) && bf_u(w) + 1 == bf_r(w); }
bool lsl_ok_32(std::uint32_t w) { return lsl_ok(w); }
bool bf_ins(std::uint32_t w) { return bf_ok(w) && bf_u(w) < bf_r(w); }
bool bf_ext(std::uint32_t w) { return bf_ok(w) && bf_u(w) >= bf_r(w); }
bool uxtb_ok(std::uint32_t w) { return bf_ok(w) && !sf_of(w) && bf_r(w) == 0 && bf_u(w) == 7; }
bool uxth_ok(std::uint32_t w) { return bf_ok(w) && !sf_of(w) && bf_r(w) == 0 && bf_u(w) == 15; }

bool ext_ok(std::uint32_t w) { return bits(w, 22, 1) == (sf_of(w) ? 1u : 0u) && (sf_of(w) || bits(w, 15, 1) == 0); }
bool ror_alias(std::uint32_t w) { return ext_ok(w) && bits(w, 5, 5) == bits(w, 16, 5); }
std::uint32_t fix_m_from_n(std::uint32_t w) { return (w & ~(31u << 16)) | (bits(w, 5, 5) << 16); }
std::uint32_t fix_n_from_sf(std::uint32_t w) { return (w & ~(1u << 22)) | (bits(w, 31, 1) << 22); }
std::uint32_t fix_ror(std::uint32_t w) { return fix_n_from_sf(fix_m_from_n(w)); }

bool as_ok(std::uint32_t w) { return bits(w, 22, 2) != 3 && (sf_of(w) || bits(w, 15, 1) == 0); }
bool ae_ok(std::uint32_t w) { return bits(w, 10, 3) <= 4; }
bool ls_ok(std::uint32_t w) { return sf_of(w) || bits(w, 15, 1) == 0; }

bool mem_option_ok(std::uint32_t w) { return (bits(w, 13, 3) & 2) != 0; }

bool cond_ok(std::uint32_t w) { return (bits(w, 12, 4) >> 1) != 7; }
bool cinc_ok(std::uint32_t w) { return cond_ok(w) && bits(w, 5, 5) == bits(w, 16, 5); }

bool fixed_ok(std::uint32_t w) { return sf_of(w) || bits(w, 15, 1) == 1; }

// ---- helpers ------------------------------------------------------------------------------------------------------------------------------------------------------

std::string bin(unsigned v, unsigned n) {
    std::string s;
    for (unsigned i = n; i-- > 0;) s += ((v >> i) & 1) ? '1' : '0';
    return s;
}

}  // namespace

void add_integer_rows(Builder& b) {
    // ---- permanently undefined ----
    b.add("udf", Op::Udf, "0000000000000000 iiiiiiiiiiiiiiii", "Ui", Flow::Stop);

    // ---- PC-relative addressing ----
    b.add("adr", Op::Adr, "0 ll 10000 hhhhhhhhhhhhhhhhhhh ddddd", "Xd,PADR");
    b.add("adrp", Op::Adrp, "1 ll 10000 hhhhhhhhhhhhhhhhhhh ddddd", "Xd,PADRP");

    // ---- add and subtract (immediate) ----
    b.add("mov", Op::AddSubImm, "s 0 0 10001 0 0 000000000000 nnnnn ddddd", "RSd,RSn", Flow::None, mov_sp);
    b.add("add", Op::AddSubImm, "s 0 0 10001 0 h iiiiiiiiiiii nnnnn ddddd", "RSd,RSn,AIMM");
    b.add("cmn", Op::AddSubImm, "s 0 1 10001 0 h iiiiiiiiiiii nnnnn 11111", "RSn,AIMM");
    b.add("adds", Op::AddSubImm, "s 0 1 10001 0 h iiiiiiiiiiii nnnnn ddddd", "Rd,RSn,AIMM");
    b.add("sub", Op::AddSubImm, "s 1 0 10001 0 h iiiiiiiiiiii nnnnn ddddd", "RSd,RSn,AIMM");
    b.add("cmp", Op::AddSubImm, "s 1 1 10001 0 h iiiiiiiiiiii nnnnn 11111", "RSn,AIMM");
    b.add("subs", Op::AddSubImm, "s 1 1 10001 0 h iiiiiiiiiiii nnnnn ddddd", "Rd,RSn,AIMM");

    // ---- logical (immediate) ----
    b.add("and", Op::LogicImm, "s 00 100100 N rrrrrr uuuuuu nnnnn ddddd", "RSd,Rn,LI", Flow::None, li_ok);
    b.add("mov", Op::LogicImm, "s 01 100100 N rrrrrr uuuuuu 11111 ddddd", "RSd,LI", Flow::None, mov_bitmask);
    b.add("orr", Op::LogicImm, "s 01 100100 N rrrrrr uuuuuu nnnnn ddddd", "RSd,Rn,LI", Flow::None, li_ok);
    b.add("eor", Op::LogicImm, "s 10 100100 N rrrrrr uuuuuu nnnnn ddddd", "RSd,Rn,LI", Flow::None, li_ok);
    b.add("tst", Op::LogicImm, "s 11 100100 N rrrrrr uuuuuu nnnnn 11111", "Rn,LI", Flow::None, li_ok);
    b.add("ands", Op::LogicImm, "s 11 100100 N rrrrrr uuuuuu nnnnn ddddd", "Rd,Rn,LI", Flow::None, li_ok);

    // ---- move wide ----
    b.add("mov", Op::MoveWide, "s 00 100101 hh iiiiiiiiiiiiiiii ddddd", "Rd,MOVNV", Flow::None, movn_alias);
    b.add("movn", Op::MoveWide, "s 00 100101 hh iiiiiiiiiiiiiiii ddddd", "Rd,MWI", Flow::None, mw_ok);
    b.add("mov", Op::MoveWide, "s 10 100101 hh iiiiiiiiiiiiiiii ddddd", "Rd,MOVZV", Flow::None, movz_alias);
    b.add("movz", Op::MoveWide, "s 10 100101 hh iiiiiiiiiiiiiiii ddddd", "Rd,MWI", Flow::None, mw_ok);
    b.add("movk", Op::MoveWide, "s 11 100101 hh iiiiiiiiiiiiiiii ddddd", "Rd,MWI", Flow::None, mw_ok);

    // ---- bitfield ----
    const auto bf = [&](const char* name, const char* pattern, const char* spec, When when) { b.add(name, Op::Bitfield, pattern, spec, Flow::None, when).fix = fix_n_from_sf; };
    const char* const sbfm = "s 00 100110 N rrrrrr uuuuuu nnnnn ddddd";
    bf("sxtb", "s 00 100110 N 000000 000111 nnnnn ddddd", "Rd,Wn", sxtb_ok);
    bf("sxth", "s 00 100110 N 000000 001111 nnnnn ddddd", "Rd,Wn", sxth_ok);
    bf("sxtw", "1 00 100110 N 000000 011111 nnnnn ddddd", "Xd,Wn", sxtw_ok);
    bf("asr", sbfm, "Rd,Rn,SHRI", asr_ok);
    bf("sbfiz", sbfm, "Rd,Rn,BFIZ", bf_ins);
    bf("sbfx", sbfm, "Rd,Rn,BFX", bf_ext);
    const char* const bfm = "s 01 100110 N rrrrrr uuuuuu nnnnn ddddd";
    bf("bfc", "s 01 100110 N rrrrrr uuuuuu 11111 ddddd", "Rd,BFIZ", bf_ins);
    bf("bfi", bfm, "Rd,Rn,BFIZ", bf_ins);
    bf("bfxil", bfm, "Rd,Rn,BFX", bf_ext);
    const char* const ubfm = "s 10 100110 N rrrrrr uuuuuu nnnnn ddddd";
    bf("uxtb", "0 10 100110 N 000000 000111 nnnnn ddddd", "Wd,Wn", uxtb_ok);
    bf("uxth", "0 10 100110 N 000000 001111 nnnnn ddddd", "Wd,Wn", uxth_ok);
    bf("lsr", ubfm, "Rd,Rn,SHRI", asr_ok);
    bf("lsl", ubfm, "Rd,Rn,LSLI", lsl_ok_32);
    bf("ubfiz", ubfm, "Rd,Rn,BFIZ", bf_ins);
    bf("ubfx", ubfm, "Rd,Rn,BFX", bf_ext);
    bf("sbfm", sbfm, "Rd,Rn,Ur,Uu", bf_ok);   // (the generic forms: the aliases above take every word, these are for the assembler)
    bf("bfm", bfm, "Rd,Rn,Ur,Uu", bf_ok);
    bf("ubfm", ubfm, "Rd,Rn,Ur,Uu", bf_ok);

    // ---- extract ----
    b.add("ror", Op::Extract, "s 00 100111 N 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,Ui", Flow::None, ror_alias).fix = fix_ror;
    b.add("extr", Op::Extract, "s 00 100111 N 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,Rm,Ui", Flow::None, ext_ok).fix = fix_n_from_sf;

    // ---- add and subtract (shifted register) ----
    b.add("cmn", Op::AddSubShift, "s 0 1 01011 hh 0 mmmmm iiiiii nnnnn 11111", "Rn,SREG", Flow::None, as_ok);
    b.add("add", Op::AddSubShift, "s 0 0 01011 hh 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, as_ok);
    b.add("adds", Op::AddSubShift, "s 0 1 01011 hh 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, as_ok);
    b.add("neg", Op::AddSubShift, "s 1 0 01011 hh 0 mmmmm iiiiii 11111 ddddd", "Rd,SREG", Flow::None, as_ok);
    b.add("sub", Op::AddSubShift, "s 1 0 01011 hh 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, as_ok);
    b.add("cmp", Op::AddSubShift, "s 1 1 01011 hh 0 mmmmm iiiiii nnnnn 11111", "Rn,SREG", Flow::None, as_ok);
    b.add("negs", Op::AddSubShift, "s 1 1 01011 hh 0 mmmmm iiiiii 11111 ddddd", "Rd,SREG", Flow::None, as_ok);
    b.add("subs", Op::AddSubShift, "s 1 1 01011 hh 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, as_ok);

    // ---- add and subtract (extended register) ----
    b.add("add", Op::AddSubExt, "s 0 0 01011 00 1 mmmmm ooo iii nnnnn ddddd", "RSd,RSn,EREG", Flow::None, ae_ok);
    b.add("cmn", Op::AddSubExt, "s 0 1 01011 00 1 mmmmm ooo iii nnnnn 11111", "RSn,EREG", Flow::None, ae_ok);
    b.add("adds", Op::AddSubExt, "s 0 1 01011 00 1 mmmmm ooo iii nnnnn ddddd", "Rd,RSn,EREG", Flow::None, ae_ok);
    b.add("sub", Op::AddSubExt, "s 1 0 01011 00 1 mmmmm ooo iii nnnnn ddddd", "RSd,RSn,EREG", Flow::None, ae_ok);
    b.add("cmp", Op::AddSubExt, "s 1 1 01011 00 1 mmmmm ooo iii nnnnn 11111", "RSn,EREG", Flow::None, ae_ok);
    b.add("subs", Op::AddSubExt, "s 1 1 01011 00 1 mmmmm ooo iii nnnnn ddddd", "Rd,RSn,EREG", Flow::None, ae_ok);

    // ---- add and subtract (with carry) ----
    b.add("adc", Op::AddSubCarry, "s 0 0 11010000 mmmmm 000000 nnnnn ddddd", "Rd,Rn,Rm");
    b.add("adcs", Op::AddSubCarry, "s 0 1 11010000 mmmmm 000000 nnnnn ddddd", "Rd,Rn,Rm");
    b.add("ngc", Op::AddSubCarry, "s 1 0 11010000 mmmmm 000000 11111 ddddd", "Rd,Rm");
    b.add("sbc", Op::AddSubCarry, "s 1 0 11010000 mmmmm 000000 nnnnn ddddd", "Rd,Rn,Rm");
    b.add("ngcs", Op::AddSubCarry, "s 1 1 11010000 mmmmm 000000 11111 ddddd", "Rd,Rm");
    b.add("sbcs", Op::AddSubCarry, "s 1 1 11010000 mmmmm 000000 nnnnn ddddd", "Rd,Rn,Rm");

    // ---- conditional compare ----
    b.add("ccmn", Op::CondCmp, "s 0 1 11010010 mmmmm cccc 0 0 nnnnn 0 vvvv", "Rn,Rm,Uv,CONDc");
    b.add("ccmp", Op::CondCmp, "s 1 1 11010010 mmmmm cccc 0 0 nnnnn 0 vvvv", "Rn,Rm,Uv,CONDc");
    b.add("ccmn", Op::CondCmp, "s 0 1 11010010 mmmmm cccc 1 0 nnnnn 0 vvvv", "Rn,Um,Uv,CONDc");
    b.add("ccmp", Op::CondCmp, "s 1 1 11010010 mmmmm cccc 1 0 nnnnn 0 vvvv", "Rn,Um,Uv,CONDc");

    // ---- conditional select ----
    b.add("cset", Op::CondSel, "s 0 0 11010100 11111 cccc 0 1 11111 ddddd", "Rd,CONDINVc", Flow::None, cond_ok);
    b.add("csetm", Op::CondSel, "s 1 0 11010100 11111 cccc 0 0 11111 ddddd", "Rd,CONDINVc", Flow::None, cond_ok);
    b.add("cinc", Op::CondSel, "s 0 0 11010100 mmmmm cccc 0 1 nnnnn ddddd", "Rd,Rn,CONDINVc", Flow::None, cinc_ok).fix = fix_m_from_n;
    b.add("cinv", Op::CondSel, "s 1 0 11010100 mmmmm cccc 0 0 nnnnn ddddd", "Rd,Rn,CONDINVc", Flow::None, cinc_ok).fix = fix_m_from_n;
    b.add("cneg", Op::CondSel, "s 1 0 11010100 mmmmm cccc 0 1 nnnnn ddddd", "Rd,Rn,CONDINVc", Flow::None, cinc_ok).fix = fix_m_from_n;
    b.add("csel", Op::CondSel, "s 0 0 11010100 mmmmm cccc 0 0 nnnnn ddddd", "Rd,Rn,Rm,CONDc");
    b.add("csinc", Op::CondSel, "s 0 0 11010100 mmmmm cccc 0 1 nnnnn ddddd", "Rd,Rn,Rm,CONDc");
    b.add("csinv", Op::CondSel, "s 1 0 11010100 mmmmm cccc 0 0 nnnnn ddddd", "Rd,Rn,Rm,CONDc");
    b.add("csneg", Op::CondSel, "s 1 0 11010100 mmmmm cccc 0 1 nnnnn ddddd", "Rd,Rn,Rm,CONDc");

    // ---- multiply and divide ----
    b.add("mul", Op::Dp3, "s 00 11011 000 mmmmm 0 11111 nnnnn ddddd", "Rd,Rn,Rm");
    b.add("madd", Op::Dp3, "s 00 11011 000 mmmmm 0 aaaaa nnnnn ddddd", "Rd,Rn,Rm,Ra");
    b.add("mneg", Op::Dp3, "s 00 11011 000 mmmmm 1 11111 nnnnn ddddd", "Rd,Rn,Rm");
    b.add("msub", Op::Dp3, "s 00 11011 000 mmmmm 1 aaaaa nnnnn ddddd", "Rd,Rn,Rm,Ra");
    b.add("smull", Op::Dp3, "1 00 11011 001 mmmmm 0 11111 nnnnn ddddd", "Xd,Wn,Wm");
    b.add("smaddl", Op::Dp3, "1 00 11011 001 mmmmm 0 aaaaa nnnnn ddddd", "Xd,Wn,Wm,Xa");
    b.add("smnegl", Op::Dp3, "1 00 11011 001 mmmmm 1 11111 nnnnn ddddd", "Xd,Wn,Wm");
    b.add("smsubl", Op::Dp3, "1 00 11011 001 mmmmm 1 aaaaa nnnnn ddddd", "Xd,Wn,Wm,Xa");
    b.add("smulh", Op::Dp3, "1 00 11011 010 mmmmm 0 11111 nnnnn ddddd", "Xd,Xn,Xm");
    b.add("umull", Op::Dp3, "1 00 11011 101 mmmmm 0 11111 nnnnn ddddd", "Xd,Wn,Wm");
    b.add("umaddl", Op::Dp3, "1 00 11011 101 mmmmm 0 aaaaa nnnnn ddddd", "Xd,Wn,Wm,Xa");
    b.add("umnegl", Op::Dp3, "1 00 11011 101 mmmmm 1 11111 nnnnn ddddd", "Xd,Wn,Wm");
    b.add("umsubl", Op::Dp3, "1 00 11011 101 mmmmm 1 aaaaa nnnnn ddddd", "Xd,Wn,Wm,Xa");
    b.add("umulh", Op::Dp3, "1 00 11011 110 mmmmm 0 11111 nnnnn ddddd", "Xd,Xn,Xm");

    // ---- data processing (2 sources) ----
    const std::pair<const char*, const char*> dp2[] = {{"udiv", "000010"}, {"sdiv", "000011"}, {"lsl", "001000"}, {"lsr", "001001"}, {"asr", "001010"}, {"ror", "001011"}};
    for (const auto& [name, opcode] : dp2) b.add(name, Op::Dp2, std::string("s 0 0 11010110 mmmmm ") + opcode + " nnnnn ddddd", "Rd,Rn,Rm");
    const char* const crc[] = {"crc32b", "crc32h", "crc32w", "crc32x", "crc32cb", "crc32ch", "crc32cw", "crc32cx"};
    for (unsigned k = 0; k < 8; ++k) {
        const bool x = (k & 3) == 3;
        b.add(crc[k], Op::Dp2, std::string(x ? "1" : "0") + " 0 0 11010110 mmmmm 010" + bin(k, 3) + " nnnnn ddddd", x ? "Wd,Wn,Xm" : "Wd,Wn,Wm");
    }

    // ---- data processing (1 source) ----
    b.add("rbit", Op::Dp1, "s 1 0 11010110 00000 000000 nnnnn ddddd", "Rd,Rn");
    b.add("rev16", Op::Dp1, "s 1 0 11010110 00000 000001 nnnnn ddddd", "Rd,Rn");
    b.add("rev", Op::Dp1, "0 1 0 11010110 00000 000010 nnnnn ddddd", "Wd,Wn");
    b.add("rev32", Op::Dp1, "1 1 0 11010110 00000 000010 nnnnn ddddd", "Xd,Xn");
    b.add("rev", Op::Dp1, "1 1 0 11010110 00000 000011 nnnnn ddddd", "Xd,Xn");
    b.add("clz", Op::Dp1, "s 1 0 11010110 00000 000100 nnnnn ddddd", "Rd,Rn");
    b.add("cls", Op::Dp1, "s 1 0 11010110 00000 000101 nnnnn ddddd", "Rd,Rn");

    // ---- logical (shifted register) ----
    b.add("and", Op::LogicShift, "s 00 01010 hh 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, ls_ok);
    b.add("bic", Op::LogicShift, "s 00 01010 hh 1 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, ls_ok);
    b.add("mov", Op::LogicShift, "s 01 01010 00 0 mmmmm 000000 11111 ddddd", "Rd,Rm");
    b.add("orr", Op::LogicShift, "s 01 01010 hh 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, ls_ok);
    b.add("mvn", Op::LogicShift, "s 01 01010 hh 1 mmmmm iiiiii 11111 ddddd", "Rd,SREG", Flow::None, ls_ok);
    b.add("orn", Op::LogicShift, "s 01 01010 hh 1 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, ls_ok);
    b.add("eor", Op::LogicShift, "s 10 01010 hh 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, ls_ok);
    b.add("eon", Op::LogicShift, "s 10 01010 hh 1 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, ls_ok);
    b.add("tst", Op::LogicShift, "s 11 01010 hh 0 mmmmm iiiiii nnnnn 11111", "Rn,SREG", Flow::None, ls_ok);
    b.add("ands", Op::LogicShift, "s 11 01010 hh 0 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, ls_ok);
    b.add("bics", Op::LogicShift, "s 11 01010 hh 1 mmmmm iiiiii nnnnn ddddd", "Rd,Rn,SREG", Flow::None, ls_ok);

    // ---- branches ----
    b.add("b", Op::B, "0 00101 iiiiiiiiiiiiiiiiiiiiiiiiii", "PRi", Flow::Jump);
    b.add("bl", Op::Bl, "1 00101 iiiiiiiiiiiiiiiiiiiiiiiiii", "PRi", Flow::Call);
    b.add_cond_suffix("b", Op::BCond, "0101010 0 iiiiiiiiiiiiiiiiiii 0 cccc", "PRi", Flow::Branch);
    b.add("cbz", Op::Cbz, "s 011010 0 iiiiiiiiiiiiiiiiiii ddddd", "Rd,PRi", Flow::Branch);
    b.add("cbnz", Op::Cbz, "s 011010 1 iiiiiiiiiiiiiiiiiii ddddd", "Rd,PRi", Flow::Branch);
    b.add("tbz", Op::Tbz, "s 011011 0 kkkkk iiiiiiiiiiiiii ddddd", "Rd,TBIT,PRi", Flow::Branch);
    b.add("tbnz", Op::Tbz, "s 011011 1 kkkkk iiiiiiiiiiiiii ddddd", "Rd,TBIT,PRi", Flow::Branch);
    b.add("br", Op::Br, "1101011 0000 11111 000000 nnnnn 00000", "Xn", Flow::Indirect);
    b.add("blr", Op::Blr, "1101011 0001 11111 000000 nnnnn 00000", "Xn", Flow::Call);
    b.add("ret", Op::Ret, "1101011 0010 11111 000000 11110 00000", "", Flow::Return);
    b.add("ret", Op::Ret, "1101011 0010 11111 000000 nnnnn 00000", "Xn", Flow::Return);
    b.add("eret", Op::Eret, "1101011 0100 11111 000000 11111 00000", "", Flow::Return);

    // ---- exception generation ----
    b.add("svc", Op::Svc, "11010100 000 iiiiiiiiiiiiiiii 00001", "Ui");
    b.add("hvc", Op::Hvc, "11010100 000 iiiiiiiiiiiiiiii 00010", "Ui");
    b.add("smc", Op::Smc, "11010100 000 iiiiiiiiiiiiiiii 00011", "Ui");
    b.add("brk", Op::Brk, "11010100 001 iiiiiiiiiiiiiiii 00000", "Ui", Flow::Stop);
    b.add("hlt", Op::Hlt, "11010100 010 iiiiiiiiiiiiiiii 00000", "Ui", Flow::Stop);

    // ---- hints and barriers ----
    const std::pair<const char*, unsigned> hints[] = {{"nop", 0}, {"yield", 1}, {"wfe", 2}, {"wfi", 3}, {"sev", 4}, {"sevl", 5}, {"paciasp", 25}, {"pacibsp", 27},
                                                       {"autiasp", 29}, {"autibsp", 31}, {"bti", 32}, {"xpaclri", 7}, {"pacia1716", 8}, {"pacib1716", 10}, {"autia1716", 12}, {"autib1716", 14}, {"paciaz", 24}, {"pacibz", 26}, {"autiaz", 28}, {"autibz", 30}};
    for (const auto& [name, imm] : hints) b.add(name, Op::Hint, "1101010100 0 00 011 0010 " + bin(imm, 7) + " 11111", "");
    b.add("hint", Op::Hint, "1101010100 0 00 011 0010 kkkkkkk 11111", "Uk");
    b.add("clrex", Op::Clrex, "1101010100 0 00 011 0011 1111 010 11111", "");
    b.add("clrex", Op::Clrex, "1101010100 0 00 011 0011 cccc 010 11111", "Uc");
    b.add("dsb", Op::Dsb, "1101010100 0 00 011 0011 cccc 100 11111", "BARc");
    b.add("dmb", Op::Dmb, "1101010100 0 00 011 0011 cccc 101 11111", "BARc");
    b.add("isb", Op::Isb, "1101010100 0 00 011 0011 1111 110 11111", "");
    b.add("isb", Op::Isb, "1101010100 0 00 011 0011 cccc 110 11111", "Uc");

    // ---- system ----
    const struct {
        const char* name;
        unsigned op1, op2;
    } pstate[] = {{"spsel", 0, 5}, {"daifset", 3, 6}, {"daifclr", 3, 7}};
    for (const auto& p : pstate) b.add("msr", Op::MsrImm, "1101010100 0 00 " + bin(p.op1, 3) + " 0100 cccc " + bin(p.op2, 3) + " 11111", std::string("=") + p.name + ",Uc");
    b.add("msr", Op::Msr, "1101010100 0 1 yyyyyyyyyyyyyyy ttttt", "SYSREG,Xt");
    b.add("mrs", Op::Mrs, "1101010100 1 1 yyyyyyyyyyyyyyy ttttt", "Xt,SYSREG");
    const struct {
        const char* name;
        const char* operation;
        unsigned op1, crn, crm, op2;
        bool register_operand;
    } cache[] = {{"dc", "zva", 3, 7, 4, 1, true},    {"dc", "civac", 3, 7, 14, 1, true}, {"dc", "cvac", 3, 7, 10, 1, true}, {"dc", "cvau", 3, 7, 11, 1, true},
                 {"dc", "ivac", 0, 7, 6, 1, true},   {"dc", "isw", 0, 7, 6, 2, true},    {"dc", "csw", 0, 7, 10, 2, true},  {"dc", "cisw", 0, 7, 14, 2, true},
                 {"ic", "ialluis", 0, 7, 1, 0, false}, {"ic", "iallu", 0, 7, 5, 0, false}, {"ic", "ivau", 3, 7, 5, 1, true}};
    for (const auto& c : cache)
        b.add(c.name, Op::Sys, "1101010100 0 01 " + bin(c.op1, 3) + bin(c.crn, 4) + bin(c.crm, 4) + bin(c.op2, 3) + (c.register_operand ? " ttttt" : " 11111"),
              std::string("=") + c.operation + (c.register_operand ? ",Xt" : ""));
}

// ---- loads and stores -------------------------------------------------------------------------------------------------------------------------------------------

void add_memory_rows(Builder& b) {
    static const char* const kSuffix[4] = {"b", "h", "", ""};
    struct Access {
        std::string name;   // "ldr", "ldrb", "ldrsw", "prfm"
        unsigned size, opc;
        bool simd;
        std::string reg;    // the operand token of Rt
        unsigned scale;     // log2 of the access size
        bool prefetch;
    };
    std::vector<Access> accesses;
    for (unsigned size = 0; size < 4; ++size) {
        const std::string r = size == 3 ? "Xt" : "Wt";
        accesses.push_back({std::string("str") + kSuffix[size], size, 0, false, r, size, false});
        accesses.push_back({std::string("ldr") + kSuffix[size], size, 1, false, r, size, false});
        if (size < 3) accesses.push_back({std::string("ldrs") + (size == 0 ? "b" : size == 1 ? "h" : "w"), size, 2, false, "Xt", size, false});
        else accesses.push_back({"prfm", size, 2, false, "PFt", size, true});
        if (size < 2) accesses.push_back({std::string("ldrs") + (size == 0 ? "b" : "h"), size, 3, false, "Wt", size, false});
        static const char* const fp[4] = {"FBt", "FHt", "FSt", "FDt"};
        accesses.push_back({"str", size, 0, true, fp[size], size, false});
        accesses.push_back({"ldr", size, 1, true, fp[size], size, false});
    }
    accesses.push_back({"str", 0, 2, true, "FQt", 4, false});
    accesses.push_back({"ldr", 0, 3, true, "FQt", 4, false});

    const auto unscaled_name = [](const Access& a) {
        if (a.name == "ldr") return std::string("ldur");
        if (a.name == "str") return std::string("stur");
        if (a.name == "ldrb") return std::string("ldurb");
        if (a.name == "ldrh") return std::string("ldurh");
        if (a.name == "strb") return std::string("sturb");
        if (a.name == "strh") return std::string("sturh");
        if (a.name == "prfm") return std::string("prfum");
        return "ldur" + a.name.substr(3);   // ldursb, ldursh, ldursw
    };
    const auto unpriv_name = [](const Access& a) { return std::string(a.name[0] == 'l' ? "ldtr" : "sttr") + a.name.substr(3); };

    for (const Access& a : accesses) {
        const std::string head = bin(a.size, 2) + " 111 " + (a.simd ? "1" : "0");
        const std::string opc = bin(a.opc, 2);
        const std::string tail = " nnnnn ttttt";
        b.add(a.name, Op::LdStImm, head + " 01 " + opc + " iiiiiiiiiiii" + tail, a.reg + ",MU" + std::to_string(a.scale));
        b.add(unscaled_name(a), Op::LdStImm, head + " 00 " + opc + " 0 iiiiiiiii 00" + tail, a.reg + ",MT");
        if (a.prefetch) {
            b.add("prfm", Op::LdStReg, head + " 00 " + opc + " 1 mmmmm ooo e 10" + tail, "PFt,MR3", Flow::None, mem_option_ok);
            continue;
        }
        b.add(a.name, Op::LdStImm, head + " 00 " + opc + " 0 iiiiiiiii 01" + tail, a.reg + ",MPOST");
        b.add(a.name, Op::LdStImm, head + " 00 " + opc + " 0 iiiiiiiii 11" + tail, a.reg + ",MPRE");
        if (!a.simd) b.add(unpriv_name(a), Op::LdStImm, head + " 00 " + opc + " 0 iiiiiiiii 10" + tail, a.reg + ",MT");
        b.add(a.name, Op::LdStReg, head + " 00 " + opc + " 1 mmmmm ooo e 10" + tail, a.reg + ",MR" + std::to_string(a.scale), Flow::None, mem_option_ok);
    }

    // literal loads
    b.add("ldr", Op::LdLit, "00 011 0 00 iiiiiiiiiiiiiiiiiii ttttt", "Wt,PRi");
    b.add("ldr", Op::LdLit, "01 011 0 00 iiiiiiiiiiiiiiiiiii ttttt", "Xt,PRi");
    b.add("ldrsw", Op::LdLit, "10 011 0 00 iiiiiiiiiiiiiiiiiii ttttt", "Xt,PRi");
    b.add("prfm", Op::LdLit, "11 011 0 00 iiiiiiiiiiiiiiiiiii ttttt", "PFt,PRi");
    b.add("ldr", Op::LdLit, "00 011 1 00 iiiiiiiiiiiiiiiiiii ttttt", "FSt,PRi");
    b.add("ldr", Op::LdLit, "01 011 1 00 iiiiiiiiiiiiiiiiiii ttttt", "FDt,PRi");
    b.add("ldr", Op::LdLit, "10 011 1 00 iiiiiiiiiiiiiiiiiii ttttt", "FQt,PRi");

    // pairs
    struct Pair {
        const char* regs;   // the two register tokens
        const char* pattern_head;
        unsigned scale;
        bool simd;
        bool signed_word;
    };
    const Pair pairs[] = {{"Wt,Wa", "00 101 0", 2, false, false}, {"Xt,Xa", "10 101 0", 3, false, false}, {"Xt,Xa", "01 101 0", 2, false, true},
                          {"FSt,FSa", "00 101 1", 2, true, false}, {"FDt,FDa", "01 101 1", 3, true, false}, {"FQt,FQa", "10 101 1", 4, true, false}};
    for (const Pair& p : pairs) {
        for (unsigned load = 0; load < 2; ++load) {
            if (p.signed_word && !load) continue;
            const std::string name = p.signed_word ? "ldpsw" : (load ? "ldp" : "stp");
            const std::string reg = std::string(p.regs) + ",";
            const std::string tail = std::string(" ") + (load ? "1" : "0") + " iiiiiii aaaaa nnnnn ttttt";
            const std::string scale = std::to_string(p.scale);
            b.add(name, Op::LdStPair, std::string(p.pattern_head) + " 0 01" + tail, reg + "MPPOST" + scale);
            b.add(name, Op::LdStPair, std::string(p.pattern_head) + " 0 10" + tail, reg + "MPO" + scale);
            b.add(name, Op::LdStPair, std::string(p.pattern_head) + " 0 11" + tail, reg + "MPPRE" + scale);
            if (!p.signed_word) b.add(load ? "ldnp" : "stnp", Op::LdStPair, std::string(p.pattern_head) + " 0 00" + tail, reg + "MPO" + scale);
        }
    }

    // exclusive and ordered
    for (unsigned size = 0; size < 4; ++size) {
        const std::string head = bin(size, 2) + " 001000 ";
        const std::string r = size == 3 ? "Xt" : "Wt";
        const std::string suffix = kSuffix[size];
        const auto row = [&](const char* name, const char* flags, std::string_view fixed_rs, std::string_view fixed_rt2, const std::string& spec) {
            // flags: o2 L o1 then (rs) o0 (rt2)
            std::string pattern = head + flags[0] + " " + flags[1] + " " + flags[2] + " " + std::string(fixed_rs) + " " + flags[3] + " " + std::string(fixed_rt2) + " nnnnn ttttt";
            b.add(name + suffix, Op::LdStExcl, pattern, spec);
        };
        row("stxr", "0000", "sssss", "11111", "Ws," + r + ",MX");
        row("stlxr", "0001", "sssss", "11111", "Ws," + r + ",MX");
        row("ldxr", "0100", "11111", "11111", r + ",MX");
        row("ldaxr", "0101", "11111", "11111", r + ",MX");
        row("stlr", "1001", "11111", "11111", r + ",MX");
        row("ldar", "1101", "11111", "11111", r + ",MX");
        if (size >= 2) {
            const std::string r2 = size == 3 ? "Xt,Xa" : "Wt,Wa";
            b.add("stxp", Op::LdStExcl, head + "0 0 1 sssss 0 aaaaa nnnnn ttttt", "Ws," + r2 + ",MX");
            b.add("stlxp", Op::LdStExcl, head + "0 0 1 sssss 1 aaaaa nnnnn ttttt", "Ws," + r2 + ",MX");
            b.add("ldxp", Op::LdStExcl, head + "0 1 1 11111 0 aaaaa nnnnn ttttt", r2 + ",MX");
            b.add("ldaxp", Op::LdStExcl, head + "0 1 1 11111 1 aaaaa nnnnn ttttt", r2 + ",MX");
        }
    }
}

// ---- scalar floating point ---------------------------------------------------------------------------------------------------------------------------------------

void add_fp_rows(Builder& b) {
    struct Type {
        const char* bits;
        const char* reg;   // FS FD FH
    };
    const Type types[] = {{"00", "FS"}, {"01", "FD"}, {"11", "FH"}};
    for (const Type& t : types) {
        const std::string ty = t.bits;
        const std::string f = t.reg;
        const std::string head = "0 0 0 11110 " + ty + " 1 ";
        // one source
        const std::pair<const char*, const char*> one[] = {{"fmov", "000000"}, {"fabs", "000001"}, {"fneg", "000010"}, {"fsqrt", "000011"}, {"frintn", "001000"}, {"frintp", "001001"},
                                                           {"frintm", "001010"}, {"frintz", "001011"}, {"frinta", "001100"}, {"frintx", "001110"}, {"frinti", "001111"}};
        for (const auto& [name, opcode] : one) b.add(name, Op::Fp1Src, head + opcode + " 10000 nnnnn ddddd", f + "d," + f + "n");
        for (const Type& to : types) {
            if (to.bits == t.bits) continue;
            b.add("fcvt", Op::Fp1Src, head + "0001" + to.bits + " 10000 nnnnn ddddd", std::string(to.reg) + "d," + f + "n");
        }
        // compare
        b.add("fcmp", Op::FpCmp, head + "mmmmm 001000 nnnnn 00 000", f + "n," + f + "m");
        b.add("fcmp", Op::FpCmp, head + "00000 001000 nnnnn 01 000", f + "n,=#0.0");
        b.add("fcmpe", Op::FpCmp, head + "mmmmm 001000 nnnnn 10 000", f + "n," + f + "m");
        b.add("fcmpe", Op::FpCmp, head + "00000 001000 nnnnn 11 000", f + "n,=#0.0");
        b.add("fccmp", Op::FpCCmp, head + "mmmmm cccc 01 nnnnn 0 vvvv", f + "n," + f + "m,Uv,CONDc");
        b.add("fccmpe", Op::FpCCmp, head + "mmmmm cccc 01 nnnnn 1 vvvv", f + "n," + f + "m,Uv,CONDc");
        b.add("fmov", Op::FpImm, head + "iiiiiiii 100 00000 ddddd", f + "d,FPIMM");
        // two sources
        const std::pair<const char*, const char*> two[] = {{"fmul", "0000"}, {"fdiv", "0001"}, {"fadd", "0010"}, {"fsub", "0011"}, {"fmax", "0100"},
                                                           {"fmin", "0101"}, {"fmaxnm", "0110"}, {"fminnm", "0111"}, {"fnmul", "1000"}};
        for (const auto& [name, opcode] : two) b.add(name, Op::Fp2Src, head + "mmmmm " + opcode + " 10 nnnnn ddddd", f + "d," + f + "n," + f + "m");
        b.add("fcsel", Op::FpCSel, head + "mmmmm cccc 11 nnnnn ddddd", f + "d," + f + "n," + f + "m,CONDc");
        // three sources
        const std::pair<const char*, const char*> three[] = {{"fmadd", "0 0"}, {"fmsub", "0 1"}, {"fnmadd", "1 0"}, {"fnmsub", "1 1"}};
        for (const auto& [name, o] : three)
            b.add(name, Op::Fp3Src, "0 0 0 11111 " + ty + " " + std::string(1, o[0]) + " mmmmm " + std::string(1, o[2]) + " aaaaa nnnnn ddddd", f + "d," + f + "n," + f + "m," + f + "a");

        // between floating point and integer registers
        const std::string ints = "s 0 0 11110 " + ty + " 1 ";
        const struct {
            const char* name;
            const char* rmode_opcode;
            bool to_int;
        } conv[] = {{"scvtf", "00 010", false}, {"ucvtf", "00 011", false}, {"fcvtzs", "11 000", true}, {"fcvtzu", "11 001", true}, {"fcvtns", "00 000", true},
                    {"fcvtnu", "00 001", true},  {"fcvtps", "01 000", true}, {"fcvtpu", "01 001", true}, {"fcvtms", "10 000", true}, {"fcvtmu", "10 001", true},
                    {"fcvtas", "00 100", true},  {"fcvtau", "00 101", true}};
        for (const auto& c : conv) b.add(c.name, Op::FpInt, ints + c.rmode_opcode + " 000000 nnnnn ddddd", c.to_int ? "Rd," + f + "n" : f + "d,Rn");
        // fmov between the register files
        if (ty == "00") {
            b.add("fmov", Op::FpInt, "0 0 0 11110 00 1 00 110 000000 nnnnn ddddd", "Wd,FSn");
            b.add("fmov", Op::FpInt, "0 0 0 11110 00 1 00 111 000000 nnnnn ddddd", "FSd,Wn");
        } else if (ty == "01") {
            b.add("fmov", Op::FpInt, "1 0 0 11110 01 1 00 110 000000 nnnnn ddddd", "Xd,FDn");
            b.add("fmov", Op::FpInt, "1 0 0 11110 01 1 00 111 000000 nnnnn ddddd", "FDd,Xn");
        } else {
            b.add("fmov", Op::FpInt, ints + "00 110 000000 nnnnn ddddd", "Rd,FHn");
            b.add("fmov", Op::FpInt, ints + "00 111 000000 nnnnn ddddd", "FHd,Rn");
        }
        // fixed point
        const std::string fixed = "s 0 0 11110 " + ty + " 0 ";
        const struct {
            const char* name;
            const char* rmode_opcode;
            bool to_int;
        } fx[] = {{"scvtf", "00 010", false}, {"ucvtf", "00 011", false}, {"fcvtzs", "11 000", true}, {"fcvtzu", "11 001", true}};
        for (const auto& c : fx) b.add(c.name, Op::FpFixed, fixed + c.rmode_opcode + " uuuuuu nnnnn ddddd", (c.to_int ? "Rd," + f + "n" : f + "d,Rn") + ",FBITS", Flow::None, fixed_ok);
    }
}

}  // namespace cb::decompile::a64::detail
