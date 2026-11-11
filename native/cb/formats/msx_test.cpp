#include "cb/formats/msx.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::msx {
namespace {

// `size` bytes with "AB" and the four header addresses at `at`.
Bytes image(std::size_t size, std::size_t at, std::uint16_t init, std::uint16_t statement = 0, std::uint16_t device = 0,
            std::uint16_t text = 0) {
    Bytes b(size, 0);
    b[at] = 'A';
    b[at + 1] = 'B';
    const std::uint16_t words[4] = {init, statement, device, text};
    for (std::size_t i = 0; i < 4; ++i) {
        b[at + 2 + 2 * i] = static_cast<std::uint8_t>(words[i] & 0xFF);
        b[at + 3 + 2 * i] = static_cast<std::uint8_t>(words[i] >> 8);
    }
    return b;
}

CB_TEST("formats.msx.a_16kb_cartridge_sits_in_page_1", [] {
    const Program p = parse(image(0x4000, 0, 0x4010, 0x4020, 0, 0x4100));
    CB_CHECK_EQ(p.origin, 0x4000u);
    CB_CHECK_EQ(p.image.size(), 0x4000u);
    CB_CHECK_EQ(p.header.init, 0x4010);
    CB_CHECK_EQ(p.header.statement, 0x4020);
    CB_CHECK_EQ(p.header.device, 0);
    CB_CHECK_EQ(p.header.text, 0x4100);
    // the TEXT address is not an entry; the unused DEVICE is skipped
    CB_CHECK_EQ(p.entries.size(), 2u);
    CB_CHECK_EQ(p.entries[0].address, 0x4010);
    CB_CHECK_EQ(p.entries[0].name, "init");
    CB_CHECK_EQ(p.entries[1].address, 0x4020);
    CB_CHECK_EQ(p.entries[1].name, "statement");
});

CB_TEST("formats.msx.two_names_for_one_address_keep_the_first", [] {
    const Program p = parse(image(0x4000, 0, 0x4010, 0x4010, 0x4010));
    CB_CHECK_EQ(p.entries.size(), 1u);
    CB_CHECK_EQ(p.entries[0].name, "init");
});

CB_TEST("formats.msx.entries_outside_the_image_are_dropped", [] {
    const Program p = parse(image(0x4000, 0, 0x4010, 0x8000, 0x3FFF));
    CB_CHECK_EQ(p.entries.size(), 1u);
    CB_CHECK_EQ(p.entries[0].address, 0x4010);
});

CB_TEST("formats.msx.the_init_address_places_the_image", [] {
    CB_CHECK_EQ(parse(image(0x4000, 0, 0x0100)).origin, 0x0000u);                  // init below $4000: at $0000
    CB_CHECK_EQ(parse(image(0x4000, 0, 0x8010)).origin, 0x8000u);                  // init at $8000+ and a 16 KB ROM
    CB_CHECK_EQ(parse(image(0x8000, 0, 0x8010)).origin, 0x4000u);                  // a 32 KB ROM fills pages 1-2
    CB_CHECK_EQ(parse(image(0x4000, 0, 0)).origin, 0x4000u);                       // no init: page 1
    CB_CHECK_EQ(parse(image(0x4000, 0, 0x0100)).entries.size(), 1u);               // and the entry is inside it
});

CB_TEST("formats.msx.a_48kb_image_with_the_header_at_4000", [] {
    const Program p = parse(image(0xC000, 0x4000, 0x4010));
    CB_CHECK_EQ(p.origin, 0u);
    CB_CHECK_EQ(p.image.size(), 0xC000u);
    CB_CHECK_EQ(p.entries.size(), 1u);
    CB_CHECK_EQ(p.entries[0].address, 0x4010);
});

CB_TEST("formats.msx.errors", [] {
    CB_CHECK_THROWS_AS(UnsupportedError, (void)parse(image(0xC001, 0, 0x4010)));   // a mapper
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x4000, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{'X', 'B', 0, 0, 0, 0, 0, 0, 0, 0}));
    CB_CHECK_THROWS_AS(TruncatedError, (void)parse(Bytes{'A', 'B', 0x10, 0x40}));   // header cut short
});

CB_TEST("formats.msx.read_and_probe_a_file", [] {
    fs::ScopedTempDir dir("msx-test");
    fs::write_file(dir.path() / "game.rom", image(0x4000, 0, 0x4010));
    fs::write_file(dir.path() / "big.rom", image(0xC000, 0x4000, 0x4010));
    fs::write_file(dir.path() / "none.rom", Bytes(0x4000, 0));
    fs::write_file(dir.path() / "game.bin", image(0x4000, 0, 0x4010));
    CB_CHECK(probe(dir.path() / "game.rom"));
    CB_CHECK(probe(dir.path() / "big.rom"));
    CB_CHECK(!probe(dir.path() / "none.rom"));
    CB_CHECK(!probe(dir.path() / "game.bin"));
    CB_CHECK(!probe(dir.path() / "missing.rom"));
    CB_CHECK_EQ(read(dir.path() / "game.rom").origin, 0x4000u);
});

}  // namespace
}  // namespace cb::formats::msx
