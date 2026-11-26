// dotnet/decompiler/il/memory_instructions.hpp -- the ILAst's instructions that touch memory and types: stack allocation, block copy
// and fill, field addresses, casts, indirect loads and stores, boxing, arrays, default values, throw, sizeof, pinning and typed
// references.
//
//     auto* load = make<LdObj>(make<LdLoca>(v), int32_type);          // ldobj System.Int32(ldloca v)
//     auto* store = make<StObj>(address, value, type);               // stobj T(address, value)
//     auto* array = make<NewArr>(element_type, std::vector<ILInstruction*>{make<LdcI4>(10)});
//     if (inst->match_ld_flda(&target, &field)) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: Arglist, LocAlloc, LocAllocSpan, Cpblk, Initblk, LdFlda, LdsFlda, CastClass, IsInst,
// LdObj, LdObjIfRef, StObj, Box, Unbox, UnboxAny, NewArr, DefaultValue, Throw, Rethrow, SizeOf, LdLen, LdElema, LdElemaInlineArray,
// GetPinnableReference, MakeRefAny, RefAnyType, RefAnyValue, from Instructions.cs and Instructions/LdFlda.cs, IsInst.cs, LdLen.cs,
// DefaultValue.cs, MemoryInstructions.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
// The Match* methods of these classes are ILInstruction members (declared in il_instruction.hpp) written in memory_instructions.cpp.
#pragma once

#include <cstdint>
#include <vector>

#include "dotnet/decompiler/il/instructions_base.hpp"

namespace cb::dotnet::decompiler::il {

// Retrieves the RuntimeArgumentHandle.
class Arglist final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::Arglist;
    Arglist() : SimpleInstruction(opcode) {}
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
};

// Allocates space in the stack frame.
class LocAlloc final : public UnaryInstruction {
public:
    static constexpr OpCode opcode = OpCode::LocAlloc;
    explicit LocAlloc(ILInstruction* argument) : UnaryInstruction(opcode, argument) {}
    StackType result_type() const override { return StackType::I; }
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
};

// Allocates space in the stack frame and wraps it in a Span.
class LocAllocSpan final : public UnaryInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::LocAllocSpan;
    LocAllocSpan(ILInstruction* argument, const IType* type) : UnaryInstruction(opcode, argument), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::O; }
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// memcpy(destAddress, sourceAddress, size);
class Cpblk final : public ILInstruction, public ISupportsVolatilePrefix, public ISupportsUnalignedPrefix {
public:
    static constexpr OpCode opcode = OpCode::Cpblk;
    static const SlotInfo dest_address_slot;
    static const SlotInfo source_address_slot;
    static const SlotInfo size_slot;
    Cpblk(ILInstruction* dest_address, ILInstruction* source_address, ILInstruction* size);
    ILInstruction* dest_address() const { return dest_address_; }
    void set_dest_address(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(dest_address_, value, 0);
    }
    ILInstruction* source_address() const { return source_address_; }
    void set_source_address(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(source_address_, value, 1);
    }
    ILInstruction* size() const { return size_; }
    void set_size(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(size_, value, 2);
    }
    int get_child_count() const override { return 3; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    bool is_volatile() const override { return is_volatile_; }
    void set_is_volatile(bool value) override { is_volatile_ = value; }
    std::uint8_t unaligned_prefix() const override { return unaligned_prefix_; }
    void set_unaligned_prefix(std::uint8_t value) override { unaligned_prefix_ = value; }
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::SideEffect; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override {
        return dest_address_->flags() | source_address_->flags() | size_->flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect;
    }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* dest_address_ = nullptr;
    ILInstruction* source_address_ = nullptr;
    ILInstruction* size_ = nullptr;
    bool is_volatile_ = false;
    std::uint8_t unaligned_prefix_ = 0;
};

// memset(address, value, size)
class Initblk final : public ILInstruction, public ISupportsVolatilePrefix, public ISupportsUnalignedPrefix {
public:
    static constexpr OpCode opcode = OpCode::Initblk;
    static const SlotInfo address_slot;
    static const SlotInfo value_slot;
    static const SlotInfo size_slot;
    Initblk(ILInstruction* address, ILInstruction* value, ILInstruction* size);
    ILInstruction* address() const { return address_; }
    void set_address(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(address_, v, 0);
    }
    ILInstruction* value() const { return value_; }
    void set_value(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(value_, v, 1);
    }
    ILInstruction* size() const { return size_; }
    void set_size(ILInstruction* v) {
        validate_child(v);
        set_child_instruction(size_, v, 2);
    }
    int get_child_count() const override { return 3; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    bool is_volatile() const override { return is_volatile_; }
    void set_is_volatile(bool v) override { is_volatile_ = v; }
    std::uint8_t unaligned_prefix() const override { return unaligned_prefix_; }
    void set_unaligned_prefix(std::uint8_t v) override { unaligned_prefix_ = v; }
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::SideEffect; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override {
        return address_->flags() | value_->flags() | size_->flags() | InstructionFlags::MayThrow | InstructionFlags::SideEffect;
    }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* address_ = nullptr;
    ILInstruction* value_ = nullptr;
    ILInstruction* size_ = nullptr;
    bool is_volatile_ = false;
    std::uint8_t unaligned_prefix_ = 0;
};

// Load address of instance field.
class LdFlda final : public ILInstruction, public IInstructionWithFieldOperand {
public:
    static constexpr OpCode opcode = OpCode::LdFlda;
    static const SlotInfo target_slot;
    LdFlda(ILInstruction* target, const IField* field);
    ILInstruction* target() const { return target_; }
    void set_target(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(target_, value, 0);
    }
    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    // NullReferenceException/IndexOutOfBoundsException only occurs when the reference is dereferenced
    bool delay_exceptions = false;
    const IField* field() const override { return field_; }
    StackType result_type() const override;
    InstructionFlags direct_flags() const override { return delay_exceptions ? InstructionFlags::None : InstructionFlags::MayThrow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override { return target_->flags() | direct_flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* target_ = nullptr;
    const IField* field_;
};

// Load static field address.
class LdsFlda final : public SimpleInstruction, public IInstructionWithFieldOperand {
public:
    static constexpr OpCode opcode = OpCode::LdsFlda;
    explicit LdsFlda(const IField* field) : SimpleInstruction(opcode), field_(field) {}
    StackType result_type() const override { return StackType::Ref; }
    const IField* field() const override { return field_; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IField* field_;
};

// Casts an object to a class.
class CastClass final : public UnaryInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::CastClass;
    CastClass(ILInstruction* argument, const IType* type) : UnaryInstruction(opcode, argument), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override;
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Test if object is instance of class or interface.
class IsInst final : public UnaryInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::IsInst;
    IsInst(ILInstruction* argument, const IType* type) : UnaryInstruction(opcode, argument), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    bool can_inline_into_slot(int child_index, ILInstruction* new_child) override;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Indirect load (ref/pointer dereference).
class LdObj final : public ILInstruction, public ISupportsVolatilePrefix, public ISupportsUnalignedPrefix, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::LdObj;
    static const SlotInfo target_slot;
    LdObj(ILInstruction* target, const IType* type);
    ILInstruction* target() const { return target_; }
    void set_target(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(target_, value, 0);
    }
    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    bool is_volatile() const override { return is_volatile_; }
    void set_is_volatile(bool value) override { is_volatile_ = value; }
    std::uint8_t unaligned_prefix() const override { return unaligned_prefix_; }
    void set_unaligned_prefix(std::uint8_t value) override { unaligned_prefix_ = value; }
    StackType result_type() const override;
    InstructionFlags direct_flags() const override { return InstructionFlags::SideEffect | InstructionFlags::MayThrow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override { return target_->flags() | InstructionFlags::SideEffect | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    void original_write_to(ITextOutput& output, const ILAstWritingOptions& options) const;
    ILInstruction* target_ = nullptr;
    const IType* type_;
    bool is_volatile_ = false;
    std::uint8_t unaligned_prefix_ = 0;
};

// If argument is a ref to a reference type, loads the object reference, stores it in a temporary, and evaluates to the address of that
// temporary (address.of(ldobj(arg))). Otherwise, returns the argument ref as-is. This is the memory-load semantics of callvirt with a
// generic type as receiver.
class LdObjIfRef final : public ILInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::LdObjIfRef;
    static const SlotInfo target_slot;
    LdObjIfRef(ILInstruction* target, const IType* type);
    ILInstruction* target() const { return target_; }
    void set_target(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(target_, value, 0);
    }
    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::Ref; }
    InstructionFlags direct_flags() const override { return InstructionFlags::SideEffect | InstructionFlags::MayThrow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override { return target_->flags() | InstructionFlags::SideEffect | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* target_ = nullptr;
    const IType* type_;
};

// Indirect store (store to ref/pointer). Evaluates to the value that was stored (when using type byte/short: evaluates to the
// truncated value, sign/zero extended back to I4 based on the type's sign).
class StObj final : public ILInstruction, public ISupportsVolatilePrefix, public ISupportsUnalignedPrefix, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::StObj;
    static const SlotInfo target_slot;
    static const SlotInfo value_slot;
    StObj(ILInstruction* target, ILInstruction* value, const IType* type);
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
    int get_child_count() const override { return 2; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    const IType* type() const override { return type_; }
    void set_type(const IType* v) {
        type_ = v;
        invalidate_flags();
    }
    bool is_volatile() const override { return is_volatile_; }
    void set_is_volatile(bool v) override { is_volatile_ = v; }
    std::uint8_t unaligned_prefix() const override { return unaligned_prefix_; }
    void set_unaligned_prefix(std::uint8_t v) override { unaligned_prefix_ = v; }
    StackType result_type() const override;
    InstructionFlags direct_flags() const override { return InstructionFlags::SideEffect | InstructionFlags::MayThrow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    void check_invariant(ILPhase phase) override;
    bool satisfies_slot_restriction_for_inlining(int child_index, ILInstruction* new_child) override;

protected:
    InstructionFlags compute_flags() const override {
        return target_->flags() | value_->flags() | InstructionFlags::SideEffect | InstructionFlags::MayThrow;
    }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    void original_write_to(ITextOutput& output, const ILAstWritingOptions& options) const;
    // called as part of check_invariant()
    void check_target_slot() const;
    ILInstruction* target_ = nullptr;
    ILInstruction* value_ = nullptr;
    const IType* type_;
    bool is_volatile_ = false;
    std::uint8_t unaligned_prefix_ = 0;
};

// Boxes a value.
class Box final : public UnaryInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::Box;
    Box(ILInstruction* argument, const IType* type) : UnaryInstruction(opcode, argument), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Compute address inside box.
class Unbox final : public UnaryInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::Unbox;
    Unbox(ILInstruction* argument, const IType* type) : UnaryInstruction(opcode, argument), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::Ref; }
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Unbox a value.
class UnboxAny final : public UnaryInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::UnboxAny;
    UnboxAny(ILInstruction* argument, const IType* type) : UnaryInstruction(opcode, argument), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override;
    InstructionFlags direct_flags() const override {
        return UnaryInstruction::direct_flags() | InstructionFlags::SideEffect | InstructionFlags::MayThrow;
    }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override {
        return UnaryInstruction::compute_flags() | InstructionFlags::SideEffect | InstructionFlags::MayThrow;
    }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Creates an array: newarr type(indices...).
class NewArr final : public ILInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::NewArr;
    static const SlotInfo indices_slot;
    NewArr(const IType* type, const std::vector<ILInstruction*>& indices = {});
    // A copy (upstream's ShallowClone) has its own, empty list of indices.
    NewArr(const NewArr& other);
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    InstructionCollection<ILInstruction>& indices() { return indices_; }
    const InstructionCollection<ILInstruction>& indices() const { return indices_; }
    int get_child_count() const override { return indices_.count(); }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::O; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
    InstructionCollection<ILInstruction> indices_;
};

// Returns the default value for a type.
class DefaultValue final : public SimpleInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::DefaultValue;
    explicit DefaultValue(const IType* type) : SimpleInstruction(opcode), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    // Whether the IL stack was empty at the point of this instruction (not counting the argument of the instruction itself).
    bool il_stack_was_empty = false;
    StackType result_type() const override;
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Throws an exception.
class Throw final : public UnaryInstruction {
public:
    static constexpr OpCode opcode = OpCode::Throw;
    explicit Throw(ILInstruction* argument) : UnaryInstruction(opcode, argument) {}
    // The stack type of the throw as an expression (upstream's internal field `resultType`, set by the IL reader).
    StackType result_type_ = StackType::Void;
    StackType result_type() const override { return result_type_; }
    InstructionFlags direct_flags() const override {
        return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow | InstructionFlags::EndPointUnreachable;
    }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override {
        return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow | InstructionFlags::EndPointUnreachable;
    }
};

// Rethrows the current exception.
class Rethrow final : public SimpleInstruction {
public:
    static constexpr OpCode opcode = OpCode::Rethrow;
    Rethrow() : SimpleInstruction(opcode) {}
    StackType result_type() const override { return StackType::Void; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::EndPointUnreachable; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override { return InstructionFlags::MayThrow | InstructionFlags::EndPointUnreachable; }
};

// Gets the size of a type in bytes.
class SizeOf final : public SimpleInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::SizeOf;
    explicit SizeOf(const IType* type) : SimpleInstruction(opcode), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::I4; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Returns the length of an array as 'native unsigned int' (or as I4 or I8).
class LdLen final : public ILInstruction {
public:
    static constexpr OpCode opcode = OpCode::LdLen;
    static const SlotInfo array_slot;
    LdLen(StackType type, ILInstruction* array);
    ILInstruction* array() const { return array_; }
    void set_array(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(array_, value, 0);
    }
    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return result_type_; }
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override { return array_->flags() | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* array_ = nullptr;
    StackType result_type_;
};

// Load address of array element.
class LdElema final : public ILInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::LdElema;
    static const SlotInfo array_slot;
    static const SlotInfo indices_slot;
    LdElema(const IType* type, ILInstruction* array, const std::vector<ILInstruction*>& indices = {});
    // A copy (upstream's ShallowClone) has its own, empty list of indices.
    LdElema(const LdElema& other);
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    ILInstruction* array() const { return array_; }
    void set_array(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(array_, value, 0);
    }
    InstructionCollection<ILInstruction>& indices() { return indices_; }
    const InstructionCollection<ILInstruction>& indices() const { return indices_; }
    int get_child_count() const override { return 1 + indices_.count(); }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    bool with_system_index = false;
    // NullReferenceException/IndexOutOfBoundsException only occurs when the reference is dereferenced
    bool delay_exceptions = false;
    // Whether the 'readonly' prefix was applied to this instruction.
    bool is_read_only = false;
    StackType result_type() const override { return StackType::Ref; }
    InstructionFlags direct_flags() const override { return delay_exceptions ? InstructionFlags::None : InstructionFlags::MayThrow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
    ILInstruction* array_ = nullptr;
    InstructionCollection<ILInstruction> indices_;
};

// Load address of inline array element.
class LdElemaInlineArray final : public ILInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::LdElemaInlineArray;
    static const SlotInfo array_slot;
    static const SlotInfo indices_slot;
    LdElemaInlineArray(const IType* type, ILInstruction* array, const std::vector<ILInstruction*>& indices = {});
    // A copy (upstream's ShallowClone) has its own, empty list of indices.
    LdElemaInlineArray(const LdElemaInlineArray& other);
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    ILInstruction* array() const { return array_; }
    void set_array(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(array_, value, 0);
    }
    InstructionCollection<ILInstruction>& indices() { return indices_; }
    const InstructionCollection<ILInstruction>& indices() const { return indices_; }
    int get_child_count() const override { return 1 + indices_.count(); }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Ref; }
    // Whether the 'readonly' prefix was applied to this instruction.
    bool is_read_only = false;
    InstructionFlags direct_flags() const override { return InstructionFlags::MayThrow; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override;
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
    ILInstruction* array_ = nullptr;
    InstructionCollection<ILInstruction> indices_;
};

// Retrieves a pinnable reference for the input object. The input must be an object reference (O). If the input is an array/string,
// evaluates to a reference to the first element/character, or to a null reference if the array is null or empty. Otherwise, uses the
// GetPinnableReference method to get the reference, or evaluates to a null reference if the input is null.
class GetPinnableReference final : public ILInstruction, public IInstructionWithMethodOperand {
public:
    static constexpr OpCode opcode = OpCode::GetPinnableReference;
    static const SlotInfo argument_slot;
    // `method` is null for arrays and strings.
    GetPinnableReference(ILInstruction* argument, const IMethod* method);
    ILInstruction* argument() const { return argument_; }
    void set_argument(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(argument_, value, 0);
    }
    int get_child_count() const override { return 1; }
    ILInstruction* get_child(int index) const override;
    void set_child(int index, ILInstruction* value) override;
    const SlotInfo& get_child_slot(int index) const override;
    ILInstruction* clone() const override;
    StackType result_type() const override { return StackType::Ref; }
    const IMethod* method() const override { return method_; }
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
    void check_invariant(ILPhase phase) override;

protected:
    InstructionFlags compute_flags() const override { return argument_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* argument_ = nullptr;
    const IMethod* method_;
};

// Push a typed reference (mkrefany).
class MakeRefAny final : public UnaryInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::MakeRefAny;
    MakeRefAny(ILInstruction* argument, const IType* type) : UnaryInstruction(opcode, argument), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

// Push the type token stored in a typed reference.
class RefAnyType final : public UnaryInstruction {
public:
    static constexpr OpCode opcode = OpCode::RefAnyType;
    explicit RefAnyType(ILInstruction* argument) : UnaryInstruction(opcode, argument) {}
    StackType result_type() const override { return StackType::O; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;
};

// Push the address stored in a typed reference.
class RefAnyValue final : public UnaryInstruction, public IInstructionWithTypeOperand {
public:
    static constexpr OpCode opcode = OpCode::RefAnyValue;
    RefAnyValue(ILInstruction* argument, const IType* type) : UnaryInstruction(opcode, argument), type_(type) {}
    const IType* type() const override { return type_; }
    void set_type(const IType* value) {
        type_ = value;
        invalidate_flags();
    }
    StackType result_type() const override { return StackType::Ref; }
    InstructionFlags direct_flags() const override { return UnaryInstruction::direct_flags() | InstructionFlags::MayThrow; }
    ILInstruction* clone() const override;
    bool perform_match(ILInstruction* other, patterns::Match& match) override;
    using ILInstruction::perform_match;

protected:
    InstructionFlags compute_flags() const override { return UnaryInstruction::compute_flags() | InstructionFlags::MayThrow; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    const IType* type_;
};

}  // namespace cb::dotnet::decompiler::il
