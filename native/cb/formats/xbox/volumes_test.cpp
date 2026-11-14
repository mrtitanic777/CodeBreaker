// Tests of cb/formats/xbox/volumes.hpp. The expected facts (kinds, labels, where, boot fields, file lists, hashes of what is read) are
// the recorded answers for the same inputs: the real Minecraft package from
// C:/Nexia360/Games when it is there (the test is silent when it is not), the FATX, Heavy Iron and ISO fixtures of formats/testdata, and
// containers made here from the XDVDFS volume of gdf_test (a plain image, CSO, CCI, a Games-on-Demand package written by god::write_god
// in both layouts, a package of bare parts, a flash, a PCK and XZP).
//
// Behaviour pinned here: a damaged container is a cb::Error; the `.ho` / FATX probes that meet a damaged or unreadable
// file end on IoError / FormatError; the header of a package whose
// parts hold the whole disc (the volume at 0 and its descriptor at 0x10000) is a FormatError.
#include "cb/formats/xbox/volumes.hpp"

#include <algorithm>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/media.hpp"
#include "cb/formats/xbox/ciso.hpp"
#include "cb/formats/xbox/gdf.hpp"
#include "cb/formats/xbox/god.hpp"

namespace volumes_test {

using namespace cb;
using namespace cb::formats;
namespace stdfs = std::filesystem;
using xbox::volumes::Volumes;

static std::string sha1_hex(ByteView v) { return crypto::to_hex(crypto::sha1(v)); }

static stdfs::path fixture(std::string_view name) { return testing::data_path(std::string("formats/testdata/") + std::string(name)); }

static Bytes the_volume() {
    Bytes xex = {'X', 'E', 'X', '2'};
    xex.resize(5004, 0);
    Bytes a(2560);
    for (std::size_t i = 0; i < a.size(); ++i) a[i] = static_cast<std::uint8_t>(i);
    Bytes text;
    for (int i = 0; i < 90; ++i) append(text, as_bytes("The quick brown fox jumps over the lazy dog. "));
    const std::vector<xbox::gdf::SourceFile> files = {{"default.xex", xex}, {"B.TXT", to_bytes("hello")}, {"a.bin", a}, {"Zed.txt", text}};
    return xbox::gdf::build(files);
}

// "path:size" of each file, in order
static std::vector<std::string> listing(media::Volume& v) {
    std::vector<std::string> out;
    for (const media::Entry& e : v.files()) out.push_back(e.path + ":" + std::to_string(e.size));
    return out;
}

static const std::vector<std::string> kFourFiles = {"a.bin:2560", "B.TXT:5", "default.xex:5004", "Zed.txt:4050"};

static std::string boot_text(const media::Volume& v) {
    std::string out;
    for (const media::BootField& f : v.boot()) out += (out.empty() ? "" : "; ") + f.name + "=" + f.text;
    return out;
}

CB_TEST("formats.xbox.volumes.disc_images_plain_and_compressed", [] {
    fs::ScopedTempDir dir("cb-xbvol");
    const auto iso = dir.path() / "disc.iso";
    fs::write_file(iso, the_volume());
    const auto cso = dir.path() / "disc.cso";
    const auto cci = dir.path() / "disc.cci";
    (void)xbox::ciso::write_cso(iso, cso);
    (void)xbox::ciso::write_cci(iso, cci);
    const struct {
        stdfs::path path;
        const char* label;
        const char* where;
    } cases[] = {{iso, "disc.iso", "disc image"}, {cso, "disc.cso", "CISO compressed disc"}, {cci, "disc.cci", "CCIM compressed disc"}};
    for (const auto& c : cases) {
        std::optional<Volumes> found = xbox::volumes::open(c.path);
        CB_CHECK(found.has_value());
        CB_CHECK_EQ(found->size(), 1u);
        media::Volume& v = *(*found)[0];
        CB_CHECK(v.kind() == media::VolumeKind::Xdvdfs);
        CB_CHECK_EQ(media::kind_name(v.kind()), "xdvdfs");
        CB_CHECK_EQ(v.label(), c.label);
        CB_CHECK_EQ(v.where(), c.where);
        CB_CHECK_EQ(boot_text(v), "volume at=0x0");
        CB_CHECK(listing(v) == kFourFiles);
        CB_CHECK_EQ(sha1_hex(v.read(v.files()[3])), "5985af7bee6133b2ddd377433d3831147a66f835");
        // `media::volumes` reads them the same way (the Xbox containers come first there)
        std::vector<std::unique_ptr<media::Volume>> general = media::volumes(c.path);
        CB_CHECK(general[0]->kind() == media::VolumeKind::Xdvdfs);
        CB_CHECK_EQ(general[0]->label(), c.label);
    }
});

CB_TEST("formats.xbox.volumes.games_on_demand_in_both_layouts", [] {
    fs::ScopedTempDir dir("cb-xbvol");
    const Bytes tpl = fs::read_file(fixture("god/empty_live.bin"));
    const Bytes disc = the_volume();

    // the parts hold the whole disc image: the folder is read with the volume found at 0
    const xbox::god::Title title{.title_id = 0x584111F7, .media_id = 0x12345678, .title = "Test Title"};
    const auto whole = dir.path() / "whole";
    const xbox::god::WriteResult a = xbox::god::write_god(
        [&](std::uint64_t off, std::size_t n) {
            if (off >= disc.size()) return Bytes{};
            return Bytes(disc.begin() + static_cast<std::ptrdiff_t>(off), disc.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(disc.size(), off + n)));
        },
        disc.size(), whole, tpl, title);
    CB_CHECK_EQ(sha1_hex(fs::read_file(a.header)), "358ff2175eec9b4f6f7f50f1cc69797f7fc8f77a");
    CB_CHECK_EQ(sha1_hex(fs::read_file(a.data_dir / "Data0000")), "56c52910673b437e593040cd5a42655427ed56db");
    for (const stdfs::path& where : {a.data_dir, a.header.parent_path()}) {
        std::optional<Volumes> found = xbox::volumes::open(where);
        CB_CHECK(found.has_value());
        media::Volume& v = *(*found)[0];
        CB_CHECK(v.kind() == media::VolumeKind::God);
        CB_CHECK_EQ(v.label(), "00007000");   // the folder the .data folder is in
        CB_CHECK_EQ(v.where(), "Games on Demand");
        CB_CHECK_EQ(boot_text(v), "volume at=0x0");
        CB_CHECK(listing(v) == kFourFiles);
        CB_CHECK_EQ(sha1_hex(v.read(v.files()[3])), "5985af7bee6133b2ddd377433d3831147a66f835");
    }
    // its header says nothing of where the descriptor is, so the header alone cannot place a volume that starts at 0
    CB_CHECK_THROWS_AS(FormatError, (void)xbox::volumes::open(a.header));

    // the parts start at the volume descriptor (the first 0x10000 bytes of the disc are not in them): the header says 16 blocks of 4 KB
    const auto tail_only = dir.path() / "tail";
    const xbox::god::WriteResult b = xbox::god::write_god(
        [&](std::uint64_t off, std::size_t n) {
            const std::uint64_t at = 0x10000 + off;
            if (at >= disc.size()) return Bytes{};
            return Bytes(disc.begin() + static_cast<std::ptrdiff_t>(at), disc.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(disc.size(), at + n)));
        },
        disc.size() - 0x10000, tail_only, tpl, title);
    Bytes header = fs::read_file(b.header);
    header[0x395] = 16;
    fs::write_file(b.header, header);
    CB_CHECK_EQ(sha1_hex(fs::read_file(b.data_dir / "Data0000")), "f65b0c91df8ed1706634e6ecaf43dff71615a293");
    CB_CHECK_EQ(sha1_hex(header), "ebef54d5522c0e6adc47cda48c205a60dbb2f307");
    std::optional<Volumes> found = xbox::volumes::open(b.header);
    CB_CHECK(found.has_value());
    media::Volume& v = *(*found)[0];
    CB_CHECK(v.kind() == media::VolumeKind::God);
    CB_CHECK_EQ(boot_text(v), "volume at=0x-10000");
    CB_CHECK(listing(v) == kFourFiles);
    CB_CHECK_EQ(sha1_hex(v.read(v.files()[3])), "5985af7bee6133b2ddd377433d3831147a66f835");
});

CB_TEST("formats.xbox.volumes.a_package_of_bare_parts", [] {
    fs::ScopedTempDir dir("cb-xbvol");
    const Bytes tpl = fs::read_file(fixture("god/empty_live.bin"));
    const auto folder = dir.path() / "Vid.data";
    stdfs::create_directories(folder);
    fs::write_file(folder / "Data0000", Bytes(30, 'a'));
    fs::write_file(folder / "Data0001", to_bytes("zz"));
    Bytes header = tpl;
    header[0x3A9] = header[0x3AA] = header[0x3AB] = header[0x3AC] = 0;   // the volume is not a disc
    fs::write_file(dir.path() / "Vid", header);
    std::optional<Volumes> found = xbox::volumes::open(dir.path() / "Vid");
    CB_CHECK(found.has_value());
    media::Volume& v = *(*found)[0];
    CB_CHECK(v.kind() == media::VolumeKind::Parts);
    CB_CHECK_EQ(v.label(), "Vid.data");
    CB_CHECK_EQ(v.where(), "");
    CB_CHECK_EQ(boot_text(v), "parts=2");
    CB_CHECK(v.boot()[0].number == 2);
    CB_CHECK((listing(v) == std::vector<std::string>{"Data0000:30", "Data0001:2"}));
    CB_CHECK_EQ(as_text(v.read(v.files()[1])), "zz");
});

CB_TEST("formats.xbox.volumes.fatx_partitions", [] {
    struct Case {
        const char* file;
        const char* boot;
        std::size_t count;
        const char* first_hash;   // readme.txt
    };
    const Case cases[] = {
        {"fatx/a360.bin", "cluster=0x200 bytes; clusters=167; byte order=big (Xbox 360)", 29, "bc53b4bb71709ff7eafadf12dae917d293775bf1"},
        {"fatx/axbox.bin", "cluster=0x400 bytes; clusters=129; byte order=little (original Xbox)", 37, "39ed928d38e57f53cc6aaf1f72803eb0c49fd058"}};
    for (const Case& c : cases) {
        std::optional<Volumes> found = xbox::volumes::open(fixture(c.file));
        CB_CHECK(found.has_value());
        CB_CHECK_EQ(found->size(), 1u);
        media::Volume& v = *(*found)[0];
        CB_CHECK(v.kind() == media::VolumeKind::Fatx);
        CB_CHECK_EQ(v.label(), "Partition");
        CB_CHECK_EQ(v.where(), "partition at 0x0");
        CB_CHECK_EQ(boot_text(v), c.boot);
        const std::vector<media::Entry>& files = v.files();
        CB_CHECK_EQ(files.size(), c.count);
        CB_CHECK_EQ(files[0].path, "readme.txt");
        CB_CHECK_EQ(files[0].start, 5u);
        CB_CHECK_EQ(files[0].when, "2005-06-15 12:30");
        CB_CHECK_EQ(sha1_hex(v.read(files[0])), c.first_hash);
        CB_CHECK_EQ(files[1].path, "empty.bin");
        CB_CHECK(v.read(files[1]).empty());
        CB_CHECK_EQ(files[3].path, "UPPER.TXT");
        CB_CHECK_EQ(files[3].when, "2004-02-29 23:59");
        CB_CHECK_EQ(sha1_hex(v.read(files[3])), "406648bfa6a200691f2b663b3f05a41f9403f1d3");
        CB_CHECK_EQ(files[4].path, "sub");   // a folder before what is in it
        CB_CHECK(files[4].is_dir);
        CB_CHECK_EQ(files[4].when, "2010-01-02 03:04");
        CB_CHECK_EQ(files[5].path, "sub/a.xex");
        CB_CHECK(v.read(files[4]).empty());
    }
    // `media::volumes` reaches them too
    CB_CHECK(media::volumes(fixture("fatx/a360.bin"))[0]->kind() == media::VolumeKind::Fatx);
});

CB_TEST("formats.xbox.volumes.heavy_iron_archives", [] {
    {
        std::optional<Volumes> found = xbox::volumes::open(fixture("heavyiron/a2.ho"));
        CB_CHECK(found.has_value());
        media::Volume& v = *(*found)[0];
        CB_CHECK(v.kind() == media::VolumeKind::HeavyIron);
        CB_CHECK_EQ(v.label(), "a2.ho");
        CB_CHECK_EQ(boot_text(v), "assets=1");
        CB_CHECK((listing(v) == std::vector<std::string>{"layer00/1234567890ABCDEF_BDE21A8D.bin:77"}));
        Bytes all = to_bytes(v.files()[0].path);
        all.push_back(0);
        append(all, v.read(v.files()[0]));
        CB_CHECK_EQ(sha1_hex(all), "20f7591b44afca01ebd88afec62e51daef3f7b7d");
    }
    std::optional<Volumes> found = xbox::volumes::open(fixture("heavyiron/a5.ho"));
    media::Volume& v = *(*found)[0];
    CB_CHECK_EQ(boot_text(v), "assets=7");
    CB_CHECK_EQ(v.files()[0].path, "layer00/0000000000000064_BDE21A8D.bin");
    CB_CHECK_EQ(v.files()[0].size, 0u);
    CB_CHECK_EQ(v.files()[2].size, 63u);
    Bytes all;
    for (const media::Entry& e : v.files()) {
        append(all, to_bytes(e.path));
        all.push_back(0);
        append(all, v.read(e));
    }
    CB_CHECK_EQ(sha1_hex(all), "9e1795a54eb251452003ff3e71620d91314203d9");
});

CB_TEST("formats.xbox.volumes.the_stfs_package_of_minecraft", [] {
    const stdfs::path package = "C:/Nexia360/Games/Minecraft Xbox 360 Edition/584111F70AAAAAAA";
    std::error_code ec;
    if (!stdfs::exists(package, ec)) return;   // (the game is not on every machine)
    std::optional<Volumes> found = xbox::volumes::open(package);
    CB_CHECK(found.has_value());
    media::Volume& v = *(*found)[0];
    CB_CHECK(v.kind() == media::VolumeKind::Stfs);
    CB_CHECK_EQ(v.label(), "Minecraft");
    CB_CHECK_EQ(boot_text(v), "magic=PIRS; title id=584111F7; content type=0x30000; display name=Minecraft Wood Theme");
    const std::vector<media::Entry>& files = v.files();
    CB_CHECK_EQ(files.size(), 25u);
    std::uint64_t total = 0;
    for (const media::Entry& e : files) total += e.size;
    CB_CHECK_EQ(total, 2664112u);
    CB_CHECK_EQ(files[0].path, "BackgroundLayer1.png");
    CB_CHECK_EQ(files[0].size, 136134u);
    CB_CHECK_EQ(sha1_hex(v.read(files[0])), "5087f1576edc0255ac79c08cce1de8901a17be1f");
    CB_CHECK_EQ(sha1_hex(v.read(files[2])), "fea75813c016def862eae14acc7f8ebd443d8b4d");
    CB_CHECK_EQ(sha1_hex(v.read(files[5])), "e064cbd02ceecbb78fb801f1608ccd0a98c66c39");
    crypto::Hasher all(crypto::Hasher::Algorithm::Sha1);
    for (const media::Entry& e : files) all.update(v.read(e));
    CB_CHECK_EQ(crypto::to_hex(all.finish()), "954c13531f158454ad32da719865a24f967a441c");
});

CB_TEST("formats.xbox.volumes.pck_and_xzp_payloads", [] {
    fs::ScopedTempDir dir("cb-xbvol");
    const Bytes pck_file = crypto::from_hex(
        "000000030000000100000007000000030076006500720000000000000004000000210000000000000009007400650078002f0061002e0070006e0067"
        "000000000000002300000001000000070062005c0063002e006c006f0063000000000000000000000002000000050065006d007000740079000000"
        "000000002800000003000000050064002e0067007200660000000000000000000000000000000089504e470d0a1a0a494844520000000000000000"
        "000000000049454e4400000000000000004c4f43444154414c4f43444154414c4f43444154414c4f43444154414c4f434441544100000000000000"
        "000000000000000000000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f2021222324252627");
    fs::write_file(dir.path() / "x.pck", pck_file);
    std::optional<Volumes> p = xbox::volumes::open(dir.path() / "x.pck");
    CB_CHECK(p.has_value());
    media::Volume& pv = *(*p)[0];
    CB_CHECK(pv.kind() == media::VolumeKind::Pck);
    CB_CHECK_EQ(pv.label(), "x.pck");
    CB_CHECK_EQ(boot_text(pv), "files=3");
    CB_CHECK((listing(pv) == std::vector<std::string>{"tex_a.png:33", "b_c.loc:35", "d.grf:39"}));
    CB_CHECK_EQ(sha1_hex(pv.read(pv.files()[0])), "637f7e4314d99ae81cf1b1983f6537a7b146c0c9");

    const Bytes xzp_file = crypto::from_hex(
        "5855495a0000000100000086000000000000004b000000000011000000000b006d0065006400690061005c0061002e0070006e0067000000140000001111"
        "006d0065006400690061005c00640065002d00440045005c0073002e0078007500730089504e470d0a1a0a7979797979797979797465787474657874746578"
        "747465787474657874");
    fs::write_file(dir.path() / "v1.xzp", xzp_file);
    std::optional<Volumes> x = xbox::volumes::open(dir.path() / "v1.xzp");
    CB_CHECK(x.has_value());
    media::Volume& xv = *(*x)[0];
    CB_CHECK(xv.kind() == media::VolumeKind::Xzp);
    CB_CHECK_EQ(xv.label(), "v1.xzp");
    CB_CHECK_EQ(boot_text(xv), "files=2");
    CB_CHECK((listing(xv) == std::vector<std::string>{"media/a.png:17", "media/de-DE/s.xus:20"}));
    CB_CHECK_EQ(sha1_hex(xv.read(xv.files()[1])), "3a8521aec2ef9a44e4f0cd246fe1fb8445ce90f8");
});

CB_TEST("formats.xbox.volumes.a_flash_and_what_is_not_xbox", [] {
    fs::ScopedTempDir dir("cb-xbvol");
    // a 16 MB dump: the flash recipe's blocks first, the rest unused
    constexpr std::size_t kPageSpare = 0x210;
    Bytes dump((0x1000000 / kPageSpare + 1) * kPageSpare, 0xFF);
    dump[0] = 0xFF;
    dump[1] = 0x4F;
    const std::string_view notice = "(c) Microsoft Corporation";
    std::copy(notice.begin(), notice.end(), dump.begin() + 0x10);
    // block 5 is the root: its spare says so (type 0x30); the directory lists one file of one block that is not there to follow
    const std::size_t spare = 5 * 32 * kPageSpare + 0x200;
    dump[spare + 12] = 0x30;
    dump[spare + 6] = 1;
    // the allocation table (even pages) is zeros: one.bin of 100 bytes starts at block 1 and ends at once
    for (std::size_t i = 0; i < 32; i += 2) std::fill_n(dump.begin() + static_cast<std::ptrdiff_t>((5 * 32 + i) * kPageSpare), 0x200, std::uint8_t{0});
    std::fill_n(dump.begin() + static_cast<std::ptrdiff_t>((5 * 32 + 1) * kPageSpare), 32, std::uint8_t{0});   // the first directory entry
    dump[(5 * 32 + 1) * kPageSpare + 0] = 'o';
    dump[(5 * 32 + 1) * kPageSpare + 1] = 'n';
    dump[(5 * 32 + 1) * kPageSpare + 2] = 'e';
    dump[(5 * 32 + 1) * kPageSpare + 23] = 1;     // starts at block 1
    dump[(5 * 32 + 1) * kPageSpare + 27] = 100;   // 100 bytes
    // that block ends its chain
    dump[5 * 32 * kPageSpare + 2 * 1 + 1] = 0xFF;
    dump[5 * 32 * kPageSpare + 2 * 1] = 0x1F;
    fs::write_file(dir.path() / "flash.bin", dump);
    std::optional<Volumes> found = xbox::volumes::open(dir.path() / "flash.bin");
    CB_CHECK(found.has_value());
    media::Volume& v = *(*found)[0];
    CB_CHECK(v.kind() == media::VolumeKind::Nand);
    CB_CHECK_EQ(v.label(), "flash");
    CB_CHECK_EQ(v.where(), "root block 0x5");
    CB_CHECK_EQ(boot_text(v), "files=1");
    CB_CHECK((listing(v) == std::vector<std::string>{"one:100"}));
    CB_CHECK_EQ(v.read(v.files()[0]).size(), 100u);

    // other kinds of file are not Xbox containers
    CB_CHECK(!xbox::volumes::open(fixture("iso9660/mini_setup.iso")).has_value());
    fs::write_file(dir.path() / "text.txt", to_bytes("just some text"));
    CB_CHECK(!xbox::volumes::open(dir.path() / "text.txt").has_value());
    CB_CHECK(!xbox::volumes::open(dir.path()).has_value());   // a folder with no parts
    CB_CHECK_THROWS_AS(NotFoundError, (void)xbox::volumes::open(dir.path() / "missing.bin"));
    // and media::volumes still reads those that are not
    CB_CHECK(media::volumes(fixture("iso9660/mini_setup.iso"))[0]->kind() == media::VolumeKind::Iso9660);
    CB_CHECK_THROWS_AS(FormatError, (void)media::volumes(dir.path()));   // a folder that is not a package
    // a damaged package is an error, not "not a container"
    fs::write_file(dir.path() / "bad.pck", Bytes(6, 0));
    CB_CHECK_THROWS_AS(TruncatedError, (void)xbox::volumes::open(dir.path() / "bad.pck"));
});

}  // namespace volumes_test
