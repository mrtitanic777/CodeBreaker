// codebreaker/formats/xbox/crypto/StreamIO.hpp -- COMPAT SHIM over cb/formats/xbox/crypto/stream_io.hpp (docs/CPP_DESIGN.md 6.3), for
// the old shadowboot / build_lib / patch_checker / xcp_dumper ports. Deleted with its last py:: consumer.
//
// The stream is native/cb/formats/xbox/crypto/stream_io.hpp. The old API is kept: the Python's int64 integers, py::bytes, the
// `offset` attribute as offset()/set_offset(), `with` as __enter__/__exit__ and close() (a closed file stream refuses further use),
// and the exceptions the old callers expect (struct.error for a short read or a value out of range, IndexError for read_byte).
// Conversion and forwarding only.
#pragma once

#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <type_traits>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/crypto/stream_io.hpp"
#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::StreamIO {

namespace detail {
namespace nw = ::cb::formats::xbox::crypto;
}

// class Endian(IntEnum)
enum class Endian : int64_t {
    LITTLE = 0,
    BIG = 1,
};

// io.SEEK_SET / SEEK_CUR / SEEK_END
inline constexpr int64_t SEEK_SET_ = 0;
inline constexpr int64_t SEEK_CUR_ = 1;
inline constexpr int64_t SEEK_END_ = 2;

class StreamIO {
public:
    Endian endian = Endian::LITTLE;

    explicit StreamIO(Endian endian_ = Endian::LITTLE) : endian(endian_), s_(detail::nw::StreamIO()) {}
    explicit StreamIO(const py::bytes& data, Endian endian_ = Endian::LITTLE)
        : endian(endian_), s_(detail::nw::StreamIO(::cb::compat::from_py(data))) {}
    explicit StreamIO(const py::bytearray& data, Endian endian_ = Endian::LITTLE)
        : endian(endian_), s_(detail::nw::StreamIO(::cb::compat::from_py(py::bytes(data.view())))) {}
    explicit StreamIO(const py::str& path, Endian endian_ = Endian::LITTLE)
        : endian(endian_), file_(true), s_(::cb::compat::guard([&] { return detail::nw::StreamIO::open(::cb::fs::from_text(path.utf8())); })) {}

    StreamIO(const StreamIO&) = delete;
    StreamIO& operator=(const StreamIO&) = delete;
    StreamIO(StreamIO&&) = default;
    StreamIO& operator=(StreamIO&&) = default;

    StreamIO& __enter__() { return *this; }
    void __exit__() { close(); }
    void close() {
        if (file_) s_.reset();
    }

    py::str _order() { return endian == Endian::BIG ? py::str(">") : py::str("<"); }

    // position
    int64_t seek(int64_t offset, int64_t whence = SEEK_SET_) {
        if (whence < 0 || whence > 2) throw py::ValueError("invalid whence (" + std::to_string(whence) + ", should be 0, 1 or 2)");
        return call([&](auto& st) {
            // (a memory stream clamps a seek before the start to 0; a file refuses it)
            const int64_t base = whence == 0 ? 0 : whence == 1 ? static_cast<int64_t>(st.position()) : static_cast<int64_t>(st.length());
            int64_t target = base + offset;
            if (target < 0 && !file_) target = 0;
            return static_cast<int64_t>(st.seek(target));
        });
    }
    int64_t tell() { return call([](auto& st) { return static_cast<int64_t>(st.position()); }); }
    py::bytes getvalue() { return call([](auto& st) { return ::cb::compat::to_py(::cb::ByteView(st.contents())); }); }
    int64_t length() { return call([](auto& st) { return static_cast<int64_t>(st.length()); }); }

    // the position as an attribute
    int64_t offset() { return tell(); }
    void set_offset(int64_t value) { seek(value); }

    // named positions
    void set_label(const py::str& name) { call([&](auto& st) { st.set_label(name.utf8()); return 0; }); }
    void set_label(const py::str& name, int64_t offset) { call([&](auto& st) { st.set_label(name.utf8(), static_cast<uint64_t>(offset)); return 0; }); }
    int64_t get_label(const py::str& name) { return call_key([&](auto& st) { return static_cast<int64_t>(st.label(name.utf8())); }); }
    int64_t goto_label(const py::str& name, int64_t offset = 0) {
        return call_key([&](auto& st) { return static_cast<int64_t>(st.goto_label(name.utf8(), offset)); });
    }

    // bytes
    py::bytes read(int64_t size = -1) {
        return call([&](auto& st) {
            const std::size_t n = size < 0 ? static_cast<std::size_t>(st.length() > st.position() ? st.length() - st.position() : 0)
                                           : static_cast<std::size_t>(size);
            return ::cb::compat::to_py(::cb::ByteView(st.read(n)));
        });
    }
    int64_t write(const py::bytes& data) { return put(data); }
    int64_t write(const py::bytearray& data) { return put(py::bytes(data.view())); }
    py::bytes read_bytes(int64_t size) { return read(size); }
    py::bytes read_ubytes(int64_t size) { return read(size); }
    int64_t read_byte() {
        py::bytes b = read(1);
        if (b.empty()) throw py::IndexError("index out of range");
        return b[0];
    }
    int64_t read_ubyte() { return read_byte(); }
    py::bytes read_bytes_at(int64_t offset, int64_t size) {
        return call([&](auto& st) { return ::cb::compat::to_py(::cb::ByteView(st.read_at(static_cast<uint64_t>(offset), static_cast<std::size_t>(size)))); });
    }
    py::bytes read_ubytes_at(int64_t offset, int64_t size) { return read_bytes_at(offset, size); }
    int64_t write_bytes(const py::bytes& data) { return put(data); }
    int64_t write_bytes(const py::bytearray& data) { return put(py::bytes(data.view())); }
    int64_t write_ubytes(const py::bytes& data) { return put(data); }
    int64_t write_ubytes(const py::bytearray& data) { return put(py::bytes(data.view())); }
    int64_t write_bytes_at(int64_t offset, const py::bytes& data) {
        return call([&](auto& st) {
            const ::cb::Bytes raw = ::cb::compat::from_py(data);
            st.write_at(static_cast<uint64_t>(offset), raw);
            return static_cast<int64_t>(raw.size());
        });
    }
    int64_t write_bytes_at(int64_t offset, const py::bytearray& data) { return write_bytes_at(offset, py::bytes(data.view())); }
    int64_t write_ubytes_at(int64_t offset, const py::bytes& data) { return write_bytes_at(offset, data); }
    int64_t write_ubytes_at(int64_t offset, const py::bytearray& data) { return write_bytes_at(offset, data); }

    // integers (struct.error for a short read or a value out of range)
    int64_t read_uint8() { return call([&](auto& st) { sync(st); return static_cast<int64_t>(st.read_u8()); }); }
    int64_t read_uint16() { return call([&](auto& st) { sync(st); return static_cast<int64_t>(st.read_u16()); }); }
    int64_t read_uint32() { return call([&](auto& st) { sync(st); return static_cast<int64_t>(st.read_u32()); }); }
    uint64_t read_uint64() { return call([&](auto& st) { sync(st); return st.read_u64(); }); }
    int64_t read_int8() { return call([&](auto& st) { sync(st); return static_cast<int64_t>(st.read_i8()); }); }
    int64_t read_int16() { return call([&](auto& st) { sync(st); return static_cast<int64_t>(st.read_i16()); }); }
    int64_t read_int32() { return call([&](auto& st) { sync(st); return static_cast<int64_t>(st.read_i32()); }); }
    int64_t read_int64() { return call([&](auto& st) { sync(st); return st.read_i64(); }); }
    int64_t write_uint8(int64_t v) { return wr(v, 0, 0xFF, 1, [](auto& st, int64_t x) { st.write_u8(static_cast<uint8_t>(x)); }); }
    int64_t write_uint16(int64_t v) { return wr(v, 0, 0xFFFF, 2, [](auto& st, int64_t x) { st.write_u16(static_cast<uint16_t>(x)); }); }
    int64_t write_uint32(int64_t v) { return wr(v, 0, 0xFFFFFFFFLL, 4, [](auto& st, int64_t x) { st.write_u32(static_cast<uint32_t>(x)); }); }
    int64_t write_uint64(uint64_t v) { return call([&](auto& st) { sync(st); st.write_u64(v); return int64_t{8}; }); }
    int64_t write_int8(int64_t v) { return wr(v, -128, 127, 1, [](auto& st, int64_t x) { st.write_i8(static_cast<int8_t>(x)); }); }
    int64_t write_int16(int64_t v) { return wr(v, -32768, 32767, 2, [](auto& st, int64_t x) { st.write_i16(static_cast<int16_t>(x)); }); }
    int64_t write_int32(int64_t v) { return wr(v, INT32_MIN, INT32_MAX, 4, [](auto& st, int64_t x) { st.write_i32(static_cast<int32_t>(x)); }); }
    int64_t write_int64(int64_t v) { return call([&](auto& st) { sync(st); st.write_i64(v); return int64_t{8}; }); }

    // Apply `function` to the `size` bytes at `offset`, write what it returns back there (in place) and return it; the
    // position is left where it was.
    template <class F> py::bytes perform_function_at(int64_t offset, int64_t size, F&& function) {
        return call([&](auto& st) {
            const ::cb::Bytes out = st.perform_function_at(static_cast<uint64_t>(offset), static_cast<std::size_t>(size),
                                                           [&](::cb::ByteView in) {
                                                               py::bytes r = function(::cb::compat::to_py(in));
                                                               return ::cb::compat::from_py(r);
                                                           });
            return ::cb::compat::to_py(::cb::ByteView(out));
        });
    }

    // ctypes structures: ValueError if the stream ends before sizeof(T) bytes
    template <class T> T read_struct() {
        static_assert(std::is_trivially_copyable<T>::value, "a structure is plain data");
        py::bytes b = read(static_cast<int64_t>(sizeof(T)));
        if (b.size() < static_cast<int64_t>(sizeof(T))) {
            throw py::ValueError("Buffer size too small (" + std::to_string(b.size()) + " instead of at least " +
                                 std::to_string(sizeof(T)) + " bytes)");
        }
        T value;
        std::memcpy(&value, b.raw().data(), sizeof(T));
        return value;
    }
    template <class T> T read_header() { return read_struct<T>(); }
    template <class T> int64_t write_struct(const T& value) {
        static_assert(std::is_trivially_copyable<T>::value, "a structure is plain data");
        return write(py::bytes(std::string(reinterpret_cast<const char*>(&value), sizeof(T))));
    }

private:
    void sync(detail::nw::StreamIO& st) {
        st.set_endian(endian == Endian::BIG ? detail::nw::Endian::Big : detail::nw::Endian::Little);
    }
    // Run f on the stream, a cb error becoming the matching py:: one; a short integer read is struct.error as before.
    template <class F> decltype(auto) call(F&& f) {
        if (!s_) throw py::ValueError("I/O operation on closed file");
        try {
            return f(*s_);
        } catch (const ::cb::TruncatedError& e) {
            throw py::StructError(e.what());
        } catch (const ::cb::Error& e) {
            ::cb::compat::rethrow_as_py(e);
        }
    }
    template <class F> decltype(auto) call_key(F&& f) {
        if (!s_) throw py::ValueError("I/O operation on closed file");
        try {
            return f(*s_);
        } catch (const ::cb::UsageError& e) {
            throw py::KeyError(e.what());
        }
    }
    int64_t put(const py::bytes& data) {
        return call([&](auto& st) {
            const ::cb::Bytes raw = ::cb::compat::from_py(data);
            st.write(raw);
            return static_cast<int64_t>(raw.size());
        });
    }
    template <class F> int64_t wr(int64_t v, int64_t lo, int64_t hi, int64_t size, F&& store) {
        if (v < lo || v > hi) throw py::StructError("argument out of range");
        return call([&](auto& st) { sync(st); store(st, v); return size; });
    }

    bool file_ = false;
    std::optional<detail::nw::StreamIO> s_;
};

}  // namespace codebreaker::formats::xbox::crypto::StreamIO
