// bytes.h -- a file's bytes, read the way the Python readers read them.
//
// The readers this library replaces were written against Python's bytes: data[i] raises
// IndexError past the end, data[a:b] quietly comes back shorter, struct.unpack_from raises
// struct.error when the value does not fit, data.index raises ValueError when there is no
// match.  A damaged file must give the same answer -- or the same exception -- here as there,
// so every read goes through Bytes, whose operations keep exactly those rules.  A negative
// position counts from the end, as in Python.
#pragma once

#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

namespace cbf {

// An exception the Python reader would raise: its name (as Python prints it: "ValueError",
// "struct.error", "IndexError" ...) and its message.
struct PyError {
    std::string kind;
    std::string message;
};

// A case this library leaves to the Python reader (an int past 64 bits, a branch that runs
// an emulator ...): the shim asks the Python reader instead.
struct Fallback {
    std::string reason;
};

[[noreturn]] inline void raise(const char *kind, std::string message) { throw PyError{kind, std::move(message)}; }
[[noreturn]] inline void fallback(std::string reason) { throw Fallback{std::move(reason)}; }

constexpr int64_t kEnd = std::numeric_limits<int64_t>::max();   // "to the end" in a slice

class Bytes {
public:
    Bytes() = default;
    Bytes(const uint8_t *p, size_t n) : p_(p), n_(static_cast<int64_t>(n)) {}
    explicit Bytes(const std::string &s) : p_(reinterpret_cast<const uint8_t *>(s.data())), n_(static_cast<int64_t>(s.size())) {}
    explicit Bytes(const std::vector<uint8_t> &v) : p_(v.data()), n_(static_cast<int64_t>(v.size())) {}

    int64_t size() const { return n_; }
    bool empty() const { return n_ == 0; }
    const uint8_t *data() const { return p_; }
    const uint8_t *begin() const { return p_; }
    const uint8_t *end() const { return p_ + n_; }

    // data[i]
    uint8_t at(int64_t i) const {
        if (i < 0) i += n_;
        if (i < 0 || i >= n_) raise("IndexError", "index out of range");
        return p_[i];
    }
    // data[a:b], Python's slice: clamped, never raises
    Bytes slice(int64_t a, int64_t b = kEnd) const {
        a = clamp(a);
        b = clamp(b);
        if (b < a) b = a;
        return Bytes(p_ + a, static_cast<size_t>(b - a));
    }
    // data[a:b] == literal
    bool slice_is(int64_t a, int64_t b, const char *lit, size_t n) const {
        Bytes s = slice(a, b);
        return s.n_ == static_cast<int64_t>(n) && std::memcmp(s.p_, lit, n) == 0;
    }
    bool is(const char *lit, size_t n) const { return n_ == static_cast<int64_t>(n) && std::memcmp(p_, lit, n) == 0; }
    bool operator==(const Bytes &o) const { return n_ == o.n_ && (n_ == 0 || std::memcmp(p_, o.p_, static_cast<size_t>(n_)) == 0); }
    bool operator!=(const Bytes &o) const { return !(*this == o); }

    // struct.unpack_from("<?", data, off) for a little-endian unsigned value of `width` bytes
    uint64_t le(int64_t off, int width) const {
        int64_t at = checked(off, width);
        uint64_t v = 0;
        for (int k = width - 1; k >= 0; --k) v = (v << 8) | p_[at + k];
        return v;
    }
    uint64_t be(int64_t off, int width) const {
        int64_t at = checked(off, width);
        uint64_t v = 0;
        for (int k = 0; k < width; ++k) v = (v << 8) | p_[at + k];
        return v;
    }
    uint8_t u8(int64_t off) const { return static_cast<uint8_t>(le(off, 1)); }
    uint16_t u16(int64_t off) const { return static_cast<uint16_t>(le(off, 2)); }
    uint32_t u32(int64_t off) const { return static_cast<uint32_t>(le(off, 4)); }
    uint64_t u64(int64_t off) const { return le(off, 8); }
    int16_t i16(int64_t off) const { return static_cast<int16_t>(le(off, 2)); }
    int32_t i32(int64_t off) const { return static_cast<int32_t>(le(off, 4)); }
    uint16_t be16(int64_t off) const { return static_cast<uint16_t>(be(off, 2)); }
    uint32_t be32(int64_t off) const { return static_cast<uint32_t>(be(off, 4)); }

    // struct.unpack(fmt, data[a:b]) of a value of `width` bytes: the slice must be exactly that
    // long.  `exact(width)` checks a slice already taken.
    const Bytes &exact(int64_t width) const {
        if (n_ != width) raise("struct.error", "unpack requires a buffer of " + std::to_string(width) + " bytes");
        return *this;
    }

    // int.from_bytes(data, "little") / "big" of the whole view (empty -> 0); at most 8 bytes
    uint64_t from_le() const;
    uint64_t from_be() const;

    // data.find(needle, start, end) / rfind / index
    int64_t find(Bytes needle, int64_t start = 0, int64_t end = kEnd) const;
    int64_t find(const char *lit, size_t n, int64_t start = 0, int64_t end = kEnd) const {
        return find(Bytes(reinterpret_cast<const uint8_t *>(lit), n), start, end);
    }
    int64_t find_byte(uint8_t b, int64_t start = 0, int64_t end = kEnd) const {
        return find(Bytes(&b, 1), start, end);
    }
    int64_t rfind(Bytes needle, int64_t start = 0, int64_t end = kEnd) const;
    int64_t index(Bytes needle, int64_t start = 0, int64_t end = kEnd) const {
        int64_t i = find(needle, start, end);
        if (i < 0) raise("ValueError", "subsection not found");
        return i;
    }
    int64_t index_byte(uint8_t b, int64_t start = 0, int64_t end = kEnd) const {
        return index(Bytes(&b, 1), start, end);
    }
    bool contains(const char *lit, size_t n) const { return find(lit, n) >= 0; }

    // data.rstrip(chars) / lstrip
    Bytes rstrip(const char *chars, size_t n) const;
    Bytes lstrip(const char *chars, size_t n) const;
    Bytes strip_ws() const;   // data.strip() (ASCII whitespace)
    std::vector<uint8_t> vec() const { return std::vector<uint8_t>(p_, p_ + n_); }
    std::string str() const { return std::string(reinterpret_cast<const char *>(p_), static_cast<size_t>(n_)); }

private:
    int64_t clamp(int64_t i) const {
        if (i == kEnd) return n_;
        if (i < 0) {
            i += n_;
            return i < 0 ? 0 : i;
        }
        return i > n_ ? n_ : i;
    }
    // the offset unpack_from reads `width` bytes at, or struct.error
    int64_t checked(int64_t off, int width) const {
        int64_t at = off;
        if (at < 0) {
            at += n_;
            if (at < 0)
                raise("struct.error", "offset " + std::to_string(off) + " out of range for " + std::to_string(n_) + "-byte buffer");
            if (n_ - at < width)
                raise("struct.error", "not enough data to unpack " + std::to_string(width) + " bytes at offset " + std::to_string(off));
            return at;
        }
        if (at > n_ - width)
            raise("struct.error", "unpack_from requires a buffer of at least " + std::to_string(at + width) +
                                      " bytes for unpacking " + std::to_string(width) + " bytes at offset " +
                                      std::to_string(at) + " (actual buffer size is " + std::to_string(n_) + ")");
        return at;
    }

    const uint8_t *p_ = nullptr;
    int64_t n_ = 0;
};

inline Bytes lit(const char *s, size_t n) { return Bytes(reinterpret_cast<const uint8_t *>(s), n); }

// Python's list[i] for a list of anything: IndexError past either end, negative from the end.
template <class V>
auto &py_at(V &v, int64_t i) {
    int64_t n = static_cast<int64_t>(v.size());
    if (i < 0) i += n;
    if (i < 0 || i >= n) raise("IndexError", "list index out of range");
    return v[static_cast<size_t>(i)];
}

// a << b for Python ints that must stay inside int64 (else the Python reader answers)
inline int64_t py_shl(int64_t a, int64_t b) {
    if (a == 0) return 0;
    if (b < 0) raise("ValueError", "negative shift count");
    if (b >= 63 || (a > 0 ? a > (std::numeric_limits<int64_t>::max() >> b) : a < (std::numeric_limits<int64_t>::min() >> b)))
        fallback("an int past 64 bits");
    return a << b;
}

}  // namespace cbf
