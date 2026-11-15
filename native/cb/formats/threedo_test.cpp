// cb-unity: alone
#include "cb/formats/threedo.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::threedo {
namespace {

void put_be(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}

// A small AIF, big-endian words: NOP, NOP, NOP, BL entry (0x40), the exit SWI, the sizes, then a push, a mov and a return at 0x40.
Bytes aif() {
    Bytes f(0x50, 0);
    put_be(f, 0, 0xE1A00000);
    put_be(f, 4, 0xE1A00000);
    put_be(f, 8, 0xE1A00000);
    put_be(f, 12, 0xEB00000B);   // BL: 0xC + 8 + 11 * 4 = 0x40
    put_be(f, 16, 0xEF000011);
    put_be(f, 20, 0x50);         // read-only size
    put_be(f, 48, 0x20);         // address mode
    put_be(f, 0x40, 0xE92D4010);
    put_be(f, 0x44, 0xE3A04001);
    put_be(f, 0x48, 0xE8BD8010);
    return f;
}

// An Opera disc: the volume header at block 0, the root directory at block 1 (one file, LaunchMe), the file at block 2.
Bytes disc(const Bytes& launch) {
    Bytes d(2048 * 2, 0);
    d[0] = 1;
    for (int i = 1; i <= 5; ++i) d[static_cast<std::size_t>(i)] = 0x5A;
    d[6] = 1;
    const std::string label = "TESTDISC";
    std::copy(label.begin(), label.end(), d.begin() + 39);
    put_be(d, 75, 2048);
    put_be(d, 79, 3);
    put_be(d, 87, 1);
    put_be(d, 91, 2048);
    put_be(d, 95, 0);
    put_be(d, 99, 1);
    const std::size_t dir = 2048;
    put_be(d, dir, 0xFFFFFFFF);
    put_be(d, dir + 4, 0xFFFFFFFF);
    put_be(d, dir + 12, 0x14 + 72);
    put_be(d, dir + 16, 0x14);
    const std::size_t e = dir + 0x14;
    put_be(d, e, 0xC0000002);
    put_be(d, e + 12, 2048);
    put_be(d, e + 16, static_cast<std::uint32_t>(launch.size()));
    put_be(d, e + 20, 1);
    const std::string name = "LaunchMe";
    std::copy(name.begin(), name.end(), d.begin() + static_cast<std::ptrdiff_t>(e) + 32);
    put_be(d, e + 64, 0);
    put_be(d, e + 68, 2);
    d.insert(d.end(), launch.begin(), launch.end());
    d.resize(2048 * 3, 0);
    return d;
}

}  // namespace

CB_TEST("formats.threedo.a_bare_aif_has_its_entry_at_the_bl_of_word_3", [] {
    const Program p = parse(aif());
    CB_CHECK(p.kind == Kind::Aif);
    CB_CHECK_EQ(p.aif.entry, 0x40u);
    CB_CHECK_EQ(p.aif.read_only, 0x50u);
    CB_CHECK_EQ(p.image_size, 0x50u);
    CB_CHECK_EQ(p.native()[0x40], 0x10);   // the word E92D4010 with its bytes reversed
    CB_CHECK_EQ(p.native()[0x43], 0xE9);
});

CB_TEST("formats.threedo.a_disc_is_walked_to_its_launchme", [] {
    const Program p = parse(disc(aif()));
    CB_CHECK(p.kind == Kind::Disc);
    CB_CHECK_EQ(p.label, "TESTDISC");
    CB_CHECK_EQ(p.root.size(), 1u);
    CB_CHECK_EQ(p.root[0].name, "LaunchMe");
    CB_CHECK_EQ(p.image_offset, 4096u);
    CB_CHECK_EQ(p.image_size, 0x50u);
    CB_CHECK_EQ(p.aif.entry, 0x40u);
    Bytes no_launch = disc(aif());
    no_launch[2048 + 0x14 + 32] = 'X';
    CB_CHECK_THROWS_AS(FormatError, (void)parse(no_launch));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(0x100, 0)));
});

CB_TEST("formats.threedo.probe", [] {
    fs::ScopedTempDir dir("cb_3do_probe");
    fs::write_file(dir.path() / "a.iso", disc(aif()));
    fs::write_file(dir.path() / "LaunchMe", aif());
    fs::write_file(dir.path() / "b.aif", aif());
    fs::write_file(dir.path() / "c.iso", Bytes(4096, 0));
    fs::write_file(dir.path() / "d.txt", aif());
    CB_CHECK(probe(dir.path() / "a.iso"));
    CB_CHECK(probe(dir.path() / "LaunchMe"));
    CB_CHECK(probe(dir.path() / "b.aif"));
    CB_CHECK(!probe(dir.path() / "c.iso"));
    CB_CHECK(!probe(dir.path() / "d.txt"));
    CB_CHECK(!probe(dir.path() / "missing.iso"));
});

}  // namespace cb::formats::threedo
