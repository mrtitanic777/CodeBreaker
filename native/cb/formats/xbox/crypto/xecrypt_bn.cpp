#include "cb/formats/xbox/crypto/xecrypt_bn.hpp"

#include <algorithm>
#include <bit>

#include "cb/core/error.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::xecrypt {

namespace {

using Limbs = std::vector<std::uint32_t>;

void bn_trim(Limbs& v) {
    while (!v.empty() && v.back() == 0) v.pop_back();
}

// The remainder of `v` divided by a 32-bit number.
std::uint32_t bn_small_remainder(const Limbs& v, std::uint32_t d) {
    std::uint64_t r = 0;
    for (std::size_t i = v.size(); i-- > 0;) r = ((r << 32) | v[i]) % d;
    return static_cast<std::uint32_t>(r);
}

// Quotient and remainder by one limb.
void bn_divide_small(const Limbs& u, std::uint32_t d, Limbs& q, std::uint32_t& r) {
    q.assign(u.size(), 0);
    std::uint64_t rem = 0;
    for (std::size_t i = u.size(); i-- > 0;) {
        const std::uint64_t cur = (rem << 32) | u[i];
        q[i] = static_cast<std::uint32_t>(cur / d);
        rem = cur % d;
    }
    bn_trim(q);
    r = static_cast<std::uint32_t>(rem);
}

// The primes below 1000, for trial division before the Miller-Rabin rounds.
const std::vector<std::uint32_t>& bn_small_primes() {
    static const std::vector<std::uint32_t> primes = [] {
        std::vector<std::uint32_t> p;
        for (std::uint32_t n = 3; n < 1000; n += 2) {
            bool prime = true;
            for (std::uint32_t d : p) {
                if (d * d > n) break;
                if (n % d == 0) {
                    prime = false;
                    break;
                }
            }
            if (prime) p.push_back(n);
        }
        return p;
    }();
    return primes;
}

// A signed number for the extended Euclid.
struct BnSigned {
    Nat mag;
    bool negative = false;
};
// a - q*b
BnSigned bn_sub_product(const BnSigned& a, const Nat& q, const BnSigned& b) {
    const Nat product = q * b.mag;
    const bool product_negative = b.negative && !product.is_zero();
    BnSigned out;
    if (a.negative != product_negative) {   // opposite signs: the magnitudes add
        out.mag = a.mag + product;
        out.negative = a.negative;
    } else if (a.mag >= product) {
        out.mag = a.mag - product;
        out.negative = a.negative && !out.mag.is_zero();
    } else {
        out.mag = product - a.mag;
        out.negative = !a.negative;
    }
    return out;
}

// ---- Montgomery multiplication, for the modular powers of an odd modulus ---------------------------------------------

// -m0^-1 mod 2^32 for an odd limb (Newton's iteration doubles the correct bits each round).
std::uint32_t bn_negated_inverse(std::uint32_t m0) {
    std::uint32_t inv = m0;   // correct to 3 bits for an odd number
    for (int i = 0; i < 4; ++i) inv *= 2 - m0 * inv;
    return 0u - inv;
}

// a * b * R^-1 mod n, R = 2^(32k): the finely integrated operand scanning of Koc et al. a and b hold k limbs, below n.
Limbs bn_montgomery_multiply(const Limbs& a, const Limbs& b, const Limbs& n, std::uint32_t n0inv) {
    const std::size_t k = n.size();
    Limbs t(k + 2, 0);
    for (std::size_t i = 0; i < k; ++i) {
        std::uint64_t carry = 0;
        const std::uint64_t bi = b[i];
        for (std::size_t j = 0; j < k; ++j) {
            const std::uint64_t cur = std::uint64_t{t[j]} + std::uint64_t{a[j]} * bi + carry;
            t[j] = static_cast<std::uint32_t>(cur);
            carry = cur >> 32;
        }
        std::uint64_t top = std::uint64_t{t[k]} + carry;
        t[k] = static_cast<std::uint32_t>(top);
        t[k + 1] = static_cast<std::uint32_t>(top >> 32);
        const std::uint64_t m = static_cast<std::uint32_t>(t[0] * n0inv);
        carry = (std::uint64_t{t[0]} + m * n[0]) >> 32;
        for (std::size_t j = 1; j < k; ++j) {
            const std::uint64_t cur = std::uint64_t{t[j]} + m * n[j] + carry;
            t[j - 1] = static_cast<std::uint32_t>(cur);
            carry = cur >> 32;
        }
        top = std::uint64_t{t[k]} + carry;
        t[k - 1] = static_cast<std::uint32_t>(top);
        t[k] = t[k + 1] + static_cast<std::uint32_t>(top >> 32);
    }
    // the result is t[0..k] and below 2n: one subtraction brings it under n
    bool at_least_n = t[k] != 0;
    if (!at_least_n) {
        at_least_n = true;   // compare t[0..k-1] with n from the top
        for (std::size_t i = k; i-- > 0;) {
            if (t[i] != n[i]) {
                at_least_n = t[i] > n[i];
                break;
            }
        }
    }
    if (at_least_n) {
        std::int64_t borrow = 0;
        for (std::size_t i = 0; i < k; ++i) {
            std::int64_t d = static_cast<std::int64_t>(t[i]) - n[i] - borrow;
            borrow = d < 0 ? 1 : 0;
            t[i] = static_cast<std::uint32_t>(d);
        }
    }
    t.resize(k);
    return t;
}

}  // namespace

Nat::Nat(std::uint64_t v) {
    if (v != 0) {
        limbs_.push_back(static_cast<std::uint32_t>(v));
        if (v >> 32) limbs_.push_back(static_cast<std::uint32_t>(v >> 32));
    }
}

void Nat::trim() { bn_trim(limbs_); }

Nat Nat::from_be(ByteView bytes) {
    Nat n;
    n.limbs_.assign((bytes.size() + 3) / 4, 0);
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        const std::size_t from_end = bytes.size() - 1 - i;   // byte i's weight is 256^from_end
        n.limbs_[from_end / 4] |= std::uint32_t{bytes[i]} << (8 * (from_end % 4));
    }
    n.trim();
    return n;
}

Nat Nat::from_le(ByteView bytes) {
    Nat n;
    n.limbs_.assign((bytes.size() + 3) / 4, 0);
    for (std::size_t i = 0; i < bytes.size(); ++i) n.limbs_[i / 4] |= std::uint32_t{bytes[i]} << (8 * (i % 4));
    n.trim();
    return n;
}

Bytes Nat::to_le(std::size_t size) const {
    if (bit_length() > size * 8) throw UsageError("a " + std::to_string(bit_length()) + "-bit number does not fit " + std::to_string(size) + " bytes");
    Bytes out(size, 0);
    for (std::size_t i = 0; i < size && i / 4 < limbs_.size(); ++i) out[i] = static_cast<std::uint8_t>(limbs_[i / 4] >> (8 * (i % 4)));
    return out;
}

Bytes Nat::to_be(std::size_t size) const {
    Bytes out = to_le(size);
    std::reverse(out.begin(), out.end());
    return out;
}

Bytes Nat::to_be() const { return to_be((bit_length() + 7) / 8); }

std::size_t Nat::bit_length() const {
    if (limbs_.empty()) return 0;
    return limbs_.size() * 32 - static_cast<std::size_t>(std::countl_zero(limbs_.back()));
}

bool Nat::bit(std::size_t i) const { return i / 32 < limbs_.size() && ((limbs_[i / 32] >> (i % 32)) & 1u); }

std::uint64_t Nat::low64() const {
    std::uint64_t v = 0;
    if (!limbs_.empty()) v = limbs_[0];
    if (limbs_.size() > 1) v |= std::uint64_t{limbs_[1]} << 32;
    return v;
}

std::strong_ordering operator<=>(const Nat& a, const Nat& b) {
    if (a.limbs_.size() != b.limbs_.size()) return a.limbs_.size() <=> b.limbs_.size();
    for (std::size_t i = a.limbs_.size(); i-- > 0;) {
        if (a.limbs_[i] != b.limbs_[i]) return a.limbs_[i] <=> b.limbs_[i];
    }
    return std::strong_ordering::equal;
}

Nat operator+(const Nat& a, const Nat& b) {
    Nat out;
    const std::size_t n = std::max(a.limbs_.size(), b.limbs_.size());
    out.limbs_.reserve(n + 1);
    std::uint64_t carry = 0;
    for (std::size_t i = 0; i < n; ++i) {
        const std::uint64_t sum = carry + (i < a.limbs_.size() ? a.limbs_[i] : 0) + (i < b.limbs_.size() ? b.limbs_[i] : 0);
        out.limbs_.push_back(static_cast<std::uint32_t>(sum));
        carry = sum >> 32;
    }
    if (carry) out.limbs_.push_back(static_cast<std::uint32_t>(carry));
    return out;
}

Nat operator-(const Nat& a, const Nat& b) {
    if (a < b) throw UsageError("a natural number cannot go below zero");
    Nat out;
    out.limbs_.reserve(a.limbs_.size());
    std::int64_t borrow = 0;
    for (std::size_t i = 0; i < a.limbs_.size(); ++i) {
        std::int64_t d = static_cast<std::int64_t>(a.limbs_[i]) - borrow - (i < b.limbs_.size() ? b.limbs_[i] : 0);
        borrow = d < 0 ? 1 : 0;
        if (d < 0) d += std::int64_t{1} << 32;
        out.limbs_.push_back(static_cast<std::uint32_t>(d));
    }
    out.trim();
    return out;
}

Nat operator*(const Nat& a, const Nat& b) {
    Nat out;
    if (a.is_zero() || b.is_zero()) return out;
    out.limbs_.assign(a.limbs_.size() + b.limbs_.size(), 0);
    for (std::size_t i = 0; i < a.limbs_.size(); ++i) {
        std::uint64_t carry = 0;
        const std::uint64_t ai = a.limbs_[i];
        for (std::size_t j = 0; j < b.limbs_.size(); ++j) {
            const std::uint64_t cur = ai * b.limbs_[j] + out.limbs_[i + j] + carry;
            out.limbs_[i + j] = static_cast<std::uint32_t>(cur);
            carry = cur >> 32;
        }
        out.limbs_[i + b.limbs_.size()] = static_cast<std::uint32_t>(carry);
    }
    out.trim();
    return out;
}

Nat Nat::shifted_left(std::size_t bits) const {
    if (limbs_.empty()) return {};
    Nat out;
    const std::size_t whole = bits / 32, part = bits % 32;
    out.limbs_.assign(limbs_.size() + whole + 1, 0);
    for (std::size_t i = 0; i < limbs_.size(); ++i) {
        const std::uint64_t v = std::uint64_t{limbs_[i]} << part;
        out.limbs_[i + whole] |= static_cast<std::uint32_t>(v);
        out.limbs_[i + whole + 1] |= static_cast<std::uint32_t>(v >> 32);
    }
    out.trim();
    return out;
}

Nat Nat::shifted_right(std::size_t bits) const {
    const std::size_t whole = bits / 32, part = bits % 32;
    if (whole >= limbs_.size()) return {};
    Nat out;
    out.limbs_.assign(limbs_.size() - whole, 0);
    for (std::size_t i = whole; i < limbs_.size(); ++i) {
        std::uint64_t v = limbs_[i];
        if (i + 1 < limbs_.size()) v |= std::uint64_t{limbs_[i + 1]} << 32;
        out.limbs_[i - whole] = static_cast<std::uint32_t>(v >> part);
    }
    out.trim();
    return out;
}

// Knuth, TAOCP vol. 2, 4.3.1, algorithm D.
Nat::DivMod Nat::div_mod(const Nat& divisor) const {
    if (divisor.is_zero()) throw UsageError("division by zero");
    if (*this < divisor) return {Nat(), *this};
    if (divisor.limbs_.size() == 1) {
        DivMod out;
        std::uint32_t r = 0;
        bn_divide_small(limbs_, divisor.limbs_[0], out.quotient.limbs_, r);
        out.remainder = Nat(r);
        return out;
    }
    const std::size_t s = static_cast<std::size_t>(std::countl_zero(divisor.limbs_.back()));
    Limbs v = divisor.shifted_left(s).limbs_;
    Limbs u = shifted_left(s).limbs_;
    const std::size_t n = v.size();
    u.resize(std::max(u.size(), limbs_.size() + 1), 0);   // one limb above the dividend
    const std::size_t m = u.size() - n - 1;
    Limbs q(m + 1, 0);
    constexpr std::uint64_t kBase = std::uint64_t{1} << 32;
    for (std::size_t j = m + 1; j-- > 0;) {
        const std::uint64_t top = (std::uint64_t{u[j + n]} << 32) | u[j + n - 1];
        std::uint64_t qhat = top / v[n - 1];
        std::uint64_t rhat = top % v[n - 1];
        while (qhat >= kBase || qhat * v[n - 2] > ((rhat << 32) | u[j + n - 2])) {
            --qhat;
            rhat += v[n - 1];
            if (rhat >= kBase) break;
        }
        // multiply and subtract
        std::int64_t borrow = 0;
        std::uint64_t carry = 0;
        for (std::size_t i = 0; i < n; ++i) {
            const std::uint64_t p = qhat * v[i] + carry;
            carry = p >> 32;
            const std::int64_t t = static_cast<std::int64_t>(u[i + j]) - borrow - static_cast<std::int64_t>(p & 0xFFFFFFFFu);
            u[i + j] = static_cast<std::uint32_t>(t);
            borrow = t < 0 ? 1 : 0;
        }
        const std::int64_t t = static_cast<std::int64_t>(u[j + n]) - borrow - static_cast<std::int64_t>(carry);
        u[j + n] = static_cast<std::uint32_t>(t);
        if (t < 0) {   // qhat was one too big: add the divisor back
            --qhat;
            std::uint64_t c = 0;
            for (std::size_t i = 0; i < n; ++i) {
                const std::uint64_t sum = std::uint64_t{u[i + j]} + v[i] + c;
                u[i + j] = static_cast<std::uint32_t>(sum);
                c = sum >> 32;
            }
            u[j + n] += static_cast<std::uint32_t>(c);
        }
        q[j] = static_cast<std::uint32_t>(qhat);
    }
    DivMod out;
    out.quotient.limbs_ = std::move(q);
    out.quotient.trim();
    u.resize(n);
    out.remainder.limbs_ = std::move(u);
    out.remainder.trim();
    out.remainder = out.remainder.shifted_right(s);
    return out;
}

Nat Nat::operator%(const Nat& m) const { return div_mod(m).remainder; }
Nat Nat::operator/(const Nat& d) const { return div_mod(d).quotient; }

Nat Nat::pow_mod(const Nat& base, const Nat& exponent, const Nat& modulus) {
    if (modulus.is_zero()) throw UsageError("modulus of zero");
    if (exponent.is_zero()) return Nat(1) % modulus;
    if (modulus.is_odd() && modulus.limbs_.size() >= 2) {   // Montgomery: no division inside the loop
        const std::size_t k = modulus.limbs_.size();
        const std::uint32_t n0inv = bn_negated_inverse(modulus.limbs_[0]);
        auto padded = [k](Limbs v) {
            v.resize(k, 0);
            return v;
        };
        const Nat r_mod_n = Nat(1).shifted_left(32 * k) % modulus;                       // 1 in Montgomery form
        const Nat base_mont = (base % modulus).shifted_left(32 * k) % modulus;           // the base in Montgomery form
        const Limbs b = padded(base_mont.limbs_);
        Limbs x = padded(r_mod_n.limbs_);
        for (std::size_t i = exponent.bit_length(); i-- > 0;) {
            x = bn_montgomery_multiply(x, x, modulus.limbs_, n0inv);
            if (exponent.bit(i)) x = bn_montgomery_multiply(x, b, modulus.limbs_, n0inv);
        }
        Limbs one(k, 0);
        one[0] = 1;
        Nat out;
        out.limbs_ = bn_montgomery_multiply(x, one, modulus.limbs_, n0inv);
        out.trim();
        return out;
    }
    Nat result = Nat(1) % modulus;
    const Nat b = base % modulus;
    for (std::size_t i = exponent.bit_length(); i-- > 0;) {
        result = (result * result) % modulus;
        if (exponent.bit(i)) result = (result * b) % modulus;
    }
    return result;
}

std::optional<Nat> Nat::inv_mod(const Nat& a, const Nat& m) {
    if (m < Nat(2)) return std::nullopt;
    Nat r0 = m, r1 = a % m;
    BnSigned t0{Nat(0), false}, t1{Nat(1), false};
    while (!r1.is_zero()) {
        const DivMod qr = r0.div_mod(r1);
        BnSigned t2 = bn_sub_product(t0, qr.quotient, t1);
        r0 = std::move(r1);
        r1 = qr.remainder;
        t0 = std::move(t1);
        t1 = std::move(t2);
    }
    if (!(r0 == Nat(1))) return std::nullopt;
    Nat mag = t0.mag % m;
    if (t0.negative && !mag.is_zero()) mag = m - mag;
    return mag;
}

Nat Nat::gcd(Nat a, Nat b) {
    while (!b.is_zero()) {
        Nat r = a % b;
        a = std::move(b);
        b = std::move(r);
    }
    return a;
}

Nat Nat::random_below(const Nat& limit, RandomSource& rng) {
    if (limit.is_zero()) throw UsageError("random_below(0)");
    const std::size_t bytes = (limit.bit_length() + 7) / 8 + 8;   // eight spare bytes make the modulo bias negligible
    return from_be(rng.bytes(bytes)) % limit;
}

bool Nat::is_probable_prime(RandomSource& rng, int rounds) const {
    if (*this < Nat(2)) return false;
    for (std::uint32_t p : bn_small_primes()) {
        if (*this == Nat(p)) return true;
        if (bn_small_remainder(limbs_, p) == 0) return false;
    }
    if (!is_odd()) return *this == Nat(2);
    const Nat minus_one = *this - Nat(1);
    std::size_t s = 0;
    while (!minus_one.bit(s)) ++s;
    const Nat d = minus_one.shifted_right(s);
    const Nat three(3);
    for (int round = 0; round < rounds; ++round) {
        const Nat a = random_below(*this - three, rng) + Nat(2);   // 2 .. n-2
        Nat x = pow_mod(a, d, *this);
        if (x == Nat(1) || x == minus_one) continue;
        bool composite = true;
        for (std::size_t i = 1; i < s; ++i) {
            x = (x * x) % *this;
            if (x == minus_one) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

Nat Nat::random_prime(std::size_t bits, RandomSource& rng) {
    if (bits < 16) throw UsageError("primes of fewer than 16 bits are not generated");
    const std::size_t bytes = (bits + 7) / 8;
    for (;;) {
        Bytes raw = rng.bytes(bytes);
        Nat candidate = from_be(raw);
        candidate = candidate % Nat(1).shifted_left(bits);   // exactly `bits` bits, then the top two set
        candidate = candidate + (Nat(3).shifted_left(bits - 2)) - (candidate.shifted_right(bits - 2).shifted_left(bits - 2));
        if (!candidate.is_odd()) candidate = candidate + Nat(1);
        if (candidate.is_probable_prime(rng)) return candidate;
    }
}

std::string Nat::to_hex() const {
    if (limbs_.empty()) return "0";
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string out;
    for (std::size_t i = limbs_.size(); i-- > 0;) {
        for (int shift = 28; shift >= 0; shift -= 4) {
            const char c = kDigits[(limbs_[i] >> shift) & 0xF];
            if (!out.empty() || c != '0') out.push_back(c);
        }
    }
    return out;
}

}  // namespace cb::formats::xbox::crypto::xecrypt
