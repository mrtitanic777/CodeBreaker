// cb/compare/image.hpp -- where a rebuilt image first departs from the original.
#pragma once

#include <cstddef>
#include <optional>

#include "cb/core/bytes.hpp"

namespace cb::compare::image {

// The offset of the first byte where `a` and `b` differ; where one ends first, its length. Absent when they are the
// same.
std::optional<std::size_t> first_difference(ByteView a, ByteView b);

}  // namespace cb::compare::image
