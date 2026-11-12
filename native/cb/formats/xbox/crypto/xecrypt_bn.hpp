// cb/formats/xbox/crypto/xecrypt_bn.hpp -- the unsigned big numbers behind XeCrypt's RSA (up to 4096-bit keys).
//
// Nat is a natural number of any size: the arithmetic RSA needs (compare, add, subtract, multiply, divide with remainder, modular
// power, modular inverse), the byte forms the Xbox 360 uses (a "BnQw" number is big-endian qwords, least significant first, so the
// conversions are in terms of plain big-endian / little-endian bytes and the qword swap lives in xecrypt.hpp), and Miller-Rabin
// primality for key generation. It is small and schoolbook-simple on purpose: keys are at most 4096 bits and used a few times
// per run, nothing here is meant to be constant-time (these are tools that work with keys you hold, not a service).
#pragma once

#include <compare>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::xbox::crypto::xecrypt {

class RandomSource;

class Nat {
public:
    Nat() = default;
    Nat(std::uint64_t v);   // NOLINT: a number from a plain integer reads naturally

    static Nat from_be(ByteView bytes);   // any length; leading zeros are fine
    static Nat from_le(ByteView bytes);
    // `size` bytes, most significant first. UsageError if the number does not fit.
    Bytes to_be(std::size_t size) const;
    Bytes to_le(std::size_t size) const;
    // The minimal big-endian bytes (empty for 0).
    Bytes to_be() const;

    bool is_zero() const { return limbs_.empty(); }
    bool is_odd() const { return !limbs_.empty() && (limbs_[0] & 1u); }
    std::size_t bit_length() const;
    bool bit(std::size_t i) const;
    // The low 64 bits.
    std::uint64_t low64() const;

    friend bool operator==(const Nat& a, const Nat& b) { return a.limbs_ == b.limbs_; }
    friend std::strong_ordering operator<=>(const Nat& a, const Nat& b);

    friend Nat operator+(const Nat& a, const Nat& b);
    // a - b; UsageError if b > a (naturals do not go below zero).
    friend Nat operator-(const Nat& a, const Nat& b);
    friend Nat operator*(const Nat& a, const Nat& b);
    Nat shifted_left(std::size_t bits) const;
    Nat shifted_right(std::size_t bits) const;

    struct DivMod;
    // Quotient and remainder; UsageError for a zero divisor.
    DivMod div_mod(const Nat& divisor) const;
    Nat operator%(const Nat& m) const;
    Nat operator/(const Nat& d) const;

    // (base ^ exponent) mod modulus. UsageError for a zero modulus.
    static Nat pow_mod(const Nat& base, const Nat& exponent, const Nat& modulus);
    // The inverse of `a` modulo `m`, or nullopt when they share a factor (or m < 2).
    static std::optional<Nat> inv_mod(const Nat& a, const Nat& m);
    // Greatest common divisor.
    static Nat gcd(Nat a, Nat b);

    // Probable prime: trial division by small primes, then `rounds` Miller-Rabin rounds with bases from `rng`.
    bool is_probable_prime(RandomSource& rng, int rounds = 24) const;
    // A random prime of exactly `bits` bits (top two bits set, so a product of two of them has the full 2*bits).
    static Nat random_prime(std::size_t bits, RandomSource& rng);
    // A random number below `limit` (limit > 0).
    static Nat random_below(const Nat& limit, RandomSource& rng);

    std::string to_hex() const;   // lower case, no leading zeros, "0" for zero

private:
    void trim();
    std::vector<std::uint32_t> limbs_;   // little-endian limbs, no leading zero limb
};

struct Nat::DivMod {
    Nat quotient;
    Nat remainder;
};

}  // namespace cb::formats::xbox::crypto::xecrypt
