// Tests of whose file it is: the marks CodeBreaker leaves in what it writes, and a file nobody here wrote.

#include "cb/source/authorship.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::source {
namespace {

CB_TEST("source.authorship.a_file_with_one_of_our_marks_in_its_head_is_ours", [] {
    for (const char* mark : {"reconstructed source", "behavioral reconstruction", "authored-source reconstruction", "(functional group, reconstructed)",
                             "decompiled by CodeBreaker", "by CodeBreaker"}) {
        CB_CHECK(carries_our_mark(std::string("/* ") + mark + " */"));
    }
    CB_CHECK(!carries_our_mark("/* written by hand */"));
    CB_CHECK(!carries_our_mark(""));
});

CB_TEST("source.authorship.made_by_us", [] {
    fs::ScopedTempDir dir("cb_auth");
    CB_CHECK(made_by_us(dir.path() / "not_there.c"));   // not there: ours to write
    fs::write_file(dir.path() / "ours.c", as_bytes("// lz32.c -- reconstructed source\nint main;\n"));
    CB_CHECK(made_by_us(dir.path() / "ours.c"));
    fs::write_file(dir.path() / "mine.c", as_bytes("// my own work\nint main;\n"));
    CB_CHECK(!made_by_us(dir.path() / "mine.c"));
    fs::write_file(dir.path() / "empty.c", Bytes{});
    CB_CHECK(!made_by_us(dir.path() / "empty.c"));
});

CB_TEST("source.authorship.only_the_head_of_the_file_is_looked_at", [] {
    fs::ScopedTempDir dir("cb_auth");
    std::string text(kHeadBytes, ' ');
    text += "by CodeBreaker";   // after the head: not seen
    fs::write_file(dir.path() / "late.c", as_bytes(text));
    CB_CHECK(!made_by_us(dir.path() / "late.c"));
    std::string early = "// by CodeBreaker\n";
    early.append(100000, 'x');
    fs::write_file(dir.path() / "early.c", as_bytes(early));
    CB_CHECK(made_by_us(dir.path() / "early.c"));
});

CB_TEST("source.authorship.a_file_that_cannot_be_read_is_not_ours_to_replace", [] {
    fs::ScopedTempDir dir("cb_auth");
    std::filesystem::create_directories(dir.path() / "folder.c");   // a folder where a file was expected
    CB_CHECK(!made_by_us(dir.path() / "folder.c"));
});

}  // namespace
}  // namespace cb::source
