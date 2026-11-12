// codebreaker/translate/backends/c.hpp -- COMPAT SHIM over cb/translate/backends/c.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/backends/c.{hpp,cpp}: which of c_text's passes the L2 core library's C writer does on the
// tree, the decompiler's scalar sizes, and a parameter's name. The writer itself is the library (native/l2, cbl2.dll), asked
// through translate/native_l2: this header keeps the old names -- the entry points are native_l2's own, named here by
// using-declarations as c.py named them, and MOVED, IN_MODULE, FLAT, POWERPC and SIZEOF are the new module's lists as the old
// layer's containers. `_param_name` reads a parameter of an L2 document held as the old Dict (the typed module reads the typed
// node). No logic of its own beyond that.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/translate/backends/c.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/native_l2.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::c {

namespace ir = codebreaker::translate::ir;

namespace detail_shim {
inline py::list<py::str> pass_list(std::span<const std::string_view> passes) {
    py::list<py::str> out;
    for (const std::string_view p : passes) out.append(cb::compat::to_py(p));
    return out;
}
}  // namespace detail_shim

// c_text's passes done on L2 (docs/IR_UNIFICATION.md step 3, L2_BINARY_TYPES.md's order)
inline const py::list<py::str> MOVED = detail_shim::pass_list(cb::translate::backends::c::moved_passes());
// ... those of them that need what the module's facts say of the function
inline const py::list<py::str> IN_MODULE = detail_shim::pass_list(cb::translate::backends::c::module_passes());
// ... those of them the 32-bit path has (compilable32)
inline const py::list<py::str> FLAT = detail_shim::pass_list(cb::translate::backends::c::flat_passes());
// ... and those of the PowerPC path
inline const py::list<py::str> POWERPC = detail_shim::pass_list(cb::translate::backends::c::powerpc_passes());

// A node this back end does not write (the C back end is the core library's: its decline is the one native_l2 raises).
using Decline = native_l2::Decline;

// (bytes of the decompiler's scalar type names)
inline const py::dict<py::str, int64_t> SIZEOF = [] {
    py::dict<py::str, int64_t> out;
    for (const cb::translate::backends::c::ScalarSize& s : cb::translate::backends::c::scalar_sizes()) {
        out.set(cb::compat::to_py(s.name), static_cast<int64_t>(s.bytes));
    }
    return out;
}();

// (p.get('sym') or {}).get('name') or p.get('name', ''): the name as the document holds it (a str, or None)
inline ir::Val _param_name(const ir::Dict& p) {
    const auto truthy = [](const ir::Val& v) {
        return std::visit(
            [](const auto& x) -> bool {
                using T = std::decay_t<decltype(x)>;
                if constexpr (std::is_same_v<T, std::monostate>) return false;
                else if constexpr (std::is_same_v<T, bool>) return x;
                else if constexpr (std::is_same_v<T, int64_t>) return x != 0;
                else if constexpr (std::is_same_v<T, double>) return x != 0.0;
                else if constexpr (std::is_same_v<T, py::str>) return py::len(x) != 0;
                else if constexpr (std::is_same_v<T, ir::TypeP>) return true;
                else return x.size() != 0;   // a list or a dict
            },
            v.v);
    };
    ir::Val sym = p.get(py::str("sym"), ir::Val());
    if (truthy(sym)) {
        if (!std::holds_alternative<ir::Dict>(sym.v)) throw py::AttributeError("object has no attribute 'get'");
        ir::Val n = std::get<ir::Dict>(sym.v).get(py::str("name"), ir::Val());
        if (truthy(n)) return n;
    }
    return p.get(py::str("name"), ir::Val(""));
}

// ---------------------------------------------------------------------------------------------- the entry points
// (each the library's: the same names, arguments, text, facts and exceptions the Python writer had)
using native_l2::argument_words;
using native_l2::arguments_at;
using native_l2::caller_frame_plan;
using native_l2::condition;
using native_l2::expr;
using native_l2::flags;
using native_l2::fragment;
using native_l2::function;
using native_l2::module;
using native_l2::module_names32;
using native_l2::parameter_names;
using native_l2::parameters;
using native_l2::prototypes;
using native_l2::seg_layout;
using native_l2::segment_plan;
using native_l2::signature;
using native_l2::struct_accesses;
using native_l2::takes32;
using native_l2::type_names;
using native_l2::type_text;
using native_l2::words;

}  // namespace codebreaker::translate::backends::c
