// codebreaker/java/projects/minecraft/platform_coverage.hpp -- C++ from codebreaker.java.projects.minecraft.platform_coverage.
//
// platform_coverage.py -- the platform API COVERAGE analyzer.
//
// The converter emits SYMBOL-ONLY stubs for every platform subsystem (video/GL, input, audio, file
// IO + save, networking, memory, display). This module scans the converted GAME code for every
// platform symbol it actually uses, then classifies each one against two sources of truth:
//
//   * the emitted STUB header      (`GL11.h`, `Keyboard.h`, ... in the output)   -> a NO-OP placeholder
//   * the hand-written REFERENCE   (`docs/platform/reference/*.reference.{h,cpp}`) -> the REAL impl
//     already written for the Alpha port (GL->D3D9 emulator, XACT audio, XInput, XNet socket, ...)
//
// and emits `PLATFORM_COVERAGE.md`: per subsystem, every symbol the game needs with its status --
//
//   REUSABLE  it's in the Alpha reference -> the real implementation already exists; ship it as-is.
//   STUB      only a no-op placeholder exists -> works if the effect is cosmetic, silently wrong if
//             it's load-bearing (the glPolygonOffset z-fight class of bug).
//   MISSING   NEITHER the stub nor the reference provides it -> genuinely new work for this version
//             (e.g. a GL call Beta uses that Alpha never did).
//
// ...plus a menu of implementation OPTIONS per subsystem, so a human decides once, up front, how each
// platform question is answered -- instead of discovering the gaps one XDK build error at a time.
//
// Typing notes: a SUBSYSTEMS entry (a dict with fixed keys) is the Subsystem record; analyze()'s per-subsystem
// dict is the Coverage record; the (symbol, count, status) and (type, count, has_ref) tuples are SymbolUse and
// TypeUse; sdk_caps (xdk_scan.detect's dict, or None) is an optional ScanResult.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/java/console/xdk_scan.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::platform_coverage {

// One SUBSYSTEMS entry: which platform holders/types belong to the subsystem, which reference files hold
// the real Alpha implementation, and the implementation-choice menu. `notes` is absent from most entries
// (the Python's sub.get('notes')): empty here.
struct Subsystem {
    py::str key;
    py::str name;
    py::list<py::str> holders;
    py::list<py::str> types;
    py::list<py::str> reference;
    py::list<py::str> options;
    py::list<py::str> notes;
};

// ---------------------------------------------------------------------------
// Subsystem map: which platform holders/types belong to each, which reference
// files hold the real Alpha implementation, and the implementation-choice menu.
// ---------------------------------------------------------------------------
extern const py::list<Subsystem> SUBSYSTEMS;

// tolerant: errors='replace', missing -> ''
py::str _read(const py::str& path);

// Every no-op/placeholder in the emitted stubs AND the hand-written references carries a
// `// J2X-STUB[tag]: ...` marker. Scanning for it enumerates the ENTIRE placeholder surface, so a
// no-op that turns out to be load-bearing (the glPolygonOffset z-fight class) is never left silently
// undone -- `grep -rn J2X-STUB` is the audit list; this folds it into the coverage report by tag.
// (re.compile(r'J2X-STUB\[([a-z0-9-]+)\]\s*:\s*([^\n]*)'), the others likewise, compiled on first use)
const py::re::Pattern& _STUB_MARKER();

// (file, line, note)
using StubSite = std::tuple<py::str, int64_t, py::str>;
using StubMarkers = py::dict<py::str, py::list<StubSite>>;

// Return {tag: [(file, line, note)]} for every J2X-STUB marker in the emitted tree.
StubMarkers scan_stub_markers(const py::str& output_dir);

// ---------------------------------------------------------------------------
// What a header PROVIDES: declared function names + named constants.
// ---------------------------------------------------------------------------
// _FN_DECL = \b([A-Za-z_]\w*)\s*\(
// _CONST_DECL = \b(?:const\s+\w[\w:<>]*|#define)\s+([A-Z][A-Z0-9_]{2,})\b
// _ENUM_CONST = \b([A-Z][A-Z0-9_]{2,})\s*=
const py::re::Pattern& _FN_DECL();
const py::re::Pattern& _CONST_DECL();
const py::re::Pattern& _ENUM_CONST();

// Symbols a stub/reference header declares (function names + ALL_CAPS constants).
py::set<py::str> _provided_symbols(const py::str& header_text);

// ---------------------------------------------------------------------------
// What the GAME code USES from each holder / platform type.
// ---------------------------------------------------------------------------
// `Holder::symbol` occurrences in the game code -> {symbol: count}.
py::dict<py::str, int64_t> _used_holder_symbols(const py::str& game_text, const py::str& holder);

// How many times a platform TYPE is referenced (new T / T* / T( / <T>).
int64_t _type_usage_count(const py::str& game_text, const py::str& typ);

// (symbol, count, status) with status 'REUSABLE' | 'STUB' | 'MISSING'
using SymbolUse = std::tuple<py::str, int64_t, py::str>;
// (type, count, has a reference)
using TypeUse = std::tuple<py::str, int64_t, bool>;

// One per-subsystem result of analyze().
struct Coverage {
    Subsystem sub;
    py::list<SymbolUse> symbols;
    py::list<TypeUse> types;
    bool ref_exists = false;
    py::list<SymbolUse> missing;
    py::list<SymbolUse> stub;
    py::list<SymbolUse> reusable;
};

// Scan the converted output for platform symbol usage and classify coverage. Returns a list of
// per-subsystem dicts. `runtime_files` = set of emitted runtime/stub header basenames to treat as
// the STUB source-of-truth (and to exclude from the 'game code' scan).
py::list<Coverage> analyze(const py::str& output_dir, const std::optional<py::str>& reference_dir = std::nullopt,
                           const std::optional<py::set<py::str>>& runtime_files = std::nullopt);

// ---------------------------------------------------------------------------
py::list<py::str> _render_stub_markers(const StubMarkers& markers);

py::str render_report(const py::list<Coverage>& results, const std::optional<py::str>& reference_dir = std::nullopt,
                      const std::optional<StubMarkers>& stub_markers = std::nullopt,
                      const std::optional<codebreaker::java::console::xdk_scan::ScanResult>& sdk_caps = std::nullopt);

// -> (the report's path, the analysis)
std::tuple<py::str, py::list<Coverage>> write_report(const py::str& output_dir, const std::optional<py::str>& reference_dir = std::nullopt,
                                                     const std::optional<py::set<py::str>>& runtime_files = std::nullopt,
                                                     const std::optional<py::str>& sdk_root = std::nullopt);

}  // namespace codebreaker::java::projects::minecraft::platform_coverage
