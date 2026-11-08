// codebreaker/formats/xbox/crypto/XeCrypt.hpp -- C++ port of codebreaker.formats.xbox.crypto.XeCrypt.
//
// Gigantic shoutout to cOz for all the help in porting this, without him this wouldn't have been possible!
//
// The Xbox 360 XeCrypt library: hashes, MACs, RC4/AES/DES ciphers, the RSA key structures and signature
// formats, the SMC cipher, the CPU-key ECC, keyvault sealing and the page ECC.
//
// Typing notes:
//   * `BinLike` (bytes | bytearray | memoryview) is py::bytes (a py::bytearray converts to it). The Python
//     functions that return a freshly made bytearray (DesParity, ParveEcb, UidEccEncode, ...) return py::bytes
//     here: the object is new, so the only difference is mutability. The three functions that change a
//     bytearray argument in place (XeCryptBnQwBeBufSwap, XeCryptKeyVaultEncrypt, XeCryptPageEccEncode) have
//     a py::bytearray overload that does the same, returning the same buffer; their py::bytes overload works
//     on a copy (what the Python does for a `bytes` argument).
//   * Python's unbounded int is `BigInt` (MemCrypto's) wherever the module really uses big values (RSA
//     numbers, the 64/128-bit masks); int64_t elsewhere. A negative int cannot be a BigInt: the Python
//     functions that could be handed one (i2b, the RSA helpers) are only ever given non-negative values.
//   * "or None" results are std::optional; a Union of two return types is std::variant.
//   * `*args: BinLike` is a py::list<py::bytes>.
//   * ctypes structures are plain structs with from_buffer_copy (big-endian); XECRYPT_KEYVAULT, which is only
//     ever a layout, is a table of (name, offset, size).
//   * the module's random numbers come from Python's `random` (randbytes): the same Mersenne Twister, with
//     random_seed(a) = random.seed(a), so a seeded run draws the same bytes as the Python.
//   * `new` is a C++ keyword: the static factory `new` of the cipher / key classes is `new_`.
//   * not ported: `__all__` (an export list); XeCryptRsaKey.to_cryptography returns the numbers of the
//     cryptography key (see its comment).
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"

namespace codebreaker::formats::xbox::crypto::XeCrypt {

using BigInt = MemCrypto::BigInt;

// ---- types (ctypes) --------------------------------------------------------------------------------
using BYTE = uint8_t;
using WORD = uint16_t;
using DWORD = uint32_t;
using QWORD = uint64_t;

// create_bitmask = lambda n: (1 << n) - 1   (ValueError for a negative n)
BigInt create_bitmask(int64_t n);

// ---- constants -------------------------------------------------------------------------------------
extern const py::bytes XECRYPT_SMC_KEY;
// a key from the key store (the xbox360_keys setting), None when not there
std::optional<py::bytes> _store_key(const py::str& name);
// XECRYPT_1BL_KEY = _store_key("1BL_key.bin"): read on first use (the setting is not known at static-init time)
const std::optional<py::bytes>& XECRYPT_1BL_KEY();
extern const py::bytes XECRYPT_1BL_SALT;
extern const py::bytes XECRYPT_SC_SALT;
extern const py::bytes XECRYPT_SD_SALT;
inline constexpr int64_t BUFFER_SIZE = 4096;

inline constexpr int64_t UINT8_MASK = 0xFF;
inline constexpr int64_t UINT16_MASK = 0xFFFF;
inline constexpr int64_t UINT32_MASK = 0xFFFFFFFFLL;
BigInt UINT64_MASK();
BigInt UINT128_MASK();

// public key sizes
inline constexpr int64_t XECRYPT_RSAPUB_1024_SIZE = 0x90;
inline constexpr int64_t XECRYPT_RSAPUB_1536_SIZE = 0xD0;
inline constexpr int64_t XECRYPT_RSAPUB_2048_SIZE = 0x110;
inline constexpr int64_t XECRYPT_RSAPUB_4096_SIZE = 0x210;

// private key sizes
inline constexpr int64_t XECRYPT_RSAPRV_1024_SIZE = 0x1D0;
inline constexpr int64_t XECRYPT_RSAPRV_1536_SIZE = 0x2B0;
inline constexpr int64_t XECRYPT_RSAPRV_2048_SIZE = 0x390;
inline constexpr int64_t XECRYPT_RSAPRV_4096_SIZE = 0x710;

inline constexpr int64_t XECRYPT_SHA_DIGEST_SIZE = 0x14;
inline constexpr int64_t XECRYPT_HMAC_SHA_MAX_KEY_SIZE = 0x40;

inline constexpr int64_t XECRYPT_DES_BLOCK_SIZE = 0x8;
inline constexpr int64_t XECRYPT_DES_KEY_SIZE = 0x8;

inline constexpr int64_t XECRYPT_DES3_BLOCK_SIZE = 0x8;
inline constexpr int64_t XECRYPT_DES3_KEY_SIZE = 0x18;

inline constexpr int64_t XECRYPT_MD5_DIGEST_SIZE = 0x10;

inline constexpr int64_t XECRYPT_AES_BLOCK_SIZE = 0x10;
inline constexpr int64_t XECRYPT_AES_KEY_SIZE = 0x10;
inline constexpr int64_t XECRYPT_AES_FEED_SIZE = 0x10;

inline constexpr int64_t XECRYPT_ROTSUM_DIGEST_SIZE = 0x20;

// ---- structures (ctypes BigEndianStructure) --------------------------------------------------------
// from_buffer_copy(data): ValueError ("Buffer size too small") when data is shorter than the structure
[[noreturn]] void buffer_too_small_(int64_t have, int64_t need);

struct XECRYPT_SIG {
    static constexpr int64_t sizeof_ = 0x100;
    std::array<uint64_t, 0x1C> aqwPad{};
    BYTE bOne = 0;
    py::bytes abSalt;   // 0xA bytes
    py::bytes abHash;   // 0x14 bytes
    BYTE bEnd = 0;
    static XECRYPT_SIG from_buffer_copy(const py::bytes& data);
};

struct XECRYPT_RSA {
    static constexpr int64_t sizeof_ = 16;
    DWORD cqw = 0;
    DWORD e = 0;
    QWORD qwReserved = 0;
    static XECRYPT_RSA from_buffer_copy(const py::bytes& data);
};

// A public key: XECRYPT_RSA then the NB-byte modulus. `Anonymous` is ctypes' `_anonymous_ = ["rsa"]`: without
// it (XECRYPT_RSAPUB_1536) the rsa fields are reachable only as `.rsa.cqw`, and `.e` / `.qwReserved` of the
// key structure are an AttributeError -- as in the Python.
template <int64_t NB, bool Anonymous>
struct RsaPubStruct {
    static constexpr int64_t sizeof_ = 16 + NB;
    static constexpr bool is_private = false;
    static constexpr bool anonymous = Anonymous;
    XECRYPT_RSA rsa;
    py::bytes n;
    static RsaPubStruct from_buffer_copy(const py::bytes& data) {
        if (py::len(data) < sizeof_) buffer_too_small_(py::len(data), sizeof_);
        return RsaPubStruct{XECRYPT_RSA::from_buffer_copy(data), data.slice(16, 16 + NB)};
    }
};

// A private key: XECRYPT_RSA, the NB-byte modulus, then p, q, dp, dq and cr (the CRT coefficient) of NB/2 bytes.
template <int64_t NB>
struct RsaPrvStruct {
    static constexpr int64_t sizeof_ = 16 + NB + 5 * (NB / 2);
    static constexpr bool is_private = true;
    static constexpr bool anonymous = true;
    XECRYPT_RSA rsa;
    py::bytes n, p, q, dp, dq, cr;
    static RsaPrvStruct from_buffer_copy(const py::bytes& data) {
        if (py::len(data) < sizeof_) buffer_too_small_(py::len(data), sizeof_);
        int64_t h = NB / 2;
        int64_t o = 16 + NB;
        return RsaPrvStruct{XECRYPT_RSA::from_buffer_copy(data), data.slice(16, o), data.slice(o, o + h),
                            data.slice(o + h, o + 2 * h), data.slice(o + 2 * h, o + 3 * h),
                            data.slice(o + 3 * h, o + 4 * h), data.slice(o + 4 * h, o + 5 * h)};
    }
};

using XECRYPT_RSAPUB_1024 = RsaPubStruct<128, true>;
using XECRYPT_RSAPUB_1536 = RsaPubStruct<192, false>;
using XECRYPT_RSAPUB_2048 = RsaPubStruct<256, true>;
using XECRYPT_RSAPUB_4096 = RsaPubStruct<512, true>;
using XECRYPT_RSAPRV_1024 = RsaPrvStruct<128>;
using XECRYPT_RSAPRV_1536 = RsaPrvStruct<192>;
using XECRYPT_RSAPRV_2048 = RsaPrvStruct<256>;
using XECRYPT_RSAPRV_4096 = RsaPrvStruct<512>;

// XECRYPT_KEYVAULT (_pack_ = 1): only its layout is ever used.
struct KeyvaultField {
    py::str name;
    int64_t offset;
    int64_t size;
};
struct XECRYPT_KEYVAULT {
    static constexpr int64_t sizeof_ = 0x4000;
    static const std::vector<KeyvaultField>& fields();
};

// ---- utilities --------------------------------------------------------------------------------------
py::bytes reverse(const py::bytes& b);
py::bytes rsa_pad(const py::bytes& data);
BigInt b2i(const py::bytes& b, bool bswap = false);
// i2b(i, size, bswap): OverflowError when i does not fit `size` bytes
py::bytes i2b(const BigInt& i, int64_t size = 0, bool bswap = false);
// rotl / rotr: ((value << shift) | (value >> (bits - shift))) & ((1 << bits) - 1). ValueError for a negative
// shift count; OverflowError when the result is past int64_t (bits >= 64) or for a negative value with
// bits > 4096.
int64_t rotl(int64_t value, int64_t shift, int64_t bits = 32);
int64_t rotr(int64_t value, int64_t shift, int64_t bits = 32);
// bswap(data, fmt): the bytes of each calcsize(fmt)-byte unit reversed. AssertionError when the length is
// not a multiple of it.
py::bytes bswap(const py::bytes& data, const py::str& fmt);
py::bytes bswap16(const py::bytes& data);
py::bytes bswap32(const py::bytes& data);
py::bytes bswap64(const py::bytes& data);
py::bytes XeCryptBnQw_SwapDwQwLeBe(const py::bytes& b);
// all(b0[i] == b1[i] for i in range(size)): IndexError when either is shorter than size
bool memcmp(const py::bytes& b0, const py::bytes& b1, int64_t size);

// random.seed(a): the generator behind XeCryptRandom, XeCryptCpuKeyGen and XeCryptKeyVaultEncrypt
void random_seed(int64_t a);
py::bytes XeCryptRandom(int64_t cb);

// ---- hashing / MAC ----------------------------------------------------------------------------------
py::bytes XeCryptMd5(const py::list<py::bytes>& args);
py::bytes XeCryptSha(const py::list<py::bytes>& args);
py::bytes XeCryptHmacMd5(const py::bytes& key, const py::list<py::bytes>& args);
py::bytes XeCryptHmacSha(const py::bytes& key, const py::list<py::bytes>& args);

// ---- ciphers -----------------------------------------------------------------------------------------
namespace detail {
class BlockModeContext;   // a block cipher run in ECB or CBC, streaming (a partial block is held back)
class Rc4State;
}  // namespace detail

// RC4 (only works in ECB mode!). ValueError for a key that is not 40..256 bits of RC4's sizes.
class XeCryptRc4 {
public:
    explicit XeCryptRc4(const py::bytes& key);
    void reset();
    static XeCryptRc4 new_(const py::bytes& key);
    // encrypt and decrypt are exactly the same for RC4
    py::bytes crypt(const py::bytes& data);
    py::bytes encrypt(const py::bytes& data);
    py::bytes decrypt(const py::bytes& data);

private:
    std::shared_ptr<detail::Rc4State> enc_;
    std::shared_ptr<detail::Rc4State> dec_;
};

class XeCryptAes {
public:
    static constexpr int64_t MODE_ECB = 1;
    static constexpr int64_t MODE_CBC = 2;
    // AssertionError for a key that is not 128 or 256 bits (or a missing IV in CBC); Exception for another
    // mode; ValueError for an IV that is not 16 bytes
    XeCryptAes(const py::bytes& key, std::optional<int64_t> mode = MODE_ECB, const std::optional<py::bytes>& iv = std::nullopt);
    void reset();
    static XeCryptAes new_(const py::bytes& key, int64_t mode = MODE_ECB, const std::optional<py::bytes>& iv = std::nullopt);
    py::bytes encrypt(const py::bytes& data);
    py::bytes decrypt(const py::bytes& data);

private:
    std::shared_ptr<detail::BlockModeContext> enc_;
    std::shared_ptr<detail::BlockModeContext> dec_;
};

// XeCryptDes (64-bit key), XeCryptDes2 (128-bit) and XeCryptDes3 (192-bit) are the same class in the Python
// but for the key size: TripleDES(key) under ECB or CBC (a 64-bit key is single DES, a 128-bit one k1,k2,k1).
template <int64_t KeyBits>
class TripleDesCipher {
public:
    static constexpr int64_t MODE_ECB = 1;
    static constexpr int64_t MODE_CBC = 2;
    TripleDesCipher(const py::bytes& key, std::optional<int64_t> mode = MODE_ECB, const std::optional<py::bytes>& iv = std::nullopt);
    void reset();
    static TripleDesCipher new_(const py::bytes& key, int64_t mode = MODE_ECB, const std::optional<py::bytes>& iv = std::nullopt);
    py::bytes encrypt(const py::bytes& data);
    py::bytes decrypt(const py::bytes& data);

private:
    std::shared_ptr<detail::BlockModeContext> enc_;
    std::shared_ptr<detail::BlockModeContext> dec_;
};
using XeCryptDes = TripleDesCipher<64>;
using XeCryptDes2 = TripleDesCipher<128>;
using XeCryptDes3 = TripleDesCipher<192>;

py::bytes XeCryptDesParity(const py::bytes& data);
// IndexError when data is shorter than 8 bytes (or key / sbox too short for what they are indexed with)
py::bytes XeCryptParveEcb(const py::bytes& key, const py::bytes& sbox, const py::bytes& data);
// struct.error for an iv that is not 8 bytes (when data is 8 or more), or a data that is not a multiple of 8
py::bytes XeCryptParveCbcMac(const py::bytes& key, const py::bytes& sbox, const py::bytes& iv, const py::bytes& data);
// struct.error for an ab / cd that is not 8 bytes or a data that is not a multiple of 8
py::bytes XeCryptChainAndSumMac(const py::bytes& cd, const py::bytes& ab, const py::bytes& data);

// ---- checksums --------------------------------------------------------------------------------------
// None for data shorter than 8 bytes
std::optional<py::bytes> XeCryptRotSum(const py::bytes& data);
// TypeError for data shorter than 8 bytes (the Python hands None to bytearray())
py::bytes XeCryptRotSumSha(const py::bytes& data);

// ---- RSA ---------------------------------------------------------------------------------------------
// pow(1 << 64, -1, v): ValueError when v is 0 or even
BigInt XeCryptBnQwNeModInv(const BigInt& v);
BigInt XeCryptBnQwNeModExpRoot(const BigInt& c, const BigInt& p, const BigInt& q, const BigInt& dp, const BigInt& dq, const BigInt& u);
// AssertionError when the length is not a multiple of 8. The bytearray overload swaps in place.
py::bytearray XeCryptBnQwBeBufSwap(py::bytearray data);
py::bytes XeCryptBnQwBeBufSwap(const py::bytes& data);
// (public key, private key) of a new key; AssertionError for cbits not in 1024, 1536, 2048, 4096; ValueError
// for an exponent that is not 3 or 65537. The primes come from std::random_device (cryptography's come from
// OpenSSL; neither is seedable), so the key itself differs from run to run.
std::tuple<py::bytes, py::bytes> XeCryptBnQwNeRsaKeyGen(int64_t cbits = 2048, int64_t exp = 0x10001);
py::bytes XeCryptBnQwBeSigFormat(int64_t cqw, const py::bytes& b_hash, const py::bytes& salt);

class XeCryptRsaKey;
py::bytes XeCryptBnQwBeSigCreate(const py::bytes& b_hash, const py::bytes& salt, const py::bytes& prv_key);
bool XeCryptBnQwBeSigVerify(const py::bytes& sig, const py::bytes& b_hash, const py::bytes& salt, const py::bytes& pub_key);
// None when cb_sig is outside 0x27..0x200
std::optional<py::bytes> XeCryptBnDwLePkcs1Format(const py::bytes& b_hash, int64_t fmt_type, int64_t cb_sig);
// None when sig is outside 0x27..0x200 bytes
std::optional<bool> XeCryptBnDwLePkcs1Verify(const py::bytes& sig, const py::bytes& b_hash, int64_t cb_sig);
// None when the key is not 1..0x40 qwords
std::optional<py::bytes> XeKeysPkcs1Create(const py::bytes& b_hash, const py::bytes& prv_key);
bool XeKeysPkcs1Verify(const py::bytes& sig, const py::bytes& b_hash, const py::bytes& pub_key);
py::bytes XeCryptBnQwNeRsaPrvCrypt(const py::bytes& data, const py::bytes& prv_key);
py::bytes XeCryptBnQwNeRsaPubCrypt(const py::bytes& data, const py::bytes& pub_key);

// ---- utility -----------------------------------------------------------------------------------------
py::bytes XeCryptSmcDecrypt(const py::bytes& data);
py::bytes XeCryptSmcEncrypt(const py::bytes& data);
int64_t XeCryptHammingWeight(const py::bytes& data);
// IndexError for data shorter than 16 bytes
py::bytes XeCryptUidEccEncode(const py::bytes& data);

// additions to the XeCrypt library that didn't exist in the original
bool XeCryptCpuKeyValid(const py::bytes& cpu_key);
py::bytes XeCryptCpuKeyGen();
py::bytes XeCryptKeyVaultDecrypt(const py::bytes& cpu_key, const py::bytes& data);
// the bytearray overload writes the nonce / obfuscation / digest into `data` as the Python does
py::bytes XeCryptKeyVaultEncrypt(const py::bytes& cpu_key, py::bytearray data);
py::bytes XeCryptKeyVaultEncrypt(const py::bytes& cpu_key, const py::bytes& data);
bool XeCryptKeyVaultVerify(const py::bytes& cpu_key, const py::bytes& data, const py::bytes& pub_key);
// the bytearray overload sets the check bits in `data` as the Python does
py::bytes XeCryptPageEccEncode(py::bytearray data);
py::bytes XeCryptPageEccEncode(const py::bytes& data);

// ---- managed public key "interfaces" ------------------------------------------------------------------
using KeyStruct = std::variant<XECRYPT_RSAPUB_1024, XECRYPT_RSAPUB_1536, XECRYPT_RSAPUB_2048, XECRYPT_RSAPUB_4096,
                               XECRYPT_RSAPRV_1024, XECRYPT_RSAPRV_1536, XECRYPT_RSAPRV_2048, XECRYPT_RSAPRV_4096>;

// the numbers of cryptography's RSAPrivateKey
struct RSAPrivateKey {
    BigInt p, q, d, dp, dq, iqmp;
    MemCrypto::RSAPublicKey public_numbers;
};

class XeCryptRsaKey {
public:
    std::optional<py::bytes> key_bytes;
    std::optional<XECRYPT_RSA> rsa_struct;
    std::optional<KeyStruct> key_struct;

    // Exception("Invalid key data specified") for a size that is not 1024/1536/2048/4096 bits; ValueError for
    // data shorter than the structure; AssertionError("Key parameters are incorrect!"); AttributeError for a
    // public 1536-bit key (its structure has no `e`, see RsaPubStruct)
    explicit XeCryptRsaKey(const py::bytes& data);
    void reset();
    XeCryptRsaKey& __enter__();
    void __exit__();
    py::bytes __bytes__() const;
    int64_t __len__() const;
    py::bytes to_bytes() const;
    // The numbers of the key as cryptography's key objects hold them. The Python builds the key through
    // RSAPrivateNumbers.private_key(), which also runs OpenSSL's key check (the primality of p and q); that
    // check is not repeated here -- verify_parameters has already established the rest.
    std::variant<MemCrypto::RSAPublicKey, RSAPrivateKey> to_cryptography();
    static XeCryptRsaKey new_(int64_t bits = 2048, int64_t exp = 0x10001);

    XeCryptRsaKey public_key();   // (a public key returns itself: a copy here)
    const KeyStruct& c_struct() const;
    bool is_private_key() const;
    int64_t n_size_in_bytes() const;
    int64_t half_n_in_bytes() const;
    int64_t n_size_in_bits() const;
    py::str struct_name() const;
    int64_t cqw() const;
    BigInt mod_inv() const;
    BigInt n() const;
    int64_t e() const;
    BigInt d() const;
    BigInt p() const;
    BigInt q() const;
    BigInt dp() const;
    BigInt dq() const;
    BigInt u() const;
    BigInt r() const;
    BigInt inv_r() const;
    BigInt inv_q() const;

    bool verify_parameters() const;
    py::bytes prv_crypt(const BigInt& n) const;
    py::bytes prv_crypt(const py::bytes& n) const;
    py::bytes pub_crypt(const BigInt& n) const;
    py::bytes pub_crypt(const py::bytes& n) const;
    py::bytes sig_create(const py::bytes& hash, const py::bytes& salt);
    bool sig_verify(const py::bytes& sig, const py::bytes& hash, const py::bytes& salt);
    std::optional<py::bytes> sig_create_pkcs1(const py::bytes& hash);
    bool sig_verify_pkcs1(const py::bytes& sig, const py::bytes& hash);
};

}  // namespace codebreaker::formats::xbox::crypto::XeCrypt
