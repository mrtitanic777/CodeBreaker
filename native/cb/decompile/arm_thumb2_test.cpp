// cb/decompile/arm_thumb2_test.cpp -- ARMv7-A Thumb: the 32-bit branches worked out by hand, the 16-bit additions, the VFP's Thumb words, and the round trip.
#include <format>
#include <string>

#include "cb/decompile/arm.hpp"
#include "cb/decompile/arm_ext.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace a = cb::decompile::arm;
using cb::Bytes;
using std::string;

namespace {

constexpr std::uint32_t kPc = 0x1000;

string thumb_text(const Bytes& code) {
    const auto insn = a::decode(code, 0, kPc | 1, a::Arch::V7A);
    return insn ? a::text(*insn) : string("?");
}

Bytes assembled(const string& line) { return a::assemble(line, kPc | 1, {}, a::Arch::V7A); }

}  // namespace

CB_TEST("decompile.arm_thumb2.words_and_text", [] {
    struct Case {
        Bytes code;
        const char* text;
    };
    const Case cases[] = {
        {{0x00, 0xF0, 0x80, 0xB8}, "b.w 0x00001104"},          // F000 B880
        {{0xC8, 0xB2}, "uxtb r0, r1"},
        {{0x18, 0xB1}, "cbz r0, 0x0000100A"},
        {{0x0C, 0xBF}, "ite eq"},
        {{0x91, 0xED, 0x01, 0x0A}, "vldr s0, [r1, #4]"},
        {{0x00, 0xBF}, "nop.n"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(thumb_text(c.code), string(c.text));
        CB_CHECK(assembled(c.text) == c.code);
    }
});

CB_TEST("decompile.arm_thumb2.far_calls_round_trip", [] {
    for (const std::int64_t distance : {-0x500000LL, -0x400004LL, -4LL, 0LL, 0x7FFELL, 0x400000LL, 0x900000LL, 0xFFFFFELL}) {
        const std::uint32_t target = static_cast<std::uint32_t>(0x2000000 + distance);
        const string line = "bl 0x" + std::format("{:X}", target);
        const Bytes code = a::assemble(line, 0x2000000 | 1, {}, a::Arch::V7A);
        const auto insn = a::decode(code, 0, 0x2000000 | 1, a::Arch::V7A);
        CB_CHECK(insn.has_value());
        CB_CHECK(insn && insn->op == a::Op::Bl);
        CB_CHECK(insn && insn->target && (*insn->target & ~1u) == target);
    }
});
