// dotnet/decompiler/il/transforms/inline_return_transform.cxx -- see inline_return_transform.hpp.
#include "dotnet/decompiler/il/transforms/inline_return_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void InlineReturnTransform::run(ILFunction* function, ILTransformContext&) {
    std::vector<std::tuple<BlockContainer*, Block*, Branch*>> instructions_to_modify;

    // Process all leave instructions in a leave-block, that is a block consisting solely of a leave instruction.
    for (ILInstruction* inst : function->descendants()) {
        auto* leave = as<Leave>(inst);
        if (leave == nullptr) continue;
        auto* leave_block = as<Block>(leave->parent());
        if (!(leave_block != nullptr && leave_block->instructions.count() == 1)) continue;
        // Skip, if the leave instruction has no value or the value is not a load of a local variable.
        ILVariable* return_var;
        if (!leave->value()->match_ld_loc(&return_var) || return_var->kind() != VariableKind::Local) continue;
        // If all instructions can be modified, add item to the global list.
        std::vector<std::tuple<BlockContainer*, Block*, Branch*>> list;
        if (can_modify_instructions(return_var, leave_block, list)) {
            instructions_to_modify.insert(instructions_to_modify.end(), list.begin(), list.end());
        }
    }

    for (const auto& [container, b, br] : instructions_to_modify) {
        Block* block = b;
        // if there is only one branch to this return block, move it to the matching container.
        // otherwise duplicate the return block.
        if (block->incoming_edge_count() == 1) {
            block->remove();
        } else {
            block = static_cast<Block*>(block->clone());
        }
        container->blocks.add(block);
        // adjust the target of the branch to the newly created block.
        br->set_target_block(block);
    }
}

bool InlineReturnTransform::can_modify_instructions(ILVariable* return_var, Block* leave_block,
                                                    std::vector<std::tuple<BlockContainer*, Block*, Branch*>>& instructions_to_modify) {
    instructions_to_modify.clear();
    for (IStoreInstruction* inst : return_var->store_instructions()) {
        auto* store = as<StLoc>(inst->as_instruction());
        if (store == nullptr) return false;
        auto* store_block = as<Block>(store->parent());
        if (store_block == nullptr) return false;
        if (store->child_index() + 2 != store_block->instructions.count()) return false;
        auto* br = as<Branch>(store_block->instructions[store->child_index() + 1]);
        if (br == nullptr) return false;
        if (br->target_block() != leave_block) return false;
        BlockContainer* target_block_container = BlockContainer::find_closest_container(store);
        if (target_block_container == nullptr) return false;
        instructions_to_modify.emplace_back(target_block_container, leave_block, br);
    }

    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms
