// codebreaker/formats/xbox/crypto/xcp_dumper.hpp -- C++ port of codebreaker.formats.xbox.crypto.xcp_dumper.
//
// A script to decrypt, merge, and convert XCP files for the Xbox 360, and the XCPFile class that unpacks an
// XCP (a stream of zlib blocks holding an SVOD index and its data files) into a content folder.
//
// Typing notes:
//   * pathlib.Path is a py::str (`Path`): the runtime has no pathlib, so the few operations the module uses
//     (Path(s), parents[0], stem, p / name) are functions of the .cpp working on the Windows spelling of a
//     path (backslashes, no "." parts, no trailing separator).
//   * ctypes structures are plain C structs of the same layout (little endian, natural alignment, so the
//     same sizes as ctypes.sizeof; the harness checks them). StreamIO::read_struct<T>() copies them in.
//   * zlib.decompressobj() is `Decompress` below (the runtime has no zlib): it gives what CPython's object
//     does for the module's calls (decompress, unused_data, eof, flush) and raises xell_unpack::ZlibError
//     (the runtime's zlib.error) with zlib's own message text.
//   * The Python's StreamIO has no `offset` property, perform_function_at, set_label or get_label (they
//     belong to upstream's fuller StreamIO). `xcp.offset = n` there only sets a plain attribute, so here it
//     is a local variable; calling one of the other three is the AttributeError Python raises, and the code
//     after such a call (which the Python cannot reach either) is ported as written.
//   * `args.key` is None, a str (as typed on the command line) or, once converted, bytes: std::variant.
//   * `main()` takes sys.argv[1:] and the program name, and returns the exit code; argparse's exits are a
//     thrown py::SystemExit, as in the other typed ports.
#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <optional>
#include <string>
#include <variant>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/StreamIO.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "codebreaker/formats/xbox/crypto/xell_unpack.hpp"   // ZlibError (zlib.error)

namespace codebreaker::formats::xbox::crypto::xcp_dumper {

using Path = py::str;   // pathlib.Path, in its Windows spelling

// constants
inline constexpr int64_t BUFF_SIZE = 4096;
inline constexpr int64_t XENON_DATA_SIZE = 0x1C;
inline const std::optional<py::str> WORK_DIR = std::nullopt;   // None
extern const py::str CACHE_DIR;
extern const py::str TMP_CAB_FILE;

// aliases
using c_word = uint16_t;
using c_dword = uint32_t;
using c_qword = uint64_t;

// LittleEndianStructure layouts
struct CAB_HEADER {
    c_dword magic;
    c_dword cksm_hdr;
    c_dword cb_cabinet;
    c_dword cksm_folders;
    c_dword off_files;
    c_dword cksm_files;
    c_word version;
    c_word cnt_folders;
    c_word cnt_files;
    c_word cnt_flags;
    c_word flags;
    c_word set_id;
    c_word i_cabinet;
};

struct CAB_FOLDER {
    c_dword off_cab_start;
    c_word cf_data;
    c_word type_compress;
    // ("xenon_data", c_ubyte * 0x1C) follows each folder in the file
};

struct CAB_ENTRY {
    c_dword cb_file;
    c_dword off_folder_start;
    c_word i_folder;
    c_word date;
    c_word time;
    c_word attribs;
};

struct CAB_DATA {
    c_dword cksm;
    c_word cb_data;
    c_word cb_uncomp;
};

struct RC4_SHA_HEADER {
    uint8_t cksm[0x14];
    uint8_t confounder[8];
};

// Type.from_buffer_copy(data): ValueError when data is shorter than the structure
template <class T> T from_buffer_copy(const py::bytes& data) {
    if (data.size() < static_cast<int64_t>(sizeof(T))) {
        throw py::ValueError("Buffer size too small (" + std::to_string(data.size()) + " instead of at least " +
                             std::to_string(sizeof(T)) + " bytes)");
    }
    T value;
    std::memcpy(&value, data.raw().data(), sizeof(T));
    return value;
}

// pathlib.Path(s), as str() prints it
Path path_type(const py::str& s);

// Run the platform's cabinet extractor (expand on Windows, cabextract on posix); true when it exits 0.
bool extract_cab(const py::str& filename, const py::str& path);

// Decrypt `size` bytes at data_offset with the RC4 key made from the RC4_SHA_HEADER at struct_offset.
// (The StreamIO of this codebase has no perform_function_at: this raises AttributeError once the header
// has been read and the cipher made, as the Python does.)
py::bytes stream_decrypt_with_struct(StreamIO::StreamIO& xcp, const py::bytes& key, int64_t struct_offset, int64_t data_offset, int64_t size);

// zlib.decompressobj() with the default window (the zlib wrapper)
class Decompress {
public:
    py::bytes unused_data;   // what followed the end of the stream
    bool eof = false;        // the end of the stream was reached

    Decompress();
    // The output the input yields; input that ends mid-symbol is held until the next call. zlib.error
    // (xell_unpack::ZlibError) for a corrupt stream.
    py::bytes decompress(const py::bytes& data);
    // What is still pending: b"" (decompress hands back all it can)
    py::bytes flush();

private:
    struct Impl;
    std::shared_ptr<Impl> impl_;
};

class XCPFile {
public:
    static constexpr int64_t SVOD_INDEX_SIZE = 0xB000;
    static constexpr int64_t SVOD_BLOCK_SIZE = 0x1000;
    static constexpr int64_t SVOD_DATA_SIZE = 0xA290000;

    std::optional<py::file> handle;   // None until opened
    int64_t total_size = 0;

    explicit XCPFile(const py::str& path);
    XCPFile& __enter__();
    void __exit__();

    // Make <out_path>/<title id>/00007000/<name> holding hdr, and the empty <name>.data folder; returns the latter.
    Path create_file_struct(const py::str& out_path, const py::bytes& hdr);

    // `stream` is an io.BytesIO or a file in the Python: a StreamIO (over bytes) or a file here.
    int64_t calc_buffer_len(StreamIO::StreamIO& stream, const py::bytes& data);
    int64_t calc_buffer_len(py::file& stream, const py::bytes& data);
    int64_t calc_bytes_left(StreamIO::StreamIO& stream, int64_t size);
    int64_t calc_bytes_left(py::file& stream, int64_t size);
    // data[offset:] for size 0, else data[offset:offset + size]
    py::bytes get_chunk(const py::bytes& data, int64_t offset = 0, int64_t size = 0);
    // -1 for the index file, else ceil((offset - index size) / data file size)
    int64_t calc_data_file_num_from_offset(int64_t offset);
    // Unpack the XCP into out_path. zlib.error, struct.error or AssertionError for a stream that is not one.
    void extract(const py::str& out_path);
};

// The command line (argv is sys.argv[1:], prog the basename of sys.argv[0]); the exit code.
// Raises SystemExit as argparse does (a usage error: code 2 with the usage and message on stderr; --help:
// code 0 with the help on stdout).
int64_t main(const py::list<py::str>& argv, const py::str& prog = py::str("xcp_dumper.py"));

}  // namespace codebreaker::formats::xbox::crypto::xcp_dumper
