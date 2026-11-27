// dotnet/decompiler/il/control_flow/condition_detection.cxx -- see condition_detection.hpp.
#include "dotnet/decompiler/il/control_flow/condition_detection.hpp"

#include <algorithm>
#include <cassert>
#include <format>

#include "dotnet/decompiler/il/control_flow/exit_points.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"

#if __has_include("dotnet/decompiler/il/transforms/expression_transforms.hpp")
#include "dotnet/decompiler/il/transforms/expression_transforms.hpp"
#define CB_HAVE_EXPRESSION_TRANSFORMS
#endif
#if __has_include("dotnet/decompiler/il/transforms/high_level_loop_transform.hpp")
#include "dotnet/decompiler/il/transforms/high_level_loop_transform.hpp"
#define CB_HAVE_HIGH_LEVEL_LOOP_TRANSFORM
#endif

namespace cb::dotnet::decompiler::il::control_flow {

void ConditionDetection::run(Block* block, transforms::BlockTransformContext& context) {
    context_ = &context;
    current_container_ = static_cast<BlockContainer*>(block->parent());

    // We only embed blocks into this block if they aren't referenced anywhere else, so those blocks are dominated by this block.
    // BlockILTransform thus guarantees that the blocks being embedded are already fully processed.

    cfg_node_ = context.control_flow_node;
    assert(cfg_node_->user_data == block);

    // Because this transform runs at the beginning of the block transforms, we know that `block` is still a (non-extended) basic block.

    // Previous-to-last instruction might have conditional control flow, usually an IfInstruction with a branch:
    if (auto* if_inst = as<IfInstruction>(block->instructions.second_to_last_or_default()))
        handle_if_instruction(block, if_inst);
    else
        inline_exit_branch(block);
}

// Repeatedly inlines and simplifies, maintaining a good block exit and then attempting to match IL order
void ConditionDetection::handle_if_instruction(Block* block, IfInstruction* if_inst) {
    while (inline_true_branch(block, if_inst) || inline_exit_branch(block)) {
        pick_better_block_exit(block, if_inst);
        merge_common_branches(block, if_inst);
        swap_empty_then(if_inst);
        introduce_short_circuit(if_inst);
    }
    pick_better_block_exit(block, if_inst);
    order_if_blocks(if_inst);
    context_->index_of_first_already_transformed_instruction = block->instructions.count();
}

//   if (...) br trueBlock;
// ->
//   if (...) { trueBlock... }
//
// Only inlines branches that are strictly dominated by this block (incoming edge count == 1)
bool ConditionDetection::inline_true_branch(Block* block, IfInstruction* if_inst) {
    if (!can_inline(if_inst->true_inst())) {
        if (block->instructions.second_to_last_or_default() == if_inst && if_inst->false_inst()->match_nop()) {
            ILInstruction* exit_inst = block->instructions.last();
            if (DetectExitPoints::compatible_exit_instruction(if_inst->true_inst(), exit_inst)) {
                // if (...) exitInst; exitInst;
                context_->step("Use empty block as then-branch", if_inst->true_inst());
                if_inst->set_true_inst(with_il_range(make<Nop>(), if_inst->true_inst()));
                // false, because we didn't inline a real block
                // this will cause HandleIfInstruction() to attempt to inline the exitInst.
                return false;
            }
        }
        return false;
    }

    context_->step("Inline block as then-branch", if_inst->true_inst());
    // The targetBlock was already processed, and is ready to embed
    Block* target_block = static_cast<Branch*>(if_inst->true_inst())->target_block();
    target_block->add_ref();  // Performance: avoid temporarily disconnecting targetBlock
    target_block->remove();
    if_inst->set_true_inst(target_block);
    target_block->release_ref();

    return true;
}

//   ...; br nextBlock;
// ->
//   ...; { nextBlock... }
//
// Only inlines branches that are strictly dominated by this block (incoming edge count == 1)
bool ConditionDetection::inline_exit_branch(Block* block) {
    ILInstruction* exit_inst = get_exit(block);
    if (!can_inline(exit_inst)) return false;

    context_->step("Inline target block of unconditional branch", exit_inst);
    // The targetBlock was already processed, and is ready to embed
    Block* target_block = static_cast<Branch*>(exit_inst)->target_block();
    block->instructions.remove_at(block->instructions.count() - 1);
    context_->index_of_first_already_transformed_instruction = block->instructions.count();
    block->instructions.add_range(target_block->instructions);
    target_block->remove();

    return true;
}

// Gets whether the instruction is a branch to a block that is dominated by cfg_node_.
// If this function returns true, we replace the branch instruction with the block itself.
bool ConditionDetection::can_inline(ILInstruction* exit_inst) {
    auto* branch = as<Branch>(exit_inst);
    if (branch != nullptr && branch->target_block()->parent() == current_container_ && branch->target_block()->incoming_edge_count() == 1) {
        // if the incoming edge count is 1, then this must be the sole branch, and dominance is already ensured
        assert(cfg_node_->dominates(context_->control_flow_graph->get_node(branch->target_block())));
        // can't have "final instructions" in control flow blocks
        assert(is<Nop>(branch->target_block()->final_instruction()));
        return true;
    }

    return false;
}

// Looks for common exits in the inlined then and else branches of an if instruction and performs inversions and simplifications to
// merge them provided they don't isolate a higher priority block exit
void ConditionDetection::merge_common_branches(Block* block, IfInstruction* if_inst) {
    std::vector<ILInstruction*> then_exits;
    add_exits(if_inst->true_inst(), 0, then_exits);
    if (then_exits.empty()) return;

    // if there are any exits from the then branch, then the else is redundant and shouldn't exist
    assert(is_empty(if_inst->false_inst()));
    assert(if_inst->parent() == block);
    std::vector<ILInstruction*> else_exits;
    int false_inst_index = block->instructions.index_of(if_inst) + 1;
    add_exits(block, false_inst_index, else_exits);

    // find the common exit with the highest block exit priority
    ILInstruction* common_exit = nullptr;
    for (ILInstruction* exit : else_exits) {
        bool is_common = std::any_of(then_exits.begin(), then_exits.end(),
                                     [&](ILInstruction* e2) { return DetectExitPoints::compatible_exit_instruction(exit, e2); });
        if (!is_common) continue;
        if (common_exit == nullptr || compare_block_exit_priority(exit, common_exit) > 0) common_exit = exit;
    }

    if (common_exit == nullptr) return;

    // if the current block exit has higher priority than the exits to merge,
    // determine if this merge will isolate the current block exit
    // that is, no sequence of inversions can restore it to the block exit position
    ILInstruction* block_exit = block->instructions.last();
    if (compare_block_exit_priority(block_exit, common_exit, true) > 0 && !will_short_circuit(block, if_inst, common_exit)) return;

    // could improve performance by directly implementing the || short-circuit when WillShortCircuit
    // currently the same general sequence of transformations introduces both operators

    context_->step_start_group("Merge common branches", if_inst);
    produce_exit(if_inst->true_inst(), 0, common_exit);
    produce_exit(block, false_inst_index, common_exit);

    // if (...) { ...; blockExit; } ...; blockExit;
    // -> if (...) { ...; blockExit; } else { ... } blockExit;
    if (if_inst != block->instructions.second_to_last_or_default()) {
        context_->step("Embed else-block for goto removal", if_inst);
        assert(is_empty(if_inst->false_inst()));
        Block* new_block = make<Block>();
        if_inst->set_false_inst(new_block);
        extract_block(block, block->instructions.index_of(if_inst) + 1, block->instructions.count() - 1, new_block);
    }

    // if (...) { ...; goto blockExit; } blockExit;
    // -> if (...) { ... } blockExit;
    // OR
    // if (...) { ...; goto blockExit; } else { ... } blockExit;
    // -> if (...) { ... } else { ... } blockExit;
    context_->step("Remove redundant 'goto blockExit;' in then-branch", if_inst);
    auto* true_block = as<Block>(if_inst->true_inst());
    if (true_block == nullptr || true_block->instructions.count() == 1)
        if_inst->set_true_inst(with_il_range(make<Nop>(), if_inst->true_inst()));
    else
        true_block->instructions.remove_at(true_block->instructions.count() - 1);

    context_->step_end_group();
}

// Finds all exits which could be brought to the block root via inversion
void ConditionDetection::add_exits(ILInstruction* search_inst, int start_index, std::vector<ILInstruction*>& exits) {
    ILInstruction* exit_inst = nullptr;
    if (!try_get_exit(search_inst, &exit_inst)) return;

    exits.push_back(exit_inst);
    if (auto* block = as<Block>(search_inst)) {
        for (int i = start_index; i < block->instructions.count(); i++) {
            if (auto* if_inst = as<IfInstruction>(block->instructions[i])) add_exits(if_inst->true_inst(), 0, exits);
        }
    }
}

// Recursively performs inversions to bring a desired exit to the root of the block
// for example:
//   if (a) {
//     ...;
//     if (b) {
//       ...;
//       targetExit;
//     }
//     ...;
//     exit1;
//   }
//   ...;
//   exit2;
// ->
//   if (!a) {
//     ...;
//     exit2;
//   }
//   ...;
//   if (!b) {
//     ...;
//     exit1;
//   }
//   ...;
//   targetExit;
bool ConditionDetection::produce_exit(ILInstruction* search_inst, int start_index, ILInstruction* target_exit) {
    ILInstruction* exit_inst = nullptr;
    if (!try_get_exit(search_inst, &exit_inst)) return false;

    if (DetectExitPoints::compatible_exit_instruction(exit_inst, target_exit)) return true;

    if (auto* block = as<Block>(search_inst)) {
        for (int i = start_index; i < block->instructions.count(); i++) {
            auto* if_inst = as<IfInstruction>(block->instructions[i]);
            if (if_inst != nullptr && produce_exit(if_inst->true_inst(), 0, target_exit)) {
                invert_if(block, if_inst);
                assert(DetectExitPoints::compatible_exit_instruction(get_exit(block), target_exit));
                return true;
            }
        }
    }

    return false;
}

// Anticipates the introduction of an || operator when merging ifInst and elseExit
//
//   if (cond) commonExit;
//   if (cond2) commonExit;
//   ...;
//   blockExit;
// will become:
//   if (cond || cond2) commonExit;
//   ...;
//   blockExit;
bool ConditionDetection::will_short_circuit(Block* block, IfInstruction* if_inst, ILInstruction* else_exit) {
    auto then_inst_is_single_exit = [](ILInstruction* inst) {
        ILInstruction* ignored_condition = nullptr;
        ILInstruction* true_inst = nullptr;
        if (!inst->match_if_instruction(&ignored_condition, &true_inst)) return false;
        auto* true_block = as<Block>(true_inst);
        if (!(true_block == nullptr || true_block->instructions.count() == 1)) return false;
        ILInstruction* ignored_exit = nullptr;
        return try_get_exit(true_inst, &ignored_exit);
    };

    if (!then_inst_is_single_exit(if_inst)) return false;

    // find the host if statement
    ILInstruction* else_if_inst = else_exit;
    while (else_if_inst->parent() != block) else_if_inst = else_if_inst->parent();

    return block->instructions.index_of(else_if_inst) == block->instructions.index_of(if_inst) + 1 && then_inst_is_single_exit(else_if_inst);
}

//   if (cond) { then... }
//   else...;
//   exit;
// ->
//   if (!cond) { else...; exit }
//   then...;
//
// Assumes ifInst does not have an else block
void ConditionDetection::invert_if(Block* block, IfInstruction* if_inst, transforms::ILTransformContext& context) {
    assert(if_inst->parent() == block);

    // assert then block terminates
    ILInstruction* exit_inst = get_exit(block);
    context.step(std::format("InvertIf at IL_{:04x}", static_cast<unsigned>(if_inst->start_il_offset())), if_inst);

    // if the then block terminates, else blocks are redundant, and should not exist
    assert(is_empty(if_inst->false_inst()));

    // save a copy
    ILInstruction* then_inst = if_inst->true_inst();
    then_inst->add_ref();

    if (if_inst != block->instructions.second_to_last_or_default()) {
        // extract "else...; exit".
        // Note that this will only extract instructions that were previously inlined from another block
        // (via InlineExitBranch), so the instructions are already fully-transformed.
        // So it's OK to move them into a nested block again (which hides them from the following block transforms).
        Block* new_block = make<Block>();
        if_inst->set_true_inst(new_block);
        extract_block(block, block->instructions.index_of(if_inst) + 1, block->instructions.count(), new_block);
    } else {
        if_inst->set_true_inst(exit_inst);
        block->instructions.remove_at(block->instructions.count() - 1);
    }

    if (auto* then_block = as<Block>(then_inst)) {
        block->instructions.add_range(then_block->instructions);
    } else {
        block->instructions.add(then_inst);
    }
    then_inst->release_ref();

    if_inst->set_condition(Comp::logic_not(if_inst->condition()));
#ifdef CB_HAVE_EXPRESSION_TRANSFORMS
    transforms::ExpressionTransforms::run_on_single_statement(if_inst, context);
#else
    // WAITS: ExpressionTransforms (run_on_single_statement)
#endif
}

//   if (cond) { } else { ... }
// ->
//   if (!cond) { ... }
void ConditionDetection::swap_empty_then(IfInstruction* if_inst) {
    if (!is_empty(if_inst->true_inst())) return;

    context_->step("Swap empty then-branch with else-branch", if_inst);
    ILInstruction* old_true = if_inst->true_inst();
    if_inst->set_true_inst(if_inst->false_inst());
    if_inst->set_false_inst(with_il_range(make<Nop>(), old_true));
    if_inst->set_condition(Comp::logic_not(if_inst->condition()));
}

//   if (cond) { if (nestedCond) { nestedThen... } }
// ->
//   if (cond && nestedCond) { nestedThen... }
void ConditionDetection::introduce_short_circuit(IfInstruction* if_inst) {
    auto* true_block = as<Block>(if_inst->true_inst());
    ILInstruction* nested_condition = nullptr;
    ILInstruction* nested_true_inst = nullptr;
    if (is_empty(if_inst->false_inst()) && true_block != nullptr && true_block->instructions.count() == 1 &&
        is<Nop>(true_block->final_instruction()) && true_block->instructions[0]->match_if_instruction(&nested_condition, &nested_true_inst)) {
        context_->step("Combine 'if (cond1 && cond2)' in then-branch", if_inst);
        if_inst->set_condition(IfInstruction::logic_and(if_inst->condition(), nested_condition));
        if_inst->set_true_inst(nested_true_inst);
    }
}

//   if (cond) { lateBlock... } else { earlyBlock... }
// ->
//   if (!cond) { earlyBlock... } else { lateBlock... }
void ConditionDetection::order_if_blocks(IfInstruction* if_inst) {
    bool ignored;
    if (is_empty(if_inst->false_inst()) || get_start_il_offset(if_inst->true_inst(), &ignored) <= get_start_il_offset(if_inst->false_inst(), &ignored))
        return;

    context_->step("Swap then-branch with else-branch to match IL order", if_inst);
    ILInstruction* old_true = if_inst->true_inst();
    old_true->add_ref();  // Performance: avoid temporarily disconnecting oldTrue
    if_inst->set_true_inst(if_inst->false_inst());
    if_inst->set_false_inst(old_true);
    old_true->release_ref();
    if_inst->set_condition(Comp::logic_not(if_inst->condition()));
}

int ConditionDetection::get_start_il_offset(ILInstruction* inst, bool* is_empty) {
    // some compilers merge the leave instructions for different arguments using stack variables
    // these get split and inlined, but the ILRange of the value remains a better indicator of the actual location
    auto* leave = as<Leave>(inst);
    if (leave != nullptr && !leave->value()->match_nop()) {
        *is_empty = leave->value()->il_range_is_empty();
        return leave->value()->start_il_offset();
    }

    *is_empty = inst->il_range_is_empty();
    return inst->start_il_offset();
}

// Compares the current block exit, and the exit of ifInst.ThenInst and inverts if necessary to pick the better exit
//
// Does nothing when ifInst has an else block (because inverting wouldn't affect the block exit)
void ConditionDetection::pick_better_block_exit(Block* block, IfInstruction* if_inst) {
    ILInstruction* exit_inst = get_exit(block);
    ILInstruction* true_exit_inst = nullptr;
    if (is_empty(if_inst->false_inst()) && try_get_exit(if_inst->true_inst(), &true_exit_inst) &&
        compare_block_exit_priority(true_exit_inst, exit_inst) > 0)
        invert_if(block, if_inst);
}

// Compares two exit instructions for block exit priority
// A higher priority exit should be kept as the last instruction in a block
// even if it prevents the merging of a two compatible lower priority exits
//
// leave from try containers must always be the final instruction, or a goto will be inserted
// loops will endeavour to leave at least one continue branch as the last instruction in the block
//
// The priority is:
//   leave > branch > other-keyword > continue > return > break
//
// non-keyword leave instructions are ordered with the outer container having higher priority
//
// if the exits have equal priority, and the `strongly` flag is not provided
// then the exits are sorted by IL order (target block for branches)
//
// break has higher priority than other keywords in a switch block (for aesthetic reasons)
int ConditionDetection::compare_block_exit_priority(ILInstruction* exit1, ILInstruction* exit2, bool strongly) {
    auto compare_to = [](int a, int b) { return a < b ? -1 : (a > b ? 1 : 0); };
    // keywords have lower priority than non-keywords
    Keyword keyword1;
    Keyword keyword2;
    bool is_keyword1 = is_keyword_exit(exit1, &keyword1);
    bool is_keyword2 = is_keyword_exit(exit2, &keyword2);
    if (is_keyword1 != is_keyword2) return is_keyword1 ? -1 : 1;

    if (is_keyword1) {
        // for keywords
        if (current_container_->kind() == ContainerKind::Switch) {
            // breaks have highest priority in a switch
            if ((keyword1 == Keyword::Break) != (keyword2 == Keyword::Break)) return keyword1 == Keyword::Break ? 1 : -1;
        } else {
            // breaks have lowest priority
            if ((keyword1 == Keyword::Break) != (keyword2 == Keyword::Break)) return keyword1 == Keyword::Break ? -1 : 1;

            // continue has highest priority (to prevent having to jump to the end of a loop block)
            if ((keyword1 == Keyword::Continue) != (keyword2 == Keyword::Continue)) return keyword1 == Keyword::Continue ? 1 : -1;
        }
    } else {
        // for non-keywords (only Branch or Leave)
        // branches have lower priority than non-keyword leaves
        bool is_branch1 = is<Branch>(exit1);
        bool is_branch2 = is<Branch>(exit2);
        if (is_branch1 != is_branch2) return is_branch1 ? -1 : 1;

        // two leaves that both want end of block priority
        BlockContainer* container1 = nullptr;
        BlockContainer* container2 = nullptr;
        if (exit1->match_leave(&container1) && exit2->match_leave(&container2) && container1 != container2) {
            // choose the outer one
            return container2->is_descendant_of(container1) ? 1 : -1;
        }
    }

    if (strongly) return 0;

    // prefer arranging stuff in IL order
    Block* block1 = nullptr;
    Block* block2 = nullptr;
    if (exit1->match_branch(&block1) && exit2->match_branch(&block2)) return compare_to(block1->start_il_offset(), block2->start_il_offset());

    // use the IL offsets of the arguments of leave instructions instead of the leaves themselves if possible
    BlockContainer* ignored_container = nullptr;
    ILInstruction* arg1 = nullptr;
    ILInstruction* arg2 = nullptr;
    if (exit1->match_leave(&ignored_container, &arg1) && exit2->match_leave(&ignored_container, &arg2))
        return compare_to(arg1->start_il_offset(), arg2->start_il_offset());

    return compare_to(exit1->start_il_offset(), exit2->start_il_offset());
}

// Determines if an exit instruction has a corresponding keyword and thus doesn't strictly need merging
// Branches can be 'continue' or goto (non-keyword)
// Leave can be 'return', 'break' or a pinned container exit (try/using/lock etc)
// All other instructions (throw, using, etc) are returned as Keyword.Other
bool ConditionDetection::is_keyword_exit(ILInstruction* exit_inst, Keyword* keyword) {
    *keyword = Keyword::Other;
    if (auto* branch = as<Branch>(exit_inst)) {
        if (is_continue_block(branch->target_container(), branch->target_block())) {
            *keyword = Keyword::Continue;
            return true;
        }
        return false;
    }
    if (auto* leave = as<Leave>(exit_inst)) {
        if (leave->is_leaving_function()) {
            *keyword = Keyword::Return;
            return true;
        }
        if (leave->target_container()->kind() != ContainerKind::Normal) {
            *keyword = Keyword::Break;
            return true;
        }
        return false;
    }
    return true;
}

// Determine if the specified instruction necessarily exits (EndPointUnreachable) and if so return last (or single) exit instruction
bool ConditionDetection::try_get_exit(ILInstruction* inst, ILInstruction** exit_inst) {
    if (auto* block = as<Block>(inst); block != nullptr && block->instructions.count() > 0) inst = block->instructions.last();

    if (inst->has_flag(InstructionFlags::EndPointUnreachable)) {
        *exit_inst = inst;
        return true;
    }

    *exit_inst = nullptr;
    return false;
}

// Gets the final instruction from a block (or a single instruction) assuming that all blocks
// or instructions in this position have unreachable endpoints
ILInstruction* ConditionDetection::get_exit(ILInstruction* inst) {
    auto* block = as<Block>(inst);
    ILInstruction* exit_inst = block != nullptr ? block->instructions.last() : inst;
    // Last instruction is one with unreachable endpoint
    // (guaranteed by combination of BlockContainer and Block invariants)
    assert(exit_inst->has_flag(InstructionFlags::EndPointUnreachable));
    return exit_inst;
}

// Returns true if inst is Nop or a Block with no instructions.
bool ConditionDetection::is_empty(ILInstruction* inst) {
    if (is<Nop>(inst)) return true;
    auto* block = as<Block>(inst);
    return block != nullptr && block->instructions.count() == 0 && is<Nop>(block->final_instruction());
}

// Import some pattern matching from HighLevelLoopTransform to guess the continue block of loop containers.
// Used to identify branches targetting this block as continue statements, for ordering priority.
bool ConditionDetection::is_continue_block(BlockContainer* container, Block* block) {
    if (container->kind() != ContainerKind::Loop) return false;

#ifdef CB_HAVE_HIGH_LEVEL_LOOP_TRANSFORM
    // increment blocks have exactly 2 incoming edges
    if (container->entry_point()->incoming_edge_count() == 2) {
        Block* for_increment = transforms::HighLevelLoopTransform::get_increment_block(container, container->entry_point());
        if (for_increment != nullptr) return block == for_increment;
    }
#else
    // WAITS: HighLevelLoopTransform (get_increment_block)
#endif

    return block == container->entry_point();
}

// Removes a subrange of instructions from a block and returns them in a new Block
void ConditionDetection::extract_block(Block* block, int start_index, int end_index, Block* extracted_block) {
    for (int i = start_index; i < end_index; i++) {
        ILInstruction* inst = block->instructions[i];
        extracted_block->instructions.add(inst);
        extracted_block->add_il_range(*inst);
    }
    block->instructions.remove_range(start_index, end_index - start_index);
}

}  // namespace cb::dotnet::decompiler::il::control_flow
