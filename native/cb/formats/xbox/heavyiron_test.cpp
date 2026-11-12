#include "cb/formats/xbox/heavyiron.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::heavyiron {
namespace {

using J = nlohmann::json;
namespace stdfs = std::filesystem;

// The archives are the synthetic ones the old harness built (a1: three layers, textures, a duplicate id; a2: one asset; a3: two
// PSLD layers; a5: odd sizes and an empty layer; a6: duplicate ids), and golden.json is the ORIGINAL Python's answers on them.
stdfs::path fixture(std::string_view name) { return testing::data_path(std::string("formats/testdata/heavyiron/") + std::string(name)); }
Bytes load(std::string_view name) { return fs::read_file(fixture(name)); }
J golden() { return J::parse(fs::read_text(fixture("golden.json"))); }
std::string sha(ByteView b) { return cb::crypto::to_hex(cb::crypto::sha1(b)); }
std::uint64_t id_of(const std::string& hex) { return text::parse_int<std::uint64_t>(hex, 16).value(); }
Bytes pattern(std::size_t n) {   // the replacement data the golden file used
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>((i * 7 + 1) & 255);
    return b;
}
std::optional<std::size_t> layer_of(const J& j) {
    if (j.is_null()) return std::nullopt;
    return j.get<std::size_t>();
}

void put32(Bytes& b, std::size_t at, std::int32_t v) {
    for (int i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(static_cast<std::uint32_t>(v) >> (24 - 8 * i));
}
std::int32_t get32(const Bytes& b, std::size_t at) {
    return static_cast<std::int32_t>((std::uint32_t{b[at]} << 24) | (std::uint32_t{b[at + 1]} << 16) | (std::uint32_t{b[at + 2]} << 8) | b[at + 3]);
}

// ---- reading, as the old reader did ---------------------------------------------------------------------------------

CB_TEST("formats.xbox.heavyiron.the_archives_parse_as_the_old_reader_parsed_them", [] {
    const J g = golden();
    for (const auto& [name, want] : g["archives"].items()) {
        const Bytes data = load(name + ".ho");
        const Archive a = Archive::parse(data);
        CB_CHECK_EQ(a.file_size(), data.size());
        CB_CHECK_EQ(a.layers().size(), want["layers"].size());
        for (std::size_t i = 0; i < a.layers().size(); ++i) {
            const Layer& l = a.layers()[i];
            const J& w = want["layers"][i];
            CB_CHECK_EQ(l.kind == Layer::Kind::Psl ? "PSL" : "PSLD", w[0].get<std::string>());
            CB_CHECK_EQ(l.start_block, w[1].get<std::uint32_t>());
            CB_CHECK_EQ(l.total_size, w[2].get<std::uint32_t>());
            CB_CHECK_EQ(l.assets.size(), w[3].get<std::size_t>());
        }
        const std::vector<const Asset*> all = a.assets();
        CB_CHECK_EQ(all.size(), want["assets"].size());
        for (std::size_t i = 0; i < all.size() && i < want["assets"].size(); ++i) {
            const J& w = want["assets"][i];
            const Asset& x = *all[i];
            CB_CHECK_EQ(x.layer, w[0].get<std::size_t>());
            CB_CHECK_EQ(x.id, id_of(w[1]));
            CB_CHECK_EQ(x.type, w[2].get<std::uint32_t>());
            CB_CHECK_EQ(x.actual_size, w[3].get<std::uint32_t>());
            CB_CHECK_EQ(x.slot_size, w[4].get<std::uint32_t>());
            CB_CHECK_EQ(x.data_pos(), w[5].get<std::uint64_t>());
            CB_CHECK_EQ(sha(asset_bytes(data, x)), w[6].get<std::string>());
        }
    }
});

CB_TEST("formats.xbox.heavyiron.the_fields_of_one_small_archive", [] {
    const Bytes data = load("a2.ho");
    const Archive a = Archive::parse(data);
    CB_CHECK_EQ(a.sect_pos(), 4096u);
    CB_CHECK_EQ(a.mast()[15], 2);
    CB_CHECK_EQ(a.sect_head()[1], 1);
    CB_CHECK_EQ(a.layers().size(), 1u);
    const Layer& l = a.layers()[0];
    CB_CHECK_EQ(std::string(l.type.data(), 4), std::string("ONE\0", 4));
    CB_CHECK(l.kind == Layer::Kind::Psl);
    CB_CHECK_EQ(l.sections.size(), 2u);
    CB_CHECK_EQ(l.sections[0].kind, 0);   // the AssetList
    CB_CHECK_EQ(l.sections[1].kind, 1);   // the asset data
    CB_CHECK_EQ(l.sections[0].minus_one, -1);
    const Asset& x = l.assets.at(0);
    CB_CHECK_EQ(x.id, 0x1234567890ABCDEFull);
    CB_CHECK_EQ(x.actual_size, 77u);
    CB_CHECK_EQ(x.slot_size, 128u);
    CB_CHECK_EQ(asset_bytes(data, x).size(), 77u);
    CB_CHECK_EQ(type_name(x.type), "RawBlob");
    CB_CHECK_EQ(type_name(kTextureType), "TextureDescriptor");
    CB_CHECK_EQ(type_name(1), "");
});

CB_TEST("formats.xbox.heavyiron.finding_an_asset_by_id_and_layer", [] {
    const Archive a1 = Archive::parse(load("a1.ho"));
    CB_CHECK(a1.find(2) != nullptr);
    CB_CHECK(a1.find(99) == nullptr);
    CB_CHECK_THROWS_AS(UsageError, (void)a1.find(1));   // in layers 0 and 2
    CB_CHECK_EQ(a1.find(1, 0)->layer, 0u);
    CB_CHECK_EQ(a1.find(1, 2)->layer, 2u);
    CB_CHECK(a1.find(1, 1) == nullptr);
    CB_CHECK_EQ(a1.find_all(1).size(), 2u);
    CB_CHECK_EQ(a1.find_all(0xFFFFFFFFFFFFFFF0ull).size(), 1u);   // an id past int64
    const Archive a6 = Archive::parse(load("a6.ho"));
    CB_CHECK_EQ(a6.find_all(7).size(), 3u);   // twice in layer 0, once in layer 1
    CB_CHECK_EQ(a6.find_all(7, 0).size(), 2u);
    CB_CHECK_THROWS_AS(UsageError, (void)a6.find(7, 0));
});

// ---- replacing, as the old writer did -------------------------------------------------------------------------------

CB_TEST("formats.xbox.heavyiron.replace_writes_what_the_old_writer_wrote", [] {
    const J g = golden();
    Bytes data;
    std::string loaded;
    int ok = 0, errors = 0;
    for (const J& c : g["replace"]) {
        const std::string name = c[0];
        if (name != loaded) {
            data = load(name + ".ho");
            loaded = name;
        }
        const Archive a = Archive::parse(data);
        const Bytes fresh = pattern(c[3].get<std::size_t>());
        if (c[4].is_string()) {   // "E": the old writer refused (an ambiguous or unknown id)
            CB_CHECK_THROWS_AS(UsageError, (void)replace(data, a, id_of(c[1]), fresh, layer_of(c[2])));
            ++errors;
            continue;
        }
        const Bytes got = replace(data, a, id_of(c[1]), fresh, layer_of(c[2]));
        CB_CHECK_EQ(got.size(), c[4][0].get<std::size_t>());
        CB_CHECK_EQ(sha(got), c[4][1].get<std::string>());
        ++ok;
    }
    CB_CHECK(ok > 50 && errors > 10);
});

CB_TEST("formats.xbox.heavyiron.a_replaced_asset_reads_back_and_the_archive_still_parses", [] {
    const Bytes data = load("a1.ho");
    const Archive a = Archive::parse(data);
    for (std::size_t n : {std::size_t{0}, std::size_t{10}, std::size_t{64}, std::size_t{100}, std::size_t{5000}, std::size_t{70000}}) {
        const Bytes fresh = pattern(n);
        const Bytes out = replace(data, a, 2, fresh, 0);   // layer 0's asset 2 has a 64-byte slot
        const Archive b = Archive::parse(out);
        CB_CHECK(equal_bytes(asset_bytes(out, *b.find(2, 0)), fresh));
        CB_CHECK_EQ(b.assets().size(), a.assets().size());
        for (const Asset* x : a.assets()) {   // every other asset is untouched
            if (x->layer == 0 && x->id == 2) continue;
            const Asset* y = b.find_all(x->id, x->layer).front();
            CB_CHECK(equal_bytes(asset_bytes(out, *y), asset_bytes(data, *x)));
        }
        if (n == 0 || n > 64) CB_CHECK(out.size() > data.size());   // another slot size: the layer's block moved to the end
        else CB_CHECK_EQ(out.size(), data.size());                // it fit its slot
    }
});

CB_TEST("formats.xbox.heavyiron.the_header_blob_and_other_regions_are_kept_byte_for_byte", [] {
    const Bytes data = load("a1.ho");
    const Archive a = Archive::parse(data);
    const Bytes out = replace(data, a, 0xA0000001, pattern(9000), std::nullopt);
    CB_CHECK(std::equal(data.begin(), data.begin() + 2048 + 96, out.begin()));   // HEL and MAST
    const Archive b = Archive::parse(out);
    CB_CHECK_EQ(b.layers()[1].start_block, a.layers()[1].start_block);   // layers that were not touched keep their blocks
    CB_CHECK_EQ(out.size() % kBlock, 0u);
    CB_CHECK(b.layers()[0].start_block != a.layers()[0].start_block);
});

// ---- damage ---------------------------------------------------------------------------------------------------------

Bytes a2_patched(std::size_t at, std::int32_t value) {
    Bytes b = load("a2.ho");
    put32(b, at, value);
    return b;
}

CB_TEST("formats.xbox.heavyiron.damaged_archives_are_format_errors_not_crashes", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(Bytes(100, 7)));
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(Bytes(2143, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(Bytes(5000, 0)));              // MAST pointer 0
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(2048 + 4 * 15, -3)));   // negative pointer
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(2048 + 4 * 15, 100000)));   // pointer past the end
    const std::size_t sect = 2 * 2048;
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(sect + 4, -1)));      // layer count
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(sect + 4, 100000)));
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(sect + 4, 50)));   // 50 layer entries: the others are not entries
    const std::size_t entry = sect + 32;
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(entry + 56, -4000)));   // section table before the file
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(entry + 56, 1000000)));
    Bytes bad_tag = load("a2.ho");
    const std::int32_t part2 = get32(bad_tag, entry + 56);
    std::memcpy(&bad_tag[sect + static_cast<std::size_t>(part2)], "XXXX", 4);
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(bad_tag));
    // asset list fields
    const std::size_t list = static_cast<std::size_t>(get32(load("a2.ho"), entry + 28)) * 2048;
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(list + 32 + 8, -5)));     // negative actual size
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(list + 32 + 4, -100)));   // negative offset
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(list + 32, -64)));        // negative slot
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(list, -3)));              // negative asset count
    CB_CHECK_THROWS_AS(FormatError, (void)Archive::parse(a2_patched(list, 5000)));         // more entries than the file holds
    // every cut of a good archive is an error or a (smaller) archive, never a crash
    const Bytes good = load("a1.ho");
    for (std::size_t cut = 0; cut < good.size(); cut += 509) {
        const Result<Archive> r = Archive::try_parse(ByteView(good).first(cut));
        if (cut < 3 * 2048 + 32) CB_CHECK(!r.ok());
    }
});

CB_TEST("formats.xbox.heavyiron.try_parse_answers_without_throwing", [] {
    CB_CHECK(Archive::try_parse(load("a1.ho")).ok());
    const Result<Archive> r = Archive::try_parse(Bytes(5000, 0));
    CB_CHECK(!r.ok());
    CB_CHECK(dynamic_cast<const FormatError*>(&r.error()) != nullptr);
});

CB_TEST("formats.xbox.heavyiron.an_asset_that_lies_outside_the_file_is_reported_when_read", [] {
    Bytes data = load("a2.ho");
    const std::size_t entry = 2 * 2048 + 32;
    const std::size_t list = static_cast<std::size_t>(get32(data, entry + 28)) * 2048;
    put32(data, list + 32 + 4, 0x7FFFFFF0);   // the offset of the data in its block
    const Archive a = Archive::parse(data);   // still listable
    CB_CHECK_EQ(a.assets().size(), 1u);
    CB_CHECK_THROWS_AS(FormatError, (void)asset_bytes(data, *a.assets().front()));
    CB_CHECK_THROWS_AS(FormatError, (void)replace(data, a, 0x1234567890ABCDEFull, pattern(10), std::nullopt));
});

CB_TEST("formats.xbox.heavyiron.replace_refuses_unknown_and_ambiguous_ids", [] {
    const Bytes data = load("a6.ho");
    const Archive a = Archive::parse(data);
    CB_CHECK_THROWS_AS(UsageError, (void)replace(data, a, 7, pattern(5), std::nullopt));   // in both layers
    CB_CHECK_THROWS_AS(UsageError, (void)replace(data, a, 9, pattern(5), std::nullopt));   // not there
    CB_CHECK_THROWS_AS(UsageError, (void)replace(data, a, 7, pattern(5), 7));              // no layer 7
    CB_CHECK_NOTHROW((void)replace(data, a, 8, pattern(5), 1));
});

// ---- textures -------------------------------------------------------------------------------------------------------

CB_TEST("formats.xbox.heavyiron.texture_descriptors", [] {
    Bytes d(72, 0);
    for (int i = 0; i < 8; ++i) d[i] = static_cast<std::uint8_t>(0x10 + i);
    d[0x33] = 0x54;
    put32(d, 0x34, (40 - 1) | ((24 - 1) << 13));
    const TextureDescriptor t = texture_descriptor(d);
    CB_CHECK_EQ(t.blob_id, 0x1011121314151617ull);
    CB_CHECK_EQ(t.format_byte, 0x54);
    CB_CHECK_EQ(t.width, 40u);
    CB_CHECK_EQ(t.height, 24u);
    CB_CHECK(texture_format(0x52) == texture::Format::Dxt1);
    CB_CHECK(texture_format(0x54) == texture::Format::Dxt5);
    CB_CHECK(!texture_format(0x55).has_value());
    CB_CHECK_THROWS_AS(TruncatedError, (void)texture_descriptor(ByteView(d).first(0x37)));
    CB_CHECK_THROWS_AS(TruncatedError, (void)texture_descriptor(Bytes{}));
    put32(d, 0x34, -1);   // all 13 + 13 size bits set: the largest sizes
    CB_CHECK_EQ(texture_descriptor(d).width, 8192u);
});

CB_TEST("formats.xbox.heavyiron.textures_decode_as_the_old_reader_found_them", [] {
    const Bytes data = load("a1.ho");
    const Archive a = Archive::parse(data);
    int ok = 0;
    const J g = golden();
    for (const J& c : g["texture"]) {
        const Result<Texture> t = decode_texture(data, a, id_of(c[0]), layer_of(c[1]), c[2].get<bool>());
        if (c[3].is_string()) {
            CB_CHECK(!t.ok());
            continue;
        }
        CB_CHECK(t.ok());
        if (!t.ok()) continue;
        const J& w = c[3];
        CB_CHECK_EQ(t.value().image.width, w[0].get<std::uint32_t>());
        CB_CHECK_EQ(t.value().image.height, w[1].get<std::uint32_t>());
        CB_CHECK_EQ(t.value().format_byte, w[2].get<int>());
        CB_CHECK_EQ(t.value().image.rgba.size(), std::size_t{t.value().image.width} * t.value().image.height * 4);
        // the old reader handed exactly this blob to the DXT decoder; so does this one
        const TextureDescriptor d = texture_descriptor(asset_bytes(data, *a.find(id_of(c[0]), layer_of(c[1]))));
        const Asset* blob = a.find_all(d.blob_id).front();
        CB_CHECK_EQ(sha(asset_bytes(data, *blob)), w[3].get<std::string>());
        texture::DecodeOptions o;
        o.force_opaque = c[2].get<bool>();
        CB_CHECK(t.value().image.rgba == texture::decode_dxt(asset_bytes(data, *blob), d.width, d.height, *texture_format(d.format_byte), o).rgba);
        ++ok;
    }
    CB_CHECK_EQ(ok, 12);
});

CB_TEST("formats.xbox.heavyiron.texture_failures_say_why", [] {
    const Bytes data = load("a1.ho");
    const Archive a = Archive::parse(data);
    const Result<Texture> not_decoded = decode_texture(data, a, 0xA0000003, std::nullopt);   // format 0x55
    CB_CHECK(!not_decoded.ok());
    CB_CHECK(dynamic_cast<const UnsupportedError*>(&not_decoded.error()) != nullptr);
    CB_CHECK(dynamic_cast<const UsageError*>(&decode_texture(data, a, 0xA0000004, std::nullopt).error()) != nullptr);   // its blob is missing
    CB_CHECK(dynamic_cast<const UsageError*>(&decode_texture(data, a, 0xDEAD, std::nullopt).error()) != nullptr);   // no descriptor
    CB_CHECK(dynamic_cast<const UsageError*>(&decode_texture(data, a, 1, std::nullopt).error()) != nullptr);        // ambiguous
});

// ---- as a volume and the command ------------------------------------------------------------------------------------

CB_TEST("formats.xbox.heavyiron.the_archive_as_a_volume", [] {
    const Bytes data = load("a6.ho");
    const Archive a = Archive::parse(data);
    const std::vector<VolumeEntry> v = volume_entries(a);
    // layer 0 holds id 7 twice: one name, the later asset (20 bytes); the order is that of the first appearance
    CB_CHECK_EQ(v.size(), 4u);
    CB_CHECK_EQ(v[0].name, "layer00/0000000000000007_BDE21A8D.bin");
    CB_CHECK_EQ(v[0].size, 20u);
    CB_CHECK_EQ(v[1].name, "layer00/0000000000000008_BDE21A8D.bin");
    CB_CHECK_EQ(v[2].name, "layer01/0000000000000007_BDE21A8D.bin");
    CB_CHECK_EQ(v[3].size, 50u);
    CB_CHECK_EQ(asset_bytes(data, *v[3].asset).size(), 50u);
});

CB_TEST("formats.xbox.heavyiron.is_archive_needs_the_extension_and_a_parse", [] {
    fs::ScopedTempDir dir("heavyiron");
    const Bytes good = load("a1.ho");
    fs::write_file(dir.path() / "a.ho", good);
    fs::write_file(dir.path() / "B.HO", good);
    fs::write_file(dir.path() / "c.bin", good);
    fs::write_file(dir.path() / "zeros.ho", Bytes(5000, 0));
    CB_CHECK(is_archive(dir.path() / "a.ho"));
    CB_CHECK(is_archive(dir.path() / "B.HO"));
    CB_CHECK(!is_archive(dir.path() / "c.bin"));
    CB_CHECK(!is_archive(dir.path() / "zeros.ho"));
    CB_CHECK(!is_archive(dir.path() / "missing.ho"));
});

CB_TEST("formats.xbox.heavyiron.png_files_of_decoded_textures", [] {
    texture::Image img;
    img.width = 3;
    img.height = 2;
    for (std::size_t i = 0; i < 24; ++i) img.rgba.push_back(static_cast<std::uint8_t>(i * 10));
    const Bytes png = png_bytes(img);
    CB_CHECK(std::equal(png.begin(), png.begin() + 8, std::begin(std::array<std::uint8_t, 8>{0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n'})));
    io::ByteReader r(png);
    r.skip(8);
    CB_CHECK_EQ(r.u32_be(), 13u);
    CB_CHECK_EQ(std::string(as_text(r.bytes(4))), "IHDR");
    CB_CHECK_EQ(r.u32_be(), 3u);
    CB_CHECK_EQ(r.u32_be(), 2u);
});

CB_TEST("formats.xbox.heavyiron.the_command_reports", [] {
    fs::ScopedTempDir dir("heavyiron");
    const std::string a1 = fs::to_text(fixture("a1.ho"));
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(command({"info", a1}), 0);
        const std::string out = cap.out();
        CB_CHECK(out.starts_with("a1.ho: 28672 bytes, 3 layers, "));
        CB_CHECK(out.find("TextureDescriptor") != std::string::npos);
        CB_CHECK(out.find("RawBlob") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(command({"layers", a1}), 0);
        CB_CHECK(cap.out().find("PSLD") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(command({"dups", a1}), 0);
        const std::string out = cap.out();
        CB_CHECK(out.starts_with("1 asset ids are in more than one layer"));   // id 1, in layers 0 and 2
        CB_CHECK(out.find("0000000000000001  layers [0, 2]") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(command({"list", a1, "--type", "0x" + std::format("{:X}", kTextureType)}), 0);
        const std::string out = cap.out();
        CB_CHECK_EQ(std::count(out.begin(), out.end(), '\n'), 5);   // the five descriptors
    }
    {
        testing::CapturedStd cap;
        const std::string folder = fs::to_text(dir.path() / "x");
        CB_CHECK_EQ(command({"extract", a1, folder}), 0);
        CB_CHECK(cap.out().starts_with("14 assets -> "));
        CB_CHECK_EQ(fs::read_file(dir.path() / "x" / std::format("00000000A0000001_{:08X}.bin", kTextureType)).size(), 72u);   // the descriptor
    }
    {
        // replace through the command, then read the result back
        testing::CapturedStd cap;
        fs::write_file(dir.path() / "new.bin", pattern(100));
        const std::string out_path = fs::to_text(dir.path() / "out.ho");
        CB_CHECK_EQ(command({"replace", a1, "2", fs::to_text(dir.path() / "new.bin"), out_path, "--layer", "0"}), 0);
        CB_CHECK(cap.out().starts_with("replaced 2 (100 bytes): 28672 -> "));
        const Bytes out = fs::read_file(dir.path() / "out.ho");
        CB_CHECK(equal_bytes(asset_bytes(out, *Archive::parse(out).find(2, 0)), pattern(100)));
        CB_CHECK_THROWS_AS(UsageError, (void)command({"replace", a1, "1", fs::to_text(dir.path() / "new.bin"), out_path}));   // ambiguous
        CB_CHECK_THROWS_AS(UsageError, (void)command({"replace", a1, "xyz", fs::to_text(dir.path() / "new.bin"), out_path}));
    }
    {
        testing::CapturedStd cap;
        const std::string folder = fs::to_text(dir.path() / "png");
        CB_CHECK_EQ(command({"textures", a1, folder}), 0);
        CB_CHECK(cap.out().starts_with("3 textures decoded, 2 skipped -> "));
        CB_CHECK(stdfs::exists(dir.path() / "png" / "00000000A0000001_64x32.png"));
        CB_CHECK(stdfs::exists(dir.path() / "png" / "00000000A0000002_40x24.png"));
        CB_CHECK(stdfs::exists(dir.path() / "png" / "00000000A0000005_128x64.png"));
    }
    {
        testing::CapturedStd cap;   // a command line it does not understand: the usage, status 2
        CB_CHECK_EQ(command({"bogus", a1}), 2);
        CB_CHECK_EQ(command({"info"}), 2);
        CB_CHECK(cap.out().find("heavyiron info|layers|dups") != std::string::npos);
        CB_CHECK_THROWS_AS(NotFoundError, (void)command({"info", fs::to_text(dir.path() / "none.ho")}));
        CB_CHECK_THROWS_AS(UsageError, (void)command({"list", a1, "--type"}));
        CB_CHECK_THROWS_AS(FormatError, (void)command({"info", fs::to_text(dir.path() / "new.bin")}));
    }
});

}  // namespace
}  // namespace cb::formats::xbox::heavyiron
