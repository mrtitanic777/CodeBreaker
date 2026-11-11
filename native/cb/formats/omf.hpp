// cb/formats/omf.hpp -- Intel/Microsoft OMF-86 object modules and libraries: the 16-bit records and the 32-bit ones a
// compiler of the period writes.
//
// One module is read whole (`read`: segments with their bytes, publics, externals, fix-ups, groups ...), a library is
// cut into its modules (`library`), the records can be walked (`records`, `walk`), and the few kinds of record the
// exact-recovery lanes write are written here too (`record`, `name`, `raw_object`, `bss_object`, `rename_*`,
// `patched`). Names are decoded from latin1 to UTF-8 at the reader.
//
// An a.out object (XENIX's compiler) is not OMF: `read` throws UnsupportedError for one, and `is_aout` tells.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::formats::omf {

// One record as stored: type and payload (without the checksum). The payload points into the data that was read.
struct Record {
    std::uint8_t type = 0;
    ByteView payload;
};

// A fix-up's target: a segment or group by name, an external by name, or something the reader cannot name.
struct Target {
    enum class Kind { Segment, Group, External, Unknown };
    Kind kind = Kind::Unknown;
    std::optional<std::string> name;
    std::optional<std::int64_t> number;  // a group that has no name: its index
};

// What the linker fills in at a fix-up, by its location field.
enum class FixupKind { Offset, Base, Pointer, LowByte, Relative, Other };
struct Fixup {
    std::string segment;  // the segment whose data the fix-up lies in
    std::int64_t at = 0;  // its offset in that segment
    FixupKind kind = FixupKind::Other;
    Target target;
    std::int64_t displacement = 0;
    std::uint8_t location = 0;  // the 4-bit location field as stored
};

struct Public {
    std::string name;
    std::optional<std::string> segment;  // absent: an absolute symbol (no segment)
    std::int64_t offset = 0;
};

struct SegDef {
    std::string name, class_name;
    std::int64_t length = 0;
    std::uint8_t alignment = 0;  // the A field of the segment attributes
};

struct Communal {
    std::uint64_t size = 0;
    bool far_pointer = false;  // a far communal: size is the product of two counts
};

struct StartAddress {
    std::string segment;
    std::int64_t offset = 0;
};

// An object module read whole.
struct Object {
    OrderedMap<std::string, Bytes> segments;  // each segment's bytes as the linker would place them
    std::vector<SegDef> segdefs;
    OrderedMap<std::string, std::vector<std::string>> groups;  // group -> its segments
    std::vector<Public> publics;
    std::vector<std::string> externals;
    OrderedMap<std::string, Communal> communals;
    std::vector<Fixup> fixups;
    OrderedMap<std::string, std::uint8_t> combine;  // segment -> combination (5: a stack segment)
    std::optional<StartAddress> start;  // the entry point MODEND names, if any
    OrderedMap<std::string, std::uint64_t> filled;  // segment -> how many bytes of it the object's data cover
};

// ---- reading ----------------------------------------------------------------------------------------------
bool is_aout(ByteView data);
// The records in order, up to and including the MODEND. The last record is cut short if the data ends inside it.
std::vector<Record> records(ByteView data);
// The module as `data` holds it (a library member or a .obj). FormatError/TruncatedError for a damaged one,
// UnsupportedError for an a.out object.
Object read(ByteView data);
// The modules of a library as (name, bytes); a plain object is one module named "(object)". A name met twice gets
// "#2", "#3" ....
std::vector<std::pair<std::string, ByteView>> library(ByteView data);
// Public and external names without reading the whole module (first appearance order, each once).
std::vector<std::string> publics(ByteView data);
std::vector<std::string> externs(ByteView data);
std::pair<std::set<std::string>, std::set<std::string>> names_of(ByteView data);  // (publics, externals) of a module

// The records walked raw, for listings: (offset, type, payload) until an F1 LIBEND; a damaged record stops the walk
// and `error` says why.
struct RawRecord {
    std::uint64_t offset = 0;
    std::uint8_t type = 0;
    ByteView payload;
};
struct Walk {
    std::vector<RawRecord> records;
    std::optional<std::string> error;
};
Walk walk(ByteView data, std::uint64_t start = 0);
// The record type's name ("SEGDEF"), or "?XX".
std::string record_name(std::uint8_t type);
// The module-by-module listing: each segment, public and external; `raw` lists every record instead.
std::string listing(ByteView data, bool raw = false);

// ---- what the code of a module is ---------------------------------------------------------------------------------
// The bytes a fix-up covers, by its location field.
std::size_t fixup_width(std::uint8_t location);
// The first code segment (class CODE or a *TEXT name; else the first segment): its filled bytes, the offsets a
// fix-up covers (so a comparison can skip them), and the publics with their segments.
struct CodeSegment {
    std::optional<std::string> segment;
    Bytes code;
    std::set<std::uint64_t> fixup_mask;
    OrderedMap<std::string, std::pair<std::string, std::int64_t>> publics;  // name -> (segment or "?", offset)
};
CodeSegment code_segment(ByteView data);
struct FunctionBytes {
    Bytes code;
    std::set<std::uint64_t> fixup_mask;  // relative to the function's start
};
// Each public of the code segment with its bytes (cut at the next public) and fix-up mask.
OrderedMap<std::string, FunctionBytes> function_bytes(ByteView data);
// Offsets of segment `seg` that fix-ups cover.
std::set<std::int64_t> holes(const Object& o, std::string_view seg);
// The names of the segments that hold code (a *_TEXT name, class CODE/PROG, _PROG, _CODE).
std::vector<std::string> code_segments(const Object& o);
struct FunctionSpan {
    std::string name;
    std::int64_t offset = 0, length = 0;
};
// The publics of segment `seg` cut into functions ("(module start)" for bytes before the first public).
std::vector<FunctionSpan> functions(const Object& o, std::string_view seg = "_TEXT");

// ---- writing --------------------------------------------------------------------------------------------------------
// One record's bytes, checksum included. FormatError when the payload does not fit a record.
Bytes record(std::uint8_t type, ByteView payload);
// A length-prefixed name (latin1 bytes). FormatError past 255 bytes.
Bytes name(std::string_view latin1_bytes);
// `data` with every external name replaced through `mapping` (names not in it are kept).
Bytes rename_externs(ByteView data, const std::map<std::string, std::string>& mapping);
// `data` with every external, communal and public name passed through `rename` (UTF-8 in and out).
Bytes rename_symbols(ByteView data, const std::function<std::string(std::string_view)>& rename);
// `data` with bytes patched in a segment: spec is "<segment>+<hex offset>=<hex bytes>". UsageError for a bad spec.
Bytes patched(ByteView data, std::string_view spec);
// A minimal object: a code segment (and optionally a data segment) with publics.
Bytes raw_object(std::string_view title, ByteView code, const OrderedMap<std::string, std::int64_t>& publics,
                 ByteView data = {}, const OrderedMap<std::string, std::int64_t>& data_publics = {},
                 std::string_view text_segment = "_TEXT", int data_alignment = 1);
// An object that defines a block of uninitialised data with the given names (offsets within [start, end)).
Bytes bss_object(const OrderedMap<std::string, std::int64_t>& names, std::int64_t start, std::int64_t end,
                 std::string_view title = "bssvars", std::string_view segment = "_BSS", bool paragraph_aligned = false);

}  // namespace cb::formats::omf
