// Tests of the Java kind: the module (what is one, what it is filed under), the preparation over a transpiler made up in the test (a
// folder of Java source in, a folder of C++ out), the profile, the build's units, and the judge on the host compiler where there is one
// (a class that does not compile; one that does but has no JDK to run beside). Where the transpiler really runs and the class is run in
// the JVM is the probe's tests' and the product check's.

#include "verify/lanes/java.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/testing.hpp"

namespace cb::verify::lanes {
namespace {

namespace stdfs = std::filesystem;

void put_text(const stdfs::path& p, std::string_view text) { fs::write_text(p, text); }

// A Java source folder with three classes: Foo (has C++), Bar (a header only) and Marker (a decompiler's empty marker).
void make_java(const stdfs::path& dir) {
    put_text(dir / "game" / "Foo.java", "package game; public class Foo { public static int one() { return 1; } }\n");
    put_text(dir / "Bar.java", "public class Bar { }\n");
    put_text(dir / "Marker.java", "public class Marker { }\n");
}

// The transpiler made up: what it would write for those classes, and the runtime's support headers.
java::transpile::Summary fake_convert(const stdfs::path&, const stdfs::path& out) {
    put_text(out / "Foo.h", "// Foo.h\nclass Foo { public: static int one(); };\n");
    put_text(out / "Foo.cpp", "// Foo.cpp\n#include \"stdafx.h\"\n#include \"Foo.h\"\nint Foo::one() { return 1; }\n");
    put_text(out / "Bar.h", "// Bar.h\nclass Bar { };\n");
    put_text(out / "JavaRT.h", "// JavaRT.h: the runtime\n");
    put_text(out / "stdafx.h", "// stdafx.h\n");
    java::transpile::Summary s;
    s.markers = {"Marker"};
    return s;
}

JavaLane::Tools no_tools() { return {}; }

bool host_compiler_runs() {
    try {
        return process::run({.argv = {"g++", "--version"}}).ok();
    } catch (const ToolError&) {
        return false;
    }
}

CB_TEST("verify.lanes.java.a_module_is_a_folder_with_java_files_or_a_java_file", [] {
    fs::ScopedTempDir dir("cb_java_lane");
    make_java(dir.path() / "src");
    put_text(dir.path() / "none" / "readme.txt", "x");
    CB_CHECK(JavaLane::is_module(dir.path() / "src"));
    CB_CHECK(JavaLane::is_module(dir.path() / "src" / "Bar.java"));
    CB_CHECK(!JavaLane::is_module(dir.path() / "none"));
    CB_CHECK(!JavaLane::is_module(dir.path() / "none" / "readme.txt"));
    CB_CHECK(!JavaLane::is_module(dir.path() / "absent"));
    CB_CHECK(JavaLane::source_dir(dir.path() / "src") == dir.path() / "src");
    CB_CHECK(JavaLane::source_dir(dir.path() / "src" / "Bar.java") == stdfs::absolute(dir.path() / "src"));
});

CB_TEST("verify.lanes.java.a_module_is_filed_under_its_files_names_and_contents", [] {
    fs::ScopedTempDir dir("cb_java_lane");
    make_java(dir.path() / "a");
    make_java(dir.path() / "b");
    const std::string sha = JavaLane::module_sha(dir.path() / "a");
    CB_CHECK_EQ(sha.size(), 64u);
    CB_CHECK_EQ(JavaLane::module_sha(dir.path() / "b"), sha);                       // the same files, elsewhere
    CB_CHECK_EQ(JavaLane::module_sha(dir.path() / "a" / "Bar.java"), sha);          // a file stands for its folder
    put_text(dir.path() / "b" / "Bar.java", "public class Bar { int x; }\n");
    CB_CHECK(JavaLane::module_sha(dir.path() / "b") != sha);                        // a changed file
    put_text(dir.path() / "b" / "Bar.java", "public class Bar { }\n");
    CB_CHECK_EQ(JavaLane::module_sha(dir.path() / "b"), sha);
    put_text(dir.path() / "b" / "Extra.java", "class Extra { }\n");
    CB_CHECK(JavaLane::module_sha(dir.path() / "b") != sha);                        // a new file
    stdfs::remove(dir.path() / "b" / "Extra.java");
    stdfs::rename(dir.path() / "b" / "Bar.java", dir.path() / "b" / "Baz.java");
    CB_CHECK(JavaLane::module_sha(dir.path() / "b") != sha);                        // a renamed file
});

CB_TEST("verify.lanes.java.the_kind_says_what_it_is", [] {
    JavaLane lane(no_tools, fake_convert);
    CB_CHECK_EQ(lane.name(), "java");
    const reconstruct::KindTraits t = JavaLane::traits();
    CB_CHECK(t.per_function && !t.identifies_library);
    CB_CHECK_EQ(t.source_rev.value_or(""), "java-3");
    fs::ScopedTempDir dir("cb_java_lane");
    make_java(dir.path() / "src");
    CB_CHECK_EQ(t.sha(dir.path() / "src").value_or(""), JavaLane::module_sha(dir.path() / "src"));
    CB_CHECK((JavaLane::flags() == std::vector<std::string>{"-std=c++03", "-fsyntax-only", "-w"}));
});

CB_TEST("verify.lanes.java.prepare_gathers_each_class_and_the_support_headers", [] {
    fs::ScopedTempDir dir("cb_java_lane");
    make_java(dir.path() / "src");
    JavaLane lane(no_tools, fake_convert);
    const std::unique_ptr<Prepared> p = lane.prepare(dir.path() / "src", dir.path() / "work");
    const auto& m = static_cast<const JavaLane::PreparedJava&>(*p);
    CB_CHECK(m.out == dir.path() / "work" / "cpp");
    CB_CHECK(m.src == dir.path() / "src");
    // the markers are not classes; the classes are named by their .java file, whatever the package
    CB_CHECK((m.all_keys == std::vector<std::string>{"Bar", "Foo"}));
    CB_CHECK((m.order == std::vector<std::string>{"Bar", "Foo"}));
    CB_CHECK_EQ(m.key.at("Foo"), "Foo");
    CB_CHECK(m.sources.at("Foo") == dir.path() / "src" / "game" / "Foo.java");
    CB_CHECK_EQ(m.sources.count("Marker"), 1u);                  // (the file is there; the class is not judged)
    // a class's text is its header, then its source
    CB_CHECK_EQ(m.texts.at("Foo"), "// Foo.h\nclass Foo { public: static int one(); };\n\n// Foo.cpp\n#include \"stdafx.h\"\n#include \"Foo.h\"\nint Foo::one() { return 1; }\n");
    CB_CHECK_EQ(m.texts.at("Bar"), "// Bar.h\nclass Bar { };\n");
    CB_CHECK_EQ(m.texts.size(), 2u);
    // the support headers (every header that is no class's) are the module's header, in name order
    CB_CHECK_EQ(m.header, "// ---- JavaRT.h\n// JavaRT.h: the runtime\n\n// ---- stdafx.h\n// stdafx.h\n\n");
    CB_CHECK_EQ(m.sig.size(), 40u);
    // preparing again, after the transpiler wrote different text, gives another signature; the same text the same one
    const std::unique_ptr<Prepared> again = lane.prepare(dir.path() / "src", dir.path() / "work");
    CB_CHECK_EQ(again->sig, m.sig);
    JavaLane other(no_tools, [](const stdfs::path& s, const stdfs::path& o) {
        java::transpile::Summary r = fake_convert(s, o);
        put_text(o / "JavaRT.h", "// JavaRT.h: another runtime\n");
        return r;
    });
    CB_CHECK(other.prepare(dir.path() / "src", dir.path() / "work")->sig != m.sig);
});

CB_TEST("verify.lanes.java.a_jar_is_refused", [] {
    fs::ScopedTempDir dir("cb_java_lane");
    JavaLane lane(no_tools, fake_convert);
    CB_CHECK_THROWS_AS(UsageError, lane.prepare(dir.path() / "game.JAR", dir.path() / "work"));
});

CB_TEST("verify.lanes.java.the_profile_is_the_tools_and_there_is_none_without_a_compiler", [] {
    fs::ScopedTempDir dir("cb_java_lane");
    put_text(dir.path() / "cxx.exe", "compiler one");
    put_text(dir.path() / "javac.exe", "javac");
    JavaLane::Tools t;
    t.cxx = dir.path() / "cxx.exe";
    JavaLane lane([&] { return t; }, fake_convert);
    const Profile plain = lane.profile({});
    CB_CHECK_EQ(plain.digest.size(), 12u);
    CB_CHECK(plain.settings.find("java rev 3") == 0);
    CB_CHECK(plain.settings.find("-std=c++03 -fsyntax-only -w") != std::string::npos);
    CB_CHECK_EQ(lane.profile({}).digest, plain.digest);          // stable
    t.javac = dir.path() / "javac.exe";
    CB_CHECK(lane.profile({}).digest != plain.digest);           // a JDK is a different judge
    const std::string with_jdk = lane.profile({}).digest;
    t.libraries = {dir.path() / "lwjgl.jar"};
    CB_CHECK(lane.profile({}).digest != with_jdk);               // so are the game's libraries
    CB_CHECK(lane.profile({}).settings.find("libraries lwjgl.jar") != std::string::npos);
    put_text(dir.path() / "cxx.exe", "a newer, bigger compiler");
    CB_CHECK(lane.profile({}).digest != with_jdk);
    t.cxx.clear();
    CB_CHECK_THROWS_AS(NotFoundError, lane.profile({}));
});

CB_TEST("verify.lanes.java.the_build_has_a_unit_for_every_class_and_a_judge_for_each", [] {
    fs::ScopedTempDir dir("cb_java_lane");
    make_java(dir.path() / "src");
    JavaLane lane(no_tools, fake_convert);
    const std::unique_ptr<Prepared> p = lane.prepare(dir.path() / "src", dir.path() / "work");
    const Built built = lane.build(*p, dir.path() / "src", dir.path() / "work");
    CB_CHECK((built.items == std::vector<std::string>{"Bar", "Foo"}));
    CB_CHECK(built.missing.empty());
    // Foo has a .cpp of its own; Bar, a header only, gets a unit that includes it
    CB_CHECK(stdfs::is_regular_file(dir.path() / "work" / "units" / "Bar.cpp"));
    CB_CHECK(!stdfs::exists(dir.path() / "work" / "units" / "Foo.cpp"));
    CB_CHECK_EQ(fs::read_text(dir.path() / "work" / "units" / "Bar.cpp"), "#include \"stdafx.h\"\n#include \"Bar.h\"\n");
    CB_CHECK(static_cast<bool>(built.judge));
});

CB_TEST("verify.lanes.java.the_host_compiler_judges_what_compiles", [] {
    if (!host_compiler_runs()) return;   // (no g++ on this machine)
    fs::ScopedTempDir dir("cb_java_lane");
    put_text(dir.path() / "inc" / "good.cpp", "int one() { return 1; }\n");
    put_text(dir.path() / "inc" / "bad.cpp", "int one() { return ; }\nint two() { return nope; }\n");
    const JavaLane::Check good = JavaLane::check("g++", dir.path() / "inc", dir.path() / "inc" / "good.cpp");
    CB_CHECK(good.ok && good.errors.empty());
    const JavaLane::Check bad = JavaLane::check("g++", dir.path() / "inc", dir.path() / "inc" / "bad.cpp");
    CB_CHECK(!bad.ok);
    CB_CHECK(!bad.errors.empty());
    CB_CHECK(bad.errors.front().find("error") != std::string::npos);
    // a header every unit includes first
    put_text(dir.path() / "inc" / "stdafx.h", "typedef int jint;\n");
    put_text(dir.path() / "inc" / "uses.cpp", "jint one() { return 1; }\n");
    CB_CHECK(JavaLane::check("g++", dir.path() / "inc", dir.path() / "inc" / "uses.cpp", dir.path() / "inc" / "stdafx.h").ok);
    CB_CHECK(!JavaLane::check("g++", dir.path() / "inc", dir.path() / "inc" / "uses.cpp").ok);
    CB_CHECK_THROWS_AS(ToolError, JavaLane::check("no-such-compiler-here", dir.path() / "inc", dir.path() / "inc" / "good.cpp"));
});

CB_TEST("verify.lanes.java.a_class_that_does_not_compile_is_no_compile_and_one_that_does_has_nothing_to_run_beside", [] {
    if (!host_compiler_runs()) return;
    fs::ScopedTempDir dir("cb_java_lane");
    put_text(dir.path() / "inc" / "Good.cpp", "int one() { return 1; }\n");
    put_text(dir.path() / "inc" / "Bad.cpp", "int one() { return nope; }\n");
    JavaLane::Context context;
    context.tools.cxx = "g++";                       // (no JDK: javac is empty)
    context.include_dir = dir.path() / "inc";
    context.work = dir.path() / "work";
    const store::Judgement bad = JavaLane::judge_class(context, {"Bad", dir.path() / "inc" / "Bad.cpp", {}});
    CB_CHECK(bad.verdict == store::Verdict::NoCompile);
    CB_CHECK(bad.reason.find("error") != std::string::npos && bad.reason.size() <= 200);
    CB_CHECK(!bad.detail.empty());
    const store::Judgement good = JavaLane::judge_class(context, {"Good", dir.path() / "inc" / "Good.cpp", {}});
    CB_CHECK(good.verdict == store::Verdict::Inconclusive);
    CB_CHECK_EQ(good.reason, "not-run");
    CB_CHECK_EQ(good.detail, "compiles (g++ -std=c++03 -fsyntax-only -w); not run: no JDK (settings: java)");
});

CB_TEST("verify.lanes.java.the_dialect_is_an_option_of_the_lane", [] {
    fs::ScopedTempDir dir("cb_java_lane");
    put_text(dir.path() / "cxx.exe", "compiler one");
    JavaLane::Tools t;
    t.cxx = dir.path() / "cxx.exe";
    const auto tools = [&] { return t; };
    JavaLane c03(tools, fake_convert);
    JavaLane c17(tools, fake_convert, {.standard = "c++17"});
    CB_CHECK((JavaLane::flags("c++17") == std::vector<std::string>{"-std=c++17", "-fsyntax-only", "-w"}));
    CB_CHECK(c17.profile({}).settings.find("-std=c++17 -fsyntax-only -w") != std::string::npos);
    CB_CHECK(c17.profile({}).settings.find("c++03") == std::string::npos);
    CB_CHECK(c17.profile({}).digest != c03.profile({}).digest);      // a verdict is for one dialect
    CB_CHECK_EQ(c03.profile({}).digest, JavaLane(tools, fake_convert, {}).profile({}).digest);   // the default is the 360's
    if (!host_compiler_runs()) return;
    // the judge compiles in the dialect of its context: a C++11 form is a refusal in C++03
    put_text(dir.path() / "inc" / "Auto.cpp", "int f() { auto x = 1; return x; }\n");
    JavaLane::Context context;
    context.tools.cxx = "g++";
    context.include_dir = dir.path() / "inc";
    context.work = dir.path() / "work";
    CB_CHECK(JavaLane::judge_class(context, {"Auto", dir.path() / "inc" / "Auto.cpp", {}}).verdict == store::Verdict::NoCompile);
    context.standard = "c++17";
    const store::Judgement as17 = JavaLane::judge_class(context, {"Auto", dir.path() / "inc" / "Auto.cpp", {}});
    CB_CHECK(as17.verdict == store::Verdict::Inconclusive);
    CB_CHECK_EQ(as17.detail, "compiles (g++ -std=c++17 -fsyntax-only -w); not run: no JDK (settings: java)");
});

CB_TEST("verify.lanes.java.precompile_is_for_gcc_only", [] {
    fs::ScopedTempDir dir("cb_java_lane");
    CB_CHECK(!JavaLane::precompile("clang++.exe", dir.path() / "inc", dir.path() / "work"));
    CB_CHECK(!stdfs::exists(dir.path() / "work"));               // nothing was made for a compiler that cannot use it
    if (!host_compiler_runs()) return;
    put_text(dir.path() / "inc" / "stdafx.h", "typedef int jint;\n");
    put_text(dir.path() / "inc" / "JavaRT.h", "typedef long jlong;\n");
    const auto head = JavaLane::precompile("g++", dir.path() / "inc", dir.path() / "work");
    CB_CHECK(head.has_value());
    if (head) CB_CHECK(stdfs::is_regular_file(fs::to_text(*head) + ".gch"));
    put_text(dir.path() / "bad" / "stdafx.h", "this is not C++\n");
    put_text(dir.path() / "bad" / "JavaRT.h", "");
    CB_CHECK(!JavaLane::precompile("g++", dir.path() / "bad", dir.path() / "work2"));   // a header that does not compile: none
});

}  // namespace
}  // namespace cb::verify::lanes
