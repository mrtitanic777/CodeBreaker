// Tests of cb/formats/xbox/gdf.hpp. The expected values (the built image's SHA-1, the root directory, the file hashes) are what the
// Python's gdf module gives -- gdf.build_xdvdfs and gdf.GDF through formats.dll -- for the same four files
// (state/work/typed-port/xbox-deferred/vol_golden.py in the work that wrote this), so the builder and the reader are compared with it.
//
// DIVERGES from the Python: a volume that is not there is FormatError (ValueError / AssertionError); `read` of a path that is not a
// file is NotFoundError (KeyError); a directory entry whose name would run past its sector is skipped, as before, and a read before the
// start of the device (a negative base the caller gave) is FormatError.
#include "cb/formats/xbox/gdf.hpp"

#include <cstring>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"

namespace gdf_test {

using namespace cb;
using namespace cb::formats::xbox;

class MemoryDevice final : public Device {
public:
    explicit MemoryDevice(Bytes data) : data_(std::move(data)) {}
    Bytes read_at(std::uint64_t offset, std::size_t length) override {
        if (offset >= data_.size()) return {};
        const std::size_t n = std::min<std::size_t>(length, data_.size() - static_cast<std::size_t>(offset));
        return Bytes(data_.begin() + static_cast<std::ptrdiff_t>(offset), data_.begin() + static_cast<std::ptrdiff_t>(offset + n));
    }
    Bytes data_;
};

static std::string sha1_hex(ByteView v) { return crypto::to_hex(crypto::sha1(v)); }

static Bytes pattern(std::size_t n, std::uint8_t step = 1) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>(i * step);
    return b;
}

// the four files the Python's golden was made from
static std::vector<gdf::SourceFile> four_files() {
    Bytes xex = {'X', 'E', 'X', '2'};
    xex.resize(5004, 0);
    Bytes text;
    const std::string_view sentence = "The quick brown fox jumps over the lazy dog. ";
    for (int i = 0; i < 90; ++i) append(text, as_bytes(sentence));
    return {{"default.xex", xex}, {"B.TXT", to_bytes("hello")}, {"a.bin", pattern(2560)}, {"Zed.txt", text}};
}

static Bytes four_file_image() {
    const auto files = four_files();
    return gdf::build(files);
}

CB_TEST("formats.xbox.gdf.build_is_the_pythons_volume", [] {
    const Bytes image = four_file_image();
    CB_CHECK_EQ(image.size(), 88064u);
    CB_CHECK_EQ(sha1_hex(image), "6caeada675d8454129f54ec1b4eb9f1c44b28f77");
    // the descriptor: magic at sector 32 and again 0x7EC on, the root at sector 33
    CB_CHECK_EQ(as_text(ByteView(image).subspan(32 * 0x800, 20)), gdf::kMagic);
    CB_CHECK_EQ(as_text(ByteView(image).subspan(32 * 0x800 + 0x7EC, 20)), gdf::kMagic);
});

CB_TEST("formats.xbox.gdf.the_volume_reads_back", [] {
    gdf::Disc disc = gdf::Disc::open(std::make_shared<MemoryDevice>(four_file_image()));
    CB_CHECK_EQ(disc.base(), 0);
    CB_CHECK_EQ(disc.root_sector(), 33u);
    CB_CHECK_EQ(disc.root_size(), 92u);
    // lower-cased order: a.bin, B.TXT, default.xex, Zed.txt
    const std::vector<gdf::Entry> files = disc.files();
    CB_CHECK_EQ(files.size(), 4u);
    CB_CHECK_EQ(files[0].path, "a.bin");
    CB_CHECK_EQ(files[0].size, 2560u);
    CB_CHECK_EQ(files[1].path, "B.TXT");
    CB_CHECK_EQ(files[2].path, "default.xex");
    CB_CHECK_EQ(files[2].size, 5004u);
    CB_CHECK_EQ(files[3].path, "Zed.txt");
    CB_CHECK_EQ(files[3].size, 4050u);
    CB_CHECK_EQ(sha1_hex(disc.read(files[0])), "f48a95799d144d5ad254355f1849533120dbec12");
    CB_CHECK_EQ(sha1_hex(disc.read("default.xex")), "d75c82566ae7818af77e4ded3a9a78fd3de8dbca");
    CB_CHECK_EQ(sha1_hex(disc.read("Zed.txt")), "5985af7bee6133b2ddd377433d3831147a66f835");
    // a path matches without regard to case
    CB_CHECK_EQ(sha1_hex(disc.read("B.txt")), "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d");
    CB_CHECK(!disc.try_read("missing.bin").has_value());
    CB_CHECK_THROWS_AS(NotFoundError, (void)disc.read("missing.bin"));
});

CB_TEST("formats.xbox.gdf.the_volume_is_found_at_the_usual_bases_and_at_a_given_one", [] {
    const Bytes image = four_file_image();
    // 64 KB in, as some images start
    Bytes shifted(0x10000, 0xAA);
    append(shifted, image);
    gdf::Disc disc = gdf::Disc::open(std::make_shared<MemoryDevice>(shifted));
    CB_CHECK_EQ(disc.base(), 0x10000);
    CB_CHECK_EQ(sha1_hex(disc.read("a.bin")), "f48a95799d144d5ad254355f1849533120dbec12");

    // a base before the device's start: a package whose first 4 KB are not in its parts
    Bytes cut(image.begin() + 0x1000, image.end());
    gdf::Disc negative = gdf::Disc::open(std::make_shared<MemoryDevice>(cut), -0x1000, 0xF000);
    CB_CHECK_EQ(negative.base(), -0x1000);
    CB_CHECK_EQ(sha1_hex(negative.read("B.TXT")), "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d");
});

CB_TEST("formats.xbox.gdf.a_device_without_a_volume_is_a_format_error", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)gdf::Disc::open(std::make_shared<MemoryDevice>(Bytes{})));
    CB_CHECK_THROWS_AS(FormatError, (void)gdf::Disc::open(std::make_shared<MemoryDevice>(Bytes(0x20000, 0x55))));
    // a given base that holds no descriptor
    CB_CHECK_THROWS_AS(FormatError, (void)gdf::Disc::open(std::make_shared<MemoryDevice>(four_file_image()), 0x4000));
    // a negative position before the device
    CB_CHECK_THROWS_AS(FormatError, (void)gdf::Disc::open(std::make_shared<MemoryDevice>(four_file_image()), -0x100000));
});

CB_TEST("formats.xbox.gdf.damaged_directories_end_the_walk", [] {
    Bytes image = four_file_image();
    // the root's first entry points its left subtree at itself and its right at an entry beyond the sector: the walk ends
    const std::size_t root = 33 * 0x800;
    image[root] = 0;
    image[root + 1] = 0;
    image[root + 2] = 0xFF;
    image[root + 3] = 0x7F;
    gdf::Disc a = gdf::Disc::open(std::make_shared<MemoryDevice>(image));
    CB_CHECK(a.entries().size() <= 4u);

    // a folder that is its own child is entered once: make the first entry a folder whose sector is the root's
    Bytes loop = four_file_image();
    loop[root + 0x0C] = 0x10;                                  // a directory
    for (int k = 0; k < 4; ++k) loop[root + 4 + k] = static_cast<std::uint8_t>((33 >> (8 * k)) & 0xFF);   // starting at the root itself
    loop[root + 8] = 92;
    gdf::Disc b = gdf::Disc::open(std::make_shared<MemoryDevice>(loop));
    CB_CHECK(b.entries().size() <= 8u);

    // a directory bigger than a megabyte is damage and lists as empty
    Bytes huge = four_file_image();
    const std::size_t vd = 32 * 0x800;
    huge[vd + 0x18] = 0;
    huge[vd + 0x19] = 0;
    huge[vd + 0x1A] = 0x20;   // 2 MB
    huge[vd + 0x1B] = 0;
    CB_CHECK(gdf::Disc::open(std::make_shared<MemoryDevice>(huge)).entries().empty());
});

CB_TEST("formats.xbox.gdf.build_refuses_names_it_cannot_hold", [] {
    const gdf::SourceFile unicode{"\xE2\x82\xAC.bin", {}};   // the euro sign is not Latin-1
    CB_CHECK_THROWS_AS(UsageError, (void)gdf::build(std::span<const gdf::SourceFile>(&unicode, 1)));
    const gdf::SourceFile empty{"", {}};
    CB_CHECK_THROWS_AS(UsageError, (void)gdf::build(std::span<const gdf::SourceFile>(&empty, 1)));
    // no files: a volume with an empty root
    const Bytes none = gdf::build({});
    gdf::Disc disc = gdf::Disc::open(std::make_shared<MemoryDevice>(none));
    CB_CHECK(disc.entries().empty());
});

CB_TEST("formats.xbox.gdf.the_command_lists_and_extracts", [] {
    fs::ScopedTempDir dir("cb-gdf");
    const auto iso = dir.path() / "disc.iso";
    fs::write_file(iso, four_file_image());
    const std::vector<std::string> list = {"list", fs::to_text(iso)};
    testing::CapturedStd listing;
    CB_CHECK_EQ(gdf::command(list), 0);
    CB_CHECK_EQ(listing.out(), "          2560  a.bin\n             5  B.TXT\n          5004  default.xex\n          4050  Zed.txt\ntotal: 4 files\n");

    const std::vector<std::string> info = {"info", fs::to_text(iso)};
    testing::CapturedStd report;
    CB_CHECK_EQ(gdf::command(info), 0);
    CB_CHECK_EQ(report.out(), "volume base : 0x0\nroot sector : 33  size 0x5C\nfiles       : 4, 11619 bytes total\n");

    const auto out = dir.path() / "got.txt";
    const std::vector<std::string> extract = {"extract", fs::to_text(iso), "b.txt", fs::to_text(out)};
    testing::CapturedStd done;
    CB_CHECK_EQ(gdf::command(extract), 0);
    CB_CHECK_EQ(fs::read_file(out).size(), 5u);
    const std::vector<std::string> bad = {"explode", fs::to_text(iso)};
    testing::CapturedStd refused;
    CB_CHECK_EQ(gdf::command(bad), 2);
});

}  // namespace gdf_test
