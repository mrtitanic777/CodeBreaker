// dotnet/decompiler/util/number_format.cpp -- .NET's "R" format: the shortest round-trip digits (std::to_chars finds the same digits .NET
// Core 3.0 and later does), laid out by .NET's general-format rule (Number.Formatting's FormatGeneral: exponent notation when the decimal
// point would fall past the larger of the digit count and the type's precision, or more than three places before the first digit).
#include "dotnet/decompiler/util/number_format.hpp"

#include <charconv>
#include <cmath>
#include <cstdlib>
#include <format>

namespace cb::dotnet::decompiler::util {
namespace {

template <class T>
std::string format_general(T value, int precision) {
    if (std::isnan(value)) return "NaN";
    if (std::isinf(value)) return value < 0 ? "-Infinity" : "Infinity";
    char buf[64];
    const auto r = std::to_chars(buf, buf + sizeof buf, value, std::chars_format::scientific);
    std::string_view sci(buf, static_cast<std::size_t>(r.ptr - buf));   // "-1.2345e+04"
    std::string out;
    if (sci.front() == '-') {
        out += '-';
        sci.remove_prefix(1);
    }
    const std::size_t e = sci.find('e');
    std::string digits;
    for (const char c : sci.substr(0, e))
        if (c != '.') digits += c;
    const int exponent = std::atoi(std::string(sci.substr(e + 1)).c_str());
    if (digits == "0") return out + "0";
    while (digits.size() > 1 && digits.back() == '0') digits.pop_back();
    const int count = static_cast<int>(digits.size());
    const int scale = exponent + 1;   // where the decimal point falls, counted from the first digit
    const int max_digits = count > precision ? count : precision;
    if (scale > max_digits || scale < -3) {
        out += digits[0];
        if (count > 1) {
            out += '.';
            out += digits.substr(1);
        }
        const int x = scale - 1;
        out += std::format("E{}{:02}", x < 0 ? '-' : '+', x < 0 ? -x : x);
        return out;
    }
    if (scale <= 0) {
        out += "0.";
        out.append(static_cast<std::size_t>(-scale), '0');
        out += digits;
    } else if (scale >= count) {
        out += digits;
        out.append(static_cast<std::size_t>(scale - count), '0');
    } else {
        out += digits.substr(0, static_cast<std::size_t>(scale));
        out += '.';
        out += digits.substr(static_cast<std::size_t>(scale));
    }
    return out;
}

}  // namespace

std::string format_r(double value) { return format_general(value, 17); }
std::string format_r(float value) { return format_general(value, 9); }

}  // namespace cb::dotnet::decompiler::util
