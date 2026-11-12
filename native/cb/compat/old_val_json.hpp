// cb/compat/old_val_json.hpp -- the old layer's JSON values (translate::ir::Val and Dict) and cb::Json, one into the other
// (docs/CPP_DESIGN.md 6.3). Only the shims of translate/l2, translate/desugar and translate/native_l2 include it.
//
// A Val is None, bool, int64, float, str, a list or a dict of those (it may also hold a Type, which has no JSON form: that
// is a TypeError, as json.dumps of a Type was). An integer a Val cannot hold (above 2**63-1) is an OverflowError going in.
#pragma once

#include <nlohmann/json.hpp>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/json_fwd.hpp"
#include "codebreaker/translate/ir.hpp"

namespace cb::compat {

inline Json to_json_value(const codebreaker::translate::ir::Val& v) {
    namespace ir = codebreaker::translate::ir;
    return std::visit(
        [](const auto& x) -> Json {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return Json(nullptr);
            else if constexpr (std::is_same_v<T, bool>) return Json(x);
            else if constexpr (std::is_same_v<T, std::int64_t>) return Json(x);
            else if constexpr (std::is_same_v<T, double>) return Json(x);
            else if constexpr (std::is_same_v<T, py::str>) return Json(x.utf8());
            else if constexpr (std::is_same_v<T, ir::TypeP>) throw py::TypeError("Object of type Type is not JSON serializable");
            else if constexpr (std::is_same_v<T, ir::Val::List>) {
                Json out = Json::array();
                for (const ir::Val& e : x) out.push_back(to_json_value(e));
                return out;
            } else {
                Json out = Json::object();
                for (const auto& kv : x.items()) out[std::get<0>(kv).utf8()] = to_json_value(std::get<1>(kv));
                return out;
            }
        },
        v.v);
}

inline Json to_json_value(const codebreaker::translate::ir::Dict& d) { return to_json_value(codebreaker::translate::ir::Val(d)); }

inline codebreaker::translate::ir::Val from_json_value(const Json& j) {
    namespace ir = codebreaker::translate::ir;
    if (j.is_null()) return ir::Val();
    if (j.is_boolean()) return ir::Val(j.get<bool>());
    if (j.is_number_integer()) {
        if (j.is_number_unsigned() && j.get<std::uint64_t>() > static_cast<std::uint64_t>(INT64_MAX))
            throw py::OverflowError("int too large for int64");
        return ir::Val(j.get<std::int64_t>());
    }
    if (j.is_number_float()) return ir::Val(j.get<double>());
    if (j.is_string()) return ir::Val(to_py(j.get<std::string>()));
    if (j.is_array()) {
        ir::Val::List out;
        for (const Json& e : j) out.append(from_json_value(e));
        return ir::Val(out);
    }
    ir::Dict out;
    for (const auto& [k, v] : j.items()) out.set(to_py(k), from_json_value(v));
    return ir::Val(out);
}

}  // namespace cb::compat
