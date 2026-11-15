// csrt.hpp -- the C# library in C++17, for whole programs written by native/cb/translate/backends/csharp_cpp17.
//
// The object model (docs/TRANSLATOR_DESIGN.md, "C# -> C++17"):
//   * a C# class is a C++ struct held through cs::Ref<T> (std::shared_ptr): reference counting in place of the collector (a cycle of
//     references is never freed). Every class derives (virtually) from cs::Object, so an interface is a struct too;
//   * a C# struct is a C++ value; an enum an `enum class`; a delegate a std::function; `T?` of a value type a std::optional<T>;
//   * a string is UTF-16 (cs::String, null kept apart from empty); text is written out as UTF-8;
//   * int arithmetic wraps (compile with -fwrapv), shifts mask their count (cs::shl, cs::shr, cs::ushr), `/` and `%` throw
//     DivideByZeroException (cs::div, cs::rem), a floating value converts to an integer as .NET does (cs::to_int: saturating, NaN is 0),
//     `checked` arithmetic traps with OverflowException (cs::checked_*);
//   * an exception is a C++ exception (cs::Thrown) carrying the cs::Exception; a catch tests its type;
//   * a property is a field with get_X / set_X, an indexer get_Item / set_Item; the library's own properties are methods (Count(), Length()).
// Only what the translated programs use is here; a library member missing is a compile error, which the judge reports.
#ifndef CSRT_HPP
#define CSRT_HPP
#include <algorithm>
#include <charconv>
#include <cmath>
#include <typeinfo>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <deque>
#include <exception>
#include <functional>
#include <initializer_list>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cs {

template <class T> using Ref = std::shared_ptr<T>;

struct Object;
struct Exception;
class String;
template <class T> struct Array;
template <class T> struct List;

// ---------------------------------------------------------------------------------------------------------- text

inline std::u16string widen(std::string_view s) {
    std::u16string out;
    out.reserve(s.size());
    for (const char c : s) out.push_back(static_cast<char16_t>(static_cast<unsigned char>(c)));
    return out;
}

inline std::string utf8(const std::u16string& s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        std::uint32_t c = s[i];
        if (c >= 0xD800 && c < 0xDC00 && i + 1 < s.size() && s[i + 1] >= 0xDC00 && s[i + 1] < 0xE000) {
            c = 0x10000 + ((c - 0xD800) << 10) + (s[i + 1] - 0xDC00);
            ++i;
        } else if (c >= 0xD800 && c < 0xE000) {
            c = 0xFFFD;
        }
        if (c < 0x80) {
            out.push_back(static_cast<char>(c));
        } else if (c < 0x800) {
            out.push_back(static_cast<char>(0xC0 | (c >> 6)));
            out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
        } else if (c < 0x10000) {
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

[[noreturn]] inline void throw_new_index_out_of_range();
[[noreturn]] inline void throw_new_argument_out_of_range(const char* what);
[[noreturn]] inline void throw_new_null_reference();
[[noreturn]] inline void throw_new_invalid_cast();

// A C# string: UTF-16 code units; null is a state of its own (a default-made string is null, as a string field is).
class String {
public:
    String() = default;
    String(const char16_t* s) : s_(s), null_(false) {}                       // NOLINT: literals are strings
    String(std::u16string s) : s_(std::move(s)), null_(false) {}             // NOLINT
    String(const char* ascii) : s_(widen(ascii)), null_(false) {}            // NOLINT: ASCII text
    static String null() { return String(); }

    bool is_null() const { return null_; }
    const std::u16string& chars() const { return s_; }
    std::string utf8_text() const { return utf8(s_); }
    int Length() const { return static_cast<int>(s_.size()); }
    char16_t operator[](int i) const {
        if (i < 0 || static_cast<std::size_t>(i) >= s_.size()) throw_new_index_out_of_range();
        return s_[static_cast<std::size_t>(i)];
    }
    char16_t get_Item(int i) const { return (*this)[i]; }

    String Substring(int start) const { return Substring(start, Length() - start); }
    String Substring(int start, int length) const {
        if (start < 0 || length < 0 || start > Length() - length) throw_new_argument_out_of_range("startIndex");
        return String(s_.substr(static_cast<std::size_t>(start), static_cast<std::size_t>(length)));
    }
    int IndexOf(const String& x) const {
        const std::size_t at = s_.find(x.s_);
        return at == std::u16string::npos ? -1 : static_cast<int>(at);
    }
    int IndexOf(char16_t x) const {
        const std::size_t at = s_.find(x);
        return at == std::u16string::npos ? -1 : static_cast<int>(at);
    }
    int LastIndexOf(char16_t x) const {
        const std::size_t at = s_.rfind(x);
        return at == std::u16string::npos ? -1 : static_cast<int>(at);
    }
    bool Contains(const String& x) const { return s_.find(x.s_) != std::u16string::npos; }
    bool Contains(char16_t x) const { return s_.find(x) != std::u16string::npos; }
    bool StartsWith(const String& x) const { return s_.size() >= x.s_.size() && s_.compare(0, x.s_.size(), x.s_) == 0; }
    bool EndsWith(const String& x) const { return s_.size() >= x.s_.size() && s_.compare(s_.size() - x.s_.size(), x.s_.size(), x.s_) == 0; }
    String Replace(const String& from, const String& to) const {
        if (from.s_.empty()) throw_new_argument_out_of_range("oldValue");
        std::u16string out;
        std::size_t at = 0;
        for (;;) {
            const std::size_t hit = s_.find(from.s_, at);
            if (hit == std::u16string::npos) break;
            out.append(s_, at, hit - at);
            out.append(to.s_);
            at = hit + from.s_.size();
        }
        out.append(s_, at, std::u16string::npos);
        return String(std::move(out));
    }
    String Replace(char16_t from, char16_t to) const {
        std::u16string out = s_;
        std::replace(out.begin(), out.end(), from, to);
        return String(std::move(out));
    }
    static bool is_space(char16_t c) { return c == u' ' || (c >= 9 && c <= 13) || c == 0x85 || c == 0xA0; }
    String Trim() const {
        std::size_t b = 0, e = s_.size();
        while (b < e && is_space(s_[b])) ++b;
        while (e > b && is_space(s_[e - 1])) --e;
        return String(s_.substr(b, e - b));
    }
    String TrimStart() const {
        std::size_t b = 0;
        while (b < s_.size() && is_space(s_[b])) ++b;
        return String(s_.substr(b));
    }
    String TrimEnd() const {
        std::size_t e = s_.size();
        while (e > 0 && is_space(s_[e - 1])) --e;
        return String(s_.substr(0, e));
    }
    static char16_t upper(char16_t c) { return (c >= u'a' && c <= u'z') ? static_cast<char16_t>(c - 32) : c; }
    static char16_t lower(char16_t c) { return (c >= u'A' && c <= u'Z') ? static_cast<char16_t>(c + 32) : c; }
    String ToUpper() const {
        std::u16string out = s_;
        for (char16_t& c : out) c = upper(c);
        return String(std::move(out));
    }
    String ToLower() const {
        std::u16string out = s_;
        for (char16_t& c : out) c = lower(c);
        return String(std::move(out));
    }
    String ToUpperInvariant() const { return ToUpper(); }
    String ToLowerInvariant() const { return ToLower(); }
    String PadLeft(int width, char16_t pad = u' ') const {
        std::u16string out = s_;
        if (width > Length()) out.insert(0, static_cast<std::size_t>(width - Length()), pad);
        return String(std::move(out));
    }
    String PadRight(int width, char16_t pad = u' ') const {
        std::u16string out = s_;
        if (width > Length()) out.append(static_cast<std::size_t>(width - Length()), pad);
        return String(std::move(out));
    }
    template <class O = int> Ref<Array<String>> Split(char16_t sep, const O& = O()) const { return split_(sep); }
    String ToString() const { return *this; }
    int CompareTo(const String& o) const { return s_ < o.s_ ? -1 : (s_ == o.s_ ? 0 : 1); }
    bool Equals(const String& o) const { return s_ == o.s_ && null_ == o.null_; }
    int GetHashCode() const { return static_cast<int>(std::hash<std::u16string>()(s_)); }

    String& operator+=(const String& o) {
        s_ += o.s_;
        null_ = false;
        return *this;
    }
    friend bool operator==(const String& a, const String& b) { return a.s_ == b.s_ && a.null_ == b.null_; }
    friend bool operator!=(const String& a, const String& b) { return !(a == b); }
    friend bool operator<(const String& a, const String& b) { return a.s_ < b.s_; }
    friend String operator+(const String& a, const String& b) { return String(a.s_ + b.s_); }

private:
    Ref<Array<String>> split_(char16_t sep) const;
    std::u16string s_;
    bool null_ = true;
};

// ---------------------------------------------------------------------------------------------------------- the object model

struct Type;

struct Object : std::enable_shared_from_this<Object> {
    Object() = default;
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    virtual ~Object() = default;
    virtual String ToString() { return String(cs_type_name()); }
    virtual bool Equals(const Ref<Object>& other) { return other.get() == this; }
    virtual int GetHashCode() { return static_cast<int>(reinterpret_cast<std::uintptr_t>(this) >> 4); }
    virtual std::u16string cs_type_name() const { return u"System.Object"; }
    Ref<Type> GetType();
};

struct Type : Object {
    explicit Type(std::u16string full) : full_(std::move(full)) {}
    String Name() const {
        const std::size_t dot = full_.rfind(u'.');
        return String(dot == std::u16string::npos ? full_ : full_.substr(dot + 1));
    }
    String FullName() const { return String(full_); }
    String ToString() override { return FullName(); }
    std::u16string cs_type_name() const override { return u"System.RuntimeType"; }

private:
    std::u16string full_;
};
inline Ref<Type> Object::GetType() { return std::make_shared<Type>(cs_type_name()); }

// `this` as a value.
template <class T> Ref<T> self(T* p) {
    if constexpr (std::is_base_of_v<Object, T>) return std::dynamic_pointer_cast<T>(p->shared_from_this());
    else return nullptr;
}

template <class T> bool is_null(const Ref<T>& p) { return p == nullptr; }
template <class T> bool is_null(const std::optional<T>& p) { return !p.has_value(); }
inline bool is_null(const String& s) { return s.is_null(); }
template <class... A> bool is_null(const std::function<A...>& f) { return !f; }

// A boxed value type, as an object.
template <class T> String str(const T& v);
template <class T> struct Boxed : Object {
    T value;
    explicit Boxed(T v) : value(std::move(v)) {}
    String ToString() override { return str(value); }
    bool Equals(const Ref<Object>& o) override {
        const auto b = std::dynamic_pointer_cast<Boxed<T>>(o);
        return b && b->value == value;
    }
    std::u16string cs_type_name() const override { return u"System.Boxed"; }
};
template <class T> Ref<Object> box(const T& v) { return std::make_shared<Boxed<T>>(v); }
template <class T> Ref<Object> box(const Ref<T>& v) { return v; }

// ---------------------------------------------------------------------------------------------------------- exceptions

struct Exception : virtual Object {
    Exception() = default;
    explicit Exception(String message) : message_(std::move(message)), has_message_(true) {}
    String Message() { return has_message_ ? message_ : default_message(); }
    String ToString() override { return String(cs_type_name() + u": ") + Message(); }
    std::u16string cs_type_name() const override { return u"System.Exception"; }

protected:
    virtual String default_message() { return String(u"Exception of type '" + cs_type_name() + u"' was thrown."); }

private:
    String message_;
    bool has_message_ = false;
};

#define CS_EXCEPTION(NAME, BASE, FULL, DEFAULT)                                       \
    struct NAME : BASE {                                                              \
        NAME() = default;                                                             \
        explicit NAME(String message) : BASE(std::move(message)) {}                   \
        std::u16string cs_type_name() const override { return FULL; }                 \
                                                                                      \
    protected:                                                                        \
        String default_message() override { return String(DEFAULT); }                 \
    }

CS_EXCEPTION(SystemException, Exception, u"System.SystemException", u"System error.");
CS_EXCEPTION(ArgumentException, SystemException, u"System.ArgumentException", u"Value does not fall within the expected range.");
CS_EXCEPTION(ArgumentNullException, ArgumentException, u"System.ArgumentNullException", u"Value cannot be null.");
CS_EXCEPTION(ArgumentOutOfRangeException, ArgumentException, u"System.ArgumentOutOfRangeException", u"Specified argument was out of the range of valid values.");
CS_EXCEPTION(InvalidOperationException, SystemException, u"System.InvalidOperationException", u"Operation is not valid due to the current state of the object.");
CS_EXCEPTION(ArithmeticException, SystemException, u"System.ArithmeticException", u"Overflow or underflow in the arithmetic operation.");
CS_EXCEPTION(DivideByZeroException, ArithmeticException, u"System.DivideByZeroException", u"Attempted to divide by zero.");
CS_EXCEPTION(OverflowException, ArithmeticException, u"System.OverflowException", u"Arithmetic operation resulted in an overflow.");
CS_EXCEPTION(IndexOutOfRangeException, SystemException, u"System.IndexOutOfRangeException", u"Index was outside the bounds of the array.");
CS_EXCEPTION(NullReferenceException, SystemException, u"System.NullReferenceException", u"Object reference not set to an instance of an object.");
CS_EXCEPTION(InvalidCastException, SystemException, u"System.InvalidCastException", u"Specified cast is not valid.");
CS_EXCEPTION(FormatException, SystemException, u"System.FormatException", u"One of the identified items was in an invalid format.");
CS_EXCEPTION(KeyNotFoundException, SystemException, u"System.Collections.Generic.KeyNotFoundException", u"The given key was not present in the dictionary.");
CS_EXCEPTION(NotSupportedException, SystemException, u"System.NotSupportedException", u"Specified method is not supported.");
CS_EXCEPTION(NotImplementedException, SystemException, u"System.NotImplementedException", u"The method or operation is not implemented.");

// A thrown C# exception.
struct Thrown {
    Ref<Exception> ex;
};

template <class E, class... A> [[noreturn]] void raise(A&&... a) { throw Thrown{std::make_shared<E>(std::forward<A>(a)...)}; }
[[noreturn]] inline void throw_new_index_out_of_range() { raise<IndexOutOfRangeException>(); }
[[noreturn]] inline void throw_new_argument_out_of_range(const char* what) { raise<ArgumentOutOfRangeException>(String(what)); }
[[noreturn]] inline void throw_new_null_reference() { raise<NullReferenceException>(); }
[[noreturn]] inline void throw_new_invalid_cast() { raise<InvalidCastException>(); }

// `catch (T e)`: the exception as a T, or null.
template <class T> Ref<T> catch_as(const Thrown& t) { return std::dynamic_pointer_cast<T>(t.ex); }

// `finally`: runs when the scope is left, by a return, a jump or an exception.
template <class F> struct Finally {
    F f;
    explicit Finally(F fn) : f(std::move(fn)) {}
    Finally(const Finally&) = delete;
    ~Finally() noexcept(false) { f(); }
};
template <class F> Finally<F> finally(F f) { return Finally<F>(std::move(f)); }

// ---------------------------------------------------------------------------------------------------------- numbers

template <class T> constexpr int bits_of = static_cast<int>(sizeof(T) * 8);

template <class R, class A, class N> constexpr R shl(A a, N n) {
    using U = std::make_unsigned_t<R>;
    return static_cast<R>(static_cast<U>(static_cast<U>(a) << (static_cast<int>(n) & (bits_of<R> - 1))));
}
template <class R, class A, class N> constexpr R shr(A a, N n) {   // >> : arithmetic for a signed type, logical for an unsigned one
    return static_cast<R>(static_cast<R>(a) >> (static_cast<int>(n) & (bits_of<R> - 1)));
}
template <class R, class A, class N> constexpr R ushr(A a, N n) {  // >>>
    using U = std::make_unsigned_t<R>;
    return static_cast<R>(static_cast<U>(static_cast<U>(a) >> (static_cast<int>(n) & (bits_of<R> - 1))));
}

template <class T> T div(T a, T b) {
    if constexpr (std::is_floating_point_v<T>) {
        return a / b;
    } else {
        if (b == 0) raise<DivideByZeroException>();
        if constexpr (std::is_signed_v<T>) {
            if (b == static_cast<T>(-1)) return static_cast<T>(0 - static_cast<std::make_unsigned_t<T>>(a));
        }
        return static_cast<T>(a / b);
    }
}
template <class T> T rem(T a, T b) {
    if constexpr (std::is_floating_point_v<T>) {
        return std::fmod(a, b);
    } else {
        if (b == 0) raise<DivideByZeroException>();
        if constexpr (std::is_signed_v<T>) {
            if (b == static_cast<T>(-1)) return 0;
        }
        return static_cast<T>(a % b);
    }
}

// A floating value as an integer, as .NET converts: truncated, saturating, NaN is 0.
template <class T, class F> T to_int(F f) {
    if (std::isnan(f)) return 0;
    if (f <= static_cast<F>(std::numeric_limits<T>::min())) return std::numeric_limits<T>::min();
    if (f >= static_cast<F>(std::numeric_limits<T>::max())) return std::numeric_limits<T>::max();
    return static_cast<T>(f);
}

// `checked` arithmetic and conversions.
template <class T> T checked_add(T a, T b) {
    constexpr T lo = std::numeric_limits<T>::min(), hi = std::numeric_limits<T>::max();
    if constexpr (std::is_signed_v<T>) {
        if ((b > 0 && a > hi - b) || (b < 0 && a < lo - b)) raise<OverflowException>();
    } else {
        if (a > hi - b) raise<OverflowException>();
    }
    return static_cast<T>(a + b);
}
template <class T> T checked_sub(T a, T b) {
    constexpr T lo = std::numeric_limits<T>::min(), hi = std::numeric_limits<T>::max();
    if constexpr (std::is_signed_v<T>) {
        if ((b < 0 && a > hi + b) || (b > 0 && a < lo + b)) raise<OverflowException>();
    } else {
        if (a < b) raise<OverflowException>();
    }
    return static_cast<T>(a - b);
}
template <class T> T checked_mul(T a, T b) {
    constexpr T lo = std::numeric_limits<T>::min(), hi = std::numeric_limits<T>::max();
    if (a == 0 || b == 0) return 0;
    if constexpr (std::is_signed_v<T>) {
        if ((a == -1 && b == lo) || (b == -1 && a == lo)) raise<OverflowException>();
        if ((a > 0 && b > 0 && a > hi / b) || (a < 0 && b < 0 && a < hi / b) || (a > 0 && b < 0 && b < lo / a) || (a < 0 && b > 0 && a < lo / b)) raise<OverflowException>();
    } else {
        if (a > hi / b) raise<OverflowException>();
    }
    return static_cast<T>(a * b);
}
template <class T, class F> T checked_cast(F f) {
    if constexpr (std::is_floating_point_v<F>) {
        if (std::isnan(f) || f < static_cast<F>(std::numeric_limits<T>::min()) || f >= static_cast<F>(std::numeric_limits<T>::max()) + static_cast<F>(1)) raise<OverflowException>();
        return static_cast<T>(f);
    } else {
        const T r = static_cast<T>(f);
        if (static_cast<F>(r) != f) raise<OverflowException>();
        if constexpr (std::is_signed_v<F> != std::is_signed_v<T>) {
            if ((r < 0) != (f < 0)) raise<OverflowException>();
        }
        return r;
    }
}

// ---------------------------------------------------------------------------------------------------------- text conversion

namespace detail {

// The shortest text that reads back as the same double, in .NET's layout: positional from 1E-5 up to 1E15, else d.dddE+XX.
template <class F> std::string format_float(F v) {
    if (std::isnan(v)) return "NaN";
    if (std::isinf(v)) return v < 0 ? "-Infinity" : "Infinity";
    if (v == 0) return std::signbit(v) ? "-0" : "0";
    char buf[64];
    const auto r = std::to_chars(buf, buf + sizeof buf, v, std::chars_format::scientific);
    std::string sci(buf, r.ptr);   // -d.ddde+XX
    const bool neg = sci[0] == '-';
    if (neg) sci.erase(0, 1);
    const std::size_t e_at = sci.find('e');
    int exp = std::stoi(sci.substr(e_at + 1));
    std::string digits;
    for (std::size_t i = 0; i < e_at; ++i) {
        if (sci[i] != '.') digits.push_back(sci[i]);
    }
    std::string out;
    if (exp >= 15 || exp < -5) {
        out = digits.substr(0, 1);
        if (digits.size() > 1) out += "." + digits.substr(1);
        char tail[16];
        std::snprintf(tail, sizeof tail, "E%c%02d", exp < 0 ? '-' : '+', exp < 0 ? -exp : exp);
        out += tail;
    } else if (exp >= 0) {
        if (static_cast<int>(digits.size()) <= exp + 1) {
            out = digits + std::string(static_cast<std::size_t>(exp + 1) - digits.size(), '0');
        } else {
            out = digits.substr(0, static_cast<std::size_t>(exp) + 1) + "." + digits.substr(static_cast<std::size_t>(exp) + 1);
        }
    } else {
        out = "0." + std::string(static_cast<std::size_t>(-exp - 1), '0') + digits;
    }
    return neg ? "-" + out : out;
}

// A number with `n` decimals: the exact value rounded to nearest, a tie to even (as .NET's Dragon4 digits are, and printf's).
inline std::string format_fixed(double v, int n) {
    char buf[1100];
    const auto r = std::to_chars(buf, buf + sizeof buf, v, std::chars_format::fixed, n);
    return std::string(buf, r.ptr);
}

}  // namespace detail

template <class T> constexpr bool is_ref_v = false;
template <class T> constexpr bool is_ref_v<Ref<T>> = true;
template <class T> constexpr bool is_optional_v = false;
template <class T> constexpr bool is_optional_v<std::optional<T>> = true;

inline String str(const String& s) { return s.is_null() ? String(u"") : s; }
inline String str(const char16_t* s) { return String(s); }
inline String str(bool b) { return String(b ? u"True" : u"False"); }
inline String str(char16_t c) { return String(std::u16string(1, c)); }
template <class T> String str(const T& v) {
    if constexpr (std::is_same_v<T, String>) {
        return str(v);
    } else if constexpr (std::is_same_v<T, bool>) {
        return str(v);
    } else if constexpr (std::is_same_v<T, char16_t>) {
        return str(v);
    } else if constexpr (std::is_same_v<T, double>) {
        return String(widen(detail::format_float(v)));
    } else if constexpr (std::is_same_v<T, float>) {
        return String(widen(detail::format_float(v)));
    } else if constexpr (std::is_integral_v<T>) {
        return String(widen(std::to_string(static_cast<std::conditional_t<std::is_signed_v<T>, long long, unsigned long long>>(v))));
    } else if constexpr (is_ref_v<T>) {
        return v ? v->ToString() : String(u"");
    } else if constexpr (is_optional_v<T>) {
        return v ? str(*v) : String(u"");
    } else if constexpr (std::is_enum_v<T>) {
        return cs_enum_name(v);   // found by argument-dependent lookup: written for each enum
    } else {
        return const_cast<T&>(v).ToString();
    }
}

// String concatenation, evaluated left to right as C# does (C++17 sequences the operands of <<, not of a call's arguments):
//     (cs::Cat() << a << b << c).done()
struct Cat {
    std::u16string s;
    template <class T> Cat& operator<<(const T& x) {
        s += str(x).chars();
        return *this;
    }
    String done() { return String(std::move(s)); }
};
template <class... A> String cat(const A&... parts) {
    std::u16string out;
    ((out += str(parts).chars()), ...);
    return String(std::move(out));
}

// Two operands evaluated in order (a braced list is sequenced left to right): `cs::LR<A, B>{l, r}.then(f)` is f(l, r) with l first.
template <class A, class B> struct LR {
    A a;
    B b;
    template <class F> auto then(F f) const { return f(a, b); }
};

// `x.ToString(format)` for numbers: D<n> (zero padded), N<n> (thousands, fixed), F<n>, X<n>.
template <class T> String to_string(T v, const String& fmt) {
    const std::string f = fmt.utf8_text();
    int n = f.size() > 1 ? std::stoi(f.substr(1)) : 0;
    const char kind = f.empty() ? 'G' : static_cast<char>(std::toupper(static_cast<unsigned char>(f[0])));
    char buf[512];
    if constexpr (std::is_integral_v<T>) {
        if (kind == 'D') {
            std::string d = std::to_string(static_cast<long long>(v < 0 ? -static_cast<long long>(v) : v));
            if (static_cast<int>(d.size()) < n) d = std::string(static_cast<std::size_t>(n) - d.size(), '0') + d;
            return String(widen((v < 0 ? "-" : "") + d));
        }
        if (kind == 'X') {
            std::snprintf(buf, sizeof buf, f[0] == 'x' ? "%llx" : "%llX", static_cast<unsigned long long>(static_cast<std::make_unsigned_t<T>>(v)));
            std::string d = buf;
            if (static_cast<int>(d.size()) < n) d = std::string(static_cast<std::size_t>(n) - d.size(), '0') + d;
            return String(widen(d));
        }
    }
    if (kind == 'F' || kind == 'N') {
        if (f.size() == 1) n = 2;
        std::string d = detail::format_fixed(static_cast<double>(v), n);
        if (kind == 'N') {
            const std::size_t dot = d.find('.');
            std::string whole = d.substr(0, dot == std::string::npos ? d.size() : dot);
            const std::string rest = dot == std::string::npos ? "" : d.substr(dot);
            for (int i = static_cast<int>(whole.size()) - 3; i > (whole[0] == '-' ? 1 : 0); i -= 3) whole.insert(static_cast<std::size_t>(i), ",");
            d = whole + rest;
        }
        return String(widen(d));
    }
    return str(v);
}

// ---------------------------------------------------------------------------------------------------------- collections

template <class T> using Storage = std::conditional_t<std::is_same_v<T, bool>, std::deque<bool>, std::vector<T>>;

template <class T> struct IEnumerable : virtual Object {
    using value_type = T;
    Storage<T> items;
    IEnumerable() = default;
    explicit IEnumerable(Storage<T> v) : items(std::move(v)) {}
    int Count() const { return static_cast<int>(items.size()); }
};

template <class T> struct Array : IEnumerable<T> {
    Array() = default;
    explicit Array(int n) {
        if (n < 0) raise<OverflowException>();
        this->items.assign(static_cast<std::size_t>(n), T{});
    }
    explicit Array(Storage<T> v) : IEnumerable<T>(std::move(v)) {}
    int Length() const { return this->Count(); }
    int Rank() const { return 1; }
    T& operator[](int i) {
        if (i < 0 || static_cast<std::size_t>(i) >= this->items.size()) throw_new_index_out_of_range();
        return this->items[static_cast<std::size_t>(i)];
    }
    std::u16string cs_type_name() const override { return u"System.Array"; }
};

template <class T> Ref<Array<T>> make_array(int n) { return std::make_shared<Array<T>>(n); }
template <class T> Ref<Array<T>> array_of(std::initializer_list<T> init) {
    Storage<T> v(init.begin(), init.end());
    return std::make_shared<Array<T>>(std::move(v));
}

// A rectangular array of two dimensions (int[,]).
template <class T> struct Array2 : Object {
    int rows = 0, cols = 0;
    Storage<T> items;
    Array2(int r, int c) : rows(r), cols(c) {
        if (r < 0 || c < 0) raise<OverflowException>();
        items.assign(static_cast<std::size_t>(r) * static_cast<std::size_t>(c), T{});
    }
    T& operator()(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols) throw_new_index_out_of_range();
        return items[static_cast<std::size_t>(r) * static_cast<std::size_t>(cols) + static_cast<std::size_t>(c)];
    }
    int Length() const { return rows * cols; }
    int Rank() const { return 2; }
    int GetLength(int d) const { return d == 0 ? rows : cols; }
};
template <class T> Ref<Array2<T>> make_array2(int r, int c) { return std::make_shared<Array2<T>>(r, c); }

template <class T> struct List : IEnumerable<T> {
    List() = default;
    explicit List(int capacity) { (void)capacity; }
    explicit List(const Ref<IEnumerable<T>>& src) { this->items.assign(src->items.begin(), src->items.end()); }
    void Add(const T& x) { this->items.push_back(x); }
    void AddRange(const Ref<IEnumerable<T>>& src) { this->items.insert(this->items.end(), src->items.begin(), src->items.end()); }
    T& operator[](int i) {
        if (i < 0 || static_cast<std::size_t>(i) >= this->items.size()) throw_new_argument_out_of_range("index");
        return this->items[static_cast<std::size_t>(i)];
    }
    T get_Item(int i) { return (*this)[i]; }
    T set_Item(int i, const T& v) { return (*this)[i] = v; }
    bool Contains(const T& x) const { return std::find(this->items.begin(), this->items.end(), x) != this->items.end(); }
    int IndexOf(const T& x) const {
        const auto it = std::find(this->items.begin(), this->items.end(), x);
        return it == this->items.end() ? -1 : static_cast<int>(it - this->items.begin());
    }
    bool Remove(const T& x) {
        const auto it = std::find(this->items.begin(), this->items.end(), x);
        if (it == this->items.end()) return false;
        this->items.erase(it);
        return true;
    }
    void RemoveAt(int i) {
        (void)(*this)[i];
        this->items.erase(this->items.begin() + i);
    }
    void Insert(int i, const T& x) {
        if (i < 0 || static_cast<std::size_t>(i) > this->items.size()) throw_new_argument_out_of_range("index");
        this->items.insert(this->items.begin() + i, x);
    }
    void Clear() { this->items.clear(); }
    void Sort() { std::stable_sort(this->items.begin(), this->items.end()); }
    void Reverse() { std::reverse(this->items.begin(), this->items.end()); }
    std::u16string cs_type_name() const override { return u"System.Collections.Generic.List"; }
};

template <class K, class V> struct KeyValuePair {
    K key{};
    V value{};
    K Key() const { return key; }
    V Value() const { return value; }
};

template <class K, class V> struct Dictionary : IEnumerable<KeyValuePair<K, V>> {
    Dictionary() = default;
    int Count() const { return static_cast<int>(this->items.size()); }
    V get_Item(const K& k) const {
        const auto it = index_.find(k);
        if (it == index_.end()) raise<KeyNotFoundException>();
        return this->items[it->second].value;
    }
    V set_Item(const K& k, const V& v) {
        const auto it = index_.find(k);
        if (it == index_.end()) {
            index_.emplace(k, this->items.size());
            this->items.push_back({k, v});
        } else {
            this->items[it->second].value = v;
        }
        return v;
    }
    void Add(const K& k, const V& v) {
        if (index_.count(k)) raise<ArgumentException>(String(u"An item with the same key has already been added."));
        set_Item(k, v);
    }
    bool ContainsKey(const K& k) const { return index_.count(k) != 0; }
    bool TryGetValue(const K& k, V& out) const {
        const auto it = index_.find(k);
        if (it == index_.end()) {
            out = V{};
            return false;
        }
        out = this->items[it->second].value;
        return true;
    }
    bool Remove(const K& k) {
        const auto it = index_.find(k);
        if (it == index_.end()) return false;
        this->items.erase(this->items.begin() + static_cast<std::ptrdiff_t>(it->second));
        index_.clear();
        for (std::size_t i = 0; i < this->items.size(); ++i) index_.emplace(this->items[i].key, i);
        return true;
    }
    void Clear() {
        this->items.clear();
        index_.clear();
    }
    Ref<IEnumerable<K>> Keys() const {
        auto out = std::make_shared<IEnumerable<K>>();
        for (const auto& p : this->items) out->items.push_back(p.key);
        return out;
    }
    Ref<IEnumerable<V>> Values() const {
        auto out = std::make_shared<IEnumerable<V>>();
        for (const auto& p : this->items) out->items.push_back(p.value);
        return out;
    }
    std::u16string cs_type_name() const override { return u"System.Collections.Generic.Dictionary"; }

private:
    std::unordered_map<K, std::size_t> index_;
};

struct StringBuilder : Object {
    StringBuilder() = default;
    explicit StringBuilder(const String& s) : s_(str(s).chars()) {}
    template <class T> Ref<StringBuilder> Append(const T& x) {
        s_ += str(x).chars();
        return self_();
    }
    template <class T> Ref<StringBuilder> Insert(int at, const T& x) {
        if (at < 0 || static_cast<std::size_t>(at) > s_.size()) throw_new_argument_out_of_range("index");
        s_.insert(static_cast<std::size_t>(at), str(x).chars());
        return self_();
    }
    Ref<StringBuilder> AppendLine() {
        s_ += u"\n";
        return self_();
    }
    int Length() const { return static_cast<int>(s_.size()); }
    void set_Length(int n) { s_.resize(static_cast<std::size_t>(n), u'\0'); }
    char16_t get_Item(int i) const { return s_.at(static_cast<std::size_t>(i)); }
    String ToString() override { return String(s_); }
    std::u16string cs_type_name() const override { return u"System.Text.StringBuilder"; }

private:
    Ref<StringBuilder> self_() { return std::dynamic_pointer_cast<StringBuilder>(shared_from_this()); }
    std::u16string s_;
};

inline Ref<Array<String>> String::split_(char16_t sep) const {
    Storage<String> parts;
    std::size_t at = 0;
    for (;;) {
        const std::size_t hit = s_.find(sep, at);
        if (hit == std::u16string::npos) break;
        parts.push_back(String(s_.substr(at, hit - at)));
        at = hit + 1;
    }
    parts.push_back(String(s_.substr(at)));
    return std::make_shared<Array<String>>(std::move(parts));
}

// foreach: the elements of a collection (an array, a list, any sequence), or the characters of a string.
template <class C> const auto& iter(const Ref<C>& c) {
    if (!c) throw_new_null_reference();
    return c->items;
}
inline std::u16string iter(const String& s) { return s.chars(); }

// ---------------------------------------------------------------------------------------------------------- conversions and tests

template <class T, class S> Ref<T> cast(const Ref<S>& o) {
    if (!o) return nullptr;
    auto r = std::dynamic_pointer_cast<T>(o);
    if (!r) throw_new_invalid_cast();
    return r;
}
template <class T> T unbox(const Ref<Object>& o) {
    if (!o) throw_new_null_reference();
    const auto b = std::dynamic_pointer_cast<Boxed<T>>(o);
    if (!b) throw_new_invalid_cast();
    return b->value;
}

// `x is T`: whether the object is a T (a class), or a boxed value of type T.
template <class T, class S> bool is(const Ref<S>& o) {
    if (!o) return false;
    if constexpr (is_ref_v<T>) return std::dynamic_pointer_cast<typename T::element_type>(o) != nullptr;
    else return std::dynamic_pointer_cast<Boxed<T>>(o) != nullptr;
}
// `x is T v`: the same, binding v.
template <class S, class V> bool is_as(const Ref<S>& o, V& v) {
    if (!o) return false;
    if constexpr (is_ref_v<V>) {
        v = std::dynamic_pointer_cast<typename V::element_type>(o);
        return v != nullptr;
    } else {
        const auto b = std::dynamic_pointer_cast<Boxed<V>>(o);
        if (!b) return false;
        v = b->value;
        return true;
    }
}
// `x as T`
template <class T, class S> Ref<T> as(const Ref<S>& o) { return std::dynamic_pointer_cast<T>(o); }
template <class T> T as_value(const Ref<Object>& o) {   // `o as string`: null when it is no string
    const auto b = std::dynamic_pointer_cast<Boxed<T>>(o);
    return b ? b->value : T{};
}

// `a ?? b`
template <class T, class F> Ref<T> coalesce(const Ref<T>& a, F b) { return a ? a : Ref<T>(b()); }
template <class T, class F> auto coalesce(const std::optional<T>& a, F b) { return a ? *a : static_cast<T>(b()); }
template <class F> String coalesce(const String& a, F b) { return a.is_null() ? String(b()) : a; }

template <class T, class U> int compare_to(const T& a, const U& b) {
    if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, char16_t>) return a < b ? -1 : (a == b ? 0 : 1);
    else if constexpr (is_ref_v<T>) return a->CompareTo(b);
    else return a.CompareTo(b);
}

// ---------------------------------------------------------------------------------------------------------- the library's statics

namespace sys {

struct Console {
    static void WriteLine() { std::fputc('\n', stdout); }
    template <class T> static void WriteLine(const T& x) {
        const std::string text = utf8(str(x).chars());
        std::fwrite(text.data(), 1, text.size(), stdout);
        std::fputc('\n', stdout);
    }
    template <class T> static void Write(const T& x) {
        const std::string text = utf8(str(x).chars());
        std::fwrite(text.data(), 1, text.size(), stdout);
    }
};

struct Math {
    template <class T> static T Min(T a, T b) { return a < b ? a : b; }
    template <class T> static T Max(T a, T b) { return a > b ? a : b; }
    template <class T> static T Abs(T a) { return a < 0 ? static_cast<T>(-a) : a; }
    static double Sqrt(double x) { return std::sqrt(x); }
    static double Pow(double x, double y) { return std::pow(x, y); }
    static double Floor(double x) { return std::floor(x); }
    static double Ceiling(double x) { return std::ceil(x); }
    static double Round(double x) { return std::nearbyint(x); }
    static double Sin(double x) { return std::sin(x); }
    static double Cos(double x) { return std::cos(x); }
};

struct Char {
    static bool IsLetter(char16_t c) { return (c >= u'a' && c <= u'z') || (c >= u'A' && c <= u'Z') || c > 0x7F; }
    static bool IsDigit(char16_t c) { return c >= u'0' && c <= u'9'; }
    static bool IsLetterOrDigit(char16_t c) { return IsLetter(c) || IsDigit(c); }
    static bool IsUpper(char16_t c) { return c >= u'A' && c <= u'Z'; }
    static bool IsLower(char16_t c) { return c >= u'a' && c <= u'z'; }
    static bool IsWhiteSpace(char16_t c) { return String::is_space(c); }
    static char16_t ToUpper(char16_t c) { return String::upper(c); }
    static char16_t ToLower(char16_t c) { return String::lower(c); }
};

template <class T> struct Limits {
    static constexpr T MaxValue = std::numeric_limits<T>::max();
    static constexpr T MinValue = std::numeric_limits<T>::lowest();
};
using Int32 = Limits<int>;
using UInt32 = Limits<unsigned>;
using Int64 = Limits<long long>;
using UInt64 = Limits<unsigned long long>;
using Int16 = Limits<short>;
using UInt16 = Limits<unsigned short>;
using Byte = Limits<unsigned char>;
using SByte = Limits<signed char>;
using Double = Limits<double>;

// System.Array's static members.
struct ArrayStatics {
    template <class T> static void Sort(const Ref<Array<T>>& a) { std::stable_sort(a->items.begin(), a->items.end()); }
    template <class T> static void Reverse(const Ref<Array<T>>& a) { std::reverse(a->items.begin(), a->items.end()); }
    template <class T> static int IndexOf(const Ref<Array<T>>& a, const T& x) {
        const auto it = std::find(a->items.begin(), a->items.end(), x);
        return it == a->items.end() ? -1 : static_cast<int>(it - a->items.begin());
    }
};

struct StringStatics {
    static bool IsNullOrEmpty(const String& s) { return s.is_null() || s.Length() == 0; }
    static int CompareOrdinal(const String& a, const String& b) {   // the difference of the first differing units, else of the lengths
        const std::u16string& x = a.chars();
        const std::u16string& y = b.chars();
        const std::size_t n = std::min(x.size(), y.size());
        for (std::size_t i = 0; i < n; ++i) {
            if (x[i] != y[i]) return static_cast<int>(x[i]) - static_cast<int>(y[i]);
        }
        return static_cast<int>(x.size()) - static_cast<int>(y.size());
    }
    template <class C> static String Join(const String& sep, const Ref<C>& items) {
        std::u16string out;
        bool first = true;
        for (const auto& x : iter(items)) {
            if (!first) out += sep.chars();
            first = false;
            out += str(x).chars();
        }
        return String(std::move(out));
    }
    template <class... A> static String Format(const String& fmt, const A&... args) {
        auto boxed = std::make_shared<Array<Ref<Object>>>();
        (boxed->items.push_back(box(args)), ...);
        return Format(fmt, boxed);
    }
    static String Format(const String& fmt, const Ref<Array<Ref<Object>>>& args) {
        const std::u16string& f = fmt.chars();
        std::u16string out;
        for (std::size_t i = 0; i < f.size(); ++i) {
            if (f[i] == u'{' && i + 1 < f.size() && f[i + 1] == u'{') {
                out.push_back(u'{');
                ++i;
            } else if (f[i] == u'}' && i + 1 < f.size() && f[i + 1] == u'}') {
                out.push_back(u'}');
                ++i;
            } else if (f[i] == u'{') {
                const std::size_t close = f.find(u'}', i);
                if (close == std::u16string::npos) raise<FormatException>();
                const std::u16string spec = f.substr(i + 1, close - i - 1);
                const std::size_t index = static_cast<std::size_t>(std::stoi(std::string(spec.begin(), spec.end())));
                if (index >= args->items.size()) raise<FormatException>();
                out += str(args->items[index]).chars();
                i = close;
            } else {
                out.push_back(f[i]);
            }
        }
        return String(std::move(out));
    }
};

// LINQ over eager sequences.
template <class S> using Elem = typename S::element_type::value_type;
template <class T> Ref<IEnumerable<T>> seq(Storage<T> v) { return std::make_shared<IEnumerable<T>>(std::move(v)); }

struct Enumerable {
    template <class S, class F> static auto Select(const S& s, F f) {
        using R = std::decay_t<decltype(f(std::declval<Elem<S>>()))>;
        Storage<R> out;
        for (const auto& x : iter(s)) out.push_back(f(x));
        return seq<R>(std::move(out));
    }
    template <class S, class F> static auto Where(const S& s, F f) {
        Storage<Elem<S>> out;
        for (const auto& x : iter(s)) {
            if (f(x)) out.push_back(x);
        }
        return seq<Elem<S>>(std::move(out));
    }
    template <class S, class F> static auto OrderBy(const S& s, F f) {
        Storage<Elem<S>> out(iter(s).begin(), iter(s).end());
        std::stable_sort(out.begin(), out.end(), [&](const auto& a, const auto& b) { return f(a) < f(b); });
        return seq<Elem<S>>(std::move(out));
    }
    template <class S, class F> static auto OrderByDescending(const S& s, F f) {
        Storage<Elem<S>> out(iter(s).begin(), iter(s).end());
        std::stable_sort(out.begin(), out.end(), [&](const auto& a, const auto& b) { return f(b) < f(a); });
        return seq<Elem<S>>(std::move(out));
    }
    template <class S, class F> static auto Sum(const S& s, F f) {
        using R = std::decay_t<decltype(f(std::declval<Elem<S>>()))>;
        R total{};
        for (const auto& x : iter(s)) total = checked_add<R>(total, f(x));
        return total;
    }
    template <class S> static Elem<S> Sum(const S& s) {
        Elem<S> total{};
        for (const auto& x : iter(s)) total = checked_add<Elem<S>>(total, x);
        return total;
    }
    template <class S> static int Count(const S& s) { return static_cast<int>(iter(s).size()); }
    template <class S, class F> static int Count(const S& s, F f) {
        int n = 0;
        for (const auto& x : iter(s)) n += f(x) ? 1 : 0;
        return n;
    }
    template <class S> static bool Any(const S& s) { return !iter(s).empty(); }
    template <class S, class F> static bool Any(const S& s, F f) {
        for (const auto& x : iter(s)) {
            if (f(x)) return true;
        }
        return false;
    }
    template <class S, class F> static bool All(const S& s, F f) {
        for (const auto& x : iter(s)) {
            if (!f(x)) return false;
        }
        return true;
    }
    template <class S> static Elem<S> First(const S& s) {
        if (iter(s).empty()) raise<InvalidOperationException>(String(u"Sequence contains no elements"));
        return iter(s).front();
    }
    template <class S, class F> static Elem<S> First(const S& s, F f) {
        for (const auto& x : iter(s)) {
            if (f(x)) return x;
        }
        raise<InvalidOperationException>(String(u"Sequence contains no matching element"));
    }
    template <class S> static auto ToList(const S& s) {
        auto out = std::make_shared<List<Elem<S>>>();
        out->items.assign(iter(s).begin(), iter(s).end());
        return out;
    }
    template <class S> static auto ToArray(const S& s) {
        return std::make_shared<Array<Elem<S>>>(Storage<Elem<S>>(iter(s).begin(), iter(s).end()));
    }
    static Ref<IEnumerable<int>> Range(int start, int count) {
        Storage<int> out;
        for (int i = 0; i < count; ++i) out.push_back(start + i);
        return seq<int>(std::move(out));
    }
    template <class S> static Elem<S> Min(const S& s) { return *std::min_element(iter(s).begin(), iter(s).end()); }
    template <class S> static Elem<S> Max(const S& s) { return *std::max_element(iter(s).begin(), iter(s).end()); }
    template <class S> static bool Contains(const S& s, const Elem<S>& x) { return std::find(iter(s).begin(), iter(s).end(), x) != iter(s).end(); }
};

}  // namespace sys

}  // namespace cs

namespace std {
template <> struct hash<cs::String> {
    size_t operator()(const cs::String& s) const { return hash<u16string>()(s.chars()); }
};
}  // namespace std

#endif  // CSRT_HPP
