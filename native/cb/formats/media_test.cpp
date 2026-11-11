// Tests of cb/formats/media.hpp on real images (testdata/media: an ISO 9660 disc built by codebreaker's disc writer, a
// pre-DOS-2.0 floppy, a Windows 1.0 boot image, a partition table with no volumes) and on FAT12/16/32 volumes the tests
// format themselves (a floppy, a hard-disk image with primary and logical partitions, a disc with an El Torito floppy, the
// same disc in raw 2352-byte sectors).
//
// The numbers asserted for the real images are the ones the Python implementation gave on the same files
// (state/work/typed-port/formats.media/expected.txt), each read against the file before it was written down here.
//
// DIVERGES from the Python reader (state/work/typed-port/_bugs/LIST.md): a volume number past the end is a UsageError
// (Python counted negative numbers from the end); a volume whose size is not more than its own tables is not a FAT volume
// (the Python went on with a negative cluster count); a file that is not a module bundle is not read whole when it is
// larger than 64 MB.
#include "cb/formats/media.hpp"

#include <algorithm>
#include <array>
#include <numeric>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/random_file.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
namespace media = cb::formats::media;

namespace {

std::filesystem::path media_data(const char* name) {
    return testing::data_path(std::string("formats/testdata/media/") + name);
}

std::uint32_t crc32(ByteView data) {
    std::uint32_t crc = 0xFFFFFFFFu;
    for (std::uint8_t b : data) {
        crc ^= b;
        for (int k = 0; k < 8; ++k) crc = (crc >> 1) ^ (0xEDB88320u & (0u - (crc & 1)));
    }
    return ~crc;
}

// "size/crc32" as the old harness printed it.
std::string sig(ByteView data) { return std::format("{}/{:08x}", data.size(), crc32(data)); }

std::unique_ptr<media::Volume> only_volume(const std::filesystem::path& image, bool long_names = true) {
    auto all = media::volumes(image, long_names);
    CB_CHECK_EQ(all.size(), 1u);
    return std::move(all.at(0));
}

Bytes pattern(std::size_t n, std::uint8_t seed) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>(seed + i * 7 + i / 251);
    return b;
}

void put_le(Bytes& b, std::size_t at, std::uint64_t v, int width) {
    for (int k = 0; k < width; ++k) b[at + static_cast<std::size_t>(k)] = static_cast<std::uint8_t>(v >> (8 * k));
}

// Formats a FAT volume of `bits` (12, 16 or 32) at `offset` of the image file, which is extended to hold it: a floppy-like
// FAT12 (2880 sectors), a 4 MB FAT16 or a 36 MB FAT32, one sector per cluster, an empty root folder.
struct Formatted {
    std::uint64_t bytes = 0;
};
Formatted format_fat(const std::filesystem::path& image, std::uint64_t offset, int bits) {
    const std::uint32_t total = bits == 12 ? 2880 : bits == 16 ? 8192 : 70000;
    const std::uint32_t reserved = bits == 32 ? 32 : 1, roots = bits == 32 ? 0 : bits == 12 ? 224 : 512;
    const std::uint32_t spf = bits == 12 ? 9 : bits == 16 ? 32 : 545;
    Bytes boot(512, 0);
    boot[0] = 0xEB, boot[1] = 0x3C, boot[2] = 0x90;
    std::copy_n("TESTFMT ", 8, boot.begin() + 3);
    put_le(boot, 11, 512, 2);
    boot[13] = 1;
    put_le(boot, 14, reserved, 2);
    boot[16] = 2;
    put_le(boot, 17, roots, 2);
    put_le(boot, 19, bits == 32 ? 0 : total, 2);
    boot[21] = bits == 12 ? 0xF0 : 0xF8;
    put_le(boot, 22, bits == 32 ? 0 : spf, 2);
    put_le(boot, 24, 18, 2);
    put_le(boot, 26, 2, 2);
    put_le(boot, 32, bits == 32 ? total : 0, 4);
    std::size_t ext = 36;
    if (bits == 32) {
        put_le(boot, 36, spf, 4);
        put_le(boot, 44, 2, 4);   // the root folder's cluster
        ext = 64;
    }
    boot[ext + 2] = 0x29;
    put_le(boot, ext + 3, 0xCAFE1234u, 4);
    std::copy_n("NO NAME    ", 11, boot.begin() + static_cast<std::ptrdiff_t>(ext) + 7);
    std::copy_n(bits == 12 ? "FAT12   " : bits == 16 ? "FAT16   " : "FAT32   ", 8, boot.begin() + static_cast<std::ptrdiff_t>(ext) + 18);
    boot[510] = 0x55, boot[511] = 0xAA;

    Bytes fat_start;   // the first entries of each FAT: the media byte and the end-of-chain marks
    if (bits == 12) fat_start = {0xF0, 0xFF, 0xFF};
    else if (bits == 16) fat_start = {0xF8, 0xFF, 0xFF, 0xFF};
    else fat_start = {0xF8, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x0F};   // + the root folder's cluster 2

    fs::RandomFile f(image, std::filesystem::exists(image) ? fs::FileMode::ReadWrite : fs::FileMode::Create);
    f.write_at(offset, boot);
    for (std::uint32_t i = 0; i < 2; ++i) f.write_at(offset + (static_cast<std::uint64_t>(reserved) + i * spf) * 512, fat_start);
    const std::uint64_t end = offset + static_cast<std::uint64_t>(total) * 512;
    if (f.size() < end) f.write_at(end - 1, Bytes(1, 0));
    return {end - offset};
}

std::vector<std::string> paths(const std::vector<media::Entry>& entries) {
    std::vector<std::string> out;
    for (const media::Entry& e : entries) out.push_back(e.path);
    return out;
}

media::NewFile file(std::string path, Bytes data) { return {std::move(path), std::move(data)}; }

}  // namespace

// ---- real images --------------------------------------------------------------------------------------------------------

CB_TEST("formats.media.iso_built_disc", [] {
    const auto v = only_volume(media_data("built.iso"));
    CB_CHECK(v->kind() == media::VolumeKind::Iso9660);
    CB_CHECK_EQ(media::kind_name(v->kind()), std::string_view("iso9660"));
    CB_CHECK_EQ(v->label(), std::string("BUILT_DISC"));
    CB_CHECK_EQ(v->where(), std::string());
    const auto boot = v->boot();
    CB_CHECK_EQ(boot.size(), 7u);
    CB_CHECK_EQ(boot[0].name, std::string("system"));
    CB_CHECK_EQ(boot[0].text, std::string());
    CB_CHECK_EQ(boot[2].name, std::string("blocks"));
    CB_CHECK_EQ(*boot[2].number, 37);
    CB_CHECK_EQ(*boot[3].number, 2048);
    CB_CHECK_EQ(boot[4].text, std::string("2026111109414500"));
    CB_CHECK_EQ(boot[5].text, std::string("no"));    // long names
    CB_CHECK_EQ(boot[6].text, std::string("no"));    // bootable

    const auto& files = v->files();
    CB_CHECK(paths(files) == (std::vector<std::string>{"EMPTY", "LONG_FIL.TXT", "README.TXT", "SETUP", "SETUP/DATA",
                                                       "SETUP/DATA/FILE1.DAT", "SETUP/DATA/FILE2.DAT", "SETUP/INSTALL.EXE"}));
    CB_CHECK(files[0].is_dir);
    CB_CHECK_EQ(files[0].start, 21u);
    CB_CHECK_EQ(files[0].size, 2048u);
    CB_CHECK_EQ(files[1].size, 4u);
    CB_CHECK_EQ(files[1].start, 24u);
    CB_CHECK_EQ(files[1].when, std::string("2026-11-11 09:41"));
    CB_CHECK_EQ(files[1].short_name, std::string("LONG_FIL.TXT"));
    CB_CHECK_EQ(files[5].start, 29u);
    CB_CHECK_EQ(files[5].extents.size(), 1u);
    CB_CHECK_EQ(files[5].extents[0].block, 29u);
    CB_CHECK_EQ(files[5].extents[0].size, 16000u);
    CB_CHECK_EQ(files[6].size, 0u);   // an empty file

    CB_CHECK_EQ(sig(v->read(files[1])), std::string("4/3b97a968"));
    CB_CHECK_EQ(sig(v->read(files[2])), std::string("450/c3d74246"));
    CB_CHECK_EQ(sig(v->read(files[5])), std::string("16000/83a8de7b"));
    CB_CHECK_EQ(sig(v->read(files[7])), std::string("5000/b29f1e36"));
    CB_CHECK(v->read(files[0]).empty());   // a folder reads as nothing
    CB_CHECK(v->read(files[6]).empty());
    CB_CHECK(&v->files() == &files);       // read once, kept

    auto* iso = static_cast<media::IsoVolume*>(v.get());
    CB_CHECK_EQ(iso->layout().root_block, 20u);
    CB_CHECK_EQ(iso->layout().root_size, 2048u);
    CB_CHECK(!iso->layout().boot_catalog.has_value());
    CB_CHECK(!iso->boot_image().has_value());
    CB_CHECK(!iso->layout().has_joliet);
});

CB_TEST("formats.media.iso_joliet_names_or_primary", [] {
    const auto joliet = testing::data_path("formats/testdata/iso9660/mini_setup.iso");
    const auto long_v = only_volume(joliet, true);
    const auto plain_v = only_volume(joliet, false);
    const auto long_paths = paths(long_v->files()), plain_paths = paths(plain_v->files());
    CB_CHECK(std::ranges::find(long_paths, "Win95/Mini.cab") != long_paths.end());
    CB_CHECK(std::ranges::find(long_paths, "Docs/Hello.txt") != long_paths.end());
    CB_CHECK(std::ranges::find(plain_paths, "WIN95/MINI.CAB") != plain_paths.end());
    CB_CHECK(std::ranges::find(plain_paths, "DOCS/HELLO.TXT") != plain_paths.end());
    CB_CHECK_EQ(long_paths.size(), plain_paths.size());
    CB_CHECK(static_cast<media::IsoVolume*>(long_v.get())->layout().has_joliet);
    CB_CHECK(static_cast<media::IsoVolume*>(long_v.get())->layout().long_names);
    CB_CHECK(!static_cast<media::IsoVolume*>(plain_v.get())->layout().long_names);
    CB_CHECK_EQ(long_v->boot()[5].text, std::string("yes"));   // the "long names" field says the disc has them either way
    CB_CHECK_EQ(plain_v->boot()[5].text, std::string("yes"));
    for (const media::Entry& e : long_v->files())
        if (e.path == "Win95/Mini.cab") CB_CHECK_EQ(e.size, 2321u);
    // the long and the primary tree name the same data
    Bytes a, b;
    for (const media::Entry& e : long_v->files())
        if (e.path == "Docs/Hello.txt") a = long_v->read(e);
    for (const media::Entry& e : plain_v->files())
        if (e.path == "DOCS/HELLO.TXT") b = plain_v->read(e);
    CB_CHECK(a == b);
    CB_CHECK_EQ(as_text(a), std::string_view("hello, volume\n"));
});

CB_TEST("formats.media.old_floppy_without_a_parameter_block", [] {
    const auto v = only_volume(media_data("old.img"));
    CB_CHECK(v->kind() == media::VolumeKind::Fat12);
    const auto* fat = static_cast<media::FatVolume*>(v.get());
    CB_CHECK(fat->geometry().old);
    CB_CHECK_EQ(fat->geometry().bytes_per_sector, 512u);
    CB_CHECK_EQ(fat->geometry().root_entries, 64u);
    CB_CHECK_EQ(fat->geometry().clusters, 313u);
    CB_CHECK_EQ(v->boot()[0].text, std::string("(no parameter block)"));
    CB_CHECK_EQ(v->boot()[7].text, std::string("FE"));   // the media byte
    const auto& files = v->files();
    CB_CHECK_EQ(files.size(), 1u);
    CB_CHECK_EQ(files[0].path, std::string("OLD.TXT"));
    CB_CHECK_EQ(files[0].size, 600u);
    CB_CHECK_EQ(files[0].start, 2u);
    CB_CHECK_EQ(files[0].attributes, 32);
    CB_CHECK_EQ(files[0].when, std::string("1980-00-00 00:00"));   // a stamp of zeros, shown as it is
    // the chain holds one cluster of the 600 bytes the entry claims: what is stored is what is read
    CB_CHECK_EQ(sig(v->read(files[0])), std::string("512/f39a9bd9"));
    // such a volume cannot be written to
    fs::ScopedTempDir dir("media-test");
    fs::write_file(dir.path() / "old.img", fs::read_file(media_data("old.img")));
    const media::NewFile one[] = {file("NEW.TXT", {1, 2, 3})};
    CB_CHECK_THROWS_AS(UnsupportedError, (void)media::put(dir.path() / "old.img", one));
    // `size` decides whether a boot sector with no parameter block is an old floppy: a big volume is not
    CB_CHECK(!media::FatVolume::try_open(media::Source(media_data("old.img")), "", 10 * 1024 * 1024).has_value());
    CB_CHECK(media::FatVolume::try_open(media::Source(media_data("old.img")), "", 163840).has_value());
});

CB_TEST("formats.media.listing_text_of_the_old_floppy", [] {
    const auto v = only_volume(media_data("old.img"));
    CB_CHECK_EQ(media::listing_text(*v), std::string("fat12 volume '': 1 files, 0 folders, 600 bytes\n"
                                                      "         600  1980-00-00 00:00  OLD.TXT\n"));
    const std::string with_boot = media::listing_text(*v, true, true);
    CB_CHECK(with_boot.find("    system               (no parameter block)\n") != std::string::npos);
    CB_CHECK(with_boot.find("    clusters             313\n") != std::string::npos);
    const auto iso = only_volume(media_data("built.iso"));
    const std::string text = media::listing_text(*iso, false, false);
    CB_CHECK(text.starts_with("iso9660 volume 'BUILT_DISC': 5 files, 3 folders, 21454 bytes\n"));
    CB_CHECK(text.find("    <folder>  2026-11-11 09:41  EMPTY\n") != std::string::npos);
});

CB_TEST("formats.media.windows_1_boot_image_is_a_bundle_of_modules", [] {
    const auto v = only_volume(media_data("alone.bin"));
    CB_CHECK(v->kind() == media::VolumeKind::Bundle);
    CB_CHECK_EQ(v->label(), std::string("alone.bin"));
    CB_CHECK_EQ(*v->boot()[0].number, 11);
    const auto& files = v->files();
    CB_CHECK_EQ(files.size(), 11u);
    CB_CHECK_EQ(files[0].path, std::string("KERNEL.EXE"));
    CB_CHECK_EQ(files[0].size, 30895u);
    CB_CHECK_EQ(files[0].start, 1024u);   // where the module's header is in the image
    CB_CHECK_EQ(files[0].when, std::string());
    CB_CHECK_EQ(files[8].path, std::string("USER.EXE"));
    CB_CHECK_EQ(files[8].size, 68688u);
    CB_CHECK_EQ(files[10].path, std::string("MSDOS.EXE"));
    CB_CHECK_EQ(sig(v->read(files[0])), std::string("30895/ebb69384"));
    CB_CHECK_EQ(sig(v->read(files[1])), std::string("1234/e39a0264"));
    CB_CHECK_EQ(sig(v->read(files[9])), std::string("192/c0b1d0d3"));
    Bytes kernel = v->read(files[0]);
    CB_CHECK(kernel.size() > 2 && kernel[0] == 'M' && kernel[1] == 'Z');   // an ordinary one-file executable
    media::Entry gone;
    gone.path = "NOPE.EXE";
    CB_CHECK_THROWS_AS(NotFoundError, (void)v->read(gone));
});

CB_TEST("formats.media.things_that_are_not_images", [] {
    fs::ScopedTempDir dir("media-test");
    // a partition table with no FAT partition, an empty file, a short one, noise
    CB_CHECK_THROWS_AS(FormatError, (void)media::volumes(media_data("mbr_only.img")));
    fs::write_file(dir.path() / "empty.bin", {});
    CB_CHECK_THROWS_AS(FormatError, (void)media::volumes(dir.path() / "empty.bin"));
    fs::write_file(dir.path() / "short.bin", pattern(32, 1));
    CB_CHECK_THROWS_AS(FormatError, (void)media::volumes(dir.path() / "short.bin"));
    fs::write_file(dir.path() / "noise.bin", pattern(50000, 9));
    CB_CHECK_THROWS_AS(FormatError, (void)media::volumes(dir.path() / "noise.bin"));
    // a folder, a file that is not there
    CB_CHECK_THROWS_AS(FormatError, (void)media::volumes(dir.path()));
    CB_CHECK_THROWS_AS(NotFoundError, (void)media::volumes(dir.path() / "nope.img"));
    CB_CHECK_THROWS_AS(FormatError, (void)media::FatVolume::open(media::Source(dir.path() / "noise.bin")));
    CB_CHECK_THROWS_AS(FormatError, (void)media::IsoVolume::open(media::Source(dir.path() / "noise.bin")));
    CB_CHECK(!media::FatVolume::try_open(media::Source(dir.path() / "short.bin")).has_value());
    CB_CHECK(!media::BundleVolume::try_open(dir.path() / "noise.bin").has_value());
    CB_CHECK(!media::BundleVolume::try_open(dir.path() / "empty.bin").has_value());
    CB_CHECK_THROWS_AS(NotFoundError, (void)media::BundleVolume::try_open(dir.path() / "nope.bin").value());
});

// ---- volumes the tests format --------------------------------------------------------------------------------------------------

CB_TEST("formats.media.fat12_put_and_read_back", [] {
    fs::ScopedTempDir dir("media-test");
    const auto image = dir.path() / "floppy.img";
    CB_CHECK_EQ(format_fat(image, 0, 12).bytes, 2880u * 512);
    {
        const auto v = only_volume(image);
        CB_CHECK(v->kind() == media::VolumeKind::Fat12);
        CB_CHECK_EQ(v->label(), std::string("NO NAME"));
        CB_CHECK_EQ(v->boot()[0].text, std::string("TESTFMT"));
        CB_CHECK_EQ(*v->boot()[12].number, 2847);   // clusters
        CB_CHECK_EQ(v->boot()[13].text, std::string("CAFE1234"));
        CB_CHECK_EQ(v->boot()[16].text, std::string("55AA"));
        CB_CHECK_EQ(v->boot()[17].text, std::string("jmp 03Eh"));
        CB_CHECK(v->files().empty());
    }
    const Bytes big = pattern(5000, 3), small = pattern(100, 50);
    const media::NewFile first[] = {file("A.TXT", small), file("lower.txt", {}), file("SUB\\DIR/DEEP.BIN", big), file("sub/Z.TXT", {7})};
    const auto done = media::put(image, first);
    CB_CHECK_EQ(done.size(), 4u);
    CB_CHECK_EQ(done[0].path, std::string("A.TXT"));
    CB_CHECK(done[0].outcome == media::PutOutcome::Added);
    CB_CHECK_EQ(done[2].path, std::string("SUB/DIR/DEEP.BIN"));
    {
        auto v = only_volume(image);
        // folders before what is in them; a folder made by put() is entered once and reused by the next file
        CB_CHECK(paths(v->files()) == (std::vector<std::string>{"A.TXT", "LOWER.TXT", "SUB", "SUB/DIR", "SUB/DIR/DEEP.BIN", "SUB/Z.TXT"}));
        const auto& files = v->files();
        CB_CHECK(files[2].is_dir);
        CB_CHECK(files[3].is_dir);
        CB_CHECK_EQ(files[2].attributes, 0x10);
        CB_CHECK_EQ(files[0].attributes, 0x20);
        CB_CHECK(v->read(files[0]) == small);
        CB_CHECK(v->read(files[1]).empty());
        CB_CHECK(v->read(files[4]) == big);
        CB_CHECK_EQ(v->read(files[5]).size(), 1u);
        CB_CHECK_EQ(files[4].size, 5000u);
        CB_CHECK_EQ(files[0].when.size(), 16u);
        CB_CHECK(files[0].when >= "2020");   // stamped with the time of the write
    }
    // both copies of the FAT were written
    {
        const auto fat12 = media::FatVolume::open(media::Source(image));
        const Bytes raw = fs::read_file(image);
        const std::size_t first_fat = 512, second_fat = 512 + 9 * 512;
        CB_CHECK(std::equal(raw.begin() + first_fat, raw.begin() + first_fat + 9 * 512, raw.begin() + second_fat));
        CB_CHECK(fat12.fat().size() == 9 * 512);
    }
    // replacing a file frees its clusters and keeps its entry; the next file reuses them
    const media::NewFile again[] = {file("a.txt", pattern(700, 8)), file("B.TXT", pattern(1, 1))};
    const auto second = media::put(image, again);
    CB_CHECK(second[0].outcome == media::PutOutcome::Replaced);
    CB_CHECK(second[1].outcome == media::PutOutcome::Added);
    {
        auto v = only_volume(image);
        const auto& files = v->files();
        CB_CHECK_EQ(files[0].path, std::string("A.TXT"));
        CB_CHECK_EQ(files[0].size, 700u);
        CB_CHECK(v->read(files[0]) == pattern(700, 8));
        CB_CHECK_EQ(paths(files).size(), 7u);   // one more file, none lost
    }
    // a file for the volume that is too big; and the table stays consistent
    const media::NewFile huge[] = {file("HUGE.BIN", Bytes(2 * 1024 * 1024, 5))};
    CB_CHECK_THROWS_AS(Error, (void)media::put(image, huge));
    {
        auto v = only_volume(image);
        CB_CHECK_EQ(v->files().size(), 7u);   // the volume still reads
    }
});

CB_TEST("formats.media.put_refuses_what_it_cannot_write", [] {
    fs::ScopedTempDir dir("media-test");
    const auto image = dir.path() / "floppy.img";
    format_fat(image, 0, 12);
    const media::NewFile ok[] = {file("COMMAND.COM", {1, 2, 3})};
    media::put(image, ok);
    for (const char* bad : {"TOO LONG NAME.TXT", "A.TOOLONG", "A*B.TXT", "A.TXT ", "", "///", "TWO.DOTS.X"}) {
        const media::NewFile one[] = {file(bad, {1})};
        CB_CHECK_THROWS_AS(UsageError, (void)media::put(image, one));
    }
    const media::NewFile as_folder[] = {file("COMMAND.COM/X", {1})};
    CB_CHECK_THROWS_AS(UsageError, (void)media::put(image, as_folder));
    const media::NewFile unicode[] = {file("\xC3\xA9.txt", {1}), file("\xE4\xB8\xAD.txt", {1})};   // e acute is DOS; a CJK character is not
    CB_CHECK_THROWS_AS(UsageError, (void)media::put(image, unicode));
    // the first file of the failed batch was written before the second was refused: the table kept its copies in step
    auto v = only_volume(image);
    CB_CHECK(paths(v->files()) == (std::vector<std::string>{"COMMAND.COM", "\xC3\x89.TXT"}));
    // volume numbers
    CB_CHECK_THROWS_AS(UsageError, (void)media::put(image, ok, 1));
    // a disc is not a FAT volume
    fs::write_file(dir.path() / "disc.iso", fs::read_file(media_data("built.iso")));
    CB_CHECK_THROWS_AS(UnsupportedError, (void)media::put(dir.path() / "disc.iso", ok));
    CB_CHECK_THROWS_AS(NotFoundError, (void)media::put(dir.path() / "missing.img", ok));
    // no files is no change
    CB_CHECK(media::put(image, {}).empty());
});

CB_TEST("formats.media.fat12_root_folder_fills_up", [] {
    fs::ScopedTempDir dir("media-test");
    const auto image = dir.path() / "floppy.img";
    format_fat(image, 0, 12);
    std::vector<media::NewFile> many;
    for (int i = 0; i < 224; ++i) many.push_back(file(std::format("F{:04}.TXT", i), {}));
    CB_CHECK_EQ(media::put(image, many).size(), 224u);
    const media::NewFile one[] = {file("ONE.TXT", {})};
    CB_CHECK_THROWS_AS(Error, (void)media::put(image, one));   // the root of FAT12/16 has a fixed number of entries
    CB_CHECK_EQ(only_volume(image)->files().size(), 224u);
    // a subfolder grows by clusters instead
    const media::NewFile in_folder[] = {file("SUB/X.TXT", {1})};
    CB_CHECK_THROWS_AS(Error, (void)media::put(image, in_folder));   // (no slot in the root for the folder itself)
});

CB_TEST("formats.media.fat16_and_fat32_with_a_folder_that_grows", [] {
    for (int bits : {16, 32}) {
        fs::ScopedTempDir dir("media-test");
        const auto image = dir.path() / "disk.img";
        format_fat(image, 0, bits);
        auto v = only_volume(image);
        CB_CHECK(v->kind() == (bits == 16 ? media::VolumeKind::Fat16 : media::VolumeKind::Fat32));
        CB_CHECK_EQ(media::kind_name(v->kind()), std::string_view(bits == 16 ? "fat16" : "fat32"));
        CB_CHECK_EQ(static_cast<media::FatVolume*>(v.get())->geometry().bits, bits);
        // more files in one folder than a cluster holds (16 entries of 32 bytes in 512): the folder takes another cluster
        std::vector<media::NewFile> files;
        for (int i = 0; i < 40; ++i) files.push_back(file(std::format("DIR/F{:03}.DAT", i), pattern(static_cast<std::size_t>(i) * 37, static_cast<std::uint8_t>(i))));
        files.push_back(file("TOP.TXT", pattern(1500, 2)));
        const auto done = media::put(image, files);
        CB_CHECK_EQ(done.size(), 41u);
        v = only_volume(image);
        const auto& listed = v->files();
        CB_CHECK_EQ(listed.size(), 42u);   // DIR, its 40 files, TOP.TXT
        CB_CHECK_EQ(listed[0].path, std::string("DIR"));
        CB_CHECK(listed[0].is_dir);
        for (int i = 0; i < 40; ++i) {
            const media::Entry& e = listed[static_cast<std::size_t>(i) + 1];
            CB_CHECK_EQ(e.path, std::format("DIR/F{:03}.DAT", i));
            CB_CHECK(v->read(e) == pattern(static_cast<std::size_t>(i) * 37, static_cast<std::uint8_t>(i)));
        }
        CB_CHECK(v->read(listed[41]) == pattern(1500, 2));
        // a chain that loops back on itself ends where it comes back
        const auto* fat = static_cast<media::FatVolume*>(v.get());
        const std::uint32_t top = static_cast<std::uint32_t>(listed[41].start);
        CB_CHECK_EQ(fat->read_chain(top).size(), 1536u);   // three clusters
        CB_CHECK_EQ(fat->read_chain(top, 600).size(), 600u);
        CB_CHECK(fat->read_chain(0).empty());
        CB_CHECK(fat->read_chain(1).empty());
    }
});

CB_TEST("formats.media.long_names_deleted_entries_and_the_volume_label", [] {
    fs::ScopedTempDir dir("media-test");
    const auto image = dir.path() / "floppy.img";
    format_fat(image, 0, 12);
    const media::NewFile put_files[] = {file("LONGNA~1.TXT", pattern(40, 4)), file("GONE.TXT", {1}), file("KEEP.TXT", {2})};
    media::put(image, put_files);
    {   // rewrite the root: [LFN "Long Name.txt"][LONGNA~1.TXT][GONE.TXT deleted][volume label "MYDISK"][KEEP.TXT]
        fs::RandomFile f(image, fs::FileMode::ReadWrite);
        const std::uint64_t root = (1 + 2 * 9) * 512;
        const Bytes slots = f.read_at(root, 96);
        Bytes lfn(32, 0);
        lfn[0] = 0x41;   // the last (and only) piece
        lfn[11] = 0x0F;
        const std::u16string name = u"Long Name.txt";   // 13 units fill one piece
        const std::array<std::size_t, 13> where = {1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30};
        for (std::size_t i = 0; i < 13; ++i) put_le(lfn, where[i], name[i], 2);
        Bytes label(32, 0);
        std::copy_n("MYDISK     ", 11, label.begin());
        label[11] = 0x08;
        Bytes gone(slots.begin() + 32, slots.begin() + 64);
        gone[0] = 0xE5;
        f.write_at(root, lfn);
        f.write_at(root + 32, Bytes(slots.begin(), slots.begin() + 32));
        f.write_at(root + 64, gone);
        f.write_at(root + 96, label);
        f.write_at(root + 128, Bytes(slots.begin() + 64, slots.begin() + 96));
    }
    auto v = only_volume(image);
    CB_CHECK_EQ(v->label(), std::string("NO NAME"));   // the boot sector has a name: it stays
    const auto& files = v->files();
    CB_CHECK(paths(files) == (std::vector<std::string>{"Long Name.txt", "KEEP.TXT"}));
    CB_CHECK_EQ(files[0].short_name, std::string("LONGNA~1.TXT"));
    CB_CHECK_EQ(files[0].size, 40u);
    CB_CHECK(v->read(files[0]) == pattern(40, 4));
    // the 8.3 names are what the plain listing shows
    CB_CHECK(media::listing_text(*v, false).find("  LONGNA~1.TXT\n") != std::string::npos);
    CB_CHECK(media::listing_text(*v, true).find("  Long Name.txt\n") != std::string::npos);
});

CB_TEST("formats.media.a_volume_with_no_label_takes_the_root_entrys", [] {
    fs::ScopedTempDir dir("media-test");
    const auto image = dir.path() / "floppy.img";
    format_fat(image, 0, 12);
    {
        fs::RandomFile f(image, fs::FileMode::ReadWrite);
        Bytes boot = f.read_at(0, 512);
        std::fill(boot.begin() + 43, boot.begin() + 54, ' ');   // a blank label in the boot sector
        f.write_at(0, boot);
        Bytes label(32, 0);
        std::copy_n("MYDISK     ", 11, label.begin());
        label[11] = 0x08;
        f.write_at((1 + 2 * 9) * 512, label);
    }
    auto v = only_volume(image);
    CB_CHECK_EQ(v->label(), std::string());
    CB_CHECK(v->files().empty());
    CB_CHECK_EQ(v->label(), std::string("MYDISK"));
});

CB_TEST("formats.media.partitions_primary_and_logical", [] {
    fs::ScopedTempDir dir("media-test");
    const auto image = dir.path() / "disk.img";
    // sector 0: an MBR with a FAT12 partition at 63 and an extended partition at 3000; there, a table naming a logical drive
    // 63 sectors further on
    format_fat(image, 63 * 512, 12);
    format_fat(image, (3000 + 63) * 512, 12);
    {
        fs::RandomFile f(image, fs::FileMode::ReadWrite);
        Bytes mbr(512, 0);
        auto entry = [&](Bytes& table, int i, std::uint8_t type, std::uint32_t first, std::uint32_t count) {
            table[446 + 16 * static_cast<std::size_t>(i) + 4] = type;
            put_le(table, 446 + 16 * static_cast<std::size_t>(i) + 8, first, 4);
            put_le(table, 446 + 16 * static_cast<std::size_t>(i) + 12, count, 4);
        };
        entry(mbr, 0, 0x01, 63, 2880);
        entry(mbr, 1, 0x05, 3000, 2943);
        mbr[510] = 0x55, mbr[511] = 0xAA;
        f.write_at(0, mbr);
        Bytes ebr(512, 0);
        entry(ebr, 0, 0x01, 63, 2880);
        ebr[510] = 0x55, ebr[511] = 0xAA;
        f.write_at(3000 * 512, ebr);
    }
    auto all = media::volumes(image);
    CB_CHECK_EQ(all.size(), 2u);
    CB_CHECK_EQ(all[0]->where(), std::string("partition at sector 63, type 01"));
    CB_CHECK_EQ(all[1]->where(), std::string("partition at sector 3063, type 01"));
    // writing goes to the volume asked for, and only there
    const media::NewFile p0[] = {file("P0.TXT", pattern(10, 1))}, p1[] = {file("P1.TXT", pattern(20, 2))};
    media::put(image, p0, 0);
    media::put(image, p1, 1);
    all = media::volumes(image);
    CB_CHECK(paths(all[0]->files()) == (std::vector<std::string>{"P0.TXT"}));
    CB_CHECK(paths(all[1]->files()) == (std::vector<std::string>{"P1.TXT"}));
    CB_CHECK(all[1]->read(all[1]->files()[0]) == pattern(20, 2));
    // extraction puts the second volume in a folder of its own
    const auto out = dir.path() / "out";
    CB_CHECK_EQ(media::extract(image, out), 2u);
    CB_CHECK(fs::read_file(out / "P0.TXT") == pattern(10, 1));
    CB_CHECK(fs::read_file(out / "volume2" / "P1.TXT") == pattern(20, 2));
});

CB_TEST("formats.media.disc_with_an_el_torito_floppy_and_the_same_disc_in_raw_sectors", [] {
    fs::ScopedTempDir dir("media-test");
    // the floppy
    const auto floppy = dir.path() / "floppy.img";
    format_fat(floppy, 0, 12);
    const media::NewFile in_floppy[] = {file("BOOT.SYS", pattern(900, 6))};
    media::put(floppy, in_floppy);
    const Bytes floppy_bytes = fs::read_file(floppy);
    // the disc: built.iso with a boot record where its terminator was, a boot catalog and the floppy appended
    Bytes disc = fs::read_file(media_data("built.iso"));
    const std::size_t terminator = 17 * 2048;
    Bytes boot_record(2048, 0);
    boot_record[0] = 0;
    std::copy_n("CD001", 5, boot_record.begin() + 1);
    boot_record[6] = 1;
    std::copy_n("EL TORITO SPECIFICATION", 23, boot_record.begin() + 7);
    const std::uint32_t catalog_sector = 40, image_sector = 41;
    put_le(boot_record, 71, catalog_sector, 4);
    std::copy(boot_record.begin(), boot_record.end(), disc.begin() + static_cast<std::ptrdiff_t>(terminator));
    Bytes end_descriptor(2048, 0);
    end_descriptor[0] = 255;
    std::copy_n("CD001", 5, end_descriptor.begin() + 1);
    end_descriptor[6] = 1;
    std::copy(end_descriptor.begin(), end_descriptor.end(), disc.begin() + 18 * 2048);
    disc.resize(static_cast<std::size_t>(image_sector) * 2048, 0);
    Bytes catalog(2048, 0);
    catalog[0] = 1;
    catalog[30] = 0x55, catalog[31] = 0xAA;
    catalog[32] = 0x88;
    catalog[33] = 2;   // a 1.44 MB floppy
    put_le(catalog, 38, 1, 2);
    put_le(catalog, 40, image_sector, 4);
    std::copy(catalog.begin(), catalog.end(), disc.begin() + static_cast<std::ptrdiff_t>(catalog_sector) * 2048);
    append(disc, floppy_bytes);
    const auto iso_path = dir.path() / "boot.iso";
    fs::write_file(iso_path, disc);

    auto all = media::volumes(iso_path);
    CB_CHECK_EQ(all.size(), 2u);
    CB_CHECK(all[0]->kind() == media::VolumeKind::Iso9660);
    CB_CHECK(all[1]->kind() == media::VolumeKind::Fat12);
    CB_CHECK_EQ(all[1]->where(), std::string("boot floppy inside the disc"));
    CB_CHECK_EQ(all[0]->boot()[6].text, std::string("yes"));
    const auto boot = static_cast<media::IsoVolume*>(all[0].get())->boot_image();
    CB_CHECK(boot.has_value());
    CB_CHECK_EQ(boot->media, 2);
    CB_CHECK_EQ(boot->size, 1474560u);
    CB_CHECK_EQ(boot->offset, static_cast<std::uint64_t>(image_sector) * 2048);
    CB_CHECK(paths(all[1]->files()) == (std::vector<std::string>{"BOOT.SYS"}));
    CB_CHECK(all[1]->read(all[1]->files()[0]) == pattern(900, 6));
    CB_CHECK_EQ(all[0]->files().size(), 8u);   // the disc's own files are still there

    // the floppy is a volume to write to; the disc's files are not disturbed
    const media::NewFile more[] = {file("MORE.TXT", pattern(30, 3))};
    media::put(iso_path, more, 1);
    all = media::volumes(iso_path);
    CB_CHECK_EQ(all[1]->files().size(), 2u);
    CB_CHECK_EQ(sig(all[0]->read(all[0]->files()[7])), std::string("5000/b29f1e36"));

    // extraction: the disc, then the floppy in volume2
    const auto out = dir.path() / "out";
    CB_CHECK_EQ(media::extract(iso_path, out), 5u + 2u);
    CB_CHECK_EQ(sig(fs::read_file(out / "SETUP" / "INSTALL.EXE")), std::string("5000/b29f1e36"));
    CB_CHECK(fs::read_file(out / "volume2" / "BOOT.SYS") == pattern(900, 6));

    // the same disc as raw 2352-byte sectors (16- and 24-byte headers): the same files
    for (const std::uint32_t skip : {16u, 24u}) {
        const std::size_t sectors = disc.size() / 2048;
        Bytes raw;
        for (std::size_t s = 0; s < sectors; ++s) {
            Bytes sector(2352, 0);
            std::copy(disc.begin() + static_cast<std::ptrdiff_t>(s) * 2048, disc.begin() + static_cast<std::ptrdiff_t>(s + 1) * 2048,
                      sector.begin() + skip);
            append(raw, sector);
        }
        const auto raw_path = dir.path() / std::format("raw{}.iso", skip);
        fs::write_file(raw_path, raw);
        const auto raw_volumes = media::volumes(raw_path);
        CB_CHECK_EQ(raw_volumes.size(), 1u);   // (a raw disc's floppy is not looked for)
        const auto* raw_iso = static_cast<media::IsoVolume*>(raw_volumes[0].get());
        CB_CHECK_EQ(raw_iso->layout().sector, 2352u);
        CB_CHECK_EQ(raw_iso->layout().skip, skip);
        const auto plain = only_volume(media_data("built.iso"));
        CB_CHECK(paths(raw_volumes[0]->files()) == paths(plain->files()));
        for (const media::Entry& e : raw_volumes[0]->files())
            if (e.path == "SETUP/DATA/FILE1.DAT") CB_CHECK_EQ(sig(raw_volumes[0]->read(e)), std::string("16000/83a8de7b"));
    }
});

CB_TEST("formats.media.extract_with_wildcards", [] {
    fs::ScopedTempDir dir("media-test");
    const auto out = dir.path() / "out";
    CB_CHECK_EQ(media::extract(media_data("built.iso"), out), 5u);
    CB_CHECK_EQ(sig(fs::read_file(out / "LONG_FIL.TXT")), std::string("4/3b97a968"));
    CB_CHECK_EQ(sig(fs::read_file(out / "SETUP" / "DATA" / "FILE1.DAT")), std::string("16000/83a8de7b"));
    CB_CHECK_EQ(fs::read_file(out / "SETUP" / "DATA" / "FILE2.DAT").size(), 0u);
    CB_CHECK(!std::filesystem::exists(out / "EMPTY") || std::filesystem::is_directory(out / "EMPTY") == false);   // folders are not made for their own sake

    const auto out2 = dir.path() / "out2";
    const std::string dat[] = {"*.dat"};
    CB_CHECK_EQ(media::extract(media_data("built.iso"), out2, dat), 2u);
    CB_CHECK(std::filesystem::exists(out2 / "SETUP" / "DATA" / "FILE1.DAT"));
    CB_CHECK(!std::filesystem::exists(out2 / "README.TXT"));

    const auto out3 = dir.path() / "out3";
    const std::string by_path[] = {"setup/install.exe", "readme.*"};   // a path, and a name with a wildcard; case ignored
    CB_CHECK_EQ(media::extract(media_data("built.iso"), out3, by_path), 2u);
    const auto out4 = dir.path() / "out4";
    const std::string none[] = {"*.zzz"};
    CB_CHECK_EQ(media::extract(media_data("built.iso"), out4, none), 0u);
    CB_CHECK_THROWS_AS(NotFoundError, (void)media::extract(dir.path() / "nothing-here.bin", dir.path() / "o"));
});

CB_TEST("formats.media.wildcards", [] {
    using media::wildcard_match;
    CB_CHECK(wildcard_match("*.txt", "README.TXT"));
    CB_CHECK(wildcard_match("*", ""));
    CB_CHECK(wildcard_match("a*b*c", "aXXbYYc"));
    CB_CHECK(!wildcard_match("a*b*c", "aXXbYY"));
    CB_CHECK(wildcard_match("?.COM", "a.com"));
    CB_CHECK(!wildcard_match("?.COM", "ab.com"));
    CB_CHECK(wildcard_match("[a-c]x", "BX"));
    CB_CHECK(!wildcard_match("[!a-c]x", "bx"));
    CB_CHECK(wildcard_match("[!a-c]x", "dx"));
    CB_CHECK(wildcard_match("[]]x", "]x"));
    CB_CHECK(wildcard_match("[x", "[x"));     // no closing bracket: a plain '['
    CB_CHECK(wildcard_match("dir/file", "DIR\\FILE"));   // '/' equals '\'
    CB_CHECK(!wildcard_match("dir/*.txt", "dir/sub/a.dat"));
    CB_CHECK(wildcard_match("", ""));
    CB_CHECK(!wildcard_match("", "a"));
});

CB_TEST("formats.media.safe_paths", [] {
    CB_CHECK(media::safe_relative_path("a/b.txt") == std::filesystem::path("a") / "b.txt");
    CB_CHECK(media::safe_relative_path("a\\b\\c.txt") == std::filesystem::path("a") / "b" / "c.txt");
    CB_CHECK(media::safe_relative_path("../../etc/passwd") == std::filesystem::path("etc") / "passwd");
    CB_CHECK(media::safe_relative_path("/abs/./x") == std::filesystem::path("abs") / "x");
    CB_CHECK(media::safe_relative_path("we:ird*na?me<>|\"") == std::filesystem::path("we_ird_na_me____"));
    CB_CHECK_THROWS_AS(FormatError, (void)media::safe_relative_path(".."));
    CB_CHECK_THROWS_AS(FormatError, (void)media::safe_relative_path(""));
});
