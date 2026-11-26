// dotnet/metadata/dump.hpp -- every part of a .NET assembly's metadata as text: the CLR header, the heaps entry by entry, every
// table row with its signatures and custom attribute values decoded, every method body. The same text, line for line, as the
// oracle (testdata/oracle/MetadataDump: .NET's own System.Reflection.Metadata) writes, which is how the reader is proved.
//
//     std::string text = dump(PEReader::open("Sample.dll"));
#pragma once

#include <string>

#include "dotnet/metadata/pe_reader.hpp"

namespace cb::dotnet::metadata {

std::string dump(const PEReader& pe);

}  // namespace cb::dotnet::metadata
