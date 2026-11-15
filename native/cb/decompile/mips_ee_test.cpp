// cb-unity: alone (file-local names that other files in a batch also use)
// The PlayStation 2's R5900 (the Emotion Engine's MIPS III with 128-bit registers): words worked out by hand from the MIPS formats and the
// EE core's manual -- MIPS IV's MOVZ/MOVN, three-operand MULT, LQ and SQ, the multimedia instructions (opcode 0x1C: MMI0-MMI3 by the shift
// field), the single-precision FPU (SQRT reads FT, the accumulator forms), COP0's EI and DI, VU0's LQC2 and macro mode -- what the R4300 and the
// R3000A refuse of them, and the round trip over words: every word that decodes is written as text that assembles to the same word.
#include "cb/decompile/mips.hpp"

#include <format>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace m = cb::decompile::mips;
using std::string;

namespace {

constexpr std::uint32_t kBase = 0x00100000;
constexpr m::Variant kEe = m::Variant::R5900;

std::uint32_t word_of(const cb::Bytes& b) {   // little-endian
    return static_cast<std::uint32_t>(b.at(0)) | static_cast<std::uint32_t>(b.at(1)) << 8 | static_cast<std::uint32_t>(b.at(2)) << 16 | static_cast<std::uint32_t>(b.at(3)) << 24;
}
std::uint32_t assembled(const string& line, std::uint32_t pc = kBase) { return word_of(m::assemble(line, pc, {}, kEe)); }
string disassembled(std::uint32_t word, std::uint32_t pc = kBase) {
    const auto insn = m::decode_word(word, pc, kEe);
    return insn ? m::text(*insn, {}) : string("-");
}

struct Case {
    std::uint32_t word;
    const char* text;
};

}  // namespace

CB_TEST("decompile.mips_ee.integer_and_multimedia_words_and_text", [] {
    const Case cases[] = {
        {0x7BA40000, "lq $a0, 0($sp)"},           {0x7FBF0010, "sq $ra, 16($sp)"},
        {0x0085100A, "movz $v0, $a0, $a1"},       {0x0085100B, "movn $v0, $a0, $a1"},
        {0x00851018, "mult $v0, $a0, $a1"},       {0x00850018, "mult $a0, $a1"},
        {0x00851019, "multu $v0, $a0, $a1"},      {0x70851000, "madd $v0, $a0, $a1"},
        {0x70850018, "mult1 $a0, $a1"},           {0x7085001A, "div1 $a0, $a1"},
        {0x70001010, "mfhi1 $v0"},                {0x70800011, "mthi1 $a0"},
        {0x708514A9, "por $v0, $a0, $a1"},        {0x70851389, "pcpyld $v0, $a0, $a1"},
        {0x708514C9, "pxor $v0, $a0, $a1"},       {0x70851008, "paddw $v0, $a0, $a1"},
        {0x70001030, "pmfhl.lw $v0"},             {0x70001070, "pmfhl.uw $v0"},
        {0x7005113C, "psllw $v0, $a1, 4"},        {0x70801004, "plzcw $v0, $a0"},
        {0x0085102D, "daddu $v0, $a0, $a1"},      {0x42000038, "ei"},
        {0x42000039, "di"},                       {0x00000000, "nop"},
        {0x0000000F, "sync"},                     {0xCC800000, "pref 0, 0($a0)"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    // the R5900 has no load-linked (the Nintendo 64's), and the R4300 and the R3000A have no MMI
    CB_CHECK_EQ(disassembled(0xC0880000), string("-"));
    CB_CHECK(!m::decode_word(0x708514A9, kBase, m::Variant::R4300));
    CB_CHECK(!m::decode_word(0x708514A9, kBase, m::Variant::R3000A));
    CB_CHECK(!m::decode_word(0x00851018, kBase, m::Variant::R3000A));   // MULT with RD set is the EE's
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("por $v0, $a0", kBase, {}, kEe));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("por $v0, $a0, $a1", kBase, {}, m::Variant::R3000A));
});

CB_TEST("decompile.mips_ee.fpu_is_single_precision_with_the_accumulator", [] {
    const Case cases[] = {
        {0x46020800, "add.s $f0, $f1, $f2"},      {0x46020004, "sqrt.s $f0, $f2"},          {0x46020818, "adda.s $f1, $f2"},
        {0x4602081C, "madd.s $f0, $f1, $f2"},     {0x4602081D, "msub.s $f0, $f1, $f2"},     {0x46800820, "cvt.s.w $f0, $f1"},
        {0x46000824, "cvt.w.s $f0, $f1"},         {0x46020832, "c.eq.s $f1, $f2"},          {0xC4800004, "lwc1 $f0, 4($a0)"},
        {0x46020816, "rsqrt.s $f0, $f1, $f2"},    {0x46020828, "max.s $f0, $f1, $f2"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    CB_CHECK_EQ(disassembled(0x46220800), string("-"));   // add.d: the EE's FPU has no doubles
    CB_CHECK_EQ(disassembled(0xD4800000), string("-"));   // ldc1
});

CB_TEST("decompile.mips_ee.vu0_macro_mode", [] {
    const Case cases[] = {
        {0xD8810010, "lqc2 $vf1, 16($a0)"},
        {0xF8810010, "sqc2 $vf1, 16($a0)"},
        {0x4BE31068, "vadd.xyzw $vf1, $vf2, $vf3"},
        {0x4AE31068, "vadd.yzw $vf1, $vf2, $vf3"},
        {0x4B00105C, "vmulq.x $vf1, $vf2, $Q"},
        {0x4A0110F0, "viadd $vi3, $vi2, $vi1"},
        {0x4A000039, "vcallmsr"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    CB_CHECK_EQ(disassembled(0x4BE00039), string("-"));   // a destination on VCALLMSR is not one
    CB_CHECK_EQ(disassembled(0x4A00003C), string("-"));   // the second group of functions is data
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("vadd.xyzw $vf1, $vf2", kBase, {}, kEe));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("vadd.wx $vf1, $vf2, $vf3", kBase, {}, kEe));
});

CB_TEST("decompile.mips_ee.is_little_endian_with_a_delay_slot", [] {
    const std::uint8_t bytes[] = {0xF0, 0xFF, 0xBD, 0x67, 0x08, 0x00, 0xE0, 0x03};   // daddiu $sp, $sp, -16 ; jr $ra
    const cb::ByteView view(bytes, sizeof bytes);
    const auto first = m::decode(view, 0, kBase, kEe);
    CB_CHECK(first);
    CB_CHECK_EQ(m::text(*first), string("daddiu $sp, $sp, -16"));
    CB_CHECK_EQ(static_cast<int>(first->raw[0]), 0xF0);
    const auto second = m::decode(view, 4, kBase + 4, kEe);
    CB_CHECK(second && second->flow == m::Flow::Return && second->delay_slot);
    const auto likely = m::decode_word(0x5080FFFF, kBase, kEe);   // beql $a0, $zero, ...
    CB_CHECK(likely && likely->likely);
});

CB_TEST("decompile.mips_ee.every_decoded_word_assembles_back", [] {
    std::uint64_t x = 0x9E3779B97F4A7C15ull;
    std::size_t decoded = 0;
    for (std::size_t i = 0; i < 400000; ++i) {
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        std::uint32_t word = static_cast<std::uint32_t>(x >> 16);
        if (i % 3 == 0) word = (word & 0x03FFFFFFu) | ((i % 2 ? 0x1Cu : 0x12u) << 26);   // the MMI and COP2 opcodes
        const auto insn = m::decode_word(word, kBase, kEe);
        if (!insn) continue;
        ++decoded;
        const string text = m::text(*insn, {});
        std::uint32_t back = 0;
        try {
            back = assembled(text, kBase);
        } catch (const cb::Error& e) {
            cb::testing::record_failure(__FILE__, __LINE__, "does not assemble: " + text + " (" + e.what() + ")");
            continue;
        }
        if (back != word) cb::testing::record_failure(__FILE__, __LINE__, text + " assembles to a different word");
    }
    CB_CHECK(decoded > 1000);
});
