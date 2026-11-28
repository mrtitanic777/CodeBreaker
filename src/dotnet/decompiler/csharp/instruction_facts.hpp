// dotnet/decompiler/csharp/instruction_facts.hpp -- what the C# builders read of an instruction, as one value per kind (stage 14 of
// docs/DOTNET_DECOMPILER_PLAN.md). A kind's facts are read from the instruction's L2 node (ExpressionBuilder::translate_from_l2, the
// node's own slots decoded as the lifting decodes them), or from the instruction itself where a body has no L2 (the lowering refused it);
// the builder's translation of the kind reads the facts only, so both give the same C#. Each mirrors the part of its instruction's
// interface the translation uses (the same names), with `self` the instruction the result is annotated with; an operand is its
// instruction (translated in turn, through its own node).
//
// Not upstream code: ILSpy's builders read the instruction's fields.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::csharp {

// stloc: the variable and the value stored.
struct StLocFacts {
    il::ILInstruction* self;
    il::ILVariable* variable_;
    il::ILInstruction* value_;

    static StLocFacts of(il::StLoc& inst) { return {&inst, inst.variable(), inst.value()}; }
    il::ILVariable* variable() const { return variable_; }
    il::ILInstruction* value() const { return value_; }
};

// conv: the operand, its stack type and sign, the target, the kind of conversion, the overflow check, the lifting.
struct ConvFacts {
    il::ILInstruction* self;
    il::ILInstruction* argument_;
    il::StackType input_type;
    typesystem::Sign input_sign;
    il::PrimitiveType target_type;
    il::ConversionKind kind;
    bool check_for_overflow;
    bool lifted;

    static ConvFacts of(il::Conv& inst) {
        return {&inst, inst.argument(), inst.input_type, inst.input_sign, inst.target_type, inst.kind, inst.check_for_overflow, inst.is_lifted()};
    }
    il::ILInstruction* argument() const { return argument_; }
    bool is_lifted() const { return lifted; }
    il::StackType underlying_result_type() const { return il::get_stack_type(target_type); }
};

// comp: the comparison, its lifting, the inputs' stack type and sign, the operands.
struct CompFacts {
    il::ILInstruction* self;
    il::ComparisonKind kind_;
    il::ComparisonLiftingKind lifting_kind;
    il::StackType input_type;
    typesystem::Sign sign;
    il::ILInstruction* left_;
    il::ILInstruction* right_;

    static CompFacts of(il::Comp& inst) { return {&inst, inst.kind(), inst.lifting_kind, inst.input_type, inst.sign, inst.left(), inst.right()}; }
    il::ComparisonKind kind() const { return kind_; }
    il::ILInstruction* left() const { return left_; }
    il::ILInstruction* right() const { return right_; }
    bool is_lifted() const { return lifting_kind != il::ComparisonLiftingKind::None; }
};

// binary.<op>: the operator, the operands and their stack types, the overflow check, the sign, the lifting.
struct BinaryNumericFacts {
    il::ILInstruction* self;
    il::BinaryNumericOperator op;
    il::ILInstruction* left_;
    il::ILInstruction* right_;
    il::StackType left_input_type;
    il::StackType right_input_type;
    bool check_for_overflow;
    typesystem::Sign sign;
    bool lifted;
    il::StackType underlying_result_type_;   // (an L2 node's: what the instruction computes from the operator and the input types)

    static BinaryNumericFacts of(il::BinaryNumericInstruction& inst) {
        return {&inst, inst.op, inst.left(), inst.right(), inst.left_input_type, inst.right_input_type, inst.check_for_overflow, inst.sign, inst.is_lifted(),
                inst.underlying_result_type()};
    }
    il::ILInstruction* left() const { return left_; }
    il::ILInstruction* right() const { return right_; }
    bool is_lifted() const { return lifted; }
    il::StackType underlying_result_type() const { return underlying_result_type_; }
};

// box, unbox, unbox.any, castclass, isinst: the operand and the type.
struct TypedUnaryFacts {
    il::ILInstruction* self;
    il::ILInstruction* argument_;
    const typesystem::IType* type_;

    template <class Inst> static TypedUnaryFacts of(Inst& inst) { return {&inst, inst.argument(), inst.type()}; }
    il::ILInstruction* argument() const { return argument_; }
    const typesystem::IType* type() const { return type_; }
};

// ldobj: the address, the type loaded, the unaligned. prefix.
struct LdObjFacts {
    il::ILInstruction* self;
    il::ILInstruction* target_;
    const typesystem::IType* type_;
    std::uint8_t unaligned;

    static LdObjFacts of(il::LdObj& inst) { return {&inst, inst.target(), inst.type(), inst.unaligned_prefix()}; }
    il::ILInstruction* target() const { return target_; }
    const typesystem::IType* type() const { return type_; }
    std::uint8_t unaligned_prefix() const { return unaligned; }
};

// stobj: the address, the value stored, the type, the unaligned. prefix.
struct StObjFacts {
    il::ILInstruction* self;
    il::ILInstruction* target_;
    il::ILInstruction* value_;
    const typesystem::IType* type_;
    std::uint8_t unaligned;

    static StObjFacts of(il::StObj& inst) { return {&inst, inst.target(), inst.value(), inst.type(), inst.unaligned_prefix()}; }
    il::ILInstruction* target() const { return target_; }
    il::ILInstruction* value() const { return value_; }
    const typesystem::IType* type() const { return type_; }
    std::uint8_t unaligned_prefix() const { return unaligned; }
};

// ldflda: the object (its address), the field, the kind of pointer made; where it stands is the instruction's (parent, slot).
struct LdFldaFacts {
    il::ILInstruction* self;
    il::ILInstruction* target_;
    const typesystem::IField* field_;
    il::StackType result_type_;

    static LdFldaFacts of(il::LdFlda& inst) { return {&inst, inst.target(), inst.field(), inst.result_type()}; }
    il::ILInstruction* target() const { return target_; }
    const typesystem::IField* field() const { return field_; }
    il::StackType result_type() const { return result_type_; }
    il::ILInstruction* parent() const { return self->parent(); }
    const il::SlotInfo* slot_info() const { return self->slot_info(); }
};

// ldsflda: the field.
struct LdsFldaFacts {
    il::ILInstruction* self;
    const typesystem::IField* field_;

    static LdsFldaFacts of(il::LdsFlda& inst) { return {&inst, inst.field()}; }
    const typesystem::IField* field() const { return field_; }
};

// ldelema: the array, the indices, the element type, whether the indices are System.Index.
struct LdElemaFacts {
    il::ILInstruction* self;
    il::ILInstruction* array_;
    std::vector<il::ILInstruction*> indices_;
    const typesystem::IType* type_;
    bool with_system_index;

    static LdElemaFacts of(il::LdElema& inst) { return {&inst, inst.array(), inst.indices().items(), inst.type(), inst.with_system_index}; }
    il::ILInstruction* array() const { return array_; }
    const std::vector<il::ILInstruction*>& indices() const { return indices_; }
    const typesystem::IType* type() const { return type_; }
};

// newarr: the element type and the dimensions' lengths.
struct NewArrFacts {
    il::ILInstruction* self;
    const typesystem::IType* type_;
    std::vector<il::ILInstruction*> indices_;

    static NewArrFacts of(il::NewArr& inst) { return {&inst, inst.type(), inst.indices().items()}; }
    const typesystem::IType* type() const { return type_; }
    const std::vector<il::ILInstruction*>& indices() const { return indices_; }
};

// addressof: the value and its type; where it stands is the instruction's.
struct AddressOfFacts {
    il::ILInstruction* self;
    il::ILInstruction* value_;
    const typesystem::IType* type_;

    static AddressOfFacts of(il::AddressOf& inst) { return {&inst, inst.value(), inst.type()}; }
    il::ILInstruction* value() const { return value_; }
    const typesystem::IType* type() const { return type_; }
};

// call, callvirt, newobj: the opcode, the method, the arguments (the `this` argument first, but for newobj), the constrained. prefix,
// the tail. prefix. The patterns a call is matched against (a delegate construction, a tuple, a string concatenation) look at the
// instruction's operands.
struct CallFacts {
    il::CallInstruction* self;
    il::OpCode op_code;
    const typesystem::IMethod* method;
    std::vector<il::ILInstruction*> arguments;
    const typesystem::IType* constrained_to;
    bool is_tail;

    static CallFacts of(il::CallInstruction& inst) {
        return {&inst, inst.op_code, inst.method, inst.arguments().items(), inst.constrained_to, inst.is_tail};
    }
};

// a ?? b: the value, the fallback, the kind of coalescing, the underlying result type.
struct NullCoalescingFacts {
    il::ILInstruction* self;
    il::ILInstruction* value_inst_;
    il::ILInstruction* fallback_inst_;
    il::NullCoalescingKind kind;
    il::StackType underlying_result_type;

    static NullCoalescingFacts of(il::NullCoalescingInstruction& inst) {
        return {&inst, inst.value_inst(), inst.fallback_inst(), inst.kind, inst.underlying_result_type};
    }
    il::ILInstruction* value_inst() const { return value_inst_; }
    il::ILInstruction* fallback_inst() const { return fallback_inst_; }
};

// ldftn, ldvirtftn: the method (ldvirtftn's object is not translated: C# has no instance function pointer).
struct MethodPointerFacts {
    il::ILInstruction* self;
    const typesystem::IMethod* method_;

    template <class Inst> static MethodPointerFacts of(Inst& inst) { return {&inst, inst.method()}; }
    const typesystem::IMethod* method() const { return method_; }
};

// await: the value awaited, the pattern's GetAwaiter and GetResult.
struct AwaitFacts {
    il::ILInstruction* self;
    il::ILInstruction* value_;
    const typesystem::IMethod* get_awaiter_method;
    const typesystem::IMethod* get_result_method;

    static AwaitFacts of(il::Await& inst) { return {&inst, inst.value(), inst.get_awaiter_method, inst.get_result_method}; }
    il::ILInstruction* value() const { return value_; }
};

// nullable.unwrap: the operand, whether it is an address, whether the result is.
struct NullableUnwrapFacts {
    il::ILInstruction* self;
    il::ILInstruction* argument_;
    bool ref_input;
    bool ref_output_;

    static NullableUnwrapFacts of(il::NullableUnwrap& inst) { return {&inst, inst.argument(), inst.ref_input, inst.ref_output()}; }
    il::ILInstruction* argument() const { return argument_; }
    bool ref_output() const { return ref_output_; }
};

// nullable.rewrap, throw (as an expression): the operand.
struct OperandFacts {
    il::ILInstruction* self;
    il::ILInstruction* argument_;

    template <class Inst> static OperandFacts of(Inst& inst) { return {&inst, inst.argument()}; }
    il::ILInstruction* argument() const { return argument_; }
};

// ldtypetoken: the type.
struct TypeTokenFacts {
    il::ILInstruction* self;
    const typesystem::IType* type_;

    static TypeTokenFacts of(il::LdTypeToken& inst) { return {&inst, inst.type()}; }
    const typesystem::IType* type() const { return type_; }
};

// if as an expression: the condition and the branches; the patterns it is matched against (&&, ||) and its slot are the instruction's.
struct IfFacts {
    il::IfInstruction* self;
    il::ILInstruction* condition_;
    il::ILInstruction* true_inst_;
    il::ILInstruction* false_inst_;
    il::StackType result_type_;

    static IfFacts of(il::IfInstruction& inst) { return {&inst, inst.condition(), inst.true_inst(), inst.false_inst(), inst.result_type()}; }
    il::ILInstruction* condition() const { return condition_; }
    il::ILInstruction* true_inst() const { return true_inst_; }
    il::ILInstruction* false_inst() const { return false_inst_; }
    il::StackType result_type() const { return result_type_; }
    bool match_logic_and(il::ILInstruction** lhs, il::ILInstruction** rhs) const { return self->match_logic_and(lhs, rhs); }
    bool match_logic_or(il::ILInstruction** lhs, il::ILInstruction** rhs) const { return self->match_logic_or(lhs, rhs); }
};

// a numeric compound assignment (x op= v, x++): the operator and its operands' stack types, sign, overflow check and lifting as a
// binary instruction's, the target and how it is reached, the value, the type, what it evaluates to.
struct NumericCompoundAssignFacts {
    il::ILInstruction* self;
    il::BinaryNumericOperator op;
    typesystem::Sign sign;
    bool check_for_overflow;
    il::StackType left_input_type;
    il::StackType right_input_type;
    bool lifted;
    il::StackType underlying_result_type_;
    const typesystem::IType* type_;
    il::CompoundEvalMode eval_mode;
    il::CompoundTargetKind target_kind;
    il::ILInstruction* target_;
    il::ILInstruction* value_;

    static NumericCompoundAssignFacts of(il::NumericCompoundAssign& inst) {
        return {&inst, inst.op, inst.sign, inst.check_for_overflow, inst.left_input_type, inst.right_input_type, inst.is_lifted(),
                inst.underlying_result_type(), inst.type(), inst.eval_mode, inst.target_kind, inst.target(), inst.value()};
    }
    bool is_lifted() const { return lifted; }
    il::StackType underlying_result_type() const { return underlying_result_type_; }
    const typesystem::IType* type() const { return type_; }
    il::ILInstruction* target() const { return target_; }
    il::ILInstruction* value() const { return value_; }
};

// a user-defined compound assignment: the operator method, the target and how it is reached, the value, what it evaluates to.
struct UserDefinedCompoundAssignFacts {
    il::ILInstruction* self;
    const typesystem::IMethod* method;
    il::CompoundEvalMode eval_mode;
    il::CompoundTargetKind target_kind;
    il::ILInstruction* target_;
    il::ILInstruction* value_;

    static UserDefinedCompoundAssignFacts of(il::UserDefinedCompoundAssign& inst) {
        return {&inst, inst.method, inst.eval_mode, inst.target_kind, inst.target(), inst.value()};
    }
    bool is_lifted() const { return false; }
    il::ILInstruction* target() const { return target_; }
    il::ILInstruction* value() const { return value_; }
};

// calli: the function pointer, its type, whether it takes `this`, the arguments.
struct CallIndirectFacts {
    il::ILInstruction* self;
    il::ILInstruction* function_pointer_;
    const typesystem::FunctionPointerType* function_pointer_type_;
    bool is_instance_;
    std::vector<il::ILInstruction*> arguments;

    static CallIndirectFacts of(il::CallIndirect& inst) {
        return {&inst, inst.function_pointer(), inst.function_pointer_type(), inst.is_instance(), inst.arguments.items()};
    }
    il::ILInstruction* function_pointer() const { return function_pointer_; }
    const typesystem::FunctionPointerType* function_pointer_type() const { return function_pointer_type_; }
    bool is_instance() const { return is_instance_; }
};

// match (an `is` pattern): the value tested and the pattern's variable; the pattern itself (its sub-patterns) is translated from the
// instruction.
struct MatchFacts {
    il::MatchInstruction* self;
    il::ILInstruction* tested_operand_;
    il::ILVariable* variable_;

    static MatchFacts of(il::MatchInstruction& inst) { return {&inst, inst.tested_operand(), inst.variable()}; }
    il::ILInstruction* tested_operand() const { return tested_operand_; }
    il::ILVariable* variable() const { return variable_; }
};

// switch (a statement or an expression): the value switched on, its type and lifting, each section (its labels, its null label, its
// body, whether it is the default the compiler made), the result type of a switch expression.
struct SwitchSectionFacts {
    il::SwitchSection* self;
    util::LongSet labels_;
    bool has_null_label_;
    bool compiler_generated_default;
    il::ILInstruction* body_;

    const util::LongSet& labels() const { return labels_; }
    bool has_null_label() const { return has_null_label_; }
    bool is_compiler_generated_default_section() const { return compiler_generated_default; }
    il::ILInstruction* body() const { return body_; }
};
struct SwitchFacts {
    il::SwitchInstruction* self;
    il::ILInstruction* value_;
    const typesystem::IType* type;
    bool is_lifted;
    il::StackType result_type_;
    std::vector<SwitchSectionFacts> sections_;

    static SwitchFacts of(il::SwitchInstruction& inst) {
        SwitchFacts facts{&inst, inst.value(), inst.type, inst.is_lifted, inst.result_type(), {}};
        facts.sections_.reserve(static_cast<std::size_t>(inst.sections.count()));
        for (il::SwitchSection* s : inst.sections)
            facts.sections_.push_back({s, s->labels(), s->has_null_label(), s->is_compiler_generated_default_section(), s->body()});
        return facts;
    }
    il::ILInstruction* value() const { return value_; }
    il::StackType result_type() const { return result_type_; }
    // the sections in order, as the instruction's list is read
    std::vector<const SwitchSectionFacts*> sections() const {
        std::vector<const SwitchSectionFacts*> out;
        out.reserve(sections_.size());
        for (const SwitchSectionFacts& s : sections_) out.push_back(&s);
        return out;
    }
    // the section with the most labels (the first of them), as SwitchInstruction::get_default_section
    const SwitchSectionFacts* get_default_section() const {
        const SwitchSectionFacts* best = sections_.empty() ? nullptr : &sections_.front();
        for (const SwitchSectionFacts& s : sections_)
            if (s.labels().count() > best->labels().count()) best = &s;
        return best;
    }
};

// the dynamic operations (C#'s `dynamic`): the binder flags, the operation, the calling context, the member's name, the type converted
// to or constructed, the type arguments, the operands and their argument infos (an operation with two operands: left and right, target
// and value; one: the operand, the argument, the target), the compound assignment's target kind.
struct DynamicFacts {
    il::ILInstruction* self;
    il::CSharpBinderFlags binder;
    il::ExpressionType operation_ = il::ExpressionType::Extension;
    const typesystem::IType* context = nullptr;
    std::string name_;
    const typesystem::IType* type_ = nullptr;
    std::vector<const typesystem::IType*> type_arguments_;
    std::vector<il::ILInstruction*> arguments_;
    std::vector<il::CSharpArgumentInfo> argument_info_;
    il::CompoundTargetKind target_kind = il::CompoundTargetKind::Dynamic;

    static DynamicFacts of(il::DynamicBinaryOperatorInstruction& i) {
        return {&i, i.binder_flags(), i.operation(), i.calling_context(), {}, nullptr, {}, {i.left(), i.right()}, {i.left_argument_info(), i.right_argument_info()}};
    }
    static DynamicFacts of(il::DynamicLogicOperatorInstruction& i) {
        return {&i, i.binder_flags(), i.operation(), i.calling_context(), {}, nullptr, {}, {i.left(), i.right()}, {i.left_argument_info(), i.right_argument_info()}};
    }
    static DynamicFacts of(il::DynamicUnaryOperatorInstruction& i) {
        return {&i, i.binder_flags(), i.operation(), i.calling_context(), {}, nullptr, {}, {i.operand()}, {i.operand_argument_info()}};
    }
    static DynamicFacts of(il::DynamicConvertInstruction& i) {
        return {&i, i.binder_flags(), il::ExpressionType::Extension, i.calling_context(), {}, i.type(), {}, {i.argument()}, {}};
    }
    static DynamicFacts of(il::DynamicGetMemberInstruction& i) {
        return {&i, i.binder_flags(), il::ExpressionType::Extension, i.calling_context(), i.name(), nullptr, {}, {i.target()}, {i.target_argument_info()}};
    }
    static DynamicFacts of(il::DynamicSetMemberInstruction& i) {
        return {&i, i.binder_flags(), il::ExpressionType::Extension, i.calling_context(), i.name(), nullptr, {}, {i.target(), i.value()},
                {i.target_argument_info(), i.value_argument_info()}};
    }
    static DynamicFacts of(il::DynamicGetIndexInstruction& i) { return arguments_of(i, {}, nullptr, {}); }
    static DynamicFacts of(il::DynamicSetIndexInstruction& i) { return arguments_of(i, {}, nullptr, {}); }
    static DynamicFacts of(il::DynamicInvokeInstruction& i) { return arguments_of(i, {}, nullptr, {}); }
    static DynamicFacts of(il::DynamicInvokeMemberInstruction& i) { return arguments_of(i, i.name(), nullptr, i.type_arguments()); }
    static DynamicFacts of(il::DynamicInvokeConstructorInstruction& i) { return arguments_of(i, {}, i.constructed_type(), {}); }
    static DynamicFacts of(il::DynamicCompoundAssign& i) {
        return {&i, i.binder_flags(), i.operation(), nullptr, {}, nullptr, {}, {i.target(), i.value()}, {i.target_argument_info(), i.value_argument_info()},
                i.target_kind};
    }

    il::CSharpBinderFlags binder_flags() const { return binder; }
    il::ExpressionType operation() const { return operation_; }
    const std::string& name() const { return name_; }
    const typesystem::IType* type() const { return type_; }
    const std::vector<const typesystem::IType*>& type_arguments() const { return type_arguments_; }
    const std::vector<il::ILInstruction*>& arguments() const { return arguments_; }
    const std::vector<il::CSharpArgumentInfo>& argument_info() const { return argument_info_; }
    il::ILInstruction* left() const { return arguments_[0]; }
    il::ILInstruction* right() const { return arguments_[1]; }
    il::ILInstruction* target() const { return arguments_[0]; }
    il::ILInstruction* value() const { return arguments_[1]; }
    il::ILInstruction* operand() const { return arguments_[0]; }
    il::ILInstruction* argument() const { return arguments_[0]; }
    const il::CSharpArgumentInfo& left_argument_info() const { return argument_info_[0]; }
    const il::CSharpArgumentInfo& right_argument_info() const { return argument_info_[1]; }
    const il::CSharpArgumentInfo& target_argument_info() const { return argument_info_[0]; }
    const il::CSharpArgumentInfo& value_argument_info() const { return argument_info_[1]; }
    const il::CSharpArgumentInfo& operand_argument_info() const { return argument_info_[0]; }
    bool is_checked() const { return (binder & il::CSharpBinderFlags::CheckedContext) != il::CSharpBinderFlags::None; }
    bool is_explicit() const { return (binder & il::CSharpBinderFlags::ConvertExplicit) != il::CSharpBinderFlags::None; }
    const il::SlotInfo* slot_info() const { return self->slot_info(); }

private:
    static DynamicFacts arguments_of(il::DynamicArgumentsInstruction& i, std::string name, const typesystem::IType* type,
                                     std::vector<const typesystem::IType*> type_arguments) {
        return {&i, i.binder_flags(), il::ExpressionType::Extension, i.calling_context(), std::move(name), type, std::move(type_arguments),
                i.arguments().items(), i.argument_info()};
    }
};

// ldelema.inlinearray: the inline array (its address), the indices, the inline array type.
struct LdElemaInlineArrayFacts {
    il::ILInstruction* self;
    il::ILInstruction* array_;
    std::vector<il::ILInstruction*> indices_;
    const typesystem::IType* type_;

    static LdElemaInlineArrayFacts of(il::LdElemaInlineArray& inst) { return {&inst, inst.array(), inst.indices().items(), inst.type()}; }
    il::ILInstruction* array() const { return array_; }
    const std::vector<il::ILInstruction*>& indices() const { return indices_; }
    const typesystem::IType* type() const { return type_; }
};

// expression.tree.cast: the operand, the type, whether it is a checked conversion.
struct ExpressionTreeCastFacts {
    il::ILInstruction* self;
    il::ILInstruction* argument_;
    const typesystem::IType* type_;
    bool checked;

    static ExpressionTreeCastFacts of(il::ExpressionTreeCast& inst) { return {&inst, inst.argument(), inst.type(), inst.is_checked()}; }
    il::ILInstruction* argument() const { return argument_; }
    const typesystem::IType* type() const { return type_; }
    bool is_checked() const { return checked; }
};

// a user-defined && or ||: the operator method (op_BitwiseAnd or op_BitwiseOr), the operands; and the three-valued && and || (no method).
struct LogicOperatorFacts {
    il::ILInstruction* self;
    const typesystem::IMethod* method_;
    il::ILInstruction* left_;
    il::ILInstruction* right_;

    static LogicOperatorFacts of(il::UserDefinedLogicOperator& inst) { return {&inst, inst.method(), inst.left(), inst.right()}; }
    static LogicOperatorFacts of(il::BinaryInstruction& inst) { return {&inst, nullptr, inst.left(), inst.right()}; }
    const typesystem::IMethod* method() const { return method_; }
    il::ILInstruction* left() const { return left_; }
    il::ILInstruction* right() const { return right_; }
};

// localloc, localloc.span: the size (the count, for a span), the span type.
struct LocAllocFacts {
    il::ILInstruction* self;
    il::ILInstruction* argument_;
    const typesystem::IType* type_;

    static LocAllocFacts of(il::LocAlloc& inst) { return {&inst, inst.argument(), nullptr}; }
    static LocAllocFacts of(il::LocAllocSpan& inst) { return {&inst, inst.argument(), inst.type()}; }
    il::ILInstruction* argument() const { return argument_; }
    const typesystem::IType* type() const { return type_; }
};

// ldvirtdelegate: the object, the delegate type, the method.
struct LdVirtDelegateFacts {
    il::LdVirtDelegate* self;
    il::ILInstruction* argument_;
    const typesystem::IType* type_;
    const typesystem::IMethod* method_;

    static LdVirtDelegateFacts of(il::LdVirtDelegate& inst) { return {&inst, inst.argument(), inst.type(), inst.method()}; }
    il::ILInstruction* argument() const { return argument_; }
    const typesystem::IType* type() const { return type_; }
    const typesystem::IMethod* method() const { return method_; }
};

// deconstruct: the instruction (its init stores, pattern, conversions and assignments are read from it as it rewrites them) and the
// value deconstructed with its type.
struct DeconstructFacts {
    il::DeconstructInstruction* self;
    il::ILInstruction* tested_operand;
    const typesystem::IType* type;

    static DeconstructFacts of(il::DeconstructInstruction& inst) {
        return {&inst, inst.pattern()->tested_operand(), inst.pattern()->variable()->type()};
    }
};

// ---- statements --------------------------------------------------------------------------------------------------------------------

// initblk, cpblk: the addresses, the value, the size, the unaligned. prefix; ckfinite: the operand.
struct MemoryBlockFacts {
    il::ILInstruction* self;
    il::ILInstruction* dest_address_;    // initblk's address
    il::ILInstruction* source_;          // initblk's value, cpblk's source address
    il::ILInstruction* size_;
    std::uint8_t unaligned;

    static MemoryBlockFacts of(il::Initblk& inst) { return {&inst, inst.address(), inst.value(), inst.size(), inst.unaligned_prefix()}; }
    static MemoryBlockFacts of(il::Cpblk& inst) { return {&inst, inst.dest_address(), inst.source_address(), inst.size(), inst.unaligned_prefix()}; }
    static MemoryBlockFacts of(il::Ckfinite& inst) { return {&inst, inst.argument(), nullptr, nullptr, 0}; }
    il::ILInstruction* address() const { return dest_address_; }
    il::ILInstruction* dest_address() const { return dest_address_; }
    il::ILInstruction* argument() const { return dest_address_; }
    il::ILInstruction* value() const { return source_; }
    il::ILInstruction* source_address() const { return source_; }
    il::ILInstruction* size() const { return size_; }
    std::uint8_t unaligned_prefix() const { return unaligned; }
};

// fixed: the pinned variable, its initial value, the body.
struct PinnedRegionFacts {
    il::ILInstruction* self;
    il::ILVariable* variable_;
    il::ILInstruction* init_;
    il::ILInstruction* body_;

    static PinnedRegionFacts of(il::PinnedRegion& inst) { return {&inst, inst.variable(), inst.init(), inst.body()}; }
    il::ILVariable* variable() const { return variable_; }
    il::ILInstruction* init() const { return init_; }
    il::ILInstruction* body() const { return body_; }
};

// using: the resource variable and expression, the body, whether it is `await using`, whether the resource is a ref struct (disposed by
// pattern). The foreach pattern a using is matched against looks at the instruction.
struct UsingFacts {
    il::UsingInstruction* self;
    il::ILVariable* variable_;
    il::ILInstruction* resource_expression_;
    il::ILInstruction* body_;
    bool async;
    bool ref_struct;

    static UsingFacts of(il::UsingInstruction& inst) {
        return {&inst, inst.variable(), inst.resource_expression(), inst.body(), inst.is_async(), inst.is_ref_struct()};
    }
    il::ILVariable* variable() const { return variable_; }
    il::ILInstruction* resource_expression() const { return resource_expression_; }
    il::ILInstruction* body() const { return body_; }
    bool is_async() const { return async; }
    bool is_ref_struct() const { return ref_struct; }
};

// leave: the container left, the value (null: none, as a nop is).
struct LeaveFacts {
    il::ILInstruction* self;
    il::BlockContainer* target_container_;
    il::ILInstruction* value_;

    static LeaveFacts of(il::Leave& inst) { return {&inst, inst.target_container(), inst.value()}; }
    il::BlockContainer* target_container() const { return target_container_; }
    il::ILInstruction* value() const { return value_; }
    bool has_value() const { return value_ != nullptr && !value_->match_nop(); }
    // the container's entry label (IL_xxxx), as Leave::target_label
    std::string target_label() const {
        return target_container_ != nullptr && target_container_->entry_point() != nullptr ? target_container_->entry_point()->label() : std::string();
    }
};

// br: the block jumped to.
struct BranchFacts {
    il::ILInstruction* self;
    il::Block* target_block_;

    static BranchFacts of(il::Branch& inst) { return {&inst, inst.target_block()}; }
    il::Block* target_block() const { return target_block_; }
};

// yield return: the value.
struct YieldReturnFacts {
    il::ILInstruction* self;
    il::ILInstruction* value_;

    static YieldReturnFacts of(il::YieldReturn& inst) { return {&inst, inst.value()}; }
    il::ILInstruction* value() const { return value_; }
};

// lock: the object locked, the body.
struct LockFacts {
    il::ILInstruction* self;
    il::ILInstruction* on_expression_;
    il::ILInstruction* body_;

    static LockFacts of(il::LockInstruction& inst) { return {&inst, inst.on_expression(), inst.body()}; }
    il::ILInstruction* on_expression() const { return on_expression_; }
    il::ILInstruction* body() const { return body_; }
};

// try-finally and try-fault: the body and the handler (the finally or the fault block).
struct TryHandlerFacts {
    il::ILInstruction* self;
    il::ILInstruction* try_block_;
    il::ILInstruction* handler_block_;

    static TryHandlerFacts of(il::TryFinally& inst) { return {&inst, inst.try_block(), inst.finally_block()}; }
    static TryHandlerFacts of(il::TryFault& inst) { return {&inst, inst.try_block(), inst.fault_block()}; }
    il::ILInstruction* try_block() const { return try_block_; }
    il::ILInstruction* finally_block() const { return handler_block_; }
    il::ILInstruction* fault_block() const { return handler_block_; }
};

// try-catch: the body and each handler (the handler instruction, which the catch clause is annotated with, its variable, filter, body).
struct TryCatchFacts {
    struct Handler {
        il::TryCatchHandler* self;
        il::ILVariable* variable_;
        il::ILInstruction* filter_;
        il::ILInstruction* body_;

        il::ILVariable* variable() const { return variable_; }
        il::ILInstruction* filter() const { return filter_; }
        il::ILInstruction* body() const { return body_; }
    };
    il::ILInstruction* self;
    il::ILInstruction* try_block_;
    std::vector<Handler> handlers;

    static TryCatchFacts of(il::TryCatch& inst) {
        TryCatchFacts facts{&inst, inst.try_block(), {}};
        for (il::TryCatchHandler* h : inst.handlers) facts.handlers.push_back({h, h->variable(), h->filter(), h->body()});
        return facts;
    }
    il::ILInstruction* try_block() const { return try_block_; }
};

// a container as a statement: whether it is a loop (not a plain container, its entry jumped to again), the switch its entry holds alone.
struct BlockContainerFacts {
    il::BlockContainer* self;
    bool is_loop;
    il::SwitchInstruction* switch_inst;
    std::optional<SwitchFacts> switch_facts;   // (the switch's facts, read from its node)
};

// a control-flow block as a statement: its final instruction where it is not a nop (its instructions are the block's, read as the
// statement builder's rewrites change them).
struct BlockFacts {
    il::Block* self;
    il::ILInstruction* final_instruction;
};

}  // namespace cb::dotnet::decompiler::csharp
