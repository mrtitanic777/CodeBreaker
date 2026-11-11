// codebreaker/formats/xbox/crypto/StreamIO.cpp -- C++ port of codebreaker.formats.xbox.crypto.StreamIO.

#include "codebreaker/formats/xbox/crypto/StreamIO.hpp"

#include <algorithm>

namespace codebreaker::formats::xbox::crypto::StreamIO {

StreamIO::StreamIO(Endian endian_) : endian(endian_) {}

StreamIO::StreamIO(const py::bytes& data, Endian endian_) : endian(endian_), mem_(data.raw()) {}

StreamIO::StreamIO(const py::bytearray& data, Endian endian_) : endian(endian_), mem_(py::bytes(data.view()).raw()) {}

StreamIO::StreamIO(const py::str& path, Endian endian_) : endian(endian_), _file(true) {
    stream_ = py::open(path, py::os::path::exists(path) ? py::str("r+b") : py::str("w+b"));
}

StreamIO& StreamIO::__enter__() { return *this; }

void StreamIO::__exit__() { close(); }

void StreamIO::close() {
    if (_file) {
        stream_.close();
    }
}

py::str StreamIO::_order() { return endian == Endian::BIG ? py::str(">") : py::str("<"); }

void StreamIO::need_open_() {
    if (_file && stream_.closed()) throw py::ValueError("I/O operation on closed file");
}

int64_t StreamIO::size_() {
    if (_file) return stream_.seek(0, 2);
    return static_cast<int64_t>(mem_.size());
}

// self._stream.read(size): size < 0 reads to the end
py::bytes StreamIO::take_(int64_t size) {
    if (_file) {
        need_open_();
        if (size < -1) throw py::ValueError("read length must be non-negative or -1");
        stream_.seek(pos_, 0);
        py::bytes b = stream_.read(size);
        pos_ += b.size();
        return b;
    }
    const int64_t have = static_cast<int64_t>(mem_.size());
    if (pos_ >= have) return py::bytes();
    const int64_t n = size < 0 ? have - pos_ : std::min(size, have - pos_);
    py::bytes b(mem_.substr(static_cast<size_t>(pos_), static_cast<size_t>(n)));
    pos_ += n;
    return b;
}

// self._stream.write(data)
int64_t StreamIO::put_(const std::string& data) {
    if (_file) need_open_();
    if (data.empty()) return 0;
    const int64_t n = static_cast<int64_t>(data.size());
    if (_file) {
        stream_.seek(pos_, 0);
        stream_.write(py::bytes(data));
        stream_.flush();
    } else {
        if (pos_ > static_cast<int64_t>(mem_.size())) mem_.resize(static_cast<size_t>(pos_), '\0');
        mem_.replace(static_cast<size_t>(pos_), std::min<size_t>(data.size(), mem_.size() - static_cast<size_t>(pos_)), data);
    }
    pos_ += n;
    return n;
}

// position
int64_t StreamIO::seek(int64_t offset, int64_t whence) {
    if (_file) {
        need_open_();
        if (whence < 0 || whence > 2) throw py::ValueError("whence value " + std::to_string(whence) + " unsupported");
        const int64_t target = whence == 0 ? offset : whence == 1 ? pos_ + offset : size_() + offset;
        if (target < 0) throw py::OSError("[Errno 22] Invalid argument");
        pos_ = target;
        return pos_;
    }
    if (offset < 0 && whence == 0) throw py::ValueError("negative seek value " + std::to_string(offset));
    int64_t target = offset;
    if (whence == 1) {
        target += pos_;
    } else if (whence == 2) {
        target += static_cast<int64_t>(mem_.size());
    } else if (whence != 0) {
        throw py::ValueError("invalid whence (" + std::to_string(whence) + ", should be 0, 1 or 2)");
    }
    if (target < 0) target = 0;
    pos_ = target;
    return pos_;
}

int64_t StreamIO::tell() {
    need_open_();
    return pos_;
}

int64_t StreamIO::offset() {
    need_open_();
    return pos_;
}

void StreamIO::set_offset(int64_t value) { seek(value); }

void StreamIO::set_label(const py::str& name) { labels_.set(name, tell()); }

void StreamIO::set_label(const py::str& name, int64_t offset) { labels_.set(name, offset); }

int64_t StreamIO::get_label(const py::str& name) { return labels_.at(name); }

int64_t StreamIO::goto_label(const py::str& name, int64_t offset) { return seek(labels_.at(name) + offset); }

py::bytes StreamIO::getvalue() {
    if (_file) {
        need_open_();
        stream_.seek(0, 0);
        return stream_.read(-1);
    }
    return py::bytes(mem_);
}

int64_t StreamIO::length() {
    need_open_();
    return size_();
}

// bytes
py::bytes StreamIO::read(int64_t size) { return take_(size); }

int64_t StreamIO::write(const py::bytes& data) { return put_(data.raw()); }

int64_t StreamIO::write(const py::bytearray& data) { return put_(py::bytes(data.view()).raw()); }

py::bytes StreamIO::read_bytes(int64_t size) { return take_(size); }

py::bytes StreamIO::read_ubytes(int64_t size) { return read_bytes(size); }

int64_t StreamIO::read_byte() {
    py::bytes b = take_(1);
    if (b.empty()) throw py::IndexError("index out of range");
    return b[0];
}

int64_t StreamIO::read_ubyte() { return read_byte(); }

py::bytes StreamIO::read_bytes_at(int64_t offset, int64_t size) {
    const int64_t here = tell();
    seek(offset);
    py::bytes data = take_(size);
    seek(here);
    return data;
}

py::bytes StreamIO::read_ubytes_at(int64_t offset, int64_t size) { return read_bytes_at(offset, size); }

int64_t StreamIO::write_bytes(const py::bytes& data) { return put_(data.raw()); }

int64_t StreamIO::write_bytes(const py::bytearray& data) { return put_(py::bytes(data.view()).raw()); }

int64_t StreamIO::write_ubytes(const py::bytes& data) { return write_bytes(data); }

int64_t StreamIO::write_ubytes(const py::bytearray& data) { return write_bytes(data); }

int64_t StreamIO::write_bytes_at(int64_t offset, const py::bytes& data) {
    const int64_t here = tell();
    seek(offset);
    const int64_t written = put_(data.raw());
    seek(here);
    return written;
}

int64_t StreamIO::write_bytes_at(int64_t offset, const py::bytearray& data) {
    return write_bytes_at(offset, py::bytes(data.view()));
}

int64_t StreamIO::write_ubytes_at(int64_t offset, const py::bytes& data) { return write_bytes_at(offset, data); }

int64_t StreamIO::write_ubytes_at(int64_t offset, const py::bytearray& data) { return write_bytes_at(offset, data); }

// integers
// struct.unpack(order + fmt, self._stream.read(size))[0], as an unsigned value
uint64_t StreamIO::read_raw_(int size) {
    py::bytes b = take_(size);
    if (b.size() != size) throw py::StructError("unpack requires a buffer of " + std::to_string(size) + " bytes");
    uint64_t v = 0;
    for (int k = 0; k < size; ++k) {
        const uint64_t byte = static_cast<uint64_t>(b[k]);
        v |= byte << (8 * (endian == Endian::BIG ? size - 1 - k : k));
    }
    return v;
}

// self._stream.write(struct.pack(order + fmt, value)) for a value already checked to fit
int64_t StreamIO::write_raw_(int size, uint64_t value) {
    std::string out(static_cast<size_t>(size), '\0');
    for (int k = 0; k < size; ++k) {
        out[static_cast<size_t>(endian == Endian::BIG ? size - 1 - k : k)] = static_cast<char>((value >> (8 * k)) & 0xFF);
    }
    return put_(out);
}

namespace {

int64_t signed_of(uint64_t raw, int size) {
    if (size < 8 && ((raw >> (8 * size - 1)) & 1)) raw |= ~uint64_t(0) << (8 * size);
    return static_cast<int64_t>(raw);
}

// struct.pack's range check of an integer for a field of `size` bytes
void check_range_(int64_t v, int size, bool is_signed) {
    const int64_t lo = is_signed ? -(int64_t(1) << (8 * size - 1)) : 0;
    const int64_t hi = is_signed ? (int64_t(1) << (8 * size - 1)) - 1 : (int64_t(1) << (8 * size)) - 1;
    if (v < lo || v > hi) throw py::StructError("argument out of range");
}

}  // namespace

int64_t StreamIO::read_uint8() { return static_cast<int64_t>(read_raw_(1)); }
int64_t StreamIO::read_uint16() { return static_cast<int64_t>(read_raw_(2)); }
int64_t StreamIO::read_uint32() { return static_cast<int64_t>(read_raw_(4)); }
uint64_t StreamIO::read_uint64() { return read_raw_(8); }
int64_t StreamIO::read_int8() { return signed_of(read_raw_(1), 1); }
int64_t StreamIO::read_int16() { return signed_of(read_raw_(2), 2); }
int64_t StreamIO::read_int32() { return signed_of(read_raw_(4), 4); }
int64_t StreamIO::read_int64() { return signed_of(read_raw_(8), 8); }

int64_t StreamIO::write_uint8(int64_t v) { check_range_(v, 1, false); return write_raw_(1, static_cast<uint64_t>(v)); }
int64_t StreamIO::write_uint16(int64_t v) { check_range_(v, 2, false); return write_raw_(2, static_cast<uint64_t>(v)); }
int64_t StreamIO::write_uint32(int64_t v) { check_range_(v, 4, false); return write_raw_(4, static_cast<uint64_t>(v)); }
int64_t StreamIO::write_uint64(uint64_t v) { return write_raw_(8, v); }
int64_t StreamIO::write_int8(int64_t v) { check_range_(v, 1, true); return write_raw_(1, static_cast<uint64_t>(v)); }
int64_t StreamIO::write_int16(int64_t v) { check_range_(v, 2, true); return write_raw_(2, static_cast<uint64_t>(v)); }
int64_t StreamIO::write_int32(int64_t v) { check_range_(v, 4, true); return write_raw_(4, static_cast<uint64_t>(v)); }
int64_t StreamIO::write_int64(int64_t v) { return write_raw_(8, static_cast<uint64_t>(v)); }

}  // namespace codebreaker::formats::xbox::crypto::StreamIO
