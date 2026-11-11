// cb/translate/frontends/python/wide.hpp -- the integers of a type's interval.
//
// The Python front end proves that every int of a program fits int64_t, so the bounds of its integer intervals must be
// exact around 2**63 and well beyond it: a bound may reach 2**128, and the product of two bounds 2**256. A `Wide` is a
// sign and a 256-bit magnitude with *saturating* arithmetic: every result is exact while it fits, and one that would
// not fit becomes the largest magnitude, which is far beyond anything a bound is allowed to be (the types drop a bound
// past 2**128 as unbounded). So an overflow can only make an interval less precise, never wrong.
//
// A fixed-capacity integer, as the design asks (docs/CPP_DESIGN.md 3.4: no general big integer in CodeBreaker).
#pragma once

#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace cb::translate::frontends::python {

class Wide {
public:
    static constexpr unsigned kBits = 256;   // the magnitude's capacity

    Wide() = default;
    Wide(std::int64_t v);   // NOLINT: implicit, as a literal is
    static Wide from_u64(std::uint64_t v);
    // 2**n (n < 256; the largest magnitude beyond)
    static Wide pow2(unsigned n);
    // The decimal digits with an optional leading '-' (a number too large saturates); nullopt if it is not a number.
    static std::optional<Wide> from_decimal(std::string_view text);

    int sign() const { return zero() ? 0 : (negative_ ? -1 : 1); }
    // Bits in the magnitude (int.bit_length of the absolute value).
    unsigned bit_length() const;
    // The value, when it is within int64_t.
    std::optional<std::int64_t> to_i64() const;
    // The value, when it is within uint64_t.
    std::optional<std::uint64_t> to_u64() const;
    // Decimal.
    std::string to_string() const;
    std::size_t hash() const;

    Wide abs() const;
    Wide operator-() const;
    friend Wide operator+(const Wide& a, const Wide& b);
    friend Wide operator-(const Wide& a, const Wide& b);
    friend Wide operator*(const Wide& a, const Wide& b);
    // a // b, rounded toward negative infinity (b != 0).
    friend Wide floor_div(const Wide& a, const Wide& b);
    // a << n, a >> n (rounded toward negative infinity), a ** n.
    friend Wide shifted_left(const Wide& a, unsigned n);
    friend Wide shifted_right(const Wide& a, unsigned n);
    friend Wide raised_to(const Wide& a, unsigned n);

    friend bool operator==(const Wide& a, const Wide& b) { return a.negative_ == b.negative_ && a.mag_ == b.mag_; }
    friend std::strong_ordering operator<=>(const Wide& a, const Wide& b);

private:
    using Mag = std::array<std::uint64_t, 4>;   // little-endian limbs
    Wide(bool negative, const Mag& mag);        // normalises a zero
    bool zero() const { return (mag_[0] | mag_[1] | mag_[2] | mag_[3]) == 0; }
    static Wide saturated(bool negative);

    bool negative_ = false;
    Mag mag_{};
};

// (declared again here so that they can be named with the namespace)
Wide floor_div(const Wide& a, const Wide& b);
Wide shifted_left(const Wide& a, unsigned n);
Wide shifted_right(const Wide& a, unsigned n);
Wide raised_to(const Wide& a, unsigned n);

}  // namespace cb::translate::frontends::python
