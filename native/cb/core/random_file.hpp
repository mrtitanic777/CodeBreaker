// cb/core/random_file.hpp -- a file read and written at offsets: disk images, partitions, anything too big to map or read
// whole, and anything patched in place.
//
// MappedFile (fs.hpp) is the read-only whole-file view and read_file/write_file the whole-file forms; this is the third
// shape, for a file used like a disk. Errors are IoError (NotFoundError / AccessError for the two common cases), like
// the rest of cb::fs.
#pragma once

#include <cstdint>
#include <filesystem>

#include "cb/core/bytes.hpp"

namespace cb::fs {

enum class FileMode {
    Read,        // an existing file, read only
    ReadWrite,   // an existing file, read and written in place
    Create,      // a new file (an existing one is emptied; missing parent folders are made), written
};

class RandomFile {
public:
    RandomFile(const std::filesystem::path& path, FileMode mode);
    RandomFile(RandomFile&& other) noexcept;
    RandomFile& operator=(RandomFile&& other) noexcept;
    RandomFile(const RandomFile&) = delete;
    RandomFile& operator=(const RandomFile&) = delete;
    ~RandomFile();

    const std::filesystem::path& path() const { return path_; }
    FileMode mode() const { return mode_; }
    std::uint64_t size() const;

    // Up to `count` bytes at `offset`: fewer if the file ends first (none at or past the end).
    Bytes read_at(std::uint64_t offset, std::size_t count) const;
    // Write at `offset`; the file grows (zero-filled between) if the write goes past its end. IoError if the file is not
    // open for writing.
    void write_at(std::uint64_t offset, ByteView data);
    void flush();

private:
    void close() noexcept;
    std::filesystem::path path_;
    FileMode mode_;
    void* handle_ = nullptr;   // a HANDLE, kept opaque so that this header does not include <windows.h>
};

}  // namespace cb::fs
