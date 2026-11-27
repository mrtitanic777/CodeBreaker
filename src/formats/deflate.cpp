#include "formats/deflate.hpp"

#include <cstring>

#include "core/error.hpp"
#include "zlib.h"

namespace cb::formats::deflate {

namespace {

int window_bits(Wrap wrap) {
    switch (wrap) {
        case Wrap::raw: return -MAX_WBITS;
        case Wrap::gzip: return 16 + MAX_WBITS;
        case Wrap::zlib: break;
    }
    return MAX_WBITS;
}

Status status_of(int rc) {
    switch (rc) {
        case Z_STREAM_END: return Status::end;
        case Z_OK: return Status::ok;
        case Z_BUF_ERROR: return Status::buffer;
        default: return Status::bad;
    }
}

struct Inflater {
    z_stream z{};
    explicit Inflater(Wrap wrap) {
        if (inflateInit2(&z, window_bits(wrap)) != Z_OK) throw InternalError("zlib: cannot start inflating");
    }
    ~Inflater() { inflateEnd(&z); }
    Inflater(const Inflater&) = delete;
    Inflater& operator=(const Inflater&) = delete;
};

struct Deflater {
    z_stream z{};
    Deflater(int level, Wrap wrap) {
        if (deflateInit2(&z, level, Z_DEFLATED, window_bits(wrap), 8, Z_DEFAULT_STRATEGY) != Z_OK) throw InternalError("zlib: cannot start deflating");
    }
    ~Deflater() { deflateEnd(&z); }
    Deflater(const Deflater&) = delete;
    Deflater& operator=(const Deflater&) = delete;
};

}  // namespace

Inflated inflate_exact(ByteView in, std::size_t expected, Wrap wrap) {
    Inflated r;
    if (expected == 0) {
        r.status = Status::end;
        return r;
    }
    Inflater s(wrap);
    r.data.resize(expected);
    s.z.next_in = const_cast<Bytef*>(in.data());
    s.z.avail_in = static_cast<uInt>(in.size());
    s.z.next_out = r.data.data();
    s.z.avail_out = static_cast<uInt>(r.data.size());
    const int rc = inflate(&s.z, Z_FINISH);
    r.status = status_of(rc);
    r.data.resize(r.data.size() - s.z.avail_out);
    r.consumed = in.size() - s.z.avail_in;
    if (r.status == Status::bad && s.z.msg) r.message = s.z.msg;
    return r;
}

Inflated inflate_all(ByteView in, Wrap wrap, ByteView dictionary) {
    Inflated r;
    Inflater s(wrap);
    if (!dictionary.empty() && inflateSetDictionary(&s.z, dictionary.data(), static_cast<uInt>(dictionary.size())) != Z_OK) {
        throw InternalError("zlib: cannot set the dictionary");
    }
    s.z.next_in = const_cast<Bytef*>(in.data());
    s.z.avail_in = static_cast<uInt>(in.size());
    Bytes chunk(1 << 16);
    int rc = Z_OK;
    do {
        s.z.next_out = chunk.data();
        s.z.avail_out = static_cast<uInt>(chunk.size());
        rc = inflate(&s.z, Z_SYNC_FLUSH);
        r.data.insert(r.data.end(), chunk.begin(), chunk.end() - static_cast<std::ptrdiff_t>(s.z.avail_out));
        if (rc == Z_BUF_ERROR && s.z.avail_out != 0) {   // the input ran out: what was inflated is what there is
            rc = Z_OK;
            s.z.avail_in = 0;
        }
    } while (rc == Z_OK && (s.z.avail_in > 0 || s.z.avail_out == 0));
    r.status = status_of(rc);
    r.consumed = in.size() - s.z.avail_in;
    if (r.status == Status::bad && s.z.msg) r.message = s.z.msg;
    return r;
}

Bytes compress(ByteView in, int level, Wrap wrap, ByteView dictionary) {
    Deflater s(level, wrap);
    if (!dictionary.empty()) deflateSetDictionary(&s.z, dictionary.data(), static_cast<uInt>(dictionary.size()));
    Bytes out(deflateBound(&s.z, static_cast<uLong>(in.size())) + 64);
    s.z.next_in = const_cast<Bytef*>(in.data());
    s.z.avail_in = static_cast<uInt>(in.size());
    s.z.next_out = out.data();
    s.z.avail_out = static_cast<uInt>(out.size());
    if (::deflate(&s.z, Z_FINISH) != Z_STREAM_END) throw InternalError("zlib: deflate did not finish");
    out.resize(out.size() - s.z.avail_out);
    return out;
}

}  // namespace cb::formats::deflate
