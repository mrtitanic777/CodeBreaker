// dotnet/decompiler/il/transforms/remove_unconstrained_generic_reference_type_check.hpp -- removes the null check the C# compiler emits
// before a constrained call on an unconstrained generic `T` (it copies a value type into a temporary and calls through its address, but
// a reference type is used as is), so the argument temporaries of the call can be inlined again:
//
//     [stloc address(...)]
//     stloc temp(default.value ``0)
//     if (comp.o(ldloc temp == ldnull)) Block IL_002a {
//         stloc temp(ldobj ``0(ldloc address))
//         stloc address(ldloca temp)
//     }
//     stloc V_i(expr_i)
//     ...(constrained[``0].callvirt Method(ldobj.if.ref ``0(ldloc address), ldloc V_i ...))...
//   =>
//     [stloc address(...)]
//     ...(constrained[``0].callvirt Method(ldobj.if.ref ``0(ldloc address), expr_i ...))...
//
//     RemoveUnconstrainedGenericReferenceTypeCheck transform;
//     transform.run(block, pos, statement_context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: RemoveUnconstrainedGenericReferenceTypeCheck, from
// Transforms/RemoveUnconstrainedGenericReferenceTypeCheck.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class RemoveUnconstrainedGenericReferenceTypeCheck final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "RemoveUnconstrainedGenericReferenceTypeCheck"; }
};

}  // namespace cb::dotnet::decompiler::il::transforms
