// cb-unity: alone
#include "cb/formats/jaguar.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::jaguar {
namespace {

void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

// A cartridge of `size` bytes: header flags $04040404 and the start address at $400.
Bytes cartridge(std::size_t size, std::uint32_t start) {
    Bytes rom(size, 0xFF);
    put32(rom, 0x400, 0x04040404);
    put32(rom, 0x404, start);
    return rom;
}

// A COFF with one .text section of `text` bytes loaded at `address`, entry `entry`.
Bytes coff(std::uint32_t address, std::uint32_t entry, const Bytes& text) {
    Bytes f(20 + 28 + 40, 0);
    f[0] = 0x01;
    f[1] = 0x50;
    f[3] = 1;
    f[17] = 28;   // the a.out header
    put32(f, 20 + 4, static_cast<std::uint32_t>(text.size()));
    put32(f, 20 + 16, entry);
    put32(f, 20 + 20, address);
    const char name[] = ".text";
    std::copy(name, name + 5, f.begin() + 48);
    put32(f, 48 + 8, address);
    put32(f, 48 + 12, address);
    put32(f, 48 + 16, static_cast<std::uint32_t>(text.size()));
    put32(f, 48 + 20, 88);
    put32(f, 48 + 36, 0x20);
    f.insert(f.end(), text.begin(), text.end());
    return f;
}

}  // namespace

CB_TEST("formats.jaguar.a_cartridge_is_at_800000_and_starts_where_its_header_says", [] {
    const Program p = parse(cartridge(0x4000, 0x802000));
    CB_CHECK(p.kind == Kind::Cartridge);
    CB_CHECK_EQ(p.entry, 0x802000u);
    CB_CHECK_EQ(p.header_flags, 0x04040404u);
    CB_CHECK_EQ(p.segments.size(), 1u);
    CB_CHECK_EQ(p.offset(0x802000).value(), 0x2000u);
    CB_CHECK(!p.offset(0x804000).has_value());
    CB_CHECK_EQ(p.memory()[0x800400], 0x04);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(cartridge(0x4000, 0x900000)));   // start outside the ROM
});

CB_TEST("formats.jaguar.a_coff_loads_its_sections_where_they_say", [] {
    const Program p = parse(coff(0x4000, 0x4002, Bytes{0x4E, 0x71, 0x4E, 0x75}));
    CB_CHECK(p.kind == Kind::Coff);
    CB_CHECK_EQ(p.entry, 0x4002u);
    CB_CHECK_EQ(p.segments.size(), 1u);
    CB_CHECK_EQ(p.segments[0].name, ".text");
    CB_CHECK_EQ(p.offset(0x4003).value(), 91u);
    CB_CHECK_EQ(p.memory()[0x4002], 0x4E);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(coff(0x4000, 0x5000, Bytes{0x4E, 0x75})));   // entry in no section
});

CB_TEST("formats.jaguar.probe", [] {
    fs::ScopedTempDir dir("cb_jag_probe");
    fs::write_file(dir.path() / "a.j64", cartridge(0x4000, 0x802000));
    fs::write_file(dir.path() / "b.rom", cartridge(0x4000, 0x802000));
    fs::write_file(dir.path() / "c.j64", Bytes(0x4000, 0xFF));
    fs::write_file(dir.path() / "d.cof", coff(0x4000, 0x4000, Bytes{0x4E, 0x75}));
    fs::write_file(dir.path() / "e.jag", coff(0x4000, 0x4000, Bytes{0x4E, 0x75}));
    fs::write_file(dir.path() / "f.txt", cartridge(0x4000, 0x802000));
    CB_CHECK(probe(dir.path() / "a.j64"));
    CB_CHECK(probe(dir.path() / "b.rom"));
    CB_CHECK(!probe(dir.path() / "c.j64"));
    CB_CHECK(probe(dir.path() / "d.cof"));
    CB_CHECK(probe(dir.path() / "e.jag"));
    CB_CHECK(!probe(dir.path() / "f.txt"));
    CB_CHECK(!probe(dir.path() / "missing.j64"));
});

}  // namespace cb::formats::jaguar
