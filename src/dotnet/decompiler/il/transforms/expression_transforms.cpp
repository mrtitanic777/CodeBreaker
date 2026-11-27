// dotnet/decompiler/il/transforms/expression_transforms.cpp -- see expression_transforms.hpp.
#include "dotnet/decompiler/il/transforms/expression_transforms.hpp"

#include <cstdint>
#include <set>
#include <stdexcept>
#include <vector>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/pointer_arithmetic_offset.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/early_expression_transforms.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/transform_assignment.hpp"
#include "dotnet/decompiler/il/transforms/user_defined_logic_transform.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/util/decimal.hpp"

// The transforms of other files that ExpressionTransforms calls into: used where their headers exist.
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#include "dotnet/decompiler/il/transforms/transform_array_initializers.hpp"
#include "dotnet/decompiler/il/transforms/index_range_transform.hpp"
#include "dotnet/decompiler/il/transforms/inline_array_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::TypeKind;

namespace {

bool has_binder_flag(CSharpBinderFlags flags, CSharpBinderFlags flag) { return (flags & flag) == flag; }

}  // namespace

void ExpressionTransforms::run_on_single_statement(ILInstruction* statement, ILTransformContext& context) {
    if (statement == nullptr) throw std::invalid_argument("statement");
    auto* parent = as<Block>(statement->parent());
    if (parent == nullptr) throw std::invalid_argument("ILInstruction must be a statement, i.e., direct child of a block.");
    ExpressionTransforms transform;
    BlockTransformContext block_context(context);
    StatementTransformContext statement_context(block_context);
    transform.run(parent, statement->child_index(), statement_context);
}

void ExpressionTransforms::run(Block* block, int pos, StatementTransformContext& context) {
    context_ = &context;
    context.step_start_group("ExpressionTransforms (" + block->label() + ":" + std::to_string(pos) + ")", block->instructions[pos]);
    block->instructions[pos]->accept_visitor(*this);
    context.step_end_group(true);
}

void ExpressionTransforms::default_visit(ILInstruction& inst) {
    for (ILInstruction* child : inst.children()) {
        child->accept_visitor(*this);
    }
}

void ExpressionTransforms::visit_block_container(BlockContainer& container) {
    if (container.kind() == ContainerKind::Switch) {
        // Special case for switch: Only visit the switch condition block.
        auto* switch_inst = static_cast<SwitchInstruction*>(container.entry_point()->instructions[0]);
        switch_inst->value()->accept_visitor(*this);

        handle_switch_expression(&container, switch_inst);
    }
    // No need to call the base method, see comment in visit_block.
}

void ExpressionTransforms::visit_block(Block& block) {
    if (block.kind == BlockKind::ControlFlow) {
        // Don't visit child control flow blocks;
        // since this is a block transform
        // we know those were already handled previously.
        return;
    }
    ILVisitor::visit_block(block);
}

void ExpressionTransforms::visit_comp(Comp& inst) {
    // "logic.not(arg)" is sugar for "comp(arg != ldc.i4 0)"
    ILInstruction* arg;
    if (inst.match_logic_not(&arg)) {
        visit_logic_not(&inst, arg);
        return;
    } else if (inst.kind() == ComparisonKind::Inequality && inst.lifting_kind == ComparisonLiftingKind::None && inst.right()->match_ldc_i4(0) &&
               (IfInstruction::is_in_condition_slot(&inst) || is<Comp>(inst.left()))) {
        // if (comp(x != 0)) ==> if (x)
        // comp(comp(...) != 0) => comp(...)
        context_->step("Remove redundant comp(... != 0)", &inst);
        inst.left()->add_il_range(inst);
        ILInstruction* left = inst.left();
        inst.replace_with(left);
        context_->end_step(left);
        left->accept_visitor(*this);
        return;
    }
    if (context_->settings().lift_nullables) {
        NullableLiftingTransform(*context_).run(&inst);
    }

    ILVisitor::visit_comp(inst);
    if (inst.is_lifted()) {
        return;
    }
    EarlyExpressionTransforms::fix_comparison_kind_ld_null(&inst, *context_);

    ILInstruction* right_without_conv = inst.right()->unwrap_conv(ConversionKind::SignExtend)->unwrap_conv(ConversionKind::ZeroExtend);
    if (right_without_conv->match_ldc_i4(0) && inst.sign == Sign::Unsigned &&
        (inst.kind() == ComparisonKind::GreaterThan || inst.kind() == ComparisonKind::LessThanOrEqual)) {
        if (inst.kind() == ComparisonKind::GreaterThan) {
            context_->step("comp.unsigned(left > ldc.i4 0) => comp(left != ldc.i4 0)", &inst);
            inst.set_kind(ComparisonKind::Inequality);
            visit_comp(inst);
            return;
        } else if (inst.kind() == ComparisonKind::LessThanOrEqual) {
            context_->step("comp.unsigned(left <= ldc.i4 0) => comp(left == ldc.i4 0)", &inst);
            inst.set_kind(ComparisonKind::Equality);
            visit_comp(inst);
            return;
        }
    } else if (right_without_conv->match_ldc_i4(0) && is_equality_or_inequality(inst.kind())) {
        ILInstruction* array;
        if (inst.left()->match_ld_len(StackType::I, &array)) {
            // comp.unsigned(ldlen array == conv i4->i(ldc.i4 0))
            // => comp(ldlen.i4 array == ldc.i4 0)
            // This is a special case where the C# compiler doesn't generate conv.i4 after ldlen.
            context_->step("comp(ldlen.i4 array == ldc.i4 0)", &inst);
            inst.input_type = StackType::I4;
            ILInstruction* old_left = inst.left();
            old_left->replace_with(with_il_range(make<LdLen>(StackType::I4, array), old_left));
            inst.set_right(right_without_conv);
        } else if (auto* conv = as<Conv>(inst.left());
                   conv != nullptr && conv->target_type == PrimitiveType::I && conv->argument()->result_type() == StackType::O) {
            // C++/CLI sometimes uses this weird comparison with null:
            context_->step("comp(conv o->i (ldloc obj) == conv i4->i <sign extend>(ldc.i4 0))", &inst);
            // -> comp(ldloc obj == ldnull)
            inst.input_type = StackType::O;
            inst.set_left(conv->argument());
            inst.set_right(with_il_range(make<LdNull>(), inst.right()));
            inst.right()->add_il_range(*right_without_conv);
        }
    }
}

void ExpressionTransforms::visit_conv(Conv& inst) {
    inst.argument()->accept_visitor(*this);
    ILInstruction* array;
    if (inst.argument()->match_ld_len(StackType::I, &array) && is_integer_type(inst.target_type) &&
        (!inst.check_for_overflow || context_->settings().assume_array_length_fits_into_int32)) {
        context_->step("conv.i4(ldlen array) => ldlen.i4(array)", &inst);
        inst.add_il_range(*inst.argument());
        LdLen* ld_len = with_il_range(make<LdLen>(get_stack_type(inst.target_type), array), inst);
        inst.replace_with(ld_len);
        context_->end_step(ld_len);
        return;
    }
    auto* conv = as<Conv>(inst.argument());
    if (is_float_type(inst.target_type) && conv != nullptr && conv->kind == ConversionKind::IntToFloat && conv->target_type == PrimitiveType::R) {
        // IL conv.r.un does not indicate whether to convert the target type to R4 or R8,
        // so the C# compiler usually follows it with an explicit conv.r4 or conv.r8.
        // To avoid emitting '(float)(double)val', we combine these two conversions:
        context_->step("conv.rN(conv.r.un(...)) => conv.rN.un(...)", &inst);
        auto* new_conv = make<Conv>(conv->argument(), conv->input_type, conv->input_sign, inst.target_type, inst.check_for_overflow,
                                    inst.is_lifted() || conv->is_lifted());
        inst.replace_with(new_conv);
        context_->end_step(new_conv);
        return;
    }
}

void ExpressionTransforms::visit_box(Box& inst) {
    inst.argument()->accept_visitor(*this);
    if (inst.type()->is_reference_type() == true && inst.argument()->result_type() == inst.result_type()) {
        // For reference types, box is a no-op.
        context_->step("box ref-type(arg) => arg", &inst);
        inst.argument()->add_il_range(inst);
        ILInstruction* arg = inst.argument();
        inst.replace_with(arg);
        context_->end_step(arg);
    }
}

void ExpressionTransforms::visit_ld_elema(LdElema& inst) {
    ILVisitor::visit_ld_elema(inst);
    clean_up_array_indices(inst.indices());
    if (IndexRangeTransform::handle_ld_elema(&inst, *context_)) return;
}

void ExpressionTransforms::visit_new_arr(NewArr& inst) {
    ILVisitor::visit_new_arr(inst);
    clean_up_array_indices(inst.indices());
}

void ExpressionTransforms::clean_up_array_indices(InstructionCollection<ILInstruction>& indices) {
    for (int i = 0; i < indices.count(); ++i) {
        ILInstruction* index = indices[i];
        auto* conv = as<Conv>(index);
        if (conv != nullptr && conv->result_type() == StackType::I &&
            ((conv->kind == ConversionKind::Truncate && conv->check_for_overflow) || conv->kind == ConversionKind::ZeroExtend ||
             conv->kind == ConversionKind::SignExtend)) {
            context_->step("Remove conv.i from array index", index);
            index->replace_with(conv->argument());
            context_->end_step(conv->argument());
        }
    }
}

void ExpressionTransforms::visit_logic_not(Comp* inst, ILInstruction* arg) {
    ILInstruction* lhs;
    ILInstruction* rhs;
    if (auto* comp = as<Comp>(arg)) {
        if ((!is_float_type(comp->input_type) && !comp->is_lifted()) || is_equality_or_inequality(comp->kind())) {
            context_->step("push negation into comparison", inst);
            comp->set_kind(negate(comp->kind()));
            comp->add_il_range(*inst);
            inst->replace_with(comp);
            context_->end_step(comp);
        }
        comp->accept_visitor(*this);
    } else if (arg->match_logic_and(&lhs, &rhs)) {
        // logic.not(if (lhs) rhs else ldc.i4 0)
        // ==> if (logic.not(lhs)) ldc.i4 1 else logic.not(rhs)
        context_->step("push negation into logic.and", inst);
        auto* if_inst = static_cast<IfInstruction*>(arg);
        ILInstruction* ldc0 = if_inst->false_inst();
        assert(ldc0->match_ldc_i4(0));
        if_inst->set_condition(with_il_range(Comp::logic_not(lhs), inst));
        if_inst->set_true_inst(with_il_range(make<LdcI4>(1), ldc0));
        if_inst->set_false_inst(with_il_range(Comp::logic_not(rhs), inst));
        inst->replace_with(if_inst);
        context_->end_step(if_inst);
        if_inst->accept_visitor(*this);
    } else if (arg->match_logic_or(&lhs, &rhs)) {
        // logic.not(if (lhs) ldc.i4 1 else rhs)
        // ==> if (logic.not(lhs)) logic.not(rhs) else ldc.i4 0)
        context_->step("push negation into logic.or", inst);
        auto* if_inst = static_cast<IfInstruction*>(arg);
        ILInstruction* ldc1 = if_inst->true_inst();
        assert(ldc1->match_ldc_i4(1));
        if_inst->set_condition(with_il_range(Comp::logic_not(lhs), inst));
        if_inst->set_true_inst(with_il_range(Comp::logic_not(rhs), inst));
        if_inst->set_false_inst(with_il_range(make<LdcI4>(0), ldc1));
        inst->replace_with(if_inst);
        context_->end_step(if_inst);
        if_inst->accept_visitor(*this);
    } else {
        arg->accept_visitor(*this);
    }
}

void ExpressionTransforms::visit_call(Call& inst) {
    {
        ILInstruction* nullable_value;
        ILInstruction* fallback;
        if (NullableLiftingTransform::match_get_value_or_default(&inst, &nullable_value, &fallback) && SemanticHelper::is_pure(fallback->flags())) {
            context_->step("call Nullable{T}.GetValueOrDefault(a, b) -> a ?? b", &inst);
            auto* ld_obj = make<LdObj>(nullable_value, inst.method->declaring_type());
            auto* replacement = make<NullCoalescingInstruction>(NullCoalescingKind::NullableWithValueFallback, ld_obj, fallback);
            replacement->underlying_result_type = fallback->result_type();
            inst.replace_with(with_il_range(replacement, inst));
            context_->end_step(replacement);
            replacement->accept_visitor(*this);
            return;
        }
    }
    {
        ILInstruction* replacement2;
        if (TransformArrayInitializers::transform_runtime_helpers_create_span_initialization(&inst, *context_, &replacement2)) {
            context_->step("TransformRuntimeHelpersCreateSpanInitialization: single-dim", &inst);
            inst.replace_with(replacement2);
            context_->end_step(replacement2);
            replacement2->accept_visitor(*this);
            return;
        }
    }
    ILVisitor::visit_call(inst);
    if (context_->settings().inline_arrays && InlineArrayTransform::run_on_expression(&inst, *context_)) {
        return;
    }
    TransformAssignment::handle_compound_assign(&inst, *context_);
}

void ExpressionTransforms::visit_call_virt(CallVirt& inst) {
    ILVisitor::visit_call_virt(inst);
    TransformAssignment::handle_compound_assign(&inst, *context_);
}

void ExpressionTransforms::visit_new_obj(NewObj& inst) {
    ILInstruction* localloc_span;
    if (transform_span_t_ctor_containing_stack_alloc(&inst, &localloc_span)) {
        context_->step("new Span<T>(stackalloc) -> stackalloc Span<T>", &inst);
        inst.replace_with(localloc_span);
        context_->end_step(localloc_span);
        ILInstruction* stmt = Block::get_containing_statement(localloc_span);
        // Special case to eliminate extra store
        if (auto* store_stmt = as<StLoc>(get_next_sibling(stmt)); store_stmt != nullptr && is<LdLoc>(store_stmt->value()))
            ILInlining::inline_if_possible(static_cast<Block*>(stmt->parent()), stmt->child_index(), *context_);
        return;
    }
    {
        ILInstruction* replacement;
        if (TransformArrayInitializers::transform_span_t_array_initialization(&inst, *context_, &replacement)) {
            context_->step("TransformSpanTArrayInitialization: single-dim", &inst);
            inst.replace_with(replacement);
            context_->end_step(replacement);
            return;
        }
    }
    LdVirtDelegate* ld_virt_delegate;
    if (transform_delegate_ctor_ld_virt_ftn_to_ld_virt_delegate(&inst, &ld_virt_delegate)) {
        context_->step("new Delegate(target, ldvirtftn Method) -> ldvirtdelegate Delegate Method(target)", &inst);
        inst.replace_with(ld_virt_delegate);
        context_->end_step(ld_virt_delegate);
        return;
    }
    ILVisitor::visit_new_obj(inst);
}

// newobj Delegate..ctor(target, ldvirtftn TargetMethod(target))
// =>
// ldvirtdelegate System.Delegate TargetMethod(target)
bool ExpressionTransforms::transform_delegate_ctor_ld_virt_ftn_to_ld_virt_delegate(NewObj* inst, LdVirtDelegate** ld_virt_delegate) {
    *ld_virt_delegate = nullptr;
    if (inst->method->declaring_type()->kind() != TypeKind::Delegate) return false;
    if (inst->arguments().count() != 2) return false;
    auto* ld_virt_ftn = as<LdVirtFtn>(inst->arguments()[1]);
    if (ld_virt_ftn == nullptr) return false;
    if (!SemanticHelper::is_pure(inst->arguments()[0]->flags())) return false;
    if (!inst->arguments()[0]->match(ld_virt_ftn->argument()).success()) return false;
    *ld_virt_delegate = make<LdVirtDelegate>(inst->arguments()[0], inst->method->declaring_type(), ld_virt_ftn->method());
    with_il_range(*ld_virt_delegate, inst);
    with_il_range(*ld_virt_delegate, ld_virt_ftn);
    with_il_range(*ld_virt_delegate, ld_virt_ftn->argument());
    return true;
}

// newobj Span..ctor(localloc(conv i4->u <zero extend>(ldc.i4 sizeInBytes)), numberOfElementsExpr)
// =>
// localloc.span T(numberOfElementsExpr)
//
// -or-
//
// newobj Span..ctor(Block IL_0000 (StackAllocInitializer) {
//     stloc I_0(localloc(conv i4->u<zero extend>(ldc.i4 sizeInBytes)))
//     ...
//     final: ldloc I_0
// }, numberOfElementsExpr)
// =>
// Block IL_0000 (StackAllocInitializer) {
//     stloc I_0(localloc.span T(numberOfElementsExpr))
//     ...
//     final: ldloc I_0
// }
bool ExpressionTransforms::transform_span_t_ctor_containing_stack_alloc(NewObj* new_obj, ILInstruction** localloc_span) {
    *localloc_span = nullptr;
    const IType* type = new_obj->method->declaring_type();
    if (!typesystem::is_known_type(type, KnownTypeCode::SpanOfT) && !typesystem::is_known_type(type, KnownTypeCode::ReadOnlySpanOfT)) return false;
    if (new_obj->arguments().count() != 2 || type->type_arguments().size() != 1) return false;
    const IType* element_type = type->type_arguments()[0];
    ILInstruction* size_in_bytes;
    if (new_obj->arguments()[0]->match_loc_alloc(&size_in_bytes) && matches_element_count(size_in_bytes, element_type, new_obj->arguments()[1])) {
        *localloc_span = make<LocAllocSpan>(new_obj->arguments()[1], type);
        return true;
    }
    auto* initializer = as<Block>(new_obj->arguments()[0]);
    if (initializer != nullptr && initializer->kind == BlockKind::StackAllocInitializer) {
        ILVariable* initializer_variable;
        ILInstruction* value;
        if (!initializer->instructions[0]->match_st_loc(&initializer_variable, &value)) return false;
        if (!(value->match_loc_alloc(&size_in_bytes) && matches_element_count(size_in_bytes, element_type, new_obj->arguments()[1]))) return false;
        ILVariable* new_variable = initializer_variable->function()->register_variable(VariableKind::InitializerTarget, type);
        std::vector<LdLoc*> loads = initializer_variable->load_instructions();
        for (LdLoc* load : loads) {
            ILInstruction* new_inst = make<LdLoc>(new_variable);
            new_inst->add_il_range(*load);
            if (load->parent() != initializer) new_inst = make<Conv>(new_inst, PrimitiveType::I, false, Sign::None);
            load->replace_with(new_inst);
        }
        std::vector<IStoreInstruction*> stores = initializer_variable->store_instructions();
        for (IStoreInstruction* store : stores) {
            store->set_variable(new_variable);
        }
        value->replace_with(make<LocAllocSpan>(new_obj->arguments()[1], type));
        *localloc_span = initializer;
        return true;
    }
    return false;
}

bool ExpressionTransforms::matches_element_count(ILInstruction* size_in_bytes_instr, const IType* element_type, ILInstruction* element_count_instr2) {
    // upstream builds a PointerType(elementType) only to read its element type back
    ILInstruction* element_count_instr = PointerArithmeticOffset::detect(size_in_bytes_instr, element_type, true, true);
    if (element_count_instr == nullptr || !element_count_instr->unwrap_conv(ConversionKind::ZeroExtend)->match(element_count_instr2).success())
        return false;
    return true;
}

bool ExpressionTransforms::transform_decimal_field_to_constant(LdObj* inst, LdcDecimal** result) {
    const IField* field;
    if (inst->match_lds_fld(&field) && typesystem::is_known_type(field->declaring_type(), KnownTypeCode::Decimal)) {
        bool has_value = false;
        util::Decimal value(0, 0, 0, false, 0);
        const std::string name = field->name();
        if (name == "One") {
            value = util::Decimal(1, 0, 0, false, 0);
            has_value = true;
        } else if (name == "MinusOne") {
            value = util::Decimal(1, 0, 0, true, 0);
            has_value = true;
        } else if (name == "Zero") {
            value = util::Decimal(0, 0, 0, false, 0);
            has_value = true;
        }
        if (has_value) {
            *result = with_il_range(with_il_range(make<LdcDecimal>(value), inst), inst->target());
            return true;
        }
    }
    *result = nullptr;
    return false;
}

void ExpressionTransforms::visit_ld_obj(LdObj& inst) {
    ILVisitor::visit_ld_obj(inst);
    EarlyExpressionTransforms::address_of_ld_loc_to_ld_loca(&inst, *context_);
    if (EarlyExpressionTransforms::ld_obj_to_ld_loc(&inst, *context_)) return;
    LdcDecimal* decimal_constant;
    if (transform_decimal_field_to_constant(&inst, &decimal_constant)) {
        context_->step("TransformDecimalFieldToConstant", &inst);
        inst.replace_with(decimal_constant);
        context_->end_step(decimal_constant);
        return;
    }
}

void ExpressionTransforms::visit_ld_obj_if_ref(LdObjIfRef& inst) {
    ILVisitor::visit_ld_obj_if_ref(inst);
    if (is<AddressOf>(inst.target())) {
        context_->step("ldobj.if.ref(addressof(...)) -> addressof(...)", &inst);
        // there already is a temporary, so the ldobj.if.ref is a no-op in both cases
        ILInstruction* target = inst.target();
        inst.replace_with(target);
        context_->end_step(target);
        return;
    }
    ILVariable* s;
    if (inst.target()->match_ld_loc(&s) && s->is_single_definition() && s->load_count() == 1) {
        // s.StoreInstructions.SingleOrDefault() is StLoc { Value: LdLoca { Variable: { AddressCount: 1, StoreCount: 1 } } }
        const auto& stores = s->store_instructions();
        auto* stloc = stores.size() == 1 ? as<StLoc>(stores[0]->as_instruction()) : nullptr;
        auto* ld_loca = stloc != nullptr ? as<LdLoca>(stloc->value()) : nullptr;
        if (ld_loca != nullptr && ld_loca->variable()->address_count() == 1 && ld_loca->variable()->store_count() == 1) {
            context_->step("Single use of ldobj.if.ref(ldloc v) -> ldloc v", &inst);
            // there already is a temporary, so the ldobj.if.ref is a no-op in both cases
            ILInstruction* target = inst.target();
            inst.replace_with(target);
            context_->end_step(target);
            return;
        }
    }
}

void ExpressionTransforms::visit_st_obj(StObj& inst) {
    ILVisitor::visit_st_obj(inst);
    if (EarlyExpressionTransforms::st_obj_to_st_loc(&inst, *context_)) {
        context_->request_rerun();
        return;
    }
    TransformAssignment::handle_compound_assign(&inst, *context_);
}

void ExpressionTransforms::visit_st_loc(StLoc& inst) {
    ILVisitor::visit_st_loc(inst);
    TransformAssignment::handle_compound_assign(&inst, *context_);
}

void ExpressionTransforms::visit_if_instruction(IfInstruction& if_instruction) {
    IfInstruction* inst = &if_instruction;
    inst->true_inst()->accept_visitor(*this);
    inst->false_inst()->accept_visitor(*this);
    inst = handle_conditional_operator(inst);

    // Bring LogicAnd/LogicOr into their canonical forms:
    // if (cond) ldc.i4 0 else RHS --> if (!cond) RHS else ldc.i4 0
    // if (cond) RHS else ldc.i4 1 --> if (!cond) ldc.i4 1 else RHS
    // Be careful: when both LHS and RHS are the constant 1, we must not
    // swap the arguments as it would lead to an infinite transform loop.
    if ((inst->true_inst()->match_ldc_i4(0) && !inst->false_inst()->match_ldc_i4(0)) ||
        (inst->false_inst()->match_ldc_i4(1) && !inst->true_inst()->match_ldc_i4(1))) {
        context_->step("canonicalize logic and/or", inst);
        ILInstruction* t = inst->true_inst();
        inst->set_true_inst(inst->false_inst());
        inst->set_false_inst(t);
        inst->set_condition(Comp::logic_not(inst->condition()));
    }
    // Process condition after our potential modifications.
    inst->condition()->accept_visitor(*this);

    if (NullableLiftingTransform(*context_).run(inst)) {
        context_->step("NullableLiftingTransform", inst);
        return;
    }

    if (transform_dynamic_add_assign_or_remove_assign(inst, *context_)) return;
    ILInstruction* condition;
    ILInstruction* true_inst;
    ILInstruction* false_inst;
    if (inst->match_if_instruction_positive_condition(&condition, &true_inst, &false_inst)) {
        ILInstruction* transformed = UserDefinedLogicTransform::transform(condition, true_inst, false_inst);
        if (transformed == nullptr) {
            transformed = UserDefinedLogicTransform::transform_dynamic(condition, true_inst, false_inst);
        }
        if (transformed != nullptr) {
            context_->step("User-defined short-circuiting logic operator (roslyn pattern)", condition);
            transformed->add_il_range(*inst);
            inst->replace_with(transformed);
            context_->end_step(transformed);
            return;
        }
    }
    ILInstruction* tested_operand;
    if (MatchInstruction::is_pattern_match(inst->condition(), &tested_operand, &context_->settings()) && inst->true_inst()->match_ldc_i4(1) &&
        inst->false_inst()->match_ldc_i4(0)) {
        context_->step("match(x) ? true : false -> match(x)", inst);
        inst->condition()->add_il_range(*inst);
        ILInstruction* match_condition = inst->condition();
        inst->replace_with(match_condition);
        context_->end_step(match_condition);
        return;
    }
}

IfInstruction* ExpressionTransforms::handle_conditional_operator(IfInstruction* inst) {
    // if (cond) stloc A(V1) else stloc A(V2) --> stloc A(if (cond) V1 else V2)
    auto* true_inst = as<Block>(inst->true_inst());
    if (true_inst == nullptr || true_inst->instructions.count() != 1) return inst;
    auto* false_inst = as<Block>(inst->false_inst());
    if (false_inst == nullptr || false_inst->instructions.count() != 1) return inst;
    ILVariable* v;
    ILInstruction* value1;
    ILInstruction* value2;
    if (true_inst->instructions[0]->match_st_loc(&v, &value1) && false_inst->instructions[0]->match_st_loc(v, &value2)) {
        context_->step("conditional operator", inst);
        auto* new_if = make<IfInstruction>(Comp::logic_not(inst->condition()), value2, value1);
        new_if->add_il_range(*inst);
        auto* st_loc = make<StLoc>(v, new_if);
        inst->replace_with(st_loc);
        context_->end_step(st_loc);
        context_->request_rerun();  // trigger potential inlining of the newly created StLoc
        return new_if;
    }
    return inst;
}

void ExpressionTransforms::handle_switch_expression(BlockContainer* container, SwitchInstruction* switch_inst) {
    if (!context_->settings().switch_expressions) return;
    assert(container->kind() == ContainerKind::Switch);
    assert(container->result_type() == StackType::Void);
    SwitchSection* default_section = switch_inst->get_default_section();
    StackType result_type = StackType::Void;
    BlockContainer* leave_target = nullptr;
    ILVariable* result_variable = nullptr;
    for (SwitchSection* section : switch_inst->sections) {
        if (section != default_section) {
            // every section except for the default must have exactly 1 label
            if (section->labels().count() != (section->has_null_label() ? 0u : 1u)) return;
        }
        Block* section_block;
        if (!section->body()->match_branch(&section_block)) return;
        if (section_block->incoming_edge_count() != 1) return;
        if (section_block->parent() != container) return;
        if (section_block->instructions.count() == 1) {
            if (is<Throw>(section_block->instructions[0])) {
                // OK
            } else if (auto* leave = as<Leave>(section_block->instructions[0])) {
                if (!leave->is_leaving_function()) return;
                if (leave_target == nullptr) leave_target = leave->target_container();
                assert(leave_target == leave->target_container());
                result_type = leave->value()->result_type();
            } else {
                return;
            }
        } else if (section_block->instructions.count() == 2) {
            ILVariable* v;
            if (!section_block->instructions[0]->match_st_loc(&v)) return;
            if (!section_block->instructions[1]->match_leave(container)) return;
            if (result_variable == nullptr) result_variable = v;
            if (result_variable != v) return;
            result_type = result_variable->stack_type;
        } else {
            return;
        }
    }
    // Exactly one of resultVariable/leaveTarget must be null
    if ((result_variable == nullptr) == (leave_target == nullptr)) return;
    // C# has no ref-returning switch expression: an arm cannot be `0 => ref x`.
    if (result_type == StackType::Ref) return;
    if (auto* str2int = as<StringToInt>(switch_inst->value())) {
        // validate that each integer is used for exactly one value
        std::set<int> integers_used;
        for (const auto& entry : str2int->map()) {
            if (!integers_used.insert(entry.second).second) return;
        }
    }

    context_->step("Switch Expression", switch_inst);
    switch_inst->set_result_type(result_type);
    for (SwitchSection* section : switch_inst->sections) {
        Block* block = static_cast<Branch*>(section->body())->target_block();
        if (block->instructions.count() == 1) {
            if (auto* t = as<Throw>(block->instructions[0])) {
                t->result_type_ = result_type;
                section->set_body(t);
            } else if (auto* leave = as<Leave>(block->instructions[0])) {
                section->set_body(leave->value());
            } else {
                throw UsageError("InvalidOperationException");
            }
        } else {
            section->set_body(static_cast<StLoc*>(block->instructions[0])->value());
        }
    }
    if (result_variable != nullptr) {
        auto* st_loc = make<StLoc>(result_variable, switch_inst);
        container->replace_with(st_loc);
        context_->end_step(st_loc);
    } else {
        auto* leave_inst = make<Leave>(leave_target, switch_inst);
        container->replace_with(leave_inst);
        context_->end_step(leave_inst);
    }
    context_->request_rerun();  // new StLoc might trigger inlining
}

bool ExpressionTransforms::transform_dynamic_add_assign_or_remove_assign(IfInstruction* inst, ILTransformContext& context) {
    ILInstruction* condition;
    ILInstruction* true_inst;
    ILInstruction* false_inst;
    if (!inst->match_if_instruction_positive_condition(&condition, &true_inst, &false_inst)) return false;
    auto* is_event = as<DynamicIsEventInstruction>(condition);
    if (is_event == nullptr) return false;
    true_inst = Block::unwrap(true_inst);
    false_inst = Block::unwrap(false_inst);
    auto* dynamic_compound_assign = as<DynamicCompoundAssign>(false_inst);
    if (dynamic_compound_assign == nullptr) return false;
    auto* get_member = as<DynamicGetMemberInstruction>(dynamic_compound_assign->target());
    if (get_member == nullptr) return false;
    if (!SemanticHelper::is_pure(is_event->argument()->flags())) return false;
    if (!is_event->argument()->match(get_member->target()).success()) return false;
    auto* invoke_member = as<DynamicInvokeMemberInstruction>(true_inst);
    if (invoke_member == nullptr) return false;
    if (!(has_binder_flag(invoke_member->binder_flags(), CSharpBinderFlags::InvokeSpecialName) &&
          has_binder_flag(invoke_member->binder_flags(), CSharpBinderFlags::ResultDiscarded)))
        return false;
    switch (dynamic_compound_assign->operation()) {
        case ExpressionType::AddAssign:
            if (invoke_member->name() != "add_" + get_member->name()) return false;
            break;
        case ExpressionType::SubtractAssign:
            if (invoke_member->name() != "remove_" + get_member->name()) return false;
            break;
        default: return false;
    }
    if (!dynamic_compound_assign->value()->match(invoke_member->arguments()[1]).success()) return false;
    if (!invoke_member->arguments()[0]->match(get_member->target()).success()) return false;
    context.step("+= / -= dynamic.isevent pattern -> dynamic.compound.op", inst);
    inst->replace_with(dynamic_compound_assign);
    context.end_step(dynamic_compound_assign);
    return true;
}

void ExpressionTransforms::visit_dynamic_set_member_instruction(DynamicSetMemberInstruction& inst) {
    ILVisitor::visit_dynamic_set_member_instruction(inst);
    transform_dynamic_set_member_instruction(&inst, *context_);
}

void ExpressionTransforms::transform_dynamic_set_member_instruction(DynamicSetMemberInstruction* inst, StatementTransformContext& context) {
    if (!has_binder_flag(inst->binder_flags(), CSharpBinderFlags::ValueFromCompoundAssignment)) return;
    auto* binary_op = as<DynamicBinaryOperatorInstruction>(inst->value());
    if (binary_op == nullptr) return;
    auto* dynamic_get_member = as<DynamicGetMemberInstruction>(binary_op->left());
    if (dynamic_get_member == nullptr) return;
    if (!dynamic_get_member->target()->match(inst->target()).success()) return;
    if (!SemanticHelper::is_pure(dynamic_get_member->target()->flags())) return;
    if (inst->name() != dynamic_get_member->name() || !DynamicCompoundAssign::is_expression_type_supported(binary_op->operation())) return;
    context.step("dynamic.setmember.compound -> dynamic.compound.op", inst);
    auto* compound_assign = make<DynamicCompoundAssign>(binary_op->operation(), binary_op->binder_flags(), binary_op->left(),
                                                        binary_op->left_argument_info(), binary_op->right(), binary_op->right_argument_info());
    inst->replace_with(compound_assign);
    context.end_step(compound_assign);
}

void ExpressionTransforms::visit_dynamic_set_index_instruction(DynamicSetIndexInstruction& inst) {
    ILVisitor::visit_dynamic_set_index_instruction(inst);

    if (!has_binder_flag(inst.binder_flags(), CSharpBinderFlags::ValueFromCompoundAssignment)) return;
    auto* binary_op = as<DynamicBinaryOperatorInstruction>(inst.arguments().last_or_default());
    if (binary_op == nullptr) return;
    auto* dynamic_get_index = as<DynamicGetIndexInstruction>(binary_op->left());
    if (dynamic_get_index == nullptr) return;
    if (inst.arguments().count() != dynamic_get_index->arguments().count() + 1) return;
    // Ensure that same arguments are passed to dynamicGetIndex and inst:
    for (int j = 0; j < dynamic_get_index->arguments().count(); j++) {
        if (!SemanticHelper::is_pure(dynamic_get_index->arguments()[j]->flags())) return;
        if (!dynamic_get_index->arguments()[j]->match(inst.arguments()[j]).success()) return;
    }
    if (!DynamicCompoundAssign::is_expression_type_supported(binary_op->operation())) return;
    context_->step("dynamic.setindex.compound -> dynamic.compound.op", &inst);
    auto* compound_assign = make<DynamicCompoundAssign>(binary_op->operation(), binary_op->binder_flags(), binary_op->left(),
                                                        binary_op->left_argument_info(), binary_op->right(), binary_op->right_argument_info());
    inst.replace_with(compound_assign);
    context_->end_step(compound_assign);
}

void ExpressionTransforms::visit_binary_numeric_instruction(BinaryNumericInstruction& inst) {
    ILVisitor::visit_binary_numeric_instruction(inst);

    auto match_expected_shift_size = [&inst](ILInstruction* rhs) {
        switch (inst.result_type()) {
            case StackType::I4: return rhs->match_ldc_i4(31);
            case StackType::I8: return rhs->match_ldc_i4(63);
            case StackType::I: {
                // sizeof(IntPtr) * 8 - 1
                ILInstruction* mult;
                ILInstruction* one;
                ILInstruction* size;
                ILInstruction* eight;
                const IType* sizeof_type;
                return rhs->match_binary_numeric_instruction(BinaryNumericOperator::Sub, &mult, &one) &&
                       mult->match_binary_numeric_instruction(BinaryNumericOperator::Mul, &size, &eight) && size->match_size_of(&sizeof_type) &&
                       get_stack_type(*sizeof_type) == StackType::I && eight->match_ldc_i4(8) && one->match_ldc_i4(1);
            }
            default: return false;
        }
    };

    switch (inst.op) {
        case BinaryNumericOperator::ShiftLeft:
        case BinaryNumericOperator::ShiftRight: {
            ILInstruction* lhs;
            ILInstruction* rhs;
            if (inst.right()->match_binary_numeric_instruction(BinaryNumericOperator::BitAnd, &lhs, &rhs) && match_expected_shift_size(rhs)) {
                // a << (b & 31) => a << b
                context_->step("Combine bit.and into shift", &inst);
                inst.set_right(lhs);
            }
            break;
        }
        case BinaryNumericOperator::BitAnd:
            if (typesystem::is_known_type(infer_type(inst.left(), &context_->type_system()), KnownTypeCode::Boolean) &&
                typesystem::is_known_type(infer_type(inst.right(), &context_->type_system()), KnownTypeCode::Boolean)) {
                if (NullableLiftingTransform(*context_).run(&inst)) {
                    // e.g. "(a.GetValueOrDefault() == b.GetValueOrDefault()) & (a.HasValue & b.HasValue)"
                }
            }
            break;
        default: break;
    }
}

void ExpressionTransforms::visit_try_catch_handler(TryCatchHandler& inst) {
    ILVisitor::visit_try_catch_handler(inst);
    if (auto* filter_container = as<BlockContainer>(inst.filter()); filter_container != nullptr && filter_container->blocks.count() == 1) {
        transform_catch_when(&inst, filter_container->entry_point());
    }
    if (auto* catch_container = as<BlockContainer>(inst.body())) transform_catch_variable(&inst, catch_container->entry_point(), true);
}

// catch ex : TException when (...) BlockContainer {
//     Block entryPoint (incoming: 1) {
//         stloc v(ldloc ex)
//         ...
//     }
// }
// =>
// catch v : TException when (...) BlockContainer {
//     Block entryPoint (incoming: 1) {
//         ...
//     }
// }
void ExpressionTransforms::transform_catch_variable(TryCatchHandler* handler, Block* entry_point, bool is_catch_block) {
    if (!handler->variable()->is_single_definition() || handler->variable()->load_count() != 1) return;  // handler.Variable already has non-trivial uses
    ILVariable* exception_var;
    ILInstruction* exception_slot_load;
    if (!entry_point->instructions[0]->match_st_loc(&exception_var, &exception_slot_load)) {
        // Not the pattern with a second exceptionVar.
        // However, it is still possible that we need to remove a pointless UnboxAny:
        if (auto* inlined_unbox_any = as<UnboxAny>(handler->variable()->load_instructions()[0]->parent())) {
            if (typesystem::type_equals(inlined_unbox_any->type(), handler->variable()->type())) {
                context_->step("TransformCatchVariable - remove inlined UnboxAny", inlined_unbox_any);
                ILInstruction* unbox_argument = inlined_unbox_any->argument();
                inlined_unbox_any->replace_with(unbox_argument);
                context_->end_step(unbox_argument);
                for (Interval range : inlined_unbox_any->il_ranges()) handler->add_exception_specifier_il_range(range);
            }
        }
        return;
    }
    if (exception_var->kind() != VariableKind::Local && exception_var->kind() != VariableKind::StackSlot) return;
    if (auto* unbox_any = as<UnboxAny>(exception_slot_load)) {
        // When catching a type parameter, csc emits an unbox.any instruction
        if (!typesystem::type_equals(unbox_any->type(), handler->variable()->type())) return;
        exception_slot_load = unbox_any->argument();
    }
    if (!exception_slot_load->match_ld_loc(handler->variable())) return;
    // Check that exceptionVar is only used within the catch block:
    std::vector<ILInstruction*> all_uses;
    for (LdLoc* load : exception_var->load_instructions()) all_uses.push_back(load);
    for (IStoreInstruction* store : exception_var->store_instructions()) all_uses.push_back(store->as_instruction());
    for (LdLoca* address : exception_var->address_instructions()) all_uses.push_back(address);
    for (ILInstruction* inst : all_uses) {
        if (!inst->is_descendant_of(handler)) return;
    }
    context_->step("TransformCatchVariable", entry_point->instructions[0]);
    exception_var->set_kind(VariableKind::ExceptionLocal);
    exception_var->name = handler->variable()->name;
    exception_var->set_type(handler->variable()->type());
    exception_var->has_generated_name = handler->variable()->has_generated_name;
    handler->set_variable(exception_var);
    if (is_catch_block) {
        for (ILInstruction* descendant : entry_point->instructions[0]->descendants()) {
            for (Interval offset : descendant->il_ranges()) handler->add_exception_specifier_il_range(offset);
        }
    }
    entry_point->instructions.remove_at(0);
}

// Inline condition from catch-when condition BlockContainer, if possible.
void ExpressionTransforms::transform_catch_when(TryCatchHandler* handler, Block* entry_point) {
    transform_catch_variable(handler, entry_point, false);
    ILInstruction* condition;
    BlockContainer* leave_target;
    if (entry_point->instructions.count() == 1 && entry_point->instructions[0]->match_leave(&leave_target, &condition)) {
        context_->step("TransformCatchWhen", entry_point->instructions[0]);
        handler->set_filter(condition);
        context_->end_step(condition);
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
