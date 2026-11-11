#include "cb/core/crypto.hpp"

#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"

namespace cb::crypto {

// ---- CNG hashes ---------------------------------------------------------------------------------------

struct Hasher::Impl {
    BCRYPT_ALG_HANDLE alg = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    std::size_t digest_size = 0;
    bool finished = false;

    ~Impl() {
        if (hash != nullptr) BCryptDestroyHash(hash);
        if (alg != nullptr) BCryptCloseAlgorithmProvider(alg, 0);
    }
};

namespace {

void check(NTSTATUS status, const char* what) {
    if (status < 0) throw InternalError(std::format("CNG {} failed: 0x{:08x}", what, static_cast<unsigned>(status)));
}

const wchar_t* algorithm_name(Hasher::Algorithm a) {
    switch (a) {
        case Hasher::Algorithm::Md5: return BCRYPT_MD5_ALGORITHM;
        case Hasher::Algorithm::Sha1: return BCRYPT_SHA1_ALGORITHM;
        case Hasher::Algorithm::Sha256: return BCRYPT_SHA256_ALGORITHM;
        case Hasher::Algorithm::Sha512: return BCRYPT_SHA512_ALGORITHM;
    }
    return BCRYPT_SHA256_ALGORITHM;
}

template <class Digest>
Digest digest_of(Hasher::Algorithm a, ByteView data) {
    Hasher h(a);
    h.update(data);
    Bytes d = h.finish();
    Digest out{};
    CB_ASSERT(d.size() == out.size(), "digest size");
    std::copy(d.begin(), d.end(), out.begin());
    return out;
}

}  // namespace

Hasher::Hasher(Algorithm a) : impl_(std::make_unique<Impl>()) {
    check(BCryptOpenAlgorithmProvider(&impl_->alg, algorithm_name(a), nullptr, 0), "open provider");
    ULONG size = 0, got = 0;
    check(BCryptGetProperty(impl_->alg, BCRYPT_HASH_LENGTH, reinterpret_cast<PUCHAR>(&size), sizeof(size), &got, 0),
          "hash length");
    impl_->digest_size = size;
    check(BCryptCreateHash(impl_->alg, &impl_->hash, nullptr, 0, nullptr, 0, 0), "create hash");
}

Hasher::Hasher(Hasher&&) noexcept = default;
Hasher& Hasher::operator=(Hasher&&) noexcept = default;
Hasher::~Hasher() = default;

void Hasher::update(ByteView data) {
    CB_ASSERT(!impl_->finished, "Hasher::update after finish");
    // CNG takes a 32-bit length: feed big inputs in pieces.
    while (!data.empty()) {
        const ULONG chunk = static_cast<ULONG>(std::min<std::size_t>(data.size(), 1u << 30));
        check(BCryptHashData(impl_->hash, const_cast<PUCHAR>(data.data()), chunk, 0), "hash data");
        data = data.subspan(chunk);
    }
}

Bytes Hasher::finish() {
    CB_ASSERT(!impl_->finished, "Hasher::finish twice");
    Bytes out(impl_->digest_size);
    check(BCryptFinishHash(impl_->hash, out.data(), static_cast<ULONG>(out.size()), 0), "finish hash");
    impl_->finished = true;
    return out;
}

Md5Digest md5(ByteView data) { return digest_of<Md5Digest>(Hasher::Algorithm::Md5, data); }
Sha1Digest sha1(ByteView data) { return digest_of<Sha1Digest>(Hasher::Algorithm::Sha1, data); }
Sha256Digest sha256(ByteView data) { return digest_of<Sha256Digest>(Hasher::Algorithm::Sha256, data); }
Sha512Digest sha512(ByteView data) { return digest_of<Sha512Digest>(Hasher::Algorithm::Sha512, data); }

// ---- hex ----------------------------------------------------------------------------------------------

std::string to_hex(ByteView data) {
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string out;
    out.reserve(data.size() * 2);
    for (std::uint8_t b : data) {
        out.push_back(kDigits[b >> 4]);
        out.push_back(kDigits[b & 15]);
    }
    return out;
}

Bytes from_hex(std::string_view hex) {
    if (hex.size() % 2 != 0) throw FormatError("hex", hex.size(), "odd number of hex digits");
    auto nibble = [&](std::size_t at) -> std::uint8_t {
        const char c = hex[at];
        if (c >= '0' && c <= '9') return static_cast<std::uint8_t>(c - '0');
        if (c >= 'a' && c <= 'f') return static_cast<std::uint8_t>(c - 'a' + 10);
        if (c >= 'A' && c <= 'F') return static_cast<std::uint8_t>(c - 'A' + 10);
        throw FormatError("hex", at, "not a hex digit");
    };
    Bytes out(hex.size() / 2);
    for (std::size_t i = 0; i < out.size(); ++i) out[i] = static_cast<std::uint8_t>((nibble(2 * i) << 4) | nibble(2 * i + 1));
    return out;
}

// ---- CRC-32 and Adler-32 ------------------------------------------------------------------------------

namespace {

const std::array<std::uint32_t, 256>& crc_table() {
    static const std::array<std::uint32_t, 256> table = [] {
        std::array<std::uint32_t, 256> t{};
        for (std::uint32_t i = 0; i < 256; ++i) {
            std::uint32_t c = i;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            t[i] = c;
        }
        return t;
    }();
    return table;
}

}  // namespace

std::uint32_t crc32(ByteView data, std::uint32_t seed) {
    const auto& table = crc_table();
    std::uint32_t c = seed ^ 0xFFFFFFFFu;
    for (std::uint8_t b : data) c = table[(c ^ b) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

std::uint32_t adler32(ByteView data, std::uint32_t seed) {
    constexpr std::uint32_t kMod = 65521;
    std::uint32_t a = seed & 0xFFFF, b = seed >> 16;
    for (std::uint8_t byte : data) {
        a = (a + byte) % kMod;
        b = (b + a) % kMod;
    }
    return (b << 16) | a;
}

}  // namespace cb::crypto
