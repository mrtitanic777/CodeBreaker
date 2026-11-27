// dotnet/decompiler/il/transforms/early_expression_transforms.cxx -- see early_expression_transforms.hpp.
#include "dotnet/decompiler/il/transforms/early_expression_transforms.hpp"

#include <cstdint>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/util/decimal.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::TypeKind;

namespace {

// new decimal(long): the magnitude in the low 64 bits.
util::Decimal decimal_from_int64(std::int64_t value) {
    bool negative = value < 0;
    std::uint64_t magnitude = negative ? 0 - static_cast<std::uint64_t>(value) : static_cast<std::uint64_t>(value);
    return util::Decimal(static_cast<std::uint32_t>(magnitude), static_cast<std::uint32_t>(magnitude >> 32), 0, negative, 0);
}

// new decimal(ulong)
util::Decimal decimal_from_uint64(std::uint64_t value) {
    return util::Decimal(static_cast<std::uint32_t>(value), static_cast<std::uint32_t>(value >> 32), 0, false, 0);
}

}  // namespace

void EarlyExpressionTransforms::run(ILFunction* function, ILTransformContext& context) {
    context_ = &context;
    default_visit(*function);
}

void EarlyExpressionTransforms::default_visit(ILInstruction& inst) {
    for (ILInstruction* child : inst.children()) {
        child->accept_visitor(*this);
    }
}

void EarlyExpressionTransforms::visit_comp(Comp& inst) {
    ILVisitor::visit_comp(inst);
    fix_comparison_kind_ld_null(&inst, *context_);
}

void EarlyExpressionTransforms::fix_comparison_kind_ld_null(Comp* inst, ILTransformContext& context) {
    if (inst->is_lifted()) {
        return;
    }
    if (inst->right()->match_ld_null()) {
        if (inst->kind() == ComparisonKind::GreaterThan) {
            context.step("comp(left > ldnull)  => comp(left != ldnull)", inst);
            inst->set_kind(ComparisonKind::Inequality);
        } else if (inst->kind() == ComparisonKind::LessThanOrEqual) {
            context.step("comp(left <= ldnull) => comp(left == ldnull)", inst);
            inst->set_kind(ComparisonKind::Equality);
        }
    } else if (inst->left()->match_ld_null()) {
        if (inst->kind() == ComparisonKind::LessThan) {
            context.step("comp(ldnull < right)  => comp(ldnull != right)", inst);
            inst->set_kind(ComparisonKind::Inequality);
        } else if (inst->kind() == ComparisonKind::GreaterThanOrEqual) {
            context.step("comp(ldnull >= right) => comp(ldnull == right)", inst);
            inst->set_kind(ComparisonKind::Equality);
        }
    }

    ILInstruction* arg;
    const IType* type;
    if (inst->right()->match_ld_null() && inst->left()->match_box(&arg, &type) && type->kind() == TypeKind::TypeParameter) {
        if (inst->kind() == ComparisonKind::Equality) {
            context.step("comp(box T(..) == ldnull) -> comp(.. == ldnull)", inst);
            inst->set_left(arg);
        }
        if (inst->kind() == ComparisonKind::Inequality) {
            context.step("comp(box T(..) != ldnull) -> comp(.. != ldnull)", inst);
            inst->set_left(arg);
        }
    }
}

void EarlyExpressionTransforms::visit_st_obj(StObj& inst) {
    ILVisitor::visit_st_obj(inst);
    st_obj_to_st_loc(&inst, *context_);
}

bool EarlyExpressionTransforms::st_obj_to_st_loc(StObj* inst, ILTransformContext& context) {
    ILVariable* v;
    if (inst->target()->match_ld_loca(&v) && typesystem::is_compatible_type_for_memory_access(*v->type(), *inst->type()) &&
        inst->unaligned_prefix() == 0 && !inst->is_volatile()) {
        const std::string name = v->name.value_or("");
        context.step("stobj(ldloca " + name + ", ...) => stloc " + name + "(...)", inst);
        ILInstruction* replacement = with_il_range(make<StLoc>(v, inst->value()), inst);
        if (v->stack_type == StackType::Unknown && inst->type()->kind() != TypeKind::Unknown && inst->slot_info() != &Block::instruction_slot) {
            replacement = make<Conv>(replacement, typesystem::to_primitive_type(*inst->type()), false, typesystem::Sign::None);
        }
        inst->replace_with(replacement);
        context.end_step(replacement);
        return true;
    }
    return false;
}

void EarlyExpressionTransforms::visit_ld_obj(LdObj& inst) {
    ILVisitor::visit_ld_obj(inst);
    address_of_ld_loc_to_ld_loca(&inst, *context_);
    ld_obj_to_ld_loc(&inst, *context_);
}

bool EarlyExpressionTransforms::ld_obj_to_ld_loc(LdObj* inst, ILTransformContext& context) {
    ILVariable* v;
    if (inst->target()->match_ld_loca(&v) && typesystem::is_compatible_type_for_memory_access(*v->type(), *inst->type()) &&
        inst->unaligned_prefix() == 0 && !inst->is_volatile()) {
        context.step("ldobj(ldloca " + v->name.value_or("") + ") => ldloc " + v->name.value_or(""), inst);
        ILInstruction* replacement = with_il_range(make<LdLoc>(v), inst);
        if (v->stack_type == StackType::Unknown && inst->type()->kind() != TypeKind::Unknown) {
            replacement = make<Conv>(replacement, typesystem::to_primitive_type(*inst->type()), false, typesystem::Sign::None);
        }
        inst->replace_with(replacement);
        context.end_step(replacement);
        return true;
    }
    return false;
}

void EarlyExpressionTransforms::address_of_ld_loc_to_ld_loca(LdObj* inst, ILTransformContext& context) {
    // ldobj(...(addressof(ldloc V))) where ... can be zero or more ldflda instructions
    // =>
    // ldobj(...(ldloca V))
    ILInstruction* temp = inst->target();
    std::vector<Interval> range = temp->il_ranges();
    ILInstruction* ldflda_target;
    const IField* ldflda_field;
    while (temp->match_ld_flda(&ldflda_target, &ldflda_field)) {
        temp = ldflda_target;
        for (Interval r : temp->il_ranges()) range.push_back(r);
    }
    ILInstruction* address_of_target;
    const IType* address_of_type;
    ILVariable* v;
    if (temp->match_address_of(&address_of_target, &address_of_type) && address_of_target->match_ld_loc(&v)) {
        context.step("ldobj(...(addressof(ldloca " + v->name.value_or("") + "))) => ldobj(...(ldloca " + v->name.value_or("") + "))", inst);
        LdLoca* replacement = with_il_range(make<LdLoca>(v), address_of_target);
        for (Interval r : range) {
            replacement = with_il_range(replacement, r);
        }
        temp->replace_with(replacement);
        context.end_step(replacement);
    }
}

void EarlyExpressionTransforms::visit_call(Call& inst) {
    ILVisitor::visit_call(inst);
    transform_async_helpers_await_to_await(&inst, *context_);
}

bool EarlyExpressionTransforms::transform_async_helpers_await_to_await(Call* inst, ILTransformContext& context) {
    if (!context.settings().async_await || !context.function()->is_async()) return false;
    if (!inst->method->is_static() || inst->arguments().count() != 1 || !is_async_helpers_method(inst->method, "Await")) return false;
    context.step("call AsyncHelpers.Await(value) => await(value)", inst);
    Await* await_inst = with_il_range(make<Await>(inst->arguments()[0]), inst);
    await_inst->get_awaiter_method = nullptr;
    await_inst->get_result_method = inst->method;
    inst->replace_with(await_inst);
    context.end_step(await_inst);
    return true;
}

bool EarlyExpressionTransforms::is_async_helpers_method(const IMethod* method, const std::string& name) {
    return method->name() == name && method->declaring_type() != nullptr &&
           method->declaring_type()->full_name() == "System.Runtime.CompilerServices.AsyncHelpers";
}

void EarlyExpressionTransforms::visit_new_obj(NewObj& inst) {
    LdcDecimal* decimal_constant;
    if (transform_decimal_ctor_to_constant(&inst, &decimal_constant)) {
        context_->step("TransformDecimalCtorToConstant", &inst);
        inst.replace_with(decimal_constant);
        context_->end_step(decimal_constant);
        return;
    }

    ILVisitor::visit_new_obj(inst);
}

bool EarlyExpressionTransforms::transform_decimal_ctor_to_constant(NewObj* inst, LdcDecimal** result) {
    const IType* t = inst->method->declaring_type();
    *result = nullptr;
    if (!typesystem::is_known_type(t, KnownTypeCode::Decimal)) return false;
    auto& args = inst->arguments();
    if (args.count() == 1) {
        std::int64_t val;
        if (args[0]->match_ldc_i(&val)) {
            const typesystem::ITypeDefinition* definition = inst->method->parameters()[0]->type()->get_definition();
            if (definition == nullptr) return false;
            switch (definition->known_type_code()) {
                case KnownTypeCode::Int32: *result = make<LdcDecimal>(decimal_from_int64(static_cast<std::int32_t>(val))); break;
                case KnownTypeCode::UInt32: *result = make<LdcDecimal>(decimal_from_uint64(static_cast<std::uint32_t>(val))); break;
                case KnownTypeCode::Int64: *result = make<LdcDecimal>(decimal_from_int64(val)); break;
                case KnownTypeCode::UInt64: *result = make<LdcDecimal>(decimal_from_uint64(static_cast<std::uint64_t>(val))); break;
                default: *result = nullptr; break;
            }
            return *result != nullptr;
        }
    } else if (args.count() == 5) {
        std::int32_t lo, mid, hi, is_negative, scale;
        if (args[0]->match_ldc_i4(&lo) && args[1]->match_ldc_i4(&mid) && args[2]->match_ldc_i4(&hi) && args[3]->match_ldc_i4(&is_negative) &&
            args[4]->match_ldc_i4(&scale) && static_cast<std::uint8_t>(scale) <= 28) {
            *result = make<LdcDecimal>(util::Decimal(static_cast<std::uint32_t>(lo), static_cast<std::uint32_t>(mid),
                                                     static_cast<std::uint32_t>(hi), is_negative != 0, static_cast<std::uint8_t>(scale)));
            return true;
        }
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::il::transforms
