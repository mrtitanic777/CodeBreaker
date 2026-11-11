// codebreaker/exact/hooks_lattice.hpp -- compat shim for cb/exact/hooks_lattice.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer.
#pragma once

#include <functional>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/hooks_lattice.hpp"
#include "py.hpp"

namespace codebreaker::exact::hooks_lattice {

// rd(segment, offset) -> the word read there (the profile hook's memory reader).
using Reader = std::function<int64_t(int64_t, int64_t)>;

inline int64_t cmp_test(const Reader& rd, int64_t bp, int64_t ss, int64_t ds) {
    using cb::compat::int_from_py;
    return cb::exact::hooks_lattice::cmp_test(
        [&](std::uint32_t seg, std::uint32_t off) { return int_from_py<std::uint16_t>(rd(seg, off)); }, int_from_py<std::uint32_t>(bp),
        int_from_py<std::uint32_t>(ss), int_from_py<std::uint32_t>(ds));
}

}  // namespace codebreaker::exact::hooks_lattice
