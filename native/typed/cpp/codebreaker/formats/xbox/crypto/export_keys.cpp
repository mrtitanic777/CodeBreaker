// codebreaker/formats/xbox/crypto/export_keys.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.export_keys.

#include "codebreaker/formats/xbox/crypto/export_keys.hpp"

#include <optional>
#include <random>

#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "codebreaker/formats/xbox/crypto/bin2lang.hpp"

namespace codebreaker::formats::xbox::crypto::export_keys {

namespace {

// os.urandom(n)
py::bytes urandom(int64_t n) {
    std::random_device rd;
    py::bytearray out = py::bytearray::zeros(n);
    for (int64_t i = 0; i < n; ++i) {
        out[i] = static_cast<int64_t>(rd() & 0xFF);
    }
    return py::bytes(out);
}

}  // namespace

void main() {
    bin2lang::lang_format(py::str("Keys/custom_prv.bin"), std::nullopt, bin2lang::Language::CPLUSPLUS, py::str("TestPrvKey"));
    bin2lang::lang_format(py::str("Keys/custom_pub.bin"), std::nullopt, bin2lang::Language::CPLUSPLUS, py::str("TestPubKey"));

    py::bytes prv_key = MemCrypto::read_file(py::str("Keys/custom_prv.bin"));
    py::bytes d = urandom(0x100);
    py::bytes h = XeCrypt::XeCryptRotSumSha(d);
    py::bytes s = XeCrypt::XeCryptBnQwBeSigCreate(h, py::bytes("SALTTEST"), prv_key);
    s = XeCrypt::XeCryptBnQwNeRsaPrvCrypt(s, prv_key);

    bin2lang::lang_format(d, std::nullopt, bin2lang::Language::CPLUSPLUS, py::str("TestData"));
    bin2lang::lang_format(s, std::nullopt, bin2lang::Language::CPLUSPLUS, py::str("TestSignature"));
}

}  // namespace codebreaker::formats::xbox::crypto::export_keys
