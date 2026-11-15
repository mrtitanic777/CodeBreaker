// cb-unity: alone
// The AArch64 table: known encodings both ways, the preferred aliases, errors, labels and the :lo12: relocation, bitmask and floating-point immediates, and the property
// the lane stands on: every word the decoder accepts has text that assembles back to it (randomised over the whole encoding space), cross-checked against Capstone.
#include "cb/decompile/a64.hpp"

#include <cctype>
#include <format>
#include <iostream>
#include <map>
#include <random>
#include <string>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace cb::decompile::a64 {
namespace {

std::string dis(std::uint32_t w, std::uint64_t pc = 0, const AddressLabels& labels = {}) {
    const std::optional<Insn> i = decode_word(w, pc);
    return i ? text(*i, labels) : std::string("?");
}

std::uint32_t asm1(const std::string& line, std::uint64_t pc = 0, const NameLabels& labels = {}) { return assemble_word(line, pc, labels); }

struct Known {
    std::uint32_t word;
    const char* text;
};

// Words known from the Arm Architecture Reference Manual and what GNU as and LLVM produce for them.
const Known kKnown[] = {
    {0x91004020, "add x0, x1, #0x10"},          {0xd65f03c0, "ret"},                         {0xa9bf7bfd, "stp x29, x30, [sp, #-0x10]!"}, {0xa8c17bfd, "ldp x29, x30, [sp], #0x10"},
    {0xd2800020, "mov x0, #1"},                 {0x7100141f, "cmp w0, #5"},                  {0xd4000001, "svc #0"},                     {0xd503201f, "nop"},
    {0xf9400528, "ldr x8, [x9, #8]"},           {0xd53bd040, "mrs x0, tpidr_el0"},           {0x1a9f17e0, "cset w0, eq"},                {0xd37df020, "lsl x0, x1, #3"},
    {0xb8627820, "ldr w0, [x1, x2, lsl #2]"},   {0x1e622820, "fadd d0, d1, d2"},             {0x6f00e400, "movi v0.2d, #0x0"},           {0x4c407000, "ld1 {v0.16b}, [x0]"},
    {0xaa0103e0, "mov x0, x1"},                 {0x910003fd, "mov x29, sp"},                 {0x9b037c20, "mul x0, x1, x3"},             {0xd61f0100, "br x8"},
    {0xd63f0100, "blr x8"},                     {0x8b020020, "add x0, x1, x2"},              {0xeb2163ff, "cmp sp, x1"},                 {0x53001c00, "uxtb w0, w0"},
    {0x93407c00, "sxtw x0, w0"},                {0x9ac20c20, "sdiv x0, x1, x2"},             {0xdac01020, "clz x0, x1"},                 {0xc89ffc01, "stlr x1, [x0]"},
    {0xc85f7c01, "ldxr x1, [x0]"},              {0xf2a00020, "movk x0, #1, lsl #16"},      {0x5ac00820, "rev w0, w1"},                 {0xd5033fdf, "isb"},
    {0xd5033bbf, "dmb ish"},                    {0x1e204020, "fmov s0, s1"},                 {0x9e670020, "fmov d0, x1"},                {0x1e6e1000, "fmov d0, #1.00000000"},
};

}  // namespace

CB_TEST("decompile.a64.known_words_both_ways", [] {
    for (const Known& k : kKnown) {
        CB_CHECK_EQ(dis(k.word), std::string(k.text));
        CB_CHECK_EQ(asm1(k.text), k.word);
    }
});

CB_TEST("decompile.a64.branches_and_labels", [] {
    CB_CHECK_EQ(asm1("bl 0x100", 0), 0x94000040u);
    CB_CHECK_EQ(asm1("b.eq 0x8", 0), 0x54000040u);
    CB_CHECK_EQ(asm1("b 0x1000", 0x2000), 0x17fffc00u);
    CB_CHECK_EQ(asm1("cbz x0, 0x20", 0x10), 0xb4000080u);
    CB_CHECK_EQ(asm1("tbnz w3, #5, 0x40", 0), 0x37280203u);
    const AddressLabels names = {{0x100, "sub_0100"}, {0x2000, "D_2000"}};
    CB_CHECK_EQ(dis(0x94000040, 0, names), "bl sub_0100");
    CB_CHECK_EQ(dis(0xb0000000, 0x1000, names), "adrp x0, D_2000");
    const NameLabels values = {{"sub_0100", 0x100}, {"D_2000", 0x2000}, {"lit", 0x40}};
    CB_CHECK_EQ(asm1("bl sub_0100", 0, values), 0x94000040u);
    CB_CHECK_EQ(asm1("adrp x0, D_2000", 0x1000, values), 0xb0000000u);
    CB_CHECK_EQ(asm1("ldr x0, lit", 0, values), 0x58000200u);
    const std::optional<Insn> bl = decode_word(0x94000040, 0x1000);
    CB_CHECK(bl && bl->flow == Flow::Call && bl->target && *bl->target == 0x1100);
    const std::optional<Insn> adrp = decode_word(0xb0000000, 0x1234);
    CB_CHECK(adrp && adrp->flow == Flow::None && adrp->target && *adrp->target == 0x2000);
    CB_CHECK(decode_word(0xd65f03c0, 0)->flow == Flow::Return);
    CB_CHECK(decode_word(0xd61f0100, 0)->flow == Flow::Indirect);
    CB_CHECK(decode_word(0xd4200000, 0)->flow == Flow::Stop);
});

CB_TEST("decompile.a64.lo12_relocation", [] {
    const NameLabels values = {{"data", 0x12A8}};
    CB_CHECK_EQ(asm1("add x0, x0, :lo12:data", 0, values), asm1("add x0, x0, #0x2A8"));
    CB_CHECK_EQ(asm1("ldr x1, [x0, :lo12:data]", 0, values), asm1("ldr x1, [x0, #0x2A8]"));
    CB_CHECK_EQ(asm1("ldrb w1, [x0, :lo12:data]", 0, values), asm1("ldrb w1, [x0, #0x2A8]"));
    const std::optional<Insn> i = decode_word(asm1("add x0, x0, #0x2A8"), 0);
    CB_CHECK_EQ(text(*i, {}, "data"), "add x0, x0, :lo12:data");
    const std::optional<Insn> l = decode_word(asm1("ldr x1, [x0, #0x2A8]"), 0);
    CB_CHECK_EQ(text(*l, {}, "data"), "ldr x1, [x0, :lo12:data]");
});

CB_TEST("decompile.a64.preferred_aliases", [] {
    CB_CHECK_EQ(dis(asm1("orr x0, xzr, x1")), "mov x0, x1");
    CB_CHECK_EQ(dis(asm1("subs xzr, x1, #3")), "cmp x1, #3");
    CB_CHECK_EQ(dis(asm1("ands wzr, w1, #0xff")), "tst w1, #0xFF");
    CB_CHECK_EQ(dis(asm1("csinc w2, wzr, wzr, ne")), "cset w2, eq");
    CB_CHECK_EQ(dis(asm1("csinc w2, w3, w3, ne")), "cinc w2, w3, eq");
    CB_CHECK_EQ(dis(asm1("ubfm x0, x1, #62, #61")), "lsl x0, x1, #2");
    CB_CHECK_EQ(dis(asm1("ubfm x0, x1, #4, #63")), "lsr x0, x1, #4");
    CB_CHECK_EQ(dis(asm1("sbfm w0, w1, #0, #15")), "sxth w0, w1");
    CB_CHECK_EQ(dis(asm1("ubfm w0, w1, #3, #7")), "ubfx w0, w1, #3, #5");
    CB_CHECK_EQ(dis(asm1("bfm w0, w1, #28, #3")), "bfi w0, w1, #4, #4");
    CB_CHECK_EQ(dis(asm1("madd x0, x1, x2, xzr")), "mul x0, x1, x2");
    CB_CHECK_EQ(dis(asm1("smaddl x0, w1, w2, xzr")), "smull x0, w1, w2");
    CB_CHECK_EQ(dis(asm1("sub x0, xzr, x1")), "neg x0, x1");
    CB_CHECK_EQ(dis(asm1("orn x0, xzr, x1")), "mvn x0, x1");
    CB_CHECK_EQ(dis(asm1("extr x0, x1, x1, #7")), "ror x0, x1, #7");
    CB_CHECK_EQ(dis(0xd2800000), "mov x0, #0");
    CB_CHECK_EQ(dis(0x92800000), "mov x0, #-1");
    CB_CHECK_EQ(dis(0xd2a00000), "movz x0, #0, lsl #16");   // (a zero chunk above the first has no mov)
    CB_CHECK_EQ(dis(asm1("orr w1, wzr, #0xaaaaaaaa")), "mov w1, #0xAAAAAAAA");
    // a mov of a value a move-wide makes is that, not the bitmask orr
    CB_CHECK_EQ(asm1("mov w0, #0xff"), asm1("movz w0, #0xff"));
});

CB_TEST("decompile.a64.rejects_what_does_not_fit", [] {
    CB_CHECK_THROWS_AS(ParseError, asm1("add x0, x1, #4097"));
    CB_CHECK_THROWS_AS(ParseError, asm1("ldr x0, [x1, #4]"));          // not a multiple of 8
    CB_CHECK_THROWS_AS(ParseError, asm1("ldr x0, [x1, #32768]"));
    CB_CHECK_THROWS_AS(ParseError, asm1("add x0, w1, w2"));            // mixed widths
    CB_CHECK_THROWS_AS(ParseError, asm1("and x0, x1, #0"));            // not a bitmask immediate
    CB_CHECK_THROWS_AS(ParseError, asm1("b 0x2", 0));                  // not word aligned
    CB_CHECK_THROWS_AS(ParseError, asm1("b 0x10000000", 0));           // out of range
    CB_CHECK_THROWS_AS(ParseError, asm1("mov x0, #0x12345"));          // needs more than one move
    CB_CHECK_THROWS_AS(ParseError, asm1("frobnicate x0"));
    CB_CHECK_THROWS_AS(ParseError, asm1("tbz x0, #3, 0x10"));          // x register needs a bit number of 32 or more
    CB_CHECK_THROWS_AS(ParseError, asm1("stp x0, x1, [x2, #4]"));      // pair offsets are multiples of 8
    CB_CHECK_THROWS_AS(ParseError, asm1("ldr x0, [x1, w2, lsl #3]"));  // lsl takes an x register
    CB_CHECK_EQ(dis(0), "udf #0");
    CB_CHECK(!decode_word(0xffffffff, 0).has_value());
});

CB_TEST("decompile.a64.bitmask_immediates", [] {
    unsigned valid = 0;
    for (unsigned n = 0; n < 2; ++n) {
        for (unsigned imms = 0; imms < 64; ++imms) {
            for (unsigned immr = 0; immr < 64; ++immr) {
                for (const unsigned size : {32u, 64u}) {
                    const std::optional<std::uint64_t> v = decode_bitmask(n, imms, immr, size);
                    if (!v) continue;
                    ++valid;
                    const std::optional<BitmaskFields> f = encode_bitmask(*v, size);
                    CB_CHECK(f.has_value());
                    CB_CHECK_EQ(decode_bitmask(f->n, f->imms, f->immr, size).value_or(0), *v);
                }
            }
        }
    }
    CB_CHECK_EQ(valid, 6636u);   // (every element size, run length and rotation, for the 64-bit and the 32-bit form)
    CB_CHECK(!encode_bitmask(0, 64) && !encode_bitmask(~0ull, 64) && !encode_bitmask(0x5, 32));
    CB_CHECK_EQ(encode_bitmask(0x0000FFFF0000FFFFull, 64)->imms, 0x0Fu);   // (an element of 32 bits: 16 ones)
    CB_CHECK_EQ(expand_fp_imm8(0x70), 1.0);
    CB_CHECK_EQ(expand_fp_imm8(0x00), 2.0);
    CB_CHECK_EQ(expand_fp_imm8(0xF0), -1.0);
    CB_CHECK_EQ(expand_fp_imm8(0x60), 0.5);
    CB_CHECK_EQ(asm1("fmov s0, #0.125"), asm1("fmov s0, #0.12500000"));
    CB_CHECK_THROWS_AS(ParseError, asm1("fmov s0, #0.1"));
});

CB_TEST("decompile.a64.system_registers_and_hints", [] {
    CB_CHECK_EQ(dis(asm1("mrs x3, nzcv")), "mrs x3, nzcv");
    CB_CHECK_EQ(dis(asm1("msr fpcr, x1")), "msr fpcr, x1");
    CB_CHECK_EQ(dis(asm1("mrs x3, s3_3_c14_c0_2")), "mrs x3, cntvct_el0");
    CB_CHECK_EQ(dis(asm1("mrs x3, s3_1_c15_c2_5")), "mrs x3, s3_1_c15_c2_5");
    CB_CHECK_EQ(dis(0xd503233f), "paciasp");
    CB_CHECK_EQ(dis(asm1("hint #0x30")), "hint #0x30");
    CB_CHECK_EQ(dis(asm1("dsb sy")), "dsb sy");
    CB_CHECK_EQ(dis(asm1("dc zva, x3")), "dc zva, x3");
    CB_CHECK_EQ(dis(asm1("msr daifset, #2")), "msr daifset, #2");
    CB_CHECK_EQ(system_register_name(0x5A10), "nzcv");
    CB_CHECK_EQ(system_register_name(0x7FFF), "");
});

CB_TEST("decompile.a64.vector_forms_read_and_write", [] {
    const char* const lines[] = {"add v0.4s, v1.4s, v2.4s",       "fmla v3.2d, v4.2d, v5.2d",      "ld1 {v0.16b, v1.16b}, [x6]",     "st4 {v0.16b, v1.16b, v2.16b, v3.16b}, [x3], #64",
                                 "ld1 {v0.b}[4], [x30]",          "ld1r {v1.16b}, [x7]",           "dup v2.16b, w3",                 "umov w1, v0.h[0]",
                                 "mov v0.h[1], w0",               "movi v1.16b, #1",               "shl v0.8h, v0.8h, #1",           "ushr d4, d1, #0x20",
                                 "uaddw v3.8h, v1.8h, v2.8b",     "smull2 v17.2d, v17.4s, v19.4s", "xtn v3.8b, v3.8h",               "sqdmulh v28.4h, v24.4h, v0.h[3]",
                                 "tbl v0.8b, {v0.16b}, v7.8b",    "ext v3.8b, v0.8b, v2.8b, #6",   "zip1 v1.8b, v0.8b, v2.8b",       "addv h10, v6.8h",
                                 "scvtf s2, s2",                  "fcvtzs v0.4s, v0.4s",           "addp d1, v1.2d",                 "mvni v7.4s, #0",
                                 "ld1 {v0.16b}, [x1], x2",        "cmeq v0.16b, v2.16b, #0",       "mov v0.16b, v1.16b",             "uxtl v3.8h, v0.8b",              "fcvtn v0.2s, v0.2d",             "fcvtl2 v0.2d, v1.4s"};
    for (const char* line : lines) {
        const std::uint32_t w = asm1(line);
        const std::string back = dis(w);
        CB_CHECK_EQ(asm1(back), w);
    }
    CB_CHECK_EQ(dis(asm1("orr v0.16b, v1.16b, v1.16b")), "mov v0.16b, v1.16b");
    CB_CHECK_EQ(dis(asm1("ushll v3.8h, v0.8b, #0")), "uxtl v3.8h, v0.8b");
    CB_CHECK_EQ(dis(asm1("umov w0, v1.s[1]")), "mov w0, v1.s[1]");
});

CB_TEST("decompile.a64.every_decoded_word_assembles_back", [] {
    std::mt19937_64 rng(20261115);
    std::size_t decoded = 0;
    std::map<std::string, std::string> failures;
    for (std::size_t i = 0; i < 4000000; ++i) {
        std::uint32_t w = static_cast<std::uint32_t>(rng());
        if (i % 3 == 0) w = (w & ~(0xFu << 25)) | (static_cast<std::uint32_t>(rng() & 0xF) << 25);   // spread over the major groups
        const std::optional<Insn> insn = decode_word(w, 0x1000);
        if (!insn) continue;
        ++decoded;
        const std::string t = text(*insn);
        std::string why;
        try {
            if (assemble_word(t, 0x1000) == w) continue;
            why = "assembles to another word";
        } catch (const std::exception& e) {
            why = e.what();
        }
        failures.try_emplace(std::string(insn->mnemonic), std::format("{:08x} {} ({})", w, t, why));
    }
    for (const auto& [m, f] : failures) std::cout << "round trip: " << f << "\n";
    CB_CHECK(failures.empty());
    CB_CHECK(decoded > 500000);
});

namespace {

std::string normalise(std::string s) {
    std::string o;
    for (std::size_t i = 0; i < s.size(); ++i) {
        const char c = static_cast<char>(std::tolower(static_cast<unsigned char>(s[i])));
        if (c == ' ' || c == '#') continue;
        if (c == '0' && i + 1 < s.size() && s[i + 1] == 'x') {
            std::size_t j = i + 2;
            unsigned long long v = 0;
            while (j < s.size() && std::isxdigit(static_cast<unsigned char>(s[j]))) {
                const char d = s[j];
                v = v * 16 + (std::isdigit(static_cast<unsigned char>(d)) ? static_cast<unsigned>(d - '0') : static_cast<unsigned>(std::tolower(d) - 'a' + 10));
                ++j;
            }
            o += std::to_string(v);
            i = j - 1;
            continue;
        }
        o += c;
    }
    return o;
}

}  // namespace

CB_TEST("decompile.a64.agrees_with_capstone", [] {
    const disasm::capstone::Api* cs = nullptr;
    try {
        cs = &disasm::capstone::api();
    } catch (const std::exception&) {
        return;   // (no Capstone on this machine)
    }
    csh handle = 0;
    if (cs->cs_open(CS_ARCH_ARM64, CS_MODE_LITTLE_ENDIAN, &handle) != CS_ERR_OK) return;
    std::mt19937_64 rng(777);
    std::size_t decoded = 0, compared = 0, same = 0;
    for (std::size_t i = 0; i < 600000; ++i) {
        const std::uint32_t w = static_cast<std::uint32_t>(rng());
        const std::optional<Insn> insn = decode_word(w, 0x4000);
        if (!insn) continue;
        ++decoded;
        const std::uint8_t b[4] = {static_cast<std::uint8_t>(w), static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w >> 16), static_cast<std::uint8_t>(w >> 24)};
        cs_insn* ci = nullptr;
        const std::size_t n = cs->cs_disasm(handle, b, 4, 0x4000, 1, &ci);
        if (n == 0) continue;
        const std::string theirs = std::string(ci[0].mnemonic) + " " + ci[0].op_str;
        cs->cs_free(ci, n);
        ++compared;
        same += normalise(text(*insn)) == normalise(theirs) ? 1 : 0;
    }
    cs->cs_close(&handle);
    // what differs is spelling: cc / lo, hint and system register names, a negative offset as unsigned, a few aliases
    CB_CHECK(decoded > 100000);
    CB_CHECK(compared * 99 > decoded * 95);        // Capstone knows nearly everything this table decodes
    CB_CHECK(same * 100 > compared * 95);
});

}  // namespace cb::decompile::a64
