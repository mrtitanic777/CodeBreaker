// Tests of the C-library mapping for the Java back end (java_libc), on the real api_maps/libc.json and the programs the clang
// front end lowered (testdata/java/CallsLib, LibCalls). LibCalls.faithful.java is what codebreaker/translate/backends/java.py
// (with java_libc.py) wrote for LibCalls.c; it was read through: `puts("hello")` keeps its string literal, `printf` its variadic
// arguments and a literal for the `char *` format, `sqrt` is the JDK's Math.sqrt, a comparator named in the call is a method
// reference and one held in a variable goes through the function table (`Fn.at`), `strlen("hello")` a literal too, and the one
// function the runtime does not have (`mystery`) gaps the function that calls it, not the program.
//
// DIVERGES from java_libc.py: the registry is the caller's (a Registry handed in), not a module-level cache; a call with no
// symbol is written as one with no prototype rather than failing on the missing symbol.
#include "cb/translate/backends/java_libc.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace java = cb::translate::backends::java;
namespace jlc = cb::translate::backends::java_libc;
namespace apimap = cb::java::apimap;
namespace ir = cb::translate::ir;
namespace l2 = cb::translate::l2;
using ir::Node;

const apimap::Registry& jlc_registry() {
    static const apimap::Registry reg = apimap::Registry::load(cb::testing::data_path("../../codebreaker/java/api_maps"));
    return reg;
}

struct JlcProgram {
    cb::Arena arena;
    Node* unit = nullptr;
};

std::unique_ptr<JlcProgram> jlc_load(const std::string& name) {
    auto p = std::make_unique<JlcProgram>();
    p->unit = l2::from_json(p->arena, cb::json::load(cb::testing::data_path("translate/testdata/java/" + name + ".l2.json")));
    return p;
}

java::Program jlc_write(JlcProgram& p, const std::string& name) {
    static const jlc::LibC library(jlc_registry());
    Node* units[] = {p.unit};
    return java::program(units, name, {.tier = java::Tier::Faithful, .library = &library});
}

}  // namespace

CB_TEST("translate.java_libc.calls_lib", [] {
    auto p = jlc_load("CallsLib");
    const java::Program prog = jlc_write(*p, "CallsLib");
    CB_CHECK(prog.source.find("        puts(\"hello\");\n        return abs(-3);\n") != std::string::npos);
    CB_CHECK(prog.source.find("import static crt.LibC.*;") != std::string::npos);
});

CB_TEST("translate.java_libc.golden", [] {
    auto p = jlc_load("LibCalls");
    const java::Program prog = jlc_write(*p, "LibCalls");
    CB_CHECK_EQ(prog.source, cb::fs::read_text(cb::testing::data_path("translate/testdata/java/LibCalls.faithful.java")));
    std::string notes;
    for (const std::string& n : prog.notes) notes += n + "\n";
    CB_CHECK_EQ(notes, cb::fs::read_text(cb::testing::data_path("translate/testdata/java/LibCalls.notes.txt")));
});

CB_TEST("translate.java_libc.how_arguments_travel", [] {
    auto p = jlc_load("LibCalls");
    const std::string src = jlc_write(*p, "LibCalls").source;
    // a variadic prototype: the literal travels as a String, the other arguments as C passed them
    CB_CHECK(src.find("printf(\"%d %s %f\\n\", Mem.getInt(v), \"txt\", Math.sqrt(2.0));") != std::string::npos);
    // a comparator named in the call: a method reference; one in a variable: the function table
    CB_CHECK(src.find("qsort(v, 4L, 4L, LibCalls::cmp_int);") != std::string::npos);
    CB_CHECK(src.find("qsort(v, 4L, 4L, (a, b) -> ((IntIntToInt) Fn.at(f)).call(a, b));") != std::string::npos);
    // the interface the callback needs is declared
    CB_CHECK(src.find("interface IntIntToInt { int call(int a0, int a1); }") != std::string::npos);
    // a `direct` row names the JDK's own method; a shim row LibC's (statically imported, so no prefix)
    CB_CHECK(src.find("Math.sqrt(2.0)") != std::string::npos);
    CB_CHECK(src.find("return (int) strlen(\"hello\");") != std::string::npos);
});

CB_TEST("translate.java_libc.a_missing_row_gaps_the_function", [] {
    auto p = jlc_load("LibCalls");
    const java::Program prog = jlc_write(*p, "LibCalls");
    CB_CHECK(prog.source.find("throw new UnsupportedOperationException(\"CodeBreaker gap: use_mystery: library function mystery: not in "
                              "the C runtime for Java (api_maps/libc.json)\");") != std::string::npos);
    CB_CHECK_EQ(prog.notes.front(), "gap: use_mystery (library function mystery: not in the C runtime for Java (api_maps/libc.json))");
});

CB_TEST("translate.java_libc.unsupported_and_missing_rows_refuse", [] {
    const apimap::Registry& reg = jlc_registry();
    // the rows the runtime marks `unsupported` say why
    const auto fopen_row = reg.resolve("fopen", "c", "java");
    CB_CHECK(fopen_row.has_value());
    CB_CHECK(fopen_row->kind == apimap::Kind::Unsupported);
    CB_CHECK_EQ(fopen_row->note, "files are not in the runtime yet");
    CB_CHECK(!reg.resolve("no_such_c_function", "c", "java").has_value());
});

CB_TEST("translate.java_libc.unsupported_function_refuses_main", [] {
    // a registry whose `puts` row is unsupported: main calls it, and main is never a gap, so the program is refused whole
    cb::fs::ScopedTempDir tmp("cbjavalibc");
    cb::fs::write_text(tmp.path() / "languages.json", R"J({"languages": {"c": {"kind": "language", "desc": "C"}, "java": {"kind": "language", "desc": "Java"}}})J");
    cb::fs::write_text(tmp.path() / "libc.json", R"J({"subsystem": "libc", "language": "c", "description": "c",
 "symbols": {"puts": {"maps": {"java": {"target": "", "kind": "unsupported", "note": "no output today"}}},
             "abs": {"maps": {"java": {"target": "crt.LibC.abs", "kind": "shim"}}}}})J");
    const apimap::Registry reg = apimap::Registry::load(tmp.path());
    CB_CHECK(reg.problems().empty());
    const jlc::LibC library(reg);
    auto p = jlc_load("CallsLib");
    Node* units[] = {p->unit};
    try {
        (void)java::program(units, "CallsLib", {.tier = java::Tier::Faithful, .library = &library});
        CB_CHECK(false);   // refused
    } catch (const java::Refused& r) {
        CB_CHECK_EQ(std::string(r.what()), "library function puts: no output today");
    }
});
