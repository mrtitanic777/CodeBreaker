// decompile/engine/pcode_wide.hh -- the p-code integer operations on values of any width (the emulator's path for a varnode wider than 8 bytes: vector registers,
// the 16-byte temporaries of a widening multiply or divide). A value is `n` bytes, LEAST significant byte first, whatever the byte order of the space it lives in; every
// operation computes from byte arrays into a separate result (the output never aliases an input), with no compiler extension (no __int128), so it builds with MSVC,
// clang-cl and MinGW GCC alike. The meaning of each operation is the core's OpBehavior's (decompile/engine/core/cpp/opbehavior.cc) taken to any size; at sizes up to 8
// bytes the test compares them to the behaviour table, and above that to a bit-serial computation written separately.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "decompile/engine/core/cpp/opcodes.hh"

namespace cb::decompile::sleigh::wide {

constexpr std::size_t kMax = 256;   // bytes in the widest value the emulator takes (a 2048-bit vector register)
using Byte = std::uint8_t;

// ---- plumbing
inline void zero(Byte* r, std::size_t n) { std::memset(r, 0, n); }
// r (n bytes) = a (m bytes) zero extended or cut.
inline void resize(Byte* r, std::size_t n, const Byte* a, std::size_t m) {
    const std::size_t k = n < m ? n : m;
    for (std::size_t i = 0; i < k; ++i) r[i] = a[i];
    for (std::size_t i = k; i < n; ++i) r[i] = 0;
}
inline bool signBit(const Byte* a, std::size_t m) { return m != 0 && (a[m - 1] & 0x80) != 0; }
// r (n bytes) = a (m bytes) sign extended or cut.
inline void sresize(Byte* r, std::size_t n, const Byte* a, std::size_t m) {
    const Byte fill = signBit(a, m) ? 0xff : 0;
    const std::size_t k = n < m ? n : m;
    for (std::size_t i = 0; i < k; ++i) r[i] = a[i];
    for (std::size_t i = k; i < n; ++i) r[i] = fill;
}
inline bool isZero(const Byte* a, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
        if (a[i] != 0) return false;
    return true;
}
// The value of a byte array as a count: its low 64 bits, or all ones when any higher byte is set (a shift count that big shifts everything out).
inline std::uint64_t asCount(const Byte* a, std::size_t n) {
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (i < 8) v |= std::uint64_t{a[i]} << (8 * i);
        else if (a[i] != 0) return ~std::uint64_t{0};
    }
    return v;
}
inline void fromU64(Byte* r, std::size_t n, std::uint64_t v) {
    for (std::size_t i = 0; i < n; ++i) r[i] = i < 8 ? static_cast<Byte>(v >> (8 * i)) : 0;
}

// ---- arithmetic (all of n bytes)
inline Byte add(Byte* r, const Byte* a, const Byte* b, std::size_t n) {   // returns the carry out
    unsigned carry = 0;
    for (std::size_t i = 0; i < n; ++i) {
        const unsigned s = unsigned{a[i]} + unsigned{b[i]} + carry;
        r[i] = static_cast<Byte>(s);
        carry = s >> 8;
    }
    return static_cast<Byte>(carry);
}
inline Byte sub(Byte* r, const Byte* a, const Byte* b, std::size_t n) {   // returns the borrow out (1 when a < b unsigned)
    unsigned borrow = 0;
    for (std::size_t i = 0; i < n; ++i) {
        const unsigned d = 0x100u + a[i] - b[i] - borrow;
        r[i] = static_cast<Byte>(d);
        borrow = (d >> 8) ^ 1u;
    }
    return static_cast<Byte>(borrow);
}
inline void mul(Byte* r, const Byte* a, const Byte* b, std::size_t n) {   // the low n bytes of the product
    Byte out[kMax] = {};
    std::size_t nb = n;   // (the bytes of b that are not leading zeros: a widened 64-bit operand has half of them)
    while (nb > 0 && b[nb - 1] == 0) --nb;
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] == 0) continue;
        unsigned carry = 0;
        std::size_t j = 0;
        for (; j < nb && i + j < n; ++j) {
            const unsigned t = unsigned{a[i]} * b[j] + out[i + j] + carry;
            out[i + j] = static_cast<Byte>(t);
            carry = t >> 8;
        }
        for (; carry != 0 && i + j < n; ++j) {   // (the carry out of the last product byte)
            const unsigned t = out[i + j] + carry;
            out[i + j] = static_cast<Byte>(t);
            carry = t >> 8;
        }
    }
    std::memcpy(r, out, n);
}
inline void bitNot(Byte* r, const Byte* a, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) r[i] = static_cast<Byte>(~a[i]);
}
inline void negate(Byte* r, const Byte* a, std::size_t n) {   // two's complement
    unsigned carry = 1;
    for (std::size_t i = 0; i < n; ++i) {
        const unsigned s = static_cast<Byte>(~a[i]) + carry;
        r[i] = static_cast<Byte>(s);
        carry = s >> 8;
    }
}
inline int compareUnsigned(const Byte* a, const Byte* b, std::size_t n) {
    for (std::size_t i = n; i-- > 0;)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}
inline int compareSigned(const Byte* a, const Byte* b, std::size_t n) {
    if (n == 0) return 0;
    const bool sa = signBit(a, n), sb = signBit(b, n);
    if (sa != sb) return sa ? -1 : 1;
    return compareUnsigned(a, b, n);
}
inline void shiftLeft(Byte* r, const Byte* a, std::size_t n, std::uint64_t bits) {
    if (bits >= std::uint64_t{8} * n) {
        zero(r, n);
        return;
    }
    const std::size_t bytes = static_cast<std::size_t>(bits / 8);
    const unsigned rest = static_cast<unsigned>(bits % 8);
    for (std::size_t i = n; i-- > 0;) {
        if (i < bytes) {
            r[i] = 0;
            continue;
        }
        const unsigned hi = a[i - bytes];
        const unsigned lo = i - bytes >= 1 ? a[i - bytes - 1] : 0u;
        r[i] = static_cast<Byte>(rest == 0 ? hi : ((hi << rest) | (lo >> (8 - rest))));
    }
}
// Shifts right filling with `fill` (0 or 0xff): logical or arithmetic.
inline void shiftRight(Byte* r, const Byte* a, std::size_t n, std::uint64_t bits, Byte fill) {
    if (bits >= std::uint64_t{8} * n) {
        for (std::size_t i = 0; i < n; ++i) r[i] = fill;
        return;
    }
    const std::size_t bytes = static_cast<std::size_t>(bits / 8);
    const unsigned rest = static_cast<unsigned>(bits % 8);
    for (std::size_t i = 0; i < n; ++i) {
        const unsigned lo = i + bytes < n ? a[i + bytes] : fill;
        const unsigned hi = i + bytes + 1 < n ? a[i + bytes + 1] : fill;
        r[i] = static_cast<Byte>(rest == 0 ? lo : ((lo >> rest) | (hi << (8 - rest))));
    }
}
inline unsigned popcount(const Byte* a, std::size_t n) {
    unsigned c = 0;
    for (std::size_t i = 0; i < n; ++i)
        for (unsigned b = a[i]; b != 0; b &= b - 1) ++c;
    return c;
}
inline unsigned leadingZeros(const Byte* a, std::size_t n) {   // of the n-byte value
    unsigned c = 0;
    for (std::size_t i = n; i-- > 0;) {
        if (a[i] == 0) {
            c += 8;
            continue;
        }
        for (unsigned bit = 0x80; (a[i] & bit) == 0; bit >>= 1) ++c;
        break;
    }
    return c;
}
// q = a / b and r = a % b, unsigned, n bytes each (b must not be zero); long division a bit at a time. The common cases are quick: a quotient and divisor that fit 64 bits are
// one host division, a dividend of up to 16 bytes by a divisor of up to 8 (x86's DIV r64 with its 128-bit dividend) 128 steps of 64-bit arithmetic, and the general one starts at
// the dividend's highest set bit.
inline void divmod(Byte* q, Byte* r, const Byte* a, const Byte* b, std::size_t n) {
    std::size_t na = n, nb = n;   // the bytes each really uses
    while (na > 0 && a[na - 1] == 0) --na;
    while (nb > 0 && b[nb - 1] == 0) --nb;
    const auto load = [](const Byte* p, std::size_t k) {
        std::uint64_t v = 0;
        for (std::size_t i = 0; i < k; ++i) v |= std::uint64_t{p[i]} << (8 * i);
        return v;
    };
    if (nb <= 8 && na <= 8) {
        const std::uint64_t x = load(a, na), y = load(b, nb);
        if (q) fromU64(q, n, x / y);
        if (r) fromU64(r, n, x % y);
        return;
    }
    if (nb <= 8 && na <= 16) {
        const std::uint64_t y = load(b, nb);
        std::uint64_t rem = 0;
        Byte quotient[16] = {};
        for (std::size_t bit = 8 * na; bit-- > 0;) {
            const bool carry = (rem >> 63) != 0;
            rem = (rem << 1) | ((a[bit / 8] >> (bit % 8)) & 1u);
            if (carry || rem >= y) {   // (with the carry the true remainder is over 2^64, so over y)
                rem -= y;
                quotient[bit / 8] = static_cast<Byte>(quotient[bit / 8] | (1u << (bit % 8)));
            }
        }
        if (q) {
            zero(q, n);
            std::memcpy(q, quotient, n < 16 ? n : 16);
        }
        if (r) fromU64(r, n, rem);
        return;
    }
    Byte quotient[kMax] = {}, rem[kMax] = {};
    for (std::size_t bit = 8 * na; bit-- > 0;) {
        shiftLeft(rem, rem, n, 1);   // (in place is fine: each byte reads lower ones first, going down)
        rem[0] = static_cast<Byte>(rem[0] | ((a[bit / 8] >> (bit % 8)) & 1u));
        if (compareUnsigned(rem, b, n) >= 0) {
            sub(rem, rem, b, n);
            quotient[bit / 8] = static_cast<Byte>(quotient[bit / 8] | (1u << (bit % 8)));
        }
    }
    if (q) std::memcpy(q, quotient, n);
    if (r) std::memcpy(r, rem, n);
}

// ---- one p-code operation on byte values. `a` is the first input (aSize bytes), `c` the second (cSize bytes; the count of a shift, the byte offset of a SUBPIECE), `r` the
// output (rSize bytes). Returns false for an operation that has no integer meaning here (floats and the rest are the caller's); sets `divideByZero` for a division by zero.
inline bool evaluate(unsigned opcode, Byte* r, std::size_t rSize, const Byte* a, std::size_t aSize, const Byte* c, std::size_t cSize, bool& divideByZero) {
    namespace gh = ghidra;
    divideByZero = false;
    if (rSize > kMax || aSize > kMax || cSize > kMax) return false;
    Byte A[kMax], C[kMax], T[kMax];
    switch (opcode) {
        case gh::CPUI_COPY:
        case gh::CPUI_INT_ZEXT: resize(r, rSize, a, aSize); return true;
        case gh::CPUI_INT_SEXT: sresize(r, rSize, a, aSize); return true;
        case gh::CPUI_PIECE: {   // a is the high part, c the low
            Byte out[kMax] = {};
            for (std::size_t i = 0; i < cSize && i < rSize; ++i) out[i] = c[i];
            for (std::size_t i = 0; i < aSize && cSize + i < rSize; ++i) out[cSize + i] = a[i];
            std::memcpy(r, out, rSize);
            return true;
        }
        case gh::CPUI_SUBPIECE: {
            const std::uint64_t at = asCount(c, cSize);
            for (std::size_t i = 0; i < rSize; ++i) r[i] = at < aSize && i < aSize - at ? a[at + i] : 0;
            return true;
        }
        case gh::CPUI_INT_AND: case gh::CPUI_INT_OR: case gh::CPUI_INT_XOR: case gh::CPUI_INT_ADD: case gh::CPUI_INT_SUB: case gh::CPUI_INT_MULT:
            resize(A, rSize, a, aSize);
            resize(C, rSize, c, cSize);
            switch (opcode) {
                case gh::CPUI_INT_AND: for (std::size_t i = 0; i < rSize; ++i) r[i] = A[i] & C[i]; break;
                case gh::CPUI_INT_OR: for (std::size_t i = 0; i < rSize; ++i) r[i] = A[i] | C[i]; break;
                case gh::CPUI_INT_XOR: for (std::size_t i = 0; i < rSize; ++i) r[i] = A[i] ^ C[i]; break;
                case gh::CPUI_INT_ADD: add(r, A, C, rSize); break;
                case gh::CPUI_INT_SUB: sub(r, A, C, rSize); break;
                default: mul(r, A, C, rSize); break;
            }
            return true;
        case gh::CPUI_INT_NEGATE: resize(A, rSize, a, aSize); bitNot(r, A, rSize); return true;
        case gh::CPUI_INT_2COMP: resize(A, rSize, a, aSize); negate(r, A, rSize); return true;
        case gh::CPUI_INT_LEFT: resize(A, rSize, a, aSize); shiftLeft(r, A, rSize, asCount(c, cSize)); return true;
        case gh::CPUI_INT_RIGHT: resize(A, rSize, a, aSize); shiftRight(r, A, rSize, asCount(c, cSize), 0); return true;
        case gh::CPUI_INT_SRIGHT: sresize(A, rSize, a, aSize); shiftRight(r, A, rSize, asCount(c, cSize), signBit(A, rSize) ? 0xff : 0); return true;
        case gh::CPUI_INT_EQUAL: case gh::CPUI_INT_NOTEQUAL: case gh::CPUI_INT_LESS: case gh::CPUI_INT_LESSEQUAL: case gh::CPUI_INT_SLESS: case gh::CPUI_INT_SLESSEQUAL: {
            resize(A, aSize, a, aSize);
            resize(C, aSize, c, cSize);
            const int u = compareUnsigned(A, C, aSize), s = compareSigned(A, C, aSize);
            bool yes = false;
            switch (opcode) {
                case gh::CPUI_INT_EQUAL: yes = u == 0; break;
                case gh::CPUI_INT_NOTEQUAL: yes = u != 0; break;
                case gh::CPUI_INT_LESS: yes = u < 0; break;
                case gh::CPUI_INT_LESSEQUAL: yes = u <= 0; break;
                case gh::CPUI_INT_SLESS: yes = s < 0; break;
                default: yes = s <= 0; break;
            }
            fromU64(r, rSize, yes ? 1 : 0);
            return true;
        }
        case gh::CPUI_INT_CARRY: {
            resize(A, aSize, a, aSize);
            resize(C, aSize, c, cSize);
            fromU64(r, rSize, add(T, A, C, aSize));
            return true;
        }
        case gh::CPUI_INT_SCARRY: case gh::CPUI_INT_SBORROW: {
            resize(A, aSize, a, aSize);
            resize(C, aSize, c, cSize);
            const bool sa = signBit(A, aSize), sb = signBit(C, aSize);
            if (opcode == gh::CPUI_INT_SCARRY) add(T, A, C, aSize);
            else sub(T, A, C, aSize);
            const bool sr = signBit(T, aSize);
            // carry: the operands' signs agree and the sum's differs; borrow: the operands' signs differ and the difference's sign is not a's
            const bool flag = opcode == gh::CPUI_INT_SCARRY ? (sr != sa) && (sa == sb) : (sa != sr) && (sr == sb);
            fromU64(r, rSize, flag ? 1 : 0);
            return true;
        }
        case gh::CPUI_INT_DIV: case gh::CPUI_INT_REM: case gh::CPUI_INT_SDIV: case gh::CPUI_INT_SREM: {
            const std::size_t w = aSize > cSize ? aSize : cSize;
            const bool isSigned = opcode == gh::CPUI_INT_SDIV || opcode == gh::CPUI_INT_SREM;
            if (isSigned) {
                sresize(A, w, a, aSize);
                sresize(C, w, c, cSize);
            } else {
                resize(A, w, a, aSize);
                resize(C, w, c, cSize);
            }
            if (isZero(C, w)) {
                divideByZero = true;
                return true;
            }
            const bool na = isSigned && signBit(A, w), nc = isSigned && signBit(C, w);
            if (na) negate(A, A, w);
            if (nc) negate(C, C, w);
            Byte q[kMax], rem[kMax];
            divmod(q, rem, A, C, w);
            const bool wantQuotient = opcode == gh::CPUI_INT_DIV || opcode == gh::CPUI_INT_SDIV;
            Byte* pick = wantQuotient ? q : rem;
            if (wantQuotient ? (na != nc) : na) negate(pick, pick, w);   // (a quotient is negative when the signs differ, a remainder follows the dividend)
            resize(r, rSize, pick, w);
            return true;
        }
        case gh::CPUI_POPCOUNT: fromU64(r, rSize, popcount(a, aSize)); return true;
        case gh::CPUI_LZCOUNT: fromU64(r, rSize, leadingZeros(a, aSize)); return true;
        default: return false;
    }
}

}  // namespace cb::decompile::sleigh::wide
