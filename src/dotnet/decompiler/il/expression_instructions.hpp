// dotnet/decompiler/il/expression_instructions.hpp -- the ILAst's expression instructions: arithmetic (BinaryNumericInstruction, BitNot,
// Ckfinite, Conv), comparison (Comp), calls (Call, CallVirt, NewObj, CallIndirect), locals (LdLoc, LdLoca, StLoc, AddressOf), three-valued
// logic, nullable lifting (NullableUnwrap, NullableRewrap), constants (LdStr, LdStrUtf8, LdcI4, LdcI8, LdcF4, LdcF8, LdcDecimal, LdNull),
// method and token loads (LdFtn, LdVirtFtn, LdVirtDelegate, LdTypeToken, LdMemberToken), StringToInt and ExpressionTreeCast.
//
//     auto* sum = make<BinaryNumericInstruction>(BinaryNumericOperator::Add, make<LdLoc>(a), make<LdcI4>(1), false, Sign::None);
//     auto* call = make<Call>(method);
//     call->arguments().add(make<LdLoc>(v));
//     if (auto* ldloc = as<LdLoc>(inst)) ldloc->set_variable(other);   // keeps both variables' load lists current while connected
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: BinaryNumericInstruction, BinaryNumericOperator, Comp, ComparisonKind,
// ComparisonLiftingKind, Conv, ConversionKind, BitNot, Ckfinite, CallInstruction, Call, CallVirt, NewObj, CallIndirect, LdLoc, LdLoca,
// StLoc, AddressOf, ThreeValuedBoolAnd, ThreeValuedBoolOr, NullableUnwrap, NullableRewrap, LdStr, LdStrUtf8, LdcI4, LdcI8, LdcF4, LdcF8,
// LdcDecimal, LdNull, LdFtn, LdVirtFtn, LdVirtDelegate, LdTypeToken, LdMemberToken, StringToInt, ExpressionTreeCast, from Instructions.cs
// and Instructions/BinaryNumericInstruction.cs, Comp.cs, Conv.cs, CallInstruction.cs, CallIndirect.cs, StLoc.cs, LogicInstructions.cs,
// NullableInstructions.cs, StringToInt.cs, ExpressionTreeCast.cs, UnaryInstruction.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler). The generated Match* methods of these classes are written in expression_instructions.cpp.
// Naming: a C# field named Operator is `op` here (operator is a C++ keyword); CallInstruction.Arguments (a property) is `arguments()`,
// CallIndirect.Arguments (a public field) is the data member `arguments`.
#pragma once

#include <cassert>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/il_variable.hpp"
#include "dotnet/decompiler/il/instructions_base.hpp"

namespace cb::dotnet::decompiler::typesystem {
class IParameter;
class FunctionPointerType;
}  // namespace cb::dotnet::decompiler::typesystem

namespace cb::dotnet::decompiler::il {

using typesystem::FunctionPointerType;
using typesystem::IParameter;

// ---- arithmetic -------------------------------------------------------------------------------------------------------------------

enum class BinaryNumericOperator : std::uint8_t { None, Add, Sub, Mul, Div, Rem, BitAnd, BitOr, BitXor, ShiftLeft, ShiftRight };

// Common instruction for add, sub, mul, div, rem, bit.and, bit.or, bit.xor, shl and shr.
class BinaryNumericInstruction final : public BinaryInstruction, public ILiftableInstruction {
public:
    static constexpr OpCode opcode = OpCode::BinaryNumericInstruction;

    BinaryNumericInstruction(BinaryNumericOperator op, ILInstruction* left, ILInstruction* right, bool check_for_overflow, Sign sign);
    BinaryNumericInstruction(BinaryNumericOperator op, ILInstruction* left, ILInstruction* right, StackType left_input_type,
                             StackType right_input_type, bool check_for_overflow, Sign sign, bool is_lifted = false);
    BinaryNumericInstruction(const BinaryNumericInstruction&) = default;

    // Whether the instruction checks for overflow.
    bool check_for_overflow;
    // For integer operations that depend on the sign, whether the operation is signed or unsigned; none for instructions that produce
    // the same result for either sign.
    Sign sign;
    StackType left_input_type;
    StackType right_input_type;
    // The operator used by this instruction.
    BinaryNumericOperator op;

    // Whether this is a lifted nullable operation: its arguments may be Nullable<T> values (T's stack type being the input type); if both
    // are non-null the operator is applied and the result wrapped in a Nullable of the underlying result type, else the result is null.
    bool is_lifted() const override { return is_lifted_; }
    StackType underlying_result_type() const override { return result_type_; }
    StackType result_type() const final { return is_lifted_ ? StackType::O : result_type_; }

    static StackType compute_result_type(BinaryNumericOperator op, StackType left, StackType right);
    static std::string get_operator_name(BinaryNumericOperator op);

    void check_invariant(ILPhase phase) override;
    InstructionFlags direct_flags() const override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    bool is_lifted_;
    StackType result_type_;
};

// Bitwise NOT.
class BitNot final : public UnaryInstruction, public ILiftableInstruction {
public:
    static constexpr OpCode opcode = OpCode::BitNot;

    explicit BitNot(ILInstruction* arg);
    BitNot(ILInstruction* arg, bool is_lifted, StackType stack_type);
    BitNot(const BitNot&) = default;

    bool is_lifted() const override { return is_lifted_; }
    StackType underlying_result_type() const override { return underlying_result_type_; }
    StackType result_type() const override { return argument()->result_type(); }

    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    bool is_lifted_ = false;
    StackType underlying_result_type_;
};

// Checks that the input float is not NaN or infinite.
class Ckfinite final : public UnaryInstruction {
public:
    static constexpr OpCode opcode = OpCode::Ckfinite;

    explicit Ckfinite(ILInstruction* argument) : UnaryInstruction(OpCode::Ckfinite, argument) {}
    Ckfinite(const Ckfinite&) = default;

    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
};

// Semantic meaning of a Conv instruction.
enum class ConversionKind : std::uint8_t {
    // Invalid conversion.
    Invalid,
    // Conversion between two types of same size. Can be used to change the sign of integer types, which may involve overflow-checking.
    Nop,
    // Integer-to-float conversion; the input sign decides whether the integer is treated as signed or unsigned.
    IntToFloat,
    // Float-to-integer conversion. Truncates toward zero; may perform overflow-checking.
    FloatToInt,
    // From the current precision on the evaluation stack to the target type's precision ("round-to-nearest" when it is reduced).
    FloatPrecisionChange,
    // Integer to a larger signed integer type. May involve overflow checking (U4 to I on 32-bit).
    SignExtend,
    // Integer to a larger unsigned integer type. May involve overflow checking (from a signed type).
    ZeroExtend,
    // To a smaller integer type; may involve overflow checking. Below the 4-byte stack width the result is zero or sign extended by
    // the target type's sign.
    Truncate,
    // Managed references and objects to unmanaged pointers.
    StopGCTracking,
    // Unmanaged pointers to managed references.
    StartGCTracking,
    // An object reference (O) to an interior pointer (Ref) to the start of the object (C++/CLI's PtrToStringChars).
    ObjectInterior,
};

// Numeric cast.
class Conv final : public UnaryInstruction, public ILiftableInstruction {
public:
    static constexpr OpCode opcode = OpCode::Conv;

    Conv(ILInstruction* argument, PrimitiveType target_type, bool check_for_overflow, Sign input_sign);
    Conv(ILInstruction* argument, StackType input_type, Sign input_sign, PrimitiveType target_type, bool check_for_overflow,
         bool is_lifted = false);
    Conv(const Conv&) = default;

    ConversionKind kind;
    // Whether the conversion performs overflow-checking.
    bool check_for_overflow;
    // The stack type of the input: the argument's result type, or its underlying type when lifted.
    StackType input_type;
    // The sign of the input type: set for conversions to integers iff overflow is checked, always for conversions to floats. It does
    // not decide zero or sign extension (the target type does).
    Sign input_sign;
    // The target type (the underlying one when lifted); None can happen for implicit conversions to O in invalid IL.
    PrimitiveType target_type;

    // Whether this is a lifted nullable conversion: the argument is a Nullable<T> (T's stack type being input_type); if non-null it
    // is extended to input_type, converted and wrapped in a Nullable of the target type, else the result is null.
    bool is_lifted() const override { return is_lifted_; }
    StackType result_type() const override { return is_lifted_ ? StackType::O : get_stack_type(target_type); }
    StackType underlying_result_type() const override { return get_stack_type(target_type); }

    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    // Does not unwrap lifted conversions.
    ILInstruction* unwrap_conv(ConversionKind kind) override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    // Ecma-335 Table 8: Conversion Operators.
    static ConversionKind get_conversion_kind(PrimitiveType target_type, StackType input_type, Sign input_sign);
    bool is_lifted_;
};

// ---- comparison -------------------------------------------------------------------------------------------------------------------

enum class ComparisonKind : std::uint8_t { Equality, Inequality, LessThan, LessThanOrEqual, GreaterThan, GreaterThanOrEqual };

bool is_equality_or_inequality(ComparisonKind kind);
ComparisonKind negate(ComparisonKind kind);
// The C# operator: ==, !=, <, <=, >, >=
std::string get_token(ComparisonKind kind);
// The C# binary operator that compares the same way (Equality -> BinaryOperatorType::Equality ...).
csharp::syntax::BinaryOperatorType to_binary_operator_type(ComparisonKind kind);

enum class ComparisonLiftingKind : std::uint8_t {
    // Not a lifted comparison.
    None,
    // C#-style lifted comparison: operands whose result type is not the input type return a Nullable<T> (T's stack type being the
    // input type); two null operands compare equal (1 for equality, else 0); one null operand: 1 for inequality, else 0; otherwise the
    // underlying comparison. The result type stays I4 as with normal comparisons.
    CSharp,
    // SQL-style lifted comparison, working like a lifted binary numeric instruction: any null operand gives null. Currently only used
    // for operator! on bool?.
    ThreeValuedLogic,
};

// Comparison. The inputs must be both integers; or both floats; or both object references (compared only for equality or inequality).
// Floating-point comparisons evaluate to 0 (false) when an input is NaN, except for 'NaN != NaN' which evaluates to 1 (true).
class Comp final : public BinaryInstruction, public ILiftableInstruction {
public:
    static constexpr OpCode opcode = OpCode::Comp;

    Comp(ComparisonKind kind, Sign sign, ILInstruction* left, ILInstruction* right);
    Comp(ComparisonKind kind, ComparisonLiftingKind lifting, StackType input_type, Sign sign, ILInstruction* left, ILInstruction* right);
    Comp(const Comp&) = default;

    ComparisonKind kind() const { return kind_; }
    void set_kind(ComparisonKind value) { kind_ = value; }
    ComparisonLiftingKind lifting_kind;
    // The stack type of the inputs (for lifted comparisons, the underlying input type).
    StackType input_type;
    // For an integer comparison, the sign used to interpret the integers.
    Sign sign;

    StackType result_type() const override { return lifting_kind == ComparisonLiftingKind::ThreeValuedLogic ? StackType::O : StackType::I4; }
    bool is_lifted() const override { return lifting_kind != ComparisonLiftingKind::None; }
    StackType underlying_result_type() const override { return StackType::I4; }

    static Comp* logic_not(ILInstruction* arg);
    static Comp* logic_not(ILInstruction* arg, bool is_lifted);

    void check_invariant(ILPhase phase) override;
    bool can_inline_into_slot(int child_index, ILInstruction* expression_being_moved) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ComparisonKind kind_;
};

// ---- calls ------------------------------------------------------------------------------------------------------------------------

// Instruction with a list of arguments.
class CallInstruction : public ILInstruction {
public:
    static const SlotInfo arguments_slot;

    static CallInstruction* create(OpCode op_code, const IMethod* method);

    const IMethod* method;
    // Whether the call has the 'tail.' prefix.
    bool is_tail = false;
    // The type of the 'constrained.' prefix (nullptr for none).
    const IType* constrained_to = nullptr;
    // Whether the IL stack was empty at the point of this call (not counting the call's own arguments and return value).
    bool il_stack_was_empty = false;

    InstructionCollection<ILInstruction>& arguments() const { return arguments_; }

    // Whether this is an instance call (the first argument is the 'this' pointer).
    bool is_instance_call() const;
    // The parameter for the argument at that index; nullptr for the `this` argument.
    const IParameter* get_parameter(int argument_index) const;
    StackType result_type() const override;
    // The stack type expected for the this pointer: Ref when constrained, for type parameters and for value types; O for reference
    // types; Unknown when the type is unknown.
    static StackType expected_type_for_this_pointer(const IType& declaring_type, const IType* constrained_to);

    int get_child_count() const final { return arguments_.count(); }
    ILInstruction* get_child(int index) const final { return arguments_[index]; }
    void set_child(int index, ILInstruction* value) final { arguments_.set(index, value); }
    const SlotInfo& get_child_slot(int) const final { return arguments_slot; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::SideEffect; }
    void check_invariant(ILPhase phase) override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) final;

protected:
    CallInstruction(OpCode op_code, const IMethod* method);
    // The copy has the same operands and an empty argument list (clone() fills it with clones).
    CallInstruction(const CallInstruction& other);
    void clone_arguments_into(CallInstruction& clone) const;
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    mutable InstructionCollection<ILInstruction> arguments_;
};

// Non-virtual method call.
class Call final : public CallInstruction, public ILiftableInstruction {
public:
    static constexpr OpCode opcode = OpCode::Call;

    explicit Call(const IMethod* method) : CallInstruction(OpCode::Call, method) {}
    Call(const Call&) = default;

    // Calls can only be lifted when calling a lifted operator (the semantics follow C#'s).
    bool is_lifted() const override;
    StackType underlying_result_type() const override;
    ILInstruction* clone() const override;
};

// Virtual method call.
class CallVirt final : public CallInstruction {
public:
    static constexpr OpCode opcode = OpCode::CallVirt;

    explicit CallVirt(const IMethod* method) : CallInstruction(OpCode::CallVirt, method) {}
    CallVirt(const CallVirt&) = default;

    ILInstruction* clone() const override;
};

// Creates an object instance and calls the constructor.
class NewObj final : public CallInstruction {
public:
    static constexpr OpCode opcode = OpCode::NewObj;

    explicit NewObj(const IMethod* method) : CallInstruction(OpCode::NewObj, method) {}
    NewObj(const NewObj&) = default;

    StackType result_type() const override;
    ILInstruction* clone() const override;
};

// Unsafe function pointer call. As in C#, the function pointer is evaluated first and the arguments later (in IL the pointer comes last).
class CallIndirect final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::CallIndirect;
    static const SlotInfo function_pointer_slot;
    static const SlotInfo argument_slot;

    CallIndirect(bool is_instance, bool has_explicit_this, const FunctionPointerType* function_pointer_type, ILInstruction* function_pointer,
                 const std::vector<ILInstruction*>& arguments);
    CallIndirect(const CallIndirect&) = delete;   // clone() builds a new one

    InstructionCollection<ILInstruction> arguments;
    bool is_instance() const { return is_instance_; }
    bool has_explicit_this() const { return has_explicit_this_; }
    const FunctionPointerType* function_pointer_type() const { return function_pointer_type_; }
    ILInstruction* function_pointer() const { return function_pointer_; }
    void set_function_pointer(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(function_pointer_, value, 0);
    }

    StackType result_type() const override;
    int get_child_count() const override { return arguments.count() + 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::SideEffect; }
    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    bool equal_signature(const CallIndirect& other) const;

    ILInstruction* function_pointer_ = nullptr;
    bool is_instance_;
    bool has_explicit_this_;
    const FunctionPointerType* function_pointer_type_;
};

// ---- locals -----------------------------------------------------------------------------------------------------------------------

// Loads the value of a local variable. (ldarg/ldloc)
class LdLoc final : public SimpleInstruction, public ILoadInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdLoc;

    explicit LdLoc(ILVariable* variable);
    LdLoc(const LdLoc&) = default;

    ILVariable* variable() const override { return variable_; }
    // Moves this load to another variable's load list while connected.
    void set_variable(ILVariable* value) override {
        assert(value != nullptr);
        if (is_connected()) variable_->remove_load_instruction(this);
        variable_ = value;
        if (is_connected()) variable_->add_load_instruction(this);
    }
    ILInstruction* as_instruction() override { return this; }

    StackType result_type() const override { return variable_->stack_type; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayReadLocals; }
    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return InstructionFlags::MayReadLocals; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
    void connected() override;
    void disconnected() override;

private:
    ILVariable* variable_;
};

// Loads the address of a local variable. (ldarga/ldloca)
class LdLoca final : public SimpleInstruction, public IAddressInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdLoca;

    explicit LdLoca(ILVariable* variable);
    LdLoca(const LdLoca&) = default;

    ILVariable* variable() const override { return variable_; }
    void set_variable(ILVariable* value) override {
        assert(value != nullptr);
        if (is_connected()) variable_->remove_address_instruction(this);
        variable_ = value;
        if (is_connected()) variable_->add_address_instruction(this);
    }
    ILInstruction* as_instruction() override { return this; }

    StackType result_type() const override { return StackType::Ref; }
    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
    void connected() override;
    void disconnected() override;

private:
    ILVariable* variable_;
};

// Stores a value into a local variable. (IL: starg/stloc) Evaluates to the value that was stored (for byte/short variables: the truncated
// value, sign/zero extended back to I4 by the variable type's sign).
class StLoc final : public ILInstruction, public IStoreInstruction {
public:
    static constexpr OpCode opcode = OpCode::StLoc;
    static const SlotInfo value_slot;

    StLoc(ILVariable* variable, ILInstruction* value);
    StLoc(const StLoc&) = default;

    // If true, this stloc represents a stack type adjustment. Only used by ILReader and BlockBuilder; transforms ignore it.
    bool is_stack_adjustment = false;
    // Whether the IL stack was empty after this store. Only set for stores from the IL, not for stores to the stack or generated by
    // transforms.
    bool il_stack_was_empty = false;

    ILVariable* variable() const override { return variable_; }
    void set_variable(ILVariable* value) override {
        assert(value != nullptr);
        if (is_connected()) variable_->remove_store_instruction(this);
        variable_ = value;
        if (is_connected()) variable_->add_store_instruction(this);
    }
    ILInstruction* as_instruction() override { return this; }

    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 0);
    }
    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    StackType result_type() const override { return variable_->stack_type; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayWriteLocals; }
    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return InstructionFlags::MayWriteLocals | value_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
    void connected() override;
    void disconnected() override;

private:
    ILVariable* variable_;
    ILInstruction* value_ = nullptr;
};

// Stores the value into an anonymous temporary variable, and returns the address of that variable.
class AddressOf final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::AddressOf;
    static const SlotInfo value_slot;

    AddressOf(ILInstruction* value, const IType* type);
    AddressOf(const AddressOf&) = default;

    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 0);
    }
    const IType* type() const { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }

    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    StackType result_type() const override { return StackType::Ref; }
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return value_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* value_ = nullptr;
    const IType* type_;
};

// ---- three-valued logic -----------------------------------------------------------------------------------------------------------

// Three valued logic and. Inputs are of type bool? or I4, output is of type bool?. Unlike logic.and(), does not short-circuit.
class ThreeValuedBoolAnd final : public BinaryInstruction, public ILiftableInstruction {
public:
    static constexpr OpCode opcode = OpCode::ThreeValuedBoolAnd;

    ThreeValuedBoolAnd(ILInstruction* left, ILInstruction* right) : BinaryInstruction(OpCode::ThreeValuedBoolAnd, left, right) {}
    ThreeValuedBoolAnd(const ThreeValuedBoolAnd&) = default;

    StackType result_type() const override { return StackType::O; }
    bool is_lifted() const override { return true; }
    StackType underlying_result_type() const override { return StackType::I4; }
    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
};

// Three valued logic or. Inputs are of type bool? or I4, output is of type bool?. Unlike logic.or(), does not short-circuit.
class ThreeValuedBoolOr final : public BinaryInstruction, public ILiftableInstruction {
public:
    static constexpr OpCode opcode = OpCode::ThreeValuedBoolOr;

    ThreeValuedBoolOr(ILInstruction* left, ILInstruction* right) : BinaryInstruction(OpCode::ThreeValuedBoolOr, left, right) {}
    ThreeValuedBoolOr(const ThreeValuedBoolOr&) = default;

    StackType result_type() const override { return StackType::O; }
    bool is_lifted() const override { return true; }
    StackType underlying_result_type() const override { return StackType::I4; }
    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
};

// ---- nullable ---------------------------------------------------------------------------------------------------------------------

// For a nullable input, gets the underlying value. The input is a reference type (non-null: the input), a nullable value type (the
// value), a generic type (depends on the type at runtime), or a managed reference to a type parameter. A null input transfers control to
// the innermost surrounding nullable.rewrap.
class NullableUnwrap final : public UnaryInstruction {
public:
    static constexpr OpCode opcode = OpCode::NullableUnwrap;

    NullableUnwrap(StackType unwrapped_type, ILInstruction* argument, bool ref_input = false);
    NullableUnwrap(const NullableUnwrap&) = default;

    // Whether the argument is dereferenced before checking for a null input (it is then a managed reference to a valid input type:
    // the C# compiler sometimes avoids copying the whole Nullable<T> before the null check).
    bool ref_input;
    // With a result type of Ref, the input reference is returned instead of the value in the non-null case (for t?.Method() on a
    // generic t, the method is called on the original reference). Only valid together with ref_input.
    bool ref_output() const { return result_type_ == StackType::Ref; }

    StackType result_type() const override { return result_type_; }
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayUnwrapNull; }
    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayUnwrapNull; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    StackType result_type_;
};

// Serves as jump target for the nullable.unwrap instruction. If the input evaluates normally, evaluates to it (wrapped in Nullable<T> if
// it is a non-nullable value type); if a nullable.unwrap encounters a null input it jumps to the end of this instruction, which then
// evaluates to null.
class NullableRewrap final : public UnaryInstruction {
public:
    static constexpr OpCode opcode = OpCode::NullableRewrap;

    explicit NullableRewrap(ILInstruction* argument) : UnaryInstruction(OpCode::NullableRewrap, argument) {}
    NullableRewrap(const NullableRewrap&) = default;

    StackType result_type() const override { return argument()->result_type() == StackType::Void ? StackType::Void : StackType::O; }
    InstructionFlags direct_flags() const override { return InstructionFlags::ControlFlow; }
    void check_invariant(ILPhase phase) override;
    bool prepare_extract(int child_index, transforms::ExtractionContext& ctx) override;
    bool can_inline_into_slot(int child_index, ILInstruction* expression_being_moved) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override;
};

// ---- constants --------------------------------------------------------------------------------------------------------------------

// Loads a constant string.
class LdStr final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdStr;

    explicit LdStr(std::string value) : SimpleInstruction(OpCode::LdStr), value(std::move(value)) {}
    LdStr(const LdStr&) = default;

    std::string value;
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Loads a constant byte string (as ReadOnlySpan<byte>).
class LdStrUtf8 final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdStrUtf8;

    explicit LdStrUtf8(std::string value) : SimpleInstruction(OpCode::LdStrUtf8), value(std::move(value)) {}
    LdStrUtf8(const LdStrUtf8&) = default;

    std::string value;
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Loads a constant 32-bit integer.
class LdcI4 final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdcI4;

    explicit LdcI4(std::int32_t value) : SimpleInstruction(OpCode::LdcI4), value(value) {}
    LdcI4(const LdcI4&) = default;

    std::int32_t value;
    StackType result_type() const override { return StackType::I4; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Loads a constant 64-bit integer.
class LdcI8 final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdcI8;

    explicit LdcI8(std::int64_t value) : SimpleInstruction(OpCode::LdcI8), value(value) {}
    LdcI8(const LdcI8&) = default;

    std::int64_t value;
    StackType result_type() const override { return StackType::I8; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Loads a constant 32-bit floating-point number.
class LdcF4 final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdcF4;

    explicit LdcF4(float value) : SimpleInstruction(OpCode::LdcF4), value(value) {}
    LdcF4(const LdcF4&) = default;

    float value;
    StackType result_type() const override { return StackType::F4; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Loads a constant 64-bit floating-point number.
class LdcF8 final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdcF8;

    explicit LdcF8(double value) : SimpleInstruction(OpCode::LdcF8), value(value) {}
    LdcF8(const LdcF8&) = default;

    double value;
    StackType result_type() const override { return StackType::F8; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Loads a constant decimal.
class LdcDecimal final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdcDecimal;

    explicit LdcDecimal(util::Decimal value) : SimpleInstruction(OpCode::LdcDecimal), value(value) {}
    LdcDecimal(const LdcDecimal&) = default;

    util::Decimal value;
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

// Loads the null reference.
class LdNull final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdNull;

    LdNull() : SimpleInstruction(OpCode::LdNull) {}
    LdNull(const LdNull&) = default;

    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
};

// ---- method pointers and tokens ---------------------------------------------------------------------------------------------------

// Load method pointer.
class LdFtn final : public SimpleInstruction, public IInstructionWithMethodOperand {
public:
    static constexpr OpCode opcode = OpCode::LdFtn;

    explicit LdFtn(const IMethod* method) : SimpleInstruction(OpCode::LdFtn), method_(method) {}
    LdFtn(const LdFtn&) = default;

    const IMethod* method() const override { return method_; }
    StackType result_type() const override { return StackType::I; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IMethod* method_;
};

// Load virtual method pointer.
class LdVirtFtn final : public UnaryInstruction, public IInstructionWithMethodOperand {
public:
    static constexpr OpCode opcode = OpCode::LdVirtFtn;

    LdVirtFtn(ILInstruction* argument, const IMethod* method) : UnaryInstruction(OpCode::LdVirtFtn, argument), method_(method) {}
    LdVirtFtn(const LdVirtFtn&) = default;

    const IMethod* method() const override { return method_; }
    StackType result_type() const override { return StackType::I; }
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IMethod* method_;
};

// Virtual delegate construction.
class LdVirtDelegate final : public UnaryInstruction, public IInstructionWithMethodOperand {
public:
    static constexpr OpCode opcode = OpCode::LdVirtDelegate;

    LdVirtDelegate(ILInstruction* argument, const IType* type, const IMethod* method)
        : UnaryInstruction(OpCode::LdVirtDelegate, argument), type_(type), method_(method) {}
    LdVirtDelegate(const LdVirtDelegate&) = default;

    const IType* type() const { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    const IMethod* method() const override { return method_; }
    StackType result_type() const override { return StackType::O; }
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
    const IMethod* method_;
};

// Loads runtime representation of a type's metadata token.
class LdTypeToken final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdTypeToken;

    explicit LdTypeToken(const IType* type) : SimpleInstruction(OpCode::LdTypeToken), type_(type) {}
    LdTypeToken(const LdTypeToken&) = default;

    const IType* type() const { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Loads runtime representation of a member's metadata token.
class LdMemberToken final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdMemberToken;

    explicit LdMemberToken(const IMember* member) : SimpleInstruction(OpCode::LdMemberToken), member_(member) {}
    LdMemberToken(const LdMemberToken&) = default;

    const IMember* member() const { return member_; }
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IMember* member_;
};

// ---- the rest ---------------------------------------------------------------------------------------------------------------------

// Maps a string value to an integer. This is used in switch(string).
class StringToInt final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::StringToInt;
    static const SlotInfo argument_slot;
    // A key (nullopt for null) and its integer.
    using MapEntry = std::pair<std::optional<std::string>, int>;

    StringToInt(ILInstruction* argument, std::vector<MapEntry> map, const IType* expected_type);
    // The map of an array: each string maps to its index.
    StringToInt(ILInstruction* argument, const std::vector<std::optional<std::string>>& map, const IType* expected_type);
    StringToInt(const StringToInt&) = default;

    ILInstruction* argument() const { return argument_; }
    void set_argument(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(argument_, value, 0);
    }
    std::vector<MapEntry>& map() { return map_; }
    const std::vector<MapEntry>& map() const { return map_; }
    const IType* expected_type() const { return expected_type_; }

    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    StackType result_type() const override { return StackType::I4; }
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }
    void check_invariant(ILPhase phase) override;
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return argument_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* argument_ = nullptr;
    std::vector<MapEntry> map_;
    const IType* expected_type_;
};

// ILAst representation of Expression.Convert.
class ExpressionTreeCast final : public UnaryInstruction {
public:
    static constexpr OpCode opcode = OpCode::ExpressionTreeCast;

    ExpressionTreeCast(const IType* type, ILInstruction* argument, bool is_checked)
        : UnaryInstruction(OpCode::ExpressionTreeCast, argument), type_(type), is_checked_(is_checked) {}
    ExpressionTreeCast(const ExpressionTreeCast&) = default;

    bool is_checked() const { return is_checked_; }
    void set_is_checked(bool value) { is_checked_ = value; }
    const IType* type() const { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override;
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    using ILInstruction::perform_match;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
    bool is_checked_;
};

}  // namespace cb::dotnet::decompiler::il
