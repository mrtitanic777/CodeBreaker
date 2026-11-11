#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/tree.hpp"

namespace {

namespace fs = cb::fs;
namespace stdfs = std::filesystem;

// The paths relative to `root`, as text.
std::vector<std::string> rel(const stdfs::path& root, const std::vector<stdfs::path>& paths) {
    std::vector<std::string> out;
    for (const stdfs::path& p : paths) out.push_back(fs::to_text(p.lexically_relative(root)));
    return out;
}

CB_TEST("java.tree.order_is_files_then_folders_by_name", [] {
    fs::ScopedTempDir tmp("cbtree");
    for (const char* name : {"b.java", "a.java", "Z.java", "sub/y.java", "sub/x.cpp", "sub/deeper/k.java", "alpha/q.java", "m.txt"}) {
        fs::write_text(tmp.path() / name, "x");
    }
    // each folder's files first (by name: upper case before lower, as the bytes sort), then its folders, each in turn
    const std::vector<std::string> want = {"Z.java", "a.java", "b.java", "m.txt", "alpha/q.java", "sub/x.cpp", "sub/y.java", "sub/deeper/k.java"};
    CB_CHECK(rel(tmp.path(), cb::java::tree::files(tmp.path())) == want);
});

CB_TEST("java.tree.extensions", [] {
    fs::ScopedTempDir tmp("cbtree");
    for (const char* name : {"A.java", "B.JAVA", "C.javax", "D.h", "E.hpp", "F.cpp", "g.CPP", "noext", ".java", "sub/H.h"}) {
        fs::write_text(tmp.path() / name, "x");
    }
    using cb::java::tree::files_with_extension;
    // case-sensitive, and the whole ending counts (".javax" is not ".java"); a file named only ".java" is one
    CB_CHECK(rel(tmp.path(), files_with_extension(tmp.path(), {".java"})) == (std::vector<std::string>{".java", "A.java"}));
    CB_CHECK(rel(tmp.path(), files_with_extension(tmp.path(), {".h", ".hpp"})) == (std::vector<std::string>{"D.h", "E.hpp", "sub/H.h"}));
    CB_CHECK(files_with_extension(tmp.path(), {".nothing"}).empty());
    CB_CHECK(files_with_extension(tmp.path(), {}).empty());
});

CB_TEST("java.tree.missing_and_odd_roots", [] {
    fs::ScopedTempDir tmp("cbtree");
    CB_CHECK(cb::java::tree::files(tmp.path() / "nowhere").empty());
    fs::write_text(tmp.path() / "a_file.java", "x");
    CB_CHECK(cb::java::tree::files(tmp.path() / "a_file.java").empty());     // a file is not a tree
    CB_CHECK(cb::java::tree::files(tmp.path()).size() == 1);
    stdfs::create_directories(tmp.path() / "empty" / "inner");               // folders without files list nothing
    CB_CHECK(cb::java::tree::files(tmp.path() / "empty").empty());
    // a folder named like a source file is a folder, not a file
    stdfs::create_directories(tmp.path() / "dir.java");
    fs::write_text(tmp.path() / "dir.java" / "Inner.java", "x");
    CB_CHECK(rel(tmp.path(), cb::java::tree::files_with_extension(tmp.path(), {".java"})) ==
             (std::vector<std::string>{"a_file.java", "dir.java/Inner.java"}));
});

CB_TEST("java.tree.non_ascii_names", [] {
    fs::ScopedTempDir tmp("cbtree");
    fs::write_text(tmp.path() / stdfs::u8path("Caf\xC3\xA9.java"), "x");
    const auto found = cb::java::tree::files_with_extension(tmp.path(), {".java"});
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK_EQ(fs::to_text(found[0].filename()), "Caf\xC3\xA9.java");
});

}  // namespace
