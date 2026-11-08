// codebreaker/formats/xbox/crypto/create_key.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.create_key.
//
// This script creates an Xbox 360 RSA keypair: a 2048-bit one, written to Keys/custom_pub.bin and
// Keys/custom_prv.bin (relative to the current folder, which must hold a Keys folder: FileNotFoundError
// when it does not).
//
// Typing notes:
//   * The primes come from XeCrypt::XeCryptBnQwNeRsaKeyGen (std::random_device), so the keys differ from run
//     to run, as the Python's do.
//   * The Python's `from .XeCrypt import *` brings the whole XeCrypt library into the module; nothing but
//     XeCryptBnQwNeRsaKeyGen is used here.
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::create_key {

void main();

}  // namespace codebreaker::formats::xbox::crypto::create_key
