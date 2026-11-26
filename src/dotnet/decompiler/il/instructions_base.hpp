// dotnet/decompiler/il/instructions_base.hpp -- the ILAst's writing options and the abstract instruction classes by child shape:
// SimpleInstruction (no children), UnaryInstruction (an argument), BinaryInstruction (left and right).
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: ILAstWritingOptions, SimpleInstruction, UnaryInstruction, BinaryInstruction, from
// Instructions.cs and Instructions/SimpleInstruction.cs, UnaryInstruction.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler).
//
// How an instruction class is written (the pattern every instruction header follows, from Instructions.cs's generated members):
//   - `static constexpr OpCode opcode = OpCode::X;` in each sealed class (il::as<X> uses it); the constructor passes it on.
//   - a child field `ILInstruction* argument_ = nullptr;` with `argument()` and `set_argument(v)` { validate_child(v);
//     set_child_instruction(argument_, v, 0); }, its slot `static const SlotInfo argument_slot;`
//   - get_child_count / get_child / set_child / get_child_slot by index, throwing std::out_of_range past the end
//   - clone(): `auto* clone = make<X>(*this);` (the copy constructor is upstream's ShallowClone) then each child set to its clone;
//     a class with an InstructionCollection writes a copy constructor that makes a fresh, empty collection for the clone
//   - compute_flags(), direct_flags(), write_to_core(), perform_match()
#pragma once

#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::il {

struct ILAstWritingOptions {
    // writes `logic.and`/`logic.or`/`logic.not` for the if instructions that are those operators
    bool use_logic_operation_sugar = false;
    // writes `ldfld`/`stfld`/`ldsfld`/`stsfld` for ldobj/stobj of a field address
    bool use_field_sugar = false;
    bool show_il_ranges = false;
    bool show_child_index_in_block = false;
};

class SimpleInstruction : public ILInstruction {
public:
    int get_child_count() const final { return 0; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }

protected:
    using ILInstruction::ILInstruction;
    SimpleInstruction(const SimpleInstruction&) = default;
    InstructionFlags compute_flags() const override { return InstructionFlags::None; }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;
};

class UnaryInstruction : public ILInstruction {
public:
    static const SlotInfo argument_slot;
    ILInstruction* argument() const { return argument_; }
    void set_argument(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(argument_, value, 0);
    }
    int get_child_count() const final { return 1; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }

protected:
    UnaryInstruction(OpCode op_code, ILInstruction* argument);
    UnaryInstruction(const UnaryInstruction&) = default;
    // The clone's argument (the derived classes' clone() call this on their copy).
    void clone_children_into(UnaryInstruction& clone) const { clone.set_argument(argument_->clone()); }
    InstructionFlags compute_flags() const override { return argument_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* argument_ = nullptr;
};

class BinaryInstruction : public ILInstruction {
public:
    static const SlotInfo left_slot;
    static const SlotInfo right_slot;
    ILInstruction* left() const { return left_; }
    void set_left(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(left_, value, 0);
    }
    ILInstruction* right() const { return right_; }
    void set_right(ILInstruction* value) {
        validate_child(value);
        set_child_instruction(right_, value, 1);
    }
    int get_child_count() const final { return 2; }
    ILInstruction* get_child(int index) const final;
    void set_child(int index, ILInstruction* value) final;
    const SlotInfo& get_child_slot(int index) const final;
    InstructionFlags direct_flags() const override { return InstructionFlags::None; }

protected:
    BinaryInstruction(OpCode op_code, ILInstruction* left, ILInstruction* right);
    BinaryInstruction(const BinaryInstruction&) = default;
    void clone_children_into(BinaryInstruction& clone) const {
        clone.set_left(left_->clone());
        clone.set_right(right_->clone());
    }
    InstructionFlags compute_flags() const override { return left_->flags() | right_->flags(); }
    void write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const override;

private:
    ILInstruction* left_ = nullptr;
    ILInstruction* right_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il
