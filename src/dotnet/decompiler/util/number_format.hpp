// dotnet/decompiler/util/number_format.hpp -- numbers as .NET writes them with the invariant culture: float and double by ToString("R")
// (the shortest text that reads back to the same value; exponent notation past 17 significant places for a double, 9 for a float, or
// below 1e-4), as ILSpy's output calls them. Proved against .NET itself (testdata/golden/numbers.txt).
//
//     format_r(0.1) == "0.1";   format_r(1e21) == "1E+21";   format_r(1e-5f) == "1E-05";
#pragma once

#include <string>

namespace cb::dotnet::decompiler::util {

std::string format_r(double value);
std::string format_r(float value);

}  // namespace cb::dotnet::decompiler::util
