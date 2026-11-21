// cb-unity: alone
// The Jaguar writer over small programs written byte by byte: the source it writes, built again by CodeBreaker's assembler, is the program.

#include "source/jaguar_writer.hpp"

#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/isa.hpp"
#include "source/risc_source.hpp"

namespace cb::source::jaguar_writer {
namespace {

void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

// start: MOVE.W #$1234,BG ; JSR sub ; BRA start.   sub: MOVE.L VC,D0 ; RTS.   (the code as bytes, with `sub` at `base + $20`)
Bytes code(std::uint32_t base) {
    Bytes c(0x28, 0xFF);
    const Bytes start = {0x33, 0xFC, 0x12, 0x34, 0x00, 0xF0, 0x00, 0x58, 0x4E, 0xB9, 0, 0, 0, 0, 0x60, 0xF0};
    std::copy(start.begin(), start.end(), c.begin());
    put32(c, 10, base + 0x20);
    const Bytes sub = {0x20, 0x39, 0x00, 0xF0, 0x00, 0x06, 0x4E, 0x75};
    std::copy(sub.begin(), sub.end(), c.begin() + 0x20);
    return c;
}

Bytes cartridge() {
    Bytes rom(0x4000, 0xFF);
    for (std::size_t i = 0; i < 0x400; ++i) rom[i] = static_cast<std::uint8_t>(i * 7);   // the boot signature
    put32(rom, 0x400, 0x04040404);
    put32(rom, 0x404, 0x802000);
    const Bytes c = code(0x802000);
    std::copy(c.begin(), c.end(), rom.begin() + 0x2000);
    return rom;
}

Bytes coff() {
    const Bytes text = code(0x4000);
    Bytes f(88, 0);
    f[0] = 0x01;
    f[1] = 0x50;
    f[3] = 1;
    f[17] = 28;
    put32(f, 20 + 4, static_cast<std::uint32_t>(text.size()));
    put32(f, 20 + 16, 0x4000);
    put32(f, 20 + 20, 0x4000);
    const char name[] = ".text";
    std::copy(name, name + 5, f.begin() + 48);
    put32(f, 48 + 8, 0x4000);
    put32(f, 48 + 12, 0x4000);
    put32(f, 48 + 16, static_cast<std::uint32_t>(text.size()));
    put32(f, 48 + 20, 88);
    put32(f, 48 + 36, 0x20);
    f.insert(f.end(), text.begin(), text.end());
    return f;
}

}  // namespace

CB_TEST("source.jaguar_writer.a_cartridge_comes_back_as_source_that_builds_the_same_file", [] {
    fs::ScopedTempDir dir("cb_jag_cart");
    const image_writer::Written r = write(formats::jaguar::parse(cartridge()), "demo.j64", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x802000, "start"}, {0x802020, "sub_802020"}}));
    CB_CHECK_EQ(r.image_bytes, 0x4000u);
    const std::string s = fs::read_text(r.source);
    for (const char* want : {"BG", "VC", ".equ BG, 0xf00058", ".org 0x800000", ".long start", "jsr sub_802020.l"}) {
        CB_CHECK(s.find(want) != std::string::npos);
    }
});

CB_TEST("source.jaguar_writer.a_coff_comes_back_as_source_that_builds_the_same_sections", [] {
    fs::ScopedTempDir dir("cb_jag_coff");
    const image_writer::Written r = write(formats::jaguar::parse(coff()), "demo.cof", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK((r.functions == image_writer::Functions{{0x4000, "start"}, {0x4020, "sub_004020"}}));
    CB_CHECK(fs::read_text(r.source).find(".org 0x004000") != std::string::npos);
});

CB_TEST("source.jaguar_writer.an_edited_source_builds_a_different_file", [] {
    fs::ScopedTempDir dir("cb_jag_edit");
    const image_writer::Written r = write(formats::jaguar::parse(coff()), "demo.cof", dir.path());
    std::string s = fs::read_text(r.source);
    const std::size_t at = s.find("#0x1234");
    CB_CHECK(at != std::string::npos);
    s.replace(at, 7, "#0x1235");
    const Bytes image = risc_source::assemble("m68k", s, dir.path());
    CB_CHECK_EQ(image[3], 0x35);   // (the image holds the sections' bytes one after another: the first section's fourth byte)
});

}  // namespace cb::source::jaguar_writer
