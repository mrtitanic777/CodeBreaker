// codebreaker/symbols/sdk_structs.cpp -- the shim's translation unit (see sdk_structs.hpp): conversions between the old
// types and cb/symbols/sdk_structs, and the layouts' home in the old knowledge base. No logic of its own.
#include "codebreaker/symbols/sdk_structs.hpp"

#include <mutex>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/sdk_structs.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/source/readable.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::symbols::sdk_structs {

namespace {

namespace ns = cb::symbols::sdk_structs;
using cb::compat::from_py;
using cb::compat::to_py;
using engine::facts::Json;
using engine::facts::Value;

std::mutex g_lock;
std::optional<std::tuple<Structs, Pointers>> g_db;   // the table as the knowledge base keeps it
std::optional<ns::StructTable> g_new;                // the same table, for the new module's lookups

// ---- old <-> new ----
ns::Layout to_new(const Layout& l) {
    ns::Layout out;
    out.size = static_cast<std::uint32_t>(l.size);
    out.alignment = static_cast<std::uint32_t>(l.biggest);
    int64_t at = 0;
    for (const Field& f : l.fields) {
        ns::Field n;
        n.offset = static_cast<std::uint32_t>(std::get<0>(f));
        n.size = static_cast<std::uint32_t>(std::get<1>(f));
        n.name = from_py(std::get<2>(f));
        n.ctype = from_py(std::get<3>(f));
        n.pointer = at < l.pointer.size() && l.pointer[at];
        out.fields.push_back(std::move(n));
        ++at;
    }
    return out;
}

Layout to_old(const ns::Layout& l) {
    Layout out;
    out.size = l.size;
    out.biggest = l.alignment;
    for (const ns::Field& f : l.fields) {
        out.fields.append({f.offset, f.size, to_py(f.name), to_py(f.ctype)});
        out.pointer.append(f.pointer);
    }
    return out;
}

ns::HeaderStructs to_new(const Structs& structs, const Pointers& pointers) {
    ns::HeaderStructs out;
    for (const auto& kv : structs.items()) {
        const auto& l = std::get<1>(kv);
        out.structs.insert_or_assign(from_py(std::get<0>(kv)), l ? std::optional<ns::Layout>(to_new(*l)) : std::nullopt);
    }
    for (const auto& kv : pointers.items()) out.pointers.insert_or_assign(from_py(std::get<0>(kv)), from_py(std::get<1>(kv)));
    return out;
}

std::tuple<Structs, Pointers> to_old(const ns::HeaderStructs& h) {
    Structs structs;
    Pointers pointers;
    for (const auto& [name, l] : h.structs) structs.set(to_py(name), l ? std::optional<Layout>(to_old(*l)) : std::nullopt);
    for (const auto& [name, target] : h.pointers) pointers.set(to_py(name), to_py(target));
    return {structs, pointers};
}

// ---- the knowledge base's forms ----
// Json forms of what the knowledge base keeps: a structure is (size, [(offset, size, name, ctype), ...], biggest) --
// tuples, as the Python kept them -- or null.
Json json_int(int64_t v) {
    Json j;
    j.kind = Json::Int;
    j.i = v;
    return j;
}
Json json_text(const py::str& s, bool shared = false) {
    Json j;
    j.kind = Json::Str;
    j.s = s;
    j.shared = shared;
    return j;
}
Json layout_json(const std::optional<Layout>& l) {
    Json j;
    if (!l) return j;   // null
    j.kind = Json::List;
    j.tuple = true;
    j.list.push_back(json_int(l->size));
    Json fields;
    fields.kind = Json::List;
    int64_t at = 0;
    for (const Field& f : l->fields) {
        Json x;
        x.kind = Json::List;
        x.tuple = true;
        x.list.push_back(json_int(std::get<0>(f)));
        x.list.push_back(json_int(std::get<1>(f)));
        x.list.push_back(json_text(std::get<2>(f)));
        x.list.push_back(json_text(std::get<3>(f), at < l->pointer.size() && l->pointer[at]));
        fields.list.push_back(x);
        ++at;
    }
    j.list.push_back(fields);
    j.list.push_back(json_int(l->biggest));
    return j;
}
std::optional<Layout> layout_of(const Json& j) {
    if (j.kind != Json::List) return std::nullopt;
    Layout l;
    l.size = j.list.at(0).i;
    for (const Json& x : j.list.at(1).list) l.fields.append({x.list.at(0).i, x.list.at(1).i, x.list.at(2).s, x.list.at(3).s});
    l.biggest = j.list.at(2).i;
    return l;
}

}  // namespace

// ---- Struct ----
std::optional<std::tuple<py::str, py::str>> Struct::typed(int64_t offset, int64_t size_) const {
    _read();
    Layout l;
    l.fields = fields;
    ns::Layout n = to_new(l);
    const auto found = g_new->typed_field(n, static_cast<std::uint32_t>(offset), static_cast<std::uint32_t>(size_));
    if (!found) return std::nullopt;
    return std::make_tuple(to_py(found->path), to_py(found->ctype));
}

std::optional<py::str> Struct::at(int64_t offset, int64_t size_) const {
    _read();
    Layout l;
    l.fields = fields;
    ns::Layout n = to_new(l);
    return to_py(g_new->field_at(n, static_cast<std::uint32_t>(offset), static_cast<std::uint32_t>(size_)));
}

std::tuple<Structs, Pointers> _from_headers() {
    ns::Constants constants;
    for (const auto& kv : source::readable::_constants().values.items()) constants[from_py(std::get<0>(kv))] = std::get<1>(kv);
    std::vector<std::filesystem::path> folders;
    for (const py::str& d : prototypes::_folders().w32) folders.push_back(cb::fs::from_text(d.utf8()));
    return to_old(ns::read_headers(folders, constants));
}

std::tuple<Structs, Pointers> _read() {
    std::lock_guard<std::mutex> guard(g_lock);
    if (g_db) return *g_db;
    prototypes::Folders folders = prototypes::_folders();
    py::str stamp = py::hashlib::sha1(py::bytes((prototypes::_stamp(folders) + py::str("|sdk_structs:") +
                                                  py::str(std::to_string(ns::kReaderRevision)))
                                                     .utf8()))
                        .hexdigest();
    auto kb = knowledge::store::open();
    std::optional<std::tuple<Structs, Pointers>> made;
    auto from_made = [&]() -> const std::tuple<Structs, Pointers>& {
        if (!made) made = _from_headers();
        return *made;
    };
    py::list<py::str> dirs;
    for (const py::str& d : folders.w32) dirs.append(d);
    py::str source = py::str("the 32-bit headers: ") + py::join(py::str("; "), dirs);
    auto structs_v = kb->ensure_values(py::str("win32"), py::str("struct"), stamp,
                                       [&]() {
                                           py::dict<py::str, Value> out;
                                           for (const auto& kv : std::get<0>(from_made()).items())
                                               out.set(std::get<0>(kv), Value(layout_json(std::get<1>(kv))));
                                           return out;
                                       },
                                       source, knowledge::store::DERIVED);
    auto pointers_v = kb->ensure_values(py::str("win32"), py::str("struct.pointer"), stamp,
                                        [&]() {
                                            py::dict<py::str, Value> out;
                                            for (const auto& kv : std::get<1>(from_made()).items())
                                                out.set(std::get<0>(kv), Value(json_text(std::get<1>(kv))));
                                            return out;
                                        },
                                        source, knowledge::store::DERIVED);
    Structs structs;
    for (const auto& kv : structs_v.items()) structs.set(std::get<0>(kv), layout_of(std::get<Json>(std::get<1>(kv))));
    Pointers pointers;
    for (const auto& kv : pointers_v.items()) {
        const Value& v = std::get<1>(kv);
        pointers.set(std::get<0>(kv), std::holds_alternative<py::str>(v) ? std::get<py::str>(v) : std::get<Json>(v).s);
    }
    g_new.emplace(to_new(structs, pointers));
    g_db = std::make_tuple(structs, pointers);
    return *g_db;
}

std::optional<Struct> layout(const py::str& name) {
    auto [structs, pointers] = _read();
    (void)pointers;
    // COMPAT(py): remove when the staged knowledge/reads shim is installed (the old engine records its reads in the old layer)
    knowledge::reads::record(py::str("win32"), py::str("struct"), name);
    auto s = structs.get(name);
    if (s && *s) return Struct(name, (*s)->size, (*s)->fields);
    return std::nullopt;
}

std::optional<py::str> pointed_to(const py::str& type_text) {
    _read();
    const auto found = g_new->pointed_to(from_py(type_text));
    // COMPAT(py): remove when the staged knowledge/reads shim is installed (the old engine records its reads in the old layer)
    knowledge::reads::record(py::str("win32"), py::str("struct.pointer"), type_text);
    if (found) knowledge::reads::record(py::str("win32"), py::str("struct"), to_py(*found));
    return to_py(found);
}

py::str definition(const Struct& s, const py::set<py::str>& known) {
    _read();
    std::vector<std::string> done;
    for (const py::str& k : known) done.push_back(from_py(k));
    return to_py(g_new->definition(from_py(s.name), done));
}

}  // namespace codebreaker::symbols::sdk_structs
