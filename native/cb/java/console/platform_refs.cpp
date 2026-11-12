#include "cb/java/console/platform_refs.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <system_error>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::console::platform_refs {

namespace {

namespace stdfs = std::filesystem;

// The files and folders of a folder, by name (unsorted); none for a folder that is missing.
std::vector<std::string> refs_names_in(const stdfs::path& dir) {
    std::vector<std::string> names;
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        names.push_back(fs::to_text(it->path().filename()));
    }
    return names;
}

bool refs_is_dir(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_directory(p, ec);
}

bool refs_is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

// The name without `suffix`, when it ends with it.
std::optional<std::string_view> refs_strip_suffix(const std::string& name, std::string_view suffix) {
    if (name.size() >= suffix.size() && name.ends_with(suffix)) return std::string_view(name).substr(0, name.size() - suffix.size());
    return std::nullopt;
}

// A base the game's own converted classes collide with. A platform file collides if the game defines a class of the
// same name (its converted .h/.cpp already occupies that slot); X.ext collides with a game-defined X as well.
bool refs_collides(const std::set<std::string>& game_classes, const std::string& base) {
    const std::string root = base.ends_with(".ext") ? base.substr(0, base.size() - 4) : base;
    return game_classes.contains(base) || game_classes.contains(root);
}

// The text of a file, "\n" line ends, undecodable bytes replaced; nothing when it cannot be read.
std::optional<std::string> refs_read(const stdfs::path& p) { return textutil::try_read_text(p); }

// True when the first 400 bytes of the file carry `needle`.
bool refs_head_has(const stdfs::path& p, std::string_view needle) {
    const auto text = refs_read(p);
    return text && std::string_view(*text).substr(0, 400).find(needle) != std::string_view::npos;
}

// "@KEY@" in the template becomes the value, for every pair.
std::string refs_fill(std::string tmpl, std::initializer_list<std::pair<std::string_view, std::string_view>> values) {
    for (const auto& [key, value] : values) tmpl = text::replace_all(tmpl, key, value);
    return tmpl;
}

}  // namespace

Platform platform_of(std::string_view name) { return name == "pc_opengl" ? Platform::PcOpenGl : Platform::Xbox360; }

const std::map<std::string, std::vector<std::string>, std::less<>>& reference_replaces() {
    static const std::map<std::string, std::vector<std::string>, std::less<>> m = {
        {"Input", {"Keyboard", "Mouse", "Controllers"}},   // Input.reference defines namespace Keyboard/Mouse/Controllers
        {"ImageIO", {"ImageIO"}},
        // Audio.reference DEFINES the real SoundSystem (paulscode -> XACT3 bridge), but the game #includes
        // "SoundSystem.h" -- so that stub is shimmed to the real Audio.h, else the no-op SoundSystem stub shadows the
        // bridge and every playSoundFX is silent (and the empty pool crashed).
        {"Audio", {"SoundSystem"}},
        // NIOBuffers.reference DEFINES ByteBuffer/FloatBuffer/IntBuffer (the real byte-granular NIO buffers the GL
        // emulator + Tessellator use). Without this, its class defs collide with the converter's ByteBuffer.h/
        // FloatBuffer.h/IntBuffer.h stubs -> C2011 redefinition in every TU that pulls both.
        {"NIOBuffers", {"ByteBuffer", "FloatBuffer", "IntBuffer"}},
        // FileIO does NOT shim the byte streams: the real java.io byte streams (OutputStream/BufferedOutputStream/
        // DataInput/DataOutput/DataInputStream/DataOutputStream) are CANONICAL standalone headers emitted by the
        // runtime (loaded from docs/platform/canonical/), and FileIO.reference.h just #includes them -- one definition
        // each, no shim, no redefinition (a shim here exploded in save-path TUs: C2011).
    };
    return m;
}

// ZipFile: its reference needs getEntry()->ZipEntry* of the stub and the full InputStream/FileInputStream.
// (Audio, Socket and Thread were skipped once and are emitted now: Audio's blockers are resolved -- the XACT3 bridge and
// its XactAudio companion are bundled; Socket's reference returns SocketAddress*/InetAddress* as the game expects;
// Thread's reference is the GATED version whose start() is a no-op by default, so the dead download/stats daemons stay
// dead and the first menu frame does not freeze, while the net reader/writer threads start for real.)
const std::set<std::string, std::less<>>& reference_skip() {
    static const std::set<std::string, std::less<>> s = {"ZipFile"};
    return s;
}

std::set<std::string> available_references(const stdfs::path& reference_dir) {
    std::set<std::string> out;
    for (const std::string& name : refs_names_in(reference_dir)) {
        if (auto base = refs_strip_suffix(name, ".reference.h")) out.insert(std::string(*base));
    }
    return out;
}

std::optional<std::string> detect_main_class(std::span<const ir::JClass> classes) {
    for (const ir::JClass& c : classes) {
        for (const ir::Method& m : c.methods) {
            if (m.name != "main" || !m.is_static || m.params.size() != 1) continue;
            const ir::JType& pt = m.params[0].type;
            if (pt.is_array() && (pt.name == "String" || pt.name == "java.lang.String")) return c.name;
        }
    }
    return std::nullopt;
}

const ir::JClass* detect_runnable_game_class(std::span<const ir::JClass> classes) {
    const ir::JClass* found = nullptr;
    for (const ir::JClass& c : classes) {
        if (std::find(c.interfaces.begin(), c.interfaces.end(), "Runnable") == c.interfaces.end()) continue;
        if (c.is_abstract || c.is_interface) continue;
        if (c.parent && *c.parent != "Object") continue;
        if (c.method("run") == nullptr) continue;
        if (found != nullptr) return nullptr;   // ambiguous
        found = &c;
    }
    return found;
}

std::string generate_entry_point(std::string_view main_class) {
    static const std::string_view kTemplate = R"cpp(// ============================ GENERIC ENTRY POINT ============================
// Auto-generated by CodeBreaker. The 360 title entry: run ordered Java static init, then call
// the game's detected main class. Device/display setup is driven by the game's own code
// through the platform shims (Display/GL11/Keyboard) -- no game-specific bootstrap here.
// (Detected main class: @MAIN@. Regenerated every convert from whatever main() the game has.)
// ==========================================================================

#include "stdafx.h"
#include "JavaRT.h"
#include "Common.h"       // DbgPrintF -- the boot-trace sink that surfaces on the 360/emulator
#include "Display.h"      // the Direct3D 9 device owner -- created FIRST (see below)
#include "j2x_init.h"     // generated: void j2x_initAll();
#include "HeapTrack.h"    // heap-leak watchdog -- start()s the monitor that prints the PASS/FAIL verdict
#include "@MAIN@.h"

void __cdecl main() {
    // DEVICE FIRST. Create the D3D9 device before anything else -- so the title claims the global
    // device (VdGlobalDevice) before the emulator's ASYNC shader-cache thread can. Creating it late
    // (after static init + the game thread) loses that race and trips the "already active D3D device"
    // assertion. The working hand-port creates its device first thing in main() for exactly this
    // reason; the game's own later Display.create() is a guarded no-op (and setDisplayMode is
    // ignored once created, so the backbuffer dims stay consistent).
    Display::create();
    // Start the heap-leak watchdog's monitor thread NOW (its operator new/delete hooks already track
    // every allocation; this launches the 2.5s reporter that prints "[HEAP] LEAK VERDICT: PASS/FAIL").
    // Compiles to a no-op when J2X_HEAPTRACK=0. This is what surfaces the leak result on the emulator.
    HeapTrack::start();
    // Boot trace via DbgPrintF (mirrors through HostPathDevice::ResolvePath, the channel that
    // always surfaces on this console/emulator). Brackets the two transitions that black-screen
    // a launch: did static init finish, and did the game's own main() get control.
    DbgPrintF("[J2X] entry: device up; running ordered static init\n");
    // Ordered static init: every class's Java static{} blocks + static field initializers,
    // in dependency order (prevents the static-init-order null-vtable crash class).
    j2x_initAll();
    DbgPrintF("[J2X] static init done; calling game main\n");

    // Hand off to the game's own entry point. No command-line args on console.
    JArray<JavaString> args(0);   // Java String[] -> JArray<JavaString> (JavaString is a value type)
    @MAIN@::main(args);
    DbgPrintF("[J2X] game main returned (exited)\n");
}
)cpp";
    return refs_fill(std::string(kTemplate), {{"@MAIN@", main_class}});
}

std::string generate_pc_entry_point(std::string_view main_class) {
    static const std::string_view kTemplate = R"cpp(// ============================ GENERIC ENTRY POINT (PC) ============================
// Auto-generated by CodeBreaker from the detected main class (@MAIN@).
// ==========================================================================

#include "stdafx.h"
#include "JavaRT.h"
#include "Common.h"
#include "Thread.h"
#include "j2x_init.h"
#include "@MAIN@.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>
#ifdef _WIN32
#include <direct.h>
#define j2x_chdir _chdir
#else
#include <unistd.h>
#define j2x_chdir chdir
#endif

int main(int argc, char** argv) {
    SDL_SetMainReady();
    char* base = SDL_GetBasePath();
    if (base) {
        std::string content = std::string(base) + "Content";
        if (j2x_chdir(content.c_str()) != 0) j2x_chdir(base);
        SDL_free(base);
    }
    DbgPrintF("[J2X] running ordered static init\n");
    j2x_initAll();
    JArray<JavaString> args(argc > 1 ? argc - 1 : 0);
    for (int i = 1; i < argc; ++i) args[i - 1] = JavaString(argv[i]);
    @MAIN@::main(args);
    j2xThreadsWait();
    return 0;
}
)cpp";
    return refs_fill(std::string(kTemplate), {{"@MAIN@", main_class}});
}

std::string runnable_ctor_args(const ir::JClass& cls) {
    // The widest constructor: the first of the longest parameter lists.
    const ir::Method* widest = nullptr;
    for (const ir::Method* ctor : cls.constructors()) {
        if (widest == nullptr || ctor->params.size() > widest->params.size()) widest = ctor;
    }
    if (widest == nullptr) return {};
    static const std::set<std::string, std::less<>> kPrimitives = {"int", "long", "short", "byte",
                                                                    "char", "boolean", "float", "double"};
    std::vector<std::string> args;
    int ints = 0;
    for (const ir::Param& p : widest->params) {
        const ir::JType& t = p.type;
        if (t.name == "int" && !t.is_array()) {
            ++ints;
            args.push_back(ints == 1 ? "SCREEN_WIDTH" : ints == 2 ? "SCREEN_HEIGHT" : "0");
        } else if (t.name == "boolean" && !t.is_array()) {
            args.push_back("false");
        } else if (kPrimitives.contains(t.name) && !t.is_array()) {
            args.push_back("0");
        } else {
            args.push_back("NULL");
        }
    }
    return text::join(args, ", ");
}

std::string generate_runnable_entry_point(const ir::JClass& cls) {
    static const std::string_view kTemplate = R"cpp(// ============================ GENERIC ENTRY POINT (Runnable) ============================
// Auto-generated by CodeBreaker. The game shell (@NAME@) has no static main -- Java starts it as a
// Runnable on a thread. The 360 title entry does the same: ordered static init, construct the
// shell (no AWT frame/canvas on console -> NULL), then call run(). No game-specific bootstrap;
// display/GL/input are driven by the shell through the platform shims.
// =======================================================================================

#include "stdafx.h"
#include "JavaRT.h"
#include "Common.h"       // DbgPrintF -- boot-trace sink that surfaces on the 360/emulator
#include "Display.h"      // the Direct3D 9 device owner -- created FIRST (device race, see below)
#include "j2x_init.h"     // generated: void j2x_initAll();
#include "HeapTrack.h"    // heap-leak watchdog -- prints the PASS/FAIL verdict
#include "@NAME@.h"

// The shell ctor takes the initial display size (Java passed the AWT canvas size). SCREEN_WIDTH
// / SCREEN_HEIGHT are the backbuffer dims #defined in Common.h -- use those, do NOT redefine.
void __cdecl main() {
    // DEVICE FIRST: claim the global D3D9 device before the emulator's async shader thread can
    // (the "already active D3D device" assertion). The shell's own later Display.create() no-ops.
    Display::create();
    HeapTrack::start();
    DbgPrintF("[J2X] entry: device up; running ordered static init\n");
    j2x_initAll();
    DbgPrintF("[J2X] static init done; constructing @NAME@\n");
    @NAME@* _game = new @NAME@(@ARGS@);
    DbgPrintF("[J2X] running game shell (Runnable.run)\n");
    _game->run();
    DbgPrintF("[J2X] game run() returned (exited)\n");
}
)cpp";
    return refs_fill(std::string(kTemplate), {{"@NAME@", cls.name}, {"@ARGS@", runnable_ctor_args(cls)}});
}

Audit audit_references(const stdfs::path& reference_dir) {
    Audit out;
    if (!refs_is_dir(reference_dir)) return out;
    std::set<std::string> cpps;
    for (const std::string& name : refs_names_in(reference_dir)) {
        if (auto base = refs_strip_suffix(name, ".reference.h")) out.complete.insert(std::string(*base));
        if (!name.ends_with(".ext.reference.cpp")) {
            if (auto base = refs_strip_suffix(name, ".reference.cpp")) cpps.insert(std::string(*base));
        }
    }
    // A base whose header ships as a COMPANION (e.g. Textures.h) is already covered -> not an orphan.
    std::set<std::string> companion_headers;
    for (const std::string& name : refs_names_in(reference_dir.parent_path() / "companions")) {
        if (auto base = refs_strip_suffix(name, ".h")) companion_headers.insert(std::string(*base));
    }
    for (const std::string& base : cpps) {   // sorted
        if (!out.complete.contains(base) && base != "Main" && !companion_headers.contains(base)) out.orphans.push_back(base);
    }
    return out;
}

std::vector<std::string> verify_integrated(const stdfs::path& out_dir, const std::set<std::string>& bases) {
    std::vector<std::string> bad;
    for (const std::string& base : bases) {   // sorted
        if (!refs_head_has(out_dir / (base + ".h"), "PLATFORM REFERENCE")) bad.push_back(base);
    }
    return bad;
}

namespace {

// The provenance banner above an emitted reference file.
std::string refs_banner(std::string_view base, std::string_view ext, Platform platform) {
    const bool pc = platform == Platform::PcOpenGl;
    const std::string_view label = pc ? "PC (SDL2 + OpenGL)" : "Xbox 360";
    const std::string_view where = pc ? "codebreaker/java/platform/pc/reference" : "codebreaker/java/platform/reference";
    return std::format(
        "// ============================ PLATFORM REFERENCE ============================\n"
        "// Hand-written {0} platform layer, emitted from\n"
        "//   {1}/{2}.reference.{3}\n"
        "// This is the REAL implementation -- it REPLACES the converter no-op stub for {2}.\n"
        "// (NOT converter-generated game code; edit the .reference.{3} source, then reconvert.)\n"
        "// ==========================================================================\n\n",
        label, where, base, ext);
}

std::string refs_companion_banner(std::string_view file_name) {
    return std::format(
        "// ============================ PLATFORM COMPANION ============================\n"
        "// Game-agnostic Xbox 360 platform file, emitted by CodeBreaker from\n"
        "//   docs/platform/companions/{}\n"
        "// A reference .cpp links against this; it is version-independent (edit the companion\n"
        "// source, then reconvert). NOT converter-generated game code.\n"
        "// ==========================================================================\n\n",
        file_name);
}

void refs_write(const stdfs::path& out_dir, const std::string& name, std::string_view content) {
    textutil::write_text(out_dir / fs::from_text(name), content);
}

void refs_write_layer_md(const stdfs::path& out_dir, const std::set<std::string>& emitted,
                         const std::vector<std::string>& files_written) {
    std::string md =
        "# PLATFORM_LAYER.md -- the hand-written platform layer (emitted from references)\n\n"
        "The converter emitted the REAL Xbox 360 platform implementations (not no-op stubs) for these\n"
        "subsystems, from `docs/platform/reference/`:\n\n"
        "| Subsystem file | Emitted |\n|---|---|\n";
    for (const std::string& base : emitted) {
        const std::string cpp = base + ".cpp";
        const bool has_cpp = std::find(files_written.begin(), files_written.end(), cpp) != files_written.end();
        md += std::format("| `{}.h` {} | yes |\n", base, has_cpp ? "+ `" + cpp + "`" : "(header-only)");
    }
    md +=
        "\n## Companion files (auto-emitted -- no hand-copying)\n\n"
        "The game-agnostic platform files a reference `.cpp` links against (Common.h, Textures.h/.cpp,\n"
        "Tesselator.h/.cpp) AND the 360 entry point (Main.cpp) are now emitted automatically from\n"
        "`docs/platform/companions/` + `Main.reference.cpp`. The full platform layer is self-contained;\n"
        "nothing needs to be copied from the Alpha tree.\n\n"
        "## What is still missing\n\n"
        "See `PLATFORM_COVERAGE.md` -> the **MISSING** rows: symbols the game uses that neither the\n"
        "reference nor the stub provides. Those are the only per-version hand-fills left.\n";
    refs_write(out_dir, "PLATFORM_LAYER.md", md);
}

}  // namespace

Emitted emit_references(const stdfs::path& out_dir, const stdfs::path& reference_dir, const EmitOptions& options) {
    Emitted result;
    std::vector<std::string> files_written;
    const std::set<std::string>& game = options.game_classes;
    const Platform platform = options.platform;

    // Clean up references that USED to be emitted but are now skipped: a prior convert wrote X.cpp/X.h (carrying the
    // PLATFORM REFERENCE banner); leaving them makes the vcxproj still compile the stale reference. Delete only OUR
    // banner'd files, never a stub the current run just wrote (stubs carry no banner) or a user file.
    for (const std::string& base : reference_skip()) {
        for (const char* ext : {"cpp", "h"}) {
            const stdfs::path fp = out_dir / (base + "." + ext);
            if (!refs_is_file(fp)) continue;
            if (refs_head_has(fp, "PLATFORM REFERENCE") || refs_head_has(fp, "PLATFORM COMPANION")) {
                std::error_code ec;
                stdfs::remove(fp, ec);   // best effort, as the stale file is only a leftover
            }
        }
    }

    std::set<std::string> collisions;
    for (const std::string& base : available_references(reference_dir)) {   // sorted
        if (reference_skip().contains(base)) continue;   // keep the converter stub
        if (refs_collides(game, base)) {                  // the game defines its own: its converted class wins
            collisions.insert(base);
            continue;
        }
        for (const char* ext : {"h", "cpp"}) {
            const auto content = refs_read(reference_dir / (base + ".reference." + ext));
            if (!content) continue;
            refs_write(out_dir, base + "." + ext, refs_banner(base, ext, platform) + *content);
            files_written.push_back(base + "." + ext);
        }
        // Overwrite the per-type stub headers this reference replaces with a shim -> the real implementation, so the
        // game's `#include "Mouse.h"` gets the controller mapping, not the no-op stub.
        if (const auto it = reference_replaces().find(base); it != reference_replaces().end()) {
            for (const std::string& repl : it->second) {
                if (repl == base) continue;
                const std::string guard = text::upper(repl) + "_H";
                refs_write(out_dir, repl + ".h",
                           std::format("// J2X: {0}.h is provided by the hand-written {1} platform reference.\n"
                                       "#ifndef {2}\n#define {2}\n#include \"{1}.h\"\n#endif // {2}\n",
                                       repl, base, guard));
                files_written.push_back(repl + ".h (-> " + base + ".h)");
            }
        }
        result.bases.insert(base);
    }

    // COMPANIONS: game-agnostic platform files a reference .cpp links against but that aren't themselves references
    // (Common.h, Textures.h/.cpp, Tesselator.h/.cpp). Bundled in docs/platform/companions/ and emitted verbatim so the
    // platform layer is SELF-CONTAINED -- no hand-copying from the Alpha tree.
    const stdfs::path companions = reference_dir.parent_path() / "companions";
    if (refs_is_dir(companions)) {
        std::vector<std::string> names = refs_names_in(companions);
        std::sort(names.begin(), names.end());
        for (const std::string& fn : names) {
            const auto cpp_base = refs_strip_suffix(fn, ".cpp");
            const auto h_base = refs_strip_suffix(fn, ".h");
            if (!cpp_base && !h_base) continue;
            const std::string base(cpp_base ? *cpp_base : *h_base);
            if (base == "Main" && options.main_class && !options.main_class->empty()) continue;   // the generic entry replaces it
            if (refs_collides(game, base)) {   // the game defines its own: skip the companion
                collisions.insert(base);
                continue;
            }
            const auto content = refs_read(companions / fn);
            if (!content) continue;
            refs_write(out_dir, fn, refs_companion_banner(fn) + *content);
            files_written.push_back(fn);
            result.bases.insert(base);
        }
    }

    // Extension .cpp companions (X.ext.reference.cpp -> X.ext.cpp): extra out-of-line definitions bolted onto a
    // companion class -- Textures.ext defines genId/deleteId/uploadRGBA/uploadSubRGBA (declared in Textures.h) that the
    // GL11->D3D9 emulator forwards to. No .h of its own. Without it: LNK2019.
    std::vector<std::string> ref_names = refs_names_in(reference_dir);
    std::sort(ref_names.begin(), ref_names.end());
    for (const std::string& fn : ref_names) {
        const auto stripped = refs_strip_suffix(fn, ".reference.cpp");
        if (!stripped || !stripped->ends_with(".ext")) continue;   // e.g. 'Textures.ext'
        const std::string base(*stripped);
        if (refs_collides(game, base)) {                    // Textures.ext collides with a game-defined Textures
            collisions.insert(base);
            continue;
        }
        const auto content = refs_read(reference_dir / fn);
        if (!content) continue;
        refs_write(out_dir, base + ".cpp", refs_banner(base, "cpp", platform) + *content);
        files_written.push_back(base + ".cpp");
        result.bases.insert(base);
    }

    // Main.cpp: the entry point. Detection-driven -- if the game has a detected main class, GENERATE a generic entry
    // that calls <main_class>::main(...) (no game hardcoding). Only fall back to the Beta-specific Main.reference.cpp
    // when NO main class was detected (and the game doesn't define Main).
    if (!refs_collides(game, "Main")) {
        if (options.main_class && !options.main_class->empty()) {
            refs_write(out_dir, "Main.cpp",
                       platform == Platform::PcOpenGl ? generate_pc_entry_point(*options.main_class)
                                                      : generate_entry_point(*options.main_class));
            files_written.push_back("Main.cpp (generic entry -> " + *options.main_class + "::main)");
            result.bases.insert("Main");
        } else if (options.runnable_game != nullptr) {
            // No static main, but a concrete Runnable game shell (Indev's Minecraft): construct it and run() it.
            refs_write(out_dir, "Main.cpp", generate_runnable_entry_point(*options.runnable_game));
            files_written.push_back("Main.cpp (generic entry -> new " + options.runnable_game->name + "().run())");
            result.bases.insert("Main");
        } else if (const auto main_ref = refs_read(reference_dir / "Main.reference.cpp")) {
            refs_write(out_dir, "Main.cpp", refs_banner("Main", "cpp", platform) + *main_ref);
            files_written.push_back("Main.cpp");
            result.bases.insert("Main");
        }
    }

    refs_write_layer_md(out_dir, result.bases, files_written);
    result.skipped_collisions.assign(collisions.begin(), collisions.end());
    return result;
}

}  // namespace cb::java::console::platform_refs
