// Tests of cb::xna::fixups. The cases are decompiled-C# shapes (ILSpy's output for an XNA game); every expected text
// was worked out by hand from the rewrite rules and agrees with the reference implementation's answer.
#include "cb/xna/fixups.hpp"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

// A comparison of containers (a failure message cannot show them): the two are equal or the check fails.
#define CB_CHECK_SAME(a, ...) CB_CHECK((a) == (__VA_ARGS__))

namespace {

using namespace cb::xna::fixups;
namespace stdfs = std::filesystem;

struct RefCase {
    const char* in;
    const char* out;
    std::size_t n;
};

struct CtorCase {
    const char* in;
    const char* out;
    std::size_t n;
    std::vector<Unplaced> left;
};

stdfs::path data(const char* name) { return cb::testing::data_path(std::string("xna/testdata/fixups_tree/") + name); }

}  // namespace

CB_TEST("xna.fixups.ref_casts", [] {
    const RefCase cases[] = {
        {"((Color)(ref white)).A = 64;", "(white).A = 64;", 1},
        // a real conversion (to the interface) stays: only the cast to the value's own type goes
        {"x = ((IDisposable)(Enumerator)(ref e)).Dispose();", "x = ((IDisposable)e).Dispose();", 1},
        {"((Vector2)(ref v)).X = 1f;", "(v).X = 1f;", 1},
        {"((Foo.Bar)(ref a.b[0].c)).Run();", "(a.b[0].c).Run();", 1},
        {"((int)(ref x));", "(x);", 1},
        {"(Color) ( ref   p[1] )", "p[1]", 1},
        {"( System.Int32 ) (ref  q )", "q", 1},
        {"((Matrix)(ref m.Left.Up)).M11", "(m.Left.Up).M11", 1},
        {"(Color)(ref white[ 3 ].A . B)", "white[ 3 ].A . B", 1},
        {"(A)(B)(C)(ref z)", "(A)(B)z", 1},
        {"((Color)(ref a)).X + ((Color)(ref b)).Y", "(a).X + (b).Y", 2},
        // not casts of a ref'd lvalue
        {"(ref x)", "(ref x)", 0},
        {"(T)(ref a b)", "(T)(ref a b)", 0},
        {"(T)(ref 1x)", "(T)(ref 1x)", 0},
        {"((List<int>)(ref l)).Add(1);", "((List<int>)(ref l)).Add(1);", 0},       // a generic type is not a plain name
        {"(Color)(ref white[a[1]])", "(Color)(ref white[a[1]])", 0},               // an index holds no brackets of its own
        {"", "", 0},
        {"no cast at all (a + b)", "no cast at all (a + b)", 0},
    };
    for (const RefCase& c : cases) {
        const Rewritten r = ref_casts(c.in);
        CB_CHECK_EQ(r.text, std::string(c.out));
        CB_CHECK_EQ(r.count, c.n);
    }
});

CB_TEST("xna.fixups.constructors", [] {
    const CtorCase cases[] = {
        // a base call becomes the initializer; a call on a declared value becomes `new`
        {"class A : B {\n    public A(int x)\n    {\n        base._002Ector(x);\n        Vector2 v = default;\n        v._002Ector(1f, 2f);\n    }\n}\n",
         "class A : B {\n    public A(int x) : base(x)\n    {\n        Vector2 v = default;\n        v = new Vector2(1f, 2f);\n    }\n}\n", 3, {}},
        {"public class C\n{\n    public C()\n    {\n        this._002Ector(1, (2 + 3));\n        int k;\n    }\n}\n",
         "public class C\n{\n    public C() : this(1, (2 + 3))\n    {\n        int k;\n    }\n}\n", 2, {}},
        {"struct S\n{\n    public int a;\n    public S(int a)\n    {\n        this = default(S);\n        this._002Ector(a);\n    }\n}\n",
         "struct S\n{\n    public int a;\n    public S(int a) : this(a)\n    {\n        this = default(S);\n    }\n}\n", 2, {}},
        // a declared type is found above the call; the nearest declaration wins; an undeclared value is listed
        {"class F\n{\n    void M()\n    {\n        Foo f;\n        f._002Ector(a, b(c(d)));\n        Bar g = null;\n        g._002Ector();\n        h._002Ector(1);\n    }\n}\n",
         "class F\n{\n    void M()\n    {\n        Foo f;\n        f = new Foo(a, b(c(d)));\n        Bar g = null;\n        g = new Bar();\n        h._002Ector(1);\n    }\n}\n", 2,
         {{9, "no declaration of h before it"}}},
        {"Foo a;\nFoo a;\na._002Ector(1);\nFoo b = new Foo();\na = 4;\nFoo c =\n0;\n", "Foo a;\nFoo a;\na = new Foo(1);\nFoo b = new Foo();\na = 4;\nFoo c =\n0;\n", 1, {}},
        {"  Foo[] y;\n  y._002Ector(1)\n", "  Foo[] y;\n  y = new Foo[](1)\n", 1, {}},
        // a parenthesised value, a call over two lines, an unclosed one (left alone, not an error)
        {"class I\n{\n    void M()\n    {\n        Vector3 v;\n        Vector3 w = new Vector3();\n        (w)._002Ector(1f, 2f, 3f);\n        v._002Ector(1f,\n 2f, 3f);\n        List<int> xs = null;\n        xs._002Ector((1 + 2);\n    }\n}\n",
         "class I\n{\n    void M()\n    {\n        Vector3 v;\n        Vector3 w = new Vector3();\n        w = new Vector3(1f, 2f, 3f);\n        v = new Vector3(1f,\n 2f, 3f);\n        List<int> xs = null;\n        xs._002Ector((1 + 2);\n    }\n}\n", 2, {}},
        // `==` is not a declaration's `=`; generic and array types
        {"class J\n{\n    void M()\n    {\n        int a, b;\n        a = b == 1 ? 1 : 2;\n        Foo x = b;\n        x._002Ector(2);\n        x == 3;\n        Dictionary<string, List<int>> d;\n        d._002Ector();\n        T[] arr;\n        arr._002Ector(3);\n    }\n}\n",
         "class J\n{\n    void M()\n    {\n        int a, b;\n        a = b == 1 ? 1 : 2;\n        Foo x = b;\n        x = new Foo(2);\n        x == 3;\n        Dictionary<string, List<int>> d;\n        d = new Dictionary<string, List<int>>();\n        T[] arr;\n        arr = new T[](3);\n    }\n}\n", 3, {}},
        // a constructor written on one line: the call goes, the rest of the line stays
        {"class K\n{\n    public K(int p) { base._002Ector(p); }\n    public K() { this._002Ector(0); }\n}\n",
         "class K\n{\n    public K(int p) : base(p) {  }\n    public K() : this(0) {  }\n}\n", 4, {}},
        {"static class N { static N() { base._002Ector(); } }", "static class N { static N() : base() {  } }", 2, {}},
        {"class G { G() { base._002Ector( 5 ); } }\n", "class G { G() : base( 5 ) {  } }\n", 2, {}},
        // a base call over two lines goes whole
        {"class P\n{\n    P(\n int x,\n  int y)\n    {\n        base._002Ector(x,\n y);\n        z = 1;\n    }\n}\n",
         "class P\n{\n    P(\n int x,\n  int y) : base(x,\n y)\n    {\n        z = 1;\n    }\n}\n", 2, {}},
        {"class P\n{\n    P(\n int x,\n  int y)\n    {\n        base._002Ector(x, y);\n    }\n}\n", "class P\n{\n    P(\n int x,\n  int y) : base(x, y)\n    {\n    }\n}\n", 2, {}},
        // a member that is called _002Ector is a call on `this`
        {"class Q { int _002Ector; Q(int e) { this._002Ector(e); } }", "class Q { int _002Ector; Q(int e) : this(e) {  } }", 2, {}},
        // what cannot be placed: no parameter list before it; in a statement block; the constructor has an initializer already
        {"base._002Ector(1);\n", "base._002Ector(1);\n", 0, {{1, "no constructor parameter list before it"}}},
        {"}\nbase._002Ector(1);\n{\n", "}\nbase._002Ector(1);\n{\n", 0, {{2, "no constructor parameter list before it"}}},
        {"class H {\n  H(int a) {\n    if (a) {\n      base._002Ector(a);\n    }\n  }\n}\n", "class H {\n  H(int a) {\n    if (a) {\n      base._002Ector(a);\n    }\n  }\n}\n", 0,
         {{4, "it is inside a statement block, not at the top of a constructor"}}},
        {"class D : E\n{\n    public D(string s, int n) : base()\n    {\n        ((E)this)._002Ector(s);\n        n = 1;\n    }\n}\n",
         "class D : E\n{\n    public D(string s, int n) : base()\n    {\n        ((E)this)._002Ector(s);\n        n = 1;\n    }\n}\n", 0,
         {{5, "the constructor already has an initializer"}}},
        {"a._002Ector(1); b._002Ector(2);", "a._002Ector(1); b._002Ector(2);", 0, {{1, "no declaration of a before it"}, {1, "no declaration of b before it"}}},
        {"x._002Ector(unclosed(\n", "x._002Ector(unclosed(\n", 0, {}},
        {"nothing here\n", "nothing here\n", 0, {}},
        {"", "", 0, {}},
        {"_002Ector", "_002Ector", 0, {}},
    };
    for (const CtorCase& c : cases) {
        const ConstructorsResult r = constructors(c.in);
        CB_CHECK_EQ(r.text, std::string(c.out));
        CB_CHECK_EQ(r.count, c.n);
        CB_CHECK_EQ(r.left.size(), c.left.size());
        for (std::size_t i = 0; i < std::min(r.left.size(), c.left.size()); ++i) {
            CB_CHECK_EQ(r.left[i].line, c.left[i].line);
            CB_CHECK_EQ(r.left[i].why, c.left[i].why);
        }
    }
});

CB_TEST("xna.fixups.cast_this_form_is_the_base_constructor", [] {
    const ConstructorsResult r = constructors("class R\n{\n    R() {\n        ((Foo.Base2)this)._002Ector(2, 3);\n    }\n}\n");
    CB_CHECK_EQ(r.text, std::string("class R\n{\n    R() : base(2, 3) {\n    }\n}\n"));
    CB_CHECK_EQ(r.count, std::size_t{2});
});

CB_TEST("xna.fixups.names_in_other_scripts", [] {
    // an obfuscator's names: every byte of a UTF-8 letter is part of the name
    const ConstructorsResult r = constructors("Vector2 \xC3\xA9" "a;\n\xC3\xA9" "a._002Ector(1);\n");
    CB_CHECK_EQ(r.text, std::string("Vector2 \xC3\xA9" "a;\n\xC3\xA9" "a = new Vector2(1);\n"));
    CB_CHECK_EQ(ref_casts("((V)(ref \xC3\xA9x)).A").text, std::string("(\xC3\xA9x).A"));
});

CB_TEST("xna.fixups.data_holders", [] {
    // the blob structs of the compiler's static-data holder become internal (C# refuses a private type in a field)
    const Rewritten a = data_holders("internal class X\n{\n    private struct Blob16 { }\n    internal static Blob16 A/* Not supported: data(00) */;\n}");
    CB_CHECK_EQ(a.text, std::string("internal class X\n{\n    internal struct Blob16 { }\n    internal static Blob16 A/* Not supported: data(00) */;\n}"));
    CB_CHECK_EQ(a.count, std::size_t{1});
    // without the marker nothing changes
    CB_CHECK_EQ(data_holders("private struct S { }").text, std::string("private struct S { }"));
    CB_CHECK_EQ(data_holders("private struct S { }").count, std::size_t{0});
    const Rewritten b = data_holders(
        "/* Not supported: data(01) */\nprivate struct A { }\nprivate   struct  B {}\nprivate class C {}\nprivate\nstruct D { }\npublic private struct E\nprivate structs\nxprivate struct F");
    CB_CHECK_EQ(b.text, std::string("/* Not supported: data(01) */\ninternal struct A { }\ninternal   struct  B {}\nprivate class C {}\ninternal\nstruct D { }\npublic internal struct E\nprivate structs\nxprivate struct F"));
    CB_CHECK_EQ(b.count, std::size_t{4});
    CB_CHECK_EQ(data_holders("").count, std::size_t{0});
    CB_CHECK_EQ(data_holders("/* Not supported: data(00) */").count, std::size_t{0});
});

CB_TEST("xna.fixups.apply_over_a_folder", [] {
    // in/ is a decompiled project (BOM, CRLF and LF files, nested folders, a .txt and a .CS the rewrite must not touch);
    // out/ is what the reference implementation made of it, line endings kept.
    cb::fs::ScopedTempDir tmp("fixups");
    const stdfs::path work = tmp.path() / "tree";
    cb::fs::copy_tree(data("in"), work);
    const Report report = apply(work);
    CB_CHECK_EQ(report.ref_casts, std::size_t{3});
    CB_CHECK_EQ(report.constructors, std::size_t{10});
    CB_CHECK_EQ(report.data_holders, std::size_t{6});

    // the report the reference gave, one line per fact
    std::vector<std::string> facts;
    for (const LeftInFile& l : report.left) facts.push_back("left " + l.file + " " + std::to_string(l.line) + " " + l.why);
    std::vector<std::string> want;
    const std::string report_text = cb::fs::read_text(data("report.txt"));   // (the lines are views into it)
    for (std::string_view line : cb::text::lines(report_text))
        if (line.starts_with("left ")) want.emplace_back(line);
    CB_CHECK_SAME(facts, want);

    std::size_t files = 0;
    for (const auto& e : stdfs::recursive_directory_iterator(data("out"))) {
        if (!e.is_regular_file()) continue;
        const stdfs::path rel = stdfs::relative(e.path(), data("out"));
        CB_CHECK_SAME(cb::fs::read_file(work / rel), cb::fs::read_file(e.path()));   // (file in the folder)
        ++files;
    }
    CB_CHECK_EQ(files, std::size_t{12});
    // a file with nothing to do keeps its bytes (and its BOM); a changed one loses the BOM, keeps its line endings
    CB_CHECK_SAME(cb::fs::read_file(work / "sub/deeper/c.cs"), cb::fs::read_file(data("in/sub/deeper/c.cs")));
    CB_CHECK_SAME(cb::fs::read_file(work / "sub/deeper/e.CS"), cb::fs::read_file(data("in/sub/deeper/e.CS")));
    const cb::Bytes changed = cb::fs::read_file(work / "a.cs");
    CB_CHECK(!cb::as_text(changed).starts_with("\xEF\xBB\xBF"));
    const cb::Bytes crlf = cb::fs::read_file(work / "sub/b.cs");
    CB_CHECK(cb::as_text(crlf).find("\r\n") != std::string_view::npos &&
             cb::text::replace_all(cb::as_text(crlf), "\r\n", "").find('\n') == std::string::npos);
});

CB_TEST("xna.fixups.apply_to_an_empty_or_missing_folder", [] {
    cb::fs::ScopedTempDir tmp("fixups");
    const Report r = apply(tmp.path());
    CB_CHECK(r.ref_casts == 0 && r.constructors == 0 && r.data_holders == 0 && r.left.empty());
    CB_CHECK_THROWS_AS(cb::NotFoundError, apply(tmp.path() / "nothing here"));
});
