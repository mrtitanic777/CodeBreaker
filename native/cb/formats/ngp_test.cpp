// cb-unity: alone
#include "cb/formats/ngp.hpp"

#include <algorithm>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::ngp {
namespace {

// A 4 KB ROM: the licence text, start $200040, catalogue $0042, colour, the title "TEST CART", then 0xFF.
Bytes cartridge() {
    Bytes rom(0x1000, 0xFF);
    const std::string licence = " LICENSED BY SNK CORPORATION";
    std::copy(licence.begin(), licence.end(), rom.begin());
    const std::uint8_t fields[] = {0x40, 0x00, 0x20, 0x00, 0x42, 0x00, 0x03, 0x10};
    std::copy(std::begin(fields), std::end(fields), rom.begin() + 0x1C);
    const std::string title = "TEST CART   ";
    std::copy(title.begin(), title.end(), rom.begin() + 0x24);
    return rom;
}

}  // namespace

CB_TEST("formats.ngp.the_header_is_read_field_by_field", [] {
    const Rom rom = parse(cartridge());
    CB_CHECK(rom.header.licensed);
    CB_CHECK_EQ(rom.header.start, 0x200040u);
    CB_CHECK_EQ(rom.header.catalog, 0x42u);
    CB_CHECK_EQ(rom.header.sub_catalog, 3);
    CB_CHECK(rom.header.colour);
    CB_CHECK_EQ(rom.header.title, "TEST CART");
    CB_CHECK_EQ(rom.address(0x40), 0x200040u);
});

CB_TEST("formats.ngp.refuses_what_is_not_one", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(100, 0xFF)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(10, 0)));
    Bytes far = cartridge();
    far[0x1F] = 0x40;   // a start address outside the ROM
    CB_CHECK_THROWS_AS(FormatError, (void)parse(far));
});

CB_TEST("formats.ngp.probe_and_names", [] {
    fs::ScopedTempDir dir("cb_ngp_probe");
    fs::write_file(dir.path() / "a.ngp", cartridge());
    fs::write_file(dir.path() / "b.ngc", cartridge());
    fs::write_file(dir.path() / "c.bin", cartridge());
    fs::write_file(dir.path() / "d.ngp", Bytes(0x1000, 0xFF));
    CB_CHECK(probe(dir.path() / "a.ngp"));
    CB_CHECK(probe(dir.path() / "b.ngc"));
    CB_CHECK(!probe(dir.path() / "c.bin"));
    CB_CHECK(!probe(dir.path() / "d.ngp"));
    CB_CHECK_EQ(hardware().at(0x8000), "K1GE_CONTROL");
    CB_CHECK_EQ(hardware().at(0x6F82), "BIOS_JOYPAD");
});

}  // namespace cb::formats::ngp
