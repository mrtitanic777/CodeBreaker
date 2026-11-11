// py_zlib.cc -- Python's zlib module over zlib 1.3.1; see py_zlib.hpp.
#include "py_zlib.hpp"

#include <cstring>

#include "../../decompiler/core/zlib/zlib.h"

namespace py {
namespace zlib {

struct stream_ {
    z_stream zs;
    bool init = false;
};

namespace {

// zlib_error() of CPython's zlibmodule.c: "Error N while <what>: <zlib's message>"
[[noreturn]] void raise_(const z_stream& zs, int err, const char* what) {
    if (err == Z_MEM_ERROR) throw MemoryError("Out of memory");
    const char* zmsg = nullptr;
    if (err == Z_VERSION_ERROR) zmsg = "library version mismatch";
    if (!zmsg) zmsg = zs.msg;
    if (!zmsg) {
        switch (err) {
            case Z_BUF_ERROR: zmsg = "incomplete or truncated stream"; break;
            case Z_STREAM_ERROR: zmsg = "inconsistent stream state"; break;
            case Z_DATA_ERROR: zmsg = "invalid input data"; break;
        }
    }
    std::string m = "Error " + std::to_string(err) + " " + what;
    if (zmsg) m += ": " + std::string(zmsg, strnlen(zmsg, 200));
    throw error(m);
}

uInt clamp_uint_(size_t n) { return n > 0x7FFFFFFFu ? 0x7FFFFFFFu : static_cast<uInt>(n); }
Bytef* bp_(const std::string& s, size_t at = 0) { return reinterpret_cast<Bytef*>(const_cast<char*>(s.data())) + at; }

// a growing output: room for the next call
struct Out {
    std::string s;
    size_t first;
    explicit Out(size_t first_chunk) : first(first_chunk) {}
    void arrange(z_stream& zs, size_t cap = SIZE_MAX) {
        size_t at = s.size();
        size_t chunk = at ? at : first;
        if (chunk > (size_t(1) << 28)) chunk = size_t(1) << 28;
        if (chunk > cap) chunk = cap;
        s.resize(at + chunk);
        zs.next_out = reinterpret_cast<Bytef*>(&s[at]);
        zs.avail_out = clamp_uint_(chunk);
    }
    void settle(const z_stream& zs) { s.resize(s.size() - zs.avail_out); }
};

}  // namespace

bytes compress(const bytes& data, int64_t level, int64_t wbits) {
    z_stream zs;
    std::memset(&zs, 0, sizeof zs);
    int err = deflateInit2(&zs, static_cast<int>(level), Z_DEFLATED, static_cast<int>(wbits), static_cast<int>(DEF_MEM_LEVEL),
                           Z_DEFAULT_STRATEGY);
    if (err == Z_MEM_ERROR) throw MemoryError("Out of memory while compressing data");
    if (err == Z_STREAM_ERROR) throw error("Bad compression level");
    if (err != Z_OK) { deflateEnd(&zs); raise_(zs, err, "while compressing data"); }
    const std::string& in = data.raw();
    Out out(16384);
    size_t fed = clamp_uint_(in.size());
    zs.next_in = bp_(in);
    zs.avail_in = static_cast<uInt>(fed);
    for (;;) {
        out.arrange(zs);
        err = deflate(&zs, fed == in.size() ? Z_FINISH : Z_NO_FLUSH);
        out.settle(zs);
        if (err == Z_STREAM_END) break;
        if (err != Z_OK && err != Z_BUF_ERROR) { deflateEnd(&zs); raise_(zs, err, "while compressing data"); }
        if (zs.avail_in == 0 && fed < in.size()) {   // (inputs of 2 GB and more feed in pieces)
            size_t more = clamp_uint_(in.size() - fed);
            zs.next_in = bp_(in, fed);
            zs.avail_in = static_cast<uInt>(more);
            fed += more;
        }
    }
    deflateEnd(&zs);
    return bytes(std::move(out.s));
}

bytes decompress(const bytes& data, int64_t wbits, int64_t bufsize) {
    if (bufsize < 0) throw ValueError("bufsize must be non-negative");
    if (bufsize == 0) bufsize = 1;
    z_stream zs;
    std::memset(&zs, 0, sizeof zs);
    int err = inflateInit2(&zs, static_cast<int>(wbits));
    if (err == Z_MEM_ERROR) throw MemoryError("Out of memory while decompressing data");
    if (err != Z_OK) { inflateEnd(&zs); raise_(zs, err, "while preparing to decompress data"); }
    const std::string& in = data.raw();
    Out out(static_cast<size_t>(bufsize));
    size_t fed = clamp_uint_(in.size());
    zs.next_in = bp_(in);
    zs.avail_in = static_cast<uInt>(fed);
    for (;;) {
        out.arrange(zs);
        err = inflate(&zs, Z_FINISH);
        out.settle(zs);
        if (err == Z_STREAM_END) break;
        if ((err == Z_OK || err == Z_BUF_ERROR) && zs.avail_out == 0) continue;
        if (err == Z_BUF_ERROR && zs.avail_in == 0 && fed < in.size()) {
            size_t more = clamp_uint_(in.size() - fed);
            zs.next_in = bp_(in, fed);
            zs.avail_in = static_cast<uInt>(more);
            fed += more;
            continue;
        }
        z_stream snap = zs;
        const char* keep = zs.msg;
        snap.msg = const_cast<char*>(keep);
        std::string text;
        try {
            raise_(snap, err, "while decompressing data");
        } catch (...) {
            inflateEnd(&zs);
            throw;
        }
    }
    err = inflateEnd(&zs);
    if (err != Z_OK) raise_(zs, err, "while finishing decompression");
    return bytes(std::move(out.s));
}

// ------------------------------------------------------------------------------------------ compressobj
compressobj::compressobj(int64_t level, int64_t method, int64_t wbits, int64_t memLevel, int64_t strategy) : z_(new stream_) {
    std::memset(&z_->zs, 0, sizeof z_->zs);
    int err = deflateInit2(&z_->zs, static_cast<int>(level), static_cast<int>(method), static_cast<int>(wbits),
                           static_cast<int>(memLevel), static_cast<int>(strategy));
    if (err == Z_MEM_ERROR) { delete z_; throw MemoryError("Can't allocate memory for compression object"); }
    if (err == Z_STREAM_ERROR) { delete z_; throw ValueError("Invalid initialization option"); }
    if (err != Z_OK) { z_stream zs = z_->zs; delete z_; raise_(zs, err, "while creating compression object"); }
    z_->init = true;
}
void compressobj::reset_() {
    if (z_ && z_->init) deflateEnd(&z_->zs);
    delete z_;
    z_ = nullptr;
}
compressobj::~compressobj() { reset_(); }
bytes compressobj::compress(const bytes& data) {
    const std::string& in = data.raw();
    Out out(16384);
    z_->zs.next_in = bp_(in);
    z_->zs.avail_in = clamp_uint_(in.size());
    do {
        out.arrange(z_->zs);
        int err = deflate(&z_->zs, Z_NO_FLUSH);
        out.settle(z_->zs);
        if (err == Z_STREAM_ERROR) raise_(z_->zs, err, "while compressing data");
    } while (z_->zs.avail_out == 0);
    return bytes(std::move(out.s));
}
bytes compressobj::flush(int64_t mode) {
    if (mode == Z_NO_FLUSH) return bytes();
    Out out(16384);
    z_->zs.next_in = nullptr;
    z_->zs.avail_in = 0;
    int err;
    do {
        out.arrange(z_->zs);
        err = deflate(&z_->zs, static_cast<int>(mode));
        out.settle(z_->zs);
        if (err == Z_STREAM_ERROR) raise_(z_->zs, err, "while flushing");
    } while (z_->zs.avail_out == 0);
    if (mode == Z_FINISH && err == Z_STREAM_END) {
        // (finished: the stream is closed with the object)
    } else if (err != Z_OK && err != Z_BUF_ERROR) {
        raise_(z_->zs, err, "while flushing");
    }
    return bytes(std::move(out.s));
}

// ------------------------------------------------------------------------------------------ decompressobj
decompressobj::decompressobj(int64_t wbits) : z_(new stream_) {
    std::memset(&z_->zs, 0, sizeof z_->zs);
    int err = inflateInit2(&z_->zs, static_cast<int>(wbits));
    if (err == Z_STREAM_ERROR) { delete z_; throw ValueError("Invalid initialization option"); }
    if (err == Z_MEM_ERROR) { delete z_; throw MemoryError("Can't allocate memory for decompression object"); }
    if (err != Z_OK) { z_stream zs = z_->zs; delete z_; raise_(zs, err, "while creating decompression object"); }
    z_->init = true;
}
void decompressobj::reset_() {
    if (z_ && z_->init) inflateEnd(&z_->zs);
    delete z_;
    z_ = nullptr;
}
decompressobj::~decompressobj() { reset_(); }

bytes decompressobj::decompress(const bytes& data, int64_t max_length) {
    if (max_length < 0) throw ValueError("max_length must be non-negative");
    if (eof) {   // past the end of the stream, everything is unused_data
        unused_data = unused_data + data;
        return bytes();
    }
    const std::string& in = data.raw();
    Out out(max_length > 0 && max_length < 16384 ? static_cast<size_t>(max_length) : 16384);
    size_t limit = max_length == 0 ? SIZE_MAX : static_cast<size_t>(max_length);
    z_stream& zs = z_->zs;
    size_t fed = clamp_uint_(in.size());
    zs.next_in = bp_(in);
    zs.avail_in = static_cast<uInt>(fed);
    int err = Z_OK;
    while (out.s.size() < limit) {
        out.arrange(zs, limit - out.s.size());
        err = inflate(&zs, Z_SYNC_FLUSH);
        out.settle(zs);
        if (err == Z_STREAM_END) break;
        if (err != Z_OK && err != Z_BUF_ERROR) raise_(zs, err, "while decompressing data");
        if (zs.avail_out == 0) continue;
        if (zs.avail_in == 0 && fed < in.size()) {
            size_t more = clamp_uint_(in.size() - fed);
            zs.next_in = bp_(in, fed);
            zs.avail_in = static_cast<uInt>(more);
            fed += more;
            continue;
        }
        break;
    }
    size_t used = in.size() - zs.avail_in;
    if (err == Z_STREAM_END) {
        eof = true;
        unused_data = unused_data + bytes(in.substr(used));
        unconsumed_tail = bytes();
        needs_input = false;
    } else {
        unconsumed_tail = bytes(in.substr(used));
        needs_input = !(max_length != 0 && out.s.size() >= limit) && true;
    }
    return bytes(std::move(out.s));
}

bytes decompressobj::flush(int64_t length) {
    if (length <= 0) throw ValueError("length must be greater than zero");
    const std::string in = unconsumed_tail.raw();
    Out out(static_cast<size_t>(length));
    z_stream& zs = z_->zs;
    zs.next_in = bp_(in);
    zs.avail_in = clamp_uint_(in.size());
    int err;
    for (;;) {
        out.arrange(zs);
        err = inflate(&zs, Z_FINISH);
        out.settle(zs);
        if (err == Z_STREAM_END) break;
        if ((err == Z_OK || err == Z_BUF_ERROR) && zs.avail_out == 0) continue;
        if (err == Z_OK || err == Z_BUF_ERROR) break;
        raise_(zs, err, "while flushing");
    }
    size_t used = in.size() - zs.avail_in;
    if (err == Z_STREAM_END) {
        eof = true;
        unused_data = unused_data + bytes(in.substr(used));
        unconsumed_tail = bytes();
    } else {
        unconsumed_tail = bytes(in.substr(used));
    }
    return bytes(std::move(out.s));
}

}  // namespace zlib

// ------------------------------------------------------------------------------------------ gzip
namespace gzip {

namespace {
constexpr int FHCRC = 2, FEXTRA = 4, FNAME = 8, FCOMMENT = 16;

// gzip._read_gzip_header over data: false at its end (no header), else `pos` is past the header
bool read_gzip_header(const std::string& data, size_t& pos) {
    auto read_exact = [&](size_t n) {
        if (data.size() - pos < n) throw EOFError("Compressed file ended before the end-of-stream marker was reached");
        size_t at = pos;
        pos += n;
        return at;
    };
    if (pos >= data.size()) return false;
    std::string magic = data.substr(pos, 2);
    pos += magic.size();
    if (magic != "\x1f\x8b") throw BadGzipFile("Not a gzipped file (" + repr(bytes(magic)).utf8() + ")");
    size_t h = read_exact(8);
    int method = static_cast<unsigned char>(data[h]);
    int flag = static_cast<unsigned char>(data[h + 1]);
    if (method != 8) throw BadGzipFile("Unknown compression method");
    if (flag & FEXTRA) {
        size_t e = read_exact(2);
        size_t extra_len = static_cast<unsigned char>(data[e]) | (static_cast<size_t>(static_cast<unsigned char>(data[e + 1])) << 8);
        read_exact(extra_len);
    }
    if (flag & FNAME) {   // (read until the zero byte or the end of the data)
        while (pos < data.size() && data[pos++] != '\0') {
        }
    }
    if (flag & FCOMMENT) {
        while (pos < data.size() && data[pos++] != '\0') {
        }
    }
    if (flag & FHCRC) read_exact(2);
    return true;
}

uint32_t le32_(const std::string& s, size_t at) {
    return static_cast<unsigned char>(s[at]) | (static_cast<uint32_t>(static_cast<unsigned char>(s[at + 1])) << 8) |
           (static_cast<uint32_t>(static_cast<unsigned char>(s[at + 2])) << 16) | (static_cast<uint32_t>(static_cast<unsigned char>(s[at + 3])) << 24);
}
}  // namespace

bytes decompress(const bytes& data_in) {
    std::string members;
    std::string data = data_in.raw();
    for (;;) {
        size_t pos = 0;
        if (!read_gzip_header(data, pos)) return bytes(members);
        zlib::decompressobj d(-15);   // (-MAX_WBITS: a raw deflate stream)
        bytes decompressed = d.decompress(bytes(data.substr(pos)));
        if (!d.eof || d.unused_data.size() < 8) throw EOFError("Compressed file ended before the end-of-stream marker was reached");
        const std::string& tail = d.unused_data.raw();
        if (le32_(tail, 0) != static_cast<uint32_t>(zlib::crc32(decompressed))) throw BadGzipFile("CRC check failed");
        if (le32_(tail, 4) != static_cast<uint32_t>(decompressed.size() & 0xffffffffu)) throw BadGzipFile("Incorrect length of data produced");
        members += decompressed.raw();
        size_t start = 8;   // do.unused_data[8:].lstrip(b"\x00")
        while (start < tail.size() && tail[start] == '\0') ++start;
        data = tail.substr(start);
    }
}

}  // namespace gzip
}  // namespace py

// ---------------------------------------------------------------------------------------------
// The zlib 1.3.1 sources themselves (native/decompiler/core/zlib), compiled here as C++
// so a port links this one file. (After the code above: their macros stay out of it.)
#include "../../decompiler/core/zlib/adler32.c"
#include "../../decompiler/core/zlib/deflate.c"
#include "../../decompiler/core/zlib/inffast.c"
#include "../../decompiler/core/zlib/inflate.c"
#include "../../decompiler/core/zlib/inftrees.c"
#include "../../decompiler/core/zlib/trees.c"
#include "../../decompiler/core/zlib/zutil.c"

// crc32() for deflate's gzip wrapper and inflate's check (the decompiler copy of zlib carries no crc32.c)
uLong ZEXPORT crc32(uLong crc, const Bytef* buf, uInt len) {
    if (buf == Z_NULL) return 0;
    return static_cast<uLong>(static_cast<uint32_t>(py::crc32(py::bytes(reinterpret_cast<const char*>(buf), len), static_cast<int64_t>(crc))));
}
