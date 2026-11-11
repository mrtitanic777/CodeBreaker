// cb/java/console/zip_archive.hpp -- read a .zip/.jar archive: list its entries and extract them.
//
// Reads the central directory of a jar (stored and deflated entries; no zip64, no encryption, no other compression
// methods -- a jar uses neither) and checks every extracted entry against its CRC-32. The archive is read into memory
// once when it is opened, so listing and extracting do not touch the disk again.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::java::console {

class ZipArchive {
public:
    // One entry of the central directory.
    struct Entry {
        std::string name;                 // UTF-8; any backslash is a '/'; cut at a NUL
        std::string stored_name;          // as written in the directory (for the cross-check with the entry's header)
        std::uint16_t flags = 0;
        std::uint16_t method = 0;         // 0 stored, 8 deflated
        std::uint32_t crc = 0;
        std::uint32_t compressed_size = 0;
        std::uint32_t size = 0;
        std::uint64_t header_offset = 0;
        bool is_directory() const { return !name.empty() && name.back() == '/'; }
    };

    // FormatError when the file is not a zip (no end-of-central-directory record, a damaged directory);
    // UnsupportedError for zip64.
    static ZipArchive open(const std::filesystem::path& path);
    static ZipArchive from_bytes(Bytes data, std::string name = "zip");

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
    Bytes data_;
    std::string name_;
    std::vector<Entry> entries_;
    std::map<std::string, std::size_t, std::less<>> by_name_;
};

}  // namespace cb::java::console
