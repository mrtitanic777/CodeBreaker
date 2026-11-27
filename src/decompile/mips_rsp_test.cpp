// Tests of the RSP's instructions (decompile/mips_rsp, Variant::Rsp): a few by hand against their encodings, then every word of a sweep over
// the vector unit, the vector loads and stores and the scalar instructions read back to itself, and what the RSP does not have is not read.
#include "decompile/mips_rsp.hpp"

#include <filesystem>
#include <format>

#include "core/fs.hpp"

#include "core/testing.hpp"
#include "decompile/mips.hpp"

namespace cb::decompile::mips {
namespace {

Bytes be(std::uint32_t w) { return {static_cast<std::uint8_t>(w >> 24), static_cast<std::uint8_t>(w >> 16), static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w)}; }

std::string text_of(std::uint32_t word) {
    const std::optional<Insn> i = decode_word(word, 0x04001000, Variant::Rsp);
    return i ? text(*i) : std::string("(none)");
}

// The word's text assembles back to the word.
bool round_trips(std::uint32_t word) {
    const std::optional<Insn> i = decode_word(word, 0x04001000, Variant::Rsp);
    if (!i) return true;
    return assemble(text(*i), 0x04001000, {}, Variant::Rsp) == be(word);
}

}  // namespace

CB_TEST("decompile.mips.rsp.instructions_by_hand", [] {
    CB_CHECK_EQ(text_of(0x4A431040), "vmulf $v1, $v2, $v3[0q]");
    CB_CHECK_EQ(text_of(0x4B04104F), "vmadh $v1, $v2, $v4[0]");     // e 8: lane 0
    CB_CHECK_EQ(text_of(0x4BA31070), "vrcp $v1[2], $v3[5]");
    CB_CHECK_EQ(text_of(0x4A000037), "vnop");
    CB_CHECK_EQ(text_of(0x48081200), "mfc2 $t0, $v2[4]");
    CB_CHECK_EQ(text_of(0x48480800), "cfc2 $t0, $vcc");
    CB_CHECK_EQ(text_of(0xC8812001), "lqv $v1[0], 16($a0)");   // the offset field 1, times 16
    CB_CHECK_EQ(text_of(0xEBA21C7F), "sdv $v2[8], -8($sp)");
    CB_CHECK_EQ(text_of(0x0000000D), "break");
    CB_CHECK_EQ(text_of(0x40042000), "mfc0 $a0, $4");               // SP_STATUS
    // assembled back
    CB_CHECK(assemble("vmulf $v1, $v2, $v3[0q]", 0, {}, Variant::Rsp) == be(0x4A431040));
    CB_CHECK(assemble("lqv $v1[0], 16($a0)", 0, {}, Variant::Rsp) == be(0xC8812001));
    CB_CHECK(assemble("sdv $v2[8], -8($sp)", 0, {}, Variant::Rsp) == be(0xEBA21C7F));
    CB_CHECK_THROWS_AS(ParseError, assemble("lqv $v1[0], 8($a0)", 0, {}, Variant::Rsp));    // not a multiple of 16
    CB_CHECK_THROWS_AS(ParseError, assemble("lqv $v1[0], 1024($a0)", 0, {}, Variant::Rsp));  // out of reach
    CB_CHECK_THROWS_AS(ParseError, assemble("vmulf $v1, $v2, $v3[9]", 0, {}, Variant::Rsp));
});

CB_TEST("decompile.mips.rsp.what_the_rsp_does_not_have_is_not_read", [] {
    for (const std::uint32_t word : {0x00850018u /* mult */, 0x0085001Au /* div */, 0x88850000u /* lwl */, 0x0000000Cu /* syscall */,
                                     0x46020800u /* add.s */, 0x50850001u /* beql */, 0xDC850000u /* ld */, 0x4A000038u /* COP2 function 0x38 */,
                                     0xC8816001u /* LWC2 opcode 12 */, 0x48081201u /* mfc2 with low bits */})
        CB_CHECK(!decode_word(word, 0x04001000, Variant::Rsp).has_value());
    // and the R4300i does not read the RSP's own
    CB_CHECK(!decode_word(0x4A431040, 0, Variant::R4300).has_value() || decode_word(0x4A431040, 0, Variant::R4300)->mnemonic != "vmulf");
});

CB_TEST("decompile.mips.rsp.every_word_read_is_written_back_as_itself", [] {
    std::size_t read = 0, bad = 0;
    std::string first_bad;
    const auto check = [&](std::uint32_t w) {
        if (!decode_word(w, 0x04001000, Variant::Rsp)) return;
        ++read;
        if (!round_trips(w)) {
            if (bad++ == 0) first_bad = std::format("{:08X} {}", w, text_of(w));
        }
    };
    // the vector unit: every function, every element, a spread of registers
    for (std::uint32_t function = 0; function < 64; ++function)
        for (std::uint32_t e = 0; e < 16; ++e)
            for (std::uint32_t r = 0; r < 32; r += 7)
                check(0x4A000000u | (e << 21) | (r << 16) | (((r * 3) & 31) << 11) | (((r * 5) & 31) << 6) | function);
    // the moves
    for (std::uint32_t fmt : {0u, 2u, 4u, 6u})
        for (std::uint32_t r = 0; r < 32; r += 3)
            for (std::uint32_t e = 0; e < 16; ++e) check(0x48000000u | (fmt << 21) | (r << 16) | ((r % 3) << 11) | (e << 7));
    // the loads and stores: every opcode, element and offset
    for (std::uint32_t opcode : {0x32u, 0x3Au})
        for (std::uint32_t op = 0; op < 32; ++op)
            for (std::uint32_t e = 0; e < 16; ++e)
                for (std::uint32_t off = 0; off < 128; off += 9) check((opcode << 26) | (29u << 21) | (5u << 16) | (op << 11) | (e << 7) | off);
    // the scalar instructions, a sweep over the whole space
    for (std::uint64_t w = 0; w < (1ull << 32); w += 104729) check(static_cast<std::uint32_t>(w));
    CB_CHECK(read > 20000);
    CB_CHECK_EQ(bad, std::size_t{0});
    CB_CHECK_EQ(first_bad, std::string());
});

// Real microcode: the RSP programs inside PeterLemon's RSP test ROMs (MIT; GitHub/homebrew-samples/n64, or where CB_HOMEBREW_SAMPLES says). The
// stretch of each ROM that holds vector instructions is read as RSP code: nearly every word is an instruction, and each reads back as itself.
CB_TEST("decompile.mips.rsp.peterlemon_microcode_reads_back", [] {
    std::filesystem::path root = testing::beside_checkout("homebrew-samples");
    if (const auto env = fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = fs::from_text(*env);
    const std::filesystem::path dir = root / "n64" / "peterlemon";
    std::size_t roms = 0;
    for (const char* name : {"RSP_DCT_FastDCTBlockDecode_RSPFastDCTBlockDecode.N64", "RSP_DCT_FastQuantizationBlockDecode_RSPFastQuantizationBlockDecode.N64",
                             "RSP_DCT_FastQuantizationMultiBlock16BIT_RSPFastQuantizationMultiBlock16BIT.N64"}) {
        if (!std::filesystem::exists(dir / name)) continue;
        ++roms;
        const Bytes rom = fs::read_file(dir / name);
        // the vector instructions' stretch: from the first COP2 / LWC2 / SWC2 word past the boot code to the last
        std::size_t first = 0, last = 0;
        for (std::size_t at = 0x1000; at + 4 <= rom.size(); at += 4) {
            const unsigned top = rom[at] >> 2;
            if ((rom[at] >> 1) == 0x25 || top == 0x32 || top == 0x3A) {
                if (first == 0) first = at;
                last = at;
            }
        }
        CB_CHECK(first != 0);
        std::size_t words = 0, read = 0, back = 0;
        for (std::size_t at = first; at <= last; at += 4) {
            ++words;
            const std::optional<Insn> i = decode(rom, at, 0x04001000 + static_cast<std::uint32_t>(at - first), Variant::Rsp);
            if (!i) continue;
            ++read;
            if (assemble(text(*i), i->pc, {}, Variant::Rsp) == Bytes(rom.begin() + static_cast<std::ptrdiff_t>(at), rom.begin() + static_cast<std::ptrdiff_t>(at) + 4)) ++back;
        }
        CB_CHECK(words > 100);
        CB_CHECK(read * 100 >= words * 95);
        CB_CHECK_EQ(back, read);
    }
    (void)roms;   // (none here: nothing to read)
});

}  // namespace cb::decompile::mips
