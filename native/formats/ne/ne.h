// ne.h -- a New Executable (Windows 1.x-3.x and the 16-bit half of Windows 95): the header and
// its tables -- segments and their relocations, the name tables, module references and
// imported names, the entry table, resources -- and the Windows 1.x boot image, whose modules
// are spread over a resident file and an overlay.
//
// The C++ form of codebreaker/formats/ne.py (NEFile, BootImageModule, embedded, standalone),
// fact for fact and failure for failure: a read past the end raises what Python raised there
// (IndexError for one byte, struct.error for a word), in the same order of reads.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../core/bytes.h"

namespace cbf::ne {

constexpr uint16_t kPreload = 0x0040;     // segment / resource flag: loaded at start
constexpr uint16_t kRelocInfo = 0x0100;   // segment flag: relocation records follow its data

struct Header {
    uint8_t linker_ver, linker_rev;
    uint16_t entry_off, entry_len;
    uint32_t crc;
    uint16_t flags, autodata_seg, heap, stack;
    uint32_t csip, sssp;
    uint16_t n_segs, n_modrefs, nonres_size, seg_tab, rsrc_tab, resname_tab, mod_tab, imp_tab;
    uint32_t nonres_off;                  // from the start of the file
    uint16_t n_movable, align_shift, n_resources;
    uint8_t exetype;
    std::optional<uint16_t> expect_winver;
};

struct Name {
    Bytes name;                           // latin1
    uint16_t ordinal;
};

struct Segment {
    int64_t idx, file_off, length;
    uint16_t flags;
    int64_t minalloc;
    bool is_data() const { return flags & 1; }
};

struct Relocation {
    enum Kind { Internal, ImportOrdinal, ImportName, OsFixup } kind;
    uint8_t src, rel_type;
    uint16_t offset;
    uint8_t seg = 0;                      // Internal
    uint16_t entry = 0;                   // Internal
    uint16_t module = 0;                  // ImportOrdinal, ImportName
    uint16_t ordinal = 0;                 // ImportOrdinal
    uint16_t name_off = 0;                // ImportName
    uint16_t ftype = 0;                   // OsFixup
    bool additive() const { return rel_type & 4; }
};

struct Entry {
    enum Kind { Unused, Moveable, Fixed } kind;
    int64_t ordinal;
    uint8_t seg = 0, flags = 0;
    uint16_t offset = 0;
};

struct Resource {
    std::string type, id;                 // UTF-8: a predefined type's name, "#n", or the table's string
    int64_t off, length;
    uint16_t flags;
};

// One module: a plain NE file, a module inside another file (from_buffer: `base` is added to
// its segment offsets), or one of a boot image's modules (`overlay_at`: where the overlay
// starts in `data`; what is not PRELOAD lives there).
class Module {
public:
    Module(Bytes data, int64_t ne_off);   // the header parsed (raises as _parse_header)
    static Module open(Bytes data);       // NEFile(path): the MZ stub's checks first

    Bytes data;
    int64_t ne_off;
    int64_t base = 0;
    bool boot = false;
    int64_t overlay_at = 0;
    Header h;

    uint16_t u16(int64_t off) const { return data.u16(off); }
    uint32_t u32(int64_t off) const { return data.u32(off); }

    std::vector<Name> read_name_table(int64_t file_off) const;
    std::vector<Name> resident_names() const;
    std::vector<Name> nonresident_names() const;
    std::string module_name() const;
    std::vector<std::string> module_refs() const;
    std::string imported_name(int64_t off) const;
    std::vector<Segment> segments() const;
    std::vector<Relocation> relocations(uint16_t flags, int64_t file_off, int64_t length) const;
    std::vector<Entry> entries() const;
    std::vector<int64_t> export_ordinals() const;
    std::vector<Resource> resources() const;
    std::string rsrc_string(int64_t base, int64_t off) const;

private:
    void parse_header();
};

// embedded(data, start): every NE header inside a file that holds several modules
std::vector<std::pair<int64_t, std::string>> embedded(Bytes data, int64_t start = 0x40);
// _tables_end / standalone: one module laid out as an ordinary one-file executable
int64_t tables_end(const Module &m);
std::string standalone(const Module &m);

const char *resource_type_name(uint16_t id);   // RSRC_TYPES, or nullptr

}  // namespace cbf::ne
