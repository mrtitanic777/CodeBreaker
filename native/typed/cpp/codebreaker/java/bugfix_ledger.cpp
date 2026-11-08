// codebreaker/java/bugfix_ledger.cpp -- typed C++ port of codebreaker.java.bugfix_ledger (see bugfix_ledger.hpp).

#include "codebreaker/java/bugfix_ledger.hpp"

#include <algorithm>
#include <vector>

#include "codebreaker/java/_paths.hpp"
#include "py_settings.hpp"

namespace codebreaker::java::bugfix_ledger {

// `FIXED BUG <slug>` or `J2X-BUGFIX <slug>`; slug is kebab-case or a YYYY-MM-DD date.
const py::re::Pattern _MARK = py::re::compile(py::str(R"((FIXED BUG|J2X-BUGFIX)\s+([A-Za-z0-9][A-Za-z0-9\-]*))"));

// Words that follow the marker token in META references to the marker system itself (e.g. a help/docstring
// line that says "grep for the J2X-BUGFIX marker") -- these are NOT real fix slugs, so skip them.
const py::set<py::str> _NON_SLUGS = py::set<py::str>::from(std::make_tuple(py::str("marker"), py::str("markers")));

// The scanner's own file: it contains the marker regex + prose ABOUT markers, so exclude it from the walk.
const py::str _SELF("bugfix_ledger.py");

// Source extensions worth scanning (skip binaries, images, generated caches).
const py::list<py::str> _EXTS = {py::str(".h"), py::str(".hpp"), py::str(".cpp"), py::str(".cc"), py::str(".c"),
                                 py::str(".py"), py::str(".md")};

// Layers scanned, most-durable first. The OUTPUT layers (canonical/reference/companion/overlay) survive a
// reconvert and the emitted tree is derived from them. The GENERATOR layer is the converter itself
// (`codebreaker/java/`) -- its emitter/runtime/toolchain fixes don't appear in the output tree but ARE revertible by an
// edit to the converter, so they belong in the ledger too (e.g. the section-sign unsigned-char cast, the
// SFX/music bypass pass, map-get-inserts). Listed last as a distinct "the converter itself" category.
const py::list<std::tuple<py::str, py::str>> _LAYERS = {
    std::make_tuple(py::str("canonical"), py::os::path::join(py::str("platform"), py::str("canonical"))),
    std::make_tuple(py::str("reference"), py::os::path::join(py::str("platform"), py::str("reference"))),
    std::make_tuple(py::str("companion"), py::os::path::join(py::str("platform"), py::str("companions"))),
    std::make_tuple(py::str("generator"), py::str("")),
};

namespace {

bool ends_with_any_ext(const py::str& fn) {
    for (const py::str& ext : _EXTS) {
        if (fn.endswith(ext)) return true;
    }
    return false;
}

}  // namespace

py::str _clean_comment(const py::str& line) {
    py::str s = line.strip();
    // drop a leading comment opener (// or /* or *) and any leftover code before it
    std::optional<py::re::Match> m = py::re::search(
        py::str(R"((FIXED BUG|J2X-BUGFIX)\s+[A-Za-z0-9][A-Za-z0-9\-]*[:\s]*(.*)$)"), s);
    py::str tail = (m ? m->group(2) : s).strip();
    tail = tail.rstrip(py::str("*/")).strip();
    return tail;
}

py::list<ScanEntry> _scan_file(const py::str& path) {
    py::list<ScanEntry> entries;
    py::list<py::str> lines;
    try {
        py::file f = py::open(path, py::str("r"), py::str("utf-8"), py::str("replace"));
        lines = f.readlines_text();
    } catch (const py::OSError&) {
        return entries;
    }
    for (int64_t i = 0; i < py::len(lines); ++i) {
        const py::str& line = lines[i];
        std::optional<py::re::Match> m = _MARK.search(line);
        if (!m) {
            continue;
        }
        py::str slug = m->group(2);
        if (_NON_SLUGS.contains(slug)) {   // meta reference to the marker system, not a fix slug
            continue;
        }
        py::str desc = _clean_comment(line);
        // If the marker line has no trailing prose, borrow the next comment line.
        if (desc.empty() && i + 1 < py::len(lines)) {
            py::str nxt = lines[i + 1].strip().lstrip(py::str("/*")).strip();
            if (!nxt.empty() && !_MARK.search(nxt)) {
                desc = nxt.rstrip(py::str("*/")).strip();
            }
        }
        ScanEntry e;
        e.slug = slug;
        e.line = i + 1;
        e.desc = desc;
        entries.append(e);
    }
    return entries;
}

py::list<Entry> collect(const py::str& repo_root) {
    py::list<Entry> out;
    for (const auto& layer_rel : _LAYERS) {
        const py::str& layer = std::get<0>(layer_rel);
        const py::str& rel = std::get<1>(layer_rel);
        py::str root = py::os::path::join(repo_root, rel);
        if (!py::os::path::isdir(root)) {
            continue;
        }
        for (const auto& step : py::os::walk(root)) {
            const py::str& dirpath = std::get<0>(step);
            const py::list<py::str>& dirs = std::get<1>(step);
            py::list<py::str> files = std::get<2>(step);
            if (dirpath.contains(py::str("__pycache__"))) {
                continue;
            }
            if (layer == py::str("generator") && dirpath == root) {
                py::list<py::str> kept;   // dirs[:] = [...]: scanned as their own layers
                for (const py::str& d : dirs) {
                    if (d != py::str("platform") && d != py::str("api_maps")) kept.append(d);
                }
                dirs.set_slice(py::none, py::none, kept);
            }
            for (const py::str& fn : py::sorted(files)) {
                if (!ends_with_any_ext(fn) || fn == _SELF) {
                    continue;
                }
                py::str full = py::os::path::join(dirpath, fn);
                py::str rel_file = py::os::path::relpath(full, repo_root).replace(py::str("\\"), py::str("/"));
                for (const ScanEntry& s : _scan_file(full)) {
                    Entry e;
                    e.slug = s.slug;
                    e.line = s.line;
                    e.desc = s.desc;
                    e.layer = layer;
                    e.file = rel_file;
                    out.append(e);
                }
            }
        }
    }
    return out;
}

py::str render(const py::list<Entry>& entries) {
    py::dict<py::str, py::list<Entry>> by_layer;
    for (const Entry& e : entries) {
        if (!by_layer.contains(e.layer)) by_layer.set(e.layer, py::list<Entry>());
        by_layer[e.layer].append(e);
    }

    int64_t total = py::len(entries);
    py::set<py::str> slugs;
    for (const Entry& e : entries) slugs.add(e.slug);
    int64_t uniq = py::len(slugs);
    py::list<py::str> lines;
    lines.append(py::str("# Bug-fix ledger"));
    lines.append(py::str(""));
    lines.append(py::str("_Auto-generated by `codebreaker/java/bugfix_ledger.py` from the live `FIXED BUG` / "
                         "`J2X-BUGFIX` markers in the converter's authored sources. Do not edit by "
                         "hand -- fix the marker in-code and re-run the converter (or "
                         "`python -m codebreaker java bugfix-ledger`) to regenerate._"));
    lines.append(py::str(""));
    lines.append(py::str("Every game-breaking fix carries a greppable in-code marker (standing rule). "
                         "This ledger is the index of those markers so a fix can never be silently "
                         "reverted by a reconvert without it showing up here as missing."));
    lines.append(py::str(""));
    lines.append(py::str("**") + py::to_str(total) + py::str(" markers across ") + py::to_str(uniq) +
                 py::str(" distinct fixes.**"));
    lines.append(py::str(""));
    for (const auto& layer_rel : _LAYERS) {
        const py::str& layer = std::get<0>(layer_rel);
        if (!by_layer.contains(layer)) continue;
        std::vector<Entry> rows = by_layer[layer].vec();
        if (rows.empty()) {
            continue;
        }
        py::dict<py::str, py::str> pretty = {
            {py::str("canonical"), py::str("Canonical headers (real java.* replacements)")},
            {py::str("reference"), py::str("Platform references (native 360/D3D/XInput/XACT layer)")},
            {py::str("companion"), py::str("Companions (bolt-on platform helpers)")},
            {py::str("overlay"), py::str("Overlays (per-project hand-fixes, re-applied every convert)")},
            {py::str("generator"), py::str("Generator / emitter / runtime / toolchain (the converter itself)")}};
        lines.append(py::str("## ") + pretty[layer]);
        lines.append(py::str(""));
        lines.append(py::str("| Fix (slug) | Location | What it fixes |"));
        lines.append(py::str("| --- | --- | --- |"));
        std::stable_sort(rows.begin(), rows.end(), [](const Entry& a, const Entry& b) {
            return std::make_tuple(a.file, a.line) < std::make_tuple(b.file, b.line);
        });
        for (const Entry& e : rows) {
            py::str desc = e.desc.replace(py::str("|"), py::str("\\|"));
            if (py::len(desc) > 220) {
                desc = desc.slice(0, 217) + py::str("...");
            }
            lines.append(py::str("| `") + e.slug + py::str("` | ") + e.file + py::str(":") + py::to_str(e.line) +
                         py::str(" | ") + desc + py::str(" |"));
        }
        lines.append(py::str(""));
    }
    py::str out;
    for (int64_t i = 0; i < py::len(lines); ++i) {
        if (i) out = out + py::str("\n");
        out = out + lines[i];
    }
    return out + py::str("\n");
}

py::list<Entry> run(const std::optional<py::str>& repo_root_in, const std::optional<py::str>& out_path_in) {
    py::str repo_root = repo_root_in ? *repo_root_in : codebreaker::java::_paths::_HERE();
    py::list<Entry> entries = collect(repo_root);
    py::str out_path;
    if (out_path_in) {
        out_path = *out_path_in;
    } else {
        // settings.state('java', 'BUGFIX_LEDGER.md'): a path inside the state folder, whose folder is created
        out_path = py::os::path::join(py::settings::get_str(py::str("state")), py::str("java"), py::str("BUGFIX_LEDGER.md"));
        py::os::makedirs(py::os::path::dirname(out_path), 0777, true);
    }
    py::file f = py::open(out_path, py::str("w"), py::str("utf-8"));
    f.write(render(entries));
    f.close();
    return entries;
}

}  // namespace codebreaker::java::bugfix_ledger
