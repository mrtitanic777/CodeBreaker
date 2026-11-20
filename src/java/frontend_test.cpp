// Tests of the Java front end (cb/java/frontend.hpp). The goldens are checked in:
// testdata/frontend/golden/<unit>.treedump.json is javac's typed tree of the unit (TreeDump's JSON) and <unit>.l2.json the L2 document
// the lowering's L2 document for it. Features.java (testdata/frontend/src) has one of each
// construct the lowering handles (generics, an enum with a body, a record, lambdas, a method reference, an anonymous class, switch
// expressions, try-with-resources with a multi-catch, an instanceof pattern, labelled loops, assert ...); EnumOS1, ICamera and MathHelper are
// the transpiler's test fixtures (testdata/tools_fixtures).
//
// The live test runs TreeDump on Features.java with the JDK on this machine (skipped when there is none) and checks that what it lowers is
// the same document.
#include "java/frontend.hpp"

#include <nlohmann/json.hpp>

#include "core/arena.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "core/testing.hpp"
#include "decompile/jar.hpp"
#include "translate/l2.hpp"

using namespace cb;
namespace fe = cb::java::frontend;
namespace ir = cb::translate::ir;

namespace {

std::filesystem::path jf_golden(std::string_view name) { return testing::data_path("java/testdata/frontend/golden/" + std::string(name)); }

// The first path at which two documents differ ("" when they are equal), for a failure message that says where.
std::string jf_difference(const Json& a, const Json& b, const std::string& path = "") {
    if (a == b) return {};
    if (a.is_object() && b.is_object()) {
        for (const auto& [key, value] : a.items()) {
            if (!b.contains(key)) return path + "/" + key + " (only in the first)";
            if (const std::string d = jf_difference(value, b.at(key), path + "/" + key); !d.empty()) return d;
        }
        for (const auto& [key, value] : b.items())
            if (!a.contains(key)) return path + "/" + key + " (only in the second)";
    }
    if (a.is_array() && b.is_array()) {
        for (std::size_t i = 0; i < std::min(a.size(), b.size()); ++i)
            if (const std::string d = jf_difference(a[i], b[i], path + "/" + std::to_string(i)); !d.empty()) return d;
        return path + " (length " + std::to_string(a.size()) + " against " + std::to_string(b.size()) + ")";
    }
    return path + ": " + a.dump().substr(0, 80) + " against " + b.dump().substr(0, 80);
}

// The goldens write a flag that is false (`"postfix": false` on every Unary, `"default": false` on a Case); this module's writer leaves a
// default out. The documents are the same.
void jf_without_default_postfix(Json& doc) {
    if (doc.is_object()) {
        for (const char* flag : {"postfix", "default", "implicit"})
            if (doc.contains(flag) && doc[flag] == false) doc.erase(flag);
        for (auto& [key, value] : doc.items()) jf_without_default_postfix(value);
    } else if (doc.is_array()) {
        for (Json& item : doc) jf_without_default_postfix(item);
    }
}

Json jf_golden_document(const std::string& unit) {
    Json doc = json::load(jf_golden(unit + ".l2.json"));
    jf_without_default_postfix(doc);
    return doc;
}

void jf_check_unit(const char* unit, const char* path) {
    Arena arena;
    const Json tree = json::load(jf_golden(std::string(unit) + ".treedump.json"));
    const ir::Node* node = fe::lower_unit(arena, tree, path);
    const Json ours = translate::l2::to_json(*node);
    const Json theirs = jf_golden_document(unit);
    CB_CHECK_EQ(jf_difference(ours, theirs), std::string());
}

}  // namespace

CB_TEST("java.frontend.parse_type", [] {
    const auto t = [](const char* text) { return ir::to_text(*fe::parse_type(text)); };
    CB_CHECK_EQ(t("int"), "i32");
    CB_CHECK_EQ(t("boolean"), "bool");
    CB_CHECK_EQ(t("char"), "u16");
    CB_CHECK_EQ(t("void"), "void");
    CB_CHECK_EQ(t("int[][]"), "i32[][]");
    CB_CHECK_EQ(t("java.lang.String"), "java.lang.String");
    CB_CHECK_EQ(t("java.util.Map<K,java.util.List<V>>"), "java.util.Map<K, java.util.List<V>>");
    CB_CHECK_EQ(t("java.util.List<? extends java.lang.Number>[]"), "java.util.List<? extends java.lang.Number>[]");
    CB_CHECK_EQ(t("T"), "T");   // a short capitalised name is a type variable
    CB_CHECK_EQ(t("capture#1 of ?"), "java.lang.Object");
    CB_CHECK_EQ(t("java.lang.Object&java.io.Serializable"), "java.lang.Object");   // an intersection: its first bound
    CB_CHECK(ir::to_text(*fe::parse_type("<nulltype>")) == ir::to_text(*ir::null_type()));
    CB_CHECK(ir::to_text(*fe::parse_type(std::nullopt)) == ir::to_text(*ir::any_type()));
});

CB_TEST("java.frontend.the_features_unit_matches_the_golden", [] { jf_check_unit("Features", "sample/features/Features"); });
CB_TEST("java.frontend.fixtures_match_the_golden", [] {
    jf_check_unit("EnumOS1", "sample/EnumOS1");
    jf_check_unit("ICamera", "sample/ICamera");
    jf_check_unit("MathHelper", "sample/MathHelper");
});

CB_TEST("java.frontend.a_dump_that_is_not_a_tree_is_a_parse_error", [] {
    Arena arena;
    CB_CHECK_THROWS_AS(ParseError, fe::lower_unit(arena, json::parse(R"({"k": "COMPILATION_UNIT", "typeDecls": [{"k": "CLASS", "members": [{"k": "METHOD", "body": {"k": "BLOCK", "statements": [{"k": "IF"}]}}]}]})"), "x"));
});

CB_TEST("java.frontend.javac_runs_and_what_it_lowers_matches_the_golden", [] {
    // the JDK: the java the decompiler finds, and javac beside it
    const std::optional<std::filesystem::path> java = decompile::jar::find_java({});
    if (!java) return;
    std::filesystem::path javac = java->parent_path() / "javac.exe";
    std::error_code ec;
    if (!std::filesystem::is_regular_file(javac, ec)) return;
    fs::ScopedTempDir dir{"cb-frontend"};
    fe::DumpTools tools;
    tools.java = *java;
    tools.javac = javac;
    tools.treedump_source = testing::data_path("../config/translate/frontends/java/TreeDump.java");
    tools.state = dir.path();
    Arena arena;
    const std::vector<ir::Node*> units = fe::lower_program(arena, testing::data_path("java/testdata/frontend/src"), tools);
    CB_CHECK_EQ(units.size(), std::size_t{1});
    const Json theirs = jf_golden_document("Features");
    CB_CHECK_EQ(jf_difference(translate::l2::to_json(*units[0]), theirs), std::string());
    // the compiled TreeDump is kept for the next run
    CB_CHECK(std::filesystem::is_directory(dir.path() / "translate"));
    // a JDK that cannot start is reported with its name
    tools.java = dir.path() / "no-such-java.exe";
    CB_CHECK_THROWS_AS(ToolError, fe::lower_program(arena, testing::data_path("java/testdata/frontend/src"), tools));
});
