#include "cb/decompile/xenos.hpp"

#include <format>
#include <string>
#include <vector>

#include "cb/core/testing.hpp"

namespace x = cb::decompile::xenos;

// DIVERGES from the Python module: the words are unsigned 32-bit (Python took any integer, including negative
// and wider ones, and the old harness fed it those); error text and the `python -m` command line are not part of
// the module (the command table of cb/app will own a `xenos` command).

namespace {

std::vector<std::string> sample_lines() { return x::disassemble(x::kSample); }

}  // namespace

// The shader fxc built (/XuFormatRaw /Fh) with fxc's own listing: "alloc colors", "exece", "muls r0.z, r0.xy",
// "mov oC0, r0.xyzx" (fxc spells the swizzle relative: r0.xy is the same channels as r0.yyxx written out).
CB_TEST("decompile.xenos.sample_shader", [] {
    auto got = sample_lines();
    CB_CHECK_EQ(got.size(), 3u);
    CB_CHECK_EQ(got[0], "CF: alloc / exece");
    CB_CHECK(got[1].starts_with("muls r0.z, r0."));
    CB_CHECK_EQ(got[1], "muls r0.z, r0.yyxx");
    CB_CHECK_EQ(got[2], "mov oC0, r0.xyzx");
    CB_CHECK_EQ(x::kSampleListing[3], "mov oC0, r0.xyzx");
});

CB_TEST("decompile.xenos.program_layout", [] {
    CB_CHECK(x::disassemble({}).empty());
    const std::uint32_t two[2] = {0, 0};
    CB_CHECK(x::disassemble(two).empty());                       // fewer than a CF triple
    const std::uint32_t cf_only[3] = {0, 0, 0};
    CB_CHECK(x::disassemble(cf_only) == std::vector<std::string>{"CF: nop / nop"});
    // a trailing partial ALU word (fewer than three words) is not decoded
    std::vector<std::uint32_t> words(x::kSample.begin(), x::kSample.begin() + 11);
    CB_CHECK_EQ(x::disassemble(words).size(), 3u);
    words.resize(7);   // CF, one full ALU word, one stray word
    CB_CHECK_EQ(x::disassemble(words).size(), 2u);
    // the zero word ends the program; whatever follows is ignored
    std::vector<std::uint32_t> after(x::kSample.begin(), x::kSample.end());
    after.insert(after.end(), {0x00000000, 0x2001c400, 0x22000000});
    CB_CHECK_EQ(x::disassemble(after).size(), 3u);
});

CB_TEST("decompile.xenos.masks_and_swizzles", [] {
    // every mask against a reference written bit by bit
    for (std::uint32_t m = 0; m < 16; ++m) {
        std::string want;
        if (m & 1) want += 'x';
        if (m & 2) want += 'y';
        if (m & 4) want += 'z';
        if (m & 8) want += 'w';
        CB_CHECK_EQ(x::mask_text(m), want);
    }
    CB_CHECK_EQ(x::mask_text(0x10), "");   // bits past the fourth are ignored
    // every swizzle byte: channel i reads channel (i + field) mod 4
    for (std::uint32_t sw = 0; sw < 256; ++sw) {
        std::string want;
        for (std::uint32_t i = 0; i < 4; ++i) want += "xyzw"[(i + ((sw >> (2 * i)) & 3)) % 4];
        CB_CHECK_EQ(x::swizzle_text(sw), want);
    }
    CB_CHECK_EQ(x::swizzle_text(0), "xyzw");
    CB_CHECK_EQ(x::swizzle_text(0x55), "yzwx");
});

CB_TEST("decompile.xenos.registers", [] {
    CB_CHECK_EQ(x::register_text(true, 3), "r3");
    CB_CHECK_EQ(x::register_text(false, 3), "c3");
    CB_CHECK_EQ(x::register_text(true, 0x3f), "r63");
    CB_CHECK_EQ(x::register_text(true, 0x43), "r3[aL]");        // bit 6: indexed by the loop counter
    CB_CHECK_EQ(x::register_text(false, 0x83), "c131_abs");     // bit 7: absolute value (constants keep their full number)
    CB_CHECK_EQ(x::register_text(true, 0xC5), "r5_abs[aL]");
});

CB_TEST("decompile.xenos.opcode_tables", [] {
    // all named vector opcodes; 0x1e and 0x1f have no name
    const char* names[] = {"add", "mul", "max", "max", "seq", "sgt", "sge", "sne", "frc", "trunc", "floor", "mad",
                           "cndeq", "cndge", "cndgt", "dp4", "dp3", "dp2add", "cube", "max4", "setp_eq_push",
                           "setp_ne_push", "setp_gt_push", "setp_ge_push", "kille", "killgt", "killge", "killne",
                           "dst", "mova"};
    for (std::uint32_t op = 0; op < 30; ++op) CB_CHECK_EQ(x::vector_name(op), names[op]);
    CB_CHECK_EQ(x::vector_name(0x1e), "");
    CB_CHECK_EQ(x::vector_name(0x20), "");
    CB_CHECK_EQ(x::vector_sources(0x0b), 3u);   // mad
    CB_CHECK_EQ(x::vector_sources(0x08), 1u);   // frc
    CB_CHECK_EQ(x::vector_sources(0x1e), 0u);
    CB_CHECK_EQ(x::vector_sources(99), 3u);
    CB_CHECK_EQ(x::scalar_name(0x02), "muls");
    CB_CHECK_EQ(x::scalar_name(0x31), "cos");
    CB_CHECK_EQ(x::scalar_name(0x19), "");      // a gap in the table
    CB_CHECK_EQ(x::scalar_name(0x29), "");
    CB_CHECK_EQ(x::scalar_name(0x32), "");
    CB_CHECK_EQ(x::control_name(0), "nop");
    CB_CHECK_EQ(x::control_name(0xc), "alloc");
    CB_CHECK_EQ(x::control_name(0xd), "");
});

CB_TEST("decompile.xenos.control_ops", [] {
    // the CF word pair is two 48-bit instructions; the opcode is bits 44..47 of each (d1 bits 12..15, d2 bits 28..31)
    for (std::uint32_t a = 0; a < 16; ++a) {
        for (std::uint32_t b = 0; b < 16; ++b) {
            auto got = x::control(0xdeadbeef, a << 12, b << 28);
            auto name = [](std::uint32_t op) {
                std::string_view n = x::control_name(op);
                return n.empty() ? std::format("cf{:x}", op) : std::string(n);
            };
            CB_CHECK_EQ(got.size(), 2u);
            CB_CHECK_EQ(got[0], name(a));
            CB_CHECK_EQ(got[1], name(b));
        }
    }
    CB_CHECK_EQ(x::control(0, 0xc000, 0xc0000000)[0], "alloc");
    CB_CHECK_EQ(x::control(0, 0xd000, 0xe0000000)[0], "cfd");
    CB_CHECK_EQ(x::control(0, 0xd000, 0xe0000000)[1], "cfe");
});

CB_TEST("decompile.xenos.alu_forms", [] {
    // no destination mask and no export: nothing issues
    CB_CHECK(x::alu(0, 0, 0).empty());
    // vector op `add` (0) into r5.xy, sources r1.xyzw and c2.xyzw
    // d0: vdst=5, vmask=3<<16 ; d2: op 0, A = reg 1 at bits 16..23 (select r: bit 31), B = reg 2 at bits 8..15 (constant)
    std::uint32_t d0 = 5 | (3u << 16);
    std::uint32_t d2 = (1u << 16) | (2u << 8) | (1u << 31);
    auto got = x::alu(d0, 0, d2);
    CB_CHECK_EQ(got.size(), 1u);
    CB_CHECK_EQ(got[0], "add r5.xy, r1.xyzw, c2.xyzw");
    // saturate and negate on source A (d1 bit 26), swizzle on source B (d1 bits 8..15)
    got = x::alu(d0 | (1u << 24), 1u << 26, d2);
    CB_CHECK_EQ(got[0], "add_sat r5.xy, -r1.xyzw, c2.xyzw");
    got = x::alu(d0, 0x55u << 8, d2);
    CB_CHECK_EQ(got[0], "add r5.xy, r1.xyzw, c2.yzwx");
    // export wins over the register destination
    got = x::alu(d0 | (1u << 15), 0, d2);
    CB_CHECK_EQ(got[0], "add oC0, r1.xyzw, c2.xyzw");
    // max of a source with itself is a move
    std::uint32_t d2max = (2u << 24) | (1u << 16) | (1u << 8) | (1u << 31) | (1u << 30);
    CB_CHECK_EQ(x::alu(d0, 0, d2max)[0], "mov r5.xy, r1.xyzw");
    CB_CHECK_EQ(x::alu(d0 | (1u << 24), 0, d2max)[0], "mov_sat r5.xy, r1.xyzw");
    // ... but max of two different sources is a max
    CB_CHECK_EQ(x::alu(d0, 0, (2u << 24) | (1u << 16) | (2u << 8) | (1u << 31) | (1u << 30))[0],
                "max r5.xy, r1.xyzw, r2.xyzw");
    // a three-source op and a one-source op print as many sources as the opcode reads
    CB_CHECK_EQ(x::alu(d0, 0, (0x0bu << 24) | d2)[0], "mad r5.xy, r1.xyzw, c2.xyzw, c0.xyzw");
    CB_CHECK_EQ(x::alu(d0, 0, (0x08u << 24) | d2)[0], "frc r5.xy, r1.xyzw");
    // an unnamed opcode is spelled by number
    CB_CHECK(x::alu(d0, 0, (0x1eu << 24) | d2)[0].starts_with("vop1e r5.xy"));
    // scalar op: reads source C; sop 0x02 = muls, mask z into r7
    std::uint32_t s0 = (0x02u << 26) | (4u << 20) | (7u << 8);
    got = x::alu(s0, 0, 1u << 29);   // source C: select bit 29 -> temporary, register 0
    CB_CHECK_EQ(got.size(), 1u);
    CB_CHECK_EQ(got[0], "muls r7.z, r0.xyzw");
    CB_CHECK_EQ(x::alu(s0 | (1u << 25), 0, 1u << 29)[0], "muls_sat r7.z, r0.xyzw");
    CB_CHECK(x::alu((0x19u << 26) | (1u << 20), 0, 0)[0].starts_with("sop19 r0.x"));
    // vector and scalar issue together, vector first
    got = x::alu(d0 | s0, 0, d2);
    CB_CHECK_EQ(got.size(), 2u);
    CB_CHECK(got[0].starts_with("add ") && got[1].starts_with("muls "));
});
