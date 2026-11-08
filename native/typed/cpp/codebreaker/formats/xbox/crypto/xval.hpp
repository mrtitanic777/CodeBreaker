// codebreaker/formats/xbox/crypto/xval.hpp -- C++ from codebreaker.formats.xbox.crypto.xval.
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::xval {

// XVal.py By Redline99, updated xvals by cz
// Decrypts the "X:" Value from the Xbox 360 dashboard
// This can indicate if the console has been flagged
// for some types of security violations

// Modified by Visual Studio to be more modern :D

extern const int64_t FLAG_SSB_NONE;
extern const int64_t FLAG_SSB_AUTH_EX_FAILURE_D;             // DEPRECATED
extern const int64_t FLAG_SSB_AUTH_EX_NO_TABLE_D;            // DEPRECATED
extern const int64_t FLAG_SSB_AUTH_EX_RESERVED;
extern const int64_t FLAG_SSB_INVALID_DVD_GEOMETRY;
extern const int64_t FLAG_SSB_INVALID_DVD_DMI;
extern const int64_t FLAG_SSB_DVD_KEYVAULT_PAIR_MISMATCH_D;  // DEPRECATED
extern const int64_t FLAG_SSB_CRL_DATA_INVALID_D;            // DEPRECATED
extern const int64_t FLAG_SSB_CRL_CERTIFICATE_REVOKED;
extern const int64_t FLAG_SSB_UNAUTHORIZED_INSTALL;
extern const int64_t FLAG_SSB_KEYVAULT_POLICY_VIOLATION;
extern const int64_t FLAG_SSB_CONSOLE_BANNED_D;              // DEPRECATED
extern const int64_t FLAG_SSB_ODD_VIOLATION;
extern const int64_t FLAG_SSB_CIV_HASH_FAILURE;
extern const int64_t FLAG_SSB_AUTH_EX_NO_TABLE;
extern const int64_t FLAG_SSB_CRL_DATA_INVALID;
extern const int64_t FLAG_SSB_ODD_ENFORCE_THROUGHPUT_LOW;
extern const int64_t FLAG_SSB_ODD_ENFORCE_THROUGHPUT_HIGH;

// Python's SystemExit (what argparse raises for --help and for a bad command line): `code` is the
// exit status.
struct SystemExit : py::Exception {
    int64_t code = 0;
    explicit SystemExit(int64_t c) : py::Exception(), code(c) {}
    const char* type() const override { return "SystemExit"; }
};

// The 8 bytes the console's X: value decrypts to, under the DES key HMAC-SHA1("XBOX360SSB",
// serial + NUL)[:8]. The serial is 12 ASCII characters; the value is 16 hex digits, "-" allowed
// anywhere. AssertionError for a wrong length, ValueError for non-hex text, UnicodeEncodeError for
// a non-ASCII serial.
py::bytes decrypt_xval(const py::str& console_serial, const py::str& console_xval);

// Prints what a decrypted X: value says: two big-endian 32-bit words, flag bits in the low one.
// struct.error (py::StructError) when `xval` is not 8 bytes.
void display_results(const py::bytes& xval);

// The command line of the script: `xval.py serial xval`. `argv` is sys.argv (the program name
// first). Written to the arguments argparse would take for this parser (positional serial and
// xval, -h / --help and its unique abbreviations, "--", negative numbers and arguments with a space
// as positionals, unknown options and surplus arguments reported); help and errors raise
// SystemExit with the text argparse writes to stdout / stderr (80-column terminal).
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::xval
