// formats/deflate.hpp -- the one place the program talks to zlib: inflate and deflate of whole buffers.
//
// The readers of a format (a cabinet, a zip, a PNG, an ELF's compressed sections, a gzip stage) differ in what the
// stream is wrapped in and in what they make of a short or damaged stream, not in how it is inflated; they ask here and
// read the Status.
#pragma once

#include <cstddef>
#include <string>

#include "core/bytes.hpp"

namespace cb::formats::deflate {

// What surrounds the deflate data: a zlib header and checksum, nothing at all (a zip entry, an MSZIP block), or a gzip header.
enum class Wrap { zlib, raw, gzip };

enum class Status {
    end,      // the stream ended (zlib's Z_STREAM_END)
    ok,       // the input ran out first, or the output is full and the stream goes on
    buffer,   // no progress possible: more output needed than the buffer holds, or more input (Z_BUF_ERROR)
    bad,      // not a valid stream (a data, memory or dictionary error); `message` says what zlib said
};

struct Inflated {
    Bytes data;                  // what was produced
    Status status = Status::bad;
    std::size_t consumed = 0;    // input bytes read when it stopped
    std::string message;
};

// Inflate into a buffer of exactly `expected` bytes in one pass; data.size() is what was produced. More data in the
// stream than that gives Status::buffer (what fits is in `data`). An expected size of 0 is a stream that is not looked at.
Inflated inflate_exact(ByteView in, std::size_t expected, Wrap wrap = Wrap::zlib);

// Inflate however much the stream holds. A stream that is cut short gives Status::ok (what was read is in `data`), one
// that is damaged Status::bad. `dictionary` primes a raw stream (the preceding MSZIP block).
Inflated inflate_all(ByteView in, Wrap wrap = Wrap::zlib, ByteView dictionary = {});

// Deflate `in` at `level` (-1: zlib's default, 0..9) in one piece, primed with `dictionary` when it is not empty.
Bytes compress(ByteView in, int level = -1, Wrap wrap = Wrap::zlib, ByteView dictionary = {});

}  // namespace cb::formats::deflate
