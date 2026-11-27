// dotnet/decompiler/il/transforms/pattern_matching_transform.cxx -- see pattern_matching_transform.hpp.
#include "dotnet/decompiler/il/transforms/pattern_matching_transform.hpp"

#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/control_flow/exit_points.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using control_flow::ControlFlowGraph;
using control_flow::DetectExitPoints;
using typesystem::KnownTypeCode;
using typesystem::MethodSemanticsAttributes;
using typesystem::SymbolKind;
using typesystem::TypeKind;

void PatternMatchingTransform::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().pattern_matching) return;
    for (ILInstruction* descendant : function->descendants()) {
        auto* container = as<BlockContainer>(descendant);
        if (container == nullptr) continue;
        GraphPtr cfg;
        // Enumerable.Reverse buffers the blocks before the first one is returned
        std::vector<Block*> blocks(container->blocks.items());
        std::reverse(blocks.begin(), blocks.end());
        for (Block* block : blocks) {
            if (pattern_match_value_types(block, container, context, cfg)) {
                continue;
            }
            if (pattern_match_ref_types(block, container, context, cfg)) {
                continue;
            }
        }
        container->blocks.remove_all([](Block* b) { return b->instructions.count() == 0; });
    }
}

/// Block {
///     ...
///     stloc V(isinst T(testedOperand))
///     if (comp.o(ldloc V == ldnull)) br falseBlock
///     br trueBlock
/// }
///
/// All other uses of V are in blocks dominated by trueBlock.
/// =>
/// Block {
///     ...
///     if (match.type[T].notnull(V = testedOperand)) br trueBlock
///     br falseBlock
/// }
///
/// - or -
///
/// Block {
///     stloc s(isinst T(testedOperand))
///     stloc v(ldloc s)
///     if (logic.not(comp.o(ldloc s != ldnull))) br falseBlock
///     br trueBlock
/// }
/// =>
/// Block {
///     ...
///     if (match.type[T].notnull(V = testedOperand)) br trueBlock
///     br falseBlock
/// }
///
/// All other uses of V are in blocks dominated by trueBlock.
bool PatternMatchingTransform::pattern_match_ref_types(Block* block, BlockContainer* container, ILTransformContext& context, GraphPtr& cfg) {
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!block->match_if_at_end_of_block(&condition, &true_inst, &false_inst)) return false;
    int pos = block->instructions.count() - 3;
    ILVariable* condition_var = nullptr;
    if (condition->match_ld_loc(&condition_var)) {
        // stloc conditionVar(comp.o(ldloc s == ldnull))
        // if (logic.not(ldloc conditionVar)) br trueBlock
        if (pos < 0) return false;
        if (!(condition_var->is_single_definition() && condition_var->load_count() == 1 && condition_var->kind() == VariableKind::StackSlot)) {
            return false;
        }
        if (!block->instructions[pos]->match_st_loc(condition_var, &condition)) return false;
        pos--;
    }
    ILInstruction* load_in_null_check = nullptr;
    if (condition->match_comp_equals_null(&load_in_null_check)) {
        std::swap(true_inst, false_inst);
    } else if (condition->match_comp_not_equals_null(&load_in_null_check)) {
        // do nothing
    } else {
        return false;
    }
    ILVariable* s = nullptr;
    if (!load_in_null_check->match_ld_loc(&s)) return false;
    if (!s->is_single_definition()) return false;
    if (!(s->kind() == VariableKind::Local || s->kind() == VariableKind::StackSlot)) return false;
    if (pos < 0) return false;
    // stloc V(isinst T(testedOperand))
    ILInstruction* store_to_v = block->instructions[pos];
    ILVariable* v = nullptr;
    ILInstruction* value = nullptr;
    if (!store_to_v->match_st_loc(&v, &value)) return false;
    if (value->match_ld_loc(s)) {
        // stloc v(ldloc s)
        pos--;
        if (pos < 0 || !block->instructions[pos]->match_st_loc(s, &value)) return false;
        if (!(v->kind() == VariableKind::Local || v->kind() == VariableKind::StackSlot)) return false;
        if (s->load_count() != 2) return false;
    } else {
        if (v != s) return false;
    }
    const IType* unbox_type = nullptr;
    if (auto* unbox_any = as<UnboxAny>(value)) {
        // stloc S(unbox.any T(isinst T(testedOperand)))
        unbox_type = unbox_any->type();
        value = unbox_any->argument();
    } else {
        unbox_type = nullptr;
    }
    auto* is_inst = as<IsInst>(value);
    if (is_inst == nullptr) return false;
    ILInstruction* tested_operand = is_inst->argument();
    const IType* type = is_inst->type();
    // `type.IsReferenceType != true`
    std::optional<bool> reference_type = type->is_reference_type();
    if (!(reference_type.has_value() && *reference_type)) return false;
    if (!(unbox_type == nullptr || typesystem::type_equals(type, unbox_type))) return false;

    if (!typesystem::type_equals(v->type(), type)) return false;
    if (!check_all_uses_dominated_by(v, container, true_inst, store_to_v, load_in_null_check, context, cfg)) return false;
    context.step("Type pattern matching " + v->name.value_or(""), block);
    //	if (match.type[T].notnull(V = testedOperand)) br trueBlock

    auto* if_inst = as<IfInstruction>(block->instructions.second_to_last_or_default());

    auto* match_inst = make<MatchInstruction>(v, tested_operand);
    match_inst->check_not_null = true;
    match_inst->check_type = true;
    with_il_range(match_inst, if_inst->condition());
    if_inst->set_condition(match_inst);
    if_inst->set_true_inst(true_inst);
    block->instructions.set(block->instructions.count() - 1, false_inst);
    block->instructions.remove_range(pos, if_inst->child_index() - pos);
    v->set_kind(VariableKind::PatternLocal);

    detect_property_sub_patterns(as<MatchInstruction>(if_inst->condition()), true_inst, false_inst, container, context, cfg);

    return true;
}

ILInstruction* PatternMatchingTransform::detect_property_sub_patterns(MatchInstruction* parent_pattern, ILInstruction* true_inst,
                                                                      ILInstruction* parent_false_inst, BlockContainer* container,
                                                                      ILTransformContext& context, GraphPtr& cfg) {
    if (!context.settings().recursive_pattern_matching) {
        return true_inst;
    }
    while (true) {
        Block* true_block = as<Block>(true_inst);
        if (!(true_block != nullptr || true_inst->match_branch(&true_block))) {
            break;
        }
        if (!(true_block->incoming_edge_count() == 1 && true_block->parent() == container)) {
            break;
        }
        ILInstruction* next_true_inst = detect_property_sub_pattern(parent_pattern, true_block, parent_false_inst, context, cfg);
        if (next_true_inst != nullptr) {
            true_inst = next_true_inst;
        } else {
            break;
        }
    }
    return true_inst;
}

ILInstruction* PatternMatchingTransform::detect_property_sub_pattern(MatchInstruction* parent_pattern, Block* block, ILInstruction* parent_false_inst,
                                                                     ILTransformContext& context, GraphPtr& cfg) {
    // if (match.notnull.type[System.String] (V_0 = callvirt get_C(ldloc V_2))) br IL_0022
    // br IL_0037
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    ILVariable* target_variable = nullptr;
    ILInstruction* operand = nullptr;
    if (match_block_containing_one_condition(block, &condition, &true_inst, &false_inst)) {
        bool negate = false;
        if (!DetectExitPoints::compatible_exit_instruction(parent_false_inst, false_inst)) {
            if (!DetectExitPoints::compatible_exit_instruction(parent_false_inst, true_inst)) {
                return nullptr;
            }
            std::swap(true_inst, false_inst);
            negate = true;
        }
        ILInstruction* pattern_operand = nullptr;
        ILInstruction* target = nullptr;
        const IMember* member = nullptr;
        if (MatchInstruction::is_pattern_match(condition, &pattern_operand, &context.settings())) {
            if (!property_or_field_access(pattern_operand, &target, &member)) {
                return nullptr;
            }
            if (!target->match_ld_loc_ref(parent_pattern->variable())) {
                return nullptr;
            }
            if (negate && !context.settings().pattern_combinators) {
                return nullptr;
            }
            context.step("Move property sub pattern", condition);
            if (negate) {
                condition = Comp::logic_not(condition);
            }
            parent_pattern->sub_patterns().add(condition);
            context.end_step(condition);
        } else if (property_or_field_access(condition, &target, &member)) {
            if (!target->match_ld_loc_ref(parent_pattern->variable())) {
                return nullptr;
            }
            if (!negate && !context.settings().pattern_combinators) {
                return nullptr;
            }
            context.step("Sub pattern: implicit != 0", condition);
            auto* sub_pattern = make<Comp>(negate ? ComparisonKind::Equality : ComparisonKind::Inequality, typesystem::Sign::None, condition,
                                           make<LdcI4>(0));
            parent_pattern->sub_patterns().add(sub_pattern);
            context.end_step(sub_pattern);
        } else {
            return nullptr;
        }
        block->instructions.clear();
        block->instructions.add(true_inst);
        return true_inst;
    } else if (block->instructions[0]->match_st_loc(&target_variable, &operand)) {
        ILInstruction* target = nullptr;
        const IMember* member = nullptr;
        if (!property_or_field_access(operand, &target, &member)) {
            return nullptr;
        }
        if (!target->match_ld_loc_ref(parent_pattern->variable())) {
            return nullptr;
        }
        if (!typesystem::type_equals(target_variable->type(), member->return_type())) {
            return nullptr;
        }
        if (!check_all_uses_dominated_by(target_variable, as<BlockContainer>(block->parent()), block, block->instructions[0], nullptr, context, cfg)) {
            return nullptr;
        }
        context.step("Property var pattern", block);
        auto* var_pattern = make<MatchInstruction>(target_variable, operand);
        with_il_range(var_pattern, block->instructions[0]);
        parent_pattern->sub_patterns().add(var_pattern);
        context.end_step(var_pattern);
        block->instructions.remove_at(0);
        target_variable->set_kind(VariableKind::PatternLocal);

        if (typesystem::is_known_type(target_variable->type(), KnownTypeCode::NullableOfT)) {
            ILInstruction* result = match_nullable_has_value_check_pattern(block, var_pattern, parent_false_inst, context, cfg);
            return result != nullptr ? result : block;
        }

        ILInstruction* instruction_after_null_check = match_null_check_pattern(block, var_pattern, parent_false_inst, context);
        std::optional<bool> reference_type = target_variable->type()->is_reference_type();
        if (instruction_after_null_check != nullptr) {
            return detect_property_sub_patterns(var_pattern, instruction_after_null_check, parent_false_inst, as<BlockContainer>(block->parent()), context,
                                                cfg);
        } else if (reference_type.has_value() && !*reference_type) {
            return detect_property_sub_patterns(var_pattern, block, parent_false_inst, as<BlockContainer>(block->parent()), context, cfg);
        } else {
            return block;
        }
    } else {
        return nullptr;
    }
}

ILInstruction* PatternMatchingTransform::match_null_check_pattern(Block* block, MatchInstruction* var_pattern, ILInstruction* parent_false_inst,
                                                                  ILTransformContext& context) {
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!match_block_containing_one_condition(block, &condition, &true_inst, &false_inst)) {
        return nullptr;
    }
    ILInstruction* arg = nullptr;
    if (condition->match_comp_equals_null(&arg) && arg->match_ld_loc(var_pattern->variable())) {
        std::swap(true_inst, false_inst);
    } else if (condition->match_comp_not_equals_null(&arg) && arg->match_ld_loc(var_pattern->variable())) {
    } else {
        return nullptr;
    }
    if (!DetectExitPoints::compatible_exit_instruction(false_inst, parent_false_inst)) {
        return nullptr;
    }
    context.step("Null check pattern", block);
    var_pattern->check_not_null = true;
    block->instructions.clear();
    block->instructions.add(true_inst);
    return true_inst;
}

ILInstruction* PatternMatchingTransform::match_nullable_has_value_check_pattern(Block* block, MatchInstruction* var_pattern,
                                                                                ILInstruction* parent_false_inst, ILTransformContext& context,
                                                                                GraphPtr& cfg) {
    if (!(var_pattern->variable()->store_count() == 1 && var_pattern->variable()->load_count() == 0)) {
        return nullptr;
    }
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!match_block_containing_one_condition(block, &condition, &true_inst, &false_inst)) {
        return nullptr;
    }
    if (!NullableLiftingTransform::match_has_value_call(condition, var_pattern->variable())) {
        return nullptr;
    }
    if (!DetectExitPoints::compatible_exit_instruction(false_inst, parent_false_inst)) {
        if (DetectExitPoints::compatible_exit_instruction(true_inst, parent_false_inst)) {
            if (!(var_pattern->variable()->address_count() == 1)) {
                return nullptr;
            }

            context.step("Nullable.HasValue check -> null pattern", block);
            auto* null_comp = make<Comp>(ComparisonKind::Equality, ComparisonLiftingKind::CSharp, StackType::O, typesystem::Sign::None,
                                         var_pattern->tested_operand(), make<LdNull>());
            var_pattern->replace_with(null_comp);
            context.end_step(null_comp);
            block->instructions.clear();
            block->instructions.add(false_inst);
            return false_inst;
        }
        return nullptr;
    }
    if (var_pattern->variable()->address_count() == 1 && context.settings().pattern_combinators) {
        context.step("Nullable.HasValue check -> not null pattern", block);
        auto* not_null_comp = make<Comp>(ComparisonKind::Inequality, ComparisonLiftingKind::CSharp, StackType::O, typesystem::Sign::None,
                                         var_pattern->tested_operand(), make<LdNull>());
        var_pattern->replace_with(not_null_comp);
        context.end_step(not_null_comp);
        block->instructions.clear();
        block->instructions.add(true_inst);
        return true_inst;
    } else if (var_pattern->variable()->address_count() != 2) {
        return nullptr;
    }
    Block* true_block = nullptr;
    if (!(true_inst->match_branch(&true_block) && true_block->parent() == block->parent() && true_block->incoming_edge_count() == 1)) {
        return nullptr;
    }
    ILVariable* new_target_variable = nullptr;
    ILInstruction* get_value_or_default_call = nullptr;
    if (true_block->instructions[0]->match_st_loc(&new_target_variable, &get_value_or_default_call) &&
        NullableLiftingTransform::match_get_value_or_default(get_value_or_default_call, var_pattern->variable())) {
        context.step("Nullable.HasValue check + Nullable.GetValueOrDefault pattern", block);
        var_pattern->check_not_null = true;
        var_pattern->set_variable(new_target_variable);
        new_target_variable->set_kind(VariableKind::PatternLocal);
        block->instructions.clear();
        block->instructions.add(true_inst);
        true_block->instructions.remove_at(0);
        return detect_property_sub_patterns(var_pattern, true_block, parent_false_inst, as<BlockContainer>(block->parent()), context, cfg);
    } else if (match_block_containing_one_condition(true_block, &condition, &true_inst, &false_inst)) {
        auto* comp = as<Comp>(condition);
        if (!(comp != nullptr && MatchInstruction::is_constant(comp->right()) &&
              NullableLiftingTransform::match_get_value_or_default(comp->left(), var_pattern->variable()))) {
            return nullptr;
        }
        if (!(context.settings().relational_patterns || comp->kind() == ComparisonKind::Equality || comp->kind() == ComparisonKind::Inequality)) {
            return nullptr;
        }
        bool negated = false;
        if (!DetectExitPoints::compatible_exit_instruction(false_inst, parent_false_inst)) {
            if (!DetectExitPoints::compatible_exit_instruction(true_inst, parent_false_inst)) {
                return nullptr;
            }
            std::swap(true_inst, false_inst);
            negated = true;
        }
        if (comp->kind() == (negated ? ComparisonKind::Equality : ComparisonKind::Inequality)) {
            return nullptr;
        }
        if (negated && !context.settings().pattern_combinators) {
            return nullptr;
        }
        context.step("Nullable.HasValue check + Nullable.GetValueOrDefault pattern", block);
        // varPattern: match (v = testedOperand)
        // comp: comp.i4(call GetValueOrDefault(ldloca v) != ldc.i4 42)
        // =>
        // comp.i4.lifted(testedOperand != ldc.i4 42)
        block->instructions.clear();
        block->instructions.add(true_inst);
        true_block->instructions.clear();
        comp->set_left(var_pattern->tested_operand());
        comp->lifting_kind = ComparisonLiftingKind::CSharp;
        if (negated) {
            comp = Comp::logic_not(comp);
        }
        var_pattern->replace_with(comp);
        context.end_step(comp);
        return true_inst;
    } else {
        return nullptr;
    }
}

bool PatternMatchingTransform::property_or_field_access(ILInstruction* operand, ILInstruction** target, const IMember** member) {
    auto* call = as<CallInstruction>(operand);
    if (call != nullptr && call->method->symbol_kind() == SymbolKind::Accessor &&
        call->method->accessor_kind() == MethodSemanticsAttributes::Getter && call->method->accessor_owner() != nullptr &&
        call->arguments().count() == 1) {
        *target = call->arguments()[0];
        *member = call->method->accessor_owner();
        return true;
    }
    const IField* field = nullptr;
    if (operand->match_ld_fld(target, &field)) {
        *member = field;
        return true;
    }
    *member = nullptr;
    return false;
}

bool PatternMatchingTransform::match_block_containing_one_condition(Block* block, ILInstruction** condition, ILInstruction** true_inst,
                                                                    ILInstruction** false_inst) {
    switch (block->instructions.count()) {
    case 2:
        return block->match_if_at_end_of_block(condition, true_inst, false_inst);
    case 3: {
        *condition = nullptr;
        ILInstruction* load_temp = nullptr;
        if (!block->match_if_at_end_of_block(&load_temp, true_inst, false_inst)) return false;
        ILVariable* temp_var = nullptr;
        if (!(load_temp->match_ld_loc(&temp_var) && temp_var->is_single_definition() && temp_var->load_count() == 1)) return false;
        if (!block->instructions[0]->match_st_loc(temp_var, condition)) return false;
        ILInstruction* arg = nullptr;
        while ((*condition)->match_logic_not(&arg)) {
            *condition = arg;
            std::swap(*true_inst, *false_inst);
        }
        return true;
    }
    default:
        *condition = nullptr;
        *true_inst = nullptr;
        *false_inst = nullptr;
        return false;
    }
}

bool PatternMatchingTransform::check_all_uses_dominated_by(ILVariable* v, BlockContainer* container, ILInstruction* true_inst, ILInstruction* store_to_v,
                                                           ILInstruction* load_in_null_check, ILTransformContext& context, GraphPtr& cfg) {
    Block* target_block = as<Block>(true_inst);
    if (target_block == nullptr && !true_inst->match_branch(&target_block)) {
        return false;
    }

    if (target_block->parent() != container) return false;
    if (target_block->incoming_edge_count() != 1) return false;
    if (cfg == nullptr) cfg = std::make_unique<ControlFlowGraph>(container, context.cancellation_token);
    auto* target_block_node = cfg->get_node(target_block);
    std::vector<ILInstruction*> uses;
    for (LdLoc* ld : v->load_instructions()) uses.push_back(ld);
    for (LdLoca* la : v->address_instructions()) uses.push_back(la);
    for (IStoreInstruction* st : v->store_instructions()) uses.push_back(st->as_instruction());
    for (ILInstruction* use : uses) {
        if (use == store_to_v || use == load_in_null_check) continue;
        Block* found = nullptr;
        for (ILInstruction* current = use; current != nullptr; current = current->parent()) {
            if (current->parent() == container) {
                found = as<Block>(current);
                break;
            }
        }
        if (found == nullptr) return false;
        auto* node = cfg->get_node(found);
        if (!target_block_node->dominates(node)) return false;
    }
    return true;
}

/// Block {
///     ...
///     [stloc temp(ldloc testedOperand)]
///     if (comp.o(isinst T(ldloc testedOperand) == ldnull)) br falseBlock
///     br unboxBlock
/// }
///
/// Block unboxBlock (incoming: 1) {
///     stloc V(unbox.any T(ldloc temp))
///     ...
/// }
/// =>
/// Block {
///     ...
///     if (match.type[T].notnull(V = testedOperand)) br unboxBlock
///     br falseBlock
/// }
bool PatternMatchingTransform::pattern_match_value_types(Block* block, BlockContainer* container, ILTransformContext& context, GraphPtr& cfg) {
    const IType* type = nullptr;
    ILInstruction* tested_operand = nullptr;
    ILVariable* tested_variable = nullptr;
    const IType* box_type1 = nullptr;
    Block* unbox_block = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!match_is_inst_block(block, &type, &tested_operand, &tested_variable, &box_type1, &unbox_block, &false_inst)) {
        return false;
    }
    StLoc* temp_store = as<StLoc>(block->instructions.element_at_or_default(block->instructions.count() - 3));
    if (temp_store == nullptr || !temp_store->value()->match_ld_loc(tested_variable)) {
        temp_store = nullptr;
    }
    ILVariable* unbox_operand = nullptr;
    const IType* box_type2 = nullptr;
    StLoc* store_to_v = nullptr;
    if (!match_unbox_block(unbox_block, type, &unbox_operand, &box_type2, &store_to_v)) {
        return false;
    }
    if (!typesystem::type_equals(box_type1, box_type2)) {
        return false;
    }
    if (unbox_operand == tested_variable) {
        // do nothing
    } else if (temp_store != nullptr && unbox_operand == temp_store->variable()) {
        if (!(temp_store->variable()->is_single_definition() && temp_store->variable()->load_count() == 1)) return false;
    } else {
        return false;
    }
    if (!check_all_uses_dominated_by(store_to_v->variable(), container, unbox_block, store_to_v, nullptr, context, cfg)) return false;
    context.step("PatternMatching with " + store_to_v->variable()->name.value_or(""), block);
    auto* if_inst = as<IfInstruction>(block->instructions.second_to_last_or_default());
    auto* match_inst = make<MatchInstruction>(store_to_v->variable(), tested_operand);
    match_inst->check_not_null = true;
    match_inst->check_type = true;
    if_inst->set_condition(match_inst);
    if_inst->set_true_inst(make<Branch>(unbox_block));
    block->instructions.set(block->instructions.count() - 1, false_inst);
    unbox_block->instructions.remove_at(0);
    if (temp_store != nullptr && unbox_operand == temp_store->variable()) {
        block->instructions.remove(temp_store);
    }
    // HACK: condition detection uses StartILOffset of blocks to decide which branch of if-else
    // should become the then-branch. Change the unboxBlock StartILOffset from an offset inside
    // the pattern matching machinery to an offset belonging to an instruction in the then-block.
    unbox_block->set_il_range(*unbox_block->instructions[0]);
    store_to_v->variable()->set_kind(VariableKind::PatternLocal);
    detect_property_sub_patterns(as<MatchInstruction>(if_inst->condition()), unbox_block, false_inst, container, context, cfg);
    return true;
}

///	...
/// if (comp.o(isinst T(ldloc testedOperand) == ldnull)) br falseBlock
/// br unboxBlock
/// - or -
/// ...
/// if (comp.o(isinst T(box ``0(ldloc testedOperand)) == ldnull)) br falseBlock
/// br unboxBlock
bool PatternMatchingTransform::match_is_inst_block(Block* block, const IType** type, ILInstruction** tested_operand, ILVariable** tested_variable,
                                                   const IType** box_type, Block** unbox_block, ILInstruction** false_inst) {
    *type = nullptr;
    *tested_operand = nullptr;
    *tested_variable = nullptr;
    *box_type = nullptr;
    *unbox_block = nullptr;
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!block->match_if_at_end_of_block(&condition, &true_inst, false_inst)) {
        return false;
    }
    ILInstruction* arg = nullptr;
    if (condition->match_comp_equals_null(&arg)) {
        std::swap(true_inst, *false_inst);
    } else if (condition->match_comp_not_equals_null(&arg)) {
        // do nothing
    } else {
        return false;
    }
    if (!arg->match_is_inst(tested_operand, type)) {
        return false;
    }
    ILInstruction* box_arg = nullptr;
    if (!((*tested_operand)->match_box(&box_arg, box_type) && (*box_type)->kind() == TypeKind::TypeParameter)) {
        box_arg = *tested_operand;
    }
    if (!box_arg->match_ld_loc(tested_variable)) {
        return false;
    }
    return true_inst->match_branch(unbox_block) && (*unbox_block)->parent() == block->parent();
}

/// Block unboxBlock (incoming: 1) {
///     stloc V(unbox.any T(ldloc testedOperand))
///     ...
///     - or -
///     stloc V(unbox.any T(isinst T(box ``0(ldloc testedOperand))))
///     ...
/// }
bool PatternMatchingTransform::match_unbox_block(Block* unbox_block, const IType* type, ILVariable** tested_variable, const IType** box_type,
                                                 StLoc** store_to_v) {
    *box_type = nullptr;
    *store_to_v = nullptr;
    *tested_variable = nullptr;
    if (unbox_block->incoming_edge_count() != 1) return false;
    *store_to_v = as<StLoc>(unbox_block->instructions[0]);
    if (*store_to_v == nullptr) return false;
    ILInstruction* value = (*store_to_v)->value();
    ILInstruction* arg = nullptr;
    const IType* t = nullptr;
    if (!(value->match_unbox_any(&arg, &t) && typesystem::type_equals(t, type))) return false;
    ILInstruction* isinst_arg = nullptr;
    const IType* isinst_type = nullptr;
    if (arg->match_is_inst(&isinst_arg, &isinst_type) && typesystem::type_equals(isinst_type, type)) {
        arg = isinst_arg;
    }
    ILInstruction* box_arg = nullptr;
    if (arg->match_box(&box_arg, box_type) && (*box_type)->kind() == TypeKind::TypeParameter) {
        arg = box_arg;
    }
    if (!arg->match_ld_loc(tested_variable)) {
        return false;
    }
    if (*box_type != nullptr && !typesystem::type_equals(*box_type, (*tested_variable)->type())) {
        return false;
    }
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms
