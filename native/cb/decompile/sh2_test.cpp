// cb/decompile/sh2_test.cpp -- the SH-2's instructions: words worked out by hand from the SH7604 manual's instruction formats (the nibbles
// of 0xtnmd: t the group, n and m registers, d a field), the text of each form, delay slots and PC-relative operands, the assembler's
// refusals, and the round trip (every word that decodes is written as text that assembles to that word).
#include "cb/decompile/sh2.hpp"

#include <format>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace s = cb::decompile::sh2;
using cb::Bytes;
using std::string;

namespace {

constexpr std::uint32_t kBase = 0x06004000;

std::uint16_t word_of(const Bytes& b) { return static_cast<std::uint16_t>(b.at(0) << 8 | b.at(1)); }

std::uint16_t assembled(const string& line, std::uint32_t pc = kBase, const cb::decompile::NameLabels& labels = {}) {
    return word_of(s::assemble(line, pc, labels));
}

string disassembled(std::uint16_t word, std::uint32_t pc = kBase, const cb::decompile::AddressLabels& labels = {}) {
    const auto insn = s::decode_word(word, pc);
    return insn ? s::text(*insn, labels) : string("-");
}

struct Case {
    std::uint16_t word;
    const char* text;
};

}  // namespace

CB_TEST("decompile.sh2.words_and_text", [] {
    const Case cases[] = {
        {0x0009, "nop"},
        {0x000B, "rts"},
        {0x002B, "rte"},
        {0x0008, "clrt"},
        {0x0018, "sett"},
        {0x0028, "clrmac"},
        {0x0019, "div0u"},
        {0x001B, "sleep"},
        {0xE105, "mov     #5,r1"},
        {0xE1FF, "mov     #-1,r1"},
        {0x7FFC, "add     #-4,r15"},
        {0x6123, "mov     r2,r1"},
        {0x2122, "mov.l   r2,@r1"},
        {0x2F86, "mov.l   r8,@-r15"},
        {0x6FF6, "mov.l   @r15+,r15"},
        {0x6812, "mov.l   @r1,r8"},
        {0x6410, "mov.b   @r1,r4"},
        {0x6411, "mov.w   @r1,r4"},
        {0x1234, "mov.l   r3,@(16,r2)"},
        {0x5234, "mov.l   @(16,r3),r2"},
        {0x8013, "mov.b   r0,@(3,r1)"},
        {0x8122, "mov.w   r0,@(4,r2)"},
        {0x8437, "mov.b   @(7,r3),r0"},
        {0x8541, "mov.w   @(2,r4),r0"},
        {0x0344, "mov.b   r4,@(r0,r3)"},
        {0x035E, "mov.l   @(r0,r5),r3"},
        {0xC001, "mov.b   r0,@(1,gbr)"},
        {0xC208, "mov.l   r0,@(32,gbr)"},
        {0xC602, "mov.l   @(8,gbr),r0"},
        {0x0129, "movt    r1"},
        {0x6128, "swap.b  r2,r1"},
        {0x621D, "extu.w  r1,r2"},
        {0x31EC, "add     r14,r1"},
        {0x3123, "cmp/ge  r2,r1"},
        {0x8805, "cmp/eq  #5,r0"},
        {0x4115, "cmp/pl  r1"},
        {0x4110, "dt      r1"},
        {0x4101, "shlr    r1"},
        {0x4128, "shll16  r1"},
        {0x4119, "shlr8   r1"},
        {0xC8A5, "tst     #0xA5,r0"},
        {0xC901, "and     #1,r0"},
        {0xCD0F, "and.b   #0xF,@(r0,gbr)"},
        {0x2128, "tst     r2,r1"},
        {0x0127, "mul.l   r2,r1"},
        {0x321D, "dmuls.l r1,r2"},
        {0x021F, "mac.l   @r1+,@r2+"},
        {0x421F, "mac.w   @r1+,@r2+"},
        {0x0102, "stc     sr,r1"},
        {0x0212, "stc     gbr,r2"},
        {0x4F22, "sts.l   pr,@-r15"},
        {0x4F26, "lds.l   @r15+,pr"},
        {0x410E, "ldc     r1,sr"},
        {0x4117, "ldc.l   @r1+,gbr"},
        {0x012A, "sts     pr,r1"},
        {0x402A, "lds     r0,pr"},
        {0x400B, "jsr     @r0"},
        {0x412B, "jmp     @r1"},
        {0x0123, "braf    r1"},
        {0x0103, "bsrf    r1"},
        {0xC306, "trapa   #6"},
        {0x411B, "tas.b   @r1"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
});

CB_TEST("decompile.sh2.branches_and_targets", [] {
    // bra at 0x06004000 with a field of +3: target = pc + 4 + 6
    const auto bra = s::decode_word(0xA003, kBase);
    CB_CHECK(bra.has_value());
    CB_CHECK(bra->target && *bra->target == kBase + 10);
    CB_CHECK(bra->delay_slot);
    CB_CHECK(bra->flow == s::Flow::Jump);
    // a negative field reaches back
    const auto back = s::decode_word(0x8BFE, kBase);   // bf: field -2 -> pc + 4 - 4
    CB_CHECK(back && back->target && *back->target == kBase);
    CB_CHECK(back->flow == s::Flow::Branch);
    CB_CHECK(!back->delay_slot);
    const auto bfs = s::decode_word(0x8F00, kBase);
    CB_CHECK(bfs && bfs->delay_slot && bfs->mnemonic == "bf/s" && *bfs->target == kBase + 4);
    const auto bsr = s::decode_word(0xB7FF, kBase);   // field 0x7FF: the far end of the 12-bit range, 2046 + 4 bytes ahead
    CB_CHECK(bsr && bsr->flow == s::Flow::Call && *bsr->target == kBase + 4 + 2 * 0x7FF);
    const auto bra_back = s::decode_word(0xA800, kBase);   // 12-bit field 0x800 is -2048
    CB_CHECK(bra_back && *bra_back->target == kBase + 4 - 4096);
    const auto rts = s::decode_word(0x000B, kBase);
    CB_CHECK(rts && rts->flow == s::Flow::Return && rts->delay_slot);
    const auto jsr = s::decode_word(0x410B, kBase);
    CB_CHECK(jsr && jsr->flow == s::Flow::Call && jsr->delay_slot && !jsr->target);
    const auto jmp = s::decode_word(0x412B, kBase);
    CB_CHECK(jmp && jmp->flow == s::Flow::Indirect);

    // the assembler makes the fields from the targets
    CB_CHECK_EQ(assembled("bra     0x0600400A"), 0xA003);
    CB_CHECK_EQ(assembled("bf      0x06004000"), 0x8BFE);
    cb::decompile::NameLabels labels = {{"sub_06004100", 0x06004100}};
    CB_CHECK_EQ(assembled("bsr     sub_06004100", kBase, labels), 0xB07E);
    CB_CHECK_EQ(disassembled(0xB07E, kBase, {{0x06004100, "sub_06004100"}}), string("bsr     sub_06004100"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("bf 0x06004200"));        // past the 8-bit field
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("bra 0x06004001"));       // odd
    CB_CHECK_THROWS_AS(cb::decompile::z80::UnknownLabel, assembled("bra nowhere"));
});

CB_TEST("decompile.sh2.pc_relative_constants", [] {
    // mov.l @(disp,pc),rn: base is (pc + 4) with the low two bits cleared, disp is the field x 4
    const auto l = s::decode_word(0xD102, 0x06004002);   // r1: field 2 -> ((0x06004002 + 4) & ~3) + 8
    CB_CHECK(l && l->literal && *l->literal == 0x06004004 + 8);
    CB_CHECK(l->imm == 8);
    // mov.w: base is pc + 4, disp is the field x 2
    const auto w = s::decode_word(0x9205, kBase);
    CB_CHECK(w && w->literal && *w->literal == kBase + 4 + 10);
    // mova @(disp,pc),r0
    const auto mova = s::decode_word(0xC703, kBase);
    CB_CHECK(mova && mova->makes_address() && *mova->literal == kBase + 4 + 12);
    // with a label for the constant, the operand is the label; without, @(disp,pc)
    CB_CHECK_EQ(disassembled(0xD102, kBase, {{kBase + 4 + 8, "K_0600400C"}}), string("mov.l   K_0600400C,r1"));
    CB_CHECK_EQ(disassembled(0xD102, kBase), string("mov.l   @(8,pc),r1"));
    CB_CHECK_EQ(disassembled(0x9205, kBase), string("mov.w   @(10,pc),r2"));
    // and back
    cb::decompile::NameLabels labels = {{"K_0600400C", 0x0600400C}};
    CB_CHECK_EQ(assembled("mov.l   K_0600400C,r1", kBase, labels), 0xD102);
    CB_CHECK_EQ(assembled("mov.l   @(8,pc),r1"), 0xD102);
    CB_CHECK_EQ(assembled("mova    @(12,pc),r0"), 0xC703);
    // at an address that is not a multiple of four the longword base rounds down: the same constant is reached with a different field
    CB_CHECK_EQ(assembled("mov.l   K_0600400C,r1", 0x06004002, labels), 0xD102);
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mov.l K_0600400C,r1", kBase + 0x2000, {{"K_0600400C", 0x0600400C}}));   // behind the instruction
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mov.l @(6,pc),r1"));       // not a multiple of four
});

CB_TEST("decompile.sh2.not_instructions", [] {
    CB_CHECK(!s::decode_word(0x0000, kBase));   // zeros are not code
    CB_CHECK(!s::decode_word(0xFFFF, kBase));   // 0xF is the SH-4's floating point
    CB_CHECK(!s::decode_word(0x3001, kBase));
    CB_CHECK(!s::decode_word(0x0020, kBase));
    const Bytes one = {0x00};
    CB_CHECK(!s::decode(one, 0, kBase));
    const Bytes code = {0x00, 0x09, 0xE1, 0x05};
    const auto second = s::decode(code, 2, kBase + 2);
    CB_CHECK(second && second->word == 0xE105);
});

CB_TEST("decompile.sh2.refusals", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("frob r1,r2"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mov.l r1,r2,r3"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mov.w r1,@(4,r2)"));        // only r0 is the source of a short displacement store
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mov.l r1,@(2,r2)"));        // not a multiple of four
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mov.l r1,@(64,r2)"));       // past the 4-bit field
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mov #300,r1"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("and #-1,r0"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("mov r16,r1"));
    CB_CHECK_THROWS_AS(cb::ParseError, assembled("jmp r1"));
    CB_CHECK_EQ(assembled("mov r15,r1"), assembled("mov sp,r1"));
    CB_CHECK_EQ(assembled("MOV.L R1,@-R15"), 0x2F16);
    CB_CHECK_EQ(assembled("mov.l  r2 , @( 4 , r1 )"), 0x1121);
    CB_CHECK_EQ(assembled("mov #(2+3),r1"), 0xE105);
});

CB_TEST("decompile.sh2.every_word_round_trips", [] {
    int decoded = 0;
    for (std::uint32_t w = 0; w < 0x10000; ++w) {
        const auto insn = s::decode_word(static_cast<std::uint16_t>(w), kBase);
        if (!insn) continue;
        ++decoded;
        // (a branch's text names its target as an address, a constant as @(disp,pc): no labels are needed)
        const string line = s::text(*insn);
        CB_CHECK_EQ(assembled(line), static_cast<std::uint16_t>(w));
        if (assembled(line) != w) return;
    }
    CB_CHECK(decoded > 30000);   // (a dense instruction set: tens of thousands of the 65536 words are instructions)
});
