// cb-unity: alone
#include "formats/atari5200.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::formats::atari5200 {
namespace {

// A ROM of `size` bytes of NOPs with the header at its end: flags 00 FF, start address `start`.
Bytes image(std::size_t size, std::uint16_t start) {
    Bytes rom(size, 0xEA);
    rom[size - 4] = 0x00;
    rom[size - 3] = 0xFF;
    rom[size - 2] = static_cast<std::uint8_t>(start & 0xFF);
    rom[size - 1] = static_cast<std::uint8_t>(start >> 8);
    return rom;
}

}  // namespace

CB_TEST("formats.atari5200.the_rom_ends_at_the_top_of_the_cartridge_window", [] {
    CB_CHECK_EQ(parse(image(0x8000, 0x4000)).base, 0x4000u);
    CB_CHECK_EQ(parse(image(0x4000, 0x8000)).base, 0x8000u);
    CB_CHECK_EQ(parse(image(0x2000, 0xA000)).base, 0xA000u);
    const Rom rom = parse(image(0x1000, 0xB000));
    CB_CHECK_EQ(rom.base, 0xB000u);
    CB_CHECK_EQ(rom.flag_b, 0xFF);
    CB_CHECK_EQ(rom.start, 0xB000);
    CB_CHECK_EQ(rom.memory()[0xB000], 0xEA);
    CB_CHECK_EQ(rom.memory()[0xAFFF], 0);
    CB_CHECK(rom.offset(0xBFFF).value() == 0xFFFu);
    CB_CHECK(!rom.offset(0xAFFF).has_value());
});

CB_TEST("formats.atari5200.refuses_what_is_not_one", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(3000, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(image(0x4000, 0xF000)));   // start outside the ROM
});

CB_TEST("formats.atari5200.probe_by_extension_and_start_address", [] {
    fs::ScopedTempDir dir("cb_a52_probe");
    fs::write_file(dir.path() / "a.a52", image(0x4000, 0x8000));
    fs::write_file(dir.path() / "b.bin", image(0x4000, 0x8000));
    fs::write_file(dir.path() / "c.bin", image(0x4000, 0xF000));   // an Atari 2600-style vector
    fs::write_file(dir.path() / "d.bin", Bytes(0x4000, 0xFF));
    fs::write_file(dir.path() / "e.txt", image(0x4000, 0x8000));
    CB_CHECK(probe(dir.path() / "a.a52"));
    CB_CHECK(probe(dir.path() / "b.bin"));
    CB_CHECK(!probe(dir.path() / "c.bin"));
    CB_CHECK(!probe(dir.path() / "d.bin"));
    CB_CHECK(!probe(dir.path() / "e.txt"));
    CB_CHECK(!probe(dir.path() / "missing.a52"));
});

}  // namespace cb::formats::atari5200
