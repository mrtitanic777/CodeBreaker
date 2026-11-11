// cb/compare/image.cpp -- see image.hpp.
#include "cb/compare/image.hpp"

#include <algorithm>
#include <cstring>

namespace cb::compare::image {

std::optional<std::size_t> first_difference(ByteView a, ByteView b) {
    const std::size_t n = std::min(a.size(), b.size());
    // Eight bytes at a time while they agree, then the byte that does not: images run to megabytes and agree to
    // the end more often than not.
    std::size_t i = 0;
    for (; i + 8 <= n; i += 8) {
        if (std::memcmp(a.data() + i, b.data() + i, 8) != 0) break;
    }
    for (; i < n; ++i) {
        if (a[i] != b[i]) return i;
    }
    if (a.size() == b.size()) return std::nullopt;
    return n;
}

}  // namespace cb::compare::image
