// dotnet/decompiler/il/transforms/il_transform_pipeline.hpp -- the ILAst pipeline: every transform the C# decompiler runs on a method's
// ILAst, in order, and the short early pipeline run on bodies read during a transform (lambdas, state machines, proxies).
//
//     auto transforms = get_il_transforms();
//     function->run_transforms(transforms, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: CSharpDecompiler.GetILTransforms and EarlyILTransforms), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <memory>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

std::vector<std::unique_ptr<IILTransform>> get_il_transforms();
std::vector<std::unique_ptr<IILTransform>> early_il_transforms(bool aggressively_duplicate_return_blocks = false);

}  // namespace cb::dotnet::decompiler::il::transforms
