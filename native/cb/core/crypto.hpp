// cb/core/crypto.hpp -- hashes and checksums (MD5, SHA-1, SHA-256, SHA-512 through Windows CNG; CRC-32 and
// Adler-32 in-tree).
//
// One-shot functions take a ByteView and return the digest as a fixed-size array; Hasher streams big inputs.
// `to_hex` renders any byte range as lowercase hex. Modular exponentiation for the Xbox formats is NOT here:
// it lives in cb::crypto::bignum inside formats/xbox (docs/CPP_DESIGN.md 3.4).
#pragma once

#include <array>
#include <cstdint>
#include <memory>
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

}  // namespace cb::crypto
