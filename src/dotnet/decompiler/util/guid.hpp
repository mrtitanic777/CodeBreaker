// dotnet/decompiler/util/guid.hpp -- System.Guid as ILSpy's project writers use it: a new random one, one parsed from its text, its text in the
// formats "D" (8-4-4-4-12), "B" (the same in braces) and "N", in lower case or upper.
//
//     Guid g = Guid::new_guid();              // a random (version 4) GUID
//     Guid t = Guid::parse("{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}");
//     t.to_string('B') == "{fae04ec0-301f-11d3-bf4b-00c04f79efbc}";    // .NET writes lower case; ILSpy upper-cases it where it wants
//
// Written for the ILSpy port from the description of System.Guid. A Guid is the metadata reader's (dotnet/metadata): sixteen bytes in the
// layout .NET keeps them (the first three groups little-endian), so a GUID read from an assembly and one parsed from text compare as .NET compares.
#pragma once

#include <string>
#include <string_view>

#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::util {

using Guid = cb::dotnet::metadata::Guid;

namespace guid {

// A random GUID (Guid.NewGuid: version 4, variant 1).
Guid new_guid();
// Guid.Parse: "D", "B", "N" (32 digits) or "P" (in parentheses) form; UsageError (a FormatException upstream) for any other text.
Guid parse(std::string_view text);
// Guid.ToString(format): 'D' (default), 'B', 'N', 'P'; lower-case hex digits. UsageError for any other format.
std::string to_string(const Guid& guid, char format = 'D');
// to_string(guid, 'B') in upper case: what the project writers write (`ToString("B").ToUpperInvariant()`).
std::string to_string_b_upper(const Guid& guid);

}  // namespace guid

}  // namespace cb::dotnet::decompiler::util
