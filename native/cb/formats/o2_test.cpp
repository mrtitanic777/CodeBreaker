// cb-unity: alone
#include "cb/formats/o2.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/o2_test_image.hpp"

namespace cb::formats::o2 {

CB_TEST("formats.o2.a_cartridge_is_its_program_memory_from_0400", [] {
    const Rom rom = parse(test_image::cartridge());
    CB_CHECK_EQ(rom.data.size(), 0x400u);
    CB_CHECK_EQ(rom.address(0x20), 0x420u);
    CB_CHECK_EQ(rom.data[0], 0xB8);
    CB_CHECK_EQ(vdc().at(0xA1), "VDC_STATUS");
});

CB_TEST("formats.o2.refuses_what_is_not_one", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes()));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(300, 0)));       // not whole pages
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x1000, 0)));    // a banked cartridge
});

CB_TEST("formats.o2.probe", [] {
    fs::ScopedTempDir dir("cb_o2_probe");
    fs::write_file(dir.path() / "a.o2", test_image::cartridge());
    fs::write_file(dir.path() / "b.bin", test_image::cartridge());
    fs::write_file(dir.path() / "c.o2", Bytes(300, 0));
    CB_CHECK(probe(dir.path() / "a.o2"));
    CB_CHECK(!probe(dir.path() / "b.bin"));
    CB_CHECK(!probe(dir.path() / "c.o2"));
    CB_CHECK(!probe(dir.path() / "missing.o2"));
});

}  // namespace cb::formats::o2
