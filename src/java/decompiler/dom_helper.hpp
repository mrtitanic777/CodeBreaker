// java/decompiler/dom_helper.hpp -- folding a method's control-flow graph into statements: the graph as one general statement of basic blocks, then
// repeatedly the simple statements found (loops, switches, ifs, sequences, try/catch, try/finally) and, where none is, a general statement cut out
// by postdominance and decomposed on its own; irreducible graphs by splitting nodes; synchronized blocks put together afterwards. With the helpers it
// uses: strongly connected components, the irreducibility test and node splitting, and the checks the statements' constructors share (DecHelper).
//
//     RootStatement* root = DomHelper::parse_graph(graph);   // std::runtime_error "parsing failure!" when the graph cannot be folded
//
// Ported from Fernflower (de.fernflower.modules.decompiler: DomHelper, StrongConnectivityHelper, DecHelper, deobfuscator.IrreducibleCFGDeobfuscator),
// Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). The original's identity-hashed sets of statements are
// insertion-ordered here (LinkedSet); the sets of ids are Java-ordered (IdSet, decompose.hpp).
//
// Calls into other areas (class::method): LabelHelper::low_continue_labels(Statement*, LinkedSet<StatEdge*>&),
// SequenceHelper::condense_sequences(Statement*), (in the .cpp) FastFixedSetFactory/FastFixedSet (fast_sets.hpp).
#pragma once

#include <unordered_map>
#include <vector>

#include "java/decompiler/cfg.hpp"
#include "java/decompiler/decompose.hpp"
#include "java/decompiler/instructions.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/keyed_list.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

// Tarjan's strongly connected components of a statement's children over regular edges.
class StrongConnectivityHelper {
public:
    StrongConnectivityHelper() = default;
    explicit StrongConnectivityHelper(Statement* stat) { find_components(stat); }

    // The components (each in the order the stack popped it), from the first statement, then from every child nobody points to, then the rest.
    std::vector<std::vector<Statement*>>& find_components(Statement* stat);
    // No regular edge leaves the component.
    static bool is_exit_component(const std::vector<Statement*>& lst);
    // The first statement of each exit component.
    static std::vector<Statement*> get_exit_reps(const std::vector<std::vector<Statement*>>& lst);
    std::vector<std::vector<Statement*>>& get_components() { return components_; }
    void set_components(std::vector<std::vector<Statement*>> components) { components_ = std::move(components); }

private:
    void visit_tree(Statement* stat);
    void visit(Statement* stat);

    ListStack<Statement*> lstack_;
    int ncounter_ = 0;
    LinkedSet<Statement*> tset_;
    std::unordered_map<Statement*, int> dfsnummap_, lowmap_;   // (only looked up)
    std::vector<std::vector<Statement*>> components_;
    LinkedSet<Statement*> set_processed_;
};

// The T1/T2 reduction of the graph of a statement's children: whether more than one node is left (irreducible), and a node's split into copies.
class IrreducibleCFGDeobfuscator {
public:
    // False too when any child has an exception edge. (Node: the original's local class of ids and their predecessor/successor sets: a .cpp detail.)
    static bool is_statement_irreducible(Statement* statement);
    // Copies the cheapest child with several predecessors (fewest successors, then smallest times predecessors) for the predecessors of the first
    // regular one; false when no child has several.
    static bool split_irreducible_node(Statement* statement);

private:
    static Statement* get_candidate_for_splitting(Statement* statement);
    static int get_statement_size(Statement* statement);
    static Statement* copy_statement(Statement* from, Statement* to, std::unordered_map<Statement*, Statement*>& map_alt_to_copies);
    static void init_copied_statement(Statement* statement);
};

class DomHelper {
public:
    // The statements the graph folds into: the root statement of a method. std::runtime_error "parsing failure!".
    static RootStatement* parse_graph(ControlFlowGraph& graph);
    // For each child (in post reverse post order), the ids of its postdominators, sorted by the reverse post order (a child that is its own first
    // postdominator is put last). The key is the child's id.
    static KeyedList<std::vector<int>, int> calc_post_dominators(Statement* container);
    static void remove_synchronized_handler(Statement* stat);

private:
    static RootStatement* graph_to_statement(ControlFlowGraph& graph);
    static void build_synchronized(Statement* stat);
    static bool process_statement(Statement* general, ExtPostMapPtr map_ext_post);
    static Statement* find_general_statement(Statement* stat, bool forceall, ExtPostMap& map_ext_post);
    static bool check_synchronized_completeness(Statement* head, const LinkedSet<Statement*>& set_nodes);
    static bool find_simple_statements(Statement* stat, ExtPostMap& map_ext_post);
    static Statement* detect_statement(Statement* head);
};

// The checks the statements' constructors share.
class DecHelper {
public:
    // Whether the statements of the list can be one statement as far as exception edges go: the handlers they differ in are in the list, with all
    // their protected statements.
    static bool check_statement_exceptions(const std::vector<Statement*>& lst);
    // Whether head's regular successors are the branches of one if/switch: `lst` receives them, then head, the post first.
    static bool is_choice_statement(Statement* head, std::vector<Statement*>& lst);
    // The exception handlers of head that no other statement's exception edge reaches.
    static LinkedSet<Statement*> get_unique_pred_exceptions(Statement* head);
    static std::vector<Exprent*> copy_exprent_list(const std::vector<Exprent*>& lst);
};

}  // namespace cb::java::decompiler
