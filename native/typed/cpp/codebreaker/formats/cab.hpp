// codebreaker/formats/cab.hpp -- typed C++ from codebreaker.formats.cab.
//
// cab.py -- Microsoft cabinet (MSCF) files: list, extract and write, with no outside tool.
//
// A cabinet holds FOLDERS (one compressed stream each, cut into CFDATA blocks of at most 32768
// uncompressed bytes) and FILES (a name plus an offset and size inside one folder's stream). A
// folder may run on into the next cabinet of a set: a file whose folder index is 0xFFFD is
// continued FROM the previous cabinet, 0xFFFE continues INTO the next one, 0xFFFF both; the
// cabinets name their neighbours (prev/next) and are looked up in the same directory.
//
//     Cabinet c = cab::read(path);   // c.info: set_id, index, prev, next, flags, folders
//                                    // c.files: Entry(name, size, folder, offset, date, time,
//                                    //                attributes, compression)
//     cab::extract(path, outdir);    // -> {name: output path}; names=[...] picks some
//     cab::write({{name, data}}, out_path, "mszip");     // or "none"; -> size
//
// Decompression is done for stored (NONE), MSZIP (zlib) and QUANTUM (written from the published
// description of the format) by formats.dll (native/formats/cab, docs/NATIVE.md step 4). LZX cabinets are listed but not extracted (CabError).
// Block checksums are verified where a cabinet carries them.
//
//     python -m codebreaker cab list <file.cab>
//     python -m codebreaker cab extract <file.cab> <outdir> [names...]
//     python -m codebreaker cab create <out.cab> <file-or-folder>... [--store]
//
// Typing notes:
//   * read / checksum / extract / write are formats.dll's questions (native/formats/cab/cab.cc, linked in and asked through
//     cbf_call).  The info dict is the record CabInfo; a file's namedtuple the record Entry (its fields in the namedtuple's order).
//   * CabError is the exception the reader names (a ValueError in Python: here too).
//   * write()'s prev / next are None, a cabinet name, or (cabinet name, disk label): `Chain`, a variant of the name and the
//     (name, label) pair (the label may be None or empty).  A list of another length than 2 (the reader's ValueError) cannot be made.
//   * main() returns the exit status; what Python prints to stderr goes to stderr.
//   * Replaces nothing of the hooks listed for this batch (the cabinet readers of the other typed ports have their own).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::formats::cab {

extern const py::dict<int64_t, py::str> COMPRESSION;
inline constexpr int64_t FROM_PREV = 0xFFFD, TO_NEXT = 0xFFFE, PREV_AND_NEXT = 0xFFFF;

// Blocks hold 32768 uncompressed bytes (all but the last of a folder). Microsoft's decoder
// and 7-Zip count on that: each MSZIP block is inflated at the START of a 32768-byte window
// that still holds the block before it, so a block's back-references only land where the packer
// meant when the block before it filled the window exactly.
inline constexpr int64_t BLOCK = 32768;
// The one exception made when WRITING. A CFDATA payload must not exceed 32768 bytes: the
// MSZIP decompressor in Windows 95 SETUP reads the payload into a fixed 32768-byte buffer and
// overruns on anything larger ("corrupt Setup (.CAB) file"). A full block of incompressible
// data cannot meet that (deflate's stored form adds 5 bytes, the "CK" signature 2), so such
// a block is cut at 32756 bytes and stored.
inline constexpr int64_t SHORT_BLOCK = 32756;

// one file of a cabinet: collections.namedtuple("Entry", "name size folder offset date time attributes compression")
struct Entry {
    py::str name;
    int64_t size = 0;
    int64_t folder = 0;
    int64_t offset = 0;
    int64_t date = 0;
    int64_t time = 0;
    int64_t attributes = 0;
    py::str compression;
};
template <class F> void for_each_field_mut(Entry& e, F&& f) {
    f("name", e.name, true);
    f("size", e.size, true);
    f("folder", e.folder, true);
    f("offset", e.offset, true);
    f("date", e.date, true);
    f("time", e.time, true);
    f("attributes", e.attributes, true);
    f("compression", e.compression, true);
}

class CabError : public py::ValueError {
  public:
    using py::ValueError::ValueError;
    const char* type() const override { return "CabError"; }
};

// the cabinet's header facts
struct CabInfo {
    std::optional<py::str> prev;
    std::optional<py::str> prev_disk;
    std::optional<py::str> next;
    std::optional<py::str> next_disk;
    int64_t set_id = 0;
    int64_t index = 0;
    int64_t flags = 0;
    py::list<py::str> folders;               // each folder's compression, by name
    int64_t size = 0;
    py::str version;
};
template <class F> void for_each_field_mut(CabInfo& c, F&& f) {
    f("prev", c.prev, true);
    f("prev_disk", c.prev_disk, true);
    f("next", c.next, true);
    f("next_disk", c.next_disk, true);
    f("set_id", c.set_id, true);
    f("index", c.index, true);
    f("flags", c.flags, true);
    f("folders", c.folders, true);
    f("size", c.size, true);
    f("version", c.version, true);
}

// One parsed cabinet: `path`, `info` and `files` (list of Entry).
class Cabinet {
  public:
    py::str path;
    CabInfo info;
    py::list<Entry> files;
    py::list<std::tuple<int64_t, int64_t, int64_t>> _folders;     // [(offset of first CFDATA, block count, type)]
    int64_t _block_reserve = 0;

    Cabinet(const py::str& path, const CabInfo& info, const py::list<Entry>& files,
            const py::list<std::tuple<int64_t, int64_t, int64_t>>& folders, int64_t block_reserve);

    // The real folder number of a file (the continuation markers name the first or the
    // last folder of the cabinet).
    int64_t folder_of(const Entry& entry) const;
};

// Parse the headers of one cabinet (nothing is decompressed).
Cabinet read(const py::str& path);

// The CFDATA checksum: 32-bit little-endian words of the block header (two sizes) and
// the payload XORed together; the 1-3 odd bytes at the end count most-significant first.
int64_t checksum(const py::bytes& payload, int64_t uncompressed);

// Extract the files listed in one cabinet (all, or those in `names`, compared without
// regard to case) into `outdir`. Neighbouring cabinets are read as needed for files whose
// folder is continued. Returns {name: output path}.
py::dict<py::str, py::str> extract(const py::str& path, const py::str& outdir,
                                   const std::optional<py::list<py::str>>& names = std::nullopt);

// a neighbour of a cabinet set: its name, or (name, disk label)
using Chain = std::variant<py::str, std::tuple<py::str, std::optional<py::str>>>;

// Write a cabinet holding `files` = [(name, bytes)] in one folder; returns its size.
//
// `short_blocks` keeps every MSZIP data block within what Windows 95 SETUP can take, at the
// price of 7-Zip rejecting cabinets that hold incompressible data; pass False for a cabinet
// meant for present-day tools only. A block that will not compress to a 32768-byte payload is
// written short and stored (SHORT_BLOCK); the block after a short one is deflated with no
// dictionary, so it is read the same by every decoder.
//
// `set_id`, `index`, `prev` and `next` (a cabinet name, or (cabinet name, disk label)) are
// the fields that tie cabinets into a set; a program that walks the set, as Windows 95
// SETUP does, needs them carried over when a cabinet of a set is rebuilt. The output is a
// valid cabinet delivering the same files, not a byte copy of any original cabinet.
int64_t write(const py::list<std::tuple<py::str, py::bytes>>& files, const py::str& out_path, const py::str& compress = py::str("mszip"),
              int64_t set_id = 0, int64_t index = 0, const std::optional<Chain>& prev = std::nullopt,
              const std::optional<Chain>& next = std::nullopt, bool short_blocks = true);

// --- command -------------------------------------------------------------------------------
py::list<std::tuple<py::str, py::bytes>> _gather(const py::list<py::str>& paths);

// python -m codebreaker cab list <file.cab> | extract <file.cab> <outdir> [names...] | create <out.cab> <file-or-folder>... [--store]
// (sys.argv[1:]); the exit status
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::cab
