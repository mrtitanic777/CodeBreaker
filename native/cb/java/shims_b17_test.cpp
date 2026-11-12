// The shims at the old header paths for java/{types, report, analyze, twophase, parse, rules}: they must compile with
// the old layer and forward to the new code. Built at /W3 (the old layer is not warning-clean).
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "codebreaker/java/analyze.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/parse.hpp"
#include "codebreaker/java/report.hpp"
#include "codebreaker/java/rules.hpp"
#include "codebreaker/java/twophase.hpp"
#include "codebreaker/java/types.hpp"
#include "codebreaker/java/typemap.hpp"

namespace {

namespace jv = codebreaker::java;
using cb::compat::from_py;
using cb::compat::to_py;

py::ref<jv::ir::JType> jt(const char* name, int64_t dims = 0, const py::list<py::ref<jv::ir::JType>>& args = py::list<py::ref<jv::ir::JType>>()) {
    return std::make_shared<jv::ir::JType>(py::str(name), dims, args);
}

CB_TEST("java.shims_b17.types", [] {
    py::set<py::str> known;
    known.add(py::str("Block"));
    py::set<py::str> values;
    values.add(py::str("Vec3D"));
    jv::types::TypeResolver r(known, values);
    CB_CHECK_EQ(from_py(r.field_type(jt("Block"))), "Block*");
    CB_CHECK_EQ(from_py(r.field_type(jt("Vec3D"))), "Vec3D");
    CB_CHECK_EQ(from_py(r.param_type(jt("String"))), "const JavaString&");
    CB_CHECK_EQ(from_py(r.field_type(jt("int", 2))), "JArray<JArray<int > >");
    CB_CHECK_EQ(from_py(r.ownership(jt("Block"))), "pointer");
    CB_CHECK(r.is_value(jt("int")) && r.is_primitive(jt("int")) && r.is_pointer(jt("Mystery")));
    py::list<py::ref<jv::ir::JType>> args;
    args.append(jt("Block"));
    CB_CHECK_EQ(from_py(r.cpp_type(jt("List", 0, args), py::str("field"))), "JList<Block*>");
    CB_CHECK_EQ(from_py(r.cpp_type(jt("Block"), py::str("element"))), "Block");
    CB_CHECK(r.port_note(jt("Object")).has_value() && !r.port_note(jt("Block")).has_value());
    // the pipeline grows the sets after construction: the resolver follows
    r.known.add(py::str("World"));
    CB_CHECK_EQ(from_py(r.field_type(jt("world"))), "world");
    r.typed_enums.add(py::str("EnumArt"));
    r.enums.add(py::str("EnumArt"));
    CB_CHECK_EQ(from_py(r.field_type(jt("EnumArt"))), "EnumArt*");
    // method renames and the member environment
    r.method_renames.set(py::str("Entity"), py::set<py::str>());
    r.method_renames[py::str("Entity")].add(py::str("health"));
    CB_CHECK_EQ(from_py(r.renamed_method(py::str("Entity"), py::str("health"))), "health_");
    jv::types::MemberEnv env;
    env.fields.set(py::str("chunks"), jt("Chunk", 1));
    env.parent = py::str("List");
    r.members.set(py::str("World"), env);
    CB_CHECK(r.member_field_type(py::str("World"), py::str("chunks")) != nullptr);
    CB_CHECK(r.member_field_type(py::str("World"), py::str("nope")) == nullptr);
    CB_CHECK(r.extends_value_container(py::str("World")));
});

CB_TEST("java.shims_b17.parse_and_twophase", [] {
    const py::str block = py::str("public class Block {\n\tprotected Block(int var1) {\n\t\tthis.id = var1;\n\t\tthis.opaque = this.isOpaqueCube();\n\t}\n\n"
                                  "\tpublic boolean isOpaqueCube() {\n\t\treturn true;\n\t}\n}\n");
    const py::str glass = py::str("public class BlockGlass extends Block {\n\tpublic BlockGlass(int var1) {\n\t\tsuper(var1);\n\t}\n\n"
                                  "\tpublic boolean isOpaqueCube() {\n\t\treturn false;\n\t}\n}\n");
    py::ref<jv::ir::JClass> b = jv::parse::parse_source(block);
    py::ref<jv::ir::JClass> g = jv::parse::parse_source(glass, py::str("BlockGlass.java"));
    CB_CHECK_EQ(from_py(b->name), "Block");
    CB_CHECK_EQ(from_py(*g->parent), "Block");
    CB_CHECK(g->source_path.has_value());
    CB_CHECK_EQ(b->constructors().size(), 1);
    CB_CHECK_EQ(from_py(jv::parse::parse_type(py::str("java.util.List<a.B>[]"))->repr()), "List<B>[]");
    CB_CHECK_THROWS_AS(py::ValueError, (void)jv::parse::parse_source(py::str("no type")));
    CB_CHECK_THROWS_AS(py::Exception, (void)jv::parse::parse_file(py::str("no/such/File.java")));

    py::list<py::ref<jv::ir::JClass>> classes;
    classes.append(b);
    classes.append(g);
    py::set<py::str> two = jv::twophase::compute(classes);
    CB_CHECK_EQ(two.size(), 2);
    CB_CHECK(two.contains(py::str("Block")) && two.contains(py::str("BlockGlass")));
    CB_CHECK_EQ(jv::twophase::compute(classes, py::set<py::str>::from(two)).size(), 0);
    auto [text, n] = jv::twophase::rewrite_news(py::str("x = new Block(f(1), 2);"), two);
    CB_CHECK_EQ(from_py(text), "x = (new Block())->j2xInit(f(1), 2);");
    CB_CHECK_EQ(n, 1);
    auto [text2, n2] = jv::twophase::rewrite_stack(py::str("Block _j2xleak0(3);"), two);
    CB_CHECK_EQ(from_py(text2), "Block _j2xleak0; _j2xleak0.j2xInit(3);");
    CB_CHECK_EQ(n2, 1);

    cb::fs::ScopedTempDir tmp("cbshim17");
    cb::fs::write_text(tmp.path() / "A.cpp", "auto b = new Block(1);\n");
    auto run = jv::twophase::run(py::str(cb::fs::to_text(tmp.path())), two);
    CB_CHECK_EQ(run.sites, 1);
    CB_CHECK_EQ(run.files.size(), 1);
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "A.cpp"), "auto b = (new Block())->j2xInit(1);\n");
});

CB_TEST("java.shims_b17.rules_analyze_and_report", [] {
    cb::fs::ScopedTempDir tmp("cbshim17");
    cb::fs::write_text(tmp.path() / "Foo.h", "// Target: Xbox 360\nclass Foo {};\n");
    cb::fs::write_text(tmp.path() / "Foo.cpp",
                       "// PORT: pointer chosen by guess\nbool* a = new bool[4];\nvoid Foo::f(BigBuf b)\n{\n\tx = getBlock(1)->id;\n}\n");
    // the old state the scan reads: heavy types by _reseed, nullable getters in the typemap globals
    py::list<py::str> heavy;
    heavy.append(py::str("BigBuf"));
    jv::rules::_reseed(heavy, py::list<py::str>());
    jv::typemap::KNOWN_NULLABLE_GETTERS.add(py::str("getBlock"));

    jv::ledger::Ledger led;
    const int64_t n = jv::rules::scan_rules(led, py::str(cb::fs::to_text(tmp.path())), py::str("xbox360"));
    CB_CHECK_EQ(n, 3);
    CB_CHECK_EQ(led.size(), 3);
    CB_CHECK_EQ(from_py(led.events[0]->rule), "uninit-array");
    CB_CHECK_EQ(from_py(led.events[0]->source), "rule");
    CB_CHECK_EQ(from_py(led.events[0]->category), "RISK");
    CB_CHECK_EQ(led.events[0]->line, 2);

    jv::ledger::Ledger all;
    const auto counts = jv::analyze::analyze_all(all, py::str(cb::fs::to_text(tmp.path())), std::nullopt, py::str(cb::fs::to_text(tmp.path() / "noroot")));
    CB_CHECK_EQ(counts.markers, 1);
    CB_CHECK_EQ(counts.rules, 3);   // the platform came from the header
    CB_CHECK_EQ(counts.lint, 0);
    CB_CHECK(!counts.build_log_path.has_value());
    CB_CHECK_EQ(all.size(), 4);
    CB_CHECK_EQ(from_py(all.events[0]->category), "GUESSED");

    py::dict<py::str, py::str> meta;
    meta.set(py::str("profile"), py::str("shim test"));
    const py::str dir = jv::report::write_reports(all, py::str(cb::fs::to_text(tmp.path())), std::nullopt, meta);
    CB_CHECK_EQ(from_py(dir), cb::fs::to_text(tmp.path() / "report"));
    const std::string index = cb::fs::read_text(tmp.path() / "report" / "index.md");
    CB_CHECK(index.find("- **Profile:** shim test") != std::string::npos);
    CB_CHECK(index.find("[Foo.cpp](files/Foo.cpp.md)") != std::string::npos);
    CB_CHECK(std::filesystem::is_regular_file(tmp.path() / "report" / "files" / "Foo.cpp.md"));
    CB_CHECK(std::filesystem::is_regular_file(tmp.path() / "report" / "report.json"));
    jv::typemap::KNOWN_NULLABLE_GETTERS = py::set<py::str>();
    jv::rules::_reseed(py::list<py::str>(), py::list<py::str>());
});

}  // namespace
