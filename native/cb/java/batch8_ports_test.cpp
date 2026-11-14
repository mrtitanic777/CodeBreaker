// Ports of the tests/java cases that had no native twin by name and whose behaviour the existing native tests did not check literally
// (batch 8, before the Python Java world was deleted). Each test names the Python case it carries. Same inputs, same expectations.
// cb-unity: alone
#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include "cb/core/testing.hpp"
#include "cb/java/corpus.hpp"
#include "cb/java/ir.hpp"
#include "cb/java/parse.hpp"
#include "cb/java/probe.hpp"
#include "cb/java/projects/minecraft/controller_nav.hpp"
#include "cb/java/projects/minecraft/edition.hpp"
#include "cb/java/reflection_factory.hpp"
#include "cb/java/rules.hpp"
#include "cb/java/runtime.hpp"
#include "cb/java/types.hpp"

namespace {

namespace b8r = cb::java::rules;
namespace b8cn = cb::java::projects::minecraft::controller_nav;
namespace b8ed = cb::java::projects::minecraft::edition;

bool b8_has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

b8r::Findings b8_fire(std::string_view rule, std::string_view name, std::string_view text,
                      std::optional<std::string_view> header = std::nullopt, const b8r::Context& ctx = b8r::Context::make("xbox360")) {
    return b8r::check(rule, b8r::FileInput{name, text, header}, ctx);
}

const cb::java::runtime::Runtime& b8_runtime() {
    static const cb::java::runtime::Runtime r{cb::java::JavaData(cb::testing::data_path("../../data/java"))};
    return r;
}

}  // namespace

// test_beta_fixes TestSideEffectInBranchRule: test_pure_branch_not_flagged, test_nested_early_return_guard_not_flagged
CB_TEST("java.batch8.side_effect_in_branch_pure_and_nested_guards", [] {
    // a branch that only returns a constant has no side effect
    CB_CHECK(b8_fire("side-effect-in-branch", "R.cpp", "bool R::a() {\n  if (cond) {\n    return true;\n  }\n  return false;\n}\n").empty());
    // `if (flg & 4) { if (p > n) return -1; p += 2; }`: reducing `return -1;` must not leave an `if (...) ;` that reads as an impure call
    CB_CHECK(b8_fire("side-effect-in-branch", "R.cpp", "int R::a() {\n  if (flg & 4) { if (p > n) return -1; p += 2; }\n  return p;\n}\n").empty());
    // the genuine one still flags (test_real_side_effect_assignment_flagged, with the Python's own text)
    CB_CHECK_EQ(b8_fire("side-effect-in-branch", "R.cpp", "void R::a() {\n  if (isHit) {\n    this->hit = new HitResult(1);\n    return;\n  }\n}\n").size(),
                static_cast<std::size_t>(1));
});

// test_beta_fixes TestSilentDispatchCommentChain.test_interior_comment_does_not_truncate_chain
CB_TEST("java.batch8.silent_dispatch_with_a_comment_and_a_terminal", [] {
    // a comment between two else-ifs must NOT hide the terminal J2X_UNHANDLED (the glEnableClientState false positive)
    const std::string cpp = "void f(int c) {\n  if (c == GL_A) x=1; else if (c == GL_B) x=2;\n  // an explanatory comment mid-chain\n"
                            "  else if (c == GL_C) x=3;\n  else J2X_UNHANDLED(\"f\", c);\n}\n";
    CB_CHECK(b8_fire("silent-dispatch", "f.cpp", cpp).empty());
    const std::string silent = "void f(int c) {\n  if (c == GL_A) x=1; else if (c == GL_B) x=2;\n  else if (c == GL_C) x=3;\n}\n";
    CB_CHECK_EQ(b8_fire("silent-dispatch", "f.cpp", silent).size(), static_cast<std::size_t>(1));
});

// test_beta_fixes TestVirtualCallInCtorRule: flagged when the hierarchy is unknown, not flagged when the whole tree is known and nobody overrides
CB_TEST("java.batch8.virtual_call_in_ctor_with_a_known_hierarchy", [] {
    const std::string hdr = "class Foo {\npublic:\n\tFoo();\n\tvirtual ~Foo();\n\tvirtual void init();\n};\n";
    const std::string cpp = "Foo::Foo() {\n\tthis->init();\n}\n";
    CB_CHECK_EQ(b8_fire("virtual-call-in-ctor", "Foo.cpp", cpp, hdr).size(), static_cast<std::size_t>(1));   // per-file: override_pairs unknown, stay conservative
    b8r::Context known = b8r::Context::make("xbox360");
    known.override_pairs = b8r::ClassMethodPairs{};   // the whole tree is known and nobody overrides Foo::init
    CB_CHECK(b8_fire("virtual-call-in-ctor", "Foo.cpp", cpp, hdr, known).empty());
    known.override_pairs = b8r::ClassMethodPairs{{"Foo", "init"}};
    CB_CHECK_EQ(b8_fire("virtual-call-in-ctor", "Foo.cpp", cpp, hdr, known).size(), static_cast<std::size_t>(1));
});

// test_beta_fixes TestDerivedNullability (test_return_null_getter_is_derived, test_value_return_null_not_derived)
CB_TEST("java.batch8.nullability_is_derived_for_references_only", [] {
    namespace cor = cb::java::corpus;
    std::vector<cb::java::ir::JClass> classes;
    classes.push_back(cb::java::parse::parse_source(
        "package sample;\npublic class Inv {\n  public ItemStack getStackInSlot(int var1) {\n    if(var1 < 0) { return null; }\n    return this.contents;\n  }\n"
        "  public int size() { return 0; }\n}\n"));
    classes.push_back(cb::java::parse::parse_source("package sample;\npublic class S {\n  public String name() { return null; }\n}\n"));
    const cb::java::types::TypeResolver resolver = cor::build_resolver(classes);
    const cor::Nullability n = cor::derive_nullability(classes, resolver);
    CB_CHECK(n.getters.contains("getStackInSlot"));            // a reference return and `return null`: nullable
    CB_CHECK_EQ(n.return_types.at("getStackInSlot"), "ItemStack");
    CB_CHECK(!n.getters.contains("size"));                     // an int cannot form getter()->deref
    CB_CHECK(!n.getters.contains("name"));                     // String is a value type here
});

// test_controller_nav test_guiscreen_cpp_gets_ctor_init_and_body, test_pointer_draw_injected_after_drawscreen
CB_TEST("java.batch8.controller_nav_bodies", [] {
    const std::string gui = "#include \"GuiScreen.h\"\nGuiScreen::GuiScreen()\n\t: mc(NULL)\n\t, width(0)\n\t, height(0)\n\t, allowUserInput(0)\n{\n}\n";
    const b8cn::Patched g = b8cn::patch_guiscreen_cpp(gui);
    CB_CHECK(g.ok);
    CB_CHECK(b8_has(g.text, ", navSelected(-1)"));
    CB_CHECK(b8_has(g.text, "void GuiScreen::handleControllerMenuNav() {"));
    CB_CHECK(b8_has(g.text, "Input::menuNavSelect(") && b8_has(g.text, "Input::menuSnapCursor("));
    CB_CHECK(b8_has(g.text, "#include \"Input.h\""));   // the adapter needs the core's header
    CB_CHECK(g.text.find(", navSelected(-1)") < g.text.find("\n{"));   // the init lands inside the init list, before the body brace

    const std::string er = "#include \"EntityRenderer.h\"\n#include \"Input.h\"\nvoid EntityRenderer::updateCameraAndRender(float var1) {\n"
                           "\td3d9::glClear(0);\n\tthis->mc->currentScreen->drawScreen(var5, var6, var1);\n}\n";
    const b8cn::Patched p = b8cn::patch_entityrenderer_pointer(er);
    CB_CHECK(p.ok);
    CB_CHECK(b8_has(p.text, "Pointer.png") && b8_has(p.text, "g_guiCursorMode"));
    CB_CHECK(b8_has(p.text, "d3d9::glBindTexture") && !b8_has(p.text, "GL11::glBindTexture"));
    CB_CHECK(p.text.find("Pointer.png") > p.text.find("->drawScreen("));   // drawn after the screen renders
});

// test_probe_objects test_cases_never_list_every_combination
CB_TEST("java.batch8.probe_cases_never_list_every_combination", [] {
    const auto many = cb::java::probe::cases({"double", "double", "double", "double", "double", "double", "double", "double"});
    CB_CHECK(!many.empty() && many.size() <= 48 + 8 * 12);   // 12^8 combinations exist
    std::set<std::vector<std::string>> distinct(many.begin(), many.end());
    CB_CHECK_EQ(distinct.size(), many.size());
});

// test_reflection_factory test_dynamic_getclass_not_registered
CB_TEST("java.batch8.a_dynamic_getclass_key_is_not_a_registration", [] {
    namespace rf = cb::java::reflection_factory;
    const std::string noarg =
        "#include \"TileEntity.h\"\n\nTileEntity* TileEntity::createAndLoadEntity(NBTTagCompound* var0)\n{\n\tTileEntity* var1 = NULL;\n"
        "\tClass* var2 = (Class*)s_nameToClassMap[var0->getString(\"id\")];\n\tvar1 = (TileEntity*)var2->newInstance();\n\treturn var1;\n}\n\n"
        "void TileEntity::initStatics()\n{\n\taddMapping(Class(typeid(TileEntityFurnace)), \"Furnace\");\n\taddMapping(Class(typeid(TileEntityChest)), \"Chest\");\n}\n";
    const rf::Registrars reg{{"addMapping"}};
    const auto plain = rf::process(noarg, "TileEntity", reg);
    CB_CHECK(plain.factory.has_value());
    // Class(typeid(*(this))) is a getClass() key lookup, not a registration
    const auto dynamic = rf::process(noarg + "\nJavaString n = s_map[Class(typeid(*(this)))];\n", "TileEntity", reg);
    CB_CHECK(dynamic.factory.has_value());
    if (dynamic.factory) {
        const auto& classes = dynamic.factory->classes;
        CB_CHECK(std::find(classes.begin(), classes.end(), "this") == classes.end());
        CB_CHECK(classes == (std::vector<std::string>{"TileEntityFurnace", "TileEntityChest"}));
    }
});

// test_runnable_entry test_glcontext_stub_has_gl3_caps; test_runtime_stubs (Entry, the ambiguous returns, Runtime not a stub)
CB_TEST("java.batch8.runtime_stub_texts", [] {
    const auto& files = b8_runtime().files();
    CB_CHECK(files.contains("GLContext.h"));
    if (files.contains("GLContext.h")) {
        const std::string& gl = files.at("GLContext.h");
        CB_CHECK(b8_has(gl, "OpenGL30") && b8_has(gl, "OpenGL32") && b8_has(gl, "GL_ARB_compatibility"));
    }
    CB_CHECK(files.contains("Entry.h"));
    if (files.contains("Entry.h")) {
        const std::string& e = files.at("Entry.h");
        CB_CHECK(b8_has(e, "Entry(void* k, void* v)") && b8_has(e, "getKey") && b8_has(e, "getValue"));
    }
    const auto& members = b8_runtime().stub_members();
    CB_CHECK(!members.contains("Runtime"));                                          // Runtime has a real baked implementation: not a stub
    CB_CHECK(!members.at("Toolkit").methods.contains("getSystemClipboard"));         // void* is not a usable pointee
});

// test_types test_unknown_capitalized_is_pointer
CB_TEST("java.batch8.an_unknown_capitalized_class_is_a_reference", [] {
    const cb::java::types::TypeResolver r{cb::java::types::ResolverData{}};
    cb::java::ir::JType t;
    t.name = "SomeNewClass";
    CB_CHECK(r.is_pointer(t));
    CB_CHECK_EQ(r.field_type(t), "SomeNewClass*");
});

// test_review_fixes test_beta_only_class_still_detects_beta
CB_TEST("java.batch8.a_class_only_beta_has_still_detects_beta", [] {
    std::vector<cb::java::ir::JClass> classes(3);
    const char* names[] = {"World", "Chunk", "BlockBed"};
    for (std::size_t i = 0; i < 3; ++i) {
        classes[i].name = names[i];
        classes[i].package = "net.minecraft.src";
    }
    const auto idx = b8ed::CorpusIndex::of(classes);
    CB_CHECK(b8ed::detect_edition(idx) == b8ed::Edition::Beta);
});

// tests/test_port_fixes.py test_java_rules_findings_of_one_line_come_in_name_order: where several names of a set match one line, the findings
// come in the sorted order of the names, not in a hash order (the Python's bug)
CB_TEST("java.batch8.findings_of_one_line_come_in_name_order", [] {
    const std::string header = "class Foo {\npublic:\n\tFoo();\n\tvirtual void zeta();\n\tvirtual void alpha();\n\tvirtual void mid();\n};\n";
    const auto v = b8_fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo()\n{\n\tzeta();\n\talpha();\n\tmid();\n}\n", header);
    CB_CHECK_EQ(v.size(), static_cast<std::size_t>(3));
    if (v.size() == 3) {
        CB_CHECK(b8_has(v[0].message, "alpha") && b8_has(v[1].message, "mid") && b8_has(v[2].message, "zeta"));
    }
    // one finding a line: the first by name
    const auto m = b8_fire("map-read-insert", "M.cpp",
                           "std::map<int,int> zz;\nstd::map<int,int> mm;\nstd::map<int,int> aa;\nint f() {\n\treturn zz[1] + aa[2] + mm[3];\n}\n");
    CB_CHECK_EQ(m.size(), static_cast<std::size_t>(1));
    if (!m.empty()) CB_CHECK(b8_has(m[0].message, "aa["));
    const auto l = b8_fire("leak-drop-no-delete", "L.cpp",
                           "std::vector<Foo*> zs;\nstd::vector<Foo*> as;\nstd::vector<Foo*> ms;\nvoid f() {\n\tzs.clear(); as.clear(); ms.pop_back();\n}\n");
    CB_CHECK_EQ(l.size(), static_cast<std::size_t>(1));
    if (!l.empty()) CB_CHECK(b8_has(l[0].message, "as."));
});
