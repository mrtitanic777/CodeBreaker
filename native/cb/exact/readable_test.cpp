#include "cb/exact/readable.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::exact;
using readable::Place;

// DIVERGES from the Python module (readable.py), on purpose:
//   * \w and \b are ASCII: a name next to a non-ASCII byte is a name (in a Latin-1 source an accented letter is a byte
//     above 127, never part of a C identifier)
//   * `mapping` gives a sorted map (it is only looked up in), not a dict in SYMBOLS' order

namespace {

SymbolTable symbols_of(std::initializer_list<std::tuple<const char*, SymKind, std::int64_t>> rows) {
    SymbolTable t;
    for (const auto& [name, kind, address] : rows) t.insert_or_assign(name, SymPlace{kind, address});
    return t;
}

}  // namespace

CB_TEST("exact.readable.address_names", [] {
    CB_CHECK(readable::parse_address_name("f1234a") == Place{Place::Kind::Function, 0x1234a});
    CB_CHECK(readable::parse_address_name("_f1234a") == Place{Place::Kind::Function, 0x1234a});
    CB_CHECK(readable::parse_address_name("g_12ab") == Place{Place::Kind::Variable, 0x12ab});
    CB_CHECK(readable::parse_address_name("g_1") == Place{Place::Kind::Variable, 1});
    for (std::string_view not_one : {"f1234", "f1234ab", "F1234A", "f1234g", "g_", "g_12345", "g_12AB", "x1234a", "_g_12", "", "f"})
        CB_CHECK(!readable::parse_address_name(not_one).has_value());
});

CB_TEST("exact.readable.mapping_from_symbols", [] {
    const SymbolTable symbols = symbols_of({{"_main", SymKind::Code, 0x1000},
                                            {"_counter", SymKind::Data, 0x20},
                                            {"_both", SymKind::Unknown, 0x30},
                                            {"f0123a", SymKind::Code, 0x2000},        // an address-name itself
                                            {"_f0123a", SymKind::Code, 0x2001},
                                            {"__f0123b", SymKind::Code, 0x2002},      // ... after any underscores
                                            {"g_0010", SymKind::Data, 0x10},
                                            {"fn:local", SymKind::Code, 0x3000},      // not a name of a place
                                            {"seg@seg", SymKind::Segment, 0x40},
                                            {"not valid", SymKind::Code, 0x5000},
                                            {"_main2", SymKind::Code, 0x1000},        // the first name for a place wins
                                            {"_big", SymKind::Data, 0x10000}});       // a variable lives in 64K
    const readable::NameMap m = readable::mapping(symbols);
    CB_CHECK_EQ(m.at(Place{Place::Kind::Function, 0x1000}), "main");
    CB_CHECK_EQ(m.at(Place{Place::Kind::Variable, 0x20}), "counter");
    CB_CHECK_EQ(m.at(Place{Place::Kind::Function, 0x30}), "both");   // a place whose kind is not said is both
    CB_CHECK_EQ(m.at(Place{Place::Kind::Variable, 0x30}), "both");
    CB_CHECK_EQ(m.size(), 4u + 0u);                                  // main, counter, both twice
    CB_CHECK(!m.contains(Place{Place::Kind::Variable, 0x10000}));
});

CB_TEST("exact.readable.rename", [] {
    readable::NameMap names;
    names[Place{Place::Kind::Function, 0x1234a}] = "Init";
    names[Place{Place::Kind::Variable, 0x10}] = "counter";
    CB_CHECK_EQ(readable::rename("int f1234a(); x = g_0010 + _f1234a;", names), "int Init(); x = counter + Init;");
    CB_CHECK_EQ(readable::rename("x = g_10 + g_10;", names), "x = counter + counter;");
    // not in a comment, a string or a character constant
    CB_CHECK_EQ(readable::rename("/* f1234a */ s = \"f1234a\"; c = 'f'; f1234a(); // f1234a\n", names),
                "/* f1234a */ s = \"f1234a\"; c = 'f'; Init(); // Init\n");
    // not part of a longer word
    CB_CHECK_EQ(readable::rename("f1234ab xf1234a f1234a_ f1234a", names), "f1234ab xf1234a f1234a_ Init");
    // a name the text has already, as something else, is not given to another place
    CB_CHECK_EQ(readable::rename("int Init; f1234a();", names), "int Init; f1234a();");
    // an unclosed comment is not one: what follows is code
    CB_CHECK_EQ(readable::rename("/* f1234a", names), "/* Init");
    CB_CHECK_EQ(readable::rename("\"abc f1234a\nf1234a", names), "\"abc Init\nInit");   // (a string holds no newline: not a string)
});

namespace {

// A project in a folder: src/*.c files and a ledger that says a function is identical unless its file says BREAK.
class FolderProject final : public readable::Project {
public:
    FolderProject(std::filesystem::path src, SymbolTable symbols) : src_(std::move(src)), symbols_(std::move(symbols)) {}
    const SymbolTable& symbols() const override { return symbols_; }
    std::vector<readable::SourceFile> sources() const override {
        std::vector<readable::SourceFile> out;
        for (const auto& e : std::filesystem::directory_iterator(src_)) {
            const std::string name = e.path().filename().string();
            if (name.size() == 8 && name[0] == 'f' && name.ends_with(".c"))
                out.push_back({static_cast<std::uint32_t>(std::stoul(name.substr(1, 5), nullptr, 16)), e.path()});
        }
        std::ranges::sort(out, {}, &readable::SourceFile::address);
        return out;
    }
    const std::filesystem::path& source_dir() const override { return src_; }
    std::vector<readable::LedgerRow> ledger() const override {
        std::vector<readable::LedgerRow> rows;
        for (const auto& s : sources()) {
            const std::string text = fs::read_text(s.path);
            rows.push_back({s.path.filename().string(), std::format("fn{:05x}", s.address),
                            text.find("BREAK") != std::string::npos ? "differs" : std::string(readable::kIdentical)});
        }
        return rows;
    }
    std::unique_ptr<readable::Project> with_source_dir(const std::filesystem::path& dir) const override {
        return std::make_unique<FolderProject>(dir, symbols_);
    }

private:
    std::filesystem::path src_;
    SymbolTable symbols_;
};

}  // namespace

CB_TEST("exact.readable.the_command", [] {
    fs::ScopedTempDir dir("readable");
    fs::write_text(dir.path() / "f1234a.c", "int f1234a() { return g_0010; }\n");
    fs::write_text(dir.path() / "f00020.c", "int f00020() { return 1; }\n");
    const SymbolTable symbols = symbols_of({{"_Init", SymKind::Code, 0x1234a}, {"_counter", SymKind::Data, 0x10}});
    const FolderProject project(dir.path(), symbols);
    {   // tried and reported, nothing written
        testing::CapturedStd out;
        CB_CHECK_EQ(readable::run(project, {}), 0);
        CB_CHECK_EQ(out.out(), "1 of 2 files would take names (2 names known)\nidentical before: 2, after the renaming: 2\n");
        CB_CHECK_EQ(fs::read_text(dir.path() / "f1234a.c"), "int f1234a() { return g_0010; }\n");
    }
    {   // --write
        testing::CapturedStd out;
        CB_CHECK_EQ(readable::run(project, readable::Options{.write = true}), 0);
        CB_CHECK_EQ(out.out(), "1 of 2 files would take names (2 names known)\nidentical before: 2, after the renaming: 2\nwritten: 1 files\n");
        CB_CHECK_EQ(fs::read_text(dir.path() / "f1234a.c"), "int Init() { return counter; }\n");
        CB_CHECK_EQ(fs::read_text(dir.path() / "f00020.c"), "int f00020() { return 1; }\n");
    }
    {   // nothing left to name
        testing::CapturedStd out;
        CB_CHECK_EQ(readable::run(project, {}), 0);
        CB_CHECK_EQ(out.out(), "0 of 2 files would take names (2 names known)\n");
    }
    {   // no names at all
        testing::CapturedStd out;
        CB_CHECK_EQ(readable::run(FolderProject(dir.path(), SymbolTable{}), {}), 1);
        CB_CHECK(out.out().starts_with("SYMBOLS has no names"));
    }
});

namespace {

// A project whose ledger loses a function when the renamed text has `Init` in it (a stand-in for "the compiler
// no longer gives the program's bytes").
class BrokenByRenaming final : public readable::Project {
public:
    BrokenByRenaming(std::filesystem::path src) : src_(std::move(src)) {
        symbols_.insert_or_assign("_Init", SymPlace{SymKind::Code, 0x1234a});
    }
    const SymbolTable& symbols() const override { return symbols_; }
    std::vector<readable::SourceFile> sources() const override { return {{0x1234a, src_ / "f1234a.c"}}; }
    const std::filesystem::path& source_dir() const override { return src_; }
    std::vector<readable::LedgerRow> ledger() const override {
        const bool broken = fs::read_text(src_ / "f1234a.c").find("Init") != std::string::npos;
        return {{"f1234a.c", "fn1234a", broken ? "differs" : "identical"}};
    }
    std::unique_ptr<readable::Project> with_source_dir(const std::filesystem::path& dir) const override {
        return std::make_unique<BrokenByRenaming>(dir);
    }

private:
    std::filesystem::path src_;
    SymbolTable symbols_;
};

}  // namespace

CB_TEST("exact.readable.a_renaming_that_loses_a_function_is_not_kept", [] {
    fs::ScopedTempDir dir("readable");
    fs::write_text(dir.path() / "f1234a.c", "int f1234a() { return 0; }\n");
    testing::CapturedStd out;
    CB_CHECK_EQ(readable::run(BrokenByRenaming(dir.path()), readable::Options{.write = true}), 1);
    CB_CHECK_EQ(out.out(), "1 of 1 files would take names (1 names known)\nidentical before: 1, after the renaming: 0\nnot kept: the renaming loses 1\n");
    CB_CHECK_EQ(fs::read_text(dir.path() / "f1234a.c"), "int f1234a() { return 0; }\n");   // untouched, --write or not
});
