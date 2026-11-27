// dotnet/decompiler/il/control_flow/symbolic_execution.cpp -- see symbolic_execution.hpp.
#include "dotnet/decompiler/il/control_flow/symbolic_execution.hpp"

#include <algorithm>

#include "dotnet/decompiler/il/control_flow/switch_analysis.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

namespace {

// The int arithmetic of C#'s unchecked(-x) and unchecked(a - b).
int unchecked_neg(int x) { return static_cast<int>(0u - static_cast<std::uint32_t>(x)); }
int unchecked_sub(int a, int b) { return static_cast<int>(static_cast<std::uint32_t>(a) - static_cast<std::uint32_t>(b)); }

}  // namespace

const char* to_string(SymbolicValueType type) {
    switch (type) {
        case SymbolicValueType::Unknown: return "Unknown";
        case SymbolicValueType::IntegerConstant: return "IntegerConstant";
        case SymbolicValueType::State: return "State";
        case SymbolicValueType::This: return "This";
        case SymbolicValueType::StateInSet: return "StateInSet";
    }
    return "";
}

SymbolicValue SymbolicValue::as_bool() const {
    if (type == SymbolicValueType::State) {
        // convert state integer to bool:
        // if (state + c) = if (state + c != 0) = if (state != -c)
        return SymbolicValue(SymbolicValueType::StateInSet, util::LongSet(static_cast<std::int64_t>(unchecked_neg(constant))).invert());
    }
    return *this;
}

std::string SymbolicValue::to_string() const {
    return std::string("[SymbolicValue ") + control_flow::to_string(type) + ": " + std::to_string(constant) + "]";
}

void SymbolicEvaluationContext::add_state_variable(ILVariable* v) {
    if (std::find(state_variables_.begin(), state_variables_.end(), v) == state_variables_.end()) state_variables_.push_back(v);
}

SymbolicValue SymbolicEvaluationContext::eval(ILInstruction* inst) {
    const SymbolicValue failed(SymbolicValueType::Unknown);
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    ILVariable* loaded_variable = nullptr;
    std::int32_t value = 0;
    auto* bni = as<BinaryNumericInstruction>(inst);
    if (bni != nullptr && bni->op == BinaryNumericOperator::Sub && (legacy_visual_basic_ || !bni->check_for_overflow)) {
        SymbolicValue left = eval(bni->left());
        SymbolicValue right = eval(bni->right());
        if (left.type != SymbolicValueType::State && left.type != SymbolicValueType::IntegerConstant) return failed;
        if (right.type != SymbolicValueType::IntegerConstant) return failed;
        return SymbolicValue(left.type, unchecked_sub(left.constant, right.constant));
    } else if (inst->match_ld_fld(&target, &field)) {
        if (eval(target).type != SymbolicValueType::This) return failed;
        if (field->member_definition() != state_field_) return failed;
        return SymbolicValue(SymbolicValueType::State);
    } else if (inst->match_ld_loc(&loaded_variable)) {
        if (std::find(state_variables_.begin(), state_variables_.end(), loaded_variable) != state_variables_.end())
            return SymbolicValue(SymbolicValueType::State);
        else if (loaded_variable->kind() == VariableKind::Parameter && loaded_variable->index() && *loaded_variable->index() < 0)
            return SymbolicValue(SymbolicValueType::This);
        else
            return failed;
    } else if (inst->match_ldc_i4(&value)) {
        return SymbolicValue(SymbolicValueType::IntegerConstant, value);
    } else if (auto* comp = as<Comp>(inst)) {
        SymbolicValue left = eval(comp->left());
        SymbolicValue right = eval(comp->right());
        if (left.type == SymbolicValueType::State && right.type == SymbolicValueType::IntegerConstant) {
            // bool: (state + left.Constant == right.Constant)
            util::LongSet true_sums = SwitchAnalysis::make_set_where_comparison_is_true(comp->kind(), right.constant, comp->sign);
            // symbolic value is true iff trueSums.Contains(state + left.Constant)
            util::LongSet true_states = true_sums.add_offset(static_cast<std::int64_t>(unchecked_neg(left.constant)));
            // symbolic value is true iff trueStates.Contains(state)
            return SymbolicValue(SymbolicValueType::StateInSet, std::move(true_states));
        } else if (left.type == SymbolicValueType::StateInSet && right.type == SymbolicValueType::IntegerConstant) {
            if (comp->kind() == ComparisonKind::Equality && right.constant == 0) {
                // comp((x in set) == 0) ==> x not in set
                return SymbolicValue(SymbolicValueType::StateInSet, left.value_set.invert());
            } else if (comp->kind() == ComparisonKind::Inequality && right.constant != 0) {
                // comp((x in set) != 0) => x in set
                return SymbolicValue(SymbolicValueType::StateInSet, left.value_set);
            } else {
                return failed;
            }
        } else {
            return failed;
        }
    } else {
        return failed;
    }
}

}  // namespace cb::dotnet::decompiler::il::control_flow
