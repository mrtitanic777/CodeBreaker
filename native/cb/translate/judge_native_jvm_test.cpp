// cb-unity: alone (test helpers named for this module)
// Tests of cb/translate/judge_native_jvm. The comparison, the messages and the names are checked on values written out below; the judge
// itself on stand-ins for the tools it drives, which are small batch files and the system's own hostname.exe: the "native program" is
// hostname.exe (what clang's output would be), the stand-in clang copies it to the file it is asked for with -o, the stand-in javac
// makes the runtime's class and succeeds or fails as told, the stand-in java prints what it is told to. A unit the front end "lowers" is an
// empty L2 unit made here, so the Java back end writes its real answer for it. Real processes are run (cmd, hostname) and a real timeout
// is waited for; the real clang, javac and java are not needed.
//
// DIVERGES from the Python module, by design: a run that times out has no exit code (the Python's tuple had the word 'timeout' in the place
// of an int); the bytes in a difference are shown quoted with C escapes, not as Python's repr; a front end that cannot read a file is a
// ToolError (the Python's RuntimeError) and a clang that is not found is a failed build (the Python raised a TypeError on None); the cache
// folder of the runtime is named from the JDK path and the sources' bytes in an order of its own, so a folder the Python made is not found
// (the runtime is compiled once more); the error type of a Java -> C++ failure is not named in its reason.
#include "cb/translate/judge_native_jvm.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace jj = cb::translate::judge_native_jvm;
namespace ir = cb::translate::ir;
namespace stdfs = std::filesystem;
using cb::Bytes;

namespace {

Bytes bytes_of(std::string_view s) { return Bytes(s.begin(), s.end()); }

jj::Run run_of(std::string_view out, std::string_view err = "", std::optional<std::int64_t> code = 0) { return {bytes_of(out), bytes_of(err), code}; }

std::string hostname_exe() { return cb::fs::get_env("SystemRoot").value_or("C:\\Windows") + "\\System32\\hostname.exe"; }

// The stand-in tools and the host that uses them.
struct World {
    cb::fs::ScopedTempDir dir{"cbjudge"};
    jj::Host host;
    std::size_t lowered = 0;
    bool unsupported = false;
    bool refuses_to_lower = false;

    stdfs::path file(const char* name) const { return dir.path() / name; }
    std::string log_of(const char* name) const { return stdfs::exists(file(name)) ? cb::fs::read_text(file(name)) : std::string(); }

    // javac_mode: 0 compiles everything, 1 fails everything, 2 fails only the program (a source under `\\java\\`: the judge's), not the runtime
    World(int javac_mode = 0, const char* java_prints = "@hostname", bool clang_ok = true) {
        cb::fs::make_dirs(dir.path() / "runtime" / "crt");
        cb::fs::write_text(dir.path() / "runtime" / "crt" / "LibC.java", "package crt; public class LibC {}\n");
        // clang: -o <exe> gets a copy of hostname.exe (a program that prints one line and ends with 0); otherwise it complains
        cb::fs::write_text(file("clang.cmd"),
                           clang_ok ? std::format("@echo off\r\n:next\r\nif \"%~1\"==\"\" exit /b 0\r\nif \"%~1\"==\"-o\" (copy /y \"{}\" \"%~2\" >nul & exit /b 0)\r\nshift\r\ngoto next\r\n", hostname_exe())
                                    : std::string("@echo off\r\necho prog.c:3:1: error: unknown type name 'foo' 1>&2\r\necho and a second line 1>&2\r\nexit /b 1\r\n"));
        // javac: counts its runs; -d <folder> gets the runtime's class; or it fails with javac's kind of message
        const std::string fails = "echo Prog.java:7: error: cannot find symbol\r\necho   more detail\r\nexit /b 1\r\n";
        const std::string compiles = ":next\r\nif \"%~1\"==\"\" exit /b 0\r\nif \"%~1\"==\"-d\" (mkdir \"%~2\\crt\" 2>nul & echo x > \"%~2\\crt\\LibC.class\" & exit /b 0)\r\nshift\r\ngoto next\r\n";
        cb::fs::write_text(file("javac.cmd"), "@echo off\r\necho run >> \"%~dp0javac.log\"\r\n" +
                                                  (javac_mode == 1 ? fails
                                                   : javac_mode == 2 ? "echo %* | findstr /c:\"-cp\" >nul && (\r\n" + fails + ")\r\n" + compiles
                                                                     : compiles));
        cb::fs::write_text(file("java.cmd"), std::format("@echo off\r\n{}\r\n", java_prints));
        host.jdk = {file("javac.cmd"), file("java.cmd")};
        host.runtime_sources = dir.path() / "runtime";
        host.state_folder = dir.path() / "state";
        host.clang = [this]() -> std::optional<stdfs::path> { return file("clang.cmd"); };
        host.language_of = [](const stdfs::path& p) { return p.extension() == ".c" ? std::string("c") : std::string("c++"); };
        host.flags = [](const std::string& language, const std::optional<std::string>&, const std::vector<std::string>&, const std::vector<std::string>&,
                        const std::vector<std::string>&) { return std::vector<std::string>{"-x", language}; };
        // the Java back end is a stand-in too (the module's own refuses a unit with no main): it writes a class and a note
        host.write_java = [](std::span<ir::Node* const>, const std::string& name) {
            cb::translate::backends::java::Program p;
            p.name = name;
            p.source = "public class " + name + " {}\n";
            p.tier = cb::translate::backends::java::Tier::Faithful;
            p.notes = {"a note", "gap: printf with %n"};
            return p;
        };
        host.lower = [this](cb::Arena& arena, const jj::LoweringRequest& request) -> ir::Node* {
            ++lowered;
            if (refuses_to_lower) throw cb::ToolError("clang", 1, "", "cannot parse " + cb::fs::to_text(request.path.filename()));
            ir::Node* unit = ir::make_node(arena, "Unit");
            ir::Unit* u = ir::get<ir::Unit>(unit);
            u->path = cb::fs::to_text(request.path);
            if (unsupported) {
                ir::Node* bad = ir::make_node(arena, "Unsupported");
                bad->line = 12;
                ir::get<ir::Unsupported>(bad)->kind = "VLA";
                ir::get<ir::Unsupported>(bad)->raw = "int a[n]";
                u->types.push_back(bad);
            }
            return unit;
        };
    }
};

}  // namespace

CB_TEST("translate.judge_native_jvm.verdict_words_and_descriptions", [] {
    CB_CHECK_EQ(jj::word(jj::Kind::Equivalent), "equivalent");
    CB_CHECK_EQ(jj::word(jj::Kind::Divergent), "divergent");
    CB_CHECK_EQ(jj::word(jj::Kind::Refused), "refused");
    CB_CHECK_EQ(jj::word(jj::Kind::NoCompile), "no-compile");
    CB_CHECK_EQ(jj::word(jj::Kind::NoOriginal), "no-original");
    jj::Verdict v;
    v.name = "Hello";
    v.verdict = jj::Kind::Divergent;
    v.reason = "exit code: C 0, Java 1";
    CB_CHECK_EQ(v.describe(), "Hello: divergent -- exit code: C 0, Java 1");
    v.reason.clear();
    CB_CHECK_EQ(v.describe(), "Hello: divergent");
});

CB_TEST("translate.judge_native_jvm.class_names_from_file_stems", [] {
    CB_CHECK_EQ(jj::class_name("hello"), "Hello");
    CB_CHECK_EQ(jj::class_name("my-prog.v2"), "My_prog_v2");
    CB_CHECK_EQ(jj::class_name("2fast"), "P2fast");
    CB_CHECK_EQ(jj::class_name(""), "P");
    CB_CHECK_EQ(jj::class_name("_x"), "_x");
    CB_CHECK_EQ(jj::class_name("ünï"), "__n__");   // every byte outside [A-Za-z0-9_]: each of the two-byte letters is two
});

CB_TEST("translate.judge_native_jvm.the_first_error_of_a_compilers_output", [] {
    CB_CHECK_EQ(jj::first_error("note: something\nProg.java:7: error: cannot find symbol\n  more"), "Prog.java:7: error: cannot find symbol");
    CB_CHECK_EQ(jj::first_error("Exception in thread \"main\" java.lang.IllegalStateException\n\tat X"), "Exception in thread \"main\" java.lang.IllegalStateException");
    CB_CHECK_EQ(jj::first_error("  just a line  \nand another"), "just a line");   // no error: the first line
    CB_CHECK_EQ(jj::first_error("\n\n   \n"), "");
    CB_CHECK_EQ(jj::first_error(""), "");
    CB_CHECK_EQ(jj::first_error(std::string(400, 'x')).size(), 300U);
    CB_CHECK_EQ(jj::first_error("ERROR in caps"), "ERROR in caps");
});

CB_TEST("translate.judge_native_jvm.two_runs_compared", [] {
    // the same: line ends are compared as \n (a Windows text-mode native program writes \r\n)
    CB_CHECK(!jj::compare(run_of("a\r\nb\r\n"), run_of("a\nb\n")).has_value());
    CB_CHECK(!jj::compare(run_of("", "warning\r\n", 3), run_of("", "warning\n", 3)).has_value());
    // stdout first: the line it differs at and a few bytes around it
    CB_CHECK_EQ(*jj::compare(run_of("one\ntwo\nthree\n"), run_of("one\ntwx\nthree\n")),
                "stdout differs at line 2: C \"one\\ntwo\\nthree\\n\", Java \"one\\ntwx\\nthree\\n\"");
    CB_CHECK_EQ(*jj::compare(run_of("abc"), run_of("abcdef")), "stdout differs at line 1: C \"abc\", Java \"abcdef\"");   // one a prefix of the other
    // a long common start: twenty bytes before the first difference and thirty after
    const std::string common(50, 'k');
    CB_CHECK_EQ(*jj::compare(run_of(common + "X" + std::string(40, 'z')), run_of(common + "Y" + std::string(40, 'z'))),
                std::format("stdout differs at line 1: C \"{}X{}\", Java \"{}Y{}\"", std::string(20, 'k'), std::string(29, 'z'), std::string(20, 'k'), std::string(29, 'z')));
    // non-printable bytes are escaped
    CB_CHECK_EQ(*jj::compare(run_of(std::string_view("a\x01\"\\", 4)), run_of("b")), "stdout differs at line 1: C \"a\\x01\\\"\\\\\", Java \"b\"");
    // then the exit code, with the JVM's first error when it said anything
    CB_CHECK_EQ(*jj::compare(run_of("x", "", 0), run_of("x", "", 1)), "exit code: C 0, Java 1");
    CB_CHECK_EQ(*jj::compare(run_of("x", "", 0), run_of("x", "Exception in thread main\n\tat Foo", 1)), "exit code: C 0, Java 1 (Exception in thread main)");
    CB_CHECK_EQ(*jj::compare(run_of("x", "", 0), run_of("x", "(timed out)", std::nullopt)), "exit code: C 0, Java timeout ((timed out))");
    CB_CHECK(!jj::compare(run_of("x", "", std::nullopt), run_of("x", "", std::nullopt)).has_value());   // both timed out: the same end
    // then stderr
    CB_CHECK_EQ(*jj::compare(run_of("x", "C says\n"), run_of("x", "Java says\n")), "stderr differs: C \"C says\\n\", Java \"Java says\\n\"");
    CB_CHECK_EQ(jj::compare(run_of("x", std::string(300, 'e')), run_of("x", "")).value_or("").size(), std::string("stderr differs: C \"").size() + 120 + std::string("\", Java \"\"").size());
});

CB_TEST("translate.judge_native_jvm.running_a_program", [] {
    cb::fs::ScopedTempDir dir("cbrun");
    const jj::Run done = jj::run_program({"cmd", "/c", "echo hello&exit /b 3"}, {}, dir.path());
    const std::string done_out = cb::text::from_latin1(done.out);   // (trim views its argument: keep the text alive)
    CB_CHECK_EQ(cb::text::trim(done_out), "hello");
    CB_CHECK_EQ(done.exit_code.value_or(-1), 3);
    CB_CHECK(!done.timed_out());
    // stdin reaches the program
    const jj::Run echoed = jj::run_program({"findstr", "."}, cb::as_bytes("from stdin\r\n"), dir.path());
    const std::string echoed_out = cb::text::from_latin1(echoed.out);
    CB_CHECK_EQ(cb::text::trim(echoed_out), "from stdin");
    // the working folder is the one given
    const jj::Run where = jj::run_program({"cmd", "/c", "cd"}, {}, dir.path());
    CB_CHECK(cb::text::from_latin1(where.out).find(cb::fs::to_text(dir.path().filename())) != std::string::npos);
    // a program that takes too long: no exit code, and the word the Python used for stderr
    const jj::Run slow = jj::run_program({"cmd", "/c", "ping -n 8 127.0.0.1 >nul"}, {}, dir.path(), std::chrono::seconds(1));
    CB_CHECK(slow.timed_out());
    CB_CHECK_EQ(cb::text::from_latin1(slow.err), "(timed out)");
    CB_CHECK(slow.out.empty());
    // a program that cannot start
    CB_CHECK_THROWS_AS(cb::ToolError, (void)jj::run_program({(dir.path() / "absent.exe").string()}, {}, dir.path()));
});

CB_TEST("translate.judge_native_jvm.the_original_is_built_with_clang", [] {
    World world;
    const std::vector<stdfs::path> files = {world.file("prog.c")};
    const jj::Compiled built = jj::build_native(world.host, files, world.file("prog.exe"), "c", std::nullopt, {}, {});
    CB_CHECK(built.ok);
    CB_CHECK(stdfs::exists(world.file("prog.exe")));   // the stand-in copied hostname.exe to the -o file
    World failing(0, "@hostname", false);
    const jj::Compiled bad = jj::build_native(failing.host, files, failing.file("prog.exe"), "c", std::nullopt, {}, {});
    CB_CHECK(!bad.ok);
    CB_CHECK(bad.message.find("error: unknown type name") != std::string::npos);   // clang's stderr
    // no clang at all
    World none;
    none.host.clang = [] { return std::optional<stdfs::path>(); };
    const jj::Compiled missing = jj::build_native(none.host, files, none.file("prog.exe"), "c", std::nullopt, {}, {});
    CB_CHECK(!missing.ok);
    CB_CHECK(missing.message.find("no clang") != std::string::npos);
});

CB_TEST("translate.judge_native_jvm.the_runtime_is_compiled_once", [] {
    World world;
    const stdfs::path first = jj::runtime_classes(world.host);
    CB_CHECK(stdfs::exists(first / "crt" / "LibC.class"));
    CB_CHECK_EQ(std::string(cb::text::trim(world.log_of("javac.log"))), "run");
    const stdfs::path again = jj::runtime_classes(world.host);
    CB_CHECK_EQ(cb::fs::to_text(again), cb::fs::to_text(first));
    CB_CHECK_EQ(std::string(cb::text::trim(world.log_of("javac.log"))), "run");   // not compiled a second time
    // a change to the runtime's sources is a new folder
    cb::fs::write_text(world.dir.path() / "runtime" / "crt" / "LibC.java", "package crt; public class LibC { int x; }\n");
    CB_CHECK(cb::fs::to_text(jj::runtime_classes(world.host)) != cb::fs::to_text(first));
    // a runtime that does not compile: an error, and no folder left to be taken for a good one
    World broken(1);
    CB_CHECK_THROWS_AS(cb::ToolError, (void)jj::runtime_classes(broken.host));
    CB_CHECK(!stdfs::exists(broken.host.state_folder) || stdfs::is_empty(broken.host.state_folder));
});

CB_TEST("translate.judge_native_jvm.a_program_that_does_not_build_has_no_original", [] {
    World world(0, "@hostname", false);
    const std::vector<stdfs::path> files = {world.file("prog.c")};
    const jj::Verdict v = jj::judge(world.host, files, world.dir.path() / "work");
    CB_CHECK_EQ(v.name, "Prog");
    CB_CHECK(v.verdict == jj::Kind::NoOriginal);
    CB_CHECK_EQ(v.reason, "clang: prog.c:3:1: error: unknown type name 'foo'");   // clang's first error
    CB_CHECK(!v.native.has_value());
    CB_CHECK_EQ(world.lowered, 0U);   // nothing was translated
});

CB_TEST("translate.judge_native_jvm.a_front_end_that_declines_refuses_the_program", [] {
    World world;
    world.refuses_to_lower = true;
    const std::vector<stdfs::path> files = {world.file("prog.c")};
    const jj::Verdict v = jj::judge(world.host, files, world.dir.path() / "work");
    CB_CHECK(v.verdict == jj::Kind::Refused);
    CB_CHECK(v.reason.starts_with("front end: clang: cannot parse prog.c"));   // the front end's message, as the tool error words it
    CB_CHECK(v.native.has_value());   // the original ran before the translation was tried
    CB_CHECK(!v.java.has_value());

    World unsupported;
    unsupported.unsupported = true;
    const std::vector<stdfs::path> two = {unsupported.file("prog.c"), unsupported.file("prog.h"), unsupported.file("util.cpp")};
    const jj::Verdict u = jj::judge(unsupported.host, two, unsupported.dir.path() / "work");
    CB_CHECK(u.verdict == jj::Kind::Refused);
    CB_CHECK_EQ(u.reason, "front end: VLA at prog.c:12 (int a[n])");   // the construct, where, and what the source said
    CB_CHECK_EQ(unsupported.lowered, 2U);   // both sources are lowered (the header is read through them), then the first unsupported construct is the reason
});

CB_TEST("translate.judge_native_jvm.a_program_javac_rejects_is_no_compile", [] {
    World world(1);   // javac fails for the runtime too: the judge cannot get that far
    const std::vector<stdfs::path> files = {world.file("prog.c")};
    CB_CHECK_THROWS_AS(cb::ToolError, (void)jj::judge(world.host, files, world.dir.path() / "work"));
    World program(2);   // the runtime compiles, the program does not
    const jj::Verdict v = jj::judge(program.host, files, program.dir.path() / "work");
    CB_CHECK(v.verdict == jj::Kind::NoCompile);
    CB_CHECK_EQ(v.reason, "javac: Prog.java:7: error: cannot find symbol");
    CB_CHECK(v.java.has_value() && v.native.has_value() && !v.jvm.has_value());   // the Java that was written, and the original that ran; nothing ran on the JVM
    CB_CHECK_EQ(v.tier.value_or(""), "faithful");
});

CB_TEST("translate.judge_native_jvm.the_judge_compares_the_two_runs", [] {
    // the program prints the host's name; so does the stand-in java: equivalent
    {
        World world;
        const std::vector<stdfs::path> files = {world.file("hello.c")};
        const jj::Verdict v = jj::judge(world.host, files, world.dir.path() / "work");
        CB_CHECK_EQ(v.name, "Hello");
        CB_CHECK(v.verdict == jj::Kind::Equivalent);
        CB_CHECK_EQ(v.reason, "gaps: printf with %n");   // a gap the run did not reach still says it is there
        CB_CHECK(v.native && v.jvm);
        CB_CHECK_EQ(v.java.value_or(""), "public class Hello {}\n");
        CB_CHECK_EQ(v.notes.size(), 2U);
        CB_CHECK(stdfs::exists(world.dir.path() / "work" / "java" / "Hello.java"));
        CB_CHECK(stdfs::exists(world.dir.path() / "work" / "Hello.exe"));
    }
    // java prints something else: divergent, and the reason is the first difference
    {
        World world(0, "@echo not the host name");
        const std::vector<stdfs::path> files = {world.file("hello.c")};
        const jj::Verdict v = jj::judge(world.host, files, world.dir.path() / "work");
        CB_CHECK(v.verdict == jj::Kind::Divergent);
        CB_CHECK(v.reason.starts_with("stdout differs at line 1: C \""));
        CB_CHECK(v.reason.ends_with("; gaps: printf with %n"));
    }
    // a name and a language given are used as they are
    {
        World world;
        std::string seen_language;
        world.host.lower = [&](cb::Arena& arena, const jj::LoweringRequest& request) -> ir::Node* {
            seen_language = request.language;
            ir::Node* unit = ir::make_node(arena, "Unit");
            ir::get<ir::Unit>(unit)->path = cb::fs::to_text(request.path);
            return unit;
        };
        jj::JudgeOptions options;
        options.name = "Chosen";
        options.language = "c++";
        const std::vector<stdfs::path> files = {world.file("hello.c")};
        const jj::Verdict v = jj::judge(world.host, files, world.dir.path() / "work", options);
        CB_CHECK_EQ(v.name, "Chosen");
        CB_CHECK_EQ(seen_language, "c++");
        CB_CHECK(stdfs::exists(world.dir.path() / "work" / "Chosen.exe"));
    }
});

CB_TEST("translate.judge_native_jvm.the_round_trip_stops_where_a_stage_does", [] {
    // the original Java does not compile: no original
    {
        World world(1);
        cb::fs::write_text(world.file("Prog.java"), "package sample;\r\npublic class Prog { public static void main(String[] a) {} }\r\n");
        const std::vector<stdfs::path> files = {world.file("Prog.java")};
        const jj::Verdict v = jj::roundtrip(world.host, files, "sample.Prog", world.dir.path() / "rt");
        CB_CHECK(v.verdict == jj::Kind::NoOriginal);
        CB_CHECK_EQ(v.reason, "javac: Prog.java:7: error: cannot find symbol");
        CB_CHECK(stdfs::exists(world.dir.path() / "rt" / "src" / "sample" / "Prog.java"));   // staged under its package, line ends as \n
    }
    // the Java lane declines
    {
        World world;
        cb::fs::write_text(world.file("Prog.java"), "public class Prog { public static void main(String[] a) {} }\n");
        world.host.java_to_cpp = [](const stdfs::path&, const stdfs::path&) -> std::string { throw cb::UnsupportedError("Java lane", 0, "a construct the C++ back end writes no code for"); };
        const std::vector<stdfs::path> files = {world.file("Prog.java")};
        const jj::Verdict v = jj::roundtrip(world.host, files, "Prog", world.dir.path() / "rt");
        CB_CHECK(v.verdict == jj::Kind::Refused);
        CB_CHECK(v.reason.starts_with("Java -> C++: "));
        CB_CHECK(v.reason.find("a construct the C++ back end writes no code for") != std::string::npos);
        CB_CHECK(v.native.has_value());
    }
    // the front end reads the C++ back and cannot
    {
        World world;
        cb::fs::write_text(world.file("Prog.java"), "public class Prog { public static void main(String[] a) {} }\n");
        world.refuses_to_lower = true;
        world.host.java_to_cpp = [](const stdfs::path&, const stdfs::path&) { return std::string(); };
        world.host.lift_java_runtime = [](cb::Arena&, std::span<ir::Node* const>) {};
        const std::vector<stdfs::path> files = {world.file("Prog.java")};
        const jj::Verdict v = jj::roundtrip(world.host, files, "Prog", world.dir.path() / "rt");
        CB_CHECK(v.verdict == jj::Kind::Refused);
        CB_CHECK(v.reason.starts_with("C++ -> Java: front end: clang: cannot parse Prog.cpp"));
    }
    // every stage goes through; the declined bodies are a note
    {
        World world;
        cb::fs::write_text(world.file("Prog.java"), "public class Prog { public static void main(String[] a) {} }\n");
        std::size_t lifted = 0;
        world.host.java_to_cpp = [](const stdfs::path&, const stdfs::path&) { return std::string("3 bodies"); };
        world.host.lift_java_runtime = [&](cb::Arena&, std::span<ir::Node* const> units) { lifted = units.size(); };
        const std::vector<stdfs::path> files = {world.file("Prog.java")};
        const jj::Verdict v = jj::roundtrip(world.host, files, "Prog", world.dir.path() / "rt", "Again");
        CB_CHECK_EQ(lifted, 1U);   // the lift saw the one unit
        CB_CHECK(v.verdict == jj::Kind::Equivalent);   // both runs are the stand-in java, which prints the same line
        CB_CHECK(std::find(v.notes.begin(), v.notes.end(), "Java -> C++ declined bodies: 3 bodies") != v.notes.end());
        CB_CHECK(stdfs::exists(world.dir.path() / "rt" / "java" / "Again.java"));
    }
});
