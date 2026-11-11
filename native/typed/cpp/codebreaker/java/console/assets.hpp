// codebreaker/java/console/assets.hpp -- SHIM over cb/java/console/assets.hpp (docs/CPP_DESIGN.md 6.3); deleted with its
// last py:: consumer (run, achievements).
//
// The old module imported Pillow when it was there (_HAS_PIL); the new one has its own PNG codec, so every non-RGBA PNG is
// converted and `unconverted` is always empty.
#pragma once

#include <optional>
#include <tuple>

#include "cb/java/console/assets.hpp"
#include "cb/java/console/png_rgba.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/console/_shim.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::assets {

// PIL.UnidentifiedImageError (an OSError): the file is not a readable PNG.
struct UnidentifiedImageError : py::OSError {
    using py::OSError::OSError;
    const char* type() const override { return "UnidentifiedImageError"; }
};

// `try: from PIL import Image` -- the PNG codec of the new module stands in for Pillow.
inline constexpr bool _HAS_PIL = true;

// PNG IHDR color type (0 gray, 2 rgb, 3 indexed, 4 gray+alpha, 6 rgba), or None if not a PNG.
inline std::optional<int64_t> _png_colortype(const py::bytes& raw) {
    const auto ct = cb::java::console::png::color_type(cb::compat::from_py(raw));
    if (!ct) return std::nullopt;
    return static_cast<int64_t>(*ct);
}

// True if this PNG is not already 8-bit straight RGBA (i.e. would misrender on the 360).
inline bool png_needs_rgba(const py::bytes& raw) { return cb::java::console::png::needs_rgba(cb::compat::from_py(raw)); }

// Return (bytes, changed). Raises UnidentifiedImageError for a PNG that cannot be read.
inline std::tuple<py::bytes, bool> normalize_png_to_rgba(const py::bytes& raw) {
    try {
        auto c = cb::java::console::png::normalize_to_rgba(cb::compat::from_py(raw));
        return {cb::compat::to_py(cb::ByteView(c.png)), c.changed};
    } catch (const cb::FormatError& e) {
        throw UnidentifiedImageError(std::string(e.what()));
    }
}

// The folder of the console-only assets the converter bundles (computed on first use, after the host has set _paths' folder).
inline const py::str& _CONSOLE_ASSETS_DIR() {
    static const py::str dir = codebreaker::java::_paths::data_path(py::list<py::str>{py::str("platform"), py::str("assets")});
    return dir;
}

inline py::list<py::str> list_jar_assets(const py::str& jar_path) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::java::console::assets::list_jar_assets(cb::java::console::ZipArchive::open(shim::path_of(jar_path))));
    });
}

inline py::dict<py::str, py::list<py::str>> code_resource_refs(const py::str& code_dir) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::java::console::assets::code_resource_refs(shim::path_of(code_dir))); });
}

inline py::str _strip_comments(const py::str& text) { return cb::compat::to_py(cb::java::console::assets::strip_comments(text.utf8())); }

inline py::list<py::str> place_console_assets(const py::str& content_dir) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::java::console::assets::place_console_assets(shim::path_of(_CONSOLE_ASSETS_DIR()), shim::path_of(content_dir)));
    });
}

inline py::list<py::str> list_console_assets() {
    return cb::compat::to_py(cb::java::console::assets::list_console_assets(shim::path_of(_CONSOLE_ASSETS_DIR())));
}

// Mirror the jar's asset entries into content_dir. If `converted` is given, the names of the PNGs that were normalized are
// appended to it (the list is a shared handle). Returns the sorted list of paths written.
inline py::list<py::str> extract(const py::str& jar_path, const py::str& content_dir,
                                 const std::optional<py::set<py::str>>& only = std::nullopt, bool normalize_png = true,
                                 const std::optional<py::list<py::str>>& converted = std::nullopt) {
    return cb::compat::guard([&] {
        cb::java::console::assets::ExtractOptions options;
        options.normalize_png = normalize_png;
        if (only) options.only = cb::compat::from_py(*only);
        const auto jar = cb::java::console::ZipArchive::open(shim::path_of(jar_path));
        auto done = cb::java::console::assets::extract(jar, shim::path_of(content_dir), options);
        if (converted) {
            py::list<py::str> shared = *converted;
            for (const std::string& n : done.normalized) shared.append(cb::compat::to_py(n));
        }
        return cb::compat::to_py(done.written);
    });
}

// What coverage() returns (a dict with these fixed keys).
struct Coverage {
    py::dict<py::str, py::list<py::str>> needed;   // {path: [files that reference it]}   (slash-less)
    py::list<py::str> present;
    py::list<py::str> missing;
    py::list<py::str> extra;
    int64_t jar_count = 0;
};

namespace detail_ {
inline Coverage to_old(const cb::java::console::assets::Coverage& c) {
    Coverage o;
    o.needed = cb::compat::to_py(c.needed);
    o.present = cb::compat::to_py(c.present);
    o.missing = cb::compat::to_py(c.missing);
    o.extra = cb::compat::to_py(c.extra);
    o.jar_count = static_cast<int64_t>(c.supply);
    return o;
}
inline cb::java::console::assets::Coverage to_new(const Coverage& c) {
    cb::java::console::assets::Coverage o;
    for (const auto& kv : c.needed.items()) o.needed[std::get<0>(kv).utf8()] = cb::compat::from_py(std::get<1>(kv));
    o.present = cb::compat::from_py(c.present);
    o.missing = cb::compat::from_py(c.missing);
    o.extra = cb::compat::from_py(c.extra);
    o.supply = static_cast<std::size_t>(c.jar_count);
    return o;
}
}  // namespace detail_

// Audit the code's texture/resource demand against the jar's supply.
inline Coverage coverage(const py::str& jar_path, const py::str& code_dir) {
    return cb::compat::guard([&] {
        const auto jar = cb::java::console::ZipArchive::open(shim::path_of(jar_path));
        return detail_::to_old(cb::java::console::assets::coverage(jar, shim::path_of(code_dir), shim::path_of(_CONSOLE_ASSETS_DIR())));
    });
}

inline py::str render_coverage(const Coverage& cov, const py::list<py::str>& converted = py::list<py::str>(),
                               const py::list<py::str>& unconverted = py::list<py::str>()) {
    (void)unconverted;   // (always empty: the new module has its own PNG codec)
    return cb::compat::to_py(cb::java::console::assets::render_coverage(detail_::to_new(cov), cb::compat::from_py(converted)));
}

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

// One-shot: extract assets into content_dir (all, or only code-referenced if only_needed), normalizing non-RGBA PNGs to
// RGBA, and -- when code_dir is given -- write TEXTURE_COVERAGE.md next to content_dir.
inline RunResult run(const py::str& jar_path, const py::str& content_dir, const std::optional<py::str>& code_dir = std::nullopt,
                     bool only_needed = false, bool normalize_png = true) {
    return cb::compat::guard([&] {
        cb::java::console::assets::RunOptions options;
        options.code_dir = shim::given(code_dir);
        options.bundle_dir = shim::path_of(_CONSOLE_ASSETS_DIR());
        options.only_needed = only_needed;
        options.normalize_png = normalize_png;
        const auto done = cb::java::console::assets::run(shim::path_of(jar_path), shim::path_of(content_dir), options);
        RunResult out;
        out.written = cb::compat::to_py(done.written);
        if (done.coverage) out.coverage = detail_::to_old(*done.coverage);
        out.content_dir = content_dir;
        out.report = shim::native(done.report);
        out.converted = cb::compat::to_py(done.converted);
        out.has_pil = _HAS_PIL;
        out.console = cb::compat::to_py(done.console);
        return out;
    });
}

}  // namespace codebreaker::java::console::assets
