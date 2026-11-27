// cb-unity: alone (file-local names that other files in a batch also use)
// PowerPC source for patching a title's code, built through the one assembler: gas-ppc's dialect over the directive layer over the
// PowerPC description (what build/ppc_asm's own tests said, word for word).
#include <format>

#include "core/error.hpp"
#include "core/testing.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

namespace cb::source {
namespace {

Bytes assemble(std::string_view text, std::uint32_t base = 0, const decompile::NameLabels& labels = {}) {
    std::string source = base != 0 ? std::format(".org {:#x}\n", base) : std::string();
    for (const auto& [name, value] : labels) source += std::format(".equ {}, {:#x}\n", name, value);
    return risc_source::assemble("ppc", dialect::find("gas-ppc").parse(source + std::string(text)), {});
}

std::uint32_t word_at(const Bytes& b, std::size_t i) {
    return (std::uint32_t{b[i]} << 24) | (std::uint32_t{b[i + 1]} << 16) | (std::uint32_t{b[i + 2]} << 8) | b[i + 3];
}

std::uint32_t one(std::string_view line, std::uint32_t address = 0, const decompile::NameLabels& labels = {}) {
    try {
        const Bytes b = assemble(std::string(line) + "\n", address, labels);
        return b.size() == 4 ? word_at(b, 0) : 0xDEADBEEF;
    } catch (const Error&) {
        return 0xBAD00000;
    }
}

}  // namespace

CB_TEST("source.ppc_source.encodings", [] {
    CB_CHECK_EQ(one("nop"), 0x60000000u);
    CB_CHECK_EQ(one("blr"), 0x4E800020u);
    CB_CHECK_EQ(one("bctr"), 0x4E800420u);
    CB_CHECK_EQ(one("li r3, 1"), 0x38600001u);
    CB_CHECK_EQ(one("lis r4, 0x8200"), 0x3C808200u);
    CB_CHECK_EQ(one("mr r31, r3"), 0x7C7F1B78u);
    CB_CHECK_EQ(one("mflr r12"), 0x7D8802A6u);
    CB_CHECK_EQ(one("mtlr r0"), 0x7C0803A6u);
    CB_CHECK_EQ(one("mtctr r11"), 0x7D6903A6u);
    CB_CHECK_EQ(one("mtspr ctr, r11"), 0x7D6903A6u);
    CB_CHECK_EQ(one("mfspr r12, lr"), 0x7D8802A6u);
    CB_CHECK_EQ(one("addi r1, r1, -0x60"), 0x3821FFA0u);
    CB_CHECK_EQ(one("subi r1, r1, 0x60"), 0x3821FFA0u);
    CB_CHECK_EQ(one("ori r3, r3, 0x1234"), 0x60631234u);
    CB_CHECK_EQ(one("lwz r3, 0x10(r31)"), 0x807F0010u);
    CB_CHECK_EQ(one("stw r0, -8(r1)"), 0x9001FFF8u);
    CB_CHECK_EQ(one("stwu r1, -0x60(r1)"), 0x9421FFA0u);
    CB_CHECK_EQ(one("lbz r5, 0(r4)"), 0x88A40000u);
    CB_CHECK_EQ(one("add r3, r4, r5"), 0x7C642A14u);
    CB_CHECK_EQ(one("cmpwi r3, 0"), 0x2C030000u);
    CB_CHECK_EQ(one("cmplw r3, r4"), 0x7C032040u);
    CB_CHECK_EQ(one("neg r3, r4"), 0x7C6400D0u);
    CB_CHECK_EQ(one("extsh r3, r4"), 0x7C830734u);
    CB_CHECK_EQ(one("LI R3,1"), 0x38600001u);
    CB_CHECK_EQ(one("trap"), 0x7FE00008u);
});

CB_TEST("source.ppc_source.destination_first_forms", [] {
    CB_CHECK_EQ(one("or r3, r4, r5"), 0x7C832B78u);
    CB_CHECK_EQ(one("ori r3, r4, 5"), 0x60830005u);
    CB_CHECK_EQ(one("and. r3, r3, r4"), 0x7C632039u);
    CB_CHECK_EQ(one("not r3, r4"), 0x7C8320F8u);
    CB_CHECK_EQ(one("sub r3, r4, r5"), 0x7C652050u);
    CB_CHECK_EQ(one("subf r3, r4, r5"), 0x7C642850u);
});

CB_TEST("source.ppc_source.branches", [] {
    CB_CHECK_EQ(one("b 0x82001000", 0x82000000), 0x48001000u);
    CB_CHECK_EQ(one("bl 0x82000000", 0x82000004), 0x4BFFFFFDu);
    CB_CHECK_EQ(one("beq 0x82000010", 0x82000008), 0x41820008u);
    CB_CHECK_EQ(one("bne 0x81FFFFF0", 0x8200000C), 0x4082FFE4u);
    CB_CHECK_EQ(one("bdnz 0x20", 0x30), 0x4200FFF0u);
    CB_CHECK_EQ(one("bc 12, 2, 0x10", 0), 0x41820010u);
    CB_CHECK_EQ(one("ba 0x100"), 0x48000102u);
    const decompile::NameLabels labels{{"top", 0x82000000}};
    CB_CHECK_EQ(one("b top", 0x82000004, labels), 0x4BFFFFFCu);
    CB_CHECK_THROWS_AS(Error, (void)assemble("b 0x10000000\n"));
    CB_CHECK_THROWS_AS(Error, (void)assemble("beq 0x9000\n"));
    CB_CHECK_THROWS_AS(Error, (void)assemble("ba 0x82000000\n"));
    CB_CHECK_THROWS_AS(Error, (void)assemble("b 0x2\n"));
    CB_CHECK_THROWS_AS(Error, (void)assemble("b nowhere\n"));
});

CB_TEST("source.ppc_source.multi_line_with_labels", [] {
    const Bytes code = assemble(
        "start:\n"
        "    li r3, 1        # comment\n"
        "    b done          # a forward label\n"
        "    nop\n"
        "done:\n"
        "    blr\n"
        "    b start\n",
        0x82000000);
    CB_CHECK_EQ(code.size(), 20u);
    CB_CHECK_EQ(word_at(code, 0), 0x38600001u);
    CB_CHECK_EQ(word_at(code, 4), 0x48000008u);
    CB_CHECK_EQ(word_at(code, 8), 0x60000000u);
    CB_CHECK_EQ(word_at(code, 12), 0x4E800020u);
    CB_CHECK_EQ(word_at(code, 16), 0x4BFFFFF0u);
    CB_CHECK(assemble("").empty());
    CB_CHECK(assemble("# nothing\n\n# here\n").empty());
});

CB_TEST("source.ppc_source.errors", [] {
    auto line_of = [](std::string_view text) {
        try {
            (void)assemble(text);
        } catch (const ParseError& e) {
            return e.line();
        } catch (const Error&) {
            return 99u;
        }
        return 0u;
    };
    CB_CHECK_EQ(line_of("nop\nfly r3\n"), 2u);
    CB_CHECK_EQ(line_of("nop\n,,\n"), 2u);
    CB_CHECK_EQ(line_of("nop\nnop\nli r3\n"), 3u);
    CB_CHECK_EQ(line_of("li r32, 1\n"), 1u);
    CB_CHECK_EQ(line_of("li rx, 1\n"), 1u);
    CB_CHECK_EQ(line_of("li r3, 0x10000\n"), 1u);
    CB_CHECK_EQ(line_of("li r3, banana\n"), 1u);
    CB_CHECK_EQ(line_of("ori r3, r3, -1\n"), 1u);
    CB_CHECK_EQ(line_of("ori r3, r3, 0x12345\n"), 1u);
    CB_CHECK_EQ(line_of("lwz r3, r4\n"), 1u);
    CB_CHECK_EQ(line_of("lwz r3, 8(r40)\n"), 1u);
    CB_CHECK_EQ(line_of("lwz r3, 0x10000(r1)\n"), 1u);
    CB_CHECK_EQ(line_of("lwz r3, ff(r1)\n"), 1u);
    CB_CHECK_EQ(line_of("mtspr 1024, r3\n"), 1u);
});

}  // namespace cb::source
