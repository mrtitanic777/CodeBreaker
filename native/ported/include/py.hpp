// py.hpp -- the Python semantics CodeBreaker's ported C++ keeps (C++17, header only).
//
// The Python front end (codebreaker/translate/frontends/python) writes C++ over these types. Each holds
// to Python's behaviour exactly where the two languages differ and a program would notice:
//
//   int        int64_t. The front end proves every int fits (or refuses the function); floordiv, mod,
//              shl, shr give Python's results for negatives and raise as Python does; the checked_*
//              forms raise OverflowError where Python would go on with a big integer (int_policy
//              "checked" only).
//   str        Unicode code points (std::u32string): len, indexing and slicing count characters.
//   bytes      negative indices, clamped slices, an item is an int; decode/encode with the codecs
//              latin1, ascii, utf-8, cp437, cp1252 and the errors strict, replace, ignore.
//   list       a reference (Python's aliasing); negative indices, IndexError; iteration by index over
//              the live size, so appending while iterating behaves as Python.
//   dict       a reference; insertion order kept; at() raises KeyError.
//   struct     unpack/unpack_from/pack/pack_into from the format string, standard sizes.
//   errors     py::ValueError, IndexError, KeyError, StructError ... derived as Python's are.
//
// Text crosses into C++ code as UTF-8 (const char*, std::string); py::str converts.
#pragma once

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "py_unicode.hpp"   // str.isalpha / isdigit / isnumeric over all of Unicode

// the C library's macros that share a name with a Python method
#ifdef isascii
#undef isascii
#endif

namespace py {
template <class T> struct is_std_tuple_ : std::false_type {};
template <class... T> struct is_std_tuple_<std::tuple<T...>> : std::true_type {};
}  // namespace py

namespace py {

template <class T> using ref = std::shared_ptr<T>;

struct none_t {};
inline constexpr none_t none{};

struct object : std::enable_shared_from_this<object> {
    virtual ~object() = default;
};

// ------------------------------------------------------------------------------------------ errors
// (a message is a std::string, a const char*, or a py::str -- anything with .utf8())
#define PY_TEXT_CTOR(Name, Base)                                                              \
    template <class S, class = decltype(std::declval<const S&>().utf8())>                     \
    explicit Name(const S& m) : Base(m.utf8()) {}
struct Exception : std::runtime_error {
    Exception() : std::runtime_error("") {}
    explicit Exception(const std::string& m) : std::runtime_error(m) {}
    explicit Exception(const char* m) : std::runtime_error(m) {}
    PY_TEXT_CTOR(Exception, std::runtime_error)
    virtual const char* type() const { return "Exception"; }
};
#define PY_EXCEPTION(Name, Base)                                    \
    struct Name : Base {                                            \
        Name() : Base() {}                                          \
        explicit Name(const std::string& m) : Base(m) {}            \
        explicit Name(const char* m) : Base(m) {}                   \
        PY_TEXT_CTOR(Name, Base)                                    \
        const char* type() const override { return #Name; }         \
    };
PY_EXCEPTION(ArithmeticError, Exception)
PY_EXCEPTION(ZeroDivisionError, ArithmeticError)
PY_EXCEPTION(OverflowError, ArithmeticError)
PY_EXCEPTION(LookupError, Exception)
PY_EXCEPTION(IndexError, LookupError)
PY_EXCEPTION(KeyError, LookupError)
PY_EXCEPTION(ValueError, Exception)
PY_EXCEPTION(UnicodeDecodeError, ValueError)
PY_EXCEPTION(UnicodeEncodeError, ValueError)
PY_EXCEPTION(TypeError, Exception)
PY_EXCEPTION(RuntimeError, Exception)
PY_EXCEPTION(NotImplementedError, RuntimeError)
PY_EXCEPTION(AssertionError, Exception)
PY_EXCEPTION(AttributeError, Exception)
PY_EXCEPTION(EOFError, Exception)
PY_EXCEPTION(OSError, Exception)
PY_EXCEPTION(IOError, OSError)
PY_EXCEPTION(FileNotFoundError, OSError)
PY_EXCEPTION(PermissionError, OSError)
PY_EXCEPTION(TimeoutError, OSError)
PY_EXCEPTION(StructError, Exception)  // struct.error
PY_EXCEPTION(PatternError, Exception)  // re.error (re.PatternError)
PY_EXCEPTION(MemoryError, Exception)
#undef PY_EXCEPTION
#undef PY_TEXT_CTOR

// ------------------------------------------------------------------------------------------ int
inline int64_t floordiv(int64_t a, int64_t b) {
    if (b == 0) throw ZeroDivisionError("integer division or modulo by zero");
    if (a == INT64_MIN && b == -1) throw OverflowError("int too large for int64");
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) --q;
    return q;
}
inline int64_t mod(int64_t a, int64_t b) {
    if (b == 0) throw ZeroDivisionError("integer division or modulo by zero");
    if (b == -1) return 0;
    int64_t r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) r += b;
    return r;
}
inline double floordiv(double a, double b) {
    if (b == 0) throw ZeroDivisionError("float floor division by zero");
    return std::floor(a / b);
}
inline double mod(double a, double b) {
    if (b == 0) throw ZeroDivisionError("float modulo");
    double r = std::fmod(a, b);
    if (r != 0 && ((r < 0) != (b < 0))) r += b;
    return r;
}
inline int64_t shl(int64_t a, int64_t n) {
    if (n < 0) throw ValueError("negative shift count");
    if (a == 0) return 0;
    if (n >= 63) throw OverflowError("int too large for int64");
    int64_t r = static_cast<int64_t>(static_cast<uint64_t>(a) << n);
    if ((r >> n) != a) throw OverflowError("int too large for int64");
    return r;
}
inline int64_t shr(int64_t a, int64_t n) {
    if (n < 0) throw ValueError("negative shift count");
    if (n >= 63) return a < 0 ? -1 : 0;
    return a >> n;  // arithmetic (MSVC), which is Python's floor
}
inline int64_t checked_add(int64_t a, int64_t b) {
    if ((b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b)) throw OverflowError("int too large for int64");
    return a + b;
}
inline int64_t checked_sub(int64_t a, int64_t b) {
    if ((b < 0 && a > INT64_MAX + b) || (b > 0 && a < INT64_MIN + b)) throw OverflowError("int too large for int64");
    return a - b;
}
inline int64_t checked_mul(int64_t a, int64_t b) {
    if (a == 0 || b == 0) return 0;
    if (a == -1) return checked_sub(0, b);
    if (b == -1) return checked_sub(0, a);
    uint64_t ua = a < 0 ? 0 - static_cast<uint64_t>(a) : static_cast<uint64_t>(a);
    uint64_t ub = b < 0 ? 0 - static_cast<uint64_t>(b) : static_cast<uint64_t>(b);
    bool neg = (a < 0) != (b < 0);
    uint64_t limit = neg ? static_cast<uint64_t>(INT64_MAX) + 1 : static_cast<uint64_t>(INT64_MAX);
    if (ua > limit / ub) throw OverflowError("int too large for int64");
    uint64_t r = ua * ub;
    return neg ? static_cast<int64_t>(0 - r) : static_cast<int64_t>(r);
}
inline int64_t checked_neg(int64_t a) { return checked_sub(0, a); }
inline int64_t checked_shl(int64_t a, int64_t n) { return shl(a, n); }
inline int64_t ipow(int64_t a, int64_t e) {
    if (e < 0) throw ValueError("negative exponent for an int result");
    int64_t r = 1;
    while (e > 0) {
        if (e & 1) r = checked_mul(r, a);
        e >>= 1;
        if (e) a = checked_mul(a, a);
    }
    return r;
}
inline int64_t checked_pow(int64_t a, int64_t e) { return ipow(a, e); }
inline uint64_t mulmod_(uint64_t a, uint64_t b, uint64_t m) {  // a, b < m < 2**63
    uint64_t r = 0;
    while (b) {
        if (b & 1) r = (r + a) % m;
        a = (a + a) % m;
        b >>= 1;
    }
    return r;
}
inline int64_t powmod(int64_t a, int64_t e, int64_t m) {
    if (m == 0) throw ValueError("pow() 3rd argument cannot be 0");
    if (e < 0) throw ValueError("pow() with a negative exponent and a modulus");
    uint64_t um = static_cast<uint64_t>(m < 0 ? -m : m);
    uint64_t r = 1 % um, b = static_cast<uint64_t>(mod(a, static_cast<int64_t>(um)));
    while (e > 0) {
        if (e & 1) r = mulmod_(r, b, um);
        b = mulmod_(b, b, um);
        e >>= 1;
    }
    int64_t out = static_cast<int64_t>(r);
    if (m < 0 && out != 0) out += m;
    return out;
}
inline double truediv(double a, double b) {
    if (b == 0) throw ZeroDivisionError("division by zero");
    return a / b;
}
inline double truediv(int64_t a, int64_t b) {
    if (b == 0) throw ZeroDivisionError("division by zero");
    return static_cast<double>(a) / static_cast<double>(b);
}
inline double truediv(int64_t a, double b) { return truediv(static_cast<double>(a), b); }
inline double truediv(double a, int64_t b) { return truediv(a, static_cast<double>(b)); }
inline double pow(double a, double b) { return std::pow(a, b); }
inline int64_t bit_length(int64_t x) {
    uint64_t v = x < 0 ? 0 - static_cast<uint64_t>(x) : static_cast<uint64_t>(x);
    int64_t n = 0;
    while (v) {
        ++n;
        v >>= 1;
    }
    return n;
}
inline int64_t abs(int64_t x) {
    if (x == INT64_MIN) throw OverflowError("int too large for int64");
    return x < 0 ? -x : x;
}
inline double abs(double x) { return std::fabs(x); }
inline std::tuple<int64_t, int64_t> divmod(int64_t a, int64_t b) { return {floordiv(a, b), mod(a, b)}; }
inline int64_t int_from_float(double x) {
    if (std::isnan(x)) throw ValueError("cannot convert float NaN to integer");
    double t = std::trunc(x);
    if (!(t >= -9223372036854775808.0 && t < 9223372036854775808.0)) throw OverflowError("float too large for int64");
    return static_cast<int64_t>(t);
}
inline int64_t floor_int(double x) { return int_from_float(std::floor(x)); }
inline int64_t ceil_int(double x) { return int_from_float(std::ceil(x)); }
inline int64_t round_int(double x) { return int_from_float(std::nearbyint(x)); }  // half to even

// ------------------------------------------------------------------------------------------ slices
// Python's slice of a sequence of n items: [lo:hi] clamped, negatives from the end.
inline void clamp_(int64_t n, int64_t& lo, int64_t& hi) {
    if (lo < 0) lo = std::max<int64_t>(lo + n, 0);
    if (lo > n) lo = n;
    if (hi < 0) hi = std::max<int64_t>(hi + n, 0);
    if (hi > n) hi = n;
    if (hi < lo) hi = lo;
}
inline int64_t index_(int64_t i, int64_t n, const char* what) {
    int64_t j = i < 0 ? i + n : i;
    if (j < 0 || j >= n) throw IndexError(std::string(what) + " index out of range");
    return j;
}
// the indices of [lo:hi:step] (step != 0), as Python's slice.indices
inline std::vector<int64_t> slice_indices_(int64_t n, std::optional<int64_t> lo, std::optional<int64_t> hi, int64_t step) {
    if (step == 0) throw ValueError("slice step cannot be zero");
    int64_t start, stop;
    if (step > 0) {
        start = lo ? *lo : 0;
        stop = hi ? *hi : n;
        if (start < 0) start = std::max<int64_t>(start + n, 0);
        if (start > n) start = n;
        if (stop < 0) stop = std::max<int64_t>(stop + n, 0);
        if (stop > n) stop = n;
    } else {
        start = lo ? *lo : n - 1;
        stop = hi ? *hi : -1 - n;
        if (start < 0) start = std::max<int64_t>(start + n, -1);
        if (start >= n) start = n - 1;
        if (stop < 0) stop = std::max<int64_t>(stop + n, -1);
        if (stop >= n) stop = n - 1;
    }
    std::vector<int64_t> out;
    for (int64_t i = start; step > 0 ? i < stop : i > stop; i += step) out.push_back(i);
    return out;
}
inline std::optional<int64_t> opt_(none_t) { return std::nullopt; }
inline std::optional<int64_t> opt_(int64_t v) { return v; }

// ------------------------------------------------------------------------------------------ str
class bytes;

class str {
    std::u32string s_;

  public:
    str() = default;
    str(const char* utf8) : s_(decode_utf8_(utf8, std::strlen(utf8))) {}
    str(const std::string& utf8) : s_(decode_utf8_(utf8.data(), utf8.size())) {}
    explicit str(std::u32string s) : s_(std::move(s)) {}
    explicit str(char32_t c) : s_(1, c) {}

    static std::u32string decode_utf8_(const char* p, size_t n) {
        std::u32string out;
        out.reserve(n);
        for (size_t i = 0; i < n;) {
            unsigned char c = static_cast<unsigned char>(p[i]);
            char32_t cp;
            int len;
            if (c < 0x80) { cp = c; len = 1; }
            else if ((c >> 5) == 6) { cp = c & 0x1F; len = 2; }
            else if ((c >> 4) == 14) { cp = c & 0x0F; len = 3; }
            else { cp = c & 0x07; len = 4; }
            for (int k = 1; k < len && i + k < n; ++k) cp = (cp << 6) | (static_cast<unsigned char>(p[i + k]) & 0x3F);
            out.push_back(cp);
            i += len;
        }
        return out;
    }
    std::string utf8() const {
        std::string out;
        for (char32_t c : s_) {
            if (c < 0x80) out.push_back(static_cast<char>(c));
            else if (c < 0x800) { out.push_back(static_cast<char>(0xC0 | (c >> 6))); out.push_back(static_cast<char>(0x80 | (c & 0x3F))); }
            else if (c < 0x10000) {
                out.push_back(static_cast<char>(0xE0 | (c >> 12)));
                out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
            } else {
                out.push_back(static_cast<char>(0xF0 | (c >> 18)));
                out.push_back(static_cast<char>(0x80 | ((c >> 12) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
            }
        }
        return out;
    }
    const std::u32string& u32() const { return s_; }
    int64_t size() const { return static_cast<int64_t>(s_.size()); }
    bool empty() const { return s_.empty(); }
    char32_t at_(int64_t i) const { return s_[static_cast<size_t>(i)]; }

    str operator[](int64_t i) const { return str(s_[static_cast<size_t>(index_(i, size(), "string"))]); }
    str slice(int64_t lo, int64_t hi) const {
        clamp_(size(), lo, hi);
        return str(s_.substr(static_cast<size_t>(lo), static_cast<size_t>(hi - lo)));
    }
    str slice(none_t, int64_t hi) const { return slice(0, hi); }
    str slice(int64_t lo, none_t) const { return slice(lo, size()); }
    template <class A, class B> str slice(A lo, B hi, int64_t step) const {
        std::u32string out;
        for (int64_t i : slice_indices_(size(), opt_(lo), opt_(hi), step)) out.push_back(s_[static_cast<size_t>(i)]);
        return str(out);
    }
    str slice_from(int64_t lo) const { return slice(lo, size()); }
    str slice_to(int64_t hi) const { return slice(0, hi); }

    friend bool operator==(const str& a, const str& b) { return a.s_ == b.s_; }
    friend bool operator!=(const str& a, const str& b) { return a.s_ != b.s_; }
    friend bool operator<(const str& a, const str& b) { return a.s_ < b.s_; }
    friend bool operator<=(const str& a, const str& b) { return a.s_ <= b.s_; }
    friend bool operator>(const str& a, const str& b) { return a.s_ > b.s_; }
    friend bool operator>=(const str& a, const str& b) { return a.s_ >= b.s_; }
    friend bool operator==(const str& a, const char* b) { return a == str(b); }
    friend bool operator!=(const str& a, const char* b) { return !(a == str(b)); }
    friend str operator+(const str& a, const str& b) { return str(a.s_ + b.s_); }
    friend str operator+(const str& a, const char* b) { return a + str(b); }
    str& operator+=(const str& b) { s_ += b.s_; return *this; }

    // iteration: each character as a one-character str
    struct iterator {
        const str* s;
        size_t i;
        str operator*() const { return str(s->s_[i]); }
        iterator& operator++() { ++i; return *this; }
        bool operator!=(const iterator& o) const { return i != o.i; }
    };
    iterator begin() const { return {this, 0}; }
    iterator end() const { return {this, s_.size()}; }

    bool contains(const str& sub) const { return s_.find(sub.s_) != std::u32string::npos; }
    int64_t find(const str& sub, int64_t lo = 0, std::optional<int64_t> hi = std::nullopt) const {
        int64_t h = hi ? *hi : size();
        clamp_(size(), lo, h);
        if (h - lo < sub.size()) return -1;
        size_t p = s_.substr(0, static_cast<size_t>(h)).find(sub.s_, static_cast<size_t>(lo));
        return p == std::u32string::npos ? -1 : static_cast<int64_t>(p);
    }
    int64_t rfind(const str& sub, int64_t lo = 0, std::optional<int64_t> hi = std::nullopt) const {
        int64_t h = hi ? *hi : size();
        clamp_(size(), lo, h);
        if (h - lo < sub.size()) return -1;
        size_t p = s_.substr(0, static_cast<size_t>(h)).rfind(sub.s_);
        return (p == std::u32string::npos || static_cast<int64_t>(p) < lo) ? -1 : static_cast<int64_t>(p);
    }
    int64_t index(const str& sub) const {
        int64_t i = find(sub);
        if (i < 0) throw ValueError("substring not found");
        return i;
    }
    int64_t count(const str& sub) const {
        if (sub.empty()) return size() + 1;
        int64_t n = 0;
        for (size_t p = s_.find(sub.s_); p != std::u32string::npos; p = s_.find(sub.s_, p + sub.s_.size())) ++n;
        return n;
    }
    bool startswith(const str& p) const { return s_.compare(0, p.s_.size(), p.s_) == 0 && p.s_.size() <= s_.size(); }
    bool endswith(const str& p) const {
        return p.s_.size() <= s_.size() && s_.compare(s_.size() - p.s_.size(), p.s_.size(), p.s_) == 0;
    }
    template <class... T> bool startswith(const std::tuple<T...>& ps) const {
        return std::apply([&](const auto&... p) { return (startswith(str(p)) || ...); }, ps);
    }
    template <class... T> bool endswith(const std::tuple<T...>& ps) const {
        return std::apply([&](const auto&... p) { return (endswith(str(p)) || ...); }, ps);
    }
    static bool space_(char32_t c) {
        return c == ' ' || (c >= 9 && c <= 13) || (c >= 0x1C && c <= 0x1F) || c == 0x85 || c == 0xA0 || c == 0x1680 ||
               (c >= 0x2000 && c <= 0x200A) || c == 0x2028 || c == 0x2029 || c == 0x202F || c == 0x205F || c == 0x3000;
    }
    str strip_(int mode, const std::u32string* chars) const {
        size_t a = 0, b = s_.size();
        auto drop = [&](char32_t c) { return chars ? chars->find(c) != std::u32string::npos : space_(c); };
        if (mode & 1) while (a < b && drop(s_[a])) ++a;
        if (mode & 2) while (b > a && drop(s_[b - 1])) --b;
        return str(s_.substr(a, b - a));
    }
    str strip() const { return strip_(3, nullptr); }
    str lstrip() const { return strip_(1, nullptr); }
    str rstrip() const { return strip_(2, nullptr); }
    str strip(const str& c) const { return strip_(3, &c.s_); }
    str lstrip(const str& c) const { return strip_(1, &c.s_); }
    str rstrip(const str& c) const { return strip_(2, &c.s_); }
    // case: exact for Latin-1 (the range the ported code meets); beyond it, characters are kept
    static char32_t upper_(char32_t c) {
        if (c >= 'a' && c <= 'z') return c - 32;
        if ((c >= 0xE0 && c <= 0xFE && c != 0xF7)) return c - 32;
        if (c == 0xB5) return 0x39C;
        if (c == 0xFF) return 0x178;
        return c;
    }
    static char32_t lower_(char32_t c) {
        if (c >= 'A' && c <= 'Z') return c + 32;
        if ((c >= 0xC0 && c <= 0xDE && c != 0xD7)) return c + 32;
        return c;
    }
    str upper() const {
        std::u32string o;
        for (char32_t c : s_) {
            if (c == 0xDF) { o += U"SS"; continue; }
            o.push_back(upper_(c));
        }
        return str(o);
    }
    str lower() const {
        std::u32string o(s_);
        for (auto& c : o) c = lower_(c);
        return str(o);
    }
    static bool alpha_(char32_t c) {
        return c < 0x80 ? ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) : unicode_::isalpha(c);
    }
    static bool digit_(char32_t c) { return c >= '0' && c <= '9'; }
    bool isdigit() const { return !s_.empty() && std::all_of(s_.begin(), s_.end(), [](char32_t c) { return digit_(c) || (c >= 0x80 && unicode_::isdigit(c)); }); }
    bool isdecimal() const { return !s_.empty() && std::all_of(s_.begin(), s_.end(), [](char32_t c) { return digit_(c) || (c >= 0x80 && unicode_::isdecimal(c)); }); }
    bool isnumeric() const { return !s_.empty() && std::all_of(s_.begin(), s_.end(), [](char32_t c) { return digit_(c) || (c >= 0x80 && unicode_::isnumeric(c)); }); }
    bool isalpha() const { return !s_.empty() && std::all_of(s_.begin(), s_.end(), alpha_); }
    bool isalnum() const { return !s_.empty() && std::all_of(s_.begin(), s_.end(), [](char32_t c) { return alpha_(c) || digit_(c) || (c >= 0x80 && unicode_::isnumeric(c)); }); }
    bool isspace() const { return !s_.empty() && std::all_of(s_.begin(), s_.end(), space_); }
    bool isupper() const {
        bool cased = false;
        for (char32_t c : s_) {
            if (lower_(c) != c) cased = true;
            else if (upper_(c) != c) return false;
        }
        return cased;
    }
    bool islower() const {
        bool cased = false;
        for (char32_t c : s_) {
            if (upper_(c) != c || c == 0xDF) cased = true;
            else if (lower_(c) != c) return false;
        }
        return cased;
    }
    bool isascii() const { return std::all_of(s_.begin(), s_.end(), [](char32_t c) { return c < 0x80; }); }
    bool isidentifier() const {
        if (s_.empty() || !(alpha_(s_[0]) || s_[0] == '_')) return false;
        return std::all_of(s_.begin(), s_.end(), [](char32_t c) { return alpha_(c) || digit_(c) || c == '_'; });
    }
    static bool printable_(char32_t c) { return !(c < 0x20 || (c >= 0x7F && c <= 0xA0) || c == 0xAD); }
    bool isprintable() const { return std::all_of(s_.begin(), s_.end(), printable_); }
    str replace(const str& a, const str& b) const {
        if (a.empty()) {
            std::u32string o = b.s_;
            for (char32_t c : s_) { o.push_back(c); o += b.s_; }
            return str(o);
        }
        std::u32string o;
        size_t i = 0;
        for (size_t p = s_.find(a.s_); p != std::u32string::npos; p = s_.find(a.s_, i)) {
            o += s_.substr(i, p - i);
            o += b.s_;
            i = p + a.s_.size();
        }
        o += s_.substr(i);
        return str(o);
    }
    str ljust(int64_t w, const str& fill = str(" ")) const {
        if (size() >= w) return *this;
        return str(s_ + std::u32string(static_cast<size_t>(w - size()), fill.s_[0]));
    }
    str rjust(int64_t w, const str& fill = str(" ")) const {
        if (size() >= w) return *this;
        return str(std::u32string(static_cast<size_t>(w - size()), fill.s_[0]) + s_);
    }
    str center(int64_t w, const str& fill = str(" ")) const {
        if (size() >= w) return *this;
        int64_t marg = w - size();
        int64_t left = marg / 2 + (marg & w & 1);
        return str(std::u32string(static_cast<size_t>(left), fill.s_[0]) + s_ +
                   std::u32string(static_cast<size_t>(marg - left), fill.s_[0]));
    }
    str zfill(int64_t w) const {
        if (size() >= w) return *this;
        std::u32string pad(static_cast<size_t>(w - size()), '0');
        if (!s_.empty() && (s_[0] == '+' || s_[0] == '-')) return str(s_.substr(0, 1) + pad + s_.substr(1));
        return str(pad + s_);
    }
    str removeprefix(const str& p) const { return startswith(p) ? slice_from(p.size()) : *this; }
    str removesuffix(const str& p) const { return (!p.empty() && endswith(p)) ? slice(0, size() - p.size()) : *this; }
    str title() const {
        std::u32string o(s_);
        bool prev = false;
        for (auto& c : o) {
            bool a = alpha_(c);
            c = a ? (prev ? lower_(c) : upper_(c)) : c;
            prev = a;
        }
        return str(o);
    }
    str capitalize() const {
        std::u32string o = lower().s_;
        if (!o.empty()) o[0] = upper_(o[0]);
        return str(o);
    }
    std::tuple<str, str, str> partition(const str& sep) const {
        size_t p = s_.find(sep.s_);
        if (p == std::u32string::npos) return {*this, str(), str()};
        return {str(s_.substr(0, p)), sep, str(s_.substr(p + sep.s_.size()))};
    }
    std::tuple<str, str, str> rpartition(const str& sep) const {
        size_t p = s_.rfind(sep.s_);
        if (p == std::u32string::npos) return {str(), str(), *this};
        return {str(s_.substr(0, p)), sep, str(s_.substr(p + sep.s_.size()))};
    }
    template <class L> L split_impl_() const;
    bytes encode(const char* enc = "utf-8", const char* errors = "strict") const;
};

struct str_hash {
    size_t operator()(const str& s) const { return std::hash<std::u32string>()(s.u32()); }
};

// ------------------------------------------------------------------------------------------ bytes
inline const char32_t* cp437_high_() {
    static const char32_t t[128] = {
        0xC7, 0xFC, 0xE9, 0xE2, 0xE4, 0xE0, 0xE5, 0xE7, 0xEA, 0xEB, 0xE8, 0xEF, 0xEE, 0xEC, 0xC4, 0xC5,
        0xC9, 0xE6, 0xC6, 0xF4, 0xF6, 0xF2, 0xFB, 0xF9, 0xFF, 0xD6, 0xDC, 0xA2, 0xA3, 0xA5, 0x20A7, 0x192,
        0xE1, 0xED, 0xF3, 0xFA, 0xF1, 0xD1, 0xAA, 0xBA, 0xBF, 0x2310, 0xAC, 0xBD, 0xBC, 0xA1, 0xAB, 0xBB,
        0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
        0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
        0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
        0x3B1, 0xDF, 0x393, 0x3C0, 0x3A3, 0x3C3, 0xB5, 0x3C4, 0x3A6, 0x398, 0x3A9, 0x3B4, 0x221E, 0x3C6, 0x3B5, 0x2229,
        0x2261, 0xB1, 0x2265, 0x2264, 0x2320, 0x2321, 0xF7, 0x2248, 0xB0, 0x2219, 0xB7, 0x221A, 0x207F, 0xB2, 0x25A0, 0xA0};
    return t;
}
inline const char32_t* cp1252_high_() {  // 0x80..0x9F; 0 = undefined in Python's codec
    static const char32_t t[32] = {0x20AC, 0, 0x201A, 0x192, 0x201E, 0x2026, 0x2020, 0x2021, 0x2C6, 0x2030, 0x160, 0x2039, 0x152, 0, 0x17D, 0,
                                   0, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x2DC, 0x2122, 0x161, 0x203A, 0x153, 0, 0x17E, 0x178};
    return t;
}

class bytearray;

class bytes {
    friend class bytearray;

  protected:
    std::string b_;

  public:
    bytes() = default;
    bytes(const char* s) : b_(s) {}
    bytes(const char* s, size_t n) : b_(s, n) {}
    explicit bytes(std::string s) : b_(std::move(s)) {}
    static bytes zeros(int64_t n) {
        if (n < 0) throw ValueError("negative count");
        return bytes(std::string(static_cast<size_t>(n), '\0'));
    }
    template <class L> static bytes from_list(const L& items) {
        std::string s;
        for (int64_t v : items) {
            if (v < 0 || v > 255) throw ValueError("bytes must be in range(0, 256)");
            s.push_back(static_cast<char>(v));
        }
        return bytes(s);
    }
    static bytes fromhex(const str& h) {
        std::string s;
        const auto& u = h.u32();
        auto val = [](char32_t c) -> int {
            if (c >= '0' && c <= '9') return int(c - '0');
            if (c >= 'a' && c <= 'f') return int(c - 'a' + 10);
            if (c >= 'A' && c <= 'F') return int(c - 'A' + 10);
            return -1;
        };
        for (size_t i = 0; i < u.size();) {
            if (str::space_(u[i])) { ++i; continue; }
            if (i + 1 >= u.size() || val(u[i]) < 0 || val(u[i + 1]) < 0) throw ValueError("non-hexadecimal number found in fromhex() arg");
            s.push_back(static_cast<char>(val(u[i]) * 16 + val(u[i + 1])));
            i += 2;
        }
        return bytes(s);
    }
    const std::string& raw() const { return b_; }
    int64_t size() const { return static_cast<int64_t>(b_.size()); }
    bool empty() const { return b_.empty(); }
    int64_t byte_(int64_t i) const { return static_cast<unsigned char>(b_[static_cast<size_t>(i)]); }
    int64_t operator[](int64_t i) const { return byte_(index_(i, size(), "index")); }

    bytes slice(int64_t lo, int64_t hi) const {
        clamp_(size(), lo, hi);
        return bytes(b_.substr(static_cast<size_t>(lo), static_cast<size_t>(hi - lo)));
    }
    bytes slice(none_t, int64_t hi) const { return slice(0, hi); }
    bytes slice(int64_t lo, none_t) const { return slice(lo, size()); }
    template <class A, class B> bytes slice(A lo, B hi, int64_t step) const {
        std::string out;
        for (int64_t i : slice_indices_(size(), opt_(lo), opt_(hi), step)) out.push_back(b_[static_cast<size_t>(i)]);
        return bytes(out);
    }
    bytes slice_from(int64_t lo) const { return slice(lo, size()); }
    bytes slice_to(int64_t hi) const { return slice(0, hi); }

    friend bool operator==(const bytes& a, const bytes& b) { return a.b_ == b.b_; }
    friend bool operator!=(const bytes& a, const bytes& b) { return a.b_ != b.b_; }
    friend bool operator<(const bytes& a, const bytes& b) {
        return std::lexicographical_compare(a.b_.begin(), a.b_.end(), b.b_.begin(), b.b_.end(),
                                            [](char x, char y) { return static_cast<unsigned char>(x) < static_cast<unsigned char>(y); });
    }
    friend bool operator>(const bytes& a, const bytes& b) { return b < a; }
    friend bool operator<=(const bytes& a, const bytes& b) { return !(b < a); }
    friend bool operator>=(const bytes& a, const bytes& b) { return !(a < b); }
    friend bytes operator+(const bytes& a, const bytes& b) { return bytes(a.b_ + b.b_); }

    struct iterator {
        const std::string* s;
        size_t i;
        int64_t operator*() const { return static_cast<unsigned char>((*s)[i]); }
        iterator& operator++() { ++i; return *this; }
        bool operator!=(const iterator& o) const { return i != o.i; }
    };
    iterator begin() const { return {&b_, 0}; }
    iterator end() const { return {&b_, b_.size()}; }

    bool contains(const bytes& sub) const { return b_.find(sub.b_) != std::string::npos; }
    bool contains(int64_t v) const {
        if (v < 0 || v > 255) throw ValueError("byte must be in range(0, 256)");
        return b_.find(static_cast<char>(v)) != std::string::npos;
    }
    int64_t find(const bytes& sub, int64_t lo = 0, std::optional<int64_t> hi = std::nullopt) const {
        int64_t h = hi ? *hi : size();
        clamp_(size(), lo, h);
        if (h - lo < sub.size()) return -1;
        size_t p = std::string_view(b_).substr(0, static_cast<size_t>(h)).find(sub.b_, static_cast<size_t>(lo));
        return p == std::string::npos ? -1 : static_cast<int64_t>(p);
    }
    int64_t find(int64_t v, int64_t lo = 0, std::optional<int64_t> hi = std::nullopt) const {
        if (v < 0 || v > 255) throw ValueError("byte must be in range(0, 256)");
        return find(bytes(std::string(1, static_cast<char>(v))), lo, hi);
    }
    int64_t rfind(const bytes& sub, int64_t lo = 0, std::optional<int64_t> hi = std::nullopt) const {
        int64_t h = hi ? *hi : size();
        clamp_(size(), lo, h);
        if (h - lo < sub.size()) return -1;
        size_t p = std::string_view(b_).substr(0, static_cast<size_t>(h)).rfind(sub.b_);
        return (p == std::string::npos || static_cast<int64_t>(p) < lo) ? -1 : static_cast<int64_t>(p);
    }
    int64_t index(const bytes& sub, int64_t lo = 0, std::optional<int64_t> hi = std::nullopt) const {
        int64_t i = find(sub, lo, hi);
        if (i < 0) throw ValueError("subsection not found");
        return i;
    }
    int64_t count(const bytes& sub) const {
        if (sub.empty()) return size() + 1;
        int64_t n = 0;
        for (size_t p = b_.find(sub.b_); p != std::string::npos; p = b_.find(sub.b_, p + sub.b_.size())) ++n;
        return n;
    }
    bool startswith(const bytes& p) const { return b_.size() >= p.b_.size() && b_.compare(0, p.b_.size(), p.b_) == 0; }
    bool endswith(const bytes& p) const {
        return b_.size() >= p.b_.size() && b_.compare(b_.size() - p.b_.size(), p.b_.size(), p.b_) == 0;
    }
    template <class... T> bool startswith(const std::tuple<T...>& ps) const {
        return std::apply([&](const auto&... p) { return (startswith(bytes(p)) || ...); }, ps);
    }
    template <class... T> bool endswith(const std::tuple<T...>& ps) const {
        return std::apply([&](const auto&... p) { return (endswith(bytes(p)) || ...); }, ps);
    }
    static bool space_(char c) { return c == ' ' || (c >= 9 && c <= 13); }
    bytes strip_(int mode, const std::string* chars) const {
        size_t a = 0, b = b_.size();
        auto drop = [&](char c) { return chars ? chars->find(c) != std::string::npos : space_(c); };
        if (mode & 1) while (a < b && drop(b_[a])) ++a;
        if (mode & 2) while (b > a && drop(b_[b - 1])) --b;
        return bytes(b_.substr(a, b - a));
    }
    bytes strip() const { return strip_(3, nullptr); }
    bytes lstrip() const { return strip_(1, nullptr); }
    bytes rstrip() const { return strip_(2, nullptr); }
    bytes strip(const bytes& c) const { return strip_(3, &c.b_); }
    bytes lstrip(const bytes& c) const { return strip_(1, &c.b_); }
    bytes rstrip(const bytes& c) const { return strip_(2, &c.b_); }
    bytes upper() const {
        std::string o(b_);
        for (auto& c : o) if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 32);
        return bytes(o);
    }
    bytes lower() const {
        std::string o(b_);
        for (auto& c : o) if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
        return bytes(o);
    }
    static bool alpha_(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    static bool digit_(char c) { return c >= '0' && c <= '9'; }
    bool isalnum() const { return !b_.empty() && std::all_of(b_.begin(), b_.end(), [](char c) { return alpha_(c) || digit_(c); }); }
    bool isalpha() const { return !b_.empty() && std::all_of(b_.begin(), b_.end(), alpha_); }
    bool isdigit() const { return !b_.empty() && std::all_of(b_.begin(), b_.end(), digit_); }
    bool isspace() const { return !b_.empty() && std::all_of(b_.begin(), b_.end(), space_); }
    bool isupper() const {
        bool cased = false;
        for (char c : b_) {
            if (c >= 'a' && c <= 'z') return false;
            if (c >= 'A' && c <= 'Z') cased = true;
        }
        return cased;
    }
    bool islower() const {
        bool cased = false;
        for (char c : b_) {
            if (c >= 'A' && c <= 'Z') return false;
            if (c >= 'a' && c <= 'z') cased = true;
        }
        return cased;
    }
    bytes replace(const bytes& a, const bytes& b) const {
        std::string o;
        if (a.b_.empty()) {
            o = b.b_;
            for (char c : b_) { o.push_back(c); o += b.b_; }
            return bytes(o);
        }
        size_t i = 0;
        for (size_t p = b_.find(a.b_); p != std::string::npos; p = b_.find(a.b_, i)) {
            o += b_.substr(i, p - i);
            o += b.b_;
            i = p + a.b_.size();
        }
        o += b_.substr(i);
        return bytes(o);
    }
    bytes ljust(int64_t w, const bytes& fill = bytes(" ")) const {
        return size() >= w ? *this : bytes(b_ + std::string(static_cast<size_t>(w - size()), fill.b_[0]));
    }
    bytes rjust(int64_t w, const bytes& fill = bytes(" ")) const {
        return size() >= w ? *this : bytes(std::string(static_cast<size_t>(w - size()), fill.b_[0]) + b_);
    }
    str hex() const {
        static const char* d = "0123456789abcdef";
        std::string o;
        for (unsigned char c : b_) { o.push_back(d[c >> 4]); o.push_back(d[c & 15]); }
        return str(o);
    }
    template <class L> bytes join(const L& parts) const {
        std::string o;
        bool first = true;
        for (const auto& p : parts) {
            if (!first) o += b_;
            o += p.raw();
            first = false;
        }
        return bytes(o);
    }

    str decode(const char* enc = "utf-8", const char* errors = "strict") const {
        std::string e(enc);
        std::string err(errors);
        std::u32string out;
        out.reserve(b_.size());
        auto bad = [&](size_t i, const char* why) {
            if (err == "strict") {
                char m[160];
                std::snprintf(m, sizeof m, "'%s' codec can't decode byte 0x%02x in position %zu: %s", enc,
                              static_cast<unsigned>(static_cast<unsigned char>(b_[i])), i, why);
                throw UnicodeDecodeError(m);
            }
            if (err == "replace") out.push_back(0xFFFD);
        };
        if (e == "latin1" || e == "latin-1" || e == "iso-8859-1") {
            for (unsigned char c : b_) out.push_back(c);
        } else if (e == "ascii") {
            for (size_t i = 0; i < b_.size(); ++i) {
                unsigned char c = static_cast<unsigned char>(b_[i]);
                if (c < 0x80) out.push_back(c);
                else bad(i, "ordinal not in range(128)");
            }
        } else if (e == "cp437") {
            for (unsigned char c : b_) out.push_back(c < 0x80 ? c : cp437_high_()[c - 0x80]);
        } else if (e == "cp1252") {
            for (size_t i = 0; i < b_.size(); ++i) {
                unsigned char c = static_cast<unsigned char>(b_[i]);
                if (c >= 0x80 && c < 0xA0) {
                    char32_t u = cp1252_high_()[c - 0x80];
                    if (u) out.push_back(u);
                    else bad(i, "character maps to <undefined>");
                } else out.push_back(c);
            }
        } else if (e == "utf-8" || e == "utf8") {
            size_t i = 0, n = b_.size();
            const unsigned char* p = reinterpret_cast<const unsigned char*>(b_.data());
            while (i < n) {
                unsigned char c = p[i];
                if (c < 0x80) { out.push_back(c); ++i; continue; }
                int len = (c >= 0xC2 && c <= 0xDF) ? 2 : (c >= 0xE0 && c <= 0xEF) ? 3 : (c >= 0xF0 && c <= 0xF4) ? 4 : 0;
                if (!len) { bad(i, "invalid start byte"); ++i; continue; }
                char32_t cp = c & (len == 2 ? 0x1F : len == 3 ? 0x0F : 0x07);
                size_t k = 1;
                bool ok = true;
                for (; k < static_cast<size_t>(len); ++k) {
                    if (i + k >= n) { ok = false; break; }
                    unsigned char d = p[i + k];
                    unsigned char lo = 0x80, hi = 0xBF;
                    if (k == 1) {
                        if (c == 0xE0) lo = 0xA0;
                        if (c == 0xED) hi = 0x9F;
                        if (c == 0xF0) lo = 0x90;
                        if (c == 0xF4) hi = 0x8F;
                    }
                    if (d < lo || d > hi) { ok = false; break; }
                    cp = (cp << 6) | (d & 0x3F);
                }
                if (!ok) {
                    bad(i, i + k >= n ? "unexpected end of data" : "invalid continuation byte");
                    i += k;
                    continue;
                }
                out.push_back(cp);
                i += len;
            }
        } else {
            throw LookupError(std::string("unknown encoding: ") + enc);
        }
        return str(out);
    }
};

// bytearray: Python's is a mutable object shared by every name that holds it, so this is a handle to
// the bytes; it reads as bytes (it converts to const bytes&) and changes in place
// a[lo:hi]'s span as Python takes it: a negative bound counts from the end, both clamped to [0, n], hi >= lo
inline int64_t slice_bound_(int64_t v, int64_t n) { return v < 0 ? std::max<int64_t>(0, v + n) : std::min(v, n); }
inline int64_t slice_bound_(none_t, int64_t) { return -1; }
template <class A, class B> std::pair<int64_t, int64_t> slice_span_(A lo, B hi, int64_t n) {
    int64_t a = slice_bound_(lo, n), b = slice_bound_(hi, n);
    if constexpr (std::is_same_v<A, none_t>) a = 0;
    if constexpr (std::is_same_v<B, none_t>) b = n;
    return {a, std::max(a, b)};
}

class bytearray {
    std::shared_ptr<bytes> p_;

  public:
    bytearray() : p_(std::make_shared<bytes>()) {}
    bytearray(const char* s, size_t n) : p_(std::make_shared<bytes>(s, n)) {}
    explicit bytearray(const bytes& b) : p_(std::make_shared<bytes>(b)) {}
    static bytearray zeros(int64_t n) { return bytearray(bytes::zeros(n)); }
    template <class L> static bytearray from_list(const L& items) { return bytearray(bytes::from_list(items)); }
    const bytes& view() const { return *p_; }
    operator const bytes&() const { return *p_; }
    const std::string& raw() const { return p_->raw(); }
    std::string& raw_mut() const { return p_->b_; }
    int64_t size() const { return p_->size(); }
    bool empty() const { return p_->empty(); }
    struct item_ref {
        std::string* s;
        size_t i;
        operator int64_t() const { return static_cast<unsigned char>((*s)[i]); }
        item_ref& operator=(int64_t v) {
            if (v < 0 || v > 255) throw ValueError("byte must be in range(0, 256)");
            (*s)[i] = static_cast<char>(v);
            return *this;
        }
    };
    item_ref operator[](int64_t i) const { return {&p_->b_, static_cast<size_t>(index_(i, size(), "bytearray"))}; }
    // b[lo:hi] = v: the span replaced by v's bytes (the length may change)
    template <class A, class B> void set_slice(A lo, B hi, const bytes& v) const {
        auto [a, b] = slice_span_(lo, hi, size());
        std::string with = v.raw();   // (a copy: v may be this one)
        p_->b_.replace(static_cast<size_t>(a), static_cast<size_t>(b - a), with);
    }
    bytearray slice(int64_t lo, int64_t hi) const { return bytearray(p_->slice(lo, hi)); }
    bytearray slice(none_t, int64_t hi) const { return bytearray(p_->slice(0, hi)); }
    bytearray slice(int64_t lo, none_t) const { return bytearray(p_->slice(lo, size())); }
    template <class A, class B> bytearray slice(A lo, B hi, int64_t step) const { return bytearray(p_->slice(lo, hi, step)); }
    bytearray slice_from(int64_t lo) const { return bytearray(p_->slice_from(lo)); }
    bytearray slice_to(int64_t hi) const { return bytearray(p_->slice_to(hi)); }
    void append(int64_t v) const {
        if (v < 0 || v > 255) throw ValueError("byte must be in range(0, 256)");
        p_->b_.push_back(static_cast<char>(v));
    }
    void extend(const bytes& b) const { p_->b_ += b.raw(); }
    const bytearray& operator+=(const bytes& b) const { p_->b_ += b.raw(); return *this; }
    bytes::iterator begin() const { return p_->begin(); }
    bytes::iterator end() const { return p_->end(); }
    template <class X> bool contains(const X& x) const { return p_->contains(x); }
    template <class... A> int64_t find(const A&... a) const { return p_->find(a...); }
    template <class... A> int64_t rfind(const A&... a) const { return p_->rfind(a...); }
    template <class... A> int64_t index(const A&... a) const { return p_->index(a...); }
    template <class X> int64_t count(const X& x) const { return p_->count(x); }
    template <class X> bool startswith(const X& x) const { return p_->startswith(x); }
    template <class X> bool endswith(const X& x) const { return p_->endswith(x); }
    str decode(const char* enc = "utf-8", const char* errors = "strict") const { return p_->decode(enc, errors); }
    str hex() const { return p_->hex(); }
    bytearray upper() const { return bytearray(p_->upper()); }
    bytearray lower() const { return bytearray(p_->lower()); }
    bytearray strip() const { return bytearray(p_->strip()); }
    bytearray rstrip() const { return bytearray(p_->rstrip()); }
    bytearray lstrip() const { return bytearray(p_->lstrip()); }
    bool isalnum() const { return p_->isalnum(); }
    bool isalpha() const { return p_->isalpha(); }
    bool isdigit() const { return p_->isdigit(); }
    friend bool operator==(const bytearray& a, const bytes& b) { return a.view() == b; }
    friend bool operator!=(const bytearray& a, const bytes& b) { return !(a.view() == b); }
    friend bool operator<(const bytearray& a, const bytes& b) { return a.view() < b; }
    friend bytearray operator+(const bytearray& a, const bytes& b) { return bytearray(a.view() + b); }
};

struct bytes_hash {
    size_t operator()(const bytes& b) const { return std::hash<std::string>()(b.raw()); }
};

inline bytes str::encode(const char* enc, const char* errors) const {
    std::string e(enc), err(errors);
    if (e == "utf-8" || e == "utf8") return bytes(utf8());
    std::string out;
    size_t limit = (e == "ascii") ? 0x80 : 0x100;
    for (size_t i = 0; i < s_.size(); ++i) {
        char32_t c = s_[i];
        if (e == "cp1252" && c >= 0x80) {
            int hit = -1;
            for (int k = 0; k < 32; ++k) if (cp1252_high_()[k] == c) hit = 0x80 + k;
            if (hit >= 0) { out.push_back(static_cast<char>(hit)); continue; }
            if (c >= 0xA0 && c < 0x100) { out.push_back(static_cast<char>(c)); continue; }
        } else if (e == "cp437" && c >= 0x80) {
            int hit = -1;
            for (int k = 0; k < 128; ++k) if (cp437_high_()[k] == c) hit = 0x80 + k;
            if (hit >= 0) { out.push_back(static_cast<char>(hit)); continue; }
        } else if (c < limit) {
            out.push_back(static_cast<char>(c));
            continue;
        }
        if (e != "latin1" && e != "latin-1" && e != "iso-8859-1" && e != "ascii" && e != "cp437" && e != "cp1252")
            throw LookupError("unknown encoding: " + e);
        if (err == "strict") throw UnicodeEncodeError("'" + e + "' codec can't encode character");
        if (err == "replace") out.push_back('?');
    }
    return bytes(out);
}

// ------------------------------------------------------------------------------------------ hashing
template <class T, class = void> struct hasher { size_t operator()(const T& v) const { return std::hash<T>()(v); } };
template <> struct hasher<str> : str_hash {};
template <> struct hasher<bytes> : bytes_hash {};
template <class T> struct hasher<std::optional<T>> {   // (a dict keyed by a value or None)
    size_t operator()(const std::optional<T>& v) const { return v ? hasher<T>()(*v) * 31 + 1 : 0; }
};
template <class... A> struct hasher<std::variant<A...>> {   // (a dict keyed by values of several kinds)
    size_t operator()(const std::variant<A...>& v) const {
        return std::visit([](const auto& x) { return hasher<std::decay_t<decltype(x)>>()(x); }, v) * 31 + v.index();
    }
};
template <class... T> struct hasher<std::tuple<T...>> {
    size_t operator()(const std::tuple<T...>& t) const {
        size_t h = 0x345678;
        std::apply([&](const auto&... x) { ((h = h * 1000003 ^ hasher<std::decay_t<decltype(x)>>()(x)), ...); }, t);
        return h;
    }
};

// ------------------------------------------------------------------------------------------ list
template <class T> class list {
    std::shared_ptr<std::vector<T>> v_;

  public:
    using value_type = T;
    list() : v_(std::make_shared<std::vector<T>>()) {}
    list(std::initializer_list<T> xs) : v_(std::make_shared<std::vector<T>>(xs)) {}
    explicit list(std::vector<T> xs) : v_(std::make_shared<std::vector<T>>(std::move(xs))) {}
    template <class It> static list from(const It& items) {
        list out;
        for (const auto& x : items) out.v_->push_back(x);
        return out;
    }
    // a[lo:hi] = v: the span replaced by v's items (the length may change)
    template <class A, class B> void set_slice(A lo, B hi, const list& v) const {
        auto [a, b] = slice_span_(lo, hi, static_cast<int64_t>(v_->size()));
        std::vector<T> with = *v.v_;   // (a copy: v may be this one)
        v_->erase(v_->begin() + a, v_->begin() + b);
        v_->insert(v_->begin() + a, with.begin(), with.end());
    }
    template <class... A> static list from(const std::tuple<A...>& items) {   // list(a_tuple)
        list out;
        std::apply([&](const auto&... x) { (out.v_->push_back(x), ...); }, items);
        return out;
    }
    std::vector<T>& vec() const { return *v_; }
    int64_t size() const { return static_cast<int64_t>(v_->size()); }
    bool empty() const { return v_->empty(); }
    T& operator[](int64_t i) const { return (*v_)[static_cast<size_t>(index_(i, size(), "list"))]; }
    void append(const T& x) const { v_->push_back(x); }
    template <class L> void extend(const L& xs) const {
        std::vector<T> add;
        for (const auto& x : xs) add.push_back(x);
        v_->insert(v_->end(), add.begin(), add.end());
    }
    void insert(int64_t i, const T& x) const {
        int64_t n = size();
        if (i < 0) i = std::max<int64_t>(i + n, 0);
        if (i > n) i = n;
        v_->insert(v_->begin() + i, x);
    }
    T pop(int64_t i = -1) const {
        if (v_->empty()) throw IndexError("pop from empty list");
        int64_t j = index_(i, size(), "pop");
        T x = (*v_)[static_cast<size_t>(j)];
        v_->erase(v_->begin() + j);
        return x;
    }
    int64_t index(const T& x) const {
        for (size_t i = 0; i < v_->size(); ++i) if ((*v_)[i] == x) return static_cast<int64_t>(i);
        throw ValueError("value is not in list");
    }
    int64_t count(const T& x) const { return static_cast<int64_t>(std::count(v_->begin(), v_->end(), x)); }
    void remove(const T& x) const { v_->erase(v_->begin() + index(x)); }
    void reverse() const { std::reverse(v_->begin(), v_->end()); }
    void clear() const { v_->clear(); }
    list copy() const { return list(*v_); }
    bool contains(const T& x) const { return std::find(v_->begin(), v_->end(), x) != v_->end(); }
    list slice(int64_t lo, int64_t hi) const {
        clamp_(size(), lo, hi);
        return list(std::vector<T>(v_->begin() + lo, v_->begin() + hi));
    }
    list slice(none_t, int64_t hi) const { return slice(0, hi); }
    list slice(int64_t lo, none_t) const { return slice(lo, size()); }
    template <class A, class B> list slice(A lo, B hi, int64_t step) const {
        list out;
        for (int64_t i : slice_indices_(size(), opt_(lo), opt_(hi), step)) out.v_->push_back((*v_)[static_cast<size_t>(i)]);
        return out;
    }
    list slice_from(int64_t lo) const { return slice(lo, size()); }
    list slice_to(int64_t hi) const { return slice(0, hi); }
    template <class K = std::nullptr_t> void sort(K key = nullptr, bool reverse = false) const;

    // iteration by index over the live size: appending while iterating sees the new items (Python)
    struct iterator {
        const std::vector<T>* v;
        size_t i;
        const T& operator*() const { return (*v)[i]; }
        iterator& operator++() { ++i; return *this; }
        bool operator!=(const iterator&) const { return i < v->size(); }
    };
    iterator begin() const { return {v_.get(), 0}; }
    iterator end() const { return {v_.get(), 0}; }

    friend bool operator==(const list& a, const list& b) { return *a.v_ == *b.v_; }
    friend bool operator!=(const list& a, const list& b) { return *a.v_ != *b.v_; }
    friend bool operator<(const list& a, const list& b) { return *a.v_ < *b.v_; }
    friend list operator+(const list& a, const list& b) {
        std::vector<T> v(*a.v_);
        v.insert(v.end(), b.v_->begin(), b.v_->end());
        return list(v);
    }
    bool identical_(const list& o) const { return v_ == o.v_; }
};

// ------------------------------------------------------------------------------------------ dict
template <class T> struct is_shared_ptr_ : std::false_type {};
template <class T> struct is_shared_ptr_<std::shared_ptr<T>> : std::true_type {};
template <class K, class V> class dict {
    struct impl {
        std::vector<std::optional<std::pair<K, V>>> items;  // insertion order; removed ones are empty
        std::unordered_map<K, size_t, hasher<K>> where;
        size_t live = 0;
    };
    std::shared_ptr<impl> d_;

  public:
    dict() : d_(std::make_shared<impl>()) {}
    dict(std::initializer_list<std::pair<K, V>> xs) : d_(std::make_shared<impl>()) {
        for (const auto& kv : xs) set(kv.first, kv.second);
    }
    int64_t size() const { return static_cast<int64_t>(d_->live); }
    bool empty() const { return d_->live == 0; }
    bool contains(const K& k) const { return d_->where.count(k) != 0; }
    const V& at(const K& k) const {
        auto it = d_->where.find(k);
        if (it == d_->where.end()) throw KeyError("key not found");
        return d_->items[it->second]->second;
    }
    V& set(const K& k, const V& v) const {
        auto it = d_->where.find(k);
        if (it != d_->where.end()) {
            d_->items[it->second]->second = v;
            return d_->items[it->second]->second;
        }
        d_->where.emplace(k, d_->items.size());
        d_->items.emplace_back(std::make_pair(k, v));
        ++d_->live;
        return d_->items.back()->second;
    }
    V& operator[](const K& k) const {  // a store: d[k] = v
        auto it = d_->where.find(k);
        if (it != d_->where.end()) return d_->items[it->second]->second;
        return set(k, V());
    }
    V get(const K& k, const V& dflt) const {
        auto it = d_->where.find(k);
        return it == d_->where.end() ? dflt : d_->items[it->second]->second;
    }
    auto get(const K& k) const {   // d.get(k): None when absent (a shared object's None is nullptr)
        auto it = d_->where.find(k);
        if constexpr (is_shared_ptr_<V>::value) {
            return it == d_->where.end() ? V() : d_->items[it->second]->second;
        } else {
            if (it == d_->where.end()) return std::optional<V>();
            return std::optional<V>(d_->items[it->second]->second);
        }
    }
    V& setdefault(const K& k, const V& v) const {
        auto it = d_->where.find(k);
        if (it != d_->where.end()) return d_->items[it->second]->second;
        return set(k, v);
    }
    V pop(const K& k) const {
        auto it = d_->where.find(k);
        if (it == d_->where.end()) throw KeyError("key not found");
        V v = d_->items[it->second]->second;
        d_->items[it->second].reset();
        d_->where.erase(it);
        --d_->live;
        return v;
    }
    V pop(const K& k, const V& dflt) const { return contains(k) ? pop(k) : dflt; }
    void update(const dict& o) const {
        for (const auto& kv : o.items()) set(std::get<0>(kv), std::get<1>(kv));
    }
    dict copy() const {
        dict out;
        for (const auto& kv : items()) out.set(std::get<0>(kv), std::get<1>(kv));
        return out;
    }
    void clear() const { *d_ = impl(); }
    std::vector<std::tuple<K, V>> items() const {
        std::vector<std::tuple<K, V>> out;
        for (const auto& x : d_->items) if (x) out.emplace_back(x->first, x->second);
        return out;
    }
    std::vector<K> keys() const {
        std::vector<K> out;
        for (const auto& x : d_->items) if (x) out.push_back(x->first);
        return out;
    }
    std::vector<V> values() const {
        std::vector<V> out;
        for (const auto& x : d_->items) if (x) out.push_back(x->second);
        return out;
    }
    struct iterator {  // iterating a dict: its keys
        const impl* d;
        size_t i;
        void skip() { while (i < d->items.size() && !d->items[i]) ++i; }
        const K& operator*() const { return d->items[i]->first; }
        iterator& operator++() { ++i; skip(); return *this; }
        bool operator!=(const iterator& o) const { return i != o.i; }
    };
    iterator begin() const { iterator it{d_.get(), 0}; it.skip(); return it; }
    iterator end() const { return {d_.get(), d_->items.size()}; }
    friend bool operator==(const dict& a, const dict& b) {
        if (a.size() != b.size()) return false;
        for (const auto& kv : a.items()) {
            if (!b.contains(std::get<0>(kv)) || !(b.at(std::get<0>(kv)) == std::get<1>(kv))) return false;
        }
        return true;
    }
};

// ------------------------------------------------------------------------------------------ set
template <class T> class set {
    struct impl {
        std::vector<T> order;
        std::unordered_map<T, size_t, hasher<T>> where;
    };
    std::shared_ptr<impl> s_;

  public:
    set() : s_(std::make_shared<impl>()) {}
    set(std::initializer_list<T> xs) : s_(std::make_shared<impl>()) { for (const auto& x : xs) add(x); }
    template <class It> static set from(const It& items) {
        set out;
        if constexpr (is_std_tuple_<It>::value) std::apply([&](const auto&... x) { (out.add(x), ...); }, items);   // frozenset(("a", "b"))
        else
            for (const auto& x : items) out.add(x);
        return out;
    }
    int64_t size() const { return static_cast<int64_t>(s_->where.size()); }
    bool empty() const { return s_->where.empty(); }
    bool contains(const T& x) const { return s_->where.count(x) != 0; }
    void add(const T& x) const {
        if (!contains(x)) {
            s_->where.emplace(x, s_->order.size());
            s_->order.push_back(x);
        }
    }
    void discard(const T& x) const {
        auto it = s_->where.find(x);
        if (it == s_->where.end()) return;
        size_t i = it->second;
        s_->where.erase(it);
        s_->order.erase(s_->order.begin() + i);
        for (auto& kv : s_->where) if (kv.second > i) --kv.second;
    }
    void remove(const T& x) const {
        if (!contains(x)) throw KeyError("element not in set");
        discard(x);
    }
    template <class L> void update(const L& xs) const { for (const auto& x : xs) add(x); }
    bool issubset(const set& o) const {
        for (const auto& x : s_->order) if (!o.contains(x)) return false;
        return true;
    }
    bool issuperset(const set& o) const { return o.issubset(*this); }
    bool isdisjoint(const set& o) const {
        for (const auto& x : s_->order) if (o.contains(x)) return false;
        return true;
    }
    // (iteration exists for sorted() and the set operations: the front end refuses code that depends
    // on a set's iteration order, which is CPython's hash layout)
    typename std::vector<T>::const_iterator begin() const { return s_->order.begin(); }
    typename std::vector<T>::const_iterator end() const { return s_->order.end(); }
    friend set operator|(const set& a, const set& b) { set o = from(a); o.update(b); return o; }
    friend set operator&(const set& a, const set& b) {
        set o;
        for (const auto& x : a) if (b.contains(x)) o.add(x);
        return o;
    }
    friend set operator-(const set& a, const set& b) {
        set o;
        for (const auto& x : a) if (!b.contains(x)) o.add(x);
        return o;
    }
    friend set operator^(const set& a, const set& b) { return (a - b) | (b - a); }
    friend bool operator==(const set& a, const set& b) { return a.size() == b.size() && a.issubset(b); }
};

// `x in (a, b, c)`
template <class X, class... T> bool in(const X& x, const std::tuple<T...>& t) {
    return std::apply([&](const auto&... y) { return ((x == y) || ...); }, t);
}
template <class L> bool any_of(const L& xs);
template <class L> bool all_of(const L& xs);

// ------------------------------------------------------------------------------------------ len, truth
template <class T> int64_t len(const T& x) { return x.size(); }
template <class... T> int64_t len(const std::tuple<T...>&) { return sizeof...(T); }

inline bool truthy(bool b) { return b; }
inline bool truthy(int64_t v) { return v != 0; }
inline bool truthy(int v) { return v != 0; }
inline bool truthy(double v) { return v != 0.0; }
inline bool truthy(none_t) { return false; }
inline bool truthy(const str& s) { return !s.empty(); }
inline bool truthy(const bytes& s) { return !s.empty(); }
template <class T> bool truthy(const list<T>& x) { return !x.empty(); }
template <class K, class V> bool truthy(const dict<K, V>& x) { return !x.empty(); }
template <class T> bool truthy(const set<T>& x) { return !x.empty(); }
template <class T> bool truthy(const std::optional<T>& x) { return x.has_value() && truthy(*x); }
template <class T> bool truthy(const ref<T>& x) { return x != nullptr; }
template <class... T> bool truthy(const std::tuple<T...>&) { return sizeof...(T) != 0; }
inline bool truthy(const bytearray& b) { return !b.empty(); }
template <class L> bool any_of(const L& xs) {
    for (const auto& x : xs)
        if (truthy(x)) return true;
    return false;
}
template <class L> bool all_of(const L& xs) {
    for (const auto& x : xs)
        if (!truthy(x)) return false;
    return true;
}

// a or b / a and b as values (Python gives back an operand); b is a plain value here: the front end
// writes these only when evaluating b first cannot be told apart
template <class T, class U> T or_(const T& a, const U& b) { return truthy(a) ? a : T(b); }
template <class T, class U> T and_(const T& a, const U& b) { return truthy(a) ? T(b) : a; }
template <class T, class U> T or_none(const std::optional<T>& a, const U& b) { return truthy(a) ? *a : T(b); }

// ------------------------------------------------------------------------------------------ text of values
inline str to_str(const str& s) { return s; }
inline str to_str(int64_t v) { return str(std::to_string(v)); }
inline str to_str(int v) { return str(std::to_string(v)); }
inline str to_str(bool b) { return str(b ? "True" : "False"); }
inline str to_str(none_t) { return str("None"); }
inline str float_repr_(double v) {
    if (std::isnan(v)) return str("nan");
    if (std::isinf(v)) return str(v > 0 ? "inf" : "-inf");
    if (v == 0) return str(std::signbit(v) ? "-0.0" : "0.0");
    char buf[64];
    auto r = std::to_chars(buf, buf + sizeof buf, v, std::chars_format::scientific);
    std::string sci(buf, r.ptr);  // d.ddde+XX, shortest round-trip digits
    size_t e = sci.find('e');
    std::string mant = sci.substr(0, e);
    int exp = std::stoi(sci.substr(e + 1));
    bool neg = mant[0] == '-';
    if (neg) mant = mant.substr(1);
    std::string digits;
    for (char c : mant) if (c != '.') digits.push_back(c);
    std::string out;
    if (exp < -4 || exp >= 16) {
        out = digits.substr(0, 1);
        if (digits.size() > 1) out += "." + digits.substr(1);
        char eb[16];
        std::snprintf(eb, sizeof eb, "e%c%02d", exp < 0 ? '-' : '+', exp < 0 ? -exp : exp);
        out += eb;
    } else if (exp < 0) {
        out = "0." + std::string(static_cast<size_t>(-exp - 1), '0') + digits;
    } else {
        int point = exp + 1;
        if (static_cast<int>(digits.size()) <= point) out = digits + std::string(static_cast<size_t>(point - digits.size()), '0') + ".0";
        else out = digits.substr(0, static_cast<size_t>(point)) + "." + digits.substr(static_cast<size_t>(point));
    }
    return str((neg ? "-" : "") + out);
}
inline str to_str(double v) { return float_repr_(v); }

inline str repr(const str& s) {
    const auto& u = s.u32();
    bool sq = u.find(U'\'') != std::u32string::npos, dq = u.find(U'"') != std::u32string::npos;
    char32_t q = (sq && !dq) ? U'"' : U'\'';
    std::u32string o(1, q);
    for (char32_t c : u) {
        if (c == q || c == U'\\') { o.push_back(U'\\'); o.push_back(c); }
        else if (c == U'\t') o += U"\\t";
        else if (c == U'\n') o += U"\\n";
        else if (c == U'\r') o += U"\\r";
        else if (c < 0x20 || c == 0x7F || (c >= 0x80 && c <= 0xA0) || c == 0xAD) {
            char b[8];
            std::snprintf(b, sizeof b, "\\x%02x", static_cast<unsigned>(c));
            o += str(b).u32();
        } else o.push_back(c);
    }
    o.push_back(q);
    return str(o);
}
inline str repr(const bytes& b) {
    const std::string& r = b.raw();
    bool sq = r.find('\'') != std::string::npos, dq = r.find('"') != std::string::npos;
    char q = (sq && !dq) ? '"' : '\'';
    std::string o = "b";
    o.push_back(q);
    for (unsigned char c : r) {
        if (c == q || c == '\\') { o.push_back('\\'); o.push_back(static_cast<char>(c)); }
        else if (c == '\t') o += "\\t";
        else if (c == '\n') o += "\\n";
        else if (c == '\r') o += "\\r";
        else if (c < 0x20 || c >= 0x7F) {
            char x[8];
            std::snprintf(x, sizeof x, "\\x%02x", c);
            o += x;
        } else o.push_back(static_cast<char>(c));
    }
    o.push_back(q);
    return str(o);
}
inline str repr(const bytearray& b) { return str("bytearray(") + repr(b.view()) + str(")"); }
inline str to_str(const bytearray& b) { return repr(b); }
inline str repr(int64_t v) { return to_str(v); }
inline str repr(bool v) { return to_str(v); }
inline str repr(double v) { return to_str(v); }
inline str repr(none_t) { return str("None"); }
inline str to_str(const bytes& b) { return repr(b); }
template <class T> str repr(const std::optional<T>& v) { return v ? repr(*v) : str("None"); }
// a record (a Python dict of known keys, a generated struct with for_each_field_mut): the dict's repr, its keys
// in the order the fields are declared; a key the dict may lack, absent, is left out
struct any_field {   // (what a record's for_each_field_mut is called with, to tell a record from other types)
    template <class F> void operator()(const char*, F&, bool) const {}
};
template <class T, class = void> struct is_record : std::false_type {};
template <class T>
struct is_record<T, std::void_t<decltype(for_each_field_mut(std::declval<T&>(), any_field{}))>> : std::true_type {};
template <class T> struct is_optional_t : std::false_type {};
template <class T> struct is_optional_t<std::optional<T>> : std::true_type {};
template <class T, std::enable_if_t<is_record<T>::value, int> = 0> str repr(const T& r) {
    str o("{");
    size_t i = 0;
    for_each_field_mut(const_cast<T&>(r), [&](const char* name, auto& f, bool required) {
        if constexpr (is_optional_t<std::decay_t<decltype(f)>>::value)
            if (!required && !f) return;
        o += (i++ ? str(", ") : str("")) + repr(str(name)) + str(": ") + repr(f);
    });
    o += str("}");
    return o;
}
template <class T, std::enable_if_t<is_record<T>::value, int> = 0> str to_str(const T& r) { return repr(r); }
template <class T> str to_str(const std::optional<T>& v) { return v ? to_str(*v) : str("None"); }
template <class T> str repr(const list<T>& xs) {
    str o("[");
    bool first = true;
    for (const auto& x : xs) {
        if (!first) o += str(", ");
        o += repr(x);
        first = false;
    }
    return o + str("]");
}
template <class... T> str repr(const std::tuple<T...>& t) {
    str o("(");
    size_t i = 0;
    std::apply([&](const auto&... x) { ((o += (i++ ? str(", ") : str("")) + repr(x)), ...); }, t);
    if (sizeof...(T) == 1) o += str(",");
    return o + str(")");
}
template <class T> str to_str(const list<T>& xs) { return repr(xs); }
template <class... T> str to_str(const std::tuple<T...>& t) { return repr(t); }

inline str hex(int64_t v) {
    char b[32];
    if (v < 0) std::snprintf(b, sizeof b, "-0x%llx", static_cast<unsigned long long>(0 - static_cast<uint64_t>(v)));
    else std::snprintf(b, sizeof b, "0x%llx", static_cast<unsigned long long>(v));
    return str(b);
}
inline std::string base_digits_(uint64_t v, int base) {
    if (v == 0) return "0";
    std::string o;
    while (v) { o.push_back("0123456789abcdef"[v % base]); v /= base; }
    std::reverse(o.begin(), o.end());
    return o;
}
inline str oct(int64_t v) { return str((v < 0 ? "-0o" : "0o") + base_digits_(v < 0 ? 0 - static_cast<uint64_t>(v) : v, 8)); }
inline str bin(int64_t v) { return str((v < 0 ? "-0b" : "0b") + base_digits_(v < 0 ? 0 - static_cast<uint64_t>(v) : v, 2)); }
inline str chr(int64_t v) {
    if (v < 0 || v > 0x10FFFF) throw ValueError("chr() arg not in range(0x110000)");
    return str(static_cast<char32_t>(v));
}
inline int64_t ord(const str& s) {
    if (s.size() != 1) throw TypeError("ord() expected a character");
    return s.at_(0);
}

// format(value, spec): [[fill]align][sign][#][0][width][,][.precision][type]
struct spec_ {
    char32_t fill = ' ';
    char align = 0, sign = '-', type = 0;
    bool alt = false, zero = false, comma = false;
    int width = 0, prec = -1;
};
inline spec_ parse_spec_(const char* s) {
    spec_ p;
    str t(s);
    const auto& u = t.u32();
    size_t i = 0;
    auto isalign = [](char32_t c) { return c == '<' || c == '>' || c == '^' || c == '='; };
    if (u.size() >= 2 && isalign(u[1])) { p.fill = u[0]; p.align = static_cast<char>(u[1]); i = 2; }
    else if (!u.empty() && isalign(u[0])) { p.align = static_cast<char>(u[0]); i = 1; }
    if (i < u.size() && (u[i] == '+' || u[i] == '-' || u[i] == ' ')) p.sign = static_cast<char>(u[i++]);
    if (i < u.size() && u[i] == '#') { p.alt = true; ++i; }
    if (i < u.size() && u[i] == '0') { p.zero = true; ++i; }
    while (i < u.size() && u[i] >= '0' && u[i] <= '9') p.width = p.width * 10 + int(u[i++] - '0');
    if (i < u.size() && u[i] == ',') { p.comma = true; ++i; }
    if (i < u.size() && u[i] == '.') {
        ++i;
        p.prec = 0;
        while (i < u.size() && u[i] >= '0' && u[i] <= '9') p.prec = p.prec * 10 + int(u[i++] - '0');
    }
    if (i < u.size()) p.type = static_cast<char>(u[i++]);
    if (p.zero && !p.align) { p.fill = '0'; p.align = '='; }
    return p;
}
inline str pad_(const std::u32string& sign, const std::u32string& body, const spec_& p, char dflt) {
    int64_t n = static_cast<int64_t>(sign.size() + body.size());
    char a = p.align ? p.align : dflt;
    if (n >= p.width) return str(sign + body);
    std::u32string fill(static_cast<size_t>(p.width - n), p.fill);
    if (a == '<') return str(sign + body + fill);
    if (a == '>') return str(fill + sign + body);
    if (a == '=') return str(sign + fill + body);
    size_t left = fill.size() / 2;
    return str(std::u32string(left, p.fill) + sign + body + std::u32string(fill.size() - left, p.fill));
}
inline std::string group_(const std::string& d) {
    std::string o;
    int k = 0;
    for (size_t i = d.size(); i-- > 0;) {
        o.push_back(d[i]);
        if (++k % 3 == 0 && i) o.push_back(',');
    }
    std::reverse(o.begin(), o.end());
    return o;
}
inline str format(int64_t v, const char* spec) {
    spec_ p = parse_spec_(spec);
    if (p.type == 'f' || p.type == 'F' || p.type == 'e' || p.type == 'E' || p.type == 'g' || p.type == '%') {
        throw NotImplementedError("float format of an int");
    }
    if (p.type == 'c') return pad_(U"", chr(v).u32(), p, '<');
    uint64_t mag = v < 0 ? 0 - static_cast<uint64_t>(v) : static_cast<uint64_t>(v);
    std::string digits, prefix;
    switch (p.type) {
        case 'x': digits = base_digits_(mag, 16); prefix = p.alt ? "0x" : ""; break;
        case 'X': digits = base_digits_(mag, 16); for (auto& c : digits) c = static_cast<char>(std::toupper(c)); prefix = p.alt ? "0X" : ""; break;
        case 'o': digits = base_digits_(mag, 8); prefix = p.alt ? "0o" : ""; break;
        case 'b': digits = base_digits_(mag, 2); prefix = p.alt ? "0b" : ""; break;
        default: digits = base_digits_(mag, 10);
    }
    if (p.comma) digits = group_(digits);
    std::string sign = v < 0 ? "-" : (p.sign == '+' ? "+" : (p.sign == ' ' ? " " : ""));
    return pad_(str(sign + prefix).u32(), str(digits).u32(), p, '>');
}
inline str format(bool v, const char* spec) { return spec[0] ? format(static_cast<int64_t>(v), spec) : to_str(v); }
inline str format(const str& s, const char* spec) {
    spec_ p = parse_spec_(spec);
    std::u32string body = s.u32();
    if (p.prec >= 0 && static_cast<int64_t>(body.size()) > p.prec) body = body.substr(0, static_cast<size_t>(p.prec));
    return pad_(U"", body, p, '<');
}
inline str format(double v, const char* spec) {
    spec_ p = parse_spec_(spec);
    if (!p.type && p.prec < 0) return pad_(U"", to_str(v).u32(), p, '>');
    char t = p.type ? p.type : 'g';
    int prec = p.prec < 0 ? 6 : p.prec;
    char fmt[16], buf[512];
    double mag = std::fabs(v);
    if (t == '%') { mag *= 100; t = 'f'; }
    std::snprintf(fmt, sizeof fmt, "%%.%d%c", prec, t == 'F' ? 'f' : t);
    std::snprintf(buf, sizeof buf, fmt, mag);
    std::string body(buf);
    if (p.type == '%') body += "%";
    if (p.comma) {
        size_t dot = body.find('.');
        body = group_(body.substr(0, dot)) + (dot == std::string::npos ? "" : body.substr(dot));
    }
    std::string sign = std::signbit(v) ? "-" : (p.sign == '+' ? "+" : (p.sign == ' ' ? " " : ""));
    return pad_(str(sign).u32(), str(body).u32(), p, '>');
}

// "fmt" % values: %d %i %x %X %o %s %r %c %f %e %g with flags - + space # 0, a width and a precision
inline void percent_one_(std::u32string& out, const std::string& flags, int width, int prec, char conv, int64_t v) {
    if (conv == 'c') { out += chr(v).u32(); return; }
    if (conv == 's' || conv == 'r') conv = 'd';
    std::string spec;
    bool left = flags.find('-') != std::string::npos;
    if (left) spec += "<";
    if (flags.find('+') != std::string::npos) spec += "+";
    else if (flags.find(' ') != std::string::npos) spec += " ";
    if (flags.find('#') != std::string::npos && conv != 'd' && conv != 'i') spec += "#";
    if (flags.find('0') != std::string::npos && !left) spec += "0";
    if (width) spec += std::to_string(width);
    str digits = format(v, (spec + (conv == 'i' || conv == 'd' || conv == 'u' ? "" : std::string(1, conv))).c_str());
    if (prec > 0) {  // %.Nd: at least N digits
        std::u32string d = digits.u32();
        size_t start = 0;
        while (start < d.size() && (d[start] == ' ' || d[start] == '-' || d[start] == '+')) ++start;
        std::u32string num = d.substr(start);
        if (static_cast<int>(num.size()) < prec) num = std::u32string(static_cast<size_t>(prec) - num.size(), '0') + num;
        out += d.substr(0, start) + num;
        return;
    }
    out += digits.u32();
}
inline void percent_one_(std::u32string& out, const std::string& flags, int width, int prec, char conv, const str& v) {
    str s = conv == 'r' ? repr(v) : v;
    std::u32string body = s.u32();
    if (prec >= 0 && static_cast<int>(body.size()) > prec) body = body.substr(0, static_cast<size_t>(prec));
    if (static_cast<int>(body.size()) < width) {
        std::u32string fill(static_cast<size_t>(width) - body.size(), ' ');
        body = flags.find('-') != std::string::npos ? body + fill : fill + body;
    }
    out += body;
}
inline void percent_one_(std::u32string& out, const std::string& flags, int width, int prec, char conv, double v) {
    if (conv == 's' || conv == 'r') { percent_one_(out, flags, width, prec, 's', to_str(v)); return; }
    std::string spec = flags.find('-') != std::string::npos ? "<" : "";
    if (flags.find('+') != std::string::npos) spec += "+";
    if (flags.find('0') != std::string::npos) spec += "0";
    if (width) spec += std::to_string(width);
    spec += "." + std::to_string(prec < 0 ? 6 : prec) + std::string(1, conv);
    out += format(v, spec.c_str()).u32();
}
inline void percent_one_(std::u32string& out, const std::string& f, int w, int p, char c, int v) {
    percent_one_(out, f, w, p, c, static_cast<int64_t>(v));
}
inline void percent_one_(std::u32string& out, const std::string& f, int w, int p, char c, const char* v) {
    percent_one_(out, f, w, p, c, str(v));
}
inline void percent_one_(std::u32string& out, const std::string& f, int w, int p, char c, bool v) {
    if (c == 's' || c == 'r') percent_one_(out, f, w, p, 's', to_str(v));
    else percent_one_(out, f, w, p, c, static_cast<int64_t>(v));
}
inline void percent_one_(std::u32string& out, const std::string& f, int w, int p, char c, const bytes& v) {
    percent_one_(out, f, w, p, 's', repr(v));
}
inline void percent_one_(std::u32string& out, const std::string& f, int w, int p, char c, none_t) {
    percent_one_(out, f, w, p, 's', str("None"));
}
template <class... A> void percent_one_(std::u32string& out, const std::string& f, int w, int p, char c, const std::variant<A...>& v) {
    std::visit([&](const auto& x) { percent_one_(out, f, w, p, c, x); }, v);   // (the value it holds, as itself)
}
template <class T> void percent_one_(std::u32string& out, const std::string& f, int w, int p, char c, const std::optional<T>& v) {
    if (v) percent_one_(out, f, w, p, c, *v);
    else percent_one_(out, f, w, p, 's', str("None"));
}
inline void percent_walk_(const std::u32string& fmt, size_t& i, std::u32string& out) {
    while (i < fmt.size()) {
        if (fmt[i] != '%') { out.push_back(fmt[i++]); continue; }
        if (i + 1 < fmt.size() && fmt[i + 1] == '%') { out.push_back('%'); i += 2; continue; }
        return;
    }
}
inline void percent_spec_(const std::u32string& fmt, size_t& i, std::string& flags, int& width, int& prec, char& conv) {
    ++i;
    while (i < fmt.size() && std::string("-+ #0").find(static_cast<char>(fmt[i])) != std::string::npos) flags.push_back(static_cast<char>(fmt[i++]));
    while (i < fmt.size() && fmt[i] >= '0' && fmt[i] <= '9') width = width * 10 + int(fmt[i++] - '0');
    if (i < fmt.size() && fmt[i] == '.') {
        ++i;
        prec = 0;
        while (i < fmt.size() && fmt[i] >= '0' && fmt[i] <= '9') prec = prec * 10 + int(fmt[i++] - '0');
    }
    if (i >= fmt.size()) throw ValueError("incomplete format");
    conv = static_cast<char>(fmt[i++]);
}
template <class... A> str percent(const str& fmt, const A&... args) {
    const auto& f = fmt.u32();
    size_t i = 0;
    std::u32string out;
    auto one = [&](const auto& v) {
        percent_walk_(f, i, out);
        if (i >= f.size()) throw TypeError("not all arguments converted during string formatting");
        std::string flags;
        int width = 0, prec = -1;
        char conv = 0;
        percent_spec_(f, i, flags, width, prec, conv);
        percent_one_(out, flags, width, prec, conv, v);
    };
    (one(args), ...);
    percent_walk_(f, i, out);
    if (i < f.size()) throw TypeError("not enough arguments for format string");
    return str(out);
}
template <class... A> str percent(const str& fmt, const std::tuple<A...>& t) {
    return std::apply([&](const auto&... a) { return percent(fmt, a...); }, t);
}

// ------------------------------------------------------------------------------------------ str methods needing list
template <> inline list<str> str::split_impl_<list<str>>() const {
    list<str> out;
    size_t i = 0, n = s_.size();
    while (i < n) {
        while (i < n && space_(s_[i])) ++i;
        if (i >= n) break;
        size_t j = i;
        while (j < n && !space_(s_[j])) ++j;
        out.append(str(s_.substr(i, j - i)));
        i = j;
    }
    return out;
}
inline list<str> split(const str& s) { return s.split_impl_<list<str>>(); }
inline list<str> split(const str& s, const str& sep) {
    if (sep.empty()) throw ValueError("empty separator");
    list<str> out;
    const auto& u = s.u32();
    size_t i = 0;
    for (size_t p = u.find(sep.u32()); p != std::u32string::npos; p = u.find(sep.u32(), i)) {
        out.append(str(u.substr(i, p - i)));
        i = p + sep.u32().size();
    }
    out.append(str(u.substr(i)));
    return out;
}
inline list<str> split(const str& s, const str& sep, int64_t maxsplit) {   // s.split(sep, maxsplit)
    if (maxsplit < 0) return split(s, sep);
    if (sep.empty()) throw ValueError("empty separator");
    list<str> out;
    const auto& u = s.u32();
    size_t i = 0;
    for (size_t p = u.find(sep.u32()); p != std::u32string::npos && maxsplit > 0; p = u.find(sep.u32(), i), --maxsplit) {
        out.append(str(u.substr(i, p - i)));
        i = p + sep.u32().size();
    }
    out.append(str(u.substr(i)));
    return out;
}
// iterating a tuple whose items are of one type (for x in (a, b, c))
template <class T, class... R> std::vector<T> tuple_items(const std::tuple<T, R...>& t) {
    std::vector<T> out;
    std::apply([&](const auto&... x) { (out.push_back(x), ...); }, t);
    return out;
}
inline list<bytes> split(const bytes& s, const bytes& sep) {
    if (sep.empty()) throw ValueError("empty separator");
    list<bytes> out;
    const auto& u = s.raw();
    size_t i = 0;
    for (size_t p = u.find(sep.raw()); p != std::string::npos; p = u.find(sep.raw(), i)) {
        out.append(bytes(u.substr(i, p - i)));
        i = p + sep.raw().size();
    }
    out.append(bytes(u.substr(i)));
    return out;
}
inline list<str> splitlines(const str& s) {
    list<str> out;
    const auto& u = s.u32();
    size_t i = 0, n = u.size();
    while (i < n) {
        size_t j = i;
        while (j < n && u[j] != '\n' && u[j] != '\r' && u[j] != 0x0B && u[j] != 0x0C && u[j] != 0x1C && u[j] != 0x1D &&
               u[j] != 0x1E && u[j] != 0x85 && u[j] != 0x2028 && u[j] != 0x2029)
            ++j;
        out.append(str(u.substr(i, j - i)));
        if (j < n && u[j] == '\r' && j + 1 < n && u[j + 1] == '\n') ++j;
        i = j + 1;
    }
    return out;
}
template <class L> str join(const str& sep, const L& parts) {
    std::u32string o;
    bool first = true;
    for (const auto& p : parts) {
        if (!first) o += sep.u32();
        o += str(p).u32();
        first = false;
    }
    return str(o);
}

// ------------------------------------------------------------------------------------------ order, sorting
template <class T> std::optional<int64_t> min_key_(const T&) { return std::nullopt; }
template <class T> T min(const T& a) { return a; }
template <class T, class... R> T min(const T& a, const T& b, const R&... rest) { return min(b < a ? b : a, rest...); }
template <class T> T max(const T& a) { return a; }
template <class T, class... R> T max(const T& a, const T& b, const R&... rest) { return max(a < b ? b : a, rest...); }
inline int64_t min(int64_t a, int64_t b) { return b < a ? b : a; }   // (a literal beside an int64_t)
inline int64_t max(int64_t a, int64_t b) { return a < b ? b : a; }
inline double min(double a, double b) { return b < a ? b : a; }
inline double max(double a, double b) { return a < b ? b : a; }
template <class L> auto min_of(const L& xs) {
    bool any = false;
    std::decay_t<decltype(*std::begin(xs))> best{};
    for (const auto& x : xs) {
        if (!any || x < best) best = x;
        any = true;
    }
    if (!any) throw ValueError("min() arg is an empty sequence");
    return best;
}
template <class L> auto max_of(const L& xs) {
    bool any = false;
    std::decay_t<decltype(*std::begin(xs))> best{};
    for (const auto& x : xs) {
        if (!any || best < x) best = x;
        any = true;
    }
    if (!any) throw ValueError("max() arg is an empty sequence");
    return best;
}
template <class L> auto sum(const L& xs) {
    std::decay_t<decltype(*std::begin(xs))> total{};
    for (const auto& x : xs) total = total + x;
    return total;
}
template <class L, class S> auto sum(const L& xs, S start) {
    for (const auto& x : xs) start = start + x;
    return start;
}

// Python's sort: stable; reverse keeps the order of equal items
template <class T> template <class K> void list<T>::sort(K key, bool reverse) const {
    auto& v = *v_;
    if constexpr (std::is_same_v<K, std::nullptr_t>) {
        if (reverse) std::stable_sort(v.begin(), v.end(), [](const T& a, const T& b) { return b < a; });
        else std::stable_sort(v.begin(), v.end(), [](const T& a, const T& b) { return a < b; });
    } else {
        using KT = std::decay_t<decltype(key(v[0]))>;
        std::vector<std::pair<KT, size_t>> keyed;
        keyed.reserve(v.size());
        for (size_t i = 0; i < v.size(); ++i) keyed.emplace_back(key(v[i]), i);
        if (reverse) std::stable_sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return b.first < a.first; });
        else std::stable_sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        std::vector<T> out;
        out.reserve(v.size());
        for (auto& kv : keyed) out.push_back(v[kv.second]);
        v.swap(out);
    }
}
template <class L, class K = std::nullptr_t> auto sorted(const L& xs, K key = nullptr, bool reverse = false) {
    using T = std::decay_t<decltype(*std::begin(xs))>;
    list<T> out = list<T>::from(xs);
    out.sort(key, reverse);
    return out;
}

// ------------------------------------------------------------------------------------------ iteration helpers
template <class L> struct enumerate_view {
    L xs;  // held, not referred to: `for (auto [i, x] : enumerate(f()))` outlives f()'s value (a list is a handle)
    int64_t start;
    struct iterator {
        decltype(std::begin(std::declval<const L&>())) it, end;
        int64_t i;
        auto operator*() const { return std::make_tuple(i, *it); }
        iterator& operator++() { ++it; ++i; return *this; }
        bool operator!=(const iterator& o) const { return it != o.it; }
    };
    iterator begin() const { return {std::begin(xs), std::end(xs), start}; }
    iterator end() const { return {std::end(xs), std::end(xs), 0}; }
};
template <class L> enumerate_view<std::decay_t<L>> enumerate(L&& xs, int64_t start = 0) { return {std::forward<L>(xs), start}; }

template <class A, class B> std::vector<std::tuple<std::decay_t<decltype(*std::begin(std::declval<const A&>()))>,
                                                   std::decay_t<decltype(*std::begin(std::declval<const B&>()))>>>
zip(const A& a, const B& b) {
    std::vector<std::tuple<std::decay_t<decltype(*std::begin(a))>, std::decay_t<decltype(*std::begin(b))>>> out;
    auto i = std::begin(a);
    auto j = std::begin(b);
    for (; i != std::end(a) && j != std::end(b); ++i, ++j) out.emplace_back(*i, *j);
    return out;
}
template <class L> auto reversed(const L& xs) {
    std::vector<std::decay_t<decltype(*std::begin(xs))>> out;
    for (const auto& x : xs) out.push_back(x);
    std::reverse(out.begin(), out.end());
    return out;
}
inline list<int64_t> range(int64_t stop) {
    list<int64_t> out;
    for (int64_t i = 0; i < stop; ++i) out.append(i);
    return out;
}
inline list<int64_t> range(int64_t start, int64_t stop, int64_t step = 1) {
    if (step == 0) throw ValueError("range() arg 3 must not be zero");
    list<int64_t> out;
    for (int64_t i = start; step > 0 ? i < stop : i > stop; i += step) out.append(i);
    return out;
}
template <class T> int64_t bisect_left(const list<T>& a, const T& x) {
    auto& v = a.vec();
    return std::lower_bound(v.begin(), v.end(), x) - v.begin();
}
template <class T> int64_t bisect_right(const list<T>& a, const T& x) {
    auto& v = a.vec();
    return std::upper_bound(v.begin(), v.end(), x) - v.begin();
}
template <class T> list<T> repeat(const list<T>& a, int64_t n) {
    list<T> out;
    for (int64_t i = 0; i < n; ++i) out.extend(a);
    return out;
}
inline str repeat(const str& a, int64_t n) {
    std::u32string o;
    for (int64_t i = 0; i < n; ++i) o += a.u32();
    return str(o);
}
inline bytes repeat(const bytes& a, int64_t n) {
    std::string o;
    for (int64_t i = 0; i < n; ++i) o += a.raw();
    return bytes(o);
}

// ------------------------------------------------------------------------------------------ struct
struct fmt_item_ {
    char code;
    int size;
    int count;  // for 's': its length
};
inline std::pair<bool, std::vector<fmt_item_>> parse_fmt_(const char* f) {
    bool little = true;  // '<' '=' '@' on Windows x64; '>' '!' big
    std::vector<fmt_item_> items;
    const char* p = f;
    if (*p == '<' || *p == '=' || *p == '@') ++p;
    else if (*p == '>' || *p == '!') { little = false; ++p; }
    while (*p) {
        if (*p == ' ') { ++p; continue; }
        int n = -1;
        while (*p >= '0' && *p <= '9') n = (n < 0 ? 0 : n * 10) + (*p++ - '0');
        char c = *p++;
        int sz = 0;
        switch (c) {
            case 'x': case 'c': case 'b': case 'B': case '?': case 's': sz = 1; break;
            case 'h': case 'H': sz = 2; break;
            case 'i': case 'I': case 'l': case 'L': case 'f': sz = 4; break;
            case 'q': case 'Q': case 'd': sz = 8; break;
            default: throw StructError("bad char in struct format");
        }
        if (c == 's') items.push_back({c, n < 0 ? 1 : n, n < 0 ? 1 : n});
        else for (int k = 0; k < (n < 0 ? 1 : n); ++k) items.push_back({c, sz, 1});
    }
    return {little, items};
}
inline int64_t calcsize(const char* f) {
    int64_t n = 0;
    for (auto& it : parse_fmt_(f).second) n += it.size;
    return n;
}
inline uint64_t read_uint_(const unsigned char* p, int size, bool little) {
    uint64_t v = 0;
    for (int k = 0; k < size; ++k) v |= static_cast<uint64_t>(p[little ? k : size - 1 - k]) << (8 * k);
    return v;
}
template <class T> T unpack_value_(const fmt_item_& it, const unsigned char* p, bool little) {
    if constexpr (std::is_same_v<T, bytes>) {
        return bytes(reinterpret_cast<const char*>(p), static_cast<size_t>(it.size));
    } else if constexpr (std::is_same_v<T, double>) {
        uint64_t raw = read_uint_(p, it.size, little);
        if (it.size == 4) { uint32_t r = static_cast<uint32_t>(raw); float f; std::memcpy(&f, &r, 4); return f; }
        double d; std::memcpy(&d, &raw, 8); return d;
    } else if constexpr (std::is_same_v<T, bool>) {
        return p[0] != 0;
    } else {
        uint64_t raw = read_uint_(p, it.size, little);
        bool sgn = it.code == 'b' || it.code == 'h' || it.code == 'i' || it.code == 'l' || it.code == 'q';
        if (sgn && it.size < 8 && (raw >> (8 * it.size - 1)) & 1) raw |= ~0ULL << (8 * it.size);
        if (!sgn && it.size == 8 && (raw >> 63)) throw OverflowError("unsigned 64-bit value beyond int64");
        return static_cast<int64_t>(raw);
    }
}
// a format parsed once: the front end writes struct formats only as literals, so a format's address names it
struct fmt_parsed_ {
    bool little = true;
    std::vector<fmt_item_> items;
    std::vector<std::pair<fmt_item_, int64_t>> values;  // each value's item and offset
    int64_t size = 0;
};
inline const fmt_parsed_& fmt_of_(const char* f) {
    static thread_local std::unordered_map<const char*, fmt_parsed_> cache;
    auto it = cache.find(f);
    if (it != cache.end()) return it->second;
    fmt_parsed_ p;
    auto parsed = parse_fmt_(f);
    p.little = parsed.first;
    p.items = parsed.second;
    for (auto& item : p.items) {
        if (item.code != 'x') p.values.emplace_back(item, p.size);
        p.size += item.size;
    }
    return cache.emplace(f, std::move(p)).first->second;
}
template <class... T> std::tuple<T...> unpack_from(const char* f, const bytes& b, int64_t off = 0) {
    const fmt_parsed_& fm = fmt_of_(f);
    int64_t n = b.size();
    if (off < 0) {
        if (off + n < 0) throw StructError("offset out of range");
        off += n;
    }
    if (n - off < fm.size) throw StructError("unpack_from requires a larger buffer");
    if (fm.values.size() != sizeof...(T)) throw StructError("format and value count differ");
    const unsigned char* base = reinterpret_cast<const unsigned char*>(b.raw().data()) + off;
    size_t k = 0;
    return std::tuple<T...>{(k++, unpack_value_<T>(fm.values[k - 1].first, base + fm.values[k - 1].second, fm.little))...};
}
template <class... T> std::tuple<T...> unpack(const char* f, const bytes& b) {
    if (calcsize(f) != b.size()) throw StructError("unpack requires a buffer of the format's size");
    return unpack_from<T...>(f, b, 0);
}
template <class T> T unpack_from1(const char* f, const bytes& b, int64_t off = 0) { return std::get<0>(unpack_from<T>(f, b, off)); }
template <class T> T unpack1(const char* f, const bytes& b) { return std::get<0>(unpack<T>(f, b)); }

inline void pack_value_(std::string& out, const fmt_item_& it, bool little, int64_t v) {
    if (it.code == 'c' || it.code == 's') throw StructError("argument for 's' must be a bytes object");
    if (it.code == '?') { out.push_back(v != 0 ? 1 : 0); return; }
    bool sgn = it.code == 'b' || it.code == 'h' || it.code == 'i' || it.code == 'l' || it.code == 'q';
    if (it.size < 8) {
        int64_t lo = sgn ? -(int64_t(1) << (8 * it.size - 1)) : 0;
        int64_t hi = sgn ? (int64_t(1) << (8 * it.size - 1)) - 1 : (int64_t(1) << (8 * it.size)) - 1;
        if (v < lo || v > hi) throw StructError("argument out of range");
    } else if (!sgn && v < 0) {
        throw StructError("argument out of range");
    }
    uint64_t u = static_cast<uint64_t>(v);
    for (int k = 0; k < it.size; ++k) {
        int sh = little ? k : it.size - 1 - k;
        out.push_back(static_cast<char>((u >> (8 * sh)) & 0xFF));
    }
}
inline void pack_value_(std::string& out, const fmt_item_& it, bool little, bool v) { pack_value_(out, it, little, static_cast<int64_t>(v)); }
inline void pack_value_(std::string& out, const fmt_item_& it, bool little, int v) { pack_value_(out, it, little, static_cast<int64_t>(v)); }
inline void pack_value_(std::string& out, const fmt_item_& it, bool little, double v) {
    uint64_t raw;
    if (it.size == 4) { float f = static_cast<float>(v); uint32_t r; std::memcpy(&r, &f, 4); raw = r; }
    else std::memcpy(&raw, &v, 8);
    for (int k = 0; k < it.size; ++k) {
        int sh = little ? k : it.size - 1 - k;
        out.push_back(static_cast<char>((raw >> (8 * sh)) & 0xFF));
    }
}
inline void pack_value_(std::string& out, const fmt_item_& it, bool, const bytes& v) {
    if (it.code != 's' && it.code != 'c') throw StructError("required argument is not an integer");
    if (it.code == 'c' && v.size() != 1) throw StructError("char format requires a bytes object of length 1");
    std::string s = v.raw().substr(0, static_cast<size_t>(it.size));
    s.resize(static_cast<size_t>(it.size), '\0');
    out += s;
}
template <class... A> bytes pack(const char* f, const A&... args) {
    auto parsed = parse_fmt_(f);
    std::vector<const fmt_item_*> vals;
    for (auto& it : parsed.second) if (it.code != 'x') vals.push_back(&it);
    if (vals.size() != sizeof...(A)) throw StructError("pack expected a different number of items");
    std::vector<std::string> parts(vals.size());
    size_t k = 0;
    ((pack_value_(parts[k], *vals[k], parsed.first, args), ++k), ...);
    std::string out;
    k = 0;
    for (auto& it : parsed.second) {
        if (it.code == 'x') out.push_back('\0');
        else out += parts[k++];
    }
    return bytes(out);
}
template <class... A> void pack_into(const char* f, const bytearray& buf, int64_t off, const A&... args) {
    bytes b = pack(f, args...);
    int64_t n = buf.size();
    if (off < 0) {
        if (off + n < 0) throw StructError("offset out of range");
        off += n;
    }
    if (n - off < b.size()) throw StructError("pack_into requires a larger buffer");
    buf.raw_mut().replace(static_cast<size_t>(off), static_cast<size_t>(b.size()), b.raw());
}

inline int64_t from_bytes(const bytes& b, const char* order, bool is_signed) {
    bool little = std::strcmp(order, "little") == 0;
    int64_t n = b.size();
    const unsigned char* p = reinterpret_cast<const unsigned char*>(b.raw().data());
    if (n == 0) return 0;
    // more than 8 bytes: fine while the high ones only extend the sign
    uint64_t v = 0;
    for (int64_t k = 0; k < n; ++k) {
        int64_t i = little ? k : n - 1 - k;
        if (k < 8) v |= static_cast<uint64_t>(p[i]) << (8 * k);
    }
    bool neg = is_signed && (p[little ? n - 1 : 0] & 0x80);
    for (int64_t k = 8; k < n; ++k) {
        int64_t i = little ? k : n - 1 - k;
        if (p[i] != (neg ? 0xFF : 0x00)) throw OverflowError("int too large for int64");
    }
    if (n < 8) {
        if (neg) v |= ~0ULL << (8 * n);
        return static_cast<int64_t>(v);
    }
    if (!is_signed && (v >> 63)) throw OverflowError("int too large for int64");
    if (is_signed && n > 8 && (((v >> 63) & 1) != static_cast<uint64_t>(neg))) throw OverflowError("int too large for int64");
    return static_cast<int64_t>(v);
}
inline bytes to_bytes(int64_t v, int64_t length, const char* order, bool is_signed = false) {
    if (!is_signed && v < 0) throw OverflowError("can't convert negative int to unsigned");
    std::string out(static_cast<size_t>(length), '\0');
    bool little = std::strcmp(order, "little") == 0;
    uint64_t u = static_cast<uint64_t>(v);
    for (int64_t k = 0; k < length; ++k) {
        unsigned char byte = k < 8 ? static_cast<unsigned char>((u >> (8 * k)) & 0xFF) : (v < 0 ? 0xFF : 0);
        out[static_cast<size_t>(little ? k : length - 1 - k)] = static_cast<char>(byte);
    }
    if (length < 8) {
        int64_t lo = is_signed ? -(int64_t(1) << (8 * length - 1)) : 0;
        int64_t hi = is_signed ? (int64_t(1) << (8 * length - 1)) - 1 : (int64_t(1) << (8 * length)) - 1;
        if (v < lo || v > hi) throw OverflowError("int too big to convert");
    }
    return bytes(out);
}
inline uint32_t crc32_table_(int i) {
    static uint32_t t[256];
    static bool made = false;
    if (!made) {
        for (uint32_t n = 0; n < 256; ++n) {
            uint32_t c = n;
            for (int k = 0; k < 8; ++k) c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            t[n] = c;
        }
        made = true;
    }
    return t[i];
}
inline int64_t crc32(const bytes& b, int64_t start = 0) {
    uint32_t c = ~static_cast<uint32_t>(start);
    for (unsigned char x : b.raw()) c = crc32_table_((c ^ x) & 0xFF) ^ (c >> 8);
    return static_cast<int64_t>(static_cast<uint32_t>(~c));
}

// ------------------------------------------------------------------------------------------ files, paths
// (files: py_os.hpp, included at the end)

namespace os {
// an OS call's failure as Python raises it: the exception by the system's own error code (py_os.hpp)
[[noreturn]] inline void raise_os_(const std::error_code& ec, const str& path);
namespace path {
inline bool sep_(char32_t c) { return c == '\\' || c == '/'; }
// ntpath.splitdrive for the forms the tools meet: "C:..." and "\\server\share..."
inline std::pair<std::u32string, std::u32string> splitdrive_(const std::u32string& p) {
    if (p.size() >= 2 && p[1] == ':') return {p.substr(0, 2), p.substr(2)};
    if (p.size() >= 2 && sep_(p[0]) && sep_(p[1]) && (p.size() < 3 || !sep_(p[2]))) {
        size_t i = 2;
        while (i < p.size() && !sep_(p[i])) ++i;
        if (i >= p.size()) return {p, U""};
        size_t j = i + 1;
        while (j < p.size() && !sep_(p[j])) ++j;
        return {p.substr(0, j), p.substr(j)};
    }
    return {U"", p};
}
inline std::tuple<str, str> split(const str& s) {
    auto dr = splitdrive_(s.u32());
    const auto& p = dr.second;
    size_t i = p.size();
    while (i > 0 && !sep_(p[i - 1])) --i;
    std::u32string head = p.substr(0, i), tail = p.substr(i);
    size_t h = head.size();
    while (h > 0 && sep_(head[h - 1])) --h;
    std::u32string stripped = head.substr(0, h);
    if (stripped.empty()) stripped = head;
    return {str(dr.first + stripped), str(tail)};
}
inline str basename(const str& s) { return std::get<1>(os::path::split(s)); }
inline str dirname(const str& s) { return std::get<0>(os::path::split(s)); }
inline std::tuple<str, str> splitext(const str& s) {
    const auto& p = s.u32();
    size_t sep = p.find_last_of(U"\\/");
    size_t dot = p.rfind(U'.');
    size_t start = sep == std::u32string::npos ? 0 : sep + 1;
    if (dot == std::u32string::npos || dot < start) return {s, str()};
    size_t k = start;
    while (k < dot && p[k] == '.') ++k;
    if (k == dot) return {s, str()};  // only leading dots: no extension
    return {str(p.substr(0, dot)), str(p.substr(dot))};
}
template <class... R> str join(const str& a, const R&... rest) {
    std::u32string result_drive, result_path;
    auto first = splitdrive_(a.u32());
    result_drive = first.first;
    result_path = first.second;
    auto add = [&](const str& x) {
        auto d = splitdrive_(x.u32());
        const auto& p_drive = d.first;
        const auto& p_path = d.second;
        if (!p_path.empty() && sep_(p_path[0])) {
            if (!p_drive.empty() || result_drive.empty()) result_drive = p_drive;
            result_path = p_path;
            return;
        } else if (!p_drive.empty() && p_drive != result_drive) {
            str a1(p_drive), b1(result_drive);
            if (a1.lower() != b1.lower()) {
                result_drive = p_drive;
                result_path = p_path;
                return;
            }
            result_drive = p_drive;
        }
        if (!result_path.empty() && !sep_(result_path.back())) result_path += U"\\";
        result_path += p_path;
    };
    (add(rest), ...);
    if (!result_path.empty() && !sep_(result_path[0]) && !result_drive.empty() && result_drive.back() != ':')
        return str(result_drive + U"\\" + result_path);
    return str(result_drive + result_path);
}
inline bool exists(const str& s) { std::error_code ec; return std::filesystem::exists(std::filesystem::u8path(s.utf8()), ec); }
inline bool isfile(const str& s) { std::error_code ec; return std::filesystem::is_regular_file(std::filesystem::u8path(s.utf8()), ec); }
inline bool isdir(const str& s) { std::error_code ec; return std::filesystem::is_directory(std::filesystem::u8path(s.utf8()), ec); }
inline int64_t getsize(const str& s) {
    std::error_code ec;
    auto n = std::filesystem::file_size(std::filesystem::u8path(s.utf8()), ec);
    if (ec) raise_os_(ec, s);
    return static_cast<int64_t>(n);
}
inline str normcase(const str& s) { return str(s.lower().u32()).replace(str("/"), str("\\")); }
}  // namespace path
inline list<str> listdir(const str& s) {
    list<str> out;
    std::error_code ec;
    auto it = std::filesystem::directory_iterator(std::filesystem::u8path(s.utf8()), ec);
    if (ec) raise_os_(ec, s);
    for (const auto& e : it) out.append(str(e.path().filename().u8string()));
    return out;
}
}  // namespace os

// print(*values): str() of each, separated by spaces
inline void print_one_(std::string& out, const str& s) { out += s.utf8(); }
template <class T> void print_one_(std::string& out, const T& v) { out += to_str(v).utf8(); }
template <class... A> void print(const A&... a) {
    std::string out;
    size_t i = 0;
    ((out += (i++ ? " " : ""), print_one_(out, a)), ...);
    out += "\n";
    std::fwrite(out.data(), 1, out.size(), stdout);
}

inline int64_t int_from_str(const str& s, int64_t base = 10) {
    std::string t = s.strip().utf8();
    std::string clean;
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i] == '_' && i > 0 && i + 1 < t.size() && std::isalnum(static_cast<unsigned char>(t[i - 1])) &&
            std::isalnum(static_cast<unsigned char>(t[i + 1])))
            continue;
        clean.push_back(t[i]);
    }
    bool neg = false;
    size_t i = 0;
    if (i < clean.size() && (clean[i] == '+' || clean[i] == '-')) neg = clean[i++] == '-';
    if (base == 0 || base == 16 || base == 8 || base == 2) {
        if (i + 1 < clean.size() && clean[i] == '0') {
            char c = static_cast<char>(std::tolower(static_cast<unsigned char>(clean[i + 1])));
            int b = c == 'x' ? 16 : c == 'o' ? 8 : c == 'b' ? 2 : 0;
            if (b && (base == 0 || base == b)) { base = b; i += 2; }
        }
        if (base == 0) base = 10;
    }
    if (i >= clean.size()) throw ValueError("invalid literal for int(): " + repr(s).utf8());
    uint64_t v = 0;
    for (; i < clean.size(); ++i) {
        int d = std::isdigit(static_cast<unsigned char>(clean[i])) ? clean[i] - '0'
              : std::isalpha(static_cast<unsigned char>(clean[i])) ? std::tolower(static_cast<unsigned char>(clean[i])) - 'a' + 10 : 99;
        if (d >= base) throw ValueError("invalid literal for int(): " + repr(s).utf8());
        if (v > (static_cast<uint64_t>(INT64_MAX) + 1 - d) / static_cast<uint64_t>(base)) throw OverflowError("int too large for int64");
        v = v * static_cast<uint64_t>(base) + static_cast<uint64_t>(d);
    }
    if (!neg && v > static_cast<uint64_t>(INT64_MAX)) throw OverflowError("int too large for int64");
    return neg ? static_cast<int64_t>(0 - v) : static_cast<int64_t>(v);
}

// float(text) as Python reads it: blanks around it, a sign, digits with _ between them, a point, an exponent;
// inf / infinity / nan in any case. No hexadecimal (strtod's) and no locale's decimal point.
inline double float_from_str(const str& s) {
    std::string t = s.strip().utf8();
    auto bad = [&]() { return ValueError("could not convert string to float: " + repr(s).utf8()); };
    size_t i = 0;
    bool neg = false;
    if (i < t.size() && (t[i] == '+' || t[i] == '-')) neg = t[i++] == '-';
    std::string rest = t.substr(i), low;
    for (char c : rest) low += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (low == "inf" || low == "infinity") return neg ? -HUGE_VAL : HUGE_VAL;
    if (low == "nan") return neg ? -std::nan("") : std::nan("");
    std::string clean;
    bool digits = false, point = false, exp = false;
    auto dig = [](char c) { return c >= '0' && c <= '9'; };
    for (size_t k = 0; k < rest.size(); ++k) {
        char c = rest[k];
        if (dig(c)) { digits = true; clean += c; continue; }
        if (c == '_' && k > 0 && k + 1 < rest.size() && dig(rest[k - 1]) && dig(rest[k + 1])) continue;
        if (c == '.' && !point && !exp) { point = true; clean += c; continue; }
        if ((c == 'e' || c == 'E') && digits && !exp) {
            exp = true;
            clean += 'e';
            if (k + 1 < rest.size() && (rest[k + 1] == '+' || rest[k + 1] == '-')) clean += rest[++k];
            if (k + 1 >= rest.size() || !dig(rest[k + 1])) throw bad();
            continue;
        }
        throw bad();
    }
    if (!digits) throw bad();
    double v = 0;
    auto r = std::from_chars(clean.data(), clean.data() + clean.size(), v);
    if (r.ptr != clean.data() + clean.size()) {
        if (r.ec != std::errc::result_out_of_range) throw bad();
        v = std::strtod(clean.c_str(), nullptr);   // (past the range: inf or 0, as Python's)
    }
    return neg ? -v : v;
}

}  // namespace py

#include "py_os.hpp"   // files in every mode, os / os.path / shutil / time, hashlib, re, generators, variants
