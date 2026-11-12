// Tests of the Java judge: the pieces that decide what is run (which methods, which inputs, which harness), the verdict words,
// the tree judge over a lane that is made up, and -- when the machine has a JDK -- a whole program judged for real: a Java
// class compiled and run by the JVM, and a "translation" (a batch file standing in for the program a back end built) that
// answers as it should, and as it should not.
//
// DIVERGES from javajudge.py:
//   * judge_tree returns its classes in the order the lane made them, whichever finished first (the Python's dict was in the
//     order of completion), and is silent unless given a log (it printed by default);
//   * judge_program answers NotRun with the reason when there is no JDK (the Python raised RuntimeError); the harness and the
//     Java sources it writes have "\n" line ends (Python's text mode wrote "\r\n" on Windows);
//   * the front end, probe, builds and lane are given to it (ProgramHost, ProbeInputs, TreeLane) instead of imported; the
//     lane's module variables (STD, FLAGS ...) the Python reassigned and restored are the lane's own business, set in
//     TreeLane::prepare;
//   * a program that a back end declines (an exception from the build) is Refused with the reason, as before; an exception of
//     another kind (a failure of the judge's own) is not caught.
#include "cb/translate/javajudge.hpp"

#include <algorithm>
#include <atomic>
#include <mutex>
#include <stdexcept>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace ir = cb::translate::ir;
namespace jj = cb::translate::javajudge;
namespace stdfs = std::filesystem;
using cb::Arena;
using ir::Node;

// ---- trees ------------------------------------------------------------------------------------------------------------

ir::TypeP string_type() { return ir::class_type("java.lang.String"); }

Node* param(Arena& a, const char* name, ir::TypeP type) {
    ir::Param p;
    p.sym = ir::make_sym(ir::SymKind::Param, name, std::nullopt, false, type);
    p.type = std::move(type);
    return ir::make(a, std::move(p));
}

struct MethodSpec {
    const char* name = "m";
    bool is_static = true;
    std::vector<std::string> mods;
    std::vector<ir::TypeP> params;
    ir::TypeP ret = ir::i32_type();
    std::vector<std::string> typeparams;
};

Node* method(Arena& a, const MethodSpec& spec) {
    ir::MethodDecl m;
    m.name = spec.name;
    m.kind = "method";
    m.sym = ir::make_sym(ir::SymKind::Method, spec.name, "C", spec.is_static);
    m.mods = spec.mods;
    m.ret = spec.ret;
    m.typeparams = spec.typeparams;
    int n = 0;
    for (const ir::TypeP& t : spec.params) m.params.push_back(param(a, ("p" + std::to_string(n++)).c_str(), t));
    return ir::make(a, std::move(m));
}

Node* class_of(Arena& a, const char* name, std::vector<Node*> members, const char* kind = "class", std::vector<std::string> mods = {}) {
    ir::ClassDecl c;
    c.name = name;
    c.kind = kind;
    c.mods = std::move(mods);
    c.members = std::move(members);
    return ir::make(a, std::move(c));
}

Node* unit_of(Arena& a, std::vector<Node*> types, std::optional<std::string> package = std::nullopt) {
    ir::Unit u;
    u.lang = "java";
    u.package = std::move(package);
    u.types = std::move(types);
    return ir::make(a, std::move(u));
}

jj::ProbeInputs fake_inputs() {
    jj::ProbeInputs in;
    in.cases = [](const std::vector<std::string>& types) {
        std::vector<std::vector<std::string>> out;
        for (const char* v : {"1", "2"}) out.emplace_back(types.size(), v);
        return out;
    };
    in.literal = [](const std::string& type, const std::string& value) { return type == "String" ? "\"" + value + "\"" : value; };
    in.results = [](std::string_view output) {
        cb::OrderedMap<std::string, std::string> out;
        for (const std::string_view line : cb::text::lines(output)) {
            const auto bar = line.find('|');
            if (bar != std::string_view::npos) out.insert_or_assign(std::string(line.substr(0, bar)), std::string(cb::text::trim(line.substr(bar + 1))));
        }
        return out;
    };
    in.same = [](const std::string&, const std::string& a, const std::string& b) { return a == b; };
    return in;
}

}  // namespace

CB_TEST("translate.javajudge.words_and_verdicts", [] {
    CB_CHECK_EQ(jj::word(jj::Verdict::Equivalent), std::string_view("equivalent"));
    CB_CHECK_EQ(jj::word(jj::Verdict::Divergent), std::string_view("divergent"));
    CB_CHECK_EQ(jj::word(jj::Verdict::Refused), std::string_view("refused"));
    CB_CHECK_EQ(jj::word(jj::Verdict::NotRun), std::string_view("not-run"));
    CB_CHECK(jj::verdict_of(jj::LaneVerdict::NoCompile) == jj::Verdict::Refused);
    CB_CHECK(jj::verdict_of(jj::LaneVerdict::Divergent) == jj::Verdict::Divergent);
    CB_CHECK(jj::verdict_of(jj::LaneVerdict::Good) == jj::Verdict::Equivalent);
    CB_CHECK(jj::verdict_of(jj::LaneVerdict::Other) == jj::Verdict::NotRun);
    CB_CHECK_EQ(jj::backend_name(jj::Backend::Cpp03), std::string_view("cpp03"));
    CB_CHECK_EQ(jj::backend_name(jj::Backend::Cpp17), std::string_view("cpp17"));
});

CB_TEST("translate.javajudge.java_names", [] {
    const std::vector<std::pair<ir::Prim, const char*>> prims = {{ir::Prim::I32, "int"},  {ir::Prim::I64, "long"},   {ir::Prim::I16, "short"},
                                                                 {ir::Prim::I8, "byte"},  {ir::Prim::U16, "char"},   {ir::Prim::Bool, "boolean"},
                                                                 {ir::Prim::F32, "float"}, {ir::Prim::F64, "double"}};
    for (const auto& [prim, name] : prims) CB_CHECK_EQ(jj::java_name(ir::prim_type(prim)).value_or("?"), std::string_view(name));
    CB_CHECK_EQ(jj::java_name(string_type()).value_or("?"), std::string_view("String"));
    // what Java has no such primitive for, a class that is not String, a void, an array, nothing
    CB_CHECK(!jj::java_name(ir::prim_type(ir::Prim::U32)));
    CB_CHECK(!jj::java_name(ir::class_type("java.util.List")));
    CB_CHECK(!jj::java_name(ir::void_type()));
    CB_CHECK(!jj::java_name(ir::array_of(ir::i32_type())));
    CB_CHECK(!jj::java_name({}));
});

CB_TEST("translate.javajudge.methods_the_harness_can_call", [] {
    Arena a;
    const ir::TypeP i = ir::i32_type(), s = string_type();
    Node* inner = class_of(a, "Inner", {method(a, {.name = "twice", .params = {i}})});
    Node* hidden = class_of(a, "Hidden", {method(a, {.name = "never", .params = {i}})}, "class", {"private"});
    Node* shape = class_of(a, "Shape", {method(a, {.name = "iface", .params = {i}})}, "interface");
    ir::ClassDecl generic;
    generic.name = "Box";
    generic.kind = "class";
    generic.typeparams = {"T"};
    generic.members = {method(a, {.name = "boxed", .params = {i}})};
    Node* box = ir::make(a, std::move(generic));
    Node* calc = class_of(a, "Calc",
                          {method(a, {.name = "add", .params = {i, i}}),                                    // taken
                           method(a, {.name = "greet", .params = {s}, .ret = s}),                           // taken: String in and out
                           method(a, {.name = "hidden", .mods = {"private"}, .params = {i}}),               // private
                           method(a, {.name = "main", .params = {ir::array_of(s)}, .ret = ir::void_type()}),
                           method(a, {.name = "inst", .is_static = false, .params = {i}}),                  // not static
                           method(a, {.name = "list", .params = {ir::class_type("java.util.List")}}),       // a parameter that is no primitive
                           method(a, {.name = "returns_list", .params = {i}, .ret = ir::class_type("java.util.List")}),
                           method(a, {.name = "gen", .params = {i}, .typeparams = {"T"}}),                  // generic
                           inner, hidden, shape, box});
    Node* harness = class_of(a, std::string(jj::kHarness).c_str(), {method(a, {.name = "skipped", .params = {i}})});
    Node* other = unit_of(a, {class_of(a, "Other", {method(a, {.name = "elsewhere", .params = {i}})})}, "q");
    Node* mine = unit_of(a, {calc, harness});
    const std::vector<Node*> units = {mine, other};
    // two packages: the program is in the default one, so the unit of package q is not its own
    CB_CHECK_EQ(jj::package_of(units), std::string(""));
    const std::vector<jj::ProbeMethod> got = jj::probe_methods(units);
    std::vector<std::string> names;
    for (const jj::ProbeMethod& m : got) names.push_back(m.owner + "." + m.name);
    CB_CHECK((names == std::vector<std::string>{"Calc.add", "Calc.greet", "Calc.Inner.twice"}));
    CB_CHECK((got[0].params == std::vector<std::string>{"int", "int"}) && got[0].result == "int");
    CB_CHECK((got[1].params == std::vector<std::string>{"String"}) && got[1].result == "String");
    // one package: it is the program's
    const std::vector<Node*> in_q = {unit_of(a, {class_of(a, "Q", {method(a, {.name = "f", .params = {i}})})}, "p.q")};
    CB_CHECK_EQ(jj::package_of(in_q), std::string("p.q"));
    CB_CHECK_EQ(jj::probe_methods(in_q).size(), std::size_t(1));
    CB_CHECK_EQ(jj::package_of({}), std::string(""));
    // the classes with a main
    CB_CHECK((jj::main_classes(units) == std::vector<std::string>{"Calc"}));
});

CB_TEST("translate.javajudge.the_harness", [] {
    const std::vector<jj::ProbeMethod> methods = {{"Calc", "add", {"int", "int"}, "int"}, {"Calc.Inner", "up", {"String"}, "String"}};
    const jj::Harness h = jj::harness(methods, "p", fake_inputs());
    // two cases of each method, keyed by method and case
    CB_CHECK_EQ(h.cases.size(), std::size_t(4));
    const jj::ProbeCase* c = h.cases.find("0.1");
    CB_CHECK(c != nullptr);
    CB_CHECK_EQ(c->method, std::string("Calc.add"));
    CB_CHECK((c->values == std::vector<std::string>{"2", "2"}));
    CB_CHECK_EQ(c->result_type, std::string("int"));
    CB_CHECK_EQ(h.cases.find("1.0")->method, std::string("Calc.Inner.up"));
    CB_CHECK_EQ(h.cases.find("1.0")->result_type, std::string("String"));
    // the source: the package, the class, the helpers, one try per case in the format of the result's type
    CB_CHECK(h.source.starts_with("package p;\npublic class J2XProbe {\n"));
    CB_CHECK(h.source.find("static String fs(String v)") != std::string::npos);
    CB_CHECK(h.source.find("  public static void main(String[] a) {\n") != std::string::npos);
    CB_CHECK(h.source.find("    try { out(\"0.0\", fi(Calc.add(1, 1))); } catch (Throwable t) { out(\"0.0\", \"throw\"); }\n") != std::string::npos);
    CB_CHECK(h.source.find("    try { out(\"1.1\", fs(Calc.Inner.up(\"2\"))); } catch (Throwable t) { out(\"1.1\", \"throw\"); }\n") != std::string::npos);
    CB_CHECK(h.source.ends_with("  }\n}\n"));
    // the default package has no package line
    CB_CHECK(jj::harness(methods, "", fake_inputs()).source.starts_with("public class J2XProbe {\n"));
    // no methods: a harness with a main that does nothing
    const jj::Harness none = jj::harness({}, "", fake_inputs());
    CB_CHECK(none.cases.empty());
    CB_CHECK(none.source.find("public static void main(String[] a) {\n  }\n}\n") != std::string::npos);
    // each result type has its format: int-like, char, boolean, float, double
    const std::vector<jj::ProbeMethod> kinds = {{"C", "a", {}, "long"}, {"C", "b", {}, "char"}, {"C", "c", {}, "boolean"}, {"C", "d", {}, "float"}, {"C", "e", {}, "double"}};
    const std::string src = jj::harness(kinds, "", fake_inputs()).source;
    for (const char* want : {"fi(C.a(", "fc(C.b(", "fb(C.c(", "ff(C.d(", "fd(C.e("}) CB_CHECK(src.find(want) != std::string::npos);
});

CB_TEST("translate.javajudge.stage_sources", [] {
    cb::fs::ScopedTempDir tmp("jj-stage");
    const stdfs::path src = tmp.path() / "src";
    cb::fs::write_text(src / "A.java", "class A {}");
    cb::fs::write_text(src / "p" / "q" / "B.java", "class B {}");
    cb::fs::write_text(src / "notes.txt", "not java");
    const stdfs::path dest = tmp.path() / "stage";
    cb::fs::write_text(dest / "Old.java", "stale");   // what was there goes
    jj::stage_sources(src, dest);
    CB_CHECK(stdfs::exists(dest / "A.java"));
    CB_CHECK(stdfs::exists(dest / "B.java"));   // flat
    CB_CHECK(!stdfs::exists(dest / "notes.txt"));
    CB_CHECK(!stdfs::exists(dest / "Old.java"));
    CB_CHECK_EQ(cb::fs::read_text(dest / "B.java"), std::string("class B {}"));
});

namespace {

// A lane that is made up: each class has a verdict, some throw.
class FakeLane final : public jj::TreeLane {
public:
    std::vector<std::string> prepare(const stdfs::path&, const stdfs::path&, jj::Backend backend) override {
        prepared_with = backend;
        return {"A", "B", "C", "D"};
    }
    jj::LaneResult judge(const std::string& cls) override {
        ++judged;
        if (cls == "A") return {jj::LaneVerdict::Good, "alike", "all 12 cases"};
        if (cls == "B") return {jj::LaneVerdict::Divergent, "differs", std::string(600, 'x')};   // a long detail
        if (cls == "C") throw std::runtime_error("the lane broke");
        return {jj::LaneVerdict::NoCompile, "no-compile", "error: x"};
    }
    std::size_t gaps(const std::string& cls) override { return cls == "D" ? 3 : 0; }
    std::atomic<int> judged = 0;
    jj::Backend prepared_with = jj::Backend::Cpp03;
};

}  // namespace

CB_TEST("translate.javajudge.judge_tree", [] {
    FakeLane lane;
    std::vector<std::string> log;
    std::mutex log_lock;
    jj::TreeOptions options;
    options.workers = 3;
    options.log = [&](std::string_view line) {
        std::lock_guard hold(log_lock);
        log.emplace_back(line);
    };
    const auto out = jj::judge_tree(lane, "src", "work", jj::Backend::Cpp17, options);
    CB_CHECK(lane.prepared_with == jj::Backend::Cpp17);
    CB_CHECK_EQ(out.size(), std::size_t(4));
    // in the lane's order, whichever finished first
    CB_CHECK((out.begin()->first == "A") && ((out.end() - 1)->first == "D"));
    CB_CHECK(out.at("A").verdict == jj::Verdict::Equivalent);
    CB_CHECK_EQ(out.at("A").detail, std::string("alike: all 12 cases"));
    CB_CHECK(out.at("B").verdict == jj::Verdict::Divergent);
    CB_CHECK_EQ(out.at("B").detail, "differs: " + std::string(400, 'x'));   // detail to 400 bytes
    // a judge that fails says so, and is not taken for alike
    CB_CHECK(out.at("C").verdict == jj::Verdict::NotRun);
    CB_CHECK_EQ(out.at("C").detail, std::string("judge-error: the lane broke"));
    CB_CHECK(out.at("D").verdict == jj::Verdict::Refused);
    CB_CHECK_EQ(out.at("D").gaps, std::size_t(3));
    // the log: the conversion, then a line per class
    CB_CHECK_EQ(log.size(), std::size_t(5));
    CB_CHECK_EQ(log.front(), std::string("[cpp17] converted: 4 classes"));
    CB_CHECK(std::ranges::find(log, "[cpp17] D: refused (3 gaps)") != log.end());

    // only some classes, one worker, no log
    FakeLane lane2;
    jj::TreeOptions some;
    some.only = std::vector<std::string>{"B", "A", "Z"};
    some.workers = 0;   // (at least one)
    const auto few = jj::judge_tree(lane2, "src", "work", jj::Backend::Cpp03, some);
    CB_CHECK_EQ(few.size(), std::size_t(2));
    CB_CHECK_EQ(lane2.judged.load(), 2);
    CB_CHECK(few.contains("A") && few.contains("B") && !few.contains("Z"));
    CB_CHECK(lane2.prepared_with == jj::Backend::Cpp03);
});

namespace {

stdfs::path on_path(const std::string& program) {
    const auto path = cb::fs::get_env("PATH").value_or("");
    for (const auto dir : cb::text::split(path, ';')) {
        const stdfs::path candidate = cb::fs::from_text(dir) / (program + ".exe");
        std::error_code ec;
        if (!dir.empty() && stdfs::is_regular_file(candidate, ec)) return candidate;
    }
    return {};
}

// The program the made-up translation "is": a batch file that prints what the Java program would, as told by JRT_MAIN.
stdfs::path translation(const stdfs::path& dir, const std::string& harness_lines, const std::string& main_line) {
    const stdfs::path bat = dir / "program.bat";
    cb::fs::write_text(bat, "@echo off\r\nif \"%JRT_MAIN%\"==\"J2XProbe\" goto probe\r\necho " + main_line + "\r\ngoto end\r\n:probe\r\n" + harness_lines + ":end\r\n");
    return bat;
}

// A program of one class: Calc.add and a main that prints a line.
const char* kCalc =
    "public class Calc {\n"
    "    public static int add(int a, int b) { return a + b; }\n"
    "    public static void main(String[] args) { System.out.println(\"hello from Calc\"); }\n"
    "}\n";

}  // namespace

CB_TEST("translate.javajudge.judge_program", [] {
    const stdfs::path javac = on_path("javac"), java = on_path("java");
    if (javac.empty() || java.empty()) return;   // no JDK on this machine
    cb::fs::ScopedTempDir tmp("jj-program");
    const stdfs::path src = tmp.path() / "src";
    cb::fs::write_text(src / "Calc.java", kCalc);

    jj::ProgramHost host;
    host.jdk = cb::translate::dotnet::judge::Jdk{javac, java};
    host.probe = fake_inputs();
    // the front end that is made up: the program's one class, as the IR would hold it
    host.lower = [&](Arena& arena, const stdfs::path&, const stdfs::path&) {
        const ir::TypeP i = ir::i32_type();
        return std::vector<Node*>{unit_of(arena, {class_of(arena, "Calc", {method(arena, {.name = "add", .params = {i, i}}),
                                                                           method(arena, {.name = "main", .params = {ir::array_of(string_type())}, .ret = ir::void_type()})})})};
    };

    // the JVM gives add(1,1)=2 and add(2,2)=4, and main prints a line
    const std::string right = "echo 0.0^|2\r\necho 0.1^|4\r\n";
    const auto judge_with = [&](const std::string& harness_lines, const std::string& main_line) {
        host.build = [&](std::span<Node* const>, const stdfs::path& work, std::string_view) {
            return jj::Built{translation(work, harness_lines, main_line), ""};
        };
        return jj::judge_program(host, src, tmp.path() / "work", "cpp17");
    };

    const jj::ProgramVerdict alike = judge_with(right, "hello from Calc");
    CB_CHECK_EQ(alike.why, std::string(""));
    CB_CHECK(alike.verdict == jj::Verdict::Equivalent);
    CB_CHECK_EQ(alike.methods, std::size_t(1));
    CB_CHECK_EQ(alike.cases, std::size_t(2));
    CB_CHECK(alike.differs.empty());
    CB_CHECK(alike.main && alike.main->at("Calc") == "alike");
    CB_CHECK_EQ(alike.backend, std::string("cpp17"));

    // one result differs
    const jj::ProgramVerdict wrong = judge_with("echo 0.0^|2\r\necho 0.1^|5\r\n", "hello from Calc");
    CB_CHECK(wrong.verdict == jj::Verdict::Divergent);
    CB_CHECK_EQ(wrong.differs.size(), std::size_t(1));
    CB_CHECK_EQ(wrong.differs[0].method, std::string("Calc.add"));
    CB_CHECK((wrong.differs[0].values == std::vector<std::string>{"2", "2"}));
    CB_CHECK_EQ(wrong.differs[0].java, std::string("4"));
    CB_CHECK_EQ(wrong.differs[0].native, std::string("5"));

    // a result missing: it crashed
    const jj::ProgramVerdict crashed = judge_with("echo 0.0^|2\r\n", "hello from Calc");
    CB_CHECK(crashed.verdict == jj::Verdict::Divergent);
    CB_CHECK_EQ(crashed.differs.at(0).native, std::string("no result (crashed?)"));

    // main says something else
    const jj::ProgramVerdict other = judge_with(right, "goodbye");
    CB_CHECK(other.verdict == jj::Verdict::Divergent);
    CB_CHECK(other.main->at("Calc").starts_with("differs: exit 0/0\n--- java\nhello from Calc"));

    // the back end declined, or what it wrote did not compile: refused, with its reason
    host.build = [](std::span<Node* const>, const stdfs::path&, std::string_view) { return jj::Built{std::nullopt, "does not compile: error C2065"}; };
    const jj::ProgramVerdict refused = jj::judge_program(host, src, tmp.path() / "work", "c");
    CB_CHECK(refused.verdict == jj::Verdict::Refused);
    CB_CHECK_EQ(refused.why, std::string("does not compile: error C2065"));
    CB_CHECK(!refused.main);
    CB_CHECK_EQ(refused.backend, std::string("c"));
    // an error from the back end is a refusal too
    host.build = [](std::span<Node* const>, const stdfs::path&, std::string_view) -> jj::Built { throw cb::UnsupportedError("cpp17", 0, "no lambdas yet"); };
    const jj::ProgramVerdict threw = jj::judge_program(host, src, tmp.path() / "work", "cpp17");
    CB_CHECK(threw.verdict == jj::Verdict::Refused);
    CB_CHECK(threw.why.find("no lambdas yet") != std::string::npos);
});

CB_TEST("translate.javajudge.judge_program_needs_what_it_runs", [] {
    // no JDK: nothing to judge against
    jj::ProgramHost host;
    host.probe = fake_inputs();
    const jj::ProgramVerdict none = jj::judge_program(host, "src", "work");
    CB_CHECK(none.verdict == jj::Verdict::NotRun);
    CB_CHECK(none.why.find("no JDK") != std::string::npos);

    // a program javac rejects
    const stdfs::path javac = on_path("javac"), java = on_path("java");
    if (javac.empty() || java.empty()) return;
    cb::fs::ScopedTempDir tmp("jj-bad");
    cb::fs::write_text(tmp.path() / "src" / "Bad.java", "public class Bad { this is not java }\n");
    host.jdk = cb::translate::dotnet::judge::Jdk{javac, java};
    host.lower = [](Arena&, const stdfs::path&, const stdfs::path&) { return std::vector<Node*>{}; };
    host.build = [](std::span<Node* const>, const stdfs::path&, std::string_view) { return jj::Built{std::nullopt, "unused"}; };
    const jj::ProgramVerdict bad = jj::judge_program(host, tmp.path() / "src", tmp.path() / "work");
    CB_CHECK(bad.verdict == jj::Verdict::NotRun);
    CB_CHECK(bad.why.starts_with("javac: "));
});
