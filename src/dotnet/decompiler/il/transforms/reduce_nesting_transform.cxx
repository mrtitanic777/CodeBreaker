// dotnet/decompiler/il/transforms/reduce_nesting_transform.cxx -- see reduce_nesting_transform.hpp.
#include "dotnet/decompiler/il/transforms/reduce_nesting_transform.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

#include "core/error.hpp"
#include "dotnet/decompiler/il/control_flow/condition_detection.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/patterns.hpp"
#include "dotnet/decompiler/util/tree_traversal.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using control_flow::ConditionDetection;
using control_flow::ControlFlowGraph;

void ReduceNestingTransform::run(ILFunction* function, ILTransformContext& context) {
    context_ = &context;
    visit(static_cast<BlockContainer*>(function->body()), nullptr);

    for (ILInstruction* node : function->descendants()) {
        if (auto* try_finally = as<TryFinally>(node)) eliminate_redundant_try_finally(try_finally, context);
    }
}

void ReduceNestingTransform::visit(BlockContainer* container, Block* continue_target) {
    switch (container->kind()) {
    case ContainerKind::Loop:
    case ContainerKind::While: continue_target = container->entry_point(); break;
    case ContainerKind::DoWhile:
    case ContainerKind::For: continue_target = container->blocks.last(); break;
    default: break;
    }

    for (int i = 0; i < container->blocks.count(); i++) {
        Block* block = container->blocks[i];
        // Note: it's possible for additional blocks to be appended to the container
        // by the Visit() call; but there should be no other changes to the Blocks collection.
        visit(block, continue_target);
        assert(container->blocks[i] == block);
    }
}

void ReduceNestingTransform::visit(Block* block, Block* continue_target, ILInstruction* next_instruction) {
    assert(block->has_flag(InstructionFlags::EndPointUnreachable) || next_instruction != nullptr);

    // process each instruction in the block.
    for (int i = 0; i < block->instructions.count(); i++) {
        //  Transformations may be applied to the current and following instructions but already processed instructions will not be changed
        ILInstruction* inst = block->instructions[i];

        // the next instruction to be executed. Transformations will change the next instruction, so this is a method instead of a variable
        auto next_insn = [&]() -> ILInstruction* {
            ILInstruction* next = block->instructions.element_at_or_default(i + 1);
            return next != nullptr ? next : next_instruction;
        };

        if (auto* container = as<BlockContainer>(inst)) {
            // visit the contents of the container
            visit(container, continue_target);

            // reduce nesting in switch blocks
            ILInstruction* keyword_exit1 = nullptr;
            if (container->kind() == ContainerKind::Switch && can_duplicate_exit(next_insn(), continue_target, &keyword_exit1) &&
                reduce_switch_nesting(block, container, keyword_exit1)) {
                remove_redundant_exit(block, next_instruction);
            }
        } else if (auto* if_inst = as<IfInstruction>(inst)) {
            improve_il_ordering(block, if_inst, continue_target);

            // reduce nesting in if/else blocks
            ILInstruction* keyword_exit2 = nullptr;
            if (can_duplicate_exit(next_insn(), continue_target, &keyword_exit2) && reduce_nesting(block, if_inst, keyword_exit2))
                remove_redundant_exit(block, next_instruction);

            // visit content blocks
            if (auto* true_block = as<Block>(if_inst->true_inst())) visit(true_block, continue_target, next_insn());

            if (auto* false_block = as<Block>(if_inst->false_inst())) {
                if (if_inst->true_inst()->has_flag(InstructionFlags::EndPointUnreachable)) {
                    extract_else_block(if_inst);
                    continue;
                }

                visit(false_block, continue_target, next_insn());
            }
        } else {
            // blocks can only exit containers via leave instructions, not fallthrough, so the only relevant context is `continueTarget`
            visit_containers(inst, continue_target);

            // reducing nesting inside Try/Using/Lock etc, may make the endpoint unreachable.
            // This should only happen by replacing a Leave with the exit instruction we're about to delete, but I can't see a good way to assert this
            // This would be better placed in ReduceNesting, but it's more difficult to find the affected instructions/blocks there than here
            if (i == block->instructions.count() - 2 && inst->has_flag(InstructionFlags::EndPointUnreachable)) {
                context_->step("Remove unreachable exit", block->instructions.last());
                block->instructions.remove_at(block->instructions.count() - 1);

                // This would be the right place to check and fix the redundant continue; in TestCases.Pretty.ReduceNesting.BreakLockInLoop
                // but doing so would require knowledge of what `inst` is, and how it works. (eg. to target the try block and not catch or finally blocks)
            }
        }
    }
}

// search for child containers to reduce nesting in
void ReduceNestingTransform::visit_containers(ILInstruction* inst, Block* continue_target) {
    if (is<ILFunction>(inst)) {
        return;  // assume inline ILFunctions are already transformed
    }
    if (auto* cont = as<BlockContainer>(inst)) {
        visit(cont, continue_target);
        return;
    }
    for (ILInstruction* child : inst->children()) visit_containers(child, continue_target);
}

/// For an if statement with an unreachable end point and no else block,
/// inverts to match IL order of the first statement of each branch
void ReduceNestingTransform::improve_il_ordering(Block* block, IfInstruction* if_inst, Block* continue_target) {
    if (!block->has_flag(InstructionFlags::EndPointUnreachable) || !if_inst->true_inst()->has_flag(InstructionFlags::EndPointUnreachable) ||
        !if_inst->false_inst()->match_nop())
        return;

    assert(if_inst != block->instructions.last());

    bool true_range_is_empty = false;
    bool false_range_is_empty = false;
    int true_range_start = ConditionDetection::get_start_il_offset(if_inst->true_inst(), &true_range_is_empty);
    int false_range_start =
        ConditionDetection::get_start_il_offset(block->instructions[block->instructions.index_of(if_inst) + 1], &false_range_is_empty);
    if (true_range_is_empty || false_range_is_empty || false_range_start >= true_range_start) return;

    auto* leave = as<Leave>(block->instructions.last());
    if (leave != nullptr && !leave->is_leaving_function() && leave->target_container()->kind() == ContainerKind::Normal) {
        // non-keyword leave. Can't move out of the last position in the block (fall-through) without introducing goto, unless it can be replaced with a keyword (return/continue)
        ILInstruction* keyword_exit = nullptr;
        if (!can_duplicate_exit(block->instructions.last(), continue_target, &keyword_exit)) return;

        context_->step("Replace leave with keyword exit", if_inst->true_inst());
        ILInstruction* keyword_exit_clone = keyword_exit->clone();
        block->instructions.last()->replace_with(keyword_exit_clone);
        context_->end_step(keyword_exit_clone);
    }

    ConditionDetection::invert_if(block, if_inst, *context_);
}

/// Reduce Nesting in if/else statements by duplicating an exit instruction.
/// Does not affect IL order
bool ReduceNestingTransform::reduce_nesting(Block* block, IfInstruction* if_inst, ILInstruction* exit_inst) {
    // start tallying stats for heuristics from then and else-if blocks
    int max_statements = 0, max_depth = 0;
    update_stats(if_inst->true_inst(), max_statements, max_depth);

    // if (cond) { ... } exit;
    if (if_inst->false_inst()->match_nop()) {
        // a separate heuristic to ShouldReduceNesting as there is visual balancing to be performed based on number of statments
        if (max_depth < 2) return false;

        //   ->
        // if (!cond) exit;
        // ...; exit;
        ensure_end_point_unreachable(block, exit_inst);
        assert(if_inst == block->instructions.second_to_last_or_default());

        // use the same exit the block has. If the block already has one (such as a leave from a try), keep it in place
        ensure_end_point_unreachable(if_inst->true_inst(), block->instructions.last());
        ConditionDetection::invert_if(block, if_inst, *context_);

        // ensure the exit inst of the if instruction is a keyword
        assert(!is<Block>(if_inst->true_inst()));
        if (!if_inst->true_inst()->match(exit_inst).success()) {
            assert(is<Leave>(if_inst->true_inst()));
            context_->step("Replace leave with keyword exit", if_inst->true_inst());
            ILInstruction* exit_clone = exit_inst->clone();
            if_inst->true_inst()->replace_with(exit_clone);
            context_->end_step(exit_clone);
        }
        return true;
    }

    // else-if trees are considered as a single group from the root IfInstruction
    if (get_else_if_parent(if_inst) != nullptr) return false;

    // find the else block and tally stats for each else-if block
    while (auto* else_if_inst = as<IfInstruction>(Block::unwrap(if_inst->false_inst()))) {
        update_stats(else_if_inst->true_inst(), max_statements, max_depth);
        if_inst = else_if_inst;
    }

    // A chain with no trailing else has no block to reduce: ifInst.FalseInst is a bare Nop.
    // Guarding here keeps the else-block cast in ExtractElseBlock safe (#3891).
    auto* false_block = as<Block>(if_inst->false_inst());
    if (false_block == nullptr || !should_reduce_nesting(false_block, max_statements, max_depth)) return false;

    // extract the else block and insert exit points all the way up the else-if tree
    do {
        IfInstruction* else_if_inst = get_else_if_parent(if_inst);

        // if (cond) { ... } else { ... } exit;
        //   ->
        // if (cond) { ...; exit; }
        // ...; exit;
        ensure_end_point_unreachable(if_inst->true_inst(), exit_inst);
        if (if_inst->false_inst()->has_flag(InstructionFlags::EndPointUnreachable)) {
            assert(if_inst->has_flag(InstructionFlags::EndPointUnreachable));
            assert(if_inst->parent() == block);
            int remove_after = if_inst->child_index() + 1;
            if (remove_after < block->instructions.count()) {
                // Remove all instructions that ended up dead
                // (this should just be exitInst itself)
                assert(block->instructions.second_to_last_or_default() == if_inst);
                assert(block->instructions.last() == exit_inst);
                block->instructions.remove_range(remove_after, block->instructions.count() - remove_after);
            }
        }
        extract_else_block(if_inst);
        if_inst = else_if_inst;
    } while (if_inst != nullptr);

    return true;
}

/// Reduce Nesting in switch statements by replacing break; in cases with the block exit, and extracting the default case
/// Does not affect IL order
bool ReduceNestingTransform::reduce_switch_nesting(Block* parent_block, BlockContainer* switch_container, ILInstruction* exit_inst) {
    // break; from outer container cannot be brought inside the switch as the meaning would change
    if (auto* leave = as<Leave>(exit_inst); leave != nullptr && !leave->is_leaving_function()) return false;

    // find the default section, and ensure it has only one incoming edge
    if (switch_container->entry_point()->instructions.count() != 1) throw UsageError("Sequence does not contain exactly one element");
    auto* switch_inst = static_cast<SwitchInstruction*>(switch_container->entry_point()->instructions[0]);
    // sections.MaxBy(s => s.Labels.Count()): the first section with the most labels
    SwitchSection* default_section = nullptr;
    std::uint64_t max_labels = 0;
    for (SwitchSection* s : switch_inst->sections) {
        std::uint64_t labels = s->labels().count();
        if (default_section == nullptr || labels > max_labels) {
            default_section = s;
            max_labels = labels;
        }
    }
    Block* default_block = nullptr;
    if (!default_section->body()->match_branch(&default_block) || default_block->incoming_edge_count() != 1) return false;
    if (default_block->parent() != switch_container) return false;

    // tally stats for heuristic from each case block
    int max_statements = 0, max_depth = 0;
    for (SwitchSection* section : switch_inst->sections) {
        Block* case_block = nullptr;
        if (section != default_section && section->body()->match_branch(&case_block) && case_block->parent() == switch_container)
            update_stats(case_block, max_statements, max_depth);
    }

    if (!should_reduce_nesting(default_block, max_statements, max_depth)) return false;

    assert(default_block->has_flag(InstructionFlags::EndPointUnreachable));

    // ensure the default case dominator tree has no exits (branches to other cases)
    ControlFlowGraph cfg(switch_container, context_->cancellation_token);
    flow_analysis::ControlFlowNode* default_node = cfg.get_node(default_block);
    std::vector<flow_analysis::ControlFlowNode*> default_tree;
    util::pre_order(
        default_node, [](flow_analysis::ControlFlowNode* n) { return n->dominator_tree_children; },
        [&](flow_analysis::ControlFlowNode* n) { default_tree.push_back(n); });
    for (flow_analysis::ControlFlowNode* n : default_tree) {
        for (flow_analysis::ControlFlowNode* successor : n->successors) {
            if (!default_node->dominates(successor)) return false;
        }
    }

    if (default_tree.size() > 1 && !is<BlockContainer>(parent_block->parent())) return false;

    context_->step("Extract default case of switch", switch_container);

    // if the switch container is followed by an instruction, it must be a Leave from a try/pinned/etc or exitInst
    // When it's a leave from a container, it's better to let the extracted default block 'fall through' rather than duplicating whatever
    // instruction eventually follows the container
    if (parent_block->instructions.second_to_last_or_default() == switch_container) {
        if (default_block->instructions.last()->match_leave(static_cast<const BlockContainer*>(switch_container)))
            default_block->instructions.last()->replace_with(parent_block->instructions.last());

        parent_block->instructions.remove_at(parent_block->instructions.count() - 1);
    }

    // replace all break; statements with the exitInst
    std::vector<ILInstruction*> leave_instructions;
    for (ILInstruction* inst : switch_container->descendants()) {
        if (inst->match_leave(static_cast<const BlockContainer*>(switch_container))) leave_instructions.push_back(inst);
    }
    for (ILInstruction* leave_inst : leave_instructions) leave_inst->replace_with(exit_inst->clone());

    // replace the default section branch with a break;
    default_section->body()->replace_with(make<Leave>(switch_container));

    // remove all default blocks from the switch container
    std::vector<Block*> default_blocks;
    for (flow_analysis::ControlFlowNode* c : default_tree) default_blocks.push_back(c->user_data);
    for (Block* block : default_blocks) switch_container->blocks.remove(block);

    assert(parent_block->instructions.last() == switch_container);
    parent_block->instructions.add_range(std::vector<ILInstruction*>(default_block->instructions.begin(), default_block->instructions.end()));

    // add any additional blocks from the default case to the parent container
    assert(default_blocks[0] == default_block);
    if (default_blocks.size() > 1) {
        auto* parent_container = static_cast<BlockContainer*>(parent_block->parent());
        int insert_at = parent_container->blocks.index_of(parent_block) + 1;
        for (std::size_t i = 1; i < default_blocks.size(); i++) parent_container->blocks.insert(insert_at++, default_blocks[i]);
    }

    return true;
}

/// Checks if an exit is a duplicable keyword exit (return; break; continue;)
bool ReduceNestingTransform::can_duplicate_exit(ILInstruction* exit, Block* continue_target, ILInstruction** keyword_exit) {
    *keyword_exit = exit;
    if (exit != nullptr && exit->match_branch(static_cast<const Block*>(continue_target))) return true;  // keyword is continue

    auto* leave = as<Leave>(exit);
    if (!(leave != nullptr && leave->value()->match_nop())) return false;  // don't duplicate valued returns

    if (leave->is_leaving_function() || leave->target_container()->kind() != ContainerKind::Normal) return true;  // keyword is return || break

    // leave from a try/pinned/lock etc, check if the target (the instruction following the target container) is duplicable, if so, set keywordExit to that
    ILInstruction* leaving_inst = leave->target_container();
    assert(!leaving_inst->has_flag(InstructionFlags::EndPointUnreachable));
    while (true) {
        auto* b = as<Block>(leaving_inst->parent());
        if (!(b == nullptr || leaving_inst == b->instructions.last())) break;
        if (auto* try_finally = as<TryFinally>(leaving_inst->parent())) {
            if (leaving_inst->slot_info() == &TryFinally::finally_block_slot) {
                // cannot duplicate leaves from finally containers
                assert(leave->target_container() == try_finally->finally_block());  // finally cannot have control flow
                return false;
            }
            if (try_finally->has_flag(InstructionFlags::EndPointUnreachable)) {
                // finally block changes return value/throws an exception? Yikes. Lets leave it alone
                assert(try_finally->finally_block()->has_flag(InstructionFlags::EndPointUnreachable));
                return false;
            }
        } else if (auto* try_fault = as<TryFault>(leaving_inst->parent());
                   try_fault != nullptr && leaving_inst->slot_info() == &TryFault::fault_block_slot) {
            // cannot duplicate leaves from fault containers either
            assert(leave->target_container() == try_fault->fault_block());
            return false;
        }

        leaving_inst = leaving_inst->parent();
        assert(!leaving_inst->has_flag(InstructionFlags::EndPointUnreachable));
        assert(!is<ILFunction>(leaving_inst));
    }

    auto* block = static_cast<Block*>(leaving_inst->parent());
    ILInstruction* target_inst = block->instructions[block->instructions.index_of(leaving_inst) + 1];
    return can_duplicate_exit(target_inst, continue_target, keyword_exit);
}

/// Ensures the end point of a block is unreachable by duplicating and appending the [exit] instruction following the end point
void ReduceNestingTransform::ensure_end_point_unreachable(ILInstruction* inst, ILInstruction* fallthrough_exit) {
    auto* block = as<Block>(inst);
    if (block == nullptr) {
        assert(inst->has_flag(InstructionFlags::EndPointUnreachable));
        return;
    }

    if (!block->has_flag(InstructionFlags::EndPointUnreachable)) {
        context_->step("Duplicate block exit", fallthrough_exit);
        ILInstruction* exit_clone = fallthrough_exit->clone();
        block->instructions.add(exit_clone);
        context_->end_step(exit_clone);
    }
}

/// Removes a redundant block exit instruction.
void ReduceNestingTransform::remove_redundant_exit(Block* block, ILInstruction* implicit_exit) {
    if (block->instructions.last()->match(implicit_exit).success()) {
        context_->step("Remove redundant exit", block->instructions.last());
        block->instructions.remove_at(block->instructions.count() - 1);
    }
}

/// Determines if an IfInstruction is an else-if and returns the preceeding (parent) IfInstruction
///
/// [else-]if (parent-cond) else { ifInst }
IfInstruction* ReduceNestingTransform::get_else_if_parent(IfInstruction* if_inst) {
    assert(is<Block>(if_inst->parent()));
    if (Block::unwrap(if_inst->parent()) == if_inst) {  // only instruction in block
        // parent of block is an IfInstruction
        auto* else_if_inst = if_inst->parent()->parent() != nullptr ? as<IfInstruction>(if_inst->parent()->parent()) : nullptr;
        if (else_if_inst != nullptr && else_if_inst->false_inst() == if_inst->parent())  // part of the false branch not the true branch
            return else_if_inst;
    }

    return nullptr;
}

/// Adds a code path to the current heuristic tally
void ReduceNestingTransform::update_stats(ILInstruction* inst, int& max_statements, int& max_depth) {
    int num_statements = 0;
    compute_stats(inst, num_statements, max_depth, 0);
    max_statements = std::max(num_statements, max_statements);
}

/// Recursively computes the number of statements and maximum nested depth of an instruction
void ReduceNestingTransform::compute_stats(ILInstruction* inst, int& num_statements, int& max_depth, int current_depth, bool is_statement) {
    if (is_statement) num_statements++;

    if (current_depth > max_depth) {
        assert(is_statement);
        max_depth = current_depth;
    }

    // enumerate children statements and containers
    if (auto* block = as<Block>(inst)) {
        if (is_statement) num_statements--;  // don't count blocks as statements

        // add each child as a statement (unless we're a named block)
        for (ILInstruction* child : block->instructions)
            compute_stats(child, num_statements, max_depth, current_depth, block->kind != BlockKind::CallWithNamedArgs && block->kind != BlockKind::CallInlineAssign);

        // final instruction as an expression
        compute_stats(block->final_instruction(), num_statements, max_depth, current_depth, false);
    } else if (auto* container = as<BlockContainer>(inst)) {
        if (!is_statement) num_statements++;  // always add a statement for a container in an expression

        Block* container_body = container->entry_point();
        if (container->kind() == ContainerKind::For || container->kind() == ContainerKind::While) {
            assert(is_statement);

            ILInstruction* ignored_condition = nullptr;
            if (!container->match_condition_block(container->entry_point(), &ignored_condition, &container_body))
                throw UsageError("Invalid condition block in loop.");
        }

        // don't count implicit leave. Can't avoid counting for loop initializers but close enough, for loops can have an extra statement of visual weight
        ILInstruction* last_inst = container_body->instructions.last();
        if (((container->kind() == ContainerKind::For || container->kind() == ContainerKind::DoWhile) &&
             last_inst->match_branch(static_cast<const Block*>(container->blocks.last()))) ||
            ((container->kind() == ContainerKind::Loop || container->kind() == ContainerKind::While) &&
             last_inst->match_branch(static_cast<const Block*>(container->blocks[0]))) ||
            (container->kind() == ContainerKind::Normal && last_inst->match_leave(static_cast<const BlockContainer*>(container))) ||
            container->kind() == ContainerKind::Switch)  // SwitchInstructyion always counts as a statement anyway, so no need to count the container as well
            num_statements--;

        // add the nested body
        compute_stats(container_body, num_statements, max_depth, current_depth + 1);
    } else if (auto* if_inst = as<IfInstruction>(inst); if_inst != nullptr && if_inst->result_type() == StackType::Void) {
        assert(is_statement);
        // nested then instruction
        compute_stats(if_inst->true_inst(), num_statements, max_depth, current_depth + 1);

        // include all nested else-if instructions at the same depth
        ILInstruction* else_inst = if_inst->false_inst();
        while (auto* else_if_inst = as<IfInstruction>(Block::unwrap(else_inst))) {
            num_statements++;
            compute_stats(else_if_inst->true_inst(), num_statements, max_depth, current_depth + 1);
            else_inst = else_if_inst->false_inst();
        }

        // include all nested else instruction
        compute_stats(else_inst, num_statements, max_depth, current_depth + 1);
    } else if (auto* section = as<SwitchSection>(inst)) {
        assert(!is_statement);  // labels are just children of the SwitchInstruction
        num_statements++;       // add a statement for each case label

        // add all the case blocks at the current depth
        // most formatters indent switch blocks twice, but we don't want this heuristic to be based on formatting
        // so we remain conservative and only include the increase in depth from the container and not the labels
        Block* case_block = nullptr;
        if (section->body()->match_branch(&case_block) && case_block->parent() == section->parent()->parent()->parent())
            compute_stats(case_block, num_statements, max_depth, current_depth);
    } else if (auto* func = as<ILFunction>(inst)) {
        int body_statements = 0;
        int body_max_depth = max_depth;
        compute_stats(func->body(), body_statements, body_max_depth, current_depth);
        if (body_statements >= 2) {  // don't count inline functions
            num_statements += body_statements;
            max_depth = body_max_depth;
        }
    } else {
        // search each child instruction. Containers will contain statements and contribute to stats
        int sub_statements = 0;
        for (ILInstruction* child : inst->children()) compute_stats(child, sub_statements, max_depth, current_depth, false);

        num_statements += sub_statements;
        if (is_statement && sub_statements > 0) num_statements--;  // don't count the first container, only its contents, because this statement is already counted
    }
}

/// Heuristic to determine whether it is worth duplicating exits into the preceeding sibling blocks (then/else-if/case)
/// in order to reduce the nesting of block by 1
bool ReduceNestingTransform::should_reduce_nesting(Block* block, int max_statements, [[maybe_unused]] int max_depth) {
    int max_statements2 = 0, max_depth2 = 0;
    update_stats(block, max_statements2, max_depth2);
    // if the max depth is 2, always reduce nesting (total depth 3 or more)
    // if the max depth is 1, reduce nesting if this block is the largest
    // otherwise reduce nesting only if this block is twice as large as any other
    return max_depth2 >= 2 || (max_depth2 >= 1 && max_statements2 > max_statements) || max_statements2 >= 2 * max_statements;
}

/// if (cond) { ...; exit; } else { ... }
/// ...;
///   ->
/// if (cond) { ...; exit; }
/// ...;
/// ...;
void ReduceNestingTransform::extract_else_block(IfInstruction* if_inst) {
    assert(if_inst->true_inst()->has_flag(InstructionFlags::EndPointUnreachable));
    auto* block = static_cast<Block*>(if_inst->parent());
    auto* false_block = static_cast<Block*>(if_inst->false_inst());

    context_->step("Extract else block", if_inst);
    int insert_at = block->instructions.index_of(if_inst) + 1;
    for (int i = 0; i < false_block->instructions.count(); i++) block->instructions.insert(insert_at++, false_block->instructions[i]);

    if_inst->set_false_inst(make<Nop>());
}

void ReduceNestingTransform::eliminate_redundant_try_finally(TryFinally* try_finally, ILTransformContext& context) {
    /* The C# compiler sometimes generates try-finally structures for fixed statements.
        After our transforms runs, these are redundant and can be removed.
        .try BlockContainer {
            Block IL_001a (incoming: 1) {
                PinnedRegion ...
            }
        } finally BlockContainer {
            Block IL_003e (incoming: 1) {
                leave IL_003e (nop)
            }
        }
        ==> PinnedRegion
    */
    auto* finally_container = as<BlockContainer>(try_finally->finally_block());
    if (finally_container == nullptr) return;
    if (!finally_container->single_instruction()->match_leave(static_cast<const BlockContainer*>(finally_container))) return;
    // Finally is empty and redundant. But we'll delete the block only if there's a PinnedRegion.
    auto* try_container = as<BlockContainer>(try_finally->try_block());
    if (try_container == nullptr) return;
    if (try_container->blocks.count() != 1) return;
    Block* try_block = try_container->blocks[0];
    if (try_block->instructions.count() == 1) {
        if (auto* pinned_region = as<PinnedRegion>(try_block->instructions[0])) {
            context.step("Removing try-finally around PinnedRegion", pinned_region);
            try_finally->replace_with(pinned_region);
        }
    } else if (try_block->instructions.count() == 2) {
        auto* pinned_region = as<PinnedRegion>(try_block->instructions[0]);
        if (pinned_region != nullptr && try_block->instructions[1]->match_leave(static_cast<const BlockContainer*>(try_container))) {
            context.step("Removing try-finally around PinnedRegion", pinned_region);
            try_finally->replace_with(pinned_region);
        }
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
