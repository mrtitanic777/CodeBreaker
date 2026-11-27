// dotnet/decompiler/il/control_flow/condition_detection.hpp -- detects the `if` structure and the other non-loop aspects of control
// flow: the blocks a branch leads to that are reached only from there are moved into the branching block, as the then-branch of an
// if or as the code after it; common exits are merged, ifs are inverted so the better exit ends the block, and nested conditions are
// combined with && and ||.
//
//     // a post-order block transform (run after LoopDetection, on basic blocks; it leaves extended basic blocks)
//     ConditionDetection transform;
//     transform.run(block, context);
//     ConditionDetection::invert_if(block, if_inst, context);   // also used by other transforms
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: ConditionDetection, from ControlFlow/ConditionDetection.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The uses of ExpressionTransforms and
// HighLevelLoopTransform wait for those headers.
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/flow_analysis/control_flow_node.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class ConditionDetection final : public transforms::IBlockTransform {
public:
    // Builds structured control flow for the block associated with the control flow node. Afterwards the block uses structured
    // control flow and has just a single "regular" exit point (the last branch instruction of the block).
    void run(Block* block, transforms::BlockTransformContext& context) override;
    std::string name() const override { return "ConditionDetection"; }

    // if (cond) { then... } else...; exit;  ->  if (!cond) { else...; exit } then...;   (ifInst must not have an else block)
    static void invert_if(Block* block, IfInstruction* if_inst, transforms::ILTransformContext& context);
    // The start IL offset of the instruction; for a leave with a value that of the value, which is a better indicator of the actual
    // location (some compilers merge the leaves of different arguments through stack variables).
    static int get_start_il_offset(ILInstruction* inst, bool* is_empty);
    // Removes the instructions [start_index, end_index) from the block and adds them to `extracted_block`.
    static void extract_block(Block* block, int start_index, int end_index, Block* extracted_block);

private:
    enum class Keyword { Break, Return, Continue, Other };

    // Repeatedly inlines and simplifies, keeping a good block exit and then attempting to match IL order.
    void handle_if_instruction(Block* block, IfInstruction* if_inst);
    // if (...) br trueBlock;  ->  if (...) { trueBlock... }   (only for blocks strictly dominated by this one)
    bool inline_true_branch(Block* block, IfInstruction* if_inst);
    // ...; br nextBlock;  ->  ...; { nextBlock... }   (only for blocks strictly dominated by this one)
    bool inline_exit_branch(Block* block);
    // Whether the instruction is a branch to a block dominated by this block's node (so it can be replaced by the block itself).
    bool can_inline(ILInstruction* exit_inst);
    // Looks for common exits in the inlined then and else branches of an if and inverts and simplifies to merge them, provided that
    // does not isolate a higher priority block exit.
    void merge_common_branches(Block* block, IfInstruction* if_inst);
    // Finds all exits which could be brought to the block root via inversion.
    void add_exits(ILInstruction* search_inst, int start_index, std::vector<ILInstruction*>& exits);
    // Recursively inverts to bring a desired exit to the root of the block.
    bool produce_exit(ILInstruction* search_inst, int start_index, ILInstruction* target_exit);
    // Anticipates the introduction of an || operator when merging the if and the else exit.
    bool will_short_circuit(Block* block, IfInstruction* if_inst, ILInstruction* else_exit);
    void invert_if(Block* block, IfInstruction* if_inst) { invert_if(block, if_inst, *context_); }
    // if (cond) { } else { ... }  ->  if (!cond) { ... }
    void swap_empty_then(IfInstruction* if_inst);
    // if (cond) { if (nestedCond) { nestedThen... } }  ->  if (cond && nestedCond) { nestedThen... }
    void introduce_short_circuit(IfInstruction* if_inst);
    // if (cond) { lateBlock... } else { earlyBlock... }  ->  if (!cond) { earlyBlock... } else { lateBlock... }
    void order_if_blocks(IfInstruction* if_inst);
    // Compares the current block exit and the exit of the then-branch and inverts if necessary to pick the better exit.
    void pick_better_block_exit(Block* block, IfInstruction* if_inst);
    // Compares two exits for block exit priority: -1, 0 or 1 if exit1 has lower, equal or higher priority than exit2.
    // The priority is: leave > branch > other-keyword > continue > return > break
    int compare_block_exit_priority(ILInstruction* exit1, ILInstruction* exit2, bool strongly = false);
    // Whether an exit instruction has a corresponding keyword and thus does not strictly need merging.
    bool is_keyword_exit(ILInstruction* exit_inst, Keyword* keyword);
    // Whether the instruction necessarily exits (end point unreachable), and if so its last (or single) exit instruction.
    static bool try_get_exit(ILInstruction* inst, ILInstruction** exit_inst);
    // The final instruction of a block (or a single instruction), assuming the end point of everything there is unreachable.
    static ILInstruction* get_exit(ILInstruction* inst);
    // Whether the instruction is a nop or a block with no instructions.
    static bool is_empty(ILInstruction* inst);
    // Whether the branch to `block` is the continue of a loop container (the increment block of a for loop, or the entry point).
    static bool is_continue_block(BlockContainer* container, Block* block);

    transforms::BlockTransformContext* context_ = nullptr;
    flow_analysis::ControlFlowNode* cfg_node_ = nullptr;
    BlockContainer* current_container_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
