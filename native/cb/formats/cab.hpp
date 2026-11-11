// cb/formats/cab.hpp -- Microsoft cabinet (MSCF) files: list, extract and write, with no outside tool.
//
// A cabinet holds FOLDERS (one compressed stream each, cut into CFDATA blocks of at most 32768 uncompressed bytes)
// and FILES (a name plus an offset and size inside one folder's stream). A folder may run on into the next cabinet of
// a set: a file whose folder index is 0xFFFD is continued FROM the previous cabinet, 0xFFFE continues INTO the next
// one, 0xFFFF both; the cabinets name their neighbours (prev/next) and are looked up in the same directory.
//
// Decompression is done for stored (NONE), MSZIP (zlib) and QUANTUM (written from the published description of the
// format). LZX cabinets are listed but not extracted (UnsupportedError). Block checksums are verified where a cabinet
// carries them.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::cab {

// What a file's folder index can say besides a folder number.
inline constexpr std::uint16_t kFromPrev = 0xFFFD;      // continued from the previous cabinet
inline constexpr std::uint16_t kToNext = 0xFFFE;        // continues into the next cabinet
inline constexpr std::uint16_t kPrevAndNext = 0xFFFF;   // both

// Blocks hold 32768 uncompressed bytes (all but the last of a folder). Microsoft's decoder and 7-Zip count on that:
// each MSZIP block is inflated at the START of a 32768-byte window that still holds the block before it, so a block's
// back-references only land where the packer meant when the block before it filled the window exactly.
inline constexpr std::size_t kBlock = 32768;
// The one exception made when WRITING. A CFDATA payload must not exceed 32768 bytes: the MSZIP decompressor in
// Windows 95 SETUP reads the payload into a fixed 32768-byte buffer and overruns on anything larger ("corrupt Setup
// (.CAB) file"). A full block of incompressible data cannot meet that (deflate's stored form adds 5 bytes, the "CK"
// signature 2), so such a block is cut at 32756 bytes and stored.
inline constexpr std::size_t kShortBlock = 32756;

enum class Compression : std::uint8_t { None = 0, MsZip = 1, Quantum = 2, Lzx = 3 };

// "NONE", "MSZIP", "QUANTUM", "LZX"; "?<n>" for a method number this reader does not know.
std::string compression_name(std::uint16_t type);

// A neighbouring cabinet of a set, as the header names it.
struct SetLink {
    std::string cabinet;
    std::string disk;   // the disk label, often empty
};

struct FileEntry {
    std::string name;             // as stored, with backslashes (UTF-8 when the entry says so, else latin-1 decoded)
    std::uint32_t size = 0;
    std::uint16_t folder = 0;     // a folder number, or one of the continuation markers above
    std::uint32_t offset = 0;     // inside the folder's uncompressed stream
    std::uint16_t date = 0;       // MS-DOS: years since 1980 << 9 | month << 5 | day
    std::uint16_t time = 0;       // MS-DOS: hour << 11 | minute << 5 | second / 2
    std::uint16_t attributes = 0;
};

struct FolderEntry {
    std::uint32_t data_offset = 0;   // of the folder's first CFDATA block in the file
    std::uint16_t block_count = 0;
    std::uint16_t type = 0;          // low nibble: Compression; for QUANTUM bits 8-12 are the window's size (a power of two)
    std::string compression() const { return compression_name(type & 15); }
};

// The headers of one cabinet (nothing is decompressed).
struct Cabinet {
    std::filesystem::path path;            // where it was read from; neighbours are looked up beside it
    std::uint32_t size = 0;                // as the header says
    std::uint16_t set_id = 0;
    std::uint16_t index = 0;               // this cabinet's number in its set
    std::uint16_t flags = 0;
    std::uint8_t version_major = 1;
    std::uint8_t version_minor = 3;
    std::optional<SetLink> prev;
    std::optional<SetLink> next;
    std::uint8_t block_reserve = 0;        // bytes reserved in every CFDATA block
    std::vector<FolderEntry> folders;
    std::vector<FileEntry> files;

    // The real folder number of a file (the continuation markers name the first or the last folder of the cabinet).
    std::size_t folder_of(const FileEntry& file) const;
    // The compression name of the folder a file lives in, or "?" if its folder number is out of range.
    std::string compression_of(const FileEntry& file) const;
};

// FormatError if it is not a cabinet (or its headers are inconsistent), TruncatedError if they are cut short.
Cabinet parse(ByteView data, const std::filesystem::path& path = {});
Cabinet read(const std::filesystem::path& path);

// The CFDATA checksum: 32-bit little-endian words of the block header (two sizes) and the payload XORed together; the
// 1-3 odd bytes at the end count most-significant first.
std::uint32_t checksum(ByteView payload, std::uint16_t uncompressed);

// The decompressed stream of folder `folder` of `cab`, at least `upto` bytes of it when given (it stops at the block
// that reaches that far). A folder that is continued from the previous cabinet is read from there, and one that
// continues into the next is followed, both looked up beside `cab` (NotFoundError names the missing cabinet).
Bytes folder_stream(const Cabinet& cab, std::size_t folder, std::optional<std::uint64_t> upto = std::nullopt);

struct Extracted {
    std::string name;                // as in the cabinet
    std::filesystem::path path;      // where it was written
};

// Extract the files of one cabinet (all, or those whose names are in `names`, compared without regard to case) into
// `outdir`, creating folders for names with a path, setting each file's time from the cabinet. Neighbouring cabinets
// are read as needed for files whose folder is continued. A name that would leave `outdir` (absolute, `..`, a drive)
// is refused (FormatError). Returned in the order extracted; a name met twice is listed once, with the later path.
std::vector<Extracted> extract(const std::filesystem::path& cabinet, const std::filesystem::path& outdir,
                               const std::optional<std::vector<std::string>>& names = std::nullopt);

struct SourceFile {
    std::string name;      // as stored in the cabinet (UTF-8; backslash-separated paths)
    ByteView data;
};

struct DosTimestamp {
    std::uint16_t date = 0;
    std::uint16_t time = 0;
};
// The local time now, in the cabinet's form.
DosTimestamp dos_timestamp_now();

struct WriteOptions {
    Compression compression = Compression::MsZip;   // MsZip or None
    // The fields that tie cabinets into a set; a program that walks the set, as Windows 95 SETUP does, needs them carried
    // over when a cabinet of a set is rebuilt.
    std::uint16_t set_id = 0;
    std::uint16_t index = 0;
    std::optional<SetLink> prev;
    std::optional<SetLink> next;
    // Keep every MSZIP data block within what Windows 95 SETUP can take (kShortBlock), at the price of 7-Zip rejecting
    // cabinets that hold incompressible data; false for a cabinet meant for present-day tools only. A block that will
    // not compress to a 32768-byte payload is written short and stored; the block after a short one is deflated with no
    // dictionary, so it is read the same by every decoder.
    bool short_blocks = true;
    std::optional<DosTimestamp> timestamp;           // for every file; the current time if not given
};

// A cabinet holding `files` in one folder: a valid cabinet delivering the same files, not a byte copy of any original.
// UnsupportedError if it would be too much for one folder (over 65535 files or blocks, or a stored file past 4 GB).
Bytes build(std::span<const SourceFile> files, const WriteOptions& options = {});
// The same, written to `out_path`; returns the size.
std::uint64_t write(std::span<const SourceFile> files, const std::filesystem::path& out_path,
                    const WriteOptions& options = {});

}  // namespace cb::formats::cab
