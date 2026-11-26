// dotnet/decompiler/il/instructions_base.cpp -- see instructions_base.hpp.
#include "dotnet/decompiler/il/instructions_base.hpp"

#include <stdexcept>

#include "dotnet/decompiler/il/instruction_output_il.hpp"

namespace cb::dotnet::decompiler::il {

namespace {
[[noreturn]] void index_out_of_range() { throw std::out_of_range("child index"); }
}  // namespace

// ---- SimpleInstruction

ILInstruction* SimpleInstruction::get_child(int) const { index_out_of_range(); }
void SimpleInstruction::set_child(int, ILInstruction*) { index_out_of_range(); }
const SlotInfo& SimpleInstruction::get_child_slot(int) const { index_out_of_range(); }

void SimpleInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
}

// ---- UnaryInstruction

const SlotInfo UnaryInstruction::argument_slot{"Argument", true};

UnaryInstruction::UnaryInstruction(OpCode op_code, ILInstruction* argument) : ILInstruction(op_code) { set_argument(argument); }

ILInstruction* UnaryInstruction::get_child(int index) const {
    if (index == 0) return argument_;
    index_out_of_range();
}
void UnaryInstruction::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_argument(value);
    index_out_of_range();
}
const SlotInfo& UnaryInstruction::get_child_slot(int index) const {
    if (index == 0) return argument_slot;
    index_out_of_range();
}

void UnaryInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write('(');
    argument_->write_to(output, options);
    output.write(')');
}

// ---- BinaryInstruction

const SlotInfo BinaryInstruction::left_slot{"Left", true};
const SlotInfo BinaryInstruction::right_slot{"Right", true};

BinaryInstruction::BinaryInstruction(OpCode op_code, ILInstruction* left, ILInstruction* right) : ILInstruction(op_code) {
    set_left(left);
    set_right(right);
}

ILInstruction* BinaryInstruction::get_child(int index) const {
    switch (index) {
        case 0: return left_;
        case 1: return right_;
        default: index_out_of_range();
    }
}
void BinaryInstruction::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_left(value);
        case 1: return set_right(value);
        default: index_out_of_range();
    }
}
const SlotInfo& BinaryInstruction::get_child_slot(int index) const {
    switch (index) {
        case 0: return left_slot;
        case 1: return right_slot;
        default: index_out_of_range();
    }
}

void BinaryInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write('(');
    left_->write_to(output, options);
    output.write(", ");
    right_->write_to(output, options);
    output.write(')');
}

}  // namespace cb::dotnet::decompiler::il
