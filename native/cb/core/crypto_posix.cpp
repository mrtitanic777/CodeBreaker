// cb-unity: alone
// The portable implementation of cb::crypto::Hasher (MD5, SHA-1, SHA-256, SHA-512), used where the system has no hash API
// of its own to call (Windows uses CNG: crypto_win.cpp). Each hash is a block function and the usual padding.
#ifndef _WIN32

#include <array>
#include <bit>
#include <cstring>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"

namespace cb::crypto {

namespace {

std::uint32_t load_le32(const std::uint8_t* p) {
    return static_cast<std::uint32_t>(p[0]) | static_cast<std::uint32_t>(p[1]) << 8 | static_cast<std::uint32_t>(p[2]) << 16 |
           static_cast<std::uint32_t>(p[3]) << 24;
}
std::uint32_t load_be32(const std::uint8_t* p) {
    return static_cast<std::uint32_t>(p[0]) << 24 | static_cast<std::uint32_t>(p[1]) << 16 | static_cast<std::uint32_t>(p[2]) << 8 |
           static_cast<std::uint32_t>(p[3]);
}
std::uint64_t load_be64(const std::uint8_t* p) { return static_cast<std::uint64_t>(load_be32(p)) << 32 | load_be32(p + 4); }
void store_be32(std::uint8_t* p, std::uint32_t v) {
    p[0] = static_cast<std::uint8_t>(v >> 24);
    p[1] = static_cast<std::uint8_t>(v >> 16);
    p[2] = static_cast<std::uint8_t>(v >> 8);
    p[3] = static_cast<std::uint8_t>(v);
}
void store_be64(std::uint8_t* p, std::uint64_t v) {
    store_be32(p, static_cast<std::uint32_t>(v >> 32));
    store_be32(p + 4, static_cast<std::uint32_t>(v));
}
void store_le32(std::uint8_t* p, std::uint32_t v) {
    p[0] = static_cast<std::uint8_t>(v);
    p[1] = static_cast<std::uint8_t>(v >> 8);
    p[2] = static_cast<std::uint8_t>(v >> 16);
    p[3] = static_cast<std::uint8_t>(v >> 24);
}

// ---- the block functions ---------------------------------------------------------------------------------------

void md5_block(std::array<std::uint32_t, 4>& s, const std::uint8_t* block) {
    static constexpr std::uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af,
        0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
        0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97,
        0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
    static constexpr int R[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9,  14, 20, 5, 9,
                                  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                                  4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
    std::uint32_t m[16];
    for (int i = 0; i < 16; ++i) m[i] = load_le32(block + 4 * i);
    std::uint32_t a = s[0], b = s[1], c = s[2], d = s[3];
    for (int i = 0; i < 64; ++i) {
        std::uint32_t f;
        int g;
        if (i < 16) { f = (b & c) | (~b & d); g = i; }
        else if (i < 32) { f = (d & b) | (~d & c); g = (5 * i + 1) % 16; }
        else if (i < 48) { f = b ^ c ^ d; g = (3 * i + 5) % 16; }
        else { f = c ^ (b | ~d); g = (7 * i) % 16; }
        const std::uint32_t next = d;
        d = c;
        c = b;
        b = b + std::rotl(a + f + K[i] + m[g], R[i]);
        a = next;
    }
    s[0] += a; s[1] += b; s[2] += c; s[3] += d;
}

void sha1_block(std::array<std::uint32_t, 5>& s, const std::uint8_t* block) {
    std::uint32_t w[80];
    for (int i = 0; i < 16; ++i) w[i] = load_be32(block + 4 * i);
    for (int i = 16; i < 80; ++i) w[i] = std::rotl(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    std::uint32_t a = s[0], b = s[1], c = s[2], d = s[3], e = s[4];
    for (int i = 0; i < 80; ++i) {
        std::uint32_t f, k;
        if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
        else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
        else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
        else { f = b ^ c ^ d; k = 0xCA62C1D6; }
        const std::uint32_t t = std::rotl(a, 5) + f + e + k + w[i];
        e = d; d = c; c = std::rotl(b, 30); b = a; a = t;
    }
    s[0] += a; s[1] += b; s[2] += c; s[3] += d; s[4] += e;
}

void sha256_block(std::array<std::uint32_t, 8>& s, const std::uint8_t* block) {
    static constexpr std::uint32_t K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01,
        0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
        0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08,
        0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
    std::uint32_t w[64];
    for (int i = 0; i < 16; ++i) w[i] = load_be32(block + 4 * i);
    for (int i = 16; i < 64; ++i) {
        const std::uint32_t s0 = std::rotr(w[i - 15], 7) ^ std::rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        const std::uint32_t s1 = std::rotr(w[i - 2], 17) ^ std::rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    std::uint32_t a = s[0], b = s[1], c = s[2], d = s[3], e = s[4], f = s[5], g = s[6], h = s[7];
    for (int i = 0; i < 64; ++i) {
        const std::uint32_t t1 = h + (std::rotr(e, 6) ^ std::rotr(e, 11) ^ std::rotr(e, 25)) + ((e & f) ^ (~e & g)) + K[i] + w[i];
        const std::uint32_t t2 = (std::rotr(a, 2) ^ std::rotr(a, 13) ^ std::rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
        h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    }
    s[0] += a; s[1] += b; s[2] += c; s[3] += d; s[4] += e; s[5] += f; s[6] += g; s[7] += h;
}

void sha512_block(std::array<std::uint64_t, 8>& s, const std::uint8_t* block) {
    static constexpr std::uint64_t K[80] = {
        0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
        0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
        0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
        0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
        0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
        0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
        0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
        0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
        0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
        0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
        0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
        0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
        0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
        0x5fcb6fab3ad6faec, 0x6c44198c4a475817};
    std::uint64_t w[80];
    for (int i = 0; i < 16; ++i) w[i] = load_be64(block + 8 * i);
    for (int i = 16; i < 80; ++i) {
        const std::uint64_t s0 = std::rotr(w[i - 15], 1) ^ std::rotr(w[i - 15], 8) ^ (w[i - 15] >> 7);
        const std::uint64_t s1 = std::rotr(w[i - 2], 19) ^ std::rotr(w[i - 2], 61) ^ (w[i - 2] >> 6);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    std::uint64_t a = s[0], b = s[1], c = s[2], d = s[3], e = s[4], f = s[5], g = s[6], h = s[7];
    for (int i = 0; i < 80; ++i) {
        const std::uint64_t t1 = h + (std::rotr(e, 14) ^ std::rotr(e, 18) ^ std::rotr(e, 41)) + ((e & f) ^ (~e & g)) + K[i] + w[i];
        const std::uint64_t t2 = (std::rotr(a, 28) ^ std::rotr(a, 34) ^ std::rotr(a, 39)) + ((a & b) ^ (a & c) ^ (b & c));
        h = g; g = f; f = e; e = d + t1; d = c; c = b; b = a; a = t1 + t2;
    }
    s[0] += a; s[1] += b; s[2] += c; s[3] += d; s[4] += e; s[5] += f; s[6] += g; s[7] += h;
}

}  // namespace

// One streaming hash of any of the four: the state words, the block buffer and the length.
struct Hasher::Impl {
    Algorithm algorithm;
    std::array<std::uint32_t, 8> s32{};
    std::array<std::uint64_t, 8> s64{};
    std::array<std::uint8_t, 128> buffer{};
    std::size_t filled = 0;
    std::uint64_t total = 0;   // bytes in
    bool finished = false;

    explicit Impl(Algorithm a) : algorithm(a) {
        switch (a) {
            case Algorithm::Md5: s32 = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476}; break;
            case Algorithm::Sha1: s32 = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0}; break;
            case Algorithm::Sha256:
                s32 = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
                break;
            case Algorithm::Sha512:
                s64 = {0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
                       0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179};
                break;
        }
    }
    std::size_t block_size() const { return algorithm == Algorithm::Sha512 ? 128 : 64; }
    void block(const std::uint8_t* p) {
        switch (algorithm) {
            case Algorithm::Md5: {
                std::array<std::uint32_t, 4> s{s32[0], s32[1], s32[2], s32[3]};
                md5_block(s, p);
                std::copy(s.begin(), s.end(), s32.begin());
                break;
            }
            case Algorithm::Sha1: {
                std::array<std::uint32_t, 5> s{s32[0], s32[1], s32[2], s32[3], s32[4]};
                sha1_block(s, p);
                std::copy(s.begin(), s.end(), s32.begin());
                break;
            }
            case Algorithm::Sha256: sha256_block(s32, p); break;
            case Algorithm::Sha512: sha512_block(s64, p); break;
        }
    }
};

Hasher::Hasher(Algorithm a) : impl_(std::make_unique<Impl>(a)) {}
Hasher::Hasher(Hasher&&) noexcept = default;
Hasher& Hasher::operator=(Hasher&&) noexcept = default;
Hasher::~Hasher() = default;

void Hasher::update(ByteView data) {
    CB_ASSERT(!impl_->finished, "Hasher::update after finish");
    Impl& h = *impl_;
    const std::size_t size = h.block_size();
    h.total += data.size();
    while (!data.empty()) {
        if (h.filled == 0 && data.size() >= size) {
            h.block(data.data());
            data = data.subspan(size);
            continue;
        }
        const std::size_t take = std::min(size - h.filled, data.size());
        std::memcpy(h.buffer.data() + h.filled, data.data(), take);
        h.filled += take;
        data = data.subspan(take);
        if (h.filled == size) {
            h.block(h.buffer.data());
            h.filled = 0;
        }
    }
}

Bytes Hasher::finish() {
    CB_ASSERT(!impl_->finished, "Hasher::finish twice");
    Impl& h = *impl_;
    const std::size_t size = h.block_size(), length_bytes = size / 8;
    const std::uint64_t bits = h.total * 8;
    h.buffer[h.filled++] = 0x80;
    if (h.filled > size - length_bytes) {
        std::fill(h.buffer.begin() + static_cast<std::ptrdiff_t>(h.filled), h.buffer.begin() + static_cast<std::ptrdiff_t>(size), 0);
        h.block(h.buffer.data());
        h.filled = 0;
    }
    std::fill(h.buffer.begin() + static_cast<std::ptrdiff_t>(h.filled), h.buffer.begin() + static_cast<std::ptrdiff_t>(size), 0);
    if (h.algorithm == Algorithm::Md5) store_le32(h.buffer.data() + size - 8, static_cast<std::uint32_t>(bits)),
        store_le32(h.buffer.data() + size - 4, static_cast<std::uint32_t>(bits >> 32));
    else store_be64(h.buffer.data() + size - 8, bits);   // (SHA-512's length is 128 bits: the high 64 stay zero)
    h.block(h.buffer.data());
    h.finished = true;

    Bytes out;
    switch (h.algorithm) {
        case Algorithm::Md5:
            out.resize(16);
            for (int i = 0; i < 4; ++i) store_le32(out.data() + 4 * i, h.s32[static_cast<std::size_t>(i)]);
            break;
        case Algorithm::Sha1:
            out.resize(20);
            for (int i = 0; i < 5; ++i) store_be32(out.data() + 4 * i, h.s32[static_cast<std::size_t>(i)]);
            break;
        case Algorithm::Sha256:
            out.resize(32);
            for (int i = 0; i < 8; ++i) store_be32(out.data() + 4 * i, h.s32[static_cast<std::size_t>(i)]);
            break;
        case Algorithm::Sha512:
            out.resize(64);
            for (int i = 0; i < 8; ++i) store_be64(out.data() + 8 * i, h.s64[static_cast<std::size_t>(i)]);
            break;
    }
    return out;
}

}  // namespace cb::crypto

#endif  // !_WIN32
