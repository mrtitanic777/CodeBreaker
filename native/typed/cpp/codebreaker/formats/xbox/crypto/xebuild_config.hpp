// codebreaker/formats/xbox/crypto/xebuild_config.hpp -- C++ port of codebreaker.formats.xbox.crypto.xebuild_config.
#pragma once

#include <iosfwd>
#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::xebuild_config {

// sys.exit's SystemExit (what argparse raises on -h and on a bad command line).
struct SystemExit : py::Exception {
    int64_t code;
    explicit SystemExit(int64_t c) : py::Exception(""), code(c) {}
    const char* type() const override { return "SystemExit"; }
};

constexpr int64_t BUFF_SIZE = 2048;

// binascii.crc32 over the data, BUFF_SIZE bytes at a time, each block continuing the running value.
// (data: bytes or bytearray)
int64_t checksum(const py::bytes& data);
int64_t checksum(const py::bytearray& data);

// Where argparse writes its usage / help (stdout) and its errors (stderr); defaults to std::cout / std::cerr.
void set_streams(std::ostream& out, std::ostream& err);

// main(): the script's entry; argv is sys.argv (argv[0] is the program name argparse uses in its usage line).
// Parses -h/--help and -d/--dir DIR, then lists the current folder (files are skipped; nothing is
// generated yet). Raises SystemExit(0) after -h and SystemExit(2) on a bad command line, as argparse does.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::crypto::xebuild_config
