// codebreaker/formats/xbox/crypto/keystore.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.keystore.

#include "codebreaker/formats/xbox/crypto/keystore.hpp"

#include "codebreaker/formats/xbox/crypto/MemCrypto.hpp"

namespace codebreaker::formats::xbox::crypto::keystore {

const py::str& KEY_PATH() {
    static const py::str path = [] {
        py::str folder = py::settings::get_str(py::str("xbox360_keys"));
        if (folder.u32().empty()) folder = py::str("Keys");   // (settings.get(...) or "Keys")
        return folder;
    }();
    return path;
}

namespace {

// data = (KEY_PATH / name).read_bytes(); assert crc32(data) == checksum, "Invalid key checksum!"
py::bytes read_and_check(const char* name, int64_t checksum) {
    py::bytes data = MemCrypto::read_file(py::os::path::join(KEY_PATH(), py::str(name)));
    if (!(py::crc32(data) == checksum)) throw py::AssertionError("Invalid key checksum!");
    return data;
}

}  // namespace

XeCryptRsaKey load_and_verify_1bl_pub() {
    py::bytes data = read_and_check("1BL_pub.bin", CKSM_1BL_PUB);
    return XeCryptRsaKey(data);
}

XeCryptRsaKey load_and_verify_xmacs_pub() {
    py::bytes data = read_and_check("XMACS_pub.bin", CKSM_XMACS_PUB);
    return XeCryptRsaKey(data);
}

XeCryptRsaKey load_and_verify_sb_prv() {
    py::bytes data = read_and_check("SB_prv.bin", CKSM_SB_PRV);
    return XeCryptRsaKey(data);
}

XeCryptRsaKey load_and_verify_hvx_prv() {
    py::bytes data = read_and_check("HVX_prv.bin", CKSM_HVX_PRV);
    return XeCryptRsaKey(data);
}

XeCryptRsaKey load_and_verify_master_pub() {
    py::bytes data = read_and_check("Master_pub.bin", CKSM_MASTER_PUB);
    return XeCryptRsaKey(data);
}

}  // namespace codebreaker::formats::xbox::crypto::keystore
