// dotnet/decompiler/il/pattern_matching.cpp -- the hand-written Match* methods of ILInstruction that are not about control flow
// (ILSpy's Instructions/PatternMatching.cs): constants, locals, comparisons, field access, binary numeric instructions, conversions.
// An `out` parameter is a pointer, set to null (or zero) when the match fails. The matchers of ldobj with a given type and of ldlen are
// written with the memory instructions' generated ones (memory_instructions.cpp); the control-flow ones with control_flow_instructions.cpp.
#include <optional>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il {

namespace {
constexpr std::optional<bool> yes = true;
}

bool ILInstruction::match_ldc_i4(std::int32_t value) const {
    auto* inst = as<LdcI4>(this);
    return inst != nullptr && inst->value == value;
}

bool ILInstruction::match_ldc_f4(float value) const {
    float v = 0;
    return match_ldc_f4(&v) && v == value;
}

bool ILInstruction::match_ldc_f8(double value) const {
    double v = 0;
    return match_ldc_f8(&v) && v == value;
}

// Matches ldc.i4, ldc.i8, and extending conversions.
bool ILInstruction::match_ldc_i(std::int64_t* value) const {
    if (match_ldc_i8(value)) return true;
    std::int32_t int_val = 0;
    if (match_ldc_i4(&int_val)) {
        *value = int_val;
        return true;
    }
    if (auto* conv = as<Conv>(this)) {
        if (conv->kind == ConversionKind::SignExtend) {
            return conv->argument()->match_ldc_i(value);
        } else if (conv->kind == ConversionKind::ZeroExtend && conv->input_type == StackType::I4) {
            if (conv->argument()->match_ldc_i(value)) {
                // clear top 32 bits
                *value &= 0xFFFFFFFFll;
                return true;
            }
        }
    }
    return false;
}

bool ILInstruction::match_ldc_i(std::int64_t value) const {
    std::int64_t v = 0;
    return match_ldc_i(&v) && v == value;
}

bool ILInstruction::match_ld_loc(const ILVariable* variable) const {
    auto* inst = as<LdLoc>(this);
    return inst != nullptr && inst->variable() == variable;
}

bool ILInstruction::match_ld_loca(const ILVariable* variable) const {
    auto* inst = as<LdLoca>(this);
    return inst != nullptr && inst->variable() == variable;
}

// Matches either ldloc (if the variable is a reference type), or ldloca (otherwise).
bool ILInstruction::match_ld_loc_ref(const ILVariable* variable) const {
    ILVariable* v = nullptr;
    return match_ld_loc_ref(&v) && v == variable;
}

bool ILInstruction::match_ld_loc_ref(ILVariable** variable) const {
    if (auto* ldloc = as<LdLoc>(this)) {
        *variable = ldloc->variable();
        return (*variable)->type()->is_reference_type() == yes;
    }
    if (auto* ldloca = as<LdLoca>(this)) {
        *variable = ldloca->variable();
        return (*variable)->type()->is_reference_type() != yes || (*variable)->type()->kind() == typesystem::TypeKind::TypeParameter;
    }
    *variable = nullptr;
    return false;
}

bool ILInstruction::match_ld_this() const {
    auto* inst = as<LdLoc>(this);
    return inst != nullptr && inst->variable()->kind() == VariableKind::Parameter && inst->variable()->index().has_value() &&
           *inst->variable()->index() < 0;
}

bool ILInstruction::match_st_loc(ILVariable** variable) const {
    if (auto* inst = as<StLoc>(this)) {
        *variable = inst->variable();
        return true;
    }
    *variable = nullptr;
    return false;
}

bool ILInstruction::match_st_loc(const ILVariable* variable, ILInstruction** value) const {
    auto* inst = as<StLoc>(this);
    if (inst != nullptr && inst->variable() == variable) {
        *value = inst->value();
        return true;
    }
    *value = nullptr;
    return false;
}

// Matches comp(left == right) or logic.not(comp(left != right)).
bool ILInstruction::match_comp_equals(ILInstruction** left, ILInstruction** right) const {
    const ILInstruction* this_inst = this;
    auto comp_kind = ComparisonKind::Equality;
    ILInstruction* arg = nullptr;
    while (this_inst->match_logic_not(&arg) && is<Comp>(arg)) {
        this_inst = arg;
        if (comp_kind == ComparisonKind::Equality)
            comp_kind = ComparisonKind::Inequality;
        else
            comp_kind = ComparisonKind::Equality;
    }
    if (auto* comp = as<Comp>(this_inst); comp != nullptr && comp->kind() == comp_kind && !comp->is_lifted()) {
        *left = comp->left();
        *right = comp->right();
        return true;
    }
    *left = nullptr;
    *right = nullptr;
    return false;
}

// Matches 'comp(arg == ldnull)'
bool ILInstruction::match_comp_equals_null(ILInstruction** arg) const {
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    if (!match_comp_equals(&left, &right)) {
        *arg = nullptr;
        return false;
    }
    if (right->match_ld_null()) {
        *arg = left;
        return true;
    }
    if (left->match_ld_null()) {
        *arg = right;
        return true;
    }
    *arg = nullptr;
    return false;
}

// Matches 'comp(arg != ldnull)'
bool ILInstruction::match_comp_not_equals_null(ILInstruction** arg) const {
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    if (!match_comp_not_equals(&left, &right)) {
        *arg = nullptr;
        return false;
    }
    if (right->match_ld_null()) {
        *arg = left;
        return true;
    }
    if (left->match_ld_null()) {
        *arg = right;
        return true;
    }
    *arg = nullptr;
    return false;
}

// Matches comp(left != right) or logic.not(comp(left == right)).
bool ILInstruction::match_comp_not_equals(ILInstruction** left, ILInstruction** right) const {
    const ILInstruction* this_inst = this;
    auto comp_kind = ComparisonKind::Inequality;
    ILInstruction* arg = nullptr;
    while (this_inst->match_logic_not(&arg) && is<Comp>(arg)) {
        this_inst = arg;
        if (comp_kind == ComparisonKind::Equality)
            comp_kind = ComparisonKind::Inequality;
        else
            comp_kind = ComparisonKind::Equality;
    }
    if (auto* comp = as<Comp>(this_inst); comp != nullptr && comp->kind() == comp_kind && !comp->is_lifted()) {
        *left = comp->left();
        *right = comp->right();
        return true;
    }
    *left = nullptr;
    *right = nullptr;
    return false;
}

bool ILInstruction::match_ld_fld(ILInstruction** target, const IField** field) const {
    if (auto* ldobj = as<LdObj>(this)) {
        auto* ldflda = as<LdFlda>(ldobj->target());
        if (ldflda != nullptr && ldobj->unaligned_prefix() == 0 && !ldobj->is_volatile()) {
            *field = ldflda->field();
            ILInstruction* address_of_target = nullptr;
            const IType* ignored = nullptr;
            if ((*field)->declaring_type()->is_reference_type() == yes || !ldflda->target()->match_address_of(&address_of_target, &ignored))
                *target = ldflda->target();
            else
                *target = address_of_target;
            return true;
        }
    }
    *target = nullptr;
    *field = nullptr;
    return false;
}

bool ILInstruction::match_lds_fld(const IField** field) const {
    if (auto* ldobj = as<LdObj>(this)) {
        auto* ldsflda = as<LdsFlda>(ldobj->target());
        if (ldsflda != nullptr && ldobj->unaligned_prefix() == 0 && !ldobj->is_volatile()) {
            *field = ldsflda->field();
            return true;
        }
    }
    *field = nullptr;
    return false;
}

bool ILInstruction::match_lds_fld(const IField* field) const {
    const IField* f = nullptr;
    return match_lds_fld(&f) && f->equals(field);
}

bool ILInstruction::match_sts_fld(const IField** field, ILInstruction** value) const {
    if (auto* stobj = as<StObj>(this)) {
        auto* ldsflda = as<LdsFlda>(stobj->target());
        if (ldsflda != nullptr && stobj->unaligned_prefix() == 0 && !stobj->is_volatile()) {
            *field = ldsflda->field();
            *value = stobj->value();
            return true;
        }
    }
    *field = nullptr;
    *value = nullptr;
    return false;
}

bool ILInstruction::match_st_fld(ILInstruction** target, const IField** field, ILInstruction** value) const {
    if (auto* stobj = as<StObj>(this)) {
        auto* ldflda = as<LdFlda>(stobj->target());
        if (ldflda != nullptr && stobj->unaligned_prefix() == 0 && !stobj->is_volatile()) {
            *target = ldflda->target();
            *field = ldflda->field();
            *value = stobj->value();
            return true;
        }
    }
    *target = nullptr;
    *field = nullptr;
    *value = nullptr;
    return false;
}

bool ILInstruction::match_binary_numeric_instruction(BinaryNumericOperator op) const {
    auto* inst = as<BinaryNumericInstruction>(this);
    return inst != nullptr && inst->op == op;
}

bool ILInstruction::match_binary_numeric_instruction(BinaryNumericOperator op, ILInstruction** left, ILInstruction** right) const {
    auto* inst = as<BinaryNumericInstruction>(this);
    if (inst != nullptr && inst->op == op) {
        *left = inst->left();
        *right = inst->right();
        return true;
    }
    *left = nullptr;
    *right = nullptr;
    return false;
}

bool ILInstruction::match_binary_numeric_instruction(BinaryNumericOperator* op, ILInstruction** left, ILInstruction** right) const {
    auto* inst = as<BinaryNumericInstruction>(this);
    if (inst != nullptr) {
        *op = inst->op;
        *left = inst->left();
        *right = inst->right();
        return true;
    }
    *op = BinaryNumericOperator::None;
    *left = nullptr;
    *right = nullptr;
    return false;
}

bool ILInstruction::match_default_or_null_or_zero() const {
    switch (op_code) {
        case OpCode::LdNull:
        case OpCode::DefaultValue:
            return true;
        case OpCode::LdcF4: return static_cast<const LdcF4*>(this)->value == 0;
        case OpCode::LdcF8: return static_cast<const LdcF8*>(this)->value == 0;
        case OpCode::LdcI4: return static_cast<const LdcI4*>(this)->value == 0;
        case OpCode::LdcI8: return static_cast<const LdcI8*>(this)->value == 0;
        default: return false;
    }
}

// If this instruction is a conversion of the specified kind, its argument (Conv overrides this); otherwise the instruction itself.
ILInstruction* ILInstruction::unwrap_conv(ConversionKind) { return this; }

}  // namespace cb::dotnet::decompiler::il
