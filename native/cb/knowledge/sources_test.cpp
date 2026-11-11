#include "cb/knowledge/sources.hpp"

#include <map>
#include <set>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from codebreaker/knowledge/sources.py and its port:
//   * a data file that cannot be read, is not JSON, or is not the shape its kind says empties the kind AND is logged;
//     the Python swallowed OSError/ValueError silently and let an IndexError (a signature naming a library that is not
//     listed) crash every ask -- listed in _bugs/LIST.md;
//   * the catalogue is data (file, layout, part), not lambdas; the old `keyed` parameter of _part was never used;
//   * `authored()` of a kind that is not authored is a UsageError, not a KeyError;
//   * the data folders are passed in (DataFolders), not read from a module-level __file__.

using namespace cb;
using namespace cb::knowledge;

namespace {

// The base as this module needs it, in memory: a kind is made again when the stamp it was made under differs.
class MemoryStore : public KindStore {
public:
    struct Entry {
        std::string stamp;
        Rows rows;
        std::string source;
    };
    Rows ensure(const KindId& kind, std::string_view stamp, const std::function<Rows()>& make,
                std::string_view source) override {
        auto it = kinds.find(kind);
        if (it != kinds.end() && it->second.stamp == stamp && !forgotten.contains(kind)) return it->second.rows;
        ++makes;
        forgotten.erase(kind);
        Entry e{std::string(stamp), make(), std::string(source)};
        kinds[kind] = e;
        return e.rows;
    }
    void forget_ensured(const KindId& kind) override { forgotten.insert(kind); }

    std::map<KindId, Entry> kinds;
    std::set<KindId> forgotten;
    int makes = 0;
};

std::filesystem::path checkout() { return std::filesystem::path(CB_SOURCE_DIR).parent_path(); }

// A folder with a symbols/ and models/ the test writes its own files into.
struct Folders {
    fs::ScopedTempDir dir{"cb-knowledge-sources"};
    DataFolders folders;
    Folders() {
        folders.symbols = dir.path() / "symbols";
        folders.models = dir.path() / "models";
        fs::make_dirs(folders.symbols);
        fs::make_dirs(folders.models);
    }
};

}  // namespace

CB_TEST("knowledge.sources.catalogue", [] {
    const auto& all = AuthoredSources::catalogue();
    CB_CHECK_EQ(all.size(), 14u);
    std::set<KindId> seen;
    for (const AuthoredKind& k : all) {
        CB_CHECK(seen.insert(k.id).second);       // every kind once
        CB_CHECK(!k.what.empty());
        CB_CHECK(!k.file.empty());
    }
    CB_CHECK(AuthoredSources::find({"win16", "api_model"}) != nullptr);
    CB_CHECK(AuthoredSources::find({"win16", "prototype"}) == nullptr);   // derived, not authored
    // the three files that hold more than one kind
    int in_driver_services = 0;
    for (const AuthoredKind& k : all) in_driver_services += k.file == "driver_services.json";
    CB_CHECK_EQ(in_driver_services, 4);
    CB_CHECK_EQ(AuthoredSources({}).source_label(*AuthoredSources::find({"vxd", "api_model"})),
                "codebreaker/knowledge/models/vxd.json");
});

CB_TEST("knowledge.sources.real_files_every_kind_loads", [] {
    AuthoredSources sources(DataFolders::under(checkout()));
    MemoryStore store;
    // The counts are what the repository's data files hold (checked against the files by hand, 2026-11-11).
    const std::map<KindId, std::size_t> expected = {
        {{"windows", "ordinal_names"}, 9},        {{"win32", "argument_bytes.header"}, 7279},
        {{"win32", "signature"}, 5233},           {{"win16", "signature"}, 3988},
        {{"vxd", "device"}, 109},                 {{"vxd", "service_table"}, 38},
        {{"vxd", "driver"}, 25},                  {{"vxd", "message"}, 51},
        {{"vxd", "api_model"}, 6},                {{"dos", "interrupt"}, 27},
        {{"xbox360", "ordinal_names"}, 3},        {{"win16", "api_model"}, 14},
        {{"win32", "api_model"}, 97},             {{"win16", "kernel_ordinal"}, 17},
    };
    CB_CHECK_EQ(expected.size(), AuthoredSources::catalogue().size());
    for (const auto& [id, count] : expected) {
        const Rows rows = sources.authored(id, store);
        CB_CHECK_EQ(rows.size(), count);
    }
    CB_CHECK_EQ(store.makes, 14);
    CB_CHECK_EQ(store.kinds.at({"vxd", "device"}).source, "codebreaker/symbols/driver_services.json");
});

CB_TEST("knowledge.sources.real_rows_content", [] {
    AuthoredSources sources(DataFolders::under(checkout()));
    MemoryStore store;
    const Rows api16 = sources.authored({"win16", "api_model"}, store);
    const Value* lock = api16.find("GLOBALLOCK");
    CB_CHECK(lock != nullptr);
    CB_CHECK_EQ(*lock->get("ax")->integer(), 0);
    CB_CHECK_EQ(*lock->get("dx")->text(), "arg0");
    CB_CHECK_EQ(*lock->get("pop")->integer(), 2);
    CB_CHECK_EQ(*sources.authored({"win16", "kernel_ordinal"}, store).at("15").text(), "GLOBALALLOC");
    CB_CHECK_EQ(*sources.authored({"vxd", "device"}, store).at("0001").text(), "VMM");
    CB_CHECK_EQ(*sources.authored({"vxd", "api_model"}, store).at("Get_Cur_VM_Handle").get("answers_in")->text(), "ebx");
    CB_CHECK_EQ(*sources.authored({"win32", "api_model"}, store).at("GetLastError").get("argc")->integer(), 0);
    CB_CHECK_EQ(*sources.authored({"win32", "argument_bytes.header"}, store).at("AVIFileInfoA").integer(), 12);
    CB_CHECK_EQ(*sources.authored({"dos", "interrupt"}, store).at("10").get("name")->text(), "Video");
    CB_CHECK_EQ(*sources.authored({"xbox360", "ordinal_names"}, store).at("xam.xex").get("1")->text(), "NetDll_WSAStartup");
    const Rows ordinals = sources.authored({"windows", "ordinal_names"}, store);
    const Value& comctl = ordinals.at("COMCTL32");
    CB_CHECK_EQ(*comctl.get("from")->text(), "COMCTL32.LIB");
    CB_CHECK_EQ(*comctl.get("names")->get("151")->text(), "_CreateMRUListA@4");
});

CB_TEST("knowledge.sources.signature_rows", [] {
    AuthoredSources sources(DataFolders::under(checkout()));
    MemoryStore store;
    const Rows sig16 = sources.authored({"win16", "signature"}, store);
    // rows are numbered "000000".. in the file's order, each [lead, size, mask, digest, name, library text]
    CB_CHECK_EQ(sig16.entries()[0].first, "000000");
    CB_CHECK_EQ(sig16.entries()[3987].first, "003987");
    const Value::List& row = *sig16.at("000003").list();
    CB_CHECK_EQ(row.size(), 6u);
    CB_CHECK_EQ(*row[0].text(), "558bec5657b9");
    CB_CHECK_EQ(*row[1].integer(), 107);
    CB_CHECK_EQ(*row[3].text(), "af606c4d0b186d0d");
    CB_CHECK_EQ(*row[4].text(), "_c_exit");
    CB_CHECK_EQ(*row[5].text(), "Microsoft Visual C++ 1.5 (16-bit): C runtime (SLIBCEW.LIB)");   // index 12 of "libraries"
    const Rows sig32 = sources.authored({"win32", "signature"}, store);
    CB_CHECK_EQ(*sig32.at("000001").list()->at(4).text(), "atol");
    CB_CHECK_EQ(*sig32.at("000001").list()->at(5).text(), "Microsoft Visual C++ 2.0: C runtime (LIBC.LIB)");
});

CB_TEST("knowledge.sources.layouts_on_small_files", [] {
    Folders f;
    fs::write_text(f.folders.symbols / "driver_services.json",
                   R"({"devices": {"0002": "DEBUG", "0001": "VMM"}, "services": {}, "drivers": null})");
    AuthoredSources sources(f.folders);
    MemoryStore store;
    const Rows devices = sources.authored({"vxd", "device"}, store);
    CB_CHECK_EQ(devices.entries()[0].first, "0002");            // the file's order
    CB_CHECK_EQ(devices.size(), 2u);
    CB_CHECK_EQ(sources.authored({"vxd", "service_table"}, store).size(), 0u);   // an empty part
    CB_CHECK_EQ(sources.authored({"vxd", "message"}, store).size(), 0u);         // an absent part
});

CB_TEST("knowledge.sources.stamp_and_reload", [] {
    Folders f;
    const std::filesystem::path file = f.folders.models / "win32.json";
    fs::write_text(file, R"({"functions": {"A": {"argc": 1}}})");
    AuthoredSources sources(f.folders);
    MemoryStore store;
    const KindId id{"win32", "api_model"};
    CB_CHECK_EQ(sources.authored(id, store).size(), 1u);
    CB_CHECK_EQ(sources.authored(id, store).size(), 1u);
    CB_CHECK_EQ(store.makes, 1);                                   // the same stamp: not made again
    CB_CHECK_EQ(sources.stamp(*AuthoredSources::find(id)),
                crypto::to_hex(crypto::sha1(fs::read_file(file))));

    // The file is rewritten by this process: told, the next ask makes the kind again.
    fs::write_text(file, R"({"functions": {"A": {"argc": 1}, "B": {"argc": 2}}})");
    CB_CHECK_EQ(sources.authored(id, store).size(), 1u);           // (not told yet: the stamp is the cached one)
    const std::vector<KindId> touched = sources.changed(file, store);
    CB_CHECK_EQ(touched.size(), 1u);
    CB_CHECK(touched[0] == id);
    CB_CHECK_EQ(sources.authored(id, store).size(), 2u);
    CB_CHECK_EQ(store.makes, 2);
    CB_CHECK(sources.changed(f.folders.models / "nothing.json", store).empty());
});

CB_TEST("knowledge.sources.missing_and_broken_files", [] {
    Folders f;
    AuthoredSources sources(f.folders);
    MemoryStore store;
    {
        // a file that is not there: the kind is empty and its stamp says so
        testing::CapturedStd captured;
        CB_CHECK_EQ(sources.authored({"dos", "interrupt"}, store).size(), 0u);
        CB_CHECK_EQ(sources.stamp(*AuthoredSources::find({"dos", "interrupt"})), "missing");
        CB_CHECK(captured.err().find("dos:interrupt") != std::string::npos);   // and it is said
    }
    fs::write_text(f.folders.symbols / "dos_interrupts.json", "{ not json");
    {
        testing::CapturedStd captured;
        sources.changed(f.folders.symbols / "dos_interrupts.json", store);
        CB_CHECK_EQ(sources.authored({"dos", "interrupt"}, store).size(), 0u);
        CB_CHECK(!captured.err().empty());
    }
    // the right JSON of the wrong shape is a FormatError from read(), and empty through authored()
    fs::write_text(f.folders.symbols / "dos_interrupts.json", "[1, 2]");
    const AuthoredKind& interrupts = *AuthoredSources::find({"dos", "interrupt"});
    CB_CHECK_THROWS_AS(FormatError, (void)sources.read(interrupts));
    fs::write_text(f.folders.symbols / "dos_interrupts.json", "{not json");
    CB_CHECK_THROWS_AS(ParseError, (void)sources.read(interrupts));
    fs::remove_tree(f.folders.symbols / "dos_interrupts.json");
    CB_CHECK_THROWS_AS(NotFoundError, (void)sources.read(interrupts));
    // a signature that names a library that is not listed
    fs::write_text(f.folders.symbols / "runtime_signatures.json",
                   R"({"libraries": ["only"], "16": [["00", 1, "", "d", "n", 3]], "32": []})");
    CB_CHECK_THROWS_AS(FormatError, (void)sources.read(*AuthoredSources::find({"win16", "signature"})));
    CB_CHECK_EQ(sources.read(*AuthoredSources::find({"win32", "signature"})).size(), 0u);
    fs::write_text(f.folders.symbols / "runtime_signatures.json", R"({"libraries": ["only"], "16": [["00", 1]]})");
    CB_CHECK_THROWS_AS(FormatError, (void)sources.read(*AuthoredSources::find({"win16", "signature"})));   // not six fields
});

CB_TEST("knowledge.sources.unknown_kind_is_misuse", [] {
    AuthoredSources sources(DataFolders::under(checkout()));
    MemoryStore store;
    CB_CHECK_THROWS_AS(UsageError, (void)sources.authored({"win16", "prototype"}, store));
});

CB_TEST("knowledge.sources.data_folders", [] {
    const DataFolders d = DataFolders::under("C:/repo");
    CB_CHECK_EQ(fs::to_text(d.symbols), "C:/repo/codebreaker/symbols");
    CB_CHECK_EQ(fs::to_text(d.models), "C:/repo/codebreaker/knowledge/models");
    const fs::ScopedEnv env("CB_CHECKOUT_ROOT", "D:/elsewhere");
    CB_CHECK_EQ(fs::to_text(DataFolders::from_environment().symbols), "D:/elsewhere/codebreaker/symbols");
});
