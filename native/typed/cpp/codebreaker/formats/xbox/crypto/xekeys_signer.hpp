// codebreaker/formats/xbox/crypto/xekeys_signer.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.xekeys_signer.
//
// A script to sign XeKeysExecute payloads.
//
// Typing notes:
//   * PayloadMagic is an enum class over int64_t (the IntEnum); the Python takes any int there, and a
//     value that is no member (static_cast) is signed the same way, struct.error when it does not fit a WORD.
//   * `out_file: str = None` is std::optional<py::str> (an empty string is falsy, as in the Python: the
//     input file is then written).
//   * The hvx key is drawn through os_urandom::urandom (the operating system's random source unless a
//     harness installs another with os_urandom::set_urandom).
//   * XECRYPT_1BL_KEY is std::optional (None when the key store has no 1BL_key.bin): the Python then fails
//     in XeCryptHmacSha with TypeError, which it does here, after the signature is made.
//   * The command line: main(argv) takes sys.argv (argv[0] is the program name argparse puts in its usage);
//     print and argparse write to the streams of set_streams (stdout / stderr when not set); argparse's
//     exits are a thrown py::SystemExit. Returns the exit code (the Python's `exit(main())`).
//   * not ported: `__all__`; the `__name__ == "__main__"` block.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::xekeys_signer {

extern const py::bytes PAYLOAD_SALT;   // b"XBOX_EX_01"

enum class PayloadMagic : int64_t {
    DEVKIT = 0x5D4D,
    RETAIL = 0x4D4D,
};

// Sign the payload file in_file (written to out_file, or over in_file when out_file is None/empty).
void sign_xekeys(const py::str& in_file, const std::optional<py::str>& out_file = std::nullopt,
                 PayloadMagic pl_magic = PayloadMagic::DEVKIT);

// The output streams of print / argparse: stdout and stderr unless set.
void set_streams(std::ostream& out, std::ostream& err);

// argv is sys.argv. Raises SystemExit(0) after -h and SystemExit(2) on a bad command line.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::xekeys_signer
