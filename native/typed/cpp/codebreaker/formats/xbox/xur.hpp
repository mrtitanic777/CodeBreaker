// codebreaker/formats/xbox/xur.hpp -- typed C++ from codebreaker.formats.xbox.xur.
//
// XUR (compiled XUI) reader -- Xbox 360 dashboard / game UI scenes.
//
// XUR is the binary form of an XUI scene (the dashboard/game UI markup). This
// parses the XUR5 container -- the big-endian header and section table -- and pulls
// the human-readable content out of the string-table (STRN) section: element
// names, texture paths, and on-screen text a scene references.
//
// Layout (clean-room from the documented format / XUIHelper by SGCSam):
//   header (20 bytes, BE):  magic 'XUIB', version(=5), flags, toolVersion(u16),
//                           fileSize, sectionCount(u16)
//   section table:          sectionCount x { magic(u32), offset(u32), length(u32) }
//   section data:           based at 20 + sectionCount*12
//   STRN section:           repeated { u16 length, UTF-16BE chars[length] }
//
// This reads the container and its strings; it does not (yet) rebuild the full
// object tree back into XUI XML -- that needs the per-control property schema.
//
// Commands:
//     xur info    <file.xur>      header + section table (magics, offsets, sizes)
//     xur strings <file.xur>      the STRN string table (names / paths / text)
#pragma once

#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xbox::xur {

inline constexpr int64_t MAGIC = 0x58554942;   // 'XUIB'
inline constexpr int64_t STRN = 0x5354524E;    // 'STRN'

py::str _tag(int64_t v);

class XUR {
  public:
    py::bytes d;
    int64_t version = 0;
    int64_t flags = 0;
    int64_t tool_version = 0;
    int64_t file_size = 0;
    int64_t section_count = 0;
    py::list<std::tuple<int64_t, int64_t, int64_t>> sections;   // (magic, offset, length)
    int64_t data_base = 0;

    explicit XUR(const py::bytes& data);
    std::optional<py::bytes> section_data(int64_t magic) const;
    py::list<py::str> strings() const;
};

// Serialise a STRN section body (for tests / round-trips).
py::bytes build_strn(const py::list<py::str>& strings);

// Re-emit an XUR with its STRN string table replaced. Rebuilds only the
// STRN body and shifts every later section by the size delta (preserving the
// original layout of all other sections), fixing the section table offsets/
// lengths and the header file size. Byte-identical to the input when the
// strings are unchanged.
py::bytes set_strings(const py::bytes& data, const py::list<py::str>& new_strings);

// Replace a single string (by index) and re-emit the XUR.
py::bytes set_string(const py::bytes& data, int64_t index, const py::str& text);

extern const py::str DESCRIPTION;

// main(): the command line without the program name (sys.argv[1:]); prints to stdout.
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::xur
