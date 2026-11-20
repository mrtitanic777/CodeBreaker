// cb-unity: alone (file-local names that other files in a batch also use)
// decompile/sh4_test.cpp -- the SH-4 variant of cb/decompile/sh2: words worked out by hand from the SH7750 manual's instruction formats
// (the FPU, the control registers, the cache and TLB instructions), the little-endian reading, the other spellings the assembler takes, and the
// round trip (every word that decodes is written as text that assembles to that word).
#include "decompile/sh2.hpp"

#include <format>
#include <iostream>
#include <set>
#include <string>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace s4t = cb::decompile::sh2;
using cb::Bytes;
using std::string;

namespace {

constexpr std::uint32_t kBase = 0x8C010000;
constexpr s4t::Variant kSh4 = s4t::Variant::Sh4;

std::uint16_t sh4_assembled(const string& line, std::uint32_t pc = kBase) {
    const Bytes b = s4t::assemble(line, pc, {}, kSh4);
    return static_cast<std::uint16_t>(b.at(1) << 8 | b.at(0));   // (little-endian)
}

string sh4_disassembled(std::uint16_t word, std::uint32_t pc = kBase) {
    const auto insn = s4t::decode_word(word, pc, kSh4);
    return insn ? s4t::text(*insn) : string("-");
}

struct Sh4Case {
    std::uint16_t word;
    const char* text;
};

}  // namespace

CB_TEST("decompile.sh4.fpu_words_and_text", [] {
    const Sh4Case cases[] = {
        {0xF210, "fadd    fr1,fr2"},     {0xF211, "fsub    fr1,fr2"},       {0xF212, "fmul    fr1,fr2"},
        {0xF213, "fdiv    fr1,fr2"},     {0xF214, "fcmp/eq fr1,fr2"},       {0xF215, "fcmp/gt fr1,fr2"},
        {0xF21C, "fmov    fr1,fr2"},     {0xF21E, "fmac    fr0,fr1,fr2"},   {0xF049, "fmov.s  @r4+,fr0"},
        {0xF048, "fmov.s  @r4,fr0"},     {0xF04A, "fmov.s  fr4,@r0"},       {0xF4AB, "fmov.s  fr10,@-r4"},
        {0xF346, "fmov.s  @(r0,r4),fr3"}, {0xF357, "fmov.s  fr5,@(r0,r3)"}, {0xF38D, "fldi0   fr3"},
        {0xF39D, "fldi1   fr3"},         {0xF12D, "float   fpul,fr1"},      {0xF13D, "ftrc    fr1,fpul"},
        {0xF00D, "fsts    fpul,fr0"},    {0xF01D, "flds    fr0,fpul"},      {0xF14D, "fneg    fr1"},
        {0xF15D, "fabs    fr1"},         {0xF16D, "fsqrt   fr1"},           {0xF57D, "fsrra   fr5"},
        {0xF2AD, "fcnvsd  fpul,dr2"},    {0xF2BD, "fcnvds  dr2,fpul"},      {0xF9ED, "fipr    fv4,fv8"},
        {0xF5FD, "ftrv    xmtrx,fv4"},   {0xF2FD, "fsca    fpul,dr2"},      {0xF3FD, "fschg"},
        {0xFBFD, "frchg"},
    };
    for (const Sh4Case& c : cases) {
        CB_CHECK_EQ(sh4_disassembled(c.word), string(c.text));
        CB_CHECK_EQ(sh4_assembled(c.text), c.word);
    }
});

CB_TEST("decompile.sh4.system_words_and_text", [] {
    const Sh4Case cases[] = {
        {0x416A, "lds     r1,fpscr"},      {0x026A, "sts     fpscr,r2"},         {0x4F66, "lds.l   @r15+,fpscr"},
        {0x4F62, "sts.l   fpscr,@-r15"},   {0x4F52, "sts.l   fpul,@-r15"},       {0x405A, "lds     r0,fpul"},
        {0x035A, "sts     fpul,r3"},       {0x0483, "pref    @r4"},              {0x0193, "ocbi    @r1"},
        {0x01A3, "ocbp    @r1"},           {0x01B3, "ocbwb   @r1"},              {0x02C3, "movca.l r0,@r2"},
        {0x0038, "ldtlb"},                 {0x0048, "clrs"},                     {0x0058, "sets"},
        {0x421C, "shad    r1,r2"},         {0x421D, "shld    r1,r2"},            {0x413E, "ldc     r1,ssr"},
        {0x414E, "ldc     r1,spc"},        {0x41FA, "ldc     r1,dbr"},           {0x41BE, "ldc     r1,r3_bank"},
        {0x0142, "stc     spc,r1"},        {0x0132, "stc     ssr,r1"},           {0x013A, "stc     sgr,r1"},
        {0x01FA, "stc     dbr,r1"},        {0x0182, "stc     r0_bank,r1"},       {0x01F2, "stc     r7_bank,r1"},
        {0x4F03, "stc.l   sr,@-r15"},      {0x4F07, "ldc.l   @r15+,sr"},         {0x4F33, "stc.l   ssr,@-r15"},
        {0x4F37, "ldc.l   @r15+,ssr"},     {0x4F43, "stc.l   spc,@-r15"},        {0x4FF2, "stc.l   dbr,@-r15"},
        {0x4F83, "stc.l   r0_bank,@-r15"}, {0x4F87, "ldc.l   @r15+,r0_bank"},    {0x0102, "stc     sr,r1"},
    };
    for (const Sh4Case& c : cases) {
        CB_CHECK_EQ(sh4_disassembled(c.word), string(c.text));
        CB_CHECK_EQ(sh4_assembled(c.text), c.word);
    }
});

CB_TEST("decompile.sh4.little_endian_and_variants", [] {
    // the bytes of a word are low first for the SH-4, high first for the SH-2
    const Bytes code = {0x10, 0xF2, 0x0B, 0x00};
    const auto fadd = s4t::decode(code, 0, kBase, kSh4);
    CB_CHECK(fadd && fadd->word == 0xF210 && fadd->mnemonic == "fadd");
    CB_CHECK(fadd->raw[0] == 0x10 && fadd->raw[1] == 0xF2);
    const auto rts = s4t::decode(code, 2, kBase + 2, kSh4);
    CB_CHECK(rts && rts->word == 0x000B && rts->flow == s4t::Flow::Return && rts->delay_slot);
    const Bytes assembled_rts = s4t::assemble("rts", kBase, {}, kSh4);
    CB_CHECK(assembled_rts == (Bytes{0x0B, 0x00}));
    CB_CHECK(s4t::assemble("rts", kBase) == (Bytes{0x00, 0x0B}));   // (big-endian for the SH-2)
    // the FPU is the SH-4's: an SH-2 neither decodes nor assembles it
    CB_CHECK(!s4t::decode_word(0xF210, kBase));
    CB_CHECK_THROWS_AS(cb::ParseError, s4t::assemble("fadd fr1,fr2", kBase));
    // the SH-2's instructions are the SH-4's too
    CB_CHECK_EQ(sh4_assembled("mov.l   r8,@-r15"), 0x2F86);
    CB_CHECK_EQ(sh4_assembled("sts.l   pr,@-r15"), 0x4F22);
    CB_CHECK_EQ(sh4_assembled("mov.l   @(8,pc),r1", kBase), 0xD102);
});

CB_TEST("decompile.sh4.other_spellings", [] {
    // PR=1 makes the operation double: the same word, named with dr registers
    CB_CHECK_EQ(sh4_assembled("fadd dr2,dr4"), sh4_assembled("fadd fr2,fr4"));
    CB_CHECK_EQ(sh4_assembled("float fpul,dr6"), sh4_assembled("float fpul,fr6"));
    CB_CHECK_EQ(sh4_assembled("ftrc dr6,fpul"), sh4_assembled("ftrc fr6,fpul"));
    // SZ=1 moves a pair; xd registers are the other bank (bit 8 or 4 set)
    CB_CHECK_EQ(sh4_assembled("fmov.d @r4,dr2"), sh4_assembled("fmov.s @r4,fr2"));
    CB_CHECK_EQ(sh4_assembled("fmov @r4+,dr2"), sh4_assembled("fmov.s @r4+,fr2"));
    CB_CHECK_EQ(sh4_assembled("fmov.d dr2,@-r15"), sh4_assembled("fmov.s fr2,@-r15"));
    CB_CHECK_EQ(sh4_assembled("fmov dr2,dr4"), sh4_assembled("fmov fr2,fr4"));
    CB_CHECK_EQ(sh4_assembled("fmov xd2,dr4"), sh4_assembled("fmov fr3,fr4"));
    CB_CHECK_EQ(sh4_assembled("fmov dr2,xd4"), sh4_assembled("fmov fr2,fr5"));
    CB_CHECK_EQ(sh4_assembled("fmov.s @r4,fr2"), 0xF248);
    CB_CHECK_THROWS_AS(cb::ParseError, sh4_assembled("fadd dr1,dr2"));        // a double register is even
    CB_CHECK_THROWS_AS(cb::ParseError, sh4_assembled("fipr fv2,fv4"));        // a vector is fv0, fv4, fv8 or fv12
    CB_CHECK_THROWS_AS(cb::ParseError, sh4_assembled("fmac fr1,fr2,fr3"));    // fmac multiplies into fr0
    CB_CHECK_THROWS_AS(cb::ParseError, sh4_assembled("fcnvsd fpul,dr3"));
    CB_CHECK_THROWS_AS(cb::ParseError, sh4_assembled("stc r8_bank,r1"));      // eight banked registers
    CB_CHECK_THROWS_AS(cb::ParseError, sh4_assembled("fschg r1"));
});

CB_TEST("decompile.sh4.not_instructions", [] {
    CB_CHECK(!s4t::decode_word(0xFFFF, kBase, kSh4));
    CB_CHECK(!s4t::decode_word(0xF7FD, kBase, kSh4));
    CB_CHECK(!s4t::decode_word(0xF1AD, kBase, kSh4));   // fcnvsd takes an even register
    CB_CHECK(!s4t::decode_word(0xF0CD, kBase, kSh4));
    CB_CHECK(!s4t::decode_word(0x0000, kBase, kSh4));
});

CB_TEST("decompile.sh4.every_word_round_trips", [] {
    int decoded = 0;
    int fpu = 0;
    std::set<string> mnemonics;
    for (std::uint32_t w = 0; w < 0x10000; ++w) {
        const auto insn = s4t::decode_word(static_cast<std::uint16_t>(w), kBase, kSh4);
        if (!insn) continue;
        ++decoded;
        if ((w >> 12) == 0xF) ++fpu;
        mnemonics.insert(insn->mnemonic);
        const string line = s4t::text(*insn);
        const Bytes bytes = s4t::assemble(line, kBase, {}, kSh4);
        const std::uint16_t back = static_cast<std::uint16_t>(bytes.at(1) << 8 | bytes.at(0));
        CB_CHECK_EQ(back, static_cast<std::uint16_t>(w));
        if (back != w) {
            std::cout << "word " << std::format("{:04X}", w) << " text \"" << line << "\"\n";
            return;
        }
        CB_CHECK(insn->raw[0] == (w & 0xFF) && insn->raw[1] == (w >> 8));
    }
    CB_CHECK(decoded > 36000);
    CB_CHECK(fpu > 3000);
    for (const char* m : {"fadd", "fsub", "fmul", "fdiv", "fcmp/eq", "fcmp/gt", "fmov", "fmov.s", "fmac", "fsts", "flds", "float", "ftrc", "fneg", "fabs", "fsqrt",
                          "fsrra", "fldi0", "fldi1", "fcnvsd", "fcnvds", "fipr", "ftrv", "fsca", "fschg", "frchg", "pref", "ocbi", "ocbp", "ocbwb", "movca.l",
                          "ldtlb", "clrs", "sets", "shad", "shld"})
        CB_CHECK(mnemonics.contains(m));
});

CB_TEST("decompile.sh4.sh2_words_round_trip_big_endian_too", [] {
    // the SH-2 variant still reads and writes big-endian, and none of the SH-4's words are in it
    for (std::uint32_t w = 0; w < 0x10000; ++w) {
        const auto insn = s4t::decode_word(static_cast<std::uint16_t>(w), kBase);
        if (!insn) continue;
        const Bytes bytes = s4t::assemble(s4t::text(*insn), kBase);
        CB_CHECK_EQ(static_cast<std::uint16_t>(bytes.at(0) << 8 | bytes.at(1)), static_cast<std::uint16_t>(w));
        CB_CHECK((w >> 12) != 0xF);
    }
});
