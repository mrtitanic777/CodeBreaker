// codebreaker/symbols/ordinals.cpp -- typed C++ port of codebreaker.symbols.ordinals (see ordinals.hpp).

#include "codebreaker/symbols/ordinals.hpp"

#include <algorithm>
#include <set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/demangle.hpp"
#include "codebreaker/symbols/libraries/coff.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "py_json.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::ordinals {

namespace {

using engine::facts::Json;

// Exports no definition file or import library names, named from documentation instead, for
// people to read (documented()). name() does not use them: the decompiler's names come from
// files only, and these would change what it writes.
struct Documented {
    py::dict<int64_t, py::str> names;
    py::str where;
};
const py::dict<py::str, Documented>& DOCUMENTED() {
    static const py::dict<py::str, Documented> d = [] {
        py::dict<py::str, Documented> out;
        Documented k;
        k.names.set(17, py::str("CommonUnimpStub"));
        k.names.set(20, py::str("DosFileHandleToWin32Handle"));
        k.names.set(21, py::str("Win32HandleToDosFileHandle"));
        k.names.set(22, py::str("DisposeLZ32Handle"));
        k.names.set(41, py::str("ThunkInitLSF"));
        k.names.set(89, py::str("FT_PrologPrime"));
        k.names.set(90, py::str("QT_ThunkPrime"));
        k.where = py::str("Geoff Chappell, Ordinal-Only Exports In KERNEL32 4.0 (names per the Windows Me symbol file)");
        out.set(py::str("KERNEL32"), k);
        return out;
    }();
    return d;
}

using Names = std::tuple<py::dict<int64_t, py::str>, py::str>;    // ({ordinal: name}, where it was read)
py::dict<py::str, Names> _tables;                                  // MODULE -> from the corpus
std::optional<py::dict<py::str, Json>> _shipped;                   // ordinal_names.json
std::optional<py::dict<py::str, py::str>> _by_module_name;         // 16-bit module name -> file
py::dict<py::str, std::optional<std::set<int64_t>>> _seen;

py::str _stem(const py::str& module) { return api_arguments::_stem(module); }

// settings.state("module_names16.json"): a path inside the state folder, which is created if it is not there
py::str state_file(const py::str& name) {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), name);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

// {MODULE NAME: path} of the corpus's 16-bit modules, by the name each gives itself (a file called mnmnet.dll can be
// the module MNMNET_). Read once and kept in the state folder.
const py::dict<py::str, py::str>& _named_modules() {
    if (!_by_module_name) {
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
                    for (const auto& kv : j.dict.at(py::str("modules")).dict.items())
                        saved_modules.set(std::get<0>(kv), std::get<1>(kv).s);
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
        _by_module_name = saved_modules;
    }
    return *_by_module_name;
}

// ({ordinal: name}, file name) read from the module that exports them, or ({}, "").
const Names& _from_corpus(const py::str& stem) {
    if (!_tables.contains(stem)) {
        py::dict<int64_t, py::str> names;
        py::str where;
        std::optional<formats::ne::NEFile> nef;
        std::optional<api_arguments::NeModule> ne = api_arguments::_module(stem);
        if (ne) {
            nef = std::get<0>(*ne);
        } else if (_named_modules().contains(stem) && !_named_modules().at(stem).empty()) {
            try {
                nef = formats::ne::NEFile(_named_modules().at(stem));
            } catch (const std::exception&) {
                nef = std::nullopt;
            }
        }
        if (nef) {
            auto nonres = nef->nonresident_names();
            auto res = nef->resident_names();
            for (int64_t i = 1; i < nonres.size(); i++) names.set(std::get<1>(nonres[i]), std::get<0>(nonres[i]));
            for (int64_t i = 1; i < res.size(); i++) names.set(std::get<1>(res[i]), std::get<0>(res[i]));
            where = py::os::path::basename(nef->path).lower();
        } else {
            for (const py::str& ext : api_arguments::_EXTS) {
                std::optional<py::str> path = base::corpus::retail(stem.lower() + ext);
                if (path && !path->empty() && base::corpus::lane(*path) == std::optional<py::str>(py::str("pe"))) {
                    names = py::dict<int64_t, py::str>();
                    for (const auto& e : formats::pe::open_pe(*path).exports())
                        if (std::get<1>(e) && !std::get<1>(e)->empty()) names.set(std::get<0>(e), *std::get<1>(e));
                    where = py::os::path::basename(*path).lower();
                    break;
                }
            }
        }
        _tables.set(stem, Names(names, where));
    }
    return _tables.at(stem);
}

Names _from_file(const py::str& stem) {
    knowledge::reads::record(py::str("windows"), py::str("ordinal_names"), stem);
    if (!_shipped) _shipped = api_arguments::_knowledge_table(py::str("windows"), py::str("ordinal_names"));   // ordinal_names.json, as the knowledge base has it
    py::dict<int64_t, py::str> names;
    py::str from;
    if (_shipped->contains(stem) && _shipped->at(stem).kind == Json::Dict) {
        const Json& entry = _shipped->at(stem);
        if (entry.dict.contains(py::str("names")) && entry.dict.at(py::str("names")).kind == Json::Dict)
            for (const auto& kv : entry.dict.at(py::str("names")).dict.items())
                names.set(py::int_parse(std::get<0>(kv), 10), std::get<1>(kv).s);
        if (entry.dict.contains(py::str("from")) && entry.dict.at(py::str("from")).kind == Json::Str) from = entry.dict.at(py::str("from")).s;
    }
    return Names(names, from);
}

}  // namespace

std::optional<py::str> linker_name(const py::str& module, int64_t ordinal) {
    py::str stem = _stem(module);
    const Names& c = _from_corpus(stem);
    std::optional<py::str> found = std::get<0>(c).get(ordinal);
    if (found && !found->empty()) return found;
    std::optional<py::str> shipped = std::get<0>(_from_file(stem)).get(ordinal);
    if (shipped && !shipped->empty()) return shipped;
    return std::nullopt;          // (`a or b` with b falsy gives b: None or "")
}

std::optional<py::str> name(const py::str& module, int64_t ordinal) {
    std::optional<py::str> raw = linker_name(module, ordinal);
    if (raw && !raw->empty() && !raw->startswith(py::str("?"))) return prototypes::spelling(*raw);
    return raw;
}

std::optional<std::tuple<py::str, py::str>> documented(const py::str& module, int64_t ordinal) {
    py::str stem = _stem(module);
    if (!DOCUMENTED().contains(stem)) return std::nullopt;
    const Documented& d = DOCUMENTED().at(stem);
    if (!d.names.contains(ordinal)) return std::nullopt;
    return std::make_tuple(d.names.at(ordinal), d.where);
}

py::str source(const py::str& module, int64_t ordinal) {
    py::str stem = _stem(module);
    const Names& c = _from_corpus(stem);
    if (std::get<0>(c).contains(ordinal)) return std::get<1>(c);
    Names f = _from_file(stem);
    return std::get<0>(f).contains(ordinal) ? std::get<1>(f) : py::str("");
}

py::dict<int64_t, py::str> table(const py::str& module) {
    py::str stem = _stem(module);
    py::dict<int64_t, py::str> out = std::get<0>(_from_file(stem)).copy();
    out.update(std::get<0>(_from_corpus(stem)));
    py::dict<int64_t, py::str> spelled;
    for (const auto& kv : out.items()) {
        const py::str& nm = std::get<1>(kv);
        spelled.set(std::get<0>(kv), !nm.startswith(py::str("?")) ? prototypes::spelling(nm) : nm);
    }
    return spelled;
}

std::optional<py::str> identifier(const py::str& module, int64_t ordinal) {
    static const py::re::Pattern ident("[A-Za-z_]\\w*", 0);
    std::optional<py::str> nm = name(module, ordinal);
    if (nm && !nm->empty() && nm->contains(py::str("@")) && !nm->startswith(py::str("?")))
        nm = std::get<0>(demangle::plain(*nm));                 // `_Name@12`, `Name@12`
    if (nm && !nm->empty() && ident.fullmatch(*nm)) return nm;
    return std::nullopt;
}

py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> imports16(const py::str& module_path) {
    formats::ne::NEFile nef(module_path);
    py::list<py::str> refs = nef.module_refs();
    py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> out;
    for (const auto& seg : nef.segments()) {
        for (const auto& r : nef.relocations(seg)) {
            if (r.kind == py::str("IMPORTORD") && r.module && 0 < *r.module && *r.module <= refs.size()) {
                std::tuple<py::str, int64_t> key(refs[*r.module - 1].upper(), r.ordinal.value_or(0));
                if (!out.contains(key)) out.set(key, name(std::get<0>(key), std::get<1>(key)));
            }
        }
    }
    return out;
}

py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> imports32(const py::str& module_path) {
    py::dict<std::tuple<py::str, int64_t>, std::optional<py::str>> out;
    for (const auto& kv : formats::pe::open_pe(module_path).imports().items()) {
        const py::str& dll = std::get<0>(std::get<1>(kv));
        const py::str& nm = std::get<1>(std::get<1>(kv));
        if (nm.startswith(py::str("#")) && nm.slice(1, nm.size()).isdigit()) {
            std::tuple<py::str, int64_t> key(_stem(dll), py::int_parse(nm.slice(1, nm.size()), 10));
            if (!out.contains(key)) out.set(key, name(std::get<0>(key), std::get<1>(key)));
        }
    }
    return out;
}

// ---- reading definition files and import libraries (to make ordinal_names.json) ------------------

namespace {

// open(path, errors="replace").read() split into lines as universal newlines do
py::list<py::str> file_lines(const py::str& path) {
    py::bytes raw = py::open_rb(path).read();
    py::str text = raw.decode("latin1");
    text = text.replace(py::str("\r\n"), py::str("\n")).replace(py::str("\r"), py::str("\n"));
    py::list<py::str> lines;
    for (const py::str& l : py::split(text, py::str("\n"))) lines.append(l);
    if (!lines.empty() && lines[lines.size() - 1].empty()) lines.pop();
    return lines;
}

}  // namespace

py::dict<py::str, py::dict<int64_t, py::str>> read_definition_file(const py::str& path) {
    // {MODULE: {ordinal: name}} from a linker definition file (`name @ number [NONAME]`).
    static const py::re::Pattern def_line("^\\s*(\\S+?)(?:=\\S+)?\\s+@\\s*(\\d+)", 0);
    static const py::re::Pattern library("\\s*LIBRARY\\s+(\\w+)", 2);
    py::str module = std::get<0>(py::os::path::splitext(py::os::path::basename(path))).upper();
    py::dict<int64_t, py::str> names;
    for (const py::str& line0 : file_lines(path)) {
        py::str line = std::get<0>(line0.partition(py::str(";")));
        std::optional<py::re::Match> m = library.match(line);
        if (m) module = m->group(1).upper();
        m = def_line.match(line);
        if (m) names.setdefault(py::int_parse(m->group(2), 10), m->group(1));
    }
    py::dict<py::str, py::dict<int64_t, py::str>> out;
    if (!names.empty()) out.set(module, names);
    return out;
}

namespace {

// bytes.find(b"\0", start) as an index (-1 when there is none)
int64_t find_nul(const py::bytes& b, int64_t start) {
    if (start < 0) start = std::max<int64_t>(0, start + b.size());
    size_t p = b.raw().find('\0', static_cast<size_t>(start));
    return p == std::string::npos ? -1 : static_cast<int64_t>(p);
}

// (dll stem, ordinal, symbol) when a library member imports one function by number.
std::optional<std::tuple<py::str, int64_t, py::str>> _coff_import(const py::bytes& obj) {
    if (obj.size() >= 20 && obj.slice(0, 4) == py::bytes(std::string("\0\0\xff\xff", 4))) {      // the short form
        auto [hint, kind] = py::unpack_from<int64_t, int64_t>("<HH", obj, 16);
        py::bytes rest = obj.slice(20, obj.size());
        const std::string& r = rest.raw();
        size_t z = r.find('\0');
        py::bytes symbol(z == std::string::npos ? r : r.substr(0, z));
        py::bytes dll;
        if (z != std::string::npos) {
            size_t z2 = r.find('\0', z + 1);
            dll = py::bytes(z2 == std::string::npos ? r.substr(z + 1) : r.substr(z + 1, z2 - z - 1));
        }
        if (((kind >> 2) & 7) == 0) return std::make_tuple(_stem(dll.decode("latin1")), hint, symbol.decode("latin1"));
        return std::nullopt;
    }
    if (obj.size() < 20) return std::nullopt;
    auto [machine, nsec, ts, symptr, nsym] = py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t>("<HHIII", obj, 0);
    (void)machine;
    (void)ts;
    std::optional<int64_t> ordinal;
    for (int64_t i = 0; i < nsec; i++) {                                       // the long form: a little object file
        int64_t o = 20 + 40 * i;
        py::bytes nm = obj.slice(o, o + 8).rstrip(py::bytes(std::string("\0", 1)));
        if (nm == py::bytes(".idata$5") || nm == py::bytes(".idata$4")) {
            auto [rawsize, rawptr] = py::unpack_from<int64_t, int64_t>("<II", obj, o + 16);
            if (rawsize >= 4) {
                int64_t value = py::unpack_from1<int64_t>("<I", obj, rawptr);
                if (value & 0x80000000) ordinal = value & 0xFFFF;
            }
        }
    }
    if (!ordinal) return std::nullopt;
    int64_t strings = symptr + 18 * nsym, i = 0;
    std::optional<py::str> symbol, dll;
    while (i < nsym) {
        py::bytes rec = obj.slice(symptr + 18 * i, symptr + 18 * i + 18);
        if (rec.size() < 18) break;
        py::str nm;
        if (rec.slice(0, 4) == py::bytes(std::string("\0\0\0\0", 4))) {
            int64_t o = strings + py::unpack_from1<int64_t>("<I", rec, 4);
            nm = obj.slice(o, find_nul(obj, o)).decode("latin1");
        } else {
            nm = rec.slice(0, 8).rstrip(py::bytes(std::string("\0", 1))).decode("latin1");
        }
        if (nm.startswith(py::str("__imp_"))) symbol = nm.slice(6, nm.size());
        else if (nm.startswith(py::str("__IMPORT_DESCRIPTOR_"))) dll = nm.slice(20, nm.size());
        i += 1 + static_cast<unsigned char>(rec.raw()[17]);
    }
    if (symbol && !symbol->empty() && dll && !dll->empty()) return std::make_tuple(_stem(*dll), *ordinal, *symbol);
    return std::nullopt;
}

}  // namespace

py::dict<py::str, py::dict<int64_t, py::str>> read_import_library(const py::str& path) {
    // {MODULE: {ordinal: name}} of the functions an import library reaches by number. Reads the 32-bit form (an
    // archive of import members) and the 16-bit one (import records).
    py::bytes data = py::open_rb(path).read();
    py::dict<py::str, py::dict<int64_t, py::str>> out;
    if (data.slice(0, 8) == py::bytes("!<arch>\n")) {
        for (const auto& m : libraries::coff::members(data)) {
            auto hit = _coff_import(std::get<1>(m));
            if (hit) out.setdefault(std::get<0>(*hit), py::dict<int64_t, py::str>()).setdefault(std::get<1>(*hit), std::get<2>(*hit));
        }
        return out;
    }
    int64_t at = 0, page = 1;
    const std::string& d = data.raw();
    while (at + 3 <= data.size()) {              // 16-bit: comment records of class A0, kind 1
        int64_t kind = static_cast<unsigned char>(d[static_cast<size_t>(at)]);
        int64_t size = py::unpack_from1<int64_t>("<H", data, at + 1);
        py::bytes body = data.slice(at + 3, at + 3 + size - 1);
        at += 3 + size;
        if (kind == 0xF0) {                      // the library header gives the page size
            page = size + 3;
        } else if (kind == 0x8A || kind == 0x8B) {   // end of a member: the next starts on a page
            at += py::mod(-at, page);
        } else if (kind == 0xF1) {
            break;
        } else if (kind == 0x88 && body.size() > 4 && static_cast<unsigned char>(body.raw()[1]) == 0xA0 && body.raw()[2] == 1) {
            const std::string& b = body.raw();
            int64_t by_number = static_cast<unsigned char>(b[3]), n = static_cast<unsigned char>(b[4]);
            py::str internal = body.slice(5, 5 + n).decode("latin1");
            int64_t q = 5 + n;
            int64_t m = static_cast<unsigned char>(b.at(static_cast<size_t>(q)));
            py::str module = body.slice(q + 1, q + 1 + m).decode("latin1").upper();
            if (by_number && q + 3 + m <= body.size())
                out.setdefault(module, py::dict<int64_t, py::str>()).setdefault(py::unpack_from1<int64_t>("<H", body, q + 1 + m), internal);
        }
    }
    return out;
}

// ---- collect ---------------------------------------------------------------------------------------------------------

namespace {

// Does the corpus module export that number at all? (None: the module is not in the corpus.)
std::optional<bool> _has_export(const py::str& stem, int64_t ordinal) {
    if (!_seen.contains(stem)) {
        std::optional<std::set<int64_t>> numbers;
        std::optional<api_arguments::NeModule> ne = api_arguments::_module(stem);
        if (ne) {
            numbers = std::set<int64_t>();
            for (int64_t k : std::get<1>(*ne).keys()) numbers->insert(k);
        } else if (_named_modules().contains(stem) && !_named_modules().at(stem).empty()) {
            numbers = std::set<int64_t>();
            for (int64_t o : formats::ne::NEFile(_named_modules().at(stem)).export_ordinals()) numbers->insert(o);
        } else {
            for (const py::str& ext : api_arguments::_EXTS) {
                std::optional<py::str> path = base::corpus::retail(stem.lower() + ext);
                if (path && !path->empty() && base::corpus::lane(*path) == std::optional<py::str>(py::str("pe"))) {
                    numbers = std::set<int64_t>();
                    for (const auto& e : formats::pe::open_pe(*path).exports()) numbers->insert(std::get<0>(e));
                    break;
                }
            }
        }
        _seen.set(stem, numbers);
    }
    const std::optional<std::set<int64_t>>& n = _seen.at(stem);
    if (!n) return std::nullopt;
    return n->count(ordinal) != 0;
}

// Does the corpus module's export with that number take the arguments the name says? True / False, or None when the
// name or the code does not settle it. A table made for another release of a module gives itself away here: its
// names land on other functions.
std::optional<bool> _agrees(const py::str& stem, int64_t ordinal, const py::str& nm) {
    std::optional<int64_t> expect = demangle::stack_bytes(nm);
    std::shared_ptr<api_arguments::_Mod> mod = expect ? api_arguments::_mod(stem) : nullptr;
    std::optional<int64_t> rva = mod ? mod->ords.get(ordinal) : std::nullopt;
    if (!rva || *rva == 0) return std::nullopt;
    std::optional<int64_t> got;
    try {
        got = api_arguments::_scan(*mod, *rva, 4);
    } catch (const std::exception&) {
        got = std::nullopt;
    }
    if (!got) return std::nullopt;
    return *got == *expect;
}

}  // namespace

py::dict<py::str, Shipped> collect(const py::list<py::str>& paths, const std::function<void(const py::str&)>& log0) {
    // Rewrite ordinal_names.json from definition files and import libraries (files or folders; earlier ones win).
    // Kept: names for numbers the corpus module exports without a name, and whole tables of modules the corpus does
    // not hold but its modules import from. Each name is checked against the export it would name (see _agrees); one
    // that disagrees is dropped, and a table that disagrees for more than one function in twenty is dropped whole.
    std::function<void(const py::str&)> log = log0 ? log0 : [](const py::str& s) { py::print(s); };
    py::list<py::str> files;
    for (const py::str& p : paths) {
        if (py::os::path::isdir(p)) {
            for (const auto& step : py::os::walk(p)) {
                py::list<py::str> dirs = std::get<1>(step), fns = std::get<2>(step);
                std::vector<py::str> keep;
                for (const py::str& d : dirs)
                    if (!(d.upper() == py::str("ALPHA") || d.upper() == py::str("MIPS") || d.upper() == py::str("PPC"))) keep.push_back(d);
                std::sort(keep.begin(), keep.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
                dirs.clear();                       // dirs[:] = sorted(...)
                for (const py::str& d : keep) dirs.append(d);
                std::vector<py::str> names;
                for (const py::str& f : fns) names.push_back(f);
                std::sort(names.begin(), names.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
                for (const py::str& f : names) files.append(py::os::path::join(std::get<0>(step), f));
            }
        } else {
            files.append(p);
        }
    }
    py::set<py::str> wanted;                        // modules something in the corpus imports by number
    for (const py::str& path : base::corpus::modules(py::str("ne"), base::corpus::EXTS_)) {
        try {
            for (const auto& kv : imports16(path).items()) wanted.add(std::get<0>(std::get<0>(kv)));
        } catch (const std::exception&) {
        }
    }
    for (const py::str& path : base::corpus::modules(py::str("pe"), base::corpus::EXTS_)) {
        try {
            for (const auto& kv : imports32(path).items()) wanted.add(std::get<0>(std::get<0>(kv)));
        } catch (const std::exception&) {
        }
    }
    py::dict<py::str, Shipped> out;
    bool coff = false;                              // (Python's `coff` keeps its last value when a file fails to read)
    for (const py::str& path : files) {
        py::str ext = std::get<1>(py::os::path::splitext(path)).lower();
        py::dict<py::str, py::dict<int64_t, py::str>> found;
        try {
            found = ext == py::str(".def") ? read_definition_file(path) : ext == py::str(".lib") ? read_import_library(path)
                                                                                                    : py::dict<py::str, py::dict<int64_t, py::str>>();
            py::file f = py::open_rb(path);
            coff = ext == py::str(".lib") && f.read(8) == py::bytes("!<arch>\n");
        } catch (const std::exception&) {
            found = py::dict<py::str, py::dict<int64_t, py::str>>();
        }
        for (const auto& mv : found.items()) {
            const py::str& module = std::get<0>(mv);
            const py::dict<int64_t, py::str>& names = std::get<1>(mv);
            py::dict<int64_t, py::str> own = std::get<0>(_from_corpus(module));
            py::dict<int64_t, py::str> keep;
            for (const auto& ov : names.items()) {
                int64_t o = std::get<0>(ov);
                std::optional<bool> present = _has_export(module, o);
                if (own.contains(o) || (present && !*present) || (!present && !wanted.contains(module))) continue;
                keep.set(o, std::get<1>(ov));
            }
            if (keep.empty() || out.contains(module)) continue;
            py::dict<int64_t, std::optional<bool>> checks;
            for (const auto& kv : keep.items()) checks.set(std::get<0>(kv), _agrees(module, std::get<0>(kv), std::get<1>(kv)));
            if (coff) {                             // a 32-bit library holds the linker's symbols
                py::dict<int64_t, py::str> plain;
                for (const auto& kv : keep.items()) plain.set(std::get<0>(kv), std::get<0>(demangle::plain(std::get<1>(kv))));
                keep = plain;
            }
            int64_t yes = 0, no = 0;
            for (const auto& kv : checks.items()) {
                if (std::get<1>(kv) && *std::get<1>(kv)) yes += 1;
                if (std::get<1>(kv) && !*std::get<1>(kv)) no += 1;
            }
            if (no * 20 > yes + no) {
                log(py::percent(py::str("%-10s NOT USED: %s names %d exports rightly and %d wrongly"), module,
                                py::os::path::basename(path), yes, no));
                continue;
            }
            Shipped s;
            s.from = py::os::path::basename(path);
            s.checked = {yes, no};
            for (const auto& kv : keep.items()) {
                std::optional<bool> c = checks.at(std::get<0>(kv));
                if (!(c && !*c)) s.names.set(py::to_str(std::get<0>(kv)), std::get<1>(kv));
            }
            out.set(module, s);
        }
    }
    // json.dump(out, f, sort_keys=True, indent=0)
    {
        auto sorted_keys = [](const std::vector<py::str>& v) {
            std::vector<py::str> k = v;
            std::sort(k.begin(), k.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
            return k;
        };
        std::string o;
        std::vector<py::str> mods;
        for (const py::str& k : out.keys()) mods.push_back(k);
        mods = sorted_keys(mods);
        if (mods.empty()) {
            o = "{}";
        } else {
            o = "{\n";
            for (size_t i = 0; i < mods.size(); i++) {
                const Shipped& s = out.at(mods[i]);
                py::json::put(o, mods[i]);
                o += ": {\n\"checked\": [\n" + std::to_string(std::get<0>(s.checked)) + ",\n" + std::to_string(std::get<1>(s.checked)) +
                     "\n],\n\"from\": ";
                py::json::put(o, s.from);
                o += ",\n\"names\": ";
                std::vector<py::str> nk;
                for (const py::str& k : s.names.keys()) nk.push_back(k);
                nk = sorted_keys(nk);
                if (nk.empty()) {
                    o += "{}";
                } else {
                    o += "{\n";
                    for (size_t j = 0; j < nk.size(); j++) {
                        py::json::put(o, nk[j]);
                        o += ": ";
                        py::json::put(o, s.names.at(nk[j]));
                        o += j + 1 < nk.size() ? ",\n" : "\n";
                    }
                    o += "}";
                }
                o += i + 1 < mods.size() ? "\n},\n" : "\n}\n";
            }
            o += "}";
        }
        py::str table_file = py::os::path::join(knowledge::sources::_SYMBOLS(), py::str("ordinal_names.json"));
        py::file f = py::open(table_file, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
        f.write(py::str(o));
        f.close();
        knowledge::sources::changed(table_file);            // the knowledge base loads it again
        _shipped = std::nullopt;
    }
    std::vector<py::str> mods;
    for (const py::str& k : out.keys()) mods.push_back(k);
    std::sort(mods.begin(), mods.end(), [](const py::str& a, const py::str& b) { return a.u32() < b.u32(); });
    for (const py::str& module : mods) {
        const Shipped& s = out.at(module);
        log(py::percent(py::str("%-10s %5d names from %-14s (%d checked against the code, %d dropped)"), module,
                        static_cast<int64_t>(s.names.size()), s.from, std::get<0>(s.checked), std::get<1>(s.checked)));
    }
    return out;
}

}  // namespace codebreaker::symbols::ordinals
