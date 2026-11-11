#include "cb/java/console/tree.hpp"

#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

std::string walk_text(const stdfs::path& root, const tree::WalkOptions& options = {}) {
    std::string out;
    tree::walk(root, options, [&](const tree::Listing& l) {
        out += fs::to_text(l.dir.lexically_relative(root)) + ":";
        for (const std::string& d : l.subdirs) out += " [" + d + "]";
        for (const std::string& f : l.files) out += " " + f;
        out += "\n";
        return true;
    });
    return out;
}

CB_TEST("java.console.tree.order", [] {
    fs::ScopedTempDir tmp("cbtree");
    for (const char* f : {"c.txt", "B.txt", "a.txt", "sub/z.txt", "sub/y.txt", "Sub2/q.txt", "sub/deep/x.txt", "Zed/z.txt"})
        fs::write_text(tmp.path() / f, "");
    // files of a folder first, in name order ignoring case ("a" before "B", though "B" is the smaller byte); folders likewise
    CB_CHECK_EQ(walk_text(tmp.path()),
                ".: [sub] [Sub2] [Zed] a.txt B.txt c.txt\n"
                "sub: [deep] y.txt z.txt\n"
                "sub/deep: x.txt\n"
                "Sub2: q.txt\n"
                "Zed: z.txt\n");
});

CB_TEST("java.console.tree.limits", [] {
    fs::ScopedTempDir tmp("cbtree");
    for (const char* f : {"a/b/c/d.txt", "a/x.txt", "skip/y.txt", "a/Skip/z.txt", "top.txt"}) fs::write_text(tmp.path() / f, "");
    tree::WalkOptions opts;
    opts.skip_dir = [](std::string_view n) { return text::lower(n) == "skip"; };
    CB_CHECK_EQ(walk_text(tmp.path(), opts), ".: [a] top.txt\na: [b] x.txt\na/b: [c]\na/b/c: d.txt\n");
    // a depth limit lists the folders at the limit but not what is under them
    opts.max_depth = 1;
    CB_CHECK_EQ(walk_text(tmp.path(), opts), ".: [a] top.txt\na: x.txt\n");
    opts.max_depth = 0;
    CB_CHECK_EQ(walk_text(tmp.path(), opts), ".: top.txt\n");
    // the walk stops when the visitor says so
    int visited = 0;
    tree::walk(tmp.path(), {}, [&](const tree::Listing&) { return ++visited < 2; });
    CB_CHECK_EQ(visited, 2);
    // a folder that is not there, or is a file, has nothing to walk
    CB_CHECK_EQ(walk_text(tmp.path() / "nowhere"), "");
    CB_CHECK_EQ(walk_text(tmp.path() / "top.txt"), "");
    // every file below, as full paths in the walk's order
    const auto files = tree::files_below(tmp.path());
    CB_CHECK_EQ(files.size(), std::size_t{5});
    CB_CHECK_PATH(files.front(), tmp.path() / "top.txt");
});

}  // namespace
