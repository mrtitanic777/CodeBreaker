// codebreaker/compare/image.hpp -- C++ from codebreaker.compare.image by CodeBreaker's Python front end.
#pragma once

#include "py.hpp"

namespace codebreaker::compare::image {

std::optional<int64_t> first_difference(const py::bytes& a, const py::bytes& b);

}  // namespace codebreaker::compare::image
