#include "cb/formats/xbox/fatx.hpp"

#include <map>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::xbox::fatx {
namespace {

using J = nlohmann::json;
namespace stdfs = std::filesystem;

stdfs::path fixture(std::string_view name) { return testing::data_path(std::string("formats/testdata/fatx/") + std::string(name)); }

// ---- timestamps ----------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.fatx.timestamps", [] {
    const Timestamp t{2005, 6, 15, 12, 30, 20};
    const std::uint32_t packed = (25u << 25) | (6u << 21) | (15u << 16) | (12u << 11) | (30u << 5) | 10u;
    CB_CHECK_EQ(encode_timestamp(t, 1980), packed);
    CB_CHECK(decode_timestamp(packed, 1980) == t);
    CB_CHECK_EQ(decode_timestamp(packed, 1980)->to_string(), "2005-06-15 12:30");
    // the original Xbox counts from 2000
    CB_CHECK(decode_timestamp(packed, 2000)->year == 2025);
    CB_CHECK(!decode_timestamp(0, 1980).has_value());
    CB_CHECK(!decode_timestamp((25u << 25) | (13u << 21) | (1u << 16), 1980).has_value());       // month 13
    CB_CHECK(!decode_timestamp((25u << 25) | (0u << 21) | (1u << 16), 1980).has_value());        // month 0
    CB_CHECK(!decode_timestamp((25u << 25) | (2u << 21) | (30u << 16), 1980).has_value());       // 30 February
    CB_CHECK(decode_timestamp((24u << 25) | (2u << 21) | (29u << 16), 1980).has_value());        // 2004 is a leap year
    CB_CHECK(!decode_timestamp((25u << 25) | (2u << 21) | (29u << 16), 1980).has_value());       // 2005 is not
    CB_CHECK(!decode_timestamp((25u << 25) | (1u << 21) | (1u << 16) | (24u << 11), 1980).has_value());   // hour 24
    CB_CHECK(!decode_timestamp((25u << 25) | (1u << 21) | (1u << 16) | (60u << 5), 1980).has_value());    // minute 60
    // a second field of 31 is 62 seconds: capped at 59
    CB_CHECK_EQ(decode_timestamp((25u << 25) | (1u << 21) | (1u << 16) | 31u, 1980)->second, 59);
    // before the epoch cannot be written: the year is clamped to the epoch
    CB_CHECK_EQ(encode_timestamp(Timestamp{1970, 1, 1, 0, 0, 0}, 1980) >> 25, 0u);
});

CB_TEST("formats.xbox.fatx.name_characters", [] {
    for (char c : std::string("AZaz09 !#$%&'()-.@[]^_`{}~")) CB_CHECK(is_valid_name_byte(static_cast<std::uint8_t>(c)));
    CB_CHECK(is_valid_name_byte(0xFF));
    for (char c : std::string("\\/:*?\"<>|+,;=")) CB_CHECK(!is_valid_name_byte(static_cast<std::uint8_t>(c)));
    CB_CHECK(!is_valid_name_byte(0));
    CB_CHECK(!is_valid_name_byte(0x80));
});

// ---- two real images: an Xbox 360 (big-endian) and an original Xbox (little-endian) ----------------------------

struct Listed {
    std::string path;
    bool is_dir;
    std::uint32_t size;
    std::uint32_t crc;
};

std::vector<Listed> list_with_crcs(Volume& v, const Partition& p, const stdfs::path& scratch) {
    std::vector<Listed> out;
    const std::vector<Entry> tree = v.list_directory(p);
    auto walk = [&](auto&& self, const std::vector<Entry>& entries) -> void {
        for (const Entry& e : entries) {
            if (e.name == "nul") continue;   // (a Windows device name: not a file here; extract_all skips it)
            if (e.is_directory) {
                out.push_back({e.path, true, 0, 0});
            } else {
                const stdfs::path tmp = scratch / "one.bin";
                v.extract_file(p, e, tmp);
                out.push_back({e.path, false, e.file_size, crypto::crc32(fs::read_file(tmp))});
            }
            self(self, e.children);
        }
    };
    walk(walk, tree);
    std::sort(out.begin(), out.end(), [](const Listed& a, const Listed& b) { return a.path < b.path; });
    return out;
}

void check_image(const std::string& name) {
    const J golden = J::parse(fs::read_text(fixture("golden.json")))[name];
    Volume v(fixture(name + ".bin"));
    const std::vector<Partition> parts = v.detect_partitions();
    CB_CHECK_EQ(parts.size(), 1u);
    if (parts.empty()) return;
    const Partition& p = parts[0];
    // [name, offset, size, endian, epoch, sectors per cluster, bytes per cluster, root, clusters, fat16, fat offset, data offset, valid]
    const J pj = golden["partition"];
    CB_CHECK_EQ(p.name, pj[0].get<std::string>());
    CB_CHECK_EQ(p.offset, pj[1].get<std::uint64_t>());
    CB_CHECK_EQ(p.size, pj[2].get<std::uint64_t>());
    CB_CHECK(p.order == (pj[3].get<std::string>() == ">" ? ByteOrder::Big : ByteOrder::Little));
    CB_CHECK_EQ(p.epoch, pj[4].get<int>());
    CB_CHECK_EQ(p.sectors_per_cluster, pj[5].get<std::uint32_t>());
    CB_CHECK_EQ(p.bytes_per_cluster, pj[6].get<std::uint32_t>());
    CB_CHECK_EQ(p.root_cluster, pj[7].get<std::uint32_t>());
    CB_CHECK_EQ(p.max_clusters, pj[8].get<std::uint64_t>());
    CB_CHECK_EQ(p.fat16, pj[9].get<bool>());
    CB_CHECK_EQ(p.fat_offset, pj[10].get<std::uint64_t>());
    CB_CHECK_EQ(p.data_offset, pj[11].get<std::uint64_t>());

    fs::ScopedTempDir dir("fatx-test");
    const std::vector<Listed> got = list_with_crcs(v, p, dir.path());
    // the old reader's listing: rows [path ("/" after a folder), size, crc]; the first row ("./") is the root
    std::vector<Listed> want;
    for (const J& row : golden["list"][1]) {
        std::string path = row[0];
        if (path == "./") continue;
        const bool is_dir = path.ends_with('/');
        if (is_dir) path.pop_back();
        want.push_back({path, is_dir, is_dir ? 0u : row[1].get<std::uint32_t>(), is_dir ? 0u : row[2].get<std::uint32_t>()});
    }
    std::sort(want.begin(), want.end(), [](const Listed& a, const Listed& b) { return a.path < b.path; });
    CB_CHECK_EQ(got.size(), want.size());
    for (std::size_t i = 0; i < got.size() && i < want.size(); ++i) {
        if (got[i].path != want[i].path || got[i].is_dir != want[i].is_dir || got[i].size != want[i].size || got[i].crc != want[i].crc) {
            testing::record_failure(__FILE__, __LINE__, "entry " + got[i].path + " differs from the old reader's " + want[i].path);
        }
    }
    // the same through extract_all
    std::size_t files = 0;
    for (const Listed& l : want) files += !l.is_dir;
    CB_CHECK_EQ(v.extract_all(p, dir.path() / "all"), files);
    CB_CHECK_EQ(fs::read_file(dir.path() / "all" / "readme.txt").size(), want.empty() ? 0u : std::size_t{p.order == ByteOrder::Big ? 1553u : 3089u});
}

CB_TEST("formats.xbox.fatx.an_xbox_360_image", [] { check_image("a360"); });
CB_TEST("formats.xbox.fatx.an_original_xbox_image", [] { check_image("axbox"); });

CB_TEST("formats.xbox.fatx.names_and_attributes", [] {
    Volume v(fixture("a360.bin"));
    const Partition p = v.detect_partitions().front();
    const std::vector<Entry> root = v.list_directory(p);
    const auto find = [&](const std::string& n) { return std::find_if(root.begin(), root.end(), [&](const Entry& e) { return e.name == n; }); };
    CB_CHECK(find("UPPER.TXT") != root.end());
    CB_CHECK(find("hi\xEF\xBF\xBD") != root.end());                  // a byte that is not ASCII reads as U+FFFD
    // a 42-byte name that fills its field
    CB_CHECK(find("longer-than-length-field-name-xxxxxxxxxxxx") != root.end());
    const auto readme = find("readme.txt");
    CB_CHECK(readme != root.end());
    CB_CHECK_EQ(readme->attributes, 32);                          // archive
    CB_CHECK(!readme->is_directory);
    CB_CHECK(readme->created.has_value());
    CB_CHECK(*readme->created == (Timestamp{2005, 6, 15, 12, 30, 20}));
    CB_CHECK(*readme->modified == *readme->created);
    const auto many = find("many");
    CB_CHECK(many != root.end());
    CB_CHECK(many->is_directory);
    CB_CHECK_EQ(many->children.size(), 13u);
    CB_CHECK_EQ(many->children[0].path, "many/f000.bin");
    // a time field that is not a date reads as none, and the entry is still listed
    const auto deep = v.list_directory(p, 0, {}, false);
    const Entry* bad_time = nullptr;
    for (const Entry& e : deep) {
        if (e.name == "sub") {
            for (const Entry& c : e.children) {
                if (c.name == "deep") {
                    for (const Entry& f : c.children) {
                        if (f.name == "bad time") bad_time = &f;
                    }
                }
            }
        }
    }
    CB_CHECK(bad_time != nullptr);
    if (bad_time) CB_CHECK(!bad_time->modified.has_value() || !bad_time->created.has_value() || !bad_time->accessed.has_value());
});

CB_TEST("formats.xbox.fatx.clusters_and_chains", [] {
    Volume v(fixture("a360.bin"));
    const Partition p = v.detect_partitions().front();
    // one 512-byte cluster per file up to 512 bytes: readme.txt is 1553 bytes = 4 clusters
    const std::vector<Entry> root = v.list_directory(p);
    const auto readme = std::find_if(root.begin(), root.end(), [](const Entry& e) { return e.name == "readme.txt"; });
    const std::vector<std::uint32_t> chain = v.cluster_chain(p, readme->first_cluster);
    CB_CHECK_EQ(chain.size(), 4u);
    CB_CHECK_EQ(chain.front(), readme->first_cluster);
    CB_CHECK_EQ(v.cluster_offset(p, 1), p.data_offset);
    CB_CHECK_EQ(v.cluster_offset(p, 3), p.data_offset + 2 * p.bytes_per_cluster);
    CB_CHECK_EQ(v.read_cluster(p, readme->first_cluster).size(), p.bytes_per_cluster);
    // a cluster number outside the FAT is a chain of one; so is a free cluster
    CB_CHECK_EQ(v.cluster_chain(p, 99999).size(), 1u);
    CB_CHECK_EQ(v.cluster_chain(p, 0).size(), 1u);
    // reading past the end of the image gives what is there
    CB_CHECK(v.read_cluster(p, 100000).empty());
});

CB_TEST("formats.xbox.fatx.not_a_volume", [] {
    fs::ScopedTempDir dir("fatx-test");
    fs::write_file(dir.path() / "none.bin", Bytes(0x20000, 0));
    Volume v(dir.path() / "none.bin");
    CB_CHECK(v.detect_partitions().empty());
    fs::write_file(dir.path() / "tiny.bin", Bytes{'F', 'A', 'T', 'X'});
    CB_CHECK(Volume(dir.path() / "tiny.bin").detect_partitions().empty());
    // FATX magic with a sectors-per-cluster that is no power of two in either byte order
    Bytes odd(0x4000, 0);
    std::copy_n("FATX", 4, odd.begin());
    odd[11] = 3;
    fs::write_file(dir.path() / "odd.bin", odd);
    CB_CHECK(Volume(dir.path() / "odd.bin").detect_partitions().empty());
    CB_CHECK_THROWS_AS(NotFoundError, Volume(dir.path() / "missing.bin"));
});

// ---- a volume built here: the writer, recovery, the carver --------------------------------------------------------

// A blank big-endian FATX volume: 512-byte clusters (1 sector), FAT16, an empty root at cluster 1, `clusters` clusters.
Bytes blank_volume(std::uint32_t clusters, ByteOrder order = ByteOrder::Big) {
    const std::uint32_t data_offset = 0x2000;   // header page, then a one-page FAT (up to 2048 clusters)
    Bytes img(data_offset + std::size_t{clusters} * 512, 0);
    auto put32 = [&](std::size_t at, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) img[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (order == ByteOrder::Big ? 24 - 8 * i : 8 * i));
    };
    std::copy_n("FATX", 4, img.begin());
    put32(4, 0x12345678);   // serial
    put32(8, 1);            // sectors per cluster
    put32(12, 1);           // root cluster
    auto put16 = [&](std::size_t at, std::uint16_t v) {
        img[at] = static_cast<std::uint8_t>(v >> (order == ByteOrder::Big ? 8 : 0));
        img[at + 1] = static_cast<std::uint8_t>(v >> (order == ByteOrder::Big ? 0 : 8));
    };
    put16(0x1000, 0xFFF8);        // FAT[0]: the media descriptor
    put16(0x1002, 0xFFFF);        // FAT[1]: the root directory's only cluster
    std::fill(img.begin() + data_offset, img.begin() + data_offset + 512, std::uint8_t{0xFF});   // an empty root
    return img;
}

struct Scratch {
    fs::ScopedTempDir dir{"fatx-test"};
    stdfs::path image(const Bytes& data) const {
        fs::write_file(dir.path() / "v.bin", data);
        return dir.path() / "v.bin";
    }
};

CB_TEST("formats.xbox.fatx.add_a_file_and_read_it_back", [] {
    Scratch s;
    const stdfs::path img = s.image(blank_volume(100));
    Volume v(img);
    const Partition p = v.detect_partitions().at(0);
    CB_CHECK_EQ(p.max_clusters, 117u);   // (the partition is the whole file: the header and FAT pages count too)
    CB_CHECK(p.fat16);
    Bytes data(1300);
    for (std::size_t i = 0; i < data.size(); ++i) data[i] = static_cast<std::uint8_t>(i * 3);
    v.add_file(p, "hello.bin", data);
    const std::vector<Entry> root = v.list_directory(p);
    CB_CHECK_EQ(root.size(), 1u);
    CB_CHECK_EQ(root[0].name, "hello.bin");
    CB_CHECK_EQ(root[0].file_size, 1300u);
    CB_CHECK_EQ(root[0].attributes, 0x20);
    CB_CHECK_EQ(root[0].first_cluster, 2u);                 // the first free cluster (1 is the root)
    CB_CHECK(root[0].created == (Timestamp{2020, 1, 1, 0, 0, 0}));
    CB_CHECK((v.cluster_chain(p, 2) == std::vector<std::uint32_t>{2, 3, 4}));
    v.extract_file(p, root[0], s.dir.path() / "out.bin");
    CB_CHECK(fs::read_file(s.dir.path() / "out.bin") == data);
    // the bytes on the image: the dirent as FATX lays it out
    const Bytes dirent = v.read_at(p.data_offset, 0x40);
    CB_CHECK_EQ(dirent[0], 9);                              // name length
    CB_CHECK_EQ(dirent[1], 0x20);                           // archive
    CB_CHECK_EQ(std::string(as_text(ByteView(dirent).subspan(2, 9))), "hello.bin");
    CB_CHECK_EQ(dirent[11], 0xFF);                          // the name field is padded with 0xFF
    CB_CHECK_EQ(dirent[44], 0);                             // big-endian first cluster 2
    CB_CHECK_EQ(dirent[47], 2);
    CB_CHECK_EQ(dirent[50], 5);                             // size 1300 = 0x0514
    CB_CHECK_EQ(dirent[51], 0x14);
    // and in the FAT: 2 -> 3 -> 4 -> end (big-endian 16-bit)
    const Bytes fat = v.read_at(p.fat_offset, 12);
    CB_CHECK_EQ(fat[5], 3);
    CB_CHECK_EQ(fat[7], 4);
    CB_CHECK_EQ(fat[8], 0xFF);
    CB_CHECK_EQ(fat[9], 0xFF);
    // the volume still mounts after being written
    CB_CHECK_EQ(Volume(img).detect_partitions().size(), 1u);
});

CB_TEST("formats.xbox.fatx.directories_and_paths", [] {
    Scratch s;
    Volume v(s.image(blank_volume(200)));
    const Partition p = v.detect_partitions().at(0);
    v.mkdir(p, "games");
    v.mkdir(p, "games\\saves");
    v.add_file(p, "games/saves/a.sav", Bytes(10, 7));
    v.add_file(p, "GAMES\\readme", Bytes{1});                   // folder names match without regard to case
    const std::vector<Entry> root = v.list_directory(p);
    CB_CHECK_EQ(root.size(), 1u);
    CB_CHECK(root[0].is_directory);
    CB_CHECK_EQ(root[0].attributes, 0x10);
    CB_CHECK_EQ(root[0].children.size(), 2u);
    CB_CHECK_EQ(root[0].children[0].path, "games/saves");
    CB_CHECK_EQ(root[0].children[0].children.size(), 1u);
    CB_CHECK_EQ(root[0].children[0].children[0].path, "games/saves/a.sav");
    CB_CHECK_EQ(root[0].children[0].children[0].file_size, 10u);
    CB_CHECK_EQ(root[0].children[1].path, "games/readme");
    // errors
    CB_CHECK_THROWS_AS(NotFoundError, v.add_file(p, "nodir/x", Bytes{1}));
    CB_CHECK_THROWS_AS(NotFoundError, v.mkdir(p, "games/nope/inner"));
    CB_CHECK_THROWS_AS(UsageError, v.add_file(p, "caf\xC3\xA9", Bytes{1}));
    CB_CHECK_THROWS_AS(UsageError, v.add_file(p, "dir/", Bytes{1}));
    CB_CHECK_NOTHROW(v.add_file(p, "a_name_that_is_far_too_long_for_a_fatx_name_field_xxxx", Bytes{1}));   // cut at 42
    const auto names = v.list_directory(p);
    CB_CHECK(std::any_of(names.begin(), names.end(), [](const Entry& e) { return e.name == std::string("a_name_that_is_far_too_long_for_a_fatx_name_field_xxxx").substr(0, 42); }));
    for (const Entry& e : names) CB_CHECK(e.name.size() <= 42);
});

CB_TEST("formats.xbox.fatx.a_directory_that_fills_its_cluster_grows", [] {
    Scratch s;
    Volume v(s.image(blank_volume(300)));
    const Partition p = v.detect_partitions().at(0);
    // 512 / 64 = 8 entries a cluster
    for (int i = 0; i < 20; ++i) v.add_file(p, "f" + std::to_string(i), Bytes{static_cast<std::uint8_t>(i)});
    const std::vector<Entry> root = v.list_directory(p);
    CB_CHECK_EQ(root.size(), 20u);
    CB_CHECK_EQ(v.cluster_chain(p, p.root_cluster).size(), 3u);
    for (int i = 0; i < 20; ++i) {
        v.extract_file(p, root[static_cast<std::size_t>(i)], s.dir.path() / "o.bin");
        CB_CHECK((fs::read_file(s.dir.path() / "o.bin") == Bytes{static_cast<std::uint8_t>(i)}));
    }
});

CB_TEST("formats.xbox.fatx.remove_frees_the_chain_and_keeps_the_dirent_recoverable", [] {
    Scratch s;
    Volume v(s.image(blank_volume(100)));
    const Partition p = v.detect_partitions().at(0);
    const Bytes data(1500, 0x5A);
    v.add_file(p, "keep.bin", Bytes(600, 1));
    v.add_file(p, "gone.bin", data);
    const std::vector<std::uint32_t> before = v.cluster_chain(p, 4);   // gone.bin follows keep.bin's two clusters
    CB_CHECK_EQ(before.size(), 3u);
    v.remove(p, "GONE.BIN");                                            // no regard to case
    CB_CHECK_EQ(v.list_directory(p).size(), 1u);
    CB_CHECK_EQ(v.list_directory(p, 0, {}, true).size(), 2u);           // still there, as deleted
    const Entry deleted = v.list_directory(p, 0, {}, true)[1];
    CB_CHECK(deleted.is_deleted);
    CB_CHECK_EQ(deleted.name, "gone.bin");                              // the name survives, from all 42 bytes of the field
    CB_CHECK_EQ(v.cluster_chain(p, 4).size(), 1u);                      // the chain is cleared
    // a deleted file's data is still on the volume: read in cluster order
    v.extract_file(p, deleted, s.dir.path() / "back.bin");
    CB_CHECK(fs::read_file(s.dir.path() / "back.bin") == data);
    // the freed clusters are used again
    v.add_file(p, "next.bin", Bytes(10, 2));
    CB_CHECK_EQ(v.list_directory(p, 0, {}, false).back().first_cluster, 4u);
    CB_CHECK_THROWS_AS(NotFoundError, v.remove(p, "not there"));
    CB_CHECK_THROWS_AS(NotFoundError, v.remove(p, "nodir/x"));
});

CB_TEST("formats.xbox.fatx.a_full_volume", [] {
    Scratch s;
    Volume v(s.image(blank_volume(5)));
    const Partition p = v.detect_partitions().at(0);
    const std::size_t free_clusters = static_cast<std::size_t>(p.max_clusters) - 2;   // cluster 0 is reserved, 1 is the root
    v.add_file(p, "a", Bytes((free_clusters - 1) * 512, 1));
    CB_CHECK_THROWS_AS(IoError, v.add_file(p, "b", Bytes(1024, 1)));   // 2 needed, 1 free
    v.add_file(p, "c", Bytes{1});                                      // the last one
    CB_CHECK_THROWS_AS(IoError, v.add_file(p, "d", Bytes{1}));
    // what failed left nothing behind
    CB_CHECK_EQ(v.list_directory(p).size(), 2u);
});

CB_TEST("formats.xbox.fatx.little_endian_volumes_and_32bit_fats", [] {
    Scratch s;
    Volume v(s.image(blank_volume(100, ByteOrder::Little)));
    const Partition p = v.detect_partitions().at(0);
    CB_CHECK(p.order == ByteOrder::Little);
    CB_CHECK_EQ(p.epoch, 2000);
    v.add_file(p, "x.bin", Bytes(700, 9));
    const Bytes dirent = v.read_at(p.data_offset, 0x40);
    CB_CHECK_EQ(dirent[44], 2);               // little-endian first cluster 2
    CB_CHECK_EQ(dirent[47], 0);
    CB_CHECK_EQ(dirent[48], 0xBC);            // size 700 = 0x02BC
    CB_CHECK_EQ(dirent[49], 0x02);
    CB_CHECK_EQ(v.list_directory(p)[0].file_size, 700u);
});

CB_TEST("formats.xbox.fatx.recover_finds_dirents_with_no_directory", [] {
    Scratch s;
    Volume v(s.image(blank_volume(100)));
    const Partition p = v.detect_partitions().at(0);
    v.mkdir(p, "dir");
    v.add_file(p, "dir/inside.txt", Bytes(100, 3));
    v.add_file(p, "loose.txt", Bytes(100, 4));
    v.remove(p, "loose.txt");
    // forget the root directory: wipe its cluster (the FAT chain still says where the directory was)
    Bytes wiped = fs::read_file(s.dir.path() / "v.bin");
    std::fill(wiped.begin() + static_cast<std::ptrdiff_t>(p.data_offset), wiped.begin() + static_cast<std::ptrdiff_t>(p.data_offset + 512), std::uint8_t{0xFF});
    // ... but the entries of "dir" sit in dir's own cluster and loose.txt's dirent is gone with the root: only dir's content survives
    fs::write_file(s.dir.path() / "w.bin", wiped);
    Volume w(s.dir.path() / "w.bin");
    const Partition q = w.detect_partitions().at(0);
    CB_CHECK(w.list_directory(q).empty());
    const std::vector<Entry> found = w.recover(q);
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK_EQ(found[0].name, "inside.txt");
    CB_CHECK_EQ(found[0].file_size, 100u);
    // deleted entries by default; with deleted_only off, the live ones too
    CB_CHECK_EQ(w.recover_all(q, s.dir.path() / "rec", true), 0u);
    CB_CHECK_EQ(w.recover_all(q, s.dir.path() / "rec", false), 1u);
    CB_CHECK(fs::read_file(s.dir.path() / "rec" / "inside.txt") == Bytes(100, 3));
    // an intact volume: the deleted file is recovered, and a directory's children get "<dir>/<name>" paths
    Volume intact(s.dir.path() / "v.bin");
    const std::vector<Entry> all = intact.recover(q);
    const auto inside = std::find_if(all.begin(), all.end(), [](const Entry& e) { return e.name == "inside.txt"; });
    CB_CHECK(inside != all.end());
    CB_CHECK_EQ(inside->path, "dir/inside.txt");
    CB_CHECK_EQ(intact.recover_all(q, s.dir.path() / "rec2", true), 1u);          // loose.txt, deleted
    CB_CHECK(fs::read_file(s.dir.path() / "rec2" / "loose.txt") == Bytes(100, 4));
});

CB_TEST("formats.xbox.fatx.carve_by_signature", [] {
    Scratch s;
    Bytes img = blank_volume(200);
    img.resize(img.size() + 0x24000);   // room for what is placed below (the STFS package alone is 0xD000 long)
    const std::size_t data = 0x2000;
    // an XBE: "XBEH", SizeOfImage at +0x10C
    std::copy_n("XBEH", 4, img.begin() + data + 0x1000);
    img[data + 0x1000 + 0x10C] = 0x00;
    img[data + 0x1000 + 0x10C + 1] = 0x20;                                  // 0x2000
    // an STFS package: "CON ", the total block count (24 bits, little-endian) at 0x395
    std::copy_n("CON ", 4, img.begin() + data + 0x8000);
    img[data + 0x8000 + 0x395] = 0x02;                                      // 2 blocks: 0xB000 + 0x2000
    // a PE image: MZ, e_lfanew = 0x80, "PE\0\0", one section with raw data ending at 0x600
    std::copy_n("MZ", 2, img.begin() + data + 0x20000 - 0x1000);
    const std::size_t pe = data + 0x20000 - 0x1000;
    img[pe + 0x3C] = 0x80;
    std::copy_n("PE\0\0", 4, img.begin() + pe + 0x80);
    img[pe + 0x80 + 4 + 2] = 1;                                             // number of sections
    img[pe + 0x80 + 4 + 16] = 0xE0;                                         // size of the optional header
    const std::size_t section = pe + 0x80 + 24 + 0xE0;
    img[section + 16] = 0x00; img[section + 17] = 0x04;                     // raw size 0x400
    img[section + 20] = 0x00; img[section + 21] = 0x02;                     // raw pointer 0x200
    Bytes padded = img;
    fs::write_file(s.dir.path() / "c.bin", padded);
    Volume v(s.dir.path() / "c.bin");
    // the carver reads the partition's data: this one-partition image is bigger than its FAT geometry says; mount sees the file size
    const Partition p = v.detect_partitions().at(0);
    const std::vector<Carved> found = v.carve(p, s.dir.path() / "out");
    std::map<std::string, Carved> by_kind;
    for (const Carved& c : found) by_kind[c.kind] = c;
    CB_CHECK(by_kind.contains("XBE"));
    CB_CHECK(by_kind.contains("STFS"));
    CB_CHECK(by_kind.contains("PE"));
    CB_CHECK_EQ(by_kind["XBE"].offset, 0x1000u);
    CB_CHECK_EQ(by_kind["XBE"].length, 0x2000u);
    CB_CHECK_EQ(by_kind["STFS"].offset, 0x8000u);
    CB_CHECK_EQ(by_kind["STFS"].length, 0xB000u + 0x2000u);
    CB_CHECK_EQ(by_kind["PE"].length, 0x600u);
    CB_CHECK_EQ(fs::file_size(by_kind["XBE"].path), 0x2000u);
    CB_CHECK_EQ(fs::to_text(by_kind["XBE"].path.filename()), "00001000_XBE.xbe");
    CB_CHECK_EQ(fs::to_text(by_kind["STFS"].path.filename()), "00008000_STFS.stfs");
    CB_CHECK_EQ(fs::to_text(by_kind["PE"].path.filename()), std::format("{:08X}_PE.exe", by_kind["PE"].offset));
    CB_CHECK(fs::read_file(by_kind["XBE"].path) == Bytes(img.begin() + data + 0x1000, img.begin() + data + 0x3000));
    CB_CHECK_THROWS_AS(UsageError, (void)v.carve(p, s.dir.path() / "out2", 0));
});

CB_TEST("formats.xbox.fatx.carving_xex", [] {
    Scratch s;
    // an unencrypted "basic" XEX2: header with the file format info, run sizes, then data
    Bytes img = blank_volume(200);
    const std::size_t at = 0x2000 + 0x1000;
    auto be32 = [&](std::size_t o, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) img[at + o + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
    };
    std::copy_n("XEX2", 4, img.begin() + at);
    be32(0x08, 0x1000);       // data offset
    be32(0x10, 0x100);        // security info
    be32(0x14, 1);            // one optional header
    be32(0x18, 0x000003FF);   // file format info
    be32(0x1C, 0x400);        // at 0x400 in the header
    be32(0x400, 24);          // size: 8 + one run of 8... (info size 24: two runs)
    img[at + 0x404] = 0; img[at + 0x405] = 0;     // not encrypted
    img[at + 0x406] = 0; img[at + 0x407] = 1;     // basic compression
    be32(0x408, 0x1800);      // run 1: 0x1800 bytes of data
    be32(0x40C, 0);
    be32(0x410, 0x300);       // run 2
    be32(0x414, 0);
    fs::write_file(s.dir.path() / "x.bin", img);
    Volume v(s.dir.path() / "x.bin");
    const Partition p = v.detect_partitions().at(0);
    const std::vector<Carved> found = v.carve(p, s.dir.path() / "out");
    CB_CHECK_EQ(found.size(), 1u);
    if (found.size() == 1) {
        CB_CHECK_EQ(found[0].kind, "XEX");
        CB_CHECK_EQ(found[0].length, 0x3000u);       // data offset + 0x1B00 of data, rounded up to the next page
    }
});

CB_TEST("formats.xbox.fatx.damaged_directories", [] {
    Scratch s;
    Bytes img = blank_volume(100);
    // a directory entry that names the root as its own content: a loop
    const std::size_t root = 0x2000;
    img[root] = 4;
    img[root + 1] = 0x10;
    std::copy_n("loop", 4, img.begin() + root + 2);
    std::fill(img.begin() + root + 6, img.begin() + root + 44, std::uint8_t{0xFF});
    img[root + 44] = img[root + 45] = img[root + 46] = 0;
    img[root + 47] = 1;   // first cluster 1 (big-endian): itself
    for (std::size_t i = 48; i < 64; ++i) img[root + i] = 0;
    fs::write_file(s.dir.path() / "l.bin", img);
    Volume v(s.dir.path() / "l.bin");
    const Partition p = v.detect_partitions().at(0);
    CB_CHECK_THROWS_AS(FormatError, (void)v.list_directory(p));
    // an entry named ".." is not written outside the output folder
    Bytes evil = blank_volume(100);
    evil[root] = 2;
    evil[root + 1] = 0x20;
    evil[root + 2] = '.';
    evil[root + 3] = '.';
    std::fill(evil.begin() + root + 4, evil.begin() + root + 44, std::uint8_t{0xFF});
    evil[root + 47] = 2;
    evil[root + 51] = 4;
    evil[0x1004] = 0xFF;   // FAT[2]: end of chain
    evil[0x1005] = 0xFF;
    fs::write_file(s.dir.path() / "e.bin", evil);
    Volume e(s.dir.path() / "e.bin");
    CB_CHECK_THROWS_AS(FormatError, (void)e.extract_all(e.detect_partitions().at(0), s.dir.path() / "out"));
    // the cluster chain of a corrupt FAT that loops stops
    Bytes cyc = blank_volume(100);
    cyc[0x1004] = 0; cyc[0x1005] = 3;      // FAT[2] -> 3
    cyc[0x1006] = 0; cyc[0x1007] = 2;      // FAT[3] -> 2
    fs::write_file(s.dir.path() / "c.bin", cyc);
    Volume c(s.dir.path() / "c.bin");
    CB_CHECK(c.cluster_chain(c.detect_partitions().at(0), 2).size() <= 118u);   // bounded by the cluster count
});

}  // namespace
}  // namespace cb::formats::xbox::fatx
