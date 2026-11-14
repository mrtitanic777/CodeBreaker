// Tests of the C# back end's Java library (csharp_java), on real Java programs: the javac front end's units as L2
// (testdata/csharp_java/NAME.l2.json) written as C# by the C# back end with this library, against the checked-in
// goldens (NAME.cs.txt, NAME.ledger.txt). The text of each was read through: the three
// corpus programs of tests/translate/corpus/java (calc, fixtures, modern: every one of them also judged equivalent to its Java
// original by the kit), and programs written for this library -- `features` (dispatch, construction order, text), `calls`
// (the API-mapping registry's templates), and one program for each thing the library declines.
//
// Behaviour pinned here:
//   * a UnaryOperator or BinaryOperator written raw (no type argument) is `Func<object, object>`;
//   * the library keeps what it learns of a program (fields it moved, the class a local was made of) in itself, one library to
//     a Program;
//   * `check_lambda` is given the lambda's type.
#include "cb/translate/backends/csharp_java.hpp"

#include <chrono>
#include <cstdio>
#include <limits>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace cs = cb::translate::backends::csharp;
namespace cj = cb::translate::backends::csharp_java;
namespace apimap = cb::java::apimap;
namespace ir = cb::translate::ir;
using ir::Node;

const apimap::Registry& cjt_registry() {
    static const apimap::Registry reg = apimap::Registry::load(cb::testing::data_path("../../data/java/api_maps"));
    return reg;
}

cs::Options cjt_options() {
    cs::Options options;
    options.libraries["java"] = cj::factory(cjt_registry());
    options.runtime_dir = cb::testing::data_path("../../data/translate/dotnet/runtime");
    return options;
}

struct CjtUnits {
    cb::Arena arena;
    std::vector<Node*> units;
};

std::unique_ptr<CjtUnits> cjt_load(const std::string& name) {
    auto u = std::make_unique<CjtUnits>();
    const cb::Json docs = cb::json::load(cb::testing::data_path("translate/testdata/csharp_java/" + name + ".l2.json"));
    for (const cb::Json& d : docs) u->units.push_back(cb::translate::l2::from_json(u->arena, d));
    return u;
}

std::string cjt_golden(const std::string& name, const std::string& ext) {
    return cb::fs::read_text(cb::testing::data_path("translate/testdata/csharp_java/" + name + "." + ext));
}

// The files as the golden lists them: each under a marker line, the runtime's left out.
std::string cjt_files_text(cs::Program& p) {
    std::string out;
    for (const cs::File& f : p.files()) {
        if (f.name == "JavaRt.cs") continue;
        out += "// ==== " + f.name + " ====\n" + f.text;
        if (!f.text.ends_with('\n')) out += '\n';
    }
    return out;
}

std::string cjt_ledger_text(cs::Program& p) {
    std::string out;
    for (const cs::Gap& g : p.ledger()) out += g.where + "\t" + g.kind + "\t" + cb::text::replace_all(g.why, "\n", " ") + "\n";
    return out;
}

bool cjt_has_runtime(cs::Program& p) {
    for (const cs::File& f : p.files()) {
        if (f.name == "JavaRt.cs") return true;
    }
    return false;
}

// One program against its golden; `runtime`: whether the output needs JavaRt.
void cjt_check(const std::string& name, bool runtime) {
    auto u = cjt_load(name);
    cs::Program p(u->units, cjt_options());
    CB_CHECK_EQ(cjt_files_text(p), cjt_golden(name, "cs.txt"));
    CB_CHECK_EQ(cjt_ledger_text(p), cjt_golden(name, "ledger.txt"));
    CB_CHECK_EQ(cjt_has_runtime(p), runtime);
}

}  // namespace

// ---- the programs ---------------------------------------------------------------------------------------------------------------

CB_TEST("translate.csharp_java.corpus.calc", [] { cjt_check("calc", true); });
CB_TEST("translate.csharp_java.corpus.fixtures", [] { cjt_check("fixtures", true); });
CB_TEST("translate.csharp_java.corpus.modern", [] { cjt_check("modern", true); });
CB_TEST("translate.csharp_java.features", [] { cjt_check("features", true); });
CB_TEST("translate.csharp_java.calls", [] { cjt_check("calls", true); });

CB_TEST("translate.csharp_java.semantics_in_the_text", [] {
    // the things a reader looks for, in the one text that holds them all (features)
    auto u = cjt_load("features");
    cs::Program p(u->units, cjt_options());
    const std::string text = cjt_files_text(p);
    const auto has = [&](std::string_view s) { return text.find(s) != std::string::npos; };
    CB_CHECK(has("public override string ToString()"));                  // toString is ToString
    CB_CHECK(has("public override bool Equals(object o)"));
    CB_CHECK(has("public override int GetHashCode()"));
    CB_CHECK(has("public virtual int CompareTo(Shape o)"));              // Comparable
    CB_CHECK(has("class Resource : IDisposable"));                       // AutoCloseable
    CB_CHECK(has("public virtual void Dispose()"));                      // close
    CB_CHECK(has("public abstract string greet(string name);"));         // the abstract class declares what its interface leaves
    CB_CHECK(has("public override string greet(string name)"));
    CB_CHECK(has("internal Derived() : base()\n"));                      // construction order: after the base constructor
    CB_CHECK(has("                w = 2;\n                w += 1;\n"));
    CB_CHECK(has("static Features()\n"));                                // static initialisers in one static constructor
    CB_CHECK(has("counter = compute();\n            counter += 1;"));
    CB_CHECK(has("internal const int LIMIT = 10;"));
    CB_CHECK(has("(object)a == (object)b"));                             // Java compares the references
    CB_CHECK(has("JavaRt.Str(d)"));                                      // a double as text
    CB_CHECK(has("using (var r = new Resource())"));
    CB_CHECK(has("public static void Main(string[] args)"));
});

CB_TEST("translate.csharp_java.what_it_declines", [] {
    // each program that has a thing the library cannot carry says so, with the reason, and the rest is written
    struct Case {
        const char* name;
        const char* kind;
        const char* in_text;     // what is still written
        const char* not_in_text; // what is not
    };
    const Case cases[] = {
        // a class it cannot carry declines the whole unit; the others only the method
        {"inner_outer", "inner-class", "was not translated: an inner class that uses its outer instance (Counter)", "plain(int x)"},
        {"enum_object", "enum", "was not translated: a Java enum with fields", "Color"},
        {"hash_order", "hash-order", "foreach (int v in m.Values)", "HashMap<"},
        {"boxed_null", "boxed-null", "int x = 5;", "Integer x"},
        {"exception_cause", "exception", "throw new ArgumentException(\"bad\");", "RuntimeException"},
    };
    for (const Case& c : cases) {
        auto u = cjt_load(c.name);
        cs::Program p(u->units, cjt_options());
        const std::string text = cjt_files_text(p);
        CB_CHECK_EQ(p.ledger().size(), std::size_t{1});
        if (!p.ledger().empty()) CB_CHECK_EQ(p.ledger()[0].kind, std::string(c.kind));
        CB_CHECK(text.find(c.in_text) != std::string::npos);
        CB_CHECK(std::string_view(c.not_in_text).empty() || text.find(c.not_in_text) == std::string::npos);
    }
});

CB_TEST("translate.csharp_java.a_unit_of_another_language_is_left_to_the_writer", [] {
    // only Java units ask this library: a C# unit (with the library registered) is written as C#'s own
    auto u = cjt_load("calc");
    for (Node* unit : u->units) ir::get<ir::Unit>(unit)->lang = "csharp";
    cs::Program p(u->units, cjt_options());
    CB_CHECK(!p.files().empty());
    CB_CHECK(!cjt_has_runtime(p));
});

CB_TEST("translate.csharp_java.a_java_unit_without_the_library_is_declined", [] {
    auto u = cjt_load("calc");
    cs::Options none;
    none.runtime_dir = cb::testing::data_path("../../data/translate/dotnet/runtime");
    cs::Program p(u->units, none);
    CB_CHECK(!p.ledger().empty());
    CB_CHECK_EQ(p.ledger()[0].kind, std::string("library"));
});

// The speed of the writing, Java library included: the best of 20, printed.
CB_TEST("translate.csharp_java.timing", [] {
    for (const char* name : {"modern", "calc", "features"}) {
        auto u = cjt_load(name);
        std::int64_t best_us = std::numeric_limits<std::int64_t>::max();
        for (int rep = 0; rep < 20; ++rep) {
            const auto t0 = std::chrono::steady_clock::now();
            cs::Program p(u->units, cjt_options());
            (void)p.files();
            best_us = std::min(best_us, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count());
        }
        std::fprintf(stderr, "csharp_java %s: %lld us\n", name, static_cast<long long>(best_us));
    }
});
