// platform_refs: the generic entry points, the detection of the game's main class, and the emission of the real
// hand-written platform layer from config/java/platform/{reference,companions} (the real files, read in place).
#include <set>
#include <algorithm>
#include <string>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "java/console/platform_refs.hpp"
#include "java/ir.hpp"

namespace {

using namespace cb;
namespace pr = cb::java::console::platform_refs;
namespace jir = cb::java::ir;
namespace stdfs = std::filesystem;

stdfs::path refs_real_reference() { return testing::data_path("../config/java/platform/reference"); }

bool refs_has(const std::string& text, std::string_view needle) { return text.find(needle) != std::string::npos; }

// A file's text without carriage returns: an emitted copy and its source compare whatever line endings each was written with.
std::string refs_lf(const stdfs::path& p) {
    std::string t = fs::read_text(p);
    t.erase(std::remove(t.begin(), t.end(), '\r'), t.end());
    return t;
}

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
    // Alpha 1.0 (no static main): Minecraft is abstract, and its one concrete subclass is what is made
    jir::JClass abstract_shell = refs_runnable("Minecraft");
    abstract_shell.is_abstract = true;
    jir::JClass impl = refs_class("MinecraftAppletImpl");
    impl.parent = "Minecraft";
    const std::vector<jir::JClass> alpha = {abstract_shell, impl};
    CB_CHECK(pr::detect_runnable_game_class(alpha) == &alpha[1]);
    jir::JClass impl2 = refs_class("MinecraftImpl");
    impl2.parent = "Minecraft";
    CB_CHECK(pr::detect_runnable_game_class(std::vector<jir::JClass>{abstract_shell, impl, impl2}) == nullptr);   // two: no answer
    CB_CHECK(pr::detect_runnable_game_class(std::vector<jir::JClass>{abstract_shell}) == nullptr);                // none
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
    // the PC entry: SDL's main, none of the 360's (D3D device, heap watchdog)
    const std::string pc = pr::generate_pc_runnable_entry_point(shell);
    // (on PC the canvas is the SDL window: Indev's AWT mouse-look reads its size and place on the screen)
    CB_CHECK(refs_has(pc, "Minecraft* _game = new Minecraft(new Canvas(), NULL, SCREEN_WIDTH, SCREEN_HEIGHT, false, 0, 0, NULL);"));
    CB_CHECK(refs_has(pc, "#include \"Canvas.h\""));
    CB_CHECK(!refs_has(pc, "@CANVAS@"));
    CB_CHECK(refs_has(pc, "_game->run();") && refs_has(pc, "j2xThreadsWait();") && refs_has(pc, "SDL_SetMainReady();"));
    CB_CHECK(refs_has(pc, "#define SCREEN_WIDTH 854"));
    CB_CHECK(!refs_has(pc, "HeapTrack") && !refs_has(pc, "Display::create") && !refs_has(pc, "@NAME@"));
    CB_CHECK(!refs_has(pc, "@SESSION@") && !refs_has(pc, "_game->session"));   // no session field: nothing
    // a shell with a session (Alpha's, inherited from Minecraft) gets the launcher's player before run()
    jir::Field session_field;
    session_field.name = "session";
    session_field.type = refs_type("Session", 0);
    jir::JClass impl = shell;
    impl.inherited_fields = {&session_field};
    jir::JClass session2 = refs_class("Session");   // Alpha's: (username, session id)
    jir::Method ctor2;
    ctor2.name = "Session";
    ctor2.is_ctor = true;
    ctor2.params = {{"a", refs_type("String")}, {"b", refs_type("String")}};
    session2.methods.push_back(ctor2);
    const std::vector<jir::JClass> alpha_classes = {session2};
    const std::string alpha = pr::generate_pc_runnable_entry_point(impl, alpha_classes);
    CB_CHECK(refs_has(alpha, "if (_game->session == NULL) _game->session = new Session(JavaString(\"Player\"), JavaString(\"-\"));"));
    CB_CHECK(refs_has(alpha, "#include \"Session.h\""));
    CB_CHECK(alpha.find("_game->session = new") < alpha.find("_game->run();"));
    jir::JClass session1 = refs_class("Session");   // early Indev's: (username)
    jir::Method ctor1 = ctor2;
    ctor1.params.pop_back();
    session1.methods.push_back(ctor1);
    const std::vector<jir::JClass> indev_classes = {session1};
    CB_CHECK(refs_has(pr::generate_pc_runnable_entry_point(impl, indev_classes), "_game->session = new Session(JavaString(\"Player\"));"));
    CB_CHECK(!refs_has(pr::generate_pc_runnable_entry_point(impl), "_game->session"));   // the class unknown: nothing
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
    CB_CHECK((repl.at("NIOBuffers") == std::vector<std::string>{"ByteBuffer", "FloatBuffer", "IntBuffer", "ShortBuffer"}));
});

CB_TEST("java.platform_refs.real_reference_layer_is_complete", [] {
    const stdfs::path dir = refs_real_reference();
    const std::set<std::string> bases = pr::available_references(dir);
    CB_CHECK(bases.contains("GL11") && bases.contains("Audio") && bases.contains("Input") && bases.contains("ZipFile"));
    CB_CHECK(!bases.contains("Main"));   // Main.reference.cpp has no header
    CB_CHECK_EQ(bases.size(), std::size_t{25});
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
    CB_CHECK(refs_has(gl, "config/java/platform/reference/GL11.reference.h"));
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
        CB_CHECK(refs_has(fs::read_text(layer.out / "GL11.h"), "config/java/platform/pc/reference/GL11.reference.h"));
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

CB_TEST("java.platform_refs.layers_replace_the_base_references", [] {
    fs::ScopedTempDir dir("cb-refs-layers");
    const stdfs::path base = dir.path() / "platform" / "pc" / "reference";
    const stdfs::path shared = dir.path() / "platform" / "reference";
    const stdfs::path d3d9 = dir.path() / "platform" / "pc" / "d3d9" / "reference";
    for (const stdfs::path& p : {base, shared, d3d9, shared.parent_path() / "companions", d3d9.parent_path() / "companions"}) stdfs::create_directories(p);
    fs::write_text(base / "GL11.reference.h", "pc gl\n");
    fs::write_text(base / "GL11.reference.cpp", "pc gl cpp\n");
    fs::write_text(base / "GL13.reference.h", "pc gl13\n");
    fs::write_text(base / "GL13.reference.cpp", "pc gl13 cpp\n");
    fs::write_text(base / "Input.reference.h", "pc input\n");
    fs::write_text(base / "Display.reference.h", "pc display\n");
    fs::write_text(shared / "GL11.reference.h", "d3d9 gl\n");
    fs::write_text(shared / "GL11.reference.cpp", "d3d9 gl cpp\n");
    fs::write_text(shared / "GL13.reference.h", "d3d9 gl13\n");                 // header-only over the PC's .h + .cpp
    fs::write_text(shared / "Display.reference.h", "360 display\n");            // not taken: the layer names only GL11, GL13
    fs::write_text(shared.parent_path() / "companions" / "Textures.h", "360 textures\n");   // nor its companions
    fs::write_text(d3d9 / "Display.reference.h", "d3d9 display\n");
    fs::write_text(d3d9.parent_path() / "companions" / "Common.h", "pc d3d9 common\n");
    const std::vector<pr::Layer> layers = {{shared, {"GL11", "GL13"}}, {d3d9, {}}};

    const std::map<std::string, stdfs::path> sources = pr::reference_sources(base, layers);
    CB_CHECK(sources.at("GL11") == shared && sources.at("GL13") == shared && sources.at("Display") == d3d9 && sources.at("Input") == base);
    CB_CHECK_EQ(sources.size(), std::size_t{4});

    fs::ScopedTempDir out("cb-refs-layers-out");
    fs::write_text(out.path() / "GL13.cpp", "// ============================ PLATFORM REFERENCE ============================\nstale\n");
    pr::EmitOptions options;
    options.platform = pr::Platform::PcOpenGl;
    options.layers = layers;
    options.label = "PC (SDL2 + Direct3D 9)";
    pr::emit_references(out.path(), base, options);
    const std::string gl = fs::read_text(out.path() / "GL11.h");
    CB_CHECK(refs_has(gl, "d3d9 gl") && refs_has(gl, "PC (SDL2 + Direct3D 9) platform layer"));
    CB_CHECK(refs_has(gl, "config/java/platform/reference/GL11.reference.h"));   // the folder it came from
    CB_CHECK(refs_has(fs::read_text(out.path() / "GL11.cpp"), "d3d9 gl cpp"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "GL13.h"), "d3d9 gl13"));
    CB_CHECK(!stdfs::exists(out.path() / "GL13.cpp"));                           // the PC's .cpp is not left beside it
    CB_CHECK(refs_has(fs::read_text(out.path() / "Display.h"), "d3d9 display"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "Display.h"), "config/java/platform/pc/d3d9/reference/Display.reference.h"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "Input.h"), "pc input"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "Common.h"), "pc d3d9 common"));   // a whole layer brings its companions
    CB_CHECK(!stdfs::exists(out.path() / "Textures.h"));                         // a named-only layer does not
});

CB_TEST("java.platform_refs.the_pc_direct3d9_layer_is_the_shared_renderer", [] {
    const stdfs::path pc = testing::data_path("../config/java/platform/pc/reference");
    const stdfs::path d3d9 = testing::data_path("../config/java/platform/pc/d3d9/reference");
    pr::EmitOptions options;
    options.platform = pr::Platform::PcOpenGl;
    options.main_class = "Minecraft";
    options.layers = {{refs_real_reference(), {"GL11", "GLU", "GL13", "ARBMultitexture", "Gpu", "GpuShaders", "GpuD3D9"}}, {d3d9, {}}};
    options.label = "PC (SDL2 + Direct3D 9)";
    fs::ScopedTempDir out("cb-refs-pcd3d9");
    pr::emit_references(out.path(), pc, options);
    // the renderer is the Xbox 360's own file, word for word below the banner
    const std::string gl = fs::read_text(out.path() / "GL11.cpp");
    CB_CHECK(refs_has(refs_lf(out.path() / "GL11.cpp"), refs_lf(refs_real_reference() / "GL11.reference.cpp")));
    CB_CHECK(refs_has(fs::read_text(out.path() / "GL11.h"), "namespace GL11 = j2xgl;"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "GLU.h"), "namespace GLU = j2xgl;"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "GL13.h"), "j2xgl::glActiveTexture"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "ARBMultitexture.h"), "j2xgl::glActiveTexture"));
    // the backend is the Xbox 360's Direct3D 9 one, the GL core knows no API
    CB_CHECK(refs_has(refs_lf(out.path() / "GpuD3D9.cpp"), refs_lf(refs_real_reference() / "GpuD3D9.reference.cpp")));
    CB_CHECK(!refs_has(gl, "IDirect3D") && !refs_has(gl, "D3DRS_") && refs_has(gl, "gpu::draw("));
    CB_CHECK(!stdfs::exists(out.path() / "GpuD3D11.cpp"));
    CB_CHECK(!stdfs::exists(out.path() / "GL13.cpp") && stdfs::exists(out.path() / "GLU.cpp"));   // (GL13 is header-only here)
    // the window and device are the PC's Direct3D 9 Display; Direct3D and the D3DX stand-in come with it
    CB_CHECK(refs_has(fs::read_text(out.path() / "Display.cpp"), "Direct3DCreate9"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "Common.h"), "#include \"J2xD3DX9.h\""));
    CB_CHECK(refs_has(fs::read_text(out.path() / "J2xD3DX9.h"), "D3DCompile"));
    // the PC's other platform files stand; nothing of the 360's beyond the renderer comes along
    CB_CHECK(refs_has(fs::read_text(out.path() / "Input.h"), "config/java/platform/pc/reference/Input.reference.h"));
    CB_CHECK(!stdfs::exists(out.path() / "Textures.h") && !stdfs::exists(out.path() / "Shaders.h") && !stdfs::exists(out.path() / "XactAudio.h"));
    // and no OpenGL anywhere in what the PC's Direct3D 9 build compiles
    for (const char* f : {"GL11.cpp", "GLU.cpp", "Display.cpp", "GL11.h", "GL13.h"})
        CB_CHECK(!refs_has(fs::read_text(out.path() / f), "<GL/gl.h>") && !refs_has(fs::read_text(out.path() / f), "SDL_GL_"));
});

CB_TEST("java.platform_refs.the_pc_direct3d11_layer_is_the_shared_renderer_on_its_own_backend", [] {
    const stdfs::path pc = testing::data_path("../config/java/platform/pc/reference");
    const stdfs::path d3d11 = testing::data_path("../config/java/platform/pc/d3d11/reference");
    pr::EmitOptions options;
    options.platform = pr::Platform::PcOpenGl;
    options.main_class = "Minecraft";
    options.layers = {{refs_real_reference(), {"GL11", "GLU", "GL13", "ARBMultitexture", "Gpu", "GpuShaders"}}, {d3d11, {}}};
    options.label = "PC (SDL2 + Direct3D 11)";
    fs::ScopedTempDir out("cb-refs-pcd3d11");
    pr::emit_references(out.path(), pc, options);
    // the GL core is the Xbox 360's file, word for word; the backend and the Display are Direct3D 11's
    CB_CHECK(refs_has(refs_lf(out.path() / "GL11.cpp"), refs_lf(refs_real_reference() / "GL11.reference.cpp")));
    CB_CHECK(refs_has(fs::read_text(out.path() / "GL11.h"), "PC (SDL2 + Direct3D 11) platform layer"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "GpuD3D11.cpp"), "ID3D11Device"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "GpuD3D11.cpp"), "kGlVertexStage"));   // the shared pipeline, not its own copy
    CB_CHECK(refs_has(fs::read_text(out.path() / "Display.cpp"), "D3D11CreateDeviceAndSwapChain"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "Display.cpp"), "D3D_DRIVER_TYPE_WARP"));
    CB_CHECK(stdfs::exists(out.path() / "Gpu.h") && stdfs::exists(out.path() / "GpuShaders.h"));
    // nothing of Direct3D 9: no backend, no D3DX stand-in, the PC's own Common.h (with the layer's diagnostics)
    CB_CHECK(!stdfs::exists(out.path() / "GpuD3D9.cpp") && !stdfs::exists(out.path() / "J2xD3DX9.h"));
    CB_CHECK(refs_has(fs::read_text(out.path() / "Common.h"), "#define J2X_UNHANDLED"));
    CB_CHECK(!refs_has(fs::read_text(out.path() / "Common.h"), "d3d9"));
    for (const char* f : {"GL11.cpp", "GLU.cpp", "Display.cpp", "GpuD3D11.cpp", "GL11.h"})
        CB_CHECK(!refs_has(fs::read_text(out.path() / f), "<GL/gl.h>") && !refs_has(fs::read_text(out.path() / f), "d3d9.h"));
});

// Direct3D 10 and 12: the same shared renderer, each on its own backend and Display.
CB_TEST("java.platform_refs.the_pc_direct3d10_and_12_layers_are_the_shared_renderer_on_their_own_backends", [] {
    const stdfs::path pc = testing::data_path("../config/java/platform/pc/reference");
    struct Api { const char* dir; const char* backend; const char* device; const char* create; const char* label; };
    const Api apis[] = {{"d3d10", "GpuD3D10.cpp", "ID3D10Device", "D3D10CreateDeviceAndSwapChain", "PC (SDL2 + Direct3D 10)"},
                        {"d3d12", "GpuD3D12.cpp", "ID3D12GraphicsCommandList", "D3D12CreateDevice", "PC (SDL2 + Direct3D 12)"}};
    for (const Api& api : apis) {
        pr::EmitOptions options;
        options.platform = pr::Platform::PcOpenGl;
        options.main_class = "Minecraft";
        options.layers = {{refs_real_reference(), {"GL11", "GLU", "GL13", "ARBMultitexture", "Gpu", "GpuShaders"}},
                          {testing::data_path(std::string("../config/java/platform/pc/") + api.dir + "/reference"), {}}};
        options.label = api.label;
        fs::ScopedTempDir out(std::string("cb-refs-pc") + api.dir);
        pr::emit_references(out.path(), pc, options);
        CB_CHECK(refs_has(refs_lf(out.path() / "GL11.cpp"), refs_lf(refs_real_reference() / "GL11.reference.cpp")));
        CB_CHECK(refs_has(fs::read_text(out.path() / "GL11.h"), std::string(api.label) + " platform layer"));
        CB_CHECK(refs_has(fs::read_text(out.path() / api.backend), api.device));
        CB_CHECK(refs_has(fs::read_text(out.path() / api.backend), "kGlVertexStage"));   // the shared pipeline, not its own copy
        CB_CHECK(refs_has(fs::read_text(out.path() / "Display.cpp"), api.create));
        CB_CHECK(refs_has(fs::read_text(out.path() / "Display.cpp"), "WARP"));         // a computer without the card still runs it
        CB_CHECK(stdfs::exists(out.path() / "Gpu.h") && stdfs::exists(out.path() / "GpuShaders.h"));
        // no other API's backend, no D3DX stand-in, the PC's own Common.h
        for (const char* other : {"GpuD3D9.cpp", "GpuD3D10.cpp", "GpuD3D11.cpp", "GpuD3D12.cpp"})
            CB_CHECK(std::string(other) == api.backend || !stdfs::exists(out.path() / other));
        CB_CHECK(!stdfs::exists(out.path() / "J2xD3DX9.h"));
        CB_CHECK(refs_has(fs::read_text(out.path() / "Common.h"), "#define J2X_UNHANDLED"));
        for (const char* f : {"GL11.cpp", "GLU.cpp", "Display.cpp", "GL11.h"})
            CB_CHECK(!refs_has(fs::read_text(out.path() / f), "<GL/gl.h>") && !refs_has(fs::read_text(out.path() / f), "d3d9.h"));
    }
    // Direct3D 10 has no B8G8R8A8 (10.1 added it): its resources are R8G8B8A8 and the shaders swap red and blue
    const std::string d3d10 = fs::read_text(testing::data_path("../config/java/platform/pc/d3d10/reference/GpuD3D10.reference.cpp"));
    CB_CHECK(!refs_has(d3d10, "B8G8R8A8_UNORM") && refs_has(d3d10, "vc.bgra") && refs_has(d3d10, "Sample(samp0, t0.xy / t0.w).bgra"));
    // Direct3D 12 waits for the GPU before reusing a frame's allocator, and frees what the game lets go of only after it
    const std::string d3d12 = fs::read_text(testing::data_path("../config/java/platform/pc/d3d12/reference/GpuD3D12.reference.cpp"));
    CB_CHECK(refs_has(d3d12, "_wait(g_allocFence[g_frame]);") && refs_has(d3d12, "_retire(t->res, t->slot);"));
    // display lists share pages (a committed resource per list blocked in CreateCommittedResource under memory pressure)
    CB_CHECK(refs_has(d3d12, "_pageAlloc(size, pg, offset)") && !refs_has(d3d12, "CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &d, D3D12_RESOURCE_STATE_COMMON, 0, __uuidof(ID3D12Resource), (void**)&res)) || !res) return 0;"));
    // the device is released before the process ends (exit with a live Direct3D 12 device hung in the driver), and a window or
    // device that cannot be made ends the program rather than letting it run without a renderer
    for (const char* api : {"d3d10", "d3d11", "d3d12"}) {
        // (the Display is a template over the Direct3D family's: its own file and the templates are searched together)
        const stdfs::path pc_dir = testing::data_path("../config/java/platform/pc");
        const std::string display = fs::read_text(pc_dir / api / "reference" / "Display.reference.cpp") + fs::read_text(pc_dir / "templates" / "DisplayDxgi.template.cpp");
        CB_CHECK(refs_has(display, "std::atexit(release_at_exit);") && refs_has(display, "std::exit(3);"));
    }
});

// Direct3D 7 and 8: the same GL layer on their fixed-function pipelines, the plan for both in GpuFixed.h; Direct3D 8's interface
// is the layer's own declaration (no SDK has d3d8.h for Visual C++), loaded from d3d8.dll at run time.
CB_TEST("java.platform_refs.the_pc_direct3d7_and_8_layers_are_the_shared_renderer_on_the_fixed_function_pipeline", [] {
    const stdfs::path pc = testing::data_path("../config/java/platform/pc/reference");
    struct Api { const char* dir; const char* backend; const char* device; const char* create; const char* label; };
    const Api apis[] = {{"d3d7", "GpuD3D7.cpp", "IDirect3DDevice7", "DirectDrawCreateEx", "PC (SDL2 + Direct3D 7)"},
                        {"d3d8", "GpuD3D8.cpp", "IDirect3DDevice8", "d3d8::create()", "PC (SDL2 + Direct3D 8)"}};
    for (const Api& api : apis) {
        pr::EmitOptions options;
        options.platform = pr::Platform::PcOpenGl;
        options.main_class = "Minecraft";
        options.layers = {{refs_real_reference(), {"GL11", "GLU", "GL13", "ARBMultitexture", "Gpu", "GpuShaders", "GpuFixed"}},
                          {testing::data_path(std::string("../config/java/platform/pc/") + api.dir + "/reference"), {}}};
        options.label = api.label;
        fs::ScopedTempDir out(std::string("cb-refs-pc") + api.dir);
        pr::emit_references(out.path(), pc, options);
        const std::string backend = fs::read_text(out.path() / api.backend);
        CB_CHECK(refs_has(fs::read_text(out.path() / "GL11.h"), std::string(api.label) + " platform layer"));
        CB_CHECK(refs_has(backend, api.device) && refs_has(backend, "fixed::plan(p, rs, c, b, g_w, g_h);"));   // the shared plan
        CB_CHECK(!refs_has(backend, "kGlVertexStage") && !refs_has(backend, "D3DCompile"));                    // no shaders at all
        CB_CHECK(refs_has(fs::read_text(out.path() / "Display.cpp"), api.create));
        CB_CHECK(stdfs::exists(out.path() / "Gpu.h") && stdfs::exists(out.path() / "GpuFixed.h"));
        for (const char* other : {"GpuD3D7.cpp", "GpuD3D8.cpp", "GpuD3D9.cpp", "GpuD3D10.cpp", "GpuD3D11.cpp", "GpuD3D12.cpp"})
            CB_CHECK(std::string(other) == api.backend || !stdfs::exists(out.path() / other));
        for (const char* f : {"GL11.cpp", "GLU.cpp", "Display.cpp", "GL11.h"})
            CB_CHECK(!refs_has(fs::read_text(out.path() / f), "<GL/gl.h>") && !refs_has(fs::read_text(out.path() / f), "d3d9.h"));
        const std::string display = fs::read_text(out.path() / "Display.cpp");
        CB_CHECK(refs_has(display, "std::atexit(release_at_exit);") && refs_has(display, "std::exit(3);") && refs_has(display, "FPU"));
    }
    CB_CHECK(stdfs::exists(testing::data_path("../config/java/platform/pc/d3d8/reference/Direct3D8.reference.h")));
    // the plan: GL's eye space mirrored into Direct3D's camera space (fog is the camera-space z), the viewport and scissor box as
    // one viewport, glCallList's colour as a factor stage, glTexGen through the camera-space position
    const std::string plan = fs::read_text(refs_real_reference() / "GpuFixed.reference.h");
    CB_CHECK(refs_has(plan, "p.world = mul(mv, s);") && refs_has(plan, "p.proj = mul(mul(s, inv), wvp);"));
    CB_CHECK(refs_has(plan, "inline bool fitViewport(") && refs_has(plan, "kTaFactor") && refs_has(plan, "kCoordEyePosition"));
});

// The PC's sound closes its device before the statics go: SDL's audio thread mixed from freed sources as the program ended.
CB_TEST("java.platform_refs.the_pc_audio_device_closes_before_exit", [] {
    const std::string audio = fs::read_text(testing::data_path("../config/java/platform/pc/reference/Audio.reference.cpp"));
    CB_CHECK(refs_has(audio, "std::atexit(closeDevice);") && refs_has(audio, "SDL_CloseAudioDevice(g_dev);"));
});

// glTexGen: the layer generates unit 0's coordinates in the shared vertex stage (object and eye planes) and samples them
// projectively; the PC's OpenGL layer forwards the calls.
CB_TEST("java.platform_refs.texture_coordinate_generation_is_in_both_gl_layers", [] {
    const std::string h = fs::read_text(refs_real_reference() / "GL11.reference.h");
    const std::string gl = fs::read_text(refs_real_reference() / "GL11.reference.cpp");
    const std::string shaders = fs::read_text(refs_real_reference() / "GpuShaders.reference.h");
    CB_CHECK(refs_has(h, "void glTexGeni(int coord, int pname, int param);") && refs_has(h, "GL_OBJECT_LINEAR = 0x2401"));
    CB_CHECK(refs_has(gl, "case GL_TEXTURE_GEN_Q:") && refs_has(gl, "!_texGenObject()"));   // (object coordinates: no eye-space batch)
    CB_CHECK(refs_has(shaders, "dot(P, gGenO[0])") && refs_has(shaders, "dot(e, gGenE[0])") && refs_has(shaders, "out float4 ot0"));
    const std::string pc = fs::read_text(testing::data_path("../config/java/platform/pc/reference/GL11.reference.h"));
    CB_CHECK(refs_has(pc, "void glTexGen(int coord, int pname, FloatBuffer* params);") && refs_has(pc, "GL_TEXTURE_GEN_S = 0x0C60"));
});

// One shader pipeline for every backend: the vertex and fragment stages live in GpuShaders.h, the backends only wrap them.
CB_TEST("java.platform_refs.the_backends_share_one_pipeline", [] {
    const std::string shared = fs::read_text(refs_real_reference() / "GpuShaders.reference.h");
    CB_CHECK(refs_has(shared, "void glVertex(") && refs_has(shared, "float4 glFragment("));
    std::vector<std::string> backends = {fs::read_text(refs_real_reference() / "GpuD3D9.reference.cpp")};
    for (const char* api : {"d3d10", "d3d11", "d3d12"})
        backends.push_back(fs::read_text(testing::data_path(std::string("../config/java/platform/pc/") + api + "/reference/Gpu" +
                                                            (std::string("D3D") + (api + 3)) + ".reference.cpp")));
    for (const std::string& backend : backends) {
        CB_CHECK(refs_has(backend, "kGlVertexStage") && refs_has(backend, "kGlFragmentStage"));
        CB_CHECK(!refs_has(backend, "toLight(") && !refs_has(backend, "env(o,"));   // (no private copy of the maths)
        CB_CHECK(refs_has(backend, "gGenO") && refs_has(backend, "t0.xy / t0.w"));    // (texgen's constants, the projective sample)
    }
    // the core names no API
    const std::string gl = fs::read_text(refs_real_reference() / "GL11.reference.cpp");
    for (const char* api : {"IDirect3D", "ID3D10", "ID3D11", "ID3D12", "D3DRS_", "D3DXMATRIX", "d3d9.h", "d3d10.h", "d3d11.h", "d3d12.h"})
        CB_CHECK(!refs_has(gl, api));
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
    CB_CHECK_EQ(references.size(), std::size_t{24});   // the 25 references less the skipped ZipFile
    CB_CHECK(pr::verify_integrated(out.path(), references).empty());
    CB_CHECK(fs::read_text(out.path() / "SoundSystem.h").find("#include \"Audio.h\"") != std::string::npos);
    CB_CHECK(fs::read_text(out.path() / "Main.cpp").find("Minecraft::main(args);") != std::string::npos);
});

}  // namespace
