#include <algorithm>
#include <cstring>
#include <format>

#include <windows.h>   // CNG (bcrypt.h) needs it; confined to the key generator below
#include <bcrypt.h>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::xecrypt {

namespace {

constexpr std::string_view kRsaReader = "xecrypt rsa key";
constexpr std::size_t kHeaderSize = 16;   // qword count, exponent, reserved qword
constexpr std::size_t kSigBlockSize = 0x100;
constexpr std::size_t kSigSaltOffset = 0xE0;   // the 1, then the 10-byte salt, then the hash and 0xBC
constexpr std::size_t kSigHashOffset = 0xEB;
constexpr std::size_t kMaxSalt = 10;

[[noreturn]] void bad_key(const std::string& what) { throw FormatError(kRsaReader, 0, what); }
[[noreturn]] void rsa_usage(const std::string& what) { throw UsageError(what); }

bool known_bits(unsigned bits) { return bits == 1024 || bits == 1536 || bits == 2048 || bits == 4096; }

std::size_t public_key_size(unsigned bits) {
    switch (bits) {
        case 1024: return kRsaPub1024Size;
        case 1536: return kRsaPub1536Size;
        case 2048: return kRsaPub2048Size;
        default: return kRsaPub4096Size;
    }
}

// The order of the qwords reversed, the bytes within each kept (XeCryptBnQwBeBufSwap).
Bytes reverse_qword_order(ByteView data) {
    if (data.size() % 8 != 0) rsa_usage("a BnQw buffer is a whole number of qwords");
    Bytes out(data.size());
    const std::size_t qwords = data.size() / 8;
    for (std::size_t i = 0; i < qwords; ++i) std::copy_n(data.begin() + static_cast<std::ptrdiff_t>(i * 8), 8, out.begin() + static_cast<std::ptrdiff_t>((qwords - 1 - i) * 8));
    return out;
}

Bytes header_and_numbers(unsigned bits, std::uint32_t exponent, std::uint64_t montgomery, const Nat& n, const Nat& p, const Nat& q, const Nat& dp, const Nat& dq, const Nat& u) {
    const std::size_t nb = bits / 8, half = nb / 2;
    io::ByteWriter w;
    w.u32_be(bits / 64);
    w.u32_be(exponent);
    w.u64_be(montgomery);
    w.bytes(nat_to_qwords(n, nb));
    for (const Nat* x : {&p, &q, &dp, &dq, &u}) w.bytes(nat_to_qwords(*x, half));
    return w.take();
}

// A key from Windows CNG (fast, from the OS generator): its p, q, dp, dq and q^-1 mod p laid out as a console key.
Bytes cng_private_key(unsigned bits) {
    BCRYPT_ALG_HANDLE alg = nullptr;
    BCRYPT_KEY_HANDLE key = nullptr;
    struct Closer {
        BCRYPT_ALG_HANDLE& alg;
        BCRYPT_KEY_HANDLE& key;
        ~Closer() {
            if (key) BCryptDestroyKey(key);
            if (alg) BCryptCloseAlgorithmProvider(alg, 0);
        }
    } closer{alg, key};
    if (BCryptOpenAlgorithmProvider(&alg, BCRYPT_RSA_ALGORITHM, nullptr, 0) != 0) throw InternalError("CNG: no RSA provider");
    if (BCryptGenerateKeyPair(alg, &key, bits, 0) != 0 || BCryptFinalizeKeyPair(key, 0) != 0) throw InternalError("CNG: key generation failed");
    ULONG size = 0;
    BCryptExportKey(key, nullptr, BCRYPT_RSAFULLPRIVATE_BLOB, nullptr, 0, &size, 0);
    Bytes blob(size);
    if (BCryptExportKey(key, nullptr, BCRYPT_RSAFULLPRIVATE_BLOB, blob.data(), size, &size, 0) != 0) throw InternalError("CNG: cannot export the key");
    BCRYPT_RSAKEY_BLOB header{};
    std::memcpy(&header, blob.data(), sizeof header);
    ByteView rest = ByteView(blob).subspan(sizeof header);
    auto take = [&rest](std::size_t n) {
        const ByteView v = rest.first(n);
        rest = rest.subspan(n);
        return Nat::from_be(v);
    };
    const Nat e = take(header.cbPublicExp), n = take(header.cbModulus), p = take(header.cbPrime1), q = take(header.cbPrime2);
    const Nat dp = take(header.cbPrime1), dq = take(header.cbPrime2), u = take(header.cbPrime1);
    return header_and_numbers(bits, static_cast<std::uint32_t>(e.low64()), montgomery_inverse(n).low64(), n, p, q, dp, dq, u);
}

}  // namespace

Nat montgomery_inverse(const Nat& v) {
    if (v.is_zero() || !v.is_odd()) rsa_usage("the inverse of 2^64 needs an odd modulus");
    const std::optional<Nat> inverse = Nat::inv_mod(Nat(1).shifted_left(64) % v, v);
    if (!inverse) rsa_usage("2^64 has no inverse modulo this number");
    return *inverse;
}

// ---- keys --------------------------------------------------------------------------------------------------------

RsaKey RsaKey::parse(ByteView key) {
    io::ByteReader r(key, kRsaReader);
    RsaKey k;
    const std::uint32_t qwords = r.u32_be();
    k.exponent_ = r.u32_be();
    k.mod_inv_ = r.u64_be();
    const std::uint64_t nb = std::uint64_t{qwords} * 8;
    if (nb != 128 && nb != 192 && nb != 256 && nb != 512) bad_key(std::format("not a 1024/1536/2048/4096-bit key (a modulus of {} qwords)", qwords));
    k.qwords_ = qwords;
    k.private_ = key.size() > kHeaderSize + nb;
    const std::size_t half = static_cast<std::size_t>(nb / 2);
    const std::size_t need = kHeaderSize + static_cast<std::size_t>(nb) + (k.private_ ? 5 * half : 0);
    if (key.size() < need) bad_key(std::format("a {}-bit {} key is {} bytes, not {}", nb * 8, k.private_ ? "private" : "public", need, key.size()));
    k.bytes_.assign(key.begin(), key.end());
    k.n_ = nat_from_qwords(r.bytes(nb));
    if (k.private_) {
        k.p_ = nat_from_qwords(r.bytes(half));
        k.q_ = nat_from_qwords(r.bytes(half));
        k.dp_ = nat_from_qwords(r.bytes(half));
        k.dq_ = nat_from_qwords(r.bytes(half));
        k.u_ = nat_from_qwords(r.bytes(half));
    }
    // the parameters have to hang together
    const Nat e(k.exponent_);
    if (k.n_ < Nat(3)) bad_key("the modulus is below 3");
    if (e < Nat(3) || !(e < k.n_)) bad_key("the exponent is below 3 or not below the modulus");
    if (!e.is_odd()) bad_key("the exponent is even");
    if (k.private_) {
        if (!(k.dp_ < k.n_) || !(k.dq_ < k.n_) || !(k.u_ < k.n_)) bad_key("dp, dq or the coefficient is not below the modulus");
        if (!k.dp_.is_odd() || !k.dq_.is_odd()) bad_key("dp or dq is even");
        if (k.p_ < Nat(3) || k.q_ < Nat(3)) bad_key("p or q is below 3");
        if (!(k.n_ == k.p_ * k.q_)) bad_key("n is not p * q");
        const Nat p1 = k.p_ - Nat(1), q1 = k.q_ - Nat(1);
        const std::optional<Nat> d = Nat::inv_mod(e, p1 * q1);
        if (!d) bad_key("the exponent has no inverse modulo (p-1)(q-1)");
        if (!(k.dp_ == *d % p1) || !(k.dq_ == *d % q1)) bad_key("dp or dq does not match the private exponent");
        const std::optional<Nat> inv_q = Nat::inv_mod(k.q_, k.p_);
        if (!inv_q || !(k.u_ == *inv_q)) bad_key("the coefficient is not q^-1 mod p");
    }
    return k;
}

RsaKey RsaKey::public_key() const {
    if (!private_) return *this;
    return parse(ByteView(bytes_).first(public_key_size(static_cast<unsigned>(bits()))));
}

Nat RsaKey::d() const {
    if (!private_) rsa_usage("a public key has no private exponent");
    return Nat::inv_mod(Nat(exponent_), (p_ - Nat(1)) * (q_ - Nat(1))).value();   // (parse() proved it exists)
}

Nat RsaKey::r() const { return Nat::pow_mod(Nat(2), Nat(exponent_ - 1).shifted_left(11), n_); }

Nat RsaKey::inv_r() const {
    const std::optional<Nat> inv = Nat::inv_mod(r(), n_);
    if (!inv) bad_key("the conversion factor has no inverse modulo n");
    return *inv;
}

Bytes RsaKey::prv_crypt(ByteView bnqw) const {
    if (!private_) rsa_usage("this is not a private key");
    const Nat c = nat_from_qwords(bnqw);   // (may be larger than n: the CRT halves reduce it)
    const Nat m1 = Nat::pow_mod(c, dp_, p_);
    const Nat m2 = Nat::pow_mod(c, dq_, q_);
    const Nat diff = (m1 + p_ - m2 % p_) % p_;   // (m1 - m2) mod p
    const Nat h = (u_ * diff) % p_;
    return nat_to_qwords(m2 + h * q_, modulus_size());
}

Bytes RsaKey::pub_crypt(ByteView bnqw) const {
    return nat_to_qwords(Nat::pow_mod(nat_from_qwords(bnqw), Nat(exponent_), n_), modulus_size());
}

Bytes RsaKey::sign(ByteView hash, ByteView salt) const { return prv_crypt(sig_create(hash, salt, *this)); }

bool RsaKey::verify(ByteView signature, ByteView hash, ByteView salt) const { return sig_verify(signature, hash, salt, *this); }

Bytes RsaKey::sign_pkcs1(ByteView hash) const {
    if (!private_) rsa_usage("this is not a private key");
    // The block is a little-endian number; the result is the signature as little-endian bytes (not BnQw).
    const Bytes block = pkcs1_format(hash, 0, modulus_size());
    const Nat c = Nat::from_le(block);
    const Nat m1 = Nat::pow_mod(c, dp_, p_);
    const Nat m2 = Nat::pow_mod(c, dq_, q_);
    const Nat h = (u_ * ((m1 + p_ - m2 % p_) % p_)) % p_;
    return (m2 + h * q_).to_le(modulus_size());
}

bool RsaKey::verify_pkcs1(ByteView signature, ByteView hash) const {
    if (signature.size() != modulus_size()) return false;
    const Bytes block = Nat::pow_mod(Nat::from_le(signature), Nat(exponent_), n_).to_le(modulus_size());
    return pkcs1_verify(block, hash, modulus_size());
}

RsaKey RsaKey::generate(unsigned bits, std::uint32_t exponent, RandomSource& rng) {
    if (!known_bits(bits)) rsa_usage(std::format("RSA keys are 1024, 1536, 2048 or 4096 bits, not {}", bits));
    if (exponent != 3 && exponent != 0x10001) rsa_usage("the public exponent is 3 or 0x10001");
    if (&rng == &os_random() && exponent == 0x10001) return parse(cng_private_key(bits));   // the OS generator: let the OS do it
    const Nat e(exponent);
    for (;;) {
        Nat p = Nat::random_prime(bits / 2, rng);
        Nat q = Nat::random_prime(bits / 2, rng);
        if (p == q) continue;
        if (p < q) std::swap(p, q);
        const Nat p1 = p - Nat(1), q1 = q - Nat(1);
        if (!(Nat::gcd(p1, e) == Nat(1)) || !(Nat::gcd(q1, e) == Nat(1))) continue;
        const Nat n = p * q;
        if (n.bit_length() != bits) continue;
        const Nat d = Nat::inv_mod(e, p1 * q1).value();
        const Nat u = Nat::inv_mod(q, p).value();
        return parse(header_and_numbers(bits, exponent, montgomery_inverse(n).low64(), n, p, q, d % p1, d % q1, u));
    }
}

KeyPair generate_key_pair(unsigned bits, std::uint32_t exponent, RandomSource& rng) {
    const RsaKey key = RsaKey::generate(bits, exponent, rng);
    const RsaKey pub = key.public_key();
    return {Bytes(pub.bytes().begin(), pub.bytes().end()), Bytes(key.bytes().begin(), key.bytes().end())};
}

// ---- the bootloader signature scheme -----------------------------------------------------------------------------

Bytes sig_format(std::size_t qwords, ByteView hash, ByteView salt) {
    if (qwords != 0x20) rsa_usage("signatures are formatted for 2048-bit keys");
    if (salt.size() > kMaxSalt) rsa_usage("the salt is 10 bytes or less");
    const Bytes zeros(8, 0);
    const Bytes digest = sha({zeros, hash, salt});
    Bytes block(kSigBlockSize, 0);
    block[kSigSaltOffset] = 1;
    std::copy(salt.begin(), salt.end(), block.begin() + kSigSaltOffset + 1);   // (padded with zeros to 10)
    const Bytes masked = Rc4(digest).encrypt(ByteView(block).first(kSigHashOffset));
    std::copy(masked.begin(), masked.end(), block.begin());
    std::copy(digest.begin(), digest.end(), block.begin() + kSigHashOffset);
    block[0xFF] = 0xBC;
    block[0] &= 0x7F;
    return reverse_qword_order(block);
}

Bytes sig_create(ByteView hash, ByteView salt, const RsaKey& key) {
    if (salt.size() > kMaxSalt) rsa_usage("the salt is 10 bytes or less");
    if (key.qword_count() != 0x20) rsa_usage("only 2048-bit keys create signatures");
    if (key.exponent() != 3 && key.exponent() != 0x10001) rsa_usage("the public exponent is 3 or 0x10001");
    const Bytes block = sig_format(key.qword_count(), hash, salt);
    if (std::all_of(block.begin(), block.end(), [](std::uint8_t b) { return b == 0; })) throw FormatError(kRsaReader, 0, "the signature block overflows the modulus");
    const Nat converted = (nat_from_qwords(block) * key.r()) % key.n();
    return nat_to_qwords(converted, key.modulus_size());
}

bool sig_verify(ByteView signature, ByteView hash, ByteView salt, const RsaKey& key) {
    if (salt.size() > kMaxSalt) rsa_usage("the salt is 10 bytes or less");
    if (key.qword_count() != 0x20) rsa_usage("only 2048-bit keys verify signatures");
    if (signature.size() != kSigBlockSize) return false;
    const Nat raised = Nat::pow_mod(nat_from_qwords(signature), Nat(key.exponent()), key.n());
    const Nat plain = (raised * key.inv_r()) % key.n();
    const Bytes block = reverse_qword_order(nat_to_qwords(plain, kSigBlockSize));
    if (block[0xFF] != 0xBC) return false;
    const ByteView stored_hash = ByteView(block).subspan(kSigHashOffset, kShaDigestSize);
    const Bytes zeros(8, 0);
    if (!equal_bytes(sha({zeros, hash, salt}), stored_hash)) return false;
    const Bytes opened = Rc4(stored_hash).decrypt(ByteView(block).first(kSigHashOffset));
    if (opened[kSigSaltOffset] != 1) return false;
    if (!std::all_of(opened.begin() + 1, opened.begin() + kSigSaltOffset, [](std::uint8_t b) { return b == 0; })) return false;
    return std::equal(salt.begin(), salt.end(), opened.begin() + kSigSaltOffset + 1);
}

// ---- PKCS#1 ------------------------------------------------------------------------------------------------------

Bytes pkcs1_format(ByteView hash, int type, std::size_t signature_size) {
    if (hash.size() != kShaDigestSize) rsa_usage("a PKCS#1 hash is 20 bytes (SHA-1)");
    if (signature_size < 0x27 || signature_size > 0x200) rsa_usage(std::format("a PKCS#1 signature is 0x27 to 0x200 bytes, not {}", signature_size));
    Bytes block(0x200, 0);
    std::fill(block.begin(), block.begin() + static_cast<std::ptrdiff_t>(signature_size), std::uint8_t{0xFF});
    block[signature_size - 1] = 0;
    block[signature_size - 2] = 1;
    std::reverse_copy(hash.begin(), hash.end(), block.begin());
    static constexpr std::uint8_t kSha1WithNull[] = {0x14, 0x04, 0x00, 0x05, 0x1A, 0x02, 0x03, 0x0E, 0x2B, 0x05, 0x06, 0x09, 0x30, 0x21, 0x30, 0x00};
    static constexpr std::uint8_t kSha1Plain[] = {0x14, 0x04, 0x1A, 0x02, 0x03, 0x0E, 0x2B, 0x05, 0x06, 0x07, 0x30, 0x1F, 0x30, 0x00};
    if (type == 0) std::copy(std::begin(kSha1WithNull), std::end(kSha1WithNull), block.begin() + 0x14);
    else if (type == 1) std::copy(std::begin(kSha1Plain), std::end(kSha1Plain), block.begin() + 0x14);
    else block[0x14] = 0;
    return block;
}

bool pkcs1_verify(ByteView block, ByteView hash, std::size_t signature_size) {
    if (block.size() < 0x27 || block.size() > 0x200) return false;
    if (signature_size < 0x27 || signature_size > 0x200 || block.size() < signature_size || hash.size() != kShaDigestSize) return false;
    const int type = block[0x16] == 0 ? 0 : block[0x16] == 0x1A ? 1 : 2;
    const Bytes expected = pkcs1_format(hash, type, signature_size);
    return std::equal(expected.begin(), expected.begin() + static_cast<std::ptrdiff_t>(signature_size), block.begin());
}

}  // namespace cb::formats::xbox::crypto::xecrypt
