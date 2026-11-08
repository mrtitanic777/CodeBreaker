// codebreaker/formats/xbox/crypto/bin2lang.hpp -- C++ port of codebreaker.formats.xbox.crypto.bin2lang.
//
// bin2lang.py -- a script to make embedding binaries in code a breeze: a binary's bytes as the source of a
// byte array in Python, C, C++, C# or PHP.
//
// Typing notes (the Python mixes types in a few places):
//   * lang_format's din is a str (the path of a file), bytes or bytearray: one overload each.
//   * lang_format's dout is None (the text is printed) or a str (the path the text is written to); the
//     Python also tests `type(dout) == TextIO`, which no real stream passes (its type is TextIOWrapper), so a
//     stream there is printed too -- a stream is therefore not an argument here.
//   * What the Python prints and what argparse prints go to the streams of set_streams (stdout and stderr
//     when not set); a script exit (sys.exit) is a SystemExit.
//   * The command line is parsed as argparse (Python 3.14) does for this parser, at its default width
//     of 80 columns (COLUMNS is not read); a line length that does not fit in 64 bits raises
//     OverflowError when the bytes are read, as Python does. Reading a file in chunks of 2**36 bytes
//     or more raises MemoryError (Python: when the machine is out of memory, which varies).
#pragma once

#include <iosfwd>
#include <optional>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::bin2lang {

// sys.exit's SystemExit.
struct SystemExit : py::Exception {
    int64_t code;
    explicit SystemExit(int64_t c) : py::Exception(""), code(c) {}
    const char* type() const override { return "SystemExit"; }
};

// lowercase = lambda s: s.lower()
py::str lowercase(const py::str& s);

// class Language(IntEnum)
enum class Language : int64_t {
    PYTHON = 0,
    C = 1,
    CPLUSPLUS = 2,
    CSHARP = 3,
    PHP = 4,
    PHP_NEW = 5,
    PHP_OLD = 6,
};

// The bytes of din (a file by its path, or bytes in memory) as the source of an array of bytes, byte_count
// bytes to a line; written to the file dout, or printed when dout is None. -> the text.
// (A language that is no Language gives the empty text; no bytes, or byte_count 0, an IndexError.)
py::str lang_format(const py::str& din, const std::optional<py::str>& dout = std::nullopt,
                    Language language = Language::CPLUSPLUS, const py::str& var_name = py::str("data"),
                    int64_t byte_count = 16);
py::str lang_format(const py::bytes& din, const std::optional<py::str>& dout = std::nullopt,
                    Language language = Language::CPLUSPLUS, const py::str& var_name = py::str("data"),
                    int64_t byte_count = 16);
py::str lang_format(const py::bytearray& din, const std::optional<py::str>& dout = std::nullopt,
                    Language language = Language::CPLUSPLUS, const py::str& var_name = py::str("data"),
                    int64_t byte_count = 16);

// The script's output streams (print / argparse): stdout and stderr unless set.
void set_streams(std::ostream& out, std::ostream& err);

// The script's entry; argv is sys.argv (argv[0] is the program name argparse uses in its usage line).
// Raises SystemExit(0) after -h and SystemExit(2) on a bad command line, as argparse does.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::bin2lang
