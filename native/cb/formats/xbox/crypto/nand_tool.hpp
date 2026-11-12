// cb/formats/xbox/crypto/nand_tool.hpp -- Xbox 360 NAND images as the console's flash holds them: pages of 512 data bytes
// followed by a 16-byte spare, the page ECC that protects each page, the layouts of the spare's block fields, and the
// batch operations that move an image between the raw (512-byte pages) and flash (528-byte pages with ECC) forms.
//
//     nand_tool::NandImage image = nand_tool::NandImage::open("flash.bin");
//     image.write_flash(0x4000, patch);          // the touched pages get their ECC recomputed
//     image.save();
//
// Two address spaces are used. A *file* offset counts the bytes of the image with the spares in (528 per page); a *flash*
// offset counts only the data (512 per page), which is what the console's software addresses. Pages are numbered from 0.
//
// The page ECC is 26 bits over the 512 data bytes and the first 102 bits of the spare (its first 12 bytes and the 6 low
// bits of byte 12, the block type), stored in the spare's last 26 bits (the top two bits of byte 12 and bytes 13..15).
//
// Errors: UsageError for a page or range outside the image or data of the wrong size; FormatError for a spare that is not
// 16 bytes. Opening a file that is missing is a NotFoundError.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::formats::xbox::crypto::nand_tool {

inline constexpr std::size_t kPageSize = 512;    // data bytes of a page
inline constexpr std::size_t kSpareSize = 16;    // spare bytes after each page's data
inline constexpr std::size_t kFullPage = 528;    // a page as the image file holds it

// ---- the spare's block fields -----------------------------------------------------------------------------------------

// The three ways flash controllers lay out a spare: small-block flash, big-block flash seen through the small-block
// interface, and big-block flash. They hold the same fields; only where each lies in the 16 bytes differs.
enum class SpareLayout { SmallBlock, BigOnSmall, BigBlock };

// The fields of a spare (byte positions per SpareLayout). The 12-bit block id's low byte is spare byte 0 or 1 by layout and
// its top four bits are the low nibble of the byte after it; the filesystem sequence is kept as the four bytes the layouts
// name seq0..seq3.
struct BlockSpare {
    std::uint16_t block_id = 0;                   // 12 bits: the block's LBA/id
    std::uint8_t fs_unused_0 = 0;                 // 4 bits
    std::array<std::uint8_t, 4> fs_sequence{};    // fs_sequence_0 .. fs_sequence_3
    std::uint8_t bad_block = 0;
    std::uint16_t fs_size = 0;                    // the certificate size (system blocks: reserve block number / flash filesystem size >> 5)
    std::uint8_t fs_page_count = 0;               // free pages left in the block (a cert using 3 pages leaves 29: 0x1d; big blocks count in fours)
    std::array<std::uint8_t, 2> fs_unused_1{};
    std::uint8_t fs_block_type = 0;               // 6 bits
    std::uint32_t ecc = 0;                        // 26 bits, as page_ecc() computes

    friend bool operator==(const BlockSpare&, const BlockSpare&) = default;
};

// The fields of a 16-byte spare. FormatError if `spare` is not 16 bytes.
BlockSpare parse_spare(SpareLayout layout, ByteView spare);
// The 16 bytes of a spare. UsageError if a field is wider than its place (block_id over 12 bits, fs_unused_0 over 4,
// fs_block_type over 6, ecc over 26).
Bytes encode_spare(SpareLayout layout, const BlockSpare& fields);

// ---- page ECC -----------------------------------------------------------------------------------------------------------

// The 26-bit ECC of a page (512 data bytes) with its spare (16 bytes; the stored ECC bits themselves are not part of the
// sum). UsageError for the wrong sizes.
std::uint32_t page_ecc(ByteView data, ByteView spare);
// The ECC value a spare holds (bytes 12..15, 26 bits).
std::uint32_t stored_ecc(ByteView spare);
// Compute the ECC of `data` and store it in `spare` (the other 102 bits are kept).
void fix_page_ecc(ByteView data, ByteSpan spare);

// ---- whole images ------------------------------------------------------------------------------------------------------

// A flash image (a whole number of 528-byte pages; bytes after the last whole page are not part of any page) held in
// memory. Writing into it keeps every page's ECC right.
class NandImage {
public:
    static NandImage from_bytes(Bytes file);
    // Read the file; save() writes it back there.
    static NandImage open(const std::filesystem::path& path);

    std::size_t file_size() const { return file_.size(); }
    std::size_t page_count() const { return file_.size() / kFullPage; }
    std::size_t flash_size() const { return page_count() * kPageSize; }   // the data without the spares
    ByteView file_bytes() const { return file_; }

    // The 512 data bytes / 16 spare bytes of a page.
    ByteView page(std::size_t index) const;
    ByteView spare(std::size_t index) const;
    // Replace a page's data; its ECC is recomputed from the new data and the spare it has.
    void set_page(std::size_t index, ByteView data);
    // Replace a page's spare as it is given (the ECC in it is not touched).
    void set_spare(std::size_t index, ByteView spare);

    // `length` bytes of flash data from `offset`, across page boundaries (the spares are skipped).
    Bytes read_flash(std::uint64_t offset, std::size_t length) const;
    // Write flash data at `offset`, across page boundaries; every touched page's ECC is recomputed.
    void write_flash(std::uint64_t offset, ByteView data);

    // Write the image to the file it was opened from (UsageError for one made from bytes) / to another file.
    void save() const;
    void save_as(const std::filesystem::path& path) const;

private:
    Bytes file_;
    std::optional<std::filesystem::path> path_;
};

// An offset in the image file (spares in) to the flash offset (data only) and back. An offset that falls inside a
// page's spare has no flash address; it maps to the start of the next page's data.
std::uint64_t file_to_flash_offset(std::uint64_t file_offset);
std::uint64_t flash_to_file_offset(std::uint64_t flash_offset);

// ---- batch operations (nandpro / xeBuild class) ---------------------------------------------------------------------------

// A flash image (528-byte pages) to its raw data (512-byte pages): the spares are dropped.
Bytes strip_spare(ByteView image);
// Raw data to a flash image with correct ECC. `meta` supplies each page's spare (16 bytes per page, in order; UsageError if
// it covers fewer pages than `raw`); without it the spares start zeroed. The ECC is recomputed either way.
Bytes add_spare(ByteView raw, std::optional<ByteView> meta = std::nullopt);
// The indexes of the pages whose stored ECC is wrong.
std::vector<std::size_t> verify_ecc(ByteView image);
// The image with every page's ECC recomputed (the repair after editing raw data).
Bytes fix_all_ecc(ByteView image);

// ---- the command -------------------------------------------------------------------------------------------------------------

// The tool's usage text.
std::string_view usage();
// `nandtool {info|verify|unecc|ecc|fix} ...`: the report on stdout. Returns the exit code (1 when `verify` finds bad pages);
// UsageError for an unknown operation or a missing argument. No arguments, `-h` or `--help` print the usage and return 0.
int command(std::span<const std::string> args);

}  // namespace cb::formats::xbox::crypto::nand_tool
