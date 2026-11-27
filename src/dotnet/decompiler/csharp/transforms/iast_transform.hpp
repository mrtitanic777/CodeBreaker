// dotnet/decompiler/csharp/transforms/iast_transform.hpp -- IAstTransform, one step of the pipeline that turns the C# syntax tree the
// ExpressionBuilder made into the tree that is written out: it is given the root of the tree and the TransformContext and changes the tree
// in place.
//
//     class ReplaceX final : public IAstTransform {
//     public:
//         void run(AstNode* root_node, TransformContext& context) override { ... }
//     };
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: IAstTransform, from IAstTransform.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/ast_fwd.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class TransformContext;

class IAstTransform {
public:
    virtual ~IAstTransform() = default;
    virtual void run(syntax::AstNode* root_node, TransformContext& context) = 0;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
