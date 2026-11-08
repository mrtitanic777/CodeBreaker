// codebreaker/formats/xbox/crypto/xell_unpack.hpp -- C++ from codebreaker.formats.xbox.crypto.xell_unpack.
#pragma once

#include <variant>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::xell_unpack {

// gzip.BadGzipFile (an OSError) and zlib.error, which gzip.decompress raises
struct BadGzipFile : py::OSError {
    using py::OSError::OSError;
    const char* type() const override { return "BadGzipFile"; }
};
struct ZlibError : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "error"; }
};

// constants
extern const py::str XELL_DIR;
inline constexpr int64_t GZIP_HEADER_SIZE = 10;
inline constexpr int64_t GZIP_FOOTER_SIZE = 8;
inline constexpr int64_t XELL_SIZE = 256 * 1024;
inline constexpr int64_t XELL_FOOTER_SIZE = 16;
inline constexpr int64_t XELL_STAGE1_SIZE = 16 * 1024;
inline constexpr int64_t XELL_STAGE2_SIZE = (XELL_SIZE - XELL_STAGE1_SIZE - XELL_FOOTER_SIZE - GZIP_HEADER_SIZE - GZIP_FOOTER_SIZE);

// `from gzip import decompress`: gzip.decompress(data), all members of a gzip stream in one shot.
// Raises BadGzipFile, EOFError or ZlibError as CPython's does.
py::bytes decompress(const py::bytes& data);

// utilities
// The file's content: bytes, or (text) a str.
std::variant<py::bytes, py::str> read_file(const py::str& filename, bool text = false);

// A str is written as text, bytes and bytearray as they are.
void write_file(const py::str& filename, const std::variant<py::str, py::bytes, py::bytearray>& data);

void main();

}  // namespace codebreaker::formats::xbox::crypto::xell_unpack
