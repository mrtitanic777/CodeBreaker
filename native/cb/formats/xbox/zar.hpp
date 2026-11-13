// cb/formats/xbox/zar.hpp -- ZArchive (.zar) files, read as a tree of files: the compressed archive format Cemu's Wii U titles ship in.
//
//     zar::Reader archive = zar::Reader::open("game.zar");
//     for (const zar::FileInfo& f : archive.files()) ...     every file with its size
//     Bytes data = archive.read("content/meta/meta.xml");    a file by path, ASCII case ignored
//
// The format is the ZArchive project's (MIT-0, Exzap/ZArchive: zarchivecommon.h, zarchivereader.cpp); this reads it as the project's
// reader does. All integers are big-endian.
//
//   [compressed data]   the virtual stream cut into 64 KiB blocks, each stored zstd-compressed, or raw when the compressed form would
//                       not be smaller (a stored size of 65536 means raw)
//   [offset records]    {u64 base, u16 size[16]} per 16 blocks, size = compressed size - 1; a block's place is the base plus the
//                       sizes before it in its record
//   [name table]        names with a 1-byte length (0..0x7F), or 2 bytes when the first byte's top bit is set (length = low 7 bits of
//                       the first | second << 7)
//   [file tree]         16-byte entries {u32 name offset (top bit set: a file), 3 words}: a file's are its offset and size low words
//                       and, in the third, their high 16 bits each (48-bit offset and size); a directory's are the index of its first
//                       child, its child count, and a reserved word. Entry 0 is the root.
//   [footer, 144 bytes] six {u64 offset, u64 size} for those sections and the two metadata ones, a SHA-256 of everything before the
//                       footer (with the footer's hash field zeroed after it), the total size, the version 0x61bf3a01, the magic
//                       0x169f52d6
//
// zstd is not part of the repository. Decompressing a block goes through `ZstdCodec`; the default is libzstd, found as `libzstd.dll`
// or `zstd.dll` beside the program (see DynLib), in the folder the environment variable CODEBREAKER_ZSTD_DLL names, or on the
// path. An archive whose blocks are all stored raw reads without it; a compressed block with no codec is UnsupportedError.
//
// Not here: writing archives and the integrity check, which the Python module has (its writer needs zstd compression too).
//
// Errors: FormatError for a structure that does not hold together (bad magic or version, a size that is not the file's, a block outside
// the data, a directory loop, an entry with no name), NotFoundError for a path that is not in the archive.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <list>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/random_file.hpp"

namespace cb::formats::xbox::zar {

inline constexpr std::size_t kBlockSize = 64 * 1024;

// Decompression of one zstd frame.
class ZstdCodec {
public:
    virtual ~ZstdCodec() = default;
    // The frame's content, which is `expected_size` bytes; FormatError for a frame that is damaged or another size.
    virtual Bytes decompress(ByteView frame, std::size_t expected_size) = 0;
};

// libzstd, loaded as described above; nullptr when it cannot be found.
std::shared_ptr<ZstdCodec> load_zstd();

struct FileInfo {
    std::string path;     // folders joined with '/'
    std::uint64_t size = 0;
};

class Reader {
public:
    // Opens and checks the archive's footer, offset records, name table and file tree. `codec` is used for compressed blocks; when it
    // is null `load_zstd()` is tried the first time one is needed.
    static Reader open(const std::filesystem::path& path, std::shared_ptr<ZstdCodec> codec = nullptr);

    Reader(Reader&&) noexcept = default;
    Reader& operator=(Reader&&) noexcept = default;

    // Every file, folder by folder in the order the tree stores them: a folder's own children first, then each subfolder's (the
    // order the reference tool lists them in).
    std::vector<FileInfo> files();
    // The folders (the root left out), in the same order.
    std::vector<std::string> directories();
    // A file's size; NotFoundError for a path that is not a file.
    std::uint64_t size(std::string_view path);
    // A file's bytes.
    Bytes read(std::string_view path);

private:
    struct Node {
        std::string path;
        std::size_t index = 0;
        bool is_file = false;
    };
    explicit Reader(fs::RandomFile file) : file_(std::move(file)) {}
    std::string name_at(std::uint32_t offset) const;
    std::vector<Node> walk() const;
    std::size_t lookup(std::string_view path) const;
    const Bytes& block(std::uint64_t index);
    Bytes read_range(std::uint64_t offset, std::uint64_t length);

    fs::RandomFile file_;
    std::shared_ptr<ZstdCodec> codec_;
    bool tried_codec_ = false;
    std::uint64_t data_offset_ = 0, data_size_ = 0;
    std::vector<std::uint64_t> block_offset_, block_size_;
    Bytes names_;
    std::vector<std::array<std::uint32_t, 4>> tree_;
    std::list<std::pair<std::uint64_t, Bytes>> cache_;   // the blocks last used, newest first
    std::unordered_map<std::uint64_t, std::list<std::pair<std::uint64_t, Bytes>>::iterator> cached_;   // lookup only
};

}  // namespace cb::formats::xbox::zar
