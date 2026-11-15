// cb-unity: alone
#include "cb/formats/chf.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/chf_test_image.hpp"

namespace cb::formats::chf {

CB_TEST("formats.chf.a_cartridge_is_the_memory_from_0800", [] {
    const Rom rom = parse(test_image::cartridge());
    CB_CHECK_EQ(rom.data.size(), 0x400u);
    CB_CHECK_EQ(rom.address(0x20), 0x820u);
    CB_CHECK_EQ(rom.data[0], 0x55);
    CB_CHECK_EQ(rom.data[1], 0x2B);
    CB_CHECK_EQ(bios().at(0x0107), "BIOS_PUSHK");
});

CB_TEST("formats.chf.refuses_what_is_not_one", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes()));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(300, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x10000, 0)));
});

CB_TEST("formats.chf.probe", [] {
    fs::ScopedTempDir dir("cb_chf_probe");
    fs::write_file(dir.path() / "a.chf", test_image::cartridge());
    fs::write_file(dir.path() / "b.bin", test_image::cartridge());
    fs::write_file(dir.path() / "c.bin", Bytes(0x400, 0xFF));
    fs::write_file(dir.path() / "d.chf", Bytes(300, 0));
    CB_CHECK(probe(dir.path() / "a.chf"));
    CB_CHECK(probe(dir.path() / "b.bin"));        // it starts $55 $2B
    CB_CHECK(!probe(dir.path() / "c.bin"));
    CB_CHECK(!probe(dir.path() / "d.chf"));
    CB_CHECK(!probe(dir.path() / "missing.chf"));
});

}  // namespace cb::formats::chf
