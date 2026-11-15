// cb-unity: alone (file-local names that other files in a batch also use)
// The PSP's Allegrex (MIPS32, little-endian, with the VFPU): words worked out by hand -- CLZ CLO MADD MSUB MAX MIN, the SPECIAL3 group (EXT INS
// SEB SEH WSBH BITREV), ROTR, HALT, the single-precision FPU, the VFPU's registers, three-operand and unary operations, memory operations and
// branches -- what the 64-bit processors and the R3000A refuse of them, and the round trip over words.
#include "cb/decompile/mips.hpp"

#include <format>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace m = cb::decompile::mips;
using std::string;

namespace {

constexpr std::uint32_t kBase = 0x08804000;
constexpr m::Variant kPsp = m::Variant::Allegrex;

std::uint32_t word_of(const cb::Bytes& b) {
    return static_cast<std::uint32_t>(b.at(0)) | static_cast<std::uint32_t>(b.at(1)) << 8 | static_cast<std::uint32_t>(b.at(2)) << 16 | static_cast<std::uint32_t>(b.at(3)) << 24;
}
std::uint32_t assembled(const string& line, std::uint32_t pc = kBase, const cb::decompile::NameLabels& labels = {}) { return word_of(m::assemble(line, pc, labels, kPsp)); }
string disassembled(std::uint32_t word, std::uint32_t pc = kBase, const cb::decompile::AddressLabels& labels = {}) {
    const auto insn = m::decode_word(word, pc, kPsp);
    return insn ? m::text(*insn, labels) : string("-");
}

struct Case {
    std::uint32_t word;
    const char* text;
};

}  // namespace

CB_TEST("decompile.mips_psp.integer_words_and_text", [] {
    const Case cases[] = {
        {0x00801016, "clz $v0, $a0"},             {0x00801017, "clo $v0, $a0"},
        {0x0085102C, "max $v0, $a0, $a1"},        {0x0085102D, "min $v0, $a0, $a1"},
        {0x0085001C, "madd $a0, $a1"},            {0x0085001D, "maddu $a0, $a1"},
        {0x0085002E, "msub $a0, $a1"},            {0x0085002F, "msubu $a0, $a1"},
        {0x7C041420, "seb $v0, $a0"},             {0x7C041620, "seh $v0, $a0"},
        {0x7C0410A0, "wsbh $v0, $a0"},            {0x7C0410E0, "wsbw $v0, $a0"},
        {0x7C041520, "bitrev $v0, $a0"},          {0x7C823900, "ext $v0, $a0, 4, 8"},
        {0x7C825904, "ins $v0, $a0, 4, 8"},       {0x00241102, "rotr $v0, $a0, 4"},
        {0x00851046, "rotrv $v0, $a1, $a0"},      {0x0085100A, "movz $v0, $a0, $a1"},
        {0x70000000, "halt"},                     {0x27BDFFF0, "addiu $sp, $sp, -16"},
        {0x0000000F, "sync"},                     {0x03E00008, "jr $ra"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    // the Allegrex has no 64-bit integers, no doubles and no MMI
    CB_CHECK_EQ(disassembled(0x67BDFFF0), string("-"));   // daddiu
    CB_CHECK_EQ(disassembled(0xDFBF0008), string("-"));   // ld
    CB_CHECK_EQ(disassembled(0x46220800), string("-"));   // add.d
    CB_CHECK_EQ(disassembled(0x708514A9), string("-"));   // por
    CB_CHECK(!m::decode_word(0x00801016, kBase, m::Variant::R3000A));   // CLZ is the Allegrex's
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("ext $v0, $a0, 30, 8", kBase, {}, kPsp));   // the field leaves the word
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("clz $v0, $a0", kBase, {}, m::Variant::R3000A));
});

CB_TEST("decompile.mips_psp.fpu_is_ieee_single", [] {
    const Case cases[] = {
        {0x46020800, "add.s $f0, $f1, $f2"},    {0x46000804, "sqrt.s $f0, $f1"},    {0x4600080D, "trunc.w.s $f0, $f1"},
        {0x46800820, "cvt.s.w $f0, $f1"},       {0x46000824, "cvt.w.s $f0, $f1"},   {0x46020832, "c.eq.s $f1, $f2"},
        {0x44800800, "mtc1 $zero, $f1"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    CB_CHECK_EQ(disassembled(0x46201004), string("-"));   // sqrt.d
    CB_CHECK_EQ(disassembled(0x46001008), string("-"));          // round.l.s
});

CB_TEST("decompile.mips_psp.vfpu", [] {
    const Case cases[] = {
        {0x60028180, "vadd.q C000, C010, C020"},
        {0x60028184, "vadd.q C100, C010, C020"},
        {0x64828180, "vdot.q S000, C010, C020"},
        {0xD0068080, "vzero.q C000"},
        {0xD0008180, "vmov.q C000, C010"},
        {0xF0088480, "vmmul.q M000, M100, M200"},
        {0xD8840000, "lv.q C100, 0($a0)"},
        {0xF8840012, "sv.q C100, 16($a0), wb"},
        {0xF8840010, "sv.q C100, 16($a0)"},
        {0xC8800000, "lv.s S000, 0($a0)"},
        {0xE8A00004, "sv.s S000, 4($a1)"},
        {0x48620000, "mfv $v0, S000"},
        {0x48E20000, "mtv $v0, S000"},
        {0xFFFF0000, "vnop"},
        {0xFFFF0320, "vsync"},
        {0xFFFF040D, "vflush"},
        {0xDC0000E4, "vpfxs 0xE4"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    // a size the instruction does not have, and a register of another size
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("vcrs.q C000, C010, C020", kBase, {}, kPsp));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("vadd.q C000, C010", kBase, {}, kPsp));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("vadd.q C001, C010, C020", kBase, {}, kPsp));   // a quad column starts at row 0 or 2
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("lv.q C101, 0($a0)", kBase, {}, kPsp));
    // a branch on the VFPU's condition
    const cb::decompile::AddressLabels labels = {{kBase + 0x20, "L_here"}};
    const auto taken = m::decode_word(0x490D0007, kBase, kPsp);   // bvt 3, +7 words
    CB_CHECK(taken);
    CB_CHECK(taken->flow == m::Flow::Branch && taken->delay_slot && !taken->likely);
    CB_CHECK_EQ(m::text(*taken, labels), string("bvt 3, L_here"));
    CB_CHECK_EQ(assembled("bvt 3, L_here", kBase, {{"L_here", kBase + 0x20}}), 0x490D0007u);
    CB_CHECK(m::decode_word(0x490B0000, kBase, kPsp)->likely);
});

CB_TEST("decompile.mips_psp.every_decoded_word_assembles_back", [] {
    std::uint64_t x = 0xD1B54A32D192ED03ull;
    std::size_t decoded = 0;
    constexpr std::uint32_t kOpcodes[] = {0x00, 0x1F, 0x18, 0x19, 0x1B, 0x34, 0x32, 0x36, 0x35, 0x3A, 0x3E, 0x3D, 0x12, 0x37, 0x3C, 0x1C, 0x11};
    for (std::size_t i = 0; i < 400000; ++i) {
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        std::uint32_t word = static_cast<std::uint32_t>(x >> 16);
        if (i % 2 == 0) word = (word & 0x03FFFFFFu) | (kOpcodes[(x >> 8) % std::size(kOpcodes)] << 26);
        const auto insn = m::decode_word(word, kBase, kPsp);
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
