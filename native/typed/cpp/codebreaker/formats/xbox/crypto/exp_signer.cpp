// codebreaker/formats/xbox/crypto/exp_signer.cpp -- typed C++ port of codebreaker.formats.xbox.crypto.exp_signer.

#include "codebreaker/formats/xbox/crypto/exp_signer.hpp"

#include <iostream>

#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "codebreaker/formats/xbox/crypto/argparse_min.hpp"
#include "codebreaker/formats/xbox/crypto/build_lib.hpp"
#include "codebreaker/formats/xbox/crypto/os_urandom.hpp"

namespace codebreaker::formats::xbox::crypto::exp_signer {

using namespace XeCrypt;

const py::bytes EXP_SALT("XBOX360EXP");

namespace {
std::ostream* out_stream_ = &std::cout;
std::ostream* err_stream_ = &std::cerr;

constexpr int64_t kHXPR = static_cast<int64_t>(ExpansionMagic::HXPR);
constexpr int64_t kHXPC = static_cast<int64_t>(ExpansionMagic::HXPC);
constexpr int64_t kSIGM = static_cast<int64_t>(ExpansionMagic::SIGM);
constexpr int64_t kSIGC = static_cast<int64_t>(ExpansionMagic::SIGC);
}  // namespace

void set_streams(std::ostream& out, std::ostream& err) {
    out_stream_ = &out;
    err_stream_ = &err;
}

void sign_exp(const py::str& in_file, const std::optional<py::str>& out_file, const py::str& key_file, ExpansionMagic exp_magic, int64_t exp_id,
              bool encrypt) {
    const py::bytes cpu_key("");

    // prv_key = read_file(key_file)
    XeCryptRsaKey prv_key(std::get<py::bytes>(build_lib::read_file(key_file)));
    py::bytes payload = std::get<py::bytes>(build_lib::read_file(in_file));
    const int64_t exp_typ = static_cast<int64_t>(exp_magic);

    // pad payload to the 16 byte boundary
    const int64_t payload_len_nopad = py::len(payload);
    payload = payload + py::bytes::zeros(((payload_len_nopad + 0xF) & ~static_cast<int64_t>(0xF)) - payload_len_nopad);
    const int64_t payload_len_pad = py::len(payload);

    // allocate 0x1000 bytes for the expansion
    py::bytearray exp_final = py::bytearray::zeros(EXP_SIZE);

    // 0x0 -> expansion header
    py::bytes exp_hdr = py::pack(">3I", exp_typ, static_cast<int64_t>(0), 0x170 + payload_len_pad);   // type, flags, padded size
    // 0xC
    exp_hdr = exp_hdr + py::bytes::zeros(0x14);   // SHA hash
    // 0x20
    exp_hdr = exp_hdr + py::bytes::zeros(0x10);   // exp_iv
    // 0x30
    exp_hdr = exp_hdr + py::bytes::zeros(0x100);   // RSA sig of above
    // 0x130 -> expansion info
    exp_hdr = exp_hdr + py::pack(">4I 2Q 4I", exp_id, static_cast<int64_t>(0), static_cast<int64_t>(0), static_cast<int64_t>(0),
                                 static_cast<int64_t>(0), static_cast<int64_t>(0), static_cast<int64_t>(0), static_cast<int64_t>(0),
                                 static_cast<int64_t>(0x160), payload_len_pad + 0x10);
    // 0x160 -> expansion section info
    exp_hdr = exp_hdr + py::pack(">3I 4x", static_cast<int64_t>(0x10), static_cast<int64_t>(0x10), payload_len_pad);
    // 0x170

    // write the header into the expansion
    exp_final.set_slice(0, py::len(exp_hdr), exp_hdr);
    // write the payload into the expansion
    exp_final.set_slice(py::len(exp_hdr), py::len(exp_hdr) + payload_len_pad, payload);

    // write the expansion hash
    py::bytes b_hash = XeCryptSha(py::list<py::bytes>{exp_final.slice(0x130, 0x170 + payload_len_pad).view()});
    exp_final.set_slice(0xC, 0xC + 0x14, b_hash);

    // write the expansion signature
    py::bytes sig;
    if (exp_typ == kHXPR) {
        b_hash = XeCryptRotSumSha(exp_final.slice(0, 0x30).view());
        sig = prv_key.sig_create(b_hash, EXP_SALT);
    } else if (exp_typ == kSIGM) {
        b_hash = XeCryptRotSumSha(exp_final.slice(0, 0x30).view());
        std::optional<py::bytes> pkcs1 = prv_key.sig_create_pkcs1(b_hash);
        if (!pkcs1) throw py::TypeError("object of type 'NoneType' has no len()");   // len(sig) below
        sig = *pkcs1;
    } else if (exp_typ == kHXPC || exp_typ == kSIGC) {
        if (!XeCryptCpuKeyValid(cpu_key)) throw py::AssertionError("A valid CPU is required for HXPC/SIGC");
        b_hash = XeCryptHmacSha(cpu_key, py::list<py::bytes>{exp_final.slice(0, 0x30).view()});
        // sig = XeKeysPkcs1Create(b_hash, prv_key)
        sig = prv_key.sig_create(b_hash, EXP_SALT);
    } else {
        throw py::Exception("Invalid expansion magic");
    }

    // write the expansion signature
    exp_final.set_slice(0x30, 0x30 + py::len(sig), sig);

    // strip padding
    exp_final = exp_final.slice(0, 0x170 + payload_len_pad);

    // write the encrypted expansion
    if (exp_typ == kHXPR) {
        if (encrypt) {   // encrypt everything after the signature
            py::bytes exp_iv = os_urandom::urandom(0x10);
            exp_final.set_slice(0x20, 0x30, exp_iv);
            if (!XECRYPT_1BL_KEY()) throw py::TypeError("object of type 'NoneType' has no len()");
            py::bytes enc_exp = XeCryptAes::new_(*XECRYPT_1BL_KEY(), XeCryptAes::MODE_CBC, exp_iv).encrypt(exp_final.slice_from(0x130).view());
            exp_final.set_slice(0x130, 0x130 + py::len(enc_exp), enc_exp);
        }
    }

    // write it to a file
    build_lib::write_file(out_file && !out_file->empty() ? *out_file : in_file, exp_final);
}

int64_t main(const py::list<py::str>& argv) {
    argparse_min::ArgumentParser parser(py::os::path::basename(argv[0]), py::str("A script to sign HvxExpansionInstall payloads"), *out_stream_,
                                        *err_stream_);
    parser.add_positional("input", py::str("The payload executable to sign"));
    parser.add_option({py::str("-o"), py::str("--ofile")}, py::str("The signed payload file"));
    parser.add_option({py::str("-i"), py::str("--expansion-id")}, py::str("The expansion ID to use"), py::str("0x48565050"));
    parser.add_option({py::str("-k"), py::str("--keyfile")}, py::str("The private key to sign with"), py::str("Keys/HVX_prv.bin"));
    parser.add_flag({py::str("--encrypt")}, py::str("Encrypt the expansion"));
    argparse_min::Namespace args = parser.parse_args(argv.slice(1, py::len(argv)));

    const py::str input = *args.get("input");
    if (!py::os::path::isfile(input)) throw py::AssertionError("The specified input file doesn't exist");
    const int64_t expansion_id = py::int_parse(*args.get("expansion_id"), 16);

    sign_exp(input, args.get("ofile"), *args.get("keyfile"), ExpansionMagic::HXPR, expansion_id, args.flag("encrypt"));

    *out_stream_ << "Done!\n";

    return 0;
}

}  // namespace codebreaker::formats::xbox::crypto::exp_signer
