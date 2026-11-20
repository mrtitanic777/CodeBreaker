// Tests of FIX95CPU's cache (verify/gate16/fix95_cache): the key, which files of the floppy are read back off an installed disk, the entry on disk,
// and what the run does with it. Small FAT images made here stand for the floppy and the installed disk.
#include "verify/gate16/fix95_cache.hpp"

#include <cstring>
#include <set>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/media.hpp"

using namespace cb;
using namespace cb::verify::gate16;

namespace {

namespace media = cb::formats::media;

Bytes bytes_of(std::string_view s) { return Bytes(s.begin(), s.end()); }

// A blank 1.44 MB FAT12 image.
Bytes blank_floppy() {
    Bytes img(2880 * 512, 0);
    std::uint8_t* b = img.data();
    b[0] = 0xEB;
    b[1] = 0x3C;
    b[2] = 0x90;
    std::memcpy(b + 3, "TESTDISK", 8);
    const auto put16 = [&](std::size_t at, unsigned v) {
        b[at] = static_cast<std::uint8_t>(v);
        b[at + 1] = static_cast<std::uint8_t>(v >> 8);
    };
    put16(11, 512);
    b[13] = 1;
    put16(14, 1);
    b[16] = 2;
    put16(17, 224);
    put16(19, 2880);
    b[21] = 0xF0;
    put16(22, 9);
    put16(24, 18);
    put16(26, 2);
    b[510] = 0x55;
    b[511] = 0xAA;
    for (const std::size_t fat : {std::size_t{512}, std::size_t{512 + 9 * 512}}) {
        b[fat] = 0xF0;
        b[fat + 1] = 0xFF;
        b[fat + 2] = 0xFF;
    }
    return img;
}

void make_image(const std::filesystem::path& path, const std::vector<media::NewFile>& files) {
    fs::write_file(path, blank_floppy());
    media::put(path, files);
}

// The floppy: IOS.VXD and NDIS.VXD for both editions, the OSR2 and RTM copies of ESDI_506.PDR, and VFBACKUP.VXD in POST.
std::vector<media::NewFile> floppy_files() {
    return {{"W95BOTH/SYSTEM/VMM32/IOS.VXD", bytes_of("ios-patched")},
            {"W95BOTH/SYSTEM/NDIS.VXD", bytes_of("ndis-patched")},
            {"W95BOTH/SYSTEM/VMM32/POST/VFBACKUP.VXD", bytes_of("vfbackup-patched")},
            {"W95OSR2/SYSTEM/IOSUBSYS/ESDI_506.PDR", bytes_of("esdi-osr2")},
            {"W95RTM/SYSTEM/IOSUBSYS/ESDI_506.PDR", bytes_of("esdi-rtm")},
            {"README.TXT", bytes_of("read me")}};
}

}  // namespace

CB_TEST("verify.gate16.fix95.key_depends_on_both_inputs", [] {
    const std::string a = fix95::key("f1", "i1");
    CB_CHECK_EQ(a, fix95::key("f1", "i1"));
    CB_CHECK(a != fix95::key("f2", "i1"));
    CB_CHECK(a != fix95::key("f1", "i2"));
    CB_CHECK(fix95::key("ab", "c") != fix95::key("a", "bc"));   // (the fields do not run together)
});

CB_TEST("verify.gate16.fix95.files_of_the_floppy", [] {
    fs::ScopedTempDir dir("fix95");
    make_image(dir.path() / "fix.ima", floppy_files());
    const std::vector<fix95::Wanted> w = fix95::files_of(dir.path() / "fix.ima");
    CB_CHECK_EQ(w.size(), std::size_t{5});   // (README.TXT is not copied into Windows)
    bool saw_post = false;
    for (const fix95::Wanted& f : w)
        if (f.name == "VFBACKUP.VXD") {
            saw_post = true;
            CB_CHECK_EQ(f.under, "SYSTEM/VMM32/VFBACKUP.VXD");   // (POST is an order of copying, not a folder)
        }
    CB_CHECK(saw_post);
});

CB_TEST("verify.gate16.fix95.capture_reads_back_what_the_disk_holds_as_the_floppy_has_it", [] {
    fs::ScopedTempDir dir("fix95");
    make_image(dir.path() / "fix.ima", floppy_files());
    // an OSR2 install: its ESDI_506.PDR is the OSR2 copy; NDIS.VXD is not the floppy's; VFBACKUP.VXD is missing
    make_image(dir.path() / "hd.img", {{"MSDOS.SYS", bytes_of("[Paths]\r\nWinDir=C:\\WINDOWS\r\n")},
                                        {"WINDOWS/SYSTEM/VMM32/IOS.VXD", bytes_of("ios-patched")},
                                        {"WINDOWS/SYSTEM/NDIS.VXD", bytes_of("ndis-original")},
                                        {"WINDOWS/SYSTEM/IOSUBSYS/ESDI_506.PDR", bytes_of("esdi-osr2")}});
    const fix95::Capture c = fix95::capture(dir.path() / "hd.img", dir.path() / "fix.ima");
    CB_CHECK_EQ(c.files.size(), std::size_t{2});
    CB_CHECK(c.conflicts.empty());
    std::set<std::string> names;
    for (const fix95::Captured& f : c.files) names.insert(f.name);
    CB_CHECK(names.contains("IOS.VXD"));
    CB_CHECK(names.contains("ESDI_506.PDR"));
    CB_CHECK_EQ(c.notes.size(), std::size_t{3});   // NDIS differs, VFBACKUP is missing, the RTM ESDI_506 is the other edition's
    for (const fix95::Captured& f : c.files)
        if (f.name == "IOS.VXD") CB_CHECK(f.data == bytes_of("ios-patched"));
});

CB_TEST("verify.gate16.fix95.capture_without_an_installed_system_captures_nothing", [] {
    fs::ScopedTempDir dir("fix95");
    make_image(dir.path() / "fix.ima", floppy_files());
    make_image(dir.path() / "hd.img", {{"AUTOEXEC.BAT", bytes_of("x")}});
    const fix95::Capture c = fix95::capture(dir.path() / "hd.img", dir.path() / "fix.ima");
    CB_CHECK(c.files.empty());
    CB_CHECK_EQ(c.notes.size(), std::size_t{1});
});

CB_TEST("verify.gate16.fix95.the_entry_round_trips_and_a_changed_file_voids_it", [] {
    fs::ScopedTempDir dir("fix95");
    fix95::Capture cap;
    cap.files = {{"IOS.VXD", "WINDOWS/SYSTEM/VMM32/IOS.VXD", bytes_of("ios-patched")}, {"NDIS.VXD", "WINDOWS/SYSTEM/NDIS.VXD", bytes_of("ndis-patched")}};
    const std::map<std::string, std::string> originals = {{"IOS.VXD", "aa"}, {"NDIS.VXD", "bb"}};
    const std::filesystem::path folder = dir.path() / "cache" / "k";
    CB_CHECK(!fix95::load(folder));   // (nothing yet)
    const fix95::Entry stored = fix95::store(folder, cap, originals);
    CB_CHECK(stored.usable());
    const std::optional<fix95::Entry> back = fix95::load(folder);
    CB_CHECK(back.has_value());
    if (!back) return;
    CB_CHECK_EQ(back->files.size(), std::size_t{2});
    CB_CHECK(back->usable());
    CB_CHECK_EQ(back->files[0].original_hash, "aa");
    fs::write_file(folder / "files" / "IOS.VXD", bytes_of("tampered"));
    CB_CHECK(!fix95::load(folder));   // (a file that is not the manifest's)
});

CB_TEST("verify.gate16.fix95.a_file_in_no_cabinet_is_an_addition_and_a_conflict_voids_the_entry", [] {
    fs::ScopedTempDir dir("fix95");
    fix95::Capture cap;
    cap.files = {{"IOS.VXD", "WINDOWS/SYSTEM/VMM32/IOS.VXD", bytes_of("a")}, {"NTKERN.VXD", "WINDOWS/SYSTEM/VMM32/NTKERN.VXD", bytes_of("b")}};
    const fix95::Entry e = fix95::store(dir.path() / "k", cap, {{"IOS.VXD", "aa"}});   // (NTKERN is in no cabinet: Setup installs none)
    CB_CHECK(e.usable());
    CB_CHECK_EQ(e.files.size(), std::size_t{1});
    CB_CHECK_EQ(e.additions.size(), std::size_t{1});
    const std::optional<fix95::Entry> back = fix95::load(dir.path() / "k");
    CB_CHECK(back.has_value());
    if (!back) return;
    CB_CHECK(back->usable());
    CB_CHECK_EQ(back->additions.size(), std::size_t{1});
    const fix95::Plan p = fix95::plan({}, back);
    CB_CHECK(p.replace.has_value());
    CB_CHECK(p.note.find("NTKERN.VXD") != std::string::npos);   // (said, not hidden)
    // a name the disk holds twice, differently, cannot be replaced by name: the floppy stays
    cap.conflicts = {"IOS.VXD"};
    const fix95::Entry c = fix95::store(dir.path() / "c", cap, {{"IOS.VXD", "aa"}});
    CB_CHECK(!c.usable());
    CB_CHECK(!fix95::plan({}, fix95::load(dir.path() / "c")).replace);
});

CB_TEST("verify.gate16.fix95.the_plan_skips_the_floppy_only_with_a_whole_entry", [] {
    fs::ScopedTempDir dir("fix95");
    fix95::Capture cap;
    cap.files = {{"IOS.VXD", "WINDOWS/SYSTEM/VMM32/IOS.VXD", bytes_of("ios")}};
    const fix95::Entry entry = fix95::store(dir.path() / "k", cap, {{"IOS.VXD", "aa"}});
    const setup16::Replace mine = {{"USER.EXE", "built/user.exe"}};
    // no entry: the floppy stage runs (and fills it)
    CB_CHECK(!fix95::plan(mine, std::nullopt).replace);
    // an entry: our modules and FIX95CPU's files together
    const fix95::Plan p = fix95::plan(mine, entry);
    CB_CHECK(p.replace.has_value());
    if (!p.replace) return;
    CB_CHECK_EQ(p.replace->size(), std::size_t{2});
    CB_CHECK(p.replace->at("IOS.VXD") == entry.path_of("IOS.VXD"));
    CB_CHECK(p.replace->at("USER.EXE") == std::filesystem::path("built/user.exe"));
    CB_CHECK_EQ(mine.size(), std::size_t{1});   // (the run's own map is not changed)
    // a run that replaces one of the files itself keeps the floppy
    CB_CHECK(!fix95::plan({{"ios.vxd", "mine/ios.vxd"}}, entry).replace);
});
