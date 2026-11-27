// dotnet/decompiler/il/control_flow/control_flow_simplification.hpp -- the first control flow clean-up of the ILAst: replaces the
// constructs the C# compiler generates in debug mode with shorter ones that are easier to analyze. 'nop's are removed, branches that
// lead to other branches go directly to the destination, branches to a 'return block' become a return (or the return block moves into
// the try block of the branch), and basic blocks are combined where possible.
//
//     ControlFlowSimplification transform;
//     transform.run(function, context);
//     while (ControlFlowSimplification::combine_block_with_next_block(container, block, context)) {}
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: ControlFlowSimplification, from ControlFlow/ControlFlowSimplification.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class ControlFlowSimplification final : public transforms::IILTransform {
public:
    // Replace every branch to a return block with a copy of the return instead of moving the return block (set by the tests).
    bool aggressively_duplicate_return_blocks = false;

    void run(ILFunction* function, transforms::ILTransformContext& context) override;
    std::string name() const override { return "ControlFlowSimplification"; }

    // Appends the block `block` branches to (when that block has no other way in) to `block`; true when it did.
    static bool combine_block_with_next_block(BlockContainer* container, Block* block, transforms::ILTransformContext& context);

private:
    static void remove_nop_instructions(Block* block);
    static void remove_dead_stack_stores(Block* block, transforms::ILTransformContext& context);
    static void inline_variable_in_return_block(Block* block, transforms::ILTransformContext& context);
    void simplify_branch_chains(ILFunction* function, transforms::ILTransformContext& context);
    static void clean_up_empty_blocks(ILFunction* function, transforms::ILTransformContext& context);
    static bool is_branch_to_return_block(Branch* branch);
    // Whether the last two instructions before the branch store the value 'true' into an unused variable.
    static bool is_dead_true_store(Block* block);
};

}  // namespace cb::dotnet::decompiler::il::control_flow
