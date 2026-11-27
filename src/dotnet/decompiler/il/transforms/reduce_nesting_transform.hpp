// dotnet/decompiler/il/transforms/reduce_nesting_transform.hpp -- reduces the indentation of the decompiled code by duplicating keyword
// exits (return, break, continue) into the branches of an if or switch, so the else block or default case can follow the statement
// instead of being nested in it, and restores IL order after the loop transforms; also removes the try-finally the C# compiler puts
// around a fixed statement.
//
//     if (a) { ... } else { many statements }  return;      =>   if (a) { ...  return; }  many statements  return;
//     ReduceNestingTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: ReduceNestingTransform, from ReduceNestingTransform.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class ReduceNestingTransform final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "ReduceNestingTransform"; }

private:
    void visit(BlockContainer* container, Block* continue_target);
    // Visits a block in context. `continue_target` marks the target block of continue statements; `next_instruction` is the
    // instruction following the end point of the block (null only if the end point is unreachable).
    void visit(Block* block, Block* continue_target, ILInstruction* next_instruction = nullptr);
    // search for child containers to reduce nesting in
    void visit_containers(ILInstruction* inst, Block* continue_target);
    // For an if statement with an unreachable end point and no else block, inverts to match IL order of the first statement of each branch.
    void improve_il_ordering(Block* block, IfInstruction* if_inst, Block* continue_target);
    // Reduce nesting in if/else statements by duplicating an exit instruction. Does not affect IL order.
    bool reduce_nesting(Block* block, IfInstruction* if_inst, ILInstruction* exit_inst);
    // Reduce nesting in switch statements by replacing break; in cases with the block exit, and extracting the default case.
    bool reduce_switch_nesting(Block* parent_block, BlockContainer* switch_container, ILInstruction* exit_inst);
    // Checks if an exit is a duplicable keyword exit (return; break; continue;).
    bool can_duplicate_exit(ILInstruction* exit, Block* continue_target, ILInstruction** keyword_exit);
    // Ensures the end point of a block is unreachable by duplicating and appending the exit instruction following the end point.
    void ensure_end_point_unreachable(ILInstruction* inst, ILInstruction* fallthrough_exit);
    void remove_redundant_exit(Block* block, ILInstruction* implicit_exit);
    // Determines if an IfInstruction is an else-if and returns the preceding (parent) IfInstruction.
    IfInstruction* get_else_if_parent(IfInstruction* if_inst);
    void update_stats(ILInstruction* inst, int& max_statements, int& max_depth);
    void compute_stats(ILInstruction* inst, int& num_statements, int& max_depth, int current_depth, bool is_statement = true);
    bool should_reduce_nesting(Block* block, int max_statements, int max_depth);
    void extract_else_block(IfInstruction* if_inst);
    void eliminate_redundant_try_finally(TryFinally* try_finally, ILTransformContext& context);

    ILTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::transforms
