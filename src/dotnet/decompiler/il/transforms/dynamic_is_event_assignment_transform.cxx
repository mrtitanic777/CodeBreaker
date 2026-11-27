// dotnet/decompiler/il/transforms/dynamic_is_event_assignment_transform.cxx -- see dynamic_is_event_assignment_transform.hpp.
#include "dotnet/decompiler/il/transforms/dynamic_is_event_assignment_transform.hpp"

#include <cassert>
#include <utility>

#include "dotnet/decompiler/il/transforms/expression_transforms.hpp"

namespace cb::dotnet::decompiler::il::transforms {

// A dynamic 'd.Event += b' lowers to a runtime is-event check that dispatches between the event
// accessor and a compound assignment. That diamond reaches this transform either as an if/else or,
// when the optimizer returns the result, as two leaves. Each shape has its own collapse method.
void DynamicIsEventAssignmentTransform::run(Block* block, int pos, StatementTransformContext& context) {
    if (try_collapse_if_else_diamond(block, pos, context)) return;
    try_collapse_return_form(block, pos, context);
}

bool DynamicIsEventAssignmentTransform::try_collapse_if_else_diamond(Block* block, int pos, StatementTransformContext& context) {
    DynamicIsEventInstruction* is_event;
    ILVariable* flag_var;
    ILVariable* get_member_var;
    DynamicGetMemberInstruction* get_member_inst;
    int diamond_pos;
    ILVariable* value_variable;
    ILInstruction* value;
    if (!match_cache_prefix(block, pos, &is_event, &flag_var, &get_member_var, &get_member_inst, &diamond_pos, &value_variable, &value)) return false;
    // Descendants is a post-order walk including the node itself, so it finds the diamond whether it
    // is the top-level statement or nested inside a call argument or leave value.
    for (ILInstruction* descendant : block->instructions[diamond_pos]->descendants()) {
        DynamicSetMemberInstruction* set_member_inst;
        ILInstruction* get_member_var_use;
        ILInstruction* is_event_condition_use;
        if (!match_is_event_assignment_if_instruction(descendant, is_event, flag_var, get_member_var, &set_member_inst, &get_member_var_use,
                                                      &is_event_condition_use))
            continue;
        context.step("DynamicIsEventAssignmentTransform", block->instructions[pos]);
        // Collapse duplicate condition
        get_member_var_use->replace_with(get_member_inst);
        is_event_condition_use->replace_with(is_event);
        block->instructions.remove_range(pos, 2);
        // Reuse ExpressionTransforms
        ExpressionTransforms::transform_dynamic_set_member_instruction(set_member_inst, context);
        // The match guarantees the collapse; if it ever failed the inlined isevent would leak.
        bool collapsed = ExpressionTransforms::transform_dynamic_add_assign_or_remove_assign(static_cast<IfInstruction*>(descendant), context);
        assert(collapsed && "dynamic is-event assignment matched but could not be collapsed to a compound assignment");
        (void)collapsed;
        inline_copy_of_value(block, pos, value_variable, value);
        context.end_step(block->instructions[pos]);
        // Re-run from the collapsed statement so the following transforms visit the new compound.
        context.request_rerun(pos);
        return true;
    }
    return false;
}

void DynamicIsEventAssignmentTransform::try_collapse_return_form(Block* block, int pos, StatementTransformContext& context) {
    DynamicIsEventInstruction* is_event;
    ILVariable* flag_var;
    ILVariable* get_member_var;
    DynamicGetMemberInstruction* get_member_inst;
    int diamond_pos;
    ILVariable* value_variable;
    ILInstruction* value;
    bool cached = match_cache_prefix(block, pos, &is_event, &flag_var, &get_member_var, &get_member_inst, &diamond_pos, &value_variable, &value);
    DynamicSetMemberInstruction* return_set_member = nullptr;
    ILInstruction* return_get_member_use = nullptr;
    if (!(diamond_pos + 1 < block->instructions.count() &&
          (cached ? match_is_event_return_assignment(block->instructions[diamond_pos], block->instructions[diamond_pos + 1], is_event, flag_var,
                                                     get_member_var, &return_set_member, &return_get_member_use)
                  : match_uncached_is_event_return_assignment(block->instructions[diamond_pos], block->instructions[diamond_pos + 1]))))
        return;
    context.step("DynamicIsEventAssignmentTransform (return form)", block->instructions[pos]);
    // Drop the 'if (isevent) leave add' branch; the compound leave stays and becomes the result.
    block->instructions.remove_at(diamond_pos);
    if (cached) {
        return_get_member_use->replace_with(get_member_inst);
        block->instructions.remove_range(pos, 2);
        ExpressionTransforms::transform_dynamic_set_member_instruction(return_set_member, context);
        inline_copy_of_value(block, pos, value_variable, value);
    }
    context.end_step(block->instructions[pos]);
    context.request_rerun(pos);
}

bool DynamicIsEventAssignmentTransform::match_cache_prefix(Block* block, int pos, DynamicIsEventInstruction** is_event, ILVariable** flag_var,
                                                           ILVariable** get_member_var, DynamicGetMemberInstruction** get_member_inst, int* diamond_pos,
                                                           ILVariable** value_variable, ILInstruction** value) {
    *is_event = nullptr;
    *flag_var = nullptr;
    *get_member_var = nullptr;
    *get_member_inst = nullptr;
    *diamond_pos = pos;
    *value_variable = nullptr;
    *value = nullptr;
    ILInstruction* inst;
    if (!(pos + 3 < block->instructions.count() && block->instructions[pos]->match_st_loc(flag_var, &inst) && is<DynamicIsEventInstruction>(inst)))
        return false;
    *is_event = static_cast<DynamicIsEventInstruction*>(inst);
    if (!((*flag_var)->is_single_definition() && (*flag_var)->load_count() == 2)) return false;
    StLoc* cache_store;
    if (!match_lhs_cache_if_instruction(block->instructions[pos + 1], *flag_var, &cache_store)) return false;
    if (!(cache_store->match_st_loc(get_member_var, &inst) && is<DynamicGetMemberInstruction>(inst))) return false;
    *get_member_inst = static_cast<DynamicGetMemberInstruction*>(inst);
    *diamond_pos = pos + 2;
    // [stloc copyOfValue(value)]: an optional temporary loaded once per diamond branch. The entry
    // guard already ensures pos + 3 is in range.
    bool all_loads_in_dynamic = false;
    if (block->instructions[*diamond_pos]->match_st_loc(value_variable, value) && (*value_variable)->is_single_definition() &&
        (*value_variable)->load_count() == 2) {
        all_loads_in_dynamic = true;
        for (LdLoc* ld : (*value_variable)->load_instructions()) {
            if (!is<DynamicInstruction>(ld->parent())) {
                all_loads_in_dynamic = false;
                break;
            }
        }
    }
    if (all_loads_in_dynamic) {
        (*diamond_pos)++;
    } else {
        *value_variable = nullptr;
        *value = nullptr;
    }
    return true;
}

void DynamicIsEventAssignmentTransform::inline_copy_of_value(Block* block, int pos, ILVariable* value_variable, ILInstruction* value) {
    if (value_variable != nullptr && value_variable->store_count() == 1 && value_variable->address_count() == 0 && value_variable->load_count() == 1 &&
        value != nullptr) {
        value_variable->load_instructions()[0]->replace_with(value);
        block->instructions.remove_at(pos);
    }
}

bool DynamicIsEventAssignmentTransform::match_event_accessor(DynamicInvokeMemberInstruction* add_invoke, ILInstruction* target, ILInstruction* value,
                                                             ExpressionType operation, const std::string& member_name) {
    bool has_accessor = true;
    std::string accessor;
    switch (operation) {
        case ExpressionType::AddAssign: accessor = "add_" + member_name; break;
        case ExpressionType::SubtractAssign: accessor = "remove_" + member_name; break;
        default: has_accessor = false; break;
    }
    return has_accessor && add_invoke->name() == accessor && add_invoke->arguments()[0]->match(target).success() &&
           add_invoke->arguments()[1]->match(value).success();
}

bool DynamicIsEventAssignmentTransform::match_is_event_return_diamond(ILInstruction* if_inst, ILInstruction* leave_inst, ILInstruction** condition,
                                                                      DynamicInvokeMemberInstruction** add_invoke, ILInstruction** compound_value) {
    *condition = nullptr;
    *add_invoke = nullptr;
    *compound_value = nullptr;
    ILInstruction* true_inst;
    if (!if_inst->match_if_instruction(condition, &true_inst)) return false;
    BlockContainer* target;
    ILInstruction* add_value;
    if (!Block::unwrap(true_inst)->match_leave(&target, &add_value)) return false;
    if (!leave_inst->match_leave(target, compound_value)) return false;
    *add_invoke = as<DynamicInvokeMemberInstruction>(add_value);
    return *add_invoke != nullptr && (*add_invoke)->arguments().count() == 2;
}

bool DynamicIsEventAssignmentTransform::match_uncached_is_event_return_assignment(ILInstruction* if_inst, ILInstruction* leave_inst) {
    ILInstruction* condition;
    DynamicInvokeMemberInstruction* add_invoke;
    ILInstruction* compound_value;
    if (!match_is_event_return_diamond(if_inst, leave_inst, &condition, &add_invoke, &compound_value)) return false;
    auto* is_event = as<DynamicIsEventInstruction>(condition);
    if (is_event == nullptr) return false;
    auto* compound = as<DynamicCompoundAssign>(compound_value);
    auto* get_member = compound != nullptr ? as<DynamicGetMemberInstruction>(compound->target()) : nullptr;
    if (get_member == nullptr) return false;
    return is_event->argument()->match(get_member->target()).success() &&
           match_event_accessor(add_invoke, get_member->target(), compound->value(), compound->operation(), get_member->name());
}

bool DynamicIsEventAssignmentTransform::match_is_event_return_assignment(ILInstruction* if_inst, ILInstruction* leave_inst, DynamicIsEventInstruction* is_event,
                                                                         ILVariable* flag_var, ILVariable* get_member_var,
                                                                         DynamicSetMemberInstruction** set_member_inst,
                                                                         ILInstruction** get_member_var_use) {
    *set_member_inst = nullptr;
    *get_member_var_use = nullptr;
    ILInstruction* condition;
    DynamicInvokeMemberInstruction* add_invoke;
    ILInstruction* compound_value;
    return match_is_event_return_diamond(if_inst, leave_inst, &condition, &add_invoke, &compound_value) && condition->match_ld_loc(flag_var) &&
           match_cached_event_compound(compound_value, add_invoke, is_event, get_member_var, set_member_inst, get_member_var_use);
}

bool DynamicIsEventAssignmentTransform::match_cached_event_compound(ILInstruction* compound, DynamicInvokeMemberInstruction* add_invoke,
                                                                    DynamicIsEventInstruction* is_event, ILVariable* get_member_var,
                                                                    DynamicSetMemberInstruction** set_member_inst, ILInstruction** get_member_var_use) {
    *set_member_inst = nullptr;
    *get_member_var_use = nullptr;
    auto* set_member = as<DynamicSetMemberInstruction>(compound);
    if (set_member == nullptr || !is_event->argument()->match(set_member->target()).success()) return false;
    auto* bin_op = as<DynamicBinaryOperatorInstruction>(set_member->value());
    if (!(bin_op != nullptr && bin_op->left()->match_ld_loc(get_member_var))) return false;
    if (!match_event_accessor(add_invoke, set_member->target(), bin_op->right(), bin_op->operation(), set_member->name())) return false;
    *set_member_inst = set_member;
    *get_member_var_use = bin_op->left();
    return true;
}

bool DynamicIsEventAssignmentTransform::match_is_event_assignment_if_instruction(ILInstruction* if_inst, DynamicIsEventInstruction* is_event,
                                                                                 ILVariable* flag_var, ILVariable* get_member_var,
                                                                                 DynamicSetMemberInstruction** set_member_inst,
                                                                                 ILInstruction** get_member_var_use,
                                                                                 ILInstruction** is_event_condition_use) {
    *set_member_inst = nullptr;
    *get_member_var_use = nullptr;
    *is_event_condition_use = nullptr;
    ILInstruction* condition;
    ILInstruction* true_inst;
    ILInstruction* false_inst;
    if (!if_inst->match_if_instruction(&condition, &true_inst, &false_inst)) return false;
    if (match_flag_equals_zero(condition, flag_var)) {
        ILInstruction* ignored;
        condition->match_comp_equals(is_event_condition_use, &ignored);
    } else if (condition->match_ld_loc(flag_var)) {
        std::swap(true_inst, false_inst);
        *is_event_condition_use = condition;
    } else {
        return false;
    }
    // trueInst is now the compound branch, falseInst the add/remove accessor invoke.
    auto* add_invoke = as<DynamicInvokeMemberInstruction>(Block::unwrap(false_inst));
    return add_invoke != nullptr && add_invoke->arguments().count() == 2 &&
           match_cached_event_compound(Block::unwrap(true_inst), add_invoke, is_event, get_member_var, set_member_inst, get_member_var_use);
}

bool DynamicIsEventAssignmentTransform::match_lhs_cache_if_instruction(ILInstruction* if_inst, ILVariable* flag_var, StLoc** cache_store) {
    *cache_store = nullptr;
    ILInstruction* condition;
    ILInstruction* true_inst;
    if (!if_inst->match_if_instruction(&condition, &true_inst)) return false;
    if (!match_flag_equals_zero(condition, flag_var)) return false;
    *cache_store = as<StLoc>(Block::unwrap(true_inst));
    return *cache_store != nullptr;
}

bool DynamicIsEventAssignmentTransform::match_flag_equals_zero(ILInstruction* condition, ILVariable* flag_var) {
    ILInstruction* left;
    ILInstruction* right;
    return condition->match_comp_equals(&left, &right) && left->match_ld_loc(flag_var) && right->match_ldc_i4(0);
}

}  // namespace cb::dotnet::decompiler::il::transforms
