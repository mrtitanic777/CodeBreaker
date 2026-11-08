// codebreaker/formats/xbox/crypto/xekeys_signer.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.xekeys_signer.

#include "codebreaker/formats/xbox/crypto/xekeys_signer.hpp"

#include <iostream>

#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "codebreaker/formats/xbox/crypto/argparse_min.hpp"
#include "codebreaker/formats/xbox/crypto/build_lib.hpp"
#include "codebreaker/formats/xbox/crypto/keystore.hpp"
#include "codebreaker/formats/xbox/crypto/os_urandom.hpp"

namespace codebreaker::formats::xbox::crypto::xekeys_signer {

using namespace XeCrypt;

const py::bytes PAYLOAD_SALT("XBOX_EX_01");

namespace {
std::ostream* out_stream_ = &std::cout;
std::ostream* err_stream_ = &std::cerr;
}  // namespace

void set_streams(std::ostream& out, std::ostream& err) {
    out_stream_ = &out;
    err_stream_ = &err;
}

void sign_xekeys(const py::str& in_file, const std::optional<py::str>& out_file, PayloadMagic pl_magic) {
    XeCryptRsaKey hvx_prv = keystore::load_and_verify_hvx_prv();
    py::bytes file_data = std::get<py::bytes>(build_lib::read_file(in_file));
    py::bytes hvx_key = os_urandom::urandom(0x10);

    // build header
    py::bytes hvx_hdr = py::pack(">2H 3I 16s 256x", static_cast<int64_t>(pl_magic), static_cast<int64_t>(0xDEAD), static_cast<int64_t>(0),
                                 static_cast<int64_t>(0x120), 0x120 + py::len(file_data), hvx_key);

    // prepend header to payload
    py::bytearray payload(hvx_hdr + file_data);

    py::bytes b_hash = XeCryptRotSumSha(payload.slice(0, 0x10).view() + payload.slice_from(0x120).view()).slice(0, 0x14);
    py::bytes sig = hvx_prv.sig_create(b_hash, PAYLOAD_SALT);
    payload.set_slice(0x20, 0x20 + py::len(sig), sig);
    if (!XECRYPT_1BL_KEY()) throw py::TypeError("key must be bytes-like");
    py::bytes rc4_key = XeCryptHmacSha(*XECRYPT_1BL_KEY(), py::list<py::bytes>{hvx_key}).slice(0, 0x10);
    py::bytes enc_payload = XeCryptRc4::new_(rc4_key).encrypt(payload.slice_from(0x20).view());
    payload.set_slice(0x20, 0x20 + py::len(enc_payload), enc_payload);

    // write the signed payload to disk
    build_lib::write_file(out_file && !out_file->empty() ? *out_file : in_file, payload);
}

int64_t main(const py::list<py::str>& argv) {
    argparse_min::ArgumentParser parser(py::os::path::basename(argv[0]), py::str("A script to sign XeKeysExecute payloads"), *out_stream_,
                                        *err_stream_);
    parser.add_positional("input", py::str("The payload executable to sign"));
    parser.add_positional("output", py::str("The signed payload file"));
    argparse_min::Namespace args = parser.parse_args(argv.slice(1, py::len(argv)));

    if (!py::os::path::isfile(*args.get("input"))) throw py::AssertionError("The specified input file doesn't exist");

    sign_xekeys(*args.get("input"), args.get("output"));

    *out_stream_ << "Done!\n";

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::xekeys_signer
