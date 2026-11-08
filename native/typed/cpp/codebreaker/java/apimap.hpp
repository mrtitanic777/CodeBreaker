// codebreaker/java/apimap.hpp -- C++ port of codebreaker.java.apimap.
//
// apimap.py -- the API Mapping Registry: a data-driven, cross-language / cross-API function
// equivalence database.
//
// WHY THIS EXISTS
// ---------------
// The converter used to bake "what maps to what" into the engine, tuned to one game's exact API
// surface. A different game -- or a different *version* of the same game -- touches different library
// calls and falls off the edge of what was hardcoded. That is a per-version treadmill.
//
// The registry breaks that: the *engine* only translates language SYNTAX (statements, types, generics).
// Everything about which library/API call becomes which other call is DATA -- rows in this registry --
// so a new version is just a different set of rows resolved by the same code, and "what's still
// unmapped" is an enumerable report instead of a mystery compile error.
//
// WHAT A ROW IS
// -------------
// Not name->name. A mapping carries a KIND, because equivalence is rarely 1:1:
//
//     direct            1:1 rename, same semantics          Math.sqrt      -> sqrtf
//     signature-adapted same idea, different args/semantics String.substring (bounds differ)
//     macro             expands to a small expression       list.size()    -> (int)v.size()
//     constant          a value/enum constant, not a call   GL_TRIANGLES   -> D3DPT_TRIANGLELIST
//     shim              resolves to a hand-written helper    glVertex3f     -> j2c::gl::vertex3f
//     unsupported       no mapping; emit a clear diagnostic  (enumerable gap, not a build break)
//
// The hard cases (immediate-mode GL -> Direct3D) still need hand-written shims -- the registry does not
// eliminate them, it *indexes* them so they are written once, registered, and never silently re-break.
//
// DATA LAYOUT
// -----------
// JSON under the package's ``api_maps/`` directory (resolved via _paths.data_path):
//
//     apimap/languages.json         the catalog of language/API "slots" (java, cpp, opengl, direct3d9,
//                                   python, csharp, c, asm.x86, ...): the whole idea's namespace.
//     apimap/<subsystem>.json       one file per source subsystem (java.util, opengl, nio, ...), each a
//                                   set of symbols and, per symbol, its mapping into one or more targets.
//
// A subsystem file:
//
//     {
//       "subsystem": "opengl",
//       "language":  "opengl",
//       "description": "...",
//       "symbols": {
//         "glVertex3f": {
//           "signature": "(float,float,float)",
//           "doc": "Immediate-mode vertex.",
//           "maps": {
//             "direct3d9": {"target": "j2c::gl::vertex3f", "kind": "shim",
//                           "note": "accumulates into a vertex buffer, flushed on glEnd"}
//           }
//         }
//       }
//     }
//
// The legacy per-CLASS Java->Xbox360 intent map (platform_equivalents.EQUIV) is BRIDGED in at load so
// none of that knowledge is lost; new work is authored as per-function JSON rows.
//
// Typing notes (where the Python keeps JSON dicts):
//   MapEntry / Override   the dict of one mapping (target, kind, note, adapter, since, by_version and the
//                         extras stmt/ordered/prec some rows carry); an Override is a by_version row.
//                         Every value in the shipped JSON is a str (ordered is a bool); another type in
//                         the data is a TypeError here where Python would carry it along.
//   SymRec                the record a symbol is indexed under (shared by all of its keys: identity is
//                         the pointer, as the Python's id(rec)).
//   Language              a catalog row: kind, desc and, for a language with versions, its dialects.
//   the JSON reader       py::json_parse (native/l2/src); a file that is not JSON, or cannot be read, is
//                         skipped as in Python.
//   the data folder       codebreaker::java::_paths::data_path('api_maps'): the host sets _paths' folder
//                         (set_here) to the package folder.
#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::apimap {

inline const py::list<py::str> KINDS = py::list<py::str>{py::str("direct"), py::str("signature-adapted"), py::str("macro"), py::str("constant"),
                                                         py::str("shim"), py::str("d3d9-helper"), py::str("unsupported")};

// One directed equivalence: source symbol (in src_lang) -> target (in dst_lang).
//
// ``dialect`` records which language *version* the resolved target was selected for (e.g. the
// ``nullptr`` target of a null literal is chosen only for a c++11+ dialect); ``None`` means the
// row is version-agnostic.
class Mapping {
  public:
    py::str name;
    py::str src_lang;
    py::str dst_lang;
    py::str target;
    py::str kind;
    py::str note;
    std::optional<py::str> adapter;
    std::optional<py::str> since;
    py::str subsystem;
    bool reverse = false;                 // True if synthesized by inverting a forward row
    std::optional<py::str> dialect;       // dialect the target was selected for (or None)

    Mapping(const py::str& name, const py::str& src_lang, const py::str& dst_lang, const py::str& target,
            const py::str& kind = py::str("direct"), const py::str& note = py::str(""),
            const std::optional<py::str>& adapter = std::nullopt, const std::optional<py::str>& since = std::nullopt,
            const py::str& subsystem = py::str(""), bool reverse = false,
            const std::optional<py::str>& dialect = std::nullopt);

    bool supported() const;
    py::str __repr__() const;
};

// the by_version row of a mapping: any of the fields it overrides
struct Override {
    std::optional<py::str> target, kind, note, adapter, since;
};

// the dict of one mapping into a target language
struct MapEntry {
    py::str target = py::str("");
    py::str kind = py::str("direct");
    py::str note = py::str("");
    std::optional<py::str> adapter, since;
    std::optional<py::dict<py::str, Override>> by_version;   // keyed by dialect id
    std::optional<py::str> stmt, prec;
    std::optional<bool> ordered;
};

// one symbol as the JSON (or the bridge) gives it
struct SymbolEntry {
    py::dict<py::str, MapEntry> maps;     // target language -> mapping
    py::str doc = py::str("");
    py::str signature = py::str("");
};

// the record a symbol is indexed under
struct SymRec {
    py::dict<py::str, MapEntry> maps;
    py::str subsystem;
    py::str doc;
    py::str signature;
};

// a row of the language / API catalog
struct Language {
    std::optional<py::str> kind;          // 'language' | 'api'
    std::optional<py::str> desc;
    std::optional<py::list<py::str>> dialects;   // ordered oldest -> newest
};

struct SubsystemDoc {
    py::str language;
    py::str description;
};

// what _select_version answers: (target, kind, note, adapter, since, chosen dialect)
using Selected = std::tuple<py::str, py::str, py::str, std::optional<py::str>, std::optional<py::str>, std::optional<py::str>>;

// coverage(): the used symbols sorted into the three classes
struct Coverage {
    py::list<std::tuple<py::str, std::shared_ptr<Mapping>>> mapped;
    py::list<std::tuple<py::str, std::shared_ptr<Mapping>>> unsupported;
    py::list<py::str> unmapped;
};

// Normalize a source symbol to registry lookup keys, most-specific first.
//
// Accepts things the emitter actually sees -- ``org.lwjgl.opengl.GL11.glVertex3f``,
// ``GL11.glVertex3f``, ``ArrayList.get``, ``glVertex3f`` -- and yields candidate keys:
// the fully-qualified tail ``Holder.method`` and the bare leaf ``method``.
py::list<py::str> _norm(py::str name);
// Last identifier of a target symbol, for building the reverse index.
// ``std::vector::push_back`` -> ``push_back``; ``j2c::gl::vertex3f`` -> ``vertex3f``.
py::str _leaf(const py::str& target);

class Registry {
  public:
    py::dict<py::str, Language> languages;               // id -> {'kind':'language'|'api', 'desc':str}
    py::dict<py::str, SubsystemDoc> subsystems;          // subsystem id -> {'language':.., 'description':..}
    py::dict<std::tuple<py::str, py::str>, std::shared_ptr<SymRec>> _sym;   // (src_lang, key) -> record
    py::dict<std::tuple<py::str, py::str, py::str>, std::shared_ptr<Mapping>> _rev;   // (dst_lang, leaf, src_lang) -> Mapping (synthesized)

    // ---- population
    void _add_symbol(const py::str& src_lang, const py::str& name, const SymbolEntry& entry, const py::str& subsystem);
    void add_subsystem_doc(const py::str& sub_id, const py::str& language, const py::str& description);

    // ---- version / dialect axis
    // The ordered (oldest -> newest) list of dialect ids a language declares, e.g. cpp ->
    // ['c++98','c++03','c++11',...]. Empty if the language declares no versions.
    py::list<py::str> dialect_order(const py::str& lang) const;
    // Given a raw map entry ``m`` and a requested target ``dialect``, return the effective
    // (target, kind, note, adapter, since, chosen_dialect). A row may carry ``by_version``
    // overrides keyed by dialect id; the NEWEST override at-or-below the requested dialect wins,
    // so a c++11 request picks up a c++11 row and a c++98 request falls back to the base.
    Selected _select_version(const MapEntry& m, const py::str& dst_lang, const std::optional<py::str>& dialect) const;

    // ---- queries
    // Forward lookup: how does ``name`` (a symbol in src_lang) map into dst_lang, optionally
    // for a specific target ``dialect`` (e.g. 'c++03', 'c++11')? Returns a Mapping, or None (null).
    std::shared_ptr<Mapping> resolve(const py::str& name, const py::str& src_lang, const py::str& dst_lang,
                                     const std::optional<py::str>& dialect = std::nullopt) const;
    // Inverse lookup (best-effort): a dst_lang symbol -> the src_lang symbol it came from (null: None).
    std::shared_ptr<Mapping> reverse(const py::str& target, const py::str& dst_lang, const py::str& src_lang) const;
    // True if the registry has ANY mapping for this source symbol (any target).
    bool known(const py::str& name, const py::str& src_lang) const;
    // Classify a set of used source symbols against the src->dst map (for a given dialect).
    // (``names`` is any iterable of str in the Python: a list or a set here.)
    Coverage coverage(const py::list<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                      const std::optional<py::str>& dialect = std::nullopt) const;
    Coverage coverage(const py::set<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                      const std::optional<py::str>& dialect = std::nullopt) const;
    // All (canonical_name, entry) for a subsystem id. Each record is indexed under several
    // keys (qualified + leaf aliases); collapse to one row per record, preferring the most
    // qualified (longest, dotted) key as the canonical name.
    py::list<std::tuple<py::str, std::shared_ptr<SymRec>>> symbols_in(const py::str& subsystem) const;
    // {(subsystem, target language): number of symbols}
    py::dict<std::tuple<py::str, py::str>, int64_t> stats() const;
};

// ---------------------------------------------------------------------------------------------
//  Loading
// ---------------------------------------------------------------------------------------------

// Fold the legacy per-CLASS Java->Xbox360 intent map into the registry as coarse
// java->direct3d9/xdk rows, so its knowledge is queryable through the same API. These are
// HOLDER-level (a class, e.g. GL11/Socket/File), complementing the per-function JSON rows.
void _bridge_equiv(Registry& reg);
// Load (and cache) the registry from the bundled apimap/ JSON plus the EQUIV bridge.
std::shared_ptr<Registry> load(bool force = false);

// ---------------------------------------------------------------------------------------------
//  Symbol extraction (for coverage reports over real source)
// ---------------------------------------------------------------------------------------------

// Holder.method(  and  bareStaticCall(  patterns as they appear in Java / decompiled Java.
extern const py::str _CALL_QUALIFIED;   // (the pattern text; compiled by py::re)
extern const py::str _CALL_CONST;

// Best-effort set of external API symbols referenced in a chunk of Java source:
// ``Holder.method`` calls and ``Holder.CONSTANT`` references.
py::set<py::str> extract_symbols(const py::str& text);
// Union of extract_symbols() over every .java under java_dir.
py::set<py::str> extract_symbols_from_dir(const py::str& java_dir);

// ---------------------------------------------------------------------------------------------
//  Coverage report
// ---------------------------------------------------------------------------------------------

// Render an APIMAP.md coverage report: which used source symbols the registry can map into
// the target language/dialect, which are explicitly unsupported, and which are still unmapped
// (the enumerable gap list). Unknown/local symbols that resolve to nothing are folded into
// 'unmapped' -- most are the game's own classes, so this is a superset worth eyeballing, not a
// strict TODO.   (``reg`` null: load(); ``title`` empty: the default line.)
py::str render_coverage_md(const py::list<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                           const std::optional<py::str>& dialect = std::nullopt,
                           const std::shared_ptr<Registry>& reg = nullptr,
                           const std::optional<py::str>& title = std::nullopt);
py::str render_coverage_md(const py::set<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                           const std::optional<py::str>& dialect = std::nullopt,
                           const std::shared_ptr<Registry>& reg = nullptr,
                           const std::optional<py::str>& title = std::nullopt);

}  // namespace codebreaker::java::apimap
