#include <nlohmann/json.hpp>

#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/apimap.hpp"

namespace {

using namespace cb::java::apimap;
namespace stdfs = std::filesystem;

// The registry as the lane ships it: the real api_maps/ folder (11 subsystem files, ~650 symbols).
const Registry& real_registry() {
    static const Registry reg = Registry::load(cb::testing::data_path("../../codebreaker/java/api_maps"));
    return reg;
}

// testdata/apimap.golden.json: what the Python registry (without the legacy-equivalents bridge) answers for the same
// folder -- every lookup key with every target language, `by_version` rows under every dialect, reverse lookups,
// coverage, symbols_in, stats. Reviewed: counts and spot rows checked by hand against the JSON files.
const cb::Json& golden() {
    static const cb::Json j = cb::json::load(cb::testing::data_path("java/testdata/apimap.golden.json"));
    return j;
}

std::optional<std::string_view> opt(const cb::Json& j) {
    static thread_local std::string holder;
    if (j.is_null()) return std::nullopt;
    holder = j.get<std::string>();
    return holder;
}

std::optional<std::string> maybe(const cb::Json& j) {
    if (j.is_null()) return std::nullopt;
    return j.get<std::string>();
}

void check_mapping(const std::optional<Mapping>& actual, const cb::Json& expected, const std::string& what) {
    if (expected.is_null()) {
        CB_CHECK(!actual.has_value());
        if (actual) cb::testing::record_failure(__FILE__, __LINE__, "expected no mapping for " + what);
        return;
    }
    CB_CHECK(actual.has_value());
    if (!actual) {
        cb::testing::record_failure(__FILE__, __LINE__, "expected a mapping for " + what);
        return;
    }
    CB_CHECK_EQ(actual->name, expected["name"].get<std::string>());
    CB_CHECK_EQ(actual->src_lang, expected["src"].get<std::string>());
    CB_CHECK_EQ(actual->dst_lang, expected["dst"].get<std::string>());
    CB_CHECK_EQ(actual->target, expected["target"].get<std::string>());
    CB_CHECK_EQ(std::string(name(actual->kind)), expected["kind"].get<std::string>());
    CB_CHECK_EQ(actual->note, expected["note"].get<std::string>());
    CB_CHECK(actual->adapter == maybe(expected["adapter"]));
    CB_CHECK(actual->since == maybe(expected["since"]));
    CB_CHECK_EQ(actual->subsystem, expected["subsystem"].get<std::string>());
    CB_CHECK_EQ(actual->reverse, expected["reverse"].get<bool>());
    CB_CHECK(actual->dialect == maybe(expected["dialect"]));
}

CB_TEST("java.apimap.loads_real_data", [] {
    const Registry& reg = real_registry();
    CB_CHECK(reg.problems().empty());
    CB_CHECK_EQ(reg.subsystems().size(), golden()["subsystems"].size());
    for (const auto& [id, doc] : golden()["subsystems"].items()) {
        CB_CHECK_EQ(reg.subsystems().at(id).language, doc["language"].get<std::string>());
        CB_CHECK_EQ(reg.subsystems().at(id).description, doc["description"].get<std::string>());
    }
    CB_CHECK_EQ(reg.languages().size(), golden()["languages"].size());
    for (const auto& [id, lang] : golden()["languages"].items()) {
        const Language& l = reg.languages().at(id);
        CB_CHECK_EQ(l.kind, lang["kind"].get<std::string>());
        CB_CHECK_EQ(l.desc, lang["desc"].get<std::string>());
        CB_CHECK(l.dialects == lang["dialects"].get<std::vector<std::string>>());
    }
    CB_CHECK_EQ(reg.all_keys().size(), golden()["nkeys"].get<std::size_t>());
    CB_CHECK(!reg.dialect_order("cpp").empty());
    CB_CHECK(reg.dialect_order("nonexistent-language").empty());
});

CB_TEST("java.apimap.resolve_matches_python", [] {
    const Registry& reg = real_registry();
    for (const cb::Json& row : golden()["resolve"]) {
        const std::string key = row[0].get<std::string>();
        auto dialect = opt(row[3]);
        check_mapping(reg.resolve(key, row[1].get<std::string>(), row[2].get<std::string>(), dialect), row[4],
                      key + " -> " + row[2].get<std::string>());
    }
});

CB_TEST("java.apimap.qualified_and_unknown_names", [] {
    const Registry& reg = real_registry();
    for (const cb::Json& row : golden()["unknown"]) {
        const std::string name_text = row[0].get<std::string>();
        check_mapping(reg.resolve(name_text, row[1].get<std::string>(), row[2].get<std::string>()), row[3], name_text);
        CB_CHECK_EQ(reg.known(name_text, row[1].get<std::string>()), row[4].get<bool>());
    }
});

CB_TEST("java.apimap.reverse_matches_python", [] {
    const Registry& reg = real_registry();
    for (const cb::Json& row : golden()["reverse"]) {
        check_mapping(reg.reverse(row[0].get<std::string>(), row[1].get<std::string>(), row[2].get<std::string>()), row[3],
                      row[0].get<std::string>());
    }
    for (const cb::Json& row : golden()["reverse_extra"]) {
        check_mapping(reg.reverse(row[0].get<std::string>(), "cpp", "java"), row[1], row[0].get<std::string>());
    }
});

CB_TEST("java.apimap.coverage_matches_python", [] {
    const Registry& reg = real_registry();
    const std::set<std::string> names = {"Math.sqrt", "GL11.glVertex3f", "ArrayList.get", "String.substring", "Nope.nothing",
                                         "System.out", "Foo.bar", "Display.create", "GL11.GL_TRIANGLES", "Thread.sleep"};
    for (const std::string lang : {"cpp", "direct3d9", "csharp"}) {
        for (const char* dialect : {"", "c++03", "c++11"}) {
            const cb::Json& expected = golden()["cov|" + lang + "|" + (*dialect ? dialect : "None")];
            Coverage c = reg.coverage(names, "java", lang, *dialect ? std::optional<std::string_view>(dialect) : std::nullopt);
            CB_CHECK_EQ(c.mapped.size(), expected["mapped"].size());
            for (std::size_t i = 0; i < c.mapped.size() && i < expected["mapped"].size(); ++i) {
                CB_CHECK_EQ(c.mapped[i].first, expected["mapped"][i][0].get<std::string>());
                check_mapping(c.mapped[i].second, expected["mapped"][i][1], c.mapped[i].first);
            }
            CB_CHECK_EQ(c.unsupported.size(), expected["unsupported"].size());
            CB_CHECK(c.unmapped == expected["unmapped"].get<std::vector<std::string>>());
        }
    }
});

CB_TEST("java.apimap.symbols_in_and_stats_match_python", [] {
    const Registry& reg = real_registry();
    for (const auto& [sub, rows] : golden()["symbols_in"].items()) {
        auto actual = reg.symbols_in(sub);
        CB_CHECK_EQ(actual.size(), rows.size());
        for (std::size_t i = 0; i < actual.size() && i < rows.size(); ++i) {
            CB_CHECK_EQ(actual[i].name, rows[i][0].get<std::string>());
            std::vector<std::string> targets;
            for (const auto& [dst, row] : actual[i].record->maps) targets.push_back(dst);
            std::sort(targets.begin(), targets.end());
            CB_CHECK(targets == rows[i][1].get<std::vector<std::string>>());
        }
    }
    CB_CHECK(reg.symbols_in("no.such.subsystem").empty());
    auto stats = reg.stats();
    CB_CHECK_EQ(stats.size(), golden()["stats"].size());
    for (const cb::Json& row : golden()["stats"]) {
        CB_CHECK_EQ(stats.at({row[0].get<std::string>(), row[1].get<std::string>()}), row[2].get<std::size_t>());
    }
});

CB_TEST("java.apimap.mapping_text", [] {
    const Registry& reg = real_registry();
    const cb::Json& reprs = golden()["reprs"];
    CB_CHECK_EQ(to_string(*reg.resolve("Math.sqrt", "java", "cpp")), reprs[0].get<std::string>());
    CB_CHECK_EQ(to_string(*reg.resolve("Math.sqrt", "java", "cpp", "c++11")), reprs[1].get<std::string>());
    CB_CHECK(!reg.reverse("sqrtf", "cpp", "java").has_value());   // Python: None
    CB_CHECK(reg.resolve("Math.sqrt", "java", "cpp")->supported());
    for (Kind k : {Kind::Direct, Kind::SignatureAdapted, Kind::Macro, Kind::Constant, Kind::Shim, Kind::D3d9Helper, Kind::Unsupported}) {
        CB_CHECK(parse_kind(name(k)) == k);
    }
    CB_CHECK(!parse_kind("Direct"));
});

CB_TEST("java.apimap.lookup_keys_and_leaf", [] {
    using V = std::vector<std::string>;
    CB_CHECK((lookup_keys("org.lwjgl.opengl.GL11.glVertex3f") == V{"org.lwjgl.opengl.GL11.glVertex3f", "GL11.glVertex3f", "glVertex3f"}));
    CB_CHECK((lookup_keys("ArrayList.get") == V{"ArrayList.get", "get"}));
    CB_CHECK((lookup_keys("  glVertex3f ") == V{"glVertex3f"}));
    CB_CHECK((lookup_keys("a.b") == V{"a.b", "b"}));
    CB_CHECK((lookup_keys("") == V{""}));
    CB_CHECK_EQ(target_leaf("std::vector::push_back"), "push_back");
    CB_CHECK_EQ(target_leaf("j2c::gl::vertex3f"), "vertex3f");
    CB_CHECK_EQ(target_leaf("sqrtf(float)"), "sqrtf");
    CB_CHECK_EQ(target_leaf("a.b"), "b");
    CB_CHECK_EQ(target_leaf("x:y"), "x:y");              // a single colon is not a separator
    CB_CHECK_EQ(target_leaf("x::y.z(a.b)"), "b)");       // split first, then cut at '(' (as the Python did)
    CB_CHECK_EQ(target_leaf("plain"), "plain");
});

CB_TEST("java.apimap.extract_symbols_matches_python", [] {
    const cb::Json expected = cb::json::load(cb::testing::data_path("java/testdata/extract_symbols.golden.json"));
    for (const cb::Json& row : expected) {
        std::set<std::string> want;
        for (const cb::Json& s : row[1]) want.insert(s.get<std::string>());
        CB_CHECK(extract_symbols(row[0].get<std::string>()) == want);
    }
});

// ---- registries built by hand: version selection, legacy rows, malformed files --------------------------

constexpr const char* kLanguages = R"J({"languages": {"cpp": {"kind": "language", "desc": "C++", "dialects": ["c++98", "c++03", "c++11", "c++17"]},
  "java": {"kind": "language", "desc": "Java"}}})J";
constexpr const char* kSubsystem = R"J({"subsystem": "java.lang", "language": "java", "description": "lang",
 "symbols": {
   "java.lang.Math.abs": {"signature": "(int)", "doc": "abs", "maps": {"cpp": {"target": "abs", "kind": "direct"}}},
   "Object.null": {"maps": {"cpp": {"target": "NULL", "kind": "constant", "note": "null literal",
        "by_version": {"c++11": {"target": "nullptr"}, "c++17": {"note": "17 note", "kind": "macro"}}}}},
   "Thread.stop": {"maps": {"cpp": {"target": "", "kind": "unsupported", "note": "removed"}}}}})J";

CB_TEST("java.apimap.version_selection", [] {
    cb::fs::ScopedTempDir tmp("cbapimap");
    cb::fs::write_text(tmp.path() / "languages.json", kLanguages);
    cb::fs::write_text(tmp.path() / "java.lang.json", kSubsystem);
    Registry reg = Registry::load(tmp.path());
    CB_CHECK(reg.problems().empty());

    auto at = [&](std::optional<std::string_view> d) { return *reg.resolve("Object.null", "java", "cpp", d); };
    CB_CHECK_EQ(at(std::nullopt).target, "NULL");                   // no dialect: the base row
    CB_CHECK(!at(std::nullopt).dialect.has_value());
    CB_CHECK_EQ(at("c++98").target, "NULL");                        // older than every override: the base
    CB_CHECK_EQ(at("c++03").target, "NULL");
    CB_CHECK_EQ(at("c++11").target, "nullptr");                     // an override at the dialect
    CB_CHECK(at("c++11").dialect == std::optional<std::string>("c++11"));
    CB_CHECK(at("c++11").since == std::optional<std::string>("c++11"));   // `since` defaults to the chosen dialect
    CB_CHECK_EQ(at("c++11").kind, Kind::Constant);                  // fields the override leaves out come from the base
    CB_CHECK_EQ(at("c++11").note, "null literal");
    CB_CHECK_EQ(at("c++17").target, "NULL");                        // the newest override at or below wins -- alone: it
    CB_CHECK_EQ(at("c++17").note, "17 note");                       //   states no target, so the BASE target shows, not c++11's
    CB_CHECK(at("c++17").kind == Kind::Macro);
    CB_CHECK_EQ(at("c++99").target, "NULL");                        // a dialect the language does not list: exact match only
    CB_CHECK(reg.resolve("Object.null", "java", "csharp").has_value() == false);   // no row for that target
    CB_CHECK(reg.resolve("Thread.stop", "java", "cpp")->supported() == false);

    // by_version targets are indexed for reverse lookup too
    CB_CHECK_EQ(reg.reverse("nullptr", "cpp", "java")->target, "Object.null");
    CB_CHECK_EQ(reg.reverse("NULL", "cpp", "java")->target, "Object.null");
    CB_CHECK(reg.reverse("NULL", "cpp", "java")->reverse);
    CB_CHECK_EQ(reg.reverse("abs", "cpp", "java")->target, "java.lang.Math.abs");   // the symbol as authored
    CB_CHECK(reg.resolve("Math.abs", "java", "cpp").has_value());                   // found under its Holder.method key
    CB_CHECK_EQ(reg.resolve("abs", "java", "cpp")->name, "abs");

    // canonical names in symbols_in: the most qualified key
    auto rows = reg.symbols_in("java.lang");
    CB_CHECK_EQ(rows.size(), std::size_t{3});
    CB_CHECK_EQ(rows[0].name, "Object.null");
    CB_CHECK_EQ(rows[1].name, "Thread.stop");
    CB_CHECK_EQ(rows[2].name, "java.lang.Math.abs");
});

CB_TEST("java.apimap.malformed_files_are_reported", [] {
    cb::fs::ScopedTempDir tmp("cbapimap");
    cb::fs::write_text(tmp.path() / "languages.json", "{ not json");
    cb::fs::write_text(tmp.path() / "a.json", R"({"subsystem": "a", "symbols": {"X.y": {"maps": {"cpp": {"target": "t", "kind": "sideways"}}}}})");
    cb::fs::write_text(tmp.path() / "b.json", R"({"subsystem": "b", "symbols": {"Z.w": {"maps": {"cpp": {"target": "ok"}}}}})");
    cb::fs::write_text(tmp.path() / "c.json", "[1, 2]");
    cb::fs::write_text(tmp.path() / "notes.txt", "ignored");
    Registry reg = Registry::load(tmp.path());
    CB_CHECK_EQ(reg.problems().size(), std::size_t{3});         // languages.json, a.json (bad kind), c.json (not an object)
    CB_CHECK(!reg.resolve("X.y", "a", "cpp").has_value());      // a bad file is skipped whole
    CB_CHECK_EQ(reg.resolve("Z.w", "b", "cpp")->target, "ok");  // language defaults to the subsystem's first dotted part
    CB_CHECK_EQ(reg.resolve("Z.w", "b", "cpp")->kind, Kind::Direct);   // kind defaults to direct
    CB_CHECK(Registry::load(tmp.path() / "missing").subsystems().empty());   // a missing folder is an empty registry
});

CB_TEST("java.apimap.legacy_equivalents", [] {
    Registry reg;
    const std::vector<LegacyEquivalent> rows = {
        {"io", "File", "real", " creates files ", "CreateFile", "xbdm.lib"},
        {"net", "Socket", "stub", "", "WSASocket", "ws2_32.lib"},
        {"dead", "Applet", "dead", "no applets", "", ""},
    };
    reg.add_legacy_equivalents(rows);
    auto file = reg.resolve("File", "java", "xbox360");
    CB_CHECK(file.has_value() && file->kind == Kind::Shim);
    CB_CHECK_EQ(file->target, "CreateFile");
    CB_CHECK_EQ(file->note, "creates files [xbdm.lib]");
    CB_CHECK_EQ(file->subsystem, "java.platform:io");
    CB_CHECK_EQ(reg.resolve("Socket", "java", "xbox360")->note, "[ws2_32.lib]");   // trimmed
    CB_CHECK(reg.resolve("Applet", "java", "xbox360")->kind == Kind::Unsupported);
    CB_CHECK_EQ(reg.subsystems().at("java.platform").language, "java");
});

CB_TEST("java.apimap.coverage_report", [] {
    cb::fs::ScopedTempDir tmp("cbapimap");
    cb::fs::write_text(tmp.path() / "languages.json", kLanguages);
    cb::fs::write_text(tmp.path() / "java.lang.json", kSubsystem);
    Registry reg = Registry::load(tmp.path());
    const std::string md = render_coverage_md(reg, {"Math.abs", "Thread.stop", "Foo.bar", "Object.null"}, "java", "cpp", "c++11");
    CB_CHECK(md.starts_with("# API Mapping Coverage\n\n**java -> cpp (c++11)**\n\n"));
    CB_CHECK(md.find("| mapped | 2 |\n| unsupported (no target) | 1 |\n| unmapped (unknown / local) | 1 |") != std::string::npos);
    CB_CHECK(md.find("| `Object.null` | `nullptr` | constant | null literal |") != std::string::npos);
    CB_CHECK(md.find("## Unsupported (registered, but no target equivalent)\n\n| symbol | note |\n|---|---|\n| `Thread.stop` | removed |") != std::string::npos);
    CB_CHECK(md.find("- `Foo.bar`") != std::string::npos);
    CB_CHECK(md.ends_with("\n"));
    const std::string empty = render_coverage_md(reg, {}, "java", "cpp", std::nullopt, "custom title");
    CB_CHECK(empty.find("custom title") != std::string::npos);
    CB_CHECK(empty.find("## ") == std::string::npos);
});

CB_TEST("java.apimap.extract_from_dir", [] {
    cb::fs::ScopedTempDir tmp("cbapimap");
    cb::fs::write_text(tmp.path() / "A.java", "class A { void f() { Math.sqrt(1); } }");
    cb::fs::write_text(tmp.path() / "pkg" / "B.java", "class B { int x = GL11.GL_LINES; }");
    cb::fs::write_text(tmp.path() / "C.txt", "Ignored.call()");
    auto found = extract_symbols_from_dir(tmp.path());
    CB_CHECK((found == std::set<std::string>{"Math.sqrt", "GL11.GL_LINES"}));
    CB_CHECK(extract_symbols_from_dir(tmp.path() / "missing").empty());
});

}  // namespace


