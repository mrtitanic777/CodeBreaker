#include "cb/formats/xbox/crypto/xecrypt.hpp"

#include <algorithm>
#include <bit>
#include <cstring>
#include <cwchar>
#include <format>

#include <windows.h>   // CNG (bcrypt.h) needs it; confined to this file
#include <bcrypt.h>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"

namespace cb::formats::xbox::crypto::xecrypt {

// ---- randomness --------------------------------------------------------------------------------------------------

Bytes RandomSource::bytes(std::size_t n) {
    Bytes out(n);
    fill(out);
    return out;
}

std::uint32_t RandomSource::u32() {
    std::uint8_t b[4];
    fill(b);
    return static_cast<std::uint32_t>(b[0]) | (static_cast<std::uint32_t>(b[1]) << 8) | (static_cast<std::uint32_t>(b[2]) << 16) | (static_cast<std::uint32_t>(b[3]) << 24);
}

namespace {

class OsRandom : public RandomSource {
public:
    void fill(ByteSpan out) override {
        if (out.empty()) return;
        if (BCryptGenRandom(nullptr, out.data(), static_cast<ULONG>(out.size()), BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0) throw InternalError("CNG: no random bytes");
    }
};

[[noreturn]] void usage(const std::string& what) { throw UsageError(what); }

void require_size(ByteView data, std::size_t want, std::string_view what) {
    if (data.size() != want) usage(std::format("{} must be {} bytes, not {}", what, want, data.size()));
}

}  // namespace

RandomSource& os_random() {
    static OsRandom rng;   // stateless: the one instance is shared
    return rng;
}

void SeededRandom::fill(ByteSpan out) {
    for (std::size_t i = 0; i < out.size(); i += 8) {   // splitmix64
        state_ += 0x9E3779B97F4A7C15ull;
        std::uint64_t z = state_;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
        z ^= z >> 31;
        for (std::size_t k = 0; k < 8 && i + k < out.size(); ++k) out[i + k] = static_cast<std::uint8_t>(z >> (8 * k));
    }
}

// ---- byte order --------------------------------------------------------------------------------------------------

Bytes reversed(ByteView data) { return Bytes(data.rbegin(), data.rend()); }

namespace {

Bytes swap_units(ByteView data, std::size_t unit) {
    if (data.size() % unit != 0) usage(std::format("{} bytes are not a whole number of {}-byte units", data.size(), unit));
    Bytes out(data.begin(), data.end());
    for (std::size_t i = 0; i < out.size(); i += unit) std::reverse(out.begin() + static_cast<std::ptrdiff_t>(i), out.begin() + static_cast<std::ptrdiff_t>(i + unit));
    return out;
}

}  // namespace

Bytes swap_words(ByteView data) { return swap_units(data, 2); }
Bytes swap_dwords(ByteView data) { return swap_units(data, 4); }
Bytes swap_qwords(ByteView data) { return swap_units(data, 8); }

Nat nat_from_qwords(ByteView bnqw) { return Nat::from_le(swap_qwords(bnqw)); }

Bytes nat_to_qwords(const Nat& n, std::size_t size) {
    if (size % 8 != 0) usage("a BnQw number is a whole number of qwords");
    return swap_qwords(n.to_le(size));
}

// ---- hashing -----------------------------------------------------------------------------------------------------

namespace {

Bytes hash_parts(cb::crypto::Hasher::Algorithm algorithm, PartSpan parts) {
    cb::crypto::Hasher h(algorithm);
    for (ByteView p : parts) h.update(p);
    return h.finish();
}

Bytes hmac_parts(cb::crypto::Hasher::Algorithm algorithm, ByteView key, PartSpan parts) {
    constexpr std::size_t kBlock = 64;   // both MD5 and SHA-1
    Bytes k(key.begin(), key.end());
    if (k.size() > kBlock) {
        cb::crypto::Hasher h(algorithm);
        h.update(key);
        k = h.finish();
    }
    k.resize(kBlock, 0);
    Bytes inner(kBlock), outer(kBlock);
    for (std::size_t i = 0; i < kBlock; ++i) {
        inner[i] = k[i] ^ 0x36;
        outer[i] = k[i] ^ 0x5C;
    }
    cb::crypto::Hasher in(algorithm);
    in.update(inner);
    for (ByteView p : parts) in.update(p);
    const Bytes digest = in.finish();
    cb::crypto::Hasher out(algorithm);
    out.update(outer);
    out.update(digest);
    return out.finish();
}

}  // namespace

Bytes md5(PartSpan parts) { return hash_parts(cb::crypto::Hasher::Algorithm::Md5, parts); }
Bytes sha(PartSpan parts) { return hash_parts(cb::crypto::Hasher::Algorithm::Sha1, parts); }
Bytes hmac_md5(ByteView key, PartSpan parts) { return hmac_parts(cb::crypto::Hasher::Algorithm::Md5, key, parts); }
Bytes hmac_sha(ByteView key, PartSpan parts) { return hmac_parts(cb::crypto::Hasher::Algorithm::Sha1, key, parts); }

// ---- RC4 ---------------------------------------------------------------------------------------------------------

Rc4::Rc4(ByteView key) {
    if (key.empty() || key.size() > 256) usage(std::format("an RC4 key is 1 to 256 bytes, not {}", key.size()));
    for (State* st : {&enc_, &dec_}) {
        for (int i = 0; i < 256; ++i) st->s[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(i);
        std::uint8_t j = 0;
        for (std::size_t i = 0; i < 256; ++i) {
            j = static_cast<std::uint8_t>(j + st->s[i] + key[i % key.size()]);
            std::swap(st->s[i], st->s[j]);
        }
    }
}

Bytes Rc4::run(State& st, ByteView data) {
    Bytes out(data.size());
    std::uint8_t s[256];   // the state in locals: the compiler keeps i, j and the table in registers and cache
    std::memcpy(s, st.s.data(), 256);
    std::uint8_t i = st.i, j = st.j;
    const std::uint8_t* in = data.data();
    std::uint8_t* o = out.data();
    for (std::size_t k = 0, n = data.size(); k < n; ++k) {
        i = static_cast<std::uint8_t>(i + 1);
        const std::uint8_t si = s[i];
        j = static_cast<std::uint8_t>(j + si);
        const std::uint8_t sj = s[j];
        s[i] = sj;
        s[j] = si;
        o[k] = in[k] ^ s[static_cast<std::uint8_t>(si + sj)];
    }
    std::memcpy(st.s.data(), s, 256);
    st.i = i;
    st.j = j;
    return out;
}

Bytes rc4(ByteView key, ByteView data) { return Rc4(key).encrypt(data); }

// ---- AES and DES (Windows CNG) -----------------------------------------------------------------------------------

namespace {

struct CngKey {   // one direction of a cipher: its key handle and its running IV
    BCRYPT_KEY_HANDLE handle = nullptr;
    Bytes iv;
    CngKey() = default;
    CngKey(const CngKey&) = delete;
    CngKey& operator=(const CngKey&) = delete;
    ~CngKey() {
        if (handle) BCryptDestroyKey(handle);
    }
};

struct CngProvider {
    BCRYPT_ALG_HANDLE handle = nullptr;
    CngProvider() = default;
    CngProvider(const CngProvider&) = delete;
    CngProvider& operator=(const CngProvider&) = delete;
    ~CngProvider() {
        if (handle) BCryptCloseAlgorithmProvider(handle, 0);
    }
};

}  // namespace

struct BlockCipher::Impl {
    CngProvider alg;   // (declared first, destroyed last: the keys go before their provider)
    std::size_t block = 0;
    bool cbc = false;
    CngKey enc, dec;
    Bytes enc_pending, dec_pending;   // a partial block held back, per direction
};

BlockCipher::BlockCipher(CipherAlgorithm algorithm, ByteView key, CipherMode mode, ByteView iv) : impl_(std::make_unique<Impl>()) {
    const wchar_t* name = nullptr;
    switch (algorithm) {
        case CipherAlgorithm::Aes:
            if (key.size() != 16 && key.size() != 32) usage(std::format("an AES key is 128 or 256 bits, not {} bytes", key.size()));
            name = BCRYPT_AES_ALGORITHM;
            impl_->block = 16;
            break;
        case CipherAlgorithm::Des:
            require_size(key, 8, "a DES key");
            name = BCRYPT_DES_ALGORITHM;
            impl_->block = 8;
            break;
        case CipherAlgorithm::Des2:
            require_size(key, 16, "a two-key 3DES key");
            name = BCRYPT_3DES_112_ALGORITHM;
            impl_->block = 8;
            break;
        case CipherAlgorithm::Des3:
            require_size(key, 24, "a 3DES key");
            name = BCRYPT_3DES_ALGORITHM;
            impl_->block = 8;
            break;
    }
    impl_->cbc = mode == CipherMode::Cbc;
    if (impl_->cbc) require_size(iv, impl_->block, "a CBC initialisation vector");
    if (BCryptOpenAlgorithmProvider(&impl_->alg.handle, name, nullptr, 0) != 0) throw InternalError("CNG: no such cipher");
    const wchar_t* chain = impl_->cbc ? BCRYPT_CHAIN_MODE_CBC : BCRYPT_CHAIN_MODE_ECB;
    if (BCryptSetProperty(impl_->alg.handle, BCRYPT_CHAINING_MODE, reinterpret_cast<PUCHAR>(const_cast<wchar_t*>(chain)), static_cast<ULONG>((wcslen(chain) + 1) * sizeof(wchar_t)), 0) != 0) {
        throw InternalError("CNG: cannot set the chaining mode");
    }
    for (CngKey* k : {&impl_->enc, &impl_->dec}) {
        if (BCryptGenerateSymmetricKey(impl_->alg.handle, &k->handle, nullptr, 0, const_cast<PUCHAR>(key.data()), static_cast<ULONG>(key.size()), 0) != 0) {
            throw InternalError("CNG: the key was refused");
        }
        k->iv.assign(iv.begin(), iv.end());
    }
}

BlockCipher::BlockCipher(BlockCipher&&) noexcept = default;
BlockCipher& BlockCipher::operator=(BlockCipher&&) noexcept = default;
BlockCipher::~BlockCipher() = default;

std::size_t BlockCipher::block_size() const { return impl_->block; }

namespace {

Bytes cng_run(bool cbc, CngKey& key, ByteView data, bool encrypt) {
    Bytes out(data.size());
    if (data.empty()) return out;
    ULONG written = 0;
    PUCHAR iv = cbc ? key.iv.data() : nullptr;
    const ULONG iv_size = cbc ? static_cast<ULONG>(key.iv.size()) : 0;
    const NTSTATUS st = encrypt ? BCryptEncrypt(key.handle, const_cast<PUCHAR>(data.data()), static_cast<ULONG>(data.size()), nullptr, iv, iv_size, out.data(), static_cast<ULONG>(out.size()), &written, 0)
                                : BCryptDecrypt(key.handle, const_cast<PUCHAR>(data.data()), static_cast<ULONG>(data.size()), nullptr, iv, iv_size, out.data(), static_cast<ULONG>(out.size()), &written, 0);
    if (st != 0 || written != out.size()) throw InternalError("CNG: the cipher failed");
    return out;
}

// What one call of a direction answers: the pending partial block and the new data, as far as whole blocks go.
Bytes cng_stream(bool cbc, std::size_t block, CngKey& key, Bytes& pending, ByteView data, bool encrypt) {
    Bytes input = std::move(pending);
    append(input, data);
    const std::size_t whole = input.size() / block * block;
    pending.assign(input.begin() + static_cast<std::ptrdiff_t>(whole), input.end());
    input.resize(whole);
    return cng_run(cbc, key, input, encrypt);
}

}  // namespace

Bytes BlockCipher::encrypt(ByteView data) { return cng_stream(impl_->cbc, impl_->block, impl_->enc, impl_->enc_pending, data, true); }
Bytes BlockCipher::decrypt(ByteView data) { return cng_stream(impl_->cbc, impl_->block, impl_->dec, impl_->dec_pending, data, false); }
std::size_t BlockCipher::pending_encrypt() const { return impl_->enc_pending.size(); }
std::size_t BlockCipher::pending_decrypt() const { return impl_->dec_pending.size(); }

Bytes des_parity(ByteView key) {
    Bytes out(key.begin(), key.end());
    for (std::uint8_t& b : out) {
        const std::uint8_t ones = static_cast<std::uint8_t>(std::popcount(static_cast<unsigned>(b & 0xFE)));
        b = static_cast<std::uint8_t>((b & 0xFE) | ((ones & 1) ? 0 : 1));   // odd parity over the whole byte
    }
    return out;
}

// ---- Parve, chain-and-sum, rot-sum -------------------------------------------------------------------------------

Bytes parve_ecb(ByteView key, ByteView sbox, ByteView block8) {
    require_size(key, 8, "a Parve key");
    require_size(sbox, 256, "a Parve s-box");
    require_size(block8, 8, "a Parve block");
    std::uint8_t block[9];
    std::memcpy(block, block8.data(), 8);
    block[8] = block[0];
    for (int i = 8; i > 0; --i) {
        for (int j = 0; j < 8; ++j) {
            const std::uint8_t x = static_cast<std::uint8_t>(key[static_cast<std::size_t>(j)] + block[j] + i);
            const std::uint8_t y = static_cast<std::uint8_t>(sbox[x] + block[j + 1]);
            block[j + 1] = static_cast<std::uint8_t>((y << 1) | (y >> 7));   // rotate left by one
        }
        block[0] = block[8];
    }
    return Bytes(block, block + 8);
}

Bytes parve_cbc_mac(ByteView key, ByteView sbox, ByteView iv, ByteView data) {
    require_size(iv, 8, "a Parve IV");
    if (data.size() % 8 != 0) usage("Parve CBC-MAC data is a whole number of 8-byte blocks");
    Bytes block(iv.begin(), iv.end());
    for (std::size_t i = 0; i < data.size(); i += 8) {
        for (std::size_t k = 0; k < 8; ++k) block[k] ^= data[i + k];
        block = parve_ecb(key, sbox, block);
    }
    return block;
}

Bytes chain_and_sum_mac(ByteView cd, ByteView ab, ByteView data) {
    require_size(cd, 8, "the `cd` key");
    require_size(ab, 8, "the `ab` key");
    if (data.size() % 8 != 0) usage("chain-and-sum data is a whole number of 8-byte blocks");
    constexpr std::uint64_t kMod = 0x7FFFFFFF;
    io::ByteReader abr(ab), cdr(cd);
    const std::uint64_t ab0 = abr.u32_be() % kMod, ab1 = abr.u32_be() % kMod;
    const std::uint64_t cd0 = cdr.u32_be() % kMod, cd1 = cdr.u32_be() % kMod;
    std::uint64_t out0 = 0, out1 = 0;
    io::ByteReader r(data);
    while (!r.eof()) {
        const std::uint64_t v0 = r.u32_be(), v1 = r.u32_be();
        std::uint64_t t = v0 * 0xE79A9C1 + out0;
        t %= kMod;
        t *= ab0;
        t += ab1;
        t %= kMod;
        out1 += t;
        t += v1;
        t *= cd0;
        t %= kMod;
        t += cd1;
        out0 = t % kMod;
        out1 += out0;
    }
    io::ByteWriter w;
    w.u32_be(static_cast<std::uint32_t>((out0 + ab1) % kMod));
    w.u32_be(static_cast<std::uint32_t>((out1 + cd1) % kMod));
    return w.take();
}

Bytes rot_sum(ByteView data) {
    std::uint64_t qw1 = 0, qw2 = 0, qw3 = 0, qw4 = 0;
    io::ByteReader r(data);
    for (std::size_t i = 0; i < data.size() / 8; ++i) {
        const std::uint64_t t = r.u64_be();
        qw2 += t;
        qw4 -= t;
        qw1 += (qw2 < t) ? 1 : 0;
        qw2 = std::rotl(qw2, 29);
        qw3 -= (t < qw4) ? 1 : 0;
        qw4 = std::rotl(qw4, 31);
    }
    io::ByteWriter w;
    w.u64_be(qw1);
    w.u64_be(qw2);
    w.u64_be(qw3);
    w.u64_be(qw4);
    return w.take();
}

Bytes rot_sum_sha(ByteView data) {
    const Bytes sum = rot_sum(data);
    Bytes inverted(sum);
    for (std::uint8_t& b : inverted) b = static_cast<std::uint8_t>(~b);
    return sha({sum, sum, data, inverted, inverted});
}

// ---- the kernel's toys -------------------------------------------------------------------------------------------

namespace {

// The SMC cipher: a 4-byte running key, each byte folding into its neighbours.
Bytes smc_run(ByteView data, bool encrypt) {
    std::array<std::uint8_t, 4> key = kSmcKey;   // only the low bytes of the running key are ever used
    Bytes out(data.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        const std::uint8_t plain_or_cipher = data[i];
        const std::uint8_t result = static_cast<std::uint8_t>(plain_or_cipher ^ key[i & 3]);
        out[i] = result;
        const std::uint32_t mod = std::uint32_t{encrypt ? result : plain_or_cipher} * 0xFB;
        key[(i + 1) & 3] = static_cast<std::uint8_t>(key[(i + 1) & 3] + mod);
        key[(i + 2) & 3] = static_cast<std::uint8_t>(key[(i + 2) & 3] + (mod >> 8));
    }
    return out;
}

}  // namespace

Bytes smc_decrypt(ByteView data) { return smc_run(data, false); }
Bytes smc_encrypt(ByteView data) { return smc_run(data, true); }

std::size_t hamming_weight(ByteView data) {
    std::size_t w = 0;
    for (std::uint8_t b : data) w += static_cast<std::size_t>(std::popcount(static_cast<unsigned>(b)));
    return w;
}

Bytes uid_ecc_encode(ByteView input) {
    if (input.size() < 16) usage("a CPU key / unique id is 16 bytes");
    Bytes data(input.begin(), input.end());
    std::uint32_t acc1 = 0, acc2 = 0;
    for (unsigned cnt = 0; cnt < 0x80; ++cnt) {
        acc1 >>= 1;
        const std::uint8_t b = data[cnt >> 3];
        const std::uint32_t bit = (b >> (cnt & 7)) & 1u;
        if (cnt < 0x6A) {
            acc1 ^= bit;
            if (acc1 & 1) acc1 ^= 0x360325;
            acc2 ^= bit;
        } else if (cnt < 0x7F) {
            if (bit != (acc1 & 1)) data[cnt >> 3] = static_cast<std::uint8_t>((1u << (cnt & 7)) ^ b);
            acc2 ^= acc1 & 1;
        } else if (bit != acc2) {
            data[0xF] = static_cast<std::uint8_t>(0x80 ^ b);
        }
    }
    return data;
}

bool cpu_key_valid(ByteView cpu_key) {
    if (cpu_key.size() != kCpuKeySize) return false;
    Bytes masked(cpu_key.begin(), cpu_key.end());   // keep the first 0x6A bits (13 bytes and the low two bits of the next): the rest are the ECC
    masked[13] &= 0x03;
    masked[14] = 0;
    masked[15] = 0;
    const std::size_t weight = hamming_weight(masked);
    const Bytes with_ecc = uid_ecc_encode(masked);
    return equal_bytes(with_ecc, cpu_key) && weight == 0x35;
}

Bytes cpu_key_generate(RandomSource& rng) {
    Bytes key(kCpuKeySize, 0);
    for (unsigned set = 0; set < 0x35; ++set) {   // set 53 distinct bits among the first 106, each chosen uniformly from the unset ones
        std::uint32_t pick = rng.u32() % (0x6A - set);
        unsigned bit = 0;
        for (; bit < 0x6A; ++bit) {
            if (((key[bit >> 3] >> (bit & 7)) & 1) == 0) {
                if (pick == 0) break;
                --pick;
            }
        }
        CB_ASSERT(bit < 0x6A, "a CPU key has room for another bit");
        key[bit >> 3] = static_cast<std::uint8_t>(key[bit >> 3] ^ (1u << (bit & 7)));
    }
    return uid_ecc_encode(key);
}

// ---- keyvaults ---------------------------------------------------------------------------------------------------

namespace {

constexpr std::array<std::uint8_t, 2> kKeyvaultVersion = {0x07, 0x12};

void require_cpu_key(ByteView cpu_key) {
    if (!cpu_key_valid(cpu_key)) usage("not a valid CPU key");
}

Bytes first16(Bytes digest) {
    digest.resize(16);
    return digest;
}

}  // namespace

Result<Bytes> keyvault_decrypt(ByteView cpu_key, ByteView encrypted) {
    require_cpu_key(cpu_key);
    if (encrypted.size() < 0x10) usage("a keyvault is at least 0x10 bytes");
    const Bytes rc4_key = first16(hmac_sha(cpu_key, encrypted.first(0x10)));
    Bytes plain(encrypted.begin(), encrypted.end());
    const Bytes body = Rc4(rc4_key).decrypt(encrypted.subspan(0x10));
    std::copy(body.begin(), body.end(), plain.begin() + 0x10);
    const Bytes digest = first16(hmac_sha(cpu_key, {ByteView(plain).subspan(0x10), ByteView(kKeyvaultVersion)}));
    if (!equal_bytes(ByteView(plain).first(0x10), digest)) return FormatError("keyvault", 0, "the digest does not match: wrong CPU key or a damaged keyvault");
    return plain;
}

Bytes keyvault_encrypt(ByteView cpu_key, ByteSpan vault, RandomSource& rng) {
    require_cpu_key(cpu_key);
    if (vault.size() < 0x20) usage("a keyvault is at least 0x20 bytes");
    rng.fill(vault.subspan(0x10, 8));   // a fresh obfuscation key (the nonce field is the digest, written below)
    const Bytes digest = first16(hmac_sha(cpu_key, {ByteView(vault).subspan(0x10), ByteView(kKeyvaultVersion)}));
    std::copy(digest.begin(), digest.end(), vault.begin());
    const Bytes rc4_key = first16(hmac_sha(cpu_key, ByteView(vault).first(0x10)));
    Bytes out(vault.begin(), vault.end());
    const Bytes body = Rc4(rc4_key).encrypt(ByteView(vault).subspan(0x10));
    std::copy(body.begin(), body.end(), out.begin() + 0x10);
    return out;
}

bool keyvault_verify(ByteView cpu_key, ByteView vault, const RsaKey& public_key) {
    require_cpu_key(cpu_key);
    if (vault.size() < kKeyvaultSize) usage("a keyvault is 0x4000 bytes");
    const ByteView kv = vault.subspan(0x18);   // past the nonce-and-obfuscation prefix
    const Bytes hash = hmac_sha(cpu_key, {kv.subspan(4, 0xD4), kv.subspan(0xE8, 0x1CF8), kv.subspan(0x1EE0, 0x2108)});
    return public_key.verify_pkcs1(kv.subspan(0x1DE0, 0x100), hash);
}

// ---- NAND page ECC -----------------------------------------------------------------------------------------------

void page_ecc_encode_in_place(ByteSpan page) {
    if (page.size() < kPageSize) usage(std::format("a page is at least {} bytes", kPageSize));
    std::uint32_t v1 = 0;
    for (unsigned bit = 0; bit < 0x1066; ++bit) {
        std::uint32_t v2 = v1 ^ (((1u << (bit % 8)) & page[bit / 8]) == 0 ? 1u : 0u);
        if (v2 & 1) v2 ^= 0x6954559;
        v1 = v2 >> 1;
    }
    for (unsigned bit = 0x1066; bit < 0x1080; ++bit) {
        if (v1 & 1) page[bit / 8] = static_cast<std::uint8_t>(page[bit / 8] & ~(1u << (bit % 8)));
        else page[bit / 8] = static_cast<std::uint8_t>(page[bit / 8] | (1u << (bit % 8)));
        v1 >>= 1;
    }
}

Bytes page_ecc_encode(ByteView page) {
    Bytes out(page.begin(), page.end());
    page_ecc_encode_in_place(out);
    return out;
}

// ---- the keyvault's layout ---------------------------------------------------------------------------------------

std::span<const KeyvaultField> keyvault_layout() {
    struct Spec {
        std::string_view name;
        std::size_t size;
    };
    // (the 1024-bit private key is 0x1D0 bytes, the 2048-bit one 0x390, the 2048-bit public one 0x110, a signature 0x100)
    static constexpr Spec kSpecs[] = {
        {"nonce", 0x10}, {"obfuscation", 8}, {"manufacturing_mode", 1}, {"alternate_keyvault", 1}, {"restricted_privilege_flags", 1},
        {"reserved_byte_3", 1}, {"odd_features", 2}, {"odd_auth_type", 2}, {"restricted_hv_ext_loader", 2}, {"reserved_ushort_1", 2},
        {"policy_flash_size", 4}, {"policy_build_in_mu_size", 4}, {"reserved_dword_4", 4}, {"restricted_privileges", 8},
        {"reserved_qword_2", 8}, {"reserved_qword_3", 8}, {"reserved_qword_4", 8}, {"reserved_key_1", 0x10}, {"reserved_key_2", 0x10},
        {"reserved_key_3", 0x10}, {"reserved_key_4", 0x10}, {"reserved_random_key_1", 0x10}, {"reserved_random_key_2", 0x10},
        {"console_serial", 0xC}, {"PADDING1", 4}, {"motherboard_serial", 8}, {"game_region", 2}, {"PADDING2", 6},
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
        {"console_private_key", kRsaPrv1024Size}, {"xeika_private_key", kRsaPrv2048Size}, {"cardea_private_key", kRsaPrv1024Size},
        {"console_certificate_size", 2}, {"console_id", 5}, {"console_part_number", 0xB}, {"console_reserved", 4},
        {"console_privileges", 2}, {"console_type", 4}, {"manufacture_date", 8}, {"console_public_key_exponent", 4},
        {"console_public_key_modulus", 0x80}, {"console_certificate_signature", kSignatureSize}, {"xeika_certificate_size", 2},
        {"xeika_public_key", kRsaPub2048Size}, {"xeika_certificate_overlay_signature", 4}, {"xeika_certificate_overlay_version", 2},
        {"xeika_odd_date_version", 1}, {"xeika_odd_drive_phase_level", 1}, {"odd_version_string", 0x28},
        {"xeika_certificate_reserved", 0x1146}, {"special_keyvault_signature", kSignatureSize}, {"cardea_certificate", 0x2108}};
    static const std::vector<KeyvaultField> table = [] {
        std::vector<KeyvaultField> t;
        std::size_t offset = 0;
        for (const Spec& spec : kSpecs) {
            t.push_back({spec.name, offset, spec.size});
            offset += spec.size;
        }
        return t;
    }();
    return table;
}

// ---- the key store -----------------------------------------------------------------------------------------------

std::optional<Bytes> load_store_key(const std::filesystem::path& folder, std::string_view name) {
    const std::filesystem::path path = folder / fs::from_text(name);
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec)) return std::nullopt;
    return fs::read_file(path);
}

}  // namespace cb::formats::xbox::crypto::xecrypt
