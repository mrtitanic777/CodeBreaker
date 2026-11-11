// codebreaker/formats/xbox/crypto/sd_signer.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.sd_signer.
//
// A script to sign retail CD's and devkit SD bootloaders for Xbox 360.
//
// Typing notes:
//   * pathlib.Path arguments are py::str (a Path is only ever made from, and turned back into, a str here;
//     `.name` is the last component).
//   * valid_file takes the parser (argparse's parser.error exits with status 2).
//   * SB_PRV_KEY is the module-level `None`: main() assigns its own local of that name (it has no `global`),
//     so the module's stays None; here it is an empty std::optional.
//   * The command line: main(argv) takes sys.argv (argv[0] is the program name argparse puts in its usage);
//     the output of print and of argparse goes to the streams of set_streams (stdout / stderr when not set).
//     argparse's exits are a thrown py::SystemExit (0 after -h, 2 on a bad command line).
//   * not ported: the `__name__ == "__main__"` block (`main()`).
#pragma once

#include <iosfwd>
#include <optional>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "codebreaker/formats/xbox/crypto/argparse_min.hpp"

namespace codebreaker::formats::xbox::crypto::sd_signer {

extern const py::str __description__;

// SB_PRV_KEY: XeCryptRsaKey = None
extern std::optional<XeCrypt::XeCryptRsaKey> SB_PRV_KEY;

// The file as a Path, or parser.error(...) when it is not a file.
py::str valid_file(const argparse_min::ArgumentParser& parser, const py::str& filename);

// The output streams of print / argparse: stdout and stderr unless set.
void set_streams(std::ostream& out, std::ostream& err);

// argv is sys.argv. Raises SystemExit(0) after -h and SystemExit(2) on a bad command line.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::sd_signer
