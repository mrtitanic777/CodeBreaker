// codebreaker/formats/xbox/fatx.hpp -- typed C++ from codebreaker.formats.xbox.fatx.
//
// FATX filesystem reader, deleted-file recovery, and signature carver.
//
// Reads Xbox 360 (big-endian) and original Xbox (little-endian) FATX partitions,
// disk images, and raw partition dumps. The on-disk layout, dirent format, FAT
// handling, timestamp packing, orphan-recovery and file-carving approach follow
// **FATX-Tools** by aerosoul94 (MIT) -- read for format facts and reimplemented
// here as original code driven by this project's own XEX/STFS parsers.
//
// Authoritative facts (per FATX-Tools):
//   - volume header  endian+'LLLL' : magic, serial, sectors_per_cluster, root_cluster
//   - dirent         endian+'BB42sLLLLL' (0x40 bytes): name_len, attr, name[42],
//                    first_cluster, size, created, modified, accessed
//   - timestamp      year=((t>>25)&0x7F)+epoch, month, day, hour, minute, second*2;
//                    epoch 1980 (Xbox 360) / 2000 (original Xbox)
//   - FAT            16-bit if max_clusters<0xFFF0 else 32-bit; FAT aligned to 0x1000
//   - offsets        fat @ vol+0x1000 ; data @ vol+0x1000+align(max_clusters*sz,0x1000)
//
// Commands: info / list / extract / recover / carve.
//
// Typing and runtime notes:
//   * The Python's two dataclasses (FatxPartition, FatxEntry) are objects other code holds and changes in place
//     (recover() links the same entries into each other's children and paths), so a FatxPartition / FatxEntry
//     is a py::ref<...> (a shared handle); None is a null handle (_mount, _parse_dirent). A dict with fixed keys
//     (the partition map entries) is the record PartitionDef.
//   * datetime is not in the runtime: DateTime below holds the six fields the module reads (year ... second) and
//     checks them as datetime.datetime(...) does (year 1..9999, month, day of the month, hour < 24, minute and
//     second < 60), a ValueError is std::nullopt as decode_timestamp returns None. Its strftime is the one
//     format the command line uses, "%Y-%m-%d %H:%M".
//   * The stream (open(path, "rb") / "r+b") is a py::file in a std::optional (None after close()); a call on a
//     closed reader raises AttributeError like the Python's None.seek.
//   * The struct formats built from the endian ('>' / '<' + "LLLL", "BB42sLLLLL", "H" * n) are done with the
//     byte-order helpers below: '>' and '!' read big-endian; '<', '=', '@' and '' little-endian (the same bytes
//     for these formats); any other endian text is struct.error.
//   * Python's RecursionError for a directory tree that loops back on itself (a dirent whose first cluster is
//     an ancestor's): list_directory counts its depth and raises the same-named exception past 900 levels.
//   * fatx.main(): takes the argument list (sys.argv[1:]) and prints with py::print.
//   * The XEX header scan in _xex_length calls the typed codebreaker::formats::xex::optional_headers.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/xex.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::fatx {

// Python's RecursionError (a RuntimeError)
struct RecursionError : py::RuntimeError {
    using py::RuntimeError::RuntimeError;
    const char* type() const override { return "RecursionError"; }
};

// ── Constants ───────────────────────────────────────────
extern const py::bytes FATX_MAGIC;
inline constexpr int64_t FATX_SECTOR_SIZE = 0x200;
inline constexpr int64_t FATX_PAGE_SIZE = 0x1000;
inline constexpr int64_t FATX_DIRENT_SIZE = 0x40;
inline constexpr int64_t FATX_MAX_FILENAME = 42;

inline constexpr int64_t DIRENT_NEVER_USED = 0x00;
inline constexpr int64_t DIRENT_NEVER_USED2 = 0xFF;
inline constexpr int64_t DIRENT_DELETED = 0xE5;

inline constexpr int64_t FILE_ATTRIBUTE_DIRECTORY = 0x10;
inline constexpr int64_t VALID_FILE_ATTRIBUTES = 0x37;   // RO|HIDDEN|SYSTEM|DIRECTORY|ARCHIVE

// Characters a real FATX file name may contain (plus 0xFF padding).
extern const py::set<int64_t> _VALID_NAME;

// One entry of a partition map: {name, offset, size} (size 0 = to end).
struct PartitionDef {
    py::str name;
    int64_t offset = 0;
    int64_t size = 0;
};

// Standard Xbox 360 retail HDD partition map (offset, size; size 0 = to end).
extern const py::list<PartitionDef> PARTITIONS;
extern const py::list<PartitionDef> USB_PARTITIONS;

// ── Timestamp ───────────────────────────────────────────
// datetime.datetime, as far as this module uses it.
struct DateTime {
    int64_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    // strftime("%Y-%m-%d %H:%M")
    py::str strftime_minute() const;
};

// Decode a packed FATX timestamp; None if it is not a valid date.
std::optional<DateTime> decode_timestamp(int64_t t, int64_t epoch);

// ── Data classes ────────────────────────────────────────
struct FatxPartition {
    py::str name = py::str("");
    int64_t offset = 0;
    int64_t size = 0;
    py::str endian = py::str(">");   // '>' Xbox 360, '<' original Xbox
    int64_t epoch = 1980;
    int64_t sectors_per_cluster = 32;
    int64_t bytes_per_cluster = 0x4000;
    int64_t root_cluster = 1;
    int64_t max_clusters = 0;
    bool fat16x = true;
    int64_t fat_offset = 0;
    int64_t data_offset = 0;
    bool valid = false;
};

struct FatxEntry {
    py::str name = py::str("");
    int64_t attributes = 0;
    int64_t first_cluster = 0;
    int64_t file_size = 0;
    std::optional<DateTime> created, modified, accessed;
    bool is_directory = false;
    bool is_deleted = false;
    py::str path = py::str("");
    int64_t cluster = 0;   // physical cluster the dirent was found in
    py::list<py::ref<FatxEntry>> children;
};

using PartitionRef = py::ref<FatxPartition>;
using EntryRef = py::ref<FatxEntry>;

// ── Volume reader ───────────────────────────────────────
class FatxReader {
  public:
    py::str path;

    explicit FatxReader(const py::str& path);
    ~FatxReader();
    FatxReader(const FatxReader&) = delete;
    FatxReader& operator=(const FatxReader&) = delete;

    void close();

    // -- partition discovery --------------------------------------------
    // Every FATX partition we can find: the standard maps, a raw partition
    // at offset 0, or a whole-file single partition.
    py::list<PartitionRef> detect_partitions();

    py::bytes _read(int64_t off, int64_t n);
    PartitionRef _mount(const PartitionDef& pdef);

    // -- FAT + clusters -------------------------------------------------
    py::list<int64_t> _fat(const PartitionRef& p);
    int64_t cluster_offset(const PartitionRef& p, int64_t cluster) const;
    py::bytes read_cluster(const PartitionRef& p, int64_t cluster);
    // Follow the FAT from `first` until the end/reserved marker.
    py::list<int64_t> cluster_chain(const PartitionRef& p, int64_t first);

    // -- dirent parsing -------------------------------------------------
    EntryRef _parse_dirent(const PartitionRef& p, const py::bytes& data, const py::str& base, int64_t cluster);
    py::list<EntryRef> list_directory(const PartitionRef& p, int64_t cluster = 0, const py::str& base = py::str(""),
                                      bool include_deleted = false);

    // -- extraction -----------------------------------------------------
    void extract_file(const PartitionRef& p, const EntryRef& e, const py::str& out, bool contiguous = false);
    py::list<EntryRef> _flatten(const py::list<EntryRef>& entries);
    int64_t extract_all(const PartitionRef& p, const py::str& out_dir);

    // -- deleted-file recovery (metadata / orphan analysis) -------------
    bool _dirent_valid(const PartitionRef& p, const EntryRef& e) const;
    // Scan every cluster for orphaned/deleted dirents and rebuild what can
    // be recovered. Returns a flat list of recoverable FatxEntry.
    py::list<EntryRef> recover(const PartitionRef& p);
    int64_t recover_all(const PartitionRef& p, const py::str& out_dir, bool deleted_only = true);

    // -- signature carver -----------------------------------------------
    // Step the partition at `interval` and carve files by magic even where
    // no dirent survives (XEX / XBE / STFS / PE). With `decrypt`, encrypted
    // normal XEX are measured exactly by decrypting the block chain (slower).
    // -> [(offset in the data area, kind, length, path written)]
    py::list<std::tuple<int64_t, py::str, int64_t, py::str>> carve(const PartitionRef& p, const py::str& out_dir,
                                                                  int64_t interval = FATX_PAGE_SIZE, bool decrypt = false);

    // -- write side: inject / mkdir / delete ----------------------------
    void _writable();
    void _write(int64_t off, const py::bytes& data);
    void _set_fat(const PartitionRef& p, int64_t cluster, int64_t value);
    int64_t _end(const PartitionRef& p) const;
    py::list<int64_t> _alloc(const PartitionRef& p, int64_t n);
    int64_t _encode_ts(const PartitionRef& p, const std::optional<DateTime>& dt = std::nullopt) const;
    // Return the first cluster of the directory named by `path` ('' = root).
    int64_t _resolve_dir(const PartitionRef& p, const py::str& path);
    void _write_dirent(const PartitionRef& p, int64_t dir_cluster, const py::str& name, int64_t attr, int64_t first, int64_t size);
    // Write `data` as a new file at dest_path ('dir/sub/name' or 'name').
    void add_file(const PartitionRef& p, const py::str& dest_path, const py::bytes& data);
    // Create a directory at dest_path.
    void mkdir(const PartitionRef& p, const py::str& dest_path);
    // Delete a file/empty-dir: mark its dirent deleted and free its chain.
    bool remove(const PartitionRef& p, const py::str& path);

  private:
    std::optional<py::file> _stream;
    bool _writable_mode = false;   // the stream's mode is "r+b"
    int64_t _size = 0;
    py::dict<int64_t, py::list<int64_t>> _fat_cache;   // partition offset -> FAT list

    py::list<EntryRef> list_directory_(const PartitionRef& p, int64_t cluster, const py::str& base, bool include_deleted, int64_t depth);
    // the entries of one directory's clusters (the first half of list_directory, kept out of the recursive frame)
    py::list<EntryRef> read_entries_(const PartitionRef& p, int64_t cluster, const py::str& base, bool include_deleted);
    py::file& stream_(const char* method);
};

// ── carver signatures ───────────────────────────────────
extern const py::dict<py::str, py::str> _EXT;

int64_t _u32be(const py::bytes& b, int64_t o);

// Return (kind, length) if a known file starts at `pos`, else None.
std::optional<std::tuple<py::str, int64_t>> _carve_at(py::file& stream, int64_t pos, int64_t avail, bool decrypt = false);

// Exact on-disk XEX2 file length = data_offset + stored-data length, derived
// from the file-format-info: walk the block chain (normal), sum data runs
// (basic), or add the image size (none). Returns None if the header is short.
std::optional<int64_t> _xex_length(py::file& stream, int64_t pos, const py::bytes& head, int64_t avail, bool decrypt = false);

py::bytes _reread(py::file& stream, int64_t pos, int64_t n);

// Exact stored-data length of an ENCRYPTED normal XEX: it needs the title key
// to walk the decrypted block chain, and the XEX engine that has it is native
// (native/xex). Returns 0, and the carver sizes the file by its other rules.
int64_t _xex_decrypt_datalen(py::file& stream, int64_t pos, int64_t data_offset, int64_t first_block, int64_t avail);

int64_t _rd_u32le(py::file& stream);

// STFS package size = 0xB000 metadata + allocated blocks * 0x1000. The
// (little-endian) total-block count sits in the volume descriptor at 0x395.
std::optional<int64_t> _stfs_length(const py::bytes& head);

// PE image length from SizeOfHeaders + section raw sizes (best effort).
std::optional<int64_t> _pe_length(py::file& stream, int64_t pos, const py::bytes& head, int64_t avail);

extern const py::str DESCRIPTION;

// ── CLI ─────────────────────────────────────────────────
void _print_tree(const py::list<EntryRef>& entries, int64_t depth = 0);

// `argv` is sys.argv[1:].
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::fatx
