// cb-unity: alone
#include "formats/vb.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/vb_test_image.hpp"

namespace cb::formats::vb {

namespace ti = test_image;

CB_TEST("formats.vb.the_header_and_the_slots_are_read", [] {
    const Rom rom = parse(ti::cartridge());
    CB_CHECK_EQ(rom.header.title, "TEST CART");
    CB_CHECK_EQ(rom.header.maker, "CB");
    CB_CHECK_EQ(rom.header.game_code, "TEST");
    CB_CHECK_EQ(rom.header.version, 1);
    CB_CHECK_EQ(rom.header_offset(), 0x4000u - 0x220);
    CB_CHECK_EQ(rom.reset_address(), 0x07003FF0u);
    const std::vector<Rom::Entry> entries = rom.entries();
    CB_CHECK_EQ(entries.size(), 2u);   // the VIP's slot and the reset slot hold code; the others are padding
    CB_CHECK_EQ(entries[0].name, "vip");
    CB_CHECK_EQ(entries[0].offset, 0x4000u - 0x1C0);
    CB_CHECK_EQ(entries[1].name, "reset");
    CB_CHECK_EQ(rom.address(0x30), 0x07000030u);
});

CB_TEST("formats.vb.refuses_what_is_not_one", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x5000, 1)));         // not a power of two
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x1000, 1)));         // too small
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x4000, 0xFF)));      // the reset slot is padding
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x4000, 0)));
});

CB_TEST("formats.vb.probe", [] {
    fs::ScopedTempDir dir("cb_vb_probe");
    fs::write_file(dir.path() / "a.vb", ti::cartridge());
    fs::write_file(dir.path() / "b.vboy", ti::cartridge());
    fs::write_file(dir.path() / "c.vb", Bytes(0x4000, 0xFF));
    fs::write_file(dir.path() / "d.bin", ti::cartridge());
    fs::write_file(dir.path() / "e.vb", Bytes(0x4001, 0x20));
    CB_CHECK(probe(dir.path() / "a.vb"));
    CB_CHECK(probe(dir.path() / "b.vboy"));
    CB_CHECK(!probe(dir.path() / "c.vb"));
    CB_CHECK(!probe(dir.path() / "d.bin"));
    CB_CHECK(!probe(dir.path() / "e.vb"));
    CB_CHECK(!probe(dir.path() / "missing.vb"));
});

}  // namespace cb::formats::vb
