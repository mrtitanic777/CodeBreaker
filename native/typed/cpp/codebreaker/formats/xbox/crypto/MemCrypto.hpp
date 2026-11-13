// codebreaker/formats/xbox/crypto/MemCrypto.hpp -- the old-layer REMNANT of the port of codebreaker.formats.xbox.crypto.MemCrypto.
//
// The memory encryption, SRAM checksums, XOSC hashes and key blob live in native/cb/formats/xbox/crypto/mem_crypto.hpp now
// (nothing in the old tree used the old class: it and its AES, RSA, GF(2) table and main() are gone). What stays here is what other
// old ports still reach through this header and no new module offers: BigInt (a Python-int stand-in the old XeCrypt and STFS ports
// are written on), SystemExit, the key-store readers (_store_key, _1BL_KEY, _bin) and read_file / write_file. It goes with the last
// of those ports.
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

// argparse's exit: --help (code 0) or an error (code 2), after the text it printed
struct SystemExit : py::Exception {
    int64_t code;
    explicit SystemExit(int64_t c) : py::Exception(std::to_string(c)), code(c) {}
    const char* type() const override { return "SystemExit"; }
};

extern const py::bytes ALL_55_KEY;                // sixteen 0x55 bytes

// the 1BL key from the key store ("1BL_key.bin" in the xbox360_keys setting's folder), None when not there
const std::optional<py::bytes>& _1BL_KEY();

// create_mask = lambda n: (1 << n) - 1
BigInt create_mask(int64_t n);
BigInt UINT64_MASK();
BigInt UINT128_MASK();

// the key store folder's file `name` under the xbox360_crypto_bin setting (upstream's bin/ folder)
py::str _bin(const py::str& name);
// a key from the key store (the xbox360_keys setting), None when not there
std::optional<py::bytes> _store_key(const py::str& name);

py::bytes read_file(const py::str& filename);
void write_file(const py::str& filename, const py::bytes& data);

}  // namespace codebreaker::formats::xbox::crypto::MemCrypto
