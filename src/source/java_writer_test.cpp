// Tests of the Java writer over a real function store (a SQLite file in a temporary folder) and a stand-in for the converter that writes
// the class files: each class's file gets the status the store has for it at its head, and STATUS.md lists them all.

#include "source/java_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace cb::source::java_writer {
namespace {

constexpr std::string_view kModule = "aa11";
constexpr std::string_view kProfile = "p1";

// four classes: one that behaves as the original, one that does not, one that does not compile (its reason runs over two lines and holds a
// bar), and one the store knows nothing of
void fill(store::FunctionStore& store) {
    store.add_module(kModule, "prog", "C:/prog", "java");
    store.add_profile(kProfile, "settings");
    const std::vector<std::string> classes = {"Block", "Item", "Broken", "Missing"};
    store.add_functions(kModule, classes);
    store::Record block{.addr = "Block", .name = "Block", .source = "class Block", .verdict = "equivalent", .detail = "every method run"};
    store::Record item{.addr = "Item", .name = "Item", .source = "class Item", .verdict = "divergent", .reason = "ret-differs", .detail = "getId returns 3, 4"};
    store::Record broken{.addr = "Broken", .name = "Broken", .source = "class Broken", .verdict = "no-compile", .reason = "error C2065: x | y\nsecond line", .detail = "ignored for a class that did not compile"};
    store.record_many(kModule, "java", "r1", kProfile, std::vector<store::Record>{block, item, broken});
}

// what the converter writes: Block.cpp with its Block.h, Item.h alone, Broken.cpp, and a class the store has no row of
void convert_into(const std::filesystem::path& out) {
    fs::make_dirs(out);
    fs::write_file(out / "Block.cpp", as_bytes("// original block\nint x;\n"));
    fs::write_file(out / "Block.h", as_bytes("// original block header\n"));
    fs::write_file(out / "Item.h", as_bytes("class Item {};\n"));
    fs::write_file(out / "Broken.cpp", as_bytes("broken\n"));
    fs::write_file(out / "Other.cpp", as_bytes("not in the store\n"));
}

CB_TEST("source.java_writer.each_classes_file_says_its_status_and_status_md_lists_them_all", [] {
    fs::ScopedTempDir dir("cb_java");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    fill(store);
    std::vector<std::string> log;
    const Written w = write_java("prog", store, kModule, kProfile, dir.path() / "out", dir.path() / "state", convert_into, [&](std::string_view l) { log.emplace_back(l); });
    CB_CHECK(w.folder == dir.path() / "out");
    CB_CHECK_EQ(w.rows.size(), 4u);
    // a class's .cpp is stamped, and its .h is left as it was
    CB_CHECK_EQ(fs::read_text(w.folder / "Block.cpp"),
                "// Block.cpp -- transpiled from Java by CodeBreaker (codebreaker/java).\n// [VERIFIED] every method run\n// original block\nint x;\n");
    CB_CHECK_EQ(fs::read_text(w.folder / "Block.h"), "// original block header\n");
    // a class with no .cpp has its .h stamped
    CB_CHECK_EQ(fs::read_text(w.folder / "Item.h"),
                "// Item.h -- transpiled from Java by CodeBreaker (codebreaker/java).\n// [UNVERIFIED] getId returns 3, 4\nclass Item {};\n");
    // a class that did not compile says why, not the detail, and only the first line
    CB_CHECK(fs::read_text(w.folder / "Broken.cpp").starts_with("// Broken.cpp -- transpiled from Java by CodeBreaker (codebreaker/java).\n// [DOES NOT COMPILE] error C2065: x | y\nbroken"));
    CB_CHECK_EQ(fs::read_text(w.folder / "Other.cpp"), "not in the store\n");   // a file of no class the store knows: untouched
    CB_CHECK_EQ(fs::read_text(w.folder / "STATUS.md"),
                "# prog -- C++03 transpiled from Java\n\n4 classes: VERIFIED 1, UNVERIFIED 1, DOES NOT COMPILE 1, NO SOURCE 1\n\n"
                "| class | status | why |\n|---|---|---|\n"
                "| Block | VERIFIED | every method run |\n"
                "| Broken | DOES NOT COMPILE | error C2065: x \\| y |\n"
                "| Item | UNVERIFIED | getId returns 3, 4 |\n"
                "| Missing | NO SOURCE | not judged on the current verifier |\n");
    CB_CHECK_EQ(log.size(), 1u);
    CB_CHECK(log[0].starts_with("prog: 4 classes -> "));
    CB_CHECK(log[0].ends_with("  (VERIFIED 1, UNVERIFIED 1, DOES NOT COMPILE 1, NO SOURCE 1)"));
});

CB_TEST("source.java_writer.a_folder_with_files_written_by_hand_is_left_alone", [] {
    fs::ScopedTempDir dir("cb_java");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    fill(store);
    fs::make_dirs(dir.path() / "out");
    fs::write_file(dir.path() / "out" / "Mine.cpp", as_bytes("// my own work\n"));
    std::vector<std::filesystem::path> converted;
    std::vector<std::string> log;
    const Written w = write_java("prog", store, kModule, kProfile, dir.path() / "out", dir.path() / "state",
                                 [&](const std::filesystem::path& to) {
                                     converted.push_back(to);
                                     convert_into(to);
                                 },
                                 [&](std::string_view l) { log.emplace_back(l); });
    CB_CHECK(w.folder == dir.path() / "state");
    CB_CHECK((converted == std::vector<std::filesystem::path>{dir.path() / "state"}));
    CB_CHECK_EQ(fs::read_text(dir.path() / "out" / "Mine.cpp"), "// my own work\n");
    CB_CHECK(!std::filesystem::exists(dir.path() / "out" / "STATUS.md"));
    CB_CHECK(std::filesystem::exists(dir.path() / "state" / "STATUS.md"));
    CB_CHECK_EQ(log.size(), 2u);
    CB_CHECK(log[0].find("holds files written by hand") != std::string::npos);
});

CB_TEST("source.java_writer.files_this_tool_wrote_may_be_written_again", [] {
    fs::ScopedTempDir dir("cb_java");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    fill(store);
    fs::make_dirs(dir.path() / "out");
    fs::write_file(dir.path() / "out" / "Old.cpp", as_bytes("// Old.cpp -- transpiled from Java by CodeBreaker (codebreaker/java).\n"));
    fs::write_file(dir.path() / "out" / "notes.txt", as_bytes("not a source file: not looked at"));
    const Written w = write_java("prog", store, kModule, kProfile, dir.path() / "out", dir.path() / "state", convert_into);
    CB_CHECK(w.folder == dir.path() / "out");   // no log given: silent
});

CB_TEST("source.java_writer.a_store_with_no_classes_gives_an_empty_report", [] {
    fs::ScopedTempDir dir("cb_java");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    store.add_module("bb22", "empty", "", "java");
    const Written w = write_java("empty", store, "bb22", kProfile, dir.path() / "out", dir.path() / "state", [](const std::filesystem::path& to) { fs::make_dirs(to); });
    CB_CHECK(w.rows.empty());
    CB_CHECK_EQ(fs::read_text(w.folder / "STATUS.md"), "# empty -- C++03 transpiled from Java\n\n0 classes: \n\n| class | status | why |\n|---|---|---|\n");
    CB_CHECK_EQ(w.counts.size(), store::kClassOrder.size());
});

CB_TEST("source.java_writer.a_long_reason_is_cut_for_the_file_and_shorter_for_the_report", [] {
    const std::string reason(300, 'x');
    fs::ScopedTempDir dir("cb_java");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    store.add_module("cc33", "long", "", "java");
    store.record_many("cc33", "java", "r1", kProfile, std::vector<store::Record>{{.addr = "Long", .source = "c", .verdict = "divergent", .reason = reason}});
    const std::vector<ClassRow> rows = class_rows(store, "cc33", kProfile);
    CB_CHECK_EQ(rows.size(), 1u);
    CB_CHECK_EQ(rows[0].said.size(), 200u);
    store::ClassCount counts;
    const std::string report = status_report("long", rows, counts);
    CB_CHECK(report.find("| " + std::string(160, 'x') + " |") != std::string::npos);
});

CB_TEST("source.java_writer.banner", [] {
    CB_CHECK_EQ(banner("A.h", store::Class::Weak, "thin match"), "// A.h -- transpiled from Java by CodeBreaker (codebreaker/java).\n// [VERIFIED (weak)] thin match\n");
});

}  // namespace
}  // namespace cb::source::java_writer
