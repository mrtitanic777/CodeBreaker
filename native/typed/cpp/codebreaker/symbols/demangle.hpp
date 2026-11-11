// codebreaker/symbols/demangle.hpp -- compat shim for cb/symbols/demangle.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer.
//
//     declaration("?AddRef@CFoo@@UAGKXZ")   "public: virtual unsigned long __stdcall CFoo::AddRef(void)"
//     short_("?AddRef@CFoo@@UAGKXZ")        "CFoo::AddRef"
//     parse("?AddRef@CFoo@@UAGKXZ")         a Parsed (name, kind, access, static, virtual, far, convention,
//                                           returns, parameters, const)
//     plain("_LZRead@12")                   ("LZRead", "__stdcall", 12)      (also "LZRead@12")
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/demangle.hpp"
#include "py.hpp"

namespace codebreaker::symbols::demangle {

// What a decorated C++ name says (the Python dict). A data name carries only name/kind/text.
struct Parsed {
    py::str name;
    py::str base;
    py::str class_;                  // key "class"
    py::str kind;                    // "function", "data" or "name"
    py::str access;
    bool static_ = false;            // key "static"
    bool virtual_ = false;           // key "virtual"
    bool far_ = false;               // key "far"
    py::str convention;
    py::str returns;
    py::list<py::str> parameters;
    py::str const_;                  // key "const"
    py::str text;
};

namespace detail {
inline Parsed to_old(const cb::symbols::demangle::Declaration& d) {
    using cb::compat::to_py;
    namespace nd = cb::symbols::demangle;
    Parsed p;
    p.name = to_py(d.name);
    p.base = to_py(d.base);
    p.class_ = to_py(d.owner);
    p.kind = py::str(d.kind == nd::Kind::Function ? "function" : d.kind == nd::Kind::Data ? "data" : "name");
    p.access = to_py(d.access);
    p.static_ = d.is_static;
    p.virtual_ = d.is_virtual;
    p.far_ = d.far;
    p.convention = to_py(d.convention);
    p.returns = to_py(d.returns);
    p.parameters = to_py(d.parameters);
    p.const_ = to_py(d.const_qualifier);
    p.text = to_py(d.text);
    return p;
}
}  // namespace detail

inline std::optional<py::str> type_name(const py::str& decorated) {
    return cb::compat::to_py(cb::symbols::demangle::type_name(cb::compat::from_py(decorated)));
}
inline std::optional<Parsed> parse(const py::str& name) {
    const auto d = cb::symbols::demangle::parse(cb::compat::from_py(name));
    if (!d) return std::nullopt;
    return detail::to_old(*d);
}
inline std::optional<py::str> declaration(const py::str& name) {
    return cb::compat::to_py(cb::symbols::demangle::declaration(cb::compat::from_py(name)));
}
inline std::optional<py::str> short_(const py::str& name) {   // Python `short`
    return cb::compat::to_py(cb::symbols::demangle::short_name(cb::compat::from_py(name)));
}
inline std::tuple<py::str, py::str, std::optional<int64_t>> plain(const py::str& name) {
    const auto p = cb::symbols::demangle::plain(cb::compat::from_py(name));
    return {cb::compat::to_py(p.name), cb::compat::to_py(p.convention), cb::compat::to_py(p.argument_bytes)};
}
inline py::str readable(const py::str& name) { return cb::compat::to_py(cb::symbols::demangle::readable(cb::compat::from_py(name))); }
inline std::optional<int64_t> stack_bytes(const py::str& name) {
    return cb::compat::to_py(cb::symbols::demangle::stack_bytes(cb::compat::from_py(name)));
}
inline int64_t main(const py::list<py::str>& argv) {
    return cb::symbols::demangle::run(cb::compat::from_py(argv));
}

}  // namespace codebreaker::symbols::demangle
