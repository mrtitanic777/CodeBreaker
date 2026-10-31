// bytes.cc -- the searches and strips of Bytes (bytes.h), with Python's rules for where they look.
#include "bytes.h"

#include <algorithm>

namespace cbf {

namespace {
// start/end of bytes.find / rfind: slice indices, so negative counts from the end, then clamped
void adjust(int64_t n, int64_t &start, int64_t &end) {
    if (end == kEnd || end > n) end = n;
    else if (end < 0) end = std::max<int64_t>(0, end + n);
    if (start < 0) start = std::max<int64_t>(0, start + n);
}

bool in(const char *chars, size_t n, uint8_t c) {
    return std::memchr(chars, c, n) != nullptr;
}

bool ascii_space(uint8_t c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\x0b' || c == '\x0c'; }
}  // namespace

uint64_t Bytes::from_le() const {
    uint64_t v = 0;
    if (n_ > 8) fallback("an int past 64 bits");
    for (int64_t k = n_ - 1; k >= 0; --k) v = (v << 8) | p_[k];
    return v;
}

uint64_t Bytes::from_be() const {
    uint64_t v = 0;
    if (n_ > 8) fallback("an int past 64 bits");
    for (int64_t k = 0; k < n_; ++k) v = (v << 8) | p_[k];
    return v;
}

int64_t Bytes::find(Bytes needle, int64_t start, int64_t end) const {
    adjust(n_, start, end);
    if (start > n_) return -1;
    int64_t m = needle.size();
    if (end - start < m) return -1;
    if (m == 0) return start;
    const uint8_t first = needle.data()[0];
    for (int64_t i = start; i + m <= end; ++i) {
        const void *hit = std::memchr(p_ + i, first, static_cast<size_t>(end - m + 1 - i));
        if (!hit) return -1;
        i = static_cast<const uint8_t *>(hit) - p_;
        if (std::memcmp(p_ + i, needle.data(), static_cast<size_t>(m)) == 0) return i;
    }
    return -1;
}

int64_t Bytes::rfind(Bytes needle, int64_t start, int64_t end) const {
    adjust(n_, start, end);
    if (start > n_) return -1;
    int64_t m = needle.size();
    if (end - start < m) return -1;
    for (int64_t i = end - m; i >= start; --i)
        if (std::memcmp(p_ + i, needle.data(), static_cast<size_t>(m)) == 0) return i;
    return -1;
}

Bytes Bytes::rstrip(const char *chars, size_t n) const {
    int64_t e = n_;
    while (e > 0 && in(chars, n, p_[e - 1])) --e;
    return Bytes(p_, static_cast<size_t>(e));
}

Bytes Bytes::lstrip(const char *chars, size_t n) const {
    int64_t s = 0;
    while (s < n_ && in(chars, n, p_[s])) ++s;
    return Bytes(p_ + s, static_cast<size_t>(n_ - s));
}

Bytes Bytes::strip_ws() const {
    int64_t s = 0, e = n_;
    while (s < e && ascii_space(p_[s])) ++s;
    while (e > s && ascii_space(p_[e - 1])) --e;
    return Bytes(p_ + s, static_cast<size_t>(e - s));
}

}  // namespace cbf
