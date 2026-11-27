// dotnet/decompiler/il/advanced_instructions.hpp -- the ILAst's compound assignments (`a += b`, `a++`, on addresses, properties and
// dynamic targets), dynamic call sites (`dynamic` expressions), the user-defined `&&`/`||`, C# pattern matching (match, deconstruct,
// deconstruct.result), and the enums they use (System.Linq.Expressions.ExpressionType and the C# runtime binder's flags, with .NET's
// names and values since the ILAst text spells them).
//
//     auto* add = make<NumericCompoundAssign>(binary, target, CompoundTargetKind::Address, value, type, CompoundEvalMode::EvaluatesToNewValue);
//     auto* get = make<DynamicGetMemberInstruction>(CSharpBinderFlags::None, "Length", context, CSharpArgumentInfo{}, target);
//     auto* m = make<MatchInstruction>(variable, tested_operand);   // `expr is var x`
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: CompoundEvalMode, CompoundTargetKind, CompoundAssignmentInstruction,
// NumericCompoundAssign, UserDefinedCompoundAssign, DynamicCompoundAssign, CSharpArgumentInfoFlags, CSharpBinderFlags,
// CSharpArgumentInfo, DynamicInstruction and the Dynamic*Instruction classes, UserDefinedLogicOperator, MatchInstruction,
// DeconstructInstruction, DeconstructResultInstruction, from Instructions.cs and Instructions/CompoundAssignmentInstruction.cs,
// DynamicInstructions.cs, MatchInstruction.cs, DeconstructInstruction.cs, DeconstructResultInstruction.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not ported yet: the DecompilerSettings switches (settings are passed as
// a pointer to a type that does not exist yet; every switch reads as upstream's default, on).
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/il_variable.hpp"
#include "dotnet/decompiler/il/instructions_base.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
}
namespace cb::dotnet::decompiler::typesystem {
enum class KnownTypeCode : std::uint8_t;
}

namespace cb::dotnet::decompiler::il {

using decompiler::DecompilerSettings;
class BinaryNumericInstruction;
class CallInstruction;
class Call;
class StLoc;
class MatchInstruction;

// ---- System.Linq.Expressions.ExpressionType (values as .NET's) -------------------------------------------------------------------

enum class ExpressionType : std::int32_t {
    Add = 0, AddChecked = 1, And = 2, AndAlso = 3, ArrayLength = 4, ArrayIndex = 5, Call = 6, Coalesce = 7, Conditional = 8, Constant = 9,
    Convert = 10, ConvertChecked = 11, Divide = 12, Equal = 13, ExclusiveOr = 14, GreaterThan = 15, GreaterThanOrEqual = 16, Invoke = 17,
    Lambda = 18, LeftShift = 19, LessThan = 20, LessThanOrEqual = 21, ListInit = 22, MemberAccess = 23, MemberInit = 24, Modulo = 25,
    Multiply = 26, MultiplyChecked = 27, Negate = 28, UnaryPlus = 29, NegateChecked = 30, New = 31, NewArrayInit = 32, NewArrayBounds = 33,
    Not = 34, NotEqual = 35, Or = 36, OrElse = 37, Parameter = 38, Power = 39, Quote = 40, RightShift = 41, Subtract = 42,
    SubtractChecked = 43, TypeAs = 44, TypeIs = 45, Assign = 46, Block = 47, DebugInfo = 48, Decrement = 49, Dynamic = 50, Default = 51,
    Extension = 52, Goto = 53, Increment = 54, Index = 55, Label = 56, RuntimeVariables = 57, Loop = 58, Switch = 59, Throw = 60, Try = 61,
    Unbox = 62, AddAssign = 63, AndAssign = 64, DivideAssign = 65, ExclusiveOrAssign = 66, LeftShiftAssign = 67, ModuloAssign = 68,
    MultiplyAssign = 69, OrAssign = 70, PowerAssign = 71, RightShiftAssign = 72, SubtractAssign = 73, AddAssignChecked = 74,
    MultiplyAssignChecked = 75, SubtractAssignChecked = 76, PreIncrementAssign = 77, PreDecrementAssign = 78, PostIncrementAssign = 79,
    PostDecrementAssign = 80, TypeEqual = 81, OnesComplement = 82, IsTrue = 83, IsFalse = 84,
};
// The enum's name (C#'s ToString()); the number for a value that has none.
std::string to_string(ExpressionType type);

// ---- the C# runtime binder's flags (Microsoft.CSharp.RuntimeBinder; defined here with the same names and values) --------------------

enum class CSharpArgumentInfoFlags : std::int32_t {
    None = 0,
    UseCompileTimeType = 1,
    Constant = 2,
    NamedArgument = 4,
    IsRef = 8,
    IsOut = 0x10,
    IsStaticType = 0x20,
};
constexpr CSharpArgumentInfoFlags operator|(CSharpArgumentInfoFlags a, CSharpArgumentInfoFlags b) {
    return static_cast<CSharpArgumentInfoFlags>(static_cast<std::int32_t>(a) | static_cast<std::int32_t>(b));
}
constexpr CSharpArgumentInfoFlags operator&(CSharpArgumentInfoFlags a, CSharpArgumentInfoFlags b) {
    return static_cast<CSharpArgumentInfoFlags>(static_cast<std::int32_t>(a) & static_cast<std::int32_t>(b));
}
// C#'s ToString() of a [Flags] enum: the set names joined with ", " ("None" for zero), or the number when a bit has no name.
std::string to_string(CSharpArgumentInfoFlags flags);

enum class CSharpBinderFlags : std::int32_t {
    None = 0,
    CheckedContext = 1,
    InvokeSimpleName = 2,
    InvokeSpecialName = 4,
    BinaryOperationLogical = 8,
    ConvertExplicit = 0x10,
    ConvertArrayIndex = 0x20,
    ResultIndexed = 0x40,
    ValueFromCompoundAssignment = 0x80,
    ResultDiscarded = 0x100,
};
constexpr CSharpBinderFlags operator|(CSharpBinderFlags a, CSharpBinderFlags b) {
    return static_cast<CSharpBinderFlags>(static_cast<std::int32_t>(a) | static_cast<std::int32_t>(b));
}
constexpr CSharpBinderFlags operator&(CSharpBinderFlags a, CSharpBinderFlags b) {
    return static_cast<CSharpBinderFlags>(static_cast<std::int32_t>(a) & static_cast<std::int32_t>(b));
}

struct CSharpArgumentInfo {
    std::string name;  // empty for none
    CSharpArgumentInfoFlags flags = CSharpArgumentInfoFlags::None;
    const IType* compile_time_type = nullptr;

    bool has_flag(CSharpArgumentInfoFlags flag) const { return (flags & flag) != CSharpArgumentInfoFlags::None; }
};

// ---- compound assignments --------------------------------------------------------------------------------------------------------

enum class CompoundEvalMode : std::uint8_t {
    // post-increment/decrement
    EvaluatesToOldValue,
    // compound assignments and pre-increment/decrement
    EvaluatesToNewValue,
};

enum class CompoundTargetKind : std::uint8_t {
    // the target computes an address; the compound.assign implicitly loads from and stores to it
    Address,
    // the target is a call to a property getter; the compound.assign implicitly calls the setter
    Property,
    // the target is a dynamic call
    Dynamic,
};

class CompoundAssignmentInstruction : public ILInstruction {
public:
    const CompoundEvalMode eval_mode;
    const CompoundTargetKind target_kind;

    static const SlotInfo target_slot;
    static const SlotInfo value_slot;
    ILInstruction* target() const { return target_; }
    void set_target(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(target_, v, 0);
    }
    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 1);
    }
    int get_child_count() const final { return 2; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }
    void check_invariant(ILPhase phase) override;

protected:
    CompoundAssignmentInstruction(OpCode op_code, CompoundEvalMode eval_mode, ILInstruction* target, CompoundTargetKind target_kind,
                                  ILInstruction* value);
    CompoundAssignmentInstruction(const CompoundAssignmentInstruction&) = default;
    void clone_children_into(CompoundAssignmentInstruction& clone) const {
        clone.set_target(target_->clone());
        clone.set_value(value_->clone());
    }
    InstructionFlags compute_flags() const override { return target_->flags() | value_->flags(); }
    void write_suffix(ITextOutput& output) const;

private:
    void check_valid_target() const;

    ILInstruction* target_ = nullptr;
    ILInstruction* value_ = nullptr;
};

class NumericCompoundAssign final : public CompoundAssignmentInstruction, public ILiftableInstruction {
public:
    static constexpr OpCode opcode = OpCode::NumericCompoundAssign;

    // whether the instruction checks for overflow
    const bool check_for_overflow;
    // for integer operations that depend on the sign; Sign::None for the ones that produce the same result either way
    const Sign sign;
    const StackType left_input_type;
    const StackType right_input_type;
    // the operator used by this assignment instruction
    const BinaryNumericOperator op;

    NumericCompoundAssign(const BinaryNumericInstruction* binary, ILInstruction* target, CompoundTargetKind target_kind, ILInstruction* value,
                          const IType* type, CompoundEvalMode eval_mode);

    // Whether the binary instruction is compatible with a compound operation on the type.
    static bool is_binary_compatible_with_type(const BinaryNumericInstruction* binary, const IType* type, const DecompilerSettings* settings);

    const IType* type() const { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    bool is_lifted() const override { return is_lifted_; }
    StackType underlying_result_type() const override { return underlying_result_type_; }
    StackType result_type() const override;
    InstructionFlags direct_flags() const override;
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
    const bool is_lifted_;
    const StackType underlying_result_type_;
};

class UserDefinedCompoundAssign final : public CompoundAssignmentInstruction {
public:
    static constexpr OpCode opcode = OpCode::UserDefinedCompoundAssign;

    const IMethod* const method;

    UserDefinedCompoundAssign(const IMethod* method, CompoundEvalMode eval_mode, ILInstruction* target, CompoundTargetKind target_kind,
                              ILInstruction* value);

    // a static operator ++ or -- (also the checked ones, unless the settings turn them off)
    static bool is_increment_or_decrement(const IMethod* method, const DecompilerSettings* settings = nullptr);
    static bool is_string_concat(const IMethod* method);

    // TODO upstream: lifted user-defined compound assignments
    bool is_lifted() const { return false; }
    StackType result_type() const override;
    InstructionFlags direct_flags() const override;
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

class DynamicCompoundAssign final : public CompoundAssignmentInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicCompoundAssign;

    DynamicCompoundAssign(ExpressionType op, CSharpBinderFlags binder_flags, ILInstruction* target, CSharpArgumentInfo target_argument_info,
                          ILInstruction* value, CSharpArgumentInfo value_argument_info,
                          CompoundTargetKind target_kind = CompoundTargetKind::Dynamic);

    ExpressionType operation() const { return operation_; }
    const CSharpArgumentInfo& target_argument_info() const { return target_argument_info_; }
    const CSharpArgumentInfo& value_argument_info() const { return value_argument_info_; }
    CSharpBinderFlags binder_flags() const { return binder_flags_; }

    static bool is_expression_type_supported(ExpressionType type);

    StackType result_type() const override { return StackType::O; }
    InstructionFlags direct_flags() const override;
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    static CompoundEvalMode compound_eval_mode_from_operation(ExpressionType op);

    ExpressionType operation_;
    CSharpArgumentInfo target_argument_info_;
    CSharpArgumentInfo value_argument_info_;
    CSharpBinderFlags binder_flags_;
};

// ---- user-defined `&&` and `||` --------------------------------------------------------------------------------------------------

class UserDefinedLogicOperator final : public ILInstruction, public IInstructionWithMethodOperand {
public:
    static constexpr OpCode opcode = OpCode::UserDefinedLogicOperator;

    UserDefinedLogicOperator(const IMethod* method, ILInstruction* left, ILInstruction* right);

    const IMethod* method() const override { return method_; }
    StackType result_type() const override { return StackType::O; }

    static const SlotInfo left_slot;
    static const SlotInfo right_slot;
    ILInstruction* left() const { return left_; }
    void set_left(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(left_, v, 0);
    }
    ILInstruction* right() const { return right_; }
    void set_right(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(right_, v, 1);
    }
    int get_child_count() const final { return 2; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return left_->flags() | right_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IMethod* method_;
    ILInstruction* left_ = nullptr;
    ILInstruction* right_ = nullptr;
};

// ---- dynamic call sites ----------------------------------------------------------------------------------------------------------

// An instruction representing a dynamic call site.
class DynamicInstruction : public ILInstruction {
public:
    CSharpBinderFlags binder_flags() const { return binder_flags_; }
    const IType* calling_context() const { return calling_context_; }

    // The argument info of the child at `index`.
    virtual CSharpArgumentInfo get_argument_info_of_child(int index) const = 0;

    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::SideEffect; }

    static void write_binder_flags(CSharpBinderFlags flags, ITextOutput& output, const ILAstWritingOptions& options);
    // `([flags: ..., name: ...] arg, ...)`
    static void write_argument_list(ITextOutput& output, const ILAstWritingOptions& options,
                                    const std::vector<std::pair<ILInstruction*, CSharpArgumentInfo>>& arguments);

protected:
    DynamicInstruction(OpCode op_code, CSharpBinderFlags binder_flags, const IType* context)
        : ILInstruction(op_code), binder_flags_(binder_flags), calling_context_(context) {}
    DynamicInstruction(const DynamicInstruction&) = default;
    InstructionFlags compute_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::SideEffect; }
    void write_binder_flags(ITextOutput& output, const ILAstWritingOptions& options) const { write_binder_flags(binder_flags_, output, options); }

private:
    CSharpBinderFlags binder_flags_;
    const IType* calling_context_;
};

// A short-circuiting binary operator inside a dynamic expression.
class DynamicLogicOperatorInstruction final : public DynamicInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicLogicOperatorInstruction;

    DynamicLogicOperatorInstruction(CSharpBinderFlags binder_flags, ExpressionType operation, const IType* context,
                                    CSharpArgumentInfo left_argument_info, ILInstruction* left, CSharpArgumentInfo right_argument_info,
                                    ILInstruction* right);

    const CSharpArgumentInfo& left_argument_info() const { return left_argument_info_; }
    const CSharpArgumentInfo& right_argument_info() const { return right_argument_info_; }
    ExpressionType operation() const { return operation_; }

    static const SlotInfo left_slot;
    static const SlotInfo right_slot;
    ILInstruction* left() const { return left_; }
    void set_left(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(left_, v, 0);
    }
    ILInstruction* right() const { return right_; }
    void set_right(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(right_, v, 1);
    }
    int get_child_count() const final { return 2; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    InstructionFlags direct_flags() const override {
        return InstructionFlags::MayThrow | InstructionFlags::SideEffect | InstructionFlags::ControlFlow;
    }
    CSharpArgumentInfo get_argument_info_of_child(int index) const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ExpressionType operation_;
    CSharpArgumentInfo left_argument_info_;
    CSharpArgumentInfo right_argument_info_;
    ILInstruction* left_ = nullptr;
    ILInstruction* right_ = nullptr;
};

// A binary operator inside a dynamic expression (Binder.BinaryOperation).
class DynamicBinaryOperatorInstruction final : public DynamicInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicBinaryOperatorInstruction;

    DynamicBinaryOperatorInstruction(CSharpBinderFlags binder_flags, ExpressionType operation, const IType* context,
                                     CSharpArgumentInfo left_argument_info, ILInstruction* left, CSharpArgumentInfo right_argument_info,
                                     ILInstruction* right);

    const CSharpArgumentInfo& left_argument_info() const { return left_argument_info_; }
    const CSharpArgumentInfo& right_argument_info() const { return right_argument_info_; }
    ExpressionType operation() const { return operation_; }

    static const SlotInfo left_slot;
    static const SlotInfo right_slot;
    ILInstruction* left() const { return left_; }
    void set_left(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(left_, v, 0);
    }
    ILInstruction* right() const { return right_; }
    void set_right(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(right_, v, 1);
    }
    int get_child_count() const final { return 2; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    CSharpArgumentInfo get_argument_info_of_child(int index) const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ExpressionType operation_;
    CSharpArgumentInfo left_argument_info_;
    CSharpArgumentInfo right_argument_info_;
    ILInstruction* left_ = nullptr;
    ILInstruction* right_ = nullptr;
};

// A unary operator inside a dynamic expression (Binder.UnaryOperation).
class DynamicUnaryOperatorInstruction final : public DynamicInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicUnaryOperatorInstruction;

    DynamicUnaryOperatorInstruction(CSharpBinderFlags binder_flags, ExpressionType operation, const IType* context,
                                    CSharpArgumentInfo operand_argument_info, ILInstruction* operand);

    const CSharpArgumentInfo& operand_argument_info() const { return operand_argument_info_; }
    ExpressionType operation() const { return operation_; }

    static const SlotInfo operand_slot;
    ILInstruction* operand() const { return operand_; }
    void set_operand(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(operand_, v, 0);
    }
    int get_child_count() const final { return 1; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override;
    CSharpArgumentInfo get_argument_info_of_child(int index) const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ExpressionType operation_;
    CSharpArgumentInfo operand_argument_info_;
    ILInstruction* operand_ = nullptr;
};

// A cast inside a dynamic expression (Binder.Convert).
class DynamicConvertInstruction final : public DynamicInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicConvertInstruction;

    DynamicConvertInstruction(CSharpBinderFlags binder_flags, const IType* type, const IType* context, ILInstruction* argument);

    const IType* type() const { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    bool is_checked() const { return (binder_flags() & CSharpBinderFlags::CheckedContext) != CSharpBinderFlags::None; }
    bool is_explicit() const { return (binder_flags() & CSharpBinderFlags::ConvertExplicit) != CSharpBinderFlags::None; }

    static const SlotInfo argument_slot;
    ILInstruction* argument() const { return argument_; }
    void set_argument(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(argument_, v, 0);
    }
    int get_child_count() const final { return 1; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override;
    CSharpArgumentInfo get_argument_info_of_child(int index) const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
    ILInstruction* argument_ = nullptr;
};

// A property get inside a dynamic expression (Binder.GetMember).
class DynamicGetMemberInstruction final : public DynamicInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicGetMemberInstruction;

    DynamicGetMemberInstruction(CSharpBinderFlags binder_flags, std::string name, const IType* context,
                                CSharpArgumentInfo target_argument_info, ILInstruction* target);

    const std::string& name() const { return name_; }
    const CSharpArgumentInfo& target_argument_info() const { return target_argument_info_; }

    static const SlotInfo target_slot;
    ILInstruction* target() const { return target_; }
    void set_target(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(target_, v, 0);
    }
    int get_child_count() const final { return 1; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    CSharpArgumentInfo get_argument_info_of_child(int index) const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    std::string name_;
    CSharpArgumentInfo target_argument_info_;
    ILInstruction* target_ = nullptr;
};

// A property set inside a dynamic expression (Binder.SetMember).
class DynamicSetMemberInstruction final : public DynamicInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicSetMemberInstruction;

    DynamicSetMemberInstruction(CSharpBinderFlags binder_flags, std::string name, const IType* context,
                                CSharpArgumentInfo target_argument_info, ILInstruction* target, CSharpArgumentInfo value_argument_info,
                                ILInstruction* value);

    const std::string& name() const { return name_; }
    const CSharpArgumentInfo& target_argument_info() const { return target_argument_info_; }
    const CSharpArgumentInfo& value_argument_info() const { return value_argument_info_; }

    static const SlotInfo target_slot;
    static const SlotInfo value_slot;
    ILInstruction* target() const { return target_; }
    void set_target(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(target_, v, 0);
    }
    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 1);
    }
    int get_child_count() const final { return 2; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    CSharpArgumentInfo get_argument_info_of_child(int index) const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    std::string name_;
    CSharpArgumentInfo target_argument_info_;
    CSharpArgumentInfo value_argument_info_;
    ILInstruction* target_ = nullptr;
    ILInstruction* value_ = nullptr;
};

// The dynamic call sites with a list of arguments (each has its argument info beside it): the children are the arguments, in a
// collection from child 0. Upstream writes this once per class; here they share this base.
class DynamicArgumentsInstruction : public DynamicInstruction {
public:
    static const SlotInfo arguments_slot;
    InstructionCollection<ILInstruction>& arguments() { return arguments_; }
    const InstructionCollection<ILInstruction>& arguments() const { return arguments_; }
    const std::vector<CSharpArgumentInfo>& argument_info() const { return argument_info_; }

    int get_child_count() const final { return arguments_.count(); }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    CSharpArgumentInfo get_argument_info_of_child(int index) const final;

protected:
    DynamicArgumentsInstruction(OpCode op_code, CSharpBinderFlags binder_flags, const IType* context,
                                std::vector<CSharpArgumentInfo> argument_info, const std::vector<ILInstruction*>& arguments);
    // The copy has the same argument infos and a fresh, empty collection (upstream: `clone.Arguments = new InstructionCollection<>(clone, 0)`).
    DynamicArgumentsInstruction(const DynamicArgumentsInstruction& other)
        : DynamicInstruction(other), argument_info_(other.argument_info_), arguments_(this, 0) {}
    InstructionFlags compute_flags() const override;
    // The clone's arguments, cloned.
    void clone_children_into(DynamicArgumentsInstruction& clone) const;
    std::vector<std::pair<ILInstruction*, CSharpArgumentInfo>> argument_list() const;

    std::vector<CSharpArgumentInfo> argument_info_;

private:
    InstructionCollection<ILInstruction> arguments_;
};

// An indexer get inside a dynamic expression (Binder.GetIndex).
class DynamicGetIndexInstruction final : public DynamicArgumentsInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicGetIndexInstruction;

    DynamicGetIndexInstruction(CSharpBinderFlags binder_flags, const IType* context, std::vector<CSharpArgumentInfo> argument_info,
                               const std::vector<ILInstruction*>& arguments)
        : DynamicArgumentsInstruction(opcode, binder_flags, context, std::move(argument_info), arguments) {}
    DynamicGetIndexInstruction(const DynamicGetIndexInstruction&) = default;

    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// An indexer set inside a dynamic expression (Binder.SetIndex).
class DynamicSetIndexInstruction final : public DynamicArgumentsInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicSetIndexInstruction;

    DynamicSetIndexInstruction(CSharpBinderFlags binder_flags, const IType* context, std::vector<CSharpArgumentInfo> argument_info,
                               const std::vector<ILInstruction*>& arguments)
        : DynamicArgumentsInstruction(opcode, binder_flags, context, std::move(argument_info), arguments) {}
    DynamicSetIndexInstruction(const DynamicSetIndexInstruction&) = default;

    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// A method call inside a dynamic expression (Binder.InvokeMember).
class DynamicInvokeMemberInstruction final : public DynamicArgumentsInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicInvokeMemberInstruction;

    DynamicInvokeMemberInstruction(CSharpBinderFlags binder_flags, std::string name, std::vector<const IType*> type_arguments,
                                   const IType* context, std::vector<CSharpArgumentInfo> argument_info,
                                   const std::vector<ILInstruction*>& arguments)
        : DynamicArgumentsInstruction(opcode, binder_flags, context, std::move(argument_info), arguments),
          name_(std::move(name)), type_arguments_(std::move(type_arguments)) {}
    DynamicInvokeMemberInstruction(const DynamicInvokeMemberInstruction&) = default;

    const std::string& name() const { return name_; }
    const std::vector<const IType*>& type_arguments() const { return type_arguments_; }

    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    std::string name_;
    std::vector<const IType*> type_arguments_;
};

// A constructor invocation inside a dynamic expression (Binder.InvokeConstructor).
class DynamicInvokeConstructorInstruction final : public DynamicArgumentsInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicInvokeConstructorInstruction;

    DynamicInvokeConstructorInstruction(CSharpBinderFlags binder_flags, const IType* type, const IType* context,
                                        std::vector<CSharpArgumentInfo> argument_info, const std::vector<ILInstruction*>& arguments)
        : DynamicArgumentsInstruction(opcode, binder_flags, context, std::move(argument_info), arguments), result_type_(type) {}
    DynamicInvokeConstructorInstruction(const DynamicInvokeConstructorInstruction&) = default;

    ILInstruction* clone() const override;
    StackType result_type() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* result_type_;  // may be null
};

// A delegate invocation inside a dynamic expression (Binder.Invoke).
class DynamicInvokeInstruction final : public DynamicArgumentsInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicInvokeInstruction;

    DynamicInvokeInstruction(CSharpBinderFlags binder_flags, const IType* context, std::vector<CSharpArgumentInfo> argument_info,
                             const std::vector<ILInstruction*>& arguments)
        : DynamicArgumentsInstruction(opcode, binder_flags, context, std::move(argument_info), arguments) {}
    DynamicInvokeInstruction(const DynamicInvokeInstruction&) = default;

    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// A call of Binder.IsEvent inside a dynamic expression.
class DynamicIsEventInstruction final : public DynamicInstruction {
public:
    static constexpr OpCode opcode = OpCode::DynamicIsEventInstruction;

    DynamicIsEventInstruction(CSharpBinderFlags binder_flags, std::string name, const IType* context, ILInstruction* argument);

    const std::string& name() const { return name_; }

    static const SlotInfo argument_slot;
    ILInstruction* argument() const { return argument_; }
    void set_argument(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(argument_, v, 0);
    }
    int get_child_count() const final { return 1; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::I4; }
    CSharpArgumentInfo get_argument_info_of_child(int index) const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    std::string name_;
    ILInstruction* argument_ = nullptr;
};

// ---- C# patterns -----------------------------------------------------------------------------------------------------------------

// ILAst representation of C# patterns. Evaluates the tested operand, then matches it against the pattern; the matching writes the
// pattern variables (even when it fails). The value is 1 (I4) when the pattern matches, else 0.
//     expr is var x            match(x = expr)
//     expr is T x              match.type[T](x = expr)
//     expr is C(var x, <4)     match.type[C].deconstruct[C.Deconstruct](t = expr) { match(x = deconstruct.result1(t)), comp(...) }
class MatchInstruction final : public ILInstruction, public IStoreInstruction, public IInstructionWithMethodOperand {
public:
    static constexpr OpCode opcode = OpCode::MatchInstruction;

    MatchInstruction(ILVariable* variable, const IMethod* method, ILInstruction* tested_operand,
                     const std::vector<ILInstruction*>& sub_patterns = {});
    MatchInstruction(ILVariable* variable, ILInstruction* tested_operand) : MatchInstruction(variable, nullptr, tested_operand) {}
    // The copy is disconnected and has a fresh, empty collection of sub-patterns.
    MatchInstruction(const MatchInstruction& other);

    ILVariable* variable() const override { return variable_; }
    void set_variable(ILVariable* value) override;
    ILInstruction* as_instruction() override { return this; }
    const IMethod* method() const override { return method_; }  // may be null

    bool is_deconstruct_call = false;
    bool is_deconstruct_tuple = false;
    bool check_type = false;
    bool check_not_null = false;

    bool is_var() const { return !check_type && !check_not_null && !is_deconstruct_call && !is_deconstruct_tuple && sub_patterns_.count() == 0; }
    bool has_designator() const { return variable_->load_count() + variable_->address_count() > sub_patterns_.count(); }
    int num_positional_patterns() const;

    static const SlotInfo tested_operand_slot;
    static const SlotInfo sub_patterns_slot;
    ILInstruction* tested_operand() const { return tested_operand_; }
    void set_tested_operand(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(tested_operand_, v, 0);
    }
    InstructionCollection<ILInstruction>& sub_patterns() { return sub_patterns_; }
    const InstructionCollection<ILInstruction>& sub_patterns() const { return sub_patterns_; }

    int get_child_count() const final { return 1 + sub_patterns_.count(); }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::I4; }
    InstructionFlags direct_flags() const override {
        return InstructionFlags::MayWriteLocals | InstructionFlags::SideEffect | InstructionFlags::MayThrow | InstructionFlags::ControlFlow;
    }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

    // Whether the instruction can represent a pattern matching operation; its tested operand is a descendant of it.
    static bool is_pattern_match(ILInstruction* inst, ILInstruction** tested_operand, const DecompilerSettings* settings);
    static bool is_call_to_op_equality(const Call* call, typesystem::KnownTypeCode known_type);
    static bool is_constant(const ILInstruction* inst);
    static bool is_deconstruct_method(const IMethod* method);
    // The type of the deconstruct result `index` (the out parameter's element type, or the tuple element's).
    const IType* get_deconstruct_result_type(int index) const;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
    void connected() override;
    void disconnected() override;

private:
    void additional_invariants() const;

    ILVariable* variable_;
    const IMethod* method_;
    ILInstruction* tested_operand_ = nullptr;
    InstructionCollection<ILInstruction> sub_patterns_;
};

// A deconstructed value (the result `index` of the match's deconstruct call or tuple).
class DeconstructResultInstruction final : public UnaryInstruction {
public:
    static constexpr OpCode opcode = OpCode::DeconstructResultInstruction;

    DeconstructResultInstruction(int index, StackType result_type, ILInstruction* argument);

    int index() const { return index_; }
    StackType result_type() const override { return result_type_; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    MatchInstruction* find_match() const;
    void additional_invariants() const;

    int index_;
    StackType result_type_;
};

// `deconstruct { init: ... pattern: ... conversions: ... assignments: ... }`: a deconstruction assignment `(a, b) = expr`.
class DeconstructInstruction final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::DeconstructInstruction;

    static const SlotInfo init_slot;
    static const SlotInfo pattern_slot;
    static const SlotInfo conversions_slot;
    static const SlotInfo assignments_slot;

    DeconstructInstruction();

    InstructionCollection<StLoc> init;

    MatchInstruction* pattern() const { return pattern_; }
    void set_pattern(MatchInstruction* v);
    Block* conversions() const { return conversions_; }
    void set_conversions(Block* v);
    Block* assignments() const { return assignments_; }
    void set_assignments(Block* v);

    int get_child_count() const final { return init.count() + 3; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }
    void instruction_collection_update_complete() override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    void check_invariant(ILPhase phase) override;

    // Whether `inst` is `stloc v(conv(ldloc input))` (or ldloca); the variables are set when it is.
    static bool is_conversion_stloc(ILInstruction* inst, ILVariable** variable, ILVariable** input_variable);
    // Whether `inst` is a property setter call, an stloc or an stobj whose arguments are simple; sets the type the value is stored as
    // and the value.
    static bool is_assignment(ILInstruction* inst, const ICompilation* type_system, const IType** expected_type, ILInstruction** value);

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    MatchInstruction* pattern_ = nullptr;
    Block* conversions_ = nullptr;
    Block* assignments_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il
