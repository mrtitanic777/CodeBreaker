// codebreaker/formats/xbox/crypto/cpu_key.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.cpu_key.
//
// Makes a random Xbox 360 CPU key and shows it: its ECC bits and its UID bits as binary, then the key in hex.
//
// Typing notes:
//   * main() prints with py::print (stdout), as the Python's print does.
//   * The key comes from XeCrypt::XeCryptCpuKeyGen, which draws from Python's `random` generator
//     (XeCrypt::random_seed(a) seeds it like random.seed(a)).
#pragma once

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::cpu_key {

void main();

}  // namespace codebreaker::formats::xbox::crypto::cpu_key
