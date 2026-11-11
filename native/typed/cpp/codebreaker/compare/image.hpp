// codebreaker/compare/image.hpp -- SHIM over cb/compare/image.hpp (docs/CPP_DESIGN.md 6.3): conversion and
// forwarding only; deleted with its last py:: consumer.
#pragma once

#include "cb/compare/image.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::compare::image {

inline std::optional<int64_t> first_difference(const py::bytes& a, const py::bytes& b) {
    return cb::compat::to_py(cb::compare::image::first_difference(cb::compat::from_py(a), cb::compat::from_py(b)));
}

}  // namespace codebreaker::compare::image
