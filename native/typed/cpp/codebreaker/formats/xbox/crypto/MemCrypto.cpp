// codebreaker/formats/xbox/crypto/MemCrypto.cpp -- C++ port of codebreaker.formats.xbox.crypto.MemCrypto.

#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"

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

// binary long division (in place on limb vectors: the RSA exponentiation runs it thousands of times)
void BigInt::divmod_(const BigInt& a, const BigInt& b, BigInt& q, BigInt& r) {
    if (b.limbs_.empty()) throw py::ZeroDivisionError("integer division or modulo by zero");
    q.limbs_.assign(a.limbs_.size(), 0);
    r.limbs_.assign(b.limbs_.size() + 1, 0);
    const size_t rn = r.limbs_.size();
    for (int64_t i = a.bit_length() - 1; i >= 0; --i) {
        // r = (r << 1) | bit i of a
        uint32_t carry = a.bit(i) ? 1 : 0;
        for (size_t k = 0; k < rn; ++k) {
            uint32_t next = r.limbs_[k] >> 31;
            r.limbs_[k] = (r.limbs_[k] << 1) | carry;
            carry = next;
        }
        // if r >= b: r -= b, set bit i of q
        bool ge = true;
        if (r.limbs_[rn - 1] == 0) {
            for (size_t k = b.limbs_.size(); k-- > 0;) {
                if (r.limbs_[k] != b.limbs_[k]) {
                    ge = r.limbs_[k] > b.limbs_[k];
                    break;
                }
            }
        }
        if (ge) {
            int64_t borrow = 0;
            for (size_t k = 0; k < rn; ++k) {
                int64_t v = static_cast<int64_t>(r.limbs_[k]) - borrow - (k < b.limbs_.size() ? static_cast<int64_t>(b.limbs_[k]) : 0);
                borrow = v < 0 ? 1 : 0;
                if (v < 0) v += (int64_t(1) << 32);
                r.limbs_[k] = static_cast<uint32_t>(v);
            }
            q.limbs_[static_cast<size_t>(i / 32)] |= 1u << (i % 32);
        }
    }
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

BigInt BigInt::powmod(const BigInt& base, const BigInt& exp, const BigInt& mod) {
    if (mod.is_zero()) throw py::ValueError("pow() 3rd argument cannot be 0");
    BigInt result = BigInt(1) % mod;
    BigInt b = base % mod;
    for (int64_t i = exp.bit_length() - 1; i >= 0; --i) {
        result = (result * result) % mod;
        if (exp.bit(i)) result = (result * b) % mod;
    }
    return result;
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
// AES (FIPS 197), ECB
// =====================================================================================================
namespace {

struct AesTables {
    uint8_t sbox[256];
    uint8_t inv[256];
    AesTables() {
        uint8_t p = 1, q = 1;
        do {
            p = static_cast<uint8_t>(p ^ (p << 1) ^ ((p & 0x80) ? 0x1B : 0));
            q ^= static_cast<uint8_t>(q << 1);
            q ^= static_cast<uint8_t>(q << 2);
            q ^= static_cast<uint8_t>(q << 4);
            if (q & 0x80) q ^= 0x09;
            auto rotl = [](uint8_t x, int s) { return static_cast<uint8_t>((x << s) | (x >> (8 - s))); };
            sbox[p] = static_cast<uint8_t>(q ^ rotl(q, 1) ^ rotl(q, 2) ^ rotl(q, 3) ^ rotl(q, 4) ^ 0x63);
        } while (p != 1);
        sbox[0] = 0x63;
        for (int i = 0; i < 256; ++i) inv[sbox[i]] = static_cast<uint8_t>(i);
    }
};
const AesTables& aes_tables() {
    static const AesTables t;
    return t;
}

uint8_t xtime(uint8_t x) { return static_cast<uint8_t>((x << 1) ^ ((x & 0x80) ? 0x1B : 0)); }
uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t r = 0;
    while (b) {
        if (b & 1) r ^= a;
        a = xtime(a);
        b >>= 1;
    }
    return r;
}

void add_round_key(uint8_t* s, const uint8_t* rk) {
    for (int i = 0; i < 16; ++i) s[i] ^= rk[i];
}

}  // namespace

AesEcb::AesEcb(const py::bytes& key, bool decrypt) : decrypt_(decrypt) {
    int64_t klen = key.size();
    if (klen != 16 && klen != 24 && klen != 32) throw py::ValueError("Invalid key size (" + std::to_string(klen) + ") for AES.");
    const AesTables& T = aes_tables();
    int nk = static_cast<int>(klen / 4);
    rounds_ = nk + 6;
    int words = 4 * (rounds_ + 1);
    round_keys_.assign(static_cast<size_t>(words) * 4, 0);
    for (int i = 0; i < nk * 4; ++i) round_keys_[static_cast<size_t>(i)] = static_cast<uint8_t>(key.byte_(i));
    uint8_t rcon = 1;
    for (int i = nk; i < words; ++i) {
        uint8_t t[4];
        std::memcpy(t, &round_keys_[static_cast<size_t>(i - 1) * 4], 4);
        if (i % nk == 0) {
            uint8_t first = t[0];
            t[0] = static_cast<uint8_t>(T.sbox[t[1]] ^ rcon);
            t[1] = T.sbox[t[2]];
            t[2] = T.sbox[t[3]];
            t[3] = T.sbox[first];
            rcon = xtime(rcon);
        } else if (nk > 6 && i % nk == 4) {
            for (auto& x : t) x = T.sbox[x];
        }
        for (int k = 0; k < 4; ++k)
            round_keys_[static_cast<size_t>(i) * 4 + k] = static_cast<uint8_t>(round_keys_[static_cast<size_t>(i - nk) * 4 + k] ^ t[k]);
    }
}

py::bytes AesEcb::update(const py::bytes& data) {
    const AesTables& T = aes_tables();
    pending_ += data.raw();
    size_t whole = pending_.size() / 16 * 16;
    std::string out(whole, '\0');
    for (size_t off = 0; off < whole; off += 16) {
        uint8_t s[16];
        std::memcpy(s, pending_.data() + off, 16);
        if (!decrypt_) {
            add_round_key(s, &round_keys_[0]);
            for (int r = 1; r <= rounds_; ++r) {
                for (auto& x : s) x = T.sbox[x];
                // ShiftRows (the state is column-major: s[4*c + row])
                uint8_t t[16];
                for (int c = 0; c < 4; ++c)
                    for (int row = 0; row < 4; ++row) t[4 * c + row] = s[4 * ((c + row) % 4) + row];
                std::memcpy(s, t, 16);
                if (r != rounds_) {
                    for (int c = 0; c < 4; ++c) {
                        uint8_t* col = s + 4 * c;
                        uint8_t a0 = col[0], a1 = col[1], a2 = col[2], a3 = col[3];
                        col[0] = static_cast<uint8_t>(xtime(a0) ^ (xtime(a1) ^ a1) ^ a2 ^ a3);
                        col[1] = static_cast<uint8_t>(a0 ^ xtime(a1) ^ (xtime(a2) ^ a2) ^ a3);
                        col[2] = static_cast<uint8_t>(a0 ^ a1 ^ xtime(a2) ^ (xtime(a3) ^ a3));
                        col[3] = static_cast<uint8_t>((xtime(a0) ^ a0) ^ a1 ^ a2 ^ xtime(a3));
                    }
                }
                add_round_key(s, &round_keys_[static_cast<size_t>(r) * 16]);
            }
        } else {
            add_round_key(s, &round_keys_[static_cast<size_t>(rounds_) * 16]);
            for (int r = rounds_ - 1; r >= 0; --r) {
                // InvShiftRows
                uint8_t t[16];
                for (int c = 0; c < 4; ++c)
                    for (int row = 0; row < 4; ++row) t[4 * ((c + row) % 4) + row] = s[4 * c + row];
                std::memcpy(s, t, 16);
                for (auto& x : s) x = T.inv[x];
                add_round_key(s, &round_keys_[static_cast<size_t>(r) * 16]);
                if (r != 0) {
                    for (int c = 0; c < 4; ++c) {
                        uint8_t* col = s + 4 * c;
                        uint8_t a0 = col[0], a1 = col[1], a2 = col[2], a3 = col[3];
                        col[0] = static_cast<uint8_t>(gmul(a0, 14) ^ gmul(a1, 11) ^ gmul(a2, 13) ^ gmul(a3, 9));
                        col[1] = static_cast<uint8_t>(gmul(a0, 9) ^ gmul(a1, 14) ^ gmul(a2, 11) ^ gmul(a3, 13));
                        col[2] = static_cast<uint8_t>(gmul(a0, 13) ^ gmul(a1, 9) ^ gmul(a2, 14) ^ gmul(a3, 11));
                        col[3] = static_cast<uint8_t>(gmul(a0, 11) ^ gmul(a1, 13) ^ gmul(a2, 9) ^ gmul(a3, 14));
                    }
                }
            }
        }
        std::memcpy(&out[off], s, 16);
    }
    pending_.erase(0, whole);
    return py::bytes(out);
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

// rsa values
const BigInt MASTER_N = BigInt::from_hex(
    "E1322F1DE92AD64B494455CB05173F6671A964A415536E2B680C40F54FDA808F19B82CD0D7E964B2224C56DE03E2462F946F4FFFAD4588"
    "CF78CEED1CE5FD0F80533AE97043EAD1D12E39880C3CAEEBFDA5ACA3A69445E542EF269D5459952D252945B0169BEF788FB1EAE548AC1A"
    "C3C878899708DE24D1ED04D0555079199527");
const RSAPublicKey MASTER_PUB = RSAPublicKey{BigInt(static_cast<uint64_t>(MASTER_E)), MASTER_N};

BigInt create_mask(int64_t n) { return BigInt(1).shl(n) - BigInt(1); }

// runtime
py::list<int64_t> GF2_TAB;

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

int64_t rotr(int64_t n, int64_t d, int64_t b) {
    // (n >> d) | (n << (b - d)) & eval(f"UINT{b}_MASK")
    int64_t right = py::shr(n, d);   // (ValueError on a negative shift count)
    if (b - d < 0) throw py::ValueError("negative shift count");
    int64_t k = b - d;
    if (b == 8 || b == 16 || b == 32 || b == 36 || b == 64) {
        // the low b bits of n << k are the low b bits of the wrapped 64-bit shift (k < 64 for d >= 1; k may be b)
        uint64_t shifted = k >= 64 ? 0 : (static_cast<uint64_t>(n) << k);
        uint64_t mask = b == 64 ? ~0ULL : ((1ULL << b) - 1);
        uint64_t left = shifted & mask;
        // OR with the (sign-extended) right shift; a non-negative result must stay below 2**63
        uint64_t combined = static_cast<uint64_t>(right) | left;
        if (right >= 0 && (combined >> 63)) throw py::OverflowError("int too large for int64");
        return static_cast<int64_t>(combined);
    }
    if (b == 128) {
        if (n < 0) throw py::OverflowError("int too large for int64");   // (the Python's negative result is wider than int64_t)
        BigInt r = BigInt(static_cast<uint64_t>(right)) | (BigInt(static_cast<uint64_t>(n)).shl(k) & UINT128_MASK());
        return r.to_int64();
    }
    throw NameError("name 'UINT" + std::to_string(b) + "_MASK' is not defined");
}

py::bytes sxor_u32(const py::bytes& s1, const py::bytes& s2) {
    if (s1.size() != s2.size()) throw py::AssertionError("s1 and s2 must be the same size");
    // (the little-endian 32-bit words XORed are the bytes XORed; the chunking only checks the length)
    py::list<py::bytes> a1 = read_chunks(s1, 0, 4);
    py::list<py::bytes> a2 = read_chunks(s2, 0, 4);
    std::string out;
    out.reserve(s1.raw().size());
    for (int64_t i = 0; i < a1.size(); ++i)
        for (int64_t k = 0; k < 4; ++k) out.push_back(static_cast<char>(a1[i].byte_(k) ^ a2[i].byte_(k)));
    return py::bytes(out);
}

py::bytes sand_u32(const py::bytes& s1, const py::bytes& s2) {
    if (s1.size() != s2.size()) throw py::AssertionError("s1 and s2 must be the same size");
    py::list<py::bytes> a1 = read_chunks(s1, 0, 4);
    py::list<py::bytes> a2 = read_chunks(s2, 0, 4);
    std::string out;
    out.reserve(s1.raw().size());
    for (int64_t i = 0; i < a1.size(); ++i)
        for (int64_t k = 0; k < 4; ++k) out.push_back(static_cast<char>(a1[i].byte_(k) & a2[i].byte_(k)));
    return py::bytes(out);
}

py::bytes sxor_b(const py::bytes& s1, const py::bytes& s2) {
    int64_t n = std::min(s1.size(), s2.size());   // (map stops at the shorter)
    std::string out;
    for (int64_t i = 0; i < n; ++i) out.push_back(static_cast<char>(s1.byte_(i) ^ s2.byte_(i)));
    return py::bytes(out);
}

py::bytes read_chunk(const py::bytes& data, int64_t offset, int64_t size) { return data.slice(offset, offset + size); }

py::list<py::bytes> read_chunks(const py::bytes& data, int64_t offset, int64_t size) {
    if (py::mod(data.size() - offset, size) != 0) throw py::AssertionError("data must be evenly divisible by size");
    py::list<py::bytes> out;
    // for i in range(offset, len(data) - offset, size)
    int64_t stop = data.size() - offset;
    if (size > 0) {
        for (int64_t i = offset; i < stop; i += size) out.append(data.slice(i, i + size));
    } else {   // (size is nonzero here: the modulo above raised for 0)
        for (int64_t i = offset; i > stop; i += size) out.append(data.slice(i, i + size));
    }
    return out;
}

py::bytes rsa_encrypt(const RSAPublicKey& key, const py::bytes& data) {
    // pow(int.from_bytes(data, "little"), e, n).to_bytes(key_size // 8, "big")
    BigInt m = BigInt::from_bytes(data, "little");
    return BigInt::powmod(m, key.e, key.n).to_bytes(py::floordiv(key.key_size(), 8), "big");
}

py::list<int64_t> generate_gf2_table(int64_t iv, int64_t poly) {
    py::list<int64_t> tab;
    for (int i = 0; i < 256; ++i) tab.append(0);
    for (int64_t i = 0; i < 256; ++i) {
        int64_t crc = iv;
        int64_t c = i << 8;
        for (int j = 0; j < 8; ++j) {
            if ((crc ^ c) & 0x8000) crc = py::shl(crc, 1) ^ poly;
            else crc = py::shl(crc, 1);
            c <<= 1;
        }
        tab[i] = crc & UINT16_MASK;
    }
    return tab;
}

py::bytearray pack_sec_eng_keys(const py::bytes& key, py::bytearray buffer, int64_t offset, int64_t length) {
    int64_t cycle = 0;
    while (length > 0) {
        py::hashlib::Hash h = py::hashlib::sha1();
        h.update(key);
        h.update(py::to_bytes(cycle, 4, "big"));
        py::bytes digest = h.digest();

        int64_t sublen = 0x14;
        if (length < 0x14) sublen = length;

        buffer.set_slice(offset, offset + sublen, sxor_b(read_chunk(buffer, offset, sublen), digest));

        offset += sublen;
        length -= sublen;
        cycle += 1;
    }
    return buffer;
}

// ---- MemoryCrypto -----------------------------------------------------------------------------------
MemoryCrypto::MemoryCrypto(const py::bytes& wkey, const py::bytes& akey, const py::bytes& hkey) {
    reset();

    white_key = wkey;
    aes_key = akey;
    hash_key = hkey;

    // c = Cipher(AES(akey), ECB()); aesdec = c.decryptor(); aesenc = c.encryptor()
    aesdec.emplace(akey, true);
    aesenc.emplace(akey, false);
}

MemoryCrypto& MemoryCrypto::__enter__() { return *this; }

void MemoryCrypto::__exit__() {}

void MemoryCrypto::reset() {
    aesdec.reset();
    aesenc.reset();
    white_key = py::bytes();
    aes_key = py::bytes();
    hash_key = py::bytes();
}

int64_t MemoryCrypto::sram_offset_to_hv_offset(int64_t sram_offset) {
    return py::floordiv(sram_offset, 2) * SRAM_CKSM_PAGE_SIZE;
}

int64_t MemoryCrypto::sram_size_to_hv_size(int64_t sram_size) { return sram_offset_to_hv_offset(sram_size); }

BigInt MemoryCrypto::get_tweak_1(const BigInt& n_in) {
    BigInt n = n_in;
    BigInt of = n.shr(128).masked(36);
    n = n.masked(128);

    // checksum the last 36 bits and XOR them into the first 128 bits
    int64_t i = 0;
    while (!of.is_zero()) {
        int64_t idx = of.low_byte();
        n = n ^ BigInt::from_signed_masked(GF2_TAB[idx], 128 + 64).shl(i * 8);   // (GF2_TAB[idx] << (i * 8), kept to the bits that survive & UINT128_MASK below)
        of = of.shr(8);
        i += 1;
    }
    return n.masked(128);
}

py::bytes MemoryCrypto::get_tweak_0(const std::optional<int64_t>& address_in) {
    BigInt key = BigInt::from_bytes(white_key, "big");
    // create space for 36 bits
    BigInt value = key.shl(36);

    // make address 32 bits (normally 36 bits)
    if (!address_in) throw py::TypeError("unsupported operand type(s) for >>=: 'NoneType' and 'int'");
    int64_t address = py::shr(*address_in, 4);

    // loop through all bits in the address and
    // transform the value only if the bit is 1
    int64_t bits = py::bit_length(address);
    for (int64_t i = 0; i < bits; ++i) {
        if (py::shr(address, i) & 1) value = value ^ key.shl(i);
    }

    return get_tweak_1(value).to_bytes(16, "big");
}

int64_t MemoryCrypto::fix_address(int64_t address) {
    if (0 <= address && address <= 0x40000) {   // in HV range
        return address | (int64_t(0x200000000) * py::floordiv(address, 0x10000));
    } else {   // outside HV range
        return address;
    }
}

py::bytes MemoryCrypto::encrypt_block(const py::bytes& dec_data_in, int64_t offset, int64_t size, std::optional<int64_t> address,
                                      bool offset_is_address) {
    if (!address && offset_is_address) address = offset;

    py::bytes dec_data = read_chunk(dec_data_in, offset, size);

    // calculate tweak key
    py::bytes tweak = get_tweak_0(address);
    // apply tweak before
    py::bytes enc_data = sxor_u32(dec_data, tweak);
    // "encrypt" memory
    if (!aesdec) throw py::AttributeError("'NoneType' object has no attribute 'update'");
    enc_data = aesdec->update(enc_data);
    // apply tweak after
    enc_data = sxor_u32(enc_data, tweak);

    return enc_data;
}

py::bytes MemoryCrypto::decrypt_block(const py::bytes& enc_data_in, int64_t offset, int64_t size, std::optional<int64_t> address,
                                      bool offset_is_address) {
    if (!address && offset_is_address) address = offset;

    py::bytes enc_data = read_chunk(enc_data_in, offset, size);

    // calculate tweak key
    py::bytes tweak = get_tweak_0(address);
    // apply tweak before
    py::bytes dec_data = sxor_u32(enc_data, tweak);
    // "decrypt" memory
    if (!aesenc) throw py::AttributeError("'NoneType' object has no attribute 'update'");
    dec_data = aesenc->update(dec_data);
    // apply tweak after
    dec_data = sxor_u32(dec_data, tweak);

    return dec_data;
}

py::bytes MemoryCrypto::encrypt(const py::bytes& hv_data_dec_in, int64_t offset, int64_t size, std::optional<int64_t> address,
                                bool offset_is_address) {
    if (!address && offset_is_address) address = offset;

    if (py::mod(size, 16) != 0) throw py::AssertionError("Size must be divisible by the AES block size (16 bytes)");

    py::bytes hv_data_dec = read_chunk(hv_data_dec_in, offset, size);

    std::string out;
    // for i in range(0, size, 16)
    for (int64_t i = 0; i < size; i += 16) {
        if (!address) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
        out += encrypt_block(hv_data_dec, i, 16, fix_address(*address + i), offset_is_address).raw();
    }
    return py::bytes(out);
}

py::bytes MemoryCrypto::decrypt(const py::bytes& hv_data_enc_in, int64_t offset, int64_t size, std::optional<int64_t> address,
                                bool offset_is_address) {
    if (!address && offset_is_address) address = offset;

    if (py::mod(size, 16) != 0) throw py::AssertionError("Size must be divisible by the AES block size (16 bytes)");

    py::bytes hv_data_enc = read_chunk(hv_data_enc_in, offset, size);

    std::string out;
    for (int64_t i = 0; i < size; i += 16) {
        if (!address) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
        out += decrypt_block(hv_data_enc, i, 16, fix_address(*address + i), offset_is_address).raw();
    }
    return py::bytes(out);
}

py::bytes MemoryCrypto::encrypt_and_calc_checksums(const py::bytes& hv_data_dec_in, int64_t offset, int64_t size,
                                                   std::optional<int64_t> address, bool offset_is_address) {
    if (!address && offset_is_address) address = offset;

    py::bytes hv_data_dec = read_chunk(hv_data_dec_in, offset, size);
    py::bytes hv_data_enc = encrypt(hv_data_dec, 0, size, address, offset_is_address);

    return calc_sram_checksums(hv_data_dec, hv_data_enc, 0, size);
}

int64_t MemoryCrypto::calc_sram_checksum(const py::bytes& data) {
    int64_t cksm = 0;
    int64_t rot_val = 1;
    for (int64_t i = 0; i < py::floordiv(data.size(), 2); ++i) {
        int64_t v = py::from_bytes(data.slice(i * 2, i * 2 + 2), "big", false);
        cksm ^= rotr(v, rot_val, 16);
        rot_val = py::floordiv(i + 1, 4) + 1;
    }
    return cksm & UINT16_MASK;
}

py::bytes MemoryCrypto::calc_sram_checksums(const py::bytes& hv_data_dec_in, const py::bytes& hv_data_enc_in, int64_t offset,
                                            int64_t size) {
    if (py::mod(size, SRAM_CKSM_PAGE_SIZE) != 0) throw py::AssertionError("Hashes require data to be evenly divisible by 0x80");

    py::bytes hv_data_dec = read_chunk(hv_data_dec_in, offset, size);
    py::bytes hv_data_enc = read_chunk(hv_data_enc_in, offset, size);

    // mask = self.hash_key * (len(hv_data_dec) // 0x10)
    std::string mask_raw;
    for (int64_t i = 0; i < py::floordiv(hv_data_dec.size(), 0x10); ++i) mask_raw += hash_key.raw();
    py::bytes mask(mask_raw);
    py::bytes masked = sand_u32(hv_data_enc, mask);
    hv_data_dec = sxor_u32(masked, hv_data_dec);

    int64_t num_cksm_pages = py::floordiv(size, SRAM_CKSM_PAGE_SIZE);
    std::string out;
    for (int64_t i = 0; i < num_cksm_pages; ++i) {
        int64_t c = calc_sram_checksum(hv_data_dec.slice(i * SRAM_CKSM_PAGE_SIZE, i * SRAM_CKSM_PAGE_SIZE + SRAM_CKSM_PAGE_SIZE));
        out += py::to_bytes(c, 2, "big", false).raw();
    }
    return py::bytes(out);
}

py::bytes MemoryCrypto::get_checksum_chunk_by_sram_offset_and_size(const py::bytes& hv_data_dec, const py::bytes& hv_data_enc,
                                                                   int64_t sram_offset, int64_t sram_size) {
    int64_t hv_offs = sram_offset_to_hv_offset(sram_offset);
    int64_t hv_size = sram_size_to_hv_size(sram_size);
    return calc_sram_checksums(hv_data_dec, hv_data_enc, hv_offs, hv_size);
}

py::bytes MemoryCrypto::calc_sram(const py::bytes& hv_data_dec) {
    py::bytes hv_data_enc = encrypt(hv_data_dec, 0, 0x40000, 0);
    return calc_sram_checksums(hv_data_dec, hv_data_enc, 0, 0x40000);
}

py::bytes MemoryCrypto::calc_hash_1_digest(const py::bytes& hv_data_dec, const py::bytes& salt_in) {
    py::bytes salt = salt_in.slice(0, 0x10);   // ensure salt is 0x10 in length

    py::hashlib::Hash h = py::hashlib::sha1();
    h.update(salt);
    h.update(read_chunk(hv_data_dec, 0x34, 0x40));
    h.update(read_chunk(hv_data_dec, 0x78, 0xFF88));
    h.update(read_chunk(hv_data_dec, 0x100C0, 0x40));
    h.update(read_chunk(hv_data_dec, 0x10350, 0x5F70));
    h.update(read_chunk(hv_data_dec, 0x16EA0, 0x9160));
    h.update(read_chunk(hv_data_dec, 0x20000, 0xFFFF));
    h.update(read_chunk(hv_data_dec, 0x30000, 0xFFFF));
    return read_chunk(h.digest(), 0xE, 6);
}

py::bytes MemoryCrypto::calc_hash_2_digest(const py::bytes& hv_data_dec, const py::bytes& salt_in, int64_t hvex_addr) {
    py::bytes salt = salt_in.slice(0, 0x10);   // ensure salt is 0x10 in length

    // HVEX_ADDR = 0x1B9

    std::string salt8;
    for (int i = 0; i < 8; ++i) salt8 += salt.raw();
    py::bytes hv_salt_dec(salt8);
    int64_t hv_hash_addr = py::shl(hvex_addr, 0x10) | (int64_t(0x7C00000000) + 0x400);

    py::hashlib::Hash h = py::hashlib::sha1();
    h.update(encrypt_and_calc_checksums(hv_salt_dec, 0, hv_salt_dec.size(), hv_hash_addr));
    h.update(read_chunk(hv_data_dec, 0x34, 0xC));
    h.update(encrypt(hv_data_dec, 0x40, 0x30, std::nullopt, true));
    h.update(read_chunk(hv_data_dec, 0x70, 4));
    h.update(read_chunk(hv_data_dec, 0x78, 8));
    h.update(encrypt_and_calc_checksums(hv_data_dec, 0x80, 0xFF80, std::nullopt, true));
    h.update(encrypt(hv_data_dec, 0x100C0, 0x40, std::nullopt, true));
    h.update(encrypt(hv_data_dec, 0x10350, 0x30, std::nullopt, true));
    h.update(encrypt_and_calc_checksums(hv_data_dec, 0x10380, 0x5F00, std::nullopt, true));
    h.update(encrypt(hv_data_dec, 0x16280, 0x40, std::nullopt, true));
    h.update(encrypt(hv_data_dec, 0x16EA0, 0x60, std::nullopt, true));
    h.update(encrypt_and_calc_checksums(hv_data_dec, 0x16F00, 0x9100, std::nullopt, true));
    h.update(encrypt_and_calc_checksums(hv_data_dec, 0x20000, 0x10000, std::nullopt, true));
    h.update(encrypt_and_calc_checksums(hv_data_dec, 0x30000, 0x10000, std::nullopt, true));
    return h.digest();
}

py::bytes MemoryCrypto::calc_key_blob(const py::bytes& blob_nonce) {
    py::bytearray key_blob = py::bytearray::zeros(0x80);
    key_blob.set_slice(1, 1 + 20, blob_nonce);
    key_blob.set_slice(0x15, 0x15 + 20, py::bytes::fromhex(py::str("DA39A3EE5E6B4B0D3255BFEF95601890AFD80709")));
    key_blob[0x4F] = 1;
    key_blob.set_slice(0x50, 0x50 + 0x10, white_key);
    key_blob.set_slice(0x60, 0x60 + 0x10, aes_key);
    key_blob.set_slice(0x70, 0x70 + 0x10, hash_key);
    key_blob = pack_sec_eng_keys(key_blob.slice(0x1, 0x15), key_blob, 0x15, 0x6B);
    key_blob = pack_sec_eng_keys(key_blob.slice(0x15, 0x15 + 0x6B), key_blob, 1, 0x14);
    return rsa_encrypt(MASTER_PUB, key_blob);
}

py::bytes MemoryCrypto::calc_100f0(const py::bytes& hv_data_dec, int64_t tbl_addr) {
    // encrypt the clean decrypted HV
    py::bytes hv_data_enc = encrypt(hv_data_dec, 0, 0x40000, 0);

    // create SHA1 hasher
    py::hashlib::Hash h = py::hashlib::sha1();
    for (int64_t i = 0; i < 6; ++i) {
        // grab unaligned addresses and sizes from the HV

        // (u_strt_addr, u_stop_addr) = unpack_from(">2I", hv_data_dec, tbl_addr + (i * 8))
        int64_t o = tbl_addr + (i * 8);
        int64_t u_strt_addr = py::from_bytes(hv_data_dec.slice(o, o + 4), "big", false);
        o += 4;
        int64_t u_stop_addr = py::from_bytes(hv_data_dec.slice(o, o + 4), "big", false);

        // 0x80 align address and size
        int64_t a_strt_addr = (u_strt_addr + 0x7F) & int64_t(-0x80);   // (& 0xFFFFFFFFFFFFFF80)
        int64_t a_stop_addr = u_stop_addr & 0xFFFFFF80;

        if (a_strt_addr < a_stop_addr) {
            // print(f"RAM:  0x{a_strt_addr:X} - 0x{a_stop_addr:X}")

            int64_t sram_offs = py::floordiv(a_strt_addr, SRAM_CKSM_PAGE_SIZE) * 2;
            int64_t sram_size = py::floordiv(a_stop_addr - a_strt_addr, SRAM_CKSM_PAGE_SIZE) * 2;

            // print(f"SRAM: 0x{sram_offs:X} - 0x{sram_offs + sram_size:X}")

            h.update(get_checksum_chunk_by_sram_offset_and_size(hv_data_dec, hv_data_enc, sram_offs, sram_size));
        }
    }

    return h.digest().slice(0, 0x10);
}

// ---- main -------------------------------------------------------------------------------------------
namespace {

std::string hex_upper(const py::bytes& b) {
    std::string h = b.hex().utf8();
    for (auto& c : h) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return h;
}

void say(const std::string& s) { py::print(py::str(s)); }

}  // namespace

int64_t main(const py::list<py::str>& argv, const py::str& prog) {
    GF2_TAB = generate_gf2_table(GF2_IV, GF2_POLY);

    // parser = ArgumentParser(description=...); subparsers "calculate" and "test" (dest="mode")
    const std::string p = prog.utf8();
    const std::string usage = "usage: " + p + " [-h] {calculate,test} ...";
    std::optional<std::string> mode;
    for (int64_t i = 0; i < argv.size(); ++i) {
        std::string a = argv[i].utf8();
        if (!mode) {
            if (a == "-h" || a == "--help") {
                say(usage);
                say("");
                say("A script to calculate Xbox 360 challenge responses for XOSC");
                say("");
                say("positional arguments:");
                say("  {calculate,test}");
                say("");
                say("options:");
                say("  -h, --help        show this help message and exit");
                throw SystemExit(0);
            }
            if (a == "calculate" || a == "test") {
                mode = a;
                continue;
            }
            if (!a.empty() && a[0] == '-' && a.size() > 1) {
                std::cerr << usage << "\n" << p << ": error: unrecognized arguments: " << a << "\n";
                throw SystemExit(2);
            }
            std::cerr << usage << "\n"
                      << p << ": error: argument mode: invalid choice: " << py::repr(argv[i]).utf8()
                      << " (choose from calculate, test)\n";
            throw SystemExit(2);
        } else {
            if (a == "-h" || a == "--help") {
                say("usage: " + p + " " + *mode + " [-h]");
                say("");
                say("options:");
                say("  -h, --help  show this help message and exit");
                throw SystemExit(0);
            }
            std::string rest;
            for (int64_t j = i; j < argv.size(); ++j) rest += (j > i ? " " : "") + argv[j].utf8();
            std::cerr << usage << "\n" << p << ": error: unrecognized arguments: " << rest << "\n";
            throw SystemExit(2);
        }
    }

    if (mode && *mode == "calculate") {
        py::bytes hv_data_dec = read_file(_bin(py::str("HV_17559_Cleaned.bin")));

        py::bytes white_key = ALL_55_KEY;
        py::bytes aes_key = ALL_55_KEY;
        py::bytes hash_key = ALL_55_KEY;

        say("W: " + hex_upper(white_key));
        say("A: " + hex_upper(aes_key));
        say("H: " + hex_upper(hash_key));
        say("");

        py::bytes blob_nonce("testtest");

        int64_t hvex_addr = 0xB00B;
        py::bytes hv_salt = py::bytes::fromhex(py::str("892BB9F952C7759392A12A3184E0358E"));
        white_key = ALL_55_KEY;
        aes_key = ALL_55_KEY;
        hash_key = ALL_55_KEY;

        say("W: " + hex_upper(white_key));
        say("A: " + hex_upper(aes_key));
        say("H: " + hex_upper(hash_key));
        say("");

        {
            MemoryCrypto mem(white_key, aes_key, hash_key);
            say("Hash 1: " + hex_upper(mem.calc_hash_1_digest(hv_data_dec, hv_salt)));
        }
        say("");

        {
            MemoryCrypto mem(white_key, aes_key, hash_key);
            say("Hash 2: " + hex_upper(mem.calc_hash_2_digest(hv_data_dec, hv_salt, hvex_addr)));
        }
        say("");

        {
            MemoryCrypto mem(white_key, aes_key, hash_key);
            say("Key blob: " + hex_upper(mem.calc_key_blob(blob_nonce)));
        }
        say("");
    } else if (mode && *mode == "test") {
        py::bytes keys = read_file(_bin(py::str("keys.bin")));
        py::bytes white_key = keys.slice(0, 0x10);
        py::bytes aes_key = keys.slice(0x10, 0x10 + 0x10);
        py::bytes hash_key = keys.slice(-0x10, py::none);

        py::bytes hv_enc = read_file(_bin(py::str("HV.enc.bin")));
        MemoryCrypto mem(white_key, aes_key, hash_key);
        write_file(py::str("HV.dec.bin"), mem.decrypt(hv_enc, 0, hv_enc.size(), std::nullopt, true));
    }

    say("Done!");

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::MemCrypto
