// dotnet/decompiler/il/transforms/combine_exits_transform.hpp -- turns a method body that ends in several `leave` instructions guarded by
// ifs (the shape lambdas get after inlining) into one `leave` of a conditional value.
//
//     if (cond) { leave (value) }                     leave (if (cond) value else elseValue)
//     leave (elseValue)                         =>
//
//     CombineExitsTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: CombineExitsTransform, from CombineExitsTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class CombineExitsTransform final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "CombineExitsTransform"; }

private:
    static Leave* combine_exits(Block* block);
};

}  // namespace cb::dotnet::decompiler::il::transforms
