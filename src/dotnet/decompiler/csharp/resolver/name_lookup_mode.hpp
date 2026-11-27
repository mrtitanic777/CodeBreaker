// dotnet/decompiler/csharp/resolver/name_lookup_mode.hpp -- what a name is looked up for: an expression, the target of an invocation, a type
// reference, a type in a using declaration or a base type reference.
//
//     resolver->lookup_simple_name_or_type_name("List", type_arguments, NameLookupMode::Type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: NameLookupMode, from NameLookupMode.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>

namespace cb::dotnet::decompiler::csharp::resolver {

enum class NameLookupMode : std::uint8_t {
    // Normal name lookup in expressions
    Expression,
    // Name lookup in an expression that is the target of an invocation. Such a lookup only returns methods and delegate-typed fields.
    InvocationTarget,
    // Normal name lookup in type references.
    Type,
    // Name lookup in the type reference inside a using declaration.
    TypeInUsingDeclaration,
    // Name lookup for base type references.
    BaseTypeReference,
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
