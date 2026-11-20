// Checks whose literals no other native test spells: the destructor the emitter writes for owned members (it frees scalars and
// containers, not borrowed ones) and the GL11 / NVFogDistance stubs.
#include <string>

#include "cb/core/testing.hpp"
#include "cb/java/corpus.hpp"
#include "cb/java/emit/source.hpp"
#include "cb/java/parse.hpp"
#include "cb/java/runtime.hpp"
#include "cb/java/typemap.hpp"
#include "cb/java/types.hpp"

namespace {

bool bfp_has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

const cb::java::runtime::Runtime& bfp_runtime() {
    static const cb::java::runtime::Runtime r{cb::java::JavaData(cb::testing::data_path("../../config/java"))};
    return r;
}

}  // namespace

CB_TEST("java.beta_fixes.the_destructor_frees_scalars_and_containers_it_owns", [] {
    const cb::java::ir::JClass cls = cb::java::parse::parse_source(
        "package s;\nimport java.util.List;\npublic class Game {\n"
        "  Level level; Timer timer; List<Zombie> zombies;\n"
        "  public Game() { this.timer = new Timer(); }\n"
        "  public void init() { this.level = new Level(); this.zombies.add(new Zombie()); }\n"
        "}\n");
    cb::java::types::ResolverData data;
    data.known = {"Game", "Level", "Timer", "Zombie"};
    const cb::java::types::TypeResolver resolver(std::move(data));
    const auto rendered = cb::java::emit::source::render_source(cls, resolver);
    CB_CHECK(rendered.text.has_value());
    const std::string cpp = rendered.text.value_or("");
    CB_CHECK(bfp_has(cpp, "delete timer;"));
    CB_CHECK(bfp_has(cpp, "delete level;"));
    CB_CHECK(bfp_has(cpp, "delete zombies[_i];"));
});

CB_TEST("java.beta_fixes.the_gl11_stub_has_the_constants_the_corpus_uses", [] {
    const std::string& gl11 = bfp_runtime().files().at("GL11.h");
    for (const char* name : {"GL_BYTE", "GL_TRIANGLES", "GL_NORMAL_ARRAY", "GL_NEAREST_MIPMAP_LINEAR", "GL_TEXTURE", "GL_ZERO", "GL_GEQUAL", "GL_UNPACK_ALIGNMENT"})
        CB_CHECK(bfp_has(gl11, std::string("const int ") + name));
});

CB_TEST("java.beta_fixes.the_gl11_stub_has_the_functions_the_corpus_uses", [] {
    const std::string& gl11 = bfp_runtime().files().at("GL11.h");
    for (const char* fn : {"glNormalPointer", "glScaled"}) CB_CHECK(bfp_has(gl11, std::string("void ") + fn + "("));
});

// ---- the AWT stubs: the AWT types Indev's MouseHelper uses get no-op stubs, so the reachable game code compiles on the console -------

CB_TEST("java.awt_stubs.the_mouse_stubs_are_generated_and_registered", [] {
    const auto& files = bfp_runtime().files();
    for (const char* name : {"Point.h", "MouseInfo.h", "PointerInfo.h", "Dialog.h"}) {
        CB_CHECK(files.contains(name));
        if (files.contains(name)) CB_CHECK(bfp_has(files.at(name), "EXTERNAL-LIBRARY STUB"));
    }
    const std::string& point = files.at("Point.h");
    CB_CHECK(bfp_has(point, "int x;") && bfp_has(point, "int y;"));
    const std::string& pointer_info = files.at("PointerInfo.h");
    CB_CHECK(bfp_has(pointer_info, "Point* getLocation()"));
    CB_CHECK(bfp_has(pointer_info, "return &"));              // a valid (static) Point, never NULL: no crash on a dereference
    const std::string& mouse_info = files.at("MouseInfo.h");
    CB_CHECK(bfp_has(mouse_info, "namespace MouseInfo"));
    CB_CHECK(bfp_has(mouse_info, "PointerInfo getPointerInfo"));
    CB_CHECK(bfp_has(files.at("Component.h"), "Point* getLocationOnScreen()"));   // it was void*: it could not be assigned to a Point*
    for (const char* name : {"Point", "PointerInfo", "Dialog", "MouseInfo"}) CB_CHECK(cb::java::typemap::runtime_headers().contains(name));
    CB_CHECK(cb::java::typemap::runtime_static_holders().contains("MouseInfo"));   // MouseInfo.getPointerInfo() is a static call
});

// ---- the runtime stubs ---------------------------------------------------------------------------------------------------------------

CB_TEST("java.runtime_stubs.jmap_has_the_java_map_surface_and_list_and_set_an_iterator_member", [] {
    const auto& files = bfp_runtime().files();
    CB_CHECK(files.contains("JMap.h") && files.contains("JIterator.h"));
    const std::string& jmap = files.at("JMap.h");
    CB_CHECK(bfp_has(jmap, "class JMap : public std::map<K, V>"));
    for (const char* member : {"put", "get", "containsKey", "remove", "isEmpty", "values", "keySet", "entrySet", "operator==", "operator!="}) CB_CHECK(bfp_has(jmap, member));
    // chained and temporary receivers cannot be reached by a text rewrite: JList and JSet carry a real iterator() member
    CB_CHECK(bfp_has(files.at("JList.h"), "Iterator* iterator()"));
    CB_CHECK(bfp_has(files.at("JSet.h"), "Iterator* iterator()"));
});

CB_TEST("java.runtime_stubs.a_corpus_class_inherits_the_methods_of_a_stub_base", [] {
    std::vector<cb::java::ir::JClass> classes;
    classes.push_back(cb::java::parse::parse_source("public class MyApplet extends Applet { }"));
    const auto env = cb::java::corpus::build_member_env(classes, bfp_runtime());
    const auto it = env.find("MyApplet");
    CB_CHECK(it != env.end());
    if (it == env.end()) return;
    const auto method = it->second.methods.find("getDocumentBase");
    CB_CHECK(method != it->second.methods.end());
    if (method != it->second.methods.end()) CB_CHECK_EQ(method->second.name, "URL");
});

CB_TEST("java.beta_fixes.nvfogdistance_is_a_static_holder_with_its_constants", [] {
    const std::string& nv = bfp_runtime().files().at("NVFogDistance.h");
    CB_CHECK(bfp_has(nv, "namespace NVFogDistance"));
    CB_CHECK(bfp_has(nv, "GL_FOG_DISTANCE_MODE_NV"));
    CB_CHECK(bfp_has(nv, "GL_EYE_RADIAL_NV"));
    CB_CHECK(cb::java::typemap::runtime_static_holders().contains("NVFogDistance"));
    CB_CHECK(cb::java::typemap::runtime_enum_like().contains("NVFogDistance"));
});
