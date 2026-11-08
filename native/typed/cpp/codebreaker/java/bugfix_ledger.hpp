// codebreaker/java/bugfix_ledger.hpp -- typed C++ port of codebreaker.java.bugfix_ledger.
//
// bugfix_ledger.py -- catalog every game-breaking bug fix so none is ever forgotten.
//
// The standing rule (see memory: fixed-bug-marker-rule) is that EVERY game-breaking
// fix is tagged in-code with a greppable `FIXED BUG <slug>` (or the older
// `J2X-BUGFIX <slug>`) marker. Those markers are the source of truth. This module
// walks the converter's AUTHORED sources -- the layers that survive a reconvert
// (docs/platform references + canonicals + companions, and the per-project
// overlays) -- collects every marker with its slug, location, and the explanatory
// comment that follows it, and renders BUGFIX_LEDGER.md.
//
// Because it is GENERATED from the live markers, the ledger can never drift out of
// sync with the code: re-run it after any fix and it re-catalogs everything. It is
// wired into pipeline.convert() so the ledger regenerates on every conversion.
//
//     bugfix_ledger::run()          // -> writes state/java/BUGFIX_LEDGER.md, returns [entries]
//
// Typing notes:
//   * The Python's entries are dicts of fixed keys: ScanEntry ({slug, line, desc}, what _scan_file
//     finds in one file) and Entry (that plus {layer, file}, what collect finds across the layers).
//   * _LAYERS and _EXTS are tuples of constants: held as lists here.
//   * run()'s repo_root default is os.path.dirname(os.path.abspath(__file__)), the folder of the Java
//     package: C++ has no __file__, so it is codebreaker::java::_paths::_HERE() (the port of the
//     package's own idea of that folder). out_path's default is settings.state('java', 'BUGFIX_LEDGER.md'):
//     the "state" setting (py::settings::get_str) joined with java/BUGFIX_LEDGER.md, its folder created.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::bugfix_ledger {

// `FIXED BUG <slug>` or `J2X-BUGFIX <slug>`; slug is kebab-case or a YYYY-MM-DD date.
extern const py::re::Pattern _MARK;

// Words that follow the marker token in META references to the marker system itself (e.g. a help/docstring
// line that says "grep for the J2X-BUGFIX marker") -- these are NOT real fix slugs, so skip them.
extern const py::set<py::str> _NON_SLUGS;

// The scanner's own file: it contains the marker regex + prose ABOUT markers, so exclude it from the walk.
extern const py::str _SELF;

// Source extensions worth scanning (skip binaries, images, generated caches). (A tuple in the Python.)
extern const py::list<py::str> _EXTS;

// Layers scanned, most-durable first. The OUTPUT layers (canonical/reference/companion/overlay) survive a
// reconvert and the emitted tree is derived from them. The GENERATOR layer is the converter itself
// (`codebreaker/java/`) -- its emitter/runtime/toolchain fixes don't appear in the output tree but ARE revertible by an
// edit to the converter, so they belong in the ledger too (e.g. the section-sign unsigned-char cast, the
// SFX/music bypass pass, map-get-inserts). Listed last as a distinct "the converter itself" category.
// [(layer, relative folder)]
extern const py::list<std::tuple<py::str, py::str>> _LAYERS;

// One marker found in one file.
struct ScanEntry {
    py::str slug;
    int64_t line = 0;
    py::str desc;
};

// One marker found by collect(): the ScanEntry's keys plus the layer and file it was found in.
struct Entry {
    py::str slug;
    int64_t line = 0;
    py::str desc;
    py::str layer;
    py::str file;
};

// Strip code/comment punctuation to recover the human explanation on a marker line.
py::str _clean_comment(const py::str& line);

// [{'slug','line','desc'}] for each marker in a file (empty if it cannot be read).
py::list<ScanEntry> _scan_file(const py::str& path);

// Return [{'layer','file','slug','line','desc'}] across every authored layer.
py::list<Entry> collect(const py::str& repo_root);

// Render the collected entries as BUGFIX_LEDGER.md text.
py::str render(const py::list<Entry>& entries);

// Collect markers and write BUGFIX_LEDGER.md (default: state/java/BUGFIX_LEDGER.md). `repo_root` is
// the folder whose layers are scanned (default: this package). Returns the entry list.
py::list<Entry> run(const std::optional<py::str>& repo_root = std::nullopt,
                    const std::optional<py::str>& out_path = std::nullopt);

}  // namespace codebreaker::java::bugfix_ledger
