// big.hpp -- Python's unbounded int, for the bounds of integer intervals.
//
// The inference proves that every int fits int64_t, so its bounds must be exact around 2**63 and
// beyond it (pytypes.py keeps bounds up to 2**128 and products of them).  Almost every value is
// small: Big keeps an int64 inline and moves to a sign-magnitude vector of 32-bit limbs only when
// a result leaves int64.  Division and modulo floor (Python's), >> floors, << and ** grow.
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace pyengine {

class Big {
public:
    Big() = default;
    Big(int64_t v) : v_(v) {}                  // NOLINT: implicit, as Python's literals
    Big(int v) : v_(v) {}
    static Big from_decimal(const std::string& digits);   // optional leading '-'
    static Big pow2(int n);                    // 2**n

    bool small() const { return small_; }
    bool fits_i64() const { return small_; }
    int64_t i64() const { return v_; }         // (only when small())
    int sign() const;
    int bit_length() const;                    // of abs(), as int.bit_length
    std::string str() const;                   // decimal, as str(int)

    friend Big operator+(const Big& a, const Big& b);
    friend Big operator-(const Big& a, const Big& b);
    friend Big operator*(const Big& a, const Big& b);
    Big operator-() const;
    friend Big floordiv(const Big& a, const Big& b);     // a // b (b != 0)
    friend Big floormod(const Big& a, const Big& b);     // a % b, the sign of b
    friend Big shl(const Big& a, int n);                 // a << n (n >= 0)
    friend Big shr(const Big& a, int n);                 // a >> n (n >= 0), floor
    friend Big ipow(const Big& a, int n);                // a ** n (n >= 0)
    Big abs() const { return sign() < 0 ? -*this : *this; }

    friend int cmp(const Big& a, const Big& b);
    friend bool operator==(const Big& a, const Big& b) { return cmp(a, b) == 0; }
    friend bool operator!=(const Big& a, const Big& b) { return cmp(a, b) != 0; }
    friend bool operator<(const Big& a, const Big& b) { return cmp(a, b) < 0; }
    friend bool operator<=(const Big& a, const Big& b) { return cmp(a, b) <= 0; }
    friend bool operator>(const Big& a, const Big& b) { return cmp(a, b) > 0; }
    friend bool operator>=(const Big& a, const Big& b) { return cmp(a, b) >= 0; }
    size_t hash() const;

    // magnitude access (big.cc's helpers)
    static Big from_mag(bool neg, std::vector<uint32_t> mag);
    void to_mag(bool& neg, std::vector<uint32_t>& mag) const;
    static void divmod_mag(const Big& a, const Big& b, Big& q, Big& r);   // a, b >= 0, truncated

private:
    bool small_ = true;
    int64_t v_ = 0;                  // small
    bool neg_ = false;               // big: sign
    std::vector<uint32_t> mag_;      // big: magnitude, little-endian, no leading zero limbs
};

}  // namespace pyengine
