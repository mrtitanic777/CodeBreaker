// dotnet/decompiler/csharp/translation_context.hpp -- what the ExpressionBuilder's visit functions are given besides the instruction.
//
//     TranslationContext context{type_hint};      // the type the surroundings expect (SpecialType::unknown_type() for none)
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: TranslationContext, from TranslationContext.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::csharp {

// Context struct passed in to ExpressionBuilder's visit functions.
struct TranslationContext {
    // The expected type during ILAst->C# translation; SpecialType::unknown_type() if no specific type is expected.
    const typesystem::IType* type_hint = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp
