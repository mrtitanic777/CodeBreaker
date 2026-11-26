// dotnet/decompiler/typesystem/type_system_dump.hpp -- a type system written out as text: the compilation's modules and known types, the
// main module's attributes and namespaces, every type definition with its type parameters, base types, attributes and members, and what
// every member, method, type and type specification reference resolves to. The test oracle (testdata/oracle/TypeSystemDump, on ILSpy's
// own type system) writes the same text, so the two can be compared line for line.
//
//     std::string text = dump_type_system(type_system);
//
// Written for the ILSpy port (no upstream counterpart).
#pragma once

#include <string>

#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"

namespace cb::dotnet::decompiler::typesystem {

std::string dump_type_system(const DecompilerTypeSystem& type_system);

}  // namespace cb::dotnet::decompiler::typesystem
