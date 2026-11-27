// dotnet/decompiler/il/transforms/remove_infeasible_path_transform.cpp -- see remove_infeasible_path_transform.hpp.
#include "dotnet/decompiler/il/transforms/remove_infeasible_path_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void RemoveInfeasiblePathTransform::run(ILFunction* function, ILTransformContext& context) {
    for (ILInstruction* inst : function->descendants()) {
        auto* container = as<BlockContainer>(inst);
        if (container == nullptr) continue;
        bool changed = false;
        for (Block* block : container->blocks) {
            changed |= do_transform(block, context);
        }

        if (changed) {
            container->sort_blocks(true);
        }
    }
}

bool RemoveInfeasiblePathTransform::do_transform(Block* block, ILTransformContext& context) {
    ILVariable* s;
    int value;
    Branch* br;
    if (!match_block1(block, &s, &value, &br)) return false;
    ILInstruction* exit_inst;
    if (!match_block2(br->target_block(), s, value, &exit_inst)) return false;
    context.step("RemoveInfeasiblePath", br);
    ILInstruction* new_inst = exit_inst->clone();
    br->replace_with(new_inst);
    context.end_step(new_inst);
    s->remove_if_redundant = true;
    return true;
}

bool RemoveInfeasiblePathTransform::match_block1(Block* block, ILVariable** variable, int* constant_value, Branch** branch) {
    *variable = nullptr;
    *constant_value = 0;
    *branch = nullptr;
    if (block->instructions.count() != 2) return false;
    auto* store = as<StLoc>(block->instructions[0]);
    if (store == nullptr || store->variable()->kind() != VariableKind::StackSlot) return false;
    auto* value_inst = as<LdcI4>(store->value());
    if (value_inst == nullptr || !(value_inst->value == 0 || value_inst->value == 1)) return false;
    auto* br = as<Branch>(block->instructions[1]);
    if (br == nullptr) return false;
    *variable = store->variable();
    *constant_value = value_inst->value;
    *branch = br;
    return true;
}

bool RemoveInfeasiblePathTransform::match_block2(Block* block, ILVariable* s, int constant_value, ILInstruction** exit_inst) {
    *exit_inst = nullptr;
    if (block->instructions.count() != 2) return false;
    if (block->incoming_edge_count() <= 1) return false;
    ILInstruction* load;
    ILInstruction* true_inst;
    ILInstruction* false_inst;
    if (!block->match_if_at_end_of_block(&load, &true_inst, &false_inst)) return false;
    if (!load->match_ld_loc(s)) return false;
    *exit_inst = constant_value != 0 ? true_inst : false_inst;
    if (is<Branch>(*exit_inst)) return true;
    auto* leave = as<Leave>(*exit_inst);
    return leave != nullptr && is<Nop>(leave->value());
}

}  // namespace cb::dotnet::decompiler::il::transforms
