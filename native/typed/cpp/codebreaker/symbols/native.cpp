// codebreaker/symbols/native.cpp -- the shim's translation unit (see native.hpp): json.loads over cb::Json, and the three
// questions of cbsymbols.dll answered by cb/symbols/demangle and cb/symbols/prototypes. No logic of its own.
#include "codebreaker/symbols/native.hpp"

#include <nlohmann/json.hpp>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/symbols/demangle.hpp"
#include "cb/symbols/prototypes.hpp"

namespace codebreaker::symbols::native {

namespace {

using cb::compat::to_py;

Json from_json(const cb::Json& j) {
    Json out;
    if (j.is_boolean()) {
        out.value = j.get<bool>();
    } else if (j.is_number_integer()) {
        out.value = static_cast<int64_t>(j.get<std::int64_t>());
    } else if (j.is_number_unsigned()) {
        out.value = static_cast<int64_t>(j.get<std::uint64_t>());
    } else if (j.is_number_float()) {
        out.value = j.get<double>();
    } else if (j.is_string()) {
        out.value = to_py(j.get<std::string>());
    } else if (j.is_array()) {
        Json::Array a;
        for (const auto& x : j) a.push_back(from_json(x));
        out.value = std::move(a);
    } else if (j.is_object()) {
        Json::Object o;
        for (const auto& [k, v] : j.items()) o.emplace_back(to_py(k), from_json(v));
        out.value = std::move(o);
    }
    return out;   // null: the monostate
}

Json text_json(const std::string& s) {
    Json j;
    j.value = to_py(s);
    return j;
}

Json declaration_json(const cb::symbols::demangle::Declaration& d) {
    namespace nd = cb::symbols::demangle;
    cb::Json j = cb::Json::object();
    if (d.kind == nd::Kind::Data && d.base.empty()) {   // a string literal: name, kind and text only
        j["name"] = d.name;
        j["kind"] = "data";
        j["text"] = d.text;
        return from_json(j);
    }
    j["name"] = d.name;
    j["base"] = d.base;
    j["class"] = d.owner;
    j["kind"] = d.kind == nd::Kind::Function ? "function" : d.kind == nd::Kind::Data ? "data" : "name";
    j["access"] = d.access;
    j["static"] = d.is_static;
    j["virtual"] = d.is_virtual;
    j["far"] = d.far;
    j["convention"] = d.convention;
    j["returns"] = d.returns;
    j["parameters"] = d.parameters;
    j["const"] = d.const_qualifier;
    j["text"] = d.text;
    return from_json(j);
}

cb::Json prototypes_json(const cb::symbols::prototypes::PrototypeMap& m) {
    cb::Json out = cb::Json::object();
    for (const auto& [name, p] : m) {
        cb::Json params = cb::Json::array();
        for (const auto& a : p.parameters) params.push_back(cb::Json::array({a.type, a.name}));
        out[name] = cb::Json{{"returns", p.returns}, {"convention", p.convention}, {"marker", p.marker}, {"parameters", params}, {"header", p.header}};
    }
    return out;
}

}  // namespace

Json loads(const py::str& text) {
    try {
        return from_json(cb::json::parse(text.utf8()));
    } catch (const cb::ParseError& e) {
        throw JSONDecodeError(e.what());
    }
}

std::optional<Json> ask(const py::str& question, const py::str& text) {
    const std::string q = question.utf8();
    const std::string t = text.utf8();
    if (q == "demangle.parse") {
        const auto d = cb::symbols::demangle::parse(t);
        if (!d) return std::nullopt;
        return declaration_json(*d);
    }
    if (q == "demangle.type_name") {
        const auto n = cb::symbols::demangle::type_name(t);
        if (!n) return std::nullopt;
        return text_json(*n);
    }
    if (q == "prototypes.build") {
        cb::symbols::prototypes::HeaderFolders folders;
        for (const std::string_view line : cb::text::lines(t)) {
            const auto parts = cb::text::split(line, '\t');
            if (parts.size() != 2) continue;
            (parts[0] == "32" ? folders.w32 : folders.w16).push_back(cb::fs::from_text(parts[1]));
        }
        const auto headers = cb::symbols::prototypes::read_headers(folders);
        return from_json(cb::Json{{"32", prototypes_json(headers.w32)}, {"16", prototypes_json(headers.w16)}});
    }
    throw py::ValueError("cbsymbols.dll does not know " + q);
}

}  // namespace codebreaker::symbols::native
