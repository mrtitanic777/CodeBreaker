// cb-unity: alone
// The Windows implementation of cb::crypto's random bytes, block ciphers and RSA (crypto.hpp): CNG.
#ifdef _WIN32
#include <windows.h>   // CNG (bcrypt.h) needs it; confined to this file
#include <bcrypt.h>

#include <cstring>
#include <cwchar>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/io.hpp"

namespace cb::crypto {

void random_bytes(ByteSpan out) {
    if (out.empty()) return;
    if (BCryptGenRandom(nullptr, out.data(), static_cast<ULONG>(out.size()), BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0) throw InternalError("CNG: no random bytes");
}

// ---- block ciphers -----------------------------------------------------------------------------------------------

struct Cipher::Impl {
    BCRYPT_ALG_HANDLE alg = nullptr;
    BCRYPT_KEY_HANDLE key = nullptr;
    bool cbc = false;
    ~Impl() {
        if (key) BCryptDestroyKey(key);   // (the key goes before its provider)
        if (alg) BCryptCloseAlgorithmProvider(alg, 0);
    }
};

Cipher::Cipher(Algorithm algorithm, ByteView key, bool cbc) : impl_(std::make_unique<Impl>()) {
    const wchar_t* name = BCRYPT_AES_ALGORITHM;
    switch (algorithm) {
        case Algorithm::Aes: name = BCRYPT_AES_ALGORITHM; break;
        case Algorithm::Des: name = BCRYPT_DES_ALGORITHM; break;
        case Algorithm::Des2: name = BCRYPT_3DES_112_ALGORITHM; break;
        case Algorithm::Des3: name = BCRYPT_3DES_ALGORITHM; break;
    }
    impl_->cbc = cbc;
    if (BCryptOpenAlgorithmProvider(&impl_->alg, name, nullptr, 0) != 0) throw InternalError("CNG: no such cipher");
    const wchar_t* chain = cbc ? BCRYPT_CHAIN_MODE_CBC : BCRYPT_CHAIN_MODE_ECB;
    if (BCryptSetProperty(impl_->alg, BCRYPT_CHAINING_MODE, reinterpret_cast<PUCHAR>(const_cast<wchar_t*>(chain)), static_cast<ULONG>((wcslen(chain) + 1) * sizeof(wchar_t)), 0) != 0)
        throw InternalError("CNG: cannot set the chaining mode");
    if (BCryptGenerateSymmetricKey(impl_->alg, &impl_->key, nullptr, 0, const_cast<PUCHAR>(key.data()), static_cast<ULONG>(key.size()), 0) != 0)
        throw InternalError("CNG: the key was refused");
}

Cipher::Cipher(Cipher&&) noexcept = default;
Cipher& Cipher::operator=(Cipher&&) noexcept = default;
Cipher::~Cipher() = default;

Bytes Cipher::run(ByteView data, bool encrypt, Bytes& iv) {
    Bytes out(data.size());
    if (data.empty()) return out;
    ULONG written = 0;
    PUCHAR iv_data = impl_->cbc ? iv.data() : nullptr;
    const ULONG iv_size = impl_->cbc ? static_cast<ULONG>(iv.size()) : 0;
    const NTSTATUS st = encrypt ? BCryptEncrypt(impl_->key, const_cast<PUCHAR>(data.data()), static_cast<ULONG>(data.size()), nullptr, iv_data, iv_size, out.data(), static_cast<ULONG>(out.size()), &written, 0)
                                : BCryptDecrypt(impl_->key, const_cast<PUCHAR>(data.data()), static_cast<ULONG>(data.size()), nullptr, iv_data, iv_size, out.data(), static_cast<ULONG>(out.size()), &written, 0);
    if (st != 0 || written != out.size()) throw InternalError("CNG: the cipher failed");
    return out;
}

// ---- RSA ---------------------------------------------------------------------------------------------------------

namespace {

struct Provider {
    BCRYPT_ALG_HANDLE h = nullptr;
    Provider() {
        if (BCryptOpenAlgorithmProvider(&h, BCRYPT_RSA_ALGORITHM, nullptr, 0) != 0) throw InternalError("CNG: no RSA provider");
    }
    ~Provider() { BCryptCloseAlgorithmProvider(h, 0); }
};

struct KeyHandle {
    BCRYPT_KEY_HANDLE h = nullptr;
    ~KeyHandle() {
        if (h) BCryptDestroyKey(h);
    }
};

// A BCRYPT_RSAKEY_BLOB: the header, then the public exponent, modulus (and primes), big-endian.
Bytes rsa_blob(ULONG magic, ByteView e, ByteView n, ByteView p, ByteView q) {
    BCRYPT_RSAKEY_BLOB h{};
    h.Magic = magic;
    h.BitLength = static_cast<ULONG>(n.size() * 8);
    h.cbPublicExp = static_cast<ULONG>(e.size());
    h.cbModulus = static_cast<ULONG>(n.size());
    h.cbPrime1 = static_cast<ULONG>(p.size());
    h.cbPrime2 = static_cast<ULONG>(q.size());
    Bytes out(sizeof h);
    std::memcpy(out.data(), &h, sizeof h);
    append(out, e);
    append(out, n);
    append(out, p);
    append(out, q);
    return out;
}

ByteView minimal(ByteView v) {   // without leading zeros (but never empty)
    std::size_t i = 0;
    while (i + 1 < v.size() && v[i] == 0) ++i;
    return v.subspan(i);
}

}  // namespace

RsaNumbers rsa_generate(unsigned bits) {
    Provider prov;
    KeyHandle key;
    if (BCryptGenerateKeyPair(prov.h, &key.h, bits, 0) != 0 || BCryptFinalizeKeyPair(key.h, 0) != 0) throw InternalError("CNG: key generation failed");
    ULONG size = 0;
    BCryptExportKey(key.h, nullptr, BCRYPT_RSAFULLPRIVATE_BLOB, nullptr, 0, &size, 0);
    Bytes blob(size);
    if (BCryptExportKey(key.h, nullptr, BCRYPT_RSAFULLPRIVATE_BLOB, blob.data(), size, &size, 0) != 0) throw InternalError("CNG: cannot export the key");
    BCRYPT_RSAKEY_BLOB header{};
    std::memcpy(&header, blob.data(), sizeof header);
    ByteView rest = ByteView(blob).subspan(sizeof header);
    auto take = [&rest](std::size_t n) {
        const ByteView v = rest.first(n);
        rest = rest.subspan(n);
        return Bytes(v.begin(), v.end());
    };
    RsaNumbers k;
    k.e = take(header.cbPublicExp);
    k.n = take(header.cbModulus);
    k.p = take(header.cbPrime1);
    k.q = take(header.cbPrime2);
    k.dp = take(header.cbPrime1);
    k.dq = take(header.cbPrime2);
    k.u = take(header.cbPrime1);
    return k;
}

std::optional<Bytes> rsa_sign_sha1(ByteView e, ByteView n, ByteView p, ByteView q, ByteView digest) {
    const Bytes blob = rsa_blob(BCRYPT_RSAPRIVATE_MAGIC, minimal(e), n, p, q);
    Provider prov;
    KeyHandle key;
    if (BCryptImportKeyPair(prov.h, nullptr, BCRYPT_RSAPRIVATE_BLOB, &key.h, const_cast<PUCHAR>(blob.data()), static_cast<ULONG>(blob.size()), 0) != 0) return std::nullopt;
    BCRYPT_PKCS1_PADDING_INFO pad{BCRYPT_SHA1_ALGORITHM};
    Bytes sig(n.size());
    ULONG written = 0;
    if (BCryptSignHash(key.h, &pad, const_cast<PUCHAR>(digest.data()), static_cast<ULONG>(digest.size()), sig.data(), static_cast<ULONG>(sig.size()), &written, BCRYPT_PAD_PKCS1) != 0 ||
        written != sig.size())
        return std::nullopt;
    return sig;
}

bool rsa_verify_sha1(ByteView e, ByteView n, ByteView digest, ByteView signature) {
    const Bytes blob = rsa_blob(BCRYPT_RSAPUBLIC_MAGIC, minimal(e), n, {}, {});
    Provider prov;
    KeyHandle key;
    if (BCryptImportKeyPair(prov.h, nullptr, BCRYPT_RSAPUBLIC_BLOB, &key.h, const_cast<PUCHAR>(blob.data()), static_cast<ULONG>(blob.size()), 0) != 0) return false;
    BCRYPT_PKCS1_PADDING_INFO pad{BCRYPT_SHA1_ALGORITHM};
    return BCryptVerifySignature(key.h, &pad, const_cast<PUCHAR>(digest.data()), static_cast<ULONG>(digest.size()), const_cast<PUCHAR>(signature.data()),
                                 static_cast<ULONG>(signature.size()), BCRYPT_PAD_PKCS1) == 0;
}

}  // namespace cb::crypto

#endif  // _WIN32
