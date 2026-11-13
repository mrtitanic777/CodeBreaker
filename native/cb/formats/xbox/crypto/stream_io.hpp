// cb/formats/xbox/crypto/stream_io.hpp -- the endian-aware byte stream the Xbox 360 crypto tools read and write images through.
//
//     StreamIO image(bytes, Endian::Big);              a stream over a copy of the bytes
//     StreamIO file = StreamIO::open(path);            a stream over a file, edited in place (created when it is not there)
//     std::uint32_t magic = image.read_u32();          integers at the position, in the stream's byte order
//     image.write_u16_at(0x10, 7);                     or at an offset, the position left where it was
//     image.set_label("body"); ...; image.goto_label("body", 4);   named positions
//     image.perform_function_at(0x40, 0x100, decrypt); a function applied to the bytes at an offset, in place
//
// A stream has one position, shared by every read and write. Reads of integers, structures and `read_exact` throw
// TruncatedError when the stream ends first; `read` and `read_at` answer what is there (fewer bytes at the end, none past it), as
// a file does. A write past the end grows the stream, zero-filled between.
#pragma once

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <type_traits>

#include "cb/core/bytes.hpp"
#include "cb/core/random_file.hpp"

namespace cb::formats::xbox::crypto {

enum class Endian { Little, Big };

enum class Whence { Set, Current, End };

class StreamIO {
public:
    // An empty stream in memory.
    explicit StreamIO(Endian endian = Endian::Little) : endian_(endian) {}
    // A stream over a copy of `data`.
    explicit StreamIO(ByteView data, Endian endian = Endian::Little) : endian_(endian), memory_(data.begin(), data.end()) {}
    // A stream over the file at `path`, read and written in place; a file that is not there is created. IoError otherwise.
    static StreamIO open(const std::filesystem::path& path, Endian endian = Endian::Little);

    StreamIO(StreamIO&&) noexcept = default;
    StreamIO& operator=(StreamIO&&) noexcept = default;
    StreamIO(const StreamIO&) = delete;
    StreamIO& operator=(const StreamIO&) = delete;

    Endian endian() const { return endian_; }
    void set_endian(Endian endian) { endian_ = endian; }

    // ---- position ----
    std::uint64_t position() const { return position_; }
    // Moves the position and answers the new one. UsageError for a position before the start; one past the end is fine.
    std::uint64_t seek(std::int64_t offset, Whence whence = Whence::Set);
    void set_position(std::uint64_t at) { position_ = at; }
    std::uint64_t length() const;
    // All of the stream's bytes (a file is read from disk), the position untouched.
    Bytes contents() const;

    // ---- named positions ----
    void set_label(std::string name) { labels_[std::move(name)] = position_; }
    void set_label(std::string name, std::uint64_t at) { labels_[std::move(name)] = at; }
    // The position named; UsageError for a name never set.
    std::uint64_t label(const std::string& name) const;
    // Moves to the named position plus `delta`.
    std::uint64_t goto_label(const std::string& name, std::int64_t delta = 0);

    // ---- bytes ----
    // Up to `count` bytes at the position (fewer at the end of the stream), moving past them.
    Bytes read(std::size_t count);
    // Exactly `count` bytes; TruncatedError when the stream ends first.
    Bytes read_exact(std::size_t count);
    Bytes read_at(std::uint64_t offset, std::size_t count) const;   // the position untouched
    void write(ByteView data);
    void write_at(std::uint64_t offset, ByteView data);             // the position untouched
    // Applies `function` to the `size` bytes at `offset`, writes what it returns back there and answers it; the position is
    // left where it was. The function may return a different number of bytes.
    Bytes perform_function_at(std::uint64_t offset, std::size_t size, const std::function<Bytes(ByteView)>& function);

    // ---- integers, in the stream's byte order ----
    std::uint8_t read_u8() { return read_int<std::uint8_t>(); }
    std::uint16_t read_u16() { return read_int<std::uint16_t>(); }
    std::uint32_t read_u32() { return read_int<std::uint32_t>(); }
    std::uint64_t read_u64() { return read_int<std::uint64_t>(); }
    std::int8_t read_i8() { return read_int<std::int8_t>(); }
    std::int16_t read_i16() { return read_int<std::int16_t>(); }
    std::int32_t read_i32() { return read_int<std::int32_t>(); }
    std::int64_t read_i64() { return read_int<std::int64_t>(); }
    void write_u8(std::uint8_t v) { write_int(v); }
    void write_u16(std::uint16_t v) { write_int(v); }
    void write_u32(std::uint32_t v) { write_int(v); }
    void write_u64(std::uint64_t v) { write_int(v); }
    void write_i8(std::int8_t v) { write_int(v); }
    void write_i16(std::int16_t v) { write_int(v); }
    void write_i32(std::int32_t v) { write_int(v); }
    void write_i64(std::int64_t v) { write_int(v); }
    std::uint16_t read_u16_at(std::uint64_t offset) { return at_offset(offset, [&] { return read_u16(); }); }
    std::uint32_t read_u32_at(std::uint64_t offset) { return at_offset(offset, [&] { return read_u32(); }); }
    std::uint64_t read_u64_at(std::uint64_t offset) { return at_offset(offset, [&] { return read_u64(); }); }
    void write_u16_at(std::uint64_t offset, std::uint16_t v) { at_offset(offset, [&] { write_u16(v); return 0; }); }
    void write_u32_at(std::uint64_t offset, std::uint32_t v) { at_offset(offset, [&] { write_u32(v); return 0; }); }
    void write_u64_at(std::uint64_t offset, std::uint64_t v) { at_offset(offset, [&] { write_u64(v); return 0; }); }

    // ---- structures ----
    // A plain structure copied in as it lies in memory: the structure's own layout carries its byte order (a big-endian
    // record is declared with big-endian field types). TruncatedError when the stream ends before sizeof(T) bytes.
    template <class T> T read_struct() {
        static_assert(std::is_trivially_copyable_v<T>, "a structure is plain data");
        const Bytes raw = read_exact(sizeof(T));
        T value;
        std::memcpy(&value, raw.data(), sizeof(T));
        return value;
    }
    template <class T> void write_struct(const T& value) {
        static_assert(std::is_trivially_copyable_v<T>, "a structure is plain data");
        write(ByteView(reinterpret_cast<const std::uint8_t*>(&value), sizeof(T)));
    }

private:
    template <class T> T read_int() {
        const Bytes raw = read_exact(sizeof(T));
        std::uint64_t v = 0;
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            const std::uint8_t b = raw[endian_ == Endian::Big ? i : sizeof(T) - 1 - i];
            v = (v << 8) | b;
        }
        return static_cast<T>(v);
    }
    template <class T> void write_int(T value) {
        std::uint8_t raw[sizeof(T)];
        auto v = static_cast<std::uint64_t>(static_cast<std::make_unsigned_t<T>>(value));
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            raw[endian_ == Endian::Big ? sizeof(T) - 1 - i : i] = static_cast<std::uint8_t>(v);
            v >>= 8;
        }
        write(ByteView(raw, sizeof(T)));
    }
    template <class F> auto at_offset(std::uint64_t offset, F&& action) {
        struct Restore {
            std::uint64_t& position;
            std::uint64_t saved;
            ~Restore() { position = saved; }
        } restore{position_, position_};
        position_ = offset;
        return action();
    }

    Endian endian_ = Endian::Little;
    Bytes memory_;                           // the stream's bytes, when it is not on a file
    std::optional<fs::RandomFile> file_;     // the file, when it is
    std::uint64_t position_ = 0;
    std::map<std::string, std::uint64_t> labels_;   // looked up by name only
};

}  // namespace cb::formats::xbox::crypto
