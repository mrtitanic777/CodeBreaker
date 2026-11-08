// codebreaker/exact/hooks_lattice.hpp -- C++ port of codebreaker.exact.hooks_lattice.
#pragma once

#include <functional>

#include "py.hpp"

namespace codebreaker::exact::hooks_lattice {

// rd(segment, offset) -> the word read there (the profile hook's memory reader).
using Reader = std::function<int64_t(int64_t, int64_t)>;

int64_t cmp_test(const Reader& rd, int64_t bp, int64_t ss, int64_t ds);

}  // namespace codebreaker::exact::hooks_lattice
