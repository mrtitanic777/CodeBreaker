// dotnet/decompiler/util/decimal.hpp -- System.Decimal as the decompiler meets it: a 96-bit integer, a sign and a scale (the number of
// digits after the point), built from a DecimalConstantAttribute's arguments, written as .NET writes it.
//
//     Decimal d(150, 0, 0, false, 2);
//     d.to_string() == "1.50"                 // decimal.ToString(CultureInfo.InvariantCulture): the scale's zeros stay
//
// Written for the ILSpy port from .NET's description of System.Decimal (the bits of new decimal(lo, mid, hi, isNegative, scale)).
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
};

}  // namespace cb::dotnet::decompiler::util
