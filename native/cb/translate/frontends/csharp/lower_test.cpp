// Tests of the C# front end's lowering (Roslyn's bound tree into the IR).
//
// Two kinds of input. The real ones: testdata/csharp_<program>.dump.json is what CsTreeDump (the .NET SDK's own Roslyn) wrote for
// a corpus program of tests/translate/corpus/csharp, and testdata/csharp_<program>.l2.json the L2 document the Python
// lowering made of it (reviewed: units, members, properties, enum constants, a sample of bodies, no Unsupported); the lowering
// here must give the same document. The made ones: testdata/csharp_cases.json holds small trees written as Roslyn writes them,
// each with the document the lowering must give and the refusals it must list, so that every construct that is refused, every
// hoist rule, every parenthesis rule and every shape the corpus does not reach has a case; the Python lowering gives the same
// documents for them (tests/translate/test_csharp_lower_trees.py).
//
// DIVERGES from codebreaker/translate/frontends/csharp/lower.py:
//   * a tree that is not a CsTreeDump tree (a member missing, a value of the wrong kind) is a ParseError naming the member and
//     the line; the Python failed with a KeyError or a TypeError wherever it happened to read it;
//   * an enum constant above 2**63-1 (a ulong enum) is an Unsupported member (the IR's constant is 64-bit signed); the Python
//     kept the big integer;
//   * a symbol of a kind the front end has no use for (an alias, an assembly) is a Global of its name; the Python made a symbol of
//     the lower-cased kind;
//   * a dump text with a half of a surrogate pair in a string (which JSON can only write as \ud800) is a ParseError at the
//     JSON reader; a char constant that is such a half is written as the three bytes generalised UTF-8 gives it;
//   * `ir.Type('class', None)` (a symbol with no owner used as a qualifier) is a class of the empty name;
//   * where the Python had a latent fault (named arguments out of order were lowered twice, so a temporary or a hoisted
//     statement an argument needed was made twice; `x op= y` with an operator Roslyn names otherwise became a plain assignment;
//     a relational pattern the same; a constructor initialiser whose argument cannot be carried made the whole file a failure),
//     both are fixed: lowered once, refused, refused, an Unsupported initialiser.
//   * an array creation with an empty initialiser carries the length 0 (the IR cannot say "present but empty").
#include "cb/translate/frontends/csharp/lower.hpp"

#include <algorithm>
#include <filesystem>
#include <limits>
#include <map>

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace cs = cb::translate::frontends::csharp;
namespace ir = cb::translate::ir;
namespace l2 = cb::translate::l2;
using cb::Arena;
using cb::Json;

// A document without what carries no information (nulls, false flags, empty lists), keys in order: a producer may write them
// or not, in any order; the typed tree has one form for "absent".
Json normal(const Json& j) {
    if (j.is_object()) {
        std::map<std::string, Json> sorted;
        for (const auto& [k, v] : j.items()) {
            if (v.is_null() || (v.is_boolean() && !v.get<bool>()) || (v.is_array() && v.empty())) continue;
            sorted.emplace(k, normal(v));
        }
        Json out = Json::object();
        for (auto& [k, v] : sorted) out[k] = std::move(v);
        return out;
    }
    if (j.is_array()) {
        Json out = Json::array();
        for (const Json& v : j) out.push_back(normal(v));
        return out;
    }
    return j;
}

// Where two normal documents first differ, as a path with both sides (empty: they do not).
std::string first_difference(const Json& a, const Json& b, const std::string& path) {
    if (a == b) return {};
    if (a.is_object() && b.is_object()) {
        for (const auto& [k, v] : a.items()) {
            if (!b.contains(k)) return path + "/" + k + ": only in the first: " + v.dump().substr(0, 200);
            if (std::string d = first_difference(v, b.at(k), path + "/" + k); !d.empty()) return d;
        }
        for (const auto& [k, v] : b.items()) {
            if (!a.contains(k)) return path + "/" + k + ": only in the second: " + v.dump().substr(0, 200);
        }
    }
    if (a.is_array() && b.is_array()) {
        if (a.size() != b.size()) return path + ": " + std::to_string(a.size()) + " items against " + std::to_string(b.size());
        for (std::size_t i = 0; i < a.size(); ++i) {
            if (std::string d = first_difference(a[i], b[i], path + "/" + std::to_string(i)); !d.empty()) return d;
        }
    }
    return path + ": " + a.dump().substr(0, 200) + " against " + b.dump().substr(0, 200);
}

// The document of the lowering of a tree, normalised.
Json lowered_document(const Json& tree, const std::string& path, std::vector<cs::Refusal>* refused = nullptr) {
    Arena arena;
    cs::Lowered out = cs::lower(arena, tree, path);
    if (refused != nullptr) *refused = out.refused;
    return normal(l2::to_json(*out.unit));
}

Json load(const char* relative) { return cb::json::load(cb::testing::data_path(relative)); }

ir::Node* find_node(ir::Node* root, const char* kind, const char* name = nullptr) {
    for (ir::Node* n : ir::walk(root)) {
        if (ir::kind_name(*n) != kind) continue;
        if (name == nullptr) return n;
        if (const auto* m = ir::get<ir::MethodDecl>(n); m != nullptr && m->name == name) return n;
        if (const auto* c = ir::get<ir::ClassDecl>(n); c != nullptr && c->name == name) return n;
        if (const auto* p = ir::get<ir::Property>(n); p != nullptr && p->sym && p->sym->name == name) return n;
    }
    return nullptr;
}

}  // namespace

// ---- the real programs -----------------------------------------------------------------------------------------------------

CB_TEST("translate.csharp.lower.corpus_programs_give_the_reviewed_documents", [] {
    std::vector<std::string> programs;   // every csharp_<program>.dump.json that has its .l2.json beside it
    for (const auto& entry : std::filesystem::directory_iterator(cb::testing::data_path("translate/frontends/csharp/testdata"))) {
        const std::string file = entry.path().filename().string();
        if (file.starts_with("csharp_") && file.ends_with(".dump.json")) programs.push_back(file.substr(7, file.size() - 7 - 10));
    }
    std::sort(programs.begin(), programs.end());
    CB_CHECK(programs.size() >= 4);
    for (const std::string& program : programs) {
        const std::string base = "translate/frontends/csharp/testdata/csharp_" + program;
        const Json dump = load((base + ".dump.json").c_str());
        const Json golden = normal(load((base + ".l2.json").c_str()));
        std::vector<cs::Refusal> refused;
        const Json made = lowered_document(dump, program, &refused);
        CB_CHECK(refused.empty());
        const std::string diff = first_difference(made, golden, "");
        CB_CHECK_EQ(diff, std::string());
        // the document reads back and writes the same
        Arena arena;
        CB_CHECK(normal(l2::to_json(*l2::from_json(arena, made))) == made);
    }
});

CB_TEST("translate.csharp.lower.shapes_properties_enums_and_records", [] {
    const Json dump = load("translate/frontends/csharp/testdata/csharp_Shapes.dump.json");
    Arena arena;
    cs::Lowered out = cs::lower(arena, dump, "Shapes");
    const auto* unit = ir::get<ir::Unit>(out.unit);
    CB_CHECK(unit != nullptr);
    CB_CHECK_EQ(unit->lang.value_or(""), std::string("csharp"));
    CB_CHECK_EQ(unit->path.value_or(""), std::string("Shapes"));
    // properties keep their shape: a Property with accessors, read as a Select
    std::vector<std::string> props;
    for (ir::Node* n : ir::walk(out.unit)) {
        if (const auto* p = ir::get<ir::Property>(n)) props.push_back(p->sym->name);
    }
    for (const char* want : {"Id", "Name", "W", "H", "Made"}) CB_CHECK(std::find(props.begin(), props.end(), want) != props.end());
    bool read_as_select = false;
    for (ir::Node* n : ir::walk(out.unit)) {
        if (const auto* s = ir::get<ir::Select>(n); s != nullptr && s->sym && s->sym->kind == ir::SymKind::Property) read_as_select = true;
    }
    CB_CHECK(read_as_select);
});

CB_TEST("translate.csharp.lower.records_numbers_and_enum_constants", [] {
    Arena arena;
    cs::Lowered out = cs::lower(arena, load("translate/frontends/csharp/testdata/csharp_Records.dump.json"), "Records");
    // a record is a ClassDecl of kind record with its primary constructor
    ir::Node* rec = nullptr;
    for (ir::Node* n : ir::walk(out.unit)) {
        if (const auto* c = ir::get<ir::ClassDecl>(n); c != nullptr && (c->kind == "record" || c->kind == "record struct")) rec = n;
    }
    CB_CHECK(rec != nullptr);
    bool primary = false;
    for (ir::Node* m : ir::get<ir::ClassDecl>(rec)->members) {
        if (const auto* d = ir::get<ir::MethodDecl>(m); d != nullptr && d->kind == "ctor") {
            primary = primary || std::find(d->mods.begin(), d->mods.end(), "primary") != d->mods.end();
        }
    }
    CB_CHECK(primary);
    // checked arithmetic is overflow trap
    Arena arena2;
    cs::Lowered numbers = cs::lower(arena2, load("translate/frontends/csharp/testdata/csharp_Numbers.dump.json"), "Numbers");
    bool trap = false;
    for (ir::Node* n : ir::walk(numbers.unit)) {
        if (const auto* b = ir::get<ir::Binary>(n); b != nullptr && b->overflow == ir::Overflow::Trap) trap = true;
        if (const auto* c = ir::get<ir::Cast>(n); c != nullptr && c->overflow == ir::Overflow::Trap) trap = true;
    }
    CB_CHECK(trap);
});

// ---- the made cases ---------------------------------------------------------------------------------------------------------

CB_TEST("translate.csharp.lower.cases_give_the_reviewed_documents_and_refusals", [] {
    const Json cases = load("translate/frontends/csharp/testdata/csharp_cases.json");
    CB_CHECK(cases.size() >= 24);
    std::size_t refusals = 0;
    for (const Json& c : cases) {
        const std::string name = c.at("name").get<std::string>();
        std::vector<cs::Refusal> refused;
        const Json made = lowered_document(c.at("tree"), "case", &refused);
        CB_CHECK_EQ(name + ": " + first_difference(made, normal(c.at("l2")), ""), name + ": ");
        const Json& want = c.at("refused");
        CB_CHECK_EQ(name + " refusals: " + std::to_string(refused.size()), name + " refusals: " + std::to_string(want.size()));
        for (std::size_t i = 0; i < std::min(refused.size(), want.size()); ++i) {
            CB_CHECK_EQ(refused[i].line, want[i].at(0).get<std::uint32_t>());
            CB_CHECK_EQ(refused[i].kind.value_or("?"), want[i].at(1).get<std::string>());
            CB_CHECK_EQ(refused[i].why, want[i].at(2).get<std::string>());
        }
        refusals += refused.size();
        // every refusal is an Unsupported in the tree, and the other way round
        Arena arena;
        cs::Lowered again = cs::lower(arena, c.at("tree"), "case");
        CB_CHECK_EQ(ir::unsupported(again.unit).size(), again.refused.size());
    }
    CB_CHECK(refusals > 60);
});

CB_TEST("translate.csharp.lower.group_adds_each_parenthesis_once", [] {
    const Json cases = load("translate/frontends/csharp/testdata/csharp_cases.json");
    const Json* parentheses = nullptr;
    for (const Json& c : cases) {
        if (c.at("name") == "parentheses") parentheses = &c;
    }
    CB_CHECK(parentheses != nullptr);
    Arena arena;
    cs::Lowered out = cs::lower(arena, parentheses->at("tree"), "case");
    std::size_t parens = 0;
    for (ir::Node* n : ir::walk(out.unit)) parens += ir::is<ir::Paren>(n) ? 1 : 0;
    CB_CHECK_EQ(parens, std::size_t(8));
    const Json before = l2::to_json(*out.unit);
    cs::group(arena, out.unit);
    CB_CHECK(l2::to_json(*out.unit) == before);
});

// ---- trees that are not what CsTreeDump writes ------------------------------------------------------------------------------

namespace {

// A unit holding class C with the method int M() { return <expression>; }
std::string returning(const std::string& expression) {
    return R"({"k":"unit","types":[{"k":"type","kind":"class","name":"C","l":1,"sym":{"kind":"NamedType","name":"C","qname":"C"},"members":[)"
           R"({"k":"method","l":2,"sym":{"kind":"Method","name":"M","owner":"C","ret":{"k":"prim","n":"int"},"params":[],"pnames":[],"refs":[]},)"
           R"("body":{"k":"Block","operations":[{"k":"Return","l":3,"returnedValue":)" +
           expression + "}]}}]}]}";
}

ir::LitValue returned_value(ir::Node* unit) {
    const auto* ret = ir::get<ir::Return>(find_node(unit, "Return"));
    CB_CHECK(ret != nullptr && ret->expr != nullptr);
    const auto* lit = ret != nullptr ? ir::get<ir::Lit>(ret->expr) : nullptr;
    CB_CHECK(lit != nullptr);
    return lit != nullptr ? lit->value : ir::LitValue{};
}

}  // namespace

CB_TEST("translate.csharp.lower.text_that_is_not_a_dump", [] {
    Arena arena;
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, "{not json", "x"));
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, "", "x"));
    // a document that is JSON but not a node: the member it lacks is named, with the line
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, R"([1,2])", "x"));
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, R"({"k":"unit","types":[{"k":"type"}]})", "x"));
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, returning(R"({"l":3})"), "x"));   // no "k"
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, returning(R"({"k":"Literal","l":3,"$const":{"t":"Int32"}})"), "x"));   // no value
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, returning(R"({"k":"Literal","l":3,"$const":{"t":"Int32","v":"12ab"}})"), "x"));
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, returning(R"({"k":"Literal","l":3,"$const":{"t":"bool","v":"yes"}})"), "x"));
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, returning(R"({"k":"Literal","l":3,"$const":{"t":"char","v":70000}})"), "x"));
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, returning(R"({"k":"Literal","l":3,"$const":{"t":"double","v":"one"}})"), "x"));
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, returning(R"({"k":"Invocation","l":3})"), "x"));   // no target method
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, R"({"k":"unit","types":[{"k":"type","kind":"class","name":"C","mods":"public"}]})", "x"));
    CB_CHECK_THROWS_AS(cb::ParseError, cs::lower_text(arena, R"({"k":"unit","types":[{"k":"type","kind":"class","name":"C","l":-4}]})", "x"));
    CB_CHECK_THROWS_AS(cb::ParseError,
                       cs::lower_text(arena, R"({"k":"unit","types":[{"k":"type","kind":"class","name":"C","members":[{"k":"field","vars":[{"sym":{}}]}]}]})", "x"));
    // the message names where
    try {
        (void)cs::lower_text(arena, returning(R"({"l":3})"), "x");
        CB_CHECK(false);
    } catch (const cb::ParseError& e) {
        CB_CHECK_EQ(e.line(), 3u);
        CB_CHECK(std::string(e.what()).find("\"k\"") != std::string::npos);
    }
    // an empty unit is a unit
    cs::Lowered empty = cs::lower_text(arena, R"({"k":"unit"})");
    const auto* unit = ir::get<ir::Unit>(empty.unit);
    CB_CHECK(unit != nullptr && unit->types.empty() && !unit->path && unit->package.value_or("?") == "");
});

CB_TEST("translate.csharp.lower.number_constants", [] {
    Arena arena;
    auto lit = [&](const char* record) {
        cs::Lowered out = cs::lower_text(arena, returning(std::string(R"({"k":"Literal","l":3,"$type":{"k":"prim","n":"long"},"$const":)") + record + "}"));
        return returned_value(out.unit);
    };
    // numbers arrive as JSON numbers or as text; a ulong above INT64_MAX is the one unsigned value
    CB_CHECK(lit(R"({"t":"Int32","v":"-7"})") == ir::LitValue(std::int64_t(-7)));
    CB_CHECK(lit(R"({"t":"Int32","v":12})") == ir::LitValue(std::int64_t(12)));
    CB_CHECK(lit(R"({"t":"long","v":"-9223372036854775808"})") == ir::LitValue(std::numeric_limits<std::int64_t>::min()));
    CB_CHECK(lit(R"({"t":"ulong","v":"9223372036854775807"})") == ir::LitValue(std::int64_t(9223372036854775807LL)));
    CB_CHECK(lit(R"({"t":"ulong","v":"9223372036854775808"})") == ir::LitValue(std::uint64_t(9223372036854775808ULL)));
    CB_CHECK(lit(R"({"t":"ulong","v":18446744073709551615})") == ir::LitValue(std::uint64_t(18446744073709551615ULL)));
    CB_CHECK(lit(R"({"t":"double","v":"2.5"})") == ir::LitValue(2.5));
    CB_CHECK(lit(R"({"t":"double","v":"-1E-05"})") == ir::LitValue(-1e-5));
    CB_CHECK(lit(R"({"t":"float","v":3})") == ir::LitValue(3.0));
    CB_CHECK(lit(R"({"t":"double","v":"∞"})") == ir::LitValue(std::numeric_limits<double>::infinity()));
    CB_CHECK(lit(R"({"t":"double","v":"-∞"})") == ir::LitValue(-std::numeric_limits<double>::infinity()));
    // a char is a UTF-16 code unit: text for a character, generalised UTF-8 for half of a surrogate pair
    CB_CHECK(lit(R"({"t":"char","v":65})") == ir::LitValue(std::string("A")));
    CB_CHECK(lit(R"({"t":"char","v":233})") == ir::LitValue(std::string("\xC3\xA9")));
    CB_CHECK(lit(R"({"t":"char","v":9786})") == ir::LitValue(std::string("\xE2\x98\xBA")));
    CB_CHECK(lit(R"({"t":"char","v":55357})") == ir::LitValue(std::string("\xED\xA0\xBD")));
    CB_CHECK(lit(R"({"t":"char","v":0})") == ir::LitValue(std::string(1, '\0')));
    CB_CHECK(lit(R"({"t":"string","v":""})") == ir::LitValue(std::string()));
    CB_CHECK(lit(R"({"t":"null"})") == ir::LitValue(std::monostate{}));
});

CB_TEST("translate.csharp.lower.enum_constant_above_int64_is_refused", [] {
    Arena arena;
    const std::string enum_with =
        R"({"k":"unit","types":[{"k":"type","kind":"enum","name":"E","l":1,"sym":{"kind":"NamedType","name":"E","qname":"E"},"underlying":{"k":"prim","n":"ulong"},"members":[)"
        R"({"k":"enummember","l":2,"sym":{"kind":"Field","name":"Big","owner":"E","const":true,"static":true},"value":{"t":"UInt64","v":"18446744073709551615"}},)"
        R"({"k":"enummember","l":3,"sym":{"kind":"Field","name":"Ok","owner":"E","const":true,"static":true},"value":{"t":"UInt64","v":"9223372036854775807"}}]}]})";
    cs::Lowered out = cs::lower_text(arena, enum_with);
    CB_CHECK_EQ(out.refused.size(), std::size_t(1));
    CB_CHECK_EQ(out.refused[0].line, 2u);
    CB_CHECK_EQ(out.refused[0].kind.value_or("?"), std::string("enummember"));
    const auto* ok = ir::get<ir::EnumConst>(find_node(out.unit, "EnumConst"));
    CB_CHECK(ok != nullptr && ok->value == std::int64_t(9223372036854775807LL));
    CB_CHECK_EQ(ir::unsupported(out.unit).size(), std::size_t(1));
    // the enum's integer type is its base
    const auto* cls = ir::get<ir::ClassDecl>(find_node(out.unit, "ClassDecl"));
    CB_CHECK(cls != nullptr && cls->extends && ir::to_text(*cls->extends) == "u64");
});

CB_TEST("translate.csharp.lower.nodes_carry_their_positions_and_the_unit_its_language", [] {
    Arena arena;
    cs::Lowered out = cs::lower_text(arena, returning(R"({"k":"Literal","l":9,"c":4,"$type":{"k":"prim","n":"int"},"$const":{"t":"Int32","v":"1"}})"), "m.cs");
    const auto* unit = ir::get<ir::Unit>(out.unit);
    CB_CHECK(unit != nullptr);
    CB_CHECK_EQ(unit->lang.value_or(""), std::string("csharp"));
    CB_CHECK_EQ(unit->path.value_or(""), std::string("m.cs"));
    ir::Node* lit = nullptr;
    for (ir::Node* n : ir::walk(out.unit)) {
        if (ir::is<ir::Lit>(n)) lit = n;
    }
    CB_CHECK(lit != nullptr && lit->line == 9 && lit->col == 4);
    ir::Node* method = find_node(out.unit, "MethodDecl", "M");
    CB_CHECK(method != nullptr && method->line == 2);
});

CB_TEST("translate.csharp.lower.a_symbol_kind_the_front_end_has_no_use_for_is_a_global", [] {
    Arena arena;
    const std::string alias_field =
        R"({"k":"unit","types":[{"k":"type","kind":"class","name":"C","sym":{"kind":"NamedType","name":"C","qname":"C"},"members":[)"
        R"({"k":"field","vars":[{"l":4,"sym":{"kind":"Alias","name":"Al","owner":"N"}}]}]}]})";
    cs::Lowered out = cs::lower_text(arena, alias_field);
    const auto* field = ir::get<ir::FieldDecl>(find_node(out.unit, "FieldDecl"));
    CB_CHECK(field != nullptr && field->sym != nullptr);
    CB_CHECK(field != nullptr && field->sym != nullptr && field->sym->kind == ir::SymKind::Global);
    CB_CHECK(field != nullptr && field->sym != nullptr && field->sym->name == "Al" && field->sym->owner == std::optional<std::string>("N"));
});
