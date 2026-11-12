// codebreaker/java/parse.hpp -- SHIM over cb/java/parse.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer.
//
// The parser's results are the old IR (shared, mutable objects the pipeline edits in place), made from the new
// parser's by ir::from_new; a parse failure reaches the old code as the ValueError the Python raised.
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/parse.hpp"
#include "codebreaker/java/ir.hpp"
#include "py.hpp"

namespace codebreaker::java::parse {

// 'int[][]' / 'List<ItemStack>' / 'net.minecraft.Block' -> ir.JType.
inline py::ref<ir::JType> parse_type(const py::str& s) {
    return cb::compat::guard([&] { return ir::from_new(cb::java::parse::parse_type(cb::compat::from_py(s))); });
}

inline py::ref<ir::JClass> parse_source(const py::str& src, const std::optional<py::str>& source_path = std::nullopt) {
    return cb::compat::guard([&] {
        std::optional<std::filesystem::path> path;
        if (source_path) path = cb::fs::from_text(cb::compat::from_py(*source_path));
        return ir::from_new(cb::java::parse::parse_source(cb::compat::from_py(src), path));
    });
}

inline py::ref<ir::JClass> parse_file(const py::str& path) {
    return cb::compat::guard([&] { return ir::from_new(cb::java::parse::parse_file(cb::fs::from_text(cb::compat::from_py(path)))); });
}

}  // namespace codebreaker::java::parse
