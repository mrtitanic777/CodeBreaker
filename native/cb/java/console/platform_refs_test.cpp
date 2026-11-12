// platform_refs: the generic entry points, the detection of the game's main class, and the emission of the real
// hand-written platform layer from codebreaker/java/platform/{reference,companions} (the real files, read in place).
#include <set>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/console/platform_refs.hpp"
#include "cb/java/ir.hpp"

namespace {

using namespace cb;
namespace pr = cb::java::console::platform_refs;
namespace jir = cb::java::ir;
namespace stdfs = std::filesystem;

stdfs::path refs_real_reference() { return testing::data_path("../../codebreaker/java/platform/reference"); }

bool refs_has(const std::string& text, std::string_view needle) { return text.find(needle) != std::string::npos; }

jir::JClass refs_class(const std::string& name) {
    jir::JClass c;
    c.name = name;
    return c;
}

jir::Method refs_method(const std::string& name, bool is_static, std::vector<jir::JType> param_types, bool ctor = false) {
    jir::Method m;
    m.name = name;
    m.is_static = is_static;
    m.is_ctor = ctor;
    for (jir::JType& t : param_types) {
        jir::Param p;
        p.type = std::move(t);
        m.params.push_back(std::move(p));
    }
    return m;
}

jir::JType refs_type(const std::string& name, std::uint32_t dims = 0) {
    jir::JType t;
    t.name = name;
    t.array_dims = dims;
    return t;
}

jir::JClass refs_main_class(const std::string& name) {
    jir::JClass c = refs_class(name);
    c.methods.push_back(refs_method("main", true, {refs_type("String", 1)}));
    return c;
}

jir::JClass refs_runnable(const std::string& name) {
    jir::JClass c = refs_class(name);
    c.interfaces = {"Runnable"};
    c.methods.push_back(refs_method("run", false, {}));
    return c;
}

CB_TEST("java.platform_refs.detect_main_class", [] {
    const std::vector<jir::JClass> classes = {refs_class("Level"), refs_main_class("RubyDung")};
    CB_CHECK(pr::detect_main_class(classes) == "RubyDung");
    CB_CHECK(!pr::detect_main_class(std::vector<jir::JClass>{refs_class("Level")}));
    CB_CHECK(!pr::detect_main_class(std::vector<jir::JClass>{}));
    // an instance main is not an entry point, nor one that takes something other than String[]
    jir::JClass instance = refs_class("X");
    instance.methods.push_back(refs_method("main", false, {refs_type("String", 1)}));
    jir::JClass wrong = refs_class("Y");
    wrong.methods.push_back(refs_method("main", true, {refs_type("String")}));
    jir::JClass two_params = refs_class("Z");
    two_params.methods.push_back(refs_method("main", true, {refs_type("String", 1), refs_type("int")}));
    CB_CHECK(!pr::detect_main_class(std::vector<jir::JClass>{instance, wrong, two_params}));
    // the qualified name counts
    jir::JClass qualified = refs_class("Q");
    qualified.methods.push_back(refs_method("main", true, {refs_type("java.lang.String", 1)}));
    CB_CHECK(pr::detect_main_class(std::vector<jir::JClass>{qualified}) == "Q");
    // the first class with a main wins
    CB_CHECK(pr::detect_main_class(std::vector<jir::JClass>{refs_main_class("A"), refs_main_class("B")}) == "A");
});

CB_TEST("java.platform_refs.detect_runnable_game_class", [] {
    // Indev: a concrete top-level Runnable shell
    std::vector<jir::JClass> indev = {refs_class("Level"), refs_runnable("Minecraft")};
    const jir::JClass* found = pr::detect_runnable_game_class(indev);
    CB_CHECK(found == &indev[1]);
    // Alpha/Beta: Minecraft is abstract and MinecraftImpl has a parent
    jir::JClass abstract_shell = refs_runnable("Minecraft");
    abstract_shell.is_abstract = true;
    jir::JClass impl = refs_runnable("MinecraftImpl");
    impl.parent = "Minecraft";
    CB_CHECK(pr::detect_runnable_game_class(std::vector<jir::JClass>{abstract_shell, impl}) == nullptr);
    // a UI Runnable has a superclass
    jir::JClass gui = refs_runnable("GuiLoadLevel");
    gui.parent = "GuiScreen";
    CB_CHECK(pr::detect_runnable_game_class(std::vector<jir::JClass>{gui}) == nullptr);
    // `extends Object` is no superclass; an interface and a Runnable without run() do not count
    jir::JClass object_parent = refs_runnable("Shell");
    object_parent.parent = "Object";
    CB_CHECK(pr::detect_runnable_game_class(std::vector<jir::JClass>{object_parent}) != nullptr);
    jir::JClass iface = refs_runnable("I");
    iface.is_interface = true;
    jir::JClass no_run = refs_class("N");
    no_run.interfaces = {"Runnable"};
    CB_CHECK(pr::detect_runnable_game_class(std::vector<jir::JClass>{iface, no_run}) == nullptr);
    // two candidates are ambiguous
    CB_CHECK(pr::detect_runnable_game_class(std::vector<jir::JClass>{refs_runnable("A"), refs_runnable("B")}) == nullptr);
});

CB_TEST("java.platform_refs.entry_points_name_the_detected_class", [] {
    const std::string x360 = pr::generate_entry_point("RubyDung");
    CB_CHECK(refs_has(x360, "RubyDung::main(args);"));
    CB_CHECK(refs_has(x360, "#include \"RubyDung.h\""));
    CB_CHECK(refs_has(x360, "(Detected main class: RubyDung."));
    CB_CHECK(refs_has(x360, "j2x_initAll();"));
    CB_CHECK(refs_has(x360, "Display::create();"));
    CB_CHECK(refs_has(x360, "DbgPrintF(\"[J2X] static init done; calling game main\\n\");"));   // a C escape, not a newline
    CB_CHECK(!refs_has(x360, "Minecraft"));   // no game hardcoding
    CB_CHECK(!refs_has(x360, "@MAIN@"));

    const std::string pc = pr::generate_pc_entry_point("Game");
    CB_CHECK(refs_has(pc, "Game::main(args);"));
    CB_CHECK(refs_has(pc, "#include \"Game.h\""));
    CB_CHECK(refs_has(pc, "j2xThreadsWait();"));
    CB_CHECK(refs_has(pc, "SDL_SetMainReady();"));
    CB_CHECK(!refs_has(pc, "@MAIN@"));
});

CB_TEST("java.platform_refs.runnable_entry_constructs_with_default_arguments", [] {
    jir::JClass shell = refs_runnable("Minecraft");
    // two constructors: the widest one is used
    shell.methods.push_back(refs_method("Minecraft", false, {refs_type("int")}, true));
    shell.methods.push_back(refs_method("Minecraft", false,
                                        {refs_type("Canvas"), refs_type("MinecraftApplet"), refs_type("int"), refs_type("int"),
                                         refs_type("boolean"), refs_type("float"), refs_type("int"), refs_type("int", 1)},
                                        true));
    CB_CHECK_EQ(pr::runnable_ctor_args(shell), "NULL, NULL, SCREEN_WIDTH, SCREEN_HEIGHT, false, 0, 0, NULL");
    const std::string entry = pr::generate_runnable_entry_point(shell);
    CB_CHECK(refs_has(entry, "Minecraft* _game = new Minecraft(NULL, NULL, SCREEN_WIDTH, SCREEN_HEIGHT, false, 0, 0, NULL);"));
    CB_CHECK(refs_has(entry, "_game->run();"));
    CB_CHECK(refs_has(entry, "#include \"Minecraft.h\""));
    CB_CHECK(refs_has(entry, "constructing Minecraft\\n"));
    CB_CHECK(!refs_has(entry, "@NAME@") && !refs_has(entry, "@ARGS@"));
    // no constructor: no arguments
    CB_CHECK_EQ(pr::runnable_ctor_args(refs_runnable("Bare")), "");
    CB_CHECK(refs_has(pr::generate_runnable_entry_point(refs_runnable("Bare")), "new Bare();"));
});

CB_TEST("java.platform_refs.platform_names", [] {
    CB_CHECK(pr::platform_of("pc_opengl") == pr::Platform::PcOpenGl);
    CB_CHECK(pr::platform_of("xbox360") == pr::Platform::Xbox360);
    CB_CHECK(pr::platform_of("") == pr::Platform::Xbox360);
    CB_CHECK(pr::platform_of("anything") == pr::Platform::Xbox360);
});

CB_TEST("java.platform_refs.tables", [] {
    CB_CHECK_EQ(pr::reference_skip().size(), std::size_t{1});
    CB_CHECK(pr::reference_skip().contains("ZipFile"));
    const auto& repl = pr::reference_replaces();
    CB_CHECK_EQ(repl.size(), std::size_t{4});
    CB_CHECK((repl.at("Input") == std::vector<std::string>{"Keyboard", "Mouse", "Controllers"}));
    CB_CHECK((repl.at("Audio") == std::vector<std::string>{"SoundSystem"}));
    CB_CHECK((repl.at("NIOBuffers") == std::vector<std::string>{"ByteBuffer", "FloatBuffer", "IntBuffer"}));
});

CB_TEST("java.platform_refs.real_reference_layer_is_complete", [] {
    const stdfs::path dir = refs_real_reference();
    const std::set<std::string> bases = pr::available_references(dir);
    CB_CHECK(bases.contains("GL11") && bases.contains("Audio") && bases.contains("Input") && bases.contains("ZipFile"));
    CB_CHECK(!bases.contains("Main"));   // Main.reference.cpp has no header
    CB_CHECK_EQ(bases.size(), std::size_t{20});
    // every reference that has a .cpp has its .h too (the GLU flat-world trap): nothing is orphaned
    const pr::Audit audit = pr::audit_references(dir);
    CB_CHECK(audit.orphans.empty());
    CB_CHECK(audit.complete == bases);
    CB_CHECK(pr::available_references(dir / "missing").empty());
    CB_CHECK(pr::audit_references(dir / "missing").complete.empty());
});

CB_TEST("java.platform_refs.audit_names_a_cpp_without_a_header", [] {
    fs::ScopedTempDir dir("cb-refs");
    const stdfs::path ref = dir.path() / "reference";
    fs::make_dirs(ref);
    fs::make_dirs(dir.path() / "companions");
    fs::write_text(ref / "GL11.reference.h", "h");
    fs::write_text(ref / "GL11.reference.cpp", "c");
    fs::write_text(ref / "GLU.reference.cpp", "c");        // an orphan: no GLU.reference.h
    fs::write_text(ref / "Main.reference.cpp", "c");       // legitimately cpp-only
    fs::write_text(ref / "Textures.ext.reference.cpp", "c");   // an extension
    fs::write_text(ref / "Textures.reference.cpp", "c");   // its header ships as a companion
    fs::write_text(dir.path() / "companions" / "Textures.h", "h");
    const pr::Audit audit = pr::audit_references(ref);
    CB_CHECK((audit.orphans == std::vector<std::string>{"GLU"}));
    CB_CHECK((audit.complete == std::set<std::string>{"GL11"}));
});

// A small layer: a reference pair, one with stub shims, an extension, a companion, a skipped reference.
struct RefsLayer {
    fs::ScopedTempDir dir{"cb-refs"};
    stdfs::path ref, out;
    RefsLayer() {
        ref = dir.path() / "reference";
        out = dir.path() / "out";
        fs::make_dirs(ref);
        fs::make_dirs(out);
        fs::make_dirs(dir.path() / "companions");
        fs::write_text(ref / "GL11.reference.h", "#pragma once\nnamespace GL11 { void glBegin(int); }\n");
        fs::write_text(ref / "GL11.reference.cpp", "#include \"GL11.h\"\n");
        fs::write_text(ref / "Input.reference.h", "// input\n");
        fs::write_text(ref / "Textures.ext.reference.cpp", "// uses Textures::genId\n");
        fs::write_text(ref / "ZipFile.reference.h", "// zip\n");
        fs::write_text(ref / "Main.reference.cpp", "// beta main\n");
        fs::write_text(dir.path() / "companions" / "Common.h", "// common\n");
        fs::write_text(dir.path() / "companions" / "Textures.h", "// textures\n");
        fs::write_text(dir.path() / "companions" / "Main.cpp", "// companion main\n");
        fs::write_text(dir.path() / "companions" / "readme.txt", "ignored");
    }
};

CB_TEST("java.platform_refs.emit_writes_references_with_a_banner_and_shims", [] {
    RefsLayer layer;
    pr::EmitOptions options;
    options.main_class = "RubyDung";
    const pr::Emitted e = pr::emit_references(layer.out, layer.ref, options);
    const std::string gl = fs::read_text(layer.out / "GL11.h");
    CB_CHECK(gl.starts_with("// ============================ PLATFORM REFERENCE"));
    CB_CHECK(refs_has(gl, "Hand-written Xbox 360 platform layer"));
    CB_CHECK(refs_has(gl, "codebreaker/java/platform/reference/GL11.reference.h"));
    CB_CHECK(refs_has(gl, "namespace GL11 { void glBegin(int); }"));
    CB_CHECK(refs_has(fs::read_text(layer.out / "GL11.cpp"), "#include \"GL11.h\""));
    // the stubs the Input reference replaces become shims onto it
    const std::string mouse = fs::read_text(layer.out / "Mouse.h");
    CB_CHECK(refs_has(mouse, "#ifndef MOUSE_H\n#define MOUSE_H\n#include \"Input.h\"\n#endif // MOUSE_H"));
    CB_CHECK(stdfs::exists(layer.out / "Keyboard.h") && stdfs::exists(layer.out / "Controllers.h"));
    // the extension, the companions, and the generic entry for the detected main class
    CB_CHECK(refs_has(fs::read_text(layer.out / "Textures.ext.cpp"), "uses Textures::genId"));
    CB_CHECK(refs_has(fs::read_text(layer.out / "Common.h"), "PLATFORM COMPANION"));
    CB_CHECK(refs_has(fs::read_text(layer.out / "Main.cpp"), "RubyDung::main(args);"));
    // the skipped reference is not emitted (its stub stands)
    CB_CHECK(!stdfs::exists(layer.out / "ZipFile.h"));
    CB_CHECK(!stdfs::exists(layer.out / "readme.txt"));
    const std::set<std::string> want = {"GL11", "Input", "Textures.ext", "Common", "Textures", "Main"};
    CB_CHECK(e.bases == want);
    CB_CHECK(e.skipped_collisions.empty());
    // the layer report lists what landed
    const std::string md = fs::read_text(layer.out / "PLATFORM_LAYER.md");
    CB_CHECK(refs_has(md, "| `GL11.h` + `GL11.cpp` | yes |"));
    CB_CHECK(refs_has(md, "| `Input.h` (header-only) | yes |"));
    // every reference base integrated
    CB_CHECK(pr::verify_integrated(layer.out, std::set<std::string>{std::string("GL11"), std::string("Input")}).empty());
    // a stub shim (Mouse.h) carries no banner, and a base with no file at all did not integrate either
    CB_CHECK((pr::verify_integrated(layer.out, {"GL11", "Missing", "Mouse"}) == std::vector<std::string>{"Missing", "Mouse"}));
});

CB_TEST("java.platform_refs.emit_skips_what_the_game_defines", [] {
    RefsLayer layer;
    pr::EmitOptions options;
    options.game_classes = {"Textures"};
    options.main_class = "RubyDung";
    const pr::Emitted e = pr::emit_references(layer.out, layer.ref, options);
    CB_CHECK(stdfs::exists(layer.out / "GL11.h"));
    CB_CHECK(!stdfs::exists(layer.out / "Textures.ext.cpp"));   // X.ext collides with the game's X
    CB_CHECK(!stdfs::exists(layer.out / "Textures.h"));          // so does the companion
    CB_CHECK((e.skipped_collisions == std::vector<std::string>{"Textures", "Textures.ext"}));
    CB_CHECK(!e.bases.contains("Textures"));
});

CB_TEST("java.platform_refs.emit_entry_point_choices", [] {
    {   // a main class: the companion Main is replaced by the generated entry
        RefsLayer layer;
        pr::EmitOptions options;
        options.main_class = "Game";
        options.platform = pr::Platform::PcOpenGl;
        pr::emit_references(layer.out, layer.ref, options);
        const std::string main = fs::read_text(layer.out / "Main.cpp");
        CB_CHECK(refs_has(main, "SDL_SetMainReady();"));
        CB_CHECK(refs_has(fs::read_text(layer.out / "GL11.h"), "PC (SDL2 + OpenGL) platform layer"));
        CB_CHECK(refs_has(fs::read_text(layer.out / "GL11.h"), "codebreaker/java/platform/pc/reference/GL11.reference.h"));
    }
    {   // no main class but a Runnable shell: construct and run it
        RefsLayer layer;
        const jir::JClass shell = refs_runnable("Minecraft");
        pr::EmitOptions options;
        options.runnable_game = &shell;
        pr::emit_references(layer.out, layer.ref, options);
        CB_CHECK(refs_has(fs::read_text(layer.out / "Main.cpp"), "_game->run();"));
    }
    {   // neither: Main.reference.cpp is the fallback, over the companion Main.cpp
        RefsLayer layer;
        pr::emit_references(layer.out, layer.ref, {});
        const std::string main = fs::read_text(layer.out / "Main.cpp");
        CB_CHECK(refs_has(main, "PLATFORM REFERENCE") && refs_has(main, "beta main"));
        // without a Main.reference.cpp the companion Main.cpp stands
        stdfs::remove(layer.ref / "Main.reference.cpp");
        fs::ScopedTempDir out2("cb-refs-out");
        pr::emit_references(out2.path(), layer.ref, {});
        CB_CHECK(refs_has(fs::read_text(out2.path() / "Main.cpp"), "companion main"));
    }
    {   // the game defines Main itself: nothing is written over it
        RefsLayer layer;
        pr::EmitOptions options;
        options.game_classes = {"Main"};
        options.main_class = "Game";
        pr::emit_references(layer.out, layer.ref, options);
        CB_CHECK(!stdfs::exists(layer.out / "Main.cpp"));
    }
});

CB_TEST("java.platform_refs.emit_removes_a_stale_skipped_reference_but_not_a_stub", [] {
    RefsLayer layer;
    // a previous run wrote ZipFile.h with the banner; a user-written ZipFile.cpp has none
    fs::write_text(layer.out / "ZipFile.h", "// ============================ PLATFORM REFERENCE ============================\nold\n");
    fs::write_text(layer.out / "ZipFile.cpp", "// a stub the converter wrote\n");
    pr::emit_references(layer.out, layer.ref, {});
    CB_CHECK(!stdfs::exists(layer.out / "ZipFile.h"));
    CB_CHECK(stdfs::exists(layer.out / "ZipFile.cpp"));
});

CB_TEST("java.platform_refs.emit_the_real_layer", [] {
    // the real reference + companion files: every reference header lands with its banner and the layer is complete
    fs::ScopedTempDir out("cb-refs-real");
    pr::EmitOptions options;
    options.main_class = "Minecraft";
    const pr::Emitted e = pr::emit_references(out.path(), refs_real_reference(), options);
    CB_CHECK(e.bases.contains("GL11") && e.bases.contains("Audio") && e.bases.contains("Main"));
    CB_CHECK(e.bases.contains("Common") && e.bases.contains("Tesselator"));   // companions
    CB_CHECK(e.bases.contains("Textures.ext"));
    CB_CHECK(!e.bases.contains("ZipFile"));
    std::set<std::string> references;
    for (const std::string& b : e.bases) {
        if (pr::available_references(refs_real_reference()).contains(b)) references.insert(b);
    }
    CB_CHECK_EQ(references.size(), std::size_t{19});   // the 20 references less the skipped ZipFile
    CB_CHECK(pr::verify_integrated(out.path(), references).empty());
    CB_CHECK(fs::read_text(out.path() / "SoundSystem.h").find("#include \"Audio.h\"") != std::string::npos);
    CB_CHECK(fs::read_text(out.path() / "Main.cpp").find("Minecraft::main(args);") != std::string::npos);
});

}  // namespace
