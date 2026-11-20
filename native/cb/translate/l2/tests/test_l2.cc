// test_l2.cc -- native/l2's unit tests (CTest): Python's semantics where the port leans on them (str,
// int(), repr, json, re), the L2 core's round trip and schema, and the C writer on small documents.
// The full gate -- every function of the gate modules, both paths -- is tools/l2c.py and the
// Python-side test (tests/translate/test_native_l2.py).
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#ifdef CBL2_CBACK
#include "cback.hpp"
#include "pyre.hpp"
#endif
#include "json.hpp"
#include "l2.hpp"
#include "pystr.hpp"

static int failures = 0, checks = 0;

#define CHECK_EQ(a, b)                                                                                     \
    do {                                                                                                   \
        checks++;                                                                                          \
        auto _a = (a);                                                                                     \
        auto _b = (b);                                                                                     \
        if (!(_a == _b)) {                                                                                 \
            failures++;                                                                                    \
            std::printf("FAIL %s:%d: %s\n   got:  %s\n   want: %s\n", __FILE__, __LINE__, #a,              \
                        show(_a).c_str(), show(_b).c_str());                                               \
        }                                                                                                  \
    } while (0)
#define CHECK(c)                                                                                           \
    do {                                                                                                   \
        checks++;                                                                                          \
        if (!(c)) {                                                                                        \
            failures++;                                                                                    \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c);                                       \
        }                                                                                                  \
    } while (0)

static std::string show(const std::string& s) { return py::str_repr(s); }
static std::string show(const char* s) { return py::str_repr(s); }
static std::string show(int64_t v) { return std::to_string(v); }
static std::string show(int v) { return std::to_string(v); }
static std::string show(size_t v) { return std::to_string(v); }
static std::string show(bool v) { return v ? "true" : "false"; }

static std::string readfile(const char* path) {
    std::ifstream f(path, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static void test_str() {
    CHECK_EQ(py::join("|", py::split_ws("  a b\t c \n")), std::string("a|b|c"));
    CHECK_EQ(py::join("|", py::split("a,,b", ",")), std::string("a||b"));
    CHECK_EQ(py::join("|", py::split("a+b+c", "+", 1)), std::string("a|b+c"));
    CHECK_EQ(py::strip("  x y \n"), std::string("x y"));
    CHECK_EQ(py::replace("aaa", "a", "b", 2), std::string("bba"));
    CHECK_EQ(py::int_parse("0x1c", 0), (int64_t)28);
    CHECK_EQ(py::int_parse("12", 0), (int64_t)12);
    CHECK_EQ(py::int_parse("000", 0), (int64_t)0);
    bool raised = false;
    try {
        py::int_parse("012", 0);
    } catch (py::PyError& e) {
        raised = e.type == "ValueError";
    }
    CHECK(raised);
    CHECK_EQ(py::int_parse("ff", 16), (int64_t)255);
    CHECK_EQ(py::float_repr(1.5), std::string("1.5"));
    CHECK_EQ(py::float_repr(1e16), std::string("1e+16"));
    CHECK_EQ(py::float_repr(123.0), std::string("123.0"));
    CHECK_EQ(py::float_repr(0.0001), std::string("0.0001"));
    CHECK_EQ(py::float_repr(1e-05), std::string("1e-05"));
    CHECK_EQ(py::float_repr(0.1), std::string("0.1"));
    CHECK_EQ(py::str_repr("it's"), std::string("\"it's\""));
    CHECK_EQ(py::str_repr("a\nb"), std::string("'a\\nb'"));
}

#ifdef CBL2_CBACK
static void test_re() {
    CHECK(re::Regex("(0x[0-9a-fA-F]+|\\d+)[uUlL]*").fullmatch("0x1fU"));
    CHECK(!re::Regex("(0x[0-9a-fA-F]+|\\d+)[uUlL]*").fullmatch("0x1fq"));
    CHECK_EQ(re::Regex("([\\w ]+?) ?\\*").fullmatch("int2 *").group(1), std::string("int2"));
    CHECK_EQ(re::Regex("(?<![\\w.>])[A-Za-z_]\\w*").findall("a.b->c d(e)").size(), (size_t)3);
    CHECK_EQ(re::Regex("\\b%s\\b").pattern(), std::string("\\b%s\\b"));
    CHECK_EQ(re::Regex("x*").sub("-", "abc"), std::string("-a-b-c-"));
    CHECK_EQ(re::Regex("(a)(b)?").sub("[\\2\\1]", "ac"), std::string("[a]c"));
    CHECK_EQ(re::Regex("(?m)^\\s+int2 unaff_BP;$").search("{\n  int2 unaff_BP;\n  x;") ? 1 : 0, 1);
    CHECK_EQ(re::Regex("/\\*.*?\\*/", re::S).sub("\"\"", "a /* b\n */ c /* d */"), std::string("a \"\" c \"\""));
    CHECK_EQ(re::Regex("(?:(?!__far)[\\w ])+?\\s*\\*").fullmatch("int2 __far *") ? 1 : 0, 0);
    CHECK_EQ(re::Regex("(?:(?!__far)[\\w ])+?\\s*\\*").fullmatch("int2 *") ? 1 : 0, 1);
    CHECK_EQ(re::Regex("&\\s*\\Z").search("f(&  ") ? 1 : 0, 1);
    CHECK_EQ(re::Regex("(?<!&)&\\s*\\Z").search("a && ") ? 1 : 0, 0);
    CHECK_EQ(re::Regex("(?m)^  (\\w+?)(\\d)?(\\s*\\*+\\s*|\\s+)(\\w*Stack_([0-9a-f]+))( \\[(\\d+)\\])?;\\s*$")
                 .finditer("\n  int2 iStack_4;\n  uint1 axStack_c [8];\n  ")
                 .back()
                 .group(0),
             std::string("  uint1 axStack_c [8];\n  "));
    CHECK_EQ(re::escape("a.b_c"), std::string("a\\.b_c"));
}

#endif

static void test_json() {
    std::string t = "{\"node\": \"Lit\", \"value\": 1.0, \"text\": \"\\u00e9\\ud83d\\ude00\", \"big\": 18446744073709551615, \"n\": -5}";
    CHECK_EQ(py::json_dump(py::json_parse(t)), t);
    py::V tagged = py::json_parse("[{\"$t\":[1,2]},{\"$s\":[3]},{\"$d\":[[true,{\"a1\":1}]]}]", true);
    CHECK(tagged[0].is_tuple());
    CHECK(tagged[1].is_set());
    CHECK(tagged[2].get(py::V(1)).is_dict());           // (True == 1, as Python's dict finds it)
    CHECK_EQ(py::json_tagged(tagged), std::string("[{\"$t\":[1,2]},{\"$s\":[3]},{\"$d\":[[true,{\"a1\":1}]]}]"));
}

static void test_l2() {
    // the round trip drops what ir.py does not keep and writes the slots in ir.py's order
    std::string doc =
        "{\"text\": \"x\", \"node\": \"Lit\", \"line\": 0, \"value\": 3, \"junk\": 1, \"type\": {\"kind\": \"ptr\", \"to\": {\"kind\": "
        "\"u8\"}, \"space\": \"far\", \"ref\": false, \"seg\": 0}}";
    std::string want = "{\"node\": \"Lit\", \"type\": {\"kind\": \"ptr\", \"space\": \"far\", \"to\": {\"kind\": \"u8\"}, \"seg\": 0}, "
                       "\"value\": 3, \"text\": \"x\"}";
    CHECK_EQ(py::json_dump(l2::roundtrip(py::json_parse(doc))), want);
    std::string m = "{\"node\": \"MethodDecl\", \"name\": \"f\", \"kind\": \"function\", \"params\": [], \"body\": [{\"node\": \"Return\"}], "
                    "\"sym\": {\"kind\": \"function\", \"name\": \"f\", \"static\": 1}}";
    CHECK_EQ(py::json_dump(l2::roundtrip(py::json_parse(m))),
             std::string("{\"node\": \"MethodDecl\", \"sym\": {\"kind\": \"function\", \"name\": \"f\", \"static\": true, \"params\": []}, "
                         "\"name\": \"f\", \"kind\": \"function\", \"params\": [], \"body\": [{\"node\": \"Return\"}]}"));
    std::string schema = readfile(L2_SCHEMA);
    if (!schema.empty()) {
        l2::Schema s(py::json_parse(schema));
        std::string why;
        CHECK(!s.valid(py::json_parse(m), &why));          // (static: 1 is no boolean, as jsonschema says)
        CHECK(s.valid(py::json_parse(py::replace(m, "\"static\": 1", "\"static\": true"))));
        CHECK(!s.valid(py::json_parse("{\"node\": \"Bogus\"}"), &why));
    }
}

#ifdef CBL2_CBACK
static void test_c() {
    std::string doc =
        "{\"node\": \"MethodDecl\", \"name\": \"sub_1_0000\", \"kind\": \"function\", \"origin\": {\"module\": \"m\", \"segment\": 1, \"offset\": 0},"
        " \"ret\": {\"kind\": \"i16\"}, \"params\": [{\"node\": \"Param\", \"sym\": {\"kind\": \"param\", \"name\": \"a1\"}, \"type\": "
        "{\"kind\": \"i16\"}}], \"mods\": [\"__cdecl16far\"], \"body\": [{\"node\": \"Local\", \"sym\": {\"kind\": \"local\", \"name\": "
        "\"iVar1\"}, \"type\": {\"kind\": \"i16\"}}, {\"node\": \"ExprStmt\", \"expr\": {\"node\": \"Assign\", \"target\": {\"node\": \"Name\","
        " \"sym\": {\"kind\": \"local\", \"name\": \"iVar1\"}}, \"value\": {\"node\": \"Binary\", \"op\": \"+\", \"left\": {\"node\": \"Name\","
        " \"sym\": {\"kind\": \"param\", \"name\": \"a1\"}}, \"right\": {\"node\": \"Lit\", \"value\": 4096}}}}, {\"node\": \"Return\", "
        "\"expr\": {\"node\": \"Name\", \"sym\": {\"kind\": \"local\", \"name\": \"iVar1\"}}}]}";
    std::string c = cb::function(py::json_parse(doc), py::V::list(), py::V(), py::V());
    CHECK_EQ(c, std::string("int2 __cdecl16far sub_1_0000(int2 a1)\n\n{\n  int2 iVar1;\n  \n  iVar1 = a1 + 0x1000;\n  return iVar1;\n}\n"));
}

#endif

int main() {
    test_str();
#ifdef CBL2_CBACK
    test_re();
#endif
    test_json();
    test_l2();
#ifdef CBL2_CBACK
    test_c();
#endif
    std::printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
