// codebreaker/formats/ne.hpp -- typed C++ from codebreaker.formats.ne.
//
// ne.py -- the New Executable (NE) reader, answered by the C++ (native/formats/ne: formats.dll,
// through native.py). Its Python reader is gone (docs/NATIVE.md, step 4); the porting engine's copy
// of it, native/ported/ne's input, is native/ported/ne/src_python/ne.py. This module is the API: each
// method's def is its signature and documentation, and formats.dll answers.
//
// The NE format debuted with Windows 1.0 (1985). The reader gives the DOS MZ stub, the NE header,
// segment table, resident/non-resident name tables (exports), module-reference + imported-name
// tables (imports), the entry table, and the resource table.
//
// Typing and runtime notes:
//   * The Python asked formats.dll through a ctypes/pickle bridge (codebreaker.formats.native). Here the
//     very C++ the DLL is built from (native/formats/ne/ne.cc: cbf::ne::Module) is called directly, with typed
//     results; the dicts with fixed keys that the DLL answered are the structs below (Header, Segment,
//     Relocation, Entry, Resource). Like the DLL, every method builds the module again from `data` and its place
//     (ne_off, base, overlay_at) -- NEFile keeps nothing else -- so a damaged header raises in the method that
//     meets it. An error the reader raises (cbf::PyError) is thrown as the same py:: exception.
//   * Relocation records are dicts whose keys depend on `kind`: the keys that kind has are set, the others are
//     std::nullopt.
//   * relocations(seg) takes the Segment as segments() gives it (the Python took any dict with flags, file_off and
//     length; a dict without them, KeyError/TypeError, cannot be made here).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::ne {

inline constexpr int64_t PRELOAD = 0x0040;   // segment / resource flag: loaded at start (else on demand)

// the header's fields (NEFile.h)
struct Header {
    int64_t linker_ver = 0, linker_rev = 0, entry_off = 0, entry_len = 0, crc = 0, flags = 0, autodata_seg = 0, heap = 0,
            stack = 0, csip = 0, sssp = 0, n_segs = 0, n_modrefs = 0, nonres_size = 0, seg_tab = 0, rsrc_tab = 0,
            resname_tab = 0, mod_tab = 0, imp_tab = 0, nonres_off = 0, n_movable = 0, align_shift = 0, n_resources = 0,
            exetype = 0;
    std::optional<int64_t> expect_winver;
};

// segments(): {idx, file_off, length, flags, minalloc, kind: 'DATA' | 'CODE'}
struct Segment {
    int64_t idx = 0, file_off = 0, length = 0, flags = 0, minalloc = 0;
    py::str kind;
};

// relocations(): {src, rel_type, additive, offset, kind, ...}; kind is INTERNAL (seg, entry), IMPORTORD (module,
// ordinal), IMPORTNAME (module, name_off) or OSFIXUP (ftype)
struct Relocation {
    int64_t src = 0, rel_type = 0;
    bool additive = false;
    int64_t offset = 0;
    py::str kind;
    std::optional<int64_t> seg, entry, module, ordinal, name_off, ftype;
};

// entries(): {ordinal, kind: 'UNUSED' | 'MOVEABLE' | 'FIXED', seg, offset, flags} (the last three not for UNUSED)
struct Entry {
    int64_t ordinal = 0;
    py::str kind;
    std::optional<int64_t> seg, offset, flags;
};

// resources(): {type, id, off, length, flags} (type and id: a predefined name, '#n', or the table's string)
struct Resource {
    py::str type, id;
    int64_t off = 0, length = 0, flags = 0;
};

// An NE module read from `path` (ne_off, h: the header's fields), every method answered by
// the C++ reader with the module's place.
class NEFile {
  public:
    int64_t base = 0;   // byte added to segment sector offsets (nonzero for embedded modules)

    py::str path;
    py::bytes data;
    int64_t ne_off = 0;
    Header h;

    explicit NEFile(const py::str& path);
    virtual ~NEFile() = default;

    // Parse an NE module that begins directly at ne_off (no MZ stub).
    // Segment sector offsets are relative to ne_off for such embedded modules.
    static NEFile from_buffer(const py::bytes& data, int64_t ne_off, const py::str& path = py::str("<buffer>"));

    void _parse_header();

    int64_t u16(int64_t off) const;
    int64_t u32(int64_t off) const;
    // Returns list of (name, ordinal). First entry = module/description.
    py::list<std::tuple<py::str, int64_t>> _read_name_table(int64_t file_off) const;
    py::list<std::tuple<py::str, int64_t>> resident_names() const;
    py::list<std::tuple<py::str, int64_t>> nonresident_names() const;
    // This module's own name -- the first resident-name-table entry.
    py::str module_name() const;
    // Imported module names, in module-reference-table order (1-based).
    py::list<py::str> module_refs() const;
    py::str imported_name(int64_t off) const;
    py::list<Segment> segments() const;
    // Parse per-segment relocation records (if RELOCINFO set).
    py::list<Relocation> relocations(const Segment& seg) const;
    // The ordinals the entry table actually exports, in order (unused gaps skipped).
    // Exports by ordinal only have no resident name, so the name tables alone miss them.
    py::list<int64_t> export_ordinals() const;
    // Parse entry-table bundles -> list of dict(ordinal, seg, offset, flags).
    py::list<Entry> entries() const;
    py::list<Resource> resources() const;
    py::str _rsrc_string(int64_t base, int64_t off) const;

    // the overlay's place when this is a boot image's module (BootImageModule), else None
    virtual std::optional<int64_t> overlay_place() const { return std::nullopt; }

  protected:
    NEFile() = default;
    static void init_buffer(NEFile& self, const py::bytes& data, int64_t ne_off, const py::str& path);
};

// One system module inside a Windows 1.x boot image (see open_boot_image).
//
// The image is two files and a module is spread over both: what is loaded at start is in the
// resident file (WIN100.BIN), what is loaded on demand in the overlay (WIN100.OVL). A segment
// or resource says which by its PRELOAD flag, and its offset counts from the start of THAT
// file; the two ranges overlap, so reading from the wrong file gives plausible nonsense. The
// names of the exports -- the non-resident name table -- are in the overlay too, at the
// header's offset counted in alignment units, not bytes. `data` here is the resident file
// followed by the overlay, with every offset this class returns pointing into that.
class BootImageModule : public NEFile {
  public:
    int64_t overlay_at = 0;

    static BootImageModule from_buffer(const py::bytes& data, int64_t ne_off, const py::str& path = py::str("<buffer>"));
    std::optional<int64_t> overlay_place() const override { return overlay_at; }

  protected:
    BootImageModule() = default;
};

// A module's place, as every question takes it first: (ne_off, base, overlay_at or None).
std::tuple<int64_t, int64_t, std::optional<int64_t>> where(const NEFile& m);

// (bytes, the module's place) as the C++ reader takes a module.
std::tuple<py::bytes, std::tuple<int64_t, int64_t, std::optional<int64_t>>> _place(const NEFile& nef);

// [(offset, module name)] of every NE header inside a file that holds several modules.
py::list<std::tuple<int64_t, py::str>> embedded(const py::bytes& data, int64_t start = 0x40);

// {module name: BootImageModule} for a Windows 1.x boot image, in file order. `overlay` is
// the demand-loaded half (default: the file of the same name ending .OVL beside `path`);
// without it only what is loaded at start can be read.
py::dict<py::str, BootImageModule> open_boot_image(const py::str& path, const std::optional<py::str>& overlay = std::nullopt);

// Offset just past the header and every table that belongs with it.
int64_t _tables_end(const NEFile& nef);

// The bytes of an ordinary one-file executable holding the module `nef` -- for a module
// read out of a boot image, so that everything that takes a path can take it. Header and
// tables are kept as they are; segments (with their relocations), resources and the
// non-resident names are laid out after them and the tables pointed at the new places.
py::bytes standalone(const NEFile& nef);

}  // namespace codebreaker::formats::ne
