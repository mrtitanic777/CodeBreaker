// cb/java/projects/minecraft/platform_coverage.hpp -- the platform API COVERAGE analyzer.
//
// The converter emits SYMBOL-ONLY stubs for every platform subsystem (video/GL, input, audio, file IO + save,
// networking, memory, display). This module scans the converted GAME code for every platform symbol it actually uses,
// then classifies each one against two sources of truth:
//
//   * the emitted STUB header      (`GL11.h`, `Keyboard.h`, ... in the output)   -> a NO-OP placeholder
//   * the hand-written REFERENCE   (`docs/platform/reference/*.reference.{h,cpp}`) -> the REAL implementation already
//     written for the Alpha port (GL->D3D9 emulator, XACT audio, XInput, XNet socket, ...)
//
// and emits `PLATFORM_COVERAGE.md`: per subsystem, every symbol the game needs with its status --
//
//   REUSABLE  it's in the Alpha reference -> the real implementation already exists; ship it as-is.
//   STUB      only a no-op placeholder exists -> works if the effect is cosmetic, silently wrong if it's load-bearing
//             (the glPolygonOffset z-fight class of bug).
//   MISSING   NEITHER the stub nor the reference provides it -> genuinely new work for this version (e.g. a GL call
//             Beta uses that Alpha never did).
//
// ...plus a menu of implementation OPTIONS per subsystem, so a human decides once, up front, how each platform
// question is answered -- instead of discovering the gaps one XDK build error at a time.
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/java/console/sdk.hpp"
#include "cb/java/console/xdk_scan.hpp"

namespace cb::java::projects::minecraft::platform_coverage {

// One platform subsystem: which platform holders/types belong to it, which reference files hold the real Alpha
// implementation, and the implementation-choice menu.
struct Subsystem {
    std::string_view key;
    std::string_view name;
    std::vector<std::string_view> holders;     // `Holder::symbol` classes (GL11, Keyboard ...)
    std::vector<std::string_view> types;       // platform types counted by use
    std::vector<std::string_view> reference;   // reference files (X.reference.h/.cpp) of the real implementation
    std::vector<std::string_view> options;     // the implementation menu
    std::vector<std::string_view> notes;       // considerations to verify even when every symbol is covered
};

std::span<const Subsystem> subsystems();

// ---- the placeholder surface ----------------------------------------------------------------------------------------
//
// Every no-op/placeholder in the emitted stubs AND the hand-written references carries a `// J2X-STUB[tag]: ...` marker.
// Scanning for it enumerates the ENTIRE placeholder surface, so a no-op that turns out to be load-bearing (the
// glPolygonOffset z-fight class) is never left silently undone -- `grep -rn J2X-STUB` is the audit list; the report
// folds it in by tag.

struct StubSite {
    std::string file;
    std::size_t line = 0;      // 1-based
    std::string note;          // the text after the marker, at most 100 bytes
};

// Tag -> its sites, tags in the order they were first met (files by name, lines in order).
using StubMarkers = OrderedMap<std::string, std::vector<StubSite>>;

// Every J2X-STUB marker in the *.cpp and *.h files directly in `output_dir`.
StubMarkers scan_stub_markers(const std::filesystem::path& output_dir);

// ---- what a header provides, and what the game uses --------------------------------------------------------------------

// The symbols a stub or reference header declares: function names, `const T NAME` and `#define NAME` constants
// (ALL_CAPS, at least three characters), and `NAME =` enumerators.
std::set<std::string> provided_symbols(std::string_view header_text);

// The `Holder::symbol` uses in the game code, with how many times each occurs, in the order each symbol first appears.
OrderedMap<std::string, std::size_t> used_holder_symbols(std::string_view game_text, std::string_view holder);

// How many times a platform TYPE is referenced (`new T`, `T*`, `T(`, `<T>`): whole-word occurrences not preceded by
// a word character or ':'.
std::size_t type_usage_count(std::string_view game_text, std::string_view type);

// ---- coverage -------------------------------------------------------------------------------------------------------

enum class Status { Reusable, Stub, Missing };

struct SymbolUse {
    std::string name;       // "GL11::glBegin"
    std::size_t count = 0;
    Status status = Status::Missing;
};

struct TypeUse {
    std::string name;
    std::size_t count = 0;
    bool in_reference = false;
};

struct Coverage {
    const Subsystem* sub = nullptr;
    std::vector<SymbolUse> symbols;   // by use, most used first
    std::vector<TypeUse> types;       // the types the game uses
    bool ref_exists = false;          // the subsystem has reference files and they declare something

    // The symbols of one status, in the order of `symbols`.
    std::vector<const SymbolUse*> with_status(Status s) const;
};

// Scan the converted output for platform symbol usage and classify coverage, one entry per subsystem.
// `runtime_files` are the emitted runtime/stub header names: the STUB source of truth (and left out of the "game
// code" scan).
std::vector<Coverage> analyze(const std::filesystem::path& output_dir,
                              const std::optional<std::filesystem::path>& reference_dir = std::nullopt,
                              const std::set<std::string>& runtime_files = {});

// The `## J2X-STUB markers` section of the report (empty when there are none), as report lines.
std::vector<std::string> render_stub_markers(const StubMarkers& markers);

// The PLATFORM_COVERAGE.md text. `sdk_caps` is what xdk_scan found in the installed SDK (requirements and
// opportunities are claimed only from it, never from memory); nullopt: no SDK was scanned.
std::string render_report(const std::vector<Coverage>& results,
                          const std::optional<std::filesystem::path>& reference_dir = std::nullopt,
                          const StubMarkers& stub_markers = {},
                          const std::optional<console::xdk_scan::ScanResult>& sdk_caps = std::nullopt);

struct WriteOptions {
    std::optional<std::filesystem::path> reference_dir;
    std::set<std::string> runtime_files;
    // The Xbox 360 SDK root to scan for requirements. Absent: the SDK is looked for with `sdk_hints`.
    std::optional<std::filesystem::path> sdk_root;
    console::sdk::SdkHints sdk_hints;
};

struct Written {
    std::filesystem::path path;           // <output_dir>/PLATFORM_COVERAGE.md
    std::vector<Coverage> results;
};

// Analyze and write PLATFORM_COVERAGE.md into `output_dir`. The platform requirements are AUTO-DETECTED from the
// installed SDK (not hardcoded) and best-effort: if no SDK can be resolved, the report says so instead of inventing
// requirements.
Written write_report(const std::filesystem::path& output_dir, const WriteOptions& options = {});

}  // namespace cb::java::projects::minecraft::platform_coverage
