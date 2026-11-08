// codebreaker/formats/xbox/crypto/se_patcher.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.se_patcher.

#include "codebreaker/formats/xbox/crypto/se_patcher.hpp"

#include <cstdio>
#include <iostream>

#include "codebreaker/formats/xbox/crypto/build_lib.hpp"
#include "codebreaker/formats/xbox/crypto/keystore.hpp"

namespace codebreaker::formats::xbox::crypto::se_patcher {

using namespace XeCrypt;
using namespace build_lib;

std::optional<XeCryptRsaKey> SB_PRV_KEY;

namespace {

std::ostream* out_stream_ = &std::cout;

// Path(filename).read_bytes()
py::bytes read_bytes(const py::str& path) {
    py::file f = py::open(path, py::str("rb"));
    py::bytes data = f.read();
    f.close();
    return data;
}

// Path(filename).write_bytes(data)
void write_bytes(const py::str& path, const py::bytearray& data) {
    py::file f = py::open(path, py::str("wb"));
    f.write(data);
    f.close();
}

std::string crc_hex(const py::bytes& data) {
    char crc[16];
    std::snprintf(crc, sizeof crc, "%08x", static_cast<unsigned>(py::crc32(data)));
    return crc;
}

// the shared body: sd_data is a bytearray (a copy of the caller's bytes, or the caller's bytearray itself)
std::tuple<SignedBootloader, SignedBootloader> rehash_sign_(py::bytearray sd_data, const py::bytes& se_data_in,
                                                            const std::optional<py::bytes>& se_patch_data) {
    py::bytes se_in = se_data_in;

    // apply patches to SE
    if (se_patch_data) se_in = py::bytes(apply_patches(se_in, std::variant<py::str, py::bytes, py::bytearray>(*se_patch_data)).view());

    // compress SE
    py::bytearray se_data(compress_se(se_in));
    se_data = py::bytearray::zeros(0x20) + se_data.view();
    // magic, build, QFE, flags, and entry point
    py::pack_into(">2s 3H I", se_data, 0, py::bytes("SE"), static_cast<int64_t>(17559), static_cast<int64_t>(0x8000), static_cast<int64_t>(0),
                  static_cast<int64_t>(0));
    // get length of SE without padding
    const int64_t se_len_nopad = py::len(se_data);
    // set SE size
    py::pack_into(">I", se_data, 0xC, se_len_nopad);
    // append padding
    se_data += py::bytes::zeros(calc_pad_size(se_len_nopad));
    // compute SE hash
    py::bytes se_hash = XeCryptRotSumSha(se_data.slice(0, 0x10).view() + se_data.slice_from(0x20).view());
    // patch SE hash into SD
    py::pack_into("20s", sd_data, 0x24C, se_hash);

    // get length of SD without padding
    int64_t sd_len_nopad = py::len(sd_data);

    // (the xell patching of the SD is commented out in the Python)

    // set SD size
    py::pack_into(">I", sd_data, 0xC, sd_len_nopad);
    // apply padding AFTER
    sd_data += py::bytes::zeros(calc_pad_size(sd_len_nopad));
    // resign SD
    if (!SB_PRV_KEY) throw py::AttributeError("'NoneType' object has no attribute 'sig_create'");
    sd_data = sign_sd_4bl(*SB_PRV_KEY, XECRYPT_SD_SALT, sd_data);

    return {SignedBootloader{sd_len_nopad, sd_data}, SignedBootloader{se_len_nopad, se_data}};
}

}  // namespace

std::tuple<SignedBootloader, SignedBootloader> patch_rehash_sign_se(const py::bytes& sd_data, const py::bytes& se_data,
                                                                    const std::optional<py::bytes>& se_patch_data) {
    return rehash_sign_(py::bytearray(sd_data), se_data, se_patch_data);
}

std::tuple<SignedBootloader, SignedBootloader> patch_rehash_sign_se(const py::bytearray& sd_data, const py::bytes& se_data,
                                                                    const std::optional<py::bytes>& se_patch_data) {
    return rehash_sign_(sd_data, se_data, se_patch_data);
}

void set_stream(std::ostream& out) { out_stream_ = &out; }

int64_t main() {
    SB_PRV_KEY = keystore::load_and_verify_sb_prv();

    const py::str con_rev("Jasper");
    const py::str con_rev_lower("jasper");

    const py::str base_dir = py::str("Output/Extracted/") + con_rev + py::str("/");

    py::bytes sd_data = read_bytes(base_dir + py::str("SD.bin"));
    py::bytes se_data = read_bytes(base_dir + py::str("hypervisor.bin")) + read_bytes(base_dir + py::str("kernel.exe"));
    py::bytes se_patch_data = read_bytes(py::str("Output/Zero/VRGL.bin"));

    auto [sd, se] = patch_rehash_sign_se(sd_data, se_data, se_patch_data);
    auto [sd_len_nopad, sd_out] = sd;
    auto [se_len_nopad, se_out] = se;

    // zero nonces
    py::pack_into("16x", sd_out, 0x10);
    py::pack_into("16x", se_out, 0x10);

    // write the SD and SE
    const py::str dir = py::str("xeBuild_1.21/Builds/17559 - RGL/bootloaders/rgl/") + con_rev_lower + py::str("bl/");
    write_bytes(dir + py::str("sd_17489.bin"), sd_out);
    write_bytes(dir + py::str("se_17559.bin"), se_out);
    // output XeBuild checksums
    *out_stream_ << "[" << con_rev_lower.utf8() << "bl]\n";
    *out_stream_ << "bootloaders/devkit/jasperbl_rgl/" << con_rev_lower.utf8() << "bl/sd_17489.bin," << crc_hex(sd_out.slice(0, sd_len_nopad).view())
                 << "\n";
    *out_stream_ << "bootloaders/devkit/jasperbl_rgl/" << con_rev_lower.utf8() << "bl/se_17559.bin," << crc_hex(se_out.slice(0, se_len_nopad).view())
                 << "\n";

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::se_patcher
