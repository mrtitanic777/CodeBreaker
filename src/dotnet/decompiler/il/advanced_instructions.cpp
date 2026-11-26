// dotnet/decompiler/il/advanced_instructions.cpp -- see advanced_instructions.hpp.
#include "dotnet/decompiler/il/advanced_instructions.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <stdexcept>
#include <unordered_set>

#include "core/error.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/patterns.hpp"
#include "dotnet/decompiler/il/pointer_arithmetic_offset.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il {

namespace {

[[noreturn]] void index_out_of_range() { throw std::out_of_range("child index"); }

// C#'s `(T)value` for a child that must be of that class.
template <class T>
T* checked_cast(ILInstruction* value) {
    if (value == nullptr) return nullptr;
    T* result = as<T>(value);
    if (result == nullptr) throw std::invalid_argument("child instruction of the wrong class");
    return result;
}

std::string to_lower(std::string text) {
    for (char& c : text) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return text;
}

}  // namespace

// ---- enums

std::string to_string(ExpressionType type) {
    static constexpr std::array<const char*, 85> names = {
        "Add", "AddChecked", "And", "AndAlso", "ArrayLength", "ArrayIndex", "Call", "Coalesce", "Conditional", "Constant", "Convert",
        "ConvertChecked", "Divide", "Equal", "ExclusiveOr", "GreaterThan", "GreaterThanOrEqual", "Invoke", "Lambda", "LeftShift", "LessThan",
        "LessThanOrEqual", "ListInit", "MemberAccess", "MemberInit", "Modulo", "Multiply", "MultiplyChecked", "Negate", "UnaryPlus",
        "NegateChecked", "New", "NewArrayInit", "NewArrayBounds", "Not", "NotEqual", "Or", "OrElse", "Parameter", "Power", "Quote",
        "RightShift", "Subtract", "SubtractChecked", "TypeAs", "TypeIs", "Assign", "Block", "DebugInfo", "Decrement", "Dynamic", "Default",
        "Extension", "Goto", "Increment", "Index", "Label", "RuntimeVariables", "Loop", "Switch", "Throw", "Try", "Unbox", "AddAssign",
        "AndAssign", "DivideAssign", "ExclusiveOrAssign", "LeftShiftAssign", "ModuloAssign", "MultiplyAssign", "OrAssign", "PowerAssign",
        "RightShiftAssign", "SubtractAssign", "AddAssignChecked", "MultiplyAssignChecked", "SubtractAssignChecked", "PreIncrementAssign",
        "PreDecrementAssign", "PostIncrementAssign", "PostDecrementAssign", "TypeEqual", "OnesComplement", "IsTrue", "IsFalse",
    };
    auto value = static_cast<std::int32_t>(type);
    if (value >= 0 && value < static_cast<std::int32_t>(names.size())) return names[static_cast<std::size_t>(value)];
    return std::to_string(value);
}

std::string to_string(CSharpArgumentInfoFlags flags) {
    struct Named {
        CSharpArgumentInfoFlags value;
        const char* name;
    };
    static constexpr std::array<Named, 6> named = {{
        {CSharpArgumentInfoFlags::UseCompileTimeType, "UseCompileTimeType"},
        {CSharpArgumentInfoFlags::Constant, "Constant"},
        {CSharpArgumentInfoFlags::NamedArgument, "NamedArgument"},
        {CSharpArgumentInfoFlags::IsRef, "IsRef"},
        {CSharpArgumentInfoFlags::IsOut, "IsOut"},
        {CSharpArgumentInfoFlags::IsStaticType, "IsStaticType"},
    }};
    auto bits = static_cast<std::int32_t>(flags);
    if (bits == 0) return "None";
    std::string result;
    std::int32_t remaining = bits;
    for (const Named& entry : named) {
        auto v = static_cast<std::int32_t>(entry.value);
        if ((remaining & v) == v) {
            if (!result.empty()) result += ", ";
            result += entry.name;
            remaining &= ~v;
        }
    }
    if (remaining != 0) return std::to_string(bits);  // a bit without a name: the number
    return result;
}

// ---- CompoundAssignmentInstruction

const SlotInfo CompoundAssignmentInstruction::target_slot{"Target", true};
const SlotInfo CompoundAssignmentInstruction::value_slot{"Value", true};

CompoundAssignmentInstruction::CompoundAssignmentInstruction(OpCode op_code, CompoundEvalMode eval_mode, ILInstruction* target,
                                                             CompoundTargetKind target_kind, ILInstruction* value)
    : ILInstruction(op_code), eval_mode(eval_mode), target_kind(target_kind) {
    set_target(target);
    set_value(value);
    check_valid_target();
}

ILInstruction* CompoundAssignmentInstruction::get_child(int index) const {
    switch (index) {
        case 0: return target_;
        case 1: return value_;
        default: index_out_of_range();
    }
}
void CompoundAssignmentInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_target(value);
        case 1: return set_value(value);
        default: index_out_of_range();
    }
}
const SlotInfo& CompoundAssignmentInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return target_slot;
        case 1: return value_slot;
        default: index_out_of_range();
    }
}

void CompoundAssignmentInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    check_valid_target();
}

void CompoundAssignmentInstruction::check_valid_target() const {
#ifndef NDEBUG
    switch (target_kind) {
        case CompoundTargetKind::Address: assert(target_->result_type() == StackType::Ref || target_->result_type() == StackType::I); break;
        case CompoundTargetKind::Property: {
            assert(target_->op_code == OpCode::Call || target_->op_code == OpCode::CallVirt);
            const auto* call = as<CallInstruction>(target_);
            const auto* owner = call != nullptr ? dynamic_cast<const typesystem::IProperty*>(call->method->accessor_owner()) : nullptr;
            assert(owner != nullptr && owner->can_set());
            break;
        }
        case CompoundTargetKind::Dynamic:
            assert(target_->op_code == OpCode::DynamicGetMemberInstruction || target_->op_code == OpCode::DynamicGetIndexInstruction);
            break;
    }
#endif
}

void CompoundAssignmentInstruction::write_suffix(ITextOutput& output) const {
    switch (target_kind) {
        case CompoundTargetKind::Address: output.write(".address"); break;
        case CompoundTargetKind::Property: output.write(".property"); break;
        case CompoundTargetKind::Dynamic: break;
    }
    switch (eval_mode) {
        case CompoundEvalMode::EvaluatesToNewValue: output.write(".new"); break;
        case CompoundEvalMode::EvaluatesToOldValue: output.write(".old"); break;
    }
}

// ---- NumericCompoundAssign

NumericCompoundAssign::NumericCompoundAssign(const BinaryNumericInstruction* binary, ILInstruction* target, CompoundTargetKind target_kind,
                                             ILInstruction* value, const IType* type, CompoundEvalMode eval_mode)
    : CompoundAssignmentInstruction(OpCode::NumericCompoundAssign, eval_mode, target, target_kind, value),
      check_for_overflow(binary->check_for_overflow),
      sign(binary->sign),
      left_input_type(binary->left_input_type),
      right_input_type(binary->right_input_type),
      op(binary->op),
      type_(type),
      is_lifted_(binary->is_lifted()),
      underlying_result_type_(binary->underlying_result_type()) {
    // upstream asserts is_binary_compatible_with_type(binary, type, nullptr) here (see its WAITS)
    add_il_range(*binary);
    assert(eval_mode == CompoundEvalMode::EvaluatesToNewValue || op == BinaryNumericOperator::Add ||
           op == BinaryNumericOperator::Sub);
    assert(result_type() == (is_lifted_ ? StackType::O : underlying_result_type_));
}

StackType NumericCompoundAssign::result_type() const { return get_stack_type(*type_); }

bool NumericCompoundAssign::is_binary_compatible_with_type(const BinaryNumericInstruction* binary, const IType* type,
                                                           const DecompilerSettings* settings) {
    // The settings switches read as upstream's defaults (WAITS: DecompilerSettings): NativeIntegers on, UnsignedRightShift on.
    (void)settings;
    if (binary->is_lifted()) {
        if (!typesystem::nullable_type::is_nullable(type)) return false;
        type = typesystem::nullable_type::get_underlying_type(type);
    }
    if (type->kind() == typesystem::TypeKind::Unknown) {
        return false;  // avoid introducing a potentially-incorrect compound assignment
    } else if (type->kind() == typesystem::TypeKind::Enum) {
        switch (binary->op) {
            case BinaryNumericOperator::Add:
            case BinaryNumericOperator::Sub:
            case BinaryNumericOperator::BitAnd:
            case BinaryNumericOperator::BitOr:
            case BinaryNumericOperator::BitXor: break;  // OK
            default: return false;                      // operator not supported on enum types
        }
    } else if (type->kind() == typesystem::TypeKind::Pointer) {
        switch (binary->op) {
            case BinaryNumericOperator::Add:
            case BinaryNumericOperator::Sub:
                // ensure that the byte offset is a multiple of the pointer size
                return PointerArithmeticOffset::detect(binary->right(), dynamic_cast<const typesystem::PointerType*>(type)->element_type(),
                                                       binary->check_for_overflow) != nullptr;
            default: return false;  // operator not supported on pointer types
        }
    } else if ((typesystem::is_known_type(type, KnownTypeCode::IntPtr) || typesystem::is_known_type(type, KnownTypeCode::UIntPtr)) &&
               type->kind() != typesystem::TypeKind::NInt && type->kind() != typesystem::TypeKind::NUInt) {
        // If the LHS is C# 9 IntPtr (but not nint or C# 11 IntPtr): "target.intptr *= 2;" is a compiler error, but
        // "target.intptr *= (nint)2;" works. (Without native integers the settings would say no here.)
        // The trick with casting the RHS to n(u)int doesn't work for shifts:
        switch (binary->op) {
            case BinaryNumericOperator::ShiftLeft:
            case BinaryNumericOperator::ShiftRight: return false;
            default: break;
        }
    }
    if (binary->sign != Sign::None) {
        bool sign_mismatch_allowed = binary->sign == Sign::Unsigned && binary->op == BinaryNumericOperator::ShiftRight;
        if (is_csharp_small_integer_type(*type)) {
            // C# will use numeric promotion to int, binary op must be signed
            if (binary->sign != Sign::Signed && !sign_mismatch_allowed) return false;
        } else {
            // C# will use sign from type; except for right shift with C# 11 >>> operator.
            if (get_sign(*type) != binary->sign && !sign_mismatch_allowed) return false;
        }
    }
    // Can't transform if the RHS value would be need to be truncated for the LHS type.
    // WAITS: Transforms::TransformAssignment::is_implicit_truncation(binary->right(), type, nullptr, binary->is_lifted()) (stage 6);
    // until it exists the value is taken as not truncated.
    return true;
}

InstructionFlags NumericCompoundAssign::compute_flags() const {
    InstructionFlags flags = target()->flags() | value()->flags() | InstructionFlags::SideEffect;
    if (check_for_overflow || op == BinaryNumericOperator::Div || op == BinaryNumericOperator::Rem)
        flags |= InstructionFlags::MayThrow;
    return flags;
}

InstructionFlags NumericCompoundAssign::direct_flags() const {
    InstructionFlags flags = InstructionFlags::SideEffect;
    if (check_for_overflow || op == BinaryNumericOperator::Div || op == BinaryNumericOperator::Rem)
        flags |= InstructionFlags::MayThrow;
    return flags;
}

ILInstruction* NumericCompoundAssign::clone() const {
    auto* clone = make<NumericCompoundAssign>(*this);
    clone_children_into(*clone);
    return clone;
}

bool NumericCompoundAssign::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<NumericCompoundAssign>(other);
    return o != nullptr && type_equals(type_, o->type_) && check_for_overflow == o->check_for_overflow && sign == o->sign &&
           op == o->op && eval_mode == o->eval_mode && target_kind == o->target_kind &&
           target()->perform_match(o->target(), match) && value()->perform_match(o->value(), match);
}

void NumericCompoundAssign::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write("." + std::string(BinaryNumericInstruction::get_operator_name(op)));
    if (check_for_overflow) output.write(".ovf");
    if (sign == Sign::Unsigned)
        output.write(".unsigned");
    else if (sign == Sign::Signed)
        output.write(".signed");
    output.write('.');
    write(output, underlying_result_type_);
    if (is_lifted_) output.write(".lifted");
    write_suffix(output);
    output.write('(');
    target()->write_to(output, options);
    output.write(", ");
    value()->write_to(output, options);
    output.write(')');
}

// ---- UserDefinedCompoundAssign

UserDefinedCompoundAssign::UserDefinedCompoundAssign(const IMethod* method, CompoundEvalMode eval_mode, ILInstruction* target,
                                                     CompoundTargetKind target_kind, ILInstruction* value)
    : CompoundAssignmentInstruction(OpCode::UserDefinedCompoundAssign, eval_mode, target, target_kind, value), method(method) {
    assert(method->is_operator() || is_string_concat(method));
    assert(eval_mode == CompoundEvalMode::EvaluatesToNewValue || is_increment_or_decrement(method));
}

bool UserDefinedCompoundAssign::is_increment_or_decrement(const IMethod* method, const DecompilerSettings* settings) {
    (void)settings;  // WAITS: DecompilerSettings (CheckedOperators reads as its default, on)
    if (!(method->is_operator() && method->is_static())) return false;
    const std::string name = method->name();
    if (name == "op_Increment" || name == "op_Decrement") return true;
    if (name == "op_CheckedIncrement" || name == "op_CheckedDecrement") return true;
    return false;
}

bool UserDefinedCompoundAssign::is_string_concat(const IMethod* method) {
    return method->name() == "Concat" && method->is_static() && typesystem::is_known_type(method->declaring_type(), KnownTypeCode::String);
}

StackType UserDefinedCompoundAssign::result_type() const { return get_stack_type(*method->return_type()); }

InstructionFlags UserDefinedCompoundAssign::compute_flags() const {
    return CompoundAssignmentInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect;
}

InstructionFlags UserDefinedCompoundAssign::direct_flags() const {
    return CompoundAssignmentInstruction::direct_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect;
}

ILInstruction* UserDefinedCompoundAssign::clone() const {
    auto* clone = make<UserDefinedCompoundAssign>(*this);
    clone_children_into(*clone);
    return clone;
}

bool UserDefinedCompoundAssign::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<UserDefinedCompoundAssign>(other);
    return o != nullptr && member_equals(method, o->method) && eval_mode == o->eval_mode && target_kind == o->target_kind &&
           target()->perform_match(o->target(), match) && value()->perform_match(o->value(), match);
}

void UserDefinedCompoundAssign::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_suffix(output);
    output.write(' ');
    il::write_to(*method, output);
    output.write('(');
    target()->write_to(output, options);
    output.write(", ");
    value()->write_to(output, options);
    output.write(')');
}

// ---- DynamicCompoundAssign

DynamicCompoundAssign::DynamicCompoundAssign(ExpressionType op, CSharpBinderFlags binder_flags, ILInstruction* target,
                                             CSharpArgumentInfo target_argument_info, ILInstruction* value,
                                             CSharpArgumentInfo value_argument_info, CompoundTargetKind target_kind)
    : CompoundAssignmentInstruction(OpCode::DynamicCompoundAssign, compound_eval_mode_from_operation(op), target, target_kind, value),
      operation_(op),
      target_argument_info_(std::move(target_argument_info)),
      value_argument_info_(std::move(value_argument_info)),
      binder_flags_(binder_flags) {
    if (!is_expression_type_supported(op)) throw std::out_of_range("op");
}

bool DynamicCompoundAssign::is_expression_type_supported(ExpressionType type) {
    switch (type) {
        case ExpressionType::AddAssign:
        case ExpressionType::AddAssignChecked:
        case ExpressionType::AndAssign:
        case ExpressionType::DivideAssign:
        case ExpressionType::ExclusiveOrAssign:
        case ExpressionType::LeftShiftAssign:
        case ExpressionType::ModuloAssign:
        case ExpressionType::MultiplyAssign:
        case ExpressionType::MultiplyAssignChecked:
        case ExpressionType::OrAssign:
        case ExpressionType::PostDecrementAssign:
        case ExpressionType::PostIncrementAssign:
        case ExpressionType::PreDecrementAssign:
        case ExpressionType::PreIncrementAssign:
        case ExpressionType::RightShiftAssign:
        case ExpressionType::SubtractAssign:
        case ExpressionType::SubtractAssignChecked: return true;
        default: return false;
    }
}

CompoundEvalMode DynamicCompoundAssign::compound_eval_mode_from_operation(ExpressionType op) {
    switch (op) {
        case ExpressionType::PostIncrementAssign:
        case ExpressionType::PostDecrementAssign: return CompoundEvalMode::EvaluatesToOldValue;
        default: return CompoundEvalMode::EvaluatesToNewValue;
    }
}

InstructionFlags DynamicCompoundAssign::compute_flags() const {
    return CompoundAssignmentInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect;
}

InstructionFlags DynamicCompoundAssign::direct_flags() const {
    return CompoundAssignmentInstruction::direct_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect;
}

ILInstruction* DynamicCompoundAssign::clone() const {
    auto* clone = make<DynamicCompoundAssign>(*this);
    clone_children_into(*clone);
    return clone;
}

bool DynamicCompoundAssign::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicCompoundAssign>(other);
    return o != nullptr && eval_mode == o->eval_mode && target_kind == o->target_kind && target()->perform_match(o->target(), match) &&
           value()->perform_match(o->value(), match);
}

void DynamicCompoundAssign::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write("." + to_lower(il::to_string(operation_)));
    DynamicInstruction::write_binder_flags(binder_flags_, output, options);
    write_suffix(output);
    output.write(' ');
    DynamicInstruction::write_argument_list(output, options, {{target(), target_argument_info_}, {value(), value_argument_info_}});
}

// ---- UserDefinedLogicOperator

const SlotInfo UserDefinedLogicOperator::left_slot{"Left", true};
const SlotInfo UserDefinedLogicOperator::right_slot{"Right"};

UserDefinedLogicOperator::UserDefinedLogicOperator(const IMethod* method, ILInstruction* left, ILInstruction* right)
    : ILInstruction(OpCode::UserDefinedLogicOperator), method_(method) {
    set_left(left);
    set_right(right);
}

ILInstruction* UserDefinedLogicOperator::get_child(int index) const {
    switch (index) {
        case 0: return left_;
        case 1: return right_;
        default: index_out_of_range();
    }
}
void UserDefinedLogicOperator::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_left(value);
        case 1: return set_right(value);
        default: index_out_of_range();
    }
}
const SlotInfo& UserDefinedLogicOperator::get_child_slot(int index) const {
    switch (index) {
        case 0: return left_slot;
        case 1: return right_slot;
        default: index_out_of_range();
    }
}

ILInstruction* UserDefinedLogicOperator::clone() const {
    auto* clone = make<UserDefinedLogicOperator>(*this);
    clone->set_left(left_->clone());
    clone->set_right(right_->clone());
    return clone;
}

void UserDefinedLogicOperator::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (method_ != nullptr) {
        output.write(' ');
        il::write_to(*method_, output);
    }
    output.write('(');
    left_->write_to(output, options);
    output.write(", ");
    right_->write_to(output, options);
    output.write(')');
}

bool UserDefinedLogicOperator::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<UserDefinedLogicOperator>(other);
    return o != nullptr && member_equals(method_, o->method_) && left_->perform_match(o->left_, match) && right_->perform_match(o->right_, match);
}

bool ILInstruction::match_user_defined_logic_operator(const IMethod** method, ILInstruction** left, ILInstruction** right) const {
    const auto* inst = as<UserDefinedLogicOperator>(this);
    if (inst != nullptr) {
        *method = inst->method();
        *left = inst->left();
        *right = inst->right();
        return true;
    }
    *method = nullptr;
    *left = nullptr;
    *right = nullptr;
    return false;
}

// ---- DynamicInstruction

void DynamicInstruction::write_binder_flags(CSharpBinderFlags flags, ITextOutput& output, const ILAstWritingOptions&) {
    auto has = [&](CSharpBinderFlags flag) { return (flags & flag) != CSharpBinderFlags::None; };
    if (has(CSharpBinderFlags::BinaryOperationLogical)) output.write(".logic");
    if (has(CSharpBinderFlags::CheckedContext)) output.write(".checked");
    if (has(CSharpBinderFlags::ConvertArrayIndex)) output.write(".arrayindex");
    if (has(CSharpBinderFlags::ConvertExplicit)) output.write(".explicit");
    if (has(CSharpBinderFlags::InvokeSimpleName)) output.write(".invokesimple");
    if (has(CSharpBinderFlags::InvokeSpecialName)) output.write(".invokespecial");
    if (has(CSharpBinderFlags::ResultDiscarded)) output.write(".discard");
    if (has(CSharpBinderFlags::ResultIndexed)) output.write(".resultindexed");
    if (has(CSharpBinderFlags::ValueFromCompoundAssignment)) output.write(".compound");
}

void DynamicInstruction::write_argument_list(ITextOutput& output, const ILAstWritingOptions& options,
                                             const std::vector<std::pair<ILInstruction*, CSharpArgumentInfo>>& arguments) {
    output.write('(');
    int j = 0;
    for (const auto& [arg, info] : arguments) {
        if (j > 0) output.write(", ");
        output.write("[flags: ");
        output.write(il::to_string(info.flags));
        output.write(", name: " + info.name + "] ");
        arg->write_to(output, options);
        j++;
    }
    output.write(')');
}

// ---- DynamicLogicOperatorInstruction

const SlotInfo DynamicLogicOperatorInstruction::left_slot{"Left", true};
const SlotInfo DynamicLogicOperatorInstruction::right_slot{"Right"};

DynamicLogicOperatorInstruction::DynamicLogicOperatorInstruction(CSharpBinderFlags binder_flags, ExpressionType operation,
                                                                 const IType* context, CSharpArgumentInfo left_argument_info,
                                                                 ILInstruction* left, CSharpArgumentInfo right_argument_info,
                                                                 ILInstruction* right)
    : DynamicInstruction(OpCode::DynamicLogicOperatorInstruction, binder_flags, context),
      operation_(operation),
      left_argument_info_(std::move(left_argument_info)),
      right_argument_info_(std::move(right_argument_info)) {
    set_left(left);
    set_right(right);
}

ILInstruction* DynamicLogicOperatorInstruction::get_child(int index) const {
    switch (index) {
        case 0: return left_;
        case 1: return right_;
        default: index_out_of_range();
    }
}
void DynamicLogicOperatorInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_left(value);
        case 1: return set_right(value);
        default: index_out_of_range();
    }
}
const SlotInfo& DynamicLogicOperatorInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return left_slot;
        case 1: return right_slot;
        default: index_out_of_range();
    }
}

ILInstruction* DynamicLogicOperatorInstruction::clone() const {
    auto* clone = make<DynamicLogicOperatorInstruction>(*this);
    clone->set_left(left_->clone());
    clone->set_right(right_->clone());
    return clone;
}

InstructionFlags DynamicLogicOperatorInstruction::compute_flags() const {
    return direct_flags() | left_->flags() | SemanticHelper::combine_branches(right_->flags(), InstructionFlags::None);
}

CSharpArgumentInfo DynamicLogicOperatorInstruction::get_argument_info_of_child(int index) const {
    switch (index) {
        case 0: return left_argument_info_;
        case 1: return right_argument_info_;
        default: throw std::out_of_range("index");
    }
}

bool DynamicLogicOperatorInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicLogicOperatorInstruction>(other);
    return o != nullptr && left_->perform_match(o->left_, match) && right_->perform_match(o->right_, match);
}

void DynamicLogicOperatorInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write(il::to_string(operation_));
    write_argument_list(output, options, {{left_, left_argument_info_}, {right_, right_argument_info_}});
}

// ---- DynamicBinaryOperatorInstruction

const SlotInfo DynamicBinaryOperatorInstruction::left_slot{"Left", true};
const SlotInfo DynamicBinaryOperatorInstruction::right_slot{"Right", true};

DynamicBinaryOperatorInstruction::DynamicBinaryOperatorInstruction(CSharpBinderFlags binder_flags, ExpressionType operation,
                                                                   const IType* context, CSharpArgumentInfo left_argument_info,
                                                                   ILInstruction* left, CSharpArgumentInfo right_argument_info,
                                                                   ILInstruction* right)
    : DynamicInstruction(OpCode::DynamicBinaryOperatorInstruction, binder_flags, context),
      operation_(operation),
      left_argument_info_(std::move(left_argument_info)),
      right_argument_info_(std::move(right_argument_info)) {
    set_left(left);
    set_right(right);
}

ILInstruction* DynamicBinaryOperatorInstruction::get_child(int index) const {
    switch (index) {
        case 0: return left_;
        case 1: return right_;
        default: index_out_of_range();
    }
}
void DynamicBinaryOperatorInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_left(value);
        case 1: return set_right(value);
        default: index_out_of_range();
    }
}
const SlotInfo& DynamicBinaryOperatorInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return left_slot;
        case 1: return right_slot;
        default: index_out_of_range();
    }
}

ILInstruction* DynamicBinaryOperatorInstruction::clone() const {
    auto* clone = make<DynamicBinaryOperatorInstruction>(*this);
    clone->set_left(left_->clone());
    clone->set_right(right_->clone());
    return clone;
}

InstructionFlags DynamicBinaryOperatorInstruction::compute_flags() const {
    return DynamicInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect | left_->flags() | right_->flags();
}

CSharpArgumentInfo DynamicBinaryOperatorInstruction::get_argument_info_of_child(int index) const {
    switch (index) {
        case 0: return left_argument_info_;
        case 1: return right_argument_info_;
        default: throw std::out_of_range("index");
    }
}

bool DynamicBinaryOperatorInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicBinaryOperatorInstruction>(other);
    return o != nullptr && left_->perform_match(o->left_, match) && right_->perform_match(o->right_, match);
}

void DynamicBinaryOperatorInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write(il::to_string(operation_));
    write_argument_list(output, options, {{left_, left_argument_info_}, {right_, right_argument_info_}});
}

// ---- DynamicUnaryOperatorInstruction

const SlotInfo DynamicUnaryOperatorInstruction::operand_slot{"Operand", true};

DynamicUnaryOperatorInstruction::DynamicUnaryOperatorInstruction(CSharpBinderFlags binder_flags, ExpressionType operation,
                                                                 const IType* context, CSharpArgumentInfo operand_argument_info,
                                                                 ILInstruction* operand)
    : DynamicInstruction(OpCode::DynamicUnaryOperatorInstruction, binder_flags, context),
      operation_(operation),
      operand_argument_info_(std::move(operand_argument_info)) {
    set_operand(operand);
}

ILInstruction* DynamicUnaryOperatorInstruction::get_child(int index) const {
    if (index == 0) return operand_;
    index_out_of_range();
}
void DynamicUnaryOperatorInstruction::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_operand(value);
    index_out_of_range();
}
const SlotInfo& DynamicUnaryOperatorInstruction::get_child_slot(int index) const {
    if (index == 0) return operand_slot;
    index_out_of_range();
}

ILInstruction* DynamicUnaryOperatorInstruction::clone() const {
    auto* clone = make<DynamicUnaryOperatorInstruction>(*this);
    clone->set_operand(operand_->clone());
    return clone;
}

StackType DynamicUnaryOperatorInstruction::result_type() const {
    switch (operation_) {
        case ExpressionType::IsFalse:
        case ExpressionType::IsTrue: return StackType::I4;  // bool
        default: return StackType::O;
    }
}

InstructionFlags DynamicUnaryOperatorInstruction::compute_flags() const {
    return DynamicInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect | operand_->flags();
}

CSharpArgumentInfo DynamicUnaryOperatorInstruction::get_argument_info_of_child(int index) const {
    if (index == 0) return operand_argument_info_;
    throw std::out_of_range("index");
}

bool DynamicUnaryOperatorInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicUnaryOperatorInstruction>(other);
    return o != nullptr && operand_->perform_match(o->operand_, match);
}

void DynamicUnaryOperatorInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write(il::to_string(operation_));
    write_argument_list(output, options, {{operand_, operand_argument_info_}});
}

// ---- DynamicConvertInstruction

const SlotInfo DynamicConvertInstruction::argument_slot{"Argument", true};

DynamicConvertInstruction::DynamicConvertInstruction(CSharpBinderFlags binder_flags, const IType* type, const IType* context,
                                                     ILInstruction* argument)
    : DynamicInstruction(OpCode::DynamicConvertInstruction, binder_flags, context), type_(type) {
    set_argument(argument);
}

ILInstruction* DynamicConvertInstruction::get_child(int index) const {
    if (index == 0) return argument_;
    index_out_of_range();
}
void DynamicConvertInstruction::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_argument(value);
    index_out_of_range();
}
const SlotInfo& DynamicConvertInstruction::get_child_slot(int index) const {
    if (index == 0) return argument_slot;
    index_out_of_range();
}

ILInstruction* DynamicConvertInstruction::clone() const {
    auto* clone = make<DynamicConvertInstruction>(*this);
    clone->set_argument(argument_->clone());
    return clone;
}

StackType DynamicConvertInstruction::result_type() const { return get_stack_type(*type_); }

InstructionFlags DynamicConvertInstruction::compute_flags() const {
    return DynamicInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect | argument_->flags();
}

CSharpArgumentInfo DynamicConvertInstruction::get_argument_info_of_child(int) const { return CSharpArgumentInfo{}; }

bool DynamicConvertInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicConvertInstruction>(other);
    return o != nullptr && type_equals(type_, o->type_) && argument_->perform_match(o->argument_, match);
}

void DynamicConvertInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(argument_->result_type() == StackType::O);
}

void DynamicConvertInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    il::write_to(*type_, output);
    output.write('(');
    argument_->write_to(output, options);
    output.write(')');
}

// ---- DynamicGetMemberInstruction

const SlotInfo DynamicGetMemberInstruction::target_slot{"Target", true};

DynamicGetMemberInstruction::DynamicGetMemberInstruction(CSharpBinderFlags binder_flags, std::string name, const IType* context,
                                                         CSharpArgumentInfo target_argument_info, ILInstruction* target)
    : DynamicInstruction(OpCode::DynamicGetMemberInstruction, binder_flags, context),
      name_(std::move(name)),
      target_argument_info_(std::move(target_argument_info)) {
    set_target(target);
}

ILInstruction* DynamicGetMemberInstruction::get_child(int index) const {
    if (index == 0) return target_;
    index_out_of_range();
}
void DynamicGetMemberInstruction::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_target(value);
    index_out_of_range();
}
const SlotInfo& DynamicGetMemberInstruction::get_child_slot(int index) const {
    if (index == 0) return target_slot;
    index_out_of_range();
}

ILInstruction* DynamicGetMemberInstruction::clone() const {
    auto* clone = make<DynamicGetMemberInstruction>(*this);
    clone->set_target(target_->clone());
    return clone;
}

InstructionFlags DynamicGetMemberInstruction::compute_flags() const {
    return DynamicInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect | target_->flags();
}

CSharpArgumentInfo DynamicGetMemberInstruction::get_argument_info_of_child(int index) const {
    if (index != 0) throw std::out_of_range("index");
    return target_argument_info_;
}

bool DynamicGetMemberInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicGetMemberInstruction>(other);
    return o != nullptr && target_->perform_match(o->target_, match);
}

void DynamicGetMemberInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(target_->result_type() == StackType::O);
}

void DynamicGetMemberInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write(name_);
    write_argument_list(output, options, {{target_, target_argument_info_}});
}

// ---- DynamicSetMemberInstruction

const SlotInfo DynamicSetMemberInstruction::target_slot{"Target", true};
const SlotInfo DynamicSetMemberInstruction::value_slot{"Value", true};

DynamicSetMemberInstruction::DynamicSetMemberInstruction(CSharpBinderFlags binder_flags, std::string name, const IType* context,
                                                         CSharpArgumentInfo target_argument_info, ILInstruction* target,
                                                         CSharpArgumentInfo value_argument_info, ILInstruction* value)
    : DynamicInstruction(OpCode::DynamicSetMemberInstruction, binder_flags, context),
      name_(std::move(name)),
      target_argument_info_(std::move(target_argument_info)),
      value_argument_info_(std::move(value_argument_info)) {
    set_target(target);
    set_value(value);
}

ILInstruction* DynamicSetMemberInstruction::get_child(int index) const {
    switch (index) {
        case 0: return target_;
        case 1: return value_;
        default: index_out_of_range();
    }
}
void DynamicSetMemberInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_target(value);
        case 1: return set_value(value);
        default: index_out_of_range();
    }
}
const SlotInfo& DynamicSetMemberInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return target_slot;
        case 1: return value_slot;
        default: index_out_of_range();
    }
}

ILInstruction* DynamicSetMemberInstruction::clone() const {
    auto* clone = make<DynamicSetMemberInstruction>(*this);
    clone->set_target(target_->clone());
    clone->set_value(value_->clone());
    return clone;
}

InstructionFlags DynamicSetMemberInstruction::compute_flags() const {
    return DynamicInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect | target_->flags() | value_->flags();
}

CSharpArgumentInfo DynamicSetMemberInstruction::get_argument_info_of_child(int index) const {
    switch (index) {
        case 0: return target_argument_info_;
        case 1: return value_argument_info_;
        default: throw std::out_of_range("index");
    }
}

bool DynamicSetMemberInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicSetMemberInstruction>(other);
    return o != nullptr && target_->perform_match(o->target_, match) && value_->perform_match(o->value_, match);
}

void DynamicSetMemberInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(target_->result_type() == StackType::O);
}

void DynamicSetMemberInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write(name_);
    write_argument_list(output, options, {{target_, target_argument_info_}, {value_, value_argument_info_}});
}

// ---- the dynamic call sites with a list of arguments

const SlotInfo DynamicArgumentsInstruction::arguments_slot{"Arguments", true};

DynamicArgumentsInstruction::DynamicArgumentsInstruction(OpCode op_code, CSharpBinderFlags binder_flags, const IType* context,
                                                         std::vector<CSharpArgumentInfo> argument_info,
                                                         const std::vector<ILInstruction*>& arguments)
    : DynamicInstruction(op_code, binder_flags, context), argument_info_(std::move(argument_info)), arguments_(this, 0) {
    arguments_.add_range(arguments);
}

ILInstruction* DynamicArgumentsInstruction::get_child(int index) const { return arguments_[index]; }
void DynamicArgumentsInstruction::set_child(int index, ILInstruction* value) { arguments_.set(index, value); }
const SlotInfo& DynamicArgumentsInstruction::get_child_slot(int) const { return arguments_slot; }

CSharpArgumentInfo DynamicArgumentsInstruction::get_argument_info_of_child(int index) const {
    if (index < 0 || index >= static_cast<int>(argument_info_.size())) throw std::out_of_range("index");
    return argument_info_[static_cast<std::size_t>(index)];
}

InstructionFlags DynamicArgumentsInstruction::compute_flags() const {
    InstructionFlags arguments_flags = InstructionFlags::None;
    for (ILInstruction* arg : arguments_) arguments_flags |= arg->flags();
    return DynamicInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect | arguments_flags;
}

void DynamicArgumentsInstruction::clone_children_into(DynamicArgumentsInstruction& clone) const {
    std::vector<ILInstruction*> cloned;
    for (ILInstruction* arg : arguments_) cloned.push_back(arg->clone());
    clone.arguments_.add_range(cloned);
}

std::vector<std::pair<ILInstruction*, CSharpArgumentInfo>> DynamicArgumentsInstruction::argument_list() const {
    // Arguments.Zip(ArgumentInfo): as many pairs as the shorter list has
    std::vector<std::pair<ILInstruction*, CSharpArgumentInfo>> pairs;
    std::size_t n = std::min(static_cast<std::size_t>(arguments_.count()), argument_info_.size());
    for (std::size_t i = 0; i < n; ++i) pairs.emplace_back(arguments_[static_cast<int>(i)], argument_info_[i]);
    return pairs;
}

// DynamicGetIndexInstruction

ILInstruction* DynamicGetIndexInstruction::clone() const {
    auto* clone = make<DynamicGetIndexInstruction>(*this);
    clone_children_into(*clone);
    return clone;
}
bool DynamicGetIndexInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicGetIndexInstruction>(other);
    return o != nullptr && patterns::ListMatch::do_match(arguments(), o->arguments(), match);
}
void DynamicGetIndexInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write("get_Item");
    write_argument_list(output, options, argument_list());
}

// DynamicSetIndexInstruction

ILInstruction* DynamicSetIndexInstruction::clone() const {
    auto* clone = make<DynamicSetIndexInstruction>(*this);
    clone_children_into(*clone);
    return clone;
}
bool DynamicSetIndexInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicSetIndexInstruction>(other);
    return o != nullptr && patterns::ListMatch::do_match(arguments(), o->arguments(), match);
}
void DynamicSetIndexInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write("set_Item");
    write_argument_list(output, options, argument_list());
}

// DynamicInvokeMemberInstruction

ILInstruction* DynamicInvokeMemberInstruction::clone() const {
    auto* clone = make<DynamicInvokeMemberInstruction>(*this);
    clone_children_into(*clone);
    return clone;
}
bool DynamicInvokeMemberInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicInvokeMemberInstruction>(other);
    return o != nullptr && patterns::ListMatch::do_match(arguments(), o->arguments(), match);
}
void DynamicInvokeMemberInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write(name_);
    if (!type_arguments_.empty()) {
        output.write('<');
        int i = 0;
        for (const IType* type_arg : type_arguments_) {
            if (i > 0) output.write(", ");
            il::write_to(*type_arg, output);
            i++;
        }
        output.write('>');
    }
    write_argument_list(output, options, argument_list());
}

// DynamicInvokeConstructorInstruction

ILInstruction* DynamicInvokeConstructorInstruction::clone() const {
    auto* clone = make<DynamicInvokeConstructorInstruction>(*this);
    clone_children_into(*clone);
    return clone;
}
StackType DynamicInvokeConstructorInstruction::result_type() const {
    return result_type_ != nullptr ? get_stack_type(*result_type_) : StackType::Unknown;
}
bool DynamicInvokeConstructorInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicInvokeConstructorInstruction>(other);
    return o != nullptr && patterns::ListMatch::do_match(arguments(), o->arguments(), match);
}
void DynamicInvokeConstructorInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    if (result_type_ != nullptr) il::write_to(*result_type_, output);
    output.write(".ctor");
    write_argument_list(output, options, argument_list());
}

// DynamicInvokeInstruction

ILInstruction* DynamicInvokeInstruction::clone() const {
    auto* clone = make<DynamicInvokeInstruction>(*this);
    clone_children_into(*clone);
    return clone;
}
bool DynamicInvokeInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicInvokeInstruction>(other);
    return o != nullptr && patterns::ListMatch::do_match(arguments(), o->arguments(), match);
}
void DynamicInvokeInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    write_argument_list(output, options, argument_list());
}

// ---- DynamicIsEventInstruction

const SlotInfo DynamicIsEventInstruction::argument_slot{"Argument", true};

DynamicIsEventInstruction::DynamicIsEventInstruction(CSharpBinderFlags binder_flags, std::string name, const IType* context,
                                                     ILInstruction* argument)
    : DynamicInstruction(OpCode::DynamicIsEventInstruction, binder_flags, context), name_(std::move(name)) {
    set_argument(argument);
}

ILInstruction* DynamicIsEventInstruction::get_child(int index) const {
    if (index == 0) return argument_;
    index_out_of_range();
}
void DynamicIsEventInstruction::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_argument(value);
    index_out_of_range();
}
const SlotInfo& DynamicIsEventInstruction::get_child_slot(int index) const {
    if (index == 0) return argument_slot;
    index_out_of_range();
}

ILInstruction* DynamicIsEventInstruction::clone() const {
    auto* clone = make<DynamicIsEventInstruction>(*this);
    clone->set_argument(argument_->clone());
    return clone;
}

InstructionFlags DynamicIsEventInstruction::compute_flags() const {
    return DynamicInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect | argument_->flags();
}

CSharpArgumentInfo DynamicIsEventInstruction::get_argument_info_of_child(int) const { return CSharpArgumentInfo{}; }

bool DynamicIsEventInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DynamicIsEventInstruction>(other);
    return o != nullptr && argument_->perform_match(o->argument_, match);
}

void DynamicIsEventInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(argument_->result_type() == StackType::O);
}

void DynamicIsEventInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    write_binder_flags(output, options);
    output.write(' ');
    output.write('(');
    argument_->write_to(output, options);
    output.write(')');
}

// ---- MatchInstruction

const SlotInfo MatchInstruction::tested_operand_slot{"TestedOperand", true};
const SlotInfo MatchInstruction::sub_patterns_slot{"SubPatterns"};

MatchInstruction::MatchInstruction(ILVariable* variable, const IMethod* method, ILInstruction* tested_operand,
                                   const std::vector<ILInstruction*>& sub_patterns)
    : ILInstruction(OpCode::MatchInstruction), variable_(variable), method_(method), sub_patterns_(this, 1) {
    if (variable == nullptr) throw std::invalid_argument("variable");
    set_tested_operand(tested_operand);
    sub_patterns_.add_range(sub_patterns);
}

MatchInstruction::MatchInstruction(const MatchInstruction& other)
    : ILInstruction(other),
      IStoreInstruction(other),
      IInstructionWithMethodOperand(other),
      is_deconstruct_call(other.is_deconstruct_call),
      is_deconstruct_tuple(other.is_deconstruct_tuple),
      check_type(other.check_type),
      check_not_null(other.check_not_null),
      variable_(other.variable_),
      method_(other.method_),
      tested_operand_(other.tested_operand_),
      sub_patterns_(this, 1) {}

void MatchInstruction::set_variable(ILVariable* value) {
    assert(value != nullptr);
    if (is_connected()) variable_->remove_store_instruction(this);
    variable_ = value;
    if (is_connected()) variable_->add_store_instruction(this);
}

void MatchInstruction::connected() {
    ILInstruction::connected();
    variable_->add_store_instruction(this);
}

void MatchInstruction::disconnected() {
    variable_->remove_store_instruction(this);
    ILInstruction::disconnected();
}

int MatchInstruction::num_positional_patterns() const {
    if (is_deconstruct_call) return static_cast<int>(method_->parameters().size()) - (method_->is_static() ? 1 : 0);
    if (is_deconstruct_tuple) {
        auto element_types = typesystem::TupleType::get_tuple_element_types(variable_->type());
        return element_types ? static_cast<int>(element_types->size()) : 0;
    }
    return 0;
}

ILInstruction* MatchInstruction::get_child(int index) const {
    if (index == 0) return tested_operand_;
    return sub_patterns_[index - 1];
}
void MatchInstruction::set_child(int index, ILInstruction* value) {
    if (index == 0)
        set_tested_operand(value);
    else
        sub_patterns_.set(index - 1, value);
}
const SlotInfo& MatchInstruction::get_child_slot(int index) const { return index == 0 ? tested_operand_slot : sub_patterns_slot; }

ILInstruction* MatchInstruction::clone() const {
    auto* clone = make<MatchInstruction>(*this);
    clone->set_tested_operand(tested_operand_->clone());
    std::vector<ILInstruction*> cloned;
    for (ILInstruction* arg : sub_patterns_) cloned.push_back(arg->clone());
    clone->sub_patterns_.add_range(cloned);
    return clone;
}

InstructionFlags MatchInstruction::compute_flags() const {
    InstructionFlags sub_flags = InstructionFlags::None;
    for (ILInstruction* arg : sub_patterns_) sub_flags |= arg->flags();
    return InstructionFlags::MayWriteLocals | tested_operand_->flags() | sub_flags | InstructionFlags::SideEffect | InstructionFlags::MayThrow |
           InstructionFlags::ControlFlow;
}

bool MatchInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<MatchInstruction>(other);
    return o != nullptr && variable_ == o->variable_ && member_equals(method_, o->method_) && is_deconstruct_call == o->is_deconstruct_call &&
           is_deconstruct_tuple == o->is_deconstruct_tuple && check_type == o->check_type && check_not_null == o->check_not_null &&
           tested_operand_->perform_match(o->tested_operand_, match) && patterns::ListMatch::do_match(sub_patterns_, o->sub_patterns_, match);
}

void MatchInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(phase <= ILPhase::InILReader || is_descendant_of(variable_->function()));
    assert(phase <= ILPhase::InILReader || variable_->function()->variables()[variable_->index_in_function()] == variable_);
    additional_invariants();
}

bool MatchInstruction::is_pattern_match(ILInstruction* inst, ILInstruction** tested_operand, const DecompilerSettings* settings) {
    // WAITS: DecompilerSettings (PatternCombinators and RelationalPatterns read as their default, on)
    (void)settings;
    if (auto* m = as<MatchInstruction>(inst)) {
        *tested_operand = m->tested_operand_;
        return true;
    }
    if (auto* comp = as<Comp>(inst)) {
        ILInstruction* operand = nullptr;
        if (comp->match_logic_not(&operand) && is_pattern_match(operand, tested_operand, settings)) {
            return true;  // settings?.PatternCombinators ?? true
        }
        *tested_operand = comp->left();
        return is_constant(comp->right());
    }
    if (auto* call = as<Call>(inst)) {
        if (is_call_to_op_equality(call, KnownTypeCode::String)) {
            *tested_operand = call->arguments()[0];
            return call->arguments()[1]->op_code == OpCode::LdStr;
        }
        if (is_call_to_op_equality(call, KnownTypeCode::Decimal)) {
            *tested_operand = call->arguments()[0];
            return call->arguments()[1]->op_code == OpCode::LdcDecimal;
        }
    }
    *tested_operand = nullptr;
    return false;
}

bool MatchInstruction::is_call_to_op_equality(const Call* call, KnownTypeCode known_type) {
    const IMethod* method = call->method;
    return method->is_operator() && method->name() == "op_Equality" && typesystem::is_known_type(method->declaring_type(), known_type) &&
           call->arguments().count() == 2;
}

bool MatchInstruction::is_constant(const ILInstruction* inst) {
    switch (inst->op_code) {
        case OpCode::LdcDecimal:
        case OpCode::LdcF4:
        case OpCode::LdcF8:
        case OpCode::LdcI4:
        case OpCode::LdcI8:
        case OpCode::LdNull: return true;
        default: return false;
    }
}

const IType* MatchInstruction::get_deconstruct_result_type(int index) const {
    if (is_deconstruct_call) {
        int first_out_param = method_->is_static() ? 1 : 0;
        const IType* out_param_type = method_->parameters().at(static_cast<std::size_t>(first_out_param + index))->type();
        const auto* brt = dynamic_cast<const typesystem::ByReferenceType*>(out_param_type);
        if (brt == nullptr) throw UsageError("deconstruct out param must be by reference");
        return brt->element_type();
    }
    if (is_deconstruct_tuple) {
        auto element_types = typesystem::TupleType::get_tuple_element_types(variable_->type());
        if (!element_types) throw std::out_of_range("index");
        return element_types->at(static_cast<std::size_t>(index));
    }
    throw UsageError("GetDeconstructResultType requires a deconstruct pattern");
}

bool MatchInstruction::is_deconstruct_method(const IMethod* method) {
    if (method == nullptr) return false;
    if (method->name() != "Deconstruct") return false;
    if (method->return_type()->kind() != typesystem::TypeKind::Void) return false;
    int first_out_param = method->is_static() ? 1 : 0;
    if (method->is_static()) {
        if (!method->is_extension_method()) return false;
        // TODO upstream: check whether all type arguments can be inferred from the first argument
    } else {
        if (!method->type_parameters().empty()) return false;
    }
    // TODO upstream: check whether the method is ambiguous
    if (static_cast<int>(method->parameters().size()) < first_out_param) return false;
    for (int i = first_out_param; i < static_cast<int>(method->parameters().size()); i++) {
        if (method->parameters()[static_cast<std::size_t>(i)]->reference_kind() != typesystem::ReferenceKind::Out) return false;
    }
    return true;
}

void MatchInstruction::additional_invariants() const {
    assert(variable_->kind() == VariableKind::PatternLocal);
    if (is_deconstruct_call)
        assert(is_deconstruct_method(method_));
    else
        assert(method_ == nullptr);
    if (is_deconstruct_tuple) assert(variable_->type()->kind() == typesystem::TypeKind::Tuple);
    assert(sub_patterns_.count() >= num_positional_patterns());
    for (ILInstruction* sub_pattern : sub_patterns_) {
        ILInstruction* operand = nullptr;
        if (!is_pattern_match(sub_pattern, &operand, nullptr)) throw UsageError("Sub-Pattern must be a valid pattern");
        // the first child is TestedOperand
        [[maybe_unused]] int sub_pattern_index = sub_pattern->child_index() - 1;
        [[maybe_unused]] ILInstruction* target = nullptr;
        [[maybe_unused]] const IField* field = nullptr;
        if (sub_pattern_index < num_positional_patterns()) {
            // positional pattern
            [[maybe_unused]] auto* result = as<DeconstructResultInstruction>(operand);
            assert(result != nullptr && result->index() == sub_pattern_index);
        } else if (operand->match_ld_fld(&target, &field)) {
            assert(target->match_ld_loc_ref(variable_));
        } else if ([[maybe_unused]] auto* call = as<CallInstruction>(operand)) {
            assert(call->method->accessor_kind() == typesystem::MethodSemanticsAttributes::Getter);
            assert(call->arguments()[0]->match_ld_loc_ref(variable_));
        } else {
            assert(false && "Tested operand of sub-pattern is invalid.");
        }
    }
}

void MatchInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (check_not_null) output.write(".notnull");
    if (check_type) {
        output.write(".type[");
        il::write_to(*variable_->type(), output);
        output.write(']');
    }
    if (is_deconstruct_call) {
        output.write(".deconstruct[");
        if (method_ == nullptr)
            output.write("<null>");
        else
            il::write_to(*method_, output);
        output.write(']');
    }
    if (is_deconstruct_tuple) output.write(".tuple");
    output.write(' ');
    output.write('(');
    variable_->write_to(output);
    output.write(" = ");
    tested_operand_->write_to(output, options);
    output.write(')');
    if (sub_patterns_.count() > 0) {
        output.mark_fold_start("{...}");
        output.write_line("{");
        output.indent();
        for (ILInstruction* pattern : sub_patterns_) {
            pattern->write_to(output, options);
            output.write_line();
        }
        output.unindent();
        output.write('}');
        output.mark_fold_end();
    }
}

bool ILInstruction::match_match_instruction(ILVariable** variable, const IMethod** method, ILInstruction** tested_operand) const {
    const auto* inst = as<MatchInstruction>(this);
    if (inst != nullptr) {
        *variable = inst->variable();
        *method = inst->method();
        *tested_operand = inst->tested_operand();
        return true;
    }
    *variable = nullptr;
    *method = nullptr;
    *tested_operand = nullptr;
    return false;
}

// ---- DeconstructResultInstruction

DeconstructResultInstruction::DeconstructResultInstruction(int index, StackType result_type, ILInstruction* argument)
    : UnaryInstruction(OpCode::DeconstructResultInstruction, argument), index_(index), result_type_(result_type) {
    assert(index >= 0);
}

ILInstruction* DeconstructResultInstruction::clone() const {
    auto* clone = make<DeconstructResultInstruction>(*this);
    clone_children_into(*clone);
    return clone;
}

bool DeconstructResultInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<DeconstructResultInstruction>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match);
}

void DeconstructResultInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    additional_invariants();
}

void DeconstructResultInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    output.write(std::to_string(index_));
    output.write('(');
    argument()->write_to(output, options);
    output.write(')');
}

MatchInstruction* DeconstructResultInstruction::find_match() const {
    for (ILInstruction* inst = const_cast<DeconstructResultInstruction*>(this); inst != nullptr; inst = inst->parent()) {
        auto* match = as<MatchInstruction>(inst->parent());
        if (match != nullptr && inst != match->tested_operand()) return match;
    }
    return nullptr;
}

void DeconstructResultInstruction::additional_invariants() const {
    [[maybe_unused]] MatchInstruction* match_inst = find_match();
    assert(match_inst != nullptr && (match_inst->is_deconstruct_call || match_inst->is_deconstruct_tuple));
    assert(argument()->match_ld_loc(match_inst->variable()));
    [[maybe_unused]] const IType* out_param_type = match_inst->get_deconstruct_result_type(index_);
    assert(get_stack_type(*out_param_type) == result_type_);
}

// ---- DeconstructInstruction

const SlotInfo DeconstructInstruction::init_slot{"Init", true, true};
const SlotInfo DeconstructInstruction::pattern_slot{"Pattern", true};
const SlotInfo DeconstructInstruction::conversions_slot{"Conversions"};
const SlotInfo DeconstructInstruction::assignments_slot{"Assignments"};

DeconstructInstruction::DeconstructInstruction() : ILInstruction(OpCode::DeconstructInstruction), init(this, 0) {}

void DeconstructInstruction::set_pattern(MatchInstruction* v) {
    validate_child(v);
    set_child_instruction(pattern_, v, init.count());
}
void DeconstructInstruction::set_conversions(Block* v) {
    validate_child(v);
    set_child_instruction(conversions_, v, init.count() + 1);
}
void DeconstructInstruction::set_assignments(Block* v) {
    validate_child(v);
    set_child_instruction(assignments_, v, init.count() + 2);
}

ILInstruction* DeconstructInstruction::get_child(int index) const {
    switch (index - init.count()) {
        case 0: return pattern_;
        case 1: return conversions_;
        case 2: return assignments_;
        default: return init[index];
    }
}
void DeconstructInstruction::set_child(int index, ILInstruction* value) {
    switch (index - init.count()) {
        case 0: return set_pattern(checked_cast<MatchInstruction>(value));
        case 1: return set_conversions(checked_cast<Block>(value));
        case 2: return set_assignments(checked_cast<Block>(value));
        default: init.set(index, checked_cast<StLoc>(value));
    }
}
const SlotInfo& DeconstructInstruction::get_child_slot(int index) const {
    switch (index - init.count()) {
        case 0: return pattern_slot;
        case 1: return conversions_slot;
        case 2: return assignments_slot;
        default: return init_slot;
    }
}

ILInstruction* DeconstructInstruction::clone() const {
    auto* clone = make<DeconstructInstruction>();
    std::vector<StLoc*> cloned_init;
    for (StLoc* inst : init) cloned_init.push_back(static_cast<StLoc*>(inst->clone()));
    clone->init.add_range(cloned_init);
    clone->set_pattern(static_cast<MatchInstruction*>(pattern_->clone()));
    clone->set_conversions(static_cast<Block*>(conversions_->clone()));
    clone->set_assignments(static_cast<Block*>(assignments_->clone()));
    return clone;
}

InstructionFlags DeconstructInstruction::compute_flags() const {
    InstructionFlags flags = InstructionFlags::None;
    for (StLoc* inst : init) flags |= inst->flags();
    flags |= pattern_->flags() | conversions_->flags() | assignments_->flags();
    return flags;
}

void DeconstructInstruction::instruction_collection_update_complete() {
    ILInstruction::instruction_collection_update_complete();
    if (pattern_ != nullptr && pattern_->parent() == this) pattern_->set_child_index(init.count());
    if (conversions_ != nullptr && conversions_->parent() == this) conversions_->set_child_index(init.count() + 1);
    if (assignments_ != nullptr && assignments_->parent() == this) assignments_->set_child_index(init.count() + 2);
}

bool DeconstructInstruction::perform_match(ILInstruction* other, patterns::Match&) { return as<DeconstructInstruction>(other) != nullptr; }

void DeconstructInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write("deconstruct ");
    output.mark_fold_start("{...}");
    output.write_line("{");
    output.indent();
    output.write_line("init:");
    output.indent();
    for (StLoc* inst : init) {
        inst->write_to(output, options);
        output.write_line();
    }
    output.unindent();
    output.write_line("pattern:");
    output.indent();
    pattern_->write_to(output, options);
    output.unindent();
    output.write_line();
    output.write("conversions: ");
    conversions_->write_to(output, options);
    output.write_line();
    output.write("assignments: ");
    assignments_->write_to(output, options);
    output.unindent();
    output.write_line();
    output.write('}');
    output.mark_fold_end();
}

bool DeconstructInstruction::is_conversion_stloc(ILInstruction* inst, ILVariable** variable, ILVariable** input_variable) {
    *input_variable = nullptr;
    ILInstruction* value = nullptr;
    if (!inst->match_st_loc(variable, &value)) return false;
    ILInstruction* input;
    if (auto* conv = as<Conv>(value))
        input = conv->argument();
    else
        return false;
    return input->match_ld_loc(input_variable) || input->match_ld_loca(input_variable);
}

namespace {
// The element type of a pointer-typed target. A pointer passing through a stack slot is typed IntPtr there, so the declared type has
// to be taken from the definition the slot was filled from. nullptr if the target is not a pointer.
const IType* get_pointer_element_type(ILInstruction* target, const ICompilation* type_system) {
    // Bounded because a definition chain could be cyclic in invalid IL.
    for (int step = 0; step < 4; step++) {
        if (const auto* pointer_type = dynamic_cast<const typesystem::PointerType*>(infer_type(target, type_system))) return pointer_type->element_type();
        ILVariable* v = nullptr;
        if (!target->match_ld_loc(&v) || !v->is_single_definition() || v->store_instructions().size() != 1) return nullptr;
        auto* store = as<StLoc>(v->store_instructions()[0]->as_instruction());
        if (store == nullptr) return nullptr;
        target = store->value();
    }
    return nullptr;
}
}  // namespace

bool DeconstructInstruction::is_assignment(ILInstruction* inst, const ICompilation* type_system, const IType** expected_type,
                                           ILInstruction** value) {
    *expected_type = nullptr;
    *value = nullptr;
    if (auto* call = as<CallInstruction>(inst)) {
        if (call->method->accessor_kind() != typesystem::MethodSemanticsAttributes::Setter) return false;
        for (int i = 0; i < call->arguments().count() - 1; i++) {
            ILInstruction* arg = call->arguments()[i];
            ILVariable* v = nullptr;
            if (arg->flags() == InstructionFlags::None) {
                // OK - we accept integer literals, etc.
            } else if (arg->match_ld_loc(&v)) {
            } else {
                return false;
            }
        }
        *expected_type = call->method->parameters().back()->type();
        *value = call->arguments().last();
        return true;
    }
    if (auto* stloc = as<StLoc>(inst)) {
        *expected_type = stloc->variable()->type();
        *value = stloc->value();
        return true;
    }
    if (auto* stobj = as<StObj>(inst)) {
        ILInstruction* target = stobj->target();
        ILInstruction* nested_target = nullptr;
        const IField* field = nullptr;
        while (target->match_ld_flda(&nested_target, &field)) target = nested_target;
        ILVariable* v = nullptr;
        if (target->flags() == InstructionFlags::None) {
            // OK - we accept integer literals, etc.
        } else if (target->match_ld_loc(&v)) {
        } else {
            return false;
        }
        if (const auto* brt = dynamic_cast<const typesystem::ByReferenceType*>(infer_type(stobj->target(), type_system))) {
            *expected_type = brt->element_type();
        } else {
            // Pointer targets do not infer a ByReferenceType. stobj.Type cannot stand in for the element type: it comes from the store
            // opcode, which is sign-agnostic, so a uint* and an int* both report int32 and a byte* reports sbyte. Recover the declared
            // type instead, and only fall back to the store where the target is not a pointer at all.
            const IType* element = get_pointer_element_type(stobj->target(), type_system);
            *expected_type = element != nullptr ? element : stobj->type();
        }
        *value = stobj->value();
        return true;
    }
    return false;
}

namespace {
// ValidatePattern of DeconstructInstruction::check_invariant
void validate_pattern(const DeconstructInstruction* self, MatchInstruction* inst, std::unordered_set<ILVariable*>& pattern_variables) {
    assert(inst->is_deconstruct_call || inst->is_deconstruct_tuple);
    assert(!inst->check_not_null && !inst->check_type);
    assert(!inst->has_designator());
    for (ILInstruction* element : inst->sub_patterns()) {
        auto* sub_pattern = static_cast<MatchInstruction*>(element);
        if (sub_pattern->is_var()) {
            assert(sub_pattern->variable()->is_single_definition() && sub_pattern->variable()->load_count() <= 1);
            if (sub_pattern->variable()->load_count() == 1) assert(sub_pattern->variable()->load_instructions()[0]->is_descendant_of(self));
            pattern_variables.insert(sub_pattern->variable());
        } else {
            validate_pattern(self, sub_pattern, pattern_variables);
        }
    }
}
}  // namespace

void DeconstructInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    std::unordered_set<ILVariable*> pattern_variables;
    std::unordered_set<ILVariable*> conversion_variables;

    for ([[maybe_unused]] StLoc* init_inst : init) {
        assert(init_inst->variable()->is_single_definition() && init_inst->variable()->load_count() == 1);
        assert(init_inst->variable()->load_instructions()[0]->is_descendant_of(assignments_));
    }

    validate_pattern(this, pattern_, pattern_variables);

    for (ILInstruction* inst : conversions_->instructions) {
        ILVariable* variable = nullptr;
        ILVariable* input_variable = nullptr;
        if (!is_conversion_stloc(inst, &variable, &input_variable)) assert(false && "inst is not a conversion stloc!");
        assert(variable->is_single_definition() && variable->load_count() == 1);
        assert(variable->load_instructions()[0]->is_descendant_of(assignments_));
        assert(pattern_variables.contains(input_variable));
        conversion_variables.insert(variable);
    }
    assert(conversions_->final_instruction()->op_code == OpCode::Nop);

    for (ILInstruction* inst : assignments_->instructions) {
        const IType* expected_type = nullptr;
        ILInstruction* value = nullptr;
        ILVariable* input_variable = nullptr;
        if (!(is_assignment(inst, nullptr, &expected_type, &value) && value->match_ld_loc(&input_variable)))
            throw UsageError("inst is not an assignment!");
        assert(pattern_variables.contains(input_variable) || conversion_variables.contains(input_variable));
    }
    assert(assignments_->final_instruction()->op_code == OpCode::Nop);
}

}  // namespace cb::dotnet::decompiler::il
