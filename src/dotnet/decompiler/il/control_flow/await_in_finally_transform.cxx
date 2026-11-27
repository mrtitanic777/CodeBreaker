// dotnet/decompiler/il/control_flow/await_in_finally_transform.cxx -- see await_in_finally_transform.hpp.
#include "dotnet/decompiler/il/control_flow/await_in_finally_transform.hpp"

#include <cassert>
#include <format>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/await_in_catch_transform.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/control_flow/state_range_analysis.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

using transforms::ILTransformContext;
using typesystem::KnownTypeCode;

void AwaitInFinallyTransform::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().await_in_catch_finally) return;
    bool needs_unreachable_code_cleanup = false;

    // analyze all try-catch statements in the function
    std::vector<TryCatch*> try_catches;
    for (ILInstruction* d : function->descendants())
        if (auto* try_catch = as<TryCatch>(d)) try_catches.push_back(try_catch);
    for (TryCatch* try_catch : try_catches) {
        BlockContainer* container = try_catch->parent() != nullptr ? as<BlockContainer>(try_catch->parent()->parent()) : nullptr;
        if (container == nullptr) continue;
        // 	} catch exceptionVariable : 02000078 System.Object when (ldc.i4 1) BlockContainer {
        // 		Block IL_004a (incoming: 1) {
        // 			stloc objectVariable(ldloc exceptionVariable)
        // 			br finallyBlock
        // 		}
        //
        // 	}
        // }
        //
        // Block finallyBlock (incoming: 2) {
        // 	if (comp.o(ldloc b == ldnull)) br afterFinallyBlock
        // 	br finallyBlockContinuation
        // }
        //
        // Block finallyBlockContinuation (incoming: 1) {
        // 	await(addressof System.Threading.Tasks.ValueTask(callvirt DisposeAsync(ldloc b)))
        // 	br afterFinallyBlock
        // }
        //
        // Block afterFinallyBlock (incoming: 2) {
        // 	stloc V_1(ldloc objectVariable)
        // 	if (comp.o(ldloc V_1 == ldnull)) br IL_00ea
        // 	br IL_00cf
        // }

        // await in finally uses a single catch block with catch-type object
        if (try_catch->handlers.count() != 1) {
            continue;
        }
        TryCatchHandler* handler = try_catch->handlers[0];
        ILVariable* exception_variable = handler->variable();
        auto* catch_block_container = as<BlockContainer>(handler->body());
        if (catch_block_container == nullptr) continue;
        if (!typesystem::is_known_type(exception_variable->type(), KnownTypeCode::Object)) continue;
        // Matches the await finally pattern:
        // [stloc V_3(ldloc E_100)	- copy exception variable to a temporary]
        // stloc V_6(ldloc V_3)	- store exception in 'global' object variable
        // br IL_0075				- jump out of catch block to the head of the finallyBlock
        ILVariable* object_variable = nullptr;
        Block* entry_point_of_finally = nullptr;
        if (!match_object_store_catch_handler(catch_block_container, exception_variable, &object_variable, &entry_point_of_finally)) {
            continue;
        }
        // globalCopyVar should only be used once, at the end of the finally-block
        if (object_variable->load_count() != 1 || object_variable->store_count() > 2) continue;

        Block* before_exception_capture_block = Block::find_closest_block(object_variable->load_instructions()[0]);
        if (before_exception_capture_block == nullptr) continue;

        auto [no_throw_block, exception_capture_block, object_variable_copy] = find_block_after_finally(context, before_exception_capture_block, object_variable);
        if (no_throw_block == nullptr || exception_capture_block == nullptr) continue;

        ILInstruction* try_catch_parent = try_catch->parent();
        int previous_index = try_catch->child_index() - 1;
        ILInstruction* previous = previous_index >= 0 && previous_index < try_catch_parent->get_child_count() ? try_catch_parent->get_child(previous_index) : nullptr;
        auto* init_of_state_variable = as<StLoc>(previous);
        if (init_of_state_variable == nullptr || !init_of_state_variable->value()->match_ldc_i4(0)) continue;

        ILVariable* state_variable = init_of_state_variable->variable();
        if (!validate_state_variable(state_variable, init_of_state_variable, try_catch, entry_point_of_finally)) continue;

        StateRangeAnalysis sra(StateRangeAnalysisMode::AwaitInFinally, nullptr, state_variable);
        sra.assign_state_ranges(no_throw_block, util::LongSet::universe());
        util::LongDict<Block*> mapping = sra.get_block_state_set_mapping(static_cast<BlockContainer*>(no_throw_block->parent()));
        util::LongDict<BlockContainer*> mapping_for_leave = sra.get_block_state_set_mapping_for_leave();

        context.step_start_group("Inline finally block with await", try_catch->handlers[0]);
        ControlFlowGraph cfg(container, context.cancellation_token);
        needs_unreachable_code_cleanup = true;

        BlockContainer* finally_container = with_il_range(make<BlockContainer>(), catch_block_container);
        ILInstruction* try_block = try_catch->try_block();
        try_catch->replace_with(with_il_range(make<TryFinally>(try_block, finally_container), try_block));

        context.step("Move blocks into finally", finally_container);
        move_dominated_blocks_to_container(entry_point_of_finally, before_exception_capture_block, cfg, finally_container, context);

        simplify_end_of_finally(context, object_variable, before_exception_capture_block, object_variable_copy, finally_container);

        if (no_throw_block->instructions[0]->match_ld_loc(state_variable)) {
            no_throw_block->instructions.remove_at(0);
        }

        for (ILInstruction* d : try_catch->try_block()->descendants()) {
            auto* branch = as<Branch>(d);
            if (branch == nullptr) continue;
            if (branch->target_block() == entry_point_of_finally) {
                int value = 0;
                auto* block = as<Block>(branch->parent());
                ILInstruction* stored = nullptr;
                if (!(block != nullptr && branch->child_index() > 0 && block->instructions[branch->child_index() - 1]->match_st_loc(state_variable, &stored) &&
                      stored->match_ldc_i4(&value))) {
                    value = 0;
                }
                Block* target_block = nullptr;
                BlockContainer* target_container = nullptr;
                if (mapping.try_get_value(value, &target_block)) {
                    context.step(std::format("branch to finally with state {} => branch to state target ", value) + target_block->label(), branch);
                    branch->set_target_block(target_block);
                } else if (mapping_for_leave.try_get_value(value, &target_container)) {
                    context.step(std::format("branch to finally with state {} => leave to state target ", value) + target_container->to_string(), branch);
                    branch->replace_with(with_il_range(make<Leave>(target_container), branch));
                } else {
                    context.step("branch to finally => branch after finally", branch);
                    branch->set_target_block(no_throw_block);
                }
            } else {
                assert(branch->target_block()->is_descendant_of(try_catch->try_block()));
            }
        }

        context.step_end_group(true);
    }

    context.step("Clean up", function);

    if (needs_unreachable_code_cleanup) {
        // Cleaning up only the modified containers is insufficient, deleting blocks in
        // any container can also cause other blocks in parent containers to become unreachable.
        // So we just clean up everything.
        for (ILInstruction* d : function->body()->descendants()) {
            if (auto* container = as<BlockContainer>(d)) container->sort_blocks(true);
        }
    }
}

void AwaitInFinallyTransform::simplify_end_of_finally(ILTransformContext& context, ILVariable* object_variable, Block* before_exception_capture_block,
                                                      ILVariable* object_variable_copy, BlockContainer* finally_container) {
    ILInstruction* cond = nullptr;
    ILInstruction* br_inst = nullptr;
    ILInstruction* value = nullptr;
    ILInstruction* arg = nullptr;
    int count = before_exception_capture_block->instructions.count();
    if (count >= 3 && before_exception_capture_block->instructions.second_to_last_or_default()->match_if_instruction(&cond, &br_inst) &&
        before_exception_capture_block->instructions.last_or_default() != nullptr && is<Branch>(before_exception_capture_block->instructions.last_or_default()) &&
        before_exception_capture_block->instructions[count - 3]->match_st_loc(object_variable_copy, &value) && value->match_ld_loc(object_variable)) {
        auto* branch = static_cast<Branch*>(before_exception_capture_block->instructions.last_or_default());
        if (cond->match_comp_equals_null(&arg) && arg->match_ld_loc(object_variable_copy)) {
            context.step("Simplify end of finally", before_exception_capture_block);
            before_exception_capture_block->instructions.remove_range(count - 3, 2);
            branch->replace_with(with_il_range(make<Leave>(finally_container), branch));
        } else if (cond->match_comp_not_equals_null(&arg) && arg->match_ld_loc(object_variable_copy)) {
            context.step("Simplify end of finally", before_exception_capture_block);
            before_exception_capture_block->instructions.remove_range(count - 3, 2);
            branch->replace_with(with_il_range(make<Leave>(finally_container), branch));
        } else {
            assert(false && "Broken beforeExceptionCaptureBlock");
        }
    } else {
        assert(false && "Broken beforeExceptionCaptureBlock");
    }
}

bool AwaitInFinallyTransform::validate_state_variable(ILVariable* state_variable, StLoc* initializer, TryCatch* try_catch, Block* entry_point_of_finally) {
    if (state_variable->address_count() > 0) return false;

    for (IStoreInstruction* store_instruction : state_variable->store_instructions()) {
        ILInstruction* store = store_instruction->as_instruction();
        if (store == initializer) continue;
        auto* stloc = as<StLoc>(store);
        if (stloc == nullptr) return false;
        std::int32_t unused_value = 0;
        if (!stloc->value()->match_ldc_i4(&unused_value)) return false;
        if (!stloc->is_descendant_of(try_catch)) return false;
        auto* block = as<Block>(stloc->parent());
        if (block == nullptr) return false;
        ILInstruction* next = block->instructions.element_at_or_default(stloc->child_index() + 1);
        if (!(next != nullptr && next->match_branch(entry_point_of_finally))) return false;
    }

    return true;
}

bool AwaitInFinallyTransform::match_object_store_catch_handler(BlockContainer* catch_block_container, ILVariable* exception_variable,
                                                               ILVariable** object_variable, Block** branch_target) {
    *object_variable = nullptr;
    *branch_target = nullptr;
    Block* entry = catch_block_container->entry_point();
    ILInstruction* value = nullptr;
    switch (entry->instructions.count()) {
        case 2:
            if (!entry->instructions[0]->match_st_loc(object_variable, &value)) return false;
            if (!value->match_ld_loc(exception_variable)) return false;
            break;
        case 3: {
            ILVariable* temporary_variable = nullptr;
            if (!entry->instructions[0]->match_st_loc(&temporary_variable, &value)) return false;
            if (!value->match_ld_loc(exception_variable)) return false;
            if (!entry->instructions[1]->match_st_loc(object_variable, &value)) return false;
            if (!value->match_ld_loc(temporary_variable)) return false;
            break;
        }
        default:
            return false;
    }
    return entry->instructions[entry->instructions.count() - 1]->match_branch(branch_target);
}

void AwaitInFinallyTransform::move_dominated_blocks_to_container(Block* new_entry_point, Block* end_block, ControlFlowGraph& graph,
                                                                 BlockContainer* target_container, ILTransformContext& context) {
    ControlFlowNode* node = graph.get_node(new_entry_point);
    ControlFlowNode* end_node = end_block == nullptr ? nullptr : graph.get_node(end_block);

    move_block(new_entry_point, target_container, context);

    for (ControlFlowNode* n : graph.nodes()) {
        Block* block = n->user_data;

        if (node->dominates(n)) {
            if (end_node != nullptr && end_node != n && end_node->dominates(n)) continue;

            if (block->parent() == target_container) continue;

            move_block(block, target_container, context);
        }
    }
}

void AwaitInFinallyTransform::move_block(Block* block, BlockContainer* target, ILTransformContext& context) {
    context.step("Move " + block->label() + " to container at IL_" + std::format("{:04x}", target->start_il_offset()), target);
    block->remove();
    target->blocks.add(block);
}

std::tuple<Block*, Block*, ILVariable*> AwaitInFinallyTransform::find_block_after_finally(ILTransformContext& context, Block* block, ILVariable* object_variable) {
    // Block IL_0327 (incoming: 2) {
    // 	stloc V_7(ldloc I_0)
    // 	if (comp.o(ldloc V_7 == ldnull)) br IL_034a
    // 	br IL_0333
    // }
    int count = block->instructions.count();
    const std::tuple<Block*, Block*, ILVariable*> none(nullptr, nullptr, nullptr);
    if (count < 3) return none;

    ILVariable* object_variable_copy = nullptr;
    ILInstruction* value = nullptr;
    if (!block->instructions[count - 3]->match_st_loc(&object_variable_copy, &value)) return none;

    if (!value->match_ld_loc(object_variable)) return none;

    ILInstruction* cond = nullptr;
    ILInstruction* no_throw_block_branch = nullptr;
    if (!block->instructions[count - 2]->match_if_instruction(&cond, &no_throw_block_branch)) return none;

    Block* no_throw_block = nullptr;
    if (!no_throw_block_branch->match_branch(&no_throw_block)) return none;

    Block* exception_capture_block = nullptr;
    if (!block->instructions[count - 1]->match_branch(&exception_capture_block)) return none;

    ILInstruction* arg = nullptr;
    if (cond->match_comp_equals_null(&arg)) {
        if (!arg->match_ld_loc(object_variable_copy)) return none;
    } else if (cond->match_comp_not_equals_null(&arg)) {
        if (!arg->match_ld_loc(object_variable_copy)) return none;
        std::swap(no_throw_block, exception_capture_block);
    } else {
        return none;
    }

    StLoc* unused_store = nullptr;
    Block* unused_capture_block = nullptr;
    Block* unused_throw_block = nullptr;
    if (!AwaitInCatchTransform::match_exception_capture_block(context, exception_capture_block, &object_variable_copy, &unused_store, &unused_capture_block,
                                                              &unused_throw_block)) {
        return none;
    }

    return std::make_tuple(no_throw_block, exception_capture_block, object_variable_copy);
}

}  // namespace cb::dotnet::decompiler::il::control_flow
