// The shims at the old header paths (native/typed/cpp/codebreaker/java/*.hpp) over the modules of this folder: they
// must compile with the old layer and forward to the new code. Built at /W3 (the old layer is not warning-clean).
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/cmake.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/overlays.hpp"
#include "codebreaker/java/stubs.hpp"
#include "codebreaker/java/textutil.hpp"
#include "codebreaker/java/typemap.hpp"

namespace {

namespace jv = codebreaker::java;
using cb::compat::from_py;
using cb::compat::to_py;

CB_TEST("java.shims.paths_and_text", [] {
    jv::_paths::set_here(py::str("root/java"));
    CB_CHECK_EQ(from_py(jv::_paths::data_path(py::list<py::str>{py::str("platform"), py::str("canonical")})),
                "root/java\\platform\\canonical");

    cb::fs::ScopedTempDir tmp("cbshim");
    const std::string file = cb::fs::to_text(tmp.path() / "a.txt");
    jv::textutil::write_text(to_py(file), py::str("x\r\ny"));
    CB_CHECK_EQ(from_py(jv::textutil::read_text(to_py(file))), "x\ny");
    CB_CHECK_EQ(from_py(jv::textutil::read_text(to_py(file + ".missing"))), "");   // missing_ok defaults to true
    CB_CHECK_THROWS_AS(py::OSError, jv::textutil::read_text(to_py(file + ".missing"), py::str("replace"), false));

    jv::cmake::write_cmakelists(to_py(cb::fs::to_text(tmp.path())), py::str("Game"));
    CB_CHECK(cb::fs::read_text(tmp.path() / "CMakeLists.txt").find("project(Game CXX)") != std::string::npos);
});

CB_TEST("java.shims.overlays", [] {
    cb::fs::ScopedTempDir tmp("cbshim");
    cb::fs::ScopedEnv root("CB_JAVA_OVERLAYS", cb::fs::to_text(tmp.path() / "overlays"));
    cb::fs::write_text(tmp.path() / "overlays" / "Proj" / "A.cpp", "fixed");
    auto dir = jv::overlays::resolve_overlay_dir(std::optional<py::str>(to_py(cb::fs::to_text(tmp.path() / "out" / "Proj"))));
    CB_CHECK(dir.has_value());
    std::vector<std::string> log;
    auto applied = jv::overlays::apply_overlays(to_py(cb::fs::to_text(tmp.path() / "out" / "Proj")), dir,
                                                [&](const py::str& s) { log.push_back(from_py(s)); });
    CB_CHECK_EQ(applied.size(), 1);
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "out" / "Proj" / "A.cpp"), "fixed");
    CB_CHECK_EQ(log.size(), std::size_t{2});
    CB_CHECK_EQ(from_py(jv::overlays::project_key(std::optional<py::str>(py::str("x/Beta")))), "Beta");
});

CB_TEST("java.shims.stubs_and_typemap", [] {
    auto [id, info] = jv::stubs::classify_stub(py::str("GL11"));
    CB_CHECK_EQ(from_py(id), "rendering");
    CB_CHECK(info.has_value() && info->members.contains(py::str("GL11")));
    CB_CHECK_EQ(from_py(std::get<0>(jv::stubs::classify_stub(py::str("Nope")))), "unknown");

    CB_CHECK(jv::typemap::PRIMITIVES.contains(py::str("int")));
    CB_CHECK_EQ(from_py(jv::typemap::JAVA_TO_CPP[py::str("byte")]), "signed char");
    CB_CHECK(jv::typemap::KNOWN_NULLABLE_GETTERS.empty());   // the generic profile
    jv::typemap::Profile p;
    p.nullable_getters.add(py::str("getBlock"));
    jv::typemap::_reseed(p);
    CB_CHECK(jv::typemap::KNOWN_NULLABLE_GETTERS.contains(py::str("getBlock")));
    jv::typemap::_reseed(jv::typemap::Profile{});
    CB_CHECK(jv::typemap::KNOWN_NULLABLE_GETTERS.empty());
});

CB_TEST("java.shims.ir_round_trip", [] {
    using namespace jv::ir;
    auto c = std::make_shared<JClass>(py::str("Entity"), std::optional<py::str>(py::str("net.minecraft")),
                                      std::optional<py::str>(py::str("Base")), py::list<py::str>{py::str("Runnable")});
    c->fields.append(std::make_shared<Field>(py::str("hp"),
                                             std::make_shared<JType>(py::str("List"), 1, py::list<py::ref<JType>>{std::make_shared<JType>(py::str("Item"))}),
                                             PRIVATE, false, true, std::optional<py::str>(py::str("new X()")), 7));
    auto m = std::make_shared<Method>(py::str("tick"), std::make_shared<JType>(py::str("void")));
    m->params.append(std::make_shared<Param>(py::str("dt"), std::make_shared<JType>(py::str("float")), true));
    m->params[0]->by_ref = true;
    m->body.append(std::make_tuple(int64_t(12), py::str("x++;")));
    c->methods.append(m);
    c->enum_consts.append(std::make_shared<EnumConst>(py::str("A"), py::list<py::str>{py::str("1")}));

    cb::java::ir::JClass n = to_new(*c);
    CB_CHECK_EQ(n.name, "Entity");
    CB_CHECK(n.owns_heap_field());
    CB_CHECK_EQ(n.fields[0].type.to_string(), "List<Item>[]");
    CB_CHECK(n.fields[0].access == cb::java::ir::Access::Private);
    CB_CHECK(n.methods[0].params[0].by_ref);
    CB_CHECK_EQ(n.methods[0].body[0].number, std::uint32_t{12});

    py::ref<JClass> back = from_new(n);
    CB_CHECK_EQ(from_py(back->name), "Entity");
    CB_CHECK(back->owns_heap_field());
    CB_CHECK(*back->fields[0]->jtype == *c->fields[0]->jtype);
    CB_CHECK(back->methods[0]->params[0]->by_ref);
    CB_CHECK_EQ(from_py(std::get<1>(back->methods[0]->body[0])), "x++;");
    CB_CHECK_EQ(from_py(back->repr()), from_py(c->repr()));
});

}  // namespace
