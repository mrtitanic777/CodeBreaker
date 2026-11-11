// codebreaker/formats/xbox/crypto/StreamIO.hpp -- C++ port of codebreaker.formats.xbox.crypto.StreamIO.
//
// StreamIO -- the endian-aware byte stream the Xbox-360-Crypto tools read and write images through.
//
// Upstream's own StreamIO module was not among the files carried over; this one gives the same calls
// those files make: a stream over bytes (or a file, by path), little or big endian, with integer,
// byte and ctypes-structure reads and writes, at the position or at an offset. The tools also use the
// position as an attribute (offset), named positions (labels) and a function applied to the bytes at an
// offset in place (perform_function_at).
//
// Typing notes:
//   * The Python builds the stream on io.BytesIO (bytes) or on open(path, "r+b" / "w+b") (a path); the
//     constructors are one per kind of argument: none, bytes, bytearray, path (py::str).
//   * ctypes structures (read_struct / write_struct / read_header) become any trivially-copyable C++
//     struct: sizeof(T) bytes are copied in or out as they lie in memory, the structure's own byte order
//     (Little/BigEndianStructure) being whatever the struct's layout is.
//   * write_uint64 takes a uint64_t and read_uint64 returns one: a value >= 2**63 is real data; the
//     negative or oversized values Python's struct.error refuses cannot be passed to it.
//   * The `offset` attribute is the pair offset() / set_offset(n); set_label(name) names the position (or
//     set_label(name, n) an offset); perform_function_at takes any callable from py::bytes to py::bytes.
//   * The C macros SEEK_SET / SEEK_CUR / SEEK_END are spelled with a trailing underscore.
#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>

#include "py.hpp"

namespace codebreaker::formats::xbox::crypto::StreamIO {

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

    // StreamIO(None, endian): an empty stream over bytes
    explicit StreamIO(Endian endian = Endian::LITTLE);
    // StreamIO(data, endian): a stream over a copy of the bytes
    explicit StreamIO(const py::bytes& data, Endian endian = Endian::LITTLE);
    explicit StreamIO(const py::bytearray& data, Endian endian = Endian::LITTLE);
    // StreamIO(path, endian): a stream over the file at the path ("r+b" if it exists, else created "w+b")
    explicit StreamIO(const py::str& path, Endian endian = Endian::LITTLE);

    StreamIO(const StreamIO&) = delete;             // (a Python object is shared, never copied)
    StreamIO& operator=(const StreamIO&) = delete;
    StreamIO(StreamIO&&) = default;
    StreamIO& operator=(StreamIO&&) = default;

    StreamIO& __enter__();
    void __exit__();   // (exc_type, exc, tb are not used)
    void close();

    py::str _order();   // ">" for big endian, else "<"

    // position
    int64_t seek(int64_t offset, int64_t whence = SEEK_SET_);
    int64_t tell();
    py::bytes getvalue();
    int64_t length();

    // the position as an attribute: `s.offset` reads it, `s.offset = n` seeks
    int64_t offset();
    void set_offset(int64_t value);

    // named positions
    void set_label(const py::str& name);                       // names the position
    void set_label(const py::str& name, int64_t offset);       // names an offset
    int64_t get_label(const py::str& name);                    // KeyError for a name never set
    int64_t goto_label(const py::str& name, int64_t offset = 0);   // seeks to the named position plus offset

    // bytes
    py::bytes read(int64_t size = -1);
    int64_t write(const py::bytes& data);
    int64_t write(const py::bytearray& data);
    py::bytes read_bytes(int64_t size);
    py::bytes read_ubytes(int64_t size);  // = read_bytes
    int64_t read_byte();
    int64_t read_ubyte();  // = read_byte
    py::bytes read_bytes_at(int64_t offset, int64_t size);
    py::bytes read_ubytes_at(int64_t offset, int64_t size);  // = read_bytes_at
    int64_t write_bytes(const py::bytes& data);
    int64_t write_bytes(const py::bytearray& data);
    int64_t write_ubytes(const py::bytes& data);  // = write_bytes
    int64_t write_ubytes(const py::bytearray& data);
    int64_t write_bytes_at(int64_t offset, const py::bytes& data);
    int64_t write_bytes_at(int64_t offset, const py::bytearray& data);
    int64_t write_ubytes_at(int64_t offset, const py::bytes& data);  // = write_bytes_at
    int64_t write_ubytes_at(int64_t offset, const py::bytearray& data);

    // integers (struct.error, here py::StructError, for a short read or a value out of range)
    int64_t read_uint8();
    int64_t read_uint16();
    int64_t read_uint32();
    uint64_t read_uint64();
    int64_t read_int8();
    int64_t read_int16();
    int64_t read_int32();
    int64_t read_int64();
    int64_t write_uint8(int64_t v);
    int64_t write_uint16(int64_t v);
    int64_t write_uint32(int64_t v);
    int64_t write_uint64(uint64_t v);
    int64_t write_int8(int64_t v);
    int64_t write_int16(int64_t v);
    int64_t write_int32(int64_t v);
    int64_t write_int64(int64_t v);

    // Apply `function` to the `size` bytes at `offset`, write what it returns back there (in place) and return it;
    // the position is left where it was.
    template <class F> py::bytes perform_function_at(int64_t offset, int64_t size, F&& function) {
        const int64_t here = tell();
        seek(offset);
        py::bytes data = function(read(size));
        seek(offset);
        write(data);
        seek(here);
        return data;
    }

    // ctypes structures (their own endianness: Little/BigEndianStructure): ValueError if the stream ends
    // before sizeof(T) bytes
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
    template <class T> T read_header() { return read_struct<T>(); }  // = read_struct

    template <class T> int64_t write_struct(const T& value) {
        static_assert(std::is_trivially_copyable<T>::value, "a structure is plain data");
        return write(py::bytes(std::string(reinterpret_cast<const char*>(&value), sizeof(T))));
    }

private:
    bool _file = false;
    std::string mem_;       // the BytesIO's bytes
    int64_t pos_ = 0;       // the position
    py::dict<py::str, int64_t> labels_;   // the named positions
    py::file stream_;       // the open file, when the stream is on a path

    int64_t size_();
    py::bytes take_(int64_t size);                  // _stream.read(size)
    int64_t put_(const std::string& data);          // _stream.write(data)
    uint64_t read_raw_(int size);
    int64_t write_raw_(int size, uint64_t value);
    void need_open_();
};

}  // namespace codebreaker::formats::xbox::crypto::StreamIO
