// cb-unity: alone
// The AArch64 assembler's directives and labels (the instructions are decompile/a64's).
#include "build/a64_asm.hpp"

#include "build/asm_text.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::build::a64_asm {

namespace {
std::uint32_t word(const Image& i, std::size_t at) {
    return static_cast<std::uint32_t>(i.data[at]) | static_cast<std::uint32_t>(i.data[at + 1]) << 8 | static_cast<std::uint32_t>(i.data[at + 2]) << 16 |
           static_cast<std::uint32_t>(i.data[at + 3]) << 24;
}
}  // namespace

CB_TEST("build.a64_asm.directives_labels_and_forward_references", [] {
    const char* source =
        "// a program\n"
        ".equ DATA, 0x2010\n"
        "        .org 0x1000\n"
        "start:\n"
        "        adrp x0, DATA\n"
        "        add x0, x0, :lo12:DATA\n"
        "        bl sub                // forward\n"
        "        b.ne done\n"
        "        ret\n"
        "sub:   ret\n"
        "done:\n"
        "        .word 0x11223344, done\n"
        "        .quad start + 8\n"
        "        .hword 0xBEEF\n"
        "        .byte 1, 2, \"ab\"\n"
        "        .asciz \"hi\"\n"
        "        .balign 8\n"
        "        .fill 3, 4, 0xAABBCCDD\n"
        "        .space 2, 7\n";
    const Image image = build(source);
    CB_CHECK_EQ(image.base, 0x1000u);
    CB_CHECK_EQ(word(image, 0), 0xB0000000u);                       // adrp x0: DATA's page is one past the page of 0x1000
    CB_CHECK_EQ(word(image, 4), 0x91004000u);                       // add x0, x0, #0x10
    CB_CHECK_EQ(word(image, 8), 0x94000003u);                       // bl +12
    CB_CHECK_EQ(word(image, 12), 0x54000061u);                      // b.ne +12
    CB_CHECK_EQ(word(image, 0x18), 0x11223344u);
    CB_CHECK_EQ(word(image, 0x1C), 0x1018u);                        // done
    CB_CHECK_EQ(word(image, 0x20), 0x1008u);                        // .quad start + 8
    CB_CHECK_EQ(image.data[0x28], 0xEFu);
    CB_CHECK_EQ(image.data[0x2A], 1u);
    CB_CHECK_EQ(image.data[0x2C], static_cast<std::uint8_t>('a'));
    CB_CHECK_EQ(image.data[0x2E], static_cast<std::uint8_t>('h'));
    CB_CHECK_EQ(image.data[0x30], 0u);                              // the zero of .asciz
    CB_CHECK_EQ(word(image, 0x38), 0xAABBCCDDu);                    // (.balign 8 put the fill at 0x38)
    CB_CHECK_EQ(image.data.size(), 0x38u + 12 + 2);
    CB_CHECK_EQ(image.data.back(), 7u);
});

CB_TEST("build.a64_asm.org_zero_padding_and_incbin", [] {
    fs::ScopedTempDir dir("cb_a64_incbin");
    fs::write_file(dir.path() / "blob.bin", Bytes{9, 8, 7});
    const Image image = build("        nop\n        .org 0x10\n        .incbin \"blob.bin\"\n", dir.path());
    CB_CHECK_EQ(image.base, 0u);
    CB_CHECK_EQ(image.data.size(), 0x13u);
    CB_CHECK_EQ(word(image, 0), 0xD503201Fu);
    CB_CHECK_EQ(image.data[4], 0u);
    CB_CHECK_EQ(image.data[0x10], 9u);
    CB_CHECK_THROWS_AS(AsmError, build("        .incbin \"missing.bin\"\n", dir.path()));
});

CB_TEST("build.a64_asm.reports_the_line_of_an_error", [] {
    try {
        build("        nop\n        frob x0\n");
        CB_CHECK(false);
    } catch (const AsmError& e) {
        CB_CHECK_EQ(e.line(), 2u);
    }
    CB_CHECK_THROWS_AS(AsmError, build("        b nowhere\n"));
    CB_CHECK_THROWS_AS(AsmError, build("        .org 0x100\n        nop\n        .org 0x10\n"));
    CB_CHECK_THROWS_AS(AsmError, build("        .bogus 1\n"));
    CB_CHECK_THROWS_AS(AsmError, build("        .balign 3\n"));
});

}  // namespace cb::build::a64_asm
