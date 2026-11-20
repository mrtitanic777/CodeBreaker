// formats/xbox/nand.hpp -- the flash file system of an Xbox 360 NAND dump, read as a folder of files.
//
//     if (nand::is_nand(path)) { nand::Flash flash = nand::Flash::parse(fs::read_file(path)); Bytes b = flash.read("smc.bin"); }
//
// A raw dump keeps 16 spare bytes after every 512-byte page; 32 pages make a block. The spare bytes of a block say what it is: all
// 0xFF is unused, type 0x30 is a root of the file system (with a sequence number; the newest wins), anything else is a data block
// that carries the logical number it stands for. The root block's even pages hold the allocation table (4096 big-endian words, the
// next block of each, 0x1FFF ends a chain) and its odd pages the directory (32-byte entries: a 22-byte name, the first block, the
// size in bytes, a time stamp, all big-endian).
//
// Errors: FormatError for a dump that is not whole pages, has no root block, or a file whose chain is bad or short.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::xbox::nand {

inline constexpr std::size_t kPage = 0x200, kSpare = 0x10, kPagesPerBlock = 32;
inline constexpr std::size_t kBlock = kPage * kPagesPerBlock;
inline constexpr std::uint32_t kChainEnd = 0x1FFF;

// A raw dump with spare bytes: a whole number of pages-with-spare, at least 16 MB, starting with the flash header's magic (FF 4F)
// and carrying Microsoft's copyright in its first 0x40 bytes. False for a file that cannot be read.
bool is_nand(const std::filesystem::path& path);

struct File {
    std::string name;
    std::uint32_t start_block = 0;
    std::uint32_t size = 0;
    std::uint32_t time = 0;
};

class Flash {
public:
    static Flash parse(Bytes raw);

    // The block of the newest root, and the files in directory order (a name met twice keeps its first place and the later entry).
    std::uint32_t root_block() const { return root_; }
    const std::vector<File>& files() const { return files_; }
    const File* find(std::string_view name) const;
    // The blocks of a file, in order. FormatError for a bad block number in the chain or a chain shorter than the size.
    std::vector<std::uint32_t> chain(const File& file) const;
    // The bytes of a file. FormatError as for `chain`, and for a name that is not a file.
    Bytes read(std::string_view name) const;

private:
    Flash() = default;
    ByteView page(std::size_t number) const;
    ByteView spare(std::size_t block) const;

    Bytes raw_;
    std::uint32_t root_ = 0;
    std::vector<std::uint16_t> alloc_;
    std::vector<File> files_;
    std::unordered_map<std::string, std::size_t> index_;   // name -> position in files_ (lookup only)
    std::unordered_map<std::uint32_t, std::uint32_t> by_id_;   // logical block -> the physical block that carries it
};

}  // namespace cb::formats::xbox::nand
