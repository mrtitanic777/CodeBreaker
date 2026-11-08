// codebreaker/java/console/assets.hpp -- C++ port of codebreaker.java.console.assets: extract the game's
// shipped assets (textures, language files, fonts, misc resources) out of the original Minecraft jar and lay
// them down in the converted project's Content folder with the EXACT same relative paths and names, and audit
// which resource paths the code loads vs which the jar provides.
//
// NOT PORTED: the PNG re-encode of normalize_png_to_rgba. Python does it with Pillow (an optional import);
// the runtime has no PNG codec, so this port behaves as Python does when Pillow is NOT installed:
// _HAS_PIL is false, a non-RGBA PNG is passed through unchanged, and run() lists it under `unconverted`.
// (Python with Pillow installed would decode it and re-encode it as 8-bit RGBA instead.)
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::assets {

// zipfile.BadZipFile (raised on a file that is not a zip, a bad header, a bad CRC)
struct BadZipFile : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "BadZipFile"; }
};

// `try: from PIL import Image` -- Pillow is not available to the C++ runtime (see above).
inline constexpr bool _HAS_PIL = false;

// Asset file extensions the game ships in its jar and loads at runtime.
extern const py::list<py::str> _ASSET_EXTS;

// A resource-path string literal as it appears in code: "/dir/name.ext" (leading slash, shipped file).
// Identical in Java source and in the converted C++ (JavaString literals keep the same text).
extern const py::re::Pattern _RES_PATH;

extern const py::list<py::str> _CODE_EXTS;

// strip // line comments and /* block */ comments so a documentation example like `"/x.png"` in a
// comment is not mistaken for a real resource load (false MISSING).
extern const py::re::Pattern _LINE_COMMENT;
extern const py::re::Pattern _BLOCK_COMMENT;

py::str _strip_comments(const py::str& text);

// --- PNG normalization: force to 8-bit straight-alpha RGBA (colortype 6) ------------------------

// PNG IHDR color type (0 gray, 2 rgb, 3 indexed, 4 gray+alpha, 6 rgba), or None if not a PNG.
std::optional<int64_t> _png_colortype(const py::bytes& raw);

// True if this PNG is not already 8-bit straight RGBA (i.e. would misrender on the 360).
bool png_needs_rgba(const py::bytes& raw);

// Return (bytes, changed). Without Pillow (see the top of this file) a PNG is returned unchanged.
std::tuple<py::bytes, bool> normalize_png_to_rgba(const py::bytes& raw);

// Return the sorted list of asset entry paths (png/txt/lang/...) inside the jar, using forward
// slashes and no leading slash -- exactly how they map under Content\.
py::list<py::str> list_jar_assets(const py::str& jar_path);

// Every resource path the code loads, as {path WITHOUT the leading slash: sorted files that reference it}.
// Scans .java and/or converted .cpp/.h -- the literal is the same in both.
py::dict<py::str, py::list<py::str>> code_resource_refs(const py::str& code_dir);

// CONSOLE-ONLY assets that no vanilla jar ships but the console platform layer needs (e.g. the controller
// cursor icon drawn by EntityRenderer in container GUIs -- see controller_nav.py). Bundled in the converter
// and copied into Content alongside the jar extraction so the feature is never a per-project manual asset.
// (Python computes it once at import: data_path('platform', 'assets'); here it is computed on first use, after
// the host has set _paths' folder, and kept.)
const py::str& _CONSOLE_ASSETS_DIR();

// Mirror the bundled console-only assets into content_dir (same relative paths). Returns the list of
// relative paths written. Skips silently if the bundle dir is absent.
py::list<py::str> place_console_assets(const py::str& content_dir);

// Mirror the jar's asset entries into content_dir, preserving each entry's relative path and name (jar
// `gui/gui.png` -> content_dir/gui/gui.png). If `only` (a set of slash-less paths) is given, extract just
// those; otherwise extract every asset. When normalize_png is on, any non-RGBA PNG is converted to RGBA8888
// on the way out (see the top of this file). If `converted` is given, the names of PNGs that were normalized
// are appended to it (the list is a shared handle). Returns the sorted list of paths written.
py::list<py::str> extract(const py::str& jar_path, const py::str& content_dir,
                          const std::optional<py::set<py::str>>& only = std::nullopt, bool normalize_png = true,
                          const std::optional<py::list<py::str>>& converted = std::nullopt);

// Bundled console-only asset rel-paths (forward slash, no leading slash). Part of the coverage
// audit's SUPPLY, since these ship from the converter rather than the jar.
py::list<py::str> list_console_assets();

// What coverage() returns (a dict with these fixed keys).
struct Coverage {
    py::dict<py::str, py::list<py::str>> needed;   // {path: [files that reference it]}   (slash-less)
    py::list<py::str> present;                     // sorted[paths the code needs AND the jar has]
    py::list<py::str> missing;                     // sorted[paths the code needs but the jar LACKS]  <- runtime NULL-image crash
    py::list<py::str> extra;                       // sorted[asset paths in the jar the code never references]
    int64_t jar_count = 0;
};

// Audit the code's texture/resource demand against the jar's supply.
Coverage coverage(const py::str& jar_path, const py::str& code_dir);

// (Python's `converted=None` and an empty list behave the same here: both are falsy, so None is an empty list.)
py::str render_coverage(const Coverage& cov, const py::list<py::str>& converted = py::list<py::str>(),
                        const py::list<py::str>& unconverted = py::list<py::str>());

// Names of PNG entries in the jar that are NOT already RGBA (would misrender raw on the 360).
py::list<py::str> indexed_pngs(const py::str& jar_path);

// What run() returns (a dict with these fixed keys).
struct RunResult {
    py::list<py::str> written;
    std::optional<Coverage> coverage;     // None when no code_dir was given
    py::str content_dir;
    std::optional<py::str> report;        // None when no coverage was written
    py::list<py::str> converted;
    py::list<py::str> unconverted;
    bool has_pil = false;
    py::list<py::str> console;
};

// One-shot: extract assets into content_dir (all, or only code-referenced if only_needed), normalizing
// non-RGBA PNGs to RGBA, and -- when code_dir is given -- write TEXTURE_COVERAGE.md next to content_dir.
// (`code_dir=None` and an empty string are both "no audit".)
RunResult run(const py::str& jar_path, const py::str& content_dir, const std::optional<py::str>& code_dir = std::nullopt,
              bool only_needed = false, bool normalize_png = true);

}  // namespace codebreaker::java::console::assets
