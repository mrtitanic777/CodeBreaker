// formats/xbox/crypto/build_lib.hpp -- the helpers of the Xbox 360 bootloader build scripts: the flash and bootloader header
// layouts, assembling PowerPC patches with the Free60 tools, patching, encrypting and signing bootloaders in place, the SE's LZX
// wrappers and the 4BL signing helpers.
//
// A bootloader image is a header (magic, build, QFE, flags, entry point, size), a 16-byte nonce, and the body the console's
// loader decrypts with RC4 and checks with a signature or a rot-sum hash. The "in place" functions take the whole flash image
// and the offset of the bootloader in it, as the scripts hold them; the ones that change bytes take the image as `Bytes&`.
// A patch table is what the console's patch slots hold: records of (address, count of words, the words) in big-endian, ended by
// an address of 0xFFFFFFFF.
//
//     Bytes sd = sign_sd_4bl(key, kSdSalt, image);                // sign a 4BL
//     Bytes patched = apply_patches(image, patch_table);           // write a patch table over an image
//     Bytes se = compress_se(codec, hypervisor_and_kernel);        // the SE bootloader's body
//
// The SE's LZX is the Xbox LZX library (LZXCompression.dll / LZXDecompression.dll of the xbox360_crypto_bin kit, or the Linux
// libraries); it is not part of this program, so the callers give an `LzxCodec` made from it (formats/xbox/crypto/lzx).
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "formats/xbox/crypto/xecrypt.hpp"

namespace cb::formats::xbox::crypto::build_lib {

// The bootloader magics: CA is the first-stage loader in the ROM, CB..CG the retail chain, SB..SG the devkit and signed one.
enum class BlMagic : std::uint16_t {
    Ca1bl = 0x0342, Cb2bl = 0x4342, Cc3bl = 0x4343, Cd4bl = 0x4344, Ce5bl = 0x4345, Cf6bl = 0x4346, Cg7bl = 0x4347,
    Sb2bl = 0x5342, Sc3bl = 0x5343, Sd4bl = 0x5344, Se5bl = 0x5345, Sf6bl = 0x5346, Sg7bl = 0x5347,
};

// ---- the layouts (big-endian in the file) ------------------------------------------------------------------------------

// The header of a NAND image's flash (0x80 bytes).
struct FlashHeader {
    static constexpr std::size_t kSize = 0x80;
    std::uint16_t magic = 0, build = 0, qfe = 0, flags = 0;
    std::uint32_t entry = 0, size = 0;
    std::array<char, 0x40> copyright{};
    std::array<std::uint8_t, 0x10> padding{};
    std::uint32_t kv_length = 0, sys_upd_addr = 0;
    std::uint16_t patch_slots = 0, kv_version = 0;
    std::uint32_t kv_offset = 0, patch_slot_size = 0, smc_config_offset = 0, smc_length = 0, smc_offset = 0;

    // TruncatedError if `data` holds fewer than kSize bytes.
    static FlashHeader parse(ByteView data);
    Bytes to_bytes() const;
};

// The header of a bootloader (0x10 bytes), and with the nonce that follows it in the signed ones (0x20).
struct BootloaderHeader {
    static constexpr std::size_t kSize = 0x10;
    std::array<std::uint8_t, 2> magic{};
    std::uint16_t build = 0, qfe = 0, flags = 0;
    std::uint32_t entry = 0, size = 0;
    // Present in 2BL..6BL of the signed chain (SB..SF): 16 bytes after the header.
    std::optional<std::array<std::uint8_t, 0x10>> nonce;

    // `with_nonce`: read the 16 bytes that follow the header too. TruncatedError when `data` is too short.
    static BootloaderHeader parse(ByteView data, bool with_nonce = true);
    Bytes to_bytes() const;

    std::size_t header_size() const { return nonce ? 0x20 : 0x10; }
    // The size rounded up to 16 bytes, which is what the console loads.
    std::uint32_t padded_size() const { return (size + 0xF) & ~std::uint32_t{0xF}; }
};

// ---- patch tables -------------------------------------------------------------------------------------------------------

// One record of a patch table.
struct Patch {
    std::uint32_t address = 0;
    ByteView words;   // a multiple of 4 bytes, in the table
};
// The records of the table, up to the 0xFFFFFFFF that ends it. TruncatedError when the table ends in the middle of a record or
// without its end.
std::vector<Patch> patch_records(ByteView table);

// The table written over a copy of `image`: each record's bytes at its address (an address past the end grows the image with zeros).
Bytes apply_patches(ByteView image, ByteView table);
// The same in place: how many records changed anything (a record already in the image counts for nothing).
std::size_t patch_in_place(Bytes& image, ByteView table);
// How many bytes of the table's records lie at or past `size` (what an image of that size must grow by to hold them).
std::size_t calc_patch_size(ByteView table, std::size_t size);

// The bytes `size` needs to reach a multiple of `bounds` (0 when it is one).
inline std::size_t calc_pad_size(std::size_t size, std::size_t bounds = 16) { return (bounds - size % bounds) % bounds; }
// The same for a bootloader's size: padded to 16.
inline std::size_t calc_bldr_pad_size(std::size_t size) { return calc_pad_size(size, 16); }

// The branch the 4BL's loader jumps through is rewritten to jump to `size`: the first word 0x4C000024 of the image becomes a
// `b` to the end of the image. FormatError when there is no such word.
Bytes apply_jump_sd_4bl(ByteView image, std::size_t size);

// ---- bootloaders in an image ---------------------------------------------------------------------------------------------

// The bytes of the bootloader at `offset` after its header and nonce, padded to 16: the size field minus 0x20, rounded up.
// FormatError when the magic is not SB, SC, SD or SE, or the bootloader does not fit the image.
std::size_t bootloader_size(ByteView image, std::size_t offset);

// RC4-encrypt (or decrypt: the same operation) the bootloader's body with `key`, in place.
void encrypt_bootloader_in_place(ByteView key, Bytes& image, std::size_t offset);
// The rot-sum SHA hash an SE is signed by: its first 0x10 bytes and its body.
Bytes calc_se_hash(ByteView image, std::size_t offset);
// Sign the bootloader at `offset` (the hash of its first 0x10 bytes and what follows the signature block) and write the signature
// over its signature block (offset + 0x20, 0x100 bytes).
void sign_bootloader_in_place(Bytes& image, std::size_t offset, const xecrypt::RsaKey& key);

// ---- the 4BL ------------------------------------------------------------------------------------------------------------------

// The salt of the SD signatures (XBOX_ROM_4).
inline constexpr std::string_view kSdSalt = xecrypt::kSaltSd;

// A copy of `image` (an SD, 4BL) signed with the private `key`: the signature over the hash of its first 0x10 bytes and what
// follows the signature block (from 0x120) is written at 0x20. FormatError when the image is shorter than 0x120.
Bytes sign_sd_4bl(const xecrypt::RsaKey& key, ByteView salt, ByteView image);
// Does the signature at 0x20 check out? FormatError when the image is shorter than 0x120.
bool verify_sd_4bl(const xecrypt::RsaKey& key, ByteView salt, ByteView image);

// RC4 of the bootloader with `key`: the whole of `data`, or with `skip_header` all but its first 0x20 bytes (the header and nonce
// stay as they are). A copy.
Bytes encrypt_bl(ByteView key, ByteView data, bool skip_header = true);

// ---- the SE's LZX ----------------------------------------------------------------------------------------------------------------

// The Xbox LZX library, as the SE bootloader uses it: the stream of chunks that `compress_continuous` makes and
// `decompress_continuous` reads. Not part of this program (see the header's note).
class LzxCodec {
public:
    virtual ~LzxCodec() = default;
    virtual Bytes compress_continuous(ByteView data) = 0;
    virtual Bytes decompress_continuous(ByteView data) = 0;
};

// The SE's body compressed. With `include_header` a 0x30-byte header comes first: the size (header included) at 0xC and the
// 0x280000 at 0x28 the loader reads.
Bytes compress_se(LzxCodec& codec, ByteView data, bool include_header = true);
// An SE body decompressed; `skip_header`: the 0x30 bytes in front of it are not part of the stream. TruncatedError when the data
// is shorter than the header.
Bytes decompress_se(LzxCodec& codec, ByteView data, bool skip_header = true);

// ---- the first source that has something ---------------------------------------------------------------------------------------

// A place to read bytes from: a file, or the bytes themselves.
using Source = std::variant<std::filesystem::path, Bytes>;
// The first source that has content: a path that is a file (its bytes), or bytes that are not empty. nullopt when none has.
std::optional<Bytes> first_source(std::span<const Source> sources);

// ---- assembling patches with the Free60 tools -----------------------------------------------------------------------------------

// Where xenon-as and xenon-objcopy are (settings xbox360_crypto_bin) and the include folder of the patches.
struct PatchTools {
    std::filesystem::path bin_dir;
    std::filesystem::path include_dir = "Patches/include";
};

// What a patch is built for: it sets the symbols its source tests (`--defsym NAME=1`).
enum class PatchTarget { Plain, Devkit, Retail, Flash, VfusesFlash, Hdd, VfusesHdd };
std::vector<std::string> target_defines(PatchTarget target);   // DEVKIT; RETAIL; FLASH; VFUSES FLASH; HDD; VFUSES HDD

// Assemble `asm_file` (xenon-as: big-endian, many, register names; includes beside the file and from the include folder) and
// write the raw binary of its code to `bin_file` (xenon-objcopy). `defines` add to the target's. ToolError (the tool and its
// exit code) when either tool fails or cannot be started.
void assemble_patch(const PatchTools& tools, const std::filesystem::path& asm_file, const std::filesystem::path& bin_file,
                    PatchTarget target = PatchTarget::Plain, std::span<const std::string> defines = {});

// A program's exit code and what it printed (stdout, UTF-8 with the bytes that are not text dropped).
struct CommandResult {
    std::uint32_t exit_code = 0;
    std::string output;
};
CommandResult run_command(const std::filesystem::path& program, std::span<const std::string> arguments);

}  // namespace cb::formats::xbox::crypto::build_lib
