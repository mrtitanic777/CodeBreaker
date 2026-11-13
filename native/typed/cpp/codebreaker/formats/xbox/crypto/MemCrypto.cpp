// codebreaker/formats/xbox/crypto/MemCrypto.cpp -- C++ port of codebreaker.formats.xbox.crypto.MemCrypto.

#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"

#include <algorithm>
#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#endif

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>

#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::xbox::crypto::MemCrypto {

// Python's NameError (UINT{b}_MASK is looked up by name in rotr)
struct NameError : py::Exception {
    explicit NameError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "NameError"; }
};

// =====================================================================================================
// BigInt
// =====================================================================================================
BigInt::BigInt(uint64_t v) {
    while (v) {
        limbs_.push_back(static_cast<uint32_t>(v & 0xFFFFFFFFu));
        v >>= 32;
    }
}

void BigInt::trim_() {
    while (!limbs_.empty() && limbs_.back() == 0) limbs_.pop_back();
}

BigInt BigInt::from_bytes(const py::bytes& b, const char* order) {
    bool little = std::strcmp(order, "little") == 0;
    BigInt r;
    int64_t n = b.size();
    r.limbs_.assign(static_cast<size_t>((n + 3) / 4), 0);
    for (int64_t k = 0; k < n; ++k) {
        int64_t i = little ? k : n - 1 - k;   // byte k counted from the least significant end
        r.limbs_[static_cast<size_t>(k / 4)] |= static_cast<uint32_t>(b.byte_(i)) << (8 * (k % 4));
    }
    r.trim_();
    return r;
}

BigInt BigInt::from_hex(const std::string& hex) {
    BigInt r;
    for (char c : hex) {
        int v = (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'f') ? c - 'a' + 10 : (c >= 'A' && c <= 'F') ? c - 'A' + 10 : -1;
        if (v < 0) throw py::ValueError("invalid literal for int() with base 16");
        r = r.shl(4) | BigInt(static_cast<uint64_t>(v));
    }
    return r;
}

BigInt BigInt::from_signed_masked(int64_t v, int64_t bits) {
    // sign-extend v to `bits` bits, as the low bits of the infinite two's complement
    BigInt r;
    int64_t nlimbs = (bits + 31) / 32;
    r.limbs_.assign(static_cast<size_t>(nlimbs), 0);
    uint64_t u = static_cast<uint64_t>(v);
    for (int64_t i = 0; i < nlimbs; ++i) {
        uint32_t limb;
        if (i < 2) limb = static_cast<uint32_t>(u >> (32 * i));
        else limb = v < 0 ? 0xFFFFFFFFu : 0;
        r.limbs_[static_cast<size_t>(i)] = limb;
    }
    r.trim_();
    return r.masked(bits);
}

py::bytes BigInt::to_bytes(int64_t length, const char* order) const {
    if (length < 0) throw py::ValueError("length argument must be non-negative");
    if (bit_length() > length * 8) throw py::OverflowError("int too big to convert");
    bool little = std::strcmp(order, "little") == 0;
    std::string out(static_cast<size_t>(length), '\0');
    for (int64_t k = 0; k < length; ++k) {
        size_t limb = static_cast<size_t>(k / 4);
        uint8_t byte = limb < limbs_.size() ? static_cast<uint8_t>((limbs_[limb] >> (8 * (k % 4))) & 0xFF) : 0;
        out[static_cast<size_t>(little ? k : length - 1 - k)] = static_cast<char>(byte);
    }
    return py::bytes(out);
}

int64_t BigInt::to_int64() const {
    if (bit_length() > 63) throw py::OverflowError("int too large for int64");
    uint64_t v = 0;
    for (size_t i = 0; i < limbs_.size() && i < 2; ++i) v |= static_cast<uint64_t>(limbs_[i]) << (32 * i);
    return static_cast<int64_t>(v);
}

int64_t BigInt::bit_length() const {
    if (limbs_.empty()) return 0;
    uint32_t top = limbs_.back();
    int64_t n = 0;
    while (top) {
        ++n;
        top >>= 1;
    }
    return static_cast<int64_t>(limbs_.size() - 1) * 32 + n;
}

bool BigInt::bit(int64_t i) const {
    size_t limb = static_cast<size_t>(i / 32);
    return limb < limbs_.size() && ((limbs_[limb] >> (i % 32)) & 1);
}

BigInt BigInt::shl(int64_t n) const {
    if (n < 0) throw py::ValueError("negative shift count");
    if (limbs_.empty()) return BigInt();
    BigInt r;
    size_t whole = static_cast<size_t>(n / 32);
    int part = static_cast<int>(n % 32);
    r.limbs_.assign(limbs_.size() + whole + 1, 0);
    for (size_t i = 0; i < limbs_.size(); ++i) {
        uint64_t v = static_cast<uint64_t>(limbs_[i]) << part;
        r.limbs_[i + whole] |= static_cast<uint32_t>(v & 0xFFFFFFFFu);
        r.limbs_[i + whole + 1] |= static_cast<uint32_t>(v >> 32);
    }
    r.trim_();
    return r;
}

BigInt BigInt::shr(int64_t n) const {
    if (n < 0) throw py::ValueError("negative shift count");
    size_t whole = static_cast<size_t>(n / 32);
    int part = static_cast<int>(n % 32);
    if (whole >= limbs_.size()) return BigInt();
    BigInt r;
    r.limbs_.assign(limbs_.size() - whole, 0);
    for (size_t i = whole; i < limbs_.size(); ++i) {
        uint64_t v = limbs_[i];
        if (i + 1 < limbs_.size()) v |= static_cast<uint64_t>(limbs_[i + 1]) << 32;
        r.limbs_[i - whole] = static_cast<uint32_t>((v >> part) & 0xFFFFFFFFu);
    }
    r.trim_();
    return r;
}

BigInt BigInt::masked(int64_t bits) const {
    BigInt r = *this;
    size_t whole = static_cast<size_t>(bits / 32);
    int part = static_cast<int>(bits % 32);
    if (whole < r.limbs_.size()) {
        if (part) {
            r.limbs_[whole] &= (1u << part) - 1;
            r.limbs_.resize(whole + 1);
        } else {
            r.limbs_.resize(whole);
        }
    }
    r.trim_();
    return r;
}

int BigInt::compare(const BigInt& o) const {
    if (limbs_.size() != o.limbs_.size()) return limbs_.size() < o.limbs_.size() ? -1 : 1;
    for (size_t i = limbs_.size(); i-- > 0;)
        if (limbs_[i] != o.limbs_[i]) return limbs_[i] < o.limbs_[i] ? -1 : 1;
    return 0;
}

BigInt operator^(const BigInt& a, const BigInt& b) {
    BigInt r;
    size_t n = std::max(a.limbs_.size(), b.limbs_.size());
    r.limbs_.assign(n, 0);
    for (size_t i = 0; i < n; ++i)
        r.limbs_[i] = (i < a.limbs_.size() ? a.limbs_[i] : 0) ^ (i < b.limbs_.size() ? b.limbs_[i] : 0);
    r.trim_();
    return r;
}

BigInt operator&(const BigInt& a, const BigInt& b) {
    BigInt r;
    size_t n = std::min(a.limbs_.size(), b.limbs_.size());
    r.limbs_.assign(n, 0);
    for (size_t i = 0; i < n; ++i) r.limbs_[i] = a.limbs_[i] & b.limbs_[i];
    r.trim_();
    return r;
}

BigInt operator|(const BigInt& a, const BigInt& b) {
    BigInt r;
    size_t n = std::max(a.limbs_.size(), b.limbs_.size());
    r.limbs_.assign(n, 0);
    for (size_t i = 0; i < n; ++i)
        r.limbs_[i] = (i < a.limbs_.size() ? a.limbs_[i] : 0) | (i < b.limbs_.size() ? b.limbs_[i] : 0);
    r.trim_();
    return r;
}

BigInt operator+(const BigInt& a, const BigInt& b) {
    BigInt r;
    size_t n = std::max(a.limbs_.size(), b.limbs_.size());
    r.limbs_.assign(n + 1, 0);
    uint64_t carry = 0;
    for (size_t i = 0; i < n; ++i) {
        uint64_t v = carry + (i < a.limbs_.size() ? a.limbs_[i] : 0) + (i < b.limbs_.size() ? b.limbs_[i] : 0);
        r.limbs_[i] = static_cast<uint32_t>(v & 0xFFFFFFFFu);
        carry = v >> 32;
    }
    r.limbs_[n] = static_cast<uint32_t>(carry);
    r.trim_();
    return r;
}

BigInt operator-(const BigInt& a, const BigInt& b) {
    if (a.compare(b) < 0) throw py::ValueError("BigInt: a negative result");
    BigInt r = a;
    int64_t borrow = 0;
    for (size_t i = 0; i < r.limbs_.size(); ++i) {
        int64_t v = static_cast<int64_t>(r.limbs_[i]) - borrow - (i < b.limbs_.size() ? static_cast<int64_t>(b.limbs_[i]) : 0);
        borrow = v < 0 ? 1 : 0;
        if (v < 0) v += (int64_t(1) << 32);
        r.limbs_[i] = static_cast<uint32_t>(v);
    }
    r.trim_();
    return r;
}

BigInt operator*(const BigInt& a, const BigInt& b) {
    BigInt r;
    if (a.limbs_.empty() || b.limbs_.empty()) return r;
    r.limbs_.assign(a.limbs_.size() + b.limbs_.size(), 0);
    for (size_t i = 0; i < a.limbs_.size(); ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < b.limbs_.size(); ++j) {
            uint64_t v = static_cast<uint64_t>(a.limbs_[i]) * b.limbs_[j] + r.limbs_[i + j] + carry;
            r.limbs_[i + j] = static_cast<uint32_t>(v & 0xFFFFFFFFu);
            carry = v >> 32;
        }
        r.limbs_[i + b.limbs_.size()] += static_cast<uint32_t>(carry);
    }
    r.trim_();
    return r;
}

// long division on 32-bit limbs (Knuth, TAOCP vol. 2, 4.3.1 algorithm D); the RSA exponentiation and the
// prime search run it thousands of times
void BigInt::divmod_(const BigInt& a, const BigInt& b, BigInt& q, BigInt& r) {
    if (b.limbs_.empty()) throw py::ZeroDivisionError("integer division or modulo by zero");
    const size_t n = b.limbs_.size();
    const size_t m = a.limbs_.size();
    q.limbs_.clear();
    r.limbs_.clear();
    if (m < n || (m == n && a.compare(b) < 0)) {
        r.limbs_ = a.limbs_;
        return;
    }
    if (n == 1) {
        const uint64_t d = b.limbs_[0];
        q.limbs_.assign(m, 0);
        uint64_t rem = 0;
        for (size_t i = m; i-- > 0;) {
            uint64_t cur = (rem << 32) | a.limbs_[i];
            q.limbs_[i] = static_cast<uint32_t>(cur / d);
            rem = cur % d;
        }
        if (rem) r.limbs_.push_back(static_cast<uint32_t>(rem));
        q.trim_();
        return;
    }
    // normalise so that the divisor's top limb has its high bit set
    int s = 0;
    while (!((b.limbs_[n - 1] << s) & 0x80000000u)) ++s;
    std::vector<uint32_t> v(n), u(m + 1);
    for (size_t i = n - 1; i > 0; --i)
        v[i] = (b.limbs_[i] << s) | (s ? static_cast<uint32_t>(static_cast<uint64_t>(b.limbs_[i - 1]) >> (32 - s)) : 0);
    v[0] = b.limbs_[0] << s;
    u[m] = s ? static_cast<uint32_t>(static_cast<uint64_t>(a.limbs_[m - 1]) >> (32 - s)) : 0;
    for (size_t i = m - 1; i > 0; --i)
        u[i] = (a.limbs_[i] << s) | (s ? static_cast<uint32_t>(static_cast<uint64_t>(a.limbs_[i - 1]) >> (32 - s)) : 0);
    u[0] = a.limbs_[0] << s;
    q.limbs_.assign(m - n + 1, 0);
    const uint64_t base = uint64_t(1) << 32;
    for (size_t j = m - n + 1; j-- > 0;) {
        uint64_t num = (static_cast<uint64_t>(u[j + n]) << 32) | u[j + n - 1];
        uint64_t qhat = num / v[n - 1];
        uint64_t rhat = num % v[n - 1];
        while (qhat >= base || qhat * v[n - 2] > ((rhat << 32) | u[j + n - 2])) {
            --qhat;
            rhat += v[n - 1];
            if (rhat >= base) break;
        }
        int64_t borrow = 0;
        uint64_t carry = 0;
        for (size_t i = 0; i < n; ++i) {
            uint64_t p = qhat * v[i] + carry;
            carry = p >> 32;
            int64_t t = static_cast<int64_t>(u[i + j]) - borrow - static_cast<int64_t>(p & 0xFFFFFFFFu);
            u[i + j] = static_cast<uint32_t>(t);
            borrow = t < 0 ? 1 : 0;
        }
        int64_t t = static_cast<int64_t>(u[j + n]) - borrow - static_cast<int64_t>(carry);
        u[j + n] = static_cast<uint32_t>(t);
        if (t < 0) {   // qhat was one too big: add the divisor back
            --qhat;
            uint64_t c = 0;
            for (size_t i = 0; i < n; ++i) {
                uint64_t sum = static_cast<uint64_t>(u[i + j]) + v[i] + c;
                u[i + j] = static_cast<uint32_t>(sum);
                c = sum >> 32;
            }
            u[j + n] += static_cast<uint32_t>(c);
        }
        q.limbs_[j] = static_cast<uint32_t>(qhat);
    }
    r.limbs_.assign(n, 0);
    for (size_t i = 0; i < n; ++i)
        r.limbs_[i] = (u[i] >> s) | (s ? static_cast<uint32_t>(static_cast<uint64_t>(u[i + 1]) << (32 - s)) : 0);
    q.trim_();
    r.trim_();
}

BigInt operator/(const BigInt& a, const BigInt& b) {
    BigInt q, r;
    BigInt::divmod_(a, b, q, r);
    return q;
}

BigInt operator%(const BigInt& a, const BigInt& b) {
    BigInt q, r;
    BigInt::divmod_(a, b, q, r);
    return r;
}

namespace {

#if defined(_MSC_VER) && !defined(__clang__)
inline uint64_t mul64(uint64_t a, uint64_t b, uint64_t& hi) { return _umul128(a, b, &hi); }
#else
inline uint64_t mul64(uint64_t a, uint64_t b, uint64_t& hi) {
    unsigned __int128 p = static_cast<unsigned __int128>(a) * b;
    hi = static_cast<uint64_t>(p >> 64);
    return static_cast<uint64_t>(p);
}
#endif
inline uint64_t add_c(uint64_t a, uint64_t b, uint64_t& carry) {   // a + b, carry set to the carry out (0 or 1)
    uint64_t s = a + b;
    carry = s < a;
    return s;
}

// Montgomery arithmetic on 64-bit limbs for an odd modulus (CIOS multiplication)
struct Mont {
    size_t k;
    std::vector<uint64_t> m;
    uint64_t n0inv;   // -m^-1 mod 2^64

    explicit Mont(std::vector<uint64_t> mod) : k(mod.size()), m(std::move(mod)) {
        uint64_t inv = 1;   // Newton's iteration for m[0]^-1 mod 2^64
        for (int i = 0; i < 6; ++i) inv *= 2 - m[0] * inv;
        n0inv = 0 - inv;
    }

    // out = a * b * R^-1 mod m, for a, b < m (t is scratch of k + 2 limbs; out may alias a or b)
    void mul(const uint64_t* a, const uint64_t* b, uint64_t* out, uint64_t* t) const {
        std::fill(t, t + k + 2, 0);
        for (size_t i = 0; i < k; ++i) {
            // t += a * b[i]
            uint64_t carry = 0;
            const uint64_t bi = b[i];
            for (size_t j = 0; j < k; ++j) {
                uint64_t hi, lo = mul64(a[j], bi, hi), c;
                lo = add_c(lo, t[j], c);
                hi += c;
                lo = add_c(lo, carry, c);
                hi += c;
                t[j] = lo;
                carry = hi;
            }
            uint64_t c;
            t[k] = add_c(t[k], carry, c);
            t[k + 1] = c;
            // t = (t + mq * m) / 2^64
            const uint64_t mq = t[0] * n0inv;
            uint64_t hi, lo = mul64(mq, m[0], hi);
            lo = add_c(lo, t[0], c);
            carry = hi + c;
            for (size_t j = 1; j < k; ++j) {
                lo = mul64(mq, m[j], hi);
                lo = add_c(lo, t[j], c);
                hi += c;
                lo = add_c(lo, carry, c);
                hi += c;
                t[j - 1] = lo;
                carry = hi;
            }
            t[k - 1] = add_c(t[k], carry, c);
            t[k] = t[k + 1] + c;
        }
        bool ge = t[k] != 0;   // conditional subtraction of m
        if (!ge) {
            ge = true;
            for (size_t j = k; j-- > 0;) {
                if (t[j] != m[j]) {
                    ge = t[j] > m[j];
                    break;
                }
            }
        }
        if (ge) {
            uint64_t borrow = 0;
            for (size_t j = 0; j < k; ++j) {
                uint64_t d = t[j] - m[j];
                uint64_t b1 = t[j] < m[j];
                uint64_t d2 = d - borrow;
                borrow = b1 | (d < borrow);
                out[j] = d2;
            }
        } else {
            std::copy(t, t + k, out);
        }
    }
};

}  // namespace

BigInt BigInt::powmod(const BigInt& base, const BigInt& exp, const BigInt& mod) {
    if (mod.is_zero()) throw py::ValueError("pow() 3rd argument cannot be 0");
    BigInt b = base % mod;
    const int64_t ebits = exp.bit_length();
    if (!(mod.limbs_[0] & 1) || mod.limbs_.size() < 2) {   // even (or one-limb) modulus: square and multiply
        BigInt result = BigInt(1) % mod;
        for (int64_t i = ebits - 1; i >= 0; --i) {
            result = (result * result) % mod;
            if (exp.bit(i)) result = (result * b) % mod;
        }
        return result;
    }
    if (ebits == 0) return BigInt(1) % mod;
    auto to64 = [](const BigInt& v, size_t k) {
        std::vector<uint64_t> out(k, 0);
        for (size_t i = 0; i < v.limbs_.size(); ++i) out[i / 2] |= static_cast<uint64_t>(v.limbs_[i]) << (32 * (i % 2));
        return out;
    };
    const size_t k = (mod.limbs_.size() + 1) / 2;
    Mont mt(to64(mod, k));
    // R^2 mod m, with R = 2^(64 k): the base goes into Montgomery form by one multiplication
    BigInt r2 = BigInt(1).shl(static_cast<int64_t>(128 * k)) % mod;
    std::vector<uint64_t> r2l = to64(r2, k), bl = to64(b, k), t(k + 2), bm(k), x(k), y(k);
    mt.mul(bl.data(), r2l.data(), bm.data(), t.data());
    // sliding window: the odd powers bm^1, bm^3, ..., bm^(2^W - 1)
    const int W = ebits > 512 ? 5 : (ebits > 128 ? 4 : (ebits > 24 ? 3 : 1));
    std::vector<std::vector<uint64_t>> tab(size_t(1) << (W - 1), std::vector<uint64_t>(k));
    tab[0] = bm;
    if (W > 1) {
        mt.mul(bm.data(), bm.data(), y.data(), t.data());   // bm^2
        for (size_t i = 1; i < tab.size(); ++i) mt.mul(tab[i - 1].data(), y.data(), tab[i].data(), t.data());
    }
    x = to64(BigInt(1).shl(static_cast<int64_t>(64 * k)) % mod, k);   // 1 in Montgomery form is R mod m
    int64_t i = ebits - 1;
    while (i >= 0) {
        if (!exp.bit(i)) {
            mt.mul(x.data(), x.data(), x.data(), t.data());
            --i;
            continue;
        }
        int64_t l = std::max<int64_t>(i - W + 1, 0);
        while (!exp.bit(l)) ++l;
        uint32_t val = 0;
        for (int64_t j = i; j >= l; --j) val = (val << 1) | (exp.bit(j) ? 1u : 0u);
        for (int64_t j = 0; j < i - l + 1; ++j) mt.mul(x.data(), x.data(), x.data(), t.data());
        mt.mul(x.data(), tab[val >> 1].data(), x.data(), t.data());
        i = l - 1;
    }
    std::vector<uint64_t> onel(k, 0);   // leave Montgomery form: multiply by 1
    onel[0] = 1;
    mt.mul(x.data(), onel.data(), y.data(), t.data());
    BigInt res;
    res.limbs_.assign(k * 2, 0);
    for (size_t j = 0; j < k; ++j) {
        res.limbs_[2 * j] = static_cast<uint32_t>(y[j]);
        res.limbs_[2 * j + 1] = static_cast<uint32_t>(y[j] >> 32);
    }
    res.trim_();
    return res;
}

BigInt BigInt::modinv(const BigInt& base, const BigInt& mod) {
    if (mod.is_zero()) throw py::ValueError("pow() 3rd argument cannot be 0");
    // extended Euclid keeping the coefficients reduced mod `mod` (so they stay non-negative)
    BigInt r0 = mod, r1 = base % mod;
    BigInt t0, t1 = BigInt(1) % mod;
    while (!r1.is_zero()) {
        BigInt q = r0 / r1;
        BigInt r2 = r0 - q * r1;
        BigInt qt = (q * t1) % mod;
        BigInt t2 = (t0 + mod - qt) % mod;
        r0 = r1;
        r1 = r2;
        t0 = t1;
        t1 = t2;
    }
    if (r0.compare(BigInt(1)) != 0) throw py::ValueError("base is not invertible for the given modulus");
    return t0;
}

// =====================================================================================================
// module
// =====================================================================================================

// constants
const py::bytes ALL_55_KEY(std::string(0x10, '\x55'));

const std::optional<py::bytes>& _1BL_KEY() {
    static const std::optional<py::bytes> key = _store_key(py::str("1BL_key.bin"));
    return key;
}

BigInt create_mask(int64_t n) { return BigInt(1).shl(n) - BigInt(1); }


BigInt UINT64_MASK() { return create_mask(64); }
BigInt UINT128_MASK() { return create_mask(128); }

py::str _bin(const py::str& name) {   // upstream's bin/ folder: the xbox360_crypto_bin setting
    return py::os::path::join(py::settings::get_str(py::str("xbox360_crypto_bin")), name);
}

std::optional<py::bytes> _store_key(const py::str& name) {
    // a key from the key store (the xbox360_keys setting), None when not there
    try {
        py::str folder = py::settings::get_str(py::str("xbox360_keys"));
        if (folder.u32().empty()) folder = py::str("Keys");   // (settings.get(...) or "Keys")
        py::file f = py::open_rb(py::os::path::join(folder, name));
        py::bytes data = f.read();
        f.close();
        return data;
    } catch (const py::OSError&) {
        return std::nullopt;
    }
}

py::bytes read_file(const py::str& filename) {
    py::file f = py::open_rb(filename);
    py::bytes data = f.read();
    f.close();
    return data;
}

void write_file(const py::str& filename, const py::bytes& data) {
    py::file f = py::open(filename, py::str("wb"));
    f.write(data);
    f.close();
}

}  // namespace codebreaker::formats::xbox::crypto::MemCrypto
