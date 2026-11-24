// core/intmath.hpp -- the few integer operations the standard library does not give by name.
//
// floor_div / floor_mod round toward negative infinity (C++ `/` and `%` truncate toward zero); they matter only
// for signed values that can be negative, such as a displacement turned into a table index. checked_add /
// checked_mul return nullopt on overflow and belong at trust boundaries (sizes read from a file); elsewhere an
// overflow of a 64-bit domain value is a bug for the sanitizer build to find.
#pragma once

#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

#include "core/error.hpp"

namespace cb {

// Quotient rounded toward negative infinity. `b` must not be zero.
template <std::signed_integral T>
constexpr T floor_div(T a, T b) {
    T q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}

// Remainder with the sign of the divisor, so that a == floor_div(a, b) * b + floor_mod(a, b). `b` must not be zero.
template <std::signed_integral T>
constexpr T floor_mod(T a, T b) {
    T r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) r = static_cast<T>(r + b);
    return r;
}

template <std::integral T>
constexpr std::optional<T> checked_add(T a, T b) {
    if constexpr (std::is_signed_v<T>) {
        if (b > 0 ? a > std::numeric_limits<T>::max() - b : a < std::numeric_limits<T>::min() - b) return std::nullopt;
        return static_cast<T>(a + b);
    } else {
        if (a > std::numeric_limits<T>::max() - b) return std::nullopt;
        return static_cast<T>(a + b);
    }
}

// (a divisor that is never zero where it is used: the zero operands have returned before any division, but a compiler
// that sees a literal zero argument and does not remove the branch warns "potential divide by 0" (MSVC C4723, an error
// at /WX); this keeps the divisor from being that constant, and divides by the operand itself otherwise)
template <std::integral T>
constexpr T nonzero(T v) { return v == 0 ? static_cast<T>(1) : v; }

template <std::integral T>
constexpr std::optional<T> checked_mul(T a, T b) {
    if (a == 0 || b == 0) return static_cast<T>(0);
    if constexpr (std::is_signed_v<T>) {
        constexpr T lo = std::numeric_limits<T>::min();
        constexpr T hi = std::numeric_limits<T>::max();
        if (a == -1) return b == lo ? std::nullopt : std::optional<T>(static_cast<T>(-b));
        if (b == -1) return a == lo ? std::nullopt : std::optional<T>(static_cast<T>(-a));
        if (a > 0 ? (b > 0 ? a > hi / nonzero(b) : b < lo / nonzero(a)) : (b > 0 ? a < lo / nonzero(b) : a < hi / nonzero(b))) return std::nullopt;
        return static_cast<T>(a * b);
    } else {
        if (a > std::numeric_limits<T>::max() / nonzero(b)) return std::nullopt;
        return static_cast<T>(a * b);
    }
}

}  // namespace cb
