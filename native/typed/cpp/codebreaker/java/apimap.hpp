// codebreaker/java/apimap.hpp -- SHIM over cb/java/apimap.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// The old Registry exposes its tables (`_sym`, `_rev`, `languages`, `subsystems`) to the translate back ends, so the
// shim builds those views from the new registry once, at load; every query forwards to the new one. The module-level
// cache of load() stays here, as it did, because the old callers rely on it; the new tree has no such global.
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/apimap.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/console/platform_equivalents.hpp"
#include "py_os.hpp"

namespace codebreaker::java::apimap {

namespace cba = cb::java::apimap;

inline const py::list<py::str> KINDS = py::list<py::str>{py::str("direct"), py::str("signature-adapted"), py::str("macro"), py::str("constant"),
                                                         py::str("shim"), py::str("d3d9-helper"), py::str("unsupported")};

// One directed equivalence: source symbol (in src_lang) -> target (in dst_lang).
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
            const std::optional<py::str>& dialect = std::nullopt)
        : name(name), src_lang(src_lang), dst_lang(dst_lang), target(target), kind(kind), note(note), adapter(adapter),
          since(since), subsystem(subsystem), reverse(reverse), dialect(dialect) {}

    explicit Mapping(const cba::Mapping& m)
        : name(cb::compat::to_py(m.name)), src_lang(cb::compat::to_py(m.src_lang)), dst_lang(cb::compat::to_py(m.dst_lang)),
          target(cb::compat::to_py(m.target)), kind(cb::compat::to_py(cba::name(m.kind))), note(cb::compat::to_py(m.note)),
          adapter(cb::compat::to_py(m.adapter)), since(cb::compat::to_py(m.since)), subsystem(cb::compat::to_py(m.subsystem)),
          reverse(m.reverse), dialect(cb::compat::to_py(m.dialect)) {}

    bool supported() const { return kind != py::str("unsupported"); }
    py::str __repr__() const {
        const std::string arrow = reverse ? "<-" : "->";
        const std::string d = dialect ? "@" + cb::compat::from_py(*dialect) : std::string();
        return cb::compat::to_py("Mapping(" + cb::compat::from_py(src_lang) + ":" + cb::compat::from_py(name) + " " + arrow + " " +
                                 cb::compat::from_py(dst_lang) + ":" + cb::compat::from_py(target) + d + " [" + cb::compat::from_py(kind) + "])");
    }
};

// A language- or dialect-specific replacement of part of a row.
struct Override {
    std::optional<py::str> target, kind, note, adapter, since;
};

// How one symbol maps into one target language, as authored.
struct MapEntry {
    py::str target = py::str("");
    py::str kind = py::str("direct");
    py::str note = py::str("");
    std::optional<py::str> adapter, since;
    std::optional<py::dict<py::str, Override>> by_version;   // keyed by dialect id
    std::optional<py::str> stmt, prec;
    std::optional<bool> ordered;
};

struct SymbolEntry {
    py::dict<py::str, MapEntry> maps;     // target language -> mapping
    py::str doc = py::str("");
    py::str signature = py::str("");
};

struct SymRec {
    py::dict<py::str, MapEntry> maps;
    py::str subsystem;
    py::str doc;
    py::str signature;
};

struct Language {
    std::optional<py::str> kind;          // 'language' | 'api'
    std::optional<py::str> desc;
    std::optional<py::list<py::str>> dialects;   // ordered oldest -> newest
};

struct SubsystemDoc {
    py::str language;
    py::str description;
};

struct Coverage {
    py::list<std::tuple<py::str, std::shared_ptr<Mapping>>> mapped;
    py::list<std::tuple<py::str, std::shared_ptr<Mapping>>> unsupported;
    py::list<py::str> unmapped;
};

inline py::list<py::str> _norm(py::str name) { return cb::compat::to_py(cba::lookup_keys(cb::compat::from_py(name))); }
inline py::str _leaf(const py::str& target) { return cb::compat::to_py(cba::target_leaf(cb::compat::from_py(target))); }

namespace conv_ {
inline std::optional<std::string> opt(const std::optional<py::str>& s) {
    return s ? std::optional<std::string>(cb::compat::from_py(*s)) : std::nullopt;
}
inline std::optional<std::string_view> view(const std::optional<std::string>& s) {
    return s ? std::optional<std::string_view>(*s) : std::nullopt;
}
inline MapEntry old_entry(const cba::TargetMap& m) {
    MapEntry e;
    e.target = cb::compat::to_py(m.target);
    e.kind = cb::compat::to_py(cba::name(m.kind));
    e.note = cb::compat::to_py(m.note);
    e.adapter = cb::compat::to_py(m.adapter);
    e.since = cb::compat::to_py(m.since);
    if (!m.by_version.empty()) {
        py::dict<py::str, Override> by;
        for (const auto& [dialect, o] : m.by_version) {
            Override ov;
            ov.target = cb::compat::to_py(o.target);
            if (o.kind) ov.kind = cb::compat::to_py(cba::name(*o.kind));
            ov.note = cb::compat::to_py(o.note);
            ov.adapter = cb::compat::to_py(o.adapter);
            ov.since = cb::compat::to_py(o.since);
            by.set(cb::compat::to_py(dialect), ov);
        }
        e.by_version = by;
    }
    e.stmt = cb::compat::to_py(m.stmt);
    e.prec = cb::compat::to_py(m.prec);
    e.ordered = m.ordered;
    return e;
}
inline std::shared_ptr<Mapping> mapping(const std::optional<cba::Mapping>& m) {
    return m ? std::make_shared<Mapping>(*m) : nullptr;
}
}  // namespace conv_

class Registry {
  public:
    py::dict<py::str, Language> languages;               // id -> {'kind':'language'|'api', 'desc':str}
    py::dict<py::str, SubsystemDoc> subsystems;          // subsystem id -> {'language':.., 'description':..}
    py::dict<std::tuple<py::str, py::str>, std::shared_ptr<SymRec>> _sym;   // (src_lang, key) -> record

    // The registry the shim forwards to; built by load().
    std::shared_ptr<cba::Registry> impl;
    std::map<const cba::SymbolRecord*, std::shared_ptr<SymRec>> old_of;   // the old record standing for each new one

    // Build the old views (languages, subsystems, _sym) of the new registry.
    explicit Registry(std::shared_ptr<cba::Registry> r) : impl(std::move(r)) {
        for (const auto& [id, l] : impl->languages()) {
            Language lang;
            lang.kind = cb::compat::to_py(l.kind);
            lang.desc = cb::compat::to_py(l.desc);
            if (!l.dialects.empty()) lang.dialects = cb::compat::to_py(l.dialects);
            languages.set(cb::compat::to_py(id), lang);
        }
        for (const auto& [id, d] : impl->subsystems()) {
            subsystems.set(cb::compat::to_py(id), SubsystemDoc{cb::compat::to_py(d.language), cb::compat::to_py(d.description)});
        }
        for (const auto& [key, rec] : impl->all_keys()) {
            auto& old = old_of[rec];   // one old record per new record, as in Python
            if (!old) {
                old = std::make_shared<SymRec>();
                for (const auto& [dst, m] : rec->maps) old->maps.set(cb::compat::to_py(dst), conv_::old_entry(m));
                old->subsystem = cb::compat::to_py(rec->subsystem);
                old->doc = cb::compat::to_py(rec->doc);
                old->signature = cb::compat::to_py(rec->signature);
            }
            _sym.set(std::make_tuple(cb::compat::to_py(key.first), cb::compat::to_py(key.second)), old);
        }
    }

    // The ordered (oldest -> newest) list of dialect ids a language declares. Empty if none.
    py::list<py::str> dialect_order(const py::str& lang) const {
        return cb::compat::to_py(impl->dialect_order(cb::compat::from_py(lang)));
    }

    // Forward lookup: how does ``name`` (a symbol in src_lang) map into dst_lang, optionally for a specific target
    // ``dialect`` (e.g. 'c++03', 'c++11')? Returns a Mapping, or None (null).
    std::shared_ptr<Mapping> resolve(const py::str& name, const py::str& src_lang, const py::str& dst_lang,
                                     const std::optional<py::str>& dialect = std::nullopt) const {
        const auto d = conv_::opt(dialect);
        return conv_::mapping(impl->resolve(cb::compat::from_py(name), cb::compat::from_py(src_lang),
                                            cb::compat::from_py(dst_lang), conv_::view(d)));
    }

    // Inverse lookup (best-effort): a dst_lang symbol -> the src_lang symbol it came from (null: None).
    std::shared_ptr<Mapping> reverse(const py::str& target, const py::str& dst_lang, const py::str& src_lang) const {
        return conv_::mapping(impl->reverse(cb::compat::from_py(target), cb::compat::from_py(dst_lang), cb::compat::from_py(src_lang)));
    }

    // True if the registry has ANY mapping for this source symbol (any target).
    bool known(const py::str& name, const py::str& src_lang) const {
        return impl->known(cb::compat::from_py(name), cb::compat::from_py(src_lang));
    }

    // Classify a set of used source symbols against the src->dst map (for a given dialect).
    Coverage coverage(const py::list<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                      const std::optional<py::str>& dialect = std::nullopt) const {
        const auto v = cb::compat::from_py(names);
        return coverage_of(std::set<std::string>(v.begin(), v.end()), src_lang, dst_lang, dialect);
    }
    Coverage coverage(const py::set<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                      const std::optional<py::str>& dialect = std::nullopt) const {
        return coverage_of(cb::compat::from_py(names), src_lang, dst_lang, dialect);
    }

    // All (canonical_name, entry) for a subsystem id, one row per record.
    py::list<std::tuple<py::str, std::shared_ptr<SymRec>>> symbols_in(const py::str& subsystem) const {
        py::list<std::tuple<py::str, std::shared_ptr<SymRec>>> out;
        const std::string sub = cb::compat::from_py(subsystem);
        for (const auto& row : impl->symbols_in(sub)) {
            out.append(std::make_tuple(cb::compat::to_py(row.name), old_of.at(row.record)));
        }
        return out;
    }

    // {(subsystem, target language): number of symbols}
    py::dict<std::tuple<py::str, py::str>, int64_t> stats() const {
        py::dict<std::tuple<py::str, py::str>, int64_t> out;
        for (const auto& [key, n] : impl->stats()) {
            out.set(std::make_tuple(cb::compat::to_py(key.first), cb::compat::to_py(key.second)), static_cast<int64_t>(n));
        }
        return out;
    }

  private:
    Coverage coverage_of(const std::set<std::string>& names, const py::str& src_lang, const py::str& dst_lang,
                         const std::optional<py::str>& dialect) const {
        const auto d = conv_::opt(dialect);
        const cba::Coverage c = impl->coverage(names, cb::compat::from_py(src_lang), cb::compat::from_py(dst_lang), conv_::view(d));
        Coverage out;
        for (const auto& [n, m] : c.mapped) out.mapped.append(std::make_tuple(cb::compat::to_py(n), conv_::mapping(m)));
        for (const auto& [n, m] : c.unsupported) out.unsupported.append(std::make_tuple(cb::compat::to_py(n), conv_::mapping(m)));
        out.unmapped = cb::compat::to_py(c.unmapped);
        return out;
    }
};

namespace shim_ {
inline std::shared_ptr<Registry>& cache() {
    static std::shared_ptr<Registry> reg;   // _CACHE
    return reg;
}
}  // namespace shim_

// Load (and cache) the registry from the bundled api_maps/ JSON plus the legacy-equivalents bridge.
inline std::shared_ptr<Registry> load(bool force = false) {
    if (shim_::cache() && !force) return shim_::cache();
    return cb::compat::guard([&] {
        auto reg = std::make_shared<cba::Registry>(
            cba::Registry::load(cb::fs::from_text(cb::compat::from_py(_paths::data_path(py::list<py::str>{py::str("api_maps")})))));
        namespace pe = codebreaker::java::console::platform_equivalents;
        std::vector<cba::LegacyEquivalent> legacy;
        for (const auto& item : pe::all_entries()) {
            const pe::Equiv& e = std::get<2>(item);
            legacy.push_back({cb::compat::from_py(std::get<0>(item)), cb::compat::from_py(std::get<1>(item)),
                              cb::compat::from_py(e.status), e.note ? cb::compat::from_py(*e.note) : std::string(),
                              cb::compat::from_py(e.xdk), cb::compat::from_py(e.lib)});
        }
        reg->add_legacy_equivalents(legacy);
        shim_::cache() = std::make_shared<Registry>(reg);
        return shim_::cache();
    });
}

// Best-effort set of external API symbols referenced in a chunk of Java source.
inline py::set<py::str> extract_symbols(const py::str& text) {
    return cb::compat::to_py(cba::extract_symbols(cb::compat::from_py(text)));
}

// Union of extract_symbols() over every .java under java_dir.
inline py::set<py::str> extract_symbols_from_dir(const py::str& java_dir) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cba::extract_symbols_from_dir(cb::fs::from_text(cb::compat::from_py(java_dir))));
    });
}

namespace shim_ {
inline py::str render(const std::set<std::string>& names, const py::str& src_lang, const py::str& dst_lang,
                      const std::optional<py::str>& dialect, const std::shared_ptr<Registry>& reg,
                      const std::optional<py::str>& title) {
    return cb::compat::guard([&] {
        const std::shared_ptr<Registry> use = reg ? reg : load();
        const auto d = conv_::opt(dialect);
        const auto t = conv_::opt(title);
        return cb::compat::to_py(cba::render_coverage_md(*use->impl, names, cb::compat::from_py(src_lang),
                                                         cb::compat::from_py(dst_lang), conv_::view(d), conv_::view(t)));
    });
}
}  // namespace shim_

// Render an APIMAP.md coverage report.
inline py::str render_coverage_md(const py::list<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                                  const std::optional<py::str>& dialect = std::nullopt,
                                  const std::shared_ptr<Registry>& reg = nullptr,
                                  const std::optional<py::str>& title = std::nullopt) {
    const auto v = cb::compat::from_py(names);
    return shim_::render(std::set<std::string>(v.begin(), v.end()), src_lang, dst_lang, dialect, reg, title);
}
inline py::str render_coverage_md(const py::set<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                                  const std::optional<py::str>& dialect = std::nullopt,
                                  const std::shared_ptr<Registry>& reg = nullptr,
                                  const std::optional<py::str>& title = std::nullopt) {
    return shim_::render(cb::compat::from_py(names), src_lang, dst_lang, dialect, reg, title);
}

}  // namespace codebreaker::java::apimap

