// codebreaker/decompile/frame16.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/decompile/frame16.{hpp,cpp}; this header keeps the old py:: surface for decompile/decompiler (c_from_l2).
// Conversion and forwarding only; deleted with its last py:: consumer.
//
//     auto [doc, said] = frame16::lay_out(doc, code, start, end);
//
// See native/cb/decompile/frame16.hpp for what the pass does.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/decompile/frame16.hpp"
#include "cb/decompile/l2tree.hpp"
#include "codebreaker/decompile/l2tree.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::decompile::frame16 {

// The tree laid out (a deep copy; the caller's is not changed), and what was done.
inline std::tuple<translate::ir::Dict, py::list<py::str>> lay_out(const translate::ir::Dict& doc, const py::bytes& code, int64_t start,
                                                                   int64_t end) {
    return ::cb::compat::guard([&] {
        ::cb::Json tree = l2tree::to_json(doc);
        const ::cb::Bytes bytes = ::cb::compat::from_py(code);
        const std::vector<std::string> said = ::cb::decompile::frame16::lay_out(
            tree, bytes, ::cb::compat::int_from_py<std::uint32_t>(start), ::cb::compat::int_from_py<std::uint32_t>(end));
        return std::make_tuple(std::get<translate::ir::Dict>(l2tree::from_json(tree).v), ::cb::compat::to_py(said));
    });
}

// Every dict node of a tree, depth first, a node before its members (the copies of them).
inline py::list<translate::ir::Dict> _walk(const translate::ir::Val& n) {
    py::list<translate::ir::Dict> out;
    const ::cb::Json tree = l2tree::to_json(n);
    for (const ::cb::Json* node : ::cb::decompile::l2tree::nodes(tree)) out.append(std::get<translate::ir::Dict>(l2tree::from_json(*node).v));
    return out;
}

}  // namespace codebreaker::decompile::frame16
