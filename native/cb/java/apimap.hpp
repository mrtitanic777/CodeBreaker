// cb/java/apimap.hpp -- the API Mapping Registry: a data-driven, cross-language / cross-API function equivalence
// database.
//
// WHY THIS EXISTS
//   The converter used to bake "what maps to what" into the engine, tuned to one game's exact API surface. A
//   different game -- or a different *version* of the same game -- touches different library calls and falls off the
//   edge of what was hardcoded. That is a per-version treadmill.
//
//   The registry breaks that: the *engine* only translates language SYNTAX (statements, types, generics). Everything
//   about which library/API call becomes which other call is DATA -- rows in this registry -- so a new version is
//   just a different set of rows resolved by the same code, and "what's still unmapped" is an enumerable report
//   instead of a mystery compile error.
//
// WHAT A ROW IS
//   Not name->name. A mapping carries a KIND, because equivalence is rarely 1:1:
//
//     direct            1:1 rename, same semantics          Math.sqrt      -> sqrtf
//     signature-adapted same idea, different args/semantics String.substring (bounds differ)
//     macro             expands to a small expression       list.size()    -> (int)v.size()
//     constant          a value/enum constant, not a call   GL_TRIANGLES   -> D3DPT_TRIANGLELIST
//     shim              resolves to a hand-written helper    glVertex3f     -> j2c::gl::vertex3f
//     unsupported       no mapping; emit a clear diagnostic  (enumerable gap, not a build break)
//
//   The hard cases (immediate-mode GL -> Direct3D) still need hand-written shims -- the registry does not eliminate
//   them, it *indexes* them so they are written once, registered, and never silently re-break.
//
// DATA LAYOUT
//   JSON under the lane's `api_maps/` folder:
//
//     languages.json     the catalog of language/API "slots" (java, cpp, opengl, direct3d9, python, csharp, c, ...):
//                        the whole idea's namespace. A language lists its `dialects` oldest -> newest.
//     <subsystem>.json   one file per source subsystem (java.util, opengl, nio, ...): a set of symbols and, per
//                        symbol, its mapping into one or more targets. A row may carry `by_version` overrides keyed
//                        by dialect id; the NEWEST override at or below the requested dialect wins.
//
//   {"subsystem": "opengl", "language": "opengl", "description": "...",
//    "symbols": {"glVertex3f": {"signature": "(float,float,float)", "doc": "Immediate-mode vertex.",
//                               "maps": {"direct3d9": {"target": "j2c::gl::vertex3f", "kind": "shim",
//                                                      "note": "accumulates into a vertex buffer, flushed on glEnd"}}}}}
//
//   The legacy per-CLASS Java->Xbox360 intent map is bridged in with `add_legacy_equivalents`, so none of that
//   knowledge is lost; new work is authored as per-function JSON rows.
//
// A Registry is a value built by `Registry::load` and handed to whoever resolves; there is no global cache.
#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "cb/core/ordered_map.hpp"

namespace cb::java::apimap {

enum class Kind { Direct, SignatureAdapted, Macro, Constant, Shim, D3d9Helper, Unsupported };

std::string_view name(Kind k);   // "direct", "signature-adapted", "macro", "constant", "shim", "d3d9-helper", "unsupported"
std::optional<Kind> parse_kind(std::string_view text);

// One directed equivalence: source symbol (in src_lang) -> target (in dst_lang).
struct Mapping {
    std::string name;
    std::string src_lang;
    std::string dst_lang;
    std::string target;
    Kind kind = Kind::Direct;
    std::string note;
    std::optional<std::string> adapter;
    std::optional<std::string> since;
    std::string subsystem;
    bool reverse = false;                    // synthesised by inverting a forward row
    // Which language *version* the resolved target was selected for (e.g. the `nullptr` target of a null literal is
    // chosen only for a c++11+ dialect); nullopt means the row is version-agnostic.
    std::optional<std::string> dialect;

    bool supported() const { return kind != Kind::Unsupported; }
};

// `Mapping(java:Math.sqrt -> cpp:sqrtf@c++11 [direct])`; `<-` for a reverse row.
std::string to_string(const Mapping& m);

// A language- or dialect-specific replacement of part of a row.
struct VersionOverride {
    std::optional<std::string> target;
    std::optional<Kind> kind;
    std::optional<std::string> note;
    std::optional<std::string> adapter;
    std::optional<std::string> since;
};

// How one symbol maps into one target language, as authored.
struct TargetMap {
    std::string target;
    Kind kind = Kind::Direct;
    std::string note;
    std::optional<std::string> adapter;
    std::optional<std::string> since;
    std::vector<std::pair<std::string, VersionOverride>> by_version;   // keyed by dialect id, in file order
    // Row extras the C# back end reads: a statement-position template, the precedence of the expansion
    // ("eq", "unary"), and whether the row only applies to ordered collection classes.
    std::optional<std::string> stmt;
    std::optional<std::string> prec;
    std::optional<bool> ordered;
};

// Everything the registry knows about one source symbol.
struct SymbolRecord {
    OrderedMap<std::string, TargetMap> maps;   // target language -> mapping, in file order
    std::string subsystem;
    std::string doc;
    std::string signature;
};

// A language or API slot of languages.json.
struct Language {
    std::string kind;   // "language" or "api"
    std::string desc;
    std::vector<std::string> dialects;   // oldest -> newest; empty if the language declares no versions
};

struct SubsystemDoc {
    std::string language;
    std::string description;
};

// The effective values of a row for a requested dialect.
struct Selected {
    std::string target;
    Kind kind = Kind::Direct;
    std::string note;
    std::optional<std::string> adapter;
    std::optional<std::string> since;
    std::optional<std::string> dialect;   // the dialect whose override was chosen, if any
};

// A used source symbol set classified against one src->dst map.
struct Coverage {
    std::vector<std::pair<std::string, Mapping>> mapped;
    std::vector<std::pair<std::string, Mapping>> unsupported;
    std::vector<std::string> unmapped;
};

// One row of the legacy per-class intent map (Java platform class -> Xbox 360 XDK API).
struct LegacyEquivalent {
    std::string subsystem;
    std::string basename;   // the Java class
    std::string status;     // "real", "stub" or "dead"
    std::string note;
    std::string xdk;        // the XDK API
    std::string lib;
};

// The keys a source symbol is looked up under, most specific first: the symbol itself, `Holder.method`, and the bare
// leaf. Accepts what the emitter actually sees -- `org.lwjgl.opengl.GL11.glVertex3f`, `GL11.glVertex3f`,
// `ArrayList.get`, `glVertex3f`.
std::vector<std::string> lookup_keys(std::string_view name);

// The last identifier of a target symbol, for the reverse index: `std::vector::push_back` -> `push_back`;
// `j2c::gl::vertex3f` -> `vertex3f`; a trailing `(signature)` is dropped.
std::string target_leaf(std::string_view target);

class Registry {
public:
    // Load every `*.json` under `api_maps_dir`: languages.json is the catalog, each other file one subsystem, in name
    // order. A file that cannot be parsed, or whose rows are malformed, is skipped and listed in `problems()`.
    static Registry load(const std::filesystem::path& api_maps_dir);

    // Fold the legacy per-class map in as coarse rows (language java, target xbox360, subsystem
    // `java.platform:<subsystem>`).
    void add_legacy_equivalents(std::span<const LegacyEquivalent> rows);

    // ---- population (what load does per symbol / per file)
    void add_symbol(std::string_view src_lang, std::string_view name, SymbolRecord record);
    void add_subsystem_doc(std::string sub_id, std::string language, std::string description);

    const std::map<std::string, Language>& languages() const { return languages_; }
    const std::map<std::string, SubsystemDoc>& subsystems() const { return subsystems_; }
    // The files load() skipped and why, one line each.
    const std::vector<std::string>& problems() const { return problems_; }

    // The ordered (oldest -> newest) dialect ids a language declares, e.g. cpp -> c++98, c++03, c++11, ... Empty if the
    // language declares no versions.
    const std::vector<std::string>& dialect_order(std::string_view lang) const;

    // Given a raw row and a requested target dialect, the effective values. A row may carry `by_version` overrides
    // keyed by dialect id; the NEWEST override at or below the requested dialect wins, so a c++11 request picks up a
    // c++11 row and a c++98 request falls back to the base. With no ordering info for the language, only an exact
    // match counts.
    Selected select_version(const TargetMap& row, std::string_view dst_lang, std::optional<std::string_view> dialect) const;

    // ---- queries
    // Forward lookup: how does `name` (a symbol in src_lang) map into dst_lang, optionally for a specific target
    // dialect ("c++03", "c++11")?
    std::optional<Mapping> resolve(std::string_view name, std::string_view src_lang, std::string_view dst_lang,
                                   std::optional<std::string_view> dialect = std::nullopt) const;
    // The record a source symbol resolves to under `key` (an exact lookup key, as lookup_keys gives), or nullptr.
    const SymbolRecord* record(std::string_view src_lang, std::string_view key) const;
    // Inverse lookup (best-effort): a dst_lang symbol -> the src_lang symbol it came from.
    std::optional<Mapping> reverse(std::string_view target, std::string_view dst_lang, std::string_view src_lang) const;
    // True if the registry has ANY mapping for this source symbol (any target).
    bool known(std::string_view name, std::string_view src_lang) const;
    // Classify a set of used source symbols against the src->dst map (for a given dialect). Each list is in name order.
    Coverage coverage(const std::set<std::string>& names, std::string_view src_lang, std::string_view dst_lang,
                      std::optional<std::string_view> dialect = std::nullopt) const;

    struct SymbolRow {
        std::string name;
        const SymbolRecord* record = nullptr;
    };
    // All symbols of a subsystem id, one row per record under its canonical name -- the most qualified (dotted, then
    // longest) of the keys it is indexed under -- sorted by name.
    std::vector<SymbolRow> symbols_in(std::string_view subsystem) const;
    // {(subsystem, target language): number of symbols}
    std::map<std::pair<std::string, std::string>, std::size_t> stats() const;

    // Every lookup key with its record, in key order (for callers that build their own view).
    std::vector<std::pair<std::pair<std::string, std::string>, const SymbolRecord*>> all_keys() const;

private:
    std::map<std::string, Language> languages_;
    std::map<std::string, SubsystemDoc> subsystems_;
    std::deque<SymbolRecord> records_;   // a deque: pointers to records stay valid as more are added
    std::map<std::pair<std::string, std::string>, std::size_t> symbol_index_;                            // (src_lang, key) -> record
    std::map<std::tuple<std::string, std::string, std::string>, Mapping> reverse_;   // (dst_lang, leaf, src_lang)
    std::vector<std::string> problems_;
};

// Best-effort set of external API symbols referenced in a chunk of Java source: `Holder.method` calls and
// `Holder.CONSTANT` references.
std::set<std::string> extract_symbols(std::string_view text);

// The union of extract_symbols over every .java file under java_dir (unreadable files are skipped).
std::set<std::string> extract_symbols_from_dir(const std::filesystem::path& java_dir);

// An APIMAP.md coverage report: which used source symbols the registry can map into the target language/dialect,
// which are explicitly unsupported, and which are still unmapped (the enumerable gap list). Unknown/local symbols that
// resolve to nothing are folded into 'unmapped' -- most are the game's own classes, so this is a superset worth
// eyeballing, not a strict TODO.
std::string render_coverage_md(const Registry& registry, const std::set<std::string>& names, std::string_view src_lang,
                               std::string_view dst_lang, std::optional<std::string_view> dialect = std::nullopt,
                               std::optional<std::string_view> title = std::nullopt);

}  // namespace cb::java::apimap

