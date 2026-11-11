// py_zlib.hpp -- Python's zlib module over the vendored zlib 1.3.1 (native/decompiler/core/zlib).
// Link native/ported/src/py_zlib.cc and the C sources listed in native/ported/src/zlib_sources.txt.
//   compress / decompress / crc32 / adler32 / compressobj / decompressobj; zlib.error with CPython's text.
// CPython 3.14 on Windows is built on zlib-ng; this is classic zlib 1.3.1. Decompression behaves the same;
// compressed bytes at levels 1-9 follow zlib 1.3.1's deflate (level 0 is stored blocks in both).
#pragma once
#include "py.hpp"

namespace py {
namespace zlib {

struct error : Exception {   // zlib.error
    error() : Exception() {}
    explicit error(const std::string& m) : Exception(m) {}
    explicit error(const char* m) : Exception(m) {}
    const char* type() const override { return "error"; }
};

inline constexpr int64_t MAX_WBITS = 15, DEFLATED = 8, DEF_MEM_LEVEL = 8, DEF_BUF_SIZE = 16384;
inline constexpr int64_t Z_NO_COMPRESSION = 0, Z_BEST_SPEED = 1, Z_BEST_COMPRESSION = 9, Z_DEFAULT_COMPRESSION = -1;
inline constexpr int64_t Z_FILTERED = 1, Z_HUFFMAN_ONLY = 2, Z_RLE = 3, Z_FIXED = 4, Z_DEFAULT_STRATEGY = 0;
inline constexpr int64_t Z_NO_FLUSH = 0, Z_PARTIAL_FLUSH = 1, Z_SYNC_FLUSH = 2, Z_FULL_FLUSH = 3, Z_FINISH = 4;

inline int64_t crc32(const bytes& b, int64_t value = 0) { return py::crc32(b, value); }
inline int64_t adler32(const bytes& b, int64_t value = 1) {
    uint32_t a = static_cast<uint32_t>(value) & 0xFFFF, s = (static_cast<uint32_t>(value) >> 16) & 0xFFFF;
    for (unsigned char x : b.raw()) { a = (a + x) % 65521u; s = (s + a) % 65521u; }
    return static_cast<int64_t>((s << 16) | a);
}

bytes compress(const bytes& data, int64_t level = Z_DEFAULT_COMPRESSION, int64_t wbits = MAX_WBITS);
bytes decompress(const bytes& data, int64_t wbits = MAX_WBITS, int64_t bufsize = DEF_BUF_SIZE);

struct stream_;   // (a z_stream, kept out of this header)

// zlib.compressobj(level, method, wbits, memLevel, strategy)
class compressobj {
  public:
    explicit compressobj(int64_t level = Z_DEFAULT_COMPRESSION, int64_t method = DEFLATED, int64_t wbits = MAX_WBITS,
                         int64_t memLevel = DEF_MEM_LEVEL, int64_t strategy = Z_DEFAULT_STRATEGY);
    compressobj(const compressobj&) = delete;
    compressobj& operator=(const compressobj&) = delete;
    compressobj(compressobj&& o) noexcept : z_(o.z_) { o.z_ = nullptr; }
    compressobj& operator=(compressobj&& o) noexcept {
        if (this != &o) { reset_(); z_ = o.z_; o.z_ = nullptr; }
        return *this;
    }
    ~compressobj();
    bytes compress(const bytes& data);
    bytes flush(int64_t mode = Z_FINISH);

  private:
    void reset_();
    stream_* z_;
};

// zlib.decompressobj(wbits)
class decompressobj {
  public:
    explicit decompressobj(int64_t wbits = MAX_WBITS);
    decompressobj(const decompressobj&) = delete;
    decompressobj& operator=(const decompressobj&) = delete;
    decompressobj(decompressobj&& o) noexcept
        : unused_data(std::move(o.unused_data)), unconsumed_tail(std::move(o.unconsumed_tail)), eof(o.eof), needs_input(o.needs_input), z_(o.z_) {
        o.z_ = nullptr;
    }
    decompressobj& operator=(decompressobj&& o) noexcept {
        if (this != &o) {
            reset_();
            unused_data = std::move(o.unused_data);
            unconsumed_tail = std::move(o.unconsumed_tail);
            eof = o.eof;
            needs_input = o.needs_input;
            z_ = o.z_;
            o.z_ = nullptr;
        }
        return *this;
    }
    ~decompressobj();
    bytes decompress(const bytes& data, int64_t max_length = 0);
    bytes flush(int64_t length = DEF_BUF_SIZE);
    bytes unused_data;       // what follows the end of the stream
    bytes unconsumed_tail;   // input left over because of max_length
    bool eof = false;
    bool needs_input = true;

  private:
    void reset_();
    stream_* z_;
};

}  // namespace zlib

// gzip.decompress(data): all the members of a gzip stream. BadGzipFile (an OSError), EOFError, zlib.error as CPython's.
namespace gzip {
struct BadGzipFile : OSError {
    BadGzipFile() : OSError() {}
    explicit BadGzipFile(const std::string& m) : OSError(m) {}
    explicit BadGzipFile(const char* m) : OSError(m) {}
    const char* type() const override { return "BadGzipFile"; }
};
bytes decompress(const bytes& data);
}  // namespace gzip
}  // namespace py
