// cb-unity: alone
#include "formats/wonderswan.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::formats::wonderswan {
namespace {

// A ROM of `size` bytes of $FF with the footer: JMPF to `segment:offset`, colour flag, game id 1, and the checksum of the bytes before it.
Bytes image(std::size_t size, std::uint16_t segment, std::uint16_t offset, std::uint8_t color = 0) {
    Bytes rom(size, 0xFF);
    const std::size_t f = size - kFooter;
    const Bytes footer = {0xEA, static_cast<std::uint8_t>(offset & 0xFF), static_cast<std::uint8_t>(offset >> 8), static_cast<std::uint8_t>(segment & 0xFF),
                          static_cast<std::uint8_t>(segment >> 8), 0, 0x42, color, 1, 2, 3, 4, 1, 0, 0, 0};
    std::copy(footer.begin(), footer.end(), rom.begin() + static_cast<std::ptrdiff_t>(f));
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i + 2 < size; ++i) sum += rom[i];
    rom[size - 2] = static_cast<std::uint8_t>(sum & 0xFF);
    rom[size - 1] = static_cast<std::uint8_t>((sum >> 8) & 0xFF);
    return rom;
}

}  // namespace

CB_TEST("formats.wonderswan.the_footer_names_the_start_and_the_rom_ends_at_fffff", [] {
    const Rom rom = parse(image(0x10000, 0xF000, 0x0010));
    CB_CHECK_EQ(rom.reset_segment, 0xF000);
    CB_CHECK_EQ(rom.reset_offset, 0x10);
    CB_CHECK_EQ(rom.entry(), 0xF0010u);
    CB_CHECK_EQ(rom.code_base(), 0xF0000u);
    CB_CHECK_EQ(rom.base(), 0xF0000u);
    CB_CHECK_EQ(rom.developer, 0x42);
    CB_CHECK_EQ(rom.game_id, 1);
    CB_CHECK(rom.vertical());
    CB_CHECK(!rom.is_color());
    CB_CHECK(rom.checksum_ok);
    CB_CHECK_EQ(rom.offset(0xF0010).value(), 0x10u);
    CB_CHECK_EQ(rom.offset(0xFFFFF).value(), 0xFFFFu);
    CB_CHECK(!rom.offset(0xEFFFF).has_value());
    CB_CHECK_EQ(rom.memory()[0xFFFF0], 0xEA);
});

CB_TEST("formats.wonderswan.a_large_rom_shows_its_last_896_kb", [] {
    const Rom rom = parse(image(0x200000, 0x2000, 0, 1));
    CB_CHECK(rom.is_color());
    CB_CHECK_EQ(rom.mapped(), 0xE0000u);
    CB_CHECK_EQ(rom.base(), 0x20000u);
    CB_CHECK_EQ(rom.offset(0x20000).value(), 0x200000u - 0xE0000u);
    CB_CHECK_EQ(rom.entry(), 0x20000u);
});

// The cartridge image a .com program becomes under tslanina's com2ws (Wondersnake, GPL-3): 512 KB, the code at $100, the footer written byte by byte as that
// tool does (rom size code 1, flags 4, the colour flag, a far jump to 8000:0100, the checksum last).
CB_TEST("formats.wonderswan.a_com2ws_image_reads_as_the_tool_wrote_it", [] {
    constexpr std::size_t kSize = 512 * 1024;
    Bytes rom(kSize, 0);
    rom[0x100] = 0x90;   // (the program: a nop and a return)
    rom[0x101] = 0xC3;
    rom[kSize - 4] = 4;
    rom[kSize - 6] = 1;
    rom[kSize - 16] = 0xEA;
    rom[kSize - 15] = 0x00;
    rom[kSize - 14] = 0x01;
    rom[kSize - 12] = 0x80;
    rom[kSize - 9] = 1;
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i < kSize - 2; ++i) sum += rom[i];
    rom[kSize - 1] = static_cast<std::uint8_t>((sum & 0xFFFF) >> 8);
    rom[kSize - 2] = static_cast<std::uint8_t>(sum & 0xFF);
    const Rom r = parse(rom);
    CB_CHECK_EQ(r.reset_segment, 0x8000);
    CB_CHECK_EQ(r.reset_offset, 0x0100);
    CB_CHECK_EQ(r.entry(), 0x80100u);
    CB_CHECK_EQ(r.base(), 0x80000u);
    CB_CHECK(r.is_color());
    CB_CHECK(!r.vertical());
    CB_CHECK_EQ(r.size_code, 1);
    CB_CHECK_EQ(r.flags, 4);
    CB_CHECK(r.checksum_ok);
    CB_CHECK_EQ(r.offset(r.entry()).value(), 0x100u);
});

CB_TEST("formats.wonderswan.refuses_what_is_not_one", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x12345, 0xFF)));      // not a power of two
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x10000, 0xFF)));      // no far jump in the footer
    CB_CHECK_THROWS_AS(FormatError, (void)parse(image(0x10000, 0x1000, 0))); // starts below the ROM
});

CB_TEST("formats.wonderswan.probe", [] {
    fs::ScopedTempDir dir("cb_ws_probe");
    fs::write_file(dir.path() / "a.ws", image(0x10000, 0xF000, 0));
    fs::write_file(dir.path() / "b.wsc", image(0x20000, 0xE000, 0, 1));
    fs::write_file(dir.path() / "c.ws", Bytes(0x10000, 0));
    fs::write_file(dir.path() / "d.bin", image(0x10000, 0xF000, 0));
    CB_CHECK(probe(dir.path() / "a.ws"));
    CB_CHECK(probe(dir.path() / "b.wsc"));
    CB_CHECK(!probe(dir.path() / "c.ws"));
    CB_CHECK(!probe(dir.path() / "d.bin"));
    CB_CHECK(!probe(dir.path() / "missing.ws"));
});

}  // namespace cb::formats::wonderswan
