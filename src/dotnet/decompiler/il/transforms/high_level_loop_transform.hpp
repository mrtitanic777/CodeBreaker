// dotnet/decompiler/il/transforms/high_level_loop_transform.hpp -- turns the plain loops of the ILAst (`while (true)` containers) into
// while (condition), do-while and for loops: the exit test at the start of the body becomes the while condition, the tests at the end
// become the do-while condition, and the increment statements before the jump back become a for loop's increment block.
//
//     loop: if (!cond) leave loop; body; branch loop        =>   while (cond) { body }
//     HighLevelLoopTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: HighLevelLoopTransform, from HighLevelLoopTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class HighLevelLoopTransform final : public IILTransform {
public:
    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "HighLevelLoopTransform"; }

    // Early match before block containers have been constructed: a block that ends in `if (c) branch/return; branch/return`.
    static bool match_do_while_condition_block(Block* block, Block** target1, Block** target2);
    // The block of the loop (not the while loop's body) that jumps back to the entry point after simple statements only; nullptr for none.
    static Block* get_increment_block(BlockContainer* loop, Block* while_loop_body);
    // A block of simple statements that ends in a branch (to `loop_head`).
    static bool match_increment_block(Block* block, Block** loop_head);
    // stloc v(add(ldloc v, arg)) or compound.assign(ldloca v, arg)
    static bool match_increment(ILInstruction* inst, ILVariable** variable);

private:
    bool match_while_loop(BlockContainer* loop, IfInstruction** condition, Block** loop_body);
    static bool might_be_header_of_for_each(BlockContainer* loop, const std::vector<ILInstruction*>& conditions);
    static void split_conditions(ILInstruction* expression, std::vector<ILInstruction*>& conditions);
    bool match_do_while_loop(BlockContainer* loop);

    struct DoWhileConditions {
        std::vector<IfInstruction*> conditions;
        bool found = false;  // upstream's conditions != null
        ILInstruction* exit = nullptr;
        bool swap = false;
        bool split = false;
        bool unwrap = false;
    };
    static DoWhileConditions analyze_do_while_conditions(BlockContainer* loop);
    static std::vector<IfInstruction*> collect_conditions(BlockContainer* loop, Block* block, bool swap);
    static bool uses_variable_captured_in_loop(BlockContainer* loop, ILInstruction* condition);
    static bool match_do_while_condition_block(BlockContainer* loop, Block* block, bool* swap_branches, bool* unwrap_condition_block,
                                               Block** condition_block);
    bool match_for_loop(BlockContainer* loop, IfInstruction* while_condition, Block* while_loop_body);
    static bool stores_ref_local_used_after_loop(BlockContainer* loop, Block* increment_block);
    static bool is_variable_used_after_loop(BlockContainer* loop, ILVariable* variable);
    static bool is_assignment(ILInstruction* inst);
    static bool is_simple_statement(ILInstruction* inst);

    ILTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::transforms
