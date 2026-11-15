// cb-unity: alone
// The POSIX implementation of cb::crypto's random bytes, block ciphers and RSA (crypto.hpp): OpenSSL's libcrypto (3.x; the
// EVP interface only). Windows uses CNG (crypto_cipher_win.cpp).
#ifndef _WIN32

#include <openssl/bn.h>
#include <openssl/core_names.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/param_build.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <memory>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"

namespace cb::crypto {

namespace {

struct CtxDeleter {
    void operator()(EVP_CIPHER_CTX* c) const { EVP_CIPHER_CTX_free(c); }
    void operator()(EVP_PKEY* k) const { EVP_PKEY_free(k); }
    void operator()(EVP_PKEY_CTX* c) const { EVP_PKEY_CTX_free(c); }
    void operator()(BIGNUM* b) const { BN_free(b); }
    void operator()(BN_CTX* c) const { BN_CTX_free(c); }
    void operator()(OSSL_PARAM_BLD* b) const { OSSL_PARAM_BLD_free(b); }
    void operator()(OSSL_PARAM* p) const { OSSL_PARAM_free(p); }
};
template <class T>
using Owned = std::unique_ptr<T, CtxDeleter>;

[[noreturn]] void fail(const char* what) {
    ERR_clear_error();
    throw InternalError(std::string("OpenSSL: ") + what);
}

Owned<BIGNUM> big(ByteView v) {
    BIGNUM* b = BN_bin2bn(v.data(), static_cast<int>(v.size()), nullptr);
    if (!b) fail("no memory for a number");
    return Owned<BIGNUM>(b);
}

Bytes to_bytes(const BIGNUM* b, std::size_t width) {
    Bytes out(width);
    if (BN_bn2binpad(b, out.data(), static_cast<int>(width)) < 0) fail("a number does not fit");
    return out;
}

Bytes minimal_bytes(const BIGNUM* b) {   // big-endian without leading zeros (one 0 byte for zero)
    Bytes out(static_cast<std::size_t>(BN_num_bytes(b)));
    if (out.empty()) return Bytes(1, 0);
    BN_bn2bin(b, out.data());
    return out;
}

}  // namespace

void random_bytes(ByteSpan out) {
    if (out.empty()) return;
    if (RAND_bytes(out.data(), static_cast<int>(out.size())) != 1) fail("no random bytes");
}

// ---- block ciphers -----------------------------------------------------------------------------------------------

struct Cipher::Impl {
    Owned<EVP_CIPHER_CTX> encrypt_ctx, decrypt_ctx;   // (the context's direction is fixed when it is set up)
    const EVP_CIPHER* cipher = nullptr;
    Bytes key;
    bool cbc = false;
};

Cipher::Cipher(Algorithm algorithm, ByteView key, bool cbc) : impl_(std::make_unique<Impl>()) {
    impl_->cbc = cbc;
    switch (algorithm) {
        case Algorithm::Aes:
            if (key.size() != 16 && key.size() != 32) throw InternalError("OpenSSL: the AES key was refused");
            impl_->cipher = key.size() == 16 ? (cbc ? EVP_aes_128_cbc() : EVP_aes_128_ecb()) : (cbc ? EVP_aes_256_cbc() : EVP_aes_256_ecb());
            impl_->key.assign(key.begin(), key.end());
            break;
        case Algorithm::Des:   // (single DES is in OpenSSL 3's legacy provider only: three-key 3DES with one key is the same cipher)
            if (key.size() != 8) throw InternalError("OpenSSL: the DES key was refused");
            impl_->cipher = cbc ? EVP_des_ede3_cbc() : EVP_des_ede3_ecb();
            for (int i = 0; i < 3; ++i) impl_->key.insert(impl_->key.end(), key.begin(), key.end());
            break;
        case Algorithm::Des2:   // K1 K2 K1
            if (key.size() != 16) throw InternalError("OpenSSL: the 3DES key was refused");
            impl_->cipher = cbc ? EVP_des_ede3_cbc() : EVP_des_ede3_ecb();
            impl_->key.assign(key.begin(), key.end());
            impl_->key.insert(impl_->key.end(), key.begin(), key.begin() + 8);
            break;
        case Algorithm::Des3:
            if (key.size() != 24) throw InternalError("OpenSSL: the 3DES key was refused");
            impl_->cipher = cbc ? EVP_des_ede3_cbc() : EVP_des_ede3_ecb();
            impl_->key.assign(key.begin(), key.end());
            break;
    }
    if (!impl_->cipher) fail("no such cipher");
}

Cipher::Cipher(Cipher&&) noexcept = default;
Cipher& Cipher::operator=(Cipher&&) noexcept = default;
Cipher::~Cipher() = default;

Bytes Cipher::run(ByteView data, bool encrypt, Bytes& iv) {
    Bytes out(data.size());
    if (data.empty()) return out;
    // A context per call: it takes the chaining value `iv` as it stands, and the next one is read back from the data.
    Owned<EVP_CIPHER_CTX> ctx(EVP_CIPHER_CTX_new());
    if (!ctx) fail("no memory for a cipher");
    if (EVP_CipherInit_ex(ctx.get(), impl_->cipher, nullptr, impl_->key.data(), impl_->cbc ? iv.data() : nullptr, encrypt ? 1 : 0) != 1) fail("the key was refused");
    EVP_CIPHER_CTX_set_padding(ctx.get(), 0);
    int written = 0;
    if (EVP_CipherUpdate(ctx.get(), out.data(), &written, data.data(), static_cast<int>(data.size())) != 1 || static_cast<std::size_t>(written) != out.size())
        fail("the cipher failed");
    if (impl_->cbc) {   // the next block chains from this call's last ciphertext block
        const std::size_t block = iv.size();
        const ByteView last_cipher = encrypt ? ByteView(out) : data;
        if (last_cipher.size() >= block) iv.assign(last_cipher.end() - static_cast<std::ptrdiff_t>(block), last_cipher.end());
    }
    return out;
}

// ---- RSA ---------------------------------------------------------------------------------------------------------

RsaNumbers rsa_generate(unsigned bits) {
    Owned<EVP_PKEY_CTX> ctx(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr));
    if (!ctx || EVP_PKEY_keygen_init(ctx.get()) != 1 || EVP_PKEY_CTX_set_rsa_keygen_bits(ctx.get(), static_cast<int>(bits)) != 1) fail("key generation failed");
    EVP_PKEY* raw = nullptr;
    if (EVP_PKEY_keygen(ctx.get(), &raw) != 1) fail("key generation failed");
    Owned<EVP_PKEY> key(raw);
    auto param = [&](const char* name) {
        BIGNUM* b = nullptr;
        if (EVP_PKEY_get_bn_param(key.get(), name, &b) != 1) fail("cannot export the key");
        return Owned<BIGNUM>(b);
    };
    const std::size_t nb = bits / 8, half = nb / 2;
    RsaNumbers k;
    k.e = minimal_bytes(param(OSSL_PKEY_PARAM_RSA_E).get());
    k.n = to_bytes(param(OSSL_PKEY_PARAM_RSA_N).get(), nb);
    k.p = to_bytes(param(OSSL_PKEY_PARAM_RSA_FACTOR1).get(), half);
    k.q = to_bytes(param(OSSL_PKEY_PARAM_RSA_FACTOR2).get(), half);
    k.dp = to_bytes(param(OSSL_PKEY_PARAM_RSA_EXPONENT1).get(), half);
    k.dq = to_bytes(param(OSSL_PKEY_PARAM_RSA_EXPONENT2).get(), half);
    k.u = to_bytes(param(OSSL_PKEY_PARAM_RSA_COEFFICIENT1).get(), half);
    return k;
}

namespace {

// An RSA key from its numbers: the public ones (e, n), and with p and q the private ones too (d and the CRT values worked out).
Owned<EVP_PKEY> rsa_key(ByteView e, ByteView n, ByteView p, ByteView q) {
    Owned<BN_CTX> bn_ctx(BN_CTX_new());
    Owned<OSSL_PARAM_BLD> build(OSSL_PARAM_BLD_new());
    if (!bn_ctx || !build) return {};
    const Owned<BIGNUM> bn_e = big(e), bn_n = big(n);
    if (!OSSL_PARAM_BLD_push_BN(build.get(), OSSL_PKEY_PARAM_RSA_N, bn_n.get()) || !OSSL_PARAM_BLD_push_BN(build.get(), OSSL_PKEY_PARAM_RSA_E, bn_e.get())) return {};
    if (!p.empty()) {
        const Owned<BIGNUM> bn_p = big(p), bn_q = big(q);
        Owned<BIGNUM> p1(BN_dup(bn_p.get())), q1(BN_dup(bn_q.get())), phi(BN_new()), d(BN_new()), dp(BN_new()), dq(BN_new()), u(BN_new());
        if (!p1 || !q1 || !phi || !d || !dp || !dq || !u) return {};
        BN_sub_word(p1.get(), 1);
        BN_sub_word(q1.get(), 1);
        // d = e^-1 mod (p-1)(q-1); dp = d mod (p-1); dq = d mod (q-1); u = q^-1 mod p
        if (!BN_mul(phi.get(), p1.get(), q1.get(), bn_ctx.get()) || !BN_mod_inverse(d.get(), bn_e.get(), phi.get(), bn_ctx.get()) ||
            !BN_mod(dp.get(), d.get(), p1.get(), bn_ctx.get()) || !BN_mod(dq.get(), d.get(), q1.get(), bn_ctx.get()) ||
            !BN_mod_inverse(u.get(), bn_q.get(), bn_p.get(), bn_ctx.get())) {
            ERR_clear_error();
            return {};
        }
        if (!OSSL_PARAM_BLD_push_BN(build.get(), OSSL_PKEY_PARAM_RSA_D, d.get()) || !OSSL_PARAM_BLD_push_BN(build.get(), OSSL_PKEY_PARAM_RSA_FACTOR1, bn_p.get()) ||
            !OSSL_PARAM_BLD_push_BN(build.get(), OSSL_PKEY_PARAM_RSA_FACTOR2, bn_q.get()) || !OSSL_PARAM_BLD_push_BN(build.get(), OSSL_PKEY_PARAM_RSA_EXPONENT1, dp.get()) ||
            !OSSL_PARAM_BLD_push_BN(build.get(), OSSL_PKEY_PARAM_RSA_EXPONENT2, dq.get()) || !OSSL_PARAM_BLD_push_BN(build.get(), OSSL_PKEY_PARAM_RSA_COEFFICIENT1, u.get()))
            return {};
    }
    Owned<OSSL_PARAM> params(OSSL_PARAM_BLD_to_param(build.get()));
    Owned<EVP_PKEY_CTX> ctx(EVP_PKEY_CTX_new_from_name(nullptr, "RSA", nullptr));
    if (!params || !ctx || EVP_PKEY_fromdata_init(ctx.get()) != 1) return {};
    EVP_PKEY* raw = nullptr;
    if (EVP_PKEY_fromdata(ctx.get(), &raw, p.empty() ? EVP_PKEY_PUBLIC_KEY : EVP_PKEY_KEYPAIR, params.get()) != 1) {
        ERR_clear_error();
        return {};
    }
    return Owned<EVP_PKEY>(raw);
}

}  // namespace

std::optional<Bytes> rsa_sign_sha1(ByteView e, ByteView n, ByteView p, ByteView q, ByteView digest) {
    const Owned<EVP_PKEY> key = rsa_key(e, n, p, q);
    if (!key) return std::nullopt;
    Owned<EVP_PKEY_CTX> ctx(EVP_PKEY_CTX_new(key.get(), nullptr));
    if (!ctx || EVP_PKEY_sign_init(ctx.get()) != 1 || EVP_PKEY_CTX_set_rsa_padding(ctx.get(), RSA_PKCS1_PADDING) != 1 || EVP_PKEY_CTX_set_signature_md(ctx.get(), EVP_sha1()) != 1) {
        ERR_clear_error();
        return std::nullopt;
    }
    Bytes sig(n.size());
    std::size_t length = sig.size();
    if (EVP_PKEY_sign(ctx.get(), sig.data(), &length, digest.data(), digest.size()) != 1 || length != sig.size()) {
        ERR_clear_error();
        return std::nullopt;
    }
    return sig;
}

bool rsa_verify_sha1(ByteView e, ByteView n, ByteView digest, ByteView signature) {
    const Owned<EVP_PKEY> key = rsa_key(e, n, {}, {});
    if (!key) return false;
    Owned<EVP_PKEY_CTX> ctx(EVP_PKEY_CTX_new(key.get(), nullptr));
    if (!ctx || EVP_PKEY_verify_init(ctx.get()) != 1 || EVP_PKEY_CTX_set_rsa_padding(ctx.get(), RSA_PKCS1_PADDING) != 1 || EVP_PKEY_CTX_set_signature_md(ctx.get(), EVP_sha1()) != 1) {
        ERR_clear_error();
        return false;
    }
    const bool ok = EVP_PKEY_verify(ctx.get(), signature.data(), signature.size(), digest.data(), digest.size()) == 1;
    ERR_clear_error();
    return ok;
}

}  // namespace cb::crypto

#endif  // !_WIN32
