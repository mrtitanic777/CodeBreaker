// codebreaker/formats/xbox/crypto/sd_signer.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.sd_signer.

#include "codebreaker/formats/xbox/crypto/sd_signer.hpp"

#include <cstdio>
#include <iostream>

#include "codebreaker/formats/xbox/crypto/build_lib.hpp"
#include "codebreaker/formats/xbox/crypto/keystore.hpp"

namespace codebreaker::formats::xbox::crypto::sd_signer {

using namespace XeCrypt;
using argparse_min::ArgumentParser;

const py::str __description__("A script to sign retail CD's and devkit SD bootloaders for Xbox 360");

std::optional<XeCryptRsaKey> SB_PRV_KEY;

namespace {
std::ostream* out_stream_ = &std::cout;
std::ostream* err_stream_ = &std::cerr;

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
}  // namespace

void set_streams(std::ostream& out, std::ostream& err) {
    out_stream_ = &out;
    err_stream_ = &err;
}

py::str valid_file(const ArgumentParser& parser, const py::str& filename) {
    if (!py::os::path::isfile(filename)) {
        parser.error(py::str("The file \"") + filename + py::str("\" doesn't exist!"));
    } else {
        return filename;
    }
}

void main(const py::list<py::str>& argv) {
    XeCryptRsaKey sb_prv_key = keystore::load_and_verify_sb_prv();

    ArgumentParser parser(py::os::path::basename(argv[0]), __description__, *out_stream_, *err_stream_);
    const auto check = [&parser](const py::str& x) { return valid_file(parser, x); };
    parser.add_positional("input", py::str("The CD/SD to sign"), check);
    parser.add_option({py::str("-o")}, py::str("The file to output to"), std::nullopt, check);
    argparse_min::Namespace args = parser.parse_args(argv.slice(1, py::len(argv)));
    const py::str input = *args.get("input");
    const std::optional<py::str> o = args.get("o");

    // make sure the CD/SD exists
    if (!py::os::path::isfile(input)) throw py::AssertionError("The input file specified doesn't exist");
    // read the CD/SD
    py::bytearray bl_data(read_bytes(input));
    // make sure it's a valid image
    if (!(bl_data.slice(0, 2) == py::bytes("CD") || bl_data.slice(0, 2) == py::bytes("SD"))) {
        throw py::AssertionError("The input file specified isn't a valid CD or SD bootloader image");
    }
    // append padding
    bl_data += py::bytes::zeros(((py::len(bl_data) + 0xF) & ~static_cast<int64_t>(0xF)) - py::len(bl_data));
    // set SD size
    py::pack_into(">I", bl_data, 0xC, py::len(bl_data));
    // resign SD
    bl_data = build_lib::sign_sd_4bl(sb_prv_key, XECRYPT_SD_SALT, bl_data);
    // zero nonces
    py::pack_into("16s", bl_data, 0x10, py::bytes::zeros(0x10));
    // write the CD/SD
    const py::str out_path = o ? *o : input;
    write_bytes(out_path, bl_data);
    // output XeBuild checksums
    char crc[16];
    std::snprintf(crc, sizeof crc, "%08x", static_cast<unsigned>(py::crc32(py::bytes(bl_data.view()))));
    *out_stream_ << py::os::path::basename(out_path).utf8() << "," << crc << "\n";
}

}  // namespace codebreaker::formats::xbox::crypto::sd_signer
