// dotnet/decompiler/il/transforms/null_coalescing_transform.cxx -- see null_coalescing_transform.hpp.
#include "dotnet/decompiler/il/transforms/null_coalescing_transform.hpp"

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

#if __has_include("dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp")
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#define CB_HAVE_NULLABLE_LIFTING_TRANSFORM
#endif

namespace cb::dotnet::decompiler::il::transforms {

void NullCoalescingTransform::run(Block* block, int pos, StatementTransformContext& context) {
    if (transform_ref_types(block, pos, context)) return;
    if (transform_hoisted_constructor_argument_null_guard(block, pos, context)) return;
    transform_throw_expression_value_types(block, pos, context);
}

bool NullCoalescingTransform::transform_ref_types(Block* block, int pos, StatementTransformContext& context) {
    auto* stloc = as<StLoc>(block->instructions[pos]);
    if (stloc == nullptr) return false;
    if (stloc->variable()->kind() != VariableKind::StackSlot) return false;
    ILInstruction* condition;
    ILInstruction* true_inst;
    if (!block->instructions[pos + 1]->match_if_instruction(&condition, &true_inst)) return false;
    ILInstruction* left;
    ILInstruction* right;
    if (!(condition->match_comp_equals(&left, &right) && left->match_ld_loc(stloc->variable()) && right->match_ld_null())) return false;
    true_inst = Block::unwrap(true_inst);
    // stloc s(valueInst)
    // if (comp(ldloc s == ldnull)) {
    //     stloc s(fallbackInst)
    // }
    // =>
    // stloc s(if.notnull(valueInst, fallbackInst))
    ILInstruction* fallback_value;
    if (true_inst->match_st_loc(stloc->variable(), &fallback_value)) {
        context.step("NullCoalescingTransform: simple (reference types)", stloc);
        stloc->set_value(make<NullCoalescingInstruction>(NullCoalescingKind::Ref, stloc->value(), fallback_value));
        block->instructions.remove_at(pos + 1);  // remove if instruction
        ILInlining::inline_one_if_possible(block, pos, InliningOptions::None, context);
        return true;
    }
    // sometimes the compiler generates:
    // stloc s(valueInst)
    // if (comp(ldloc s == ldnull)) {
    //     stloc v(fallbackInst)
    //     stloc s(ldloc v)
    // }
    // v must be single-assign and single-use.
    ILVariable* temporary;
    ILInstruction* use_of_temporary;
    auto* true_block = as<Block>(true_inst);
    if (true_block != nullptr && true_block->instructions.count() == 2 && true_block->instructions[0]->match_st_loc(&temporary, &fallback_value) &&
        temporary->is_single_definition() && temporary->load_count() == 1 &&
        true_block->instructions[1]->match_st_loc(stloc->variable(), &use_of_temporary) && use_of_temporary->match_ld_loc(temporary)) {
        context.step("NullCoalescingTransform: with temporary variable (reference types)", stloc);
        stloc->set_value(make<NullCoalescingInstruction>(NullCoalescingKind::Ref, stloc->value(), fallback_value));
        block->instructions.remove_at(pos + 1);  // remove if instruction
        ILInlining::inline_one_if_possible(block, pos, InliningOptions::None, context);
        return true;
    }
    // stloc obj(valueInst)
    // if (comp(ldloc obj == ldnull)) {
    //     throw(...)
    // }
    // =>
    // stloc obj(if.notnull(valueInst, throw(...)))
    auto* throw_inst = as<Throw>(true_inst);
    if (context.settings().throw_expressions && throw_inst != nullptr) {
        context.step("NullCoalescingTransform (reference types + throw expression)", stloc);
        throw_inst->result_type_ = StackType::O;
        stloc->set_value(make<NullCoalescingInstruction>(NullCoalescingKind::Ref, stloc->value(), throw_inst));
        block->instructions.remove_at(pos + 1);  // remove if instruction
        ILInlining::inline_one_if_possible(block, pos, InliningOptions::None, context);
        return true;
    }
    return false;
}

bool NullCoalescingTransform::transform_hoisted_constructor_argument_null_guard(Block* block, int pos, StatementTransformContext& context) {
    // A throw-expression is the only way to express the folded form.
    if (!context.settings().throw_expressions) return false;

    // block.Ancestors.OfType<ILFunction>().FirstOrDefault()
    ILFunction* function = nullptr;
    for (ILInstruction* ancestor : block->ancestors()) {
        function = as<ILFunction>(ancestor);
        if (function != nullptr) break;
    }
    if (function == nullptr || function->method == nullptr || !function->method->is_constructor() || function->method->is_static()) return false;

    // Match `if (comp(ldloc param == ldnull)) throw(...)` with no else branch.
    ILInstruction* guard = block->instructions[pos];
    LdLoc* param_load;
    Throw* throw_inst;
    if (!is_argument_null_guard(guard, &param_load, &throw_inst)) return false;

    // Only a guard sitting in the initializer's argument evaluation, i.e. before the
    // chained constructor call, is necessarily compiler-hoisted.
    if (!ILInlining::is_in_constructor_initializer(function, guard)) return false;

    // The compiler hoists the guard directly in front of the statement containing the
    // parameter use it was lifted out of, so that use must be order-safely reachable
    // within the immediately following statement.
    ILInstruction* param_load_parent = param_load->parent();
    int param_load_child_index = param_load->child_index();
    ILInstruction* throw_inst_parent = throw_inst->parent();
    int throw_inst_child_index = throw_inst->child_index();
    auto* expression_with_throw = make<NullCoalescingInstruction>(NullCoalescingKind::Ref, param_load, throw_inst);
    ILInlining::FindResult result =
        ILInlining::find_load_in_next(block->instructions[pos + 1], param_load->variable(), expression_with_throw, InliningOptions::None);
    auto* first_use = result.type == ILInlining::FindResultType::Found ? as<LdLoc>(result.load_inst) : nullptr;
    if (first_use == nullptr) {
        // reset the primary positions (see remarks on ILInstruction.Parent)
        param_load_parent->children().set(param_load_child_index, param_load);
        throw_inst_parent->children().set(throw_inst_child_index, throw_inst);
        return false;
    }

    context.step("NullCoalescingTransform: fold hoisted null-guard of '" + param_load->variable()->name.value_or("") +
                     "' into argument of chained constructor call",
                 guard);

    ILVariable* temp = function->register_variable(VariableKind::StackSlot, param_load->variable()->type());
    first_use->set_variable(temp);
    throw_inst->result_type_ = StackType::O;
    auto* stloc = make<StLoc>(temp, expression_with_throw);
    stloc->add_il_range(*guard);
    block->instructions.set(pos, stloc);
    context.end_step(stloc);
    ILInlining::inline_one_if_possible(block, pos, InliningOptions::None, context);
    return true;
}

bool NullCoalescingTransform::is_argument_null_guard(ILInstruction* inst, LdLoc** param_load, Throw** throw_inst) {
    *param_load = nullptr;
    *throw_inst = nullptr;
    ILInstruction* condition;
    ILInstruction* true_inst;
    if (!inst->match_if_instruction(&condition, &true_inst)) return false;
    auto* t = as<Throw>(Block::unwrap(true_inst));
    if (t == nullptr) return false;
    ILInstruction* left;
    ILInstruction* right;
    if (!(condition->match_comp_equals(&left, &right) && right->match_ld_null() && is<LdLoc>(left))) return false;
    auto* load = static_cast<LdLoc*>(left);
    if (load->variable()->kind() != VariableKind::Parameter) return false;
    *param_load = load;
    *throw_inst = t;
    return true;
}

bool NullCoalescingTransform::transform_throw_expression_value_types(Block* block, int pos, StatementTransformContext& context) {
#ifdef CB_HAVE_NULLABLE_LIFTING_TRANSFORM
    if (pos + 2 >= block->instructions.count()) return false;
    auto* stloc = as<StLoc>(block->instructions[pos]);
    if (stloc == nullptr) return false;
    ILVariable* v = stloc->variable();
    if (!(v->store_count() == 1 && v->load_count() == 0 && v->address_count() == 2)) return false;
    ILInstruction* condition;
    ILInstruction* true_inst;
    if (!block->instructions[pos + 1]->match_if_instruction(&condition, &true_inst)) return false;
    auto* throw_inst = as<Throw>(Block::unwrap(true_inst));
    if (throw_inst == nullptr) return false;
    ILInstruction* arg;
    if (!condition->match_logic_not(&arg)) return false;
    auto* call = as<CallInstruction>(arg);
    if (!(call != nullptr && NullableLiftingTransform::match_has_value_call(call, v))) return false;
    ILInstruction* throw_inst_parent = throw_inst->parent();
    int throw_inst_child_index = throw_inst->child_index();
    auto* null_coalescing_with_throw = make<NullCoalescingInstruction>(NullCoalescingKind::NullableWithValueFallback, stloc->value(), throw_inst);
    StackType result_type = typesystem::get_stack_type(*typesystem::nullable_type::get_underlying_type(call->method->declaring_type()));
    null_coalescing_with_throw->underlying_result_type = result_type;
    ILInlining::FindResult result = ILInlining::find_load_in_next(block->instructions[pos + 2], v, null_coalescing_with_throw, InliningOptions::None);
    if (result.type == ILInlining::FindResultType::Found && NullableLiftingTransform::match_get_value_or_default(result.load_inst->parent(), v)) {
        context.step("NullCoalescingTransform (value types + throw expression)", stloc);
        throw_inst->result_type_ = result_type;
        result.load_inst->parent()->replace_with(null_coalescing_with_throw);
        block->instructions.remove_range(pos, 2);  // remove store(s) and if instruction
        return true;
    } else {
        // reset the primary position (see remarks on ILInstruction.Parent)
        stloc->set_value(stloc->value());
        throw_inst_parent->children().set(throw_inst_child_index, throw_inst);
        return false;
    }
#else
    // WAITS: NullableLiftingTransform (match_has_value_call, match_get_value_or_default): the value-type throw-expression form
    (void)block;
    (void)pos;
    (void)context;
    return false;
#endif
}

}  // namespace cb::dotnet::decompiler::il::transforms
