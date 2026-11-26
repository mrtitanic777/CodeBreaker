#include "java/emit/source.hpp"

#include <nlohmann/json.hpp>

#include <map>
#include <string>
#include <vector>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "java/emit/source_test_support.hpp"
#include "java/parse.hpp"

namespace {

namespace src = cb::java::emit::source;
namespace hd = cb::java::emit::header;
namespace ir = cb::java::ir;
namespace ledger = cb::java::ledger;
namespace types = cb::java::types;
using cb::Json;

std::filesystem::path srct_dir() { return cb::testing::data_path("java/testdata/source"); }

types::TypeResolver srct_resolver(const Json& r) {
    const auto names = [&r](const char* key) {
        cb::java::typemap::NameSet out;
        if (r.contains(key)) {
            for (const auto& n : r[key]) out.insert(n.get<std::string>());
        }
        return out;
    };
    types::ResolverData data;
    data.known = names("known");
    data.values = names("values");
    data.interfaces = names("interfaces");
    data.enums = names("enums");
    data.typed_enums = names("typed_enums");
    if (r.contains("members")) {
        for (const auto& [cls, rec] : r["members"].items()) {
            types::MemberEnv env;
            if (rec.contains("fields")) {
                for (const auto& [n, t] : rec["fields"].items()) env.fields.emplace(n, cb::java::parse::parse_type(t.get<std::string>()));
            }
            if (rec.contains("methods")) {
                for (const auto& [n, t] : rec["methods"].items()) env.methods.emplace(n, cb::java::parse::parse_type(t.get<std::string>()));
            }
            data.members.emplace(cls, std::move(env));
        }
    }
    if (r.contains("method_renames")) {
        for (const auto& [cls, list] : r["method_renames"].items()) {
            for (const auto& n : list) data.method_renames[cls].insert(n.get<std::string>());
        }
    }
    return types::TypeResolver(std::move(data));
}

std::string srct_events(const ledger::Ledger& l, bool has_init_statics) {
    std::string out = std::string("has_init_statics=") + (has_init_statics ? "true" : "false") + "\n";
    for (const ledger::Event& e : l.events()) {
        out += std::format("{}|{}|{}|{}|{}|{}|{}\n", e.file, ledger::name(e.category), ledger::name(e.severity), e.source, e.rule, e.line, e.message);
    }
    return out;
}

// testdata/source/<case>.{cpp,events}.golden: the recorded output for each case of cases.json.
CB_TEST("java.source.cases_match_golden", [] {
    const Json cases = cb::json::load(srct_dir() / "cases.json");
    CB_CHECK_EQ(cases["cases"].size(), std::size_t{18});
    for (const Json& c : cases["cases"]) {
        const std::string name = c["name"].get<std::string>();
        std::map<std::string, ir::JClass> classes;   // (nodes stay where they are: the MI forwards point into them)
        for (const auto& cn : c["classes"]) classes.emplace(cn.get<std::string>(), cb::java::parse::parse_file(srct_dir() / (cn.get<std::string>() + ".java")));
        ir::JClass& target = classes.at(c["target"].get<std::string>());
        if (c.contains("mi_forwards")) {
            for (const Json& mf : c["mi_forwards"]) {
                const ir::Method* method = classes.at(mf["interface"].get<std::string>()).method(mf["method"].get<std::string>());
                target.mi_forwards.push_back({method, mf["base"].get<std::string>()});
            }
        }
        const types::TypeResolver resolver = srct_resolver(c["resolver"]);
        src::FakeBodies fake;
        ledger::Ledger led;
        src::Options options;
        if (c["year"].get<int>() != 0) options.cpp_std = hd::CppStd::of_year(c["year"].get<int>());
        options.platform_label = c["platform"].get<std::string>();
        options.ledger = &led;
        if (c.value("bodies", true)) options.bodies = &fake;
        if (c.contains("two_phase")) {
            for (const auto& n : c["two_phase"]) options.two_phase.insert(n.get<std::string>());
        }
        const src::Rendered out = src::render_source(target, resolver, options);
        const std::string text = out.text ? *out.text : "<none>\n";
        CB_CHECK_EQ(text, cb::testing::regold_golden(srct_dir() / (name + ".cpp.golden"), text));
        const std::string events = srct_events(led, out.has_init_statics);
        CB_CHECK_EQ(events, cb::testing::regold_golden(srct_dir() / (name + ".events.golden"), events));
    }
});

CB_TEST("java.source.deferrable_static_init", [] {
    CB_CHECK(!src::is_deferrable_static_init("16", "int"));                          // a constant stays eager
    CB_CHECK(!src::is_deferrable_static_init("new Foo()", "const Foo*"));            // a const member cannot be reassigned
    CB_CHECK(src::is_deferrable_static_init("new Foo()", "Foo*"));                   // constructs an object
    CB_CHECK(src::is_deferrable_static_init("Block::s_air", "Block*"));              // reads another class's statics
    CB_CHECK(src::is_deferrable_static_init("x->y", "int"));
    CB_CHECK(src::is_deferrable_static_init("func_25153_a(\"stat\", 16)", "JArray<int >"));   // an obfuscated function call
    CB_CHECK(src::is_deferrable_static_init("1 + compute(2)", "int"));
    CB_CHECK(!src::is_deferrable_static_init("JArray<int >(16)", "JArray<int >"));   // a PascalCase constructor only allocates
    CB_CHECK(!src::is_deferrable_static_init("renew + 1", "int"));                   // `new` inside a word is no `new`
    CB_CHECK(!src::is_deferrable_static_init("a.b(2)", "int"));                      // a member call is preceded by `.`
    CB_CHECK(src::is_deferrable_static_init("(f (2))", "int"));                      // whitespace before the parenthesis
    CB_CHECK(!src::is_deferrable_static_init("Foo::bar", "const int"));              // `const` first: eager whatever the init
});

CB_TEST("java.source.multi_dimensional_aggregate", [] {
    CB_CHECK(src::is_multi_dimensional_aggregate("new int[][]{{1}}"));
    CB_CHECK(src::is_multi_dimensional_aggregate("new a.B [ ] [ ] [ ] { }"));
    CB_CHECK(!src::is_multi_dimensional_aggregate("new int[]{1,2}"));       // the one-dimensional form is a flat array
    CB_CHECK(!src::is_multi_dimensional_aggregate("new int[][4]"));          // a sized dimension is no aggregate
    CB_CHECK(!src::is_multi_dimensional_aggregate("new int[][]"));           // no braces
    CB_CHECK(!src::is_multi_dimensional_aggregate("newint[][]{}"));          // `new` needs a space after it
    CB_CHECK(!src::is_multi_dimensional_aggregate(" new int[][]{}"));        // anchored at the start
    CB_CHECK(!src::is_multi_dimensional_aggregate(""));
});

CB_TEST("java.source.default_value", [] {
    CB_CHECK_EQ(src::default_value("Foo*").value(), "NULL");
    CB_CHECK_EQ(src::default_value("int").value(), "0");
    CB_CHECK_EQ(src::default_value("signed char").value(), "0");   // Java byte: it was missing once, and every byte field held garbage
    CB_CHECK_EQ(src::default_value("long long").value(), "0");
    CB_CHECK(!src::default_value("JavaString").has_value());       // a value type runs its default constructor
    CB_CHECK(!src::default_value("JArray<int >").has_value());
});

CB_TEST("java.source.referenced_types_closure", [] {
    const ir::JClass cls = cb::java::parse::parse_source(R"(package t;
public class Holder extends Base implements Tickable {
    private Hop first;
    public Leaf find(Other o) { return null; }
}
)");
    const types::TypeResolver resolver = srct_resolver(Json::parse(R"({
        "known": ["Holder", "Base", "Tickable", "Hop", "Node", "Leaf", "Other", "Unused", "Deep", "Body"],
        "members": {"Hop": {"fields": {"next": "Node"}}, "Node": {"fields": {"inner": "Deep"}}}})"));
    // the signature's types, the capitalised words of the body, `Class` for the RTTI calls, and what the member chains reach;
    // never the class itself or what is not a user or runtime type
    const std::vector<std::string> got = src::referenced_types(cls, resolver, "x = new Body(); Foo::bar; j2xClassOf(typeid(x));");
    CB_CHECK(got == (std::vector<std::string>{"Base", "Body", "Class", "Deep", "Hop", "Leaf", "Node", "Other", "Tickable"}));
});

}  // namespace
