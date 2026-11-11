#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/textutil.hpp"

namespace {

using namespace cb;
using namespace cb::java;
namespace stdfs = std::filesystem;
CB_TEST("java.textutil.literals", [] {
    using textutil::Span;
    const std::string s = R"(a "x\"y" b 'c' "unterminated)";
    auto spans = textutil::literal_spans(s);
    CB_CHECK_EQ(spans.size(), std::size_t{2});
    CB_CHECK_EQ(s.substr(spans[0].begin, spans[0].end - spans[0].begin), R"("x\"y")");
    CB_CHECK_EQ(s.substr(spans[1].begin, spans[1].end - spans[1].begin), "'c'");
    // the other quote kind inside a literal does not end it
    CB_CHECK_EQ(textutil::literal_at("\"it's\"", 0)->end, std::size_t{6});
    // a literal does not start where there is no quote; a backslash before a newline kills it
    CB_CHECK(!textutil::literal_at("abc", 0));
    CB_CHECK(!textutil::literal_at("\"a\\\nb\"", 0));
    CB_CHECK(textutil::literal_spans("").empty());
    // a newline inside is allowed (only an escaped newline is not)
    CB_CHECK(textutil::literal_at("\"a\nb\"", 0).has_value());
});

CB_TEST("java.textutil.read_write", [] {
    fs::ScopedTempDir tmp("cbjava");
    const stdfs::path file = tmp.path() / "sub" / "t.txt";
    textutil::write_text(file, "line1\r\nline2\rline3\n");
    CB_CHECK_EQ(textutil::read_text(file), "line1\nline2\nline3\n");   // newlines normalised on read
    CB_CHECK(!textutil::try_read_text(tmp.path() / "missing.txt"));
    CB_CHECK(!textutil::try_read_text(tmp.path()));                    // a directory is not a text file
    CB_CHECK_THROWS_AS(NotFoundError, textutil::read_text(tmp.path() / "missing.txt"));
});

CB_TEST("java.textutil.decode_modes", [] {
    fs::ScopedTempDir tmp("cbjava");
    const stdfs::path file = tmp.path() / "bad.txt";
    const std::string bad = "ok\xFF" "end";
    fs::write_text(file, bad);
    CB_CHECK_EQ(textutil::read_text(file), "ok\xEF\xBF\xBD" "end");                // replaced by U+FFFD
    CB_CHECK_THROWS_AS(FormatError, textutil::read_text(file, textutil::Decode::Strict));
    CB_CHECK_EQ(textutil::read_text(file, textutil::Decode::Replace).size(), std::size_t{8});
});

}  // namespace

