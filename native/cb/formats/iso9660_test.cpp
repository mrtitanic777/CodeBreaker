#include "cb/formats/iso9660.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// The fixture testdata/iso9660/mini_setup.iso was made by pycdlib (an independent writer): ISO 9660 level 1 with a
// Joliet tree, two directories and four files:
//     /WIN95/MINI.CAB (Joliet "Mini.cab"): "MINICAB-ORIGINAL-" then bytes 0..255 nine times = 2321 bytes
//     /WIN95/SETUP.EXE ("Setup.exe"):      "MZ-setup-exe-original" twenty times = 420 bytes
//     /WIN95/README.  ("Readme"):          "read me first\r\n"
//     /DOCS/HELLO.TXT ("Hello.txt"):       "hello, volume\n"
// DIVERGES from pycdlib's rm_file + add_fp (what verify/gate16/setup16.py did to put a file on the CD): the volume is
// not laid out again; the new data is appended and the records repointed, so every other file stays where it was.

using namespace cb;
using namespace cb::formats::iso9660;

namespace {

std::filesystem::path fixture() { return testing::data_path("formats/testdata/iso9660/mini_setup.iso"); }

Bytes minicab() {
    Bytes b = to_bytes("MINICAB-ORIGINAL-");
    for (int i = 0; i < 9; ++i)
        for (int v = 0; v < 256; ++v)
            b.push_back(static_cast<std::uint8_t>(v));
    return b;
}

Bytes setup_exe() {
    Bytes b;
    for (int i = 0; i < 20; ++i)
        append(b, as_bytes("MZ-setup-exe-original"));
    return b;
}

std::vector<std::string> names(const std::vector<Entry>& entries) {
    std::vector<std::string> out;
    for (const Entry& e : entries)
        out.push_back(e.name);
    return out;
}

// A copy of the fixture to patch.
struct Copy {
    fs::ScopedTempDir dir{"cb-iso"};
    std::filesystem::path path = dir.path() / "cd.iso";
    Copy() { fs::write_file(path, fs::read_file(fixture())); }
};

}  // namespace

CB_TEST("formats.iso9660.list", [] {
    const Volume cd(fixture());
    CB_CHECK(cd.has_joliet());
    CB_CHECK_EQ(cd.sector_count() * kSectorSize, fs::file_size(fixture()));
    CB_CHECK(names(cd.list("/")) == std::vector<std::string>{"DOCS", "WIN95"});
    const std::vector<Entry> win95 = cd.list("/WIN95");
    CB_CHECK(names(win95) == std::vector<std::string>{"MINI.CAB", "README", "SETUP.EXE"});   // no ";1", no trailing "."
    CB_CHECK_EQ(win95[0].size, 2321u);
    CB_CHECK_EQ(win95[2].size, 420u);
    CB_CHECK(!win95[0].directory);
    CB_CHECK(cd.list("/")[0].directory);
    // the Joliet tree's names keep their case
    CB_CHECK(names(cd.list("/Win95", Names::Joliet)) == std::vector<std::string>{"Mini.cab", "Readme", "Setup.exe"});
});

CB_TEST("formats.iso9660.read", [] {
    const Volume cd(fixture());
    CB_CHECK(cd.read("/WIN95/MINI.CAB") == minicab());
    CB_CHECK(cd.read("/WIN95/SETUP.EXE") == setup_exe());
    CB_CHECK_EQ(std::string(as_text(cd.read("/WIN95/README"))), "read me first\r\n");
    CB_CHECK_EQ(std::string(as_text(cd.read("/DOCS/HELLO.TXT"))), "hello, volume\n");
    // case does not matter, nor does the ";1"
    CB_CHECK(cd.read("/win95/mini.cab") == minicab());
    CB_CHECK(cd.read("/WIN95/MINI.CAB;1") == minicab());
    CB_CHECK(cd.read("WIN95//MINI.CAB") == minicab());
    // the Joliet tree reaches the same data
    CB_CHECK(cd.read("/Win95/Mini.cab", Names::Joliet) == minicab());
});

CB_TEST("formats.iso9660.errors", [] {
    const Volume cd(fixture());
    CB_CHECK_THROWS_AS(NotFoundError, cd.read("/WIN95/NOPE.CAB"));
    CB_CHECK_THROWS_AS(NotFoundError, cd.list("/NOPE"));
    CB_CHECK_THROWS_AS(UsageError, cd.read("/WIN95"));                 // a directory
    CB_CHECK_THROWS_AS(FormatError, cd.list("/WIN95/MINI.CAB"));        // a file is not a directory
    Volume ro(fixture());
    CB_CHECK_THROWS_AS(UsageError, ro.replace("/WIN95/MINI.CAB", Bytes{1}));   // opened read-only
    CB_CHECK_THROWS_AS(NotFoundError, Volume("no/such/image.iso"));
});

CB_TEST("formats.iso9660.not_a_volume", [] {
    fs::ScopedTempDir tmp("cb-iso");
    fs::write_file(tmp.path() / "junk.iso", Bytes(100000, 0x5A));
    CB_CHECK_THROWS_AS(FormatError, Volume(tmp.path() / "junk.iso"));
    fs::write_file(tmp.path() / "empty.iso", Bytes{});
    CB_CHECK_THROWS_AS(FormatError, Volume(tmp.path() / "empty.iso"));
    // cut off inside the directories: a Truncated/Format error, not a crash
    Bytes cut = fs::read_file(fixture());
    cut.resize(18 * kSectorSize);   // the descriptors are there; the root directory's extent is not
    fs::write_file(tmp.path() / "cut.iso", cut);
    const Volume cd(tmp.path() / "cut.iso");
    CB_CHECK_THROWS_AS(FormatError, cd.list("/"));
});

CB_TEST("formats.iso9660.replace_a_file", [] {
    Copy c;
    const std::uint32_t sectors_before = Volume(c.path).sector_count();
    Bytes bigger = minicab();
    bigger.resize(5000, 0xAB);   // three sectors where it had two
    {
        Volume cd(c.path, Access::ReadWrite);
        cd.replace("/WIN95/MINI.CAB", bigger);
        CB_CHECK_EQ(cd.sector_count(), sectors_before + 3);
        CB_CHECK(cd.read("/WIN95/MINI.CAB") == bigger);
    }
    // a fresh open sees it in both trees, with the new size in the listing, the others unchanged
    const Volume cd(c.path);
    CB_CHECK_EQ(cd.sector_count(), sectors_before + 3);
    CB_CHECK_EQ(cd.sector_count() * kSectorSize, fs::file_size(c.path));
    CB_CHECK(cd.read("/WIN95/MINI.CAB") == bigger);
    CB_CHECK(cd.read("/Win95/Mini.cab", Names::Joliet) == bigger);
    CB_CHECK_EQ(cd.list("/WIN95")[0].size, 5000u);
    CB_CHECK_EQ(cd.list("/Win95", Names::Joliet)[0].size, 5000u);
    CB_CHECK(cd.read("/WIN95/SETUP.EXE") == setup_exe());
    CB_CHECK_EQ(std::string(as_text(cd.read("/DOCS/HELLO.TXT"))), "hello, volume\n");
    // nothing before the appended data moved: the sectors of the original are byte for byte as they were
    const Bytes before = fs::read_file(fixture()), after = fs::read_file(c.path);
    const std::size_t prefix = std::size_t{sectors_before} * kSectorSize;
    // (the records and the descriptors' size fields are what changed: the rest of every sector is the same)
    std::size_t differing = 0;
    for (std::size_t i = 0; i < prefix; ++i)
        differing += before[i] != after[i];
    CB_CHECK(differing < 64);   // (two records' extents and sizes, two descriptors' volume sizes: at most 48 bytes)
    CB_CHECK(differing > 0);
});

CB_TEST("formats.iso9660.replace_twice_and_with_nothing", [] {
    Copy c;
    Volume cd(c.path, Access::ReadWrite);
    cd.replace("/WIN95/README", as_bytes("one"));
    cd.replace("/WIN95/README", as_bytes("a second text, replacing the first"));
    CB_CHECK_EQ(std::string(as_text(cd.read("/WIN95/README"))), "a second text, replacing the first");
    cd.replace("/WIN95/SETUP.EXE", Bytes{});   // an empty file
    CB_CHECK(cd.read("/WIN95/SETUP.EXE").empty());
    CB_CHECK_EQ(cd.list("/WIN95")[2].size, 0u);
    CB_CHECK_THROWS_AS(UsageError, cd.replace("/WIN95", as_bytes("x")));   // not a directory
    CB_CHECK_THROWS_AS(NotFoundError, cd.replace("/WIN95/NOPE.CAB", as_bytes("x")));
    // a failed replacement leaves the volume as it was
    CB_CHECK(cd.read("/WIN95/MINI.CAB") == minicab());
});

CB_TEST("formats.iso9660.an_image_that_is_not_a_whole_number_of_sectors", [] {
    Copy c;
    fs::append_file(c.path, as_bytes("trailing bytes after the last sector"));
    Volume cd(c.path, Access::ReadWrite);
    cd.replace("/WIN95/README", as_bytes("new readme"));
    CB_CHECK_EQ(std::string(as_text(cd.read("/WIN95/README"))), "new readme");
    CB_CHECK_EQ(fs::file_size(c.path) % kSectorSize, 0u);   // the appended data begins on a sector
    CB_CHECK_EQ(cd.sector_count() * kSectorSize, fs::file_size(c.path));
});
