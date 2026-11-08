// codebreaker/formats/xbox/crypto/MemCrypto.hpp -- C++ port of codebreaker.formats.xbox.crypto.MemCrypto.
//
// Xbox 360 hypervisor memory encryption (the AES-ECB + GF(2) tweak scheme), SRAM checksums, the XOSC
// challenge-response hashes and the key blob.
//
// References:
//   https://github.com/CWest07/Xbox-360-Hypervisor-Manager/blob/main/Form1.cs
//   https://gist.github.com/SciresM/a4b9ae50a9ae89e6119c4de9def22435#file-aes128-py
//
// Typing notes:
//   * Python's int is unbounded and this module genuinely uses 128..164-bit and 1024-bit values (the
//     AES tweak, the RSA modulus). Those are `BigInt` (an unsigned arbitrary-precision integer, below);
//     everything else is int64_t. The masks UINT64_MASK and UINT128_MASK, which do not fit int64_t, are
//     BigInt-valued functions.
//   * `bytes | bytearray` parameters are py::bytes (a py::bytearray converts to it). The one function
//     that changes its buffer, pack_sec_eng_keys, takes a py::bytearray (a handle: it changes in place
//     and returns the same object, as the Python does). MemoryCrypto keeps its three keys as py::bytes
//     copies (the Python keeps the caller's object, so a later change to a bytearray key would show
//     through there and not here).
//   * `cryptography`'s Cipher(AES(key), ECB()) is `AesEcb` here (written in this module: the runtime has
//     no AES); `Hash(SHA1())` is hashlib::sha1. RSAPublicNumbers(e, n).public_key() is `RSAPublicKey`.
//   * `address: int = None` is std::optional<int64_t>.
//   * module-level state that Python builds at import time: GF2_TAB (filled by main()), MASTER_PUB and
//     _1BL_KEY (read from the key store: a function here, read on first use, since the setting is not
//     known at static-initialisation time).
//   * main() takes the argument list (sys.argv[1:]) and returns the exit code; argparse's exits (--help,
//     an error) are `SystemExit`.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::MemCrypto {

// ---- an unsigned arbitrary-precision integer (the part of Python's int this module needs) ----------
class BigInt {
public:
    BigInt() = default;
    BigInt(uint64_t v);   // (implicit: a BigInt stands where the Python writes an int literal)
    static BigInt from_bytes(const py::bytes& b, const char* order);   // int.from_bytes(b, order)
    static BigInt from_hex(const std::string& hex);                    // int(hex, 16)
    // the value of the signed int64 v taken as a two's-complement number of `bits` bits (v & ((1<<bits)-1))
    static BigInt from_signed_masked(int64_t v, int64_t bits);
    py::bytes to_bytes(int64_t length, const char* order) const;   // int.to_bytes(length, order); OverflowError
    int64_t to_int64() const;                                       // OverflowError past int64_t
    int64_t bit_length() const;
    bool is_zero() const { return limbs_.empty(); }
    bool bit(int64_t i) const;
    BigInt shl(int64_t n) const;   // ValueError on a negative count
    BigInt shr(int64_t n) const;
    BigInt masked(int64_t bits) const;   // self & ((1 << bits) - 1)
    int64_t low_byte() const { return limbs_.empty() ? 0 : static_cast<int64_t>(limbs_[0] & 0xFF); }
    int compare(const BigInt& o) const;   // -1, 0, 1
    friend bool operator==(const BigInt& a, const BigInt& b) { return a.compare(b) == 0; }
    friend bool operator!=(const BigInt& a, const BigInt& b) { return a.compare(b) != 0; }
    friend bool operator<(const BigInt& a, const BigInt& b) { return a.compare(b) < 0; }
    friend bool operator<=(const BigInt& a, const BigInt& b) { return a.compare(b) <= 0; }
    friend BigInt operator^(const BigInt& a, const BigInt& b);
    friend BigInt operator&(const BigInt& a, const BigInt& b);
    friend BigInt operator|(const BigInt& a, const BigInt& b);
    friend BigInt operator+(const BigInt& a, const BigInt& b);
    friend BigInt operator-(const BigInt& a, const BigInt& b);   // ValueError if b > a (no negative BigInts)
    friend BigInt operator*(const BigInt& a, const BigInt& b);
    friend BigInt operator/(const BigInt& a, const BigInt& b);   // ZeroDivisionError
    friend BigInt operator%(const BigInt& a, const BigInt& b);   // ZeroDivisionError
    // pow(base, exp, mod): ValueError when mod is 0
    static BigInt powmod(const BigInt& base, const BigInt& exp, const BigInt& mod);
    // pow(base, -1, mod): ValueError when base is not invertible
    static BigInt modinv(const BigInt& base, const BigInt& mod);

private:
    std::vector<uint32_t> limbs_;   // little-endian, no trailing zero limb
    void trim_();
    static void divmod_(const BigInt& a, const BigInt& b, BigInt& q, BigInt& r);
};

// ---- AES-ECB, the part of cryptography's Cipher(AES(key), ECB()) used here ---------------------------
// A streaming context: update() returns the blocks completed so far and holds a partial block back.
class AesEcb {
public:
    // ValueError for a key that is not 16, 24 or 32 bytes ("Invalid key size (N) for AES.")
    AesEcb(const py::bytes& key, bool decrypt);
    py::bytes update(const py::bytes& data);

private:
    std::vector<uint8_t> round_keys_;
    int rounds_ = 0;
    bool decrypt_ = false;
    std::string pending_;
};

// RSAPublicKey (from RSAPublicNumbers(e, n).public_key())
struct RSAPublicKey {
    BigInt e;
    BigInt n;
    int64_t key_size() const { return n.bit_length(); }   // (cryptography rounds to a whole bit count: the modulus's bits)
};

// argparse's exit: --help (code 0) or an error (code 2), after the text it printed
struct SystemExit : py::Exception {
    int64_t code;
    explicit SystemExit(int64_t c) : py::Exception(std::to_string(c)), code(c) {}
    const char* type() const override { return "SystemExit"; }
};

// ---- constants ------------------------------------------------------------------------------------
extern const py::bytes ALL_55_KEY;                // b"\x55" * 0x10
inline constexpr int64_t GF2_IV = 0;
inline constexpr int64_t GF2_POLY = 0x87;
inline constexpr int64_t HVEX_ADDR = 0x01B5;
inline constexpr int64_t HV_17559_TABLE_ADDR = 0x10878;
inline constexpr int64_t SRAM_CKSM_PAGE_SIZE = 0x80;

// the 1BL key from the key store ("1BL_key.bin" in the xbox360_keys setting's folder), None when not there
const std::optional<py::bytes>& _1BL_KEY();

// rsa values
extern const BigInt MASTER_N;
inline constexpr int64_t MASTER_E = 0x10001;
extern const RSAPublicKey MASTER_PUB;

// create_mask = lambda n: (1 << n) - 1
BigInt create_mask(int64_t n);

// runtime
extern py::list<int64_t> GF2_TAB;   // filled by main(); empty until then

// masks
inline constexpr int64_t UINT8_MASK = 0xFF;
inline constexpr int64_t UINT16_MASK = 0xFFFF;
inline constexpr int64_t UINT32_MASK = 0xFFFFFFFFLL;
inline constexpr int64_t UINT36_MASK = 0xFFFFFFFFFLL;
BigInt UINT64_MASK();
BigInt UINT128_MASK();

// ---- functions ------------------------------------------------------------------------------------
// the key store folder's file `name` under the xbox360_crypto_bin setting (upstream's bin/ folder)
py::str _bin(const py::str& name);
// a key from the key store (the xbox360_keys setting), None when not there
std::optional<py::bytes> _store_key(const py::str& name);

py::bytes read_file(const py::str& filename);
void write_file(const py::str& filename, const py::bytes& data);

// (n >> d) | (n << (b - d)) & UINT{b}_MASK. Typed over int64_t: b in 8, 16, 32, 36, 64 for any n; 128 for
// n >= 0; any other b is Python's NameError (UINT{b}_MASK does not exist); a result past int64_t is
// OverflowError.
int64_t rotr(int64_t n, int64_t d, int64_t b);

py::bytes sxor_u32(const py::bytes& s1, const py::bytes& s2);
py::bytes sand_u32(const py::bytes& s1, const py::bytes& s2);
py::bytes sxor_b(const py::bytes& s1, const py::bytes& s2);
py::bytes read_chunk(const py::bytes& data, int64_t offset, int64_t size);
// (the Python generator, run to the end: its assertion is raised at the call)
py::list<py::bytes> read_chunks(const py::bytes& data, int64_t offset, int64_t size);

py::bytes rsa_encrypt(const RSAPublicKey& key, const py::bytes& data);
py::list<int64_t> generate_gf2_table(int64_t iv, int64_t poly);
py::bytearray pack_sec_eng_keys(const py::bytes& key, py::bytearray buffer, int64_t offset, int64_t length);

class MemoryCrypto {
public:
    std::optional<AesEcb> aesdec;   // None until built, and after reset()
    std::optional<AesEcb> aesenc;

    py::bytes white_key;
    py::bytes aes_key;
    py::bytes hash_key;

    MemoryCrypto(const py::bytes& wkey, const py::bytes& akey, const py::bytes& hkey);

    MemoryCrypto& __enter__();
    void __exit__();   // (exc_type, exc_val, exc_tb are not used)

    void reset();

    int64_t sram_offset_to_hv_offset(int64_t sram_offset);
    int64_t sram_size_to_hv_size(int64_t sram_size);

    BigInt get_tweak_1(const BigInt& n);
    py::bytes get_tweak_0(const std::optional<int64_t>& address);
    int64_t fix_address(int64_t address);

    py::bytes encrypt_block(const py::bytes& dec_data, int64_t offset, int64_t size, std::optional<int64_t> address = std::nullopt,
                            bool offset_is_address = false);
    py::bytes decrypt_block(const py::bytes& enc_data, int64_t offset, int64_t size, std::optional<int64_t> address = std::nullopt,
                            bool offset_is_address = false);
    py::bytes encrypt(const py::bytes& hv_data_dec, int64_t offset, int64_t size, std::optional<int64_t> address = std::nullopt,
                      bool offset_is_address = false);
    py::bytes decrypt(const py::bytes& hv_data_enc, int64_t offset, int64_t size, std::optional<int64_t> address = std::nullopt,
                      bool offset_is_address = false);
    py::bytes encrypt_and_calc_checksums(const py::bytes& hv_data_dec, int64_t offset, int64_t size,
                                         std::optional<int64_t> address = std::nullopt, bool offset_is_address = false);

    int64_t calc_sram_checksum(const py::bytes& data);
    py::bytes calc_sram_checksums(const py::bytes& hv_data_dec, const py::bytes& hv_data_enc, int64_t offset, int64_t size);
    py::bytes get_checksum_chunk_by_sram_offset_and_size(const py::bytes& hv_data_dec, const py::bytes& hv_data_enc,
                                                         int64_t sram_offset, int64_t sram_size);
    py::bytes calc_sram(const py::bytes& hv_data_dec);
    py::bytes calc_hash_1_digest(const py::bytes& hv_data_dec, const py::bytes& salt);
    py::bytes calc_hash_2_digest(const py::bytes& hv_data_dec, const py::bytes& salt, int64_t hvex_addr);
    py::bytes calc_key_blob(const py::bytes& blob_nonce);
    py::bytes calc_100f0(const py::bytes& hv_data_dec, int64_t tbl_addr);
};

// A script to calculate Xbox 360 challenge responses for XOSC. `argv` is sys.argv[1:]; `prog` is the
// script name argparse prints in its usage line.
int64_t main(const py::list<py::str>& argv, const py::str& prog = py::str("MemCrypto.py"));

}  // namespace codebreaker::formats::xbox::crypto::MemCrypto
