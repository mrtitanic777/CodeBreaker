// Tests of the C back end for Java (c_java), on real Java programs: the units the Java front end lowers (L2), written as C and
// compared with the checked-in goldens (the goldens in testdata/c_java: the translator's own corpus --
// bank, calc, generics, sample, shapes, textstats, and the programs of its suites t03 and t17 -- plus `zoo` (the phase-6 test's
// program: interfaces, abstract classes, records, lambdas, try/catch/finally), `mix` (enums, inner classes, an anonymous class,
// statics, labelled jumps, string switch, 2-D arrays, shifts, long arithmetic, boxed lambdas, method references) and `names` (locals
// named as C keywords, `$` in names, non-ASCII strings). The goldens were read through, and `zoo`, `mix` and `names` were built
// with clang and run beside the JVM: the output is the JVM's (expected.txt).
//
// The programs the back end declines (feat, minecraft) are declined here for the same reason.
//
// Behaviour the goldens pin down:
//   * the first line of the C text names the C++ module;
//   * a string literal's length and its escapes are in UTF-16 units: a character above U+FFFF is a surrogate pair, not one code
//     point written `\x1f600`;
//   * a lambda reading a local whose C name differs from its Java name (`register`, `self`, `a$b`) finds it by the C name;
//   * an array initialiser's nested `new T[]{...}` without a type of its own takes its element type without the tree being changed;
#include "translate/backends/c_java.hpp"

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "translate/backends/c_java_internal.hpp"
#include "translate/l2.hpp"

namespace {

namespace cj = cb::translate::backends::c_java;
namespace ir = cb::translate::ir;
using cb::Json;

std::filesystem::path data(const std::string& relative) { return cb::testing::data_path("translate/testdata/" + relative); }

// the units of the cpp03 corpus program `name` (units/0.json ...), as L2 trees in `arena`
std::vector<ir::Node*> corpus_units(cb::Arena& arena, const std::string& name) {
    std::vector<ir::Node*> units;
    const std::filesystem::path dir = data("cpp03/" + name + "/units");
    for (std::size_t i = 0;; ++i) {
        const std::filesystem::path file = dir / (std::to_string(i) + ".json");
        if (!std::filesystem::exists(file)) break;
        units.push_back(cb::translate::l2::from_json(arena, cb::json::load(file)));
    }
    CB_CHECK(!units.empty());
    return units;
}

// the units of one of this module's own programs: units.l2.json is an array of Unit documents
std::vector<ir::Node*> own_units(cb::Arena& arena, const std::string& name) {
    std::vector<ir::Node*> units;
    const Json docs = cb::json::load(data("c_java/" + name + "/units.l2.json"));
    for (const Json& doc : docs) units.push_back(cb::translate::l2::from_json(arena, doc));
    return units;
}

// a failed check with a message of our own
void fail(const std::string& message) {
    cb::testing::count_check();
    cb::testing::record_failure(__FILE__, __LINE__, message);
}

void check_text(const std::string& got, const std::string& golden_file) {
    const std::string want = cb::fs::read_text(data(golden_file));
    if (got == want) return;
    // point at the first line that differs
    const std::vector<std::string_view> a = cb::text::lines(got), b = cb::text::lines(want);
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    fail(std::format("{}: line {} differs\n  got:  {}\n  want: {}", golden_file, i + 1, i < a.size() ? std::string(a[i]) : "(end)",
                     i < b.size() ? std::string(b[i]) : "(end)"));
}

void check_corpus(const std::string& name) {
    cb::Arena arena;
    std::vector<ir::Node*> units = corpus_units(arena, name);
    check_text(cj::program(arena, units), "c_java/" + name + ".golden.txt");
}

void check_declined(const std::string& name) {
    cb::Arena arena;
    std::vector<ir::Node*> units = corpus_units(arena, name);
    const std::string want(cb::text::trim(cb::fs::read_text(data("c_java/" + name + ".decline.txt"))));
    try {
        (void)cj::program(arena, units);
    } catch (const cj::Decline& d) {
        CB_CHECK_EQ(std::string("Decline: ") + d.what(), want);
        return;
    }
    fail(name + " was written, but the golden declines it");
}

}  // namespace

CB_TEST("translate.c_java.corpus.bank", [] { check_corpus("bank"); });
CB_TEST("translate.c_java.corpus.calc", [] { check_corpus("calc"); });
CB_TEST("translate.c_java.corpus.generics", [] { check_corpus("generics"); });
CB_TEST("translate.c_java.corpus.sample", [] { check_corpus("sample"); });
CB_TEST("translate.c_java.corpus.shapes", [] { check_corpus("shapes"); });
CB_TEST("translate.c_java.corpus.t03", [] { check_corpus("t03"); });
CB_TEST("translate.c_java.corpus.t17", [] { check_corpus("t17"); });
CB_TEST("translate.c_java.corpus.textstats", [] { check_corpus("textstats"); });

CB_TEST("translate.c_java.own.zoo", [] {
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "zoo");
    check_text(cj::program(arena, units), "c_java/zoo/golden.txt");
});
CB_TEST("translate.c_java.own.mix", [] {
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "mix");
    check_text(cj::program(arena, units), "c_java/mix/golden.txt");
});
CB_TEST("translate.c_java.own.names", [] {
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "names");
    check_text(cj::program(arena, units), "c_java/names/golden.txt");
});

// The programs the back end does not write say why.
CB_TEST("translate.c_java.declines.feat", [] { check_declined("feat"); });
CB_TEST("translate.c_java.declines.minecraft", [] { check_declined("minecraft"); });
CB_TEST("translate.c_java.modernlib_is_written", [] {   // (held to the JVM by translate.capability's Library)
    cb::Arena arena;
    std::vector<ir::Node*> units = corpus_units(arena, "modernlib");
    CB_CHECK(!cj::program(arena, units).empty());
});

// The main class names the program's entry when the environment names none.
CB_TEST("translate.c_java.main_class", [] {
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "zoo");
    const std::string text = cj::program(arena, units, std::string("Zoo"));
    CB_CHECK(text.find("return jrt_run_main(Zoo_main, argc, argv);") != std::string::npos);
});

// What the text is made of.
CB_TEST("translate.c_java.names", [] {
    using namespace cb::translate::backends::c_java::detail;
    CB_CHECK_EQ(ident("register"), "register_");
    CB_CHECK_EQ(ident("a$b"), "a_b");
    CB_CHECK_EQ(ident("plain"), "plain");
    CB_CHECK_EQ(after_dot("java.util.Map.Entry"), "Entry");
    CB_CHECK_EQ(after_dot("Plain"), "Plain");
});

CB_TEST("translate.c_java.utf16_literal", [] {
    using namespace cb::translate::backends::c_java::detail;
    const Utf16Literal plain = utf16_literal("a\"b\\c\nd");
    CB_CHECK_EQ(plain.text, "u\"a\\\"b\\\\c\\nd\"");
    CB_CHECK_EQ(plain.length, std::size_t(7));
    // a hex escape followed by a hex digit closes the literal: \x00e9 and `a` would read as one escape
    const Utf16Literal accent = utf16_literal("\xC3\xA9" "a");
    CB_CHECK_EQ(accent.text, "u\"\\x00e9\" u\"a\"");
    CB_CHECK_EQ(accent.length, std::size_t(2));
    // a character above U+FFFF is a surrogate pair, two units
    const Utf16Literal astral = utf16_literal("\xF0\x9F\x98\x80");
    CB_CHECK_EQ(astral.text, "u\"\\xd83d\\xde00\"");
    CB_CHECK_EQ(astral.length, std::size_t(2));
    CB_CHECK_EQ(utf16_literal("").text, "u\"\"");
    CB_CHECK_EQ(utf16_literal("\t").text, "u\"\\x0009\"");
});

CB_TEST("translate.c_java.type_codes", [] {
    using namespace cb::translate::backends::c_java::detail;
    CB_CHECK_EQ(type_code(ir::i32_type()), "I");
    CB_CHECK_EQ(type_code(ir::class_type("java.lang.String")), "Str");
    CB_CHECK_EQ(type_code(ir::class_type("a.B")), "O");
    CB_CHECK_EQ(type_code(ir::array_of(ir::i32_type())), "IA");
    CB_CHECK_EQ(type_code(ir::array_of(ir::class_type("a.B"))), "OA");
    CB_CHECK_EQ(type_code(ir::TypeP()), "");
    CB_CHECK_EQ(array_store("jstring*"), "jstring*");
    CB_CHECK_EQ(array_store("Foo*"), "jobject*");
    CB_CHECK_EQ(array_store("int32_t"), "int32_t");
    CB_CHECK(library_static("java.lang.Integer", "MAX_VALUE") != nullptr);
    CB_CHECK(library_static("java.lang.Integer", "MAX") == nullptr);
});

// Built against the runtime and run, the program prints what the JVM printed (expected.txt). Skipped where clang (or gcc) is not
// installed: it is the build's, not this module's.
namespace {

void check_runs(const std::string& name) {
    const std::filesystem::path clang = cb::fs::from_text(std::string(cj::kClang));
    if (!std::filesystem::is_regular_file(clang)) return;
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, name);
    cb::fs::ScopedTempDir dir;
    const cj::Built built = cj::build(arena, units, dir.path());
    if (!built.exe) fail(built.why);
    if (!built.exe) return;
    cb::process::ProcessSpec spec;
    spec.argv = {cb::fs::to_text(*built.exe)};
    spec.timeout = std::chrono::seconds(60);
    const cb::process::ProcessResult ran = cb::process::run(spec);
    std::string got(cb::as_text(ran.out));
    got = cb::text::replace_all(got, "\r\n", "\n");
    CB_CHECK_EQ(got, cb::text::replace_all(cb::fs::read_text(data("c_java/" + name + "/expected.txt")), "\r\n", "\n"));
}

}  // namespace

CB_TEST("translate.c_java.runs.zoo", [] { check_runs("zoo"); });
CB_TEST("translate.c_java.runs.mix", [] { check_runs("mix"); });
CB_TEST("translate.c_java.runs.names", [] { check_runs("names"); });

// A program the back end declines is an answer: no executable, the reason.
CB_TEST("translate.c_java.build_declined", [] {
    cb::Arena arena;
    std::vector<ir::Node*> units = corpus_units(arena, "feat");
    cb::fs::ScopedTempDir dir;
    const cj::Built built = cj::build(arena, units, dir.path());
    CB_CHECK(!built.exe.has_value());
    CB_CHECK(built.why.starts_with("Decline: "));
});

// The C runtime's collector. Its own check: garbage goes, what a static or only the stack holds stays. Then real programs with a collection at
// every allocation (JRT_GC_STRESS=1): a block freed too early would change what they print, which is held to what the JVM printed.
namespace {
// A golden's files written under dir/src: the text is each file under a line `// ==== path ====`.
std::vector<std::string> write_golden_files(const std::string& golden_file, const std::filesystem::path& dir) {
    std::vector<std::string> sources;
    std::string path, text;
    const auto flush = [&] {
        if (path.empty()) return;
        cb::fs::write_text(dir / "src" / cb::fs::from_text(path), text);
        if (path.ends_with(".c")) sources.push_back(cb::fs::to_text(dir / "src" / cb::fs::from_text(path)));
    };
    const std::string whole = cb::text::replace_all(cb::fs::read_text(data(golden_file)), "\r\n", "\n");
    for (const std::string_view line : cb::text::lines(whole)) {
        if (line.starts_with("// ==== ") && line.ends_with(" ====")) {
            flush();
            path = std::string(line.substr(8, line.size() - 13));
            text.clear();
        } else {
            text += std::string(line) + "\n";
        }
    }
    flush();
    return sources;
}

// The program of a golden built with the runtime: every file in one compiler run.
std::optional<std::string> built_from_golden(const std::string& golden_file, const std::filesystem::path& dir) {
    const std::filesystem::path clang = cb::fs::from_text(std::string(cj::kClang));
    if (!std::filesystem::is_regular_file(clang)) return std::nullopt;
    const std::filesystem::path runtime = cb::testing::data_path("../config/translate/runtime/c");
    const std::string exe = cb::fs::to_text(dir / "program.exe");
    cb::process::ProcessSpec build;
    build.argv = {cb::fs::to_text(clang), "-std=c11", "-fwrapv", "-O1", "-w", "-I" + cb::fs::to_text(runtime), "-I" + cb::fs::to_text(dir / "src"), "-o", exe};
    for (const std::string& source : write_golden_files(golden_file, dir)) build.argv.push_back(source);
    build.argv.push_back(cb::fs::to_text(runtime / "jrt.c"));
    build.timeout = std::chrono::seconds(120);
    CB_CHECK(cb::process::run(build).exit_code == 0);
    return exe;
}

std::optional<std::string> built_with_runtime(const std::filesystem::path& source, const std::filesystem::path& dir) {
    const std::filesystem::path clang = cb::fs::from_text(std::string(cj::kClang));
    if (!std::filesystem::is_regular_file(clang)) return std::nullopt;
    const std::filesystem::path runtime = cb::testing::data_path("../config/translate/runtime/c");
    const std::string exe = cb::fs::to_text(dir / "program.exe");
    cb::process::ProcessSpec build;
    build.argv = {cb::fs::to_text(clang), "-std=c11", "-fwrapv", "-O1", "-w", "-I" + cb::fs::to_text(runtime), "-o", exe, "-x", "c", cb::fs::to_text(source),
                  "-x", "c", cb::fs::to_text(runtime / "jrt.c")};
    build.timeout = std::chrono::seconds(120);
    CB_CHECK(cb::process::run(build).exit_code == 0);
    return exe;
}

void runs_as_the_jvm_under_stress(const std::string& name, const std::string& golden) {
    cb::fs::ScopedTempDir dir;
    const auto exe = built_from_golden(golden, dir.path());
    if (!exe) return;
    const std::string want = cb::text::replace_all(cb::fs::read_text(data("cpp17/" + name + ".jvm.txt")), "\r\n", "\n");
    for (const bool stress : {false, true}) {
        const cb::fs::ScopedEnv env("JRT_GC_STRESS", stress ? std::optional<std::string>("1") : std::nullopt);
        cb::process::ProcessSpec spec;
        spec.argv = {*exe};
        spec.timeout = std::chrono::seconds(120);
        const cb::process::ProcessResult ran = cb::process::run(spec);
        CB_CHECK(ran.exit_code == 0);
        CB_CHECK_EQ(cb::text::replace_all(std::string(cb::as_text(ran.out)), "\r\n", "\n"), want);
    }
}
}  // namespace

CB_TEST("translate.c_java.collector.frees_garbage_and_keeps_what_is_reachable", [] {
    cb::fs::ScopedTempDir dir;
    const auto exe = built_with_runtime(data("c_java/gc_check.c"), dir.path());
    if (!exe) return;
    cb::process::ProcessSpec spec;
    spec.argv = {*exe};
    spec.timeout = std::chrono::seconds(120);
    CB_CHECK(cb::process::run(spec).exit_code == 0);
});
CB_TEST("translate.c_java.collector.bank_runs_under_stress", [] { runs_as_the_jvm_under_stress("bank", "c_java/bank.golden.txt"); });
CB_TEST("translate.c_java.collector.calc_runs_under_stress", [] { runs_as_the_jvm_under_stress("calc", "c_java/calc.golden.txt"); });
CB_TEST("translate.c_java.collector.generics_runs_under_stress", [] { runs_as_the_jvm_under_stress("generics", "c_java/generics.golden.txt"); });
CB_TEST("translate.c_java.collector.shapes_runs_under_stress", [] { runs_as_the_jvm_under_stress("shapes", "c_java/shapes.golden.txt"); });
CB_TEST("translate.c_java.collector.textstats_runs_under_stress", [] { runs_as_the_jvm_under_stress("textstats", "c_java/textstats.golden.txt"); });
CB_TEST("translate.c_java.collector.mix_runs_under_stress", [] { runs_as_the_jvm_under_stress("mix", "c_java/mix/golden.txt"); });
CB_TEST("translate.c_java.collector.names_runs_under_stress", [] { runs_as_the_jvm_under_stress("names", "c_java/names/golden.txt"); });
CB_TEST("translate.c_java.collector.zoo_runs_under_stress", [] { runs_as_the_jvm_under_stress("zoo", "c_java/zoo/golden.txt"); });
CB_TEST("translate.c_java.collector.t17_runs_under_stress", [] { runs_as_the_jvm_under_stress("t17", "c_java/t17.golden.txt"); });
