// dotnet/decompiler/il/transforms/detect_catch_when_condition_blocks.cpp -- see detect_catch_when_condition_blocks.hpp.
#include "dotnet/decompiler/il/transforms/detect_catch_when_condition_blocks.hpp"

#include <vector>

#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void DetectCatchWhenConditionBlocks::run(ILFunction* function, ILTransformContext& context) {
    for (ILInstruction* inst : function->descendants()) {
        auto* catch_block = as<TryCatchHandler>(inst);
        if (catch_block == nullptr) continue;
        auto* container = as<BlockContainer>(catch_block->filter());
        if (container == nullptr) continue;
        const IType* exception_type = nullptr;
        ILInstruction* exception_slot = nullptr;
        Block* when_condition_block = nullptr;
        if (!(match_catch_when_entry_point(catch_block->variable(), container, container->entry_point(), &exception_type, &exception_slot,
                                           &when_condition_block) &&
              typesystem::get_stack_type(*exception_type) == catch_block->variable()->stack_type))
            continue;
        // set exceptionType
        catch_block->variable()->set_type(exception_type);
        // Block entryPoint (incoming: 1)  {
        //   stloc temp(isinst exceptionType(ldloc exceptionVar))
        //   if (comp(ldloc temp != ldnull)) br whenConditionBlock
        //   br falseBlock
        // }
        // =>
        // Block entryPoint (incoming: 1)  {
        //   stloc temp(ldloc exceptionSlot)
        //   br whenConditionBlock
        // }
        auto& instructions = container->entry_point()->instructions;
        if (instructions.count() == 3) {
            // stloc temp(isinst exceptionType(ldloc exceptionVar))
            // if (comp(ldloc temp != ldnull)) br whenConditionBlock
            // br falseBlock
            context.step("Detected catch-when for " + catch_block->variable()->name.value_or("") + " (extra store)", instructions[0]);
            static_cast<StLoc*>(instructions[0])->set_value(exception_slot);
            instructions[1]->replace_with(make<Branch>(when_condition_block));
            instructions.remove_at(2);
            container->sort_blocks(true);
        } else if (instructions.count() == 2) {
            // if (comp(isinst exceptionType(ldloc exceptionVar) != ldnull)) br whenConditionBlock
            // br falseBlock
            context.step("Detected catch-when for " + catch_block->variable()->name.value_or(""), instructions[0]);
            auto* branch = make<Branch>(when_condition_block);
            instructions[0]->replace_with(branch);
            instructions.remove_at(1);
            context.end_step(branch);
            container->sort_blocks(true);
        }

        propagate_exception_variable(context, catch_block);
    }
}

// catch E_189 : 0200007C System.Exception when (BlockContainer {
//   Block IL_0079 (incoming: 1) {
//     stloc S_30(ldloc E_189)
//     br IL_0085
//   }
//   Block IL_0085 (incoming: 1) {
//     stloc I_1(ldloc S_30)
// where S_30 and I_1 are single definition
// =>
// copy-propagate E_189 to replace all uses of S_30 and I_1
void DetectCatchWhenConditionBlocks::propagate_exception_variable(ILTransformContext& context, TryCatchHandler* handler) {
    ILVariable* exception_variable = handler->variable();
    if (!exception_variable->is_single_definition()) return;
    context.step_start_group("PropagateExceptionVariable");
    int i = 0;

    auto propagate_exception_instance = [&](StLoc* store) {
        std::vector<LdLoc*> loads = store->variable()->load_instructions();
        for (LdLoc* load : loads) {
            if (!load->is_descendant_of(handler)) continue;
            load->replace_with(with_il_range(make<LdLoc>(exception_variable), load));
        }
        auto* block = as<Block>(store->parent());
        if (store->variable()->load_count() == 0 && block != nullptr) {
            block->instructions.remove_at(store->child_index());
        } else {
            i++;
        }
    };

    while (i < exception_variable->load_count()) {
        LdLoc* load = exception_variable->load_instructions()[static_cast<std::size_t>(i)];

        if (!load->is_descendant_of(handler)) {
            i++;
            continue;
        }

        // We are only interested in store "statements" copying the exception variable without modifying it.
        auto* stloc = as<StLoc>(Block::get_containing_statement(load));
        if (stloc == nullptr) {
            i++;
            continue;
        }
        // simple copy case:
        // stloc b(ldloc a)
        if (stloc->value() == load) {
            propagate_exception_instance(stloc);
        }
        // If the type of the cast-class instruction matches the exceptionType, this cast can be removed without losing any
        // side-effects. (This would also hold if exceptionType were derived from cc.Type, but the pattern is matched exactly.)
        // stloc b(castclass exceptionType(ldloc a))
        else if (auto* cc = as<CastClass>(stloc->value());
                 cc != nullptr && typesystem::type_equals(cc->type(), exception_variable->type()) && cc->argument() == load) {
            stloc->set_value(load);
            propagate_exception_instance(stloc);
        } else {
            i++;
        }
    }
    context.step_end_group(false);
}

// Block entryPoint (incoming: 1)  {
//   stloc temp(isinst exceptionType(ldloc exceptionVar))
//   if (comp(ldloc temp != ldnull)) br whenConditionBlock
//   br falseBlock
// }
bool DetectCatchWhenConditionBlocks::match_catch_when_entry_point(ILVariable* exception_var, BlockContainer* container, Block* entry_point,
                                                                  const IType** exception_type, ILInstruction** exception_slot,
                                                                  Block** when_condition_block) {
    *exception_type = nullptr;
    *exception_slot = nullptr;
    *when_condition_block = nullptr;
    if (entry_point == nullptr || entry_point->incoming_edge_count() != 1) return false;
    if (entry_point->instructions.count() == 3) {
        // stloc temp(isinst exceptionType(ldloc exceptionVar))
        // if (comp(ldloc temp != ldnull)) br whenConditionBlock
        // br falseBlock
        ILVariable* temp = nullptr;
        ILInstruction* isinst = nullptr;
        if (!entry_point->instructions[0]->match_st_loc(&temp, &isinst) || temp->kind() != VariableKind::StackSlot ||
            !isinst->match_is_inst(exception_slot, exception_type))
            return false;
        if (!(*exception_slot)->match_ld_loc(exception_var)) return false;
        ILInstruction* condition = nullptr;
        ILInstruction* branch = nullptr;
        if (!entry_point->instructions[1]->match_if_instruction(&condition, &branch)) return false;
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        if (!condition->match_comp_not_equals(&left, &right)) return false;
        Block* false_block = nullptr;
        ILVariable* return_var = nullptr;
        Block* exit_block = nullptr;
        if (!entry_point->instructions[2]->match_branch(&false_block) || !match_false_block(container, false_block, &return_var, &exit_block))
            return false;
        if ((left->match_ld_null() && right->match_ld_loc(temp)) || (right->match_ld_null() && left->match_ld_loc(temp))) {
            return branch->match_branch(when_condition_block);
        }
    } else if (entry_point->instructions.count() == 2) {
        // if (comp(isinst exceptionType(ldloc exceptionVar) != ldnull)) br whenConditionBlock
        // br falseBlock
        ILInstruction* condition = nullptr;
        ILInstruction* branch = nullptr;
        if (!entry_point->instructions[0]->match_if_instruction(&condition, &branch)) return false;
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        if (!condition->match_comp_not_equals(&left, &right)) return false;
        Block* false_block = nullptr;
        ILVariable* return_var = nullptr;
        Block* exit_block = nullptr;
        if (!entry_point->instructions[1]->match_branch(&false_block) || !match_false_block(container, false_block, &return_var, &exit_block))
            return false;
        if (!left->match_is_inst(exception_slot, exception_type)) return false;
        if (!(*exception_slot)->match_ld_loc(exception_var)) return false;
        if (right->match_ld_null()) {
            return branch->match_branch(when_condition_block);
        }
    }
    return false;
}

// Block falseBlock (incoming: 1)  {
//   stloc returnVar(ldc.i4 0)
//   br exitBlock
// }
bool DetectCatchWhenConditionBlocks::match_false_block(BlockContainer* container, Block* false_block, ILVariable** return_var,
                                                       Block** exit_block) {
    *return_var = nullptr;
    *exit_block = nullptr;
    if (false_block->incoming_edge_count() != 1 || false_block->instructions.count() != 2) return false;
    ILInstruction* zero = nullptr;
    return false_block->instructions[0]->match_st_loc(return_var, &zero) && zero->match_ldc_i4(0) &&
           false_block->instructions[1]->match_branch(exit_block) && match_exit_block(container, *exit_block, *return_var);
}

// Block exitBlock(incoming: 2) {
//   leave container(ldloc returnVar)
// }
bool DetectCatchWhenConditionBlocks::match_exit_block(BlockContainer* container, Block* exit_block, ILVariable* return_var) {
    if (exit_block->incoming_edge_count() != 2 || exit_block->instructions.count() != 1) return false;
    ILInstruction* value = nullptr;
    return exit_block->instructions[0]->match_leave(container, &value) && value->match_ld_loc(return_var);
}

}  // namespace cb::dotnet::decompiler::il::transforms
