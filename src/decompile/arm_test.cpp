// decompile/arm_test.cpp -- the ARM7TDMI's instructions: words worked out by hand from the ARM and Thumb instruction formats (ARM
// Architecture Reference Manual, ARMv4T), the text of each form, the assembler's refusals, and the round trip (every word that decodes is
// written as text that assembles to that word: all of the Thumb halfwords, and a spread of ARM words).
#include "decompile/arm.hpp"

#include <format>
#include <string>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace a = cb::decompile::arm;
using cb::Bytes;
using std::string;

namespace {

constexpr std::uint32_t kBase = 0x08000100;

std::uint32_t word_of(const Bytes& b) {
    std::uint32_t v = 0;
    for (std::size_t i = 0; i < b.size() && i < 4; ++i) v |= static_cast<std::uint32_t>(b[i]) << (8 * i);
    return v;
}

std::uint32_t arm_assembled(const string& line, std::uint32_t pc = kBase, const cb::decompile::NameLabels& labels = {}) { return word_of(a::assemble(line, pc, labels)); }
Bytes thumb_assembled(const string& line, std::uint32_t pc = kBase, const cb::decompile::NameLabels& labels = {}) { return a::assemble(line, pc | 1, labels); }

string arm_text(std::uint32_t word, std::uint32_t pc = kBase, const cb::decompile::AddressLabels& labels = {}) {
    const auto insn = a::decode_arm(word, pc);
    return insn ? a::text(*insn, labels) : string("-");
}
string thumb_text(std::uint32_t word, std::uint32_t pc = kBase, const cb::decompile::AddressLabels& labels = {}) {
    const auto insn = a::decode_thumb(static_cast<std::uint16_t>(word), static_cast<std::uint16_t>(word >> 16), pc | 1);
    return insn ? a::text(*insn, labels) : string("-");
}

struct Case {
    std::uint32_t word;
    const char* text;
};

}  // namespace

CB_TEST("decompile.arm.arm_words_and_text", [] {
    const Case cases[] = {
        {0xE3A00000, "mov r0, #0"},
        {0xE1A00000, "nop"},
        {0xE1A0F00E, "mov pc, lr"},
        {0xE12FFF1E, "bx lr"},
        {0xE92D4010, "push {r4, lr}"},
        {0xE8BD8010, "pop {r4, pc}"},
        {0xE92D4FF0, "push {r4-r11, lr}"},
        {0xE5910004, "ldr r0, [r1, #4]"},
        {0xE5110004, "ldr r0, [r1, #-4]"},
        {0xE5910000, "ldr r0, [r1]"},
        {0xE52D0004, "str r0, [sp, #-4]!"},
        {0xE49D0004, "ldr r0, [sp], #4"},
        {0xE7910102, "ldr r0, [r1, r2, lsl #2]"},
        {0xE7110002, "ldr r0, [r1, -r2]"},
        {0xE5D10001, "ldrb r0, [r1, #1]"},
        {0xE4910004, "ldr r0, [r1], #4"},
        {0xE4B10004, "ldrt r0, [r1], #4"},
        {0xE4F10001, "ldrbt r0, [r1], #1"},
        {0xE0810002, "add r0, r1, r2"},
        {0xE0810102, "add r0, r1, r2, lsl #2"},
        {0xE0810132, "add r0, r1, r2, lsr r1"},
        {0xE0810062, "add r0, r1, r2, rrx"},
        {0xE0810022, "add r0, r1, r2, lsr #32"},
        {0xE2500001, "subs r0, r0, #1"},
        {0xE3500000, "cmp r0, #0"},
        {0x13500000, "cmpne r0, #0"},
        {0xE1500001, "cmp r0, r1"},
        {0xE1300001, "teq r0, r1"},
        {0xE3A004FF, "mov r0, #0xFF000000"},
        {0xE3A00100, "mov r0, #0, 2"},
        {0xE3E00000, "mvn r0, #0"},
        {0xE1B00001, "movs r0, r1"},
        {0xE0000291, "mul r0, r1, r2"},
        {0xE0200291, "mla r0, r1, r2, r0"},
        {0xE0834392, "umull r4, r3, r2, r3"},
        {0xE0C10392, "smull r0, r1, r2, r3"},
        {0xE1D100B0, "ldrh r0, [r1]"},
        {0xE1D100D0, "ldrsb r0, [r1]"},
        {0xE1D100F0, "ldrsh r0, [r1]"},
        {0xE1C100B0, "strh r0, [r1]"},
        {0xE1D120B4, "ldrh r2, [r1, #4]"},
        {0xE19100B2, "ldrh r0, [r1, r2]"},
        {0xE10F0000, "mrs r0, cpsr"},
        {0xE14F0000, "mrs r0, spsr"},
        {0xE121F000, "msr cpsr_c, r0"},
        {0xE129F000, "msr cpsr_fc, r0"},
        {0xE328F20F, "msr cpsr_f, #0xF0000000"},
        {0xE8B00006, "ldmia r0!, {r1, r2}"},
        {0xE8900006, "ldmia r0, {r1, r2}"},
        {0xE9000006, "stmdb r0, {r1, r2}"},
        {0xE8D07FFF, "ldmia r0, {r0-r12, sp, lr}^"},
        {0xEF060000, "swi 0x60000"},
        {0xE1010090, "swp r0, r0, [r1]"},
        {0xE1410090, "swpb r0, r0, [r1]"},
        {0x00000000, "andeq r0, r0, r0"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(arm_text(c.word), string(c.text));
        CB_CHECK_EQ(arm_assembled(c.text), c.word);
    }
});

CB_TEST("decompile.arm.arm_branches_use_labels_and_the_pc_offset", [] {
    cb::decompile::AddressLabels labels = {{kBase, "here"}, {kBase + 8, "next"}, {kBase + 0x100, "pool"}};
    cb::decompile::NameLabels names = {{"here", kBase}, {"next", kBase + 8}, {"pool", kBase + 0x100}};
    CB_CHECK_EQ(arm_text(0xEAFFFFFE, kBase, labels), "b here");
    CB_CHECK_EQ(arm_text(0xEB000000, kBase, labels), "bl next");
    CB_CHECK_EQ(arm_text(0x0A000000, kBase, labels), "beq next");
    CB_CHECK_EQ(arm_text(0x1A000001, kBase), "bne 0x0800010C");
    CB_CHECK_EQ(arm_assembled("b here", kBase, names), 0xEAFFFFFEu);
    CB_CHECK_EQ(arm_assembled("bl next", kBase, names), 0xEB000000u);
    CB_CHECK_EQ(arm_assembled("beq next", kBase, names), 0x0A000000u);
    CB_CHECK_EQ(arm_assembled("bleq next", kBase, names), 0x0B000000u);
    CB_CHECK_EQ(arm_assembled("blt next", kBase, names), 0xBA000000u);
    CB_CHECK_EQ(arm_assembled("bls next", kBase, names), 0x9A000000u);
    CB_CHECK_EQ(arm_assembled("bhs next", kBase, names), 0x2A000000u);
    // a PC-relative load of a label: the word at pc + 8 + offset
    const auto load = a::decode_arm(0xE59F00F8, kBase);   // ldr r0, [pc, #0xF8]: reads kBase + 8 + 0xF8
    CB_CHECK(load.has_value());
    CB_CHECK_EQ(a::text(*load, labels), "ldr r0, pool");
    CB_CHECK_EQ(a::text(*load), "ldr r0, [pc, #0xF8]");
    CB_CHECK_EQ(arm_assembled("ldr r0, pool", kBase, names), 0xE59F00F8u);
    CB_CHECK_EQ(arm_assembled("ldr r0, here", kBase, names), 0xE51F0008u);   // below the pc: the offset is subtracted
    CB_CHECK_EQ(arm_assembled("b . + 8", kBase), 0xEA000000u);
});

CB_TEST("decompile.arm.flow", [] {
    const auto flow = [](std::uint32_t w) { return a::decode_arm(w, kBase)->flow; };
    CB_CHECK(flow(0xE12FFF1E) == a::Flow::Return);                 // bx lr
    CB_CHECK(flow(0xE12FFF11) == a::Flow::Indirect);               // bx r1
    CB_CHECK(flow(0x012FFF1E) == a::Flow::None);                   // bxeq lr falls through
    CB_CHECK(flow(0xE1A0F00E) == a::Flow::Return);                 // mov pc, lr
    CB_CHECK(flow(0xE8BD8010) == a::Flow::Return);                 // pop {r4, pc}
    CB_CHECK(flow(0xE59FF000) == a::Flow::Indirect);               // ldr pc, [pc]
    CB_CHECK(flow(0xE080F100) == a::Flow::Indirect);               // add pc, r0, r0, lsl #2
    CB_CHECK(flow(0xEA000000) == a::Flow::Jump);
    CB_CHECK(flow(0x1A000000) == a::Flow::Branch);
    CB_CHECK(flow(0xEB000000) == a::Flow::Call);
    CB_CHECK(flow(0xEF000000) == a::Flow::None);
    const auto b = a::decode_arm(0xEA000002, kBase);
    CB_CHECK_EQ(*b->target, kBase + 8 + 8);
    const auto back = a::decode_arm(0xEAFFFFFC, kBase);
    CB_CHECK_EQ(*back->target, kBase - 8);
});

CB_TEST("decompile.arm.arm_encodings_that_are_not_instructions", [] {
    CB_CHECK(!a::decode_arm(0xF0000000, kBase));   // the NV condition
    CB_CHECK(!a::decode_arm(0xE1A10000, kBase));   // mov with a nonzero Rn
    CB_CHECK(!a::decode_arm(0xE151F001, kBase));   // cmp with a nonzero Rd
    CB_CHECK(!a::decode_arm(0xEE000000, kBase));   // a coprocessor instruction
    CB_CHECK(!a::decode_arm(0xE1C000D0, kBase));   // LDRD (ARMv5)
    CB_CHECK(!a::decode_arm(0xE8900000, kBase));   // ldm with no registers
    CB_CHECK(a::decode_arm(0xE0100090, kBase).has_value());   // muls r0, r0, r0
    CB_CHECK(!a::decode_arm(0xE0101090, kBase));   // mul with a nonzero Rn
    CB_CHECK(!a::decode(cb::ByteView(), 0, kBase));
});

CB_TEST("decompile.arm.thumb_words_and_text", [] {
    const Case cases[] = {
        {0x2000, "movs r0, #0"},
        {0x4770, "bx lr"},
        {0x4740, "bx r8"},
        {0xB510, "push {r4, lr}"},
        {0xBD10, "pop {r4, pc}"},
        {0xB4F0, "push {r4-r7}"},
        {0x1888, "adds r0, r1, r2"},
        {0x1A88, "subs r0, r1, r2"},
        {0x1C08, "adds r0, r1, #0"},
        {0x1E48, "subs r0, r1, #1"},
        {0x3001, "adds r0, #1"},
        {0x3801, "subs r0, #1"},
        {0x0088, "lsls r0, r1, #2"},
        {0x0888, "lsrs r0, r1, #2"},
        {0x1088, "asrs r0, r1, #2"},
        {0x0808, "lsrs r0, r1, #32"},
        {0x0008, "movs r0, r1"},
        {0x6848, "ldr r0, [r1, #4]"},
        {0x6008, "str r0, [r1]"},
        {0x7848, "ldrb r0, [r1, #1]"},
        {0x8848, "ldrh r0, [r1, #2]"},
        {0x5688, "ldrsb r0, [r1, r2]"},
        {0x5E88, "ldrsh r0, [r1, r2]"},
        {0x5288, "strh r0, [r1, r2]"},
        {0x5088, "str r0, [r1, r2]"},
        {0x4802, "ldr r0, [pc, #8]"},
        {0x9001, "str r0, [sp, #4]"},
        {0x9802, "ldr r0, [sp, #8]"},
        {0xA801, "add r0, sp, #4"},
        {0xA002, "add r0, pc, #8"},
        {0xB002, "add sp, #8"},
        {0xB082, "sub sp, #8"},
        {0x2800, "cmp r0, #0"},
        {0x4288, "cmp r0, r1"},
        {0x4540, "cmp r0, r8"},
        {0x46C0, "nop"},
        {0x4688, "mov r8, r1"},
        {0x4440, "add r0, r8"},
        {0x4348, "muls r0, r1, r0"},
        {0x4249, "negs r1, r1"},
        {0x4208, "tst r0, r1"},
        {0x4008, "ands r0, r1"},
        {0x4048, "eors r0, r1"},
        {0x4148, "adcs r0, r1"},
        {0x41C8, "rors r0, r1"},
        {0x4088, "lsls r0, r1"},
        {0x43C8, "mvns r0, r1"},
        {0xC006, "stmia r0!, {r1, r2}"},
        {0xC806, "ldmia r0!, {r1, r2}"},
        {0xC801, "ldmia r0, {r0}"},
        {0xDF06, "swi 0x6"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(thumb_text(c.word), string(c.text));
        const Bytes b = thumb_assembled(c.text);
        CB_CHECK_EQ(word_of(b), c.word);
    }
});

CB_TEST("decompile.arm.thumb_branches_and_the_long_call", [] {
    cb::decompile::AddressLabels labels = {{kBase, "here"}, {kBase + 8, "far"}};
    cb::decompile::NameLabels names = {{"here", kBase}, {"far", kBase + 8}};
    CB_CHECK_EQ(thumb_text(0xE7FE, kBase, labels), "b here");
    CB_CHECK_EQ(thumb_text(0xD0FE, kBase, labels), "beq here");
    CB_CHECK_EQ(thumb_text(0xD1FF, kBase), "bne 0x08000102");
    CB_CHECK_EQ(thumb_text(0xD001, kBase), "beq 0x08000106");
    CB_CHECK_EQ(word_of(thumb_assembled("b here", kBase, names)), 0xE7FEu);
    CB_CHECK_EQ(word_of(thumb_assembled("beq here", kBase, names)), 0xD0FEu);
    CB_CHECK_EQ(word_of(thumb_assembled("bgt far", kBase, names)), 0xDC02u);
    // BL: two halfwords, the high part of the offset first
    const auto bl = a::decode_thumb(0xF000, 0xF800, kBase | 1);
    CB_CHECK(bl.has_value());
    CB_CHECK_EQ(bl->length, 4);
    CB_CHECK_EQ(*bl->target, (kBase + 4) | 1u);
    CB_CHECK_EQ(thumb_text(0xF800F000, kBase, labels), "bl 0x08000104");
    const Bytes call = thumb_assembled("bl far", kBase, names);
    CB_CHECK_EQ(call.size(), std::size_t{4});
    CB_CHECK_EQ(word_of(call), 0xF802F000u);   // far = pc + 8: offset 4 past pc + 4
    const auto back = a::decode_thumb(0xF7FF, 0xFFFE, kBase | 1);   // offset -4 from pc + 4: back to pc
    CB_CHECK(back.has_value());
    CB_CHECK_EQ(*back->target, kBase | 1u);
    CB_CHECK(!a::decode_thumb(0xF000, 0x0000, kBase | 1));   // a first half with no second
    CB_CHECK(!a::decode_thumb(0xF800, 0xF800, kBase | 1));   // a second half alone
    CB_CHECK(!a::decode_thumb(0xE800, 0x0000, kBase | 1));   // BLX is ARMv5
    CB_CHECK(!a::decode_thumb(0x4600, 0, kBase | 1));        // mov between two low registers
    CB_CHECK(!a::decode_thumb(0xB400, 0, kBase | 1));        // push of nothing
    CB_CHECK(!a::decode_thumb(0xDE00, 0, kBase | 1));        // the undefined condition
    // flow
    const auto flow = [](std::uint32_t w) { return a::decode_thumb(static_cast<std::uint16_t>(w), 0, kBase | 1)->flow; };
    CB_CHECK(flow(0x4770) == a::Flow::Return);
    CB_CHECK(flow(0x4708) == a::Flow::Indirect);   // bx r1
    CB_CHECK(flow(0xBD10) == a::Flow::Return);
    CB_CHECK(flow(0xE7FE) == a::Flow::Jump);
    CB_CHECK(flow(0xD0FE) == a::Flow::Branch);
    CB_CHECK(flow(0x46F7) == a::Flow::Return);     // mov pc, lr
});

CB_TEST("decompile.arm.assembler_refusals", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("mov r0, #0x101"));          // not an 8-bit value rotated
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("frob r0"));
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("ldr r0, =0x1234"));         // literal pools are not assembled
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("add r0"));                  // an operand is missing
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("mov r0, r16"));
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("mov r0, r1", kBase + 2));   // not at a multiple of four
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("b 0x0C000000"));            // out of reach (32 MB and more)
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("ldr r0, [r1, #4096]"));
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("ldrh r0, [r1, #256]"));
    CB_CHECK_THROWS_AS(cb::ParseError, arm_assembled("ldm r0, {}"));
    CB_CHECK_THROWS_AS(cb::decompile::z80::UnknownLabel, arm_assembled("b nowhere"));
    CB_CHECK_THROWS_AS(cb::ParseError, thumb_assembled("movs r8, #1"));           // a low register
    CB_CHECK_THROWS_AS(cb::ParseError, thumb_assembled("movs r0, #256"));
    CB_CHECK_THROWS_AS(cb::ParseError, thumb_assembled("add r0, r1"));            // two low registers: sets no flags in no encoding
    CB_CHECK_THROWS_AS(cb::ParseError, thumb_assembled("ldr r0, [r1, #3]"));      // not a multiple of four
    CB_CHECK_THROWS_AS(cb::ParseError, thumb_assembled("moveq r0, r1"));          // no conditional execution in Thumb
    CB_CHECK_THROWS_AS(cb::ParseError, thumb_assembled("beq 0x08000500"));        // out of reach
    CB_CHECK_THROWS_AS(cb::ParseError, thumb_assembled("mul r0, r1, r2"));        // an ARM instruction
});

CB_TEST("decompile.arm.expressions", [] {
    cb::decompile::NameLabels names = {{"sym", 0x08000200}};
    CB_CHECK_EQ(a::value("sym + 4", names), 0x08000204);
    CB_CHECK_EQ(a::value("(1 << 4) | 3", names), 19);
    CB_CHECK_EQ(a::value("0x10 + 0b101", names), 21);
    CB_CHECK_EQ(a::value(". + 8", names, 0x100), 0x108);
    CB_CHECK_EQ(a::value("~0 & 0xFF", names), 0xFF);
    CB_CHECK_THROWS_AS(cb::decompile::z80::UnknownLabel, a::value("missing", names));
    CB_CHECK_THROWS_AS(cb::ParseError, a::value("1 +", names));
    CB_CHECK_EQ(arm_assembled("mov r0, #sym >> 20", kBase, names), 0xE3A00080u);
});

CB_TEST("decompile.arm.thumb_round_trip_of_every_halfword", [] {
    // every halfword that decodes is written as text that assembles back to it; the one family that does not is the 3-bit
    // add/subtract of an immediate to a register itself (`adds r0, r0, #3`), which GNU as writes in the 8-bit format
    std::size_t decoded = 0, exceptions = 0;
    for (std::uint32_t h = 0; h < 0x10000; ++h) {
        const std::uint32_t word = (h >> 11) == 0x1E ? (h | 0xF8000000u) : h;   // (a BL's first half, with a second half that goes with it)
        const auto insn = a::decode_thumb(static_cast<std::uint16_t>(word), static_cast<std::uint16_t>(word >> 16), kBase | 1);
        if (!insn) continue;
        ++decoded;
        const string t = a::text(*insn);
        const Bytes b = a::assemble(t, kBase | 1);
        const bool same = b == Bytes(insn->bytes().begin(), insn->bytes().end());
        const bool f2_same_register = (h >> 11) == 3 && ((h >> 10) & 1) && (h & 7) == ((h >> 3) & 7);
        if (!same) ++exceptions;
        CB_CHECK(same || f2_same_register);
    }
    CB_CHECK(decoded > 55000);
    CB_CHECK_EQ(exceptions, std::size_t{16 * 8});   // 2 (add, sub) x 8 (immediates) x 8 (registers)
});

CB_TEST("decompile.arm.arm_round_trip_of_a_spread_of_words", [] {
    std::uint64_t state = 0x2545F4914F6CDD1DULL;
    std::size_t decoded = 0, failures = 0;
    for (unsigned n = 0; n < 400000; ++n) {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        std::uint32_t w = static_cast<std::uint32_t>(state >> 32);
        if (n % 3 == 0) w = (w & 0x0FFFFFFFu) | 0xE0000000u;   // (mostly "always": more of the words are of the common kinds)
        const auto insn = a::decode_arm(w, kBase);
        if (!insn) continue;
        ++decoded;
        const string t = a::text(*insn);
        string result;
        try {
            result = std::format("{:08X}", word_of(a::assemble(t, kBase)));
        } catch (const cb::Error& e) {
            result = std::string("did not assemble: ") + e.what();
        }
        const string expected = std::format("{:08X}", w);
        if (result != expected && ++failures <= 10) CB_CHECK_EQ(std::format("{} {}", t, result), std::format("{} {}", t, expected));
    }
    CB_CHECK_EQ(failures, std::size_t{0});
    CB_CHECK(decoded > 100000);
});

// ---- ARMv5TE (the Nintendo DS's ARM946E-S) ---------------------------------------------------------------------------------------

namespace {

string v5_text(std::uint32_t word, std::uint32_t pc = kBase) {
    const auto insn = a::decode_arm(word, pc, a::Arch::V5TE);
    return insn ? a::text(*insn) : string("-");
}

std::uint32_t v5_assembled(const string& line, std::uint32_t pc = kBase) { return word_of(a::assemble(line, pc, {}, a::Arch::V5TE)); }

}  // namespace

CB_TEST("decompile.arm.armv5te_words_and_text", [] {
    // each word worked out from the ARM Architecture Reference Manual's encodings (ARMv5TE)
    const Case cases[] = {
        {0xE12FFF33, "blx r3"},                  // BLX rm: cond 0001 0010 1111 1111 1111 0011 rm
        {0xE16F0F11, "clz r0, r1"},              // CLZ rd, rm: 0001 0110 1111 rd 1111 0001 rm
        {0xE1010052, "qadd r0, r2, r1"},         // QADD rd, rm, rn: 0001 0000 rn rd 0000 0101 rm
        {0xE1210052, "qsub r0, r2, r1"},
        {0xE1410052, "qdadd r0, r2, r1"},
        {0xE1610052, "qdsub r0, r2, r1"},
        {0xE1030281, "smlabb r3, r1, r2, r0"},   // SMLAxy rd, rm, rs, rn: 0001 0000 rd rn rs 1 y x 0 rm
        {0xE10302A1, "smlatb r3, r1, r2, r0"},   // (x is bit 5: the top half of rm)
        {0xE10302C1, "smlabt r3, r1, r2, r0"},   // (y is bit 6: the top half of rs)
        {0xE10302E1, "smlatt r3, r1, r2, r0"},
        {0xE1630281, "smulbb r3, r1, r2"},
        {0xE1454281, "smlalbb r4, r5, r1, r2"},  // SMLALxy rdlo, rdhi, rm, rs
        {0xE1230281, "smlawb r3, r1, r2, r0"},   // SMLAWy rd, rm, rs, rn: 0001 0010 rd rn rs 1 y 0 0 rm
        {0xE12302C1, "smlawt r3, r1, r2, r0"},
        {0xE12302A1, "smulwb r3, r1, r2"},       // SMULWy: bit 5 set, rn zero
        {0xE1C420D8, "ldrd r2, r3, [r4, #8]"},   // LDRD: the halfword format with L=0, S=1, H=0
        {0xE1C420F8, "strd r2, r3, [r4, #8]"},   // STRD: S=1, H=1
        {0xE1A400D2, "ldrd r0, r1, [r4, r2]!"},  // (a register offset, written back)
        {0xF5D1F004, "pld [r1, #4]"},            // PLD: 1111 0101 U101 rn 1111 imm12
        {0xF551F004, "pld [r1, #-4]"},
        {0xEE070F9A, "mcr p15, 0, r0, c7, c10, 4"},   // MCR cp, opc1, rd, crn, crm, opc2: 1110 opc1 0 crn rd cp opc2 1 crm
        {0xEE101F10, "mrc p15, 0, r1, c0, c0, 0"},
        {0xE1212374, "bkpt 0x1234"},
        {0xFA00003E, "blx 0x08000200"},          // BLX label: 1111 101 H imm24; the target is pc + 8 + imm24 * 4 + H * 2
        {0xFB00003E, "blx 0x08000202"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(v5_text(c.word), string(c.text));
        CB_CHECK_EQ(v5_assembled(c.text), c.word);
        CB_CHECK_EQ(arm_text(c.word), string("-"));   // none of them is an instruction of the ARM7TDMI
    }
    // the ARM7TDMI's assembler does not know them either
    CB_CHECK_THROWS_AS(cb::ParseError, a::assemble("clz r0, r1", kBase));
    CB_CHECK_THROWS_AS(cb::ParseError, a::assemble("blx r3", kBase));
    CB_CHECK_THROWS_AS(cb::ParseError, a::assemble("ldrd r2, r3, [r4]", kBase));
    CB_CHECK_THROWS_AS(cb::ParseError, a::assemble("ldrd r1, r2, [r4]", kBase, {}, a::Arch::V5TE));   // (an odd first register is undefined)
    // the flow: a BLX is a call, whatever its operand
    const auto call = a::decode_arm(0xFA00003E, kBase, a::Arch::V5TE);
    CB_CHECK(call && call->flow == a::Flow::Call && call->target && *call->target == (0x08000200u | 1u));   // (the target is Thumb: odd)
    const auto call_reg = a::decode_arm(0xE12FFF33, kBase, a::Arch::V5TE);
    CB_CHECK(call_reg && call_reg->flow == a::Flow::Call && !call_reg->target);
});

CB_TEST("decompile.arm.armv5te_thumb_blx", [] {
    // Thumb BLX rm: 0100 0111 1 rm 000
    const auto reg = a::decode_thumb(0x4798, 0, kBase | 1, a::Arch::V5TE);
    CB_CHECK(reg && a::text(*reg) == "blx r3" && reg->flow == a::Flow::Call);
    CB_CHECK(!a::decode_thumb(0x4798, 0, kBase | 1));   // (not on the ARM7TDMI)
    CB_CHECK(a::assemble("blx r3", kBase | 1, {}, a::Arch::V5TE) == (Bytes{0x98, 0x47}));
    // BLX label: the long-branch prefix and a second halfword 11101: the target is a word address in ARM state
    const auto call = a::decode_thumb(0xF000, 0xE87E, kBase | 1, a::Arch::V5TE);
    CB_CHECK(call && call->length == 4 && a::text(*call) == "blx 0x08000200");
    CB_CHECK(call->target && *call->target == 0x08000200u);   // (even: ARM state)
    CB_CHECK(!a::decode_thumb(0xF000, 0xE87E, kBase | 1));
    CB_CHECK(a::assemble("blx 0x08000200", kBase | 1, {}, a::Arch::V5TE) == (Bytes{0x00, 0xF0, 0x7E, 0xE8}));
    // a BL still is a BL
    const auto bl = a::decode_thumb(0xF000, 0xF87E, kBase | 1, a::Arch::V5TE);
    CB_CHECK(bl && a::text(*bl) == "bl 0x08000200" && *bl->target == (0x08000200u | 1u));
});

CB_TEST("decompile.arm.armv5te_round_trip_of_a_spread_of_words", [] {
    std::uint64_t state = 0x9E3779B97F4A7C15ULL;
    std::size_t decoded = 0, failures = 0, v5_only = 0;
    for (unsigned n = 0; n < 400000; ++n) {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        std::uint32_t w = static_cast<std::uint32_t>(state >> 32);
        if (n % 3 != 2) w = (w & 0x0FFFFFFFu) | (n % 7 == 0 ? 0xF0000000u : 0xE0000000u);
        const auto insn = a::decode_arm(w, kBase, a::Arch::V5TE);
        if (!insn) continue;
        ++decoded;
        if (!a::decode_arm(w, kBase)) ++v5_only;
        const string t = a::text(*insn);
        string result;
        try {
            result = std::format("{:08X}", word_of(a::assemble(t, kBase, {}, a::Arch::V5TE)));
        } catch (const cb::Error& e) {
            result = std::string("did not assemble: ") + e.what();
        }
        const string expected = std::format("{:08X}", w);
        if (result != expected && ++failures <= 10) CB_CHECK_EQ(std::format("{} {}", t, result), std::format("{} {}", t, expected));
    }
    CB_CHECK_EQ(failures, std::size_t{0});
    CB_CHECK(decoded > 100000);
    CB_CHECK(v5_only > 1000);   // (a fair share of the words are ones only the ARMv5TE has)
});

CB_TEST("decompile.arm.armv5te_thumb_round_trip_of_every_halfword_pair_kind", [] {
    std::size_t blx = 0;
    for (std::uint32_t h = 0; h < 0x10000; ++h) {
        const std::uint32_t word = (h >> 11) == 0x1E ? (h | 0xE8000000u) : h;   // (a prefix, with a BLX suffix)
        const auto insn = a::decode_thumb(static_cast<std::uint16_t>(word), static_cast<std::uint16_t>(word >> 16), kBase | 1, a::Arch::V5TE);
        if (!insn) continue;
        if (insn->op != a::Op::Blx) continue;
        ++blx;
        const string t = a::text(*insn);
        CB_CHECK(a::assemble(t, kBase | 1, {}, a::Arch::V5TE) == Bytes(insn->bytes().begin(), insn->bytes().end()));
    }
    CB_CHECK_EQ(blx, std::size_t{16 + 0x800});   // the sixteen `blx rm`, and a prefix for each of 2048
}
);
