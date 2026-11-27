// Tests of formats/disc_image over images made in the test (formats/disc_test_image.hpp): cooked and raw sectors in both modes, the
// data read across sectors and at offsets, .cue sheets, and an ISO 9660 volume read through a raw image.
#include "formats/disc_image.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/disc_test_image.hpp"
#include "formats/iso9660.hpp"

namespace cb::formats::disc {
namespace {

namespace dt = disc_test;

// Three sectors whose data is their number in every byte.
Bytes numbered() {
    Bytes b(3 * kSector);
    for (std::size_t i = 0; i < b.size(); ++i) b[i] = static_cast<std::uint8_t>(i / kSector + 1);
    return b;
}

}  // namespace

CB_TEST("formats.disc.cooked_and_raw_sectors_hold_the_same_data", [] {
    const Bytes cooked = numbered();
    for (const Bytes& image : {cooked, dt::raw(cooked, 1), dt::raw(cooked, 2)}) {
        const Sectors s = Sectors::of(image);
        CB_CHECK_EQ(s.raw(), image.size() != cooked.size());
        CB_CHECK_EQ(s.count(), 3u);
        CB_CHECK(s.sector(1) == Bytes(kSector, 2));
        CB_CHECK(s.sector(3).empty());
        // across a sector's end, and cut at the image's
        const Bytes across = s.read_at(kSector - 2, 4);
        CB_CHECK(across == (Bytes{1, 1, 2, 2}));
        CB_CHECK_EQ(s.read(2, 5000).size(), kSector);
        CB_CHECK(s.read_at(3 * kSector, 1).empty());
    }
    // told by the caller, not by the bytes
    CB_CHECK(!Sectors::of(dt::raw(cooked), kSector).raw());
    CB_CHECK(Sectors::of(cooked, kRawSector).raw());
});

CB_TEST("formats.disc.a_cue_sheet_names_the_image_and_its_mode", [] {
    const fs::ScopedTempDir dir("cbdisc");
    const Bytes raw = dt::raw(numbered(), 2);
    fs::write_file(dir.path() / "game.bin", raw);
    fs::write_text(dir.path() / "game.cue", "FILE \"game.bin\" BINARY\r\n  TRACK 01 MODE2/2352\r\n    INDEX 01 00:00:00\r\n  TRACK 02 AUDIO\r\n");
    fs::write_text(dir.path() / "none.cue", "TRACK 01 MODE1/2352\n");
    const std::optional<Cue> cue = read_cue(dir.path() / "game.cue");
    CB_CHECK(cue.has_value());
    CB_CHECK(cue->file == dir.path() / "game.bin");
    CB_CHECK_EQ(cue->sector_size, kRawSector);
    CB_CHECK(!read_cue(dir.path() / "none.cue").has_value());
    CB_CHECK(!read_cue(dir.path() / "missing.cue").has_value());
    const Sectors s = Sectors::open(dir.path() / "game.cue");
    CB_CHECK(s.raw());
    CB_CHECK(s.file() == dir.path() / "game.bin");
    CB_CHECK(s.sector(2) == Bytes(kSector, 3));
    CB_CHECK_THROWS_AS(FormatError, Sectors::open(dir.path() / "none.cue"));
    CB_CHECK(is_image_name("x.CUE") && is_image_name("x.bin") && is_image_name("x.iso") && !is_image_name("x.exe"));
});

CB_TEST("formats.disc.an_iso9660_volume_reads_through_raw_sectors", [] {
    const fs::ScopedTempDir dir("cbdisc");
    const Bytes big(5000, 0x5A);
    const Bytes cooked = dt::iso({{"README.TXT", dt::bytes_of("hello")}, {"BIG.DAT", big}});
    fs::write_file(dir.path() / "a.bin", dt::raw(cooked, 2));
    fs::write_file(dir.path() / "b.iso", cooked);
    fs::write_text(dir.path() / "a.cue", "FILE \"a.bin\" BINARY\n  TRACK 01 MODE2/2352\n");
    for (const char* name : {"a.bin", "b.iso", "a.cue"}) {
        const iso9660::Volume v(dir.path() / name);
        const std::vector<iso9660::Entry> root = v.list("/");
        CB_CHECK_EQ(root.size(), 2u);
        CB_CHECK_EQ(root[1].name, "BIG.DAT");
        CB_CHECK_EQ(root[1].extent, 20u);
        CB_CHECK(v.read("/readme.txt") == dt::bytes_of("hello"));
        CB_CHECK(v.read("/BIG.DAT") == big);
    }
    // and one in memory
    const Bytes raw = dt::raw(cooked, 1);
    CB_CHECK(iso9660::Volume(Sectors::of(raw)).read("/BIG.DAT") == big);
    // a raw image is not written in place
    CB_CHECK_THROWS_AS(UsageError, iso9660::Volume(dir.path() / "a.bin").replace("/README.TXT", dt::bytes_of("x")));
});

}  // namespace cb::formats::disc
