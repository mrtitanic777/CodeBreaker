// Tests of cb/formats/xbox/zar.hpp. The compressed archive below is the output of a reference writer (zstd level 6) for a
// folder of four files: b.bin (768 bytes), Z-empty, data/a.txt (78000 bytes of repeated text,
// two blocks) and data/sub/Deep.TXT (9 bytes); the reader lists them as below and reads the SHA-1s checked here. Reading it
// needs libzstd (found as zar::load_zstd does, or at the Git for Windows copy); where there is none, those checks are skipped. The
// structure checks run on archives made by hand here, with raw (stored) blocks and a stand-in codec for the compressed kind.
//
// Behaviour pinned here: the archive's SHA-256 is not checked on open (verifying integrity is a call of its
// own); a file tree that leads back into a folder is FormatError; reading a file
// that runs past the archive's data is FormatError.
#include "cb/formats/xbox/zar.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/volumes.hpp"

namespace zar_test {

using namespace cb;
using namespace cb::formats::xbox;

static std::string sha1_hex(ByteView v) { return crypto::to_hex(crypto::sha1(v)); }

static Bytes golden_archive() {
    return crypto::from_hex(
        "28b52ffd6000ff1d0a004413000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d"
        "2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f6061626364656667"
        "68696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1"
        "a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadb"
        "dcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff5a41726368697665207465737420746578742c207265"
        "70656174656420736f207468617420697420636f6d707265737365732e20020094ccbfaa4300fd03ea9a0228b52ffd6000ff650200e40320736f"
        "207468617420697420636f6d707265737365732e205a41726368697665207465737420746578742c207265706561746564646565702066696c65"
        "000200434c9c8e286f5e5e410100000000000000000000000000014c005500000000000000000000000000000000000000000000000000000000"
        "05622e62696e0464617461075a2d656d70747905612e7478740373756208446565702e5458547fffffff00000001000000030000000080000000"
        "000000000000030000000000000000060000000400000002000000008000000b000133b900000000000000008000001300000300000130b00000"
        "0000000000190000000600000001000000008000001d000133b00000000900000000000000000000000000000000000001a300000000000001a8"
        "000000000000002800000000000001d0000000000000002600000000000001f60000000000000070000000000000026600000000000000000000"
        "00000000026600000000000000006f7498e560fe969159cde202c56a84fb707013a1823c6215420eb369eee36ebe00000000000002f661bf3a01"
        "169f52d6");
}

// libzstd where there is one: through zar::load_zstd, else the copy Git for Windows ships; null when neither loads
static std::shared_ptr<zar::ZstdCodec> zstd_if_there() {
    if (auto codec = zar::load_zstd()) return codec;
    const std::filesystem::path git_copy = "C:/Program Files/Git/mingw64/bin/libzstd.dll";
    std::error_code ec;
    if (!std::filesystem::exists(git_copy, ec)) return nullptr;
    fs::ScopedEnv env("CODEBREAKER_ZSTD_DLL", fs::to_text(git_copy));
    return zar::load_zstd();
}

CB_TEST("formats.xbox.zar.the_recorded_archive_reads_back", [] {
    const std::shared_ptr<zar::ZstdCodec> codec = zstd_if_there();
    if (!codec) return;   // (no libzstd on this machine)
    fs::ScopedTempDir dir("cb-zar");
    const auto path = dir.path() / "t.zar";
    const Bytes bytes = golden_archive();
    CB_CHECK_EQ(bytes.size(), 758u);
    CB_CHECK_EQ(sha1_hex(bytes), "b2540dce2c8f2402ccabe46d3a71d57a40c1ec9d");
    fs::write_file(path, bytes);
    zar::Reader archive = zar::Reader::open(path, codec);
    const std::vector<zar::FileInfo> files = archive.files();
    CB_CHECK_EQ(files.size(), 4u);
    CB_CHECK_EQ(files[0].path, "b.bin");
    CB_CHECK_EQ(files[0].size, 768u);
    CB_CHECK_EQ(files[1].path, "Z-empty");
    CB_CHECK_EQ(files[1].size, 0u);
    CB_CHECK_EQ(files[2].path, "data/a.txt");
    CB_CHECK_EQ(files[2].size, 78000u);
    CB_CHECK_EQ(files[3].path, "data/sub/Deep.TXT");
    CB_CHECK((archive.directories() == std::vector<std::string>{"data", "data/sub"}));
    CB_CHECK_EQ(sha1_hex(archive.read("b.bin")), "ac2a264c8ec1f4232a40854e8239bc3a697ab1d2");
    CB_CHECK(archive.read("Z-empty").empty());
    CB_CHECK_EQ(sha1_hex(archive.read("data/a.txt")), "a050de40365c5b30eaccd4dcef61422d2971f857");
    CB_CHECK_EQ(sha1_hex(archive.read("data/sub/Deep.TXT")), "399a02ad01953e83736e517b676f1a1f3ad56927");
    // names compare without regard to ASCII case, and '\' separates as '/' does
    CB_CHECK_EQ(sha1_hex(archive.read("DATA\\SUB/deep.txt")), "399a02ad01953e83736e517b676f1a1f3ad56927");
    CB_CHECK_EQ(archive.size("data/a.txt"), 78000u);
    CB_CHECK_THROWS_AS(NotFoundError, (void)archive.read("data"));          // a folder is not a file
    CB_CHECK_THROWS_AS(NotFoundError, (void)archive.read("data/missing"));
    CB_CHECK_THROWS_AS(NotFoundError, (void)archive.read("b.bin/inside"));  // a file has no children

});

CB_TEST("formats.xbox.zar.as_a_volume", [] {
    if (!zstd_if_there()) return;
    fs::ScopedTempDir dir("cb-zar");
    fs::write_file(dir.path() / "t.zar", golden_archive());
    // the volume loads libzstd as zar::load_zstd does: through the environment variable where it is not beside the program
    std::optional<fs::ScopedEnv> env;
    if (!zar::load_zstd()) env.emplace("CODEBREAKER_ZSTD_DLL", "C:/Program Files/Git/mingw64/bin/libzstd.dll");
    std::optional<volumes::Volumes> found = volumes::open(dir.path() / "t.zar");
    CB_CHECK(found.has_value());
    formats::media::Volume& v = *(*found)[0];
    CB_CHECK(v.kind() == formats::media::VolumeKind::Zar);
    CB_CHECK_EQ(v.label(), "t.zar");
    CB_CHECK_EQ(v.where(), "");
    CB_CHECK_EQ(v.boot().size(), 1u);
    CB_CHECK_EQ(v.boot()[0].name, "files");
    CB_CHECK(v.boot()[0].number == 4);
    CB_CHECK_EQ(v.files().size(), 4u);
    CB_CHECK_EQ(v.files()[2].path, "data/a.txt");
    CB_CHECK_EQ(v.files()[2].size, 78000u);
    CB_CHECK_EQ(sha1_hex(v.read(v.files()[2])), "a050de40365c5b30eaccd4dcef61422d2971f857");
});

// ---- archives made by hand ---------------------------------------------------------------------------------------------------

struct HandMade {
    Bytes data;                         // the compressed-data section
    std::vector<std::uint64_t> sizes;   // each block's stored size
    Bytes names;
    Bytes tree;
};

static void put_be(Bytes& b, std::uint64_t v, int bytes) {
    for (int i = bytes - 1; i >= 0; --i) b.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
}

static std::uint32_t add_name(HandMade& m, std::string_view name) {
    const auto at = static_cast<std::uint32_t>(m.names.size());
    if (name.size() < 0x80) {
        m.names.push_back(static_cast<std::uint8_t>(name.size()));
    } else {   // the 2-byte header
        m.names.push_back(static_cast<std::uint8_t>(0x80 | (name.size() & 0x7F)));
        m.names.push_back(static_cast<std::uint8_t>(name.size() >> 7));
    }
    append(m.names, as_bytes(name));
    return at;
}

static void add_entry(HandMade& m, std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d) {
    for (const std::uint32_t w : {a, b, c, d}) put_be(m.tree, w, 4);
}

static Bytes assemble(const HandMade& m, std::uint64_t claimed_total = 0) {
    Bytes out = m.data;
    while (out.size() % 8 != 0) out.push_back(0);
    const std::uint64_t records_at = out.size();
    for (std::size_t i = 0; i < m.sizes.size(); i += 16) {   // a record per 16 blocks
        std::uint64_t base = 0;
        for (std::size_t k = 0; k < i; ++k) base += m.sizes[k];
        put_be(out, base, 8);
        for (std::size_t k = 0; k < 16; ++k) put_be(out, i + k < m.sizes.size() ? m.sizes[i + k] - 1 : 0, 2);
    }
    const std::uint64_t records_size = out.size() - records_at, names_at = out.size();
    append(out, m.names);
    const std::uint64_t tree_at = out.size();
    append(out, m.tree);
    const std::uint64_t total = out.size() + 144;
    put_be(out, 0, 8), put_be(out, m.data.size(), 8);                 // compressed data
    put_be(out, records_at, 8), put_be(out, records_size, 8);         // offset records
    put_be(out, names_at, 8), put_be(out, m.names.size(), 8);         // names
    put_be(out, tree_at, 8), put_be(out, m.tree.size(), 8);           // tree
    put_be(out, 0, 8), put_be(out, 0, 8);                             // meta directory
    put_be(out, 0, 8), put_be(out, 0, 8);                             // meta data
    out.resize(out.size() + 32, 0);                                   // the integrity hash
    put_be(out, claimed_total ? claimed_total : total, 8);
    put_be(out, 0x61BF3A01, 4);
    put_be(out, 0x169F52D6, 4);
    return out;
}

// root { Dir { inner.TXT (file: first 10 bytes of block 0), ...}, top.bin (file: 70000 bytes from 100: two blocks) }
static Bytes hand_made(std::uint64_t claimed_total = 0, bool loop = false) {
    HandMade m;
    Bytes block0(zar::kBlockSize);
    for (std::size_t i = 0; i < block0.size(); ++i) block0[i] = static_cast<std::uint8_t>(i * 7);
    append(m.data, block0);
    m.sizes.push_back(zar::kBlockSize);                         // stored raw
    append(m.data, Bytes{1, 2, 3});                             // a "compressed" block of 3 bytes
    m.sizes.push_back(3);
    const std::uint32_t dir = add_name(m, "Dir"), inner = add_name(m, "inner.TXT"), top = add_name(m, "top.bin");
    const std::string long_name(200, 'n');                      // a name that needs the 2-byte header
    const std::uint32_t big = add_name(m, long_name);
    add_entry(m, 0x7FFFFFFF, 1, 2, 0);                          // the root: children 1..2
    add_entry(m, dir, loop ? 1u : 3u, 2, 0);                    // Dir: children 3..4 (or itself)
    add_entry(m, 0x80000000u | top, 100, 70000, 0);             // top.bin
    add_entry(m, 0x80000000u | inner, 0, 10, 0);                // Dir/inner.TXT
    add_entry(m, 0x80000000u | big, 5, 5, 0);                   // Dir/nnnn...
    return assemble(m, claimed_total);
}

// stands in for zstd: every frame decodes to the same 64 KiB of 0xEE
class FakeZstd final : public zar::ZstdCodec {
public:
    int calls = 0;
    Bytes decompress(ByteView frame, std::size_t expected) override {
        ++calls;
        if (frame.size() != 3) throw FormatError("fake", 0, "a frame of the wrong size");
        return Bytes(expected, 0xEE);
    }
};

CB_TEST("formats.xbox.zar.a_hand_made_archive_with_raw_and_stand_in_blocks", [] {
    fs::ScopedTempDir dir("cb-zar");
    fs::write_file(dir.path() / "h.zar", hand_made());
    auto codec = std::make_shared<FakeZstd>();
    zar::Reader archive = zar::Reader::open(dir.path() / "h.zar", codec);
    // a folder's own children first, then each subfolder's (Dir and top.bin, then what is in Dir)
    const std::vector<zar::FileInfo> files = archive.files();
    CB_CHECK_EQ(files.size(), 3u);
    CB_CHECK_EQ(files[0].path, "top.bin");
    CB_CHECK_EQ(files[0].size, 70000u);
    CB_CHECK_EQ(files[1].path, "Dir/inner.TXT");
    CB_CHECK_EQ(files[2].path, "Dir/" + std::string(200, 'n'));   // (the 2-byte name header)
    CB_CHECK((archive.directories() == std::vector<std::string>{"Dir"}));
    const Bytes inner = archive.read("dir/INNER.txt");
    CB_CHECK_EQ(inner.size(), 10u);
    for (std::size_t i = 0; i < 10; ++i) CB_CHECK_EQ(inner[i], static_cast<std::uint8_t>(i * 7));
    CB_CHECK_EQ(codec->calls, 0);   // the first block is stored raw
    // a file across the two blocks: bytes 100.. of block 0, then 0xEE from the stand-in's block
    const Bytes top = archive.read("top.bin");
    CB_CHECK_EQ(top.size(), 70000u);
    CB_CHECK_EQ(top[0], static_cast<std::uint8_t>(100 * 7));
    CB_CHECK_EQ(top[zar::kBlockSize - 100 - 1], static_cast<std::uint8_t>((zar::kBlockSize - 1) * 7));
    CB_CHECK_EQ(top[zar::kBlockSize - 100], 0xEE);
    CB_CHECK_EQ(top.back(), 0xEE);
    CB_CHECK_EQ(codec->calls, 1);
    (void)archive.read("top.bin");
    CB_CHECK_EQ(codec->calls, 1);   // the block is kept
    CB_CHECK_EQ(archive.read("Dir/" + std::string(200, 'n')).size(), 5u);
});

CB_TEST("formats.xbox.zar.a_codec_that_refuses_a_frame_is_reported", [] {
    fs::ScopedTempDir dir("cb-zar");
    fs::write_file(dir.path() / "h.zar", hand_made());
    // a codec that refuses every frame (a null codec would mean "try to load libzstd", which may succeed)
    class Refusing final : public zar::ZstdCodec {
    public:
        Bytes decompress(ByteView, std::size_t) override { throw FormatError("zstd", 0, "damaged frame"); }
    };
    zar::Reader archive = zar::Reader::open(dir.path() / "h.zar", std::make_shared<Refusing>());
    (void)archive.read("dir/inner.txt");                       // raw blocks need no codec
    CB_CHECK_THROWS_AS(FormatError, (void)archive.read("top.bin"));   // a damaged frame is reported as it is
});

CB_TEST("formats.xbox.zar.damaged_archives_are_format_errors", [] {
    fs::ScopedTempDir dir("cb-zar");
    const auto at = [&](const char* name) { return dir.path() / name; };
    fs::write_file(at("tiny.zar"), Bytes(100, 0));
    CB_CHECK_THROWS_AS(FormatError, (void)zar::Reader::open(at("tiny.zar")));
    Bytes zeros(500, 0);
    fs::write_file(at("zeros.zar"), zeros);
    CB_CHECK_THROWS_AS(FormatError, (void)zar::Reader::open(at("zeros.zar")));            // no magic

    Bytes good = hand_made();
    Bytes wrong_total = hand_made(good.size() + 1);
    fs::write_file(at("total.zar"), wrong_total);
    CB_CHECK_THROWS_AS(FormatError, (void)zar::Reader::open(at("total.zar")));

    Bytes version = good;
    version[version.size() - 5] ^= 1;   // the version's low byte
    fs::write_file(at("version.zar"), version);
    CB_CHECK_THROWS_AS(UnsupportedError, (void)zar::Reader::open(at("version.zar")));

    Bytes magic = good;
    magic[magic.size() - 1] ^= 1;
    fs::write_file(at("magic.zar"), magic);
    CB_CHECK_THROWS_AS(FormatError, (void)zar::Reader::open(at("magic.zar")));

    // a folder that is its own child ends the walk instead of running for ever
    fs::write_file(at("loop.zar"), hand_made(0, true));
    zar::Reader loop = zar::Reader::open(at("loop.zar"));
    CB_CHECK_THROWS_AS(FormatError, (void)loop.files());

    // a file that claims more than the archive holds
    Bytes huge = good;
    const std::size_t tree_at = huge.size() - 144 - 5 * 16;
    huge[tree_at + 2 * 16 + 8] = 0x7F;   // top.bin's size, top byte of the low word
    fs::write_file(at("huge.zar"), huge);
    zar::Reader big = zar::Reader::open(at("huge.zar"), std::make_shared<FakeZstd>());
    CB_CHECK_THROWS_AS(FormatError, (void)big.read("top.bin"));

    CB_CHECK_THROWS_AS(NotFoundError, (void)zar::Reader::open(at("missing.zar")));
});

}  // namespace zar_test
