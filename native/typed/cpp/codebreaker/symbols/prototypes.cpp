// codebreaker/symbols/prototypes.cpp -- typed C++ port of codebreaker.symbols.prototypes (see prototypes.hpp).

#include "codebreaker/symbols/prototypes.hpp"

#include <mutex>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/symbols/native.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::prototypes {

namespace {

using engine::facts::Json;
using engine::facts::Value;

std::mutex g_lock;
std::optional<Table> g_table;

py::list<py::str> existing_dirs(const std::initializer_list<py::str>& dirs) {
    py::list<py::str> out;
    for (const py::str& d : dirs) {
        if (!d.empty() && py::os::path::isdir(d)) out.append(d);
    }
    return out;
}

Json json_str(const py::str& s) {
    Json j;
    j.kind = Json::Str;
    j.s = s;
    return j;
}

Json to_json(const Prototype& p) {
    Json j;
    j.kind = Json::Dict;
    j.dict.set(py::str("returns"), json_str(p.returns));
    j.dict.set(py::str("convention"), json_str(p.convention));
    j.dict.set(py::str("marker"), json_str(p.marker));
    j.dict.set(py::str("header"), json_str(p.header));
    Json params;
    params.kind = Json::List;
    for (const auto& t : p.parameters) {
        Json pair;
        pair.kind = Json::List;
        pair.list.push_back(json_str(std::get<0>(t)));
        pair.list.push_back(json_str(std::get<1>(t)));
        params.list.push_back(pair);
    }
    j.dict.set(py::str("parameters"), params);
    return j;
}

Prototype from_json(const Json& j) {
    Prototype p;
    p.returns = j.dict.at(py::str("returns")).s;
    p.convention = j.dict.at(py::str("convention")).s;
    p.marker = j.dict.at(py::str("marker")).s;
    p.header = j.dict.at(py::str("header")).s;
    for (const Json& pair : j.dict.at(py::str("parameters")).list) p.parameters.append({pair.list.at(0).s, pair.list.at(1).s});
    return p;
}

py::dict<py::str, Prototype> prototypes_of(const py::dict<py::str, Value>& saved) {
    py::dict<py::str, Prototype> out;
    for (const auto& kv : saved.items()) out.set(std::get<0>(kv), from_json(std::get<Json>(std::get<1>(kv))));
    return out;
}

// {"32": {name: prototype}, "16": {...}}: every header of every folder, the first folder's declaration kept,
// a later folder's naming the parameters a first one left unnamed. (As the DLL's JSON: kept as it is.)
py::dict<py::str, py::dict<py::str, Value>> build(const Folders& folders) {
    // spec = "".join("%s\t%s\n" % (bits, d) for bits in folders for d in folders[bits])
    py::str spec;
    for (const py::str& d : folders.w32) spec += py::str("32\t") + d + py::str("\n");
    for (const py::str& d : folders.w16) spec += py::str("16\t") + d + py::str("\n");
    std::optional<native::Json> answer = native::ask(py::str("prototypes.build"), spec);
    py::dict<py::str, py::dict<py::str, Value>> out;
    if (!answer) throw py::RuntimeError("prototypes.build: no answer");
    Json all = engine::facts::from_native(*answer);
    for (const auto& kv : all.dict.items()) {
        py::dict<py::str, Value> protos;
        for (const auto& pv : std::get<1>(kv).dict.items()) protos.set(std::get<0>(pv), Value(std::get<1>(pv)));
        out.set(std::get<0>(kv), protos);
    }
    return out;
}

}  // namespace

Folders _folders() {
    py::str ow = build::watcom::openwatcom_root();
    py::str ref = py::settings::get_str(py::str("reference"));
    if (ref.empty()) ref = py::os::path::join(base::corpus::design_dir(), py::str("reference"));
    Folders f;
    f.w32 = existing_dirs({ow.empty() ? py::str("") : py::os::path::join(ow, py::str("h"), py::str("nt")),
                           py::os::path::join(ref, py::str("WIN32SDK"), py::str("MSTOOLS"), py::str("INCLUDE")),
                           py::os::path::join(ref, py::str("Microsoft Windows 95 DDK"), py::str("INC32"))});
    f.w16 = existing_dirs({ow.empty() ? py::str("") : py::os::path::join(ow, py::str("h"), py::str("win")),
                           py::os::path::join(ref, py::str("Microsoft Windows 95 DDK"), py::str("INC16"))});
    return f;
}

py::str _stamp(const Folders& folders) {
    // the reader is part of what the table is made from
    py::bytes dll = py::open_rb(native::dll_path()).read();
    py::list<py::str> parts{py::str(std::to_string(REV)), py::hashlib::sha1(dll).hexdigest().slice(0, 16)};
    auto add = [&](const py::list<py::str>& dirs) {
        for (const py::str& d : dirs) {
            int64_t names = 0;
            for (const py::str& fn : py::os::listdir(d)) {
                if (fn.lower().endswith(".h")) names += 1;
            }
            parts.append(py::percent(py::str("%s:%d:%d"), d, names, static_cast<int64_t>(py::os::path::getmtime(d))));
        }
    };
    add(folders.w16);                                 // sorted(folders): "16" before "32"
    add(folders.w32);
    return py::join(py::str("|"), parts);
}

const Table& table() {
    std::lock_guard<std::mutex> guard(g_lock);
    if (!g_table) {
        Folders folders = _folders();
        py::str stamp = _stamp(folders);
        auto kb = knowledge::store::open();
        std::optional<py::dict<py::str, py::dict<py::str, Value>>> built;

        auto made = [&](const char* bits) {
            return [&, bits]() -> py::dict<py::str, Value> {
                if (!built) built = build(folders);
                return built->at(py::str(bits));
            };
        };
        py::list<py::str> dirs;
        for (const py::str& d : folders.w16) dirs.append(d);
        for (const py::str& d : folders.w32) dirs.append(d);
        py::str source = py::str("the headers: ") + py::join(py::str("; "), dirs);
        auto saved32 = kb->ensure_values(py::str("win32"), py::str("prototype"), stamp, made("32"), source, knowledge::store::DERIVED);
        auto saved16 = kb->ensure_values(py::str("win16"), py::str("prototype"), stamp, made("16"), source, knowledge::store::DERIVED);
        Table t;
        t.w32 = prototypes_of(saved32);
        t.w16 = prototypes_of(saved16);
        for (const py::str& name : t.w32.keys()) t.spelling.set(name.upper(), name);   // the 16-bit spelling wins:
        for (const py::str& name : t.w16.keys()) t.spelling.set(name.upper(), name);   // that is where it is needed
        g_table = std::move(t);
    }
    return *g_table;
}

std::optional<Prototype> of(const py::str& name, int64_t bits) {
    const Table& t = table();
    if (bits != 32 && bits != 16) throw py::KeyError(py::repr(py::str(std::to_string(bits))).utf8());
    const py::dict<py::str, Prototype>& protos = bits == 16 ? t.w16 : t.w32;
    py::str platform = bits == 16 ? py::str("win16") : py::str("win32");
    std::optional<Prototype> found = protos.get(name);
    knowledge::reads::record(platform, py::str("prototype"), name);   // (the step asking is made from it)
    if (!found) {
        py::str spelled = t.spelling.get(name.upper(), py::str(""));
        if (!spelled.empty() && spelled != name) knowledge::reads::record(platform, py::str("prototype"), spelled);
        found = protos.get(spelled);
    }
    return found;
}

py::str spelling(const py::str& name) {
    if (name.empty() || !name.isupper()) return name;
    const py::dict<py::str, py::str>& known = table().spelling;
    auto a = known.get(name);
    if (a && !a->empty()) return *a;
    if (name.startswith(py::str("_"))) {
        auto b = known.get(name.slice_from(1));
        if (b && !b->empty()) return *b;
    }
    return name;
}

std::optional<py::str> declaration(const py::str& name, int64_t bits) {
    std::optional<Prototype> p = of(name, bits);
    if (!p) return std::nullopt;
    py::list<py::str> params;
    for (const auto& t : p->parameters) params.append((std::get<0>(t) + py::str(" ") + std::get<1>(t)).strip());
    py::str joined = py::join(py::str(", "), params);
    if (joined.empty()) joined = py::str("void");
    const Table& t = table();
    const py::dict<py::str, Prototype>& protos = bits == 16 ? t.w16 : t.w32;
    py::str real = !protos.contains(name) ? t.spelling.get(name.upper(), name) : name;
    return py::percent(py::str("%s %s %s(%s)"), p->returns, p->marker, real, joined);
}

}  // namespace codebreaker::symbols::prototypes
