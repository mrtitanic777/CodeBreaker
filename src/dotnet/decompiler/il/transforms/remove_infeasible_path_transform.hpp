// dotnet/decompiler/il/transforms/remove_infeasible_path_transform.hpp -- short-cuts a branch to a block that tests a flag the branching
// block has just set to a constant: the test can only go one way, so the branch goes straight to that exit.
//
//     Block IL_0018 { stloc s(ldc.i4 1); br IL_0019 }
//     Block IL_0019 (incoming: > 1) { if (logic.not(ldloc s)) br IL_0027; br IL_001d }
//     =>  `br IL_0019` in IL_0018 becomes `br IL_001d` (the true side), and `s` is marked RemoveIfRedundant
//
//     RemoveInfeasiblePathTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: RemoveInfeasiblePathTransform, from
// Transforms/RemoveInfeasiblePathTransform.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class RemoveInfeasiblePathTransform final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "RemoveInfeasiblePathTransform"; }

private:
    static bool do_transform(Block* block, ILTransformContext& context);
    // Block { stloc s(ldc.i4 0 or 1); br target }
    static bool match_block1(Block* block, ILVariable** variable, int* constant_value, Branch** branch);
    // Block (incoming: > 1) { if (ldloc s) trueInst; falseInst } where each exit is a branch or a leave without a value
    static bool match_block2(Block* block, ILVariable* s, int constant_value, ILInstruction** exit_inst);
};

}  // namespace cb::dotnet::decompiler::il::transforms
