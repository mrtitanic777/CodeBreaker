// core/fs.hpp -- what std::filesystem lacks: whole-file I/O with CodeBreaker's errors, memory mapping,
// scoped temp directories and environment changes, tree copy/removal, and UTF-8 <-> path conversion.
//
// Paths are std::filesystem::path internally and joined with `/`. Text crosses to and from paths with
// to_text / from_text (UTF-8). Every file error is a cb::IoError (NotFoundError / AccessError for the two
// common cases); nothing outside this file and process.cpp touches FILE* or ifstream.
// See docs/CPP_DESIGN.md 3.9.
#pragma once

#include <chrono>
#include <filesystem>
#include <system_error>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/error.hpp"

namespace cb::fs {

namespace stdfs = std::filesystem;

// Path <-> UTF-8 text. to_text uses '/' separators.
std::string to_text(const stdfs::path& p);
stdfs::path from_text(std::string_view utf8);

// Prefix an absolute path with \\?\ so that it may exceed MAX_PATH (260 characters). A path that already has
// the prefix, or is relative, is returned made absolute first.
stdfs::path long_path(const stdfs::path& p);

// An existing path with its 8.3 short names spelled out (C:/Users/RUNNER~1 becomes C:/Users/runneradmin), as Windows
// itself names it; a path that does not exist is returned as it is. The temporary folder is often given short.
stdfs::path full_names(const stdfs::path& p);

// Windows path equality: case-insensitive, either separator, redundant parts normalised.
bool same_path(const stdfs::path& a, const stdfs::path& b);

// Whole-file reads. NotFoundError / AccessError / IoError on failure.
Bytes read_file(const stdfs::path& p);
// The bytes of the file as text (no decoding; a UTF-8 BOM is kept).
std::string read_text(const stdfs::path& p);

// Whole-file writes; missing parent directories are created. The file is truncated if it exists.
void write_file(const stdfs::path& p, ByteView data);
void write_text(const stdfs::path& p, std::string_view text);
// Append to the file (created, with its parent directories, if it does not exist).
void append_file(const stdfs::path& p, ByteView data);
// Write to a temporary file in the same directory, then rename over the target: readers see the old file or the
// new one, never a half-written one.
void write_file_atomic(const stdfs::path& p, ByteView data);

// Create the directory and its parents; no error if it exists.
void make_dirs(const stdfs::path& p);
// Size of a regular file in bytes.
std::uint64_t file_size(const stdfs::path& p);

// A file's or directory's last-write time, to the 100 ns the file system keeps (the standard library may round it
// to a second, or refuse to set a directory's time). The getter gives the minimum time with ec set on failure;
// the throwing setter raises IoError.
stdfs::file_time_type last_write_time(const stdfs::path& p, std::error_code& ec) noexcept;
stdfs::file_time_type last_write_time(const stdfs::path& p);
void set_last_write_time(const stdfs::path& p, stdfs::file_time_type when, std::error_code& ec) noexcept;
void set_last_write_time(const stdfs::path& p, stdfs::file_time_type when);

// Copy a directory tree (files overwritten). The destination is created.
void copy_tree(const stdfs::path& from, const stdfs::path& to);
// Delete a file or a whole tree, also clearing read-only attributes. Returns the number of entries removed
// (0 if it did not exist).
std::uint64_t remove_tree(const stdfs::path& p);

// A read-only view of a whole file, mapped into memory (for ISOs, packages, big inputs). Empty files are
// allowed and give an empty view.
class MappedFile {
public:
    explicit MappedFile(const stdfs::path& p);
    MappedFile(MappedFile&& other) noexcept;
    MappedFile& operator=(MappedFile&& other) noexcept;
    MappedFile(const MappedFile&) = delete;
    MappedFile& operator=(const MappedFile&) = delete;
    ~MappedFile();
    ByteView view() const { return {data_, size_}; }
    operator ByteView() const { return view(); }   // NOLINT: a MappedFile is a ByteView where one is wanted

private:
    void close() noexcept;
    const std::uint8_t* data_ = nullptr;
    std::size_t size_ = 0;
    void* file_ = nullptr;      // HANDLEs, kept opaque so that this header does not include <windows.h>
    void* mapping_ = nullptr;
};

// A directory that exists for the life of the object and is deleted, with everything in it, when the object
// goes out of scope -- including by an exception. Harnesses use this so that they stop leaking temp folders.
class ScopedTempDir {
public:
    // Creates <base>/<prefix>-<unique>. `base` defaults to the system temp directory.
    explicit ScopedTempDir(std::string_view prefix = "cb", const std::optional<stdfs::path>& base = std::nullopt);
    ScopedTempDir(ScopedTempDir&& other) noexcept;
    ScopedTempDir& operator=(ScopedTempDir&& other) noexcept;
    ScopedTempDir(const ScopedTempDir&) = delete;
    ScopedTempDir& operator=(const ScopedTempDir&) = delete;
    ~ScopedTempDir();
    const stdfs::path& path() const { return path_; }
    // Keep the directory (a failing run you want to look at); returns its path.
    stdfs::path release();

private:
    stdfs::path path_;
};

// Set (or unset) an environment variable for a scope and restore the previous state afterwards.
class ScopedEnv {
public:
    ScopedEnv(std::string_view name, std::optional<std::string_view> value);
    ScopedEnv(const ScopedEnv&) = delete;
    ScopedEnv& operator=(const ScopedEnv&) = delete;
    ~ScopedEnv();

private:
    std::string name_;
    std::optional<std::string> previous_;
};

// The value of an environment variable, if set.
std::optional<std::string> get_env(std::string_view name);

// Set (or, with nullopt, unset) an environment variable of this process: what children inherit and what getenv sees.
void set_env(std::string_view name, std::optional<std::string_view> value);

// The operating system's number for this process.
std::uint32_t process_id();
// A counter of the finest resolution the system has, for names and timing (the unit is the system's; only differences mean something).
std::uint64_t monotonic_ticks();
// A name part that no other call, in this process or another, makes: <pid>-<ticks>-<n>, in hex.
std::string unique_suffix();
// The running program's file (empty if the system will not say).
stdfs::path executable_path();
// Set or clear the read-only attribute of a file (on POSIX: the owner's write permission).
void set_read_only(const stdfs::path& p, bool read_only);

}  // namespace cb::fs
