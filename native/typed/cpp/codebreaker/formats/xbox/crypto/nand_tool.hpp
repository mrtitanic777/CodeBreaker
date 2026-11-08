// codebreaker/formats/xbox/crypto/nand_tool.hpp -- typed C++ from codebreaker.formats.xbox.crypto.nand_tool.
//
// Xbox 360 NAND image tools: the 16-byte spare ("meta") layouts, page ECC, a NAND image reader/writer that
// works in file offsets (528-byte pages: 512 data + 16 spare) or in flash offsets (512-byte pages), and the
// image build / ECC batch operations (nandpro / xeBuild class).
//
// Typing notes:
//   * The three ctypes BigEndianStructure classes (SMALLBLOCK, BIGONSMALL, BIGBLOCK) become plain structs of
//     the same fields, in the same names. A ctypes bit-field member (block_id_0:4, fs_unused_0:4,
//     fs_block_type:6, ecc_3:2) is a whole byte here holding the field's value; from_buffer_copy() splits
//     the shared byte (first field in the high bits, as ctypes does for a big-endian structure) and
//     to_bytes() -- the Python's bytes(structure) -- joins it, masking each field to its width.
//   * The class attributes block_id / fs_size are computed by __getattribute__ in Python; here they are the
//     methods block_id() / fs_size(). BIGBLOCK refuses to set them (NotImplementedError), as set_block_id()
//     / set_fs_size() do.
//   * NANDImage(filename_or_data) is one constructor per kind of argument: path (py::str), bytes, bytearray.
//     The two properties file_offset / flash_offset are the getter file_offset() and the setter
//     set_file_offset() (and the same for flash_offset).
//   * `data: bytes | bytearray` parameters take a py::bytes (a bytearray converts to it); fix_page_ecc has an
//     overload for a bytearray spare, which it fills in place, as the Python does.
//   * SEEK_SET / SEEK_CUR / SEEK_END are spelled with a trailing underscore (C macros).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/xbox/crypto/StreamIO.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::crypto::nand_tool {

// constants
inline constexpr int64_t SEEK_SET_ = 0;
inline constexpr int64_t SEEK_CUR_ = 1;
inline constexpr int64_t SEEK_END_ = 2;

inline constexpr int64_t MODE_FILE = 0;
inline constexpr int64_t MODE_FLASH = 1;

// types
using BYTE = uint8_t;
using WORD = uint16_t;
using DWORD = uint32_t;
using QWORD = uint64_t;

// The fields the three spare layouts share (they differ in the order the bytes lie in).
struct BlockSpare {
    BYTE block_id_1 = 0;   // lba/id = (((BlockID0<<8)&0xF)+(BlockID1&0xFF))
    BYTE block_id_0 = 0;   // 4 bits
    BYTE fs_unused_0 = 0;  // 4 bits
    BYTE fs_sequence_0 = 0;
    BYTE fs_sequence_1 = 0;
    BYTE fs_sequence_2 = 0;
    BYTE bad_block = 0;
    BYTE fs_sequence_3 = 0;
    BYTE fs_size_1 = 0;  // (((FsSize0<<8)&0xFF)+(FsSize1&0xFF)) = cert size
    BYTE fs_size_0 = 0;
    BYTE fs_page_count = 0;
    // free pages left in block (ie: if 3 pages are used by cert then this would be 29:0x1d)
    std::array<BYTE, 2> fs_unused_1{};
    BYTE fs_block_type = 0;  // 6 bits
    BYTE ecc_3 = 0;          // 2 bits; 26 bit ECD
    BYTE ecc_2 = 0;
    BYTE ecc_1 = 0;
    BYTE ecc_0 = 0;

    int64_t block_id() const;
    int64_t fs_size() const;
};

struct SMALLBLOCK : BlockSpare {
    static constexpr int64_t SIZE = 16;   // ctypes.sizeof
    static SMALLBLOCK from_buffer_copy(const py::bytes& buffer);   // ValueError if shorter than SIZE
    py::bytes to_bytes() const;
};

struct BIGONSMALL : BlockSpare {
    static constexpr int64_t SIZE = 16;
    static BIGONSMALL from_buffer_copy(const py::bytes& buffer);
    py::bytes to_bytes() const;
};

struct BIGBLOCK : BlockSpare {
    static constexpr int64_t SIZE = 16;
    static BIGBLOCK from_buffer_copy(const py::bytes& buffer);
    py::bytes to_bytes() const;
    // FS: 06 (system reserve block number) else (((FsSize0<<8)&0xFF)+(FsSize1&0xFF)) = cert size
    // FS: 20 (size of flash filesys in smallblocks >>5)
    // FS: 04 (system config reserve) free pages left in block (multiples of 4 pages, ie if 3f then 3f*4 pages are free after)
    void set_block_id(int64_t value);   // NotImplementedError("block_id can't be set yet!")
    void set_fs_size(int64_t value);    // NotImplementedError("fs_size can't be set yet!")
};

int64_t calc_page_ecc(const py::bytes& data, const py::bytes& spare);

// (data, spare): the spare filled in as a bytearray (a copy of a bytes spare; the argument itself when it
// is a bytearray)
std::tuple<py::bytes, py::bytearray> fix_page_ecc(const py::bytes& data, const py::bytes& spare);
std::tuple<py::bytes, py::bytearray> fix_page_ecc(const py::bytes& data, const py::bytearray& spare);
std::tuple<py::bytearray, py::bytearray> fix_page_ecc(const py::bytearray& data, const py::bytes& spare);
std::tuple<py::bytearray, py::bytearray> fix_page_ecc(const py::bytearray& data, const py::bytearray& spare);

class NANDImage {
public:
    int64_t mode = MODE_FILE;
    std::optional<StreamIO::StreamIO> stream;   // None until opened
    int64_t file_size = 0;
    int64_t flash_size = 0;
    int64_t num_pages = 0;

    int64_t file_offset() const;
    void set_file_offset(int64_t offset);
    int64_t flash_offset() const;
    void set_flash_offset(int64_t offset);

    // NANDImage(path): FileNotFoundError if there is no such file
    explicit NANDImage(const py::str& filename, int64_t mode = MODE_FILE);
    explicit NANDImage(const py::bytes& data, int64_t mode = MODE_FILE);
    explicit NANDImage(const py::bytearray& data, int64_t mode = MODE_FILE);

    NANDImage& __enter__();
    void __exit__();

    void reset();

    // stream primitives
    int64_t seek(int64_t offset, int64_t whence = SEEK_SET_);
    int64_t tell();
    py::bytes read(int64_t num = 0);
    int64_t write(const py::bytes& data);
    void flush();

    // extended I/O functions

    // offset translation functions
    // Convert file offset to flash offset
    int64_t file_to_flash_offset(int64_t file_offset) const;
    // Convert flash offset to file offset
    int64_t flash_to_file_offset(int64_t flash_offset) const;
    // Get the page a file offset lands on
    int64_t file_offset_to_page(int64_t offset) const;
    // Get the page a flash offset lands on
    int64_t flash_offset_to_page(int64_t offset) const;
    // Calculates the start or end offset for page I/O
    int64_t flash_calc_page_offset(int64_t offset) const;
    // Calculates the start offset for a given page number
    int64_t calc_page_offset(int64_t num) const;
    int64_t calc_spare_offset(int64_t num) const;

    // file primitives
    int64_t file_tell();
    int64_t file_seek(int64_t offset, int64_t whence = SEEK_SET_);
    py::bytes file_read(int64_t num = 0);
    int64_t file_write(const py::bytes& data);

    // flash primitives
    int64_t flash_tell();
    int64_t flash_seek(int64_t offset, int64_t whence = SEEK_SET_);
    py::bytes flash_read(int64_t num = 0);
    int64_t flash_write(const py::bytes& data);

    // page I/O
    // Get a page by page number
    py::bytes get_page(int64_t num);
    // Get a spare by page number
    py::bytes get_spare(int64_t num);
    // Set a page by page number (AssertionError for a page out of range or data that is not 512 bytes)
    int64_t set_page(int64_t num, const py::bytes& data);
    // Set a spare by spare number (AssertionError for a page out of range or data that is not 16 bytes)
    int64_t set_spare(int64_t num, const py::bytes& data);

private:
    int64_t _file_offset = 0;
    int64_t _flash_offset = 0;
    bool closed_ = false;

    StreamIO::StreamIO& stream_();   // the stream (AttributeError if None, ValueError once closed)
};

// ---- image build / ECC batch operations (nandpro / xeBuild class) ----------
inline constexpr int64_t PAGE = 512;
inline constexpr int64_t SPARE = 16;
inline constexpr int64_t FULL = 528;

// The 26-bit ECC value packed into spare[12..15] by fix_page_ecc.
int64_t _stored_ecc(const py::bytes& spare);

// 528-byte-page flash image  ->  raw 512-byte-page data (drop spare/meta).
py::bytes strip_spare(const py::bytes& img);

// raw 512-byte-page data  ->  528-byte-page image with correct ECC. `meta`
// (optional) supplies the per-page 16-byte spare template; ECC is recomputed.
py::bytes add_spare(const py::bytes& raw, const std::optional<py::bytes>& meta = std::nullopt);

// Return the list of page indices whose stored ECC does not match.
py::list<int64_t> verify_ecc(const py::bytes& img);

// Recompute every page's ECC in place (repair after editing raw data).
py::bytes fix_all_ecc(const py::bytes& img);

extern const py::str DESCRIPTION;

// main(): `a` is sys.argv[1:]; prints to stdout. sys.exit(n) is a thrown py::SystemExit(n).
void main(const py::list<py::str>& a);

}  // namespace codebreaker::formats::xbox::crypto::nand_tool
