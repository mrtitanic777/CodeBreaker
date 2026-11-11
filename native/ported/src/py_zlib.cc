// py_zlib.cc -- Python's zlib module over zlib 1.3.1; see py_zlib.hpp.
#include "py_zlib.hpp"

#include <cstring>

#include "zlib.h"

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
compressobj::~compressobj() {
    if (z_ && z_->init) deflateEnd(&z_->zs);
    delete z_;
}
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
        deflateEnd(&z_->zs);
        z_->init = false;
        // (a finished object: a second flush(Z_FINISH) or compress() fails, as CPython's does after deflateEnd)
        z_->init = false;
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
decompressobj::~decompressobj() {
    if (z_ && z_->init) inflateEnd(&z_->zs);
    delete z_;
}

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
}  // namespace py
