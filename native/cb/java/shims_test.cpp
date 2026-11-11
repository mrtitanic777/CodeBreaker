// The shims at the old header paths (native/typed/cpp/codebreaker/java/*.hpp) over the modules of this folder: they
// must compile with the old layer and forward to the new code. Built at /W3 (the old layer is not warning-clean).
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/apimap.hpp"
#include "codebreaker/java/cmake.hpp"
#include "codebreaker/java/console/assets.hpp"
#include "codebreaker/java/console/project_layout.hpp"
#include "codebreaker/java/console/project_scan.hpp"
#include "codebreaker/java/console/sdk.hpp"
#include "codebreaker/java/console/vcxproj.hpp"
#include "codebreaker/java/console/worklist.hpp"
#include "codebreaker/java/console/xact.hpp"
#include "codebreaker/java/console/xdk_scan.hpp"
#include "codebreaker/java/debug.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/overlays.hpp"
#include "codebreaker/java/platforms.hpp"
#include "codebreaker/java/reflection_factory.hpp"
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


CB_TEST("java.shims.platforms", [] {
    auto x360 = jv::platforms::get_platform(py::str("xbox360"));
    CB_CHECK(x360.has_value());
    CB_CHECK_EQ(from_py(x360->label), "Xbox 360");
    CB_CHECK(x360->big_endian && x360->sdk_dir.has_value());
    CB_CHECK_EQ(from_py(x360->subsystems.get(py::str("audio")).value().sdk), "XACT3");
    CB_CHECK(!jv::platforms::get_platform(py::str("amiga")).has_value());
    CB_CHECK_EQ(jv::platforms::PLATFORMS.size(), 7);
    CB_CHECK_EQ(from_py(jv::platforms::get_stub_comment(py::str("wii"), py::str("input"))),
                "// TODO [Nintendo Wii]: Implement with WPAD/PAD (libwiiuse.a, libogc) -- WPAD_Init, WPAD_ScanPads, WPAD_ButtonsDown, PAD_ScanPads");
    CB_CHECK(from_py(jv::platforms::generate_stdafx(py::str("psp"))).find("#include <pspkernel.h>") != std::string::npos);
});


CB_TEST("java.shims.ledger_and_debug", [] {
    namespace lg = jv::ledger;
    lg::Ledger l;
    l.add(py::str("b.cpp"), lg::Category::ERROR, py::str("bad"), 4);
    l.add(py::str("a.cpp"), lg::Category::PORTED, py::str("fine"));
    l.add(py::str("a.cpp"), lg::Category::GUESSED, py::str("hm"), 2, std::optional<py::str>(lg::Severity::LOW));
    CB_CHECK_EQ(l.size(), 3);
    CB_CHECK_EQ(from_py(l.events[0]->severity), "HIGH");               // the category's default
    CB_CHECK_EQ(from_py(l.events[2]->severity), "LOW");                // the producer's
    CB_CHECK_EQ(l.files().size(), 2);
    auto by_file = l.by_file();
    CB_CHECK(by_file.get(py::str("a.cpp")).value()[0] == l.events[1]);   // the old event objects, not copies
    CB_CHECK_EQ(l.totals_by_category().get(py::str("BLOCKED")).value(), 0);
    auto rows = l.file_risk_table();
    CB_CHECK_EQ(from_py(std::get<0>(rows[0])), "b.cpp");
    CB_CHECK_EQ(std::get<1>(rows[0]), 5);
    CB_CHECK(from_py(l.to_json()).find("\"totals_by_severity\"") != std::string::npos);
    CB_CHECK_THROWS_AS(py::ValueError, lg::Event(py::str("x"), py::str("NOPE"), py::str("m")));

    cb::fs::ScopedTempDir tmp("cbshim");
    auto paths = jv::debug::write_debug(to_py(cb::fs::to_text(tmp.path())), true);
    CB_CHECK_EQ(paths.size(), 3);
    auto markers = jv::debug::scan_debug_markers(to_py(cb::fs::to_text(tmp.path())));
    CB_CHECK(markers.contains(py::str("once")));
    CB_CHECK_EQ(from_py(std::get<0>(markers.get(py::str("once")).value()[0])), "Debug.h");
});


CB_TEST("java.shims.apimap", [] {
    namespace am = jv::apimap;
    // (load() needs the old platform_equivalents port, which is not part of this test: build the registry by hand)
    am::Registry reg(std::make_shared<cb::java::apimap::Registry>(
        cb::java::apimap::Registry::load(cb::testing::data_path("../../codebreaker/java/api_maps"))));
    auto m = reg.resolve(py::str("Math.sqrt"), py::str("java"), py::str("cpp"));
    CB_CHECK(m != nullptr);
    CB_CHECK_EQ(from_py(m->kind), "direct");
    CB_CHECK_EQ(from_py(m->__repr__()), "Mapping(java:Math.sqrt -> cpp:sqrt [direct])");
    auto m11 = reg.resolve(py::str("Math.sqrt"), py::str("java"), py::str("cpp"), std::optional<py::str>(py::str("c++11")));
    CB_CHECK_EQ(from_py(*m11->dialect), "c++11");
    CB_CHECK(reg.resolve(py::str("Nope.nothing"), py::str("java"), py::str("cpp")) == nullptr);
    CB_CHECK(reg.known(py::str("Math.sqrt"), py::str("java")));
    CB_CHECK(reg.languages.contains(py::str("cpp")) && reg.languages.get(py::str("cpp")).value().dialects->size() > 0);
    CB_CHECK(reg._sym.contains(std::make_tuple(py::str("java"), py::str("Math.sqrt"))));
    CB_CHECK(reg._sym.get(std::make_tuple(py::str("java"), py::str("Math.sqrt")))->maps.get(py::str("cpp")).value().kind.size() > 0);
    auto cov = reg.coverage(py::list<py::str>{py::str("Math.sqrt"), py::str("Nope.nothing")}, py::str("java"), py::str("cpp"));
    CB_CHECK_EQ(cov.mapped.size(), 1);
    CB_CHECK_EQ(cov.unmapped.size(), 1);
    CB_CHECK(reg.symbols_in(py::str("java.lang")).size() > 0);
    CB_CHECK(reg.stats().size() > 0);
    CB_CHECK_EQ(from_py(am::_leaf(py::str("std::vector::push_back"))), "push_back");
    CB_CHECK(am::extract_symbols(py::str("Math.sqrt(1)")).contains(py::str("Math.sqrt")));
});


CB_TEST("java.shims.reflection_factory", [] {
    namespace rf = jv::reflection_factory;
    cb::fs::ScopedTempDir tmp("cbshim");
    cb::fs::write_text(tmp.path() / "T.cpp", "#include \"t.h\"\nvoid i() { addMapping(Class(typeid(Foo))); }\nBase* g() { return (Base*)p->newInstance(); }\n");
    auto r = rf::run(to_py(cb::fs::to_text(tmp.path())), std::optional<py::list<py::str>>(py::list<py::str>{py::str("addMapping")}));
    CB_CHECK_EQ(r.total_sites, 1);
    CB_CHECK_EQ(r.total_classes, 1);
    CB_CHECK_EQ(from_py(r.registries[0].registry), "T");
    CB_CHECK(cb::fs::read_text(tmp.path() / "T.cpp").find("T_createByReflection(p)") != std::string::npos);
    auto [text, sites, fac] = rf::process(py::str("x"), py::str("X"));
    CB_CHECK_EQ(sites, 0);
    CB_CHECK(!fac.has_value());
});

// ---- java/console: the shims over cb/java/console ------------------------------------------------------------------

std::string console_data(const std::string& relative) {
    return cb::fs::to_text(cb::testing::data_path("java/console/testdata/" + relative));
}

CB_TEST("java.shims.console_scan_and_sdk", [] {
    namespace con = jv::console;
    // (the old settings layer wants to know the checkout; no settings.json there is no settings)
    cb::fs::ScopedEnv checkout("CB_CHECKOUT_ROOT", cb::fs::to_text(cb::testing::data_path("../..")));
    // xdk_scan: the old record shapes, with kind/severity/effort as the text the old callers read
    auto scan = con::xdk_scan::detect(std::optional<py::str>(to_py(console_data("xdk/sdk1"))));
    CB_CHECK_EQ(scan.found.size(), 13);
    CB_CHECK_EQ(scan.missing.size(), 0);
    CB_CHECK_EQ(from_py(scan.found[0].needs), "XContentCreate");
    CB_CHECK_EQ(from_py(scan.found[0].kind), "requirement");
    CB_CHECK_EQ(from_py(*scan.found[0].sev), "correctness");
    CB_CHECK(!scan.found[0].effort.has_value());
    CB_CHECK_EQ(from_py(*scan.found[6].effort), "S");
    CB_CHECK_EQ(scan.found[0].func_count, 3);
    CB_CHECK_EQ(from_py(scan.found[0].header), "xam.h");
    CB_CHECK_EQ(from_py(scan.funcs.at(py::str("XamLoaderLaunchTitle"))), "xam.h");
    CB_CHECK(!con::xdk_scan::detect(std::nullopt).include.has_value());
    CB_CHECK_EQ(con::xdk_scan::PATTERNS().size(), 13);

    // sdk: the gate, with the old exception
    CB_CHECK(con::sdk::requires_sdk(std::optional<py::str>(py::str("xbox360"))));
    CB_CHECK(!con::sdk::requires_sdk(std::nullopt) && con::sdk::is_ungated(std::optional<py::str>(py::str("pc"))));
    cb::fs::ScopedTempDir tmp("cbshim");
    cb::fs::write_text(tmp.path() / "xdk" / "include" / "xbox" / "xtl.h", "void XamShimSymbol();\n");
    auto info = con::sdk::detect(std::optional<py::str>(py::str("xbox360")), std::optional<py::str>(to_py(cb::fs::to_text(tmp.path() / "xdk"))));
    CB_CHECK(info.has_value());
    CB_CHECK_EQ(from_py(info->source), "override");
    CB_CHECK_EQ(from_py(info->name), "Xbox 360 SDK (XDK)");
    CB_CHECK(info->has_symbol(py::str("XamShimSymbol")) && !info->has_symbol(py::str("XamOther")));
    CB_CHECK_EQ(info->include_dirs.size(), 2);
    const std::optional<py::str> nowhere(to_py(cb::fs::to_text(tmp.path() / "empty")));
    CB_CHECK_THROWS_AS(con::sdk::SdkNotFound, (void)con::sdk::require(std::optional<py::str>(py::str("xbox360")), nowhere));
    CB_CHECK(from_py(con::sdk::describe(std::optional<py::str>(py::str("wii")), nowhere)).find("NOT found") != std::string::npos);

    // project_scan
    auto r = con::project_scan::resolve(to_py(console_data("scan/proj1")));
    CB_CHECK(r.jar.has_value() && from_py(*r.jar).ends_with("deobfuscated.jar"));
    CB_CHECK(r.lang.has_value() && !r.base.empty());
    CB_CHECK(!con::project_scan::find_jar(to_py(cb::fs::to_text(tmp.path() / "empty"))).has_value());
});

CB_TEST("java.shims.console_assets_and_banks", [] {
    namespace con = jv::console;
    // assets: the png codec behind the old names, and the old exception for a bad one
    const auto raw = cb::fs::read_file(cb::testing::data_path("java/console/testdata/assets/png/pal4.png"));
    CB_CHECK(con::assets::png_needs_rgba(to_py(cb::ByteView(raw))));
    CB_CHECK_EQ(con::assets::_png_colortype(to_py(cb::ByteView(raw))).value(), 3);
    auto [converted, changed] = con::assets::normalize_png_to_rgba(to_py(cb::ByteView(raw)));
    CB_CHECK(changed && !con::assets::png_needs_rgba(converted));
    const auto cut = cb::fs::read_file(cb::testing::data_path("java/console/testdata/assets/png/bad_cut.png"));
    CB_CHECK_THROWS_AS(con::assets::UnidentifiedImageError, (void)con::assets::normalize_png_to_rgba(to_py(cb::ByteView(cut))));
    cb::fs::ScopedTempDir tmp("cbshim");
    auto res = con::assets::run(to_py(console_data("assets/drift.jar")), to_py(cb::fs::to_text(tmp.path() / "Content")),
                                std::optional<py::str>(to_py(console_data("assets/code_drift"))), true);
    CB_CHECK_EQ(res.written.size(), 2);
    CB_CHECK_EQ(res.converted.size(), 1);
    CB_CHECK_EQ(res.unconverted.size(), 0);
    CB_CHECK(res.coverage.has_value() && res.coverage->missing.size() == 1);
    CB_CHECK(res.report.has_value());

    // xact: dump and the ffmpeg script through the old signatures
    auto text = con::xact::dump(to_py(console_data("xact/sounds.xsb")));
    CB_CHECK(from_py(text).starts_with("SoundBank \"sounds\""));
    con::xact::Match m;
    m.matched[py::str("step_grass")] = con::xact::Matched{py::str("step.grass"), py::list<py::str>{py::str("newsound/step/grass1.ogg")}};
    auto bat = con::xact::write_ffmpeg_script(m, to_py(cb::fs::to_text(tmp.path() / "audio")), to_py(console_data("xact/res")));
    CB_CHECK(cb::fs::read_text(cb::fs::from_text(from_py(bat))).find("step_grass.wav") != std::string::npos);
});

CB_TEST("java.shims.console_project_files", [] {
    namespace con = jv::console;
    cb::fs::ScopedTempDir tmp("cbshim");
    cb::fs::copy_tree(cb::testing::data_path("java/console/testdata/layout/in_flat"), tmp.path() / "in");
    con::project_layout::set_here(to_py(cb::fs::to_text(cb::testing::data_path("../../codebreaker/java/console"))));
    py::list<std::tuple<py::str, py::str>> rules;
    rules.append(std::make_tuple(py::str("^Block$"), py::str("Blocks")));
    con::vcxproj::_reseed(rules);
    auto made = con::project_layout::layout(to_py(cb::fs::to_text(tmp.path() / "in" / "Game")));
    CB_CHECK_EQ(from_py(made.name), "Game");
    CB_CHECK_EQ(made.moved, 8);
    CB_CHECK(cb::fs::read_text(cb::fs::from_text(from_py(made.sln))).find("EndProject") != std::string::npos);
    CB_CHECK(cb::fs::read_text(cb::fs::from_text(from_py(made.vcxproj))).find("ProjectGuid") != std::string::npos);
    CB_CHECK(cb::fs::read_text(tmp.path() / "in" / "Game" / "Game" / "Game.vcxproj.filters").find("Blocks") != std::string::npos);
    con::vcxproj::_reseed(py::list<std::tuple<py::str, py::str>>{});
    // worklist: a compiler that is the stand-in of the tests
    cb::fs::write_text(tmp.path() / "wl" / "x.cpp", "x.cpp:1: error: boom\n");
    auto code = con::worklist::run(to_py(cb::fs::to_text(tmp.path() / "wl")), std::optional<py::str>(to_py(console_data("worklist/fakecc.cmd"))),
                                   py::str("c++03"), std::optional<py::str>(to_py(cb::fs::to_text(tmp.path() / "WL.md"))));
    CB_CHECK_EQ(code, 0);
    CB_CHECK(cb::fs::read_text(tmp.path() / "WL.md").find("| 1 | x.cpp | boom |") != std::string::npos);
});

}  // namespace
