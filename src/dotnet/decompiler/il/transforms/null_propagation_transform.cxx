// dotnet/decompiler/il/transforms/null_propagation_transform.cxx -- see null_propagation_transform.hpp.
#include "dotnet/decompiler/il/transforms/null_propagation_transform.hpp"

#include <cassert>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::MethodSemanticsAttributes;
using typesystem::TypeKind;
namespace nullable_type = typesystem::nullable_type;

namespace {

// context.CSharpResolver.CanTransformToExtensionMethodCall(method).
bool can_transform_to_extension_method_call(const IMethod*, ILTransformContext&) {
    // WAITS: CSharpResolver (C# resolver; an extension method call never takes part in an access chain until it is ported)
    return false;
}

}  // namespace

bool NullPropagationTransform::is_protected_if_inst(IfInstruction* if_inst) {
    // We exclude logic.and to avoid turning
    // "logic.and(comp(interfaces != ldnull), call get_Count(interfaces))"
    // into "if ((interfaces?.Count ?? 0) != 0)".
    if (if_inst == nullptr) return false;
    ILInstruction* lhs = nullptr;
    ILInstruction* rhs = nullptr;
    return (if_inst->match_logic_and(&lhs, &rhs) || if_inst->match_logic_or(&lhs, &rhs)) && IfInstruction::is_in_condition_slot(if_inst);
}

const char* NullPropagationTransform::mode_name(Mode mode) {
    switch (mode) {
        case Mode::ReferenceType: return "ReferenceType";
        case Mode::NullableByValue: return "NullableByValue";
        case Mode::NullableByReference: return "NullableByReference";
        case Mode::UnconstrainedType: return "UnconstrainedType";
    }
    return "";
}

ILInstruction* NullPropagationTransform::run(ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst) {
    assert(context_->settings().null_propagation);
#ifndef NDEBUG
    {
        ILInstruction* ignored = nullptr;
        assert(!condition->match_logic_not(&ignored));  // Caller should pass in positive condition
    }
#endif
    ILVariable* tested_var = nullptr;
    ILInstruction* load_inst = nullptr;
    auto* comp = as<Comp>(condition);
    if (comp != nullptr && comp->left()->match_ld_loc(&tested_var) && comp->right()->match_ld_null()) {
        if (comp->lifting_kind != ComparisonLiftingKind::None) return nullptr;
        if (comp->kind() == ComparisonKind::Equality) {
            // testedVar == null ? trueInst : falseInst
            return try_null_propagation(tested_var, false_inst, true_inst, Mode::ReferenceType);
        } else if (comp->kind() == ComparisonKind::Inequality) {
            return try_null_propagation(tested_var, true_inst, false_inst, Mode::ReferenceType);
        }
    } else if (NullableLiftingTransform::match_has_value_call(condition, &load_inst)) {
        // loadInst.HasValue ? trueInst : falseInst
        if (load_inst->match_ld_loca(&tested_var)) {
            return try_null_propagation(tested_var, true_inst, false_inst, Mode::NullableByValue);
        } else if (load_inst->match_ld_loc(&tested_var)) {
            return try_null_propagation(tested_var, true_inst, false_inst, Mode::NullableByReference);
        }
    }
    return nullptr;
}

ILInstruction* NullPropagationTransform::try_null_propagation(ILVariable* tested_var, ILInstruction* non_null_inst, ILInstruction* null_inst,
                                                              Mode mode) {
    bool removed_rewrap_or_nullable_ctor = false;
    const IType* ignored_type = nullptr;
    ILInstruction* arg = nullptr;
    if (NullableLiftingTransform::match_nullable_ctor(non_null_inst, &ignored_type, &arg)) {
        non_null_inst = arg;
        removed_rewrap_or_nullable_ctor = true;
    } else if (non_null_inst->match_nullable_rewrap(&arg)) {
        non_null_inst = arg;
        removed_rewrap_or_nullable_ctor = true;
    }
    ILInstruction* var_load = nullptr;
    if (!is_valid_access_chain(tested_var, mode, non_null_inst, &var_load)) return nullptr;
    // note: InferType will be accurate in this case because the access chain consists of calls and field accesses
    const IType* return_type = infer_type(non_null_inst, &context_->type_system());
    const IType* null_inst_type = nullptr;
    if (null_inst->match_ld_null()) {
        context_->step(std::string("Null propagation (mode=") + mode_name(mode) + ", output=reference type)", non_null_inst);
        // testedVar != null ? testedVar.AccessChain : null
        // => testedVar?.AccessChain
        introduce_unwrap(tested_var, var_load, mode);
        auto* result = make<NullableRewrap>(non_null_inst);
        context_->end_step(result);
        return result;
    } else if (null_inst->match_default_value(&null_inst_type) && typesystem::is_known_type(null_inst_type, KnownTypeCode::NullableOfT)) {
        context_->step(std::string("Null propagation (mode=") + mode_name(mode) + ", output=value type)", non_null_inst);
        // testedVar != null ? testedVar.AccessChain : default(T?)
        // => testedVar?.AccessChain
        introduce_unwrap(tested_var, var_load, mode);
        auto* result = make<NullableRewrap>(non_null_inst);
        context_->end_step(result);
        return result;
    } else if (!removed_rewrap_or_nullable_ctor && nullable_type::is_non_nullable_value_type(return_type) && !return_type->is_by_ref_like()) {
        context_->step(std::string("Null propagation (mode=") + mode_name(mode) + ", output=null coalescing)", non_null_inst);
        // testedVar != null ? testedVar.AccessChain : nullInst
        // => testedVar?.AccessChain ?? nullInst
        // (only valid if AccessChain returns a non-nullable value; a by-ref-like type such as Span<T> is excluded because it cannot be
        // wrapped in Nullable<T> for the ?. / ?? form)
        introduce_unwrap(tested_var, var_load, mode);
        auto* rewrap = make<NullableRewrap>(non_null_inst);
        auto* result = make<NullCoalescingInstruction>(NullCoalescingKind::NullableWithValueFallback, rewrap, null_inst);
        result->underlying_result_type = null_inst->result_type();
        context_->end_step(result);
        return result;
    }
    return nullptr;
}

void NullPropagationTransform::run_statements(Block* block, int pos) {
    auto* if_inst = as<IfInstruction>(block->instructions[pos]);
    if (if_inst != nullptr && if_inst->false_inst()->match_nop()) {
        ILVariable* tested_var = nullptr;
        ILInstruction* arg = nullptr;
        auto* comp = as<Comp>(if_inst->condition());
        if (comp != nullptr && comp->kind() == ComparisonKind::Inequality && comp->left()->match_ld_loc(&tested_var) &&
            comp->right()->match_ld_null()) {
            try_null_prop_for_void_call(tested_var, Mode::ReferenceType, as<Block>(if_inst->true_inst()), if_inst);
        } else if (NullableLiftingTransform::match_has_value_call(if_inst->condition(), &arg)) {
            if (arg->match_ld_loca(&tested_var)) {
                try_null_prop_for_void_call(tested_var, Mode::NullableByValue, as<Block>(if_inst->true_inst()), if_inst);
            } else if (arg->match_ld_loc(&tested_var)) {
                try_null_prop_for_void_call(tested_var, Mode::NullableByReference, as<Block>(if_inst->true_inst()), if_inst);
            }
        }
    }
    ILVariable* tested_variable = nullptr;
    ILInstruction* non_null_inst = nullptr;
    ILInstruction* null_inst = nullptr;
    Block* end_block = nullptr;
    if (transform_null_propagation_on_unconstrained_generic_expression(block, pos, &tested_variable, &non_null_inst, &null_inst, &end_block)) {
        ILInstruction* parent_instruction = non_null_inst->parent();
        ILInstruction* replacement = try_null_propagation(tested_variable, non_null_inst, null_inst, Mode::UnconstrainedType);
        if (replacement == nullptr) return;
        context_->step("TransformNullPropagationOnUnconstrainedGenericExpression", block);
        if (auto* stloc = as<StLoc>(parent_instruction)) {
            stloc->set_value(replacement);
        } else if (auto* leave = as<Leave>(parent_instruction)) {
            leave->set_value(replacement);
        } else {
            // if this ever happens, the pattern checked by TransformNullPropagationOnUnconstrainedGenericExpression has changed, but
            // this part of the code was not properly adjusted.
            throw UsageError("null propagation on an unconstrained generic expression: unexpected parent instruction");
        }
        context_->end_step(replacement);
        // Remove the fallback conditions and blocks
        block->instructions.remove_range(pos + 1, 2);
        // if the endBlock is only reachable through the current block, combine both blocks.
        if (end_block != nullptr && end_block->incoming_edge_count() == 1) {
            block->instructions.add_range(end_block->instructions);
            block->instructions.remove_at(pos + 2);
            end_block->remove();
        }
        ILInlining::inline_if_possible(block, pos, *context_);
    }
}

void NullPropagationTransform::try_null_prop_for_void_call(ILVariable* tested_var, Mode mode, Block* body, IfInstruction* if_inst) {
    if (body == nullptr || body->instructions.count() != 1) return;
    ILInstruction* body_inst = body->instructions[0];
    ILInstruction* arg = nullptr;
    if (body_inst->match_nullable_rewrap(&arg)) {
        body_inst = arg;
    }
    ILInstruction* var_load = nullptr;
    if (!is_valid_access_chain(tested_var, mode, body_inst, &var_load)) return;
    context_->step(std::string("Null-propagation (mode=") + mode_name(mode) + ", output=void call)", body);
    // if (testedVar != null) { testedVar.AccessChain(); }
    // => testedVar?.AccessChain();
    introduce_unwrap(tested_var, var_load, mode);
    auto* replacement = with_il_range(make<NullableRewrap>(body_inst), if_inst);
    if_inst->replace_with(replacement);
    context_->end_step(replacement);
}

bool NullPropagationTransform::is_valid_access_chain(ILVariable* tested_var, Mode mode, ILInstruction* inst, ILInstruction** final_load) {
    *final_load = nullptr;
    int chain_length = 0;

    auto arguments_after_first_may_unwrap_null = [](const InstructionCollection<ILInstruction>& arguments) {
        // ensure the access chain does not contain any 'nullable.unwrap' that aren't directly part of the chain
        for (int i = 1; i < arguments.count(); ++i) {
            if (arguments[i]->has_flag(InstructionFlags::MayUnwrapNull)) {
                return true;
            }
        }
        return false;
    };

    auto is_valid_end_of_chain = [&]() -> bool {
        switch (mode) {
            case Mode::ReferenceType:
                // either reference type (expect: ldloc(testedVar)) or unconstrained generic type (expect: ldloca(testedVar)).
                return inst->match_ld_loc_ref(tested_var);
            case Mode::NullableByValue:
                return NullableLiftingTransform::match_get_value_or_default(inst, tested_var);
            case Mode::NullableByReference: {
                ILInstruction* arg = nullptr;
                return NullableLiftingTransform::match_get_value_or_default(inst, &arg) && arg->match_ld_loc(tested_var);
            }
            case Mode::UnconstrainedType: {
                // unconstrained generic type (expect: ldloc(testedVar))
                ILInstruction* tested_var_load = nullptr;
                const IType* ignored_type = nullptr;
                return inst->match_ld_loc(tested_var) || (inst->match_ld_obj_if_ref(&tested_var_load, &ignored_type) && tested_var_load->match_ld_loc(tested_var));
            }
        }
        throw std::out_of_range("mode");
    };

    while (true) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        if (is_valid_end_of_chain()) {
            // valid end of chain
            *final_load = inst;
            return chain_length >= 1;
        } else if (inst->match_ld_fld(&target, &field)) {
            inst = target;
        } else if (inst->match_ld_flda(&target, &field)) {
            auto* address_of = as<AddressOf>(target);
            if (address_of != nullptr && field->declaring_type()->kind() == TypeKind::Struct) {
                inst = address_of->value();
            } else {
                inst = target;
            }
        } else if (auto* call = as<CallInstruction>(inst); call != nullptr && call->op_code != OpCode::NewObj) {
            if (call->arguments().count() == 0) {
                return false;
            }
            if (call->method->is_static() && (!call->method->is_extension_method() || !can_transform_to_extension_method_call(call->method, *context_))) {
                return false;  // only instance or extension methods can be called with ?. syntax
            }
            if (call->method->is_accessor() && !is_getter(call->method)) {
                return false;  // setter/adder/remover cannot be called with ?. syntax
            }
            inst = call->arguments()[0];
            ILInstruction* arg = nullptr;
            const IType* ignored_type = nullptr;
            const IType* this_type = call->constrained_to != nullptr ? call->constrained_to : call->method->declaring_type();
            if (this_type->is_reference_type() == false && inst->match_address_of(&arg, &ignored_type)) {
                inst = arg;
            } else if (auto* ld_obj_if_ref = as<LdObjIfRef>(inst)) {
                inst = ld_obj_if_ref->target();
            }
            // ensure the access chain does not contain any 'nullable.unwrap' that aren't directly part of the chain
            if (arguments_after_first_may_unwrap_null(call->arguments())) return false;
        } else if (auto* ld_len = as<LdLen>(inst)) {
            inst = ld_len->array();
        } else if (auto* ld_elema = as<LdElema>(inst)) {
            inst = ld_elema->array();
            // ensure the access chain does not contain any 'nullable.unwrap' that aren't directly part of the chain
            for (ILInstruction* index : ld_elema->indices()) {
                if (index->has_flag(InstructionFlags::MayUnwrapNull)) return false;
            }
        } else if (auto* unwrap = as<NullableUnwrap>(inst)) {
            inst = unwrap->argument();
            if (unwrap->ref_input) {
                if (auto* address_of = as<AddressOf>(inst)) inst = address_of->value();
            }
            // The argument of the unwrap instruction cannot be the end of the chain, because this would result in two null-conditional
            // operators immediately following each other, which is not valid in C#.
            if (is_valid_end_of_chain()) return false;
        } else if (auto* dyn_get_member = as<DynamicGetMemberInstruction>(inst)) {
            inst = dyn_get_member->target();
        } else if (auto* dyn_invoke_member = as<DynamicInvokeMemberInstruction>(inst)) {
            inst = dyn_invoke_member->arguments()[0];
            if (arguments_after_first_may_unwrap_null(dyn_invoke_member->arguments())) return false;
        } else if (auto* dyn_get_index = as<DynamicGetIndexInstruction>(inst)) {
            inst = dyn_get_index->arguments()[0];
            if (arguments_after_first_may_unwrap_null(dyn_get_index->arguments())) return false;
        } else {
            // unknown node -> invalid chain
            return false;
        }
        chain_length++;
    }
}

bool NullPropagationTransform::is_getter(const IMethod* method) { return method->accessor_kind() == MethodSemanticsAttributes::Getter; }

void NullPropagationTransform::introduce_unwrap(ILVariable* tested_var, ILInstruction* var_load, Mode mode) {
    ILInstruction* old_parent = var_load->parent();
    const int old_child_index = var_load->child_index();
    ILInstruction* replacement = nullptr;
    switch (mode) {
        case Mode::ReferenceType:
        case Mode::UnconstrainedType:
            // Wrap varLoad in nullable.unwrap:
            replacement = make<NullableUnwrap>(var_load->result_type(), var_load, var_load->result_type() == StackType::Ref);
            break;
        case Mode::NullableByValue: {
            assert(NullableLiftingTransform::match_get_value_or_default(var_load, tested_var));
            ILInstruction* ld_loc = with_il_range(make<LdLoc>(tested_var), var_load->get_child(0));
            replacement = with_il_range(make<NullableUnwrap>(var_load->result_type(), ld_loc), var_load);
            break;
        }
        case Mode::NullableByReference: {
            ILInstruction* ld_loc = with_il_range(make<LdLoc>(tested_var), var_load->get_child(0));
            replacement = with_il_range(make<NullableUnwrap>(var_load->result_type(), ld_loc, true), var_load);
            break;
        }
        default:
            throw std::out_of_range("mode");
    }
    old_parent->set_child(old_child_index, replacement);
}

// stloc target(targetInst)
// stloc defaultTemporary(default.value type)
// if (logic.not(comp.o(box `0(ldloc defaultTemporary) != ldnull))) Block fallbackBlock {
// 	stloc defaultTemporary(ldobj type(ldloc target))
// 	stloc target(ldloca defaultTemporary)
// 	if (comp.o(ldloc defaultTemporary == ldnull)) Block fallbackBlock2 {
// 		stloc resultTemporary(nullInst)
// 		br endBlock
// 	}
// }
// stloc resultTemporary(constrained[type].call_instruction(ldloc target, ...))
// br endBlock
// =>
// stloc resultTemporary(nullable.rewrap(constrained[type].call_instruction(nullable.unwrap(targetInst), ...)))
//
// -or-
//
// stloc target(targetInst)
// stloc defaultTemporary(default.value type)
// if (logic.not(comp.o(box `0(ldloc defaultTemporary) != ldnull))) Block fallbackBlock {
// 	stloc defaultTemporary(ldobj type(ldloc target))
// 	stloc target(ldloca defaultTemporary)
// 	if (comp.o(ldloc defaultTemporary == ldnull)) Block fallbackBlock2 {
// 		leave(nullInst)
// 	}
// }
// leave (constrained[type].call_instruction(ldloc target, ...))
// =>
// leave (nullable.rewrap(constrained[type].call_instruction(nullable.unwrap(targetInst), ...)))
bool NullPropagationTransform::transform_null_propagation_on_unconstrained_generic_expression(Block* block, int pos, ILVariable** target,
                                                                                              ILInstruction** non_null_inst,
                                                                                              ILInstruction** null_inst, Block** end_block) {
    *target = nullptr;
    *non_null_inst = nullptr;
    *null_inst = nullptr;
    *end_block = nullptr;
    if (pos + 3 >= block->instructions.count()) return false;
    // stloc target(...)
    ILInstruction* ignored_value = nullptr;
    if (!block->instructions[pos]->match_st_loc(target, &ignored_value)) return false;
    if (!((*target)->kind() == VariableKind::StackSlot && (*target)->load_count() == 2 && (*target)->store_count() == 2)) return false;
    // stloc defaultTemporary(default.value type)
    ILVariable* default_temporary = nullptr;
    ILInstruction* default_expression = nullptr;
    const IType* type = nullptr;
    if (!(block->instructions[pos + 1]->match_st_loc(&default_temporary, &default_expression) && default_expression->match_default_value(&type))) return false;
    // In the above pattern the defaultTemporary variable is used two times in stloc and ldloc instructions and once in a ldloca instruction
    if (!(default_temporary->kind() == VariableKind::Local && default_temporary->load_count() == 2 && default_temporary->store_count() == 2 &&
          default_temporary->address_count() == 1))
        return false;
    // if (logic.not(comp.o(box `0(ldloc defaultTemporary) != ldnull))) Block fallbackBlock
    ILInstruction* condition = nullptr;
    ILInstruction* fallback_block1 = nullptr;
    ILInstruction* arg = nullptr;
    if (!(block->instructions[pos + 2]->match_if_instruction(&condition, &fallback_block1) && condition->match_comp_equals_null(&arg) &&
          arg->match_ld_loc(default_temporary)))
        return false;
    if (!match_st_loc_result_temporary(block, pos, type, *target, default_temporary, fallback_block1, non_null_inst, null_inst, end_block) &&
        !match_leave_result(block, pos, type, *target, default_temporary, fallback_block1, non_null_inst, null_inst))
        return false;
    return true;
}

// stloc resultTemporary(constrained[type].call_instruction(ldloc target, ...))
// br endBlock
bool NullPropagationTransform::match_st_loc_result_temporary(Block* block, int pos, const IType* type, ILVariable* target,
                                                             ILVariable* default_temporary, ILInstruction* fallback_block,
                                                             ILInstruction** non_null_inst, ILInstruction** null_inst, Block** end_block) {
    *end_block = nullptr;
    *non_null_inst = nullptr;
    *null_inst = nullptr;

    if (pos + 4 >= block->instructions.count()) return false;
    // stloc resultTemporary(constrained[type].call_instruction(ldloc target, ...))
    ILVariable* result_temporary = nullptr;
    if (!block->instructions[pos + 3]->match_st_loc(&result_temporary, non_null_inst)) return false;
    // br endBlock
    if (!block->instructions[pos + 4]->match_branch(end_block)) return false;
    // Analyze Block fallbackBlock
    auto* b = as<Block>(fallback_block);
    if (b == nullptr || !is_fallback_block(b, type, target, default_temporary, result_temporary, *end_block, null_inst)) return false;

    return true;
}

bool NullPropagationTransform::match_leave_result(Block* block, int pos, const IType* type, ILVariable* target, ILVariable* default_temporary,
                                                  ILInstruction* fallback_block, ILInstruction** non_null_inst, ILInstruction** null_inst) {
    *non_null_inst = nullptr;
    *null_inst = nullptr;

    // leave (constrained[type].call_instruction(ldloc target, ...))
    auto* leave = as<Leave>(block->instructions[pos + 3]);
    if (!(leave != nullptr && leave->is_leaving_function())) return false;
    *non_null_inst = leave->value();
    // Analyze Block fallbackBlock
    auto* b = as<Block>(fallback_block);
    if (b == nullptr || !is_fallback_block(b, type, target, default_temporary, nullptr, leave->target_container(), null_inst)) return false;
    return true;
}

// Block fallbackBlock {
// 	stloc defaultTemporary(ldobj type(ldloc target))
// 	stloc target(ldloca defaultTemporary)
// 	if (comp.o(ldloc defaultTemporary == ldnull)) Block fallbackBlock {
// 		stloc resultTemporary(ldnull)
// 		br endBlock
// 	}
// }
bool NullPropagationTransform::is_fallback_block(Block* block, const IType* type, ILVariable* target, ILVariable* default_temporary,
                                                 ILVariable* result_temporary, ILInstruction* end_block_or_leave_container,
                                                 ILInstruction** null_inst) {
    *null_inst = nullptr;
    if (!(block->instructions.count() == 3)) return false;
    // stloc defaultTemporary(ldobj type(ldloc target))
    ILInstruction* value = nullptr;
    if (!block->instructions[0]->match_st_loc(default_temporary, &value)) return false;
    ILInstruction* inst = nullptr;
    const IType* t = nullptr;
    if (!(value->match_ld_obj(&inst, &t) && type->equals(t) && inst->match_ld_loc(target))) return false;
    // stloc target(ldloca defaultTemporary)
    ILInstruction* default_address = nullptr;
    if (!(block->instructions[1]->match_st_loc(target, &default_address) && default_address->match_ld_loca(default_temporary))) return false;
    // if (comp.o(ldloc defaultTemporary == ldnull)) Block fallbackBlock
    ILInstruction* condition = nullptr;
    ILInstruction* tmp = nullptr;
    ILInstruction* arg = nullptr;
    if (!(block->instructions[2]->match_if_instruction(&condition, &tmp) && condition->match_comp_equals_null(&arg) && arg->match_ld_loc(default_temporary)))
        return false;
    // Block fallbackBlock {
    //   stloc resultTemporary(nullInst)
    //   br endBlock
    // }
    ILInstruction* fallback_inst = Block::unwrap(tmp);
    auto* fallback_block = as<Block>(fallback_inst);
    auto* end_block = as<Block>(end_block_or_leave_container);
    if (fallback_block != nullptr && end_block != nullptr) {
        if (!(fallback_block->instructions.count() == 2)) return false;
        if (!fallback_block->instructions[0]->match_st_loc(result_temporary, null_inst)) return false;
        if (!fallback_block->instructions[1]->match_branch(end_block)) return false;
    } else {
        auto* fallback_leave = as<Leave>(fallback_inst);
        auto* leave_container = as<BlockContainer>(end_block_or_leave_container);
        if (!(fallback_leave != nullptr && leave_container != nullptr && fallback_leave->target_container() == leave_container &&
              !fallback_leave->value()->match_nop()))
            return false;
        *null_inst = fallback_leave->value();
    }
    return true;
}

void NullPropagationStatementTransform::run(Block* block, int pos, StatementTransformContext& context) {
    if (!context.settings().null_propagation) return;
    NullPropagationTransform(context).run_statements(block, pos);
}

}  // namespace cb::dotnet::decompiler::il::transforms
