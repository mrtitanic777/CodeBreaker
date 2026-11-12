// cb/formats/xbox/crypto/xecrypt.hpp -- the Xbox 360 XeCrypt library: hashes and MACs, RC4/AES/DES, the RSA key records and
// signature formats, the SMC cipher, the CPU-key ECC, keyvault sealing and the page ECC.
//
// This is what the console's kernel does with its keys, written out so the tools that rebuild boot chains and keyvaults can run
// anywhere. Credit where it is due: the algorithms were worked out by cOz and the Xbox 360 scene (the Python this replaces was
// "Gigantic shoutout to cOz for all the help in porting this").
//
// Conventions
//   * Byte strings are ByteView in and Bytes out. A function that edits its argument takes a ByteSpan and says so.
//   * A "BnQw" number is the console's big number: an array of big-endian qwords, least significant first. The RSA key records and
//     every signature use it, which is why the swap helpers (swap_qwords) are public.
//   * Anything random comes from a RandomSource (default: the operating system's). Tests pass a SeededRandom, so a test run is
//     repeatable; a seeded run makes the same bytes on every machine but NOT the bytes any other program's generator would.
//   * Errors: wrong sizes, wrong key kinds and out-of-range arguments are UsageError (the caller's bug); a key or a keyvault that
//     does not hold together is FormatError; "this signature/vault does not check out" is a bool or a Result, not an exception.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <initializer_list>
#include <memory>
#include <optional>
#include <span>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/formats/xbox/crypto/xecrypt_bn.hpp"

namespace cb::formats::xbox::crypto::xecrypt {

// ---- constants ---------------------------------------------------------------------------------------------------

inline constexpr std::array<std::uint8_t, 4> kSmcKey = {0x42, 0x75, 0x4E, 0x79};
inline constexpr std::string_view kSalt1bl = "XBOX_ROM_B";   // the salts the bootloader signatures use (10 bytes)
inline constexpr std::string_view kSaltSc = "XBOX_ROM_3";
inline constexpr std::string_view kSaltSd = "XBOX_ROM_4";

inline constexpr std::size_t kRsaPub1024Size = 0x90;
inline constexpr std::size_t kRsaPub1536Size = 0xD0;
inline constexpr std::size_t kRsaPub2048Size = 0x110;
inline constexpr std::size_t kRsaPub4096Size = 0x210;
inline constexpr std::size_t kRsaPrv1024Size = 0x1D0;
inline constexpr std::size_t kRsaPrv1536Size = 0x2B0;
inline constexpr std::size_t kRsaPrv2048Size = 0x390;
inline constexpr std::size_t kRsaPrv4096Size = 0x710;

inline constexpr std::size_t kShaDigestSize = 0x14;
inline constexpr std::size_t kMd5DigestSize = 0x10;
inline constexpr std::size_t kHmacShaMaxKeySize = 0x40;
inline constexpr std::size_t kDesBlockSize = 0x8;
inline constexpr std::size_t kDesKeySize = 0x8;
inline constexpr std::size_t kDes3KeySize = 0x18;
inline constexpr std::size_t kAesBlockSize = 0x10;
inline constexpr std::size_t kAesKeySize = 0x10;
inline constexpr std::size_t kRotSumDigestSize = 0x20;
inline constexpr std::size_t kSignatureSize = 0x100;   // a 2048-bit RSA signature block
inline constexpr std::size_t kKeyvaultSize = 0x4000;
inline constexpr std::size_t kCpuKeySize = 0x10;
inline constexpr std::size_t kPageSize = 0x210;        // a NAND page's data and spare, as far as its ECC reaches

// ---- randomness --------------------------------------------------------------------------------------------------

class RandomSource {
public:
    virtual ~RandomSource() = default;
    virtual void fill(ByteSpan out) = 0;
    Bytes bytes(std::size_t n);
    std::uint32_t u32();
};

// The operating system's cryptographic generator (BCryptGenRandom).
RandomSource& os_random();

// A fast generator with a seed, for tests and for reproducing a run: the same seed gives the same bytes. (splitmix64; not
// cryptographic.)
class SeededRandom : public RandomSource {
public:
    explicit SeededRandom(std::uint64_t seed) : state_(seed) {}
    void fill(ByteSpan out) override;

private:
    std::uint64_t state_;
};

// ---- byte-order helpers ------------------------------------------------------------------------------------------

Bytes reversed(ByteView data);
// Each 2-, 4- or 8-byte unit reversed. UsageError if the length is not a multiple of the unit.
Bytes swap_words(ByteView data);    // 16-bit
Bytes swap_dwords(ByteView data);   // 32-bit
Bytes swap_qwords(ByteView data);   // 64-bit: little-endian BnQw <-> big-endian number, and back
// A whole number of qwords given as little-endian/big-endian bytes <-> Nat
Nat nat_from_qwords(ByteView bnqw);               // a BnQw as a number
Bytes nat_to_qwords(const Nat& n, std::size_t size);   // `size` bytes (a multiple of 8, UsageError if n is too big)

// ---- hashing and MACs --------------------------------------------------------------------------------------------

// The pieces hashed in order, as if joined: a braced list at the call site (`sha({a, b})`), or a span of views.
using Parts = std::initializer_list<ByteView>;
using PartSpan = std::span<const ByteView>;

Bytes md5(PartSpan parts);
Bytes sha(PartSpan parts);
Bytes hmac_md5(ByteView key, PartSpan parts);
Bytes hmac_sha(ByteView key, PartSpan parts);
inline Bytes md5(Parts parts) { return md5(PartSpan(parts.begin(), parts.size())); }
inline Bytes sha(Parts parts) { return sha(PartSpan(parts.begin(), parts.size())); }
inline Bytes hmac_md5(ByteView key, Parts parts) { return hmac_md5(key, PartSpan(parts.begin(), parts.size())); }
inline Bytes hmac_sha(ByteView key, Parts parts) { return hmac_sha(key, PartSpan(parts.begin(), parts.size())); }
inline Bytes md5(ByteView data) { return md5(PartSpan(&data, 1)); }
inline Bytes sha(ByteView data) { return sha(PartSpan(&data, 1)); }
inline Bytes hmac_sha(ByteView key, ByteView data) { return hmac_sha(key, PartSpan(&data, 1)); }
inline Bytes hmac_md5(ByteView key, ByteView data) { return hmac_md5(key, PartSpan(&data, 1)); }

// ---- RC4 ---------------------------------------------------------------------------------------------------------

// RC4. The two directions are separate streams over the same key (what the console's pairs of contexts do): encrypt() and
// decrypt() each continue their own keystream, so `Rc4(k).encrypt(x)` and `Rc4(k).decrypt(x)` are the same operation.
// Keys are 1 to 256 bytes (UsageError otherwise).
class Rc4 {
public:
    explicit Rc4(ByteView key);
    Bytes encrypt(ByteView data) { return run(enc_, data); }
    Bytes decrypt(ByteView data) { return run(dec_, data); }

private:
    struct State {
        std::array<std::uint8_t, 256> s{};
        std::uint8_t i = 0, j = 0;
    };
    static Bytes run(State& st, ByteView data);
    State enc_, dec_;
};

// One-shot: the whole of `data` under a fresh RC4 of `key`.
Bytes rc4(ByteView key, ByteView data);

// ---- AES and DES -------------------------------------------------------------------------------------------------

enum class CipherMode { Ecb, Cbc };
enum class CipherAlgorithm {
    Aes,    // 128- or 256-bit key
    Des,    // 64-bit key (single DES)
    Des2,   // 128-bit key (3DES with k1, k2, k1)
    Des3,   // 192-bit key
};

// A block cipher in ECB or CBC mode, no padding, as a stream. Each direction keeps its own chaining state across calls, so a
// long message can be fed in pieces of any size: a call answers with the blocks that are complete, and holds a partial block
// back for the next call (pending_*() says how many bytes wait). There is no final block: the caller feeds whole blocks.
// The key size must fit the algorithm (UsageError otherwise); CBC needs an IV of one block.
class BlockCipher {
public:
    BlockCipher(CipherAlgorithm algorithm, ByteView key, CipherMode mode = CipherMode::Ecb, ByteView iv = {});
    BlockCipher(BlockCipher&&) noexcept;
    BlockCipher& operator=(BlockCipher&&) noexcept;
    ~BlockCipher();
    std::size_t block_size() const;
    Bytes encrypt(ByteView data);
    Bytes decrypt(ByteView data);
    std::size_t pending_encrypt() const;   // bytes held back, less than a block
    std::size_t pending_decrypt() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// DES key parity: the low bit of each byte makes its byte's parity odd.
Bytes des_parity(ByteView key);

// ---- the console's own MACs and checksums ------------------------------------------------------------------------

// Parve, the 8-byte block cipher/MAC of the SMC era. `key` is 8 bytes, `sbox` 256, `data` one block (ecb) or a whole number of
// blocks (cbc-mac; `iv` is 8 bytes).
Bytes parve_ecb(ByteView key, ByteView sbox, ByteView block);
Bytes parve_cbc_mac(ByteView key, ByteView sbox, ByteView iv, ByteView data);
// The 8-byte chain-and-sum MAC: `cd` and `ab` are 8-byte keys, `data` a whole number of 8-byte blocks.
Bytes chain_and_sum_mac(ByteView cd, ByteView ab, ByteView data);
// 32 bytes: four big-endian qwords over the whole qwords of `data` (a trailing partial qword is ignored; fewer than 8 bytes give
// all zeros).
Bytes rot_sum(ByteView data);
// SHA-1 over the rot-sum (twice), the data, and the inverted rot-sum (twice): the digest bootloader signatures sign.
Bytes rot_sum_sha(ByteView data);

// ---- RSA ---------------------------------------------------------------------------------------------------------

// A console RSA key: the 16-byte header (qword count, exponent, a reserved qword -- the Montgomery constant, or 0), the modulus,
// and for a private key p, q, dp, dq and the CRT coefficient, each half the modulus' size. All BnQw. Sizes: 1024, 1536, 2048 or
// 4096 bits. parse() checks the key hangs together (n = p*q, dp and dq from d, the coefficient is q^-1 mod p).
class RsaKey {
public:
    // FormatError if the bytes are not a key of a known size or the parameters are inconsistent.
    static RsaKey parse(ByteView key);
    // A new private key of `bits` (1024, 1536, 2048 or 4096) with public exponent 3 or 0x10001; UsageError otherwise. With the
    // default (operating system) generator and exponent 0x10001 Windows CNG makes the key, fast; with any other generator, or
    // exponent 3, the primes are searched here from `rng`, so a seeded run gives the same key every time.
    static RsaKey generate(unsigned bits = 2048, std::uint32_t exponent = 0x10001, RandomSource& rng = os_random());

    ByteView bytes() const { return bytes_; }
    bool is_private() const { return private_; }
    std::size_t qword_count() const { return qwords_; }
    std::size_t modulus_size() const { return qwords_ * 8; }   // bytes
    std::size_t bits() const { return qwords_ * 64; }
    std::uint32_t exponent() const { return exponent_; }
    const Nat& n() const { return n_; }
    // The public half (a public key returns a copy of itself).
    RsaKey public_key() const;
    // The Montgomery constant 2^64 mod-inverse (the header's reserved qword, or computed when it is 0).
    std::uint64_t montgomery_constant() const { return mod_inv_; }

    // The raw RSA operations on BnQw numbers of the modulus' size: private (CRT) and public. UsageError for a public key on
    // prv_crypt, or a number of the wrong size.
    Bytes prv_crypt(ByteView bnqw) const;
    Bytes pub_crypt(ByteView bnqw) const;

    // The bootloader signature scheme (2048-bit keys, salts up to 10 bytes): sign_hash = prv_crypt(sig_create(...)).
    Bytes sign(ByteView hash, ByteView salt) const;
    bool verify(ByteView signature, ByteView hash, ByteView salt) const;
    // PKCS#1 v1.5 over a 20-byte hash in the console's byte order (XeKeysPkcs1).
    Bytes sign_pkcs1(ByteView hash) const;
    bool verify_pkcs1(ByteView signature, ByteView hash) const;

    // The parts, for code that needs the numbers (a private key only for p, q, dp, dq, u).
    const Nat& p() const { return p_; }
    const Nat& q() const { return q_; }
    const Nat& dp() const { return dp_; }
    const Nat& dq() const { return dq_; }
    const Nat& u() const { return u_; }
    Nat d() const;       // private exponent
    Nat r() const;       // 2^((e-1)*2048) mod n: the signature scheme's conversion factor
    Nat inv_r() const;   // its inverse mod n

private:
    RsaKey() = default;
    Bytes bytes_;
    bool private_ = false;
    std::size_t qwords_ = 0;
    std::uint32_t exponent_ = 0;
    std::uint64_t mod_inv_ = 0;
    Nat n_, p_, q_, dp_, dq_, u_;
};

struct KeyPair {
    Bytes public_key;    // the first kRsaPub...Size bytes of the private key
    Bytes private_key;
};
KeyPair generate_key_pair(unsigned bits = 2048, std::uint32_t exponent = 0x10001, RandomSource& rng = os_random());

// -pow(2^64, -1, v)'s partner: the inverse of 2^64 modulo an odd modulus (UsageError for an even or zero one).
Nat montgomery_inverse(const Nat& v);

// The bootloader signature block (0x100 bytes) of a hash: RC4-masked padding, salt, hash, 0xBC; the block `sign()` exponentiates.
// `qwords` must be 0x20 (a 2048-bit key). The salt is at most 10 bytes.
Bytes sig_format(std::size_t qwords, ByteView hash, ByteView salt);
// sig_format converted into the key's number domain (the value prv_crypt takes). UsageError for a key that is not a 2048-bit
// private key with exponent 3 or 0x10001, or a salt over 10 bytes; FormatError if the block overflows the modulus.
Bytes sig_create(ByteView hash, ByteView salt, const RsaKey& private_key);
bool sig_verify(ByteView signature, ByteView hash, ByteView salt, const RsaKey& public_key);

// PKCS#1 v1.5 block of a 20-byte hash for a `signature_size`-byte signature (0x27..0x200), in the console's little-endian
// order, 0x200 bytes with the block in the first signature_size of them. `type`: 0 = SHA-1 with NULL parameters, 1 = without, any
// other = no DigestInfo. UsageError outside the sizes.
Bytes pkcs1_format(ByteView hash, int type, std::size_t signature_size);
bool pkcs1_verify(ByteView block, ByteView hash, std::size_t signature_size);

// ---- the rest of the kernel's toys ---------------------------------------------------------------------------------

// The SMC image cipher.
Bytes smc_decrypt(ByteView data);
Bytes smc_encrypt(ByteView data);

std::size_t hamming_weight(ByteView data);
// The ECC bits of a 16-byte CPU key / unique id: returns the key with them set. UsageError for fewer than 16 bytes.
Bytes uid_ecc_encode(ByteView data);

// A CPU key is 16 bytes, 53 set bits in the first 106, and the right ECC.
bool cpu_key_valid(ByteView cpu_key);
Bytes cpu_key_generate(RandomSource& rng = os_random());

// Keyvault sealing (the 0x4000-byte vault under a CPU key). decrypt: UsageError for an invalid CPU key, and an error Result when
// the vault's digest does not match (wrong key or damaged vault).
Result<Bytes> keyvault_decrypt(ByteView cpu_key, ByteView encrypted);
// Writes a fresh obfuscation key (8 bytes at 0x10) and the digest (the nonce field, 0x10) into `vault` (the plain vault, edited in
// place) and returns the encrypted vault. UsageError for an invalid CPU key or a vault under 0x20 bytes.
Bytes keyvault_encrypt(ByteView cpu_key, ByteSpan vault, RandomSource& rng = os_random());
// Verify the vault's console certificate signature with the public key. `vault` is the plain vault including its 0x18-byte prefix.
bool keyvault_verify(ByteView cpu_key, ByteView vault, const RsaKey& public_key);

// The ECC bits of a NAND page (0x1080 bits: 0x210 bytes), set in `page` in place; `page_ecc_encode` returns a copy.
void page_ecc_encode_in_place(ByteSpan page);
Bytes page_ecc_encode(ByteView page);

// ---- the keyvault's layout -----------------------------------------------------------------------------------------

// The fields of the 0x4000-byte plain keyvault (XECRYPT_KEYVAULT), in order, with their offsets and sizes.
struct KeyvaultField {
    std::string_view name;
    std::size_t offset;
    std::size_t size;
};
std::span<const KeyvaultField> keyvault_layout();

// ---- the key store -------------------------------------------------------------------------------------------------

// A key file from the key-store folder (the `xbox360_keys` setting, "Keys" by default), nullopt when it is not there.
std::optional<Bytes> load_store_key(const std::filesystem::path& folder, std::string_view name);

}  // namespace cb::formats::xbox::crypto::xecrypt
