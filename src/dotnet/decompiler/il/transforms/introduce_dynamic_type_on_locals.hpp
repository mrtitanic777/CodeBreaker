// dotnet/decompiler/il/transforms/introduce_dynamic_type_on_locals.hpp -- gives a local of type object the type `dynamic` where it is
// used as a dynamic operand (the target or an argument of a dynamic call site that does not use its compile-time type, or an awaited
// dynamic value), and does the same for every local of the same slot in the function.
//
//     IntroduceDynamicTypeOnLocals transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: IntroduceDynamicTypeOnLocals, from IntroduceDynamicTypeOnLocals.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class IntroduceDynamicTypeOnLocals final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "IntroduceDynamicTypeOnLocals"; }
};

}  // namespace cb::dotnet::decompiler::il::transforms
