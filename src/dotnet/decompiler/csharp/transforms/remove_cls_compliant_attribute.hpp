// dotnet/decompiler/csharp/transforms/remove_cls_compliant_attribute.hpp -- RemoveCLSCompliantAttribute: removes the CLSCompliant
// attributes the compiler added (outside the assembly-level ones), to get rid of many warnings when a whole assembly is written as a project.
//
//     RemoveCLSCompliantAttribute().run(syntax_tree, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: RemoveCLSCompliantAttribute, from RemoveCLSCompliantAttribute.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// Only enabled when exporting a full assembly as a project.
class RemoveCLSCompliantAttribute final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
