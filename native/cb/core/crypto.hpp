// cb/core/crypto.hpp -- hashes and checksums (MD5, SHA-1, SHA-256, SHA-512 through Windows CNG, or in-tree elsewhere;
// CRC-32 and Adler-32 in-tree), and the three things the Xbox formats take from the system: random bytes, block ciphers
// (AES, DES, 3DES) and RSA keys (CNG on Windows, OpenSSL's libcrypto elsewhere: crypto_cipher_win.cpp and
// crypto_cipher_posix.cpp).
//
// One-shot functions take a ByteView and return the digest as a fixed-size array; Hasher streams big inputs.
// `to_hex` renders any byte range as lowercase hex. Modular exponentiation for the Xbox formats is NOT here:
// it lives in cb::crypto::bignum inside formats/xbox (docs/CPP_DESIGN.md 3.4).
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::crypto {

using Md5Digest = std::array<std::uint8_t, 16>;
using Sha1Digest = std::array<std::uint8_t, 20>;
using Sha256Digest = std::array<std::uint8_t, 32>;
using Sha512Digest = std::array<std::uint8_t, 64>;

Md5Digest md5(ByteView data);
Sha1Digest sha1(ByteView data);
Sha256Digest sha256(ByteView data);
Sha512Digest sha512(ByteView data);

// Lowercase hex of any bytes ("deadbeef").
std::string to_hex(ByteView data);
// Hex to bytes; throws cb::FormatError on a non-hex digit or an odd length.
Bytes from_hex(std::string_view hex);

// CRC-32 (the zlib/PNG/ZIP polynomial, reflected) and Adler-32. Pass the previous result as `seed` to continue
// over a second block.
std::uint32_t crc32(ByteView data, std::uint32_t seed = 0);
std::uint32_t adler32(ByteView data, std::uint32_t seed = 1);

// Streaming digest.
class Hasher {
public:
    enum class Algorithm { Md5, Sha1, Sha256, Sha512 };
    explicit Hasher(Algorithm a);
    Hasher(Hasher&&) noexcept;
    Hasher& operator=(Hasher&&) noexcept;
    ~Hasher();
    void update(ByteView data);
    // The digest; the hasher is finished afterwards (update/finish again is a bug).
    Bytes finish();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// ---- random bytes, block ciphers and RSA ------------------------------------------------------------------------------

// Fill `out` from the operating system's random generator. Throws InternalError if there is none.
void random_bytes(ByteSpan out);

// One direction of a block cipher with no padding: whole blocks only (the caller holds partial blocks back).
class Cipher {
public:
    enum class Algorithm { Aes, Des, Des2, Des3 };   // key sizes: AES 16 or 32; DES 8; two-key 3DES 16; three-key 3DES 24
    // Throws InternalError if the system refuses the key.
    Cipher(Algorithm algorithm, ByteView key, bool cbc);
    Cipher(Cipher&&) noexcept;
    Cipher& operator=(Cipher&&) noexcept;
    ~Cipher();
    // `data` (a whole number of blocks) through the cipher. In CBC mode `iv` is the chaining value, one block, and is left as the
    // next call's (the last ciphertext block); in ECB mode it is not used.
    Bytes run(ByteView data, bool encrypt, Bytes& iv);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// An RSA private key's numbers, big-endian: n is bits/8 bytes, p, q, dp, dq and u (q^-1 mod p) half of that, e minimal.
struct RsaNumbers {
    Bytes e, n, p, q, dp, dq, u;
};
// A fresh key of `bits` bits (public exponent 65537). Throws InternalError on failure.
RsaNumbers rsa_generate(unsigned bits);
// PKCS#1 v1.5 signature over a SHA-1 `digest` (20 bytes) with the key e, n, p, q (big-endian): n.size() bytes. nullopt if the
// key is not a valid RSA key or cannot sign.
std::optional<Bytes> rsa_sign_sha1(ByteView e, ByteView n, ByteView p, ByteView q, ByteView digest);
// Whether `signature` (big-endian) is a valid PKCS#1 v1.5 RSA-SHA1 signature of `digest` under the public key e, n.
bool rsa_verify_sha1(ByteView e, ByteView n, ByteView digest, ByteView signature);

}  // namespace cb::crypto
