// java/decompiler/expr_processor.hpp -- turns a method's statements into expressions: each basic block's bytecode is run on an expression stack (a load
// pushes a variable, an add pops two and pushes a function ...), following the direct graph so every path through the finally entry points gets its own
// stack; and the helpers every pass uses to write expressions as Java: type names, casts, the break and continue a statement ends with, lists of
// expressions as lines.
//
//     ExprProcessor proc;                                         // for the method being processed (the context's variable processor)
//     proc.process_statement(root, class_struct);                 // every basic block of `root` now has its expressions
//     TextBuffer text = ExprProcessor::list_to_java(block->get_exprents(), 2, tracer);
//
// Ported from Fernflower (de.fernflower.modules.decompiler: ExprProcessor, ExprentStack, PrimitiveExprsList), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0 (see NOTICE in this folder). The original's tables (opcode to function, if and monitor type ...) are in the .cpp.
#pragma once

#include <deque>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "java/decompiler/exprents.hpp"
#include "java/decompiler/instructions.hpp"
#include "java/decompiler/text_buffer.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

class BytecodeMappingTracer;
class Statement;
class RootStatement;
class BasicBlockStatement;
class StructClass;
class FlattenStatementsHelper;   // sforms (area D)
class VarProcessor;

// The expression stack of a block being processed (the original's list-based stack: pop takes the top off, a copy of the stack is a new stack).
class ExprentStack : public ListStack<Exprent*> {
public:
    ExprentStack() = default;
    explicit ExprentStack(const ListStack<Exprent*>& list) : ListStack<Exprent*>(list) {}
    int size() const { return static_cast<int>(items.size()); }
    ExprentStack clone() const { return *this; }
};

// What processing a block has made so far: the expressions of its statements, in order, and the stack as it stands.
class PrimitiveExprsList {
public:
    PrimitiveExprsList() = default;
    // A new list with a copy of the stack (the expressions in it are the same ones) and no expressions.
    PrimitiveExprsList copy_stack() const {
        PrimitiveExprsList lst;
        lst.set_stack(stack_.clone());
        return lst;
    }
    std::vector<Exprent*>& get_lst_exprents() { return lst_exprents_; }
    ExprentStack& get_stack() { return stack_; }
    void set_stack(ExprentStack stack) { stack_ = std::move(stack); }

private:
    std::vector<Exprent*> lst_exprents_;
    ExprentStack stack_;
};

class ExprProcessor {
public:
    static constexpr std::string_view UNDEFINED_TYPE_STRING = "<undefinedtype>", UNKNOWN_TYPE_STRING = "<unknown>", NULL_TYPE_STRING = "<null>";

    // The processor of the method being decompiled: the expressions it makes refer to the context's current variable processor.
    ExprProcessor() : var_processor_(DecompilerContext::current().current_var_processor) {}

    // Every basic block of the statement tree gets its expressions (set_exprents), then every statement its own (init_exprents).
    void process_statement(RootStatement* root, const StructClass* cl);
    // The expressions of one block, run from its bytecode against the stack in `data`.
    void process_block(BasicBlockStatement* stat, PrimitiveExprsList& data, const StructClass* cl);

    // Each entry replaced by a copy of itself.
    static void copy_entries(std::vector<Exprent*>& stack);
    static void copy_entries(ListStack<Exprent*>& stack);

    static std::string get_type_name(const VarType& type);
    static std::string get_type_name(const VarType& type, bool get_short);
    // The type name with "[]" for each array dimension.
    static std::string get_cast_type_name(const VarType& type);
    static std::string get_cast_type_name(const VarType& type, bool get_short);
    // A catch variable's first expression: a stack variable (STACK_BASE) assigned the variable, and the stack holding that variable.
    static PrimitiveExprsList get_expression_data(VarExprent* var);
    static bool ends_with_semikolon(Exprent* expr);
    // The statement's text, and the break or continue its single outgoing edge needs; ";" when both are empty and `semicolon`.
    static TextBuffer jmp_wrapper(Statement* stat, int indent, bool semicolon, BytecodeMappingTracer& tracer);
    // "a.b.C" from "a/b/C"; "$" kept as it is for an own class, made "." for a library one.
    static std::string build_java_class_name(const std::string& name);
    // Each expression on a line of its own (indented, ";" where it takes one). A null list (the original's) writes nothing.
    static TextBuffer list_to_java(const std::vector<Exprent*>& lst, int indent, BytecodeMappingTracer& tracer);
    static TextBuffer list_to_java(const std::optional<std::vector<Exprent*>>& lst, int indent, BytecodeMappingTracer& tracer);
    // The value a new array of that element type starts with (null, 0.0F, 0L, 0.0D or 0).
    static ConstExprent* get_default_array_value(const VarType& arrtype);
    // The expression's text appended to `buffer`, in a cast to `left_type` when its type is not one (or when `cast_always`): true when it was cast.
    static bool get_casted_exprent(Exprent* exprent, const VarType& left_type, TextBuffer& buffer, int indent, bool cast_null, BytecodeMappingTracer& tracer);
    static bool get_casted_exprent(Exprent* exprent, const VarType& left_type, TextBuffer& buffer, int indent, bool cast_null, bool cast_always,
                                   BytecodeMappingTracer& tracer);

private:
    // The finally entry points a path went through, as one string ("a:b:"); none (the original's null) for no entry points.
    static std::optional<std::string> build_entry_point_key(const std::deque<std::string>& entrypoints);
    static PrimitiveExprsList copy_var_exprents(PrimitiveExprsList data);   // its stack's entries copied
    // catch variable of each handler's first node, by node id.
    static void collect_catch_vars(Statement* stat, FlattenStatementsHelper& flatthelper, std::map<std::string, VarExprent*>& map);
    static void init_statement_exprents(Statement* stat);
    static bool is_int_constant(Exprent* exprent);
    // Pushes `exprent` as a new stack variable (an assignment to it joins the list) and the variable onto the stack.
    void push_ex(ExprentStack& stack, std::vector<Exprent*>& exprlist, Exprent* exprent);
    void push_ex(ExprentStack& stack, std::vector<Exprent*>& exprlist, Exprent* exprent, const std::optional<VarType>& vartype);
    // The dup_x family: the entries down to `offset` taken off, each stored in a stack variable, and the one at `copyoffset` among them put back
    // below them as well.
    void insert_by_offset_ex(int offset, ExprentStack& stack, std::vector<Exprent*>& exprlist, int copyoffset);

    VarProcessor* var_processor_;
};

}  // namespace cb::java::decompiler
