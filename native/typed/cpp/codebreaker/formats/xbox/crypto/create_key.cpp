// codebreaker/formats/xbox/crypto/create_key.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.create_key.

#include "codebreaker/formats/xbox/crypto/create_key.hpp"

#include <tuple>

#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

namespace codebreaker::formats::xbox::crypto::create_key {

void main() {
    auto [pub_key, prv_key] = XeCrypt::XeCryptBnQwNeRsaKeyGen(2048);
    // Path("Keys/custom_pub.bin").write_bytes(pub_key)
    MemCrypto::write_file(py::str("Keys/custom_pub.bin"), pub_key);
    MemCrypto::write_file(py::str("Keys/custom_prv.bin"), prv_key);
    py::print(py::str("Done!"));
}

}  // namespace codebreaker::formats::xbox::crypto::create_key
