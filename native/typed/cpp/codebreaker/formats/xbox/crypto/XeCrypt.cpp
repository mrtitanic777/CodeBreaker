// codebreaker/formats/xbox/crypto/XeCrypt.cpp -- C++ port of codebreaker.formats.xbox.crypto.XeCrypt.

#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <random>
#include <string>

namespace codebreaker::formats::xbox::crypto::XeCrypt {

// =====================================================================================================
// Helpers the runtime does not have (written once, here): Python's random.Random (Mersenne Twister), RC4,
// DES / TripleDES, a generic HMAC, the block-mode (ECB / CBC) streaming context, a prime generator.
// AES-ECB, SHA-1, MD5 and the big integers come from MemCrypto and py::hashlib.
// =====================================================================================================
namespace detail {

// ---- random.Random: MT19937 seeded and drawn the way CPython does ------------------------------------
class MersenneTwister {
public:
    MersenneTwister() {   // random.Random() with no seed: seeded from the OS
        std::random_device rd;
        std::vector<uint32_t> key(624);
        for (auto& k : key) k = rd();
        init_by_array(key);
    }
    // random.seed(a) for an int a: init_by_array over |a| in 32-bit little-endian words (one word for 0)
    void seed(int64_t a) {
        uint64_t v = a < 0 ? 0 - static_cast<uint64_t>(a) : static_cast<uint64_t>(a);
        std::vector<uint32_t> key;
        key.push_back(static_cast<uint32_t>(v & 0xFFFFFFFFu));
        if (v >> 32) key.push_back(static_cast<uint32_t>(v >> 32));
        init_by_array(key);
    }
    uint32_t genrand() {
        if (index_ >= 624) twist_();
        uint32_t y = mt_[index_++];
        y ^= y >> 11;
        y ^= (y << 7) & 0x9D2C5680u;
        y ^= (y << 15) & 0xEFC60000u;
        y ^= y >> 18;
        return y;
    }
    // random.randbytes(n) = getrandbits(n * 8).to_bytes(n, "little")
    std::string randbytes(int64_t n) {
        if (n < 0) throw py::ValueError("number of bits must be non-negative");
        std::string out;
        int64_t k = n * 8;
        if (k == 0) return out;
        if (k <= 32) {
            uint32_t r = genrand() >> (32 - k);
            for (int i = 0; i < 4; ++i) out.push_back(static_cast<char>((r >> (8 * i)) & 0xFF));
        } else {
            while (k > 0) {
                uint32_t r = genrand();
                if (k < 32) r >>= (32 - k);
                for (int i = 0; i < 4; ++i) out.push_back(static_cast<char>((r >> (8 * i)) & 0xFF));
                k -= 32;
            }
        }
        out.resize(static_cast<size_t>(n));
        return out;
    }

private:
    uint32_t mt_[624];
    int index_ = 624;
    void init_genrand(uint32_t s) {
        mt_[0] = s;
        for (int i = 1; i < 624; ++i) mt_[i] = 1812433253u * (mt_[i - 1] ^ (mt_[i - 1] >> 30)) + static_cast<uint32_t>(i);
        index_ = 624;
    }
    void init_by_array(const std::vector<uint32_t>& key) {
        init_genrand(19650218u);
        size_t i = 1, j = 0;
        size_t klen = key.size();
        for (size_t k = std::max<size_t>(624, klen); k; --k) {
            mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1664525u)) + key[j] + static_cast<uint32_t>(j);
            ++i;
            ++j;
            if (i >= 624) { mt_[0] = mt_[623]; i = 1; }
            if (j >= klen) j = 0;
        }
        for (size_t k = 623; k; --k) {
            mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1566083941u)) - static_cast<uint32_t>(i);
            ++i;
            if (i >= 624) { mt_[0] = mt_[623]; i = 1; }
        }
        mt_[0] = 0x80000000u;
        index_ = 624;
    }
    void twist_() {
        for (int i = 0; i < 624; ++i) {
            uint32_t y = (mt_[i] & 0x80000000u) | (mt_[(i + 1) % 624] & 0x7FFFFFFFu);
            mt_[i] = mt_[(i + 397) % 624] ^ (y >> 1) ^ ((y & 1) ? 0x9908B0DFu : 0);
        }
        index_ = 0;
    }
};

MersenneTwister& rng() {
    static MersenneTwister r;
    return r;
}

// ---- RC4 (cryptography's ARC4: a stream; the key sizes OpenSSL accepts) ----------------------------------
class Rc4State {
public:
    explicit Rc4State(const std::string& key) {
        static const int64_t sizes[] = {5, 7, 8, 10, 16, 20, 24, 32};
        int64_t n = static_cast<int64_t>(key.size());
        if (std::find(std::begin(sizes), std::end(sizes), n) == std::end(sizes))
            throw py::ValueError("Invalid key size (" + std::to_string(n * 8) + ") for RC4.");
        for (int i = 0; i < 256; ++i) s_[i] = static_cast<uint8_t>(i);
        int j = 0;
        for (int i = 0; i < 256; ++i) {
            j = (j + s_[i] + static_cast<uint8_t>(key[static_cast<size_t>(i) % key.size()])) & 0xFF;
            std::swap(s_[i], s_[j]);
        }
    }
    std::string update(const std::string& data) {
        std::string out(data.size(), '\0');
        for (size_t k = 0; k < data.size(); ++k) {
            i_ = (i_ + 1) & 0xFF;
            j_ = (j_ + s_[i_]) & 0xFF;
            std::swap(s_[i_], s_[j_]);
            out[k] = static_cast<char>(static_cast<uint8_t>(data[k]) ^ s_[(s_[i_] + s_[j_]) & 0xFF]);
        }
        return out;
    }

private:
    uint8_t s_[256];
    int i_ = 0, j_ = 0;
};

// ---- DES (FIPS 46-3); TripleDES is encrypt-decrypt-encrypt under three schedules ---------------------------
const uint8_t IP[64] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22,
                        14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35,
                        27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
const uint8_t FP[64] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22,
                        62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11,
                        51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
const uint8_t EXPAND[48] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                            16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
const uint8_t PBOX[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
                          2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};
const uint8_t PC1[56] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
                         63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};
const uint8_t PC2[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
                         41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
const uint8_t SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
const uint8_t SBOX[8][64] = {
    {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
     4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
    {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
     0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
    {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
     13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
    {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
     10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
    {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
     4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
    {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
     9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
    {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
     1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
    {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
     7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};

// the n_out bits `table` names (1-based from the top of an n_in-bit value), as an n_out-bit value
uint64_t permute(uint64_t value, const uint8_t* table, int n_out, int n_in) {
    uint64_t out = 0;
    for (int i = 0; i < n_out; ++i) out = (out << 1) | ((value >> (n_in - table[i])) & 1);
    return out;
}

uint64_t load_be64(const uint8_t* p) {
    uint64_t v = 0;
    for (int i = 0; i < 8; ++i) v = (v << 8) | p[i];
    return v;
}
void store_be64(uint8_t* p, uint64_t v) {
    for (int i = 7; i >= 0; --i) { p[i] = static_cast<uint8_t>(v & 0xFF); v >>= 8; }
}

class DesSchedule {
public:
    explicit DesSchedule(const uint8_t* key8) {   // the parity bits are ignored
        uint64_t k = permute(load_be64(key8), PC1, 56, 64);
        uint32_t c = static_cast<uint32_t>(k >> 28) & 0xFFFFFFF, d = static_cast<uint32_t>(k) & 0xFFFFFFF;
        for (int r = 0; r < 16; ++r) {
            for (int s = 0; s < SHIFTS[r]; ++s) {
                c = ((c << 1) | (c >> 27)) & 0xFFFFFFF;
                d = ((d << 1) | (d >> 27)) & 0xFFFFFFF;
            }
            sub_[r] = permute((static_cast<uint64_t>(c) << 28) | d, PC2, 48, 56);
        }
    }
    uint64_t crypt(uint64_t block, bool decrypt) const {
        uint64_t state = permute(block, IP, 64, 64);
        uint32_t left = static_cast<uint32_t>(state >> 32), right = static_cast<uint32_t>(state);
        for (int r = 0; r < 16; ++r) {
            uint64_t x = permute(right, EXPAND, 48, 32) ^ sub_[decrypt ? 15 - r : r];
            uint64_t sboxed = 0;
            for (int i = 0; i < 8; ++i) {
                unsigned six = static_cast<unsigned>((x >> (42 - 6 * i)) & 0x3F);
                unsigned row = ((six >> 4) & 2) | (six & 1), col = (six >> 1) & 0xF;
                sboxed = (sboxed << 4) | SBOX[i][row * 16 + col];
            }
            uint32_t f = static_cast<uint32_t>(permute(sboxed, PBOX, 32, 32));
            uint32_t next = left ^ f;
            left = right;
            right = next;
        }
        return permute((static_cast<uint64_t>(right) << 32) | left, FP, 64, 64);
    }

private:
    uint64_t sub_[16];
};

// ---- HMAC (RFC 2104) over a 64-byte-block hash --------------------------------------------------------
py::bytes hmac(const std::function<py::bytes(const py::bytes&)>& hash, const py::bytes& key_in, const py::bytes& msg) {
    const int64_t block = 64;
    py::bytes key = key_in;
    if (py::len(key) > block) key = hash(key);
    key = key + py::bytes::zeros(block - py::len(key));
    std::string inner_pad, outer_pad;
    for (int64_t i = 0; i < block; ++i) {
        inner_pad.push_back(static_cast<char>(key[i] ^ 0x36));
        outer_pad.push_back(static_cast<char>(key[i] ^ 0x5C));
    }
    return hash(py::bytes(outer_pad) + hash(py::bytes(inner_pad) + msg));
}

py::bytes sha1_of(const py::bytes& b) { return py::hashlib::sha1(b).digest(); }
py::bytes md5_of(const py::bytes& b) { return py::hashlib::md5(b).digest(); }

py::bytes join_(const py::list<py::bytes>& parts) {
    std::string s;
    for (const auto& p : parts.vec()) s += p.raw();
    return py::bytes(s);
}

// ---- a block cipher in ECB or CBC, as cryptography's streaming encryptor / decryptor ---------------------
class BlockModeContext {
public:
    using BlockFn = std::function<void(uint8_t* block)>;   // transforms one block in place
    BlockModeContext(size_t block_size, BlockFn fn, bool decrypt, bool cbc, std::string iv)
        : bs_(block_size), fn_(std::move(fn)), decrypt_(decrypt), cbc_(cbc), prev_(std::move(iv)) {}
    // the blocks completed so far; a partial block waits for the next call
    py::bytes update(const py::bytes& data) {
        pending_ += data.raw();
        std::string out;
        size_t whole = pending_.size() / bs_ * bs_;
        for (size_t off = 0; off < whole; off += bs_) {
            std::string blk = pending_.substr(off, bs_);
            auto* b = reinterpret_cast<uint8_t*>(&blk[0]);
            if (!cbc_) {
                fn_(b);
            } else if (!decrypt_) {
                for (size_t i = 0; i < bs_; ++i) b[i] ^= static_cast<uint8_t>(prev_[i]);
                fn_(b);
                prev_ = blk;
            } else {
                std::string cipher = blk;
                fn_(b);
                for (size_t i = 0; i < bs_; ++i) b[i] ^= static_cast<uint8_t>(prev_[i]);
                prev_ = cipher;
            }
            out += blk;
        }
        pending_.erase(0, whole);
        return py::bytes(out);
    }

private:
    size_t bs_;
    BlockFn fn_;
    bool decrypt_;
    bool cbc_;
    std::string prev_;   // CBC: the previous ciphertext block (the IV first)
    std::string pending_;
};

// the mode argument shared by the AES and DES classes; returns whether it is CBC
bool check_mode_(std::optional<int64_t> mode, const std::optional<py::bytes>& iv, int64_t iv_size) {
    if (mode.has_value() && *mode == 1) return false;
    if (mode.has_value() && *mode == 2) {
        if (!iv.has_value()) throw py::AssertionError("IV is required for the CBC cipher mode");
        if (py::len(*iv) != iv_size)
            throw py::ValueError("Invalid IV size (" + std::to_string(py::len(*iv)) + ") for CBC.");
        return true;
    }
    throw py::Exception("Invalid cipher mode entered");
}

// ---- a prime generator for the RSA key generation ----------------------------------------------------
class SecureRandom {
public:
    std::string bytes(size_t n) {
        std::string s;
        while (s.size() < n) {
            uint32_t r = rd_();
            for (int i = 0; i < 4 && s.size() < n; ++i) s.push_back(static_cast<char>((r >> (8 * i)) & 0xFF));
        }
        return s;
    }

private:
    std::random_device rd_;
};

// base ** exp % mod for an odd mod > 1, by Montgomery multiplication on 32-bit limbs (BigInt::powmod divides
// after every product, which is too slow for the thousands of exponentiations a prime search makes)
BigInt montgomery_powmod(const BigInt& base, const BigInt& exp, const BigInt& mod) {
    auto limbs_of = [](const BigInt& v, size_t k) {
        std::string raw = v.to_bytes(static_cast<int64_t>(k) * 4, "little").raw();
        std::vector<uint32_t> out(k);
        for (size_t i = 0; i < k; ++i) std::memcpy(&out[i], raw.data() + i * 4, 4);   // (little-endian host)
        return out;
    };
    auto from_limbs = [](const std::vector<uint32_t>& l) {
        std::string raw(l.size() * 4, '\0');
        std::memcpy(&raw[0], l.data(), raw.size());
        return BigInt::from_bytes(py::bytes(raw), "little");
    };
    const size_t k = static_cast<size_t>((mod.bit_length() + 31) / 32);
    std::vector<uint32_t> m = limbs_of(mod, k);
    uint32_t inv = 1;   // m[0]^-1 mod 2^32 by Newton's iteration
    for (int i = 0; i < 5; ++i) inv *= 2 - m[0] * inv;
    const uint32_t n0inv = 0 - inv;

    auto mul = [&](const std::vector<uint32_t>& a, const std::vector<uint32_t>& b) {
        std::vector<uint32_t> t(k + 2, 0);
        for (size_t i = 0; i < k; ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < k; ++j) {
                uint64_t s = static_cast<uint64_t>(t[j]) + static_cast<uint64_t>(a[j]) * b[i] + carry;
                t[j] = static_cast<uint32_t>(s);
                carry = s >> 32;
            }
            uint64_t s = static_cast<uint64_t>(t[k]) + carry;
            t[k] = static_cast<uint32_t>(s);
            t[k + 1] = static_cast<uint32_t>(s >> 32);
            uint32_t q = t[0] * n0inv;
            s = static_cast<uint64_t>(t[0]) + static_cast<uint64_t>(q) * m[0];
            carry = s >> 32;
            for (size_t j = 1; j < k; ++j) {
                s = static_cast<uint64_t>(t[j]) + static_cast<uint64_t>(q) * m[j] + carry;
                t[j - 1] = static_cast<uint32_t>(s);
                carry = s >> 32;
            }
            s = static_cast<uint64_t>(t[k]) + carry;
            t[k - 1] = static_cast<uint32_t>(s);
            t[k] = t[k + 1] + static_cast<uint32_t>(s >> 32);
        }
        bool ge = t[k] != 0;
        if (!ge) {
            ge = true;
            for (size_t i = k; i-- > 0;) {
                if (t[i] != m[i]) { ge = t[i] > m[i]; break; }
            }
        }
        if (ge) {
            int64_t borrow = 0;
            for (size_t i = 0; i < k; ++i) {
                int64_t d = static_cast<int64_t>(t[i]) - m[i] - borrow;
                borrow = d < 0;
                t[i] = static_cast<uint32_t>(d);
            }
        }
        t.resize(k);
        return t;
    };

    BigInt r_mod = BigInt(1).shl(static_cast<int64_t>(k) * 32) % mod;   // R mod m: 1 in Montgomery form
    std::vector<uint32_t> x = limbs_of(r_mod, k);
    std::vector<uint32_t> b = limbs_of((base % mod).shl(static_cast<int64_t>(k) * 32) % mod, k);
    for (int64_t i = exp.bit_length() - 1; i >= 0; --i) {
        x = mul(x, x);
        if (exp.bit(i)) x = mul(x, b);
    }
    std::vector<uint32_t> unit(k, 0);
    unit[0] = 1;
    return from_limbs(mul(x, unit));
}

bool miller_rabin(const BigInt& n, int rounds, SecureRandom& rnd) {
    BigInt one(1), n1 = n - one;
    BigInt d = n1;
    int64_t s = 0;
    while (!d.bit(0)) { d = d.shr(1); ++s; }
    int64_t nbytes = (n.bit_length() + 7) / 8;
    for (int r = 0; r < rounds; ++r) {
        BigInt a = BigInt::from_bytes(py::bytes(rnd.bytes(static_cast<size_t>(nbytes))), "big") % (n - BigInt(3)) + BigInt(2);
        BigInt x = montgomery_powmod(a, d, n);
        if (x == one || x == n1) continue;
        bool composite = true;
        for (int64_t i = 1; i < s; ++i) {
            x = (x * x) % n;
            if (x == n1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

// a prime of exactly `bits` bits (the top two bits set, so that two of them multiply to a 2*bits-bit
// modulus) with gcd(e, p - 1) == 1 for a prime e
BigInt random_prime(int64_t bits, int64_t e, SecureRandom& rnd) {
    static const std::vector<int64_t> small = [] {
        std::vector<int64_t> v;
        for (int64_t i = 3; i < 4000; i += 2) {
            bool prime = true;
            for (int64_t d : v) { if (d * d > i) break; if (i % d == 0) { prime = false; break; } }
            if (prime) v.push_back(i);
        }
        return v;
    }();
    BigInt be(static_cast<uint64_t>(e));
    for (;;) {
        std::string s = rnd.bytes(static_cast<size_t>(bits / 8));
        s[0] = static_cast<char>(static_cast<uint8_t>(s[0]) | 0xC0);
        s.back() = static_cast<char>(static_cast<uint8_t>(s.back()) | 1);
        BigInt base = BigInt::from_bytes(py::bytes(s), "big");
        std::vector<int64_t> rems;
        for (int64_t p : small) rems.push_back((base % BigInt(static_cast<uint64_t>(p))).to_int64());
        for (int64_t delta = 0; delta < 20000; delta += 2) {
            bool ok = true;
            for (size_t k = 0; k < small.size(); ++k) {
                if ((rems[k] + delta) % small[k] == 0) { ok = false; break; }
            }
            if (!ok) continue;
            BigInt cand = base + BigInt(static_cast<uint64_t>(delta));
            if (cand.bit_length() != bits) break;
            if (((cand - BigInt(1)) % be).is_zero()) continue;
            if (miller_rabin(cand, 8, rnd)) return cand;
        }
    }
}

}  // namespace detail

namespace {

using detail::BlockModeContext;
using std::string;

inline string raw(const py::bytes& b) { return b.raw(); }
inline py::bytes mk(const string& s) { return py::bytes(s); }
inline uint8_t u8(char c) { return static_cast<uint8_t>(c); }

uint64_t be64_at(const string& s, size_t off) {
    uint64_t v = 0;
    for (size_t i = 0; i < 8; ++i) v = (v << 8) | u8(s[off + i]);
    return v;
}
string be64_str(uint64_t v) {
    string s(8, '\0');
    for (int i = 7; i >= 0; --i) { s[static_cast<size_t>(i)] = static_cast<char>(v & 0xFF); v >>= 8; }
    return s;
}
string be32_str(uint32_t v) {
    string s(4, '\0');
    for (int i = 3; i >= 0; --i) { s[static_cast<size_t>(i)] = static_cast<char>(v & 0xFF); v >>= 8; }
    return s;
}
uint32_t be32_at(const string& s, size_t off) {
    uint32_t v = 0;
    for (size_t i = 0; i < 4; ++i) v = (v << 8) | u8(s[off + i]);
    return v;
}

// the order of the 8-byte units reversed (the length must be a multiple of 8)
string reverse_qwords(const string& s) {
    string out(s.size(), '\0');
    size_t cqw = s.size() / 8;
    for (size_t i = 0; i < cqw; ++i) std::memcpy(&out[i * 8], &s[(cqw - 1 - i) * 8], 8);
    return out;
}

py::bytes slice_b(const py::bytes& b, int64_t lo, int64_t hi) { return b.slice(lo, hi); }

const py::bytes VERSION = py::bytes::fromhex(py::str("0712"));

py::bytes hmac_sha(const py::bytes& key, std::initializer_list<py::bytes> parts) {
    py::list<py::bytes> l;
    for (const auto& p : parts) l.append(p);
    return XeCryptHmacSha(key, l);
}
py::list<py::bytes> one(const py::bytes& b) { py::list<py::bytes> l; l.append(b); return l; }

}  // namespace

// =====================================================================================================
// constants
// =====================================================================================================
BigInt create_bitmask(int64_t n) { return MemCrypto::create_mask(n); }

const py::bytes XECRYPT_SMC_KEY = py::bytes::fromhex(py::str("42754E79"));
std::optional<py::bytes> _store_key(const py::str& name) { return MemCrypto::_store_key(name); }
const std::optional<py::bytes>& XECRYPT_1BL_KEY() { return MemCrypto::_1BL_KEY(); }
const py::bytes XECRYPT_1BL_SALT("XBOX_ROM_B");
const py::bytes XECRYPT_SC_SALT("XBOX_ROM_3");
const py::bytes XECRYPT_SD_SALT("XBOX_ROM_4");

BigInt UINT64_MASK() { return create_bitmask(64); }
BigInt UINT128_MASK() { return create_bitmask(128); }

// =====================================================================================================
// structures
// =====================================================================================================
void buffer_too_small_(int64_t have, int64_t need) {
    throw py::ValueError("Buffer size too small (" + std::to_string(have) + " instead of at least " + std::to_string(need) + " bytes)");
}

XECRYPT_SIG XECRYPT_SIG::from_buffer_copy(const py::bytes& data) {
    if (py::len(data) < sizeof_) buffer_too_small_(py::len(data), sizeof_);
    string s = data.raw();
    XECRYPT_SIG r;
    for (size_t i = 0; i < 0x1C; ++i) r.aqwPad[i] = be64_at(s, i * 8);
    r.bOne = u8(s[0xE0]);
    r.abSalt = mk(s.substr(0xE1, 0xA));
    r.abHash = mk(s.substr(0xEB, 0x14));
    r.bEnd = u8(s[0xFF]);
    return r;
}

XECRYPT_RSA XECRYPT_RSA::from_buffer_copy(const py::bytes& data) {
    if (py::len(data) < sizeof_) buffer_too_small_(py::len(data), sizeof_);
    string s = data.raw();
    XECRYPT_RSA r;
    r.cqw = be32_at(s, 0);
    r.e = be32_at(s, 4);
    r.qwReserved = be64_at(s, 8);
    return r;
}

const std::vector<KeyvaultField>& XECRYPT_KEYVAULT::fields() {
    static const std::vector<KeyvaultField> table = [] {
        struct F { const char* name; int64_t size; };
        static const F layout[] = {
            {"nonce", 0x10}, {"obfuscation", 8}, {"manufacturing_mode", 1}, {"alternate_keyvault", 1},
            {"restricted_privilege_flags", 1}, {"reserved_byte_3", 1}, {"odd_features", 2}, {"odd_auth_type", 2},
            {"restricted_hv_ext_loader", 2}, {"reserved_ushort_1", 2}, {"policy_flash_size", 4},
            {"policy_build_in_mu_size", 4}, {"reserved_dword_4", 4}, {"restricted_privileges", 8},
            {"reserved_qword_2", 8}, {"reserved_qword_3", 8}, {"reserved_qword_4", 8}, {"reserved_key_1", 0x10},
            {"reserved_key_2", 0x10}, {"reserved_key_3", 0x10}, {"reserved_key_4", 0x10},
            {"reserved_random_key_1", 0x10}, {"reserved_random_key_2", 0x10}, {"console_serial", 0xC},
            {"PADDING1", 4}, {"motherboard_serial", 8}, {"game_region", 2}, {"PADDING2", 6},
            {"console_obf_key", 0x10}, {"key_obf_key", 0x10}, {"roamable_obf_key", 0x10}, {"odd_key", 0x10},
            {"primary_activation_key", 0x18}, {"secondary_activation_key", 0x10},
            {"global_dev_2des_key_1", 0x10}, {"global_dev_2des_key_2", 0x10},
            {"wireless_controller_2des_key_1", 0x10}, {"wireless_controller_2des_key_2", 0x10},
            {"wired_webcam_2des_key_1", 0x10}, {"wired_webcam_2des_key_2", 0x10},
            {"wired_controller_2des_key_1", 0x10}, {"wired_controller_2des_key_2", 0x10},
            {"memory_unit_2des_key_1", 0x10}, {"memory_unit_2des_key_2", 0x10},
            {"other_xsm3_dev_2des_key_1", 0x10}, {"other_xsm3_dev_2des_key_2", 0x10},
            {"wireless_controller_3p2des_key_1", 0x10}, {"wireless_controller_3p2des_key_2", 0x10},
            {"wired_webcam_3p2des_key_1", 0x10}, {"wired_webcam_3p2des_key_2", 0x10},
            {"wired_controller_3p2des_key_1", 0x10}, {"wired_controller_3p2des_key_2", 0x10},
            {"memory_unit_3p2des_key_1", 0x10}, {"memory_unit_3p2des_key_2", 0x10},
            {"other_xsm3_dev_3p2des_key_1", 0x10}, {"other_xsm3_dev_3p2des_key_2", 0x10},
            {"console_private_key", XECRYPT_RSAPRV_1024::sizeof_}, {"xeika_private_key", XECRYPT_RSAPRV_2048::sizeof_},
            {"cardea_private_key", XECRYPT_RSAPRV_1024::sizeof_}, {"console_certificate_size", 2},
            {"console_id", 5}, {"console_part_number", 0xB}, {"console_reserved", 4}, {"console_privileges", 2},
            {"console_type", 4}, {"manufacture_date", 8}, {"console_public_key_exponent", 4},
            {"console_public_key_modulus", 0x80}, {"console_certificate_signature", XECRYPT_SIG::sizeof_},
            {"xeika_certificate_size", 2}, {"xeika_public_key", XECRYPT_RSAPUB_2048::sizeof_},
            {"xeika_certificate_overlay_signature", 4}, {"xeika_certificate_overlay_version", 2},
            {"xeika_odd_date_version", 1}, {"xeika_odd_drive_phase_level", 1}, {"odd_version_string", 0x28},
            {"xeika_certificate_reserved", 0x1146}, {"special_keyvault_signature", XECRYPT_SIG::sizeof_},
            {"cardea_certificate", 0x2108}};
        std::vector<KeyvaultField> t;
        int64_t off = 0;
        for (const F& f : layout) {
            t.push_back(KeyvaultField{py::str(f.name), off, f.size});
            off += f.size;
        }
        return t;
    }();
    return table;
}

// =====================================================================================================
// utilities
// =====================================================================================================
py::bytes reverse(const py::bytes& b) {
    string s = b.raw();
    std::reverse(s.begin(), s.end());
    return mk(s);
}

py::bytes rsa_pad(const py::bytes& data) {
    const int64_t bounds = 8;
    if (py::len(data) % bounds == 0) return data;
    int64_t ps = bounds - (py::len(data) % bounds);
    return data + py::bytes::zeros(ps);
}

BigInt b2i(const py::bytes& b, bool do_bswap) {
    py::bytes v = do_bswap ? bswap64(b) : b;
    return BigInt::from_bytes(v, "little");
}

py::bytes i2b(const BigInt& i, int64_t size, bool do_bswap) {
    py::bytes data = i.to_bytes(size == 0 ? (i.bit_length() + 7) / 8 : size, "little");
    data = rsa_pad(data);   // sometimes the data isn't evenly divisible by 8
    if (do_bswap) data = bswap64(data);
    return data;
}

namespace {
// bit i of the two's-complement value (an arithmetic shift: bits past the top repeat the sign)
bool bit_of(int64_t v, int64_t i) {
    if (i < 0) return false;
    if (i >= 63) return v < 0;
    return ((v >> i) & 1) != 0;
}
// the assembled `bits`-bit result as an int64
int64_t from_bits_(const std::vector<bool>& r) {
    uint64_t v = 0;
    for (size_t j = 0; j < r.size(); ++j) {
        if (!r[j]) continue;
        if (j >= 63) throw py::OverflowError("Python int too large to convert to C long");
        v |= uint64_t(1) << j;
    }
    return static_cast<int64_t>(v);
}
int64_t rotate_(int64_t value, int64_t shift, int64_t bits, bool left) {
    // left: (value << shift) | (value >> (bits - shift));  right: (value >> shift) | (value << (bits - shift))
    int64_t other = bits - shift;
    if (shift < 0 || other < 0) throw py::ValueError("negative shift count");
    if (bits < 0) throw py::ValueError("negative shift count");   // (1 << bits) with a negative bits
    if (value >= 0) {
        BigInt v(static_cast<uint64_t>(value));
        BigInt r = left ? (v.shl(shift) | v.shr(other)) : (v.shr(shift) | v.shl(other));
        return r.masked(bits).to_int64();
    }
    if (bits > 4096) throw py::OverflowError("Python int too large to convert to C long");
    std::vector<bool> r(static_cast<size_t>(bits), false);
    int64_t up = left ? shift : other;     // the amount shifted left
    int64_t down = left ? other : shift;   // the amount shifted right
    for (int64_t j = 0; j < bits; ++j) r[static_cast<size_t>(j)] = (j >= up && bit_of(value, j - up)) || bit_of(value, j + down);
    return from_bits_(r);
}
}  // namespace

int64_t rotl(int64_t value, int64_t shift, int64_t bits) { return rotate_(value, shift, bits, true); }
int64_t rotr(int64_t value, int64_t shift, int64_t bits) { return rotate_(value, shift, bits, false); }

py::bytes bswap(const py::bytes& data, const py::str& fmt) {
    int64_t size = py::calcsize(fmt.utf8().c_str());
    if (size == 0) throw py::ZeroDivisionError("integer modulo by zero");
    if (!(py::len(data) % size == 0)) throw py::AssertionError("data isn't evenly divisible by size!");
    string s = data.raw(), out;
    for (size_t i = 0; i < s.size(); i += static_cast<size_t>(size)) {
        string t = s.substr(i, static_cast<size_t>(size));
        std::reverse(t.begin(), t.end());
        out += t;
    }
    return mk(out);
}
py::bytes bswap16(const py::bytes& data) { return bswap(data, py::str("H")); }
py::bytes bswap32(const py::bytes& data) { return bswap(data, py::str("I")); }
py::bytes bswap64(const py::bytes& data) { return bswap(data, py::str("Q")); }
py::bytes XeCryptBnQw_SwapDwQwLeBe(const py::bytes& b) { return bswap64(b); }

bool memcmp(const py::bytes& b0, const py::bytes& b1, int64_t size) {
    bool all = true;
    for (int64_t i = 0; i < size; ++i) all = (b0[i] == b1[i]) && all;   // (the list is built whole: every index is read)
    return all;
}

void random_seed(int64_t a) { detail::rng().seed(a); }
py::bytes XeCryptRandom(int64_t cb) { return mk(detail::rng().randbytes(cb)); }

// =====================================================================================================
// hashing / MAC
// =====================================================================================================
py::bytes XeCryptMd5(const py::list<py::bytes>& args) { return detail::md5_of(detail::join_(args)); }
py::bytes XeCryptSha(const py::list<py::bytes>& args) { return detail::sha1_of(detail::join_(args)); }
py::bytes XeCryptHmacMd5(const py::bytes& key, const py::list<py::bytes>& args) {
    return detail::hmac(detail::md5_of, key, detail::join_(args));
}
py::bytes XeCryptHmacSha(const py::bytes& key, const py::list<py::bytes>& args) {
    return detail::hmac(detail::sha1_of, key, detail::join_(args));
}

// =====================================================================================================
// RC4 / AES / DES
// =====================================================================================================
XeCryptRc4::XeCryptRc4(const py::bytes& key) {
    reset();
    enc_ = std::make_shared<detail::Rc4State>(key.raw());
    dec_ = std::make_shared<detail::Rc4State>(key.raw());
}
void XeCryptRc4::reset() {}   // (the Python's reset() only forgets the Cipher object, which nothing else reads)
XeCryptRc4 XeCryptRc4::new_(const py::bytes& key) { return XeCryptRc4(key); }
py::bytes XeCryptRc4::crypt(const py::bytes& data) { return encrypt(data); }
py::bytes XeCryptRc4::encrypt(const py::bytes& data) { return mk(enc_->update(data.raw())); }
py::bytes XeCryptRc4::decrypt(const py::bytes& data) { return mk(dec_->update(data.raw())); }

XeCryptAes::XeCryptAes(const py::bytes& key, std::optional<int64_t> mode, const std::optional<py::bytes>& iv) {
    reset();
    if (!(py::len(key) * 8 == 128 || py::len(key) * 8 == 256)) throw py::AssertionError("AES key must be 128 or 256 bits");
    bool cbc = detail::check_mode_(mode, iv, 16);
    for (int pass = 0; pass < 2; ++pass) {
        bool decrypt = pass == 1;
        auto aes = std::make_shared<MemCrypto::AesEcb>(key, decrypt);
        auto fn = [aes](uint8_t* block) {
            py::bytes out = aes->update(py::bytes(reinterpret_cast<const char*>(block), 16));
            std::memcpy(block, out.raw().data(), 16);
        };
        auto ctx = std::make_shared<BlockModeContext>(16, fn, decrypt, cbc, cbc ? iv->raw() : string());
        (decrypt ? dec_ : enc_) = ctx;
    }
}
void XeCryptAes::reset() {}
XeCryptAes XeCryptAes::new_(const py::bytes& key, int64_t mode, const std::optional<py::bytes>& iv) { return XeCryptAes(key, mode, iv); }
py::bytes XeCryptAes::encrypt(const py::bytes& data) { return enc_->update(data); }
py::bytes XeCryptAes::decrypt(const py::bytes& data) { return dec_->update(data); }

template <int64_t KeyBits>
TripleDesCipher<KeyBits>::TripleDesCipher(const py::bytes& key, std::optional<int64_t> mode, const std::optional<py::bytes>& iv) {
    reset();
    const char* name = KeyBits == 64 ? "DES" : (KeyBits == 128 ? "DES2" : "DES3");
    if (!(py::len(key) * 8 == KeyBits))
        throw py::AssertionError(string(name) + " key must be " + std::to_string(KeyBits) + " bits");
    bool cbc = detail::check_mode_(mode, iv, 8);
    // TripleDES(key): an 8-byte key is k,k,k and a 16-byte one k1,k2,k1
    string k = key.raw();
    if (k.size() == 8) k = k + k + k;
    else if (k.size() == 16) k = k + k.substr(0, 8);
    auto k1 = std::make_shared<detail::DesSchedule>(reinterpret_cast<const uint8_t*>(k.data()));
    auto k2 = std::make_shared<detail::DesSchedule>(reinterpret_cast<const uint8_t*>(k.data() + 8));
    auto k3 = std::make_shared<detail::DesSchedule>(reinterpret_cast<const uint8_t*>(k.data() + 16));
    for (int pass = 0; pass < 2; ++pass) {
        bool decrypt = pass == 1;
        auto fn = [k1, k2, k3, decrypt](uint8_t* block) {
            uint64_t v = detail::load_be64(block);
            if (!decrypt) v = k3->crypt(k2->crypt(k1->crypt(v, false), true), false);
            else v = k1->crypt(k2->crypt(k3->crypt(v, true), false), true);
            detail::store_be64(block, v);
        };
        auto ctx = std::make_shared<BlockModeContext>(8, fn, decrypt, cbc, cbc ? iv->raw() : string());
        (decrypt ? dec_ : enc_) = ctx;
    }
}
template <int64_t KeyBits>
void TripleDesCipher<KeyBits>::reset() {}
template <int64_t KeyBits>
TripleDesCipher<KeyBits> TripleDesCipher<KeyBits>::new_(const py::bytes& key, int64_t mode, const std::optional<py::bytes>& iv) {
    return TripleDesCipher(key, mode, iv);
}
template <int64_t KeyBits>
py::bytes TripleDesCipher<KeyBits>::encrypt(const py::bytes& data) { return enc_->update(data); }
template <int64_t KeyBits>
py::bytes TripleDesCipher<KeyBits>::decrypt(const py::bytes& data) { return dec_->update(data); }
template class TripleDesCipher<64>;
template class TripleDesCipher<128>;
template class TripleDesCipher<192>;

py::bytes XeCryptDesParity(const py::bytes& data) {
    string out(data.raw().size(), '\0');
    for (size_t i = 0; i < out.size(); ++i) {
        int p = u8(data.raw()[i]);
        p ^= p >> 4;
        p ^= p >> 2;
        p ^= p >> 1;
        out[i] = static_cast<char>((u8(data.raw()[i]) & 0xFE) | (~p & 1));
    }
    return mk(out);
}

py::bytes XeCryptParveEcb(const py::bytes& key, const py::bytes& sbox, const py::bytes& data) {
    // block[:8] = data[:8] on a 9-byte array: a shorter data shrinks it, and block[8] is then out of range
    if (py::len(data) < 8) throw py::IndexError("bytearray index out of range");
    uint8_t block[9];
    for (int i = 0; i < 8; ++i) block[i] = static_cast<uint8_t>(data[i]);
    block[8] = block[0];
    for (int i = 8; i > 0; --i) {
        for (int j = 0; j < 8; ++j) {
            int64_t x = (key[j] + block[j] + i) & UINT8_MASK;
            int64_t y = (sbox[x] + block[j + 1]) & UINT8_MASK;
            y = rotl(y, 1, 8);   // y *= 2
            block[j + 1] = static_cast<uint8_t>(y & UINT8_MASK);
        }
        block[0] = block[8];
    }
    return py::bytes(reinterpret_cast<const char*>(block), 8);
}

py::bytes XeCryptParveCbcMac(const py::bytes& key, const py::bytes& sbox, const py::bytes& iv, const py::bytes& data) {
    py::bytes block = iv;   // block[:8] = iv
    if (py::len(data) >= 8) {
        for (int64_t i = 0; i < py::len(data); i += 8) {
            if (py::len(block) != 8) throw py::StructError("unpack requires a buffer of 8 bytes");
            if (i + 8 > py::len(data)) throw py::StructError("unpack_from requires a buffer of at least 8 bytes");
            uint64_t v0 = be64_at(block.raw(), 0);
            uint64_t v1 = be64_at(data.raw(), static_cast<size_t>(i));
            v0 ^= v1;
            block = XeCryptParveEcb(key, sbox, mk(be64_str(v0)));
        }
    }
    return block.slice(0, 8);
}

py::bytes XeCryptChainAndSumMac(const py::bytes& cd, const py::bytes& ab, const py::bytes& data) {
    if (py::len(ab) != 8) throw py::StructError("unpack requires a buffer of 8 bytes");
    const uint64_t M = 0x7FFFFFFF;
    uint64_t out0 = 0, out1 = 0;
    uint64_t ab0 = be32_at(ab.raw(), 0) % M, ab1 = be32_at(ab.raw(), 4) % M;
    if (py::len(cd) != 8) throw py::StructError("unpack requires a buffer of 8 bytes");
    uint64_t cd0 = be32_at(cd.raw(), 0) % M, cd1 = be32_at(cd.raw(), 4) % M;
    for (int64_t i = 0; i < py::len(data); i += 8) {
        if (i + 8 > py::len(data)) throw py::StructError("unpack_from requires a buffer of at least 8 bytes");
        uint64_t v0 = be32_at(data.raw(), static_cast<size_t>(i)), v1 = be32_at(data.raw(), static_cast<size_t>(i) + 4);
        uint64_t t = v0 * 0xE79A9C1ULL;
        t += out0;
        t %= M;
        t *= ab0;
        t += ab1;
        t %= M;
        out1 += t;
        t += v1;
        t *= cd0;
        t %= M;
        t += cd1;
        out0 = t % M;
        out1 += out0;
    }
    return mk(be32_str(static_cast<uint32_t>((out0 + ab1) % M)) + be32_str(static_cast<uint32_t>((out1 + cd1) % M)));
}

// =====================================================================================================
// checksums
// =====================================================================================================
std::optional<py::bytes> XeCryptRotSum(const py::bytes& data) {
    int64_t cqw = py::len(data) / 8;
    if (cqw == 0) return std::nullopt;
    uint64_t qw1 = 0, qw2 = 0, qw3 = 0, qw4 = 0;
    for (int64_t i = 0; i < cqw; ++i) {
        uint64_t tqw = be64_at(data.raw(), static_cast<size_t>(i) * 8);
        qw2 += tqw;
        qw4 -= tqw;

        qw1 += (qw2 < tqw);
        qw2 = (qw2 * 0x20000000ULL) | (qw2 >> 0x23);

        qw3 -= (tqw < qw4);
        qw4 = (qw4 * 0x80000000ULL) | (qw4 >> 0x21);
    }
    return mk(be64_str(qw1) + be64_str(qw2) + be64_str(qw3) + be64_str(qw4));
}

py::bytes XeCryptRotSumSha(const py::bytes& data) {
    std::optional<py::bytes> sum = XeCryptRotSum(data);
    if (!sum.has_value()) throw py::TypeError("cannot convert 'NoneType' object to bytearray");
    py::bytes rot_sum = *sum;
    string inverted = rot_sum.raw();
    for (char& c : inverted) c = static_cast<char>(~u8(c) & 0xFF);
    return detail::sha1_of(rot_sum + rot_sum + data + mk(inverted) + mk(inverted));
}

// =====================================================================================================
// RSA
// =====================================================================================================
BigInt XeCryptBnQwNeModInv(const BigInt& v) { return BigInt::modinv(BigInt(1).shl(64), v); }

BigInt XeCryptBnQwNeModExpRoot(const BigInt& c, const BigInt& p, const BigInt& q, const BigInt& dp, const BigInt& dq, const BigInt& u) {
    BigInt m1 = BigInt::powmod(c, dp, p);
    BigInt m2 = BigInt::powmod(c, dq, q);
    // h = (u * (m1 - m2)) % p, the difference taken mod p (Python's % is never negative for a positive p)
    BigInt diff = (m1 % p + p - m2 % p) % p;
    BigInt h = (u * diff) % p;
    return m2 + h * q;
}

py::bytearray XeCryptBnQwBeBufSwap(py::bytearray data) {
    if (!(data.size() % 8 == 0)) throw py::AssertionError("");
    data.raw_mut() = reverse_qwords(data.raw());
    return data;
}
py::bytes XeCryptBnQwBeBufSwap(const py::bytes& data) {
    return py::bytes(XeCryptBnQwBeBufSwap(py::bytearray(data)).raw());
}

std::tuple<py::bytes, py::bytes> XeCryptBnQwNeRsaKeyGen(int64_t cbits, int64_t exp) {
    if (!(cbits == 1024 || cbits == 1536 || cbits == 2048 || cbits == 4096)) throw py::AssertionError("Invalid bit count specified!");
    if (exp != 3 && exp != 65537)
        throw py::ValueError("public_exponent must be either 3 (for legacy compatibility) or 65537. Almost everyone should choose 65537 here!");
    int64_t cb = cbits / 8;
    int64_t cbh = cb / 2;
    int64_t cqw = cb / 8;

    detail::SecureRandom rnd;
    BigInt p = detail::random_prime(cbits / 2, exp, rnd);
    BigInt q = detail::random_prime(cbits / 2, exp, rnd);
    while (q == p) q = detail::random_prime(cbits / 2, exp, rnd);
    if (p < q) std::swap(p, q);
    BigInt e(static_cast<uint64_t>(exp));
    BigInt pub_n = p * q;
    BigInt d = BigInt::modinv(e, (p - BigInt(1)) * (q - BigInt(1)));
    BigInt dmp1 = d % (p - BigInt(1));
    BigInt dmq1 = d % (q - BigInt(1));
    BigInt iqmp = BigInt::modinv(q, p);

    py::bytes n = i2b(pub_n, cb, true);
    py::bytes pb = i2b(p, cbh, true);
    py::bytes qb = i2b(q, cbh, true);
    py::bytes dpb = i2b(dmp1, cbh, true);
    py::bytes dqb = i2b(dmq1, cbh, true);
    py::bytes ub = i2b(iqmp, cbh, true);

    BigInt mod_inv = XeCryptBnQwNeModInv(pub_n);
    mod_inv = mod_inv & UINT64_MASK();

    // pack(">2IQ <n> <p> <q> <dp> <dq> <u>")
    py::bytes b_prv_key = mk(be32_str(static_cast<uint32_t>(cqw)) + be32_str(static_cast<uint32_t>(exp)) + mod_inv.to_bytes(8, "big").raw()) +
                          n + pb + qb + dpb + dqb + ub;
    int64_t pub_size = cbits == 1024 ? XECRYPT_RSAPUB_1024_SIZE
                       : cbits == 1536 ? XECRYPT_RSAPUB_1536_SIZE
                       : cbits == 2048 ? XECRYPT_RSAPUB_2048_SIZE
                                       : XECRYPT_RSAPUB_4096_SIZE;
    return {b_prv_key.slice(0, pub_size), b_prv_key};
}

py::bytes XeCryptBnQwBeSigFormat(int64_t cqw, const py::bytes& b_hash, const py::bytes& salt) {
    if (cqw < 0) throw py::ValueError("negative count");
    string sig(static_cast<size_t>(cqw) * 8, '\0');

    py::bytes ab_hash = detail::sha1_of(py::bytes::zeros(8) + b_hash + salt);

    // the pack_into calls below write up to offset 0x100: struct.error on a buffer shorter than that
    if (sig.size() < 0x100) throw py::StructError("pack_into requires a buffer of at least 256 bytes");
    sig[0xE0] = 1;
    string s10 = salt.raw().substr(0, 10);
    s10.resize(10, '\0');
    sig.replace(0xE1, 10, s10);
    detail::Rc4State rc4(ab_hash.raw());
    sig.replace(0, 0xEB, rc4.update(sig.substr(0, 0xEB)));
    sig.replace(0xEB, 20, ab_hash.raw());
    sig[0xFF] = static_cast<char>(0xBC);
    sig[0] = static_cast<char>(u8(sig[0]) & 0x7F);
    return XeCryptBnQwBeBufSwap(mk(sig));
}

py::bytes XeCryptBnQwBeSigCreate(const py::bytes& b_hash, const py::bytes& salt, const py::bytes& prv_key) {
    if (py::len(salt) > 10) throw py::Exception("Salt parameter must be 10 bytes or less");

    XeCryptRsaKey key(prv_key);
    if (key.cqw() != 0x20) throw py::Exception("Only PXECRYPT_RSAPRV_2048 can create signatures");   // PXECRYPT_RSAPRV_2048

    if (key.e() != 0x3 && key.e() != 0x10001) throw py::Exception("Public exponent must be 0x3 or 0x10001");

    py::bytes sig = XeCryptBnQwBeSigFormat(key.cqw(), b_hash, salt);
    if (sig.raw() == py::bytes::zeros(key.n_size_in_bytes()).raw()) throw py::Exception("Output signature size overflow");

    BigInt si = b2i(sig, true);
    BigInt se = (si * key.r()) % key.n();   // convert out
    return i2b(se, key.cqw() * 8, true);
}

bool XeCryptBnQwBeSigVerify(const py::bytes& sig, const py::bytes& b_hash, const py::bytes& salt, const py::bytes& pub_key) {
    if (py::len(salt) > 10) throw py::Exception("Salt parameter must be 10 bytes or less");

    XeCryptRsaKey key(pub_key);

    if (key.cqw() != 0x20) throw py::Exception("Only PXECRYPT_RSAPUB_2048 can verify signatures");   // PXECRYPT_RSAPUB_2048

    BigInt si = b2i(sig, true);
    BigInt se = BigInt::powmod(si, BigInt(static_cast<uint64_t>(key.e())), key.n());   // reverse of pow(sig, key.d, key.n)
    BigInt sd_int = (se * key.inv_r()) % key.n();   // reverse of (si * key.r) % key.n
    py::bytes sb = i2b(sd_int, key.cqw() * 8, true);

    py::bytes sd = XeCryptBnQwBeBufSwap(sb);
    string s = sd.raw();

    if (u8(s[0xFF]) != 0xBC) return false;

    py::bytes digest = detail::sha1_of(py::bytes::zeros(8) + b_hash + salt);
    string hash_part = s.substr(0xEB, s.size() - 1 - 0xEB);   // sd[0xEB:-1]
    if (digest.raw() != hash_part) return false;

    detail::Rc4State rc4(hash_part);
    string plain = rc4.update(s.substr(0, 0xEB));

    if (u8(plain[0xE0]) != 1) return false;

    for (size_t i = 1; i < 0xE0; ++i) {
        if (plain[i] != 0) return false;
    }

    if (plain.substr(0xE1, static_cast<size_t>(py::len(salt))) != salt.raw()) return false;

    return true;
}

std::optional<py::bytes> XeCryptBnDwLePkcs1Format(const py::bytes& b_hash, int64_t fmt_type, int64_t cb_sig) {
    if (cb_sig < 0x27 || cb_sig > 0x200) return std::nullopt;

    string sig(0x200, '\0');
    sig.replace(0, static_cast<size_t>(cb_sig), string(static_cast<size_t>(cb_sig), static_cast<char>(0xFF)));
    sig[static_cast<size_t>(cb_sig - 1)] = 0;
    sig[static_cast<size_t>(cb_sig - 2)] = 1;
    // sig[:20] = b_hash[::-1]: a hash that is not 20 bytes changes the length of sig
    string rev = b_hash.raw();
    std::reverse(rev.begin(), rev.end());
    sig.replace(0, 20, rev);
    if (fmt_type == 0) {
        string tbuf = py::bytes::fromhex(py::str("140400051A02030E2B05060930213000")).raw();
        sig.replace(0x14, tbuf.size(), tbuf);
    } else if (fmt_type == 1) {
        string tbuf = py::bytes::fromhex(py::str("14041A02030E2B050607301F3000")).raw();
        sig.replace(0x14, tbuf.size(), tbuf);
    } else {
        sig[0x14] = 0;
    }
    return mk(sig);
}

std::optional<bool> XeCryptBnDwLePkcs1Verify(const py::bytes& sig, const py::bytes& b_hash, int64_t cb_sig) {
    if (0x27 <= py::len(sig) && py::len(sig) <= 0x200) {
        int64_t typ = 2;
        if (sig[0x16] == 0) typ = 0;
        else if (sig[0x16] == 0x1A) typ = 1;
        std::optional<py::bytes> buf = XeCryptBnDwLePkcs1Format(b_hash, typ, cb_sig);
        if (!buf.has_value()) {   // memcmp(None, sig, cb_sig): range(cb_sig) is empty only for cb_sig <= 0
            if (cb_sig <= 0) return true;
            throw py::TypeError("'NoneType' object is not subscriptable");
        }
        return memcmp(*buf, sig, cb_sig);
    }
    return std::nullopt;
}

std::optional<py::bytes> XeKeysPkcs1Create(const py::bytes& b_hash, const py::bytes& prv_key) {
    XeCryptRsaKey key(prv_key);
    if (0 < key.cqw() && key.cqw() <= 0x40) {
        int64_t typ = 0;
        std::optional<py::bytes> buf = XeCryptBnDwLePkcs1Format(b_hash, typ, key.cqw() << 3);
        if (!buf.has_value()) throw py::TypeError("object of type 'NoneType' has no len()");
        py::bytes swapped = bswap64(*buf);
        py::bytes crypted = key.prv_crypt(swapped);
        return bswap64(crypted);
    }
    return std::nullopt;
}

bool XeKeysPkcs1Verify(const py::bytes& sig, const py::bytes& b_hash, const py::bytes& pub_key) {
    XeCryptRsaKey key(pub_key);
    if (0 < key.cqw() && key.cqw() <= 0x40) {
        py::bytes buf = bswap64(sig);
        buf = key.pub_crypt(buf);
        buf = bswap64(buf);
        // (buf is the key's size, 0x80..0x200 bytes: always inside the range Pkcs1Verify takes, so never None)
        return XeCryptBnDwLePkcs1Verify(buf, b_hash, key.cqw() << 3).value_or(false);
    }
    return false;
}

py::bytes XeCryptBnQwNeRsaPrvCrypt(const py::bytes& data, const py::bytes& prv_key) {
    XeCryptRsaKey key(prv_key);
    return key.prv_crypt(data);
}

py::bytes XeCryptBnQwNeRsaPubCrypt(const py::bytes& data, const py::bytes& pub_key) {
    XeCryptRsaKey key(pub_key);
    return key.pub_crypt(data);
}

// =====================================================================================================
// utility
// =====================================================================================================
py::bytes XeCryptSmcDecrypt(const py::bytes& data) {
    // (only the low byte of each key word is ever used, so 32-bit words that wrap give the same bytes)
    uint32_t key[4];
    for (int i = 0; i < 4; ++i) key[i] = static_cast<uint32_t>(XECRYPT_SMC_KEY[i]);
    string out;
    for (int64_t i = 0; i < py::len(data); ++i) {
        uint32_t j = static_cast<uint32_t>(data[i]);
        uint32_t mod = j * 0xFB;
        out.push_back(static_cast<char>(j ^ (key[i & 3] & 0xFF)));
        key[(i + 1) & 3] += mod;
        key[(i + 2) & 3] += mod >> 8;
    }
    return mk(out);
}

py::bytes XeCryptSmcEncrypt(const py::bytes& data) {
    uint32_t key[4];
    for (int i = 0; i < 4; ++i) key[i] = static_cast<uint32_t>(XECRYPT_SMC_KEY[i]);
    string out;
    for (int64_t i = 0; i < py::len(data); ++i) {
        uint32_t j = static_cast<uint32_t>(data[i]) ^ (key[i & 3] & 0xFF);
        uint32_t mod = j * 0xFB;
        out.push_back(static_cast<char>(j));
        key[(i + 1) & 3] += mod;
        key[(i + 2) & 3] += mod >> 8;
    }
    return mk(out);
}

int64_t XeCryptHammingWeight(const py::bytes& data) {
    int64_t wght = 0;
    for (int64_t i = 0; i < py::len(data); ++i) {
        int64_t val = data[i];
        for (int j = 0; j < 8; ++j) {
            wght += val & 1;
            val >>= 1;
        }
    }
    return wght;
}

py::bytes XeCryptUidEccEncode(const py::bytes& data_in) {
    string data = data_in.raw();
    int64_t acc1 = 0;
    int64_t acc2 = 0;
    for (int64_t cnt = 0; cnt < 0x80; ++cnt) {
        acc1 >>= 1;
        if (static_cast<size_t>(cnt >> 3) >= data.size()) throw py::IndexError("bytearray index out of range");
        int64_t b_tmp = u8(data[static_cast<size_t>(cnt >> 3)]);
        int64_t dw_tmp = (b_tmp >> (cnt & 7)) & 1;
        if (cnt < 0x6A) {
            acc1 ^= dw_tmp;
            if (acc1 & 1) acc1 ^= 0x360325;
            acc2 ^= dw_tmp;
        } else if (cnt < 0x7F) {
            if (dw_tmp != (acc1 & 1)) data[static_cast<size_t>(cnt >> 3)] = static_cast<char>((int64_t(1) << (cnt & 7)) ^ (b_tmp & 0xFF));
            acc2 ^= (acc1 & 1);
        } else if (dw_tmp != acc2) {
            data[0xF] = static_cast<char>((0x80 ^ b_tmp) & 0xFF);
        }
    }
    return mk(data);
}

// additions to the XeCrypt library that didn't exist in the original
bool XeCryptCpuKeyValid(const py::bytes& cpu_key) {
    if (py::len(cpu_key) != 0x10) return false;

    py::bytes wght_mask = py::bytes::fromhex(py::str("FFFFFFFFFFFFFFFFFFFFFFFFFF030000"));
    string key_tmp(0x10, '\0');
    for (int i = 0; i < 0x10; ++i) key_tmp[static_cast<size_t>(i)] = static_cast<char>(cpu_key[i] & wght_mask[i]);
    int64_t wght = XeCryptHammingWeight(mk(key_tmp));
    py::bytes encoded = XeCryptUidEccEncode(mk(key_tmp));
    bool ecc_good = (cpu_key.raw() == encoded.raw());
    bool wght_good = (wght == 0x35);
    return ecc_good && wght_good;
}

py::bytes XeCryptCpuKeyGen() {
    string key(0x10, '\0');
    for (int64_t dw_unset_count = 0; dw_unset_count < 0x35; ++dw_unset_count) {
        py::bytes r = XeCryptRandom(4);
        int64_t dw_rand = (static_cast<int64_t>(u8(r.raw()[0])) | (int64_t(u8(r.raw()[1])) << 8) | (int64_t(u8(r.raw()[2])) << 16) |
                           (int64_t(u8(r.raw()[3])) << 24)) %
                          ((~dw_unset_count) + 0x6A + 1);
        int64_t bit_pos = 0;
        for (bit_pos = 0; bit_pos < 0x6A; ++bit_pos) {
            if (((u8(key[static_cast<size_t>((bit_pos >> 3) & 0x1F)]) >> (bit_pos & 0x7)) & 1) == 0) {
                if (dw_rand == 0) break;
                dw_rand -= 1;
            }
        }
        if (bit_pos == 0x6A) bit_pos = 0x69;   // (a Python for loop leaves its variable on the last value)
        if (bit_pos == 0x6A || dw_rand) std::printf("Error, dw_rand: 0x%llX\n", static_cast<unsigned long long>(dw_rand));
        size_t idx = static_cast<size_t>((bit_pos >> 3) & 0x1F);
        key[idx] = static_cast<char>((int64_t(1) << (bit_pos & 0x7)) ^ u8(key[idx]));
    }
    return XeCryptUidEccEncode(mk(key));
}

py::bytes XeCryptKeyVaultDecrypt(const py::bytes& cpu_key, const py::bytes& data_in) {
    if (!XeCryptCpuKeyValid(cpu_key)) throw py::AssertionError("Invalid CPU key");
    py::bytes kv_hash = hmac_sha(cpu_key, {data_in.slice(0, 0x10)}).slice(0, 0x10);
    XeCryptRc4 rc4 = XeCryptRc4::new_(kv_hash);
    py::bytes data = data_in.slice(0, 0x10) + rc4.decrypt(data_in.slice(0x10, py::len(data_in)));
    kv_hash = hmac_sha(cpu_key, {data.slice(0x10, py::len(data)), VERSION}).slice(0, 0x10);
    if (!(data.slice(0, 0x10).raw() == kv_hash.raw())) throw py::AssertionError("Invalid KV digest");
    return data;
}

py::bytes XeCryptKeyVaultEncrypt(const py::bytes& cpu_key, py::bytearray data) {
    if (!XeCryptCpuKeyValid(cpu_key)) throw py::AssertionError("Invalid CPU key");
    string& buf = data.raw_mut();
    // random nonce
    py::bytes nonce = XeCryptRandom(0x10);
    if (buf.size() < 8) throw py::StructError("pack_into requires a buffer of at least 8 bytes");
    buf.replace(0, 8, nonce.raw().substr(0, 8));   // pack_into("8s", ...) keeps 8 of the 16 bytes
    // random obfuscation key
    py::bytes obf = XeCryptRandom(8);
    if (buf.size() < 0x18) throw py::StructError("pack_into requires a buffer of at least 24 bytes");
    buf.replace(0x10, 8, obf.raw());
    py::bytes mac = hmac_sha(cpu_key, {mk(buf.substr(0x10)), VERSION}).slice(0, 0x10);
    buf.replace(0, 16, mac.raw());
    py::bytes rc4_key = hmac_sha(cpu_key, {mk(buf.substr(0, 0x10))}).slice(0, 0x10);
    return mk(buf.substr(0, 0x10)) + XeCryptRc4::new_(rc4_key).encrypt(mk(buf.substr(0x10)));
}
py::bytes XeCryptKeyVaultEncrypt(const py::bytes& cpu_key, const py::bytes& data) {
    return XeCryptKeyVaultEncrypt(cpu_key, py::bytearray(data));
}

bool XeCryptKeyVaultVerify(const py::bytes& cpu_key, const py::bytes& data, const py::bytes& pub_key) {
    if (!XeCryptCpuKeyValid(cpu_key)) throw py::AssertionError("Invalid CPU key");
    py::bytes kv_data = data.slice(0x18, py::len(data));
    py::bytes kv_hash = hmac_sha(cpu_key, {kv_data.slice(4, 4 + 0xD4), kv_data.slice(0xE8, 0xE8 + 0x1CF8), kv_data.slice(0x1EE0, 0x1EE0 + 0x2108)});
    return XeKeysPkcs1Verify(kv_data.slice(0x1DE0, 0x1DE0 + 0x100), kv_hash, pub_key);
}

py::bytes XeCryptPageEccEncode(py::bytearray data) {
    string& buf = data.raw_mut();
    auto at = [&buf](int64_t i) -> char& {
        if (static_cast<size_t>(i) >= buf.size()) throw py::IndexError("bytearray index out of range");
        return buf[static_cast<size_t>(i)];
    };
    int64_t v1 = 0;
    for (int64_t bit = 0; bit < 0x1066; ++bit) {
        int64_t v2 = v1 ^ (((int64_t(1) << (bit % 8)) & u8(at(bit / 8))) == 0 ? 1 : 0);
        if (v2 & 1) v2 ^= 0x6954559;
        v1 = v2 >> 1;
    }
    for (int64_t bit = 0x1066; bit < 0x1080; ++bit) {
        if (v1 & 1) at(bit / 8) = static_cast<char>(u8(at(bit / 8)) & ~(1 << (bit % 8)) & 0xFF);
        else at(bit / 8) = static_cast<char>(u8(at(bit / 8)) | (1 << (bit % 8)));
        v1 >>= 1;
    }
    return mk(buf);
}
py::bytes XeCryptPageEccEncode(const py::bytes& data) { return XeCryptPageEccEncode(py::bytearray(data)); }

// =====================================================================================================
// managed public key "interfaces"
// =====================================================================================================
namespace {
[[noreturn]] void no_attr_(const char* name) {
    throw py::AttributeError(string("key structure has no attribute '") + name + "'");
}
}  // namespace

XeCryptRsaKey::XeCryptRsaKey(const py::bytes& data) {
    reset();

    key_bytes = data;
    rsa_struct = XECRYPT_RSA::from_buffer_copy(data.slice(0, XECRYPT_RSA::sizeof_));
    bool prv = is_private_key();
    switch (n_size_in_bits()) {
        case 1024: key_struct = prv ? KeyStruct(XECRYPT_RSAPRV_1024::from_buffer_copy(data)) : KeyStruct(XECRYPT_RSAPUB_1024::from_buffer_copy(data)); break;
        case 1536: key_struct = prv ? KeyStruct(XECRYPT_RSAPRV_1536::from_buffer_copy(data)) : KeyStruct(XECRYPT_RSAPUB_1536::from_buffer_copy(data)); break;
        case 2048: key_struct = prv ? KeyStruct(XECRYPT_RSAPRV_2048::from_buffer_copy(data)) : KeyStruct(XECRYPT_RSAPUB_2048::from_buffer_copy(data)); break;
        case 4096: key_struct = prv ? KeyStruct(XECRYPT_RSAPRV_4096::from_buffer_copy(data)) : KeyStruct(XECRYPT_RSAPUB_4096::from_buffer_copy(data)); break;
        default: throw py::Exception("Invalid key data specified");
    }
    // verify key parameters
    if (!verify_parameters()) throw py::AssertionError("Key parameters are incorrect!");
}

void XeCryptRsaKey::reset() {
    key_bytes.reset();
    rsa_struct.reset();
    key_struct.reset();
}

XeCryptRsaKey& XeCryptRsaKey::__enter__() { return *this; }
void XeCryptRsaKey::__exit__() {}

py::bytes XeCryptRsaKey::__bytes__() const { return to_bytes(); }

int64_t XeCryptRsaKey::__len__() const {
    if (!key_bytes.has_value()) throw py::TypeError("object of type 'NoneType' has no len()");
    return py::len(*key_bytes);
}

py::bytes XeCryptRsaKey::to_bytes() const {
    if (!key_bytes.has_value()) throw py::TypeError("'NoneType' is not bytes-like");
    return *key_bytes;
}

std::variant<MemCrypto::RSAPublicKey, RSAPrivateKey> XeCryptRsaKey::to_cryptography() {
    MemCrypto::RSAPublicKey pn{BigInt(static_cast<uint64_t>(e())), n()};
    if (is_private_key()) return RSAPrivateKey{p(), q(), d(), dp(), dq(), inv_q(), pn};
    return pn;
}

XeCryptRsaKey XeCryptRsaKey::new_(int64_t bits, int64_t exp) {
    auto keys = XeCryptBnQwNeRsaKeyGen(bits, exp);
    return XeCryptRsaKey(std::get<1>(keys));
}

XeCryptRsaKey XeCryptRsaKey::public_key() {
    if (!is_private_key()) return *this;
    int64_t size;
    switch (n_size_in_bits()) {   // XECRYPT_RSAPUB_{bits}_SIZE (a private key's size is one of the four)
        case 1024: size = XECRYPT_RSAPUB_1024_SIZE; break;
        case 1536: size = XECRYPT_RSAPUB_1536_SIZE; break;
        case 2048: size = XECRYPT_RSAPUB_2048_SIZE; break;
        case 4096: size = XECRYPT_RSAPUB_4096_SIZE; break;
        default: throw py::Exception("Invalid key data specified");
    }
    return XeCryptRsaKey(to_bytes().slice(0, size));
}

const KeyStruct& XeCryptRsaKey::c_struct() const {
    if (!key_struct.has_value()) throw py::TypeError("key structure is None");
    return *key_struct;
}

bool XeCryptRsaKey::is_private_key() const {
    return __len__() > (XECRYPT_RSA::sizeof_ + n_size_in_bytes());
}

int64_t XeCryptRsaKey::n_size_in_bytes() const { return cqw() * 8; }
int64_t XeCryptRsaKey::half_n_in_bytes() const { return n_size_in_bytes() / 2; }
int64_t XeCryptRsaKey::n_size_in_bits() const { return n_size_in_bytes() * 8; }

py::str XeCryptRsaKey::struct_name() const {
    return py::str(string("XECRYPT_RSA") + (is_private_key() ? "PRV" : "PUB") + "_" + std::to_string(n_size_in_bits()));
}

int64_t XeCryptRsaKey::cqw() const {
    if (!rsa_struct.has_value()) throw py::AttributeError("'NoneType' object has no attribute 'cqw'");
    return rsa_struct->cqw;
}

BigInt XeCryptRsaKey::mod_inv() const {
    QWORD qw = std::visit(
        [](const auto& s) -> QWORD {
            using S = std::decay_t<decltype(s)>;
            if constexpr (S::anonymous) return s.rsa.qwReserved;
            else no_attr_("qwReserved");
        },
        c_struct());
    BigInt v(qw);
    if (qw == 0) v = XeCryptBnQwNeModInv(n());
    return v & UINT64_MASK();
}

BigInt XeCryptRsaKey::n() const {
    return b2i(std::visit([](const auto& s) { return s.n; }, c_struct()), true);
}

int64_t XeCryptRsaKey::e() const {
    return std::visit(
        [](const auto& s) -> int64_t {
            using S = std::decay_t<decltype(s)>;
            if constexpr (S::anonymous) return s.rsa.e;
            else no_attr_("e");
        },
        c_struct());
}

// pow(e, -1, (p - 1) * (q - 1)). A modulus of 0 (p or q is 1) is a ValueError; a p or q of 0 gives a negative
// modulus in Python and a negative result, which a BigInt cannot hold (OverflowError) -- unless the modulus
// is -1, where Python's answer is 0.
BigInt XeCryptRsaKey::d() const {
    BigInt P = p(), Q = q(), one(1);
    if (P == one || Q == one) throw py::ValueError("pow() 3rd argument cannot be 0");
    BigInt E(static_cast<uint64_t>(e()));
    if (P.is_zero() || Q.is_zero()) {
        BigInt m = (P.is_zero() ? one : P - one) * (Q.is_zero() ? one : Q - one);   // |(p - 1) * (q - 1)|
        BigInt r = BigInt::modinv(E, m);
        if (!r.is_zero()) throw py::OverflowError("negative result is not representable");
        return r;
    }
    return BigInt::modinv(E, (P - one) * (Q - one));
}

BigInt XeCryptRsaKey::p() const {
    return b2i(std::visit([](const auto& s) -> py::bytes {
                   if constexpr (std::decay_t<decltype(s)>::is_private) return s.p;
                   else no_attr_("p");
               }, c_struct()), true);
}
BigInt XeCryptRsaKey::q() const {
    return b2i(std::visit([](const auto& s) -> py::bytes {
                   if constexpr (std::decay_t<decltype(s)>::is_private) return s.q;
                   else no_attr_("q");
               }, c_struct()), true);
}
BigInt XeCryptRsaKey::dp() const {
    return b2i(std::visit([](const auto& s) -> py::bytes {
                   if constexpr (std::decay_t<decltype(s)>::is_private) return s.dp;
                   else no_attr_("dp");
               }, c_struct()), true);
}
BigInt XeCryptRsaKey::dq() const {
    return b2i(std::visit([](const auto& s) -> py::bytes {
                   if constexpr (std::decay_t<decltype(s)>::is_private) return s.dq;
                   else no_attr_("dq");
               }, c_struct()), true);
}
BigInt XeCryptRsaKey::u() const {
    return b2i(std::visit([](const auto& s) -> py::bytes {
                   if constexpr (std::decay_t<decltype(s)>::is_private) return s.cr;
                   else no_attr_("cr");
               }, c_struct()), true);
}

BigInt XeCryptRsaKey::r() const {
    return BigInt::powmod(BigInt(2), BigInt(static_cast<uint64_t>(e() - 1)).shl(11), n());
}

BigInt XeCryptRsaKey::inv_r() const { return BigInt::modinv(r(), n()); }
BigInt XeCryptRsaKey::inv_q() const { return BigInt::modinv(q(), p()); }

bool XeCryptRsaKey::verify_parameters() const {
    // public key and private
    BigInt N = n();
    if (N < BigInt(3)) return false;
    int64_t E = e();
    if (E < 3 || !(BigInt(static_cast<uint64_t>(E)) < N)) return false;
    if ((E & 1) == 0) return false;

    if (is_private_key()) {
        if (!(dp() < N)) return false;
        if (!(dq() < N)) return false;
        if (!(u() < N)) return false;
        if (!dp().bit(0)) return false;
        if (!dq().bit(0)) return false;

        BigInt P = p(), Q = q(), one(1);
        BigInt calc_n = P * Q;
        // calc_dp = self.d % (self.p - 1); calc_dq = self.d % (self.q - 1)
        if (P == one || Q == one) throw py::ValueError("pow() 3rd argument cannot be 0");   // self.d: the modulus is 0
        if (P.is_zero() || Q.is_zero()) {
            // self.d is pow(e, -1, a negative or unit modulus): it raises when e is not invertible, and the
            // comparison below is then False (n >= 3 is never p * q = 0)
            BigInt m = (P.is_zero() ? one : P - one) * (Q.is_zero() ? one : Q - one);   // |(p - 1) * (q - 1)|
            BigInt::modinv(BigInt(static_cast<uint64_t>(E)), m);
            return false;
        }
        // d % (p - 1) is pow(e, -1, p - 1) (and d % (q - 1) likewise): (p - 1) divides (p - 1) * (q - 1), and e is
        // invertible mod the product exactly when it is mod both factors -- the same ValueError otherwise, from
        // two half-size inversions instead of one full-size one
        BigInt calc_dp = BigInt::modinv(BigInt(static_cast<uint64_t>(E)), P - one);
        BigInt calc_dq = BigInt::modinv(BigInt(static_cast<uint64_t>(E)), Q - one);

        if (N != calc_n) return false;
        if (dp() != calc_dp) return false;
        if (dq() != calc_dq) return false;
        if (u() != inv_q()) return false;
    }
    return true;
}

py::bytes XeCryptRsaKey::prv_crypt(const BigInt& n_) const {
    if (!is_private_key()) throw py::AssertionError("Key isn't a private key!");
    return i2b(XeCryptBnQwNeModExpRoot(n_, p(), q(), dp(), dq(), u()), n_size_in_bytes(), true);
}
py::bytes XeCryptRsaKey::prv_crypt(const py::bytes& n_) const {
    if (!is_private_key()) throw py::AssertionError("Key isn't a private key!");
    return prv_crypt(b2i(n_, true));
}

py::bytes XeCryptRsaKey::pub_crypt(const BigInt& n_) const {
    return i2b(BigInt::powmod(n_, BigInt(static_cast<uint64_t>(e())), n()), n_size_in_bytes(), true);
}
py::bytes XeCryptRsaKey::pub_crypt(const py::bytes& n_) const { return pub_crypt(b2i(n_, true)); }

py::bytes XeCryptRsaKey::sig_create(const py::bytes& hash, const py::bytes& salt) {
    if (!is_private_key()) throw py::AssertionError("Key isn't a private key!");
    py::bytes sig = XeCryptBnQwBeSigCreate(hash, salt, to_bytes());
    return prv_crypt(sig);
}

bool XeCryptRsaKey::sig_verify(const py::bytes& sig, const py::bytes& hash, const py::bytes& salt) {
    return XeCryptBnQwBeSigVerify(sig, hash, salt, public_key().to_bytes());
}

std::optional<py::bytes> XeCryptRsaKey::sig_create_pkcs1(const py::bytes& hash) {
    if (!is_private_key()) throw py::AssertionError("Key isn't a private key!");
    return XeKeysPkcs1Create(hash, to_bytes());
}

bool XeCryptRsaKey::sig_verify_pkcs1(const py::bytes& sig, const py::bytes& hash) {
    return XeKeysPkcs1Verify(sig, hash, public_key().to_bytes());
}

}  // namespace codebreaker::formats::xbox::crypto::XeCrypt
