// codebreaker/symbols/prototypes.cpp -- the shim's translation unit (see prototypes.hpp): conversions between the old
// types and cb/symbols/prototypes, and the table's home in the old knowledge base. No logic of its own.
#include "codebreaker/symbols/prototypes.hpp"

#include <mutex>

#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/prototypes.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::prototypes {

namespace {

namespace np = cb::symbols::prototypes;
using cb::compat::from_py;
using cb::compat::to_py;
using engine::facts::Json;
using engine::facts::Value;

std::mutex g_lock;
std::optional<Table> g_table;
std::optional<np::PrototypeTable> g_new;   // the same declarations, for the new module's lookups

np::HeaderFolders to_new(const Folders& f) {
    np::HeaderFolders out;
    for (const py::str& d : f.w32) out.w32.push_back(cb::fs::from_text(d.utf8()));
    for (const py::str& d : f.w16) out.w16.push_back(cb::fs::from_text(d.utf8()));
    return out;
}

Folders to_old(const np::HeaderFolders& f) {
    Folders out;
    for (const auto& d : f.w32) out.w32.append(to_py(cb::fs::to_text(d)));
    for (const auto& d : f.w16) out.w16.append(to_py(cb::fs::to_text(d)));
    return out;
}

Prototype to_old(const np::Prototype& p) {
    Prototype out;
    out.returns = to_py(p.returns);
    out.convention = to_py(p.convention);
    out.marker = to_py(p.marker);
    out.header = to_py(p.header);
    for (const np::Parameter& a : p.parameters) out.parameters.append({to_py(a.type), to_py(a.name)});
    return out;
}

np::Prototype to_new(const Prototype& p) {
    np::Prototype out;
    out.returns = from_py(p.returns);
    out.convention = from_py(p.convention);
    out.marker = from_py(p.marker);
    out.header = from_py(p.header);
    for (const auto& t : p.parameters) out.parameters.push_back({from_py(std::get<0>(t)), from_py(std::get<1>(t))});
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

py::dict<py::str, Value> values_of(const np::PrototypeMap& map) {
    py::dict<py::str, Value> out;
    for (const auto& [name, p] : map) out.set(to_py(name), Value(to_json(to_old(p))));
    return out;
}

np::PrototypeMap map_of(const py::dict<py::str, Prototype>& d) {
    np::PrototypeMap out;
    for (const auto& kv : d.items()) out.insert_or_assign(from_py(std::get<0>(kv)), to_new(std::get<1>(kv)));
    return out;
}

}  // namespace

Folders _folders() {
    const py::str ow = build::watcom::openwatcom_root();
    py::str ref = py::settings::get_str(py::str("reference"));
    if (ref.empty()) ref = py::os::path::join(base::corpus::design_dir(), py::str("reference"));
    return to_old(np::HeaderFolders::standard(cb::fs::from_text(ow.utf8()), cb::fs::from_text(ref.utf8())));
}

py::str _stamp(const Folders& folders) { return to_py(np::stamp(to_new(folders))); }

const Table& table() {
    std::lock_guard<std::mutex> guard(g_lock);
    if (!g_table) {
        const Folders folders = _folders();
        const py::str stamp = _stamp(folders);
        auto kb = knowledge::store::open();
        std::optional<np::Headers> built;
        const auto made = [&](bool w32) {
            return [&, w32]() -> py::dict<py::str, Value> {
                if (!built) built = np::read_headers(to_new(folders));
                return values_of(w32 ? built->w32 : built->w16);
            };
        };
        py::list<py::str> dirs;
        for (const py::str& d : folders.w16) dirs.append(d);
        for (const py::str& d : folders.w32) dirs.append(d);
        const py::str source = py::str("the headers: ") + py::join(py::str("; "), dirs);
        auto saved32 = kb->ensure_values(py::str("win32"), py::str("prototype"), stamp, made(true), source, knowledge::store::DERIVED);
        auto saved16 = kb->ensure_values(py::str("win16"), py::str("prototype"), stamp, made(false), source, knowledge::store::DERIVED);
        Table t;
        t.w32 = prototypes_of(saved32);
        t.w16 = prototypes_of(saved16);
        g_new.emplace(np::Headers{map_of(t.w32), map_of(t.w16)});
        for (const py::str& name : t.w32.keys()) t.spelling.set(name.upper(), name);   // the 16-bit spelling wins:
        for (const py::str& name : t.w16.keys()) t.spelling.set(name.upper(), name);   // that is where it is needed
        g_table = std::move(t);
    }
    return *g_table;
}

std::optional<Prototype> of(const py::str& name, int64_t bits) {
    const Table& t = table();
    if (bits != 32 && bits != 16) throw py::KeyError(py::repr(py::str(std::to_string(bits))).utf8());
    const np::Prototype* found = g_new->of(from_py(name), bits == 16 ? np::Bits::W16 : np::Bits::W32);
    // COMPAT(py): remove when the staged knowledge/reads shim is installed (the old engine records its reads in the old layer)
    const py::str platform = bits == 16 ? py::str("win16") : py::str("win32");
    knowledge::reads::record(platform, py::str("prototype"), name);
    const py::str spelled = t.spelling.get(name.upper(), py::str(""));
    if (!spelled.empty() && spelled != name && !(bits == 16 ? t.w16 : t.w32).contains(name)) {
        knowledge::reads::record(platform, py::str("prototype"), spelled);
    }
    if (found == nullptr) return std::nullopt;
    return to_old(*found);
}

py::str spelling(const py::str& name) {
    table();
    return to_py(g_new->spelling(from_py(name)));
}

std::optional<py::str> declaration(const py::str& name, int64_t bits) {
    if (!of(name, bits)) return std::nullopt;   // (asks, and records the read, as the old layer does)
    return to_py(g_new->declaration(from_py(name), bits == 16 ? np::Bits::W16 : np::Bits::W32));
}

}  // namespace codebreaker::symbols::prototypes
