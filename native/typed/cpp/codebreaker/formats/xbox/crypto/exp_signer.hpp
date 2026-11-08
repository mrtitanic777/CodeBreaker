// codebreaker/formats/xbox/crypto/exp_signer.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.exp_signer.
//
// A script to sign HvxExpansionInstall payloads.
//
// Typing notes:
//   * ExpansionMagic is an enum class over int64_t (the IntEnum); sign_exp takes the magic as that enum and
//     the Python's `int(exp_magic)` is its value. A value that is no member (static_cast) gives the Python's
//     Exception("Invalid expansion magic") after the header is built (struct.error before that when it does
//     not fit a DWORD).
//   * `out_file: str = None` is std::optional<py::str> (an empty string is falsy, as in the Python: the
//     input file is then written).
//   * The expansion IV is drawn through os_urandom::urandom (the operating system's random source unless a
//     harness installs another with os_urandom::set_urandom).
//   * XECRYPT_1BL_KEY is std::optional (None when the key store has no 1BL_key.bin): with --encrypt the
//     Python then fails in XeCryptAes with TypeError, which it does here.
//   * The CPU key of the HXPC / SIGC branch is the Python's `b""` (a constant), so those always end in the
//     AssertionError the Python raises.
//   * The command line: main(argv) takes sys.argv (argv[0] is the program name argparse puts in its usage);
//     print and argparse write to the streams of set_streams (stdout / stderr when not set); argparse's
//     exits are a thrown py::SystemExit. Returns the exit code (the Python's `exit(main())`). `--expansion-id`
//     is read with int(text, 16): ValueError for a text that is no hex number.
//   * not ported: `__all__`; the `__name__ == "__main__"` block.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::exp_signer {

extern const py::bytes EXP_SALT;   // b"XBOX360EXP"
inline constexpr int64_t EXP_SIZE = 0x1000;

enum class ExpansionMagic : int64_t {
    HXPR = 0x48585052,
    HXPC = 0x48585043,
    SIGM = 0x5349474D,
    SIGC = 0x53494743,
};

// Sign the payload file in_file with the private key in key_file (written to out_file, or over in_file when
// out_file is None/empty).
void sign_exp(const py::str& in_file, const std::optional<py::str>& out_file = std::nullopt,
              const py::str& key_file = py::str("Keys/HVX_prv.bin"), ExpansionMagic exp_magic = ExpansionMagic::HXPR,
              int64_t exp_id = 0x48565050, bool encrypt = true);

// The output streams of print / argparse: stdout and stderr unless set.
void set_streams(std::ostream& out, std::ostream& err);

// argv is sys.argv. Raises SystemExit(0) after -h and SystemExit(2) on a bad command line.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::exp_signer
