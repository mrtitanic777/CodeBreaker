// dotnet/decompiler/csharp/transforms/rename_visual_basic_anonymous_types.hpp -- RenameVisualBasicAnonymousTypes: gives the anonymous types
// of a VB assembly that are declared (they have a settable property, so no C# anonymous type can stand for them) a name that is legal in C#.
//
//     RenameVisualBasicAnonymousTypes().run(syntax_tree, context);       // "VB$AnonymousType_0`1" -> "VB_AnonymousType_0`1"
//
// The VB compiler separates the parts of such a name with '$', which is not a legal identifier character in C#, and does the same for the
// backing fields. The renaming is done per identifier through its symbol, so a reference is renamed the same way whether or not the
// declaration is part of the same output.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: RenameVisualBasicAnonymousTypes, from RenameVisualBasicAnonymousTypes.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class RenameVisualBasicAnonymousTypes final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
