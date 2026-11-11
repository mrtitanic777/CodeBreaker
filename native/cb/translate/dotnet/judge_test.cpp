#include "cb/translate/dotnet/judge.hpp"

#include <fstream>

#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

// std::string::contains is C++23; this answers for strings and for sets and maps alike
template <class Haystack, class Needle>
bool contains(const Haystack& haystack, const Needle& needle) {
    if constexpr (requires { haystack.find(needle) != Haystack::npos; })
        return haystack.find(needle) != Haystack::npos;
    else
        return haystack.contains(needle);
}
namespace jd = cb::translate::dotnet::judge;
namespace dn = cb::translate::dotnet;
namespace stdfs = std::filesystem;

jd::Run ran(std::string out, std::uint32_t exit = 0) { return {.ok = true, .exit = exit, .out = std::move(out)}; }

void touch(const stdfs::path& p, const std::string& text) {
    stdfs::create_directories(p.parent_path());
    std::ofstream(p, std::ios::binary) << text;
}

// A program of this machine found on PATH (javac.exe, g++.exe), or empty.
stdfs::path on_path(const std::string& program) {
    const auto path = cb::fs::get_env("PATH").value_or("");
    for (const auto dir : cb::text::split(path, ';')) {
        const stdfs::path candidate = cb::fs::from_text(dir) / (program + ".exe");
        std::error_code ec;
        if (!dir.empty() && stdfs::is_regular_file(candidate, ec)) return candidate;
    }
    return {};
}
}  // namespace

CB_TEST("translate.judge.compare_equivalent", [] {
    // line endings and trailing line ends do not count
    auto j = jd::compare(ran("a\r\nb\r\n"), ran("a\nb\n\n"));
    CB_CHECK(j.verdict == jd::Verdict::Equivalent);
    CB_CHECK_EQ(j.detail, std::string("2 lines"));
    CB_CHECK_EQ(jd::compare(ran(""), ran("")).detail, std::string("0 lines"));
    CB_CHECK_EQ(jd::word(jd::compare(ran("x", 3), ran("x", 3)).verdict), std::string_view("equivalent"));
});

CB_TEST("translate.judge.compare_divergent", [] {
    auto j = jd::compare(ran("one\ntwo\nthree\n"), ran("one\nTWO\nthree\n"));
    CB_CHECK(j.verdict == jd::Verdict::Divergent);
    CB_CHECK_EQ(j.detail, std::string("line 2: original 'two', translation 'TWO'"));
    // one is shorter
    CB_CHECK_EQ(jd::compare(ran("a\nb\n"), ran("a\n")).detail, std::string("line 2: original 'b', translation '<end>'"));
    CB_CHECK_EQ(jd::compare(ran("a\nb"), ran("a\nb\nc")).detail, std::string("line 3: original '<end>', translation 'c'"));
    // the same output, a different exit code
    j = jd::compare(ran("a", 0), ran("a", 1));
    CB_CHECK(j.verdict == jd::Verdict::Divergent);
    CB_CHECK_EQ(j.detail, std::string("exit code 0, translation 1"));
    // a line that is quoted: escapes, and cut at 160 bytes without breaking a character
    CB_CHECK_EQ(jd::compare(ran("it's\ttab\n"), ran("x\n")).detail, std::string("line 1: original 'it\\'s\\ttab', translation 'x'"));
    std::string long_line(159, 'a');
    long_line += "\xC3\xA9";   // the character straddles byte 160
    const auto cut = jd::compare(ran(long_line), ran("b")).detail;
    CB_CHECK(contains(cut, "original '" + std::string(159, 'a') + "'"));
    // the translation ran out of time
    jd::Run timeout = ran("a");
    timeout.exit.reset();
    j = jd::compare(ran("a"), timeout);
    CB_CHECK(j.verdict == jd::Verdict::Divergent);
    CB_CHECK(contains(j.detail, "timeout"));
});

CB_TEST("translate.judge.compare_other_verdicts", [] {
    // refused wins over everything, once each, in order, cut at 400
    const std::vector<std::string> refused{"goto in main", "unsafe", "goto in main"};
    auto j = jd::compare({}, {}, refused);
    CB_CHECK(j.verdict == jd::Verdict::Refused);
    CB_CHECK_EQ(j.detail, std::string("goto in main; unsafe"));
    const std::vector<std::string> many{std::string(500, 'r')};
    CB_CHECK_EQ(jd::compare({}, {}, many).detail.size(), std::size_t{400});
    // the original did not run: its log (else its error), the last 400
    jd::Run broken = {.ok = false, .log = "no JDK"};
    j = jd::compare(broken, ran("x"));
    CB_CHECK(j.verdict == jd::Verdict::NoOriginal);
    CB_CHECK_EQ(j.detail, std::string("no JDK"));
    jd::Run hung = {.ok = true, .exit = std::nullopt, .err = "timeout"};
    CB_CHECK_EQ(jd::compare(hung, ran("x")).detail, std::string("timeout"));
    CB_CHECK_EQ(jd::compare({.ok = false, .log = std::string(600, 'l') + "END"}, ran("")).detail.size(), std::size_t{400});
    // the translation did not compile: the first 600 of its log
    j = jd::compare(ran("x"), {.ok = false, .log = std::string(700, 'e')});
    CB_CHECK(j.verdict == jd::Verdict::NoCompile);
    CB_CHECK_EQ(j.detail.size(), std::size_t{600});
    CB_CHECK_EQ(jd::word(jd::Verdict::NoCompile), std::string_view("no-compile"));
    CB_CHECK_EQ(jd::word(jd::Verdict::NoOriginal), std::string_view("no-original"));
    CB_CHECK_EQ(jd::word(jd::Verdict::Refused), std::string_view("refused"));
    CB_CHECK_EQ(jd::word(jd::Verdict::Divergent), std::string_view("divergent"));
});

CB_TEST("translate.judge.first_errors", [] {
    std::string log = "Prog.cs(1,2): warning CS0168: unused\n";
    for (int i = 1; i <= 15; ++i) log += "Prog.cs(" + std::to_string(i) + ",1): error CS1002: ; expected\n";
    const auto errors = jd::first_errors(log);
    CB_CHECK_EQ(cb::text::split(errors, '\n').size(), std::size_t{12});
    CB_CHECK(!contains(errors, "warning"));
    CB_CHECK(errors.starts_with("Prog.cs(1,1): error"));
    // no error lines: the end of the log
    CB_CHECK_EQ(jd::first_errors(std::string(900, 'x') + "tail"), std::string(796, 'x') + "tail");
    CB_CHECK_EQ(jd::first_errors(""), std::string());
});

CB_TEST("translate.judge.csharp_real", [] {
    auto sdk = dn::Sdk::find();
    if (!sdk || !sdk->available()) return;   // no .NET SDK
    cb::fs::ScopedTempDir dir("cb-judge");
    touch(dir.path() / "src" / "Prog.cs",
          "using System;\nclass P { static int Main(string[] a) { Console.WriteLine(\"hi \" + a.Length); Console.Error.WriteLine(\"e\"); return 4; } }\n");
    stdfs::create_directories(dir.path() / "work");
    touch(dir.path() / "work" / "stale.txt", "left over");
    const auto r = jd::csharp(jd::CSharpToolchain::of(*sdk), {dir.path() / "src" / "Prog.cs"}, dir.path() / "work", {.args = {"a", "b"}});
    CB_CHECK(r.ok);
    CB_CHECK_EQ(r.exit.value(), std::uint32_t{4});
    CB_CHECK_EQ(r.out, std::string("hi 2\r\n"));
    CB_CHECK(contains(r.err, "e"));
    CB_CHECK(!stdfs::exists(dir.path() / "work" / "stale.txt"));   // the work folder is made new
    // a program that does not compile: only the error lines
    touch(dir.path() / "src" / "Bad.cs", "class P { static void Main() { int x = \"no\"; } }\n");
    const auto bad = jd::csharp(jd::CSharpToolchain::of(*sdk), {dir.path() / "src" / "Bad.cs"}, dir.path() / "work");
    CB_CHECK(!bad.ok);
    CB_CHECK(contains(bad.log, "error CS0029"));
    CB_CHECK(!contains(bad.log, "Build succeeded"));
    // and judged against each other
    const auto j = jd::compare(r, r);
    CB_CHECK(j.verdict == jd::Verdict::Equivalent);
    CB_CHECK(jd::compare(r, bad).verdict == jd::Verdict::NoCompile);
});

CB_TEST("translate.judge.java_real", [] {
    const auto javac = on_path("javac");
    const auto java = on_path("java");
    if (javac.empty() || java.empty()) return;   // no JDK
    cb::fs::ScopedTempDir dir("cb-judge");
    touch(dir.path() / "src" / "demo" / "Main.java",
          "package demo;\npublic class Main { public static void main(String[] a) { System.out.println(\"java \" + a.length + \" \" + Util.twice(21)); System.exit(7); } }\n");
    touch(dir.path() / "src" / "demo" / "Util.java", "package demo;\nclass Util { static int twice(int x) { return 2 * x; } }\n");
    const jd::Jdk jdk{javac, java};
    const auto r = jd::java(jdk, dir.path() / "src", "demo.Main", dir.path() / "work", {.args = {"q"}});
    CB_CHECK(r.ok);
    CB_CHECK_EQ(r.exit.value(), std::uint32_t{7});
    CB_CHECK_EQ(cb::text::trim(r.out), std::string_view("java 1 42"));
    // does not compile
    touch(dir.path() / "bad" / "A.java", "class A { void f() { int x = \"s\"; } }\n");
    const auto bad = jd::java(jdk, dir.path() / "bad", "A", dir.path() / "work");
    CB_CHECK(!bad.ok);
    CB_CHECK(contains(bad.log, "incompatible types"));
    // no JDK
    CB_CHECK_EQ(jd::java(std::nullopt, dir.path() / "src", "demo.Main", dir.path() / "work").log, std::string("no JDK"));
});

CB_TEST("translate.judge.cpp_with_a_host_build", [] {
    cb::fs::ScopedTempDir dir("cb-judge");
    touch(dir.path() / "cpp" / "Hello.cpp", "// source\n");
    touch(dir.path() / "cpp" / "Other.cpp", "// source\n");
    touch(dir.path() / "cpp" / "Hello.h", "// header\n");
    CB_CHECK_EQ(jd::cpp(std::nullopt, dir.path() / "cpp", "a.b.Hello", dir.path() / "w").log,
                std::string("no host C++ compiler (settings.json: host_cxx)"));

    // the library does not build
    jd::HostBuild host;
    host.library = [](const stdfs::path&, const stdfs::path&, unsigned) { return std::optional<stdfs::path>(); };
    CB_CHECK_EQ(jd::cpp(host, dir.path() / "cpp", "Hello", dir.path() / "w").log,
                std::string("the program did not compile into a library"));

    // the library builds, the program does not: which sources left no object is said
    unsigned jobs_seen = 0;
    host.library = [&](const stdfs::path&, const stdfs::path& work, unsigned jobs) {
        jobs_seen = jobs;
        touch(work / "probe" / "_objects" / "Hello.o", "obj");
        return std::optional<stdfs::path>(work / "lib.a");
    };
    host.program = [](const stdfs::path&, const stdfs::path&, const stdfs::path&, const stdfs::path&) {
        return jd::HostBuild::Native{false, "link failed"};
    };
    CB_CHECK_EQ(jd::cpp(host, dir.path() / "cpp", "a.b.Hello", dir.path() / "w", {.jobs = 3}).log,
                std::string("link failed; did not compile: Other.cpp"));
    CB_CHECK_EQ(jobs_seen, 3u);

    // the program builds: the harness is the one the static initialisers need, and the program is run in `work`
    const auto gxx = on_path("g++");
    if (gxx.empty()) return;
    std::string harness_seen;
    host.program = [&](const stdfs::path&, const stdfs::path& harness, const stdfs::path& exe, const stdfs::path&) {
        harness_seen = cb::fs::read_text(harness);
        touch(exe.parent_path() / "real.cpp",
              "#include <cstdio>\n#include <cstring>\nint main(int argc, char** argv) { std::puts(argc > 1 ? argv[1] : \"none\"); "
              "std::fputs(\"err\", stderr); return 5; }\n");
        const auto r = cb::process::run({.argv = {cb::fs::to_text(gxx), cb::fs::to_text(exe.parent_path() / "real.cpp"), "-o", cb::fs::to_text(exe)}});
        return jd::HostBuild::Native{r.exit_code == 0, "g++ failed"};
    };
    const auto r = jd::cpp(host, dir.path() / "cpp", "a.b.Hello", dir.path() / "w", {.args = {"arg"}});
    CB_CHECK(r.ok);
    CB_CHECK_EQ(r.exit.value(), std::uint32_t{5});
    CB_CHECK_EQ(cb::text::trim(r.out), std::string_view("arg"));
    CB_CHECK_EQ(r.err, std::string("err"));
    CB_CHECK(contains(harness_seen, "#include \"Hello.h\""));
    CB_CHECK(contains(harness_seen, "Hello::main(JArray<JavaString >());"));
    CB_CHECK(contains(harness_seen, "j2x_initAll();"));
});
