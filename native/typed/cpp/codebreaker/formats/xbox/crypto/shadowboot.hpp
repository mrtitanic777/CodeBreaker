// codebreaker/formats/xbox/crypto/shadowboot.hpp -- typed C++ port of codebreaker.formats.xbox.crypto.shadowboot.
//
// A script to extract and build shadowboots (the Xbox 360 NAND-less boot image: flash header, SMC, keyvault and
// the SB / SC / SD / SE bootloaders), and the ShadowbootImage class that parses one.
//
// Typing notes:
//   * pathlib.Path is a py::str in its Windows spelling (backslashes, no "." parts, no trailing separator);
//     the runtime has no pathlib, so the few operations the module uses (Path(s), p / name, absolute(),
//     is_file(), read_bytes() ...) are functions of the .cpp.
//   * `BinLike` (bytes | bytearray | memoryview) is py::bytes (a py::bytearray converts to it).
//   * The attributes of ShadowbootImage are public members; "or None" ones are std::optional. The ctypes
//     FLASH_HEADER is build_lib's struct. `img_map` is the Python's dict of dicts: the dict of an
//     ImageMapEntry whose fields exist only for the keys the Python put there ("SMC" / "KV" have offset and
//     size; the bootloaders also pad_size, header and key). `patches` is a list of Patch, the dict the
//     Python builds (the first has offset / size_bytes / patch_loader, the others the rest).
//   * `_stream` is the io.BytesIO of parse(): a StreamIO over the bytes (only its byte access is used). parse()
//     closes it on the way out, as its `with` does, so read / write / seek / tell / getvalue on a parsed
//     image are the ValueError of a closed file; on a reset image they are the AttributeError of None.
//   * ShadowbootImage.create builds the image on a StreamIO with its labels (set_label / goto_label /
//     get_label). Its arguments are `optional` where main() can hand it None (try_read_sources); `build_version` is
//     std::optional<int64_t> where nullopt stands for any object that is not an int (None, str, float ...:
//     struct.error where it is packed, TypeError where ctypes takes it); `kv_data` is the (cpu key,
//     keyvault) pair, its keyvault None when main() found none.
//   * main(): `argv` is sys.argv[1:], `prog` the script name argparse prints; it returns the exit code, argparse's
//     exits are a thrown py::SystemExit, and the printing goes to stdout (error text of argparse to stderr).
//     The command line is read by the top-level parser written here (the runtime's py::argparse has no
//     sub-parsers) and one py::argparse parser per command; the text of `split`'s usage and -h (its options
//     have `choices`, which py::argparse shows as a plain metavar) is the Python's own, kept as a string.
//     A manifest is JSON of any shape: it is read as a py::V (the runtime's JSON value), the one place
//     the module's data is not typed.
//   * not ported: `__author__` / `__description__` / `__platforms__` / `__thanks__` as variables other than the
//     description main() prints; the `if __name__ == "__main__"` block (`exit(main())`); BinLike's TypeVar.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "codebreaker/formats/xbox/crypto/StreamIO.hpp"
#include "codebreaker/formats/xbox/crypto/XeCrypt.hpp"
#include "codebreaker/formats/xbox/crypto/build_lib.hpp"

namespace codebreaker::formats::xbox::crypto::shadowboot {

using BinLike = py::bytes;   // bytes | bytearray | memoryview

// module metadata
extern const py::str __author__;
extern const py::str __description__;

// constants
// settings.get("xbox360_crypto_bin"): Free60 xenon-as / xenon-objcopy dir (read on first use)
const py::str& BIN_DIR();
extern const py::str XELL_DIR;
extern const py::str BUILD_DIR;   // Path("Build")
extern const py::str PATCH_DIR;
inline constexpr int64_t BUILD_VER = 17559;
extern const py::str MANIFEST_FILE;
inline constexpr int64_t BUFFER_SIZE = 4096;
inline constexpr int64_t NOP = 0x60000000;

// size defines
inline constexpr int64_t HYPERVISOR_SIZE = 0x40000;
inline constexpr int64_t SMC_CONFIG_SIZE = 280;
inline constexpr int64_t SHADOWBOOT_SIZE = 0xD4000;

// keys: the 1BL public key, and the one used to sign the SD (its public key is in SB), loaded from the key
// store when the tool runs (main), not on import
extern std::optional<XeCrypt::XeCryptRsaKey> ONE_BL_KEY;
extern std::optional<XeCrypt::XeCryptRsaKey> SB_PRV_KEY;

// the crc32 of a file, read BUFFER_SIZE bytes at a time
int64_t checksum_file(const py::str& filename);
// True when cksm is 0 or the file is not there
bool verify_checksum(const py::str& filename, int64_t cksm);
// the 4 bytes of i, big endian, as 8 hex digits (struct.error when i does not fit an unsigned 32-bit number)
py::str pad_hex(int64_t i);
// the Path of a command line value (argparse's `type`); the parser argument is not used
py::str path_type(const py::str& value);
// a flash image's data without the spare bytes that follow each block
py::bytes unecc(const py::str& path, int64_t block_size = 512, int64_t spare_size = 16);

// img_map entry
struct ImageMapEntry {
    int64_t offset = 0;
    int64_t size = 0;
    std::optional<int64_t> pad_size;                // the bootloaders
    std::optional<build_lib::BLHeader> header;      // the bootloaders
    std::optional<py::bytes> key;                   // SB, SC, SD and SE
};

// a patch (the loader first, then the records)
struct Patch {
    int64_t offset = 0;
    int64_t size_bytes = 0;
    std::optional<py::bytes> patch_loader;          // the loader
    std::optional<int64_t> address;                 // a record
    std::optional<int64_t> size_int32;
    std::optional<py::bytes> patch_code;
    std::optional<py::bytes> whole_patch;
};

class ShadowbootImage {
public:
    // I/O stream
    std::shared_ptr<StreamIO::StreamIO> _stream;
    bool _stream_closed = false;

    // bootloader header map
    py::dict<py::str, ImageMapEntry> img_map;

    // headers
    std::optional<build_lib::FLASH_HEADER> flash_header;

    // shadowboot data (all decrypted)
    std::optional<py::bytes> smc_data;
    std::optional<py::bytes> smc_config_data;
    std::optional<py::bytes> kv_data;
    std::optional<py::bytes> sb_data;
    std::optional<py::bytes> sc_data;
    std::optional<py::bytes> sd_data;
    std::optional<py::bytes> se_data;
    std::optional<py::bytes> kernel_data;
    std::optional<py::bytes> hypervisor_data;
    py::list<Patch> patches;

    // metadata
    // SB
    std::optional<py::bytes> sb_sig;
    std::optional<XeCrypt::XeCryptRsaKey> sb_pub_key;
    std::optional<py::bytes> sc_nonce;
    std::optional<py::bytes> sc_salt;
    std::optional<py::bytes> sd_salt;
    std::optional<py::bytes> sd_digest;
    // SC
    std::optional<py::bytes> sc_sig;
    // SD
    std::optional<py::bytes> sd_sig;
    std::optional<XeCrypt::XeCryptRsaKey> sd_pub_key;
    std::optional<py::bytes> sf_nonce;
    std::optional<py::bytes> sf_salt;
    std::optional<py::bytes> se_digest;
    // bootloaders
    std::optional<int64_t> sb_build;
    std::optional<int64_t> sc_build;
    std::optional<int64_t> sd_build;
    std::optional<int64_t> se_build;
    // kernel
    std::optional<int64_t> kernel_version;
    // HV
    std::optional<int64_t> hypervisor_version;
    // SMC
    std::optional<py::str> console_type;
    std::optional<py::str> smc_version;
    // misc.
    bool is_retail = false;
    bool is_testkit = false;

    ShadowbootImage();

    // Parse a shadowboot image. perform_checks: the signatures and digests are checked (Exception("Invalid SB
    // signature") ... when one is wrong).
    static ShadowbootImage parse(const py::bytes& data, bool perform_checks = true, bool parse_patches = true);

    // Build an image from the parts. kv_data is (cpu key, keyvault).
    static py::bytes create(const std::optional<py::bytes>& sb_data, const std::optional<py::bytes>& sc_data,
                            const std::optional<py::bytes>& sd_data, const std::optional<py::bytes>& se_data,
                            const std::optional<py::bytes>& smc_data = std::nullopt,
                            const std::optional<std::tuple<py::bytes, std::optional<py::bytes>>>& kv_data = std::nullopt,
                            const std::optional<py::bytes>& patches = std::nullopt, bool test_kit = false,
                            std::optional<int64_t> build_version = BUILD_VER);

    ShadowbootImage& __enter__();
    void __exit__();

    void reset();

    py::bytes read(int64_t size);
    int64_t write(const py::bytes& data);
    int64_t seek(int64_t offset, int64_t whence = StreamIO::SEEK_SET_);
    int64_t tell();
    py::bytes getvalue();

    // header_type.from_buffer_copy(self._stream.read(sizeof(header_type))): ValueError when the stream is short
    template <class T> T read_header() { return T::from_buffer_copy(read(T::SIZE)); }

    void map_shadowboot();
    void parse_smc();
    void parse_smc_config();
    void parse_keyvault();
    void parse_sb_2bl();
    void parse_sc_3bl();
    void parse_sd_4bl();
    void parse_se_5bl();
    void decompress_se_5bl();
    void parse_hypervisor();
    void parse_patches();
    void parse_metadata();

    bool check_signature_sb_2bl();
    bool check_signature_sc_3bl();
    bool check_signature_sd_4bl();
    bool check_hash_sd_4bl();
    bool check_hash_se_5bl();

    void print_info();
};

// The command line tool. `argv` is sys.argv[1:]; returns the exit code.
int64_t main(const py::list<py::str>& argv, const py::str& prog = py::str("shadowboot.py"));

}  // namespace codebreaker::formats::xbox::crypto::shadowboot
