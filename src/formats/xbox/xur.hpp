// formats/xbox/xur.hpp -- XUR, the compiled form of an Xbox 360 XUI scene (the dashboard's and games' UI): its container and its strings.
//
//     xur::Scene s = xur::Scene::parse(bytes);
//     s.strings();                                        element names, texture paths, on-screen text
//     Bytes edited = xur::set_string(bytes, 2, "Press START");
//
// The XUR5 container, big-endian: a 20-byte header {magic "XUIB", version, flags, u16 tool version, u32 file size, u16 section count}, a table of
// sections {u32 tag, u32 offset, u32 length}, and the section data: each section is at its offset from the START OF THE FILE (the first begins where the
// table ends, 20 + 12 * count, and the rest follow end to end). The STRN section is a run of {u16 length in UTF-16 units, UTF-16BE characters}. This reads the container and its strings; it does not rebuild the
// object tree into XUI XML (that needs the per-control property schema).
//
// `set_strings` re-emits a scene with its string table replaced: only STRN's body is rebuilt, every later section moves by the size change, the
// section table and the header's file size are fixed up, and the rest of the layout is kept. It is byte-identical to the input when the strings
// are unchanged.
//
// Errors: FormatError (TruncatedError) for bytes that are not a scene or whose section table or data lies outside the file; UsageError for a
// string index out of range, or a scene without STRN to edit.
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"

namespace cb::formats::xbox::xur {

inline constexpr std::uint32_t kMagic = 0x58554942;   // 'XUIB'
inline constexpr std::uint32_t kStrn = 0x5354524E;    // 'STRN'
inline constexpr std::size_t kHeaderSize = 20;
inline constexpr std::size_t kSectionEntrySize = 12;

// A section's tag as the four characters it spells ("STRN").
std::string tag_text(std::uint32_t tag);

struct Section {
    std::uint32_t tag = 0, offset = 0, length = 0;
};

class Scene {
public:
    static Scene parse(ByteView file);

    std::uint32_t version = 0, flags = 0;
    std::uint16_t tool_version = 0;
    std::uint32_t file_size = 0;   // as the header declares it
    std::vector<Section> sections;

    // Where the section table ends (and the first section's data begins).
    std::size_t table_end() const { return kHeaderSize + sections.size() * kSectionEntrySize; }
    // The data of the first section with this tag; nullopt when there is none. FormatError when it runs past the file.
    std::optional<ByteView> section_data(std::uint32_t tag) const;
    // The STRN table's strings in order; empty without one.
    std::vector<std::string> strings() const;

private:
    Bytes file_;
};

// A STRN section body for these strings.
Bytes build_strn(std::span<const std::string> strings);
// The scene with its string table replaced.
Bytes set_strings(ByteView file, std::span<const std::string> strings);
// The scene with string `index` replaced.
Bytes set_string(ByteView file, std::size_t index, std::string_view text);

// `xur info|strings|set <file.xur> ...`: the module's command line.
cli::Command command();

}  // namespace cb::formats::xbox::xur
