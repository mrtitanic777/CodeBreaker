// codebreaker/decompile/l2tree.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The L2 document passes (asm_blocks, frame16,
// shape16) now work on a cb::Json (native/cb/decompile/l2tree.{hpp,cpp}); what stays here is the conversion between the
// old layer's document (translate::ir::Val, a Dict of Vals) and that Json, for the shims of those three modules and
// decompile/decompiler. Conversion only; deleted with the last of them.
#pragma once

#include <cmath>
#include <cstdint>

#include <nlohmann/json.hpp>

#include "cb/compat/py_bridge.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::decompile::l2tree {

using translate::ir::Dict;
using translate::ir::Val;

// An old-layer value as the Json the new passes read (a Type becomes its L2 dict).
inline ::cb::Json to_json(const Val& v) {
    if (std::holds_alternative<std::monostate>(v.v)) return ::cb::Json(nullptr);
    if (const bool* b = std::get_if<bool>(&v.v)) return ::cb::Json(*b);
    if (const int64_t* i = std::get_if<int64_t>(&v.v)) return ::cb::Json(*i);
    if (const double* d = std::get_if<double>(&v.v)) return ::cb::Json(*d);
    if (const py::str* s = std::get_if<py::str>(&v.v)) return ::cb::Json(s->utf8());
    if (const translate::ir::TypeP* t = std::get_if<translate::ir::TypeP>(&v.v)) return to_json(translate::l2::type_json(*t));
    if (const Val::List* l = std::get_if<Val::List>(&v.v)) {
        ::cb::Json out = ::cb::Json::array();
        for (const Val& x : l->vec()) out.push_back(to_json(x));
        return out;
    }
    ::cb::Json out = ::cb::Json::object();
    for (const auto& kv : std::get<Dict>(v.v).items()) out[std::get<0>(kv).utf8()] = to_json(std::get<1>(kv));
    return out;
}

// What the new passes made, as an old-layer value. A whole number beyond int64 is refused (the old layer has no bigger).
inline Val from_json(const ::cb::Json& j) {
    if (j.is_null()) return Val();
    if (j.is_boolean()) return Val(j.get<bool>());
    if (j.is_number_unsigned()) {
        const auto u = j.get<std::uint64_t>();
        if (u > static_cast<std::uint64_t>(INT64_MAX)) throw py::OverflowError("Python int too large to convert to C long");
        return Val(static_cast<int64_t>(u));
    }
    if (j.is_number_integer()) return Val(j.get<std::int64_t>());
    if (j.is_number_float()) return Val(j.get<double>());
    if (j.is_string()) return Val(py::str(j.get<std::string>()));
    if (j.is_array()) {
        Val::List out;
        for (const ::cb::Json& x : j) out.append(from_json(x));
        return Val(out);
    }
    Dict out;
    for (const auto& entry : j.items()) out.set(py::str(entry.key()), from_json(entry.value()));
    return Val(out);
}

inline ::cb::Json to_json(const Dict& d) { return to_json(Val(d)); }

}  // namespace codebreaker::decompile::l2tree
