// codebreaker/formats/xex.hpp -- typed C++ of codebreaker.formats.xex
//
// xex.py -- an Xbox 360 executable (.xex): the PowerPC image inside it, and what its header says.
//
//     xex::is_xex(path)               the file starts with XEX2
//     xex::optional_headers(data)     (entry's offset, key, value) of each entry of the header's table:
//                                     the one reader of it (xex_edit, xex_imports, god, fatx's carver)
//     xex::basefile(path)             the decrypted, decompressed PowerPC image (a PE, machine 0x1F2),
//                                     written once under state/unpacked/xex/ and reused, with the
//                                     header beside it (<image>.json)
//     xex::import_libraries(image)    the libraries that header named, for an image written so
//     h = xex::header(path)
//     h.name                          the image's own name (bootanim.dll, default.exe)
//     h.base, h.entry                 its load address and entry point
//     h.imports                       [library name] in the order the image's import stubs number them
//     h.libraries                     [(static library, version)]: what it was built with
//     run(args)                       `codebreaker xex <command> ...`: the native program's commands
//
// A .xex wraps a PE image in its own header: the image is compressed (LZX or by blocks), usually
// encrypted with a per-title key, and the libraries its imports come from are named in the header,
// not in the image. The native program (native/xex, built with CMake) does the decrypting and
// decompressing; the PowerPC lane reads the basefile it writes, so a .xex is reconstructed like any
// PowerPC image (corpus.resolve hands every stage the basefile in its place).
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * The header reader is the C++ of native/formats (xex/questions.cc over native/xex's xex_file.cpp), linked
//     in: no ctypes, no pickle.  An exception it raised comes back as the py:: exception of the same Python
//     type (struct.error is py::StructError).
//   * optional_headers is a generator in the Python: here a py::iter over entries; the failure the reader met
//     is thrown by the step that reaches it, after the entries before it were given.
//   * The Header class is the struct `Header` (name / base / entry are None when the header has none).
//   * import_libraries returns json.load(f)["imports"], which can be any JSON value: the
//     codebreaker::symbols::native::Json variant (the typed json.loads result); None is nullopt.
//   * main(argv): Python's main takes argv; the exit status is the returned value.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/symbols/native.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xex {

extern const py::bytes MAGIC;
extern const int64_t FILE_FORMAT_INFO;

// Whether the file starts with XEX2.
bool is_xex(const py::str& path);

// (the entry's offset, key, value) of each entry of the header's optional-header table, in order (read
// as asked: a reader that stops at the key it wants reads no further). A value is the data itself for a
// key whose low byte is 0 or 1, else the offset of the data in the header.
using HeaderEntry = std::tuple<int64_t, int64_t, int64_t>;
py::iter<HeaderEntry> optional_headers(const py::bytes& data);

// (media id, title id) from the header's execution info, or (0, 0) for a header without one (or what is
// not an XEX2 header)
std::tuple<int64_t, int64_t> ids(const py::bytes& data);

py::str program();
py::str _sha(const py::str& path);

// The PowerPC image inside `path`, extracted once per distinct file (named by its contents,
// so a changed .xex is a new basefile and nothing is stale).
py::str basefile(const py::str& path);

// Write `path` as an unencrypted XEX with basic compression: every header byte kept (optional
// headers, security info, imports, exports, resources), the file format info replaced by one
// block holding the whole image. XenonRecomp and the Xbox 360 runtime read only this form. The
// security info's hashes no longer match; nothing that reads it checks them.
py::str basic(const py::str& path, const py::str& out);

// [library name] for a basefile written by basefile() -- the .xex header named them -- or
// None for an image that did not come from a .xex.
std::optional<symbols::native::Json> import_libraries(const py::str& image_path);

struct Header {
    std::optional<py::str> name;
    std::optional<int64_t> base, entry;
    py::list<py::str> imports;
    py::list<std::tuple<py::str, py::str>> libraries;
};

// What the .xex header states, read from the native program's listing.
Header header(const py::str& path);

// `codebreaker xex <command> <file.xex> [args]`: info, list, extract, resources, xml, idc,
// compress, decompress, binary, encrypt, decrypt, machine, remove-limits, bounding-path, patch,
// selftest (`codebreaker xex` alone lists them).
int64_t run(const py::list<py::str>& argv);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xex
