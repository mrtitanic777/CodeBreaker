// codebreaker/formats/xbox/zar.hpp -- C++ port of codebreaker.formats.xbox.zar.
//
// Pure-Python (stdlib-only) reader and writer for the ZArchive (.zar) format.
//
// Adapted (MIT) from xVerter's zar_native.py; the format itself is documented by
// the ZArchive project (https://github.com/Exzap/ZArchive, MIT-0), files
// src/zarchivecommon.h, src/zarchivereader.cpp, src/zarchivewriter.cpp.
// Zstd comes from Python 3.14's stdlib `compression.zstd` -- no third-party dep.
//
// Layout (all integers BIG-endian on disk):
//
//   [ compressed data section ]   starts at file offset 0; the virtual
//                                 uncompressed stream is split into 64 KiB
//                                 blocks, each stored zstd-compressed, or raw
//                                 when the compressed size would be >= 64 KiB
//                                 (stored size == 65536 means "uncompressed").
//   [ pad to 8 bytes ]
//   [ offset-record section ]     CompressionOffsetRecord[]: { u64 baseOffset;
//                                 u16 size[16] } where size[i] is
//                                 (compressed block size - 1). Block i maps to
//                                 record i//16, sub-entry i%16; its offset is
//                                 baseOffset + sum(size[j]+1 for j < i%16),
//                                 relative to the compressed-data section.
//   [ name table section ]        length-prefixed names: 1 header byte
//                                 (len 0..0x7F), or 2 header bytes when MSB of
//                                 the first is set: len = (b0 & 0x7F)|(b1 << 7).
//   [ file tree section ]         FileDirectoryEntry[16 bytes]: u32
//                                 nameOffsetAndTypeFlag (MSB set -> file, lower
//                                 31 bits -> name table offset, 0x7FFFFFFF for
//                                 the unnamed root) + 3 u32 payload words.
//                                 File: offsetLow, sizeLow, offsetAndSizeHigh
//                                 (low 16 bits extend offset to 48 bits, high
//                                 16 bits extend size to 48 bits). Directory:
//                                 nodeStartIndex, count, reserved. Entry 0 is
//                                 the root directory; a directory's children
//                                 occupy indices [nodeStartIndex,
//                                 nodeStartIndex+count).
//   [ meta directory / meta data sections ]  (currently always empty)
//   [ footer, 144 bytes ]         6 x { u64 offset; u64 size } for the sections
//                                 above (in the order compressedData,
//                                 offsetRecords, names, fileTree, metaDirectory,
//                                 metaData), u8 integrityHash[32], u64 totalSize,
//                                 u32 version (0x61bf3a01), u32 magic
//                                 (0x169f52d6).
//
// The integrity hash is SHA-256 over the whole file up to the footer, followed
// by the serialized footer with its hash field zeroed.
//
// The writer (``ZarWriter`` / ``pack``) mirrors the reference
// ``ZArchiveWriter``: 64 KiB blocks zstd-compressed at level 6 (stored raw
// when compression does not shrink them), the final partial block
// zero-padded to a full block, directory entries sorted case-insensitively
// (ASCII fold, bytewise) within each directory, and the tree serialized
// breadth-first. Output is deterministic for a given source tree.
//
// Zstd support comes from the stdlib ``compression.zstd`` module on
// Python 3.14+, or the ``zstandard`` package on older interpreters (which
// the wheel pulls in automatically via an environment marker). The
// module-level bool ``HAVE_ZSTD`` reports availability (constructing a
// ``ZarReader`` or ``ZarWriter`` without either raises ``ZarNativeError``).
//
// Typing and runtime notes:
//   * Zstd: the C++ runtime has no zstd, so zar.cpp loads libzstd (1.5.7, the version Python 3.14 bundles; one-shot
//     ZSTD_compress at level 6 gives byte-identical frames to compression.zstd.compress) at start-up with
//     LoadLibrary: the file named by the environment variable CODEBREAKER_ZSTD_DLL, else "libzstd.dll" / "zstd.dll"
//     on the DLL search path, else the MSYS2 copies. HAVE_ZSTD is false when none loads, exactly as Python's
//     is when neither zstd module imports. zstd's errors (compression.zstd.ZstdError) are py::Exception here, with
//     CPython's messages.
//   * Python's generators (_walk, _iter_range, read_iter) are py::iter<T> over generator classes: lazy, with their
//     errors raised at the iteration that reaches them. They hold a pointer to the reader, which must outlive them
//     (Python's keep it alive by reference).
//   * A file's content is py::bytes; `append` takes py::bytes or py::bytearray (the Python takes any buffer).
//   * The data-section hash is py::hashlib (sha256 / hashlib.new for hash_walk: md5, sha1, sha256, sha512 only --
//     the runtime has no other algorithm; any other name is ValueError "unsupported hash type NAME").
//   * ZarWriter's "with" is __enter__ / __exit__(raised): `raised` is "an exception is leaving the with block".
//     Block compression runs on std::async tasks, at most 32 in flight (Python: a pool of up to 8 workers, 32 pending).
//   * cli / main take the argument list (sys.argv[1:]); argparse is reproduced for this one parser (usage,
//     -h/--help and its prefixes, the four error messages, exit status 2 = py::SystemExit).
//   * progress callbacks are std::function<void(int64_t, int64_t)>; an empty one is None.
//   * Python ints are unbounded; the on-disk u64 fields are uint64_t and are checked as Python's unbounded sums would
//     be; file offsets, sizes and block numbers (below 2**48) are int64_t.
#pragma once

#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <future>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "py.hpp"

namespace codebreaker::formats::xbox::zar {

// ``__all__ = ["ZarReader", "ZarWriter", "ZarNativeError", "pack", "HAVE_ZSTD"]``
extern const bool HAVE_ZSTD;   // zstd available (see the notes above)

inline constexpr int64_t _FOOTER_SIZE = 144;
inline constexpr int64_t _FOOTER_MAGIC = 0x169F52D6;
inline constexpr int64_t _FOOTER_VERSION1 = 0x61BF3A01;
inline constexpr int64_t _BLOCK_SIZE = 64 * 1024;
inline constexpr int64_t _ENTRIES_PER_RECORD = 16;
inline constexpr int64_t _RECORD_SIZE = 8 + 2 * _ENTRIES_PER_RECORD;   // 40
inline constexpr int64_t _ENTRY_SIZE = 16;
inline constexpr int64_t _ROOT_NAME_OFFSET = 0x7FFFFFFF;
inline constexpr int64_t _ZSTD_LEVEL = 6;            // matches the reference writer (StoreBlock, level 6)
inline constexpr int64_t _MAX_NAME_BYTES = 128;      // the reference reader misparses names >= 128 bytes
inline constexpr int64_t _MAX_FILE_FIELD = int64_t(1) << 48;   // file offset/size are 48-bit in FileDirectoryEntry
inline constexpr int64_t _PACK_CHUNK = int64_t(1) << 20;

// _FOOTER_STRUCT = ">12Q32sQ2I" (144 bytes), _RECORD_STRUCT = ">Q16H" (40), _ENTRY_STRUCT = ">4I" (16)

extern const py::str DESCRIPTION;   // "ZArchive (.zar) container: list/extract/hash/verify + pack a directory"

// Raised for any structural, I/O, or integrity problem with a .zar file.
struct ZarNativeError : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "ZarNativeError"; }
};

// one file-tree entry: the four big-endian u32 words
using Entry = std::array<int64_t, 4>;
// _walk's items: (path, entry index, is_file)
using WalkItem = std::tuple<py::str, int64_t, bool>;
// progress(done, total); empty = None
using Progress = std::function<void(int64_t, int64_t)>;

// The reference only case-folds ASCII A-Z.
py::str _lower_ascii(const py::str& s);

class ZarReader;

// Read-only, seekable file-like view of a single archived file (an io.RawIOBase).
class _ZarFileHandle {
public:
    _ZarFileHandle(ZarReader& reader, int64_t file_offset, int64_t file_size);

    bool readable() const;
    bool seekable() const;
    int64_t tell() const;
    int64_t seek(int64_t offset, int64_t whence = 0);   // os.SEEK_SET
    int64_t readinto(const py::bytearray& b);
    int64_t size() const;

    // RawIOBase's own: read(size) / readall() on top of readinto, close, closed, with
    py::bytes read(int64_t size = -1);
    py::bytes readall();
    void close();
    bool closed() const;
    _ZarFileHandle& __enter__();
    void __exit__();

private:
    ZarReader* _reader;
    int64_t _file_offset;
    int64_t _file_size;
    int64_t _pos;
    bool _closed = false;
};

// Reader for ZArchive (.zar) files.
//
//     ZarReader zr(path);
//     for (auto& [name, size] : zr.files()) ...
class ZarReader {
public:
    explicit ZarReader(const py::str& path, int64_t cache_blocks = 64);
    ~ZarReader();   // __del__: best-effort close
    ZarReader(const ZarReader&) = delete;
    ZarReader& operator=(const ZarReader&) = delete;

    // lifecycle
    void close();
    ZarReader& __enter__();
    void __exit__();

    // name table
    py::str _get_name(int64_t name_offset);

    // file tree
    static bool _entry_is_file(const Entry& entry);
    static int64_t _entry_file_offset(const Entry& entry);
    static int64_t _entry_file_size(const Entry& entry);
    // Yield (path, entry_index, is_file) depth-first in stored order.
    py::iter<WalkItem> _walk();
    // Resolve a path to a file-tree index (reference LookUp semantics).
    int64_t _lookup(const py::str& path);

    // block layer
    py::bytes _load_block(int64_t block_index);
    py::iter<py::bytes> _iter_range(int64_t virtual_offset, int64_t length);
    py::bytes _pread(int64_t virtual_offset, int64_t length);

    // public API
    // Return a list of (path, size) for every file in the archive.
    py::list<std::tuple<py::str, int64_t>> files();
    // Return a list of directory paths (excluding the root).
    py::list<py::str> directories();
    int64_t file_size(const py::str& path);
    // Yield the content of an archived file as a sequence of chunks.
    //
    // Chunks are naturally 64 KiB block-sized; ``chunk_size`` larger than a
    // block coalesces whole blocks per yield.
    py::iter<py::bytes> read_iter(const py::str& path, int64_t chunk_size = _BLOCK_SIZE);
    // Open an archived file as a read-only, seekable file-like object.
    _ZarFileHandle open_read(const py::str& path);
    // Extract the archive under out_dir. Returns the number of files.
    int64_t extract_all(const py::str& out_dir);
    // Return {path: hexdigest} for every file, streamed without writing.
    py::dict<py::str, py::str> hash_walk(const py::str& algorithm = py::str("sha1"), const Progress& progress = nullptr);
    // Check the embedded SHA-256 over the whole archive.
    //
    // The hash covers every byte up to the footer, plus the footer itself
    // with its 32-byte hash field zeroed. Returns True when it matches.
    bool verify_integrity(int64_t chunk_size = 8 * 1024 * 1024, const Progress& progress = nullptr);

private:
    void _parse(py::file& fh);

    py::str _path;
    int64_t _cache_limit;
    // the OrderedDict {block index: block}: most recently used last
    std::list<std::pair<int64_t, std::shared_ptr<const py::bytes>>> _cache_order;
    std::unordered_map<int64_t, std::list<std::pair<int64_t, std::shared_ptr<const py::bytes>>>::iterator> _cache_index;
    std::optional<py::file> _fh;

    int64_t _cdata_off = 0;
    uint64_t _cdata_size = 0;
    int64_t _file_size = 0;
    py::bytes _integrity_hash;
    py::bytes _footer_raw;
    py::list<uint64_t> _blk_off;
    py::list<int64_t> _blk_csize;
    int64_t _block_count = 0;
    py::bytes _name_table;
    py::list<Entry> _tree;
};

// ---------------------------------------------------------------------- writer

// ASCII-only case-fold table, mirroring the reference CompareNodeName().
extern const py::bytes _FOLD_TABLE;

py::bytes _fold_bytes(const py::bytes& raw);

struct _PathNode {
    py::bytes name;
    bool is_file;
    py::list<std::shared_ptr<_PathNode>> children;
    py::dict<py::bytes, std::shared_ptr<_PathNode>> lookup;   // folded name -> node
    int64_t file_offset = 0;
    int64_t file_size = 0;
    int64_t start_index = 0;
    int64_t name_off = 0;

    _PathNode(const py::bytes& name, bool is_file) : name(name), is_file(is_file) {}
};

// _compress_or_store(block): the zstd frame, or the block itself when it does not shrink it
py::bytes _compress_or_store(const py::bytes& block);

// Streaming writer for ZArchive (.zar) files.
//
//     ZarWriter zw(out_path);
//     zw.make_dir(str("dir"));
//     zw.start_file(str("dir/file.bin"));
//     zw.append(data);
//     zw.finalize();
//     zw.__exit__(false);
//
// File data is consumed incrementally (memory use is O(64 KiB block)
// plus the metadata tree). Leaving the ``with`` block without a
// successful ``finalize()`` deletes the partial output file.
class ZarWriter {
public:
    explicit ZarWriter(const py::str& path);
    ZarWriter(const ZarWriter&) = delete;
    ZarWriter& operator=(const ZarWriter&) = delete;

    // lifecycle
    void close();
    // Close and delete the (unfinished) output file.
    void abort();
    ZarWriter& __enter__();
    void __exit__(bool raised);

    // path/tree
    void _check_open();
    static py::list<py::bytes> _split(const py::str& path);
    std::shared_ptr<_PathNode> _ensure_dir(const py::list<py::bytes>& parts);
    // Create a directory (and any missing parents).
    void make_dir(const py::str& path);
    // Create a file entry; subsequent append() calls add its content.
    void start_file(const py::str& path);
    // Append bytes to the file opened by the last start_file().
    void append(const py::bytes& data);
    void append(const py::bytearray& data);

    // block layer
    void _write(const py::bytes& data);
    void _store_block(const py::bytes& block);
    void _flush_one();
    void _drain_blocks();
    void _append_raw(const py::bytes& data);

    // Write all metadata sections and the footer.
    void finalize();
    // Sort children and assign BFS indices; returns nodes in BFS order.
    py::list<std::shared_ptr<_PathNode>> _assign_indices();
    std::string _build_name_table(const py::list<std::shared_ptr<_PathNode>>& order);
    Entry _entry_words(const std::shared_ptr<_PathNode>& node);

private:
    py::str _path;
    std::shared_ptr<_PathNode> _root;
    std::shared_ptr<_PathNode> _current;
    std::string _buffer;   // the bytearray of the block being filled
    // [(base offset, [16 sizes - 1])]
    std::vector<std::pair<int64_t, std::array<int64_t, 16>>> _records;
    int64_t _block_count = 0;
    int64_t _out_offset = 0;
    int64_t _input_offset = 0;
    py::hashlib::Hash _sha;
    std::deque<std::future<py::bytes>> _pending;
    bool _finalized = false;
    std::optional<py::file> _fh;
};

// Pack a directory tree into a new .zar file. Returns the file count.
//
// Deterministic: entries are packed in the archive's canonical order
// (per-directory bytewise sort on ASCII-case-folded names), so packing
// the same tree twice yields byte-identical archives.
int64_t pack(const py::str& src_dir, const py::str& zar_path, const Progress& progress = nullptr);

// _pack_tree: `state` is the one-item list [bytes done so far]; empty `state` is None
int64_t _pack_tree(ZarWriter& zw, const py::str& dir_path, const py::str& arc_prefix, const Progress& progress = nullptr,
                   int64_t grand = 0, const py::list<int64_t>* state = nullptr);

// Standalone CLI:  xv-zar {list|extract|hash|verify|pack} archive.zar [dir]
// argv is the argument list (sys.argv[1:]); ap.error(...) is py::SystemExit(2), -h is py::SystemExit(0).
int64_t cli(const py::list<py::str>& argv);

int64_t main(const py::list<py::str>& argv);   // main(): cli(sys.argv[1:])

}  // namespace codebreaker::formats::xbox::zar
