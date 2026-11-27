// dotnet/decompiler/il/control_flow/control_flow_simplification.cpp -- see control_flow_simplification.hpp.
#include "dotnet/decompiler/il/control_flow/control_flow_simplification.hpp"

#include <cassert>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/il/control_flow/switch_detection.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

namespace {

// The closest block container from `inst` up (inst.Ancestors.OfType<BlockContainer>().First()).
BlockContainer* first_container_ancestor(ILInstruction* inst) {
    for (ILInstruction* ancestor : inst->ancestors()) {
        if (auto* container = as<BlockContainer>(ancestor)) return container;
    }
    throw std::out_of_range("no block container ancestor");
}

}  // namespace

void ControlFlowSimplification::run(ILFunction* function, transforms::ILTransformContext& context) {
    for (ILInstruction* inst : function->descendants()) {
        auto* block = as<Block>(inst);
        if (block == nullptr) continue;
        context.cancellation_token.throw_if_cancellation_requested();

        remove_nop_instructions(block);
        remove_dead_stack_stores(block, context);

        inline_variable_in_return_block(block, context);
        // 1st pass SimplifySwitchInstruction before SimplifyBranchChains() starts duplicating return instructions.
        SwitchDetection::simplify_switch_instruction(block, context);
    }
    simplify_branch_chains(function, context);
    clean_up_empty_blocks(function, context);
}

void ControlFlowSimplification::remove_nop_instructions(Block* block) {
    // Move ILRanges of special nop instructions to the previous non-nop instruction.
    for (int i = block->instructions.count() - 1; i > 0; i--) {
        auto* nop = as<Nop>(block->instructions[i]);
        if (nop != nullptr && nop->kind == NopKind::Pop) {
            block->instructions[i - 1]->add_il_range(*nop);
        }
    }

    // Remove 'nop' instructions
    block->instructions.remove_all([](ILInstruction* inst) { return inst->op_code == OpCode::Nop; });
}

void ControlFlowSimplification::remove_dead_stack_stores(Block* block, transforms::ILTransformContext& context) {
    bool aggressive = context.settings().remove_dead_stores;
    auto is_simple = [](ILInstruction* inst) {
        switch (inst->op_code) {
            case OpCode::LdLoc:
            case OpCode::LdStr:  // C# 1.0 compiler sometimes emits redundant ldstr in switch-on-string pattern
                return true;
            default: return false;
        }
    };
    // Previously copy propagation did this; ideally the ILReader would already do this, for now do this here (even though it's not
    // control-flow related).
    for (int i = block->instructions.count() - 1; i >= 0; i--) {
        auto* stloc = as<StLoc>(block->instructions[i]);
        if (stloc != nullptr && stloc->variable()->is_single_definition() && stloc->variable()->load_count() == 0 &&
            stloc->variable()->kind() == VariableKind::StackSlot) {
            context.step("Remove dead stack store " + stloc->variable()->name.value_or(""), stloc);
            if (aggressive ? SemanticHelper::is_pure(stloc->value()->flags()) : is_simple(stloc->value())) {
                assert(SemanticHelper::is_pure(stloc->value()->flags()));
                block->instructions.remove_at(i++);
            } else {
                stloc->value()->add_il_range(*stloc);
                stloc->replace_with(stloc->value());
            }
        }
    }
}

void ControlFlowSimplification::inline_variable_in_return_block(Block* block, transforms::ILTransformContext& context) {
    // In debug mode, the C# compiler generates 'return blocks' that unnecessarily store the return value to a local and then load it
    // again:
    //   v = <inst>
    //   ret(v)
    // (where 'v' has no other uses)
    // Simplify these to a simple `ret(<inst>)` so that they match the release build version.
    ILInstruction* value;
    if (block->instructions.count() == 2 && block->instructions[1]->match_return(&value)) {
        auto* ret = static_cast<Leave*>(block->instructions[1]);
        ILVariable* v;
        ILInstruction* inst;
        if (value->match_ld_loc(&v) && v->is_single_definition() && v->load_count() == 1 && block->instructions[0]->match_st_loc(v, &inst)) {
            context.step("Inline variable in return block", block);
            inst->add_il_range(*ret->value());
            inst->add_il_range(*block->instructions[0]);
            ret->set_value(inst);
            block->instructions.remove_at(0);
        }
    }
}

void ControlFlowSimplification::simplify_branch_chains(ILFunction* function, transforms::ILTransformContext& context) {
    std::vector<std::pair<Block*, BlockContainer*>> blocks_to_move;
    std::unordered_set<Block*> visited_blocks;
    for (ILInstruction* inst : function->descendants()) {
        auto* branch = as<Branch>(inst);
        if (branch == nullptr) continue;
        // Resolve chained branches to the final target:
        Block* target_block = branch->target_block();
        visited_blocks.clear();
        while (target_block->instructions.count() == 1 && target_block->instructions[0]->op_code == OpCode::Branch) {
            if (!visited_blocks.insert(target_block).second) {
                // prevent infinite loop when branch chain is cyclic
                break;
            }
            context.step("Simplify branch to branch", branch);
            auto* next_branch = static_cast<Branch*>(target_block->instructions[0]);
            branch->set_target_block(next_branch->target_block());
            branch->add_il_range(*next_branch);
            if (target_block->incoming_edge_count() == 0) target_block->instructions.clear();  // mark the block for deletion
            target_block = branch->target_block();
        }
        Leave* leave = target_block->instructions.count() == 1 ? as<Leave>(target_block->instructions[0]) : nullptr;
        if (is_branch_to_return_block(branch)) {
            if (aggressively_duplicate_return_blocks) {
                // Replace branches to 'return blocks' with the return instruction
                context.step("Replace branch to return with return", branch);
                branch->replace_with(target_block->instructions[0]->clone());
            } else if (branch->target_container() != first_container_ancestor(branch) && target_block->incoming_edge_count() == 1) {
                // We don't want to always inline the return directly, because this might force us to place the return within a loop,
                // when it's better placed outside. But we do want to move the return block into the correct try-finally scope, so
                // that loop detection at least has the option to put it inside the loop body.
                context.step("Move return block into try block", branch);
                BlockContainer* local_container = first_container_ancestor(branch);
                blocks_to_move.emplace_back(target_block, local_container);
            }
        } else if (leave != nullptr && leave->value()->match_nop()) {
            context.step("Replace branch to leave with leave", branch);
            // Replace branches to 'leave' instruction with the leave instruction
            auto* leave2 = static_cast<Leave*>(leave->clone());
            if (!branch->il_range_is_empty()) leave2->add_il_range(*branch);  // use the ILRange of the branch if possible
            branch->replace_with(leave2);
        }
        if (target_block->incoming_edge_count() == 0) target_block->instructions.clear();  // mark the block for deletion
    }
    for (const auto& [block, target_container] : blocks_to_move) {
        block->remove();
        target_container->blocks.add(block);
    }
}

void ControlFlowSimplification::clean_up_empty_blocks(ILFunction* function, transforms::ILTransformContext& context) {
    for (ILInstruction* inst : function->descendants()) {
        auto* container = as<BlockContainer>(inst);
        if (container == nullptr) continue;
        for (Block* block : container->blocks) {
            if (block->instructions.count() == 0) continue;  // block is already marked for deletion
            while (combine_block_with_next_block(container, block, context)) {
                // repeat combining blocks until it is no longer possible
                // (this loop terminates because a block is deleted in every iteration)
            }
        }
        // Remove return blocks that are no longer reachable:
        container->blocks.remove_all([](Block* b) { return b->incoming_edge_count() == 0 && b->instructions.count() == 0; });
    }
}

bool ControlFlowSimplification::is_branch_to_return_block(Branch* branch) {
    Block* target_block = branch->target_block();
    if (target_block->instructions.count() != 1) return false;
    ILInstruction* value;
    if (!target_block->instructions[0]->match_return(&value)) return false;
    ILVariable* return_var;
    if (!value->match_ld_loc(&return_var)) return false;
    BlockContainer* container = branch->target_container();
    for (ILInstruction* inst = branch; inst != container; inst = inst->parent()) {
        auto* try_finally = as<TryFinally>(inst->parent());
        if (try_finally != nullptr && inst->slot_info() == &TryInstruction::try_block_slot) {
            // The branch will trigger the finally block. Moving the return block into the try is only possible if the finally block
            // doesn't touch the return variable.
            if (return_var->is_used_within(try_finally->finally_block())) return false;
        }
    }
    return true;
}

bool ControlFlowSimplification::combine_block_with_next_block(BlockContainer* container, Block* block, transforms::ILTransformContext& context) {
    assert(container == block->parent());
    // Ensure the block will stay a basic block -- we don't want extended basic blocks prior to LoopDetection.
    if (block->instructions.count() > 1 && block->instructions[block->instructions.count() - 2]->has_flag(InstructionFlags::MayBranch)) return false;
    auto* br = as<Branch>(block->instructions.last());
    // Check whether we can combine the target block with this block
    if (br == nullptr || br->target_block()->parent() != container || br->target_block()->incoming_edge_count() != 1) return false;
    if (br->target_block() == block) return false;  // don't inline block into itself
    context.step("CombineBlockWithNextBlock", br);
    Block* target_block = br->target_block();
    if (target_block->start_il_offset() < block->start_il_offset() && is_dead_true_store(block)) {
        // The C# compiler generates a dead store for the condition of while (true) loops.
        block->instructions.remove_at(block->instructions.count() - 2);
    }

    if (block->il_range_is_empty()) block->add_il_range(*target_block);

    block->instructions.remove(br);
    block->instructions.add_range(target_block->instructions);
    target_block->instructions.clear();  // mark targetBlock for deletion
    return true;
}

bool ControlFlowSimplification::is_dead_true_store(Block* block) {
    if (block->instructions.count() < 2) return false;
    auto* dead_store = as<StLoc>(block->instructions.second_to_last_or_default());
    if (dead_store == nullptr) return false;
    if (!(dead_store->variable()->load_count() == 0 && dead_store->variable()->address_count() == 0)) return false;
    return dead_store->value()->match_ldc_i4(1) &&
           typesystem::is_known_type(dead_store->variable()->type(), typesystem::KnownTypeCode::Boolean);
}

}  // namespace cb::dotnet::decompiler::il::control_flow
