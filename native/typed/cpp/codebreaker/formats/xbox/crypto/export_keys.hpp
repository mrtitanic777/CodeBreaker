// codebreaker/formats/xbox/crypto/export_keys.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.export_keys.
//
// Prints the custom keypair (Keys/custom_prv.bin, Keys/custom_pub.bin) as C++ byte arrays, then a test
// vector: 0x100 random bytes and their RSA signature made with the private key.
//
// Typing notes:
//   * The random bytes come from std::random_device (os.urandom in the Python).
//   * lang_format prints through bin2lang's own streams (stdout unless bin2lang::set_streams was called).
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::export_keys {

void main();

}  // namespace codebreaker::formats::xbox::crypto::export_keys
