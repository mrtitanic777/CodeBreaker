// Tests of the C# -> C++03 pass (cs2cpp), on real C# programs: the units of the C# front end as L2 (testdata/cs2cpp/PROG/units/N.json) run
// through ToJava, against the recorded answer for the same program (expected.json): every refusal (where, kind, why) in order,
// and the Java skeleton of every class that was kept. The programs are the repo's C# corpus (corpus, gen, top), the front end's own tool,
// the Java runtime in C# (javart), the back end's generated programs (back), the shader checker, and `own`: CppOk (properties, foreach over
// arrays, strings and lists, string switches, Console, Math, string and List calls, exceptions) and CppRefuse (one refused construct per
// class: structs, records, enums, generics, nested classes, operators, ref/out, lambdas, unsigned integers, double text, checked, ??,
// using, lock, goto, `throw;`, nullables, rectangular arrays, library calls, properties in expressions, foreach and switch forms).
// CB_CS2CPP_DATA names a folder of further programs of the same shape (the XNA runtime and stubs).
//
// Behaviour pinned here:
//   * a string comparison operator with other than two operands is refused;
//   * the nodes the pass shares between places (the iterated collection, the switched string, the property's object) are copied, so no
//     node has two parents.
#include "translate/dotnet/cs2cpp.hpp"

#include <cstdlib>
#include <cstdio>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "translate/l2.hpp"

namespace {

namespace cs = cb::translate::dotnet::cs2cpp;
namespace ir = cb::translate::ir;
using cb::Json;

struct Cs2Program {
    std::vector<Json> units;
    Json expected;
};

Cs2Program cs2_load(const std::filesystem::path& dir) {
    Cs2Program p;
    p.expected = cb::json::load(dir / "expected.json");
    for (std::size_t i = 0; i < p.expected.at("units").get<std::size_t>(); ++i) p.units.push_back(cb::json::load(dir / "units" / (std::to_string(i) + ".json")));
    return p;
}

void cs2_check(const std::filesystem::path& dir) {
    const Cs2Program p = cs2_load(dir);
    cb::Arena arena;
    std::vector<ir::Node*> units;
    for (const Json& u : p.units) units.push_back(cb::translate::l2::from_json(arena, u));
    cs::ToJava pass(arena, units);
    const std::vector<cs::Refusal> refused = pass.run();
    const Json& want = p.expected.at("refused");
    std::size_t bad = 0;
    if (refused.size() != want.size()) {
        ++bad;
        std::fprintf(stderr, "cs2cpp %s: %zu refusals, expected %zu\n", dir.filename().string().c_str(), refused.size(), want.size());
    }
    for (std::size_t i = 0; i < std::min(refused.size(), want.size()); ++i) {
        const Json got = Json::array({refused[i].where ? Json(*refused[i].where) : Json(nullptr), refused[i].kind, refused[i].why});
        if (got != want[i] && ++bad <= 5) std::fprintf(stderr, "cs2cpp refusal %zu\n  got      %s\n  expected %s\n", i, got.dump().c_str(), want[i].dump().c_str());
    }
    std::vector<std::pair<std::string, Json>> skeletons;
    for (const ir::Node* un : units) {
        for (const ir::Node* c : ir::get<ir::Unit>(un)->types) {
            std::string text;
            try {
                text = cs::skeleton(*c);
            } catch (const cs::Refuse&) {
                skeletons.emplace_back(ir::get<ir::ClassDecl>(c)->name.value_or("None"), nullptr);
                continue;
            }
            skeletons.emplace_back(ir::get<ir::ClassDecl>(c)->name.value_or("None"), text);
        }
    }
    const Json& want_skeletons = p.expected.at("skeletons");
    if (skeletons.size() != want_skeletons.size()) {
        ++bad;
        std::fprintf(stderr, "cs2cpp %s: %zu skeletons, expected %zu\n", dir.filename().string().c_str(), skeletons.size(), want_skeletons.size());
    }
    for (std::size_t i = 0; i < std::min(skeletons.size(), want_skeletons.size()); ++i) {
        if ((skeletons[i].first != want_skeletons[i][0] || skeletons[i].second != want_skeletons[i][1]) && ++bad <= 5)
            std::fprintf(stderr, "cs2cpp skeleton %s\n  got      %.400s\n  expected %.400s\n", skeletons[i].first.c_str(), skeletons[i].second.dump().c_str(),
                         want_skeletons[i][1].dump().c_str());
    }
    CB_CHECK_EQ(bad, std::size_t(0));
}

}  // namespace

CB_TEST("translate.cs2cpp.own", [] { cs2_check(cb::testing::data_path("translate/testdata/cs2cpp/own")); });
CB_TEST("translate.cs2cpp.back", [] { cs2_check(cb::testing::data_path("translate/testdata/cs2cpp/back")); });
CB_TEST("translate.cs2cpp.corpus", [] { cs2_check(cb::testing::data_path("translate/testdata/cs2cpp/corpus")); });
CB_TEST("translate.cs2cpp.gen", [] { cs2_check(cb::testing::data_path("translate/testdata/cs2cpp/gen")); });
CB_TEST("translate.cs2cpp.top", [] { cs2_check(cb::testing::data_path("translate/testdata/cs2cpp/top")); });
CB_TEST("translate.cs2cpp.tool", [] { cs2_check(cb::testing::data_path("translate/testdata/cs2cpp/tool")); });
CB_TEST("translate.cs2cpp.javart", [] { cs2_check(cb::testing::data_path("translate/testdata/cs2cpp/javart")); });
CB_TEST("translate.cs2cpp.shadercheck", [] { cs2_check(cb::testing::data_path("translate/testdata/cs2cpp/shadercheck")); });

CB_TEST("translate.cs2cpp.big", [] {
    const auto root = cb::fs::get_env("CB_CS2CPP_DATA");
    if (!root) return;
    const std::string data_root = *root;
    for (const auto& e : std::filesystem::directory_iterator(data_root)) {
        if (e.is_directory()) cs2_check(e.path());
    }
});

CB_TEST("translate.cs2cpp.java_text", [] {
    CB_CHECK_EQ(cs::java_type(ir::i32_type()), std::string("int"));
    CB_CHECK_EQ(cs::java_type({}), std::string("void"));
    CB_CHECK_EQ(cs::java_type(ir::array_of(ir::class_type("System.String"))), std::string("String[]"));
    CB_CHECK_EQ(cs::java_type(ir::class_type("java.util.ArrayList", {ir::class_type("java.lang.Integer")})), std::string("ArrayList<Integer>"));
    CB_CHECK_EQ(cs::java_type(ir::typevar_type("E")), std::string("E"));
    CB_CHECK_EQ(cs::java_type(ir::null_type()), std::string("Object"));
    CB_CHECK_EQ(cs::default_value(ir::bool_type()), std::string(" false"));
    CB_CHECK_EQ(cs::default_value(ir::i32_type()), std::string(" 0"));
    CB_CHECK_EQ(cs::default_value(ir::class_type("X")), std::string(" null"));
    CB_CHECK_EQ(cs::default_value(ir::void_type()), std::string(""));
});

// convert(): the skeleton folder, the stand-in converter's answer, the refusals it adds, and the host PrintStream.
CB_TEST("translate.cs2cpp.convert", [] {
    const Cs2Program p = cs2_load(cb::testing::data_path("translate/testdata/cs2cpp/own"));
    cb::Arena arena;
    std::vector<ir::Node*> units;
    for (const Json& u : p.units) units.push_back(cb::translate::l2::from_json(arena, u));
    cb::fs::ScopedTempDir work;
    std::size_t skeleton_files = 0;
    const cs::Converter converter = [&](const std::filesystem::path& skeleton_dir, const std::filesystem::path& out_dir, std::span<ir::Node* const>) {
        for (const auto& e : std::filesystem::directory_iterator(skeleton_dir)) skeleton_files += e.path().extension() == ".java";
        cb::fs::write_text(out_dir / "JavaRT.h",
                           "namespace java {\n\tstruct _PrintStream {\n\t\ttemplate<class T> void println(const T&) {}\n\t\tvoid println() {}\n"
                           "\t\ttemplate<class T> void print(const T&) {}\n\t};\n\tstatic _PrintStream out;\n\tstatic _PrintStream err;\n}\n");
        cs::Converted c;
        c.has_back_end = true;
        c.reasons["expression Lambda"] = 2;
        c.declined["expression"] = 2;
        c.declined["not found"] = 1;
        return c;
    };
    const cs::Conversion done = cs::convert(arena, units, work.path(), converter);
    CB_CHECK(skeleton_files > 0);
    CB_CHECK_EQ(done.cpp_dir.filename().string(), std::string("cpp"));
    CB_CHECK(done.refused.size() > 2);
    const cs::Refusal& last = done.refused.back();
    CB_CHECK_EQ(last.where.value_or(""), std::string("C++03 back end"));
    CB_CHECK_EQ(last.why, std::string("1 bodies: not found"));   // (the reasons were given: only `not found` of the counts is added)
    const std::string rt = cb::fs::read_text(done.cpp_dir / "JavaRT.h");
    CB_CHECK(rt.find("FILE* f;") != std::string::npos);
    CB_CHECK(rt.starts_with("#include <sstream>\n#include <cstdio>\n"));
    CB_CHECK(!cs::host_print(done.cpp_dir));   // already replaced: nothing to do
});
