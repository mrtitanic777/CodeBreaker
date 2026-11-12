// The old java scaffold, emit/ownership and emit/header API (codebreaker::*, over py::) as the compat shims at
// native/typed/cpp/codebreaker/java/... give it: conversion and forwarding to cb/java. Builds at /W3. (The signatures,
// projects/package and knowledge/show shims need the old pipeline, the Minecraft project and the knowledge store: they are
// compiled by shim_check_w2b9.cpp and run through the exe.)
#include "codebreaker/java/emit/header.hpp"
#include "codebreaker/java/emit/ownership.hpp"
#include "codebreaker/java/parse.hpp"
#include "codebreaker/java/scaffold.hpp"
#include "codebreaker/java/types.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace jv = codebreaker::java;
namespace own = codebreaker::java::emit::ownership;
namespace hd = codebreaker::java::emit::header;
using cb::compat::from_py;

std::vector<std::string> strs(const py::list<py::str>& l) {
    std::vector<std::string> out;
    for (const py::str& s : l) out.push_back(s.utf8());
    return out;
}

jv::types::TypeResolver old_resolver() {
    jv::types::TypeResolver r;
    for (const char* n : {"Thing", "Item", "Vec3D"}) r.known.add(py::str(n));
    r.values.add(py::str("Vec3D"));
    return r;
}

CB_TEST("compat.w2b9.scaffold_run_writes_the_skeletons", [] {
    cb::fs::ScopedTempDir tmp("cbw2b9");
    cb::fs::write_text(tmp.path() / "in" / "Empty.java", "public class Empty { public static void run() { } }");
    cb::fs::write_text(tmp.path() / "in" / "Bad.java", "not java");
    const auto r = jv::scaffold::run(py::str(cb::fs::to_text(tmp.path() / "in")), py::str(cb::fs::to_text(tmp.path() / "out")), py::str("rust"));
    CB_CHECK((strs(r.written) == std::vector<std::string>{"Empty.rs"}));
    CB_CHECK_EQ(r.classes, int64_t{1});
    CB_CHECK(jv::scaffold::LANG_LABEL.at(py::str("csharp")) == py::str("C#"));
    CB_CHECK_THROWS_AS(py::ValueError, (void)jv::scaffold::run(py::str("a"), py::str("b"), py::str("cobol")));
});

CB_TEST("compat.w2b9.ownership_answers_like_the_new_module", [] {
    const jv::types::TypeResolver r = old_resolver();
    const auto cls = jv::parse::parse_source(py::str(R"(public class Owner {
    private Thing made = new Thing();
    private Thing borrowed;
    public Owner(Thing t) { borrowed = t; }
    public void nullable(Vec3D v) { if (v == null) { return; } }
    public Thing make() { return new Thing(); }
    public Vec3D vec() { return null; }
}
)"));
    const auto owned = own::owned_pointers(cls, r);
    CB_CHECK_EQ(owned.size(), int64_t{1});
    CB_CHECK(owned.at(py::str("made")) == false);
    CB_CHECK(own::owns_heap(cls, r));
    CB_CHECK(own::params_str(cls->method(py::str("nullable")), r) == py::str("Vec3D* v"));
    CB_CHECK(own::return_cpp_type(cls->method(py::str("make")), r) == py::str("Thing*"));
    CB_CHECK(own::return_cpp_type(cls->method(py::str("vec")), r) == py::str("Vec3D*"));
    CB_CHECK(own::returns_pointer(cls->method(py::str("vec")), r));
    CB_CHECK(own::returns_owned_scalar(cls->method(py::str("make")), r));
    py::list<own::ClassRef> classes;
    classes.append(cls);
    const auto scalars = own::owned_scalar_return_methods(classes, r);
    CB_CHECK(scalars.at(py::str("make")) == py::str("Thing"));
    const auto retained = own::retained_param_positions(classes);
    CB_CHECK(retained.at(py::str("nullable")).contains(int64_t{0}));   // compared with null: not a pure read
});

CB_TEST("compat.w2b9.header_renders_and_records_on_the_old_ledger", [] {
    const jv::types::TypeResolver r = old_resolver();
    const auto cls = jv::parse::parse_source(py::str(R"(public class Holder {
    private Thing thing = new Thing();
    public Holder() { }
}
)"));
    jv::ledger::Ledger led;
    py::set<py::str> two_phase;
    two_phase.add(py::str("Holder"));
    const py::str h = hd::render_header(cls, r, &led, hd::CppStd(2011), py::str("xbox360"), two_phase);
    const std::string text = from_py(h);
    CB_CHECK(text.find("// Target: xbox360 / C++11\n") != std::string::npos);
    CB_CHECK(text.find("\tHolder* j2xInit();\n") != std::string::npos);
    CB_CHECK(text.find("\tHolder(const Holder&);\n") != std::string::npos);   // owns a heap field: the copy is disabled
    CB_CHECK_EQ(led.events.size(), int64_t{2});   // the Rule-of-Three risk, then "generated header"
    CB_CHECK(hd::_banner(py::str("A")) == py::str("// Auto-generated from A.java\n// Target: unknown / C++03"));
});

}  // namespace
