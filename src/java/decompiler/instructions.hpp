// java/decompiler/instructions.hpp -- a method's bytecode as the decompiler works on it: instructions in the decompiler's normal form (iconst_n is
// bipush n, iload_n is iload n, goto_w is goto ...), a jump's or switch's targets as instruction indexes, the exception table by instruction, the
// sequence they live in, and the stack and local-variable types an instruction leaves (the interpreter that takes out jsr and ret).
//
//     std::shared_ptr<InstructionSequence> seq = parse_bytecode(method);   // method->code() as instructions
//     seq->instr(3)->to_string();                                         // "@invokevirtual 1c"
//
// Ported from Fernflower (de.fernflower.code: Instruction, JumpInstruction, IfInstruction, SwitchInstruction, InstructionSequence and its two kinds,
// ExceptionHandler, ExceptionTable, ConstantsUtil; code.interpreter: InstructionImpact, Util.instanceOf; struct.gen.DataPoint; util.ListStack;
// StructMethod.parseBytecode), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). Writing instructions back to
// bytes is java/classfile's (the original's per-opcode classes only wrote them), so an instruction is one type with a kind.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "java/decompiler/constant_pool.hpp"
#include "java/decompiler/keyed_list.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

class InstructionSequence;
class StructContext;
class StructMethod;

class Instruction {
public:
    // What the original's class for the opcode was: an if (a jump too), a goto or jsr, a switch, or any other.
    enum class Kind { General, Jump, If, Switch };

    int opcode = 0;
    int group = 0;   // cc::GROUP_*
    bool wide = false;
    int bytecode_version = 0;
    Kind kind = Kind::General;
    std::vector<int> operands;
    // A jump's target, a switch's default and cases: instruction indexes in the sequence (-1: none), set by init_instruction.
    int destination = 0;
    std::vector<int> destinations, values;
    int default_dest = 0;

    static std::shared_ptr<Instruction> make(int opcode, bool wide, int group, int bytecode_version, std::vector<int> operands);
    std::shared_ptr<Instruction> clone() const { return std::make_shared<Instruction>(*this); }

    bool is_jump() const { return kind == Kind::Jump || kind == Kind::If; }   // (an if is a jump, as its class extends the jump's)
    bool is_switch() const { return kind == Kind::Switch; }
    int operands_count() const { return static_cast<int>(operands.size()); }
    int operand(int i) const { return operands.at(static_cast<std::size_t>(i)); }
    bool can_fallthrough() const;
    bool equals_instruction(const Instruction& other) const;
    // A jump's or switch's targets from its relative offsets, against the sequence's current pointer.
    void init_instruction(InstructionSequence& seq);
    // "@wide @iinc 2 -1" (operands in hex, a negative one as -hex).
    std::string to_string() const;
};

// The opcode's name ("invokevirtual").
std::string_view opcode_name(int opcode);

struct ExceptionHandler {
    int from = 0, to = 0, handler = 0;                   // byte offsets
    int from_instr = 0, to_instr = 0, handler_instr = 0;  // instruction indexes
    int class_index = 0;
    std::optional<std::string> exception_class;          // none: any (finally, synchronized)
};

struct ExceptionTable {
    std::vector<std::shared_ptr<ExceptionHandler>> handlers;
};

class InstructionSequence {
public:
    InstructionSequence() : exception_table_(std::make_shared<ExceptionTable>()) {}
    // The full sequence of a method: the handlers' offsets turned into instruction indexes.
    static std::shared_ptr<InstructionSequence> full(KeyedList<std::shared_ptr<Instruction>, int> instructions, std::shared_ptr<ExceptionTable> table);

    // A copy holding the same instructions (as the original's clone: the list is new, the instructions are shared).
    std::shared_ptr<InstructionSequence> clone() const;
    void clear();
    void add_instruction(std::shared_ptr<Instruction> instr, int offset) { instructions_.add_with_key(std::move(instr), offset); }
    void add_instruction(std::size_t index, std::shared_ptr<Instruction> instr, int offset) { instructions_.add_with_key_and_index(index, std::move(instr), offset); }
    void add_sequence(const InstructionSequence& seq);
    void remove_instruction(std::size_t index) { instructions_.remove(index); }
    const std::shared_ptr<Instruction>& current_instr() const { return instructions_.get(static_cast<std::size_t>(pointer_)); }
    const std::shared_ptr<Instruction>& instr(std::size_t index) const { return instructions_.get(index); }
    const std::shared_ptr<Instruction>& last_instr() const { return instructions_.items().back(); }
    int current_offset() const { return *instructions_.key(static_cast<std::size_t>(pointer_)); }
    int offset(std::size_t index) const { return instructions_.key(index).value_or(-1); }
    int pointer_by_abs_offset(int offset) const;
    int pointer_by_rel_offset(int offset) const;
    void set_pointer_by_abs_offset(int offset);
    std::size_t length() const { return instructions_.size(); }
    bool empty() const { return instructions_.empty(); }
    void add_to_pointer(int diff) { pointer_ += diff; }
    int pointer() const { return pointer_; }
    void set_pointer(int pointer) { pointer_ = pointer; }
    const std::shared_ptr<ExceptionTable>& exception_table() const { return exception_table_; }
    void set_exception_table(std::shared_ptr<ExceptionTable> table) { exception_table_ = std::move(table); }
    // Each instruction on a line: "offset: @name operands" with `indent` levels of the indent string before it.
    std::string to_string(int indent = 0) const;

private:
    KeyedList<std::shared_ptr<Instruction>, int> instructions_;
    int pointer_ = 0;
    std::shared_ptr<ExceptionTable> exception_table_;
};

// The method's code as an instruction sequence (the instructions in normal form, the jumps resolved). The method must contain code.
std::shared_ptr<InstructionSequence> parse_bytecode(const StructMethod& method);

// ---- the interpreter

// A stack that remembers its height (a ListStack): pop and get_by_offset throw std::out_of_range past its bottom, as the original's list does.
template <class T>
class ListStack {
public:
    std::vector<T> items;
    int pointer = 0;

    T push(T item) {
        items.push_back(item);
        ++pointer;
        return item;
    }
    T pop() {
        --pointer;
        T o = items.at(static_cast<std::size_t>(pointer));
        items.erase(items.begin() + pointer);
        return o;
    }
    std::optional<T> pop(int count) {
        std::optional<T> o;
        for (int i = count; i > 0; --i) o = pop();
        return o;
    }
    void remove_multiple(int count) {
        for (; count > 0; --count) {
            --pointer;
            (void)items.at(static_cast<std::size_t>(pointer));
            items.erase(items.begin() + pointer);
        }
    }
    bool empty() const { return pointer == 0; }
    const T& get_by_offset(int offset) const { return items.at(static_cast<std::size_t>(pointer + offset)); }
    void insert_by_offset(int offset, T item) {
        const int at = pointer + offset;
        if (at < 0 || at > static_cast<int>(items.size())) throw std::out_of_range("ListStack");
        items.insert(items.begin() + at, std::move(item));
        ++pointer;
    }
    void clear() {
        items.clear();
        pointer = 0;
    }
};

struct DataPoint {
    std::vector<VarType> local_variables;
    ListStack<VarType> stack;

    void set_variable(int index, const VarType& value);
    VarType variable(int index) const;
    DataPoint copy() const { return *this; }
    // The locals at a method's entry: `this` (an object of no known class) unless static, then the parameters (a long or double and an empty half).
    static DataPoint initial(const StructMethod& method);
};

// The types an instruction takes from and leaves on the stack and in the locals.
void step_types(DataPoint& data, const Instruction& instr, const ConstantPool& pool);

// Whether `value_class` is `ref_class` or extends or implements it, as far as the context's classes say.
bool instance_of(const StructContext& context, const std::string& value_class, const std::string& ref_class);

}  // namespace cb::java::decompiler
