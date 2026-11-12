#include "cb/java/corpus.hpp"

#include <nlohmann/json.hpp>

#include <map>
#include <set>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/parse.hpp"

namespace {

namespace cor = cb::java::corpus;
namespace ir = cb::java::ir;
using cb::Json;
using JsonMap = std::map<std::string, Json>;

std::filesystem::path cort_dir() { return cb::testing::data_path("java/testdata/corpus"); }

std::vector<ir::JClass> cort_parse_all() {
    std::vector<ir::JClass> classes;
    for (const auto& path : cor::discover(cort_dir())) classes.push_back(cb::java::parse::parse_file(path));
    return classes;
}

Json cort_names(const std::set<std::string, std::less<>>& names) { return Json(std::vector<std::string>(names.begin(), names.end())); }

// testdata/corpus.golden.json: what the original Python passes (codebreaker.java.transpile) made of the corpus in testdata/corpus, as
// compact JSON with sorted keys; this test builds the same document from the new passes.
CB_TEST("java.corpus.passes_match_python", [] {
    const auto runtime = cb::java::runtime::Runtime(cb::java::JavaData(cb::testing::data_path("../../codebreaker/java")));
    std::vector<ir::JClass> classes = cort_parse_all();
    std::size_t methods_before = 0, methods_after = 0;
    for (const ir::JClass& c : classes) methods_before += c.methods.size();
    cb::java::ledger::Ledger led;
    const cor::Dropped dropped = cor::drop_synthetic_artifacts(classes, &led);
    for (const ir::JClass& c : classes) methods_after += c.methods.size();

    JsonMap out;
    std::vector<std::string> discovered, kept, events;
    for (const auto& p : cor::discover(cort_dir())) discovered.push_back(cb::fs::to_text(p.filename()));
    for (const ir::JClass& c : classes) kept.push_back(c.name);
    for (const auto& e : led.events()) events.push_back(e.message);
    out["discover"] = discovered;
    out["dropped"] = JsonMap{{"markers", cort_names(dropped.markers)}, {"kept", kept}, {"bridge_ctors", methods_before - methods_after}, {"events", events}};
    CB_CHECK_EQ(dropped.bridge_constructors, std::size_t{1});

    cor::resolve_inheritance(classes);
    cor::compute_mi_forwards(classes);
    JsonMap inherited, mi;
    for (const ir::JClass& c : classes) {
        std::vector<std::string> fields;
        for (const ir::Field* f : c.inherited_fields) fields.push_back(f->name);
        inherited[c.name] = fields;
        std::vector<std::vector<std::string>> forwards;
        for (const ir::MiForward& m : c.mi_forwards) forwards.push_back({m.interface_method->name, m.base_class});
        mi[c.name] = forwards;
    }
    out["inherited"] = inherited;
    out["mi"] = mi;

    cb::java::types::TypeResolver resolver = cor::build_resolver(classes, {.value_types = cb::java::typemap::NameSet{"Foo"}, .runtime = &runtime, .markers = dropped.markers});
    JsonMap env;
    std::vector<std::string> selected;
    for (const ir::JClass& c : classes) selected.push_back(c.name);
    for (const char* n : {"URL", "File", "Runtime", "Element", "StringBuilder", "Applet", "Frame"}) selected.push_back(n);
    for (const std::string& n : selected) {
        const auto it = resolver.data().members.find(n);
        if (it == resolver.data().members.end()) continue;
        JsonMap fields, methods;
        for (const auto& [k, v] : it->second.fields) fields[k] = v.to_string();
        for (const auto& [k, v] : it->second.methods) methods[k] = v.to_string();
        env[n] = JsonMap{{"fields", fields}, {"methods", methods}, {"parent", it->second.parent ? Json(*it->second.parent) : Json()}};
    }
    out["member_env"] = env;
    JsonMap renames;
    for (const auto& [k, v] : resolver.data().method_renames) renames[k] = cort_names(v);
    out["renames"] = renames;
    out["thread_like"] = cort_names(resolver.data().thread_like);

    const std::size_t count = cor::compute_collection_out_params(classes);
    std::vector<std::string> marked;
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            for (std::size_t i = 0; i < m.params.size(); ++i) {
                if (m.params[i].by_ref) marked.push_back(std::format("{}.{}/{}#{}", c.name, m.name, m.params.size(), i));
            }
        }
    }
    std::ranges::sort(marked);
    out["out_params"] = JsonMap{{"count", count}, {"marked", marked}};

    const cor::Nullability nullability = cor::derive_nullability(classes, resolver);
    out["nullability"] = JsonMap{{"getters", cort_names(nullability.getters)}, {"types", JsonMap(nullability.return_types.begin(), nullability.return_types.end())}};
    out["this_escapes"] = cort_names(cor::compute_this_escapes(classes));

    const auto& data = resolver.data();
    JsonMap scalars, retained;
    for (const auto& [k, v] : data.own_catalog.scalar_methods) scalars[k] = v;
    for (const auto& [k, v] : data.own_catalog.retained_params) retained[k] = std::vector<std::size_t>(v.begin(), v.end());
    out["resolver"] = JsonMap{{"known", cort_names(data.known)},       {"interfaces", cort_names(data.interfaces)},     {"enums", cort_names(data.enums)},
                              {"typed_enums", cort_names(data.typed_enums)}, {"values", cort_names(data.values)},       {"thread_like", cort_names(data.thread_like)},
                              {"markers", cort_names(data.markers)},   {"return_methods", cort_names(data.own_catalog.return_methods)},
                              {"scalar_methods", scalars},             {"retained_params", retained},                    {"corpus_methods", cort_names(data.own_catalog.corpus_methods)}};

    struct Call { const char* body; const char* name; std::size_t arity; };
    const Call calls[] = {{"a(1, 2); foo(a(x, y), z);", "a", 2}, {"foo(a(x, y), z);", "foo", 2}, {"foo(a(x, y), z);", "a", 2}, {"x.foo( )", "foo", 0},
                          {"foo(bar(1)", "foo", 1}, {"xfoo(1); foo (1, 2 , 3);", "foo", 3}, {"foo(1,", "foo", 2}, {"foo(()), foo((1), (2, 3))", "foo", 1},
                          {"foo( a , b ) foo(c)", "foo", 2}, {"", "foo", 0}};
    std::vector<Json> call_rows;
    for (const Call& c : calls) {
        std::vector<Json> args;
        for (const cor::CallArg& a : cor::call_args(c.body, c.name, c.arity)) args.push_back(Json::array({a.index, a.text}));
        call_rows.push_back(Json::array({c.body, c.name, c.arity, Json(args)}));
    }
    out["call_args"] = call_rows;

    const std::string expected = cb::fs::read_text(cb::testing::data_path("java/testdata/corpus.golden.json"));
    CB_CHECK_EQ(Json(out).dump() + "\n", expected);
});

CB_TEST("java.corpus.discover_keeps_the_first_of_a_name", [] {
    cb::fs::ScopedTempDir tmp("cbcorpus");
    const auto write = [&tmp](const char* relative, const char* text) { cb::fs::write_text(tmp.path() / relative, text); };
    write("b/Dup.java", "class Dup { }");
    write("a/Dup.java", "class Dup { }");     // a folder met before b
    write("Top.java", "class Top { }");
    write("z/deep/Zed.java", "class Zed { }");
    write("notes.txt", "not java");
    write("Dup.java", "class Dup { }");       // the files of a folder come before its sub-folders
    const auto found = cor::discover(tmp.path());
    std::vector<std::string> rel;
    for (const auto& p : found) rel.push_back(std::filesystem::relative(p, tmp.path()).generic_string());
    CB_CHECK(rel == (std::vector<std::string>{"Dup.java", "Top.java", "z/deep/Zed.java"}));
    CB_CHECK(cor::discover(tmp.path() / "missing").empty());
});

CB_TEST("java.corpus.markers_that_are_bases_stay", [] {
    std::vector<ir::JClass> classes = {
        cb::java::parse::parse_source("class Empty7 { }"),
        cb::java::parse::parse_source("class User { User(Empty7 e) { this(); } User() { } }"),
        cb::java::parse::parse_source("class Used { }"),
        cb::java::parse::parse_source("class Child extends Used { Child(Used u) { } }"),
    };
    const cor::Dropped dropped = cor::drop_synthetic_artifacts(classes);
    CB_CHECK(dropped.markers == cb::java::typemap::NameSet{"Empty7"});
    CB_CHECK_EQ(classes.size(), std::size_t{3});
    CB_CHECK_EQ(classes[0].methods.size(), std::size_t{1});   // the bridge constructor of User went with the marker
    CB_CHECK_EQ(classes[2].methods.size(), std::size_t{1});   // Child(Used): Used is a base, so no marker and the constructor stays
    CB_CHECK(cor::drop_synthetic_artifacts(classes).markers.empty());
});

CB_TEST("java.corpus.stub_member_types", [] {
    const auto runtime = cb::java::runtime::Runtime(cb::java::JavaData(cb::testing::data_path("../../codebreaker/java")));
    const auto env = cor::build_member_env({}, runtime);
    // a runtime stub's method counts when its result is a single reference pointer or std::string
    CB_CHECK_EQ(env.at("URL").methods.at("openStream").to_string(), "InputStream");
    CB_CHECK_EQ(env.at("URL").methods.at("getHost").to_string(), "String");
    CB_CHECK(!env.at("URL").methods.contains("toURI"));          // void*: left out, the chain resolver does not guess
    CB_CHECK(!env.at("File").methods.contains("listFiles"));     // JArray<File* >
    CB_CHECK_EQ(env.at("Applet").parent.value(), "Component");
    CB_CHECK(env.contains("StringBuilder"));
});

}  // namespace
