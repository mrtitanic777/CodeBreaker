// engine/pcode_vector_model.hpp -- test support for pcode_vector_test.cpp and pcode_vector_float_test.cpp: a small signed big integer, enough exact arithmetic to model
// what a lane operation computes before it is cut to the lane. Written without any use of the library's own arithmetic.
#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

namespace cb::vecmodel {

using U64 = std::uint64_t;
using I64 = std::int64_t;

// A signed big integer: sign and magnitude in 32 bit limbs.
struct Big {
    bool neg = false;
    std::vector<std::uint32_t> m;   // least significant first, no leading zero limbs

    void trim() {
        while (!m.empty() && m.back() == 0) m.pop_back();
        if (m.empty()) neg = false;
    }
    bool zero() const { return m.empty(); }
};
inline Big bigU(U64 v) {
    Big b;
    if (v) b.m.push_back(static_cast<std::uint32_t>(v));
    if (v >> 32) b.m.push_back(static_cast<std::uint32_t>(v >> 32));
    return b;
}
inline Big bigS(I64 v) {
    Big b = bigU(v < 0 ? ~static_cast<U64>(v) + 1 : static_cast<U64>(v));
    b.neg = v < 0 && !b.zero();
    return b;
}
inline int cmpMag(const Big& a, const Big& b) {
    if (a.m.size() != b.m.size()) return a.m.size() < b.m.size() ? -1 : 1;
    for (std::size_t i = a.m.size(); i-- > 0;)
        if (a.m[i] != b.m[i]) return a.m[i] < b.m[i] ? -1 : 1;
    return 0;
}
inline int cmp(const Big& a, const Big& b) {
    if (a.neg != b.neg) return a.neg ? -1 : 1;
    const int c = cmpMag(a, b);
    return a.neg ? -c : c;
}
inline Big addMag(const Big& a, const Big& b) {
    Big r;
    U64 carry = 0;
    for (std::size_t i = 0; i < std::max(a.m.size(), b.m.size()) || carry; ++i) {
        const U64 s = (i < a.m.size() ? a.m[i] : 0) + static_cast<U64>(i < b.m.size() ? b.m[i] : 0) + carry;
        r.m.push_back(static_cast<std::uint32_t>(s));
        carry = s >> 32;
    }
    r.trim();
    return r;
}
inline Big subMag(const Big& a, const Big& b) {   // |a| >= |b|
    Big r;
    I64 borrow = 0;
    for (std::size_t i = 0; i < a.m.size(); ++i) {
        I64 d = static_cast<I64>(a.m[i]) - (i < b.m.size() ? b.m[i] : 0) - borrow;
        borrow = d < 0 ? 1 : 0;
        if (d < 0) d += (I64{1} << 32);
        r.m.push_back(static_cast<std::uint32_t>(d));
    }
    r.trim();
    return r;
}
inline Big operator-(Big a) {
    if (!a.zero()) a.neg = !a.neg;
    return a;
}
inline Big operator+(const Big& a, const Big& b) {
    if (a.neg == b.neg) {
        Big r = addMag(a, b);
        r.neg = a.neg && !r.zero();
        return r;
    }
    const int c = cmpMag(a, b);
    if (c == 0) return Big{};
    Big r = c > 0 ? subMag(a, b) : subMag(b, a);
    r.neg = (c > 0 ? a.neg : b.neg) && !r.zero();
    return r;
}
inline Big operator-(const Big& a, const Big& b) { return a + (-b); }
inline Big operator*(const Big& a, const Big& b) {
    Big r;
    r.m.assign(a.m.size() + b.m.size(), 0);
    for (std::size_t i = 0; i < a.m.size(); ++i) {
        U64 carry = 0;
        for (std::size_t j = 0; j < b.m.size() || carry; ++j) {
            const U64 cur = r.m[i + j] + carry + (j < b.m.size() ? static_cast<U64>(a.m[i]) * b.m[j] : 0);
            r.m[i + j] = static_cast<std::uint32_t>(cur);
            carry = cur >> 32;
        }
    }
    r.neg = a.neg != b.neg;
    r.trim();
    return r;
}
inline Big shlBig(const Big& a, unsigned k) {
    if (a.zero()) return a;
    Big r;
    r.neg = a.neg;
    r.m.assign(k / 32, 0);
    const unsigned s = k % 32;
    std::uint32_t carry = 0;
    for (std::uint32_t limb : a.m) {
        r.m.push_back(static_cast<std::uint32_t>((static_cast<U64>(limb) << s) | carry));
        carry = s ? static_cast<std::uint32_t>(static_cast<U64>(limb) >> (32 - s)) : 0;
    }
    if (carry) r.m.push_back(carry);
    r.trim();
    return r;
}
inline Big shrMag(const Big& a, unsigned k) {   // truncating, magnitude
    Big r;
    const std::size_t ws = k / 32, bs = k % 32;
    for (std::size_t i = ws; i < a.m.size(); ++i) {
        U64 v = a.m[i] >> bs;
        if (bs && i + 1 < a.m.size()) v |= static_cast<U64>(a.m[i + 1]) << (32 - bs);
        r.m.push_back(static_cast<std::uint32_t>(v));
    }
    r.trim();
    return r;
}
inline unsigned bitLength(const Big& a) {
    if (a.zero()) return 0;
    unsigned n = static_cast<unsigned>(a.m.size() - 1) * 32;
    for (std::uint32_t t = a.m.back(); t; t >>= 1) ++n;
    return n;
}
inline bool bitSet(const Big& a, unsigned k) { return k / 32 < a.m.size() && ((a.m[k / 32] >> (k % 32)) & 1); }
inline Big shrFloor(const Big& a, unsigned k) {   // floor(a / 2^k) for either sign
    Big q = shrMag(a, k);
    if (a.neg) {
        bool lost = false;
        for (unsigned i = 0; i < k && !lost; ++i) lost = bitSet(a, i);
        q.neg = true;
        if (lost) q = q - bigU(1);
        q.trim();
        if (q.zero()) q.neg = false;
    }
    return q;
}
// non negative a / b: quotient and remainder, bit by bit
inline void divMod(const Big& a, const Big& b, Big& q, Big& r) {
    q = Big{};
    r = Big{};
    for (unsigned i = bitLength(a); i-- > 0;) {
        r = shlBig(r, 1);
        if (bitSet(a, i)) r = r + bigU(1);
        q = shlBig(q, 1);
        if (cmpMag(r, b) >= 0) {
            r = r - b;
            q = q + bigU(1);
        }
    }
}
inline Big isqrt(const Big& n) {
    Big r;
    for (unsigned i = bitLength(n) / 2 + 1; i-- > 0;) {
        const Big c = r + shlBig(bigU(1), i);
        if (cmp(c * c, n) <= 0) r = c;
    }
    return r;
}
inline U64 lowBits(const Big& v, unsigned w) {   // two's complement, the low w bits
    U64 low = (v.m.size() > 0 ? v.m[0] : 0) | (static_cast<U64>(v.m.size() > 1 ? v.m[1] : 0) << 32);
    if (v.neg) low = ~low + 1;
    return w >= 64 ? low : low & ((U64{1} << w) - 1);
}
inline Big laneBig(U64 raw, unsigned ew, bool isSigned) {
    const unsigned w = 8 * ew;
    if (isSigned && ((raw >> (w - 1)) & 1)) return bigS(static_cast<I64>(w == 64 ? raw : raw | ~((U64{1} << w) - 1)));
    return bigU(raw);
}
inline Big hiBound(unsigned w, bool isSigned) { return isSigned ? shlBig(bigU(1), w - 1) - bigU(1) : shlBig(bigU(1), w) - bigU(1); }
inline Big loBound(unsigned w, bool isSigned) { return isSigned ? -shlBig(bigU(1), w - 1) : Big{}; }
inline Big clamp(const Big& v, unsigned w, bool isSigned) {
    if (cmp(v, hiBound(w, isSigned)) > 0) return hiBound(w, isSigned);
    if (cmp(v, loBound(w, isSigned)) < 0) return loBound(w, isSigned);
    return v;
}


}  // namespace cb::vecmodel
