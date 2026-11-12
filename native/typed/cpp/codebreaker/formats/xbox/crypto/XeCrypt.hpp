// codebreaker/formats/xbox/crypto/XeCrypt.hpp -- compat shim over cb/formats/xbox/crypto/xecrypt.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/xbox/crypto/xecrypt.{hpp,cpp} (+ xecrypt_bn, xecrypt_rsa); this header keeps the old py::
// surface for the ports that have not been refactored yet (build_lib, harness, shadowboot, xsm3, xcp_dumper, ...) and is deleted
// with the last of them. Conversion and forwarding only. Not carried over, because nothing used it: the ctypes structure
// records, the BigInt-valued helpers (b2i, i2b, rotl, rotr, XeCryptBnQwNeModInv, ModExpRoot, to_cryptography) and the
// accessors of XeCryptRsaKey that returned BigInts. `random_seed` is gone with Python's Mersenne Twister: randomness is the
// operating system's (the C++ API takes a RandomSource for repeatable runs).
#pragma once

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/formats/xbox/crypto/xecrypt.hpp"
#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::XeCrypt {

namespace xc = cb::formats::xbox::crypto::xecrypt;

using BigInt = MemCrypto::BigInt;
using BYTE = uint8_t;
using WORD = uint16_t;
using DWORD = uint32_t;
using QWORD = uint64_t;

namespace detail {
// the bytes of a py::bytes as a view (valid while the argument is)
inline cb::ByteView V(const py::bytes& b) { return cb::as_bytes(b.raw()); }
inline py::bytes P(cb::ByteView b) { return cb::compat::to_py(b); }
inline std::vector<cb::ByteView> parts_of(const py::list<py::bytes>& args) {
    std::vector<cb::ByteView> out;
    for (const py::bytes& a : args.vec()) out.push_back(V(a));
    return out;
}
}  // namespace detail

// ---- constants --------------------------------------------------------------------------------------------------
inline const py::bytes XECRYPT_SMC_KEY = detail::P(xc::kSmcKey);
// a key from the key store (the xbox360_keys setting), None when not there
inline std::optional<py::bytes> _store_key(const py::str& name) { return MemCrypto::_store_key(name); }
inline const std::optional<py::bytes>& XECRYPT_1BL_KEY() { return MemCrypto::_1BL_KEY(); }
inline const py::bytes XECRYPT_1BL_SALT = py::bytes(std::string(xc::kSalt1bl));
inline const py::bytes XECRYPT_SC_SALT = py::bytes(std::string(xc::kSaltSc));
inline const py::bytes XECRYPT_SD_SALT = py::bytes(std::string(xc::kSaltSd));
inline constexpr int64_t BUFFER_SIZE = 4096;

inline constexpr int64_t XECRYPT_RSAPUB_1024_SIZE = 0x90;
inline constexpr int64_t XECRYPT_RSAPUB_1536_SIZE = 0xD0;
inline constexpr int64_t XECRYPT_RSAPUB_2048_SIZE = 0x110;
inline constexpr int64_t XECRYPT_RSAPUB_4096_SIZE = 0x210;
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

// ---- the keyvault layout (a table of name, offset, size) ----------------------------------------------------------
// from_buffer_copy(data): ValueError ("Buffer size too small") when data is shorter than the structure
[[noreturn]] inline void buffer_too_small_(int64_t have, int64_t need) {
    throw py::ValueError("Buffer size too small (" + std::to_string(have) + " instead of at least " + std::to_string(need) + " bytes)");
}
struct KeyvaultField {
    py::str name;
    int64_t offset;
    int64_t size;
};
struct XECRYPT_KEYVAULT {
    static constexpr int64_t sizeof_ = 0x4000;
    static const std::vector<KeyvaultField>& fields() {
        static const std::vector<KeyvaultField> table = [] {
            std::vector<KeyvaultField> t;
            for (const xc::KeyvaultField& f : xc::keyvault_layout()) t.push_back({cb::compat::to_py(f.name), static_cast<int64_t>(f.offset), static_cast<int64_t>(f.size)});
            return t;
        }();
        return table;
    }
};

// ---- byte order ---------------------------------------------------------------------------------------------------
inline py::bytes reverse(const py::bytes& b) { return detail::P(xc::reversed(detail::V(b))); }
inline py::bytes bswap16(const py::bytes& d) { return cb::compat::guard([&] { return detail::P(xc::swap_words(detail::V(d))); }); }
inline py::bytes bswap32(const py::bytes& d) { return cb::compat::guard([&] { return detail::P(xc::swap_dwords(detail::V(d))); }); }
inline py::bytes bswap64(const py::bytes& d) { return cb::compat::guard([&] { return detail::P(xc::swap_qwords(detail::V(d))); }); }
inline py::bytes XeCryptBnQw_SwapDwQwLeBe(const py::bytes& b) { return bswap64(b); }
inline py::bytes XeCryptRandom(int64_t cb) { return detail::P(xc::os_random().bytes(static_cast<std::size_t>(cb))); }

// ---- hashing / MAC ------------------------------------------------------------------------------------------------
inline py::bytes XeCryptMd5(const py::list<py::bytes>& args) { auto p = detail::parts_of(args); return detail::P(xc::md5(xc::PartSpan(p))); }
inline py::bytes XeCryptSha(const py::list<py::bytes>& args) { auto p = detail::parts_of(args); return detail::P(xc::sha(xc::PartSpan(p))); }
inline py::bytes XeCryptHmacMd5(const py::bytes& key, const py::list<py::bytes>& args) { auto p = detail::parts_of(args); return detail::P(xc::hmac_md5(detail::V(key), xc::PartSpan(p))); }
inline py::bytes XeCryptHmacSha(const py::bytes& key, const py::list<py::bytes>& args) { auto p = detail::parts_of(args); return detail::P(xc::hmac_sha(detail::V(key), xc::PartSpan(p))); }

// ---- ciphers --------------------------------------------------------------------------------------------------------
// RC4 (only works in ECB mode!)
class XeCryptRc4 {
public:
    explicit XeCryptRc4(const py::bytes& key) : rc4_(cb::compat::guard([&] { return std::make_shared<xc::Rc4>(detail::V(key)); })) {}
    void reset() { rc4_.reset(); }
    static XeCryptRc4 new_(const py::bytes& key) { return XeCryptRc4(key); }
    py::bytes crypt(const py::bytes& data) { return encrypt(data); }
    py::bytes encrypt(const py::bytes& data) { return detail::P(rc4_->encrypt(detail::V(data))); }
    py::bytes decrypt(const py::bytes& data) { return detail::P(rc4_->decrypt(detail::V(data))); }

private:
    std::shared_ptr<xc::Rc4> rc4_;
};

// AES / DES / DES2 / DES3 in ECB or CBC mode (a partial block is held back until the next call completes it)
template <xc::CipherAlgorithm Algorithm>
class BlockCipherShim {
public:
    static constexpr int64_t MODE_ECB = 1;
    static constexpr int64_t MODE_CBC = 2;
    BlockCipherShim(const py::bytes& key, std::optional<int64_t> mode = MODE_ECB, const std::optional<py::bytes>& iv = std::nullopt)
        : cipher_(cb::compat::guard([&] {
              if (mode != MODE_ECB && mode != MODE_CBC) throw py::ValueError("Invalid cipher mode entered");
              return std::make_shared<xc::BlockCipher>(Algorithm, detail::V(key), mode == MODE_CBC ? xc::CipherMode::Cbc : xc::CipherMode::Ecb,
                                                       iv ? detail::V(*iv) : cb::ByteView());
          })) {}
    void reset() { cipher_.reset(); }
    static BlockCipherShim new_(const py::bytes& key, int64_t mode = MODE_ECB, const std::optional<py::bytes>& iv = std::nullopt) { return BlockCipherShim(key, mode, iv); }
    py::bytes encrypt(const py::bytes& data) { return cb::compat::guard([&] { return detail::P(cipher_->encrypt(detail::V(data))); }); }
    py::bytes decrypt(const py::bytes& data) { return cb::compat::guard([&] { return detail::P(cipher_->decrypt(detail::V(data))); }); }

private:
    std::shared_ptr<xc::BlockCipher> cipher_;
};
using XeCryptAes = BlockCipherShim<xc::CipherAlgorithm::Aes>;
using XeCryptDes = BlockCipherShim<xc::CipherAlgorithm::Des>;
using XeCryptDes2 = BlockCipherShim<xc::CipherAlgorithm::Des2>;
using XeCryptDes3 = BlockCipherShim<xc::CipherAlgorithm::Des3>;

inline py::bytes XeCryptDesParity(const py::bytes& d) { return detail::P(xc::des_parity(detail::V(d))); }
inline py::bytes XeCryptParveEcb(const py::bytes& key, const py::bytes& sbox, const py::bytes& data) {
    return cb::compat::guard([&] { return detail::P(xc::parve_ecb(detail::V(key), detail::V(sbox), detail::V(data).first(std::min<std::size_t>(8, detail::V(data).size())))); });
}
inline py::bytes XeCryptParveCbcMac(const py::bytes& key, const py::bytes& sbox, const py::bytes& iv, const py::bytes& data) {
    return cb::compat::guard([&] { return detail::P(xc::parve_cbc_mac(detail::V(key), detail::V(sbox), detail::V(iv), detail::V(data))); });
}
inline py::bytes XeCryptChainAndSumMac(const py::bytes& cd, const py::bytes& ab, const py::bytes& data) {
    return cb::compat::guard([&] { return detail::P(xc::chain_and_sum_mac(detail::V(cd), detail::V(ab), detail::V(data))); });
}

// ---- checksums ----------------------------------------------------------------------------------------------------
// None for data shorter than 8 bytes (the C++ module answers zeros there)
inline std::optional<py::bytes> XeCryptRotSum(const py::bytes& data) {
    if (data.size() < 8) return std::nullopt;
    return detail::P(xc::rot_sum(detail::V(data)));
}
inline py::bytes XeCryptRotSumSha(const py::bytes& data) { return detail::P(xc::rot_sum_sha(detail::V(data))); }

// ---- RSA ----------------------------------------------------------------------------------------------------------
inline py::bytes XeCryptBnQwBeBufSwap(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes in = cb::to_bytes(data.raw());
        cb::Bytes out(in.size());
        if (in.size() % 8 != 0) throw cb::UsageError("a BnQw buffer is a whole number of qwords");
        const std::size_t qwords = in.size() / 8;
        for (std::size_t i = 0; i < qwords; ++i) std::copy_n(in.begin() + static_cast<std::ptrdiff_t>(i * 8), 8, out.begin() + static_cast<std::ptrdiff_t>((qwords - 1 - i) * 8));
        return detail::P(out);
    });
}
inline py::bytearray XeCryptBnQwBeBufSwap(py::bytearray data) {
    py::bytes swapped = XeCryptBnQwBeBufSwap(data.view());
    data.raw_mut() = swapped.raw();
    return data;
}

class XeCryptRsaKey {
public:
    // Exception("Invalid key data specified") / ValueError for a key that does not hold together
    explicit XeCryptRsaKey(const py::bytes& data) : key_(cb::compat::guard([&] { return xc::RsaKey::parse(detail::V(data)); })) {}
    explicit XeCryptRsaKey(xc::RsaKey key) : key_(std::move(key)) {}
    void reset() {}
    py::bytes to_bytes() const { return detail::P(key_.bytes()); }
    static XeCryptRsaKey new_(int64_t bits = 2048, int64_t exp = 0x10001) {
        return cb::compat::guard([&] { return XeCryptRsaKey(xc::RsaKey::generate(static_cast<unsigned>(bits), static_cast<std::uint32_t>(exp))); });
    }
    XeCryptRsaKey public_key() const { return XeCryptRsaKey(key_.public_key()); }
    bool is_private_key() const { return key_.is_private(); }
    int64_t n_size_in_bytes() const { return static_cast<int64_t>(key_.modulus_size()); }
    int64_t n_size_in_bits() const { return static_cast<int64_t>(key_.bits()); }
    int64_t cqw() const { return static_cast<int64_t>(key_.qword_count()); }
    int64_t e() const { return key_.exponent(); }
    // the constructor has already checked the parameters
    bool verify_parameters() const { return true; }
    py::bytes prv_crypt(const py::bytes& n) const { return cb::compat::guard([&] { return detail::P(key_.prv_crypt(detail::V(n))); }); }
    py::bytes pub_crypt(const py::bytes& n) const { return cb::compat::guard([&] { return detail::P(key_.pub_crypt(detail::V(n))); }); }
    py::bytes sig_create(const py::bytes& hash, const py::bytes& salt) { return cb::compat::guard([&] { return detail::P(key_.sign(detail::V(hash), detail::V(salt))); }); }
    bool sig_verify(const py::bytes& sig, const py::bytes& hash, const py::bytes& salt) { return cb::compat::guard([&] { return key_.public_key().verify(detail::V(sig), detail::V(hash), detail::V(salt)); }); }
    std::optional<py::bytes> sig_create_pkcs1(const py::bytes& hash) { return cb::compat::guard([&] { return std::optional<py::bytes>(detail::P(key_.sign_pkcs1(detail::V(hash)))); }); }
    bool sig_verify_pkcs1(const py::bytes& sig, const py::bytes& hash) { return cb::compat::guard([&] { return key_.public_key().verify_pkcs1(detail::V(sig), detail::V(hash)); }); }
    const xc::RsaKey& cb_key() const { return key_; }

private:
    xc::RsaKey key_;
};

// (public key, private key) of a new key
inline std::tuple<py::bytes, py::bytes> XeCryptBnQwNeRsaKeyGen(int64_t cbits = 2048, int64_t exp = 0x10001) {
    return cb::compat::guard([&] {
        const xc::KeyPair pair = xc::generate_key_pair(static_cast<unsigned>(cbits), static_cast<std::uint32_t>(exp));
        return std::make_tuple(detail::P(pair.public_key), detail::P(pair.private_key));
    });
}
inline py::bytes XeCryptBnQwBeSigFormat(int64_t cqw, const py::bytes& hash, const py::bytes& salt) {
    return cb::compat::guard([&] { return detail::P(xc::sig_format(static_cast<std::size_t>(cqw), detail::V(hash), detail::V(salt))); });
}
inline py::bytes XeCryptBnQwBeSigCreate(const py::bytes& hash, const py::bytes& salt, const py::bytes& prv_key) {
    return cb::compat::guard([&] { return detail::P(xc::sig_create(detail::V(hash), detail::V(salt), xc::RsaKey::parse(detail::V(prv_key)))); });
}
inline bool XeCryptBnQwBeSigVerify(const py::bytes& sig, const py::bytes& hash, const py::bytes& salt, const py::bytes& pub_key) {
    return cb::compat::guard([&] { return xc::sig_verify(detail::V(sig), detail::V(hash), detail::V(salt), xc::RsaKey::parse(detail::V(pub_key))); });
}
// None when cb_sig is outside 0x27..0x200
inline std::optional<py::bytes> XeCryptBnDwLePkcs1Format(const py::bytes& hash, int64_t fmt_type, int64_t cb_sig) {
    if (cb_sig < 0x27 || cb_sig > 0x200) return std::nullopt;
    return cb::compat::guard([&] { return std::optional<py::bytes>(detail::P(xc::pkcs1_format(detail::V(hash), static_cast<int>(fmt_type), static_cast<std::size_t>(cb_sig)))); });
}
// None when sig is outside 0x27..0x200 bytes
inline std::optional<bool> XeCryptBnDwLePkcs1Verify(const py::bytes& sig, const py::bytes& hash, int64_t cb_sig) {
    if (sig.size() < 0x27 || sig.size() > 0x200) return std::nullopt;
    return xc::pkcs1_verify(detail::V(sig), detail::V(hash), static_cast<std::size_t>(cb_sig));
}
inline std::optional<py::bytes> XeKeysPkcs1Create(const py::bytes& hash, const py::bytes& prv_key) {
    return cb::compat::guard([&] { return std::optional<py::bytes>(detail::P(xc::RsaKey::parse(detail::V(prv_key)).sign_pkcs1(detail::V(hash)))); });
}
inline bool XeKeysPkcs1Verify(const py::bytes& sig, const py::bytes& hash, const py::bytes& pub_key) {
    return cb::compat::guard([&] { return xc::RsaKey::parse(detail::V(pub_key)).verify_pkcs1(detail::V(sig), detail::V(hash)); });
}
inline py::bytes XeCryptBnQwNeRsaPrvCrypt(const py::bytes& data, const py::bytes& prv_key) {
    return cb::compat::guard([&] { return detail::P(xc::RsaKey::parse(detail::V(prv_key)).prv_crypt(detail::V(data))); });
}
inline py::bytes XeCryptBnQwNeRsaPubCrypt(const py::bytes& data, const py::bytes& pub_key) {
    return cb::compat::guard([&] { return detail::P(xc::RsaKey::parse(detail::V(pub_key)).pub_crypt(detail::V(data))); });
}

// ---- utility ------------------------------------------------------------------------------------------------------
inline py::bytes XeCryptSmcDecrypt(const py::bytes& data) { return detail::P(xc::smc_decrypt(detail::V(data))); }
inline py::bytes XeCryptSmcEncrypt(const py::bytes& data) { return detail::P(xc::smc_encrypt(detail::V(data))); }
inline int64_t XeCryptHammingWeight(const py::bytes& data) { return static_cast<int64_t>(xc::hamming_weight(detail::V(data))); }
inline py::bytes XeCryptUidEccEncode(const py::bytes& data) { return cb::compat::guard([&] { return detail::P(xc::uid_ecc_encode(detail::V(data))); }); }

// additions to the XeCrypt library that didn't exist in the original
inline bool XeCryptCpuKeyValid(const py::bytes& cpu_key) { return xc::cpu_key_valid(detail::V(cpu_key)); }
inline py::bytes XeCryptCpuKeyGen() { return detail::P(xc::cpu_key_generate()); }
inline py::bytes XeCryptKeyVaultDecrypt(const py::bytes& cpu_key, const py::bytes& data) {
    return cb::compat::guard([&] { return detail::P(xc::keyvault_decrypt(detail::V(cpu_key), detail::V(data)).value()); });
}
// the bytearray overload writes the obfuscation key / digest into `data`, as the Python does
inline py::bytes XeCryptKeyVaultEncrypt(const py::bytes& cpu_key, py::bytearray data) {
    return cb::compat::guard([&] {
        cb::Bytes vault = cb::to_bytes(data.raw());
        const py::bytes sealed = detail::P(xc::keyvault_encrypt(detail::V(cpu_key), vault));
        data.raw_mut() = std::string(cb::as_text(vault));
        return sealed;
    });
}
inline py::bytes XeCryptKeyVaultEncrypt(const py::bytes& cpu_key, const py::bytes& data) { return XeCryptKeyVaultEncrypt(cpu_key, py::bytearray(data)); }
inline bool XeCryptKeyVaultVerify(const py::bytes& cpu_key, const py::bytes& data, const py::bytes& pub_key) {
    return cb::compat::guard([&] { return xc::keyvault_verify(detail::V(cpu_key), detail::V(data), xc::RsaKey::parse(detail::V(pub_key))); });
}
// the bytearray overload sets the check bits in `data`
inline py::bytes XeCryptPageEccEncode(py::bytearray data) {
    return cb::compat::guard([&] {
        cb::Bytes page = cb::to_bytes(data.raw());
        xc::page_ecc_encode_in_place(page);
        data.raw_mut() = std::string(cb::as_text(page));
        return detail::P(page);
    });
}
inline py::bytes XeCryptPageEccEncode(const py::bytes& data) { return XeCryptPageEccEncode(py::bytearray(data)); }

}  // namespace codebreaker::formats::xbox::crypto::XeCrypt
