// dotnet/decompiler/util/culture_compare.hpp -- .NET's culture-sensitive string comparison (string.CompareTo, StringComparison.CurrentCulture,
// OrderBy(string) with the default comparer, string.Compare(a, b)) as ILSpy runs it: ilspycmd on .NET 9, Windows, culture en-US, where the
// runtime sorts with ICU's collation. ILSpy sorts attributes, local functions and using declarations by it, so the port must give the same order.
//
//     culture_compare("a", "A") < 0;        // lower case before upper case
//     culture_compare("a-b", "ab") < 0;     // punctuation is not ignored and sorts before letters
//     culture_compare("\xC3\xA9", "f") < 0;  // an accented e sorts with e (UTF-8 in)
//     culture_compare("System.IO", "System.io");
//
// What is implemented: the root collation at tertiary strength (the default of en-US; no tailoring), alternate handling non-ignorable
// (punctuation and symbols have weights, as .NET sets them up), case at the tertiary level (lower case first), no numeric ordering.
// The weights are ICU's own collation elements, dumped from icu.dll by work/ilspy-port/il4/CCgen into culture_compare_data.inc for: all
// of U+0000..U+33FF, the forms at U+FB00..U+FFEF (ligatures, fullwidth), the mathematical alphanumerics and the pictographs of plane 1
// (U+1D400..U+1D7FF, U+1F000..U+1FAFF); Hangul syllables are decomposed into their jamo, and Han characters of the CJK Unified
// Ideographs and Extension A blocks are ordered by their radical-stroke rank. Every other code point (the other scripts above U+3400
// and the supplementary planes besides those, and Han Extensions B and later) is ordered as ICU orders an unassigned code point: after
// everything above, by code point.
// Not implemented: the contractions of the root collation (Thai, Lao and Tibetan reordering, some Indic sequences), the canonical
// reordering of combining marks that are not in canonical order, and invalid UTF-8 (a bad byte is read as U+FFFD).
//
// Written for the ILSpy port, with the tables derived from the .NET runtime itself (work/ilspy-port/il4/CC is the oracle).
#pragma once

#include <string_view>

namespace cb::dotnet::decompiler::util {

// string.Compare(a, b, StringComparison.CurrentCulture) of en-US (ICU): -1, 0 or 1. Equal (0) does not mean identical: ignorable characters
// (controls, U+00AD, U+200D ...) do not count, and a precomposed letter equals its decomposed form.
int culture_compare(std::string_view a, std::string_view b);

// string.Compare(a, b, StringComparison.CurrentCultureIgnoreCase) / StringComparer.CurrentCultureIgnoreCase.Compare: the same without the
// tertiary level (case and the other tertiary differences are ignored).
int culture_compare_ignore_case(std::string_view a, std::string_view b);

}  // namespace cb::dotnet::decompiler::util
