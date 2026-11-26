// dotnet/decompiler/util/bit_set.cpp -- Interval (ICSharpCode.Decompiler.Util.Interval).
#include "dotnet/decompiler/util/bit_set.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::util {

Interval::Interval(int start_, int end_) : start(start_), end(end_) {
    if (!(start_ <= static_cast<int>(static_cast<std::uint32_t>(end_) - 1) || start_ == end_)) throw UsageError("Interval: the end must be after the start");
}

Interval Interval::intersect(Interval other) const {
    const int s = std::max(start, other.start);
    const int e = std::min(inclusive_end(), other.inclusive_end());
    if (s <= e) return Interval(s, static_cast<int>(static_cast<std::uint32_t>(e) + 1));
    return Interval();
}

std::string Interval::to_string() const {
    if (end == INT32_MIN) return std::format("[{}..int.MaxValue]", start);
    return std::format("[{}..{})", start, end);
}

}  // namespace cb::dotnet::decompiler::util
