// codebreaker/symbols/ordinals.cpp -- the shim's translation unit (see ordinals.hpp). The new module (cb/symbols/ordinals)
// does the naming; this file gives it the corpus (`OldCorpus`: the old format readers and corpus behind the two
// interfaces the module asks for) and the knowledge base's shipped table, and converts types.
#include "codebreaker/symbols/ordinals.hpp"

#include <algorithm>
#include <mutex>
#include <set>

#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/ordinals.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "py_json.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::ordinals {

namespace {

namespace no = cb::symbols::ordinals;
using cb::compat::from_py;
using cb::compat::to_py;
using engine::facts::Json;

// settings.state("module_names16.json"): a path inside the state folder, which is created if it is not there
py::str state_file(const py::str& name) {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), name);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

// {MODULE NAME: path} of the corpus's 16-bit modules, by the name each gives itself (a file called mnmnet.dll can be
// the module MNMNET_). Read once and kept in the state folder.
const py::dict<py::str, py::str>& named_modules() {
    static std::optional<py::dict<py::str, py::str>> by_module_name;
    if (!by_module_name) {
        py::str cache = state_file(py::str("module_names16.json"));
        py::str saved_root;
        py::dict<py::str, py::str> saved_modules;
        bool have = false;
        try {
            Json j = engine::facts::json_loads(py::open(cache, py::str("r"), py::str("utf-8")).read_text());
            if (j.kind == Json::Dict) {
                if (j.dict.contains(py::str("root")) && j.dict.at(py::str("root")).kind == Json::Str) {
                    saved_root = j.dict.at(py::str("root")).s;
                    have = true;
                }
                if (j.dict.contains(py::str("modules")) && j.dict.at(py::str("modules")).kind == Json::Dict)
                    for (const auto& kv : j.dict.at(py::str("modules")).dict.items()) saved_modules.set(std::get<0>(kv), std::get<1>(kv).s);
            }
        } catch (const std::exception&) {
            have = false;
        }
        if (!have || saved_root != base::corpus::root()) {
            py::dict<py::str, py::str> found;
            for (const py::str& path : base::corpus::modules(py::str("ne"), base::corpus::EXTS_)) {
                try {
                    found.setdefault(formats::ne::NEFile(path).module_name().upper(), path);
                } catch (const std::exception&) {
                }
            }
            saved_modules = found;
            // json.dump({"root": ..., "modules": ...}, sort_keys=True)
            std::string o = "{\"modules\": {";
            std::vector<py::str> keys;
            for (const py::str& k : found.keys()) keys.push_back(k);
            std::sort(keys.begin(), keys.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
            bool first = true;
            for (const py::str& k : keys) {
                if (!first) o += ", ";
                first = false;
                py::json::put(o, k);
                o += ": ";
                py::json::put(o, found.at(k));
            }
            o += "}, \"root\": ";
            py::json::put(o, base::corpus::root());
            o += "}";
            py::file f = py::open(cache, py::str("w"), py::str("utf-8"));
            f.write(py::str(o));
            f.close();
        }
        by_module_name = saved_modules;
    }
    return *by_module_name;
}

// The corpus as the new module asks about it, over the old readers.
class OldCorpus final : public no::ModuleExports, public no::ExportCheck {
public:
    std::optional<Table> table(std::string_view stem_text) const override {
        const py::str stem = to_py(stem_text);
        std::optional<formats::ne::NEFile> nef;
        std::optional<api_arguments::NeModule> ne = api_arguments::_module(stem);
        if (ne) {
            nef = std::get<0>(*ne);
        } else if (named_modules().contains(stem) && !named_modules().at(stem).empty()) {
            try {
                nef = formats::ne::NEFile(named_modules().at(stem));
            } catch (const std::exception&) {
                nef = std::nullopt;
            }
        }
        if (nef) {
            Table out;
            auto nonres = nef->nonresident_names();
            auto res = nef->resident_names();
            for (int64_t i = 1; i < nonres.size(); i++) out.names[static_cast<std::uint32_t>(std::get<1>(nonres[i]))] = from_py(std::get<0>(nonres[i]));
            for (int64_t i = 1; i < res.size(); i++) out.names[static_cast<std::uint32_t>(std::get<1>(res[i]))] = from_py(std::get<0>(res[i]));
            out.file = from_py(py::os::path::basename(nef->path).lower());
            return out;
        }
        for (const py::str& ext : api_arguments::_EXTS) {
            std::optional<py::str> path = base::corpus::retail(stem.lower() + ext);
            if (path && !path->empty() && base::corpus::lane(*path) == std::optional<py::str>(py::str("pe"))) {
                Table out;
                for (const auto& e : formats::pe::open_pe(*path).exports())
                    if (std::get<1>(e) && !std::get<1>(e)->empty()) out.names[static_cast<std::uint32_t>(std::get<0>(e))] = from_py(*std::get<1>(e));
                out.file = from_py(py::os::path::basename(*path).lower());
                return out;
            }
        }
        return std::nullopt;   // (the Python's ({}, ""): the module is not in the corpus)
    }

    std::optional<std::set<std::uint32_t>> numbers(std::string_view stem_text) const override {
        const py::str stem = to_py(stem_text);
        std::set<std::uint32_t> numbers;
        std::optional<api_arguments::NeModule> ne = api_arguments::_module(stem);
        if (ne) {
            for (int64_t k : std::get<1>(*ne).keys()) numbers.insert(static_cast<std::uint32_t>(k));
            return numbers;
        }
        if (named_modules().contains(stem) && !named_modules().at(stem).empty()) {
            for (int64_t o : formats::ne::NEFile(named_modules().at(stem)).export_ordinals()) numbers.insert(static_cast<std::uint32_t>(o));
            return numbers;
        }
        for (const py::str& ext : api_arguments::_EXTS) {
            std::optional<py::str> path = base::corpus::retail(stem.lower() + ext);
            if (path && !path->empty() && base::corpus::lane(*path) == std::optional<py::str>(py::str("pe"))) {
                for (const auto& e : formats::pe::open_pe(*path).exports()) numbers.insert(static_cast<std::uint32_t>(std::get<0>(e)));
                return numbers;
            }
        }
        return std::nullopt;
    }

    std::optional<bool> arguments_agree(std::string_view stem, std::uint32_t ordinal, std::uint32_t argument_bytes) const override {
        std::shared_ptr<api_arguments::_Mod> mod = api_arguments::_mod(to_py(stem));
        std::optional<int64_t> rva = mod ? mod->ords.get(ordinal) : std::nullopt;
        if (!rva || *rva == 0) return std::nullopt;
        std::optional<int64_t> got;
        try {
            got = api_arguments::_scan(*mod, *rva, 4);
        } catch (const std::exception&) {
            got = std::nullopt;
        }
        if (!got) return std::nullopt;
        return *got == static_cast<int64_t>(argument_bytes);
    }

    std::set<std::string> imported_by_number() const override {
        std::set<std::string> wanted;   // modules something in the corpus imports by number
        for (const py::str& path : base::corpus::modules(py::str("ne"), base::corpus::EXTS_)) {
            try {
                for (const auto& kv : imports16(path).items()) wanted.insert(from_py(std::get<0>(std::get<0>(kv))));
            } catch (const std::exception&) {
            }
        }
        for (const py::str& path : base::corpus::modules(py::str("pe"), base::corpus::EXTS_)) {
            try {
                for (const auto& kv : imports32(path).items()) wanted.insert(from_py(std::get<0>(std::get<0>(kv))));
            } catch (const std::exception&) {
            }
        }
        return wanted;
    }
};

std::mutex g_lock;
std::optional<OldCorpus> g_corpus;
std::optional<no::Ordinals> g_ordinals;

// ordinal_names.json, as the knowledge base has it
no::ShippedTables shipped_tables() {
    no::ShippedTables out;
    for (const auto& kv : knowledge::_table(py::str("windows"), py::str("ordinal_names")).items()) {
        if (!std::holds_alternative<Json>(std::get<1>(kv))) continue;
        const Json& entry = std::get<Json>(std::get<1>(kv));
        if (entry.kind != Json::Dict) continue;
        no::ShippedModule m;
        if (entry.dict.contains(py::str("from")) && entry.dict.at(py::str("from")).kind == Json::Str) m.from = from_py(entry.dict.at(py::str("from")).s);
        if (entry.dict.contains(py::str("checked")) && entry.dict.at(py::str("checked")).kind == Json::List && entry.dict.at(py::str("checked")).list.size() == 2) {
            m.checked_yes = static_cast<std::uint32_t>(entry.dict.at(py::str("checked")).list[0].i);
            m.checked_no = static_cast<std::uint32_t>(entry.dict.at(py::str("checked")).list[1].i);
        }
        if (entry.dict.contains(py::str("names")) && entry.dict.at(py::str("names")).kind == Json::Dict)
            for (const auto& nv : entry.dict.at(py::str("names")).dict.items())
                m.names[static_cast<std::uint32_t>(py::int_parse(std::get<0>(nv), 10))] = from_py(std::get<1>(nv).s);
        out[from_py(std::get<0>(kv))] = std::move(m);
    }
    return out;
}

const no::Ordinals& ordinals() {
    std::lock_guard<std::mutex> guard(g_lock);
    if (!g_ordinals) {
        g_corpus.emplace();
        g_ordinals.emplace(*g_corpus, shipped_tables(),
                           [](std::string_view name) { return from_py(prototypes::spelling(to_py(name))); });
    }
    return *g_ordinals;
}

// A number the new module can ask about: beyond 32 bits (or negative) a table has no entry.
std::optional<std::uint32_t> ordinal_of(int64_t ordinal) {
    if (ordinal < 0 || ordinal > static_cast<int64_t>(UINT32_MAX)) return std::nullopt;
    return static_cast<std::uint32_t>(ordinal);
}

// COMPAT(py): remove when the staged knowledge/reads shim is installed (the old engine records its reads in the old layer)
void record_shipped_read(const py::str& module) {
    knowledge::reads::record(py::str("windows"), py::str("ordinal_names"), to_py(no::module_stem(from_py(module))));
}

}  // namespace

std::optional<py::str> linker_name(const py::str& module, int64_t ordinal) {
    const auto n = ordinal_of(ordinal);
    record_shipped_read(module);
    return n ? to_py(ordinals().linker_name(from_py(module), *n)) : std::nullopt;
}

std::optional<py::str> name(const py::str& module, int64_t ordinal) {
    const auto n = ordinal_of(ordinal);
    record_shipped_read(module);
    return n ? to_py(ordinals().name(from_py(module), *n)) : std::nullopt;
}

std::optional<std::tuple<py::str, py::str>> documented(const py::str& module, int64_t ordinal) {
    const auto n = ordinal_of(ordinal);
    const auto d = n ? no::documented(from_py(module), *n) : std::nullopt;
    if (!d) return std::nullopt;
    return std::make_tuple(to_py(d->name), to_py(d->where));
}

py::str source(const py::str& module, int64_t ordinal) {
    const auto n = ordinal_of(ordinal);
    record_shipped_read(module);
    return n ? to_py(ordinals().source(from_py(module), *n)) : py::str("");
}

py::dict<int64_t, py::str> table(const py::str& module) {
    record_shipped_read(module);
    return to_py(ordinals().table(from_py(module)));
}

std::optional<py::str> identifier(const py::str& module, int64_t ordinal) {
    const auto n = ordinal_of(ordinal);
    record_shipped_read(module);
    return n ? to_py(ordinals().identifier(from_py(module), *n)) : std::nullopt;
}

py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> imports16(const py::str& module_path) {
    formats::ne::NEFile nef(module_path);
    py::list<py::str> refs = nef.module_refs();
    std::vector<no::ImportRef> imports;
    for (const auto& seg : nef.segments()) {
        for (const auto& r : nef.relocations(seg)) {
            if (r.kind == py::str("IMPORTORD") && r.module && 0 < *r.module && *r.module <= refs.size()) {
                imports.push_back({from_py(refs[*r.module - 1].upper()), static_cast<std::uint32_t>(r.ordinal.value_or(0))});
            }
        }
    }
    py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> out;
    for (const no::NamedImport& n : ordinals().names_of(imports)) {
        record_shipped_read(to_py(n.import.module));
        out.set(std::make_tuple(to_py(n.import.module), static_cast<int64_t>(n.import.ordinal)), to_py(n.name));
    }
    return out;
}

py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> imports32(const py::str& module_path) {
    std::vector<no::ImportRef> imports;
    for (const auto& kv : formats::pe::open_pe(module_path).imports().items()) {
        const py::str& dll = std::get<0>(std::get<1>(kv));
        const py::str& nm = std::get<1>(std::get<1>(kv));
        if (nm.startswith(py::str("#")) && nm.slice(1, nm.size()).isdigit()) {
            imports.push_back({no::module_stem(from_py(dll)), static_cast<std::uint32_t>(py::int_parse(nm.slice(1, nm.size()), 10))});
        }
    }
    py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> out;
    for (const no::NamedImport& n : ordinals().names_of(imports)) {
        record_shipped_read(to_py(n.import.module));
        out.set(std::make_tuple(to_py(n.import.module), static_cast<int64_t>(n.import.ordinal)), to_py(n.name));
    }
    return out;
}

py::dict<py::str, py::dict<int64_t, py::str>> read_definition_file(const py::str& path) {
    const auto file = cb::fs::from_text(path.utf8());
    return cb::compat::guard([&] {
        return to_py(no::read_definition_file(cb::text::from_cp1252(cb::fs::read_file(file)), cb::fs::to_text(file.stem())));
    });
}

py::dict<py::str, py::dict<int64_t, py::str>> read_import_library(const py::str& path) {
    const auto file = cb::fs::from_text(path.utf8());
    return cb::compat::guard([&] { return to_py(no::read_import_library(cb::fs::read_file(file))); });
}

py::dict<py::str, Shipped> collect(const py::list<py::str>& paths, const std::function<void(const py::str&)>& log) {
    std::vector<std::filesystem::path> files;
    for (const py::str& p : paths) files.push_back(cb::fs::from_text(p.utf8()));
    OldCorpus corpus;
    const no::ShippedTables made = no::collect(files, corpus, corpus, [&](std::string_view line) {
        if (log) log(to_py(line));
        else py::print(to_py(line));
    });
    const py::str table_file = py::os::path::join(knowledge::sources::_SYMBOLS(), py::str("ordinal_names.json"));
    cb::fs::write_text(cb::fs::from_text(table_file.utf8()), no::format_shipped(made));
    knowledge::sources::changed(table_file);   // the knowledge base loads it again
    {
        std::lock_guard<std::mutex> guard(g_lock);
        g_ordinals.reset();
    }
    py::dict<py::str, Shipped> out;
    for (const auto& [module, m] : made) {
        Shipped s;
        s.from = to_py(m.from);
        s.checked = {m.checked_yes, m.checked_no};
        for (const auto& [ordinal, name] : m.names) s.names.set(py::to_str(static_cast<int64_t>(ordinal)), to_py(name));
        out.set(to_py(module), s);
    }
    return out;
}

}  // namespace codebreaker::symbols::ordinals
