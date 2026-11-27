// formats/zip.hpp -- read a .zip/.jar/.appx archive: list its entries and extract them; write a stored one.
//
// Reads the central directory of an archive (stored and deflated entries; no encryption, no other compression methods)
// and checks every extracted entry against its CRC-32. A jar is read into memory once when it is opened, so listing and
// extracting do not touch the disk again; a package that is already in memory (`view`) is read where it lies. zip64 is
// refused unless the reader is asked for it (`Options::zip64`), and a package whose writer left the CRCs out is read with
// `Options::check_crc` off.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats {

class ZipArchive {
public:
    // One entry of the central directory.
    struct Entry {
        std::string name;                 // UTF-8; any backslash is a '/'; cut at a NUL
        std::string stored_name;          // as written in the directory (for the cross-check with the entry's header)
        std::uint16_t flags = 0;
        std::uint16_t method = 0;         // 0 stored, 8 deflated
        std::uint32_t crc = 0;
        std::uint64_t compressed_size = 0;
        std::uint64_t size = 0;
        std::uint64_t header_offset = 0;
        bool is_directory() const { return !name.empty() && name.back() == '/'; }
    };

    struct Options {
        bool zip64 = false;       // read zip64 archives (else UnsupportedError)
        bool check_crc = true;    // compare every extracted entry with its CRC-32
    };

    // FormatError when the file is not a zip (no end-of-central-directory record, a damaged directory);
    // UnsupportedError for zip64.
    static ZipArchive open(const std::filesystem::path& path);
    static ZipArchive from_bytes(Bytes data, std::string name = "zip", Options options = {});
    // The archive in `data`, which must outlive the ZipArchive and everything read through it.
    static ZipArchive view(ByteView data, std::string name = "zip", Options options = {});

    // The entries in directory order (directories included).
    const std::vector<Entry>& entries() const { return entries_; }

    // The entry of that name; when a name occurs twice, the later one (as an extractor would).
    const Entry* find(std::string_view name) const;

    // The entry's contents. FormatError for a damaged entry or a bad CRC; UnsupportedError for an encrypted entry or
    // a compression method other than stored/deflated.
    Bytes read(const Entry& entry) const;

    // read(*find(name)); FormatError if there is no such entry.
    Bytes read(std::string_view name) const;

private:
    static ZipArchive parse(ZipArchive z);
    ByteView data() const { return owned_.empty() ? borrowed_ : ByteView(owned_); }

    Bytes owned_;
    ByteView borrowed_;
    Options options_;
    std::string name_;
    std::vector<Entry> entries_;
    std::map<std::string, std::size_t, std::less<>> by_name_;
};

// Write a .zip/.jar: entries added in order, each stored (not compressed) with its CRC-32; a name ending in '/' is a directory.
//
//     ZipWriter z;
//     z.add("META-INF/", {});
//     z.add("a/B.class", bytes);
//     fs::write_file(path, z.finish());
class ZipWriter {
public:
    void add(const std::string& name, ByteView data);
    Bytes finish();   // the archive (the writer is left empty)

private:
    struct Written {
        std::string name;
        std::uint32_t crc = 0, size = 0, offset = 0;
    };
    Bytes out_;
    std::vector<Written> written_;
};

}  // namespace cb::formats
