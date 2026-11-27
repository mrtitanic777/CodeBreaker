// dotnet/decompiler/util/decimal.hpp -- System.Decimal as the decompiler meets it: a 96-bit integer, a sign and a scale (the number of
// digits after the point), built from a DecimalConstantAttribute's arguments, written as .NET writes it.
//
//     Decimal d(150, 0, 0, false, 2);
//     d.to_string() == "1.50"                 // decimal.ToString(CultureInfo.InvariantCulture): the scale's zeros stay
//     Decimal::add(d, d).to_string() == "3.00"
//     Decimal::divide(Decimal(1, 0, 0, false, 0), Decimal(3, 0, 0, false, 0)).to_string() == "0.3333333333333333333333333333"
//
// Written for the ILSpy port from .NET's description of System.Decimal (the bits of new decimal(lo, mid, hi, isNegative, scale)). The
// arithmetic (add, subtract, multiply, divide, remainder, compare, the conversions to and from the integer types, float and double)
// is .NET 9's own, ported from System.Private.CoreLib's Decimal.DecCalc.cs and Decimal.cs (MIT, .NET Foundation; see NOTICE in
// dotnet/decompiler): the same rounding (to even), scaling and overflow as .NET produces, not an approximation. An overflow throws
// util::OverflowError, a division by zero util::DivideByZeroError (util/arithmetic_error.hpp).
#pragma once

#include <cstdint>
#include <string>

namespace cb::dotnet::decompiler::util {

struct Decimal {
    std::uint32_t lo = 0, mid = 0, hi = 0;
    bool negative = false;
    std::uint8_t scale = 0;   // 0 .. 28

    Decimal() = default;
    // new decimal(int lo, int mid, int hi, bool isNegative, byte scale): UsageError for a scale above 28, as .NET's
    // ArgumentOutOfRangeException.
    Decimal(std::uint32_t lo_, std::uint32_t mid_, std::uint32_t hi_, bool negative_, std::uint8_t scale_);

    bool is_zero() const { return lo == 0 && mid == 0 && hi == 0; }
    // The invariant culture's ToString(): digits, a '.' before the last `scale` of them, '-' for a negative non-zero value.
    std::string to_string() const;
    friend bool operator==(const Decimal&, const Decimal&) = default;

    // ---- arithmetic: decimal's operators + - * / % and unary -, Decimal.Compare ----------------------------------------------
    static Decimal add(const Decimal& a, const Decimal& b);
    static Decimal subtract(const Decimal& a, const Decimal& b);
    static Decimal multiply(const Decimal& a, const Decimal& b);
    static Decimal divide(const Decimal& a, const Decimal& b);
    static Decimal remainder(const Decimal& a, const Decimal& b);
    // decimal.Negate: the sign flips, also of zero (-0m), as .NET's does (equal to 0m, written "0").
    static Decimal negate(const Decimal& d);
    // Decimal.Compare: -1, 0 or 1; by value (1.0m and 1.00m are equal).
    static int compare(const Decimal& a, const Decimal& b);
    // Decimal.Truncate: the integer part.
    static Decimal truncate(const Decimal& d);

    // ---- conversions from the primitive types (new decimal(x) / (decimal)x) ----------------------------------------------------
    static Decimal from_int64(std::int64_t value);
    static Decimal from_uint64(std::uint64_t value);
    // (decimal)float and (decimal)double: rounded to 7 and 15 significant digits as .NET does (VarDecFromR4, VarDecFromR8);
    // OverflowError for NaN, infinities and values of 2^96 and more.
    static Decimal from_float(float value);
    static Decimal from_double(double value);

    // ---- conversions to the primitive types (the explicit operators): truncating, OverflowError when the value does not fit -----
    std::int8_t to_sbyte() const;
    std::uint8_t to_byte() const;
    std::int16_t to_int16() const;
    std::uint16_t to_uint16() const;
    std::int32_t to_int32() const;
    std::uint32_t to_uint32() const;
    std::int64_t to_int64() const;
    std::uint64_t to_uint64() const;
    // (char)decimal: ToUInt16.
    char16_t to_char() const;
    float to_float() const;      // Decimal.ToSingle (VarR4FromDec: the double, then a float)
    double to_double() const;    // Decimal.ToDouble (VarR8FromDec)
};

}  // namespace cb::dotnet::decompiler::util
