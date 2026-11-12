// cb/java/console/platform_refs.hpp -- emit the hand-written platform layer instead of no-op stubs.
//
// The converter normally emits SYMBOL-ONLY stubs for the platform subsystems (GL11, Audio, Input, Socket, ...). With
// the platform layer on, this module emits the REAL, hand-written implementations from
// `docs/platform/reference/*.reference.{h,cpp}` in their place -- the GL->D3D9 emulator, XACT audio, XInput, XNet
// socket, etc., already proven on the Alpha port. The GL/platform API is version-independent, so these are directly
// reusable; platform_coverage reports the small per-version delta (the MISSING symbols) that still needs hand-filling.
//
// This is what makes the port NOT blind: the converter ships the reference code AND knows (via platform_coverage)
// exactly which symbols each version's game code needs that the reference doesn't yet provide -- instead of
// discovering it one XDK build error at a time.
//
// Caveat: a reference `.cpp` links against companion platform files that are NOT in the reference set (stdafx.h,
// Common.h, Textures.h/.cpp, Tesselator.h/.cpp) -- those are game-agnostic and bundled in `docs/platform/companions/`
// (next to the reference folder). emit_references() emits them too and writes a PLATFORM_LAYER.md listing what landed.
//
// The module also generates the title's entry point (Main.cpp) from the game's detected main class.
#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/ir.hpp"

namespace cb::java::console::platform_refs {

// The platform layer being emitted: which hand-written layer the references come from.
enum class Platform { Xbox360, PcOpenGl };

// The platform named on the command line ("xbox360", "pc_opengl"); every other name is the Xbox 360 layer (the layer
// the tool started with).
Platform platform_of(std::string_view name);

// A reference often provides MORE than one header's worth of symbols. The game #includes the per-type stub header
// (Mouse.h / Keyboard.h / SoundSystem.h), so emitting only `Input.h` / `Audio.h` would leave the no-op stub shadowing
// the real implementation -- the mouse-input-dead-on-controller trap. For each such reference the stubs it replaces are
// ALSO overwritten with a thin `#include` shim: reference base -> the stub headers it replaces.
const std::map<std::string, std::vector<std::string>, std::less<>>& reference_replaces();

// References left out on purpose: the converter's stub (which the game already compiles and links against) stands.
// Today only ZipFile (its reference needs the full InputStream/FileInputStream and the stub's getEntry() shape).
const std::set<std::string, std::less<>>& reference_skip();

// The base names X that have an X.reference.h in `reference_dir` ("GL11", "Audio", "Socket" ...); none when the folder
// is missing.
std::set<std::string> available_references(const std::filesystem::path& reference_dir);

// The game's entry class: the one declaring `public static void main(String[])`. Detection-driven -- no hardcoded game
// name -- so the generic entry point can invoke whatever main class the converted game actually has (RubyDung,
// Minecraft, anything).
std::optional<std::string> detect_main_class(std::span<const ir::JClass> classes);

// The game's shell class when it has NO `static main` but IS a Runnable started on a thread (Indev/Infdev: `Minecraft
// implements Runnable`, constructed by the applet and run() on a thread), or nullptr.
//
// Detection-driven, no hardcoded game name: the shell is a CONCRETE, TOP-LEVEL (no superclass) class that implements
// Runnable and declares run(). This is deliberately narrow so it does NOT fire for Alpha/Beta -- there `Minecraft` is
// ABSTRACT (the concrete `MinecraftImpl` has a parent), so both fail the concrete+top-level test and the Beta-specific
// Main.reference.cpp path is kept. A UI Runnable (Indev's GuiLoadLevel extends GuiScreen) is excluded by the
// no-superclass test. Answers nullptr when more than one class qualifies.
const ir::JClass* detect_runnable_game_class(std::span<const ir::JClass> classes);

// The GENERIC Xbox 360 entry point, generated from the detected main class. It runs ordered static init then hands
// control to the game's OWN `main(String[])`, which drives display creation and the game loop through the platform
// shims (Display/GL/Input). The platform device is created lazily by the Display/GL layer when the game calls
// Display.create, so the entry itself carries no game-specific bootstrap.
std::string generate_entry_point(std::string_view main_class);

// The PC entry: ordered static init, then the game's own main(String[]) with the command line, then -- as the JVM --
// wait for the game's non-daemon threads (the game makes its window on the thread that calls Display.create()).
// Resources load from a `Content` folder beside the program when there is one, else from the working folder.
std::string generate_pc_entry_point(std::string_view main_class);

// Default arguments to construct the Runnable game shell from the entry point: NULL for every reference parameter (no
// AWT frame/canvas/applet on the console), the display size for the first two int parameters, false for booleans, 0
// for other primitives. Uses the widest constructor.
std::string runnable_ctor_args(const ir::JClass& cls);

// The 360 entry point for a Runnable game shell with no static main (Indev): run ordered static init, construct the
// shell (NULL AWT args + display size), then call its Runnable run() -- the same thing the Java applet's thread does.
std::string generate_runnable_entry_point(const ir::JClass& cls);

// Classify every reference so NONE can silently fail to integrate (the GLU flat-world bug: a GLU.reference.cpp existed
// but its GLU.reference.h did NOT, so emit_references -- which keys off .h -- never emitted it and the game kept the
// no-op GLU stub -> flat 3D world).
//
// A reference X REPLACES the no-op X.h stub, so it needs an X.reference.h. An X.reference.cpp WITHOUT a matching .h
// can't replace the stub -> its real definitions are shadowed by the stub's inline no-ops.
struct Audit {
    std::set<std::string> complete;    // bases with a .reference.h
    std::vector<std::string> orphans;  // sorted .cpp-only bases (not Main, not *.ext, not covered by a companion)
};
Audit audit_references(const std::filesystem::path& reference_dir);

// After emission, confirm each reference base's tree X.h actually carries the PLATFORM REFERENCE banner -- the stub
// was REALLY replaced, not silently left as the no-op. Returns the bases that did NOT integrate (empty: every
// reference landed), sorted.
std::vector<std::string> verify_integrated(const std::filesystem::path& out_dir, const std::set<std::string>& bases);

struct EmitOptions {
    // Names the CONVERTED game defines itself. A platform reference/companion whose base collides with one of these
    // is SKIPPED: the game's own converted class wins (RubyDung defines Textures/Tesselator/Vertex, so a Beta
    // Textures/Tesselator is never emitted over them -- that was the C2039 genId / Textures.ext mismatch). The
    // agnostic platform layer only provides the API surface the game IMPORTS but does not define.
    std::set<std::string> game_classes;
    // If given, the entry point is GENERATED generically to call <main_class>::main(...), instead of the
    // Beta-specific Main.reference.cpp.
    std::optional<std::string> main_class;
    // A concrete Runnable shell with no static main (see detect_runnable_game_class); used when there is no main class.
    const ir::JClass* runnable_game = nullptr;
    Platform platform = Platform::Xbox360;
};

struct Emitted {
    std::set<std::string> bases;                // the base names emitted (references, companions, Main)
    std::vector<std::string> skipped_collisions;   // sorted: platform files skipped because the game defines the name
};

// Write each X.reference.{h,cpp} to out_dir/X.{h,cpp} (with a provenance banner), replacing the emitted stub, then the
// companions and the entry point. Also writes PLATFORM_LAYER.md. Detection-driven, no game hardcoding.
Emitted emit_references(const std::filesystem::path& out_dir, const std::filesystem::path& reference_dir,
                        const EmitOptions& options = {});

}  // namespace cb::java::console::platform_refs
