// dotnet/decompiler/util/decimal.cpp -- System.Decimal's digits.
#include "dotnet/decompiler/util/decimal.hpp"

#include <algorithm>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::util {

Decimal::Decimal(std::uint32_t lo_, std::uint32_t mid_, std::uint32_t hi_, bool negative_, std::uint8_t scale_)
    : lo(lo_), mid(mid_), hi(hi_), negative(negative_), scale(scale_) {
    if (scale_ > 28) throw UsageError("Decimal's scale value must be between 0 and 28, inclusive.");
}

std::string Decimal::to_string() const {
    // the 96-bit integer's decimal digits, by repeated division by 10 (most significant word first)
    std::uint32_t words[3] = {hi, mid, lo};
    std::string digits;
    while (words[0] != 0 || words[1] != 0 || words[2] != 0) {
        std::uint64_t remainder = 0;
        for (std::uint32_t& w : words) {
            const std::uint64_t cur = (remainder << 32) | w;
            w = static_cast<std::uint32_t>(cur / 10);
            remainder = cur % 10;
        }
        digits.push_back(static_cast<char>('0' + remainder));
    }
    std::reverse(digits.begin(), digits.end());
    if (digits.size() <= scale) digits.insert(0, std::string(scale + 1 - digits.size(), '0'));
    if (scale > 0) digits.insert(digits.size() - scale, 1, '.');
    if (negative && !is_zero()) digits.insert(0, 1, '-');
    return digits;
}

}  // namespace cb::dotnet::decompiler::util
