// dotnet/decompiler/il/transforms/fix_remaining_increments.hpp -- turns a call to a user-defined `op_Increment`/`op_Decrement` that
// TransformAssignment did not make a compound assignment of (Roslyn may drop the variable being incremented) into a statement that
// stores the argument and increments it: `stloc V(call op_Increment(x))` becomes `stloc V(x)` followed by `compound.assign op_Increment(V)`.
//
//     FixRemainingIncrements transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: FixRemainingIncrements, from Transforms/FixRemainingIncrements.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class FixRemainingIncrements final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "FixRemainingIncrements"; }
};

}  // namespace cb::dotnet::decompiler::il::transforms
