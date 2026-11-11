// cb/exact/object.hpp -- the object-file record the exact lanes share: what an OMF-86 object module or a Version 7
// a.out object (XENIX's C compiler) holds, in one typed shape.
//
// The linker emulation (linkemu), the function comparer (compare) and the a.out reader (aout) all speak this record.
// The OMF reader (formats/omf) will produce it when that module is refactored; until then a shim converts the old
// reader's result into it. Names are UTF-8 text decoded from the file's Latin-1 bytes; segment bytes stay Bytes.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::exact {

// Where the object came from. An a.out object keeps its instructions and data apart (each piece begins at an even
// address when linked), which the linker emulation needs to know.
enum class ObjectKind { Omf, Aout };

// What a fix-up word is. `Lo` is the OMF "low byte" location; the comparers hold no opinion about it.
enum class FixKind { Off, Base, Ptr, Lo, Rel };

// What a fix-up names: a segment's start, a group, or an external symbol.
enum class TargetKind { Segment, Group, External };

struct Target {
    TargetKind kind = TargetKind::Segment;
    std::string name;   // segment, group or symbol name; an a.out word naming nothing known is Segment "?"
    friend bool operator==(const Target&, const Target&) = default;
};

// One segment definition, in the order the object defines them.
struct SegDef {
    std::string name;
    std::string cls;               // "CODE", "DATA", "BSS", "STACK" ...
    std::uint32_t length = 0;
    std::uint32_t align = 1;       // the OMF alignment code: 0/1 byte, 2 word, 3 paragraph, 4 page, 5 dword
};

// A word of a segment the linker fills in. `displacement` is what OMF calls the fix-up's displacement (for an a.out
// object it is set so a relative word counts as an OMF self-relative fix-up does).
struct Fixup {
    std::string segment;
    std::uint32_t offset = 0;
    FixKind kind = FixKind::Off;
    Target target;
    std::int32_t displacement = 0;
};

struct Public {
    std::string name;
    std::optional<std::string> segment;   // absent: an absolute symbol (OMF only)
    std::uint32_t offset = 0;
};

// A communal variable: an external with a size.
struct CommonVar {
    std::uint32_t size = 0;
    bool far = false;
};

struct StartAddress {
    std::string segment;
    std::uint32_t offset = 0;
};

struct ObjectFile {
    ObjectKind kind = ObjectKind::Omf;
    // Segment bytes (LEDATA and LIDATA laid out); only the first `filled[name]` bytes are the object's own.
    OrderedMap<std::string, Bytes> seg;
    std::vector<SegDef> segs;
    OrderedMap<std::string, std::uint32_t> filled;
    OrderedMap<std::string, std::vector<std::string>> groups;   // group -> its segments, in the group's order
    std::vector<Public> pubs;
    std::vector<std::string> ext;
    OrderedMap<std::string, CommonVar> comm;
    std::vector<Fixup> fix;
    OrderedMap<std::string, std::uint8_t> combine;               // segment -> OMF combination (5: stack); OMF only
    std::optional<StartAddress> start;                           // OMF only
    Bytes raw;
};

}  // namespace cb::exact
