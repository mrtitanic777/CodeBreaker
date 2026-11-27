#include "dotnet/decompiler/util/csharp_primitive_cast.hpp"

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace cb::dotnet::decompiler::util {

using typesystem::ConstantValue;
using typesystem::KnownTypeCode;

namespace {

// ---- float to integer --------------------------------------------------------------------------------------------------------------

// A double (or a float, widened) cast to an integer type, as .NET 9 does it on x64 (conv.* and conv.ovf.*).
//
// Checked: the value truncated toward zero must be in the type's range, else OverflowException (NaN too).
// Unchecked: .NET 9 saturates (NaN gives 0) where earlier versions gave the hardware's "integer indefinite" value. The 64-bit types and
// uint saturate at their own range; every smaller type (sbyte, short, byte, ushort, char) and int convert to int with saturation and
// then keep the low bits: (byte)300.7 is 44, (byte)1e10 is 255 (int.MaxValue's low byte), (byte)-1.0 is 255, (ushort)-1e20 is 0.
// All of this was checked against a C# program run on .NET 9.0 (testdata of util/decimal_test.cpp).
template <class To>
constexpr bool saturates_at_own_range_v = std::is_same_v<To, std::int64_t> || std::is_same_v<To, std::uint64_t> || std::is_same_v<To, std::uint32_t>;

// d truncated and clamped to [lo, 2^digits): NaN is 0.
template <class To>
To saturating(double d) {
    if (std::isnan(d)) return 0;
    const double t = std::trunc(d);
    constexpr double lo = static_cast<double>(std::numeric_limits<To>::min());
    const double hi = std::ldexp(1.0, std::numeric_limits<To>::digits);  // max + 1, exact
    if (t < lo) return std::numeric_limits<To>::min();
    if (t >= hi) return std::numeric_limits<To>::max();
    return static_cast<To>(t);
}

template <class To>
To float_to_integral(double d, bool check) {
    if (check) {
        const double t = std::trunc(d);
        constexpr double lo = static_cast<double>(std::numeric_limits<To>::min());
        const double hi = std::ldexp(1.0, std::numeric_limits<To>::digits);  // max + 1, exact
        if (std::isnan(d) || !(t >= lo && t < hi)) throw OverflowError();
        return static_cast<To>(t);
    }
    if constexpr (saturates_at_own_range_v<To>) {
        return saturating<To>(d);
    } else {
        return static_cast<To>(saturating<std::int32_t>(d));
    }
}

// ---- decimal ------------------------------------------------------------------------------------------------------------------------

template <class S>
Decimal decimal_from_integral(S v) {
    if constexpr (std::is_signed_v<S>) {
        return Decimal::from_int64(static_cast<std::int64_t>(v));
    } else {
        return Decimal::from_uint64(static_cast<std::uint64_t>(v));
    }
}

// (To)decimal: truncates; out of range is an OverflowException in a checked and in an unchecked context.
template <class To>
To decimal_to_integral(const Decimal& d) {
    if constexpr (std::is_same_v<To, std::int8_t>) return d.to_sbyte();
    else if constexpr (std::is_same_v<To, std::uint8_t>) return d.to_byte();
    else if constexpr (std::is_same_v<To, std::int16_t>) return d.to_int16();
    else if constexpr (std::is_same_v<To, std::uint16_t>) return d.to_uint16();
    else if constexpr (std::is_same_v<To, std::int32_t>) return d.to_int32();
    else if constexpr (std::is_same_v<To, std::uint32_t>) return d.to_uint32();
    else if constexpr (std::is_same_v<To, std::int64_t>) return d.to_int64();
    else return d.to_uint64();
}

// ---- the cast of one value ---------------------------------------------------------------------------------------------------------

template <class T>
inline constexpr bool is_integer_v = std::is_integral_v<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char16_t>;
template <class T>
inline constexpr bool is_float_v = std::is_same_v<T, float> || std::is_same_v<T, double>;

template <class To, class S>
To convert_value(const S& v, bool check) {
    if constexpr (std::is_same_v<S, bool>) {
        return convert_value<To, std::int32_t>(v ? 1 : 0, check);
    } else if constexpr (std::is_same_v<S, char16_t>) {
        return convert_value<To, std::uint16_t>(static_cast<std::uint16_t>(v), check);
    } else if constexpr (std::is_same_v<To, char16_t>) {
        return static_cast<char16_t>(convert_value<std::uint16_t, S>(v, check));
    } else if constexpr (std::is_same_v<To, Decimal>) {
        if constexpr (is_integer_v<S>) return decimal_from_integral(v);
        else if constexpr (std::is_same_v<S, float>) return Decimal::from_float(v);
        else if constexpr (std::is_same_v<S, double>) return Decimal::from_double(v);
        else return v;
    } else if constexpr (std::is_same_v<S, Decimal>) {
        if constexpr (is_integer_v<To>) return decimal_to_integral<To>(v);
        else if constexpr (std::is_same_v<To, float>) return v.to_float();
        else return v.to_double();
    } else if constexpr (is_integer_v<To>) {
        if constexpr (is_integer_v<S>) {
            if (check && !std::in_range<To>(v)) throw OverflowError();
            return static_cast<To>(v);
        } else {
            return float_to_integral<To>(static_cast<double>(v), check);
        }
    } else {
        return static_cast<To>(v);   // to float or double
    }
}

template <class To>
ConstantValue convert_to(const ConstantValue& input, bool check) {
    return std::visit(
        [&](const auto& v) -> ConstantValue {
            using S = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<S, std::monostate> || std::is_same_v<S, std::u16string>) {
                throw PrimitiveCastInvalidError("Cast from a value that is not a primitive is not supported.");
            } else {
                return ConstantValue(convert_value<To, S>(v, check));
            }
        },
        input);
}

KnownTypeCode known_type_code_of(const ConstantValue& value) {
    switch (value.index()) {
        case 1: return KnownTypeCode::Boolean;
        case 2: return KnownTypeCode::Char;
        case 3: return KnownTypeCode::SByte;
        case 4: return KnownTypeCode::Byte;
        case 5: return KnownTypeCode::Int16;
        case 6: return KnownTypeCode::UInt16;
        case 7: return KnownTypeCode::Int32;
        case 8: return KnownTypeCode::UInt32;
        case 9: return KnownTypeCode::Int64;
        case 10: return KnownTypeCode::UInt64;
        case 11: return KnownTypeCode::Single;
        case 12: return KnownTypeCode::Double;
        case 13: return KnownTypeCode::String;
        case 14: return KnownTypeCode::Decimal;
        default: return KnownTypeCode::None;
    }
}

}  // namespace

ConstantValue csharp_primitive_cast(KnownTypeCode target_type, const ConstantValue& input, bool check_for_overflow) {
    if (std::holds_alternative<std::monostate>(input)) return {};
    if (known_type_code_of(input) == target_type) return input;
    switch (target_type) {
        case KnownTypeCode::Char: return convert_to<char16_t>(input, check_for_overflow);
        case KnownTypeCode::SByte: return convert_to<std::int8_t>(input, check_for_overflow);
        case KnownTypeCode::Byte: return convert_to<std::uint8_t>(input, check_for_overflow);
        case KnownTypeCode::Int16: return convert_to<std::int16_t>(input, check_for_overflow);
        case KnownTypeCode::UInt16: return convert_to<std::uint16_t>(input, check_for_overflow);
        case KnownTypeCode::Int32: return convert_to<std::int32_t>(input, check_for_overflow);
        case KnownTypeCode::UInt32: return convert_to<std::uint32_t>(input, check_for_overflow);
        case KnownTypeCode::Int64: return convert_to<std::int64_t>(input, check_for_overflow);
        case KnownTypeCode::UInt64: return convert_to<std::uint64_t>(input, check_for_overflow);
        case KnownTypeCode::Single: return convert_to<float>(input, check_for_overflow);
        case KnownTypeCode::Double: return convert_to<double>(input, check_for_overflow);
        case KnownTypeCode::Decimal: return convert_to<Decimal>(input, check_for_overflow);
        default: throw PrimitiveCastInvalidError("Cast to this type is not supported.");
    }
}

}  // namespace cb::dotnet::decompiler::util
