// dotnet/decompiler/il/transforms/copy_propagation.cpp -- see copy_propagation.hpp.
#include "dotnet/decompiler/il/transforms/copy_propagation.hpp"

#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/il_extraction.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void CopyPropagation::propagate(StLoc* store, ILTransformContext& context) {
    assert(store->variable()->is_single_definition());
    Block* block = as<Block>(store->parent());
    assert(block != nullptr);
    int i = store->child_index();
    do_propagate(store->variable(), store->value(), block, i, context);
}

void CopyPropagation::run(ILFunction* function, ILTransformContext& context) {
    // The variables that exist several times (equal by the variable comparer): the groups with more than one member.
    std::unordered_map<ILVariable*, int, ILVariableHash, ILVariableEqualityComparer> group_sizes;
    std::vector<ILVariable*> group_keys;  // each group's first variable, in order
    for (ILVariable* v : function->variables()) {
        auto [it, inserted] = group_sizes.try_emplace(v, 0);
        if (inserted) group_keys.push_back(v);
        ++it->second;
    }
    VariableSet split_variables;
    for (ILVariable* key : group_keys) {
        if (group_sizes[key] > 1) split_variables.insert(key);
    }
    for (ILInstruction* inst : function->descendants()) {
        auto* block = as<Block>(inst);
        if (block == nullptr) continue;
        if (block->kind != BlockKind::ControlFlow) continue;
        run_on_block(block, context, &split_variables);
    }
}

void CopyPropagation::run_on_block(Block* block, ILTransformContext& context, const VariableSet* split_variables) {
    for (int i = 0; i < block->instructions.count(); i++) {
        ILVariable* v;
        ILInstruction* copied_expr;
        if (block->instructions[i]->match_st_loc(&v, &copied_expr)) {
            if (v->is_single_definition() && v->load_count() == 0 && v->kind() == VariableKind::StackSlot) {
                // dead store to stack
                if (SemanticHelper::is_pure(copied_expr->flags())) {
                    // no-op -> delete
                    context.step("remove dead store to stack: no-op -> delete", block->instructions[i]);
                    block->instructions.remove_at(i);
                    // This can open up new inlining opportunities:
                    int c = ILInlining::inline_into(block, i, InliningOptions::None, context);
                    i -= c + 1;
                } else {
                    // evaluate the value for its side-effects
                    context.step("remove dead store to stack: evaluate the value for its side-effects", block->instructions[i]);
                    copied_expr->add_il_range(*block->instructions[i]);
                    block->instructions.set(i, copied_expr);
                    context.end_step(copied_expr);
                }
            } else if (v->is_single_definition() && can_perform_copy_propagation(v, copied_expr, split_variables, context.settings())) {
                do_propagate(v, copied_expr, block, i, context);
            }
        }
    }
}

bool CopyPropagation::can_perform_copy_propagation(ILVariable* target, ILInstruction* value, const VariableSet* split_variables,
                                                   const DecompilerSettings& settings) {
    assert(target->stack_type == value->result_type());
    if (typesystem::is_small_integer_type(*target->type())) return false;
    if (split_variables != nullptr && split_variables->contains(target)) {
        return false;  // non-local code move might change semantics when there's split variables
    }
    switch (value->op_code) {
        case OpCode::LdLoca:
        case OpCode::LdsFlda:
            // All address-loading instructions always return the same value for a given operand/argument combination, so they can be
            // safely copied.
            // ... except for LdElema and LdFlda, because those might throw an exception, and we don't want to change the place where
            // the exception is thrown.
            return true;
        case OpCode::LdElema:
        case OpCode::LdFlda: return !settings.use_ref_locals_for_accurate_order_of_evaluation;
        case OpCode::LdLoc: {
            ILVariable* v = static_cast<LdLoc*>(value)->variable();
            if (split_variables != nullptr && split_variables->contains(v)) {
                return false;  // non-local code move might change semantics when there's split variables
            }
            switch (v->kind()) {
                case VariableKind::Parameter:
                    // Parameters can be copied only if they aren't assigned to (directly or indirectly via ldarga)
                    // note: the initialization by the caller is the first store -> StoreCount must be 1
                    return v->is_single_definition();
                default:
                    // Variables can be copied if both are single-definition. To avoid removing too many variables, we do this only if
                    // the target is either a stackslot or a ref local.
                    assert(target->is_single_definition());
                    return v->is_single_definition() && (target->kind() == VariableKind::StackSlot || target->stack_type == StackType::Ref);
            }
        }
        default:
            // All instructions without special behavior that target a stack-variable can be copied.
            return value->flags() == InstructionFlags::None && value->get_child_count() == 0 && target->kind() == VariableKind::StackSlot;
    }
}

void CopyPropagation::do_propagate(ILVariable* v, ILInstruction* copied_expr, Block* block, int& i, ILTransformContext& context) {
    context.step("Copy propagate " + v->name.value_or(""), copied_expr);
    // un-inline the arguments of the ldArg instruction
    std::vector<ILVariable*> uninlined_args(static_cast<std::size_t>(copied_expr->get_child_count()));
    for (std::size_t j = 0; j < uninlined_args.size(); j++) {
        ILInstruction* arg = copied_expr->get_child(static_cast<int>(j));
        const IType* type = find_type_of_stack_type(context.type_system(), arg->result_type());
        uninlined_args[j] = make<ILVariable>(VariableKind::StackSlot, type, arg->result_type());
        uninlined_args[j]->name = "C_" + std::to_string(arg->start_il_offset());
        uninlined_args[j]->has_generated_name = true;
        int insert_at = i++;
        block->instructions.insert(insert_at, make<StLoc>(uninlined_args[j], arg));
    }
    for (ILVariable* uninlined : uninlined_args) v->function()->variables().add(uninlined);
    // perform copy propagation:
    const std::vector<LdLoc*> loads = v->load_instructions();
    for (LdLoc* expr : loads) {
        ILInstruction* clone = copied_expr->clone();
        for (std::size_t j = 0; j < uninlined_args.size(); j++) {
            clone->get_child(static_cast<int>(j))->replace_with(make<LdLoc>(uninlined_args[j]));
        }
        // We are copying an expression from far away, reusing the ILRange would result in incorrect sequence points.
        clone->set_il_range(Interval());
        expr->replace_with(clone);
    }
    block->instructions.remove_at(i);
    int c = ILInlining::inline_into(block, i, InliningOptions::None, context);
    i -= c + 1;
}

}  // namespace cb::dotnet::decompiler::il::transforms
