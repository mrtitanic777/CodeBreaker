// formats/format_gen_runtime.hpp -- what the generated readers share (docs/FORMAT_CODEGEN.md): the data a read goes over,
// its errors (the engine's: TruncatedError for a field cut short, FormatError for the rest) and the engine's built-in
// functions and arithmetic, each as the engine evaluates it. Included by the code format_gen writes, not by readers.
#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/error.hpp"

namespace cb::formats::gen::rt {

// The data and the format's name (the errors name the format being read, as the engine's do).
struct Base {
    ByteView data;
    std::string_view fmt;

    [[noreturn]] void fail(std::uint64_t at, const std::string& what) const { throw FormatError(fmt, at, what); }
    [[noreturn]] void truncated(std::uint64_t at, std::uint64_t wanted, std::uint64_t available) const {
        throw TruncatedError(fmt, at, wanted, available);
    }
    // An expression that fails where it is evaluated (an unknown name, a value that is not a number ...): an int64 so
    // that it stands for a number in the generated code.
    [[noreturn]] std::int64_t fail_value(std::uint64_t at, const std::string& what) const { fail(at, what); }
};

// A field of `width` bytes at c must lie before `end`.
inline void room(const Base& cx, std::uint64_t c, std::uint64_t need, std::uint64_t end) {
    if (c > end || need > end - c) cx.truncated(c, need, end > c ? end - c : 0);
}

// An unsigned value of `width` bytes, in the given order.
template <unsigned Width, bool Big>
inline std::uint64_t load(const std::uint8_t* p) {
    std::uint64_t v = 0;
    if constexpr (Big) {
        for (unsigned k = 0; k < Width; ++k) v = (v << 8) | p[k];
    } else {
        for (unsigned k = Width; k-- > 0;) v = (v << 8) | p[k];
    }
    return v;
}

// A signed value of `width` bytes: sign-extended to 64 bits (the engine's node value).
template <unsigned Width, bool Big>
inline std::uint64_t load_signed(const std::uint8_t* p) {
    std::uint64_t v = load<Width, Big>(p);
    if constexpr (Width < 8)
        if ((v >> (8 * Width - 1)) & 1) v |= ~std::uint64_t{0} << (8 * Width);
    return v;
}

inline std::string_view text_at(const Base& cx, std::uint64_t at, std::uint64_t n) {
    return {reinterpret_cast<const char*>(cx.data.data()) + at, static_cast<std::size_t>(n)};
}
inline ByteView bytes_at(const Base& cx, std::uint64_t at, std::uint64_t n) {
    return cx.data.subspan(static_cast<std::size_t>(at), static_cast<std::size_t>(n));
}

// A length or an offset: a negative one is the description's error.
inline std::uint64_t length(const Base& cx, std::uint64_t pos, std::int64_t n) {
    if (n < 0) cx.fail(pos, "a negative length " + std::to_string(n));
    return static_cast<std::uint64_t>(n);
}

// ---- the arithmetic (64-bit; + - * wrap) ----

inline std::int64_t add(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(b)); }
inline std::int64_t sub(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) - static_cast<std::uint64_t>(b)); }
inline std::int64_t mul(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b)); }
inline std::int64_t neg(std::int64_t a) { return static_cast<std::int64_t>(0 - static_cast<std::uint64_t>(a)); }
inline std::int64_t div(const Base& cx, std::uint64_t pos, std::int64_t a, std::int64_t b) {
    if (b == 0) cx.fail(pos, "division by zero");
    return a / b;
}
inline std::int64_t mod(const Base& cx, std::uint64_t pos, std::int64_t a, std::int64_t b) {
    if (b == 0) cx.fail(pos, "division by zero");
    return a % b;
}
inline std::int64_t shl(std::int64_t a, std::int64_t b) { return b < 0 || b >= 64 ? 0 : static_cast<std::int64_t>(static_cast<std::uint64_t>(a) << b); }
inline std::int64_t shr(std::int64_t a, std::int64_t b) { return b < 0 || b >= 64 ? 0 : a >> b; }

// ---- references: a field of a structure that may not be there, an element of an array ----

template <class T>
inline const T& need(const Base& cx, std::uint64_t pos, const std::optional<T>& v, std::string_view field, std::string_view of) {
    if (!v) cx.fail(pos, "no field '" + std::string(field) + "' in " + std::string(of));
    return *v;
}

template <class T>
inline const T& element(const Base& cx, std::uint64_t pos, const std::vector<T>& v, std::int64_t i) {
    if (i < 0 || static_cast<std::uint64_t>(i) >= v.size()) cx.fail(pos, "index " + std::to_string(i) + " is outside the array");
    return v[static_cast<std::size_t>(i)];
}

// ---- the built-in functions ----

template <unsigned Width, bool Big>
inline std::int64_t peek(const Base& cx, std::int64_t at) {
    const auto u = static_cast<std::uint64_t>(at);
    if (at < 0 || u > cx.data.size() || Width > cx.data.size() - u) return -1;
    return static_cast<std::int64_t>(load<Width, Big>(cx.data.data() + u));
}

inline std::int64_t scan(const Base& cx, std::int64_t from, std::string_view needle) {
    if (from < 0 || needle.empty() || static_cast<std::uint64_t>(from) > cx.data.size()) return -1;
    const std::string_view hay(reinterpret_cast<const char*>(cx.data.data()), cx.data.size());
    const std::size_t at = hay.find(needle, static_cast<std::size_t>(from));
    return at == std::string_view::npos ? -1 : static_cast<std::int64_t>(at);
}

inline std::int64_t at_text(const Base& cx, std::int64_t o, std::string_view t) {
    const auto u = static_cast<std::uint64_t>(o);
    return o >= 0 && u <= cx.data.size() && t.size() <= cx.data.size() - u && text_at(cx, u, t.size()) == t ? 1 : 0;
}

// sum8 (Printable false) and printable (true) of the n raw bytes at a (the part outside the file counts as zeros).
inline std::int64_t sum_bytes(const Base& cx, std::uint64_t pos, std::int64_t a, std::int64_t b, bool printable, std::string_view name) {
    if (a < 0 || b < 0) cx.fail(pos, std::string(name) + " of a negative range");
    std::int64_t sum = 0;
    for (std::int64_t k = 0; k < b; ++k) {
        const std::uint64_t at = static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(k);
        const unsigned byte = at < cx.data.size() ? cx.data[static_cast<std::size_t>(at)] : 0;
        sum += printable ? (byte >= 0x20 && byte < 0x7F ? 1 : 0) : static_cast<std::int64_t>(byte);
    }
    return sum;
}

inline std::int64_t align(const Base& cx, std::uint64_t pos, std::int64_t a, std::int64_t b) {
    if (b <= 0) cx.fail(pos, "align to a non-positive number");
    return (a + b - 1) / b * b;
}

// The zero-terminated string at c: up to the terminator (kept), `limit` bytes, or the end.
inline std::uint64_t cstr_length(const Base& cx, std::uint64_t c, std::uint64_t limit) {
    std::uint64_t len = 0;
    const std::uint8_t* p = cx.data.data() + c;
    while (len < limit && p[len] != 0) ++len;
    return len < limit ? len + 1 : len;
}

template <class V>
inline bool magic_bytes(V v, std::string_view lit) {
    return v.size() == lit.size() && std::equal(lit.begin(), lit.end(), reinterpret_cast<const char*>(v.data()));
}

}  // namespace cb::formats::gen::rt
