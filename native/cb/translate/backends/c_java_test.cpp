// Tests of the C back end for Java (c_java), on real Java programs: the units the Java front end lowers (L2), written as C and
// compared with what the Python back end wrote for the same units (the goldens in testdata/c_java: the translator's own corpus --
// bank, calc, generics, sample, shapes, textstats, and the programs of its suites t03 and t17 -- plus `zoo` (the phase-6 test's
// program: interfaces, abstract classes, records, lambdas, try/catch/finally), `mix` (enums, inner classes, an anonymous class,
// statics, labelled jumps, string switch, 2-D arrays, shifts, long arithmetic, boxed lambdas, method references) and `names` (locals
// named as C keywords, `$` in names, non-ASCII strings). The goldens were read through, and `zoo`, `mix` and `names` were built
// with clang and run beside the JVM: the output is the JVM's (expected.txt).
//
// The programs the back end declines (feat, minecraft, modernlib, streams) are declined here for the same reason.
//
// DIVERGES from c_java.py:
//   * the first line of the C text names the C++ module, not the Python file (the goldens' first line is changed to match; every
//     other line is the Python's);
//   * a string literal's length and its escapes are in UTF-16 units: a character above U+FFFF is a surrogate pair (the Python
//     counted code points and wrote `\x1f600`); the Python is fixed alike;
//   * a lambda reading a local whose C name differs from its Java name (`register`, `self`, `a$b`) finds it (the Python looked the
//     capture up by the Java name and declined "unknown local"); the Python is fixed alike;
//   * an array initialiser's nested `new T[]{...}` without a type of its own takes its element type without the tree being changed;
#include "cb/translate/backends/c_java.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/backends/c_java_internal.hpp"
#include "cb/translate/l2.hpp"

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
    check_text(cj::program(arena, units), "c_java/" + name + ".golden.c");
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
    fail(name + " was written, the Python declined it");
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
    check_text(cj::program(arena, units), "c_java/zoo/golden.c");
});
CB_TEST("translate.c_java.own.mix", [] {
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "mix");
    check_text(cj::program(arena, units), "c_java/mix/golden.c");
});
CB_TEST("translate.c_java.own.names", [] {
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "names");
    check_text(cj::program(arena, units), "c_java/names/golden.c");
});

// The programs the back end does not write say why.
CB_TEST("translate.c_java.declines.feat", [] { check_declined("feat"); });
CB_TEST("translate.c_java.declines.minecraft", [] { check_declined("minecraft"); });
CB_TEST("translate.c_java.declines.modernlib", [] { check_declined("modernlib"); });
CB_TEST("translate.c_java.declines.streams", [] { check_declined("streams"); });

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
