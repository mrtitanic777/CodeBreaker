// dotnet/decompiler/nr_extensions.hpp -- ILSpy's helpers over the type system's entities that decide what a compiler made: whether a type
// is a C# or Visual Basic anonymous type, whether an entity is compiler-generated or in a compiler-generated class.
//
//     if (is_anonymous_type(type)) ...      // a C# anonymous type (immutable, compiler-generated, no namespace)
//
// Ported from ILSpy (ICSharpCode.Decompiler: NRExtensions, from NRExtensions.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). Not yet ported: GetDocumentation (it waits on ICSharpCode.Decompiler.Documentation, XmlDocLoader).
#pragma once

#include <cstdint>

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler {

// IEntity.IsCompilerGenerated: the entity has the CompilerGenerated attribute (false for null).
bool is_compiler_generated(const typesystem::IEntity* entity);
// Whether the entity or one of the types it is declared in is compiler-generated (false for null).
bool is_compiler_generated_or_is_in_compiler_generated_class(const typesystem::IEntity* entity);

// Whether the type's name is one a compiler made ('<' first, or a '$' in it).
bool has_generated_name(const typesystem::IType* type);
// Whether the member's name is one a compiler made (it starts with '<').
bool has_generated_name(const typesystem::IMember* member);

// A C# anonymous type that keeps its own declaration because it cannot be written as a C# anonymous type: a VB anonymous type with at
// least one settable, non-'Key' property.
bool is_anonymous_type_declared_as_named_type(const typesystem::ITypeDefinition* type);

bool is_anonymous_type(const typesystem::IType* type);

// Whether the type is, or has among its type arguments or element types, an anonymous type.
bool contains_anonymous_type(const typesystem::IType* type);

// The TypeAttributes flags of the type's row in its module's metadata (0 for a type without a metadata file or token).
std::uint32_t get_metadata_attributes(const typesystem::ITypeDefinition* type);

}  // namespace cb::dotnet::decompiler
