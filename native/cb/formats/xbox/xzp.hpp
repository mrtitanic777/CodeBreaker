// cb/formats/xbox/xzp.hpp -- XUIZ (.xzp) containers: the Xbox 360 UI and texture packs (Minecraft Legacy Console Edition's, and the
// dashboard's).
//
//     xzp::Pack pack = xzp::parse(bytes);               the entry table
//     for (const xzp::Payload& p : xzp::payloads(bytes)) ...   every stored file under a safe relative path
//     Bytes made = xzp::build(folder, template_pack);   a folder packed as a v1 container
//
// Two layouts share the magic 'XUIZ', told apart by the version word at 0x04. Both put the payload base at 0x16 plus the word at
// 0x10; entry offsets are from that base.
//
//   v1 (Minecraft LCE)        table at 0x17 (the format's one byte of skew), entries until the base: {u24be size, u32be offset,
//                             u16le name length in UTF-16 units, the name in UTF-16LE}; the count is not stored.
//   v3 (dashboard / XUI)      table at 0x16, a u16be entry count at 0x14: {u32be size, u32be offset, u8 name length, Latin-1 name}.
//
// v3 names carry backslash paths with a leading `..\`; `payloads` drops the empty, `.` and `..` parts and any with a colon, so an
// extraction cannot leave its folder. A v1 table ends at the first entry that does not hold together (no name, a name over 512 units,
// bytes that run past the file).
//
// Errors: FormatError when the file is not XUIZ or its header is cut short.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::xbox::xzp {

struct Entry {
    std::uint32_t size = 0;
    std::uint32_t offset = 0;   // from `Pack::base`
    std::string name;           // printable ASCII for v1; Latin-1 decoded for v3
};

struct Pack {
    int version = 0;            // 1 or 3 (a later one is read as 3)
    std::uint32_t file_size = 0;   // what the header says
    std::uint64_t base = 0;
    std::size_t table_end = 0;
    std::vector<Entry> entries;
};

Pack parse(ByteView file);

// One stored file: its safe relative path ('/' between folders) and where its bytes are. A payload that the file cuts short is as long
// as the file holds.
struct Payload {
    std::string path;
    std::size_t offset = 0;
    std::size_t size = 0;
};
std::vector<Payload> payloads(ByteView file);

// Pack every file below `folder` as a v1 (Minecraft) container. Each is named by its path from `folder` with `\` between the parts (so
// packing a folder `XZP` that holds `media/skin.xur` gives the entry `media\skin.xur` the game loads) and the entries are in the order of
// those names lowercased. The first 0x17 bytes of `template_pack` are the header, so the fields no one has a name for are kept; the
// payload base and the file size are made anew. FormatError when the template is not a XUIZ container.
Bytes build(const std::filesystem::path& folder, ByteView template_pack);

}  // namespace cb::formats::xbox::xzp
