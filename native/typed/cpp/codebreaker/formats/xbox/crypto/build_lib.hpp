// codebreaker/formats/xbox/crypto/build_lib.hpp -- C++ port of codebreaker.formats.xbox.crypto.build_lib.
//
// The helpers of the Xbox 360 bootloader build scripts: the flash and bootloader header layouts, BLHeader,
// reading and writing files, assembling PowerPC patches with the Free60 tools, patching / encrypting /
// signing bootloaders in place, the SE LZX wrappers and the 4BL signing helpers.
//
// Typing notes:
//   * BinLike (bytes | bytearray | memoryview) is py::bytes (a py::bytearray converts to it). Functions
//     annotated `-> bytearray` return a new py::bytearray; compress_se, annotated `-> bytes`, is a py::bytes
//     even with its header (the Python returns the bytearray it built there; as in XeCrypt, the object is
//     new, so only its mutability differs).
//   * pathlib.Path arguments are py::str (a Path is only ever made from, and turned back into, a str here).
//   * ctypes BigEndianStructures are structs of host-order values with to_bytes() / from_buffer_copy() (as
//     lzx's LZXBOX_BLOCK); sizes are the ctypes ones. `copyright` (c_char * 0x40) is kept as its 0x40 raw bytes.
//   * `stream: BinaryIO` (an io.BytesIO or an open file in the callers) is a template over the stream class:
//     StreamIO::StreamIO (a memory stream) and py::file (a file opened "r+b") are the instances made here.
//   * BLHeader.__getitem__ answers what getattr(self, name, None) is: a field (bytes / int / bool), None, or
//     -- for a method, a property-less dunder or any other attribute that is no field -- `Attribute::Object`.
//   * BLHeader.__setitem__ takes the value as bytes, int or bool; the Python accepts any object and the
//     attribute then simply holds it. Here the value must be of the field's own type (else TypeError), and a
//     key that names a method or a dunder (setattr would put an instance attribute over it) changes nothing.
//   * `*sources: str | Path | BinLike` is a py::list of std::variant; `*defines: str` a py::list<py::str>.
//   * The module-level BIN_DIR (settings.get("xbox360_crypto_bin") at import time) is read on first use.
//   * Not ported: the unreachable code after `sio.offset` in apply_jump_sd_4bl (the Python's StreamIO has
//     no `offset`, so that function ends in AttributeError there).
//   * Programs are run through `cmd /c` as subprocess.run(shell=True) does on Windows (Windows only).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/StreamIO.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "codebreaker/formats/xbox/crypto/lzx.hpp"

namespace codebreaker::formats::xbox::crypto::build_lib {

using BinLike = py::bytes;   // bytes | bytearray | memoryview

// settings.get("xbox360_crypto_bin"): Free60 xenon-as / xenon-objcopy dir (read on first use)
const py::str& BIN_DIR();
extern const py::str INCLUDE_DIR;   // "Patches/include"

// types
using BYTE = uint8_t;
using WORD = uint16_t;
using DWORD = uint32_t;
using QWORD = uint64_t;

enum class BLMagic : int64_t {
    CA_1BL = 0x0342,
    CB_2BL = 0x4342,
    CC_3BL = 0x4343,
    CD_4BL = 0x4344,
    CE_5BL = 0x4345,
    CF_6BL = 0x4346,
    CG_7BL = 0x4347,
    SB_2BL = 0x5342,
    SC_3BL = 0x5343,
    SD_4BL = 0x5344,
    SE_5BL = 0x5345,
    SF_6BL = 0x5346,
    SG_7BL = 0x5347,
};

// structures (BigEndianStructure): from_buffer_copy -> ValueError when the buffer is shorter than the structure
struct FLASH_HEADER {
    static constexpr int64_t SIZE = 0x80;
    WORD magic = 0;
    WORD build = 0;
    WORD qfe = 0;
    WORD flags = 0;
    DWORD entry = 0;
    DWORD size = 0;
    std::array<BYTE, 0x40> copyright{};
    std::array<BYTE, 0x10> padding{};
    DWORD kv_length = 0;
    DWORD sys_upd_addr = 0;
    WORD patch_slots = 0;
    WORD kv_version = 0;
    DWORD kv_offset = 0;
    DWORD patch_slot_size = 0;
    DWORD smc_config_offset = 0;
    DWORD smc_length = 0;
    DWORD smc_offset = 0;
    py::bytes to_bytes() const;
    static FLASH_HEADER from_buffer_copy(const py::bytes& data);
};

struct BLDR_HEADER {
    static constexpr int64_t SIZE = 0x10;
    std::array<BYTE, 2> magic{};
    WORD build = 0;
    WORD qfe = 0;
    WORD flags = 0;
    DWORD entry = 0;
    DWORD size = 0;
    py::bytes to_bytes() const;
    static BLDR_HEADER from_buffer_copy(const py::bytes& data);
};

struct SB_2BL_HEADER {
    static constexpr int64_t SIZE = 0x20;
    BLDR_HEADER header;
    std::array<BYTE, 0x10> nonce{};
    py::bytes to_bytes() const;
    static SB_2BL_HEADER from_buffer_copy(const py::bytes& data);
};

using SC_3BL_HEADER = SB_2BL_HEADER;
using SD_4BL_HEADER = SB_2BL_HEADER;
using SE_5BL_HEADER = SB_2BL_HEADER;
using SF_6BL_HEADER = SB_2BL_HEADER;

using HV_HEADER = BLDR_HEADER;

// BLHeader.__dict__(): the fields as a record (nonce only when the header includes one)
struct BLHeaderDict {
    std::optional<py::bytes> magic;
    std::optional<int64_t> build;
    std::optional<int64_t> qfe;
    std::optional<int64_t> flags;
    std::optional<int64_t> entry_point;
    std::optional<int64_t> size;
    bool has_nonce = false;
    std::optional<py::bytes> nonce;   // meaningful when has_nonce
};

// What getattr(header, name, None) is: None, a field (bytes, int, bool) or some other object (a method ...).
struct Attribute {
    struct Object {};
    std::variant<std::monostate, py::bytes, int64_t, bool, Object> value;
};

// a value for BLHeader.__setitem__
using FieldValue = std::variant<py::bytes, int64_t, bool>;

class BLHeader {
public:
    bool include_nonce = true;

    std::optional<py::bytes> magic;
    std::optional<int64_t> build;
    std::optional<int64_t> qfe;
    std::optional<int64_t> flags;
    std::optional<int64_t> entry_point;
    std::optional<int64_t> size;

    std::optional<py::bytes> nonce;

    explicit BLHeader(bool include_nonce = true);

    // bytes(header): struct.error for a missing or out-of-range field, TypeError for a missing nonce
    py::bytes __bytes__() const;
    BLHeaderDict __dict__() const;
    Attribute __getitem__(const py::str& item) const;
    void __setitem__(const py::str& key, const FieldValue& value);

    int64_t header_size() const;
    int64_t padded_size() const;   // TypeError while size is None
    bool requires_padding() const;

    // struct.error for data that is too short
    static BLHeader parse(const BinLike& data, bool include_nonce = true);

    void reset();
};

// None for a name that is no file; text makes the content a str (read with the locale's encoding)
std::optional<std::variant<py::bytes, py::str>> read_file_or_none(const py::str& filename, bool text = false);
std::variant<py::bytes, py::str> read_file(const py::str& filename, bool text = false);
// A str is written as text, bytes and bytearray as they are.
void write_file(const py::str& filename, const std::variant<py::str, py::bytes, py::bytearray>& data);

// a source: a path (str or Path), or binary data
using Source = std::variant<py::str, py::bytes, py::bytearray>;
// Read from sources until one is found and if not then return none.
// sources: str, Path, or binary sources, string will read from a path, binary returns itself
// returns: source data otherwise none
std::optional<std::variant<py::bytes, py::bytearray>> try_read_sources(const py::list<Source>& sources);

// Assemble an xenon .S file to a raw binary with the Free60 tools (AssertionError when a tool fails)
void assemble_patch(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines = py::list<py::str>());
void assemble_devkit_patch(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines = py::list<py::str>());
void assemble_retail_patch(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines = py::list<py::str>());
void assemble_rgl_flash(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines = py::list<py::str>());
void assemble_rgl_vfuses_flash(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines = py::list<py::str>());
void assemble_rgl_hdd(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines = py::list<py::str>());
void assemble_rgl_vfuses_hdd(const py::str& asm_filename, const py::str& bin_filename, const py::list<py::str>& defines = py::list<py::str>());

// (return code, what the program wrote to stdout decoded as UTF-8, bad bytes dropped)
std::tuple<int64_t, py::str> run_command(const py::str& path, const py::list<py::str>& args = py::list<py::str>());

// ---- in-place bootloader work on a stream (io.BytesIO or file in the Python) ----
// AssertionError("Invalid bootloader magic!") unless the magic at offset is SB, SC, SD or SE
template <class S> int64_t get_bldr_size_in_place(S& stream, int64_t offset);
// apply the (address, size, words) records of `patches`; the number of records that changed something
template <class S> int64_t patch_in_place(S& stream, const BinLike& patches);
template <class S> void encrypt_bldr_in_place(const py::bytes& key, S& stream, int64_t offset);
template <class S> py::bytes calc_se_hash_in_place(S& stream, int64_t offset);
template <class S> void sign_bldr_in_place(S& stream, int64_t offset, XeCrypt::XeCryptRsaKey& key);

// C functions
py::bytes decompress_se(const py::bytes& data, bool skip_header = true);
py::bytes compress_se(const py::bytes& data, bool include_header = true);
// key: an XeCryptRsaKey or its bytes. A bytes data is copied; a bytearray is signed in place and returned.
py::bytearray sign_sd_4bl(XeCrypt::XeCryptRsaKey& key, const py::bytes& salt, const py::bytes& data);
py::bytearray sign_sd_4bl(XeCrypt::XeCryptRsaKey& key, const py::bytes& salt, const py::bytearray& data);
py::bytearray sign_sd_4bl(const py::bytes& key, const py::bytes& salt, const py::bytes& data);
py::bytearray sign_sd_4bl(const py::bytes& key, const py::bytes& salt, const py::bytearray& data);
bool verify_sd_4bl(XeCrypt::XeCryptRsaKey& key, const py::bytes& salt, const py::bytes& data);
bool verify_sd_4bl(const py::bytes& key, const py::bytes& salt, const py::bytes& data);
py::bytearray encrypt_bl(const py::bytes& key, const BinLike& data, bool skip_header = true);
// (always AttributeError once the jump word is found, struct.error when it is not: see the notes)
py::bytearray apply_jump_sd_4bl(const py::bytes& data, int64_t size);
// patch_data: a path (str), or the patches as bytes
py::bytearray apply_patches(const py::bytes& bl_data, const std::variant<py::str, py::bytes, py::bytearray>& patch_data);
int64_t calc_patch_size(const py::bytes& patch_data, int64_t size);
int64_t calc_pad_size(int64_t size, int64_t bounds = 16);
int64_t calc_bldr_pad_size(int64_t size);

// __all__
py::list<py::str> __all__();

}  // namespace codebreaker::formats::xbox::crypto::build_lib
