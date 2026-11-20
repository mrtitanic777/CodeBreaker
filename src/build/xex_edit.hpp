// build/xex_edit.hpp -- change an Xbox 360 executable: its privilege flags, or its code at an address.
//
//     codebreaker xex-edit privileges <in.xex>                         the privilege bits that are set
//     codebreaker xex-edit privileges <in.xex> <+bit|-bit ...> <out>   set (+) or clear (-) bits; `pal50` names bit 10, PAL-50 incompatible
//     codebreaker xex-edit nop   <in.xex> <address> [count] <out>      count instructions become nop
//     codebreaker xex-edit bytes <in.xex> <address> <hex> <out>        raw bytes over the code
//     codebreaker xex-edit asm   <in.xex> <address> "<asm>" <out>      PowerPC assembled there
//     codebreaker xex-edit hook  <in.xex> <address> <cave> "<asm>" <out>
//                                                                      `b cave` at address; at the cave the asm, the displaced
//                                                                      instruction, and `b address+4`
//     codebreaker xex-edit asm-only "<asm>" [address]                  the bytes, nothing written
//
// Privileges live in the header's privilege entries (key 0x30000: bits 0-31; 0x30100: bits 32-63), changed where they stand. A code
// edit writes the module as an unencrypted XEX holding the whole image in one block (xex::write_basic) and changes the image there;
// that is what the recompiler, the Xbox 360 runtime and emulators read. Several asm lines are separated by `;` or newlines. The
// header's hashes and signature are not redone: `codebreaker xex compress` / `machine` re-pack and re-sign. Only bit 10 is named
// here: the meaning of the other bits is not settled from a source this project trusts, so they are given by number.
//
// Problems with the arguments (a bit out of range, an address outside the image, an instruction that cannot be moved, an assembler
// error) are reported as "xex-edit: <message>" on stdout with exit code 1.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "formats/xex.hpp"

namespace cb::build::xex_edit {

inline constexpr std::uint32_t kPrivilegeKeyLow = 0x00030000;    // bits 0-31
inline constexpr std::uint32_t kPrivilegeKeyHigh = 0x00030100;   // bits 32-63
inline constexpr std::uint32_t kPal50Incompatible = 10;
inline constexpr std::uint32_t kNop = 0x60000000;

// The privilege bits a header sets (ascending), and where its two entries' values are (offsets in the file).
struct Privileges {
    std::vector<unsigned> bits;
    std::optional<std::size_t> low_at, high_at;
};
// FormatError when the data is not an XEX2 header or its entry table is cut short.
Privileges privileges(ByteView xex_file);

struct Change {
    unsigned bit = 0;   // 0-63
    bool on = true;
};
// `xex_file` with each change made where the bit stands. UsageError for a bit whose entry the header lacks.
Bytes set_privileges(ByteView xex_file, std::span<const Change> changes);

// A module written as a basic XEX: its header, then the whole image from `pe_offset`.
class Image {
public:
    // The module written through the xex engine (FormatError when it has no load address).
    static Image open(const formats::xex::Tools& tools, const std::filesystem::path& xex);
    // For a basic XEX already in memory with its load address.
    Image(Bytes file, std::uint32_t base);

    std::uint32_t base() const { return base_; }
    // `size` bytes at a virtual address. UsageError when they are outside the image.
    Bytes read(std::uint32_t address, std::size_t size) const;
    void write(std::uint32_t address, ByteView bytes);
    const Bytes& file() const { return file_; }
    void save(const std::filesystem::path& out) const;

private:
    std::size_t offset(std::uint32_t address, std::size_t size) const;
    Bytes file_;
    std::uint32_t base_ = 0;
    std::size_t pe_offset_ = 0;
};

// Puts `b cave` at `address`; at the cave the assembled `asm_text`, the displaced instruction and `b address+4`. UsageError for a
// displaced instruction that branches relative to itself (it cannot move); AsmError for the assembler's errors.
void hook(Image& image, std::uint32_t address, std::uint32_t cave, std::string_view asm_text);

struct Environment {
    formats::xex::Tools tools;   // where unpacked images are kept, for the code edits
};

// `xex-edit <op> ...` as above. The arguments after the operation are the command's own (`-pal50` is a bit, not an option).
cli::Command command(const Environment& environment);

}  // namespace cb::build::xex_edit
