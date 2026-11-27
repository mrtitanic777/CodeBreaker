// dotnet/decompiler/util/csharp_primitive_cast.hpp -- a C# cast between primitive types on a constant: bool, char, the integer types, float,
// double and decimal, with C#'s checked and unchecked meaning (wrapping or overflow for integers, truncation of floats, bool as 0 and 1).
//
//     ConstantValue v = csharp_primitive_cast(KnownTypeCode::Byte, std::int32_t{300}, /*check_for_overflow*/ false);    // byte 44
//     csharp_primitive_cast(KnownTypeCode::Byte, std::int32_t{300}, true);                                                // PrimitiveCastOverflowError
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: CSharpPrimitiveCast, from CSharpPrimitiveCast.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Upstream writes down every pair of types twice (once in a checked block, once in an
// unchecked one) and lets the C# compiler give the meaning; here one template per kind of pair does it. The target is told by its known
// type code (System.TypeCode's numbering), as upstream's is.
//
// Exact against .NET 9 on x64: decimal goes through util::Decimal (.NET's own arithmetic and conversions, including (decimal)double and
// (decimal)float with their 15 and 7 significant digits), and a float cast to an integer does what .NET 9's JIT does (saturates when
// unchecked, NaN is 0; overflow when checked), verified against a C# program's output in util/decimal_test.cpp.
#pragma once

#include <stdexcept>

#include "dotnet/decompiler/util/arithmetic_error.hpp"
#include "dotnet/decompiler/util/decimal.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::util {

// System.OverflowException: the checked cast overflowed (a decimal cast overflows in both modes). It is util::OverflowError
// (util/arithmetic_error.hpp), the exception Decimal throws too.
using PrimitiveCastOverflowError = OverflowError;

// System.InvalidCastException: the cast is not one C# has (a bool to nothing but a bool, a string, an unknown target type).
class PrimitiveCastInvalidError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// The input cast to the target type; null in, null out.
typesystem::ConstantValue csharp_primitive_cast(typesystem::KnownTypeCode target_type, const typesystem::ConstantValue& input, bool check_for_overflow);

}  // namespace cb::dotnet::decompiler::util
