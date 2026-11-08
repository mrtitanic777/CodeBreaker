// codebreaker/java/apimap.cpp -- C++ port of codebreaker.java.apimap (see the header).

#include "codebreaker/java/apimap.hpp"

#include <unordered_map>

#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/console/platform_equivalents.hpp"
#include "json.hpp"

namespace codebreaker::java::apimap {

namespace {

namespace pe = codebreaker::java::console::platform_equivalents;

// ---- reading the JSON rows into the typed records ------------------------------------------------

std::optional<py::str> opt_str(const py::V& d, const char* key) {
    if (!d.has(key)) return std::nullopt;
    const py::V& v = d.get(key);
    if (v.is_none()) return std::nullopt;
    if (!v.is_str()) throw py::TypeError(std::string("api_maps: ") + key + " is not a str");
    return py::str(v.str());
}

py::str str_or(const py::V& d, const char* key, const char* dflt) {
    std::optional<py::str> v = opt_str(d, key);
    return v ? *v : py::str(dflt);
}

MapEntry map_entry(const py::V& m) {
    if (!m.is_dict()) throw py::AttributeError("api_maps: a mapping is not an object");
    MapEntry e;
    e.target = str_or(m, "target", "");
    e.kind = str_or(m, "kind", "direct");
    e.note = str_or(m, "note", "");
    e.adapter = opt_str(m, "adapter");
    e.since = opt_str(m, "since");
    e.stmt = opt_str(m, "stmt");
    e.prec = opt_str(m, "prec");
    if (m.has("ordered") && !m.get("ordered").is_none()) {
        if (!m.get("ordered").is_bool()) throw py::TypeError("api_maps: ordered is not a bool");
        e.ordered = m.get("ordered").b;
    }
    const py::V& by = m.get("by_version");
    if (!by.is_none()) {
        if (!by.is_dict()) throw py::AttributeError("api_maps: by_version is not an object");
        py::dict<py::str, Override> out;
        for (const auto& kv : by.items()) {
            const py::V& o = kv.second;
            if (!o.is_dict()) throw py::AttributeError("api_maps: a by_version row is not an object");
            out.set(py::str(kv.first.str()), Override{opt_str(o, "target"), opt_str(o, "kind"), opt_str(o, "note"),
                                                      opt_str(o, "adapter"), opt_str(o, "since")});
        }
        e.by_version = out;
    }
    return e;
}

SymbolEntry symbol_entry(const py::V& entry) {
    if (!entry.is_dict()) throw py::AttributeError("api_maps: a symbol is not an object");
    SymbolEntry s;
    s.doc = str_or(entry, "doc", "");
    s.signature = str_or(entry, "signature", "");
    const py::V& maps = entry.get("maps");
    if (!maps.is_none()) {
        if (!maps.is_dict()) throw py::AttributeError("api_maps: maps is not an object");
        for (const auto& kv : maps.items()) s.maps.set(py::str(kv.first.str()), map_entry(kv.second));
    }
    return s;
}

// json.load(open(path, 'r', encoding='utf-8')): the parsed value; a file that cannot be read or
// is not JSON (IOError, OSError, ValueError) is nothing
std::optional<py::V> read_json(const py::str& path) {
    try {
        py::file f = py::open(path, py::str("r"), py::str("utf-8"));
        py::str text = f.read_text();
        f.close();
        return py::json_parse(text.utf8());
    } catch (const py::OSError&) {
        return std::nullopt;
    } catch (const py::ValueError&) {
        return std::nullopt;
    } catch (const py::PyError& e) {
        if (e.type == "ValueError") return std::nullopt;
        throw;
    }
}

}  // namespace

// ---------------------------------------------------------------------------------------------
//  Mapping
// ---------------------------------------------------------------------------------------------

Mapping::Mapping(const py::str& name_, const py::str& src_lang_, const py::str& dst_lang_, const py::str& target_,
                 const py::str& kind_, const py::str& note_, const std::optional<py::str>& adapter_,
                 const std::optional<py::str>& since_, const py::str& subsystem_, bool reverse_,
                 const std::optional<py::str>& dialect_)
    : name(name_), src_lang(src_lang_), dst_lang(dst_lang_), target(target_), kind(kind_), note(note_),
      adapter(adapter_), since(since_), subsystem(subsystem_), reverse(reverse_), dialect(dialect_) {}

bool Mapping::supported() const {
    return kind != "unsupported";
}

py::str Mapping::__repr__() const {
    py::str arrow = reverse ? py::str("<-") : py::str("->");
    py::str d = (dialect && !dialect->empty()) ? py::str("@") + *dialect : py::str("");
    return py::str("Mapping(") + src_lang + py::str(":") + name + py::str(" ") + arrow + py::str(" ") + dst_lang + py::str(":") + target +
           d + py::str(" [") + kind + py::str("])");
}

// ---------------------------------------------------------------------------------------------
//  Normalizing
// ---------------------------------------------------------------------------------------------

py::list<py::str> _norm(py::str name) {
    name = name.strip();
    py::list<py::str> keys = py::list<py::str>{name};
    py::list<py::str> parts = py::split(name, py::str("."));
    if (py::len(parts) >= 2) {
        py::str holder_method = parts[-2] + "." + parts[-1];
        if (!keys.contains(holder_method)) {
            keys.append(holder_method);
        }
    }
    py::str leaf = parts[-1];
    if (!keys.contains(leaf)) {
        keys.append(leaf);
    }
    return keys;
}

py::str _leaf(const py::str& target) {
    py::str t = py::re::split(py::str("::|\\."), target.strip())[-1];
    t = py::re::sub(py::str("\\(.*$"), py::str(""), t);
    return t.strip();
}

// ---------------------------------------------------------------------------------------------
//  Registry
// ---------------------------------------------------------------------------------------------

void Registry::_add_symbol(const py::str& src_lang, const py::str& name, const SymbolEntry& entry, const py::str& subsystem) {
    const py::dict<py::str, MapEntry>& maps = entry.maps;
    auto rec = std::make_shared<SymRec>(SymRec{maps, subsystem, entry.doc, entry.signature});
    for (const py::str& key : _norm(name)) {
        auto k = std::make_tuple(src_lang, key);
        if (!_sym.contains(k)) _sym.set(k, rec);
    }
    for (const auto& kv : maps.items()) {
        const py::str& dst_lang = std::get<0>(kv);
        const MapEntry& m = std::get<1>(kv);
        // index the base target AND every by_version override target for reverse lookup
        std::vector<std::tuple<py::str, py::str>> variants;
        variants.emplace_back(m.target, m.kind);
        if (m.by_version) {
            for (const Override& o : m.by_version->values()) {
                if (o.target && !o.target->empty()) {
                    variants.emplace_back(*o.target, o.kind ? *o.kind : m.kind);
                }
            }
        }
        for (const auto& v : variants) {
            const py::str& tgt = std::get<0>(v);
            const py::str& kind = std::get<1>(v);
            if (tgt.empty()) {
                continue;
            }
            auto rmap = std::make_shared<Mapping>(tgt, dst_lang, src_lang, name, kind, m.note, std::nullopt, std::nullopt, subsystem, true);
            auto rk = std::make_tuple(dst_lang, _leaf(tgt), src_lang);
            if (!_rev.contains(rk)) _rev.set(rk, rmap);
        }
    }
}

void Registry::add_subsystem_doc(const py::str& sub_id, const py::str& language, const py::str& description) {
    subsystems.set(sub_id, SubsystemDoc{language, description});
}

py::list<py::str> Registry::dialect_order(const py::str& lang) const {
    if (!languages.contains(lang)) return py::list<py::str>{};
    const Language& l = languages.at(lang);
    return l.dialects ? *l.dialects : py::list<py::str>{};
}

Selected Registry::_select_version(const MapEntry& m, const py::str& dst_lang, const std::optional<py::str>& dialect) const {
    Selected base = std::make_tuple(m.target, m.kind, m.note, m.adapter, m.since, std::optional<py::str>());
    if (!m.by_version || m.by_version->empty() || !dialect || dialect->empty()) {
        return base;
    }
    const py::dict<py::str, Override>& by = *m.by_version;
    py::list<py::str> order = dialect_order(dst_lang);
    if (order.contains(*dialect)) {
        // walk newest->oldest within the prefix up to the requested dialect
        int64_t upto = 0;
        for (int64_t i = 0; i < order.size(); i++) {
            if (order[i] == *dialect) {
                upto = i;
                break;
            }
        }
        for (int64_t i = upto; i >= 0; i--) {
            const py::str& ver = order[i];
            if (by.contains(ver)) {
                const Override& o = by.at(ver);
                return std::make_tuple(o.target ? *o.target : std::get<0>(base), o.kind ? *o.kind : std::get<1>(base),
                                       o.note ? *o.note : std::get<2>(base), o.adapter ? o.adapter : std::get<3>(base),
                                       std::optional<py::str>(o.since ? *o.since : ver), std::optional<py::str>(ver));
            }
        }
        return base;
    }
    // no ordering info: honor only an exact match
    if (by.contains(*dialect)) {
        const Override& o = by.at(*dialect);
        return std::make_tuple(o.target ? *o.target : std::get<0>(base), o.kind ? *o.kind : std::get<1>(base),
                               o.note ? *o.note : std::get<2>(base), o.adapter ? o.adapter : std::get<3>(base),
                               std::optional<py::str>(o.since ? *o.since : *dialect), std::optional<py::str>(*dialect));
    }
    return base;
}

std::shared_ptr<Mapping> Registry::resolve(const py::str& name, const py::str& src_lang, const py::str& dst_lang,
                                           const std::optional<py::str>& dialect) const {
    for (const py::str& key : _norm(name)) {
        std::shared_ptr<SymRec> rec = _sym.get(std::make_tuple(src_lang, key));
        if (rec && rec->maps.contains(dst_lang)) {
            Selected s = _select_version(rec->maps.at(dst_lang), dst_lang, dialect);
            return std::make_shared<Mapping>(key, src_lang, dst_lang, std::get<0>(s), std::get<1>(s), std::get<2>(s),
                                             std::get<3>(s), std::get<4>(s), rec->subsystem, false, std::get<5>(s));
        }
    }
    return nullptr;
}

std::shared_ptr<Mapping> Registry::reverse(const py::str& target, const py::str& dst_lang, const py::str& src_lang) const {
    return _rev.get(std::make_tuple(dst_lang, _leaf(target), src_lang));
}

bool Registry::known(const py::str& name, const py::str& src_lang) const {
    for (const py::str& k : _norm(name)) {
        if (_sym.contains(std::make_tuple(src_lang, k))) return true;
    }
    return false;
}

Coverage Registry::coverage(const py::list<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                            const std::optional<py::str>& dialect) const {
    return coverage(py::set<py::str>::from(names), src_lang, dst_lang, dialect);
}

Coverage Registry::coverage(const py::set<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                            const std::optional<py::str>& dialect) const {
    Coverage cov;
    for (const py::str& name : py::sorted(names)) {
        std::shared_ptr<Mapping> m = resolve(name, src_lang, dst_lang, dialect);
        if (!m) {
            cov.unmapped.append(name);
        } else if (m->kind == "unsupported") {
            cov.unsupported.append(std::make_tuple(name, m));
        } else {
            cov.mapped.append(std::make_tuple(name, m));
        }
    }
    return cov;
}

py::list<std::tuple<py::str, std::shared_ptr<SymRec>>> Registry::symbols_in(const py::str& subsystem) const {
    // one entry per record (by identity), in the order records were first met
    std::vector<std::tuple<py::str, std::shared_ptr<SymRec>>> best;
    std::unordered_map<const SymRec*, size_t> where;
    for (const auto& kv : _sym.items()) {
        const py::str& key = std::get<1>(std::get<0>(kv));
        const std::shared_ptr<SymRec>& rec = std::get<1>(kv);
        if (rec->subsystem != subsystem) {
            continue;
        }
        auto it = where.find(rec.get());
        if (it == where.end()) {
            where[rec.get()] = best.size();
            best.emplace_back(key, rec);
        } else {
            const py::str& cur = std::get<0>(best[it->second]);
            if ((key.contains(py::str(".")) && !cur.contains(py::str("."))) || key.size() > cur.size()) {
                best[it->second] = std::make_tuple(key, rec);
            }
        }
    }
    // sorted(best.values()): the tuples compare by name, then (a TypeError) by the records
    std::stable_sort(best.begin(), best.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    for (size_t i = 1; i < best.size(); i++) {
        if (std::get<0>(best[i - 1]) == std::get<0>(best[i])) {
            throw py::TypeError("'<' not supported between instances of 'dict' and 'dict'");
        }
    }
    return py::list<std::tuple<py::str, std::shared_ptr<SymRec>>>(best);
}

py::dict<std::tuple<py::str, py::str>, int64_t> Registry::stats() const {
    py::dict<std::tuple<py::str, py::str>, int64_t> pairs;
    std::vector<std::shared_ptr<SymRec>> seen;   // {id(r): r for r in self._sym.values()}.values()
    std::unordered_map<const SymRec*, bool> met;
    for (const auto& rec : _sym.values()) {
        if (!met[rec.get()]) {
            met[rec.get()] = true;
            seen.push_back(rec);
        }
    }
    for (const auto& rec : seen) {
        for (const py::str& dst : rec->maps.keys()) {
            auto k = std::make_tuple(rec->subsystem, dst);
            pairs.set(k, pairs.get(k, 0) + 1);
        }
    }
    return pairs;
}

// ---------------------------------------------------------------------------------------------
//  Loading
// ---------------------------------------------------------------------------------------------

static std::shared_ptr<Registry> _CACHE;   // None

void _bridge_equiv(Registry& reg) {
    for (const auto& item : pe::all_entries()) {
        const py::str& subsystem = std::get<0>(item);
        const py::str& basename = std::get<1>(item);
        const pe::Equiv& e = std::get<2>(item);
        py::str kind = py::str("shim");
        if (e.status == "dead") kind = py::str("unsupported");
        py::str note = (e.note ? *e.note : py::str("")).strip();
        MapEntry m;
        m.target = e.xdk;
        m.kind = kind;
        m.note = (note + py::str(" [") + e.lib + py::str("]")).strip();
        SymbolEntry entry;
        entry.doc = note;
        entry.maps.set(py::str("xbox360"), m);
        reg._add_symbol(py::str("java"), basename, entry, py::str("java.platform:") + subsystem);
    }
    reg.add_subsystem_doc(py::str("java.platform"), py::str("java"),
                          py::str("Legacy per-class Java-platform -> Xbox 360 XDK intent map (bridged from "
                                  "platform_equivalents.EQUIV)."));
}

std::shared_ptr<Registry> load(bool force) {
    if (_CACHE && !force) {
        return _CACHE;
    }
    auto reg = std::make_shared<Registry>();
    py::str root = _paths::data_path(py::list<py::str>{py::str("api_maps")});

    // 1. language / API catalog
    py::str lang_file = py::os::path::join(root, py::str("languages.json"));
    if (py::os::path::isfile(lang_file)) {
        std::optional<py::V> cat = read_json(lang_file);
        if (cat) {
            if (!cat->is_dict()) throw py::AttributeError("catalog is not an object");
            const py::V& langs = cat->get("languages");
            if (!langs.is_none()) {
                if (!langs.is_dict()) throw py::AttributeError("languages is not an object");
                for (const auto& kv : langs.items()) {
                    const py::V& v = kv.second;
                    if (!v.is_dict()) throw py::AttributeError("a language is not an object");
                    Language l;
                    l.kind = opt_str(v, "kind");
                    l.desc = opt_str(v, "desc");
                    if (!v.get("dialects").is_none()) {
                        py::list<py::str> ds;
                        for (const py::V& d : v.get("dialects").seq()) ds.append(py::str(d.str()));
                        l.dialects = ds;
                    }
                    reg->languages.set(py::str(kv.first.str()), l);
                }
            }
        }
    }

    // 2. every subsystem file (any *.json except the catalog)
    py::list<py::str> files;
    try {
        for (const py::str& f : py::os::listdir(root)) {
            if (f.endswith(py::str(".json")) && f != "languages.json") files.append(f);
        }
        files = py::sorted(files);
    } catch (const py::OSError&) {
        files = py::list<py::str>{};
    }
    for (const py::str& fn : files) {
        std::optional<py::V> data = read_json(py::os::path::join(root, fn));
        if (!data) {
            continue;
        }
        if (!data->is_dict()) throw py::AttributeError("a subsystem file is not an object");
        std::optional<py::str> given_id = opt_str(*data, "subsystem");
        py::str sub_id = given_id ? *given_id : std::get<0>(py::os::path::splitext(fn));
        std::optional<py::str> given_lang = opt_str(*data, "language");
        py::str lang = given_lang ? *given_lang : py::split(sub_id, py::str("."))[0];
        reg->add_subsystem_doc(sub_id, lang, str_or(*data, "description", ""));
        const py::V& symbols = data->get("symbols");
        if (!symbols.is_none()) {
            if (!symbols.is_dict()) throw py::AttributeError("symbols is not an object");
            for (const auto& kv : symbols.items()) {
                reg->_add_symbol(lang, py::str(kv.first.str()), symbol_entry(kv.second), sub_id);
            }
        }
    }

    // 3. bridge the legacy EQUIV table
    _bridge_equiv(*reg);

    _CACHE = reg;
    return reg;
}

// ---------------------------------------------------------------------------------------------
//  Symbol extraction (for coverage reports over real source)
// ---------------------------------------------------------------------------------------------

const py::str _CALL_QUALIFIED = py::str("\\b([A-Z][A-Za-z0-9_]*)\\.([a-z][A-Za-z0-9_]*)\\s*\\(");
const py::str _CALL_CONST = py::str("\\b([A-Z][A-Za-z0-9_]*)\\.([A-Z][A-Z0-9_]{2,})\\b");

py::set<py::str> extract_symbols(const py::str& text) {
    py::set<py::str> out;
    for (const auto& m : py::re::finditer(_CALL_QUALIFIED, text)) {
        out.add(m.group(1) + "." + m.group(2));
    }
    for (const auto& m : py::re::finditer(_CALL_CONST, text)) {
        out.add(m.group(1) + "." + m.group(2));
    }
    return out;
}

py::set<py::str> extract_symbols_from_dir(const py::str& java_dir) {
    py::set<py::str> syms;
    for (const auto& step : py::os::walk(java_dir)) {
        const py::str& root = std::get<0>(step);
        for (const py::str& fn : std::get<2>(step)) {
            if (!fn.endswith(py::str(".java"))) {
                continue;
            }
            try {
                py::file f = py::open(py::os::path::join(root, fn), py::str("r"), py::str("utf-8"), py::str("replace"));
                py::str text = f.read_text();
                f.close();
                syms.update(extract_symbols(text));
            } catch (const py::OSError&) {
                continue;
            }
        }
    }
    return syms;
}

// ---------------------------------------------------------------------------------------------
//  Coverage report
// ---------------------------------------------------------------------------------------------

static py::str render_(const py::set<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                       const std::optional<py::str>& dialect, const std::shared_ptr<Registry>& reg_in,
                       const std::optional<py::str>& title) {
    std::shared_ptr<Registry> reg = reg_in ? reg_in : load();
    Coverage cov = reg->coverage(names, src_lang, dst_lang, dialect);
    py::str dia = (dialect && !dialect->empty()) ? py::str(" (") + *dialect + py::str(")") : py::str("");
    py::list<py::str> L = py::list<py::str>{
        py::str("# API Mapping Coverage"),
        py::str(""),
        py::str("**") + src_lang + py::str(" -> ") + dst_lang + dia + py::str("**"),
        py::str(""),
        (title && !title->empty()) ? *title : py::str("Symbols referenced by the source, resolved against the API-mapping registry."),
        py::str(""),
        py::str("| status | count |"),
        py::str("|---|---|"),
        py::str("| mapped | ") + py::to_str(py::len(cov.mapped)) + py::str(" |"),
        py::str("| unsupported (no target) | ") + py::to_str(py::len(cov.unsupported)) + py::str(" |"),
        py::str("| unmapped (unknown / local) | ") + py::to_str(py::len(cov.unmapped)) + py::str(" |"),
        py::str("")};
    if (!cov.mapped.empty()) {
        L.extend(py::list<py::str>{py::str("## Mapped"), py::str(""), py::str("| symbol | target | kind | note |"), py::str("|---|---|---|---|")});
        for (const auto& item : cov.mapped) {
            const py::str& name = std::get<0>(item);
            const Mapping& m = *std::get<1>(item);
            L.append(py::str("| `") + name + py::str("` | `") + m.target + py::str("` | ") + m.kind + py::str(" | ") +
                     m.note.replace(py::str("|"), py::str("\\|")) + py::str(" |"));
        }
        L.append(py::str(""));
    }
    if (!cov.unsupported.empty()) {
        L.extend(py::list<py::str>{py::str("## Unsupported (registered, but no target equivalent)"), py::str(""),
                                   py::str("| symbol | note |"), py::str("|---|---|")});
        for (const auto& item : cov.unsupported) {
            const py::str& name = std::get<0>(item);
            const Mapping& m = *std::get<1>(item);
            L.append(py::str("| `") + name + py::str("` | ") + m.note.replace(py::str("|"), py::str("\\|")) + py::str(" |"));
        }
        L.append(py::str(""));
    }
    if (!cov.unmapped.empty()) {
        L.extend(py::list<py::str>{py::str("## Unmapped (unknown to the registry -- mostly the source's own classes)"), py::str("")});
        for (const py::str& n : cov.unmapped) {
            L.append(py::str("- `") + n + py::str("`"));
        }
        L.append(py::str(""));
    }
    return py::join(py::str("\n"), L);
}

py::str render_coverage_md(const py::list<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                           const std::optional<py::str>& dialect, const std::shared_ptr<Registry>& reg,
                           const std::optional<py::str>& title) {
    return render_(py::set<py::str>::from(names), src_lang, dst_lang, dialect, reg, title);
}

py::str render_coverage_md(const py::set<py::str>& names, const py::str& src_lang, const py::str& dst_lang,
                           const std::optional<py::str>& dialect, const std::shared_ptr<Registry>& reg,
                           const std::optional<py::str>& title) {
    return render_(names, src_lang, dst_lang, dialect, reg, title);
}

}  // namespace codebreaker::java::apimap
