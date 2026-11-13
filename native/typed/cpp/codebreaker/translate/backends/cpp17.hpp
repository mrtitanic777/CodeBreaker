// codebreaker/translate/backends/cpp17.hpp -- COMPAT SHIM over cb/translate/backends/cpp17.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// The module is native/cb/translate/backends/cpp17.{hpp,cpp}: the client of the C++17 back end (the L2 core library, native/l2), over the typed
// IR tree. The old callers hold trees of the old kind, so each call hands the new module their L2 documents as typed nodes (in an arena of its
// own) and the library the old L2 shim keeps; the library's decline is the old Cpp17Decline.
//
//     write_unit(unit)       a Python module -> (header, source, {function: (first, last) source line})
//     program(units, main)   a whole Java program from L2 -> its C++ text
//
// RUNTIME and API_MAPS are folders found from the checkout, so they are functions (a variable would look for the checkout when the program
// starts).
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/translate/backends/cpp17.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/translate/native_l2.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::cpp17 {

namespace ir = codebreaker::translate::ir;
namespace nc = cb::translate::backends::cpp17;

// The C++17 back end does not write this unit (the reason says why).
using Decline = translate::native_l2::Cpp17Decline;   // (the library raises it for the C++17 back end)

inline py::list<py::str> flags_() {
    py::list<py::str> out;
    for (const std::string_view f : nc::kFlags) out.append(py::str(std::string(f)));
    return out;
}
inline const py::list<py::str> FLAGS = flags_();

// codebreaker/translate/runtime/cpp17
inline py::str RUNTIME() { return cb::compat::to_py(cb::fs::to_text(nc::runtime_dir())); }
// codebreaker/java/api_maps
inline py::str API_MAPS() { return cb::compat::to_py(cb::fs::to_text(nc::api_maps_dir())); }

inline py::str header_path(const py::str& module) { return cb::compat::to_py(nc::header_path(module.utf8())); }
inline py::str source_path(const py::str& module) { return cb::compat::to_py(nc::source_path(module.utf8())); }

// An ir.Unit of the Python front end -> (header text, source text, {function: (first, last) source line})
// -- the lines say where each function's body is in the source, so a compiler's error can be pinned on it.
inline std::tuple<py::str, py::str, py::dict<py::str, std::tuple<int64_t, int64_t>>> write_unit(
    const ir::NodeP& unit, const std::optional<py::str>& = std::nullopt) {
    cb::Arena arena;
    cb::translate::ir::Node* typed = cb::translate::l2::from_json(arena, cb::compat::to_json_value(l2::to_json(unit)));
    try {
        const nc::UnitText m = nc::write_unit(l2::shim_detail::library(), *typed);
        py::dict<py::str, std::tuple<int64_t, int64_t>> spans;
        for (const auto& [name, span] : m.spans) spans.set(cb::compat::to_py(name), std::make_tuple(span.first, span.second));
        return {cb::compat::to_py(m.header), cb::compat::to_py(m.source), spans};
    } catch (const cb::translate::native_l2::Cpp17Declined& e) {
        throw Decline(std::string(e.what()));
    }
}

// The C++17 text of a whole Java program (its L2 units), or raise Decline.
inline py::str program(const py::list<ir::NodeP>& units, const std::optional<py::str>& main = std::nullopt) {
    cb::Arena arena;
    std::vector<cb::translate::ir::Node*> typed;
    for (const ir::NodeP& u : units) typed.push_back(cb::translate::l2::from_json(arena, cb::compat::to_json_value(l2::to_json(u))));
    try {
        return cb::compat::to_py(nc::program(l2::shim_detail::library(), arena, typed, main ? std::optional<std::string>(main->utf8()) : std::nullopt));
    } catch (const cb::translate::native_l2::Cpp17Declined& e) {
        throw Decline(std::string(e.what()));
    }
}

}  // namespace codebreaker::translate::backends::cpp17
