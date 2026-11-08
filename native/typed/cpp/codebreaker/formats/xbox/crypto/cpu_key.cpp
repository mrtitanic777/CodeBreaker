// codebreaker/formats/xbox/crypto/cpu_key.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.cpu_key.

#include "codebreaker/formats/xbox/crypto/cpu_key.hpp"

#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"

namespace codebreaker::formats::xbox::crypto::cpu_key {

void main() {
    py::bytes key = XeCrypt::XeCryptCpuKeyGen();
    // "".join([f"{x:0>8b}" for x in key])
    py::str key_bits("");
    for (int64_t x : key) {
        key_bits = key_bits + py::format(x, "0>8b");
    }

    py::print(py::str("ECC Bits: ") + key_bits.slice_from(-26));
    py::print(py::str("UID Bits: ") + key_bits.slice_to(-26));

    py::print(key.hex().upper());
}

}  // namespace codebreaker::formats::xbox::crypto::cpu_key
