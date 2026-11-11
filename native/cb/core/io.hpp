// cb/core/io.hpp -- bounds-checked reading and writing of binary data.
//
// ByteReader is a cursor over a ByteView; it replaces struct.unpack. Little-endian is the default (that is what
// PE, NE, LE, DOS and x86 use); the `_be` forms read big-endian (PowerPC, Xbox 360, ID3, network order). A read
// past the end throws cb::TruncatedError with the reader's name, the offset and the size wanted. A reader that
// must answer with less on damaged input uses the `try_` forms, which return std::optional instead.
//
// ByteWriter is the builder counterpart (little-endian default, `_be` forms), with patch-in-place for the
// "write the size after the body" idiom.
//
// checked_range() is how every offset+length that came from an untrusted file is turned into a view:
// overflow-safe, and a FormatError when the file lies.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::io {

// The sub-view [offset, offset+length) of `data`. Throws FormatError (reader "range") if it does not lie
// entirely inside. Arithmetic cannot overflow.
ByteView checked_range(ByteView data, std::uint64_t offset, std::uint64_t length);
// The same, but absent rather than an exception.
std::optional<ByteView> try_range(ByteView data, std::uint64_t offset, std::uint64_t length);

class ByteReader {
public:
    // `name` says what is being read ("pe", "ne.segment table"); it is in every error message.
    explicit ByteReader(ByteView data, std::string_view name = "data") : data_(data), name_(name) {}

    std::size_t size() const { return data_.size(); }
    std::size_t pos() const { return pos_; }
    std::size_t remaining() const { return data_.size() - pos_; }
    bool eof() const { return pos_ >= data_.size(); }
    ByteView data() const { return data_; }

    // Move the cursor. seek() to the size itself is allowed (an empty remainder); beyond it is an error.
    void seek(std::uint64_t offset);
    void skip(std::uint64_t count);
    // Skip to the next multiple of `alignment` (a power of two) measured from the start of the data.
    void align(std::size_t alignment);

    // Little-endian reads (the default).
    std::uint8_t u8() { return read_le<std::uint8_t>(); }
    std::uint16_t u16() { return read_le<std::uint16_t>(); }
    std::uint32_t u32() { return read_le<std::uint32_t>(); }
    std::uint64_t u64() { return read_le<std::uint64_t>(); }
    std::int8_t i8() { return read_le<std::int8_t>(); }
    std::int16_t i16() { return read_le<std::int16_t>(); }
    std::int32_t i32() { return read_le<std::int32_t>(); }
    std::int64_t i64() { return read_le<std::int64_t>(); }
    // Big-endian reads.
    std::uint16_t u16_be() { return read_be<std::uint16_t>(); }
    std::uint32_t u32_be() { return read_be<std::uint32_t>(); }
    std::uint64_t u64_be() { return read_be<std::uint64_t>(); }
    std::int16_t i16_be() { return read_be<std::int16_t>(); }
    std::int32_t i32_be() { return read_be<std::int32_t>(); }
    std::int64_t i64_be() { return read_be<std::int64_t>(); }

    // A real-mode far pointer as stored in memory: offset word first, then segment word.
    Far16 far16();

    // The next n bytes as a view into the data (no copy); the view lives as long as the data does.
    ByteView bytes(std::uint64_t n);
    // The same bytes as an owned buffer.
    Bytes bytes_copy(std::uint64_t n);
    // A NUL-terminated string (the NUL is consumed, not returned). TruncatedError if there is no NUL.
    // The bytes are returned as they are; decode with cb::text::from_latin1 / from_cp437 when it is text.
    ByteView cstr();
    // A reader over the next n bytes; this reader advances past them.
    ByteReader sub(std::uint64_t n, std::string_view name = {});

    // The look-ahead forms: read without moving.
    std::optional<std::uint8_t> peek_u8() const;
    std::optional<std::uint16_t> peek_u16() const;
    std::optional<std::uint32_t> peek_u32() const;

    // The `try_` forms: nullopt (and the cursor unmoved) when the data is too short.
    std::optional<std::uint8_t> try_u8() { return try_read<std::uint8_t, false>(); }
    std::optional<std::uint16_t> try_u16() { return try_read<std::uint16_t, false>(); }
    std::optional<std::uint32_t> try_u32() { return try_read<std::uint32_t, false>(); }
    std::optional<std::uint64_t> try_u64() { return try_read<std::uint64_t, false>(); }
    std::optional<std::uint16_t> try_u16_be() { return try_read<std::uint16_t, true>(); }
    std::optional<std::uint32_t> try_u32_be() { return try_read<std::uint32_t, true>(); }
    std::optional<ByteView> try_bytes(std::uint64_t n);

    // Generic forms, for code that is itself generic over the integer type.
    template <class T> T read_le() { return load<T, false>(take(sizeof(T))); }
    template <class T> T read_be() { return load<T, true>(take(sizeof(T))); }

private:
    const std::uint8_t* take(std::uint64_t n);

    template <class T, bool Big>
    static T load(const std::uint8_t* p) {
        using U = std::make_unsigned_t<T>;
        U v = 0;
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            const std::size_t shift = 8 * (Big ? sizeof(T) - 1 - i : i);
            v = static_cast<U>(v | (static_cast<U>(p[i]) << shift));
        }
        return static_cast<T>(v);
    }

    template <class T, bool Big>
    std::optional<T> try_read() {
        if (remaining() < sizeof(T)) return std::nullopt;
        return load<T, Big>(take(sizeof(T)));
    }

    ByteView data_;
    std::string name_;
    std::size_t pos_ = 0;
};

class ByteWriter {
public:
    std::size_t size() const { return buf_.size(); }
    const Bytes& data() const { return buf_; }
    // Hand the buffer over (the writer is empty afterwards).
    Bytes take() { Bytes out = std::move(buf_); buf_.clear(); return out; }

    void u8(std::uint8_t v) { put_le(v); }
    void u16(std::uint16_t v) { put_le(v); }
    void u32(std::uint32_t v) { put_le(v); }
    void u64(std::uint64_t v) { put_le(v); }
    void i8(std::int8_t v) { put_le(static_cast<std::uint8_t>(v)); }
    void i16(std::int16_t v) { put_le(static_cast<std::uint16_t>(v)); }
    void i32(std::int32_t v) { put_le(static_cast<std::uint32_t>(v)); }
    void i64(std::int64_t v) { put_le(static_cast<std::uint64_t>(v)); }
    void u16_be(std::uint16_t v) { put_be(v); }
    void u32_be(std::uint32_t v) { put_be(v); }
    void u64_be(std::uint64_t v) { put_be(v); }
    void far16(Far16 p) { u16(p.off); u16(p.seg); }

    void bytes(ByteView v) { append(buf_, v); }
    // The text's bytes followed by a NUL.
    void cstr(std::string_view s) { bytes(as_bytes(s)); u8(0); }
    void zeros(std::size_t n) { buf_.insert(buf_.end(), n, std::uint8_t{0}); }
    // Pad with zeros to the next multiple of `alignment`.
    void align(std::size_t alignment);

    // Overwrite already-written bytes (to fill in a size or offset once it is known).
    void patch_u16(std::size_t offset, std::uint16_t v);
    void patch_u32(std::size_t offset, std::uint32_t v);

private:
    template <class T> void put_le(T v) {
        for (std::size_t i = 0; i < sizeof(T); ++i) buf_.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
    }
    template <class T> void put_be(T v) {
        for (std::size_t i = sizeof(T); i-- > 0;) buf_.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
    }
    Bytes buf_;
};

}  // namespace cb::io
