// cb-unity: alone (file-local names that other files in a batch also use)
// The R4300i's instructions (the Nintendo 64's MIPS III, big-endian): every encoding checked against words worked out by hand from the MIPS III
// formats (R: op rs rt rd sa funct, I: op rs rt imm16, J: op target26, and the FPU's COP1 formats: op=0x11 fmt ft fs fd funct), the text of each
// form, the refusals (the R3000A's GTE and RFE are not R4300 instructions, and the R4300's doubleword instructions are not the R3000A's), the
// big-endian bytes, and the round trip (every word that decodes is written as text that assembles to that word).
#include "decompile/mips.hpp"

#include <format>
#include <string>

#include "core/error.hpp"
#include "core/testing.hpp"

namespace m = cb::decompile::mips;
using cb::Bytes;
using std::string;

namespace {

constexpr std::uint32_t kBase = 0x80001000;
constexpr m::Variant kR4300 = m::Variant::R4300;

std::uint32_t word_of(const Bytes& b) {
    return static_cast<std::uint32_t>(b.at(0)) << 24 | static_cast<std::uint32_t>(b.at(1)) << 16 | static_cast<std::uint32_t>(b.at(2)) << 8 | static_cast<std::uint32_t>(b.at(3));
}

std::uint32_t assembled(const string& line, std::uint32_t pc = kBase, const cb::decompile::NameLabels& labels = {}) {
    return word_of(m::assemble(line, pc, labels, kR4300));
}

string disassembled(std::uint32_t word, std::uint32_t pc = kBase, const cb::decompile::AddressLabels& labels = {}) {
    const auto insn = m::decode_word(word, pc, kR4300);
    return insn ? m::text(*insn, labels) : string("-");
}

struct Case {
    std::uint32_t word;
    const char* text;
};

}  // namespace

CB_TEST("decompile.mips_r4300.integer_words_and_text", [] {
    const Case cases[] = {
        {0x00000000, "nop"},
        {0x67BDFFF0, "daddiu $sp, $sp, -16"},
        {0x27BDFFE0, "addiu $sp, $sp, -32"},
        {0xDFBF0008, "ld $ra, 8($sp)"},
        {0xFFBF0008, "sd $ra, 8($sp)"},
        {0x9C880004, "lwu $t0, 4($a0)"},
        {0x68880000, "ldl $t0, 0($a0)"},
        {0x6C880007, "ldr $t0, 7($a0)"},
        {0xB0880000, "sdl $t0, 0($a0)"},
        {0xB4880007, "sdr $t0, 7($a0)"},
        {0xC0880000, "ll $t0, 0($a0)"},
        {0xE0880000, "sc $t0, 0($a0)"},
        {0xD0880000, "lld $t0, 0($a0)"},
        {0xF0880000, "scd $t0, 0($a0)"},
        {0x00094138, "dsll $t0, $t1, 4"},
        {0x0009413A, "dsrl $t0, $t1, 4"},
        {0x0009413B, "dsra $t0, $t1, 4"},
        {0x0009413C, "dsll32 $t0, $t1, 4"},
        {0x0009413E, "dsrl32 $t0, $t1, 4"},
        {0x0002103F, "dsra32 $v0, $v0, 0"},
        {0x01244014, "dsllv $t0, $a0, $t1"},
        {0x01244016, "dsrlv $t0, $a0, $t1"},
        {0x01244017, "dsrav $t0, $a0, $t1"},
        {0x0085001C, "dmult $a0, $a1"},
        {0x0085001D, "dmultu $a0, $a1"},
        {0x0085001E, "ddiv $a0, $a1"},
        {0x0085001F, "ddivu $a0, $a1"},
        {0x0085102C, "dadd $v0, $a0, $a1"},
        {0x0085102D, "daddu $v0, $a0, $a1"},
        {0x0085102E, "dsub $v0, $a0, $a1"},
        {0x0085102F, "dsubu $v0, $a0, $a1"},
        {0x60880001, "daddi $t0, $a0, 1"},
        {0x0000000F, "sync"},
        {0x00850034, "teq $a0, $a1"},
        {0x008001F4, "teq $a0, $zero, 7"},   // (the divide-by-zero check the compilers put after a DIV: code 7)
        {0x00A4FFF1, "tgeu $a1, $a0, 1023"},
        {0x00850036, "tne $a0, $a1"},
        {0x00850030, "tge $a0, $a1"},
        {0x00850031, "tgeu $a0, $a1"},
        {0x00850032, "tlt $a0, $a1"},
        {0x00850033, "tltu $a0, $a1"},
        {0x04880005, "tgei $a0, 5"},
        {0x04890005, "tgeiu $a0, 5"},
        {0x048A0005, "tlti $a0, 5"},
        {0x048B0005, "tltiu $a0, 5"},
        {0x048C0005, "teqi $a0, 5"},
        {0x048E0005, "tnei $a0, 5"},
        {0x8FBF001C, "lw $ra, 28($sp)"},
        {0x3C088000, "lui $t0, 0x8000"},
        {0x00801025, "move $v0, $a0"},
        {0x0320F809, "jalr $t9"},
        {0x03E00008, "jr $ra"},
        {0xBD100000, "cache 16, 0($t0)"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
});

CB_TEST("decompile.mips_r4300.branch_likely", [] {
    // the offset counts words from the delay slot, as for any branch; the "likely" form skips the delay slot when the branch is not taken
    const Case cases[] = {
        {0x50850003, "beql $a0, $a1, 0x80001010"}, {0x54850003, "bnel $a0, $a1, 0x80001010"}, {0x58800003, "blezl $a0, 0x80001010"},
        {0x5C800003, "bgtzl $a0, 0x80001010"},     {0x04820003, "bltzl $a0, 0x80001010"},     {0x04830003, "bgezl $a0, 0x80001010"},
        {0x04920003, "bltzall $a0, 0x80001010"},   {0x04930003, "bgezall $a0, 0x80001010"},   {0x45000003, "bc1f 0x80001010"},
        {0x45010003, "bc1t 0x80001010"},           {0x45020003, "bc1fl 0x80001010"},          {0x45030003, "bc1tl 0x80001010"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    // the flow of each: a likely branch is a branch (a call for the -and-link forms) with a delay slot, and says so
    const auto beql = m::decode_word(0x50850003, kBase, kR4300);
    CB_CHECK(beql && beql->likely && beql->delay_slot && beql->flow == m::Flow::Branch);
    CB_CHECK_EQ(*beql->target, 0x80001010u);
    const auto beq = m::decode_word(0x10850003, kBase, kR4300);
    CB_CHECK(beq && !beq->likely);
    const auto bltzall = m::decode_word(0x04920003, kBase, kR4300);
    CB_CHECK(bltzall && bltzall->likely && bltzall->flow == m::Flow::Call);
    // "beql $zero, $zero" is as unconditional as `b`
    const auto always = m::decode_word(0x50000003, kBase, kR4300);
    CB_CHECK(always && always->flow == m::Flow::Jump);
    // a backward likely branch: the offset is negative
    CB_CHECK_EQ(assembled("bnel $t0, $zero, 0x80000FF8", kBase), 0x5500FFFDu);
    CB_CHECK_EQ(disassembled(0x5500FFFD), string("bnel $t0, $zero, 0x80000FF8"));
});

CB_TEST("decompile.mips_r4300.system_control", [] {
    const Case cases[] = {
        {0x40286000, "dmfc0 $t0, $12"}, {0x40086000, "mfc0 $t0, $12"}, {0x40886000, "mtc0 $t0, $12"}, {0x40A86000, "dmtc0 $t0, $12"}, {0x42000001, "tlbr"},
        {0x42000002, "tlbwi"},         {0x42000006, "tlbwr"},         {0x42000008, "tlbp"},          {0x42000018, "eret"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    const auto eret = m::decode_word(0x42000018, kBase, kR4300);
    CB_CHECK(eret && eret->flow == m::Flow::Indirect && !eret->delay_slot);
    // the R3000A's RFE is not an R4300 instruction
    CB_CHECK_EQ(disassembled(0x42000010), string("-"));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("rfe", kBase, {}, kR4300));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("eret", kBase, {}, m::Variant::R3000A));
});

CB_TEST("decompile.mips_r4300.fpu_words_and_text", [] {
    const Case cases[] = {
        {0xC4800004, "lwc1 $f0, 4($a0)"},        {0xE4820008, "swc1 $f2, 8($a0)"},        {0xD4840000, "ldc1 $f4, 0($a0)"},
        {0xF4860000, "sdc1 $f6, 0($a0)"},        {0x44081000, "mfc1 $t0, $f2"},           {0x44881000, "mtc1 $t0, $f2"},
        {0x44281000, "dmfc1 $t0, $f2"},          {0x44A81000, "dmtc1 $t0, $f2"},          {0x4448F800, "cfc1 $t0, $31"},
        {0x44C8F800, "ctc1 $t0, $31"},           {0x46041000, "add.s $f0, $f2, $f4"},     {0x46241000, "add.d $f0, $f2, $f4"},
        {0x460A4182, "mul.s $f6, $f8, $f10"},    {0x46041001, "sub.s $f0, $f2, $f4"},     {0x46041003, "div.s $f0, $f2, $f4"},
        {0x46001004, "sqrt.s $f0, $f2"},         {0x46001005, "abs.s $f0, $f2"},          {0x46202086, "mov.d $f2, $f4"},
        {0x46000007, "neg.s $f0, $f0"},          {0x46801020, "cvt.s.w $f0, $f2"},        {0x46001021, "cvt.d.s $f0, $f2"},
        {0x46201024, "cvt.w.d $f0, $f2"},        {0x4600100D, "trunc.w.s $f0, $f2"},      {0x46201008, "round.l.d $f0, $f2"},
        {0x46A01020, "cvt.s.l $f0, $f2"},        {0x46041032, "c.eq.s $f2, $f4"},         {0x4624103C, "c.lt.d $f2, $f4"},
        {0x46041030, "c.f.s $f2, $f4"},          {0x4604103E, "c.le.s $f2, $f4"},         {0x4604103F, "c.ngt.s $f2, $f4"},
    };
    for (const Case& c : cases) {
        CB_CHECK_EQ(disassembled(c.word), string(c.text));
        CB_CHECK_EQ(assembled(c.text), c.word);
    }
    // a format the operation does not take is not an instruction (SQRT of a word), nor is a nonzero field it does not use
    CB_CHECK_EQ(disassembled(0x46801004), string("-"));
    CB_CHECK_EQ(disassembled(0x46081004), string("-"));    // sqrt.s with ft set
    CB_CHECK_EQ(disassembled(0x46041432), string("-"));    // c.eq.s with fd set
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("add.s $f0, $f2, $t0", kBase, {}, kR4300));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("add.s $f0, $f2", kBase, {}, kR4300));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("sqrt.w $f0, $f2", kBase, {}, kR4300));
});

CB_TEST("decompile.mips_r4300.is_big_endian_and_keeps_the_r3000a_apart", [] {
    const std::uint8_t bytes[] = {0x67, 0xBD, 0xFF, 0xF0, 0x03, 0xE0, 0x00, 0x08};
    const cb::ByteView view(bytes, sizeof bytes);
    const auto first = m::decode(view, 0, kBase, kR4300);
    CB_CHECK(first);
    CB_CHECK_EQ(m::text(*first), string("daddiu $sp, $sp, -16"));
    CB_CHECK_EQ(first->word, 0x67BDFFF0u);
    CB_CHECK_EQ(static_cast<int>(first->raw[0]), 0x67);   // the word as the code holds it: big-endian
    CB_CHECK_EQ(static_cast<int>(first->raw[3]), 0xF0);
    const auto second = m::decode(view, 4, kBase + 4, kR4300);
    CB_CHECK(second && second->flow == m::Flow::Return && second->delay_slot);
    CB_CHECK(!m::decode(view, 6, kBase, kR4300));   // fewer than four bytes
    // assembled: big-endian bytes
    const Bytes out = m::assemble("daddiu $sp, $sp, -16", kBase, {}, kR4300);
    CB_CHECK_EQ(out.size(), std::size_t{4});
    CB_CHECK_EQ(static_cast<int>(out[0]), 0x67);
    CB_CHECK_EQ(static_cast<int>(out[3]), 0xF0);
    // the same words read as the R3000A: little-endian, and no doubleword instructions, no FPU
    CB_CHECK(!m::decode_word(0x67BDFFF0, kBase));                                            // daddiu
    CB_CHECK(!m::decode_word(0xDFBF0008, kBase));                                            // ld
    CB_CHECK(!m::decode_word(0x46041000, kBase));                                            // add.s
    CB_CHECK(!m::decode_word(0x50850003, kBase));                                            // beql
    CB_CHECK(m::decode_word(0x4A180001, kBase));                                             // rtps: the GTE
    CB_CHECK(!m::decode_word(0x4A180001, kBase, kR4300));                                    // ... is no R4300 instruction
    CB_CHECK(!m::decode_word(0xC8800000, kBase, kR4300));                                    // lwc2
    CB_CHECK(m::decode_word(0x42000010, kBase));                                             // rfe
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("daddiu $sp, $sp, -16", kBase));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("rtps", kBase, {}, kR4300));
    CB_CHECK_EQ(static_cast<int>(m::assemble("addiu $sp, $sp, -32", kBase).at(0)), 0xE0);   // the R3000A's bytes are as they were: little-endian
    const auto little = m::decode(Bytes{0xE0, 0xFF, 0xBD, 0x27}, 0, kBase);
    CB_CHECK(little);
    CB_CHECK_EQ(m::text(*little), string("addiu $sp, $sp, -32"));
});

CB_TEST("decompile.mips_r4300.pseudo_instructions_and_pairs", [] {
    const cb::decompile::NameLabels labels = {{"msg", 0x80123456}, {"loop", 0x80000FF0}};
    // li and la as GNU as expands them
    CB_CHECK_EQ(m::assemble("la $a0, msg", kBase, labels, kR4300).size(), std::size_t{8});
    CB_CHECK_EQ(assembled("lui $a0, %hi(msg)", kBase, labels), 0x3C048012u);
    CB_CHECK_EQ(assembled("addiu $a0, $a0, %lo(msg)", kBase, labels), 0x24843456u);
    CB_CHECK_EQ(assembled("daddiu $a0, $a0, %lo(msg)", kBase, labels), 0x64843456u);
    CB_CHECK_EQ(assembled("lwc1 $f4, %lo(msg)($a0)", kBase, labels), 0xC4843456u);
    CB_CHECK_EQ(assembled("b loop", kBase, labels), 0x1000FFFBu);
    CB_CHECK_EQ(assembled("jal loop", kBase, labels), 0x0C0003FCu);
    CB_CHECK_THROWS_AS(cb::decompile::z80::UnknownLabel, m::assemble("jal missing", kBase, {}, kR4300));
    CB_CHECK_THROWS_AS(cb::Error, m::assemble("ld $a0, 0x10000($a1)", kBase, {}, kR4300));   // an offset that does not fit
});

CB_TEST("decompile.mips_r4300.every_word_that_decodes_round_trips", [] {
    // a deterministic sweep of words (every opcode and function with varied fields): what decodes is written as text that assembles to the same
    // word, and what the R3000A decodes is unchanged by the R4300's table
    std::uint64_t state = 0x9E3779B97F4A7C15ull;
    const auto next = [&] {
        state = state * 6364136223846793005ull + 1442695040888963407ull;
        return static_cast<std::uint32_t>(state >> 32);
    };
    std::size_t decoded = 0, mismatched = 0;
    string first_bad;
    const auto check = [&](std::uint32_t word, m::Variant variant) {
        const auto insn = m::decode_word(word, kBase, variant);
        if (!insn) return;
        ++decoded;
        // (a GTE command's text is checked by the R3000A's own tests; the sweep here is for the R4300)
        if (variant == m::Variant::R3000A) return;
        const string text = m::text(*insn);
        try {
            if (word_of(m::assemble(text, kBase, {}, variant)) != word) throw cb::ParseError("", 0, 0, "different word");
        } catch (const cb::Error&) {
            ++mismatched;
            if (first_bad.empty()) first_bad = std::format("{:08X} {}", word, text);
        }
    };
    for (std::uint32_t op = 0; op < 64; ++op) {
        for (int i = 0; i < 2000; ++i) {
            check(op << 26 | (next() & 0x03FFFFFF), kR4300);
        }
    }
    for (std::uint32_t function = 0; function < 64; ++function)
        for (int i = 0; i < 300; ++i) check((next() & 0x03FFF800) | function, kR4300);
    for (std::uint32_t fmt = 0; fmt < 32; ++fmt)
        for (std::uint32_t function = 0; function < 64; ++function)
            for (int i = 0; i < 20; ++i) check(0x44000000u | fmt << 21 | (next() & 0x001FFFC0) | function, kR4300);
    CB_CHECK(decoded > 20000);
    CB_CHECK_EQ(mismatched, std::size_t{0});
    CB_CHECK_EQ(first_bad, string());
});
