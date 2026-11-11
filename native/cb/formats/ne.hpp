// cb/formats/ne.hpp -- the New Executable (NE) reader: Windows 1.x through 3.x modules and 16-bit Win95 DLLs.
//
// A `Module` owns the file's bytes and the header; the segment table, name tables, imports, entry table,
// relocations and resource table are read from them on demand. The reader is strict (a damaged table throws
// FormatError/TruncatedError), except relocation tables, whose count can be garbage when a segment base was
// moved without moving its table: those give an empty list.
//
// A module may live inside a larger file (several NE modules in one image): `Module::parse(data, ne_offset)`
// takes where its header is. A module of a Windows 1.x boot image is spread over two files (the resident
// WIN100.BIN and the overlay WIN100.OVL); `open_boot_image` reads both and returns the modules with offsets that
// point into resident-followed-by-overlay.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::ne {

inline constexpr std::uint16_t kPreload = 0x0040;    // segment / resource flag: loaded at start (else on demand)
inline constexpr std::uint16_t kRelocInfo = 0x0100;  // segment flag: relocation records follow its data
inline constexpr std::uint16_t kSegmentData = 0x0001;

// The header's fields. Offsets ending in _tab are relative to the start of the NE header.
struct Header {
    std::uint8_t linker_version = 0, linker_revision = 0;
    std::uint16_t entry_table_offset = 0, entry_table_length = 0;
    std::uint32_t crc = 0;
    std::uint16_t flags = 0, autodata_segment = 0, heap_size = 0, stack_size = 0;
    std::uint32_t cs_ip = 0, ss_sp = 0;
    std::uint16_t segment_count = 0, module_ref_count = 0, nonresident_size = 0;
    std::uint16_t segment_table = 0, resource_table = 0, resident_names_table = 0, module_ref_table = 0, imported_names_table = 0;
    std::uint32_t nonresident_offset = 0;  // from the start of the file
    std::uint16_t movable_entries = 0;
    std::uint16_t alignment_shift = 9;  // a stored 0 means 9
    std::uint16_t resource_segments = 0;
    std::uint8_t target_os = 0;
    std::optional<std::uint16_t> expected_windows_version;  // present when the segment table leaves room for it
};

struct Name {
    std::string name;  // latin1 -> UTF-8
    std::uint16_t ordinal = 0;
};

struct Segment {
    std::uint16_t index = 0;  // 1-based, as relocations and the entry table number them
    std::uint64_t file_offset = 0;  // 0: the segment has no data in the file
    std::uint32_t length = 0;
    std::uint16_t flags = 0;
    std::uint32_t min_alloc = 0;
    bool is_data() const { return (flags & kSegmentData) != 0; }
};

// What a relocation record points at.
struct InternalRef {  // another place in this module
    std::uint8_t segment = 0;  // 0xFF: a moveable entry; `entry` is then an entry-table ordinal
    std::uint16_t entry = 0;   // the offset in the segment, or the ordinal
};
struct ImportOrdinalRef {  // an exported ordinal of another module
    std::uint16_t module = 0;  // 1-based index into the module reference table
    std::uint16_t ordinal = 0;
};
struct ImportNameRef {  // an exported name of another module
    std::uint16_t module = 0;
    std::uint16_t name_offset = 0;  // into the imported-names table
};
struct OsFixupRef {  // a floating-point or similar emulator fix-up
    std::uint16_t fixup_type = 0;
};

struct Relocation {
    std::uint8_t source = 0;      // the low nibble of the source type: byte, selector, far pointer, offset ...
    std::uint8_t flags = 0;       // the relocation flags byte as stored (low 2 bits: target kind, bit 2: additive)
    std::uint16_t offset = 0;     // where in the segment
    std::variant<InternalRef, ImportOrdinalRef, ImportNameRef, OsFixupRef> target;
    bool additive() const { return (flags & 4) != 0; }
};

struct Entry {
    enum class Kind { Unused, Moveable, Fixed };
    std::uint32_t ordinal = 0;
    Kind kind = Kind::Unused;
    std::uint8_t segment = 0, flags = 0;  // not for Unused
    std::uint16_t offset = 0;
};

struct Resource {
    std::string type;  // a predefined type's name ("BITMAP"), "#n", or the table's own string
    std::string id;    // "#n" or the table's own string
    std::optional<std::uint16_t> type_number;  // the RT_* number when the table stores one
    std::optional<std::uint16_t> id_number;
    std::uint64_t file_offset = 0;
    std::uint64_t length = 0;
    std::uint16_t flags = 0;
};

class Module {
public:
    // The module whose header is at `ne_offset` of `data`. FormatError if there is no "NE" there.
    static Module parse(Bytes data, std::uint64_t ne_offset);
    // A normal executable: the MZ stub's e_lfanew finds the header. FormatError when it is not an NE file.
    static Module open(Bytes data);
    static Module open_file(const std::filesystem::path& file);
    // A module of a boot image: `data` is resident-file + overlay, `overlay_at` where the overlay starts.
    static Module parse_boot_module(Bytes data, std::uint64_t ne_offset, std::uint64_t overlay_at);
    // For probing ("is this an NE file?").
    static std::optional<Module> try_open(ByteView data);

    ByteView data() const { return data_; }
    std::uint64_t ne_offset() const { return ne_offset_; }
    // Added to segment sector offsets (nonzero for a module embedded at an offset); 0 for ordinary files.
    std::uint64_t base() const { return base_; }
    void set_base(std::uint64_t base) { base_ = base; }
    std::optional<std::uint64_t> overlay_at() const { return overlay_at_; }
    const Header& header() const { return header_; }

    std::vector<Name> resident_names() const;
    std::vector<Name> nonresident_names() const;
    std::string module_name() const;                // the first resident name, "SELF" when there is none
    std::vector<std::string> module_refs() const;  // imported module names, in module-reference order (1-based)
    std::string imported_name(std::uint64_t offset) const;  // "?" when the offset is outside the file
    std::vector<Segment> segments() const;
    // The segment's relocation records (empty when it has none or the table is not there).
    std::vector<Relocation> relocations(const Segment& segment) const;
    std::vector<Entry> entries() const;
    std::vector<std::uint32_t> export_ordinals() const;  // the ordinals the entry table exports, gaps skipped
    std::vector<Resource> resources() const;
    // Offset just past the header and every table that belongs with it.
    std::uint64_t tables_end() const;
    // The bytes of an ordinary one-file executable holding this module (for a boot-image module, so that
    // everything that takes a file can take it): the tables are kept, segments (with their relocations),
    // resources and the non-resident names are laid out after them and the tables pointed at the new places.
    Bytes standalone() const;

private:
    Module() = default;
    std::vector<Name> read_name_table(std::uint64_t offset) const;
    std::string resource_string(std::uint64_t table, std::uint64_t offset) const;
    std::uint64_t shifted(std::uint64_t value) const;

    Bytes data_;
    std::uint64_t ne_offset_ = 0;
    std::uint64_t base_ = 0;
    std::optional<std::uint64_t> overlay_at_;
    Header header_;
};

// Every NE header inside a file that holds several modules: (offset, module name), found by scanning for "NE"
// from `start` and checking that what follows looks like a header with a plausible name.
std::vector<std::pair<std::uint64_t, std::string>> embedded(ByteView data, std::uint64_t start = 0x40);

// The modules of a Windows 1.x boot image, in file order (a name met twice keeps its first module). `overlay` is
// the demand-loaded half; without it only what is loaded at start can be read.
std::vector<std::pair<std::string, Module>> open_boot_image(ByteView resident, ByteView overlay);
// The same from files; `overlay` defaults to the file of the same name ending .OVL beside `resident`.
std::vector<std::pair<std::string, Module>> open_boot_image_files(const std::filesystem::path& resident,
                                                                  const std::optional<std::filesystem::path>& overlay = std::nullopt);

// The predefined resource type's name ("CURSOR" ... "VERSION"), or nullptr.
const char* resource_type_name(std::uint16_t id);

}  // namespace cb::formats::ne
