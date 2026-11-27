// dotnet/decompiler/il/transforms/detect_catch_when_condition_blocks.hpp -- finds the `catch (E) when (...)` filters the C# compiler
// writes as a filter block starting with the isinst test of the exception type, and strips that test: the handler's variable gets the
// exception type, the filter's entry point goes straight to the when condition, and the copies of the exception variable inside the
// handler are propagated.
//
//     DetectCatchWhenConditionBlocks transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: DetectCatchWhenConditionBlocks, from
// Transforms/DetectCatchWhenConditionBlocks.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class DetectCatchWhenConditionBlocks final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "DetectCatchWhenConditionBlocks"; }

private:
    // Copy-propagates the exception variable over the single-definition copies of it inside the handler.
    static void propagate_exception_variable(ILTransformContext& context, TryCatchHandler* handler);
    // The entry block of a filter: the isinst test of the exception type, then a branch to the when condition or to the false block.
    static bool match_catch_when_entry_point(ILVariable* exception_var, BlockContainer* container, Block* entry_point,
                                             const IType** exception_type, ILInstruction** exception_slot, Block** when_condition_block);
    // Block falseBlock (incoming: 1) { stloc returnVar(ldc.i4 0); br exitBlock }
    static bool match_false_block(BlockContainer* container, Block* false_block, ILVariable** return_var, Block** exit_block);
    // Block exitBlock (incoming: 2) { leave container(ldloc returnVar) }
    static bool match_exit_block(BlockContainer* container, Block* exit_block, ILVariable* return_var);
};

}  // namespace cb::dotnet::decompiler::il::transforms
