// Tests of cb/formats/xbox/ciso.hpp. The images this writes from the 88064-byte test volume are compared byte for byte with what the
// Python's ciso module writes (formats.dll's writers) for the same ISO: the CSO is 1964 bytes with SHA-1 8e95613e..., the CCI 1808 with
// ab76c999... (state/work/typed-port/xbox-deferred/vol_golden.py); reading them back gives the volume again, as the Python does.
//
// DIVERGES from the Python: a damaged LZ4 block, an index that does not fit the file and a block whose padding exceeds it are
// FormatError (CisoError / Lz4Error / IndexError); a size that the image's own header makes impossible is refused before an index of
// that size is read.
#include "cb/formats/xbox/ciso.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/gdf.hpp"

namespace ciso_test {

using namespace cb;
using namespace cb::formats::xbox;

static std::string sha1_hex(ByteView v) { return crypto::to_hex(crypto::sha1(v)); }

static Bytes the_volume() {
    Bytes xex = {'X', 'E', 'X', '2'};
    xex.resize(5004, 0);
    Bytes a(2560);
    for (std::size_t i = 0; i < a.size(); ++i) a[i] = static_cast<std::uint8_t>(i);
    Bytes text;
    for (int i = 0; i < 90; ++i) append(text, as_bytes("The quick brown fox jumps over the lazy dog. "));
    const std::vector<gdf::SourceFile> files = {{"default.xex", xex}, {"B.TXT", to_bytes("hello")}, {"a.bin", a}, {"Zed.txt", text}};
    return gdf::build(files);
}

CB_TEST("formats.xbox.ciso.cso_and_cci_are_the_pythons_bytes", [] {
    fs::ScopedTempDir dir("cb-ciso");
    const auto iso = dir.path() / "disc.iso";
    fs::write_file(iso, the_volume());

    const auto cso = dir.path() / "disc.cso";
    CB_CHECK_EQ(ciso::write_cso(iso, cso), 88064u);
    const Bytes cso_bytes = fs::read_file(cso);
    CB_CHECK_EQ(cso_bytes.size(), 1964u);
    CB_CHECK_EQ(sha1_hex(cso_bytes), "8e95613eb915c8bfa590b39ec30016187efb9b9a");

    const auto cci = dir.path() / "disc.cci";
    CB_CHECK_EQ(ciso::write_cci(iso, cci), 88064u);
    const Bytes cci_bytes = fs::read_file(cci);
    CB_CHECK_EQ(cci_bytes.size(), 1808u);
    CB_CHECK_EQ(sha1_hex(cci_bytes), "ab76c9999801d6d0bae94adc3a21574f76de87bf");
});

CB_TEST("formats.xbox.ciso.an_image_reads_as_the_disc_it_holds", [] {
    fs::ScopedTempDir dir("cb-ciso");
    const auto iso = dir.path() / "disc.iso";
    const Bytes volume = the_volume();
    fs::write_file(iso, volume);
    for (const auto kind : {ciso::Kind::Cso, ciso::Kind::Cci}) {
        const auto image_path = dir.path() / (kind == ciso::Kind::Cso ? "d.cso" : "d.cci");
        kind == ciso::Kind::Cso ? (void)ciso::write_cso(iso, image_path) : (void)ciso::write_cci(iso, image_path);
        CB_CHECK(ciso::kind_of(image_path) == kind);
        auto image = std::make_shared<ciso::Image>(ciso::Image::open(image_path));
        CB_CHECK(image->kind() == kind);
        CB_CHECK_EQ(image->size(), 88064u);
        CB_CHECK_EQ(image->block_count(), 43u);
        CB_CHECK_EQ(image->compressed_blocks(), 43u);
        // reads across blocks, at the end and past it
        CB_CHECK(image->read_at(2040, 20) == Bytes(volume.begin() + 2040, volume.begin() + 2060));
        CB_CHECK_EQ(image->read_at(88060, 100).size(), 4u);
        CB_CHECK(image->read_at(88064, 10).empty());
        gdf::Disc disc = gdf::Disc::open(image);
        CB_CHECK_EQ(disc.files().size(), 4u);
        CB_CHECK_EQ(sha1_hex(disc.read("Zed.txt")), "5985af7bee6133b2ddd377433d3831147a66f835");
    }
    // and back to the plain image
    const auto back = dir.path() / "back.iso";
    CB_CHECK_EQ(ciso::to_iso(dir.path() / "d.cso", back), 88064u);
    CB_CHECK(fs::read_file(back) == volume);
    CB_CHECK_EQ(ciso::to_iso(dir.path() / "d.cci", back), 88064u);
    CB_CHECK(fs::read_file(back) == volume);
});

CB_TEST("formats.xbox.ciso.lz4_blocks_round_trip", [] {
    for (const std::size_t n : {0u, 1u, 5u, 12u, 13u, 14u, 100u, 2048u, 5000u}) {
        Bytes plain(n);
        for (std::size_t i = 0; i < n; ++i) plain[i] = static_cast<std::uint8_t>((i / 7) * 3 + (i % 5 == 0 ? i : 0));
        const Bytes packed = ciso::lz4_compress_block(plain);
        CB_CHECK(ciso::lz4_decode_block(packed, n) == plain);
    }
    // incompressible data still round trips
    Bytes noise(3000);
    std::uint32_t x = 12345;
    for (auto& b : noise) {
        x = x * 1664525u + 1013904223u;
        b = static_cast<std::uint8_t>(x >> 24);
    }
    CB_CHECK(ciso::lz4_decode_block(ciso::lz4_compress_block(noise), noise.size()) == noise);
});

CB_TEST("formats.xbox.ciso.damaged_lz4_is_a_format_error", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::lz4_decode_block(Bytes{}, 10));
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::lz4_decode_block(Bytes{0x10}, 10));                    // a literal that is not there
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::lz4_decode_block(Bytes{0x11, 'a', 0, 0}, 10));        // a zero offset
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::lz4_decode_block(Bytes{0x11, 'a', 9, 0}, 10));        // a match before the start
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::lz4_decode_block(Bytes{0x10, 'a'}, 10));              // the wrong size
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::lz4_decode_block(Bytes{0x1F, 'a', 1, 0}, 3));         // longer than the size said
    CB_CHECK(ciso::lz4_decode_block(Bytes{}, 0).empty());
});

CB_TEST("formats.xbox.ciso.files_that_are_not_images_or_are_damaged", [] {
    fs::ScopedTempDir dir("cb-ciso");
    const auto plain = dir.path() / "plain.bin";
    fs::write_file(plain, Bytes(100, 7));
    CB_CHECK(!ciso::kind_of(plain).has_value());
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::Image::open(plain));
    CB_CHECK_THROWS_AS(NotFoundError, (void)ciso::Image::open(dir.path() / "missing.cso"));

    // a header that claims more blocks than the file could index
    Bytes head = to_bytes("CISO");
    head.resize(0x18, 0);
    head[4] = 0x18;
    head[8 + 7] = 0x7F;   // a size of 2^62
    head[16] = 0x00;
    head[17] = 0x08;      // 2048-byte blocks
    const auto huge = dir.path() / "huge.cso";
    fs::write_file(huge, head);
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::Image::open(huge));

    // a block size other than 2048
    Bytes wrong = head;
    wrong[8 + 7] = 0;
    wrong[16] = 0x00;
    wrong[17] = 0x10;
    const auto bad_block = dir.path() / "bad.cso";
    fs::write_file(bad_block, wrong);
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::Image::open(bad_block));

    // a header cut short
    const auto short_file = dir.path() / "short.cci";
    fs::write_file(short_file, to_bytes("CCIM"));
    CB_CHECK_THROWS_AS(FormatError, (void)ciso::Image::open(short_file));
});

CB_TEST("formats.xbox.ciso.the_command_converts_and_lists", [] {
    fs::ScopedTempDir dir("cb-ciso");
    const auto iso = dir.path() / "disc.iso";
    fs::write_file(iso, the_volume());
    const auto cso = dir.path() / "disc.cso";
    {
        testing::CapturedStd out;
        CB_CHECK_EQ(ciso::command(std::vector<std::string>{"tocso", fs::to_text(iso), fs::to_text(cso)}), 0);
        CB_CHECK_EQ(out.out(), "wrote CSO (0 MB uncompressed) -> " + fs::to_text(cso) + "\n");
    }
    {
        testing::CapturedStd out;
        CB_CHECK_EQ(ciso::command(std::vector<std::string>{"info", fs::to_text(cso)}), 0);
        CB_CHECK(out.out().find("uncompressed: 88064 bytes (43 blocks); 43 compressed, 0 raw; on-disk 1964 bytes") != std::string::npos);
    }
    {
        testing::CapturedStd out;
        CB_CHECK_EQ(ciso::command(std::vector<std::string>{"list", fs::to_text(cso)}), 0);
        CB_CHECK_EQ(out.out(), "          2560  a.bin\n             5  B.TXT\n          5004  default.xex\n          4050  Zed.txt\n");
    }
    {
        testing::CapturedStd out;
        const auto folder = dir.path() / "out";
        CB_CHECK_EQ(ciso::command(std::vector<std::string>{"extract", fs::to_text(cso), fs::to_text(folder)}), 0);
        CB_CHECK_EQ(fs::read_file(folder / "Zed.txt").size(), 4050u);
    }
    testing::CapturedStd out;
    CB_CHECK_EQ(ciso::command(std::vector<std::string>{"info"}), 2);
});

}  // namespace ciso_test
