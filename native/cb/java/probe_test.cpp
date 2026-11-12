// Tests of cb/java/probe.hpp: the pure parts (inputs, literals, comparing, what can be run, the harness text) on small Java classes and
// emitted headers written here, and the whole probe -- javac, the JVM, the C++ compiler, the linker -- on a class and a hand-written
// translation of it, which runs only where those tools are installed.
//
// DIVERGES from the Python module:
//  - The class index is in path order (the Python's came from os.walk's order, unspecified); a class name declared twice keeps the first.
//  - The probe's settings (the C++ dialect, the libraries, the stand-ins) are a Toolchain handed down, not module variables.
//  - A harness literal of a String input is made from the string's UTF-16 units (a character above U+FFFF is two \u escapes, as Java
//    reads it; the Python wrote an invalid five-digit escape). The inputs are fixed and none is above U+FFFF.
//  - A static method is looked for in the header as `static`, a type (word characters, <>*, spaces and tabs), then the name and `(`;
//    the Python's pattern let `[\w<>* ]` run over any text of those characters, which is the same on emitted headers.
#include "cb/java/probe.hpp"

#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/parse.hpp"

using namespace cb;
namespace probe = cb::java::probe;
namespace ir = cb::java::ir;

// a check the rest of the test depends on: when it fails the test stops there
#define PROBE_REQUIRE(...) do { CB_CHECK(__VA_ARGS__); if (!(__VA_ARGS__)) return; } while (false)

namespace probe_test {

constexpr std::string_view kCalcJava = R"(package demo.calc;

public class Calc {
	public int value;
	private int secret;
	public static final int LIMIT = 5;

	public Calc(int v) {
		value = v;
	}

	public static int add(int a, int b) {
		return a + b;
	}

	public static String greet(String n) {
		return "hi " + n;
	}

	public int plus(int d) {
		return value + d;
	}

	public void bump() {
		value = value + 1;
	}

	public static boolean isEven(int a) {
		return a % 2 == 0;
	}

	public static long now() {
		return System.currentTimeMillis();
	}

	private int hidden() {
		return 1;
	}
}
)";

// what the transpiler emits for it (the header the probe reads; the program is C++03)
constexpr std::string_view kCalcHeader = R"(#pragma once
#include "stdafx.h"
#include "JavaRT.h"
class Calc {
public:
	int value;
	Calc(int v);
	static int add(int a, int b);
	static JavaString greet(JavaString n);
	int plus(int d);
	void bump();
	static bool isEven(int a);
	static long long now();
private:
	int secret;
	int hidden();
};
)";

std::string calc_cpp(bool correct) {
    return std::string("#include \"Calc.h\"\n"
                       "Calc::Calc(int v) : value(v), secret(0) {}\n"
                       "int Calc::add(int a, int b) { return a ") + (correct ? "+" : "-") + " b; }\n"
                       "JavaString Calc::greet(JavaString n) { return JavaString(\"hi \") + n; }\n"
                       "int Calc::plus(int d) { return value + d; }\n"
                       "void Calc::bump() { value = value + 1; }\n"
                       "bool Calc::isEven(int a) { return a % 2 == 0; }\n"
                       "long long Calc::now() { return 0; }\n"
                       "int Calc::hidden() { return 1; }\n";
}

// A staged Java tree with Calc in it and an emitted program for it.
struct Fixture {
    fs::ScopedTempDir dir{"probe"};
    std::filesystem::path java = dir.path() / "java", staged = dir.path() / "staged", cpp = dir.path() / "cpp", work = dir.path() / "work";
    explicit Fixture(bool correct = true) {
        fs::write_text(java / "Calc.java", kCalcJava);   // (flat: staging puts it under demo/calc)
        fs::write_text(cpp / "Calc.h", kCalcHeader);
        fs::write_text(cpp / "Calc.cpp", calc_cpp(correct));
        fs::write_text(cpp / "stdafx.h", "#pragma once\n");
        fs::write_text(cpp / "JavaRT.h", "#pragma once\n#include <string>\n#include <typeinfo>\ntypedef std::string JavaString;\ninline void j2xRegisterBase(const char*, const char*) {}\n");
        probe::stage_sources(java, staged);
    }
};

// (std::string::contains is C++23)
bool has(std::string_view haystack, std::string_view needle) { return haystack.find(needle) != std::string_view::npos; }

bool tool_runs(const char* name, const char* flag) {
    try {
        return process::run({.argv = {name, flag}}).ok();
    } catch (const ToolError&) {
        return false;
    }
}

probe::Toolchain installed_tools() {
    probe::Toolchain t;
    t.cxx = "g++";
    t.javac = "javac";
    t.java = "java";
    return t;
}

bool have_tools() { return tool_runs("g++", "--version") && tool_runs("javac", "-version") && tool_runs("java", "-version"); }

CB_TEST("java.probe.inputs", [] {
    using Cases = std::vector<std::vector<std::string>>;
    // few enough: every combination, the last varying fastest
    const Cases bools = probe::cases({"boolean", "boolean"});
    CB_CHECK(bools == (Cases{{"true", "true"}, {"true", "false"}, {"false", "true"}, {"false", "false"}}));
    CB_CHECK(probe::cases({}) == Cases{{}});
    CB_CHECK_EQ(probe::cases({"int"}).size(), std::size_t{15});
    CB_CHECK_EQ(probe::cases({"int"}).front()[0], "0");
    CB_CHECK_EQ(probe::cases({"boolean", "short"}).size(), std::size_t{12});
    // too many: a fixed spread, then every input of every parameter once more if it was missed
    const Cases wide = probe::cases({"int", "int"});
    // 225 combinations: the stride walk of 48 steps repeats after 15 (15 distinct rows), and ten more rows bring in the second
    // parameter's inputs the walk skipped (it only reaches every third); the Python module gave the same 25
    CB_CHECK_EQ(wide.size(), std::size_t{25});
    CB_CHECK(wide[0] == (std::vector<std::string>{"0", "1"}));
    CB_CHECK(wide[1] == (std::vector<std::string>{"1", "7"}));   // k-th parameter at stride 2k+1, offset k
    CB_CHECK(wide == probe::cases({"int", "int"}));
    CB_CHECK(wide[14] == (std::vector<std::string>{"-2147483648", "2147483647"}));
    CB_CHECK(wide[15] == (std::vector<std::string>{"0", "0"}));   // the first added row: the walk never had a 0 second
    for (std::size_t k = 0; k < 2; ++k) {
        for (const char* v : {"0", "1", "-1", "2", "7", "-13", "100", "255", "256", "1000", "-1000", "65535", "123456", "2147483647", "-2147483648"}) {
            bool seen = false;
            for (const auto& row : wide) seen = seen || row[k] == v;
            CB_CHECK(seen);
        }
    }
    const Cases mixed = probe::cases({"String", "double", "long", "boolean"});
    CB_CHECK_EQ(mixed.size(), std::size_t{56});
    for (const auto& row : mixed) CB_CHECK_EQ(row.size(), std::size_t{4});
    CB_CHECK_THROWS_AS(UsageError, probe::cases({"Object"}));
});

CB_TEST("java.probe.literals", [] {
    CB_CHECK_EQ(probe::java_literal("int", "7"), "7");
    CB_CHECK_EQ(probe::java_literal("byte", "-1"), "(byte)-1");
    CB_CHECK_EQ(probe::java_literal("char", "65"), "(char)65");
    CB_CHECK_EQ(probe::java_literal("long", "5L"), "5L");
    CB_CHECK_EQ(probe::java_literal("float", "-0.0F"), "-0.0F");
    CB_CHECK_EQ(probe::java_literal("String", "a\"b\\c"), "\"a\\u0022b\\u005cc\"");
    CB_CHECK_EQ(probe::java_literal("String", "\xC2\xA7" "cRed"), "\"\\u00a7cRed\"");
    CB_CHECK_EQ(probe::java_literal("String", ""), "\"\"");

    CB_CHECK_EQ(probe::cpp_literal("int", "7"), "7");
    CB_CHECK_EQ(probe::cpp_literal("int", "-2147483648"), "(-2147483647 - 1)");
    CB_CHECK_EQ(probe::cpp_literal("long", "5L"), "5LL");
    CB_CHECK_EQ(probe::cpp_literal("long", "-123456789012L"), "-123456789012LL");
    CB_CHECK_EQ(probe::cpp_literal("long", "9223372036854775807L"), "9223372036854775807LL");
    CB_CHECK_EQ(probe::cpp_literal("double", "-0.5"), "(-0.5)");
    CB_CHECK_EQ(probe::cpp_literal("float", "1.0F"), "1.0F");
    CB_CHECK_EQ(probe::cpp_literal("byte", "-128"), "(signed char)-128");
    CB_CHECK_EQ(probe::cpp_literal("char", "167"), "(unsigned char)167");
    CB_CHECK_EQ(probe::cpp_literal("String", "x=1,y"), "JavaString(\"x=1,y\")");
    // a byte above 127 is its own escape, closed off from the digits that follow
    CB_CHECK_EQ(probe::cpp_literal("String", "\xC2\xA7" "cRed"), "JavaString(\"\\xa7\" \"cRed\")");
});

CB_TEST("java.probe.compare", [] {
    const auto printed = probe::parse_results("0.0|1f\r\n  0.1|throw \n\nnoseparator\n|nokey\n0.2|a|b\n");
    CB_CHECK_EQ(printed.size(), std::size_t{4});
    CB_CHECK_EQ(printed.at("0.0"), "1f");
    CB_CHECK_EQ(printed.at("0.1"), "throw");   // (the line is trimmed, then split at the first bar; the key is not trimmed again)
    CB_CHECK_EQ(printed.at("noseparator"), "");
    CB_CHECK_EQ(printed.at("0.2"), "a|b");

    CB_CHECK(probe::same("int", "5", "5"));
    CB_CHECK(!probe::same("int", "5", "6"));
    CB_CHECK(!probe::same("String", "61", "62"));
    // floats by bits; any NaN is any other NaN, and only a NaN is
    CB_CHECK(probe::same("float", "7fc00000", "7f800001"));
    CB_CHECK(probe::same("double", "7ff8000000000000", "fff8000000000001"));
    CB_CHECK(!probe::same("float", "7f800000", "ff800000"));   // the infinities differ
    CB_CHECK(!probe::same("float", "7fc00000", "3f800000"));
    CB_CHECK(!probe::same("float", "zzz", "7fc00000"));
    CB_CHECK(!probe::same("float", "17fc00000", "7fc00000"));   // not 32 bits
    CB_CHECK(!probe::same("double", "0", "8000000000000000"));  // +0.0 and -0.0 differ
});

CB_TEST("java.probe.what_can_be_run", [] {
    Fixture f;
    const probe::SourceTree tree(f.staged);
    CB_CHECK(std::filesystem::exists(f.staged / "demo" / "calc" / "Calc.java"));
    const ir::JClass* calc = tree.lookup("Calc");
    PROBE_REQUIRE(calc != nullptr);
    CB_CHECK_EQ(calc->name, "Calc");
    CB_CHECK(tree.lookup("Calc") == calc);        // parsed once
    CB_CHECK(tree.lookup("Nothing") == nullptr);

    const auto made = probe::maker(calc);
    PROBE_REQUIRE(made.has_value());
    CB_CHECK(!made->factory.has_value());
    CB_CHECK(made->params == std::vector<std::string>{"int"});
    CB_CHECK(!probe::maker(nullptr).has_value());

    // the state a class is compared by: its non-static, non-private primitive fields the header declares
    const probe::FieldList fields = probe::comparable_fields(tree, *calc, f.cpp);
    CB_CHECK(fields == (probe::FieldList{{"value", "int"}}));   // `secret` is private, LIMIT static

    const auto methods = probe::probeable(tree, *calc, kCalcHeader, f.cpp);
    std::vector<std::string> names;
    for (const auto& m : methods) names.push_back(m.name);
    // now() reads the clock and is left out, hidden() is private; constructors are not methods to call
    CB_CHECK(names == (std::vector<std::string>{"add", "greet", "plus", "bump", "isEven"}));
    CB_CHECK(!methods[0].made.has_value());   // static
    CB_CHECK(methods[2].made.has_value());    // an instance method: made by the constructor
    CB_CHECK(methods[3].outcome.kind == probe::Outcome::Kind::Self);   // a void instance method: the object afterwards
    CB_CHECK(methods[3].outcome.fields == fields);
    CB_CHECK_EQ(methods[0].outcome.type, "int");
    CB_CHECK_EQ(methods[4].outcome.type, "boolean");

    // a method the header does not declare is not run
    const auto fewer = probe::probeable(tree, *calc, "class Calc { static int add(int a, int b); };", f.cpp);
    CB_CHECK_EQ(fewer.size(), std::size_t{1});
    CB_CHECK_EQ(probe::counted(*calc), std::size_t{8});   // the constructor and seven methods, none abstract

    const probe::ClockReads clock = probe::reads_clock(*calc);
    CB_CHECK(clock.methods == std::vector<std::string>{"now"});
    CB_CHECK(!clock.constructors);
    CB_CHECK(clock.any());
});

CB_TEST("java.probe.clock_reads_reach_callers", [] {
    const ir::JClass c = cb::java::parse::parse_source(R"(public class Timer {
	long last;
	public Timer() { }
	public long read() { return System.nanoTime(); }
	public long viaThis() { return this.read() + 1; }
	public long plain() { return read(); }
	public long other(Timer t) { return t.read(); }
	public int fine() { return 3; }
})");
    const probe::ClockReads r = probe::reads_clock(c);
    // read() reads it, and so do the methods that call it as their own (not t.read(): another object's)
    CB_CHECK(r.methods == (std::vector<std::string>{"plain", "read", "viaThis"}));
    CB_CHECK(!r.constructors);
    const ir::JClass d = cb::java::parse::parse_source(R"(public class Stamp {
	long at = System.currentTimeMillis();
	public int value() { return 1; }
})");
    const probe::ClockReads s = probe::reads_clock(d);
    CB_CHECK(s.methods.empty());
    CB_CHECK(s.constructors);   // a field initialiser reading the clock reaches every constructor
});

CB_TEST("java.probe.harness_text", [] {
    Fixture f;
    const probe::SourceTree tree(f.staged);
    const ir::JClass& calc = *tree.lookup("Calc");
    const auto methods = probe::probeable(tree, calc, kCalcHeader, f.cpp);

    const std::string java = probe::java_harness("Calc", methods, "demo.calc", "J2XProbe_Calc");
    CB_CHECK(java.starts_with("package demo.calc;\npublic class J2XProbe_Calc {\n  static void out(String k, String v)"));
    CB_CHECK(has(java, "  static String dump_Calc(Calc o) { if (o == null) return \"null\"; return \"\" + \"value:\" + fi(o.value); }"));
    CB_CHECK(has(java, "    try { out(\"0.0\", fi(Calc.add(0, 1))); } catch (Throwable t) { out(\"0.0\", \"throw\"); }"));
    CB_CHECK(has(java, "out(\"1.1\", fs(Calc.greet(\"a\")));"));
    CB_CHECK(has(java, "out(\"2.0\", fi((new Calc(0)).plus(1)));"));
    CB_CHECK(has(java, "Calc o = new Calc(0); o.bump(); out(\"3.0\", dump_Calc(o));"));
    CB_CHECK(has(java, "out(\"4.1\", fb(Calc.isEven(1)));"));
    CB_CHECK(java.ends_with("  }\n}\n"));

    // only the cases asked for
    const std::set<std::string> only = {"0.0", "4.1"};
    const std::string some = probe::java_harness("Calc", methods, std::nullopt, "J2XProbe_Calc", &only);
    CB_CHECK(some.starts_with("public class J2XProbe_Calc {"));
    CB_CHECK(has(some, "\"0.0\""));
    CB_CHECK(!has(some, "\"0.1\""));
    CB_CHECK(has(some, "\"4.1\""));

    const std::set<std::string> keep = {"0.0", "1.1", "2.3", "3.0"};
    const std::string cpp = probe::cpp_harness("Calc", methods, keep, {"Calc"}, {"Other"}, {{"Calc", "Base"}});
    CB_CHECK(cpp.starts_with("#include \"stdafx.h\"\n#include \"JavaRT.h\"\n#include \"Calc.h\"\n#include \"Other.h\"\n#include \"Base.h\"\n#include <cstdio>\n"));
    CB_CHECK(has(cpp, "static std::string dump_Calc(Calc* o) { if (o == NULL) return \"null\"; return std::string() + \"value:\" + fi(o->value); }"));
    CB_CHECK(has(cpp, "int main() {\n\tj2xRegisterBase(typeid(Calc).name(), typeid(Base).name());\n\tCalc::initStatics();\n"));
    CB_CHECK(has(cpp, "\t{ out(\"0.0\", fi(Calc::add(0, 1))); }\n"));
    CB_CHECK(has(cpp, "\t{ out(\"1.1\", fs(Calc::greet(JavaString(\"a\")))); }\n"));
    CB_CHECK(has(cpp, "\t{ Calc* o = new Calc(0); o->bump(); out(\"3.0\", dump_Calc(o)); }\n"));
    CB_CHECK(!has(cpp, "\"4.1\""));   // not kept
    CB_CHECK(cpp.ends_with("\treturn 0;\n}\n"));
});

CB_TEST("java.probe.program_files", [] {
    fs::ScopedTempDir dir("probefiles");
    fs::write_text(dir.path() / "j2x_rtti.cpp", "void f() {\n  j2xRegisterBase(typeid(Zombie).name(), typeid(Mob).name());\n  j2xRegisterBase(typeid(Mob).name(), typeid(Entity).name());\n}\n");
    fs::write_text(dir.path() / "j2x_init.cpp", "void g() {\n\tEntity::initStatics();\n  Mob::initStatics();\n // Old::initStatics();\n}\n");
    const auto edges = probe::read_ancestry(dir.path());
    CB_CHECK(edges == (std::vector<std::pair<std::string, std::string>>{{"Zombie", "Mob"}, {"Mob", "Entity"}}));
    CB_CHECK(probe::read_init_order(dir.path()) == (std::vector<std::string>{"Entity", "Mob"}));
    CB_CHECK(probe::read_ancestry(dir.path() / "none").empty());
    CB_CHECK(probe::read_init_order(dir.path() / "none").empty());
});

CB_TEST("java.probe.staging_and_libraries", [] {
    fs::ScopedTempDir dir("probestage");
    fs::write_text(dir.path() / "src" / "a" / "One.java", "package p.q;\nclass One {}\n");
    fs::write_text(dir.path() / "src" / "b" / "One.java", "package other;\nclass One {}\n");   // the same name: the first path wins
    fs::write_text(dir.path() / "src" / "Two.java", "  package   r ;\nclass Two {}\n");
    fs::write_text(dir.path() / "src" / "Three.java", "class Three {}\n");
    fs::write_text(dir.path() / "src" / "notes.txt", "x");
    probe::stage_sources(dir.path() / "src", dir.path() / "out");
    CB_CHECK(std::filesystem::exists(dir.path() / "out" / "p" / "q" / "One.java"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "out" / "other"));
    CB_CHECK(std::filesystem::exists(dir.path() / "out" / "r" / "Two.java"));
    CB_CHECK(std::filesystem::exists(dir.path() / "out" / "Three.java"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "out" / "notes.txt"));

    const probe::SourceTree tree(dir.path() / "out");
    CB_CHECK(tree.lookup("One") != nullptr);
    CB_CHECK(tree.lookup("Three") != nullptr);

    fs::write_text(dir.path() / "libs" / "b.JAR", "x");
    fs::write_text(dir.path() / "libs" / "a.jar", "x");
    fs::write_text(dir.path() / "libs" / "c.txt", "x");
    const auto jars = probe::list_libraries(dir.path() / "libs");
    CB_CHECK_EQ(jars.size(), std::size_t{2});
    CB_CHECK_EQ(jars[0].filename().string(), "a.jar");
    CB_CHECK(probe::list_libraries(dir.path() / "none").empty());

    // a source that does not parse is a class with nothing to run
    fs::write_text(dir.path() / "out" / "Broken.java", "this is not java {");
    const probe::SourceTree again(dir.path() / "out");
    CB_CHECK(again.lookup("Broken") == nullptr);
});

CB_TEST("java.probe.the_whole_probe", [] {
    if (!have_tools()) return;   // needs a JDK and g++ on the PATH
    Fixture f;
    const probe::SourceTree tree(f.staged);
    const probe::Toolchain tools = installed_tools();
    const ir::JClass& calc = *tree.lookup("Calc");

    // the translation computes what the Java does: every method of the class runs on both sides and agrees
    const std::vector<probe::ClassPlan> todo = probe::plan(tree, f.cpp, {"Calc", "Missing"});
    PROBE_REQUIRE(todo.size() == 1);
    CB_CHECK_EQ(todo[0].methods.size(), std::size_t{5});
    const probe::Compiled compiled = probe::compile_java(tree, todo, f.work, tools);
    PROBE_REQUIRE(compiled.contains("Calc"));
    CB_CHECK(compiled.at("Calc").ok());
    CB_CHECK_EQ(compiled.at("Calc").main_class, "demo.calc.J2XProbe_Calc");
    const probe::RunResult good = probe::run(tree, f.cpp, calc, f.work, tools, &compiled);
    CB_CHECK_EQ(good.why, "");
    CB_CHECK_EQ(good.methods.size(), std::size_t{5});
    CB_CHECK_EQ(good.judged, std::size_t{5});
    CB_CHECK_EQ(good.total, std::size_t{8});
    CB_CHECK(good.cases > 40);
    CB_CHECK(good.differs.empty());
    CB_CHECK_EQ(good.unsteady, std::size_t{0});
});

CB_TEST("java.probe.a_translation_that_differs", [] {
    if (!have_tools()) return;
    Fixture f(/*correct=*/false);   // add() subtracts
    const probe::SourceTree tree(f.staged);
    const probe::Toolchain tools = installed_tools();
    const probe::RunResult r = probe::run(tree, f.cpp, *tree.lookup("Calc"), f.work, tools);   // (compiled alone: no batch given)
    CB_CHECK_EQ(r.why, "");
    CB_CHECK_EQ(r.judged, std::size_t{5});
    PROBE_REQUIRE(!r.differs.empty());
    for (const probe::Difference& d : r.differs) CB_CHECK_EQ(d.method, "add");
    // the first case is 0 + 0 = 0 - 0: it agrees; a case with b != 0 does not
    bool saw_difference = false;
    for (const probe::Difference& d : r.differs) {
        saw_difference = saw_difference || (d.inputs.size() == 2 && d.inputs[1] != "0" && d.java != d.native);
    }
    CB_CHECK(saw_difference);
});

CB_TEST("java.probe.nothing_to_run", [] {
    Fixture f;
    const probe::SourceTree tree(f.staged);
    probe::Toolchain tools;   // (nothing below starts a tool)
    // a header that declares none of the methods: nothing it can call
    fs::write_text(f.cpp / "Calc.h", "class Calc {};\n");
    const probe::RunResult r = probe::run(tree, f.cpp, *tree.lookup("Calc"), f.work, tools);
    CB_CHECK_EQ(r.judged, std::size_t{0});
    CB_CHECK(r.methods.empty());
    CB_CHECK_EQ(r.why, "it reads the clock or the clipboard (they differ between the two runs)");   // (Calc.now() reads it)
    // no header at all: the same answer
    std::filesystem::remove(f.cpp / "Calc.h");
    CB_CHECK_EQ(probe::run(tree, f.cpp, *tree.lookup("Calc"), f.work, tools).why, "it reads the clock or the clipboard (they differ between the two runs)");

    // a class with no clock: the other reason
    fs::write_text(f.staged / "Plain.java", "public class Plain { public int x() { return 1; } }\n");
    const probe::SourceTree tree2(f.staged);
    fs::write_text(f.cpp / "Plain.h", "class Plain {};\n");
    CB_CHECK_EQ(probe::run(tree2, f.cpp, *tree2.lookup("Plain"), f.work, tools).why,
                "no method it can call and compare (its arguments made, its result or object read back)");
});

}  // namespace probe_test
