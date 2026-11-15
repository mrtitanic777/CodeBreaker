#include "cb/formats/coleco.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::coleco {
namespace {

// A 16 KB cartridge: $55 $AA, the start at $8040, a JP in the RST $10 vector, RETN as the NMI, a title.
Bytes cartridge(bool title_screen = false) {
    Bytes b(0x4000, 0);
    b[0] = title_screen ? 0xAA : 0x55;
    b[1] = title_screen ? 0x55 : 0xAA;
    b[0x0A] = 0x40;
    b[0x0B] = 0x80;
    b[0x0F] = 0xC3;   // RST $10 at $800F
    b[0x10] = 0x00;
    b[0x11] = 0x90;
    b[0x21] = 0xED;   // the NMI vector: RETN
    b[0x22] = 0x45;
    const std::string title = "DEMO/TEST/2026";
    for (std::size_t i = 0; i < title.size(); ++i) b[0x24 + i] = static_cast<std::uint8_t>(title[i]);
    b[0x40] = 0xC9;
    return b;
}

CB_TEST("formats.coleco.the_header_gives_the_start_the_vectors_and_the_title", [] {
    const Program p = parse(cartridge(true));
    CB_CHECK(p.header.title_screen);
    CB_CHECK_EQ(p.header.start, 0x8040);
    CB_CHECK_EQ(p.header.title, "DEMO/TEST/2026");
    CB_CHECK_EQ(p.image.size(), 0x4000u);
    CB_CHECK_EQ(p.entries.size(), 3u);
    CB_CHECK_EQ(p.entries[0].address, 0x8040);
    CB_CHECK_EQ(p.entries[0].name, "start");
    CB_CHECK_EQ(p.entries[1].address, 0x800F);   // RST $10
    CB_CHECK_EQ(p.entries[1].name, "rst10");
    CB_CHECK_EQ(p.entries[2].address, 0x8021);
    CB_CHECK_EQ(p.entries[2].name, "nmi");
    CB_CHECK(!parse(cartridge(false)).header.title_screen);
});

CB_TEST("formats.coleco.a_start_outside_the_cartridge_is_not_an_entry", [] {
    Bytes b = cartridge();
    b[0x0A] = 0x00;
    b[0x0B] = 0x20;
    const Program p = parse(b);
    for (const Entry& e : p.entries) CB_CHECK(e.name != "start");
});

CB_TEST("formats.coleco.what_is_not_a_cartridge_is_refused", [] {
    Bytes junk(0x4000, 0);
    CB_CHECK_THROWS_AS(FormatError, parse(junk));
    CB_CHECK_THROWS_AS(UnsupportedError, parse(Bytes(0x10000, 0xAA)));   // 64 KB: a mapper
});

CB_TEST("formats.coleco.probe_takes_col_files_and_small_roms_with_the_header", [] {
    fs::ScopedTempDir dir("cb_coleco");
    fs::write_file(dir.path() / "game.col", cartridge());
    fs::write_file(dir.path() / "game.rom", cartridge(true));
    fs::write_file(dir.path() / "junk.col", Bytes(0x4000, 0));
    fs::write_file(dir.path() / "game.txt", cartridge());
    CB_CHECK(probe(dir.path() / "game.col"));
    CB_CHECK(probe(dir.path() / "game.rom"));
    CB_CHECK(!probe(dir.path() / "junk.col"));
    CB_CHECK(!probe(dir.path() / "game.txt"));
    CB_CHECK(!probe(dir.path() / "absent.col"));
    CB_CHECK(read(dir.path() / "game.col").header.start == 0x8040);
});

CB_TEST("formats.coleco.the_bios_entry_points_have_the_names_of_its_listing", [] {
    CB_CHECK_EQ(bios_names().at(0x1FD6), "NO_SOUND");
    CB_CHECK_EQ(bios_names().at(0x1FBE), "BLK_WRT_VRAM");
    CB_CHECK_EQ(bios_names().at(0x1FFD), "RANDOM");
});

}  // namespace
}  // namespace cb::formats::coleco
