#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/projects/minecraft/converted_tree.hpp"

namespace {

using namespace cb;
namespace tree = cb::java::projects::minecraft::converted_tree;

CB_TEST("java.minecraft.converted_tree.lists_cpp_files_sorted", [] {
    fs::ScopedTempDir dir("cb-ctree");
    fs::write_text(dir.path() / "b.cpp", "x");
    fs::write_text(dir.path() / "a.cpp", "x");
    fs::write_text(dir.path() / "c.h", "x");
    fs::write_text(dir.path() / "d.CPP", "x");   // the extension is matched exactly, as before
    fs::make_dirs(dir.path() / "sub");
    fs::write_text(dir.path() / "sub" / "e.cpp", "x");   // not recursive
    const auto names = tree::cpp_files(dir.path());
    CB_CHECK_EQ(names.size(), std::size_t{2});
    CB_CHECK_EQ(names[0], "a.cpp");
    CB_CHECK_EQ(names[1], "b.cpp");
    CB_CHECK(tree::cpp_files(dir.path() / "missing").empty());
});

CB_TEST("java.minecraft.converted_tree.keeps_the_line_end_style", [] {
    fs::ScopedTempDir dir("cb-ctree");
    fs::write_text(dir.path() / "lf.cpp", "a\nb\n");
    fs::write_text(dir.path() / "crlf.cpp", "a\r\nb\r\n");
    const auto lf = tree::read_cpp(dir.path() / "lf.cpp");
    const auto crlf = tree::read_cpp(dir.path() / "crlf.cpp");
    CB_CHECK(lf && !lf->crlf);
    CB_CHECK(crlf && crlf->crlf);
    CB_CHECK_EQ(lf->text, "a\nb\n");
    CB_CHECK_EQ(crlf->text, "a\nb\n");   // processed with "\n"
    tree::write_cpp(dir.path() / "crlf.cpp", "a\nc\n", true);
    tree::write_cpp(dir.path() / "lf.cpp", "a\nc\n", false);
    CB_CHECK_EQ(fs::read_text(dir.path() / "crlf.cpp"), "a\r\nc\r\n");
    CB_CHECK_EQ(fs::read_text(dir.path() / "lf.cpp"), "a\nc\n");
});

CB_TEST("java.minecraft.converted_tree.unreadable_file_is_skipped", [] {
    fs::ScopedTempDir dir("cb-ctree");
    CB_CHECK(!tree::read_cpp(dir.path() / "missing.cpp"));
    fs::make_dirs(dir.path() / "folder.cpp");   // a folder named like a source file
    CB_CHECK(!tree::read_cpp(dir.path() / "folder.cpp"));
    // bytes that are not UTF-8 are kept as they are
    fs::write_file(dir.path() / "odd.cpp", Bytes{0x61, 0xE9, 0x0A});
    const auto odd = tree::read_cpp(dir.path() / "odd.cpp");
    CB_CHECK(odd);
    CB_CHECK_EQ(odd->text.size(), std::size_t{3});
});

}  // namespace
