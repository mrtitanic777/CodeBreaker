// decompile/arm_ext_test.cpp -- the ARMv6 and VFP rows: words worked out by hand from the ARM Architecture Reference Manual, the text of each, the
// assembler's refusals, and the round trip (every word a row matches is written as text that assembles to that word).
#include "decompile/arm_ext.hpp"

#include <string>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace a = cb::decompile::arm;
namespace ex = cb::decompile::arm::ext;
using std::string;

namespace {

string text_of(std::uint32_t word) {
    const int row = ex::find_arm(word, a::Arch::V6K);
    return row < 0 ? string("?") : ex::text(row, word, 0x1000, {});
}

std::uint32_t word_of(const string& line) {
    const auto w = ex::assemble(line, 0x1000, {}, a::Arch::V6K);
    return w ? *w : 0xFFFFFFFFu;
}

}  // namespace

CB_TEST("decompile.arm_ext.words_and_text", [] {
    struct Case {
        std::uint32_t word;
        const char* text;
    };
    const Case cases[] = {
        {0xE6BF0F31, "rev r0, r1"},
        {0xE6EF2473, "uxtb r2, r3, ror #8"},
        {0xE6E10072, "uxtab r0, r1, r2"},
        {0xE1910F9F, "ldrex r0, [r1]"},
        {0xE1810F92, "strex r0, r2, [r1]"},
        {0x16BF0F31, "revne r0, r1"},
        {0xEE300A81, "vadd.f32 s0, s1, s2"},
        {0xEEB00A60, "vmov.f32 s0, s1"},
        {0xED2D8B10, "vpush {d8-d15}"},
        {0xED910A01, "vldr s0, [r1, #4]"},
        {0xEEF1FA10, "vmrs APSR_nzcv, fpscr"},
        {0xF57FF01F, "clrex"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(text_of(c.word), string(c.text));
        CB_CHECK_EQ(word_of(c.text), c.word);
    }
    CB_CHECK_EQ(ex::find_arm(0xE6BF0F31, a::Arch::V5TE), -1);   // (the ARMv5TE has none of them)
});

CB_TEST("decompile.arm_ext.refusals", [] {
    { CB_CHECK_THROWS_AS(cb::ParseError, word_of("uxtb r0, r1, ror #3")); }
    { CB_CHECK_THROWS_AS(cb::ParseError, word_of("vadd.f32 s0, s1")); }
    CB_CHECK(!ex::assemble("mov r0, r1", 0x1000, {}, a::Arch::V6K).has_value());
    CB_CHECK(!ex::assemble("rev r0, r1", 0x1000, {}, a::Arch::V4T).has_value());
});

// Every word a row matches reads back to itself: a spread of the 32-bit space, the fixed bits of each row set.
CB_TEST("decompile.arm_ext.round_trip", [] {
    std::uint32_t x = 0x9E3779B9u;
    unsigned checked = 0;
    for (unsigned n = 0; n < 1500000; ++n) {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        const std::uint32_t word = (n & 1) ? (x & 0x0FFFFFFFu) | 0xE0000000u : x;
        const int row = ex::find_arm(word, a::Arch::V7A);
        if (row < 0) continue;
        const string line = ex::text(row, word, 0x1000, {});
        const auto back = ex::assemble(line, 0x1000, {}, a::Arch::V7A);
        CB_CHECK(back.has_value());
        if (!back || *back != word) {
            CB_CHECK_EQ(line, string("a line that assembles to the word"));
            return;
        }
        ++checked;
    }
    CB_CHECK(checked > 1000);
});
