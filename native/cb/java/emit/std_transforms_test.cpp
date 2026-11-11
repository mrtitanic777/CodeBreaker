// DIVERGES (checked against the Python on its 544 recorded cases: apply_std_transforms, sanitize_reserved_members and the
// literal-masked substitution agree on all of them):
//   * annotate_first with an EMPTY pattern: the Python split the line on it and raised ValueError("empty separator"); it now
//     finds nothing and returns the text unchanged (32 of the recorded cases).
//   * `\b` and `\w` are ASCII (docs/CPP_DESIGN.md 3.7): a NULL next to a non-ASCII letter ("\xC3\xA9NULL") is a whole word
//     here, not part of a longer one as in Python. Emitted code does not have such neighbours.

#include <string>

#include "cb/core/testing.hpp"
#include "cb/java/emit/std_transforms.hpp"

namespace {

using namespace cb::java::emit::std_transforms;

CB_TEST("java.emit.std_transforms.before_cpp11_untouched", [] {
    const std::string text = "if (x == NULL) { static const int A = 1; }\nstd::map<int,int>::iterator it = m.begin();\n";
    CB_CHECK_EQ(apply_std_transforms(text, 2003), text);
    CB_CHECK_EQ(apply_std_transforms(text, 0), text);
    CB_CHECK_EQ(apply_std_transforms(text, 2010), text);
});

CB_TEST("java.emit.std_transforms.null_to_nullptr", [] {
    // outside literals only; whole words only; the first occurrence is annotated
    const std::string in = "if (p == NULL) return NULL;\nconst char* s = \"NULL\"; char c = 'NULL';\nint NULLABLE = 1; NULL_x();\n";
    const std::string out = apply_std_transforms(in, 2011);
    CB_CHECK_EQ(out,
                "if (p == nullptr) return nullptr;  // [C++11] nullptr replaces NULL\n"
                "const char* s = \"NULL\"; char c = 'NULL';\n"
                "int NULLABLE = 1; NULL_x();\n");
    // no NULL, no annotation
    CB_CHECK_EQ(apply_std_transforms("int x = 1;\n", 2011), "int x = 1;\n");
});

CB_TEST("java.emit.std_transforms.null_annotation_skips_lines_with_comments", [] {
    const std::string out = apply_std_transforms("a = NULL; // keep\nb = NULL;\n", 2014);
    // the first line already has a comment after `nullptr`, so the next line carries the note
    CB_CHECK_EQ(out, "a = nullptr; // keep\nb = nullptr;  // [C++11] nullptr replaces NULL\n");
});

CB_TEST("java.emit.std_transforms.iterators", [] {
    const std::string in =
        "void f() {\n"
        "    for (std::map<int, std::vector<int>>::iterator it = m.begin(); it != m.end(); ++it) {}\n"
        "    for(std::vector<int>::const_iterator j = v.begin(); j != v.end(); ++j) {}\n"
        "    std::set<int>::iterator k = s.begin();\n"
        "    const list<Foo>::const_iterator q = l.begin();\n"
        "    MyMap<int>::iterator other = x;\n"
        "    for (std::map<int, std::vector<int> >::iterator spaced = m.begin();;) {}\n"
        "}\n";
    const std::string out = apply_std_transforms(in, 2011);
    CB_CHECK_EQ(out,
                "void f() {\n"
                "    for(auto it = m.begin(); it != m.end(); ++it) {}  // [C++11] auto type deduction\n"
                "    for(auto j = v.begin(); j != v.end(); ++j) {}\n"
                "    auto k = s.begin();\n"
                "    auto q = l.begin();\n"
                "    MyMap<int>::iterator other = x;\n"      // not a std container: left alone
                // as the original: the template argument list may nest one level, and `> >` with a space is not `>>`
                "    for (std::map<int, std::vector<int> >::iterator spaced = m.begin();;) {}\n"
                "}\n");
});

CB_TEST("java.emit.std_transforms.static_const_int", [] {
    const std::string in = "struct E {\n    static const int A = 1;\n    static  const\tint B = 2;\n    const char* s = \"static const int\";\n};\n";
    const std::string out = apply_std_transforms(in, 2011);
    CB_CHECK_EQ(out,
                "struct E {\n"
                "    static constexpr int A = 1;  // [C++11] constexpr replaces const\n"
                "    static constexpr int B = 2;\n"
                "    const char* s = \"static const int\";\n"
                "};\n");
});

CB_TEST("java.emit.std_transforms.sanitize_reserved_members", [] {
    CB_CHECK_EQ(sanitize_reserved_members("Enum::NULL, Enum::TRUE; E::INTERFACE x; a::IN b::INX c::OUT"),
                "Enum::NULL_, Enum::TRUE_; E::INTERFACE_ x; a::IN_ b::INX c::OUT_");
    CB_CHECK_EQ(sanitize_reserved_members("E::NULL_ E::Null E::null ::"), "E::NULL_ E::Null E::null ::");
    const std::string once = sanitize_reserved_members("X::ERROR");
    CB_CHECK_EQ(sanitize_reserved_members(once), once);                          // idempotent
    CB_CHECK_EQ(sanitize_reserved_members(""), "");
    CB_CHECK_EQ(sanitize_reserved_members("NULL"), "NULL");                      // no `::` in front: not an enum reference
});

CB_TEST("java.emit.std_transforms.map_outside_literals", [] {
    auto upper = [](std::string_view piece) {
        std::string s(piece);
        for (char& c : s) c = static_cast<char>(c >= 'a' && c <= 'z' ? c - 32 : c);
        return s;
    };
    CB_CHECK_EQ(map_outside_literals("a \"b\" c 'd' e", upper), "A \"b\" C 'd' E");
    CB_CHECK_EQ(map_outside_literals("x \"a\\\"b\" y", upper), "X \"a\\\"b\" Y");     // an escaped quote stays inside
    CB_CHECK_EQ(map_outside_literals("it's \"open", upper), "IT'S \"OPEN");           // unclosed quotes are not literals
    CB_CHECK_EQ(map_outside_literals("", upper), "");
});

CB_TEST("java.emit.std_transforms.annotate_first", [] {
    CB_CHECK_EQ(annotate_first("a\nuse X here   \nuse X again\n", "X", "// n"), "a\nuse X here  // n\nuse X again\n");
    CB_CHECK_EQ(annotate_first("use X // c\nuse X\n", "X", "// n"), "use X // c\nuse X  // n\n");
    CB_CHECK_EQ(annotate_first("none\n", "X", "// n"), "none\n");
    CB_CHECK_EQ(annotate_first("tail X", "X", "// n"), "tail X  // n");
    CB_CHECK_EQ(annotate_first("", "X", "// n"), "");
    // DIVERGES: an empty pattern. The Python split the line on it and raised "empty separator"; it now finds nothing.
    CB_CHECK_EQ(annotate_first("a\nb\n", "", "// n"), "a\nb\n");
});

}  // namespace
