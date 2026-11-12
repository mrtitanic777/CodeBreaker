// cb/translate/backends/cpp03_text.cpp -- the C++ text rules of the C++03 back end: literals, boxing, null (see cpp03.hpp).

#include <algorithm>
#include <charconv>
#include <cmath>

#include "cb/core/text.hpp"
#include "cb/java/emit/std_transforms.hpp"
#include "cb/translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

namespace detail {

// The code points of UTF-8 text; a byte that is no valid UTF-8 reads as itself (Latin-1), as the front ends' text came.
std::vector<std::uint32_t> code_points(std::string_view s) {
    std::vector<std::uint32_t> out;
    for (std::size_t i = 0; i < s.size();) {
        const auto b0 = static_cast<unsigned char>(s[i]);
        std::size_t len = b0 < 0x80 ? 1 : (b0 >> 5) == 0x6 ? 2 : (b0 >> 4) == 0xE ? 3 : (b0 >> 3) == 0x1E ? 4 : 0;
        bool ok = len != 0 && i + len <= s.size();
        std::uint32_t cp = len == 1 ? b0 : len == 2 ? (b0 & 0x1Fu) : len == 3 ? (b0 & 0x0Fu) : (b0 & 0x07u);
        for (std::size_t k = 1; ok && k < len; ++k) {
            const auto bk = static_cast<unsigned char>(s[i + k]);
            if ((bk & 0xC0) != 0x80) ok = false;
            else cp = (cp << 6) | (bk & 0x3Fu);
        }
        if (!ok) {
            out.push_back(b0);
            ++i;
        } else {
            out.push_back(cp);
            i += len;
        }
    }
    return out;
}

}  // namespace detail

namespace {

namespace text_impl {

bool is_word(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

// The initial value of a temporary of each primitive C++ type.
const char* primitive_zero(std::string_view ctype) {
    static const std::map<std::string, const char*, std::less<>> zeros = {
        {"bool", "false"}, {"int", "0"},          {"long long", "0"},   {"short", "0"},        {"float", "0"},        {"double", "0"},
        {"unsigned char", "0"}, {"signed char", "0"}, {"char", "0"}, {"unsigned int", "0"}, {"long", "0"}};
    const auto it = zeros.find(ctype);
    return it == zeros.end() ? nullptr : it->second;
}

// What a boxed value is asked for to become each primitive.
const char* unboxer(std::string_view ctype) {
    static const std::map<std::string, const char*, std::less<>> accessors = {
        {"int", "intValue"},      {"short", "shortValue"},   {"signed char", "byteValue"},     {"long long", "longValue"},
        {"float", "floatValue"},  {"double", "doubleValue"}, {"bool", "booleanValue"},         {"unsigned char", "charValue"}};
    const auto it = accessors.find(ctype);
    return it == accessors.end() ? nullptr : it->second;
}

}  // namespace text_impl

}  // namespace

std::string unbox(const std::string& code, std::string_view ctype, std::string_view target) {
    const std::string t = detail::strip_const_ref(target);
    const char* accessor = text_impl::unboxer(t);
    if (ctype.starts_with("JObject") && accessor != nullptr) return "(" + code + ")." + accessor + "()";
    return code;
}

std::string null_of(std::string_view ctype) {
    const std::string t = detail::strip_const_ref(ctype);
    if (t.empty() || t.ends_with('*') || t == "void*") return "NULL";
    for (const std::string_view collection : {"JList", "JMap", "JSet"}) {   // a collection's null is a state, made from 0
        if (t.starts_with(collection) && (t.size() == collection.size() || !text_impl::is_word(t[collection.size()])))
            return t + "(0LL)";
    }
    return t + "()";
}

std::string coerce(const std::string& code, std::string_view target) {
    if (code == "NULL" && !target.empty() && !is_pointer(target)) return null_of(target);
    return code;
}

std::string zero_of(std::string_view ctype) {
    if (const char* zero = text_impl::primitive_zero(ctype)) return zero;
    return null_of(ctype);
}

std::string comment_safe(std::string_view text) { return text::replace_all(text, "*/", "* /"); }

std::string float_literal(const ir::LitValue& value, std::string_view suffix) {
    double d = 0;
    if (const auto* f = std::get_if<double>(&value)) d = *f;
    else if (const auto* i = std::get_if<std::int64_t>(&value)) d = static_cast<double>(*i);
    else if (const auto* u = std::get_if<std::uint64_t>(&value)) d = static_cast<double>(*u);
    else if (const auto* b = std::get_if<bool>(&value)) d = *b ? 1.0 : 0.0;
    else if (const auto* s = std::get_if<std::string>(&value)) {   // the front end writes what is not finite as text
        const std::string_view t = text::trim(*s);
        const auto [end, ec] = std::from_chars(t.data(), t.data() + t.size(), d);
        if (ec != std::errc() || end != t.data() + t.size()) throw Decline("non-finite literal");
    } else {
        throw Decline("float literal " + detail::format_value(value));
    }
    if (!std::isfinite(d)) throw Decline("non-finite literal");

    // the shortest digits that read back, then laid out as plain decimal for 1e-4 <= |x| < 1e16 and as d.d e+XX beyond
    char buf[64];
    const auto res = std::to_chars(buf, buf + sizeof buf, d, std::chars_format::scientific);
    std::string sci(buf, res.ptr);
    const bool negative = sci.starts_with('-');
    if (negative) sci.erase(0, 1);
    const std::size_t e_at = sci.find('e');
    std::string digits = sci.substr(0, e_at);
    const int exponent = std::stoi(sci.substr(e_at + 1));
    digits.erase(std::remove(digits.begin(), digits.end(), '.'), digits.end());
    std::string out;
    if (exponent >= -4 && exponent < 16) {
        if (exponent >= 0) {
            const std::size_t whole = static_cast<std::size_t>(exponent) + 1;
            if (digits.size() <= whole) out = digits + std::string(whole - digits.size(), '0') + ".0";
            else out = digits.substr(0, whole) + "." + digits.substr(whole);
        } else {
            out = "0." + std::string(static_cast<std::size_t>(-exponent - 1), '0') + digits;
        }
    } else {
        std::string mantissa = digits.substr(0, 1) + "." + (digits.size() > 1 ? digits.substr(1) : "0");
        const int mag = exponent < 0 ? -exponent : exponent;
        out = mantissa + "e" + (exponent < 0 ? "-" : "+") + (mag < 10 ? "0" : "") + std::to_string(mag);
    }
    return (negative ? "-" : "") + out + std::string(suffix);
}

std::string c_string_literal(std::string_view utf8, const std::map<std::uint32_t, std::uint32_t>& char_map) {
    std::string out = "\"";
    for (std::uint32_t o : detail::code_points(utf8)) {
        if (const auto it = char_map.find(o); it != char_map.end()) o = it->second;   // a code the program's font draws elsewhere
        if (o == '\\') out += "\\\\";
        else if (o == '"') out += "\\\"";
        else if (o == '\n') out += "\\n";
        else if (o == '\r') out += "\\r";
        else if (o == '\t') out += "\\t";
        else if (o < 32 || (127 <= o && o <= 255)) {   // the runtime's String is a char per byte (Latin-1), as the judge's inputs are
            out.push_back('\\');                       // (the section sign is one byte, 0xA7)
            out.push_back(static_cast<char>('0' + ((o >> 6) & 7)));
            out.push_back(static_cast<char>('0' + ((o >> 3) & 7)));
            out.push_back(static_cast<char>('0' + (o & 7)));
        } else if (o > 255) {
            out.push_back('?');                        // outside the 8-bit runtime, as Latin-1 encodes it
        } else {
            out.push_back(static_cast<char>(o));
        }
    }
    return out + "\"";
}

std::string keep_null(std::string_view text) {
    if (!detail::has(text, "NULL")) return std::string(text);
    return cb::java::emit::std_transforms::map_outside_literals(text, [](std::string_view piece) {
        std::string out;
        for (std::size_t i = 0; i < piece.size();) {
            const bool word = piece.substr(i).starts_with("NULL") && (i == 0 || !text_impl::is_word(piece[i - 1])) &&
                              (i + 4 >= piece.size() || !text_impl::is_word(piece[i + 4]));
            if (word) {
                out += "__null";
                i += 4;
            } else {
                out.push_back(piece[i++]);
            }
        }
        return out;
    });
}

}  // namespace cb::translate::backends::cpp03
