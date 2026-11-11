// codebreaker/formats/xbox/crypto/harness.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.harness.
//
// The XeCrypt self-test: known-answer tests of the RSA, RC4, AES, DES, DES3, MD5, SHA-1 / RotSumSha / HMAC and
// the miscellaneous (RotSum, CPU key) functions, run on a test data file and a test RSA key read from Data/.
//
// Typing notes:
//   * The module's variables (TEST_DATA, the expected digests, the test salt ...) are module-level
//     variables that main() sets (TEST_DATA) and a caller may reassign; here they are plain non-const globals.
//   * `valid_cpu_key` is `_store_key("cpu_key.bin")` evaluated at import (the key store folder is not known
//     when a C++ program starts): read the first time it is asked for, then kept; None is std::nullopt.
//   * The tests print to stdout (py::print) and return whether they passed; do_rsa_test generates a new
//     RSA key (XeCryptBnQwNeRsaKeyGen(), 2048 bits) each time it runs.
//   * `unpack_from(f"<{n}s ...", TEST_DATA, 0)` is cut out of TEST_DATA by sizes; a TEST_DATA that is too
//     short is the struct.error of unpack_from.
//   * not ported: the `from .XeCrypt import *` / `from .build_lib import *` names; the
//     `if __name__ == "__main__"` block (`exit(main())`).
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

namespace codebreaker::formats::xbox::crypto::harness {

// test data
extern py::bytes TEST_DATA;

// RSA
extern py::bytes rsa_test_hash;
extern py::bytes rsa_test_salt;

// RC4 ECB
extern py::bytes rc4_1_round;
extern py::bytes rc4_100_round;

// AES ECB
extern py::bytes aes_ecb_1_round;
extern py::bytes aes_ecb_100_round;

// AES CBC
extern py::bytes aes_cbc_1_round;
extern py::bytes aes_cbc_1_round_feed;
extern py::bytes aes_cbc_100_round;
extern py::bytes aes_cbc_100_round_feed;

// DES CBC
extern py::bytes des_1_round;
extern py::bytes des_100_round;
extern py::bytes des_cbc_round;

// DES3 CBC
extern py::bytes des3_1_round;
extern py::bytes des3_100_round;
extern py::bytes des3_cbc_round;

// MD5
extern py::bytes md5_digest;

// SHA-1
extern py::bytes sha_digest;
extern py::bytes rotsumsha_digest;
extern py::bytes hmac_1_digest;
extern py::bytes hmac_2_digest;

// RotSum
extern py::bytes rotsum_digest;

// CPU keys: a console's CPU key from the key store (not committed), None when there is none
const std::optional<py::bytes>& valid_cpu_key();
extern py::bytes invalid_cpu_key;

bool sig_create_verify_test(XeCrypt::XeCryptRsaKey& prv_key);
bool do_rsa_test();
bool do_rc4_test();
bool do_aes_test();
bool do_des_test();
bool do_md5_test();
bool do_sha_test();
bool do_misc_test();

// Runs every test; 0 when all passed, else 1.
int64_t main();

}  // namespace codebreaker::formats::xbox::crypto::harness
