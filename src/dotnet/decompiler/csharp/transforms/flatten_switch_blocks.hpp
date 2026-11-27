// dotnet/decompiler/csharp/transforms/flatten_switch_blocks.hpp -- FlattenSwitchBlocks: a switch section whose only statement is a block
// without local declarations gets the block's statements directly (`case 1: { a(); break; }` becomes `case 1: a(); break;`).
//
//     FlattenSwitchBlocks transform;
//     transform.run(root_node, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: FlattenSwitchBlocks, from FlattenSwitchBlocks.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class FlattenSwitchBlocks final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
