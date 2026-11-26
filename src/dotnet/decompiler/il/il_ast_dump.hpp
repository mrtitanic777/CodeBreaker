// dotnet/decompiler/il/il_ast_dump.hpp -- every method body of a type system's main module read into the ILAst and written as text,
// method by method ("method <token> <Type>::<name>", then the ILFunction, or "error <kind>: <message>"). The test oracle
// (testdata/oracle/DecompilerDump, mode `ilast`, on ILSpy's own ILReader) writes the same text, so the two compare line for line.
//
//     std::string text = dump_il_ast(type_system);
//
// Written for the ILSpy port (no upstream counterpart).
#pragma once

#include <string>

#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"

namespace cb::dotnet::decompiler::il {

std::string dump_il_ast(const typesystem::DecompilerTypeSystem& type_system);

}  // namespace cb::dotnet::decompiler::il
