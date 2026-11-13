// codebreaker/formats/xbox/package.hpp -- compat shim over cb/formats/xbox/summary.hpp (docs/CPP_DESIGN.md 6.3), the old formats/xbox/__init__.py.
//
// The module itself is native/cb/formats/xbox/summary.{hpp,cpp}; this header keeps the old py:: surface for the ports that have not been
// refactored yet and is deleted with the last of them. Conversion and forwarding only. What changed for old callers: the readers of the
// container volumes, XBE, XDBF and XPR are native/cb's now, so the hooks the old port needed for them (`Volume`, `XbeInfo`, `XdbfInfo`,
// `Hooks`, `set_hooks`) are not needed and `set_hooks` does nothing; every value of a summary is text (a count is its digits), as `info` shows it.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/xbox/summary.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox {

struct NonScalar {};   // a bytes / bytearray / list / dict value (never produced now)
using Field = std::variant<std::monostate, bool, int64_t, double, py::str, NonScalar>;
using Summary = py::dict<py::str, Field>;

// (the old readers' hooks: not needed, see the header)
struct Hooks {};
inline void set_hooks(const Hooks&) {}

namespace shim_ {

// The facts of `format` for the file at `path`: the new summary asked as `info` asks it, with the format line keyed as the old table keyed it.
inline Summary facts_(const py::str& path, const std::string& format, const std::string& command = "info") {
    cb::formats::detect::Detected match;
    match.format = format;
    match.command = command;
    Summary out;
    for (const auto& [key, value] : cb::compat::guard([&] { return cb::formats::xbox::summary(cb::fs::from_text(cb::compat::from_py(path)), match); })) {
        out.set(cb::compat::to_py(key), Field(cb::compat::to_py(value)));
    }
    return out;
}

}  // namespace shim_

inline Summary _container(const py::str& path) { return shim_::facts_(path, "", "media"); }
inline Summary _xbe(const py::str& path) { return shim_::facts_(path, "XBE"); }
inline Summary _xdbf(const py::str& path) { return shim_::facts_(path, "XDBF"); }
inline Summary _xpr(const py::str& path) { return shim_::facts_(path, "resource bundle"); }
inline Summary _xwb(const py::str& path) { return shim_::facts_(path, "wave bank"); }
inline Summary _xur(const py::str& path) { return shim_::facts_(path, "XUI scene"); }
inline Summary _bink(const py::str& path) { return shim_::facts_(path, "Bink"); }
inline Summary _chd(const py::str& path) { return shim_::facts_(path, "CHD"); }

// (key, summarising function): the formats `summary` knows, in the order it asks
inline const py::list<std::tuple<py::str, std::function<Summary(const py::str&)>>>& _BY_FORMAT() {
    static const py::list<std::tuple<py::str, std::function<Summary(const py::str&)>>> table = {
        {py::str("XBE"), _xbe},       {py::str("XDBF"), _xdbf}, {py::str("resource bundle"), _xpr}, {py::str("wave bank"), _xwb},
        {py::str("XUI scene"), _xur}, {py::str("Bink"), _bink}, {py::str("CHD"), _chd}};
    return table;
}

// What `codebreaker info` shows for an Xbox file: {field: value}, or {} when there is nothing to add to the format line. `match` is the first of
// detect.identify(path).
inline Summary summary(const py::str& path, const py::dict<py::str, py::str>& match) {
    cb::formats::detect::Detected m;
    for (const auto& kv : match.items()) {
        const std::string key = cb::compat::from_py(std::get<0>(kv)), value = cb::compat::from_py(std::get<1>(kv));
        if (key == "format") m.format = value;
        else if (key == "detail") m.detail = value;
        else if (key == "family") m.family = value;
        else if (key == "command") m.command = value;
    }
    Summary out;
    for (const auto& [key, value] : cb::compat::guard([&] { return cb::formats::xbox::summary(cb::fs::from_text(cb::compat::from_py(path)), m); })) {
        out.set(cb::compat::to_py(key), Field(cb::compat::to_py(value)));
    }
    return out;
}

}  // namespace codebreaker::formats::xbox
