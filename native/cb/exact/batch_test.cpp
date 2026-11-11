#include "cb/exact/batch.hpp"

#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact;

// DIVERGES from the Python module (batch.py), on purpose:
//   * error_lines ignores a warning however it is spaced ("T.C(5) : warning ..."); the Python's pattern let a warning
//     with a blank after the colon through as an error line, which only made a batch drop candidates it need not
//   * \w is ASCII

namespace {

// (the compiler's own layout: the type on a line of its own, then the name and its parameters)
const std::string kFn = "/* MFLAGS: -Od */\n#include <stdio.h>\n\nint\nf(a)\nint a;\n{\n    return g(a) + 1;\n}\n";

std::string candidate(std::string_view name, std::string_view body_line) {
    return std::format("/* MFLAGS: -Od */\n#include <stdio.h>\nextern int g();\nint\n{}(a)\nint a;\n{{\n    return {};\n}}\n", name, body_line);
}

}  // namespace

CB_TEST("exact.batch.split_cuts_the_preamble", [] {
    const auto s = batch::split(kFn);
    CB_CHECK(s.has_value());
    CB_CHECK_EQ(s->preamble, "/* MFLAGS: -Od */\n#include <stdio.h>");   // blank lines are dropped
    CB_CHECK_EQ(s->body, "int\nf(a)\nint a;\n{\n    return g(a) + 1;\n}\n");
    // a preprocessor line after the preamble: not batched
    CB_CHECK(!batch::split("#include <a.h>\n#define X 1\nint f() {}\n").has_value());
    CB_CHECK(!batch::split("int f()\n{\n#define X 1\n}\n").has_value());
    // braces and parentheses that do not pair up (outside strings and comments): not batched
    CB_CHECK(!batch::split("int f()\n{\n    return 1;\n").has_value());
    CB_CHECK(!batch::split("int f(\n{\n}\n").has_value());
    // ... but inside a string, a comment or a character constant they are not counted
    CB_CHECK(batch::split("int f()\n{\n    s = \"}\"; /* { ( */ c = '{';\n}\n").has_value());
    // a file comment of another form is the body's first line
    CB_CHECK_EQ(batch::split("/* not a file comment */\nint f() {}\n")->preamble, "");
});

CB_TEST("exact.batch.own_names", [] {
    const std::set<std::string> f = batch::own_names("int\nf(a)\nint a;\n{\n    return g(a);\n}\n");
    CB_CHECK((f == std::set<std::string>{"f"}));
    // the function can have its type on the line before; static and struct lines are not it
    CB_CHECK((batch::own_names("static int h();\nunsigned long\nwork(a)\n{\n}\n") == std::set<std::string>{"work"}));
    // what it declares extern, but not the words of the declaration; a struct's name is not one
    CB_CHECK((batch::own_names("extern int count, tab[], (*fn)();\nextern struct s *sp;\nextern char buf[10];\nint\nf()\n{\n}\n") ==
              std::set<std::string>{"f", "count", "tab", "fn", "sp", "buf"}));
    // the structs it defines
    CB_CHECK((batch::own_names("struct node {\n    int v;\n};\nint\nf()\n{\n}\n") == std::set<std::string>{"f", "node"}));
    CB_CHECK(batch::own_names("int x;\n").empty());
});

CB_TEST("exact.batch.renamed_gives_each_name_a_suffix", [] {
    const std::string body = "extern int g();\nint\nf(a)\nint a;\n{\n    return g(a) + f(\"f g\") + fx;\n}\n";
    CB_CHECK_EQ(batch::renamed(body, 3),
                "extern int g__Q3();\nint\nf__Q3(a)\nint a;\n{\n    return g__Q3(a) + f__Q3(\"f g\") + fx;\n}\n");   // not inside a string, not a longer name
    CB_CHECK_EQ(batch::renamed("int x;\n", 1), "int x;\n");                                               // no names of its own: as it is
    CB_CHECK_EQ(batch::strip_suffix("f__Q12"), "f");
    CB_CHECK_EQ(batch::strip_suffix("f__Q"), "f__Q");
    CB_CHECK_EQ(batch::strip_suffix("a__Q1b"), "a__Q1b");
    CB_CHECK_EQ(batch::strip_suffix("a__Q1__Q2"), "a__Q1");
});

CB_TEST("exact.batch.build_makes_one_source", [] {
    const std::vector<std::string> srcs = {candidate("f", "g(a)"), candidate("f", "g(a) + 1"), "#include <x.h>\nint z() {}\n"};
    const auto b = batch::build(srcs);
    CB_CHECK(b.has_value());
    // the third has another preamble: left out
    CB_CHECK((b->candidates == std::vector<std::size_t>{0, 1}));
    CB_CHECK_EQ(b->spans.size(), 2u);
    CB_CHECK(b->text.starts_with("/* MFLAGS: -Od */\n#include <stdio.h>\n"));
    CB_CHECK(b->text.find("\nf__Q0(a)") != std::string::npos && b->text.find("\nf__Q1(a)") != std::string::npos);
    // each candidate's lines are where the span says
    const auto lines = text::split(b->text, '\n');
    CB_CHECK(lines[b->spans[1].first - 1].starts_with("extern int g__Q1();"));
    CB_CHECK(b->text.ends_with("}\n\n\n"));   // (each candidate ends with a blank line, and the text with a newline)
    CB_CHECK(!batch::build(std::vector<std::string>{candidate("f", "1")}).has_value());   // one is not a batch
});

CB_TEST("exact.batch.error_lines", [] {
    const auto lines = [](std::string_view con) { return batch::error_lines(con); };
    CB_CHECK((lines("T.C(5) : error C2000: bad\nT.C(7): error\n") == std::vector<std::size_t>{5, 7}));
    CB_CHECK(lines("T.C(5) : warning C4000: meh\n").empty());
    CB_CHECK((lines("T.C(1):error|T.C(2):warning|T.C(3): warning x|T.C(4) :error") == std::vector<std::size_t>{1, 4}));
    CB_CHECK((lines("t.c(9) : error") == std::vector<std::size_t>{9}));   // the case does not matter
    CB_CHECK(lines("fatal error: no file").empty());
    CB_CHECK((lines("T.C(3) : error\nT.C(3) : error\n") == std::vector<std::size_t>{3}));
});

namespace {

// A compiler that refuses a source with BAD on a line (an error blaming that line); otherwise it makes an
// "object": the text itself, which the renamer then gives back its names.
class FakeCompiler final : public batch::Toolchain {
public:
    int runs = 0;
    batch::CompileOutcome compile(ByteView source, const batch::CompileOptions& options) override {
        ++runs;
        const std::string text = text::from_latin1(source);
        std::string errors;
        std::size_t n = 0;
        for (std::string_view line : text::split(text, '\n')) {
            ++n;
            if (line.find("BAD") != std::string_view::npos) errors += std::format("T.C({}) : error C2000: bad\n", n);
        }
        if (!errors.empty()) return {std::nullopt, errors + "T.C(1) : warning C4000: w\n"};
        seen_headers = options.headers.size();
        return {to_bytes(text), ""};
    }
    std::size_t seen_headers = 0;
};

}  // namespace

CB_TEST("exact.batch.compile_many_leaves_out_what_the_compiler_refuses", [] {
    FakeCompiler cc;
    batch::CompileOptions options;
    options.headers["stdio.h"] = Bytes{1};
    const batch::SymbolRenamer rename = [](ByteView object, const std::function<std::string(std::string_view)>& fn) {
        // (stand-in for the OMF renamer: every f__Qn in the text goes through fn)
        std::string text(as_text(object)), out;
        for (std::size_t i = 0; i < text.size();) {
            std::size_t end = i;
            while (end < text.size() && (std::isalnum(static_cast<unsigned char>(text[end])) || text[end] == '_')) ++end;
            if (end == i) {
                out += text[i++];
                continue;
            }
            out += fn(std::string_view(text).substr(i, end - i));
            i = end;
        }
        return to_bytes(out);
    };
    const std::vector<std::string> srcs = {candidate("f", "g(a)"), candidate("f", "BAD(a)"), candidate("f", "g(a) + 2")};
    const auto done = batch::compile_many(cc, options, rename, srcs);
    CB_CHECK(done.has_value());
    CB_CHECK_EQ(cc.runs, 2);                       // the first run refused, the second took the two that were left
    CB_CHECK_EQ(cc.seen_headers, 1u);
    CB_CHECK_EQ(done->placed.size(), 2u);
    CB_CHECK_EQ(done->placed[0].source, 0u);
    CB_CHECK_EQ(done->placed[0].function, 0u);
    CB_CHECK_EQ(done->placed[1].source, 2u);       // candidate 2 is the object's second function
    CB_CHECK_EQ(done->placed[1].function, 1u);
    const std::string object(as_text(done->object));
    CB_CHECK(object.find("__Q") == std::string::npos);   // the names are given back
    // fewer than two left: nothing, the caller proves them alone
    CB_CHECK(!batch::compile_many(cc, options, rename, std::vector<std::string>{candidate("f", "BAD"), candidate("f", "BAD(2)"), candidate("f", "1")}).has_value());
});
