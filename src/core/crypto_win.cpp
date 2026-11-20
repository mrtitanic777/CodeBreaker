// The Windows implementation of cb::crypto::Hasher: CNG.
#ifdef _WIN32
#include "core/crypto.hpp"

#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <format>

#include "core/error.hpp"

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

}  // namespace cb::crypto

#endif  // _WIN32
