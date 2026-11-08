// codebreaker/java/console/platform_refs.cpp -- typed C++ port of codebreaker.java.console.platform_refs.
// (The functions the Python front end produced -- the banners, the entry-point generators, available_references --
// are kept as they were; the rest is ported by hand on the real java::ir types.)

#include "codebreaker/java/console/platform_refs.hpp"

#include <algorithm>

#include "codebreaker/java/textutil.hpp"

namespace codebreaker::java::console::platform_refs {

py::dict<py::str, std::tuple<py::str, py::str>> LAYER_ = py::dict<py::str, std::tuple<py::str, py::str>>{{py::str("xbox360"), std::tuple<py::str, py::str>{py::str("Xbox 360"), py::str("codebreaker/java/platform/reference")}}, {py::str("pc_opengl"), std::tuple<py::str, py::str>{py::str("PC (SDL2 + OpenGL)"), py::str("codebreaker/java/platform/pc/reference")}}};

py::dict<py::str, py::list<py::str>> REFERENCE_REPLACES = py::dict<py::str, py::list<py::str>>{
    {py::str("Input"), py::list<py::str>{py::str("Keyboard"), py::str("Mouse"), py::str("Controllers")}},   // Input.reference defines namespace Keyboard/Mouse/Controllers
    {py::str("ImageIO"), py::list<py::str>{py::str("ImageIO")}},
    // FIXED BUG audio-not-emitted: Audio.reference DEFINES the real SoundSystem (paulscode -> XACT3 bridge),
    // but the game #includes "SoundSystem.h" -- so shim that stub to the real Audio.h, else the no-op
    // SoundSystem stub shadows the bridge and every playSoundFX is silent (and the empty pool crashed).
    {py::str("Audio"), py::list<py::str>{py::str("SoundSystem")}},
    // NIOBuffers.reference DEFINES ByteBuffer/FloatBuffer/IntBuffer (the real byte-granular NIO buffers the
    // GL emulator + Tessellator use). Without this, its class defs collide with the converter's ByteBuffer.h/
    // FloatBuffer.h/IntBuffer.h stubs -> C2011 redefinition in every TU that pulls both.
    {py::str("NIOBuffers"), py::list<py::str>{py::str("ByteBuffer"), py::str("FloatBuffer"), py::str("IntBuffer")}},
    // FileIO no longer SHIMS the byte streams. FIXED BUG filestream-shim-redefines-FileOutputStream: the real
    // java.io byte streams are now CANONICAL standalone headers emitted by runtime.py (loaded from
    // docs/platform/canonical/), and FileIO.reference.h just #includes them -- one definition each, no shim.
};

// References that lack a Beta-specific method the converter's STUB already provides (and would otherwise
// shadow it), or that need companion types not yet bundled. SKIP emitting them -> the converter's stub
// (which the game already compiles+links against) stands. First-boot tradeoff: rendering/input/file-IO are
// real; audio/zip/socket/threading/image-decode fall back to the no-op stubs until each is reconciled.
//   ZipFile     -> getEntry()->ZipEntry* (stub) + reference needs full InputStream/FileInputStream
//   Socket      -> getRemoteSocketAddress()->SocketAddress* (stub)
//   Thread      -> dumpStack()/join(1-arg) (stub). REVERTED to the STUB: making java.lang.Thread real
//                  (CreateThread on every start()) turned on the dead-on-console daemon threads
//                  (StatsSyncher/ThreadDownload*/timer) and froze the first menu render frame. The game's
//                  real worker pool is j2x_pool (ThreadPool.h, CreateThread directly) -- it does NOT need
//                  java Thread to be real. (Reference kept for that.)
// NOT skipped: BufferedImage -- its reference is the REAL ARGB pixel buffer that ImageIO.reference decodes
// PNGs into (w/h/pixels fields), and it now carries getRaster()/flush()/createGraphics() too.
// FIXED BUG audio-not-emitted: Audio was UN-SKIPPED (the Beta-new SoundSystem::backgroundMusic(...)
// catch-all now exists in Audio.reference.h, and the XactAudio.h/.cpp companion is bundled), so the XACT3
// questionnaire choice emits the real paulscode->XACT3 bridge instead of the no-op stub.
// FIXED BUG net-socket-stubbed: Socket was UN-SKIPPED (the reference's getRemoteSocketAddress() now returns
// SocketAddress*/InetAddress*), so the real Winsock/XNet TCP socket emits instead of the no-op stub.
// FIXED BUG mp-threads-never-run: Thread was UN-SKIPPED. Thread.reference.h is now the GATED version --
// start() is a virtual NO-OP by default (daemons stay dead), and only the net reader/writer/connect thread
// classes override start() (in the overlay) to spawn a real thread.
py::set<py::str> REFERENCE_SKIP = py::set<py::str>{py::str("ZipFile")};

py::set<py::str> ENTRY_PRIMS_ = py::set<py::str>{py::str("int"), py::str("long"), py::str("short"), py::str("byte"), py::str("char"), py::str("boolean"), py::str("float"), py::str("double")};

py::set<py::str> SPECIAL_CPP_ONLY_ = py::set<py::str>{py::str("Main")};

py::list<py::str> emit_references_last_skipped;

namespace {

// f.read(400) of a text file opened with errors='replace': the first 400 characters, newlines translated.
py::str read_head_(const py::str& fp) {
    py::str t = codebreaker::java::textutil::read_text(fp, py::str("replace"), false);
    return t.size() > 400 ? t.slice_to(400) : t;
}

}  // namespace

// Base names X that have an X.reference.h in reference_dir (e.g. {'GL11','Audio','Socket',...}).
py::set<py::str> available_references(const py::str& reference_dir) {
    if (!(!reference_dir.empty()) || !py::os::path::isdir(reference_dir)) {
        return py::set<py::str>{};
    }
    py::list<py::str> items;
    for (const py::str& f : py::os::listdir(reference_dir)) {
        if (f.endswith(py::str(".reference.h"))) {
            items.append(f.slice_to(-py::len(py::str(".reference.h"))));
        }
    }
    return py::set<py::str>::from(items);
}

// The GENERIC Xbox 360 entry point, auto-generated from the detected main class -- no game
// hardcoding. It runs ordered static init then hands control to the game's OWN `main(String[])`,
// which drives display creation + the game loop through the platform shims (Display/GL/Input).
// The platform device is created lazily by the Display/GL layer when the game calls Display.create,
// so the entry itself carries no Beta-specific bootstrap (texture preloads, MinecraftImpl, Shaders).
py::str generate_entry_point(const py::str& main_class) {
    return py::percent("// ============================ GENERIC ENTRY POINT ============================\n// Auto-generated by CodeBreaker. The 360 title entry: run ordered Java static init, then call\n// the game's detected main class. Device/display setup is driven by the game's own code\n// through the platform shims (Display/GL11/Keyboard) -- no game-specific bootstrap here.\n// (Detected main class: %s. Regenerated every convert from whatever main() the game has.)\n// ==========================================================================\n\n#include \"stdafx.h\"\n#include \"JavaRT.h\"\n#include \"Common.h\"       // DbgPrintF -- the boot-trace sink that surfaces on the 360/emulator\n#include \"Display.h\"      // the Direct3D 9 device owner -- created FIRST (see below)\n#include \"j2x_init.h\"     // generated: void j2x_initAll();\n#include \"HeapTrack.h\"    // heap-leak watchdog -- start()s the monitor that prints the PASS/FAIL verdict\n#include \"%s.h\"\n\nvoid __cdecl main() {\n    // DEVICE FIRST. Create the D3D9 device before anything else -- so the title claims the global\n    // device (VdGlobalDevice) before the emulator's ASYNC shader-cache thread can. Creating it late\n    // (after static init + the game thread) loses that race and trips the \"already active D3D device\"\n    // assertion. The working hand-port creates its device first thing in main() for exactly this\n    // reason; the game's own later Display.create() is a guarded no-op (and setDisplayMode is\n    // ignored once created, so the backbuffer dims stay consistent).\n    Display::create();\n    // Start the heap-leak watchdog's monitor thread NOW (its operator new/delete hooks already track\n    // every allocation; this launches the 2.5s reporter that prints \"[HEAP] LEAK VERDICT: PASS/FAIL\").\n    // Compiles to a no-op when J2X_HEAPTRACK=0. This is what surfaces the leak result on the emulator.\n    HeapTrack::start();\n    // Boot trace via DbgPrintF (mirrors through HostPathDevice::ResolvePath, the channel that\n    // always surfaces on this console/emulator). Brackets the two transitions that black-screen\n    // a launch: did static init finish, and did the game's own main() get control.\n    DbgPrintF(\"[J2X] entry: device up; running ordered static init\\n\");\n    // Ordered static init: every class's Java static{} blocks + static field initializers,\n    // in dependency order (prevents the static-init-order null-vtable crash class).\n    j2x_initAll();\n    DbgPrintF(\"[J2X] static init done; calling game main\\n\");\n\n    // Hand off to the game's own entry point. No command-line args on console.\n    JArray<JavaString> args(0);   // Java String[] -> JArray<JavaString> (JavaString is a value type)\n    %s::main(args);\n    DbgPrintF(\"[J2X] game main returned (exited)\\n\");\n}\n", main_class, main_class, main_class);
}

// The PC entry: ordered static init, then the game's own main(String[]) with the command line, then
// -- as the JVM -- wait for the game's non-daemon threads (the game makes its window on the thread that
// calls Display.create()). Resources load from a `Content` folder beside the program when there is one,
// else from the working folder.
py::str generate_pc_entry_point(const py::str& main_class) {
    return py::percent("// ============================ GENERIC ENTRY POINT (PC) ============================\n// Auto-generated by CodeBreaker from the detected main class (%s).\n// ==========================================================================\n\n#include \"stdafx.h\"\n#include \"JavaRT.h\"\n#include \"Common.h\"\n#include \"Thread.h\"\n#include \"j2x_init.h\"\n#include \"%s.h\"\n#define SDL_MAIN_HANDLED\n#include <SDL2/SDL.h>\n#include <string>\n#ifdef _WIN32\n#include <direct.h>\n#define j2x_chdir _chdir\n#else\n#include <unistd.h>\n#define j2x_chdir chdir\n#endif\n\nint main(int argc, char** argv) {\n    SDL_SetMainReady();\n    char* base = SDL_GetBasePath();\n    if (base) {\n        std::string content = std::string(base) + \"Content\";\n        if (j2x_chdir(content.c_str()) != 0) j2x_chdir(base);\n        SDL_free(base);\n    }\n    DbgPrintF(\"[J2X] running ordered static init\\n\");\n    j2x_initAll();\n    JArray<JavaString> args(argc > 1 ? argc - 1 : 0);\n    for (int i = 1; i < argc; ++i) args[i - 1] = JavaString(argv[i]);\n    %s::main(args);\n    j2xThreadsWait();\n    return 0;\n}\n", main_class, main_class, main_class);
}

py::str _banner(const py::str& base, const py::str& ext, const py::str& platform) {
    auto [label, where] = LAYER_.get(platform, LAYER_.at("xbox360"));
    return py::percent("// ============================ PLATFORM REFERENCE ============================\n// Hand-written %s platform layer, emitted from\n//   %s/%s.reference.%s\n// This is the REAL implementation -- it REPLACES the converter no-op stub for %s.\n// (NOT converter-generated game code; edit the .reference.%s source, then reconvert.)\n// ==========================================================================\n\n", label, where, base, ext, base, ext);
}

py::str _companion_banner(const py::str& fn) {
    return py::percent("// ============================ PLATFORM COMPANION ============================\n// Game-agnostic Xbox 360 platform file, emitted by CodeBreaker from\n//   docs/platform/companions/%s\n// A reference .cpp links against this; it is version-independent (edit the companion\n// source, then reconvert). NOT converter-generated game code.\n// ==========================================================================\n\n", fn);
}


// The game's entry class: the one declaring `public static void main(String[])`. Returns its
// name, or None. Detection-driven -- NO hardcoded game name -- so the generic entry point can invoke
// whatever main class the converted game actually has (RubyDung, Minecraft, anything).
std::optional<py::str> detect_main_class(const py::list<py::ref<ir::JClass>>& classes) {
    for (const py::ref<ir::JClass>& c : classes) {
        for (const py::ref<ir::Method>& m : c->methods) {
            if (m->name != "main" || !m->is_static) {
                continue;
            }
            const py::list<py::ref<ir::Param>>& params = m->params;
            if (py::len(params) != 1) {
                continue;
            }
            const py::ref<ir::JType>& pt = params[0]->jtype;
            if (pt->is_array() && (pt->name == "String" || pt->name == "java.lang.String")) {
                return c->name;
            }
        }
    }
    return std::nullopt;
}

// The game's shell class when it has NO `static main` but IS a Runnable started on a thread
// (Indev/Infdev: `Minecraft implements Runnable`, constructed by the applet + run() on a thread).
// Returns the JClass, or None.
//
// Detection-driven, NO hardcoded game name: the shell is a CONCRETE, TOP-LEVEL (no superclass)
// class that implements Runnable and declares run(). This is deliberately narrow so it does NOT
// fire for Alpha/Beta -- there `Minecraft` is ABSTRACT (the concrete `MinecraftImpl` has a parent),
// so both fail the concrete+top-level test and the Beta-specific Main.reference.cpp path is kept.
// A UI Runnable (Indev's GuiLoadLevel extends GuiScreen) is excluded by the no-superclass test.
py::ref<ir::JClass> detect_runnable_game_class(const py::list<py::ref<ir::JClass>>& classes) {
    py::list<py::ref<ir::JClass>> cands;
    for (const py::ref<ir::JClass>& c : classes) {
        if (!c->interfaces.contains(py::str("Runnable"))) {
            continue;
        }
        if (c->is_abstract || c->is_interface) {
            continue;
        }
        if (c->parent.has_value() && *c->parent != "Object") {
            continue;
        }
        bool has_run = false;
        for (const py::ref<ir::Method>& m : c->methods) {
            if (m->name == "run") {
                has_run = true;
                break;
            }
        }
        if (!has_run) {
            continue;
        }
        cands.append(c);
    }
    return py::len(cands) == 1 ? cands[0] : nullptr;   // only when unambiguous
}

// Default arguments to construct the Runnable game shell from the entry point: NULL for every
// reference param (no AWT frame/canvas/applet on the console), the display size for the first two
// int params, false for booleans, 0 for other primitives. Uses the widest constructor.
py::str _runnable_ctor_args(const py::ref<ir::JClass>& cls) {
    py::list<py::ref<ir::Method>> ctors = cls->constructors();
    if (ctors.empty()) {
        return py::str("");
    }
    py::ref<ir::Method> ctor = ctors[0];   // max(): the first of the widest
    for (const py::ref<ir::Method>& m : ctors) {
        if (py::len(m->params) > py::len(ctor->params)) {
            ctor = m;
        }
    }
    py::list<py::str> args;
    int64_t ints = 0;
    for (const py::ref<ir::Param>& p : ctor->params) {
        const py::ref<ir::JType>& jt = p->jtype;
        bool is_arr = jt->is_array();
        if (jt->name == "int" && !is_arr) {
            ints += 1;
            args.append(ints == 1 ? py::str("SCREEN_WIDTH") : ints == 2 ? py::str("SCREEN_HEIGHT") : py::str("0"));
        } else if (jt->name == "boolean" && !is_arr) {
            args.append(py::str("false"));
        } else if (ENTRY_PRIMS_.contains(jt->name) && !is_arr) {
            args.append(py::str("0"));
        } else {
            args.append(py::str("NULL"));
        }
    }
    return py::join(py::str(", "), args);
}

// The 360 entry point for a Runnable game shell with no static main (Indev): run ordered static
// init, construct the shell (NULL AWT args + display size), then call its Runnable run() -- the same
// thing the Java applet's thread does. Detection-driven; no game-specific bootstrap.
py::str generate_runnable_entry_point(const py::ref<ir::JClass>& cls) {
    py::str name = cls->name;
    py::str args = _runnable_ctor_args(cls);
    return py::percent("// ============================ GENERIC ENTRY POINT (Runnable) ============================\n// Auto-generated by CodeBreaker. The game shell (%s) has no static main -- Java starts it as a\n// Runnable on a thread. The 360 title entry does the same: ordered static init, construct the\n// shell (no AWT frame/canvas on console -> NULL), then call run(). No game-specific bootstrap;\n// display/GL/input are driven by the shell through the platform shims.\n// =======================================================================================\n\n#include \"stdafx.h\"\n#include \"JavaRT.h\"\n#include \"Common.h\"       // DbgPrintF -- boot-trace sink that surfaces on the 360/emulator\n#include \"Display.h\"      // the Direct3D 9 device owner -- created FIRST (device race, see below)\n#include \"j2x_init.h\"     // generated: void j2x_initAll();\n#include \"HeapTrack.h\"    // heap-leak watchdog -- prints the PASS/FAIL verdict\n#include \"%s.h\"\n\n// The shell ctor takes the initial display size (Java passed the AWT canvas size). SCREEN_WIDTH\n// / SCREEN_HEIGHT are the backbuffer dims #defined in Common.h -- use those, do NOT redefine.\nvoid __cdecl main() {\n    // DEVICE FIRST: claim the global D3D9 device before the emulator's async shader thread can\n    // (the \"already active D3D device\" assertion). The shell's own later Display.create() no-ops.\n    Display::create();\n    HeapTrack::start();\n    DbgPrintF(\"[J2X] entry: device up; running ordered static init\\n\");\n    j2x_initAll();\n    DbgPrintF(\"[J2X] static init done; constructing %s\\n\");\n    %s* _game = new %s(%s);\n    DbgPrintF(\"[J2X] running game shell (Runnable.run)\\n\");\n    _game->run();\n    DbgPrintF(\"[J2X] game run() returned (exited)\\n\");\n}\n", name, name, name, name, name, args);
}

// Classify every reference so NONE can silently fail to integrate (the GLU flat-world bug: a
// GLU.reference.cpp existed but its GLU.reference.h did NOT, so emit_references -- which keys off .h --
// never emitted it and the game kept the no-op GLU stub -> flat 3D world).
//
// A reference X REPLACES the no-op X.h stub, so it needs an X.reference.h. An X.reference.cpp WITHOUT a
// matching .h can't replace the stub -> its real definitions are shadowed by the stub's inline no-ops.
// Returns {'complete': set(bases with .h), 'orphans': sorted[.cpp-only bases, excl. Main / *.ext]}.
AuditResult audit_references(const py::str& reference_dir) {
    if (reference_dir.empty() || !py::os::path::isdir(reference_dir)) {
        return AuditResult{py::set<py::str>(), py::list<py::str>()};
    }
    py::list<py::str> files = py::os::listdir(reference_dir);
    py::set<py::str> hs;
    py::set<py::str> cpps;
    for (const py::str& f : files) {
        if (f.endswith(py::str(".reference.h"))) {
            hs.add(f.slice_to(-py::len(py::str(".reference.h"))));
        }
    }
    for (const py::str& f : files) {
        if (f.endswith(py::str(".reference.cpp")) && !f.endswith(py::str(".ext.reference.cpp"))) {
            cpps.add(f.slice_to(-py::len(py::str(".reference.cpp"))));
        }
    }
    // a base whose header ships as a COMPANION (e.g. Textures.h) is already covered -> not an orphan
    py::str comp_dir = py::os::path::join(py::os::path::dirname(reference_dir), py::str("companions"));
    py::set<py::str> comp_hs;
    if (py::os::path::isdir(comp_dir)) {
        for (const py::str& f : py::os::listdir(comp_dir)) {
            if (f.endswith(py::str(".h"))) {
                comp_hs.add(f.slice_to(-2));
            }
        }
    }
    py::list<py::str> orphans;
    for (const py::str& b : cpps) {
        if (!hs.contains(b) && !SPECIAL_CPP_ONLY_.contains(b) && !comp_hs.contains(b)) {
            orphans.append(b);
        }
    }
    return AuditResult{hs, py::sorted(orphans)};
}

// After emission, confirm each reference base's tree X.h actually carries the PLATFORM REFERENCE banner
// -- i.e. the stub was REALLY replaced, not silently left as the no-op. Returns the bases that did NOT
// integrate (empty list = every reference landed).
py::list<py::str> verify_integrated(const py::str& out_dir, const py::set<py::str>& bases) {
    py::list<py::str> bad;
    for (const py::str& base : py::sorted(bases)) {
        py::str fp = py::os::path::join(out_dir, py::percent("%s.h", base));
        py::str head;
        try {
            head = read_head_(fp);
        } catch (const py::OSError&) {   // (IOError is OSError)
            bad.append(base);
            continue;
        }
        if (!head.contains(py::str("PLATFORM REFERENCE"))) {
            bad.append(base);
        }
    }
    return bad;
}

// Write each X.reference.{h,cpp} to out_dir/X.{h,cpp} (with a provenance banner), replacing the
// emitted stub. Returns the set of base names emitted. Also writes PLATFORM_LAYER.md.
//
// Detection-driven, NO game hardcoding:
//   * game_classes -- names the CONVERTED game defines itself. A platform reference/companion whose
//     base collides with one of these is SKIPPED: the game's own converted class wins (e.g. RubyDung
//     defines Textures/Tesselator/Vertex, so we never emit a Beta Textures/Tesselator over them --
//     that was the C2039 genId / Textures.ext mismatch). The agnostic platform layer only provides
//     the API surface the game IMPORTS but does not define (GL/GLU/NIO/Display/input/system).
//   * main_class -- if given, the 360 entry point is GENERATED generically to call
//     <main_class>::main(...), instead of the Beta-specific Main.reference.cpp (Minecraft.h include).
py::set<py::str> emit_references(const py::str& out_dir, const py::str& reference_dir,
                                 const py::set<py::str>& game_classes_in, const std::optional<py::str>& main_class,
                                 const py::ref<ir::JClass>& runnable_game, const py::str& platform) {
    py::set<py::str> emitted;
    py::list<py::str> files_written;
    py::set<py::str> game_classes = py::set<py::str>::from(game_classes_in);

    // A platform file collides if the game defines a class of the same name (its converted
    // .h/.cpp already occupies that slot). Also covers X.ext against a game-defined X.
    auto _collides = [&](const py::str& base) -> bool {
        py::str root = base.endswith(py::str(".ext")) ? base.slice_to(-4) : base;
        return game_classes.contains(base) || game_classes.contains(root);
    };
    // Clean up references that USED to be emitted but are now skipped: a prior convert wrote X.cpp/X.h
    // (carrying the PLATFORM REFERENCE banner); leaving them makes the vcxproj still compile the stale
    // reference (e.g. a skipped Audio.cpp -> unresolved XactAudio.h). Delete only OUR banner'd files, never
    // a stub the current run just wrote (stubs carry no banner) or a user file.
    for (const py::str& base : py::sorted(REFERENCE_SKIP)) {
        for (const py::str& ext : {py::str("cpp"), py::str("h")}) {
            py::str fp = py::os::path::join(out_dir, py::percent("%s.%s", base, ext));
            if (py::os::path::isfile(fp)) {
                try {
                    py::str head = read_head_(fp);
                    if (head.contains(py::str("PLATFORM REFERENCE")) || head.contains(py::str("PLATFORM COMPANION"))) {
                        py::os::remove(fp);
                    }
                } catch (const py::OSError&) {
                }
            }
        }
    }
    py::list<py::str> skipped_collisions;
    for (const py::str& base : py::sorted(available_references(reference_dir))) {
        if (REFERENCE_SKIP.contains(base)) {          // keep the converter stub (see REFERENCE_SKIP note)
            continue;
        }
        if (_collides(base)) {                        // game defines its own -> its converted class wins
            skipped_collisions.append(base);
            continue;
        }
        for (const py::str& ext : {py::str("h"), py::str("cpp")}) {
            py::str src = py::os::path::join(reference_dir, py::percent("%s.reference.%s", base, ext));
            if (!py::os::path::isfile(src)) {
                continue;
            }
            py::str content = codebreaker::java::textutil::read_text(src, py::str("replace"), false);
            py::str dst = py::os::path::join(out_dir, py::percent("%s.%s", base, ext));
            codebreaker::java::textutil::write_text(dst, _banner(base, ext, platform) + content);
            files_written.append(py::percent("%s.%s", base, ext));
        }
        // overwrite the per-type stub headers this reference replaces with a shim -> the real impl,
        // so the game's `#include "Mouse.h"` gets the controller mapping, not the no-op stub.
        for (const py::str& repl : REFERENCE_REPLACES.get(base, py::list<py::str>())) {
            if (repl == base) {
                continue;
            }
            py::str guard = repl.upper() + py::str("_H");
            py::str shim = py::percent("// J2X: %s.h is provided by the hand-written %s platform reference.\n#ifndef %s\n#define %s\n#include \"%s.h\"\n#endif // %s\n", repl, base, guard, guard, base, guard);
            codebreaker::java::textutil::write_text(py::os::path::join(out_dir, py::percent("%s.h", repl)), shim);
            files_written.append(py::percent("%s.h (-> %s.h)", repl, base));
        }
        emitted.add(base);
    }

    // COMPANIONS: game-agnostic platform files a reference .cpp links against but that aren't themselves
    // references (Common.h, Textures.h/.cpp, Tesselator.h/.cpp). Bundled in docs/platform/companions/ and
    // emitted verbatim so the platform layer is SELF-CONTAINED -- no hand-copying from the Alpha tree.
    py::str comp_dir = py::os::path::join(py::os::path::dirname(reference_dir), py::str("companions"));
    if (py::os::path::isdir(comp_dir)) {
        for (const py::str& fn : py::sorted(py::os::listdir(comp_dir))) {
            if (!(fn.endswith(py::str(".h")) || fn.endswith(py::str(".cpp")))) {
                continue;
            }
            py::str comp_base = fn.endswith(py::str(".cpp")) ? fn.slice_to(-4) : fn.slice_to(-2);
            if (comp_base == "Main" && main_class.has_value() && !main_class->empty()) {   // generic entry replaces the Beta Main companion
                continue;
            }
            if (_collides(comp_base)) {               // game defines its own -> skip the Beta companion
                skipped_collisions.append(comp_base);
                continue;
            }
            py::str content = codebreaker::java::textutil::read_text(py::os::path::join(comp_dir, fn), py::str("replace"), false);
            codebreaker::java::textutil::write_text(py::os::path::join(out_dir, fn), _companion_banner(fn) + content);
            files_written.append(fn);
            emitted.add(fn.endswith(py::str(".cpp")) ? fn.slice_to(-py::len(py::str(".cpp"))) : fn.slice_to(-py::len(py::str(".h"))));
        }
    }

    // Extension .cpp companions (X.ext.reference.cpp -> X.ext.cpp): extra out-of-line definitions bolted
    // onto a companion class -- Textures.ext defines genId/deleteId/uploadRGBA/uploadSubRGBA (declared in
    // Textures.h) that the GL11->D3D9 emulator forwards to. No .h of its own. Without it: LNK2019.
    for (const py::str& fn : py::sorted(py::os::listdir(reference_dir))) {
        if (fn.endswith(py::str(".ext.reference.cpp"))) {
            py::str base = fn.slice_to(-py::len(py::str(".reference.cpp")));        // e.g. 'Textures.ext'
            if (_collides(base)) {                    // Textures.ext collides with a game-defined Textures
                skipped_collisions.append(base);
                continue;
            }
            py::str content = codebreaker::java::textutil::read_text(py::os::path::join(reference_dir, fn), py::str("replace"), false);
            codebreaker::java::textutil::write_text(py::os::path::join(out_dir, base + py::str(".cpp")), _banner(base, py::str("cpp"), platform) + content);
            files_written.append(base + py::str(".cpp"));
            emitted.add(base);
        }
    }

    // Main.cpp: the 360 entry point. Detection-driven -- if the game has a detected main class, GENERATE
    // a generic entry that calls <main_class>::main(...) (no game hardcoding). Only fall back to the
    // Beta-specific Main.reference.cpp when NO main class was detected (and the game doesn't define Main).
    if (main_class.has_value() && !main_class->empty() && !_collides(py::str("Main"))) {
        codebreaker::java::textutil::write_text(py::os::path::join(out_dir, py::str("Main.cpp")),
                                                platform == "pc_opengl" ? generate_pc_entry_point(*main_class)
                                                                        : generate_entry_point(*main_class));
        files_written.append(py::percent("Main.cpp (generic entry -> %s::main)", *main_class));
        emitted.add(py::str("Main"));
    } else if (runnable_game != nullptr && !_collides(py::str("Main"))) {
        // No static main, but a concrete Runnable game shell (Indev's Minecraft): construct + run() it.
        codebreaker::java::textutil::write_text(py::os::path::join(out_dir, py::str("Main.cpp")),
                                                generate_runnable_entry_point(runnable_game));
        files_written.append(py::percent("Main.cpp (generic entry -> new %s().run())", runnable_game->name));
        emitted.add(py::str("Main"));
    } else if (!_collides(py::str("Main"))) {
        py::str main_ref = py::os::path::join(reference_dir, py::str("Main.reference.cpp"));
        if (py::os::path::isfile(main_ref)) {
            py::str content = codebreaker::java::textutil::read_text(main_ref, py::str("replace"), false);
            codebreaker::java::textutil::write_text(py::os::path::join(out_dir, py::str("Main.cpp")),
                                                    _banner(py::str("Main"), py::str("cpp"), platform) + content);
            files_written.append(py::str("Main.cpp"));
            emitted.add(py::str("Main"));
        }
    }

    _write_platform_layer_md(out_dir, py::sorted(emitted), files_written);
    emit_references_last_skipped = py::sorted(py::set<py::str>::from(skipped_collisions));   // for the pipeline progress line
    return emitted;
}

void _write_platform_layer_md(const py::str& out_dir, const py::list<py::str>& emitted,
                              const py::list<py::str>& files_written) {
    py::list<py::str> L = py::list<py::str>{
        py::str("# PLATFORM_LAYER.md -- the hand-written platform layer (emitted from references)"), py::str(""),
        py::str("The converter emitted the REAL Xbox 360 platform implementations (not no-op stubs) for these"),
        py::str("subsystems, from `docs/platform/reference/`:"), py::str(""),
        py::str("| Subsystem file | Emitted |"), py::str("|---|---|")};
    for (const py::str& base : emitted) {
        py::str cpp = py::percent("%s.cpp", base);
        L.append(py::percent("| `%s.h` %s | yes |", base,
                             files_written.contains(cpp) ? py::percent("+ `%s`", cpp) : py::str("(header-only)")));
    }
    L = L + py::list<py::str>{
        py::str(""), py::str("## Companion files (auto-emitted -- no hand-copying)"), py::str(""),
        py::str("The game-agnostic platform files a reference `.cpp` links against (Common.h, Textures.h/.cpp,"),
        py::str("Tesselator.h/.cpp) AND the 360 entry point (Main.cpp) are now emitted automatically from"),
        py::str("`docs/platform/companions/` + `Main.reference.cpp`. The full platform layer is self-contained;"),
        py::str("nothing needs to be copied from the Alpha tree."), py::str(""),
        py::str("## What is still missing"), py::str(""),
        py::str("See `PLATFORM_COVERAGE.md` -> the **MISSING** rows: symbols the game uses that neither the"),
        py::str("reference nor the stub provides. Those are the only per-version hand-fills left."), py::str("")};
    codebreaker::java::textutil::write_text(py::os::path::join(out_dir, py::str("PLATFORM_LAYER.md")),
                                            py::join(py::str("\n"), L) + py::str("\n"));
}

}  // namespace codebreaker::java::console::platform_refs
