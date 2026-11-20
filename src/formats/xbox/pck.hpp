// formats/xbox/pck.hpp -- Minecraft Legacy Console Edition .pck containers (PNG, localisation, grf and colour payloads).
//
//     pck::Archive a = pck::parse(bytes);           the properties and the entry table
//     pck::Payloads p = pck::payloads(bytes);       where each payload is, and where reading stopped
//
// Layout, all big-endian: a version and a count of properties; each property is {u32 id, u32 length, UTF-16BE name, u32 zero}; a
// count of entries; each entry is {u32 size, u32 type, u32 length, UTF-16BE name, u32 zero}; then the payloads. The sizes in the
// table are exact, but the payloads are separated by padding of varying length (4, 16 and 316 bytes seen), so the payloads are not
// placed by arithmetic: each is looked for from the end of the one before -- a PNG by its signature (it is self-delimiting), any other
// by skipping zero padding.
//
// Errors: TruncatedError / FormatError for a table that runs past the end of the file or has a name longer than 8192 characters.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::xbox::pck {

struct Entry {
    std::uint32_t size = 0;
    std::uint32_t type = 0;
    std::string name;
};

struct Archive {
    std::uint32_t version = 0;
    std::vector<std::pair<std::uint32_t, std::string>> properties;   // id, name
    std::vector<Entry> entries;
    std::size_t table_end = 0;   // where the entry table stops
};

Archive parse(ByteView file);

// One payload found: its file-safe name ('/' and '\\' made '_', NULs trimmed; "entryN" when nothing is left) and where its bytes are
// in the file. A payload the file cuts short is as long as the file holds.
struct Payload {
    std::string name;
    std::size_t offset = 0;
    std::size_t size = 0;
};

struct Payloads {
    std::vector<Payload> found;
    std::size_t end = 0;           // where reading stopped
    std::size_t file_size = 0;
};

Payloads payloads(ByteView file);

}  // namespace cb::formats::xbox::pck
