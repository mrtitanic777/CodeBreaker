// dotnet/decompiler/il/transforms/nullable_lifting_transform.cpp -- see nullable_lifting_transform.hpp.
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"

#include <algorithm>
#include <cassert>

#include "dotnet/decompiler/csharp/resolver/csharp_operators.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/il_extraction.hpp"
#include "dotnet/decompiler/il/transforms/null_propagation_transform.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::TypeKind;
using typesystem::TypeParameterSubstitution;
namespace nullable_type = typesystem::nullable_type;

namespace {

using csharp::resolver::CSharpOperators;

// `inst?.WithILRange(source)`.
ILInstruction* with_il_range_or_null(ILInstruction* inst, ILInstruction* source) {
    if (inst == nullptr) return nullptr;
    inst->add_il_range(*source);
    return inst;
}

bool contains(const std::vector<ILVariable*>& list, const ILVariable* v) { return std::find(list.begin(), list.end(), v) != list.end(); }

}  // namespace

// ---- Run ----------------------------------------------------------------------------------------------------------------------------

bool NullableLiftingTransform::run(IfInstruction* if_inst) {
    ILInstruction* lifted = lift(if_inst, if_inst->condition(), if_inst->true_inst(), if_inst->false_inst());
    if (lifted != nullptr) {
        if_inst->replace_with(lifted);
        context_->end_step(lifted);
        return true;
    }
    return false;
}

bool NullableLiftingTransform::run(BinaryNumericInstruction* bni) {
    assert(!bni->is_lifted() && bni->op == BinaryNumericOperator::BitAnd);
    // caller ensures that bni.Left/bni.Right are booleans
    ILInstruction* lifted = lift(bni, bni->left(), bni->right(), make<LdcI4>(0));
    if (lifted != nullptr) {
        bni->replace_with(lifted);
        context_->end_step(lifted);
        return true;
    }
    return false;
}

void NullableLiftingTransform::run(Comp* comp) {
    if (!comp->is_lifted() && is_equality_or_inequality(comp->kind())) {
        ILInstruction* left = comp->left();
        ILInstruction* right = comp->right();
        ILInstruction* arg = nullptr;
        std::int64_t value = 0;
        if (match_get_value_or_default(left, &arg) && right->match_ldc_i(&value) && value != 0) {
            context_->step("comp(a.GetValueOrDefault() == const) -> comp.lifted(a == const)", comp);
            comp->lifting_kind = ComparisonLiftingKind::CSharp;
            comp->set_left(make<LdObj>(arg, static_cast<Call*>(left)->method->declaring_type()));
        } else if (match_get_value_or_default(right, &arg) && left->match_ldc_i(&value) && value != 0) {
            context_->step("comp(const == a.GetValueOrDefault()) -> comp.lifted(const == a)", comp);
            comp->lifting_kind = ComparisonLiftingKind::CSharp;
            comp->set_right(make<LdObj>(arg, static_cast<Call*>(right)->method->declaring_type()));
        }
    }
}

bool NullableLiftingTransform::run_statements(Block* block, int pos) {
    // e.g.:
    //  if (!condition) Block {
    //    leave IL_0000 (default.value System.Nullable`1[[System.Int64]])
    //  }
    //  leave IL_0000 (newobj .ctor(exprToLift))
    if (pos != block->instructions.count() - 2) return false;
    auto* if_inst = as<IfInstruction>(block->instructions[pos]);
    if (if_inst == nullptr) return false;
    auto* then_leave = as<Leave>(Block::unwrap(if_inst->true_inst()));
    if (then_leave == nullptr) return false;
    if (!if_inst->false_inst()->match_nop()) return false;

    auto* else_leave = as<Leave>(block->instructions[pos + 1]);
    if (else_leave == nullptr) return false;
    if (else_leave->target_container() != then_leave->target_container()) return false;

    ILInstruction* lifted = lift(if_inst, if_inst->condition(), then_leave->value(), else_leave->value());
    if (lifted != nullptr) {
        then_leave->set_value(lifted);
        if_inst->replace_with(then_leave);
        context_->end_step(then_leave);
        block->instructions.remove(else_leave);
        return true;
    }
    return false;
}

// ---- AnalyzeCondition ---------------------------------------------------------------------------------------------------------------

bool NullableLiftingTransform::analyze_condition(ILInstruction* condition) {
    ILVariable* v = nullptr;
    if (match_has_value_call(condition, &v)) {
        nullable_vars_.push_back(v);
        return true;
    } else if (auto* bitand_ = as<BinaryNumericInstruction>(condition)) {
        if (!(bitand_->op == BinaryNumericOperator::BitAnd && bitand_->result_type() == StackType::I4)) return false;
        return analyze_condition(bitand_->left()) && analyze_condition(bitand_->right());
    }
    return false;
}

bool NullableLiftingTransform::analyze_negated_condition(ILInstruction* condition) {
    ILInstruction* arg = nullptr;
    return condition->match_logic_not(&arg) && analyze_condition(arg);
}

// ---- Main lifting logic -------------------------------------------------------------------------------------------------------------

ILInstruction* NullableLiftingTransform::lift(ILInstruction* if_inst, ILInstruction* condition, ILInstruction* true_inst,
                                              ILInstruction* false_inst) {
    // ifInst is usually the IfInstruction to which condition belongs; but can also be a BinaryNumericInstruction.
    ILInstruction* arg = nullptr;
    while (condition->match_logic_not(&arg)) {
        condition = arg;
        std::swap(true_inst, false_inst);
    }
    if (context_->settings().null_propagation && !NullPropagationTransform::is_protected_if_inst(as<IfInstruction>(if_inst))) {
        ILInstruction* null_propagated = with_il_range_or_null(NullPropagationTransform(*context_).run(condition, true_inst, false_inst), if_inst);
        if (null_propagated != nullptr) return null_propagated;
    }
    if (!context_->settings().lift_nullables) return nullptr;
    if (analyze_condition(condition)) {
        // (v1 != null && ... && vn != null) ? trueInst : falseInst
        // => normal lifting
        return with_il_range_or_null(lift_normal(true_inst, false_inst), if_inst);
    }
    CompOrDecimal comp;
    if (match_comp_or_decimal(condition, &comp)) {
        // This might be a C#-style lifted comparison
        // (C# checks the underlying value before checking the HasValue bits)
        if (is_equality_or_inequality(comp.kind)) {
            // for equality/inequality, the HasValue bits must also compare equal/inequal
            if (comp.kind == ComparisonKind::Inequality) {
                // handle inequality by swapping one last time
                std::swap(true_inst, false_inst);
            }
            if (false_inst->match_ldc_i4(0)) {
                // (a.GetValueOrDefault() == b.GetValueOrDefault()) ? (a.HasValue == b.HasValue) : false
                // => a == b
                ILInstruction* result = lift_csharp_equality_comparison(comp, ComparisonKind::Equality, true_inst);
                if (result != nullptr) return result;
                return lift_csharp_user_equality_comparison(comp, ComparisonKind::Equality, true_inst);
            } else if (false_inst->match_ldc_i4(1)) {
                // (a.GetValueOrDefault() == b.GetValueOrDefault()) ? (a.HasValue != b.HasValue) : true
                // => a != b
                ILInstruction* result = lift_csharp_equality_comparison(comp, ComparisonKind::Inequality, true_inst);
                if (result != nullptr) return result;
                return lift_csharp_user_equality_comparison(comp, ComparisonKind::Inequality, true_inst);
            } else if (!comp.is_lifted && is_generic_new_pattern(comp.left, comp.right, true_inst, false_inst)) {
                // (default(T) == null) ? Activator.CreateInstance<T>() : default(T)
                // => Activator.CreateInstance<T>()
                return true_inst;
            }
        } else if (!comp.is_lifted) {
            // Not (in)equality, but one of < <= > >=.
            // Returns false unless all HasValue bits are true.
            if (false_inst->match_ldc_i4(0) && analyze_condition(true_inst)) {
                // comp(lhs, rhs) ? (v1 != null && ... && vn != null) : false
                // => comp.lifted[C#](lhs, rhs)
                return lift_csharp_comparison(comp, comp.kind);
            }
            if (true_inst->match_ldc_i4(0) && analyze_condition(false_inst)) {
                // comp(lhs, rhs) ? false : (v1 != null && ... && vn != null)
                return lift_csharp_comparison(comp, negate(comp.kind));
            }
            if (false_inst->match_ldc_i4(1) && analyze_negated_condition(true_inst)) {
                // comp(lhs, rhs) ? !(v1 != null && ... && vn != null) : true
                // => !comp.lifted[C#](lhs, rhs)
                ILInstruction* result = lift_csharp_comparison(comp, comp.kind);
                if (result == nullptr) return result;
                return Comp::logic_not(result);
            }
            if (true_inst->match_ldc_i4(1) && analyze_negated_condition(false_inst)) {
                // comp(lhs, rhs) ? true : !(v1 != null && ... && vn != null)
                ILInstruction* result = lift_csharp_comparison(comp, negate(comp.kind));
                if (result == nullptr) return result;
                return Comp::logic_not(result);
            }
        }
    }
    ILVariable* v = nullptr;
    // Handle equality comparisons with bool?:
    if (match_get_value_or_default(condition, &v) &&
        typesystem::is_known_type(nullable_type::get_underlying_type(v->type()), KnownTypeCode::Boolean)) {
        if (match_has_value_call(true_inst, v) && false_inst->match_ldc_i4(0)) {
            // v.GetValueOrDefault() ? v.HasValue : false
            // ==> v == true
            context_->step("NullableLiftingTransform: v == true", if_inst);
            ILInstruction* left = with_il_range(make<LdLoc>(v), true_inst);
            ILInstruction* right = with_il_range(make<LdcI4>(1), false_inst);
            return with_il_range(make<Comp>(ComparisonKind::Equality, ComparisonLiftingKind::CSharp, StackType::I4, Sign::None, left, right), if_inst);
        } else if (true_inst->match_ldc_i4(0) && match_has_value_call(false_inst, v)) {
            // v.GetValueOrDefault() ? false : v.HasValue
            // ==> v == false
            context_->step("NullableLiftingTransform: v == false", if_inst);
            ILInstruction* left = with_il_range(make<LdLoc>(v), false_inst);
            return with_il_range(
                make<Comp>(ComparisonKind::Equality, ComparisonLiftingKind::CSharp, StackType::I4, Sign::None, left, true_inst /* LdcI4(0) */),
                if_inst);
        } else if (match_negated_has_value_call(true_inst, v) && false_inst->match_ldc_i4(1)) {
            // v.GetValueOrDefault() ? !v.HasValue : true
            // ==> v != true
            context_->step("NullableLiftingTransform: v != true", if_inst);
            ILInstruction* left = with_il_range(make<LdLoc>(v), true_inst);
            return with_il_range(
                make<Comp>(ComparisonKind::Inequality, ComparisonLiftingKind::CSharp, StackType::I4, Sign::None, left, false_inst /* LdcI4(1) */),
                if_inst);
        } else if (true_inst->match_ldc_i4(1) && match_negated_has_value_call(false_inst, v)) {
            // v.GetValueOrDefault() ? true : !v.HasValue
            // ==> v != false
            context_->step("NullableLiftingTransform: v != false", if_inst);
            ILInstruction* left = with_il_range(make<LdLoc>(v), false_inst);
            ILInstruction* right = with_il_range(make<LdcI4>(0), true_inst);
            return with_il_range(make<Comp>(ComparisonKind::Inequality, ComparisonLiftingKind::CSharp, StackType::I4, Sign::None, left, right),
                                 if_inst);
        }
    }
    // Handle & and | on bool?:
    if (true_inst->match_ld_loc(&v)) {
        const IType* utype = nullptr;
        ILInstruction* nullable_arg = nullptr;
        if (match_nullable_ctor(false_inst, &utype, &nullable_arg) && typesystem::is_known_type(utype, KnownTypeCode::Boolean) &&
            nullable_arg->match_ldc_i4(0)) {
            // condition ? v : (bool?)false
            // => condition & v
            context_->step("NullableLiftingTransform: 3vl.bool.and(bool, bool?)", if_inst);
            return with_il_range(make<ThreeValuedBoolAnd>(condition, true_inst), if_inst);
        }
        ILVariable* v2 = nullptr;
        if (false_inst->match_ld_loc(&v2)) {
            // condition ? v : v2
            ILVariable* nullable1 = nullptr;
            ILVariable* nullable2 = nullptr;
            if (match_three_valued_logic_condition_pattern(condition, &nullable1, &nullable2)) {
                // (nullable1.GetValueOrDefault() || (!nullable2.GetValueOrDefault() && !nullable1.HasValue)) ? v : v2
                if (v == nullable1 && v2 == nullable2) {
                    context_->step("NullableLiftingTransform: 3vl.bool.or(bool?, bool?)", if_inst);
                    return with_il_range(make<ThreeValuedBoolOr>(true_inst, false_inst), if_inst);
                } else if (v == nullable2 && v2 == nullable1) {
                    context_->step("NullableLiftingTransform: 3vl.bool.and(bool?, bool?)", if_inst);
                    return with_il_range(make<ThreeValuedBoolAnd>(false_inst, true_inst), if_inst);
                }
            }
        }
    } else if (false_inst->match_ld_loc(&v)) {
        const IType* utype = nullptr;
        ILInstruction* nullable_arg = nullptr;
        if (match_nullable_ctor(true_inst, &utype, &nullable_arg) && typesystem::is_known_type(utype, KnownTypeCode::Boolean) &&
            nullable_arg->match_ldc_i4(1)) {
            // condition ? (bool?)true : v
            // => condition | v
            context_->step("NullableLiftingTransform: 3vl.logic.or(bool, bool?)", if_inst);
            return with_il_range(make<ThreeValuedBoolOr>(condition, false_inst), if_inst);
        }
    }
    return nullptr;
}

bool NullableLiftingTransform::is_generic_new_pattern(ILInstruction* comp_left, ILInstruction* comp_right, ILInstruction* true_inst,
                                                      ILInstruction* false_inst) {
    // (default(T) == null) ? Activator.CreateInstance<T>() : default(T)
    const IType* type = nullptr;
    if (!false_inst->match_default_value(&type)) return false;
    auto* c = as<Call>(true_inst);
    if (!(c != nullptr && c->method->full_name() == "System.Activator.CreateInstance" && c->method->type_arguments().size() == 1)) return false;
    if (type->kind() != TypeKind::TypeParameter) return false;
    const IType* type2 = nullptr;
    if (!comp_left->match_default_value(&type2)) return false;
    return type->equals(type2) && comp_right->match_ld_null();
}

bool NullableLiftingTransform::match_three_valued_logic_condition_pattern(ILInstruction* condition, ILVariable** nullable1,
                                                                          ILVariable** nullable2) {
    // Try to match: nullable1.GetValueOrDefault() || (!nullable2.GetValueOrDefault() && !nullable1.HasValue)
    *nullable1 = nullptr;
    *nullable2 = nullptr;
    ILInstruction* lhs = nullptr;
    ILInstruction* rhs = nullptr;
    if (!condition->match_logic_or(&lhs, &rhs)) return false;
    if (!match_get_value_or_default(lhs, nullable1)) return false;
    if (!typesystem::is_known_type(nullable_type::get_underlying_type((*nullable1)->type()), KnownTypeCode::Boolean)) return false;
    ILInstruction* rhs_value = rhs;
    if (!rhs_value->match_logic_and(&lhs, &rhs)) return false;
    ILInstruction* arg = nullptr;
    if (!lhs->match_logic_not(&arg)) return false;
    if (!match_get_value_or_default(arg, nullable2)) return false;
    if (!typesystem::is_known_type(nullable_type::get_underlying_type((*nullable2)->type()), KnownTypeCode::Boolean)) return false;
    if (!rhs->match_logic_not(&arg)) return false;
    return match_has_value_call(arg, *nullable1);
}

// ---- CSharpComp ---------------------------------------------------------------------------------------------------------------------

bool NullableLiftingTransform::match_comp_or_decimal(ILInstruction* inst, CompOrDecimal* result) {
    *result = CompOrDecimal();
    result->instruction = inst;
    if (auto* comp = as<Comp>(inst)) {
        result->kind = comp->kind();
        result->left = comp->left();
        result->right = comp->right();
        result->is_lifted = comp->is_lifted();
        return true;
    } else if (auto* call = as<Call>(inst); call != nullptr && call->method->is_operator() && call->arguments().count() == 2 && !call->is_lifted()) {
        const std::string method_name = call->method->name();
        if (method_name == "op_Equality") {
            result->kind = ComparisonKind::Equality;
        } else if (method_name == "op_Inequality") {
            result->kind = ComparisonKind::Inequality;
        } else if (method_name == "op_LessThan") {
            result->kind = ComparisonKind::LessThan;
        } else if (method_name == "op_LessThanOrEqual") {
            result->kind = ComparisonKind::LessThanOrEqual;
        } else if (method_name == "op_GreaterThan") {
            result->kind = ComparisonKind::GreaterThan;
        } else if (method_name == "op_GreaterThanOrEqual") {
            result->kind = ComparisonKind::GreaterThanOrEqual;
        } else {
            return false;
        }
        result->left = call->arguments()[0];
        result->right = call->arguments()[1];
        return typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::Decimal);
    }
    return false;
}

const IType* NullableLiftingTransform::CompOrDecimal::left_expected_type() const {
    if (auto* call = as<Call>(instruction)) {
        return call->method->parameters()[0]->type();
    }
    return &typesystem::SpecialType::unknown_type();
}

const IType* NullableLiftingTransform::CompOrDecimal::right_expected_type() const {
    if (auto* call = as<Call>(instruction)) {
        return call->method->parameters()[1]->type();
    }
    return &typesystem::SpecialType::unknown_type();
}

ILInstruction* NullableLiftingTransform::CompOrDecimal::make_lifted(ComparisonKind new_comparison_kind, ILInstruction* left_arg,
                                                                    ILInstruction* right_arg) const {
    if (auto* comp = as<Comp>(instruction)) {
        return with_il_range(make<Comp>(new_comparison_kind, ComparisonLiftingKind::CSharp, comp->input_type, comp->sign, left_arg, right_arg),
                             instruction);
    } else if (auto* call = as<Call>(instruction)) {
        const IMethod* method = nullptr;
        if (new_comparison_kind == kind) {
            method = call->method;
        } else if (new_comparison_kind == ComparisonKind::Inequality && call->method->name() == "op_Equality") {
            const IMethod* found = nullptr;
            for (const IMethod* m : call->method->declaring_type()->get_methods([](const IMethod* candidate) { return candidate->name() == "op_Inequality"; })) {
                if (typesystem::parameter_lists_equal(m->parameters(), call->method->parameters())) {
                    found = m;
                    break;
                }
            }
            method = found;
            if (method == nullptr) return nullptr;
        } else {
            return nullptr;
        }
        auto* new_call = make<Call>(CSharpOperators::lift_user_defined_operator(method));
        new_call->arguments().add(left_arg);
        new_call->arguments().add(right_arg);
        new_call->constrained_to = call->constrained_to;
        new_call->il_stack_was_empty = call->il_stack_was_empty;
        new_call->is_tail = call->is_tail;
        return with_il_range(new_call, call);
    } else {
        return nullptr;
    }
}

// ---- Lift...Comparison --------------------------------------------------------------------------------------------------------------

ILInstruction* NullableLiftingTransform::lift_csharp_equality_comparison(const CompOrDecimal& value_comp, ComparisonKind new_comparison_kind,
                                                                         ILInstruction* has_value_test) {
    assert(is_equality_or_inequality(new_comparison_kind));
    bool has_value_test_negated = false;
    ILInstruction* arg = nullptr;
    while (has_value_test->match_logic_not(&arg)) {
        has_value_test = arg;
        has_value_test_negated = !has_value_test_negated;
    }
    // The HasValue comparison must be the same operator as the Value comparison.
    ILVariable* v = nullptr;
    if (auto* has_value_comp = as<Comp>(has_value_test)) {
        if (value_comp.is_lifted || has_value_comp->is_lifted()) return nullptr;
        // Comparing two nullables: HasValue comparison must be the same operator as the Value comparison
        if ((has_value_test_negated ? negate(has_value_comp->kind()) : has_value_comp->kind()) != new_comparison_kind) return nullptr;
        ILVariable* left_var = nullptr;
        if (!match_has_value_call(has_value_comp->left(), &left_var)) return nullptr;
        ILVariable* right_var = nullptr;
        if (!match_has_value_call(has_value_comp->right(), &right_var)) return nullptr;
        nullable_vars_ = {left_var};
        LiftResult left = do_lift(value_comp.left);
        nullable_vars_[0] = right_var;
        LiftResult right = do_lift(value_comp.right);
        if (left.inst != nullptr && right.inst != nullptr && (*left.bits)[0] && (*right.bits)[0] &&
            SemanticHelper::is_pure(left.inst->flags()) && SemanticHelper::is_pure(right.inst->flags())) {
            context_->step("NullableLiftingTransform: C# (in)equality comparison", value_comp.instruction);
            return value_comp.make_lifted(new_comparison_kind, left.inst, right.inst);
        }
    } else if (new_comparison_kind == ComparisonKind::Equality && !has_value_test_negated && match_has_value_call(has_value_test, &v)) {
        // Comparing nullable with non-nullable -> we can fall back to the normal comparison code.
        nullable_vars_ = {v};
        return lift_csharp_comparison(value_comp, new_comparison_kind);
    } else if (new_comparison_kind == ComparisonKind::Inequality && has_value_test_negated && match_has_value_call(has_value_test, &v)) {
        // Comparing nullable with non-nullable -> we can fall back to the normal comparison code.
        nullable_vars_ = {v};
        return lift_csharp_comparison(value_comp, new_comparison_kind);
    }
    return nullptr;
}

ILInstruction* NullableLiftingTransform::lift_csharp_comparison(const CompOrDecimal& comp, ComparisonKind new_comparison_kind) {
    if (comp.is_lifted) {
        // Happens when legacy csc generates 'num.GetValueOrDefault() == const && num.HasValue', checking HasValue is redundant here,
        // modern Roslyn versions optimize it and our NullableLifting transform on Comp will already lift the lhs of the logic.and.
        // Treat this case as if the transform had not undone the optimization yet.
        if (nullable_vars_.size() != 1) {
            return nullptr;
        }

        if (comp.left->match_ld_loc(nullable_vars_[0]) || comp.right->match_ld_loc(nullable_vars_[0])) {
            ILInstruction* left_clone = comp.left->clone();
            ILInstruction* right_clone = comp.right->clone();
            return comp.make_lifted(new_comparison_kind, left_clone, right_clone);
        }

        return nullptr;
    }
    LiftBinaryResult lifted = do_lift_binary(comp.left, comp.right, comp.left_expected_type(), comp.right_expected_type());
    // due to the restrictions on side effects, we only allow instructions that are pure after lifting.
    // (we can't check this before lifting due to the calls to GetValueOrDefault())
    if (lifted.left != nullptr && lifted.right != nullptr && SemanticHelper::is_pure(lifted.left->flags()) &&
        SemanticHelper::is_pure(lifted.right->flags())) {
        if (!lifted.bits->all(0, static_cast<int>(nullable_vars_.size()))) {
            // don't lift if a nullableVar doesn't contribute to the result
            return nullptr;
        }
        context_->step("NullableLiftingTransform: C# comparison", comp.instruction);
        return comp.make_lifted(new_comparison_kind, lifted.left, lifted.right);
    }
    return nullptr;
}

ILInstruction* NullableLiftingTransform::lift_csharp_user_equality_comparison(const CompOrDecimal& has_value_comp,
                                                                              ComparisonKind new_comparison_kind,
                                                                              ILInstruction* nested_if_inst) {
    // User-defined equality operator:
    //   if (comp(call get_HasValue(ldloca nullable1) == call get_HasValue(ldloca nullable2)))
    //      if (logic.not(call get_HasValue(ldloca nullable)))
    //          ldc.i4 1
    //      else
    //          call op_Equality(call GetValueOrDefault(ldloca nullable1), call GetValueOrDefault(ldloca nullable2)
    //   else
    //      ldc.i4 0

    // User-defined inequality operator:
    //   if (comp(call get_HasValue(ldloca nullable1) != call get_HasValue(ldloca nullable2)))
    //      ldc.i4 1
    //   else
    //      if (call get_HasValue(ldloca nullable))
    //         call op_Inequality(call GetValueOrDefault(ldloca nullable1), call GetValueOrDefault(ldloca nullable2))
    //      else
    //         ldc.i4 0
    if (has_value_comp.is_lifted) return nullptr;
    ILVariable* nullable1 = nullptr;
    if (!match_has_value_call(has_value_comp.left, &nullable1)) return nullptr;
    ILVariable* nullable2 = nullptr;
    if (!match_has_value_call(has_value_comp.right, &nullable2)) return nullptr;
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!nested_if_inst->match_if_instruction_positive_condition(&condition, &true_inst, &false_inst)) return nullptr;
    ILVariable* nullable = nullptr;
    if (!match_has_value_call(condition, &nullable)) return nullptr;
    if (nullable != nullable1 && nullable != nullable2) return nullptr;
    if (!false_inst->match_ldc_i4(new_comparison_kind == ComparisonKind::Equality ? 1 : 0)) return nullptr;
    bool true_inst_negated = false;
    ILInstruction* arg = nullptr;
    while (true_inst->match_logic_not(&arg)) {
        true_inst_negated = !true_inst_negated;
        true_inst = arg;
    }
    auto* call = as<Call>(true_inst);
    if (call == nullptr) return nullptr;
    if (!(call->method->is_operator() && call->arguments().count() == 2)) return nullptr;
    bool expect_equality_operator = (new_comparison_kind == ComparisonKind::Equality) ^ true_inst_negated;
    if (call->method->name() != (expect_equality_operator ? "op_Equality" : "op_Inequality")) return nullptr;
    const IMethod* lifted_operator = CSharpOperators::lift_user_defined_operator(call->method);
    if (lifted_operator == nullptr) return nullptr;
    nullable_vars_ = {nullable1};
    LiftResult left = do_lift(call->arguments()[0]);
    nullable_vars_[0] = nullable2;
    LiftResult right = do_lift(call->arguments()[1]);
    if (left.inst != nullptr && right.inst != nullptr && (*left.bits)[0] && (*right.bits)[0] && SemanticHelper::is_pure(left.inst->flags()) &&
        SemanticHelper::is_pure(right.inst->flags())) {
        context_->step("NullableLiftingTransform: C# user-defined (in)equality comparison", nested_if_inst);
        auto* new_call = make<Call>(lifted_operator);
        new_call->arguments().add(left.inst);
        new_call->arguments().add(right.inst);
        new_call->constrained_to = call->constrained_to;
        new_call->il_stack_was_empty = call->il_stack_was_empty;
        new_call->is_tail = call->is_tail;
        ILInstruction* replacement = with_il_range(new_call, call);
        if (true_inst_negated) {
            replacement = Comp::logic_not(replacement);
        }
        return replacement;
    }
    return nullptr;
}

ILInstruction* NullableLiftingTransform::lift_csharp_user_comparison(ILInstruction* true_inst, ILInstruction* false_inst) {
    // (v1 != null && ... && vn != null) ? trueInst : falseInst
    bool true_inst_negated = false;
    ILInstruction* arg = nullptr;
    while (true_inst->match_logic_not(&arg)) {
        true_inst_negated = !true_inst_negated;
        true_inst = arg;
    }
    auto* call = as<Call>(true_inst);
    if (call != nullptr && !call->is_lifted() && CSharpOperators::is_comparison_operator(call->method) &&
        false_inst->match_ldc_i4(((call->method->name() == "op_Inequality") ^ true_inst_negated) ? 1 : 0)) {
        // (v1 != null && ... && vn != null) ? call op_LessThan(lhs, rhs) : ldc.i4(0)
        const IMethod* lifted_operator = CSharpOperators::lift_user_defined_operator(call->method);
        if ((call->method->name() == "op_Equality" || call->method->name() == "op_Inequality") && nullable_vars_.size() != 1) {
            // Equality is special (returns true if both sides are null), only handle it in the normal code path if we're dealing with
            // only a single nullable var (comparing nullable with non-nullable).
            return nullptr;
        }
        if (lifted_operator == nullptr) {
            return nullptr;
        }
        context_->step("Lift user-defined comparison operator", true_inst);
        LiftBinaryResult lifted = do_lift_binary(call->arguments()[0], call->arguments()[1], call->method->parameters()[0]->type(),
                                                 call->method->parameters()[1]->type());
        if (lifted.left != nullptr && lifted.right != nullptr && lifted.bits->all(0, static_cast<int>(nullable_vars_.size()))) {
            auto* new_call = make<Call>(lifted_operator);
            new_call->arguments().add(lifted.left);
            new_call->arguments().add(lifted.right);
            new_call->constrained_to = call->constrained_to;
            new_call->il_stack_was_empty = call->il_stack_was_empty;
            new_call->is_tail = call->is_tail;
            ILInstruction* result = with_il_range(new_call, call);
            if (true_inst_negated) {
                result = Comp::logic_not(result);
            }
            return result;
        }
    }

    return nullptr;
}

// ---- LiftNormal / DoLift ------------------------------------------------------------------------------------------------------------

ILInstruction* NullableLiftingTransform::lift_normal(ILInstruction* true_inst, ILInstruction* false_inst) {
    ILInstruction* nested_condition = nullptr;
    ILInstruction* nested_true = nullptr;
    ILInstruction* nested_false = nullptr;
    if (true_inst->match_if_instruction_positive_condition(&nested_condition, &nested_true, &nested_false)) {
        // Sometimes Roslyn generates pointless conditions like:
        //   if (nullable.HasValue && (!nullable.HasValue || nullable.GetValueOrDefault() == b))
        ILVariable* v = nullptr;
        if (match_has_value_call(nested_condition, &v) && contains(nullable_vars_, v)) {
            true_inst = nested_true;
        }
    }

    bool is_null_coalescing_with_non_nullable_fallback = false;
    const IType* utype = nullptr;
    ILInstruction* expr_to_lift = nullptr;
    if (!match_nullable_ctor(true_inst, &utype, &expr_to_lift)) {
        is_null_coalescing_with_non_nullable_fallback = true;
        utype = find_type_of_stack_type(context_->type_system(), true_inst->result_type());
        expr_to_lift = true_inst;
        if (nullable_vars_.size() == 1 && expr_to_lift->match_ld_loc(nullable_vars_[0])) {
            // v.HasValue ? ldloc v : fallback
            // => v ?? fallback
            context_->step("v.HasValue ? v : fallback => v ?? fallback", true_inst);
            auto* result = make<NullCoalescingInstruction>(NullCoalescingKind::Nullable, true_inst, false_inst);
            result->underlying_result_type = get_stack_type(*nullable_type::get_underlying_type(nullable_vars_[0]->type()));
            return result;
        }
        ILInstruction* result = lift_csharp_user_comparison(true_inst, false_inst);
        if (result != nullptr) return result;
    }
    ILInstruction* lifted = nullptr;
    if (nullable_vars_.size() == 1 && match_get_value_or_default(expr_to_lift, nullable_vars_[0])) {
        // v.HasValue ? call GetValueOrDefault(ldloca v) : fallback
        // => conv.nop.lifted(ldloc v) ?? fallback
        // This case is handled separately from DoLift() because that doesn't introduce nop-conversions.
        context_->step("v.HasValue ? v.GetValueOrDefault() : fallback => v ?? fallback", true_inst);
        const IType* input_utype = nullable_type::get_underlying_type(nullable_vars_[0]->type());
        lifted = make<LdLoc>(nullable_vars_[0]);
        if (!input_utype->equals(utype) && to_primitive_type(*utype) != PrimitiveType::None) {
            // While the ILAst allows implicit conversions between short and int (because both map to I4); it does not allow implicit
            // conversions between short? and int? (structs of different types). So use 'conv.nop.lifted' to allow the conversion.
            StackType input_stack_type = get_stack_type(*input_utype);
            Sign input_sign = get_sign(*input_utype);
            lifted = make<Conv>(lifted, input_stack_type, input_sign, to_primitive_type(*utype), /*check_for_overflow*/ false, /*is_lifted*/ true);
        }
    } else {
        context_->step("NullableLiftingTransform.DoLift", true_inst);
        LiftResult result = do_lift(expr_to_lift);
        lifted = result.inst;
        if (lifted == nullptr) {
            return nullptr;
        }
        if (!result.bits->all(0, static_cast<int>(nullable_vars_.size()))) {
            // don't lift if a nullableVar doesn't contribute to the result
            return nullptr;
        }
#ifndef NDEBUG
        auto* liftable = dynamic_cast<ILiftableInstruction*>(lifted);
        assert(liftable != nullptr && liftable->is_lifted() && liftable->underlying_result_type() == expr_to_lift->result_type());
#endif
    }
    if (is_null_coalescing_with_non_nullable_fallback) {
        auto* coalescing = make<NullCoalescingInstruction>(NullCoalescingKind::NullableWithValueFallback, lifted, false_inst);
        coalescing->underlying_result_type = expr_to_lift->result_type();
        lifted = coalescing;
    } else if (!match_null(false_inst, utype)) {
        // Normal lifting, but the falseInst isn't `default(utype?)`
        // => use the `??` operator to provide the fallback value.
        auto* coalescing = make<NullCoalescingInstruction>(NullCoalescingKind::Nullable, lifted, false_inst);
        coalescing->underlying_result_type = expr_to_lift->result_type();
        lifted = coalescing;
    }
    return lifted;
}

NullableLiftingTransform::LiftResult NullableLiftingTransform::do_lift(ILInstruction* inst) {
    ILVariable* input_var = nullptr;
    ILVariable* v = nullptr;
    const int var_count = static_cast<int>(nullable_vars_.size());
    if (match_get_value_or_default(inst, &input_var)) {
        // n.GetValueOrDefault() lifted => n.
        util::BitSet found_indices(var_count);
        for (int i = 0; i < var_count; i++) {
            if (nullable_vars_[static_cast<std::size_t>(i)] == input_var) {
                found_indices.set(i);
            }
        }
        if (found_indices.any()) {
            LiftResult result;
            result.inst = with_il_range(make<LdLoc>(input_var), inst);
            result.bits = found_indices;
            return result;
        }
        return {};
    } else if (auto* conv = as<Conv>(inst)) {
        LiftResult arg = do_lift(conv->argument());
        if (arg.inst != nullptr) {
            if (conv->has_direct_flag(InstructionFlags::MayThrow) && !arg.bits->all(0, var_count)) {
                // Cannot execute potentially-throwing instruction unless all the nullableVars are arguments to the instruction (thus
                // causing it not to throw when any of them is null).
                return {};
            }
            auto* new_inst = with_il_range(make<Conv>(arg.inst, conv->input_type, conv->input_sign, conv->target_type, conv->check_for_overflow, true), conv);
            LiftResult result;
            result.inst = new_inst;
            result.bits = std::move(arg.bits);
            return result;
        }
    } else if (auto* bitnot = as<BitNot>(inst)) {
        LiftResult arg = do_lift(bitnot->argument());
        if (arg.inst != nullptr) {
            auto* new_inst = with_il_range(make<BitNot>(arg.inst, true, bitnot->result_type()), bitnot);
            LiftResult result;
            result.inst = new_inst;
            result.bits = std::move(arg.bits);
            return result;
        }
    } else if (auto* binary = as<BinaryNumericInstruction>(inst)) {
        LiftBinaryResult lifted = do_lift_binary(binary->left(), binary->right(), &typesystem::SpecialType::unknown_type(), &typesystem::SpecialType::unknown_type());
        if (lifted.left != nullptr && lifted.right != nullptr) {
            if (binary->has_direct_flag(InstructionFlags::MayThrow) && !lifted.bits->all(0, var_count)) {
                // Cannot execute potentially-throwing instruction unless all the nullableVars are arguments to the instruction (thus
                // causing it not to throw when any of them is null).
                return {};
            }
            auto* new_inst = with_il_range(make<BinaryNumericInstruction>(binary->op, lifted.left, lifted.right, binary->left_input_type,
                                                                          binary->right_input_type, binary->check_for_overflow, binary->sign, true),
                                           binary);
            LiftResult result;
            result.inst = new_inst;
            result.bits = std::move(lifted.bits);
            return result;
        }
    } else if (auto* comp = as<Comp>(inst);
               comp != nullptr && !comp->is_lifted() && comp->kind() == ComparisonKind::Equality &&
               match_get_value_or_default(comp->left(), &v) && contains(nullable_vars_, v) &&
               typesystem::is_known_type(nullable_type::get_underlying_type(v->type()), KnownTypeCode::Boolean) && comp->right()->match_ldc_i4(0)) {
        // C# doesn't support ComparisonLiftingKind.ThreeValuedLogic, except for operator! on bool?.
        LiftResult arg = do_lift(comp->left());
        assert(arg.inst != nullptr);
        ILInstruction* right_clone = comp->right()->clone();
        auto* new_inst = with_il_range(
            make<Comp>(comp->kind(), ComparisonLiftingKind::ThreeValuedLogic, comp->input_type, comp->sign, arg.inst, right_clone), comp);
        LiftResult result;
        result.inst = new_inst;
        result.bits = std::move(arg.bits);
        return result;
    } else if (auto* call = as<Call>(inst); call != nullptr && call->method->is_operator()) {
        // Lifted user-defined operators, except for comparison operators (as those return bool, not bool?)
        const IMethod* lifted_operator = CSharpOperators::lift_user_defined_operator(call->method);
        if (lifted_operator == nullptr || !nullable_type::is_nullable(lifted_operator->return_type())) return {};
        std::vector<ILInstruction*> new_args;
        std::optional<util::BitSet> new_bits;
        if (call->arguments().count() == 1) {
            LiftResult arg = do_lift(call->arguments()[0]);
            new_args = {arg.inst};
            new_bits = std::move(arg.bits);
        } else if (call->arguments().count() == 2) {
            LiftBinaryResult lifted = do_lift_binary(call->arguments()[0], call->arguments()[1], call->method->parameters()[0]->type(),
                                                     call->method->parameters()[1]->type());
            new_args = {lifted.left, lifted.right};
            new_bits = std::move(lifted.bits);
        } else {
            return {};
        }
        if (!new_bits || !new_bits->all(0, var_count)) {
            // all nullable vars must be involved when calling a method (side effect)
            return {};
        }
        auto* new_inst = make<Call>(lifted_operator);
        new_inst->constrained_to = call->constrained_to;
        new_inst->is_tail = call->is_tail;
        new_inst->il_stack_was_empty = call->il_stack_was_empty;
        with_il_range(new_inst, call);
        new_inst->arguments().add_range(new_args);
        LiftResult result;
        result.inst = new_inst;
        result.bits = std::move(new_bits);
        return result;
    }
    return {};
}

NullableLiftingTransform::LiftBinaryResult NullableLiftingTransform::do_lift_binary(ILInstruction* lhs, ILInstruction* rhs,
                                                                                    const IType* left_expected_type,
                                                                                    const IType* right_expected_type) {
    LiftResult left = do_lift(lhs);
    LiftResult right = do_lift(rhs);
    if (left.inst != nullptr && right.inst == nullptr && SemanticHelper::is_pure(rhs->flags())) {
        // Embed non-nullable pure expression in lifted expression.
        right.inst = new_nullable(rhs->clone(), right_expected_type);
    }
    if (left.inst == nullptr && right.inst != nullptr && SemanticHelper::is_pure(lhs->flags())) {
        // Embed non-nullable pure expression in lifted expression.
        left.inst = new_nullable(lhs->clone(), left_expected_type);
    }
    if (left.inst != nullptr && right.inst != nullptr) {
        LiftBinaryResult result;
        result.left = left.inst;
        result.right = right.inst;
        // var bits = leftBits ?? rightBits; if (rightBits != null) bits.UnionWith(rightBits);
        if (left.bits) {
            result.bits = std::move(left.bits);
            if (right.bits) result.bits->union_with(*right.bits);
        } else {
            result.bits = std::move(right.bits);  // upstream unions the set with itself here, which changes nothing
        }
        return result;
    }
    return {};
}

ILInstruction* NullableLiftingTransform::new_nullable(ILInstruction* inst, const IType* underlying_type) {
    if (underlying_type == &typesystem::SpecialType::unknown_type()) return inst;
    const typesystem::ITypeDefinition* nullable = context_->type_system().find_type(KnownTypeCode::NullableOfT)->get_definition();
    const IMethod* ctor = nullptr;
    if (nullable != nullptr) {
        for (const IMethod* m : nullable->methods()) {
            if (m->is_constructor() && m->parameters().size() == 1) {
                ctor = m;
                break;
            }
        }
    }
    if (ctor != nullptr) {
        ctor = ctor->specialize_method(TypeParameterSubstitution(std::vector<const IType*>{underlying_type}, std::nullopt));
        auto* new_obj = make<NewObj>(ctor);
        new_obj->arguments().add(inst);
        return new_obj;
    } else {
        return inst;
    }
}

// ---- Match...Call -------------------------------------------------------------------------------------------------------------------

bool NullableLiftingTransform::match_has_value_call(ILInstruction* inst, ILInstruction** arg) {
    *arg = nullptr;
    auto* call = as<Call>(inst);
    if (call == nullptr) return false;
    if (call->arguments().count() != 1) return false;
    if (call->method->name() != "get_HasValue") return false;
    const typesystem::ITypeDefinition* definition = call->method->declaring_type_definition();
    if (definition == nullptr || definition->known_type_code() != KnownTypeCode::NullableOfT) return false;
    *arg = call->arguments()[0];
    return true;
}

bool NullableLiftingTransform::match_has_value_call(ILInstruction* inst, ILVariable** v) {
    ILInstruction* arg = nullptr;
    if (match_has_value_call(inst, &arg)) {
        return arg->match_ld_loca(v);
    }
    *v = nullptr;
    return false;
}

bool NullableLiftingTransform::match_has_value_call(ILInstruction* inst, const ILVariable* v) {
    ILVariable* v2 = nullptr;
    return match_has_value_call(inst, &v2) && v == v2;
}

bool NullableLiftingTransform::match_negated_has_value_call(ILInstruction* inst, const ILVariable* v) {
    ILInstruction* arg = nullptr;
    return inst->match_logic_not(&arg) && match_has_value_call(arg, v);
}

bool NullableLiftingTransform::match_nullable_ctor(ILInstruction* inst, const IType** underlying_type, ILInstruction** arg) {
    *underlying_type = nullptr;
    *arg = nullptr;
    auto* newobj = as<NewObj>(inst);
    if (newobj == nullptr) return false;
    if (!newobj->method->is_constructor() || newobj->arguments().count() != 1) return false;
    const typesystem::ITypeDefinition* definition = newobj->method->declaring_type_definition();
    if (definition == nullptr || definition->known_type_code() != KnownTypeCode::NullableOfT) return false;
    *arg = newobj->arguments()[0];
    *underlying_type = nullable_type::get_underlying_type(newobj->method->declaring_type());
    return true;
}

bool NullableLiftingTransform::match_get_value_or_default(ILInstruction* inst, ILInstruction** arg) {
    *arg = nullptr;
    auto* call = as<Call>(inst);
    if (call == nullptr) return false;
    if (call->method->name() != "GetValueOrDefault" || call->arguments().count() != 1) return false;
    const typesystem::ITypeDefinition* definition = call->method->declaring_type_definition();
    if (definition == nullptr || definition->known_type_code() != KnownTypeCode::NullableOfT) return false;
    *arg = call->arguments()[0];
    return true;
}

bool NullableLiftingTransform::match_get_value_or_default(ILInstruction* inst, ILInstruction** nullable_value, ILInstruction** fallback) {
    *nullable_value = nullptr;
    *fallback = nullptr;
    auto* call = as<Call>(inst);
    if (call == nullptr) return false;
    if (call->method->name() != "GetValueOrDefault" || call->arguments().count() != 2) return false;
    const typesystem::ITypeDefinition* definition = call->method->declaring_type_definition();
    if (definition == nullptr || definition->known_type_code() != KnownTypeCode::NullableOfT) return false;
    *nullable_value = call->arguments()[0];
    *fallback = call->arguments()[1];
    return true;
}

bool NullableLiftingTransform::match_get_value_or_default(ILInstruction* inst, ILVariable** v) {
    *v = nullptr;
    ILInstruction* arg = nullptr;
    return match_get_value_or_default(inst, &arg) && arg->match_ld_loca(v);
}

bool NullableLiftingTransform::match_get_value_or_default(ILInstruction* inst, const ILVariable* v) {
    ILVariable* v2 = nullptr;
    return match_get_value_or_default(inst, &v2) && v == v2;
}

bool NullableLiftingTransform::match_null(ILInstruction* inst, const IType** underlying_type) {
    *underlying_type = nullptr;
    const IType* type = nullptr;
    if (inst->match_default_value(&type)) {
        *underlying_type = nullable_type::get_underlying_type(type);
        return nullable_type::is_nullable(type);
    }
    *underlying_type = nullptr;
    return false;
}

bool NullableLiftingTransform::match_null(ILInstruction* inst, const IType* underlying_type) {
    const IType* utype = nullptr;
    return match_null(inst, &utype) && utype->equals(underlying_type);
}

void NullableLiftingStatementTransform::run(Block* block, int pos, StatementTransformContext& context) {
    NullableLiftingTransform(context).run_statements(block, pos);
}

}  // namespace cb::dotnet::decompiler::il::transforms
