// cb/exact/bytesearch.hpp -- looking for a run of bytes in a program's image, which the exact lanes do for every
// module they place.
#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>

#include "cb/core/bytes.hpp"

namespace cb::exact {

// Where `needle` first stands in hay[from, to); hay.size() when it does not (so a caller tests `== hay.size()`).
inline std::size_t find_bytes(ByteView hay, ByteView needle, std::size_t from = 0, std::size_t to = static_cast<std::size_t>(-1)) {
    to = std::min(to, hay.size());
    if (needle.empty() || from > to || to - from < needle.size()) return hay.size();
    const auto first = hay.begin() + static_cast<std::ptrdiff_t>(from);
    const auto last = hay.begin() + static_cast<std::ptrdiff_t>(to);
    const auto it = std::search(first, last, std::boyer_moore_horspool_searcher(needle.begin(), needle.end()));
    return it == last ? hay.size() : static_cast<std::size_t>(it - hay.begin());
}

}  // namespace cb::exact
