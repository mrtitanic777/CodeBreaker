// Tests of cb/build/floppy.hpp: a small FAT12 floppy the test lays out (files that span clusters, a volume label, a file that is replaced
// by bigger or smaller contents, errors), and Windows 95's Setup boot floppy (setup/media/boot.img of the corpus; a test that needs it is
// skipped when the corpus is not on this machine) against what the Python's `floppy.read` / `floppy.make` and `boot-floppy --extract`
// gave for it (testdata/floppy: the extracted files' names, sizes and SHA-1, and the hashes of two images made from it).
#include "cb/build/floppy.hpp"

#include <map>
#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"

using namespace cb;
namespace floppy = cb::build::floppy;

namespace {

// A 32 KB floppy: 512-byte sectors and clusters, 1 reserved, 2 FATs of 1 sector, 16 root entries, 64 sectors.
Bytes floppy_boot() {
    Bytes boot(512, 0);
    const auto put16 = [&](std::size_t at, std::uint16_t v) { boot[at] = static_cast<std::uint8_t>(v), boot[at + 1] = static_cast<std::uint8_t>(v >> 8); };
    put16(11, 512);
    boot[13] = 1;
    put16(14, 1);
    boot[16] = 2;
    put16(17, 16);
    put16(19, 64);
    boot[21] = 0xF0;
    put16(22, 1);
    boot[510] = 0x55;
    boot[511] = 0xAA;
    return boot;
}

floppy::Entry floppy_entry(const std::string& base, const std::string& ext, std::uint8_t attributes) {
    floppy::Entry e;
    e.raw.fill(' ');
    for (std::size_t i = 0; i < base.size(); ++i) e.raw[i] = static_cast<std::uint8_t>(base[i]);
    for (std::size_t i = 0; i < ext.size(); ++i) e.raw[8 + i] = static_cast<std::uint8_t>(ext[i]);
    e.raw[11] = attributes;
    for (std::size_t i = 12; i < 32; ++i) e.raw[i] = 0;
    if ((attributes & 0x18) == 0) e.data = Bytes{};
    return e;
}

floppy::Disk floppy_disk() {
    floppy::Disk disk;
    disk.boot = floppy_boot();
    disk.bpb = floppy::bpb_of(disk.boot);
    disk.entries = {floppy_entry("DISK", "", 0x08), floppy_entry("ALPHA", "TXT", 0x20), floppy_entry("BETA", "BIN", 0x27), floppy_entry("EMPTY", "", 0x20)};
    return disk;
}

Bytes floppy_pattern(std::size_t size, std::uint8_t seed) {
    Bytes out(size);
    for (std::size_t i = 0; i < size; ++i) out[i] = static_cast<std::uint8_t>(seed + i * 7);
    return out;
}

std::optional<Bytes> floppy_corpus_image() {
    std::vector<std::filesystem::path> candidates;
    if (const auto env = fs::get_env("CB_CORPUS")) candidates.push_back(fs::from_text(*env) / "setup" / "media" / "boot.img");
    candidates.push_back("C:/Users/mrtit/Documents/GitHub/Windows95/design/corpus/files/setup/media/boot.img");
    for (const auto& path : candidates) {
        std::error_code ec;
        if (std::filesystem::is_regular_file(path, ec)) return fs::read_file(path);
    }
    return std::nullopt;
}

std::string floppy_golden(const char* name) { return fs::read_text(testing::data_path(std::string("build/testdata/floppy/") + name)); }

}  // namespace

CB_TEST("build.floppy.the_parameter_block_gives_the_layout", [] {
    const floppy::Bpb b = floppy::bpb_of(floppy_boot());
    CB_CHECK_EQ(b.bytes_per_sector, 512);
    CB_CHECK_EQ(b.root_entries, 16);
    CB_CHECK_EQ(b.media, 0xF0);
    CB_CHECK_EQ(b.root_at, std::size_t{3 * 512});          // after the boot sector and two one-sector FATs
    CB_CHECK_EQ(b.data_at, std::size_t{3 * 512 + 16 * 32});
    CB_CHECK_THROWS_AS(FormatError, floppy::bpb_of(Bytes(20, 0)));
});

CB_TEST("build.floppy.files_are_laid_out_in_runs_of_clusters_in_directory_order_and_read_back", [] {
    const floppy::Disk disk = floppy_disk();
    const Bytes alpha = floppy_pattern(1300, 1), beta = floppy_pattern(512, 9);   // 3 clusters, and exactly one
    const Bytes image = floppy::make(disk, disk.boot, {{"ALPHA.TXT", alpha}, {"BETA.BIN", beta}});
    CB_CHECK_EQ(image.size(), std::size_t{64 * 512});
    const floppy::Disk back = floppy::read(image);
    CB_CHECK_EQ(back.entries.size(), std::size_t{4});
    CB_CHECK_EQ(back.entries[0].name(), "DISK");
    CB_CHECK(!back.entries[0].is_file());
    CB_CHECK_EQ(back.entries[1].name(), "ALPHA.TXT");
    CB_CHECK(back.entries[1].data == alpha);
    CB_CHECK(back.entries[2].data == beta);
    CB_CHECK(back.entries[3].data == Bytes{});
    CB_CHECK_EQ(back.entries[2].raw[11], 0x27);   // attributes kept
    // ALPHA at clusters 2-4, BETA at 5; the FAT says so, in both copies
    CB_CHECK_EQ(image[512 + 0], 0xF0);            // the media byte
    CB_CHECK_EQ(image[512 + 3], 0x03);            // cluster 2 -> 3
    const Bytes fat1(image.begin() + 512, image.begin() + 1024), fat2(image.begin() + 1024, image.begin() + 1536);
    CB_CHECK(fat1 == fat2);
    // (BETA starts the cluster after ALPHA's three)
    CB_CHECK(Bytes(image.begin() + static_cast<std::ptrdiff_t>(back.bpb.data_at + 3 * 512), image.begin() + static_cast<std::ptrdiff_t>(back.bpb.data_at + 4 * 512)) == beta);
    // a file not given keeps what the original had
    const Bytes again = floppy::make(back, back.boot, {{"BETA.BIN", floppy_pattern(100, 3)}});
    const floppy::Disk second = floppy::read(again);
    CB_CHECK(second.entries[1].data == alpha);
    CB_CHECK(second.entries[2].data == floppy_pattern(100, 3));
});

CB_TEST("build.floppy.make_refuses_a_foreign_boot_sector_and_files_that_do_not_fit", [] {
    const floppy::Disk disk = floppy_disk();
    Bytes foreign = disk.boot;
    foreign[17] = 32;   // 32 root entries
    CB_CHECK_THROWS_AS(UsageError, floppy::make(disk, foreign, {}));
    // 64 sectors less the boot sector, two FATs and the one sector of root leave 60 clusters of 512 bytes
    CB_CHECK_THROWS_AS(floppy::DoNotFitError, floppy::make(disk, disk.boot, {{"ALPHA.TXT", Bytes(60 * 512 + 1, 1)}}));
    CB_CHECK_NOTHROW((void)floppy::make(disk, disk.boot, {{"ALPHA.TXT", Bytes(60 * 512, 1)}}));
    CB_CHECK_THROWS_AS(floppy::DoNotFitError, floppy::make(disk, disk.boot, {{"ALPHA.TXT", Bytes(30 * 512, 1)}, {"BETA.BIN", Bytes(30 * 512 + 1, 2)}}));
});

CB_TEST("build.floppy.windows_95_setup_floppy_reads_and_makes_like_the_python", [] {
    const auto original = floppy_corpus_image();
    if (!original) return;
    const floppy::Disk disk = floppy::read(*original);
    CB_CHECK_EQ(disk.entries.size(), std::size_t{32});
    CB_CHECK_EQ(disk.entries[0].name(), "COMMAND.COM");
    CB_CHECK_EQ(disk.entries[0].data->size(), std::size_t{93812});
    CB_CHECK_EQ(disk.bpb.root_at, std::size_t{9728});
    CB_CHECK_EQ(disk.bpb.data_at, std::size_t{16896});
    // laid out again with nothing changed, and with two files replaced (the hashes `floppy.make` gave)
    CB_CHECK_EQ(crypto::to_hex(crypto::sha1(floppy::make(disk, disk.boot, {}))), "7709133a1f58687087debb4e3c08600e0c83c835");
    const Bytes changed = floppy::make(disk, disk.boot, {{"AUTOEXEC.BAT", to_bytes("echo hi\r\n")}, {"HIMEM.SYS", Bytes(40000, 'x')}});
    CB_CHECK_EQ(crypto::to_hex(crypto::sha1(changed)), "a2adc0087752dca17034bc00aa62bc6eb44a537c");
    CB_CHECK_THROWS_AS(floppy::DoNotFitError, floppy::make(disk, disk.boot, {{"HIMEM.SYS", Bytes(2000000, 'x')}}));
});

CB_TEST("build.floppy.extract_writes_the_originals_files_like_the_python", [] {
    const auto original = floppy_corpus_image();
    if (!original) return;
    const fs::ScopedTempDir dir{"cb-floppy"};
    fs::write_file(dir.path() / "boot.img", *original);
    floppy::Environment env;
    const std::string target = fs::to_text(dir.path() / "files");
    const std::string printed = testing::run_command(floppy::command(env), {"--image", fs::to_text(dir.path() / "boot.img"), "--extract", target});
    CB_CHECK_EQ(text::replace_all(printed, target, "<dir>"), floppy_golden("extract.txt"));
    // the manifest of the Python lists names as the floppy has them (upper case, in byte order)
    CB_CHECK_EQ(testing::tree_manifest(dir.path() / "files"), floppy_golden("extract_manifest.txt"));
});
