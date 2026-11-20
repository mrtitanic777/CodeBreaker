#include "decompile/jar.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace jar = cb::decompile::jar;
namespace fs = cb::fs;
using std::filesystem::path;

// Notes:
//  - repair_array_params works on bytes: a source with bytes that are not UTF-8 keeps them and line ends are untouched;
//  - the PATH search does not look in the current directory first;
//  - settings arrive as a jar::Settings value instead of being read from settings.json inside the module;
//  - one `src` directory wins a tie by name order.
// (These tests run the flows against a batch-file fake `java`.)

namespace {

void put(const path& root, const std::string& rel, const std::string& content = "") {
    fs::write_text(root / path(rel), content);
}

std::string get(const path& root, const std::string& rel) { return fs::read_text(root / path(rel)); }

// The mangled forms RetroMCP made of Indev 20100213 array parameters (Fernflower's body keeps `varN`).
const char* kMangled =
    "class A {\n"
    "  void f(Block[] block;4, int x) { var4/* $VF was: block;4 */.foo(); }\n"
    "  void g(byte[] [B4) { }\n"
    "  void h(String[][] m;[Ljava.lang.String;7, int y) { g(var7/* $VF was: m;7 */); }\n"
    "  void ok(int[] arr, int n) {}\n"
    "  void two(Foo[] a;1, Bar[][] b;[[LBar;2) {}\n"
    "}\n";

const char* kRepaired =
    "class A {\n"
    "  void f(Block[] var4, int x) { var4.foo(); }\n"
    "  void g(byte[] var4) { }\n"
    "  void h(String[][] var7, int y) { g(var7); }\n"
    "  void ok(int[] arr, int n) {}\n"
    "  void two(Foo[] var1, Bar[][] var2) {}\n"
    "}\n";

std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> out;
    for (std::string_view piece : cb::text::split(s, sep)) out.emplace_back(piece);
    return out;
}

}  // namespace

CB_TEST("decompile.jar.find_src", [] {
    fs::ScopedTempDir tmp("cb_jar_test");
    const path& w = tmp.path();
    // the src with the most .java files wins; a directory that is not named src does not count
    put(w, "findsrc1/a/src/x.java");
    put(w, "findsrc1/b/src/p.java");
    put(w, "findsrc1/b/src/q.java");
    put(w, "findsrc1/b/src/sub/r.java");
    put(w, "findsrc1/c/notsrc/z.java");
    put(w, "findsrc1/c/notsrc/y.java");
    put(w, "findsrc1/c/notsrc/x.java");
    put(w, "findsrc1/c/notsrc/w.java");
    auto found = jar::find_src(w / "findsrc1");
    CB_CHECK(found && fs::same_path(*found, w / "findsrc1/b/src"));
    // a tie goes to the first by name
    put(w, "tie/s2/src/b.java");
    put(w, "tie/s1/src/a.java");
    found = jar::find_src(w / "tie");
    CB_CHECK(found && fs::same_path(*found, w / "tie/s1/src"));
    // a src with no .java files, or none at all
    put(w, "nojava/src/readme.txt");
    CB_CHECK(!jar::find_src(w / "nojava"));
    fs::make_dirs(w / "empty");
    CB_CHECK(!jar::find_src(w / "empty"));
    CB_CHECK(!jar::find_src(w / "nonexistent"));
    // the workspace itself named src
    put(w, "src_is_root/src/m.java");
    found = jar::find_src(w / "src_is_root/src");
    CB_CHECK(found && fs::same_path(*found, w / "src_is_root/src"));
    // a src inside a src: the outer one counts its whole subtree and wins
    put(w, "nest/src/a.java");
    put(w, "nest/src/inner/src/b.java");
    found = jar::find_src(w / "nest");
    CB_CHECK(found && fs::same_path(*found, w / "nest/src"));
    // .java is matched case-sensitively and only on files
    put(w, "case/src/A.JAVA");
    fs::make_dirs(w / "case/src/dir.java");
    CB_CHECK(!jar::find_src(w / "case"));
});

CB_TEST("decompile.jar.repair_text", [] {
    std::size_t n = 0;
    CB_CHECK_EQ(jar::repair_array_params_text(kMangled, n), kRepaired);
    CB_CHECK_EQ(n, 5u);
    // nothing to repair: the text comes back as it was, comments included
    const std::string plain = "class N { void f(int[] a, int b) {} /* $VF was: z */ }";
    CB_CHECK_EQ(jar::repair_array_params_text(plain, n), plain);
    CB_CHECK_EQ(n, 0u);
    // a parameter that is not mangled is not touched; arrays of any depth, a descriptor-style name
    CB_CHECK_EQ(jar::repair_array_params_text("void f(int[][] a, Foo[] b;3) {}", n), "void f(int[][] a, Foo[] var3) {}");
    CB_CHECK_EQ(n, 1u);
    CB_CHECK_EQ(jar::repair_array_params_text("void f(char[] [C12)", n), "void f(char[] var12)");
    // the name must end the parameter: a digit run followed by something else is not one
    CB_CHECK_EQ(jar::repair_array_params_text("x(Foo[] a;3 y)", n), "x(Foo[] a;3 y)");
    CB_CHECK_EQ(n, 0u);
    // comments are dropped only when a repair happened, and only the `$VF was` form
    CB_CHECK_EQ(jar::repair_array_params_text("g(Foo[] a;4) { var4/* $VF was: a;4 */; /* keep */ }", n),
                "g(Foo[] var4) { var4; /* keep */ }");
    // bytes that are not UTF-8, and non-ASCII text, pass through unchanged
    const std::string raw = "class D { void g(byte[] [B9) { var9/* $VF was: x */; } \xFF\xFE \xC3\xA9\xE4\xB8\xAD }";
    CB_CHECK_EQ(jar::repair_array_params_text(raw, n), "class D { void g(byte[] var9) { var9; } \xFF\xFE \xC3\xA9\xE4\xB8\xAD }");
    // line ends are kept as they are
    CB_CHECK_EQ(jar::repair_array_params_text("a(Foo[] b;1)\r\n{}\r\n", n), "a(Foo[] var1)\r\n{}\r\n");
    // an unterminated comment prefix is left alone
    CB_CHECK_EQ(jar::repair_array_params_text("a(Foo[] b;1) /* $VF was: x ", n), "a(Foo[] var1) /* $VF was: x ");
});

CB_TEST("decompile.jar.repair_tree", [] {
    fs::ScopedTempDir tmp("cb_jar_test");
    const path& w = tmp.path();
    put(w, "A.java", kMangled);
    put(w, "sub/B.java", kMangled);
    put(w, "sub/C.java", "class C {}\n");
    put(w, "sub/D.txt", "Block[] block;4, int x\n");      // not a .java file
    put(w, "none.java", "class N { void f(int[] a, int b) {} /* $VF was: z */ }");
    CB_CHECK_EQ(jar::repair_array_params(w), 10u);
    CB_CHECK_EQ(get(w, "A.java"), kRepaired);
    CB_CHECK_EQ(get(w, "sub/B.java"), kRepaired);
    CB_CHECK_EQ(get(w, "sub/C.java"), "class C {}\n");
    CB_CHECK_EQ(get(w, "sub/D.txt"), "Block[] block;4, int x\n");
    CB_CHECK_EQ(get(w, "none.java"), "class N { void f(int[] a, int b) {} /* $VF was: z */ }");
    CB_CHECK_EQ(jar::repair_array_params(w), 0u);        // idempotent
    CB_CHECK_EQ(jar::repair_array_params(w / "missing"), 0u);
});

CB_TEST("decompile.jar.find_java", [] {
    CB_WINDOWS_ONLY("it runs java.exe stand-ins written as .cmd files");
    fs::ScopedTempDir tmp("cb_jar_test");
    const path& d = tmp.path();
    put(d, "home/bin/java.exe", "x");
    put(d, "pathdir/java.exe", "x");
    put(d, "mine/java.exe", "x");
    put(d, "other/javaw.exe", "x");
    fs::make_dirs(d / "nothing");
    jar::Settings s;
    s.java = d / "mine/java.exe";
    // settings.java first, then an explicit one wins over it
    {
        fs::ScopedEnv home("JAVA_HOME", (d / "home").string());
        auto got = jar::find_java(s);
        CB_CHECK(got && fs::same_path(*got, d / "mine/java.exe"));
        got = jar::find_java(s, d / "other/javaw.exe");
        CB_CHECK(got && fs::same_path(*got, d / "other/javaw.exe"));
        // an explicit path that does not exist falls through to the next source (settings are not consulted)
        got = jar::find_java(s, d / "nowhere.exe");
        CB_CHECK(got && fs::same_path(*got, d / "home/bin/java.exe"));
    }
    s.java = d / "nothere.exe";
    {
        fs::ScopedEnv home("JAVA_HOME", (d / "home").string());
        auto got = jar::find_java(s);
        CB_CHECK(got && fs::same_path(*got, d / "home/bin/java.exe"));
    }
    {
        fs::ScopedEnv home("JAVA_HOME", (d / "nowhere").string());
        fs::ScopedEnv path_var("PATH", (d / "pathdir").string());
        auto got = jar::find_java(s);
        CB_CHECK(got && fs::same_path(*got, d / "pathdir/java.exe"));
    }
    {
        fs::ScopedEnv home("JAVA_HOME", std::nullopt);
        fs::ScopedEnv path_var("PATH", (d / "nothing").string() + ";" + (d / "pathdir").string());   // second entry has it
        auto got = jar::find_java(s);
        CB_CHECK(got && fs::same_path(*got, d / "pathdir/java.exe"));
    }
    {
        fs::ScopedEnv home("JAVA_HOME", (d / "nowhere").string());
        fs::ScopedEnv path_var("PATH", (d / "nothing").string());
        CB_CHECK(!jar::find_java(s));
    }
});

CB_TEST("decompile.jar.decompile_without_a_version_set_up", [] {
    // no setup asked for, nothing in the workspace: no sides to decompile, so no source (the native preparation itself is proven against RetroMCP's
    // workspaces of six versions, which need the network: work/retire-retromcp)
    fs::ScopedTempDir tmp("cb_jar_test");
    jar::DecompileOptions options;
    options.setup = false;
    std::string message;
    try {
        (void)jar::decompile("a1.2.6", tmp.path() / "ws", options);
    } catch (const jar::DecompileError& e) {
        message = e.what();
    }
    CB_CHECK(message.find("Decompile produced no Java source under ") != std::string::npos);
    CB_CHECK(std::filesystem::is_directory(tmp.path() / "ws"));   // the workspace is made
    CB_CHECK_THROWS_AS(cb::Error, throw jar::DecompileError("x"));
});
