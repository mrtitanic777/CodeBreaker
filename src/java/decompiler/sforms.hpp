// java/decompiler/sforms.hpp -- a method's statements flattened into a graph of nodes that each hold a list of expressions (the direct graph), and the
// static single assignment form over it: every assignment of a variable gets a version, every use is given the version(s) that can reach it, and where
// two reach it a phi node joins them. SSAConstructorSparseEx versions the variables; SSAUConstructorSparseEx also keeps the graph of how versions are used
// and which are live (the stack variables processor decides on it which assignments to inline).
//
//     FlattenStatementsHelper flatten;
//     DirectGraph* graph = flatten.build_direct_graph(root);
//     SSAConstructorSparseEx ssa;
//     ssa.split_variables(root, method);     // sets VarExprent::version of every variable in root's expressions
//     for (auto& [pair, versions] : ssa.get_phi().entries()) ...
//
// Ported from Fernflower (de.fernflower.modules.decompiler.sforms: DirectNode, DirectGraph, FlattenStatementsHelper, SSAConstructorSparseEx,
// SSAUConstructorSparseEx), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
//
// Notes on the port:
//  * Nodes and the graph are objects of the run's arena. A node's `exprents` is a pointer: the original's list IS the statement's list for a block (and a
//    loop's init/condition/increment list, a head list ...), so what the passes do to a node's expressions they do to the statement's. A node that has
//    no statement list points at its own (own_exprents).
//  * The original's String[] of node ids is std::array<std::optional<std::string>, 2> (direct, continue); its String[5]/[4] of a finally path is a struct.
//  * The maps of node ids that are only looked up are std::unordered_map; the ones the passes iterate (the phi map: its order decides the order of the
//    merged versions) are JavaHashMap in Java's order.
//  * A method that takes the original's `StructMethod` takes a `const StructMethod*`.
#pragma once

#include <array>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "java/decompiler/context.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/fast_sets.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/keyed_list.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

class StructMethod;

// ---- DirectNode / DirectGraph

class DirectNode : public ArenaObject {
public:
    static constexpr int NODE_DIRECT = 1, NODE_TAIL = 2, NODE_INIT = 3, NODE_CONDITION = 4, NODE_INCREMENT = 5, NODE_TRY = 6;

    int type;
    std::string id;                      // the statement's id, with a suffix for the extra nodes of a statement ("12_tail", "12_cond" ...)
    BasicBlockStatement* block = nullptr;
    Statement* statement;
    std::vector<Exprent*> own_exprents;
    std::vector<Exprent*>* exprents = &own_exprents;   // own_exprents, or the list of the statement this node stands for
    std::vector<DirectNode*> succs;
    std::vector<DirectNode*> preds;

    DirectNode(int type, Statement* statement, std::string id) : type(type), id(std::move(id)), statement(statement) {}
    // A node of a basic block: its id is the block's.
    DirectNode(int type, Statement* statement, BasicBlockStatement* block)
        : type(type), id(std::to_string(block->id)), block(block), statement(statement) {}
    DirectNode(const DirectNode&) = delete;
    DirectNode& operator=(const DirectNode&) = delete;
    std::string to_string() const { return id; }
};

// The original's DirectGraph.ExprentIterator: 0 = success, do nothing; 1 = cancel the iteration; 2 = success, delete the expression.
using ExprentIterator = std::function<int(Exprent* exprent)>;

// A path out of a finally block: where the block was left from, where it goes, and where the finally block was entered.
class FinallyPathWrapper {
public:
    std::string source;
    std::string destination;
    std::string entry;

    // The original's equals/hashCode: those of "source:destination:entry".
    bool operator==(const FinallyPathWrapper& o) const;
    bool operator<(const FinallyPathWrapper& o) const;   // (the order of that text: for the Java-ordered sets' index)
    std::int32_t java_hash() const;
    std::string to_string() const;   // "source->(entry)->destination"

private:
    friend class FlattenStatementsHelper;
    FinallyPathWrapper(std::string source, std::string destination, std::string entry)
        : source(std::move(source)), destination(std::move(destination)), entry(std::move(entry)) {}
};
struct FinallyPathWrapperHash {
    std::int32_t operator()(const FinallyPathWrapper& w) const { return w.java_hash(); }
};

class DirectGraph : public ArenaObject {
public:
    KeyedList<DirectNode*, std::string> nodes;   // by node id, in reverse post order after sort_reverse_post_order
    DirectNode* first = nullptr;
    // exit node id -> [source, destination, entry] of the paths leaving a finally block (short range: to the first outer exit; long range: the whole way)
    std::unordered_map<std::string, std::vector<FinallyPathWrapper>> map_short_range_finally_paths;
    std::unordered_map<std::string, std::vector<FinallyPathWrapper>> map_long_range_finally_paths;
    // node id -> the id of its destination on the negative branch of an if (recorded for && and ||)
    std::unordered_map<std::string, std::string> map_neg_if_branch;
    // nodes that are exception exits of a finally block with a monitor variable
    std::unordered_map<std::string, std::string> map_finally_monitor_exception_path_exits;

    void sort_reverse_post_order();
    // Each expression of each node reachable from first, the first node first (a stack, a node's successors pushed after it). false: cancelled.
    bool iterate_exprents(const ExprentIterator& iter);

private:
    static void add_to_reverse_post_order_list_iterative(DirectNode* root, std::vector<DirectNode*>& lst);
};

// ---- FlattenStatementsHelper

// statement id -> [id of its direct node, id of its continue node (none for most)]
using DestinationNodeIds = std::array<std::optional<std::string>, 2>;

class FlattenStatementsHelper {
public:
    DirectGraph* build_direct_graph(RootStatement* root);
    std::unordered_map<int, DestinationNodeIds>& get_map_destination_nodes() { return map_destination_nodes_; }

private:
    // The stack of the finally blocks an edge leaves (the original's StackEntry).
    struct StackEntry {
        CatchAllStatement* catchstatement;
        bool state;   // true: in the finally handler; false: in the protected block
        int edgetype;
        bool is_finally_exception_path;
        Statement* destination;
        Statement* finally_short_range_entry;
        Statement* finally_long_range_entry;
        DirectNode* finally_short_range_source;
        DirectNode* finally_long_range_source;

        StackEntry(CatchAllStatement* catchstatement, bool state, int edgetype, Statement* destination, Statement* finally_short_range_entry,
                   Statement* finally_long_range_entry, DirectNode* finally_short_range_source, DirectNode* finally_long_range_source,
                   bool is_finally_exception_path)
            : catchstatement(catchstatement), state(state), edgetype(edgetype), is_finally_exception_path(is_finally_exception_path),
              destination(destination), finally_short_range_entry(finally_short_range_entry), finally_long_range_entry(finally_long_range_entry),
              finally_short_range_source(finally_short_range_source), finally_long_range_source(finally_long_range_source) {}
        StackEntry(CatchAllStatement* catchstatement, bool state)
            : StackEntry(catchstatement, state, -1, nullptr, nullptr, nullptr, nullptr, nullptr, false) {}
    };
    // node.id(source), statement.id(destination), edge type
    struct Edge {
        std::string sourceid;
        int statid;
        int edgetype;
    };
    // A finally path before its ids are turned into nodes (the original's String[]: source node id, destination statement id, entry statement id, and
    // the flags: through the monitor exception path (short range only), a continue edge).
    struct FinallyPathIds {
        std::string source;
        int destination;
        int entry;
        bool is_finally_monitor_exception_path;
        bool is_continue_edge;
    };

    void flatten_statement();
    void save_edge(DirectNode* sourcenode, Statement* destination, int edgetype, DirectNode* finally_short_range_source, DirectNode* finally_long_range_source,
                   Statement* finally_short_range_entry, Statement* finally_long_range_entry, bool is_finally_monitor_exception_path);
    void set_edges();

    std::unordered_map<int, DestinationNodeIds> map_destination_nodes_;
    std::vector<Edge> list_edges_;
    // node.id(exit) -> its paths (iterated by set_edges: Java's order)
    JavaHashMap<std::string, std::vector<FinallyPathIds>, JavaStringHash> map_short_range_finally_path_ids_;
    JavaHashMap<std::string, std::vector<FinallyPathIds>, JavaStringHash> map_long_range_finally_path_ids_;
    // node.id -> the id of the statement of its positive if branch
    std::unordered_map<std::string, int> map_pos_if_branch_;
    DirectGraph* graph_ = nullptr;
    RootStatement* root_ = nullptr;
};

// ---- SSAConstructorSparseEx

class SSAConstructorSparseEx {
public:
    // Gives every variable of the statements' expressions its version (VarExprent::set_version), and fills the phi map.
    void split_variables(RootStatement* root, const StructMethod* mt);
    // (var, version) of a phi node -> the versions it joins; iterated in Java's HashMap order.
    JavaHashMap<VarVersionPair, FastSparseSet<int>*, VarVersionPairHash>& get_phi() { return phi_; }
    std::vector<VarVersionPair>& get_start_vars() { return start_vars_; }

private:
    void ssa_statements(DirectGraph* dgraph, std::unordered_set<std::string>& updated);
    // varmaparr: [the map after the expression (true branch), the map of the negative branch or null]
    void process_exprent(Exprent* expr, std::array<SFormsFastMapDirect*, 2>& varmaparr);
    int get_next_free_version(int var);
    void merge_in_var_maps(DirectNode* node, DirectGraph* dgraph);
    SFormsFastMapDirect* get_filtered_out_map(const std::string& nodeid, const std::string& predid, DirectGraph* dgraph, const std::string& destid);
    static SFormsFastMapDirect* merge_maps(SFormsFastMapDirect* map_to, const SFormsFastMapDirect* map2);
    static bool maps_equal(const SFormsFastMapDirect* map1, const SFormsFastMapDirect* map2);
    void set_current_var(SFormsFastMapDirect* varmap, int var, int vers);
    void set_catch_maps(Statement* stat, DirectGraph* dgraph, FlattenStatementsHelper* flatthelper);
    SFormsFastMapDirect* create_first_map(const StructMethod* mt);

    // node id -> var -> versions (in; out of the direct branch; out of the negative branch; extra: where a catch variable or a parameter starts)
    std::unordered_map<std::string, SFormsFastMapDirect*> in_var_versions_;
    std::unordered_map<std::string, SFormsFastMapDirect*> out_var_versions_;
    std::unordered_map<std::string, SFormsFastMapDirect*> out_neg_var_versions_;
    std::unordered_map<std::string, SFormsFastMapDirect*> extra_var_versions_;
    JavaHashMap<VarVersionPair, FastSparseSet<int>*, VarVersionPairHash> phi_;   // (var, version) -> versions
    std::map<int, int> lastversion_;                                            // var -> last version
    std::vector<VarVersionPair> start_vars_;
    FastSparseSetFactory<int>* factory_ = nullptr;
};

// ---- SSAUConstructorSparseEx

class SSAUConstructorSparseEx {
public:
    void split_variables(RootStatement* root, const StructMethod* mt);
    std::map<VarVersionPair, IntHashSet>& get_phi() { return phi_; }
    VarVersionsGraph* get_ssuversions() { return &ssuversions_; }
    // The live versions at that version's use, or null for a version with no node.
    SFormsFastMapDirect* get_live_var_versions_map(const VarVersionPair& varpaar);
    // version -> the id of the first protected range (catch, finally, synchronized) its statement is in.
    std::map<VarVersionPair, int>& get_map_version_first_range() { return map_version_first_range_; }
    // exprent id -> the (negative) variable standing for that field access
    std::map<int, int>& get_map_field_vars() { return map_field_vars_; }

private:
    void ssa_statements(DirectGraph* dgraph, std::unordered_set<std::string>& updated, bool calc_live_vars);
    void process_exprent(Exprent* expr, std::array<SFormsFastMapDirect*, 2>& varmaparr, Statement* stat, bool calc_live_vars);
    void create_or_update_phi_node(const VarVersionPair& phivar, FastSparseSet<int>* vers, Statement* stat);
    void var_map_to_graph(const VarVersionPair& varpaar, SFormsFastMapDirect* varmap);
    // stat: the statement the version is made in (null: a phantom version)
    int get_next_free_version(int var, Statement* stat);
    void merge_in_var_maps(DirectNode* node, DirectGraph* dgraph);
    SFormsFastMapDirect* get_filtered_out_map(const std::string& nodeid, const std::string& predid, DirectGraph* dgraph, const std::string& destid);
    static SFormsFastMapDirect* merge_maps(SFormsFastMapDirect* map_to, const SFormsFastMapDirect* map2);
    static bool maps_equal(const SFormsFastMapDirect* map1, const SFormsFastMapDirect* map2);
    void set_current_var(SFormsFastMapDirect* varmap, int var, int vers);
    void set_catch_maps(Statement* stat, DirectGraph* dgraph, FlattenStatementsHelper* flatthelper);
    SFormsFastMapDirect* create_first_map(const StructMethod* mt, RootStatement* root);
    static std::optional<int> get_first_protected_range(Statement* stat);   // the id of the statement it is the protected part of

    std::unordered_map<std::string, SFormsFastMapDirect*> in_var_versions_;
    std::unordered_map<std::string, SFormsFastMapDirect*> out_var_versions_;
    std::unordered_map<std::string, SFormsFastMapDirect*> out_neg_var_versions_;
    std::unordered_map<std::string, SFormsFastMapDirect*> extra_var_versions_;
    std::map<VarVersionPair, IntHashSet> phi_;   // (var, version) -> versions
    std::map<int, int> lastversion_;
    std::map<VarVersionPair, int> map_version_first_range_;
    std::map<VarVersionPair, VarVersionPair> phantomppnodes_;   // ++ and --: version -> its phantom version
    std::unordered_map<std::string, std::map<VarVersionPair, VarVersionPair>> phantomexitnodes_;   // finally exits: node id -> version -> phantom version
    VarVersionsGraph ssuversions_;   // versions' memory dependencies
    std::map<int, int> map_field_vars_;
    int fieldvarcounter_ = -1;
    FastSparseSetFactory<int>* factory_ = nullptr;
};

}  // namespace cb::java::decompiler
