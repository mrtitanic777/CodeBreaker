// dotnet/decompiler/il/control_flow/runtime_async_manual_await_transform.cxx -- see runtime_async_manual_await_transform.hpp.
#include "dotnet/decompiler/il/control_flow/runtime_async_manual_await_transform.hpp"

#include <vector>

#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/early_expression_transforms.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

using transforms::ILTransformContext;

void RuntimeAsyncManualAwaitTransform::run(ILFunction* function, ILTransformContext& context) {
    bool changed = false;
    std::vector<Block*> blocks;
    for (ILInstruction* d : function->descendants())
        if (auto* block = as<Block>(d)) blocks.push_back(block);
    for (Block* block : blocks) {
        if (detect_runtime_async_manual_await(block, context)) changed = true;
    }
    if (changed) {
        for (ILInstruction* d : function->body()->descendants()) {
            if (auto* c = as<BlockContainer>(d)) c->sort_blocks(true);
        }
    }
}

bool RuntimeAsyncManualAwaitTransform::detect_runtime_async_manual_await(Block* block, ILTransformContext& context) {
    int count = block->instructions.count();
    if (count < 3) return false;

    ILVariable* awaiter_var = nullptr;
    ILInstruction* awaiter_value = nullptr;
    if (!block->instructions[count - 3]->match_st_loc(&awaiter_var, &awaiter_value)) return false;
    auto* get_awaiter_call = as<CallInstruction>(awaiter_value);
    if (get_awaiter_call == nullptr) return false;
    if (get_awaiter_call->method->name() != "GetAwaiter" || get_awaiter_call->arguments().count() != 1) return false;

    auto* if_inst = as<IfInstruction>(block->instructions[count - 2]);
    if (if_inst == nullptr) return false;
    ILInstruction* condition = if_inst->condition();
    ILInstruction* negated = nullptr;
    bool is_negated = condition->match_logic_not(&negated);
    if (is_negated) condition = negated;
    auto* is_completed_call = as<CallInstruction>(condition);
    if (is_completed_call == nullptr) return false;
    if (is_completed_call->method->name() != "get_IsCompleted" || is_completed_call->arguments().count() != 1) return false;
    if (!is_completed_call->arguments()[0]->match_ld_loca(awaiter_var)) return false;

    Block* completed_block = nullptr;
    Block* pause_block = nullptr;
    Block* true_branch_target = nullptr;
    Block* false_branch_target = nullptr;
    if (if_inst->true_inst()->match_branch(&true_branch_target) && block->instructions[count - 1]->match_branch(&false_branch_target)) {
        if (is_negated) {
            // if (!IsCompleted) br pauseBlock; br completedBlock — flipped
            pause_block = true_branch_target;
            completed_block = false_branch_target;
        } else {
            // if (IsCompleted) br completedBlock; br pauseBlock — canonical
            completed_block = true_branch_target;
            pause_block = false_branch_target;
        }
    } else {
        return false;
    }

    // Block pauseBlock { call AsyncHelpers.UnsafeAwaitAwaiter(ldloc awaiter); br completedBlock }
    if (pause_block->instructions.count() != 2) return false;
    auto* pause_call = as<Call>(pause_block->instructions[0]);
    if (pause_call == nullptr) return false;
    if (!transforms::EarlyExpressionTransforms::is_async_helpers_method(pause_call->method, "UnsafeAwaitAwaiter") || pause_call->arguments().count() != 1)
        return false;
    if (!pause_call->arguments()[0]->match_ld_loc(awaiter_var)) return false;
    Block* pause_tail = nullptr;
    if (!pause_block->instructions[1]->match_branch(&pause_tail) || pause_tail != completed_block) return false;

    // completedBlock starts with: GetResult(ldloca awaiter), possibly inlined.
    if (completed_block->instructions.count() < 1) return false;
    CallInstruction* get_result_call = transforms::ILInlining::find_first_inlined_call(completed_block->instructions[0]);
    if (get_result_call == nullptr) return false;
    if (get_result_call->method->name() != "GetResult" || get_result_call->arguments().count() != 1) return false;
    if (!get_result_call->arguments()[0]->match_ld_loca(awaiter_var)) return false;

    // Determine the awaitable expression: the original GetAwaiter argument (often `ldloca tmp`)
    // or, if `tmp` is a fresh temporary set immediately above, the value it was set to.
    ILInstruction* awaitable = nullptr;
    bool remove_temporary_store = false;
    ILVariable* tmp_var = nullptr;
    ILInstruction* tmp_value = nullptr;
    if (get_awaiter_call->arguments()[0]->match_ld_loca(&tmp_var) && block->instructions.count() >= 4 &&
        block->instructions[count - 4]->match_st_loc(tmp_var, &tmp_value) && tmp_var->store_count() == 1 && tmp_var->load_count() == 0 &&
        tmp_var->address_count() == 1) {
        awaitable = tmp_value;
        remove_temporary_store = true;
    } else {
        awaitable = get_awaiter_call->arguments()[0];
    }

    context.step("Reduce manual await pattern to Await", block);

    // Capture IL ranges before we remove the suspend machinery. The new Await stands in
    // for the whole pattern (temp store + GetAwaiter + IsCompleted check + UnsafeAwaitAwaiter
    // + GetResult), and the new `br completedBlock` replaces the original `br pauseBlock`.
    ILInstruction* old_tail_branch = block->instructions[count - 1];
    Await* await_inst = with_il_range(make<Await>(awaitable->clone()), get_result_call);
    if (remove_temporary_store) await_inst->add_il_range(*block->instructions[count - 4]);
    await_inst->add_il_range(*block->instructions[count - 3]);
    await_inst->add_il_range(*block->instructions[count - 2]);
    for (ILInstruction* inst : pause_block->instructions) await_inst->add_il_range(*inst);
    await_inst->get_awaiter_method = get_awaiter_call->method;
    await_inst->get_result_method = get_result_call->method;

    // Remove the trailing 3 (or 4) instructions of the head block; replace with `br completedBlock`.
    int remove_count = remove_temporary_store ? 4 : 3;
    block->instructions.remove_range(block->instructions.count() - remove_count, remove_count);
    block->instructions.add(with_il_range(make<Branch>(completed_block), old_tail_branch));

    get_result_call->replace_with(await_inst);

    return true;
}

}  // namespace cb::dotnet::decompiler::il::control_flow
