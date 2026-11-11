#include "cb/decompile/jar.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace jar = cb::decompile::jar;
namespace fs = cb::fs;
using std::filesystem::path;

// DIVERGES from the Python module:
//  - repair_array_params works on bytes: a source with bytes that are not UTF-8 keeps them (Python decoded with
//    errors="replace", so a repaired file came back with U+FFFD in their place) and line ends are untouched (Python's
//    text mode rewrote a repaired file with the platform's CRLF);
//  - the PATH search does not look in the current directory first (shutil.which did);
//  - settings arrive as a jar::Settings value instead of being read from settings.json inside the module;
//  - one `src` directory wins a tie by name order (os.walk's order was the file system's).
// (The old harness, 6699-case style, had the module run a fake `java` written in Python against the same flows;
// these tests run the same flows against a batch-file fake so they need no Python.)

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

// A stand-in for `java -jar retromcp-cli.jar <setup|decompile> ...` as a batch file. FAKE_MODE picks what it does;
// FAKE_PAYLOAD is copied to minecraft_x on `decompile`.
const char* kFakeJava =
    "@echo off\r\n"
    "echo ARGS %~1 %~nx2 %~3 %~4\r\n"
    "if \"%~3\"==\"setup\" call :readstdin\r\n"
    "if \"%FAKE_MODE%\"==\"nse\" ( echo Exception in thread \"main\" java.util.NoSuchElementException & exit /b 1 )\r\n"
    "if \"%FAKE_MODE%\"==\"setupfail\" ( if \"%~3\"==\"setup\" ( echo boom & exit /b 5 ) )\r\n"
    "if \"%FAKE_MODE%\"==\"decfail\" ( if \"%~3\"==\"decompile\" ( echo bad & exit /b 2 ) )\r\n"
    "if \"%FAKE_MODE%\"==\"nosrc\" ( if \"%~3\"==\"decompile\" ( mkdir minecraft_x\\notsrc & exit /b 0 ) )\r\n"
    "echo working on %~3\r\n"
    "if \"%~3\"==\"decompile\" ( if defined FAKE_PAYLOAD xcopy /e /i /q /y \"%FAKE_PAYLOAD%\" minecraft_x >nul )\r\n"
    "exit /b 0\r\n"
    ":readstdin\r\n"
    "set /p V=\r\n"
    "echo STDIN %V%\r\n"
    "exit /b 0\r\n";

struct FakeBackend {
    fs::ScopedTempDir dir{"cb_jar_test"};
    path java() const { return dir.path() / "fakejava.cmd"; }
    path jar() const { return dir.path() / "fakejar.jar"; }
    path workspace() const { return dir.path() / "ws"; }
    path payload() const { return dir.path() / "payload"; }
    FakeBackend() {
        put(dir.path(), "fakejava.cmd", kFakeJava);
        put(dir.path(), "fakejar.jar", "not a jar");
        put(payload(), "src/net/A.java", kMangled);
        put(payload(), "src/net/B.java", "class B {}\n");
        put(payload(), "src/net/notes.txt", "Block[] block;4, int x\n");
        put(payload(), "other/F.java", "class F {}\n");
    }
    jar::Settings settings() const { return {jar(), java()}; }
};

std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> out;
    for (std::string_view piece : cb::text::split(s, sep)) out.emplace_back(piece);
    return out;
}

}  // namespace

CB_TEST("decompile.jar.line_splitter", [] {
    // "\n", "\r" and "\r\n" each end a line; the pieces may arrive in any chunking
    auto run = [](const std::vector<std::string>& chunks) {
        jar::LineSplitter splitter;
        std::vector<std::string> lines;
        auto emit = [&](std::string_view l) { lines.emplace_back(l); };
        for (const std::string& c : chunks) splitter.feed(cb::as_bytes(c), emit);
        splitter.finish(emit);
        return lines;
    };
    const std::vector<std::string> expected = {"one", "two", "three", "four", "last no newline"};
    CB_CHECK(run({"one\r\ntwo\rthree\nfour\r\nlast no newline"}) == expected);
    CB_CHECK(run({"one\r", "\ntwo\r", "three\n", "four\r", "\nlast no", " newline"}) == expected);   // \r\n split across chunks
    CB_CHECK(run({"o", "n", "e", "\r\n", "two\rthree\nfour\r\nlast no newline"}) == expected);
    CB_CHECK(run({"first\r\nsecond\r"}) == std::vector<std::string>({"first", "second"}));   // a lone \r at the end
    CB_CHECK(run({"a\n\nb\n"}) == std::vector<std::string>({"a", "", "b"}));                  // empty lines are lines
    CB_CHECK(run({"a\r\r\nb"}) == std::vector<std::string>({"a", "", "b"}));
    CB_CHECK(run({}).empty());
    CB_CHECK(run({""}).empty());
    CB_CHECK(run({"\n"}) == std::vector<std::string>({""}));
    // a big output in odd chunk sizes comes out line for line
    std::string big;
    for (int i = 0; i < 3000; ++i) big += "line " + std::to_string(i) + " of the big output\r\n";
    std::vector<std::string> chunks;
    for (std::size_t at = 0; at < big.size(); at += 997) chunks.push_back(big.substr(at, 997));
    auto lines = run(chunks);
    CB_CHECK_EQ(lines.size(), 3000u);
    CB_CHECK_EQ(lines[2999], "line 2999 of the big output");
});

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
        // an explicit path that does not exist falls through to the next source (settings are not consulted: Python
        // took `explicit or settings` and then fell through to JAVA_HOME)
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
        jar::Settings with_jar = s;
        with_jar.retromcp_jar = d / "home/bin/java.exe";   // any existing file
        CB_CHECK(!jar::available(with_jar));                // no java
    }
    {
        fs::ScopedEnv path_var("PATH", (d / "pathdir").string());
        fs::ScopedEnv home("JAVA_HOME", std::nullopt);
        jar::Settings ok = s;
        ok.retromcp_jar = d / "home/bin/java.exe";
        CB_CHECK(jar::available(ok));
        ok.retromcp_jar = d / "no.jar";
        CB_CHECK(!jar::available(ok));                      // no backend jar
        ok.retromcp_jar.clear();
        CB_CHECK(!jar::available(ok));
    }
});

CB_TEST("decompile.jar.run_java", [] {
    FakeBackend fake;
    fs::make_dirs(fake.workspace());
    fs::ScopedEnv mode("FAKE_MODE", "ok");
    std::vector<std::string> seen;
    jar::RunResult r = jar::run_java(fake.java(), fake.jar(), {"setup", "x"}, fake.workspace(),
                                     [&](std::string_view l) { seen.emplace_back(l); }, std::string("v1\nv2\n"));
    CB_CHECK_EQ(r.exit_code, 0u);
    // ARGS: the first argument is -jar, the jar's file name, then ours; STDIN: the first line the program read
    CB_CHECK(r.lines.size() == 3u);
    if (r.lines.size() == 3u) {
        CB_CHECK_EQ(r.lines[0], "ARGS -jar fakejar.jar setup x");
        CB_CHECK_EQ(r.lines[1], "STDIN v1");
        CB_CHECK_EQ(r.lines[2], "working on setup");
    }
    // on_output sees the command first, then every line
    CB_CHECK_EQ(seen.size(), 4u);
    if (!seen.empty()) CB_CHECK_EQ(seen[0], "$ java -jar fakejar.jar setup x");
    CB_CHECK(std::vector<std::string>(seen.begin() + 1, seen.end()) == r.lines);
    // the log has the command, the lines and the exit code
    const std::string log = get(fake.workspace(), "codebreaker-decompile.log");
    CB_CHECK(log.find("\n$ ") == 0);
    CB_CHECK(log.find("ARGS -jar fakejar.jar setup x\n") != std::string::npos);
    CB_CHECK(log.find("STDIN v1\n") != std::string::npos);
    CB_CHECK(log.ends_with("[exit 0]\n"));
    // a second run appends
    jar::run_java(fake.java(), fake.jar(), {"decompile"}, fake.workspace());
    CB_CHECK_EQ(split(get(fake.workspace(), "codebreaker-decompile.log"), '$').size(), 3u);
    // no stdin text: the program's input is empty
    jar::RunResult empty_in = jar::run_java(fake.java(), fake.jar(), {"setup"}, fake.workspace());
    CB_CHECK(empty_in.lines.size() >= 2u && empty_in.lines[1] == "STDIN ");
    // exit codes pass through; a program that cannot be started, or a missing directory, is a ToolError
    fs::ScopedEnv fail("FAKE_MODE", "setupfail");
    jar::RunResult failed = jar::run_java(fake.java(), fake.jar(), {"setup"}, fake.workspace());
    CB_CHECK_EQ(failed.exit_code, 5u);
    CB_CHECK_THROWS_AS(cb::ToolError, (void)jar::run_java(fake.dir.path() / "nothere.cmd", fake.jar(), {"setup"}, fake.workspace()));
    CB_CHECK_THROWS_AS(cb::ToolError, (void)jar::run_java(fake.java(), fake.jar(), {"decompile"}, fake.dir.path() / "nodir"));
    CB_CHECK(!std::filesystem::exists(fake.dir.path() / "nodir"));   // (and the log did not create it)
});

CB_TEST("decompile.jar.decompile_flow", [] {
    FakeBackend fake;
    fs::ScopedEnv mode("FAKE_MODE", "ok");
    fs::ScopedEnv payload("FAKE_PAYLOAD", fs::to_text(fake.payload()));
    std::vector<std::string> lines;
    jar::DecompileOptions options;
    options.settings = fake.settings();
    options.on_output = [&](std::string_view l) { lines.emplace_back(l); };
    path src = jar::decompile("a1.2.6", fake.workspace(), options);
    // the src with the most .java files (net: two) under the workspace's minecraft_x, repaired
    CB_CHECK(fs::same_path(src, fake.workspace() / "minecraft_x/src"));
    CB_CHECK(src.is_absolute());
    CB_CHECK_EQ(get(src, "net/A.java"), kRepaired);
    CB_CHECK_EQ(get(src, "net/notes.txt"), "Block[] block;4, int x\n");   // not a .java file: untouched
    // what was said: setup banner, the setup run (with the version on stdin), the decompile banner and run, the summary
    CB_CHECK_EQ(lines.front(), "[decompile] setting up Minecraft a1.2.6 (downloads version + mappings)...");
    auto has = [&](const std::string& s) { return std::find(lines.begin(), lines.end(), s) != lines.end(); };
    CB_CHECK(has("$ java -jar fakejar.jar setup setup=a1.2.6"));
    CB_CHECK(has("ARGS -jar fakejar.jar setup setup"));   // (cmd.exe splits `setup=a1.2.6` at the `=`)
    CB_CHECK(has("STDIN a1.2.6"));
    CB_CHECK(has("[decompile] decompiling with Fernflower..."));
    CB_CHECK(has("$ java -jar fakejar.jar decompile"));
    CB_CHECK(has("[decompile] repaired 5 mangled array parameter name(s)"));
    CB_CHECK(lines[lines.size() - 2].starts_with("[decompile] done: 2 .java files at "));
    // setup = false skips the first step
    lines.clear();
    options.setup = false;
    jar::decompile("a1.2.6", fake.workspace() / ".." / "ws", options);
    CB_CHECK(!has("$ java -jar fakejar.jar setup setup=a1.2.6"));
    CB_CHECK(has("[decompile] decompiling with Fernflower..."));
    // the workspace is created when it does not exist
    path fresh = fake.dir.path() / "new/deep/ws";
    CB_CHECK(!std::filesystem::exists(fresh));
    options.setup = true;
    (void)jar::decompile("c0.30_01c", fresh, options);
    CB_CHECK(std::filesystem::is_directory(fresh / "minecraft_x/src"));
});

CB_TEST("decompile.jar.decompile_failures", [] {
    FakeBackend fake;
    jar::DecompileOptions options;
    options.settings = fake.settings();
    fs::ScopedEnv payload("FAKE_PAYLOAD", fs::to_text(fake.payload()));
    auto attempt = [&](const char* mode, std::string_view version = "a1.2.6") -> std::string {
        fs::ScopedEnv m("FAKE_MODE", mode);
        try {
            (void)jar::decompile(version, fake.workspace(), options);
        } catch (const jar::DecompileError& e) {
            return e.what();
        }
        return "";
    };
    CB_CHECK(attempt("ok").empty());
    // the backend does not know the version: asked for another one, or an unknown one
    CB_CHECK(attempt("nse", "zzz").find("Minecraft version 'zzz' was not found in the configured version manifest") != std::string::npos);
    // setup failing for some other reason
    CB_CHECK(attempt("setupfail").find("RetroMCP setup failed for version 'a1.2.6' (exit code 5).") != std::string::npos);
    CB_CHECK(attempt("decfail").find("RetroMCP decompile failed (exit code 2).") != std::string::npos);
    // a run that produced no src
    {
        fs::ScopedEnv no_payload("FAKE_PAYLOAD", std::nullopt);
        fs::remove_tree(fake.workspace());
        CB_CHECK(attempt("nosrc").find("Decompile produced no Java source under ") != std::string::npos);
    }
    // no Java, no backend jar
    jar::DecompileOptions broken = options;
    broken.settings.java = fake.dir.path() / "none.exe";
    {
        fs::ScopedEnv home("JAVA_HOME", std::nullopt);
        fs::ScopedEnv path_var("PATH", fs::to_text(fake.dir.path() / "emptydir"));
        std::string message;
        try {
            (void)jar::decompile("a1.2.6", fake.workspace(), broken);
        } catch (const jar::DecompileError& e) {
            message = e.what();
        }
        CB_CHECK(message.find("Java runtime not found.") != std::string::npos);
    }
    broken = options;
    broken.settings.retromcp_jar = fake.dir.path() / "missing.jar";
    std::string message;
    try {
        (void)jar::decompile("a1.2.6", fake.workspace(), broken);
    } catch (const jar::DecompileError& e) {
        message = e.what();
    }
    CB_CHECK(message.find("Bundled RetroMCP backend jar not found at: ") != std::string::npos);
    // a DecompileError is a cb::Error
    CB_CHECK_THROWS_AS(cb::Error, throw jar::DecompileError("x"));
});
