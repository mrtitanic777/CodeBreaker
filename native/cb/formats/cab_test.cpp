#include "cb/formats/cab.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <random>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::cab {
namespace {

std::filesystem::path testdata(std::string_view name) {
    return testing::data_path(std::string("formats/testdata/cab/") + std::string(name));
}

std::uint32_t crc_of(const std::filesystem::path& p) { return crypto::crc32(fs::read_file(p)); }

// ---- headers ---------------------------------------------------------------------------------------------

CB_TEST("formats.cab.header_of_a_cabinet_in_a_set", [] {
    // Built by the old writer: set 9, the first of two, one 100-byte file in one MSZIP folder.
    const Cabinet c = read(testdata("set_a.cab"));
    CB_CHECK_EQ(c.set_id, 9);
    CB_CHECK_EQ(c.index, 0);
    CB_CHECK_EQ(c.flags, 2);
    CB_CHECK(!c.prev.has_value());
    CB_CHECK(c.next.has_value());
    CB_CHECK_EQ(c.next->cabinet, "set_b.cab");
    CB_CHECK_EQ(c.next->disk, "");
    CB_CHECK_EQ(c.size, 91u);
    CB_CHECK_EQ(c.version_major, 1);
    CB_CHECK_EQ(c.version_minor, 3);
    CB_CHECK_EQ(c.folders.size(), 1u);
    CB_CHECK_EQ(c.folders[0].data_offset, 75u);
    CB_CHECK_EQ(c.folders[0].block_count, 1);
    CB_CHECK_EQ(c.folders[0].type, 1);
    CB_CHECK_EQ(c.folders[0].compression(), "MSZIP");
    CB_CHECK_EQ(c.files.size(), 1u);
    CB_CHECK_EQ(c.files[0].name, "one");
    CB_CHECK_EQ(c.files[0].size, 100u);
    CB_CHECK_EQ(c.files[0].folder, 0);
    CB_CHECK_EQ(c.files[0].offset, 0u);
    CB_CHECK_EQ(c.files[0].date, 23915);
    CB_CHECK_EQ(c.files[0].time, 6860);
    CB_CHECK_EQ(c.files[0].attributes, 32);
    CB_CHECK_EQ(c.compression_of(c.files[0]), "MSZIP");
});

CB_TEST("formats.cab.neighbours_with_disk_labels", [] {
    const Cabinet c = read(testdata("w_chain.cab"));
    CB_CHECK_EQ(c.set_id, 7);
    CB_CHECK_EQ(c.index, 1);
    CB_CHECK_EQ(c.flags, 3);
    CB_CHECK_EQ(c.prev->cabinet, "a.cab");
    CB_CHECK_EQ(c.prev->disk, "Disk 1");
    CB_CHECK_EQ(c.next->cabinet, "c.cab");
    CB_CHECK_EQ(c.next->disk, "Disk 3");
});

CB_TEST("formats.cab.continuation_markers_name_a_folder", [] {
    const Cabinet next = read(testdata("cont_next.cab"));
    CB_CHECK_EQ(next.files[0].folder, kToNext);
    CB_CHECK_EQ(next.folder_of(next.files[0]), 0u);   // the last (only) folder
    const Cabinet prev = read(testdata("cont_prev.cab"));
    CB_CHECK_EQ(prev.files[0].folder, kFromPrev);
    CB_CHECK_EQ(prev.folder_of(prev.files[0]), 0u);
    const Cabinet both = read(testdata("cont_both.cab"));
    CB_CHECK_EQ(both.files[0].folder, kPrevAndNext);
    CB_CHECK_EQ(both.folder_of(both.files[0]), 0u);
    CB_CHECK_EQ(both.compression_of(both.files[0]), "MSZIP");
});

CB_TEST("formats.cab.names_in_utf8_or_latin1", [] {
    // 'été.txt' is stored as UTF-8 (attribute 0x80, so 160 = 0xA0 with the archive bit); 'plain' is ASCII.
    const Cabinet c = read(testdata("w_uni.cab"));
    CB_CHECK_EQ(c.set_id, 4660);
    CB_CHECK_EQ(c.index, 2);
    CB_CHECK_EQ(c.files.size(), 2u);
    CB_CHECK_EQ(c.files[0].name, "\xC3\xA9t\xC3\xA9.txt");
    CB_CHECK_EQ(c.files[0].attributes, 160);
    CB_CHECK_EQ(c.files[1].name, "plain");
});

CB_TEST("formats.cab.unlisted_methods_and_the_real_folder_kinds", [] {
    CB_CHECK_EQ(compression_name(0), "NONE");
    CB_CHECK_EQ(compression_name(1), "MSZIP");
    CB_CHECK_EQ(compression_name(2), "QUANTUM");
    CB_CHECK_EQ(compression_name(3), "LZX");
    CB_CHECK_EQ(compression_name(9), "?9");
    CB_CHECK_EQ(read(testdata("type_0002.cab")).folders[0].compression(), "QUANTUM");
    CB_CHECK_EQ(read(testdata("type_0003.cab")).folders[0].compression(), "LZX");
    CB_CHECK_EQ(read(testdata("w_none.cab")).folders[0].compression(), "NONE");
});

CB_TEST("formats.cab.not_a_cabinet", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)read(testdata("empty.cab")));
    CB_CHECK_THROWS_AS(FormatError, (void)read(testdata("magic.cab")));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes{'M', 'S', 'C', 'F'}));
    CB_CHECK_THROWS_AS(NotFoundError, (void)read(testdata("missing.cab")));
});

CB_TEST("formats.cab.cut_headers", [] {
    // cut_mini1/3 are the first 343 / 588 bytes of the Windows 95 MINI.CAB: the file table runs past the end
    CB_CHECK_THROWS_AS(TruncatedError, (void)read(testdata("cut_mini1.cab")));
    CB_CHECK_THROWS_AS(TruncatedError, (void)read(testdata("cut_mini3.cab")));
    // every prefix of a good cabinet is an error, never a crash
    const Bytes good = fs::read_file(testdata("w_text.cab"));
    for (std::size_t n = 0; n < 112; ++n) CB_CHECK_THROWS_AS(FormatError, (void)parse(ByteView(good).first(n)));
});

CB_TEST("formats.cab.the_head_of_a_real_cabinet_reads", [] {
    // The first 5078 bytes of MINI.CAB (Windows 95): all 23 headers are there, the data is not.
    const Cabinet c = read(testdata("cut_mini6.cab"));
    CB_CHECK_EQ(c.set_id, 16261);
    CB_CHECK_EQ(c.size, 441905u);
    CB_CHECK_EQ(c.files.size(), 23u);
    CB_CHECK_EQ(c.files[0].name, "DOSX.EXE");
    CB_CHECK_EQ(c.files[0].size, 32682u);
    CB_CHECK_EQ(c.files[1].name, "USER.EXE");
    CB_CHECK_EQ(c.files[1].size, 264016u);
    CB_CHECK_EQ(c.files[1].offset, 32682u);
    CB_CHECK_EQ(c.files[2].name, "GDI.EXE");
    CB_CHECK_EQ(c.files[2].offset, 296698u);
    CB_CHECK_EQ(c.files[0].date, 7915);
    CB_CHECK_EQ(c.files[0].time, 20032);
    CB_CHECK_EQ(c.folders[0].data_offset, 659u);
    CB_CHECK_EQ(c.folders[0].block_count, 25);
    fs::ScopedTempDir dir("cab-test");
    CB_CHECK_THROWS_AS(FormatError, (void)extract(testdata("cut_mini6.cab"), dir.path()));   // the data blocks are cut short
});

// ---- checksum ----------------------------------------------------------------------------------------------

CB_TEST("formats.cab.checksum", [] {
    CB_CHECK_EQ(checksum(Bytes{}, 0), 0u);
    // the header bytes 03 00 03 00 form one word 0x00030003; the odd payload bytes 01 02 03 count most-significant first
    CB_CHECK_EQ(checksum(Bytes{1, 2, 3}, 3), 0x00030003u ^ 0x00010203u);
    // a payload that fills whole words: 04 00 04 00 | 01 02 03 04 -> words 0x00040004 and 0x04030201
    CB_CHECK_EQ(checksum(Bytes{1, 2, 3, 4}, 4), 0x00040004u ^ 0x04030201u);
    // one odd byte (the tail is 1 byte: 0x05) and two whole words
    CB_CHECK_EQ(checksum(Bytes{1, 2, 3, 4, 5}, 5), 0x00050005u ^ 0x04030201u ^ 0x05u);
    // whole words only (nothing odd): 04 00 04 00 | 00 00 0A 0B
    CB_CHECK_EQ(checksum(Bytes{0, 0, 0xA, 0xB}, 4), 0x00040004u ^ 0x0B0A0000u);
    // two odd bytes, most significant first: 0x0102
    CB_CHECK_EQ(checksum(Bytes{1, 2}, 2), 0x00020002u ^ 0x0102u);
});

Bytes random_bytes(std::size_t n, unsigned seed) {
    std::mt19937 rng(seed);
    Bytes b(n);
    for (auto& x : b) x = static_cast<std::uint8_t>(rng());
    return b;
}

const DosTimestamp kStamp{23915, 6860};

// ---- extraction --------------------------------------------------------------------------------------------

CB_TEST("formats.cab.extracts_mszip_files_and_makes_folders", [] {
    fs::ScopedTempDir dir("cab-test");
    const std::vector<Extracted> got = extract(testdata("w_text.cab"), dir.path());
    CB_CHECK_EQ(got.size(), 3u);
    CB_CHECK_EQ(got[0].name, "a.txt");
    CB_CHECK_EQ(got[1].name, "b\\c.txt");
    CB_CHECK_EQ(got[2].name, "empty");
    CB_CHECK(got[1].path == dir.path() / "b" / "c.txt");
    // sizes and CRC-32 of the contents, as the old tool extracted them
    CB_CHECK_EQ(fs::file_size(got[0].path), 1800u);
    CB_CHECK_EQ(crc_of(got[0].path), 1687536614u);
    CB_CHECK_EQ(fs::file_size(got[1].path), 3u);
    CB_CHECK_EQ(crc_of(got[1].path), 2436942953u);
    CB_CHECK_EQ(fs::file_size(got[2].path), 0u);
});

CB_TEST("formats.cab.file_times_come_from_the_cabinet", [] {
    fs::ScopedTempDir dir("cab-test");
    const auto got = extract(testdata("set_a.cab"), dir.path());
    // date 23915 = 2026-11-11, time 6860 = 03:22:24 (local time, the DOS way)
    const auto sys = std::chrono::clock_cast<std::chrono::system_clock>(std::filesystem::last_write_time(got[0].path));
    const std::time_t t = std::chrono::system_clock::to_time_t(sys);
    std::tm tm{};
    localtime_s(&tm, &t);
    CB_CHECK_EQ(tm.tm_year + 1900, 2026);
    CB_CHECK_EQ(tm.tm_mon + 1, 11);
    CB_CHECK_EQ(tm.tm_mday, 11);
    CB_CHECK_EQ(tm.tm_hour, 3);
    CB_CHECK_EQ(tm.tm_min, 22);
    CB_CHECK_EQ(tm.tm_sec, 24);
});

CB_TEST("formats.cab.extracts_stored_files", [] {
    fs::ScopedTempDir dir("cab-test");
    const auto got = extract(testdata("w_none.cab"), dir.path());
    CB_CHECK_EQ(got.size(), 2u);
    CB_CHECK_EQ(crc_of(got[0].path), 1687536614u);   // a.txt, the same text as in w_text
    CB_CHECK_EQ(crc_of(got[1].path), 2965430058u);   // z.bin
    CB_CHECK_EQ(fs::file_size(got[1].path), 768u);
});

CB_TEST("formats.cab.a_name_picks_a_file_without_regard_to_case", [] {
    fs::ScopedTempDir dir("cab-test");
    const auto got = extract(testdata("w_text.cab"), dir.path(), std::vector<std::string>{"A.TXT"});
    CB_CHECK_EQ(got.size(), 1u);
    CB_CHECK_EQ(got[0].name, "a.txt");
    CB_CHECK(!std::filesystem::exists(dir.path() / "empty"));
    CB_CHECK_EQ(extract(testdata("w_text.cab"), dir.path(), std::vector<std::string>{}).size(), 0u);
    CB_CHECK_EQ(extract(testdata("w_text.cab"), dir.path(), std::vector<std::string>{"nothing"}).size(), 0u);
});

CB_TEST("formats.cab.utf8_names_extract", [] {
    fs::ScopedTempDir dir("cab-test");
    const auto got = extract(testdata("w_uni.cab"), dir.path());
    CB_CHECK_EQ(got.size(), 2u);
    CB_CHECK_EQ(crc_of(dir.path() / "plain"), 2181537457u);
    CB_CHECK_EQ(crc_of(fs::from_text("\xC3\xA9t\xC3\xA9.txt") == "" ? dir.path() : dir.path() / fs::from_text("\xC3\xA9t\xC3\xA9.txt")), 1734464235u);
});

CB_TEST("formats.cab.a_block_of_exactly_32768_bytes", [] {
    fs::ScopedTempDir dir("cab-test");
    const auto got = extract(testdata("w_exact_block.cab"), dir.path());
    CB_CHECK_EQ(fs::file_size(got[0].path), 32768u);
    CB_CHECK_EQ(crc_of(got[0].path), 2673409839u);
});

CB_TEST("formats.cab.many_blocks_each_primed_with_the_one_before", [] {
    // 11 blocks of MSZIP, 328890 bytes of text: each block's back-references reach into the block before it
    fs::ScopedTempDir dir("cab-test");
    const auto got = extract(testdata("bad_data.cab"), dir.path());
    CB_CHECK_EQ(fs::file_size(got[0].path), 328890u);
    CB_CHECK_EQ(crc_of(got[0].path), 1468754228u);
});

CB_TEST("formats.cab.a_folder_continued_into_the_next_cabinet", [] {
    fs::ScopedTempDir dir("cab-test");
    for (const char* n : {"set_a.cab", "set_b.cab", "cont_next.cab", "cont_prev.cab", "cont_both.cab"}) {
        fs::write_file(dir.path() / n, fs::read_file(testdata(n)));
    }
    const auto next = extract(dir.path() / "cont_next.cab", dir.path() / "o1");
    CB_CHECK_EQ(crc_of(next[0].path), 712284130u);
    // 'two' of cont_prev is the folder's start in set_a (100 bytes) read from the previous cabinet
    const auto prev = extract(dir.path() / "cont_prev.cab", dir.path() / "o2");
    CB_CHECK_EQ(prev[0].name, "two");
    CB_CHECK_EQ(crc_of(prev[0].path), 712284130u);
    const auto both = extract(dir.path() / "cont_both.cab", dir.path() / "o3");
    CB_CHECK_EQ(crc_of(both[0].path), 712284130u);
    // set_b on its own is a plain cabinet
    CB_CHECK_EQ(crc_of(extract(dir.path() / "set_b.cab", dir.path() / "o4")[0].path), 2684718941u);
    // folder_stream: the decompressed stream, up to a length
    const Bytes stream = folder_stream(read(dir.path() / "set_a.cab"), 0);
    CB_CHECK(stream.size() >= 100);
    CB_CHECK_EQ(crypto::crc32(ByteView(stream).first(100)), 712284130u);
});

CB_TEST("formats.cab.a_missing_neighbour_is_named", [] {
    fs::ScopedTempDir dir("cab-test");
    fs::write_file(dir.path() / "cont_prev.cab", fs::read_file(testdata("cont_prev.cab")));   // its predecessor is not there
    try {
        (void)extract(dir.path() / "cont_prev.cab", dir.path() / "out");
        CB_CHECK(false);
    } catch (const NotFoundError& e) {
        CB_CHECK(std::string(e.what()).find("set_a.cab") != std::string::npos);
    }
});

CB_TEST("formats.cab.unsupported_and_damaged_folders", [] {
    fs::ScopedTempDir dir("cab-test");
    CB_CHECK_THROWS_AS(UnsupportedError, (void)extract(testdata("type_0003.cab"), dir.path()));   // LZX
    CB_CHECK_THROWS_AS(FormatError, (void)extract(testdata("type_0002.cab"), dir.path()));        // Quantum data that is not
    // a block's checksum, when the cabinet carries one, catches a flipped payload byte
    const Bytes data = random_bytes(3000, 12);
    const SourceFile f{"x", data};
    Bytes cab = build(std::span<const SourceFile>(&f, 1), {.compression = Compression::None, .timestamp = kStamp});
    const Cabinet c = parse(cab);
    const std::size_t block = c.folders[0].data_offset;       // the first CFDATA: checksum, sizes, payload
    const std::uint32_t sum = checksum(ByteView(cab).subspan(block + 8, 3000), 3000);
    for (int i = 0; i < 4; ++i) cab[block + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(sum >> (8 * i));
    fs::write_file(dir.path() / "good.cab", cab);
    CB_CHECK(fs::read_file(extract(dir.path() / "good.cab", dir.path() / "o1")[0].path) == data);
    cab[block + 8 + 100] ^= 0x55;
    fs::write_file(dir.path() / "bad.cab", cab);
    CB_CHECK_THROWS_AS(FormatError, (void)extract(dir.path() / "bad.cab", dir.path() / "o2"));
});

CB_TEST("formats.cab.names_that_leave_the_folder_are_refused", [] {
    fs::ScopedTempDir dir("cab-test");
    std::vector<std::string> bad_names = {"..\\evil.txt", "a\\..\\..\\evil", "C:\\evil", ".", ""};
    std::string survivors;
    for (const std::string& name : bad_names) {
        const Bytes data = {1, 2, 3};
        const SourceFile f{name, data};
        const Bytes cab = build(std::span<const SourceFile>(&f, 1), {.timestamp = DosTimestamp{23915, 6860}});
        fs::write_file(dir.path() / "evil.cab", cab);
        try {
            (void)extract(dir.path() / "evil.cab", dir.path() / "out");
            survivors += "[" + name + "]";
        } catch (const FormatError&) {
        }
    }
    CB_CHECK_EQ(survivors, "");
    CB_CHECK(!std::filesystem::exists(dir.path() / "evil.txt"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "evil"));
    // a leading slash is not a way out: the name lands inside the folder
    const Bytes data = {1, 2, 3};
    const SourceFile rooted{"/abs/path", data};
    fs::write_file(dir.path() / "rooted.cab", build(std::span<const SourceFile>(&rooted, 1), {.timestamp = DosTimestamp{23915, 6860}}));
    CB_CHECK(extract(dir.path() / "rooted.cab", dir.path() / "out2")[0].path == dir.path() / "out2" / "abs" / "path");
});

// ---- writing -----------------------------------------------------------------------------------------------

CB_TEST("formats.cab.build_and_extract_round_trip", [] {
    const Bytes text = to_bytes(text::join(std::vector<std::string>(300, "the quick brown fox "), ""));
    const Bytes noise = random_bytes(100000, 1);
    const Bytes none;
    const SourceFile files[] = {{"docs\\fox.txt", text}, {"noise.bin", noise}, {"empty", none}, {"été.txt", text}};
    for (Compression c : {Compression::MsZip, Compression::None}) {
        const Bytes cab = build(files, {.compression = c, .timestamp = kStamp});
        const Cabinet parsed = parse(cab);
        CB_CHECK_EQ(parsed.size, cab.size());
        CB_CHECK_EQ(parsed.files.size(), 4u);
        CB_CHECK_EQ(parsed.folders.size(), 1u);
        CB_CHECK_EQ(parsed.folders[0].compression(), c == Compression::MsZip ? "MSZIP" : "NONE");
        CB_CHECK_EQ(parsed.files[0].name, "docs\\fox.txt");
        CB_CHECK_EQ(parsed.files[3].name, "\xC3\xA9t\xC3\xA9.txt");
        CB_CHECK_EQ(parsed.files[3].attributes & 0x80, 0x80);       // the UTF-8 flag
        CB_CHECK_EQ(parsed.files[0].attributes & 0x80, 0);
        CB_CHECK_EQ(parsed.files[1].offset, text.size());
        CB_CHECK_EQ(parsed.files[0].date, kStamp.date);
        CB_CHECK_EQ(parsed.files[0].time, kStamp.time);
        fs::ScopedTempDir dir("cab-test");
        fs::write_file(dir.path() / "t.cab", cab);
        const auto got = extract(dir.path() / "t.cab", dir.path() / "out");
        CB_CHECK_EQ(got.size(), 4u);
        CB_CHECK(fs::read_file(got[0].path) == text);
        CB_CHECK(fs::read_file(got[1].path) == noise);
        CB_CHECK(fs::read_file(got[2].path).empty());
        CB_CHECK(fs::read_file(got[3].path) == text);
        CB_CHECK(std::filesystem::exists(dir.path() / "out" / "docs" / "fox.txt"));
    }
});

CB_TEST("formats.cab.build_is_deterministic_and_set_fields_round_trip", [] {
    const Bytes data = random_bytes(5000, 2);
    const SourceFile f{"x", data};
    WriteOptions o;
    o.timestamp = kStamp;
    o.set_id = 7;
    o.index = 1;
    o.prev = SetLink{"a.cab", "Disk 1"};
    o.next = SetLink{"c.cab", "Disk 3"};
    const Bytes a = build(std::span<const SourceFile>(&f, 1), o);
    CB_CHECK(a == build(std::span<const SourceFile>(&f, 1), o));
    const Cabinet c = parse(a);
    CB_CHECK_EQ(c.set_id, 7);
    CB_CHECK_EQ(c.index, 1);
    CB_CHECK_EQ(c.flags, 3);
    CB_CHECK_EQ(c.prev->cabinet, "a.cab");
    CB_CHECK_EQ(c.prev->disk, "Disk 1");
    CB_CHECK_EQ(c.next->cabinet, "c.cab");
    CB_CHECK_EQ(c.next->disk, "Disk 3");
    // the one header the Python writer made for the same (name, labels): w_chain.cab (101 bytes with a 1-byte file)
    const Bytes one = {0x78};
    const SourceFile x{"x", one};
    const Bytes made = build(std::span<const SourceFile>(&x, 1), {.compression = Compression::MsZip, .set_id = 7, .index = 1,
                                                                   .prev = SetLink{"a.cab", "Disk 1"}, .next = SetLink{"c.cab", "Disk 3"},
                                                                   .timestamp = kStamp});
    const Bytes old = fs::read_file(testdata("w_chain.cab"));
    CB_CHECK_EQ(made.size(), old.size());
    // everything up to the folder data (headers, set links, file table) is byte for byte what the old writer made
    CB_CHECK(Bytes(made.begin(), made.begin() + 88) == Bytes(old.begin(), old.begin() + 88));
});

CB_TEST("formats.cab.incompressible_blocks_stay_within_what_setup_takes", [] {
    const Bytes noise = random_bytes(70000, 3);   // does not deflate: a full block would exceed a 32768-byte payload
    const SourceFile f{"noise", noise};
    auto payload_sizes = [&](bool short_blocks) {
        const Bytes cab = build(std::span<const SourceFile>(&f, 1), {.short_blocks = short_blocks, .timestamp = kStamp});
        const Cabinet c = parse(cab);
        io::ByteReader r(cab, "test");
        r.seek(c.folders[0].data_offset);
        std::vector<std::pair<unsigned, unsigned>> out;   // (payload, uncompressed)
        for (unsigned i = 0; i < c.folders[0].block_count; ++i) {
            r.skip(4);
            const unsigned compressed = r.u16();
            const unsigned uncompressed = r.u16();
            r.skip(compressed);
            out.emplace_back(compressed, uncompressed);
        }
        fs::ScopedTempDir dir("cab-test");
        fs::write_file(dir.path() / "n.cab", cab);
        CB_CHECK(fs::read_file(extract(dir.path() / "n.cab", dir.path() / "o")[0].path) == noise);   // and both read back
        return out;
    };
    const auto shorted = payload_sizes(true);
    for (const auto& [payload, uncompressed] : shorted) CB_CHECK(payload <= kBlock);
    CB_CHECK_EQ(shorted[0].second, kShortBlock);   // the first block was cut short and stored
    CB_CHECK_EQ(shorted[0].first, kShortBlock + 5 + 2);
    CB_CHECK_EQ(shorted[1].second, kShortBlock);
    unsigned total = 0;
    for (const auto& [payload, uncompressed] : shorted) total += uncompressed;
    CB_CHECK_EQ(total, noise.size());
    const auto full = payload_sizes(false);
    CB_CHECK_EQ(full[0].second, kBlock);
    CB_CHECK(full[0].first > kBlock);   // 7-Zip-style: a full block, its payload past 32768
});

CB_TEST("formats.cab.limits", [] {
    const Bytes d = {1};
    CB_CHECK_THROWS_AS(UsageError, (void)build(std::span<const SourceFile>(), {.compression = Compression::Lzx}));
    CB_CHECK_NOTHROW((void)build(std::span<const SourceFile>(), {}));   // an empty cabinet is valid
    const Cabinet empty = parse(build(std::span<const SourceFile>(), {.timestamp = kStamp}));
    CB_CHECK_EQ(empty.files.size(), 0u);
    CB_CHECK_EQ(empty.folders.size(), 1u);
    CB_CHECK_EQ(empty.folders[0].block_count, 0);
    std::vector<SourceFile> many(65536, SourceFile{"f", d});
    CB_CHECK_THROWS_AS(UnsupportedError, (void)build(many, {.compression = Compression::None, .timestamp = kStamp}));
});

CB_TEST("formats.cab.write_returns_the_size", [] {
    fs::ScopedTempDir dir("cab-test");
    const Bytes d = random_bytes(1000, 4);
    const SourceFile f{"x", d};
    const std::uint64_t n = write(std::span<const SourceFile>(&f, 1), dir.path() / "sub" / "x.cab", {.timestamp = kStamp});
    CB_CHECK_EQ(n, fs::file_size(dir.path() / "sub" / "x.cab"));
});

// ---- against the operating system's own expander and the real Windows 95 cabinets ----------------------

bool expand_works(const std::filesystem::path& cab, const std::filesystem::path& dest) {
    try {
        std::filesystem::create_directories(dest);
        // Windows' expand (the PATH may find a GNU one first), its paths in the native form: it takes '/' for an option
        const std::filesystem::path expand =
            std::filesystem::path(fs::get_env("SystemRoot").value_or("C:\\Windows")) / "System32" / "expand.exe";
        const process::ProcessResult r =
            process::run({.argv = {expand.string(), "-F:*", std::filesystem::path(cab).make_preferred().string(),
                                   std::filesystem::path(dest).make_preferred().string()},
                          .timeout = std::chrono::seconds(120)});
        return r.ok();
    } catch (const ToolError&) {
        return false;
    }
}

CB_TEST("formats.cab.what_we_write_expand_reads", [] {
    // Windows' own expand.exe (the cabinet DLL SETUP-era tools descend from) as the independent reader of our writer.
    const Bytes text = to_bytes(text::join(std::vector<std::string>(5000, "pack my box with five dozen liquor jugs. "), ""));
    const Bytes noise = random_bytes(140000, 9);
    const SourceFile files[] = {{"a.txt", text}, {"b.bin", noise}};
    fs::ScopedTempDir dir("cab-test");
    for (bool short_blocks : {false}) {   // (short blocks are for Windows 95 SETUP; expand.exe is the modern decoder, like 7-Zip)
        fs::write_file(dir.path() / "t.cab", build(files, {.short_blocks = short_blocks, .timestamp = kStamp}));
        if (!expand_works(dir.path() / "t.cab", dir.path() / "out")) {
            std::printf("      (skipped: expand.exe is not usable here)\n");
            return;
        }
        CB_CHECK(fs::read_file(dir.path() / "out" / "a.txt") == text);
        CB_CHECK(fs::read_file(dir.path() / "out" / "b.bin") == noise);
    }
});

std::filesystem::path real_cabinet(std::string_view name) {
    return std::filesystem::path("C:/Users/mrtit/Documents/GitHub/CodeBreaker/state/work/typed-port/formats.cab/real") / std::string(name);
}

CB_TEST("formats.cab.real_windows_95_mini_cab_against_expand", [] {
    // MINI.CAB: 15 modules in one MSZIP folder of 25 blocks. Not checked in (it is Windows 95); run where it is.
    const std::filesystem::path path = real_cabinet("MINI.CAB");
    if (!std::filesystem::is_regular_file(path)) {
        std::printf("      (skipped: %s is not here)\n", path.string().c_str());
        return;
    }
    fs::ScopedTempDir dir("cab-test");
    const auto got = extract(path, dir.path() / "ours");
    CB_CHECK_EQ(got.size(), 23u);
    const Cabinet c = read(path);
    for (std::size_t i = 0; i < got.size(); ++i) CB_CHECK_EQ(fs::file_size(got[i].path), c.files[i].size);
    if (!expand_works(path, dir.path() / "theirs")) {
        std::printf("      (expand.exe is not usable here: sizes only)\n");
        return;
    }
    for (const Extracted& e : got) {
        CB_CHECK(fs::read_file(e.path) == fs::read_file(dir.path() / "theirs" / fs::from_text(e.name)));
    }
});

CB_TEST("formats.cab.real_quantum_cabinet", [] {
    // PRECOPY1.CAB: a Quantum folder that runs on into PRECOPY2.CAB. The first file of PRECOPY1 is read through the
    // block chain; file contents are checked by their own structure (SETUP's .INF/.DLL/.SYS files).
    const std::filesystem::path path = real_cabinet("PRECOPY1.CAB");
    if (!std::filesystem::is_regular_file(path)) {
        std::printf("      (skipped: %s is not here)\n", path.string().c_str());
        return;
    }
    const Cabinet c = read(path);
    CB_CHECK_EQ(c.folders[0].compression(), "QUANTUM");
    CB_CHECK(c.next.has_value());
    CB_CHECK(std::any_of(c.files.begin(), c.files.end(), [](const FileEntry& e) { return e.folder == kToNext; }));
    fs::ScopedTempDir dir("cab-test");
    const auto got = extract(path, dir.path(), std::vector<std::string>{"drvspace.sys"});   // 2 KB, 160 KB into the folder
    CB_CHECK_EQ(got.size(), 1u);
    const Bytes data = fs::read_file(got[0].path);
    const auto entry = std::find_if(c.files.begin(), c.files.end(), [](const FileEntry& e) { return text::iequals(e.name, "drvspace.sys"); });
    CB_CHECK_EQ(data.size(), entry->size);
    // a DOS device driver: the header's "next device" link is -1:-1 (it is a one-driver file)
    CB_CHECK(data.size() >= 4 && data[0] == 0xFF && data[1] == 0xFF);
});

}  // namespace
}  // namespace cb::formats::cab
