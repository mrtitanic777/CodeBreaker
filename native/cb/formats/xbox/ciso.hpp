// cb/formats/xbox/ciso.hpp -- CSO (Project Stellar's CISO v2) and CCI (Cerbios) block-compressed Xbox disc images, and the LZ4 blocks
// inside them.
//
//     ciso::Image image = ciso::Image::open("game.cso");     read as a device: gdf::Disc::open(std::make_shared<ciso::Image>(...))
//     ciso::write_cso("game.iso", "game.cso");               and back: ciso::to_iso("game.cso", "game.iso")
//
// Both wrap a raw XDVDFS image as 2048-byte sectors, each LZ4-compressed or stored, indexed by a table of 32-bit offsets (in 4-byte
// units; the top bit marks a compressed block; one more entry ends the table).
//
//   CSO  magic 'CISO', a 24-byte header, the index at 0x18; a block is [u32 length, top bit = stored][data].
//   CCI  magic 'CCIM', a 32-byte header giving where the index is (at the end of the file); a block is [u8 padding][lz4][padding].
//
// Made from the documented formats (facts cross-checked against xVerter, MIT); the decoded stream is a plain ISO, so the XDVDFS reader
// walks it as it is. Errors: FormatError for a file that is not one of these, a damaged index or a damaged LZ4 block.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/xbox/device.hpp"

namespace cb::formats::xbox::ciso {

inline constexpr std::uint32_t kBlockSize = 2048;

enum class Kind { Cso, Cci };

// A compressed image as a device of the uncompressed bytes, one block decoded at a time (the last one kept).
class Image final : public Device {
public:
    // FormatError when the file is neither (or is damaged); NotFoundError / AccessError when it cannot be opened.
    static Image open(const std::filesystem::path& path);

    Kind kind() const { return kind_; }
    // The uncompressed size, its blocks, and how many of them are stored compressed.
    std::uint64_t size() const { return size_; }
    std::uint64_t block_count() const { return block_count_; }
    std::uint64_t compressed_blocks() const;

    Bytes read_at(std::uint64_t offset, std::size_t length) override;

private:
    Image(const std::filesystem::path& path, Kind kind) : file_(path, fs::FileMode::Read), kind_(kind) {}
    Bytes decode_block(std::uint64_t index);

    fs::RandomFile file_;
    Kind kind_;
    std::uint64_t size_ = 0, block_count_ = 0;
    std::vector<std::uint64_t> offsets_;   // block i starts at offsets_[i] and ends where block i+1 starts
    std::vector<bool> compressed_;
    std::uint64_t cached_index_ = ~std::uint64_t{0};
    Bytes cached_;
};

// The kind of an image file by its magic; nullopt for any other file.
std::optional<Kind> kind_of(const std::filesystem::path& path);

// A raw LZ4 block (no frame) that decodes to `uncompressed_size` bytes. FormatError for an empty or truncated block, a zero or too far
// match offset, or a size that is not the one given.
Bytes lz4_decode_block(ByteView block, std::size_t uncompressed_size);
// A raw LZ4 block of `data` (greedy matching with a hash of 4 bytes: what the images' writers make).
Bytes lz4_compress_block(ByteView data);

// The ISO as a CSO / CCI image (the last sector zero-padded to a whole one); answers the uncompressed size.
std::uint64_t write_cso(const std::filesystem::path& iso, const std::filesystem::path& out);
std::uint64_t write_cci(const std::filesystem::path& iso, const std::filesystem::path& out);
// A CSO / CCI image as a plain ISO; answers the bytes written.
std::uint64_t to_iso(const std::filesystem::path& image, const std::filesystem::path& out);

std::string_view usage();
// `ciso info|list|extract|toiso|tocso|tocci ...`: the report on stdout and the exit code (0; 2 with the usage text for a command line
// it does not understand).
int command(std::span<const std::string> args);

}  // namespace cb::formats::xbox::ciso
