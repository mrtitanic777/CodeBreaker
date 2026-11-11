#include "cb/formats/xbox/stfs.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::stfs {
namespace {

using J = nlohmann::json;
namespace stdfs = std::filesystem;

stdfs::path fixture(std::string_view name) { return testing::data_path(std::string("formats/testdata/stfs/") + std::string(name)); }
Bytes load(std::string_view name) { return fs::read_file(fixture(name)); }

// "b<length>:<sha1>": a byte string as the old harness recorded it
std::string tag(ByteView b) { return std::format("b{}:{}", b.size(), crypto::to_hex(crypto::sha1(b))); }

// The template packages: header skeletons (console cert + metadata layout) of a CON, a LIVE and a PIRS package.
Bytes tpl(const std::string& kind) { return load("tpl_" + kind + ".bin"); }

// the files of a folder fixture, by name, as `build` takes them
struct Folder {
    std::vector<std::pair<std::string, Bytes>> files;
    std::vector<SourceFile> sources() const {
        std::vector<SourceFile> out;
        for (const auto& [n, d] : files) out.push_back({n, d});
        return out;
    }
};
Folder folder_of(const std::string& name) {
    Folder f;
    const stdfs::path dir = fixture("fold_" + name);
    std::vector<stdfs::path> names;
    for (const auto& e : stdfs::directory_iterator(dir)) {
        if (e.is_regular_file()) names.push_back(e.path());
    }
    std::sort(names.begin(), names.end(), [](const auto& a, const auto& b) { return fs::to_text(a.filename()) < fs::to_text(b.filename()); });
    for (const auto& p : names) f.files.emplace_back(fs::to_text(p.filename()), fs::read_file(p));
    return f;
}

Bytes pack(const std::string& folder, const std::string& kind) {
    const Folder f = folder_of(folder);
    return build(f.sources(), tpl(kind));
}

// ---- packing: byte for byte what the old packer wrote --------------------------------------------------------

CB_TEST("formats.xbox.stfs.packing_is_byte_exact_with_the_old_packer", [] {
    const J golden = J::parse(fs::read_text(fixture("golden.json")));
    int checked = 0;
    for (const J& g : golden["build"]) {
        const std::string result = g["result"];
        if (result.starts_with("E:")) continue;   // (an output folder that did not exist: this writer makes it)
        const std::string want = result.substr(result.find("file:") + 5);
        const Bytes got = pack(g["fold"], g["tpl"]);
        if (tag(got) != want) {
            testing::record_failure(__FILE__, __LINE__, "pack " + g["fold"].get<std::string>() + " on " + g["tpl"].get<std::string>() + ": " + tag(got) + ", the old packer made " + want);
        }
        ++checked;
    }
    CB_CHECK(checked >= 14);
});

// ---- reading what was packed ---------------------------------------------------------------------------------

CB_TEST("formats.xbox.stfs.what_is_packed_reads_back", [] {
    for (const char* kind : {"con", "live", "pirs"}) {
        for (const char* name : {"one", "mixed", "many70", "many200", "withdir"}) {
            const Folder f = folder_of(name);
            const Package p = Package::parse(pack(name, kind));
            // names that Latin-1 cannot hold are dropped to nothing, and an entry with no name is not listed
            std::size_t expected = 0;
            for (const auto& [n, d] : f.files) {
                std::string kept;
                for (std::size_t i = 0; i < n.size();) {
                    const unsigned char c = static_cast<unsigned char>(n[i]);
                    if (c < 0x80) { kept.push_back(static_cast<char>(c)); ++i; }
                    else if ((c >> 5) == 6) { kept.push_back(static_cast<char>(((c & 0x1F) << 6) | (static_cast<unsigned char>(n[i + 1]) & 0x3F))); i += 2; }
                    else { i += (c >> 4) == 14 ? 3 : 4; }
                }
                if (kept.empty()) continue;
                ++expected;
                const std::string stored = text::from_latin1(as_bytes(kept.substr(0, 40)));
                const FileEntry* e = p.find(stored);
                CB_CHECK(e != nullptr);
                if (!e) continue;
                CB_CHECK_EQ(e->size, d.size());
                CB_CHECK(p.read_file(*e) == d);
            }
            CB_CHECK_EQ(p.files().size(), expected);
        }
    }
});

CB_TEST("formats.xbox.stfs.package_fields", [] {
    const Package p = Package::parse(pack("mixed", "con"));
    CB_CHECK(p.magic() == Magic::Con);
    CB_CHECK_EQ(to_string(p.magic()), "CON ");
    CB_CHECK(p.male_layout());                     // packages are written in the male layout
    CB_CHECK_EQ(p.block_separation(), 0);
    CB_CHECK_EQ(p.top_hash_level(), 0u);
    CB_CHECK_EQ(p.file_table_block_count(), 1);
    CB_CHECK_EQ(p.file_table_block_number(), 0u);
    CB_CHECK_EQ(p.header_size(), 38682u);          // (the template's)
    CB_CHECK_EQ(p.first_hash_offset(), 40960u);
    // 1 file-table block, then: 77 B, 1 B, 4095 B, 4096 B, 4097 B (2 blocks), 20480 B (5), 300 B, 1 B, 20 B, 0 B (1)
    CB_CHECK_EQ(p.allocated_blocks(), 17u);
    CB_CHECK_EQ(p.unallocated_blocks(), 0u);
    CB_CHECK_EQ(p.content_type(), 330542814u);
    CB_CHECK_EQ(p.title_id(), "E21B0D08");
    CB_CHECK_EQ(p.profile_id(), "2AA176CC2A772EA2");
    CB_CHECK(!p.display_name().empty());
    CB_CHECK(Package::parse(pack("one", "live")).magic() == Magic::Live);
    CB_CHECK(Package::parse(pack("one", "pirs")).magic() == Magic::Pirs);
});

CB_TEST("formats.xbox.stfs.a_file_of_no_bytes_and_a_file_table_of_several_blocks", [] {
    const Package e = Package::parse(pack("empty_file", "con"));
    CB_CHECK_EQ(e.files().size(), 1u);
    CB_CHECK_EQ(e.files()[0].size, 0u);
    CB_CHECK(e.read_file(e.files()[0]).empty());
    // 200 files need four file-table blocks and push the package past one level-0 hash table: top level 1
    const Package m = Package::parse(pack("many200", "con"));
    CB_CHECK_EQ(m.file_table_block_count(), 4);
    CB_CHECK_EQ(m.allocated_blocks(), 204u);
    CB_CHECK_EQ(m.top_hash_level(), 1u);
    CB_CHECK_EQ(m.files().size(), 200u);
    const Folder f = folder_of("many200");
    for (const auto& [n, d] : f.files) CB_CHECK(m.read_file(n) == d);
});

CB_TEST("formats.xbox.stfs.extract_all", [] {
    fs::ScopedTempDir dir("stfs-test");
    const Package p = Package::parse(pack("mixed", "con"));
    CB_CHECK_EQ(p.extract_all(dir.path() / "out"), p.files().size());
    for (const FileEntry& f : p.files()) {
        CB_CHECK(fs::read_file(dir.path() / "out" / fs::from_text(f.path)) == p.read_file(f));
    }
    CB_CHECK_THROWS_AS(FormatError, (void)p.read_file("no such file"));
});

CB_TEST("formats.xbox.stfs.the_folder_form_packs_the_files_of_a_folder", [] {
    fs::ScopedTempDir dir("stfs-test");
    const stdfs::path out = build(fixture("fold_mixed"), dir.path() / "deep" / "m.stfs", fixture("tpl_con.bin"));
    CB_CHECK(fs::read_file(out) == pack("mixed", "con"));
    // a single file instead of a folder
    const stdfs::path single = stdfs::directory_iterator(fixture("fold_one"))->path();
    const stdfs::path one = build(single, dir.path() / "o.stfs", fixture("tpl_con.bin"));
    CB_CHECK_EQ(Package::parse(fs::read_file(one)).files().size(), 1u);
    CB_CHECK_THROWS_AS(NotFoundError, (void)build(fixture("fold_nothing"), dir.path() / "x.stfs", fixture("tpl_con.bin")));
});

CB_TEST("formats.xbox.stfs.damage", [] {
    // not a package
    CB_CHECK_THROWS_AS(FormatError, (void)Package::parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)Package::parse(Bytes(100, 0)));
    Bytes pkg = pack("mixed", "con");
    Bytes badmagic = pkg;
    badmagic[0] = 'X';
    CB_CHECK_THROWS_AS(FormatError, (void)Package::parse(badmagic));
    // a header cut short, a file table cut off, a body cut off
    CB_CHECK_THROWS_AS(FormatError, (void)Package::parse(Bytes(pkg.begin(), pkg.begin() + 0x300)));
    CB_CHECK_THROWS_AS(FormatError, (void)Package::parse(Bytes(pkg.begin(), pkg.begin() + 40960)));
    const Package cut = Package::parse(Bytes(pkg.begin(), pkg.end() - 4 * kBlock));
    bool threw = false;
    for (const FileEntry& f : cut.files()) {
        try {
            (void)cut.read_file(f);
        } catch (const FormatError&) {
            threw = true;
        }
    }
    CB_CHECK(threw);
    // templates that are not packages
    const Bytes not_a_package(100, 0);
    CB_CHECK_THROWS_AS(FormatError, (void)build(folder_of("one").sources(), not_a_package));
    CB_CHECK_THROWS_AS(UsageError, (void)build(std::vector<SourceFile>{}, tpl("con")));
    // every prefix of a package is an error or a package
    for (std::size_t n = 0; n < 0x1000; n += 61) {
        try {
            (void)Package::parse(Bytes(pkg.begin(), pkg.begin() + static_cast<std::ptrdiff_t>(n)));
        } catch (const FormatError&) {
        }
    }
});

// ---- resigning: byte for byte what the old signer wrote -----------------------------------------------------

CB_TEST("formats.xbox.stfs.resigning_is_byte_exact_with_the_old_signer", [] {
    const J golden = J::parse(fs::read_text(fixture("golden.json")));
    int checked = 0, errors = 0;
    for (const J& g : golden["resign"]) {
        const std::string pk = g["pk"];
        const std::vector<std::string> args = g["args"].get<std::vector<std::string>>();
        if (args[0] != "N") continue;                     // (the variants that write to a file: tested separately)
        const std::string fold = pk.substr(0, pk.rfind('_'));
        const std::string kind = pk.substr(pk.rfind('_') + 1);
        const Bytes package = pack(fold, kind);
        const Bytes kv = load("kv_" + g["kv"].get<std::string>() + ".bin");
        const std::string result = g["result"];
        ResignOptions o;
        o.bind_console = args[1] == "T";
        if (result.starts_with("E:")) {
            CB_CHECK_THROWS_AS(FormatError, (void)resign(package, kv, o));
            ++errors;
            continue;
        }
        const std::string want = result.substr(0, result.find(' '));
        const Bytes got = resign(package, kv, o);
        if (tag(got) != want) {
            testing::record_failure(__FILE__, __LINE__, "resign " + pk + " with " + g["kv"].get<std::string>() + ": " + tag(got) + ", the old signer made " + want);
        }
        ++checked;
    }
    CB_CHECK(checked >= 10);
    CB_CHECK(errors >= 3);
});

CB_TEST("formats.xbox.stfs.resigning_changes_what_it_says_it_changes", [] {
    const Bytes kv = load("kv_good.bin");
    const Bytes live = pack("mixed", "live");
    const Bytes con = resign(live, kv);
    CB_CHECK(Package::parse(con).magic() == Magic::Con);     // LIVE converted to CON
    CB_CHECK_EQ(con.size(), live.size());
    // the certificate is the keyvault's
    CB_CHECK(Bytes(con.begin() + 4, con.begin() + 4 + kKvCertLength) == Bytes(kv.begin() + kKvCertOffset, kv.begin() + kKvCertOffset + kKvCertLength));
    // the license descriptor: one unrestricted entry
    CB_CHECK(std::all_of(con.begin() + 0x22C, con.begin() + 0x234, [](std::uint8_t b) { return b == 0xFF; }));
    CB_CHECK(std::all_of(con.begin() + 0x234, con.begin() + 0x32C, [](std::uint8_t b) { return b == 0; }));
    // everything from the metadata on is the package's own, except the header hash; the payload is untouched
    CB_CHECK(Bytes(con.begin() + 0x344, con.end()) == Bytes(live.begin() + 0x344, live.end()));
    CB_CHECK(Bytes(con.begin() + 0x234 + 0xF8, con.begin() + 0x32C) == Bytes(0x32C - 0x234 - 0xF8, 0));
    // the files still read
    CB_CHECK_EQ(Package::parse(con).files().size(), Package::parse(live).files().size());
    // binding stamps the console id and nothing else of the package
    ResignOptions bind;
    bind.bind_console = true;
    const Bytes bound = resign(live, kv, bind);
    CB_CHECK_EQ(text::upper(crypto::to_hex(ByteView(bound).subspan(0x36C, 5))), console_id(kv));
    CB_CHECK(Bytes(bound.begin() + 0x371, bound.end()) == Bytes(live.begin() + 0x371, live.end()));
    // a CON package resigned with the key it was signed with is unchanged where it was already right
    const Bytes again = resign(con, kv);
    CB_CHECK(again == con);
});

CB_TEST("formats.xbox.stfs.resign_errors", [] {
    const Bytes pkg = pack("one", "con");
    CB_CHECK_THROWS_AS(FormatError, (void)resign(Bytes(100, 0), load("kv_good.bin")));              // not a package
    CB_CHECK_THROWS_AS(FormatError, (void)resign(pkg, load("kv_short.bin")));                       // 0x3FFF bytes
    CB_CHECK_THROWS_AS(FormatError, (void)resign(pkg, load("kv_zero.bin")));                        // no key at all
    CB_CHECK_THROWS_AS(FormatError, (void)resign(pkg, load("kv_mismatch.bin")));                    // key and certificate disagree
    CB_CHECK_EQ(console_id(load("kv_good.bin")).size(), 10u);
});

CB_TEST("formats.xbox.stfs.resign_file", [] {
    fs::ScopedTempDir dir("stfs-test");
    const Bytes pkg = pack("one", "pirs");
    fs::write_file(dir.path() / "p.stfs", pkg);
    fs::write_file(dir.path() / "kv.bin", load("kv_good.bin"));
    resign_file(dir.path() / "p.stfs", dir.path() / "kv.bin", dir.path() / "out" / "q.stfs");
    CB_CHECK(fs::read_file(dir.path() / "p.stfs") == pkg);                     // the input is left alone
    CB_CHECK(fs::read_file(dir.path() / "out" / "q.stfs") == resign(pkg, load("kv_good.bin")));
    resign_file(dir.path() / "p.stfs", dir.path() / "kv.bin");                  // in place
    CB_CHECK(fs::read_file(dir.path() / "p.stfs") == resign(pkg, load("kv_good.bin")));
});

}  // namespace
}  // namespace cb::formats::xbox::stfs
