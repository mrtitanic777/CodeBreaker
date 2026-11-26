// dotnet/decompiler/typesystem/reflection_helper.hpp -- a type named as reflection names it ("System.Collections.Generic.List`1[[System.Int32,
// mscorlib]]", "`0", "Outer+Inner, Assembly") found in a compilation: what a custom attribute's System.Type argument resolves to.
//
//     const IType* t = parse_reflection_name("System.String[]", SimpleTypeResolveContext(compilation));
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: ReflectionHelper.ParseReflectionName and its ReflectionNameParseException),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The parsing itself is System.Reflection.Metadata's
// TypeName (dotnet/metadata/type_name.hpp), as upstream's. ReflectionHelper's FindType(Type) and TypeCode members have no users here.
#pragma once

#include <string_view>

#include "core/error.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::typesystem {

// The text is not a type name (ReflectionNameParseException).
class ReflectionNameParseError : public Error {
public:
    ReflectionNameParseError(int position, const std::string& message) : Error(message), position_(position) {}
    int position() const { return position_; }
    CB_ERROR_BOILERPLATE(ReflectionNameParseError)

private:
    int position_;
};

// The type of that name; an UnknownType for one the compilation does not have. ReflectionNameParseError for text that is not a type
// name (InvalidAssemblyNameError, as upstream, for one whose assembly part is not an assembly name).
const IType* parse_reflection_name(std::string_view reflection_type_name, const ITypeResolveContext& resolve_context);

}  // namespace cb::dotnet::decompiler::typesystem
