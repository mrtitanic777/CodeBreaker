// codebreaker/decompile/shape16.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/shape16.{hpp,cpp}; this header keeps the old py:: surface for decompile/decompiler (c_from_l2).
// Conversion and forwarding only; deleted with its last py:: consumer.
//
//     auto [doc, said] = shape16::reshape(doc, code);
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/shape16.hpp"
#include "codebreaker/decompile/l2tree.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::decompile::shape16 {

// The tree reshaped (a deep copy; the caller's is not changed), and what was done.
inline std::tuple<translate::ir::Dict, py::list<py::str>> reshape(const translate::ir::Dict& doc, const py::bytes& code) {
    return ::cb::compat::guard([&] {
        ::cb::Json tree = l2tree::to_json(doc);
        const ::cb::Bytes bytes = ::cb::compat::from_py(code);
        const std::vector<std::string> said = ::cb::decompile::shape16::reshape(tree, bytes);
        return std::make_tuple(std::get<translate::ir::Dict>(l2tree::from_json(tree).v), ::cb::compat::to_py(said));
    });
}

}  // namespace codebreaker::decompile::shape16
