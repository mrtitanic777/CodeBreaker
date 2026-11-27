// dotnet/decompiler/il/transforms/user_defined_logic_transform.cxx -- see user_defined_logic_transform.hpp.
#include "dotnet/decompiler/il/transforms/user_defined_logic_transform.hpp"

#include <cassert>

#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;

void UserDefinedLogicTransform::run(Block* block, int pos, StatementTransformContext& context) {
    if (legacy_pattern(block, pos, context)) return;
    if (roslyn_optimized(block, pos, context)) return;
}

bool UserDefinedLogicTransform::roslyn_optimized(Block* block, int pos, StatementTransformContext& context) {
    // Roslyn, optimized pattern in combination with return statement:
    //   if (logic.not(call op_False(ldloc lhsVar))) leave IL_0000 (call op_BitwiseAnd(ldloc lhsVar, rhsInst))
    //   leave IL_0000(ldloc lhsVar)
    // ->
    //   user.logic op_BitwiseAnd(ldloc lhsVar, rhsInst)
    ILInstruction* condition;
    ILInstruction* true_inst;
    ILInstruction* false_inst;
    if (!block->instructions[pos]->match_if_instruction_positive_condition(&condition, &true_inst, &false_inst)) return false;
    if (true_inst->op_code == OpCode::Nop) {
        true_inst = block->instructions[pos + 1];
    } else if (false_inst->op_code == OpCode::Nop) {
        false_inst = block->instructions[pos + 1];
    } else {
        return false;
    }
    ILInstruction* true_value;
    ILInstruction* false_value;
    if (true_inst->match_return(&true_value) && false_inst->match_return(&false_value)) {
        ILInstruction* transformed = transform(condition, true_value, false_value);
        if (transformed == nullptr) {
            transformed = transform_dynamic(condition, true_value, false_value);
        }
        if (transformed != nullptr) {
            context.step("User-defined short-circuiting logic operator (optimized return)", condition);
            static_cast<Leave*>(block->instructions[pos + 1])->set_value(transformed);
            block->instructions.remove_at(pos);
            context.end_step(transformed);
            return true;
        }
    }
    return false;
}

bool UserDefinedLogicTransform::legacy_pattern(Block* block, int pos, StatementTransformContext& context) {
    // Legacy csc pattern:
    //   stloc s(lhsInst)
    //   if (logic.not(call op_False(ldloc s))) Block {
    //     stloc s(call op_BitwiseAnd(ldloc s, rhsInst))
    //   }
    // ->
    //   stloc s(user.logic op_BitwiseAnd(lhsInst, rhsInst))
    ILVariable* s;
    ILInstruction* lhs_inst;
    if (!block->instructions[pos]->match_st_loc(&s, &lhs_inst)) return false;
    if (!(s->kind() == VariableKind::StackSlot)) return false;
    auto* if_inst = as<IfInstruction>(block->instructions[pos + 1]);
    if (if_inst == nullptr) return false;
    ILInstruction* condition;
    if (!if_inst->condition()->match_logic_not(&condition)) return false;
    ILVariable* s2;
    std::string condition_method_name;
    if (!(match_condition(condition, &s2, &condition_method_name) && s2 == s)) return false;
    if (if_inst->false_inst()->op_code != OpCode::Nop) return false;
    ILInstruction* true_inst = Block::unwrap(if_inst->true_inst());
    ILInstruction* store_value;
    if (!true_inst->match_st_loc(s, &store_value)) return false;
    if (auto* call = as<Call>(store_value)) {
        if (!match_bitwise_call(call, s, condition_method_name)) return false;
        if (s->is_used_within(call->arguments()[1])) return false;
        context.step("User-defined short-circuiting logic operator (legacy pattern)", condition);
        auto* user_logic_op = with_il_range(make<UserDefinedLogicOperator>(call->method, lhs_inst, call->arguments()[1]), call);
        static_cast<StLoc*>(block->instructions[pos])->set_value(user_logic_op);
        block->instructions.remove_at(pos + 1);
        context.end_step(user_logic_op);
        context.request_rerun();  // the 'stloc s' may now be eligible for inlining
        return true;
    }
    return false;
}

bool UserDefinedLogicTransform::match_condition(ILInstruction* condition, ILVariable** v, std::string* name) {
    *v = nullptr;
    name->clear();
    auto* call = as<Call>(condition);
    if (!(call != nullptr && call->method->is_operator() && call->arguments().count() == 1 && !call->is_lifted())) return false;
    *name = call->method->name();
    if (!(*name == "op_True" || *name == "op_False")) return false;
    return call->arguments()[0]->match_ld_loc(v);
}

bool UserDefinedLogicTransform::match_bitwise_call(Call* call, ILVariable* v, const std::string& condition_method_name) {
    if (!(call != nullptr && call->method->is_operator() && call->arguments().count() == 2 && !call->is_lifted())) return false;
    if (!call->arguments()[0]->match_ld_loc(v)) return false;

    return (condition_method_name == "op_False" && call->method->name() == "op_BitwiseAnd") ||
           (condition_method_name == "op_True" && call->method->name() == "op_BitwiseOr");
}

ILInstruction* UserDefinedLogicTransform::transform(ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst) {
    ILVariable* lhs_var;
    std::string condition_method_name;
    if (!match_condition(condition, &lhs_var, &condition_method_name)) return nullptr;
    if (!true_inst->match_ld_loc(lhs_var)) return nullptr;
    auto* call = as<Call>(false_inst);
    if (!match_bitwise_call(call, lhs_var, condition_method_name)) return nullptr;

    auto* result = make<UserDefinedLogicOperator>(call->method, call->arguments()[0], call->arguments()[1]);
    result->add_il_range(*condition);
    result->add_il_range(*true_inst);
    result->add_il_range(*call);
    return result;
}

ILInstruction* UserDefinedLogicTransform::transform_dynamic(ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst) {
    // Check condition:
    ExpressionType unary_op;
    ILVariable* lhs_var;
    std::string operator_method_name;
    if (condition->match_ld_loc(&lhs_var)) {
        // if (ldloc lhsVar) box bool(ldloc lhsVar) else dynamic.binary.operator.logic Or(ldloc lhsVar, rhsInst)
        //    -> dynamic.logic.operator OrElse(ldloc lhsVar, rhsInst)
        auto* box = as<Box>(true_inst);
        auto* box2 = as<Box>(false_inst);
        if (box != nullptr && typesystem::is_known_type(box->type(), KnownTypeCode::Boolean)) {
            unary_op = ExpressionType::IsTrue;
            true_inst = box->argument();
        } else if (box2 != nullptr && typesystem::is_known_type(box2->type(), KnownTypeCode::Boolean)) {
            // negate condition and swap true/false
            unary_op = ExpressionType::IsFalse;
            false_inst = true_inst;
            true_inst = box2->argument();
        } else {
            return nullptr;
        }
    } else if (auto* unary = as<DynamicUnaryOperatorInstruction>(condition)) {
        // if (dynamic.unary.operator IsFalse(ldloc lhsVar)) ldloc lhsVar else dynamic.binary.operator.logic And(ldloc lhsVar, rhsInst)
        //    -> dynamic.logic.operator AndAlso(ldloc lhsVar, rhsInst)
        unary_op = unary->operation();
        if (!unary->operand()->match_ld_loc(&lhs_var)) return nullptr;
    } else if (match_condition(condition, &lhs_var, &operator_method_name)) {
        // if (call op_False(ldloc s)) box S(ldloc s) else dynamic.binary.operator.logic And(ldloc s, rhsInst))
        if (operator_method_name == "op_True") {
            unary_op = ExpressionType::IsTrue;
        } else {
            assert(operator_method_name == "op_False");
            unary_op = ExpressionType::IsFalse;
        }
        const IType* call_param_type = typesystem::skip_modifiers(static_cast<Call*>(condition)->method->parameters()[0]->type());
        if (call_param_type->is_reference_type() == false) {
            // If lhs is a value type, eliminate the boxing instruction.
            auto* box = as<Box>(true_inst);
            if (box != nullptr && typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(box->type(), call_param_type)) {
                true_inst = box->argument();
            } else if (true_inst->op_code == OpCode::LdcI4) {
                // special case, handled below in 'check trueInst'
            } else {
                return nullptr;
            }
        }
    } else {
        return nullptr;
    }

    // Check trueInst:
    DynamicUnaryOperatorInstruction* rhs_unary;
    if (true_inst->match_ld_loc(lhs_var)) {
        // OK, typical pattern where the expression evaluates to 'dynamic'
        rhs_unary = nullptr;
    } else if (true_inst->match_ldc_i4(1) && unary_op == ExpressionType::IsTrue) {
        // logic.or(IsTrue(lhsVar), IsTrue(lhsVar | rhsInst))
        // => IsTrue(lhsVar || rhsInst)
        rhs_unary = as<DynamicUnaryOperatorInstruction>(false_inst);
        if (rhs_unary != nullptr) {
            if (rhs_unary->operation() != ExpressionType::IsTrue) return nullptr;
            false_inst = rhs_unary->operand();
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }

    ExpressionType expected_bitop;
    ExpressionType logic_op;
    if (unary_op == ExpressionType::IsFalse) {
        expected_bitop = ExpressionType::And;
        logic_op = ExpressionType::AndAlso;
    } else if (unary_op == ExpressionType::IsTrue) {
        expected_bitop = ExpressionType::Or;
        logic_op = ExpressionType::OrElse;
    } else {
        return nullptr;
    }

    // Check falseInst:
    auto* binary = as<DynamicBinaryOperatorInstruction>(false_inst);
    if (binary == nullptr) return nullptr;
    if (binary->operation() != expected_bitop) return nullptr;
    if (!binary->left()->match_ld_loc(lhs_var)) return nullptr;
    auto* logic_inst = with_il_range(make<DynamicLogicOperatorInstruction>(binary->binder_flags(), logic_op, binary->calling_context(),
                                                                          binary->left_argument_info(), binary->left(), binary->right_argument_info(),
                                                                          binary->right()),
                                     binary);
    if (rhs_unary != nullptr) {
        rhs_unary->set_operand(logic_inst);
        return rhs_unary;
    } else {
        return logic_inst;
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
