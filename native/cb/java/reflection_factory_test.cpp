#include <nlohmann/json.hpp>

#include <algorithm>
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/reflection_factory.hpp"

namespace {

using namespace cb::java::reflection_factory;
namespace stdfs = std::filesystem;

stdfs::path data(const char* relative) { return cb::testing::data_path(std::string("java/testdata/reflection/") + relative); }

void copy_into(const stdfs::path& from, const stdfs::path& to) {
    for (const auto& e : stdfs::directory_iterator(from)) cb::fs::write_file(to / e.path().filename(), cb::fs::read_file(e.path()));
}

// Compare every file of a tree with the expected folder (and nothing more, nothing less).
void expect_tree(const stdfs::path& actual, const stdfs::path& expected) {
    std::vector<std::string> a, e;
    for (const auto& f : stdfs::directory_iterator(actual)) a.push_back(f.path().filename().string());
    for (const auto& f : stdfs::directory_iterator(expected)) e.push_back(f.path().filename().string());
    std::sort(a.begin(), a.end());
    std::sort(e.begin(), e.end());
    CB_CHECK(a == e);
    for (const std::string& name : e) {
        if (!stdfs::exists(actual / name)) continue;
        CB_CHECK_EQ(cb::fs::read_text(actual / name), cb::fs::read_text(expected / name));
    }
}

void expect_result(const RunResult& r, const cb::Json& j) {
    CB_CHECK_EQ(r.total_sites, j["total_sites"].get<std::size_t>());
    CB_CHECK_EQ(r.total_classes, j["total_classes"].get<std::size_t>());
    CB_CHECK_EQ(r.registries.size(), j["registries"].size());
    for (std::size_t i = 0; i < r.registries.size() && i < j["registries"].size(); ++i) {
        const cb::Json& want = j["registries"][i];
        CB_CHECK_EQ(r.registries[i].file, want["file"].get<std::string>());
        CB_CHECK_EQ(r.registries[i].sites, want["sites"].get<std::size_t>());
        CB_CHECK_EQ(r.registries[i].factory.registry, want["registry"].get<std::string>());
        CB_CHECK_EQ(r.registries[i].factory.ret, want["ret"].get<std::string>());
        CB_CHECK(r.registries[i].factory.args == want["args"].get<std::vector<std::string>>());
        CB_CHECK(r.registries[i].factory.classes == want["classes"].get<std::vector<std::string>>());
    }
    CB_CHECK_EQ(r.reused.size(), j["reused"].size());
    for (std::size_t i = 0; i < r.reused.size() && i < j["reused"].size(); ++i) {
        CB_CHECK_EQ(r.reused[i].file, j["reused"][i]["file"].get<std::string>());
        CB_CHECK_EQ(r.reused[i].sites, j["reused"][i]["sites"].get<std::size_t>());
    }
}

// testdata/reflection/{in,in2,expected1,expected2,results.json}: an emitted tree with a World-arg registry
// (EntityList), a no-arg one (TileEntity), files that reuse the factories (World.cpp: a base-class cast, a site no
// factory fits, a plain site; Other.cpp already includes the header; NoIncludes.cpp has no include line), and an
// untouched file; then a second run after Late.cpp is added, which must find the factories left by the first. Both
// expected trees and result tables are what the Python module produced (reviewed by reading each file).
// DIVERGES: the Python rewrote files with CRLF line ends (text mode on Windows); the C++ writes LF, and the expected
// files were normalised to LF.
CB_TEST("java.reflection_factory.run_matches_python", [] {
    cb::fs::ScopedTempDir tmp("cbrefl");
    copy_into(data("in"), tmp.path());
    const Registrars registrars{{"addMapping", "addIdClassMapping"}};
    const cb::Json results = cb::json::load(data("results.json"));

    expect_result(run(tmp.path(), registrars), results["run1"]);
    expect_tree(tmp.path(), data("expected1"));

    copy_into(data("in2"), tmp.path());
    expect_result(run(tmp.path(), registrars), results["run2"]);
    expect_tree(tmp.path(), data("expected2"));

    // a third run changes nothing
    RunResult third = run(tmp.path(), registrars);
    CB_CHECK(third.registries.empty() && third.reused.empty());
    expect_tree(tmp.path(), data("expected2"));
});

// The default registrars: any call to a function named register*; the same text of a line that has another
// registrar (addMapping) is not part of the table.
CB_TEST("java.reflection_factory.default_registrars", [] {
    cb::fs::ScopedTempDir tmp("cbrefl");
    copy_into(data("in_default"), tmp.path());
    RunResult r = run(tmp.path());
    expect_result(r, cb::json::load(data("results_default.json")));
    expect_tree(tmp.path(), data("expected_default"));
});

CB_TEST("java.reflection_factory.registrars_match_lines", [] {
    CB_CHECK(Registrars{}.matches_line("\tregisterEntity(j2xClassOf(typeid(Pig)), 1);"));
    CB_CHECK(Registrars{}.matches_line("x.register (y)"));
    CB_CHECK(!Registrars{}.matches_line("unregister(y)"));          // the name must start with register
    CB_CHECK(!Registrars{}.matches_line("registerX = 1;"));         // and be called
    CB_CHECK(!Registrars{}.matches_line("addMapping(a)"));
    const Registrars named{{"addMapping"}};
    CB_CHECK(named.matches_line("  addMapping(j2xClassOf(typeid(Pig)))"));
    CB_CHECK(named.matches_line("table.addMapping (x)"));
    CB_CHECK(!named.matches_line("addMappingX(x)"));                // whole identifiers only
    CB_CHECK(!named.matches_line("xaddMapping(x)"));
    CB_CHECK(!named.matches_line("registerEntity(x)"));             // naming registrars replaces the default
    CB_CHECK(!named.matches_line("addMapping;"));
});

CB_TEST("java.reflection_factory.process_details", [] {
    const Registrars reg{{"add"}};
    // nothing to rewrite: the text comes back (only resolved review tags would be dropped)
    auto none = process("int main() { return 0; }\n", "Main", reg);
    CB_CHECK_EQ(none.sites, std::size_t{0});
    CB_CHECK(!none.factory.has_value());

    // a site but no registration table: untouched, no factory
    const std::string lonely = "#include \"a.h\"\nT* f() { return (T*)c->newInstance(); }\n";
    auto no_table = process(lonely, "Lonely", reg);
    CB_CHECK_EQ(no_table.text, lonely);
    CB_CHECK_EQ(no_table.sites, std::size_t{0});
    CB_CHECK(!no_table.factory.has_value());

    // a no-arg registry: the factory goes after the last include, the site calls it
    const std::string text = "#include <a>\n#include \"b.h\"\nint x;\nvoid t() { add(Class(typeid(Foo))); add(j2xClassOf(typeid(Bar))); }\nBase* g() { return (Base*)p[i]->newInstance(); }\n";
    auto r = process(text, "Reg", reg);
    CB_CHECK_EQ(r.sites, std::size_t{1});
    CB_CHECK(r.factory.has_value());
    CB_CHECK_EQ(r.factory->ret, "Base");
    CB_CHECK(r.factory->classes == (std::vector<std::string>{"Foo", "Bar"}));
    CB_CHECK(r.text.find("#include \"b.h\"\n\n// ==== CodeBreaker-generated reflection factory") != std::string::npos);
    CB_CHECK(r.text.find("Base* Reg_createByReflection(Class* c)\n{\n\tif (!c) return NULL;\n\tif (*c == Class(typeid(Foo))) return new Foo();") != std::string::npos);
    CB_CHECK(r.text.find("return Reg_createByReflection(p[i]);") != std::string::npos);
    CB_CHECK(r.text.find("->newInstance(") == std::string::npos);   // (the factory's own comment names newInstance)
});

CB_TEST("java.reflection_factory.header", [] {
    std::vector<FactoryRef> f = {{"EntityList_createByReflection", "Entity", {"World"}},
                                 {"TileEntity_createByReflection", "TileEntity", {}},
                                 {"EntityList_createByReflection", "Entity", {"World"}}};   // a duplicate declares once
    const std::string h = header_text(f);
    CB_CHECK(h.starts_with("// ReflectionFactory.h -- CodeBreaker-generated."));
    CB_CHECK(h.find("class Class; class Entity; class TileEntity; class World;\n") != std::string::npos);
    CB_CHECK(h.find("Entity* EntityList_createByReflection(Class* c, World* a0);\nTileEntity* TileEntity_createByReflection(Class* c);\n") != std::string::npos);
    CB_CHECK_EQ(h.find("EntityList_createByReflection(Class* c, World* a0);", h.find("EntityList_createByReflection(Class* c, World* a0);") + 1), std::string::npos);
    CB_CHECK(h.ends_with("#endif // J2X_REFLECTION_FACTORY_H\n"));
});

CB_TEST("java.reflection_factory.reuse_choices", [] {
    const std::vector<FactoryRef> factories = {{"E_createByReflection", "Entity", {"World"}}, {"T_createByReflection", "TileEntity", {}}};
    // a base of the cast is served by the factory whose return type is a prefix of it
    auto r = reuse("(EntityLiving*)v->getConstructor((JArrayInit<Class* >(), Class(typeid(World)))).newInstance((JArrayInit<JObject >(), w))", factories);
    CB_CHECK_EQ(r.second, std::size_t{1});
    CB_CHECK_EQ(r.first, "(EntityLiving*)E_createByReflection(v, (World*)w)");
    // two no-arg-less choices and no return-type match: no unique answer, the site stays
    const std::vector<FactoryRef> two = {{"A_createByReflection", "A", {}}, {"B_createByReflection", "B", {}}};
    auto stay = reuse("(C*)x->newInstance()", two);
    CB_CHECK_EQ(stay.second, std::size_t{0});
    CB_CHECK_EQ(stay.first, "(C*)x->newInstance()");
    CB_CHECK_EQ(reuse("(B*)x->newInstance()", two).first, "(B*)B_createByReflection(x)");
    // receivers: this->field, indexing, chains
    CB_CHECK_EQ(reuse("(TileEntity*)this->a[i]->b->newInstance()", factories).first, "(TileEntity*)T_createByReflection(this->a[i]->b)");
    // an arrow inside an argument does not split it
    CB_CHECK_EQ(reuse("(Entity*)r->getConstructor((JArrayInit<Class* >(), j2xClassOf(typeid(World)))).newInstance((JArrayInit<JObject >(), a->b, c))", factories).first,
                "(Entity*)E_createByReflection(r, (World*)a->b)");
});

CB_TEST("java.reflection_factory.resolved_review_tags", [] {
    const std::vector<FactoryRef> factories = {{"T_createByReflection", "T", {}}};
    auto r = reuse("// J2X-REVIEW[reflection-unsupported]\nx = (T*)v->newInstance();\n// J2X-REVIEW[reflection-unsupported]\ny = 1;\n", factories);
    CB_CHECK_EQ(r.first, "x = (T*)T_createByReflection(v);\n// J2X-REVIEW[reflection-unsupported]\ny = 1;\n");   // only the resolved one goes
});

CB_TEST("java.reflection_factory.run_errors", [] {
    cb::fs::ScopedTempDir tmp("cbrefl");
    CB_CHECK_THROWS_AS(cb::NotFoundError, run(tmp.path() / "missing"));
    auto empty = run(tmp.path());
    CB_CHECK(empty.registries.empty() && empty.reused.empty());
    CB_CHECK(!stdfs::exists(tmp.path() / "ReflectionFactory.h"));   // no factory, no header
});

}  // namespace


