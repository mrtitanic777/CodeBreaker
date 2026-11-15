// cb-unity: alone
#include "cb/formats/vectrex.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/vectrex_test_image.hpp"

namespace cb::formats::vectrex {

namespace ti = test_image;

CB_TEST("formats.vectrex.the_header_is_read_field_by_field", [] {
    const Rom rom = parse(ti::cartridge());
    CB_CHECK_EQ(rom.copyright, "g GCE 1982");
    CB_CHECK_EQ(rom.music, 0xFD0D);
    CB_CHECK_EQ(rom.title.size(), 1u);
    CB_CHECK_EQ(rom.title[0].text, "TEST");
    CB_CHECK_EQ(rom.title[0].height, -8);
    CB_CHECK_EQ(rom.title[0].width, 0x50);
    CB_CHECK_EQ(rom.title[0].x, -48);
    CB_CHECK_EQ(rom.entry, 0x17u);
    CB_CHECK_EQ(rom.memory()[0x17], 0xBD);
    CB_CHECK_EQ(rom.memory()[0xE000], 0);
});

CB_TEST("formats.vectrex.refuses_what_is_not_one", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(100, 0xFF)));          // no header
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x9000, 0)));          // too big
    Bytes cut = ti::cartridge(0x10);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(cut));                       // the header does not end
});

CB_TEST("formats.vectrex.probe", [] {
    fs::ScopedTempDir dir("cb_vec_probe");
    fs::write_file(dir.path() / "a.vec", ti::cartridge());
    fs::write_file(dir.path() / "b.bin", ti::cartridge());
    fs::write_file(dir.path() / "c.bin", Bytes(0x40, 0xFF));
    fs::write_file(dir.path() / "d.txt", ti::cartridge());
    CB_CHECK(probe(dir.path() / "a.vec"));
    CB_CHECK(probe(dir.path() / "b.bin"));
    CB_CHECK(!probe(dir.path() / "c.bin"));
    CB_CHECK(!probe(dir.path() / "d.txt"));
    CB_CHECK(!probe(dir.path() / "missing.vec"));
});

}  // namespace cb::formats::vectrex
