// java/decompiler/cfg.hpp -- a method's control-flow graph: its instructions cut into basic blocks (a jump, a switch, a return, a handler's bounds start
// one), the edges between them (regular and to exception handlers), the exception ranges, the jsr subroutines (inlined away), and the passes that
// clean the graph (dead blocks, empty blocks, gotos, merging, value returns).
//
//     ControlFlowGraph graph(*parse_bytecode(method));
//     graph.inline_jsr(method);
//     dead_code::remove_gotos(graph);
//
// Ported from Fernflower (de.fernflower.code.cfg: ControlFlowGraph, BasicBlock, ExceptionRangeCFG; modules.code.DeadCodeHelper), Copyright 2000-2015
// JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). The graph owns its blocks (a block taken out of it stays alive until the graph
// goes, as a Java object does while referenced). Where the original iterates a hash set of blocks (identity-hashed, so its order is the JVM's), this
// iterates in the order the blocks were put in.
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "java/decompiler/instructions.hpp"
#include "java/decompiler/keyed_list.hpp"

namespace cb::java::decompiler {

class StructMethod;

class BasicBlock {
public:
    int id = 0;
    int mark = 0;

    std::shared_ptr<InstructionSequence> seq = std::make_shared<InstructionSequence>();
    std::vector<BasicBlock*> preds, succs, pred_exceptions, succ_exceptions;
    std::vector<int> instr_old_offsets;

    std::shared_ptr<Instruction> instruction(std::size_t index) const { return seq->instr(index); }
    std::shared_ptr<Instruction> last_instruction() const { return seq->empty() ? nullptr : seq->last_instr(); }
    int old_offset(std::size_t index) const { return index < instr_old_offsets.size() ? instr_old_offsets[index] : -1; }
    std::size_t size() const { return seq->length(); }
    void free();

    void add_predecessor(BasicBlock* block) { preds.push_back(block); }
    void remove_predecessor(BasicBlock* block);
    void add_successor(BasicBlock* block);
    void remove_successor(BasicBlock* block);
    void replace_successor(BasicBlock* old_block, BasicBlock* new_block);
    void add_predecessor_exception(BasicBlock* block) { pred_exceptions.push_back(block); }
    void remove_predecessor_exception(BasicBlock* block);
    void add_successor_exception(BasicBlock* block);
    void remove_successor_exception(BasicBlock* block);
    bool is_successor(const BasicBlock* block) const;
    bool is_predecessor(const BasicBlock* block) const;
    // The regular predecessors, then the exception ones.
    std::vector<BasicBlock*> predecessors() const;
    std::string to_string(int indent = 0) const;
};

class ExceptionRangeCFG {
public:
    ExceptionRangeCFG(std::vector<BasicBlock*> protected_range, BasicBlock* handler, std::optional<std::vector<std::string>> exception_types);

    std::vector<BasicBlock*> protected_range;
    BasicBlock* handler = nullptr;
    std::optional<std::vector<std::string>> exception_types;   // none: any (a finally)

    bool is_circular() const;
    void add_exception_type(const std::optional<std::string>& type);
    // The types, each once, joined by ':' in the order Java's HashSet<String> iterates them; none for any.
    std::optional<std::string> unique_exceptions_string() const;
};

class ControlFlowGraph {
public:
    explicit ControlFlowGraph(const InstructionSequence& seq);
    ControlFlowGraph(const ControlFlowGraph&) = delete;
    ControlFlowGraph& operator=(const ControlFlowGraph&) = delete;

    int last_id = 0;

    void free();
    void remove_markers();
    // Every jsr subroutine made part of the code that calls it, the jsr and ret gone.
    void inline_jsr(const StructMethod& method);
    void remove_block(BasicBlock* block);
    ExceptionRangeCFG* exception_range(const BasicBlock* handler, const BasicBlock* block) const;
    std::vector<BasicBlock*> reverse_post_order() const;
    std::string to_string() const;

    // A new block owned by the graph (not yet in `blocks`).
    BasicBlock* make_block();
    BasicBlock* clone_block(const BasicBlock& block);

    KeyedList<BasicBlock*, int>& blocks() { return blocks_; }
    const KeyedList<BasicBlock*, int>& blocks() const { return blocks_; }
    BasicBlock* first() const { return first_; }
    void set_first(BasicBlock* b) { first_ = b; }
    BasicBlock* last() const { return last_; }
    void set_last(BasicBlock* b) { last_ = b; }
    const std::vector<BasicBlock*>& end_blocks() const { return last_->preds; }
    std::vector<std::unique_ptr<ExceptionRangeCFG>>& exceptions() { return exceptions_; }
    const std::vector<std::unique_ptr<ExceptionRangeCFG>>& exceptions() const { return exceptions_; }
    std::vector<std::pair<BasicBlock*, BasicBlock*>>& subroutines() { return subroutines_; }
    std::vector<BasicBlock*>& finally_exits() { return finally_exits_; }

private:
    void build_blocks(const InstructionSequence& seq);
    void set_exception_edges(const InstructionSequence& seq, const std::vector<BasicBlock*>& instr_blocks);
    void set_subroutine_edges();
    void set_first_and_last_blocks();
    void process_jsr();
    int process_jsr_ranges();
    std::vector<BasicBlock*> jsr_range(BasicBlock* jsr, BasicBlock* ret);
    void split_jsr_range(BasicBlock* jsr, BasicBlock* ret, const std::vector<BasicBlock*>& common_blocks);
    void split_jsr_exception_ranges(const std::vector<BasicBlock*>& common_blocks, const std::map<int, BasicBlock*>& new_nodes);
    void remove_jsr(const StructMethod& method);

    std::vector<std::unique_ptr<BasicBlock>> arena_;
    KeyedList<BasicBlock*, int> blocks_;
    BasicBlock* first_ = nullptr;
    BasicBlock* last_ = nullptr;
    std::vector<std::unique_ptr<ExceptionRangeCFG>> exceptions_;
    std::vector<std::pair<BasicBlock*, BasicBlock*>> subroutines_;   // jsr block -> the block after it (the ret's return point)
    std::vector<BasicBlock*> finally_exits_;
};

namespace dead_code {
void remove_dead_blocks(ControlFlowGraph& graph);
void remove_empty_blocks(ControlFlowGraph& graph);
bool is_dominator(const ControlFlowGraph& graph, BasicBlock* block, BasicBlock* dom);
void remove_gotos(ControlFlowGraph& graph);
void connect_dummy_exit_block(ControlFlowGraph& graph);
void incorporate_value_returns(ControlFlowGraph& graph);
void merge_basic_blocks(ControlFlowGraph& graph);
}  // namespace dead_code

// exception_deobfuscator.cpp
namespace exception_deobfuscator {
void restore_pop_ranges(ControlFlowGraph& graph);
void insert_empty_exception_handler_blocks(ControlFlowGraph& graph);
void remove_empty_ranges(ControlFlowGraph& graph);
void remove_circular_ranges(ControlFlowGraph& graph);
bool has_obfuscated_exceptions(const ControlFlowGraph& graph);
}  // namespace exception_deobfuscator

// The graph of a method as the decompiler builds it before making statements: built, dead blocks gone, jsr inlined, the exit connected, gotos gone,
// the exception ranges made regular, value returns taken in, empty handler blocks put in, blocks merged (MethodProcessorRunnable.codeToJava's start).
std::unique_ptr<ControlFlowGraph> build_method_graph(const StructMethod& method);

// Java's String.hashCode (over the UTF-16 code units of a UTF-8 string), and the order a java.util.HashSet<String> of these strings iterates in.
std::int32_t java_string_hash(std::string_view utf8);
std::vector<std::string> java_hash_set_order(const std::vector<std::string>& inserted);

}  // namespace cb::java::decompiler
