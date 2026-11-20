// java/decompiler/vars.hpp -- the variables of a method: which of its local variable slots hold which variable at which point (the versions of SSA), their
// types (the least and greatest type each must have), which are final, their names, and where each is declared.
//
//     VarProcessor* proc = make<VarProcessor>();
//     proc->set_var_versions(root);            // SSA, merged phi versions, types, new variable indexes
//     proc->set_var_definitions(root);         // names, and where each variable is first declared
//     proc->get_var_name(VarVersionPair(3, 0)); // "var3"
//
// Ported from Fernflower (de.fernflower.modules.decompiler.vars: VarProcessor, VarVersionsProcessor, VarTypeProcessor, VarDefinitionHelper, VarVersionNode,
// VarVersionEdge, VarVersionsGraph), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). VarVersionPair and
// CheckTypesResult stay in exprents.hpp.
//
// Notes on the port:
//  * Nodes, edges and the processors are objects of the run's arena. HashSet<VarVersionEdge> keeps the original's equals (same type, source and
//    destination) over insertion order (VarVersionEdgeSet).
//  * Maps keyed by VarVersionPair that the original iterates are JavaHashMap with the original's hash (var * 3 + version): VarVersionMap<V>. Their get()
//    returns a pointer (null: absent) that stays good until the next put.
//  * VarVersionNode no longer implements IGraphNode (the dominator engine here takes its predecessor function as a lambda).
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "java/decompiler/context.hpp"
#include "java/decompiler/dominators.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/fast_sets.hpp"
#include "java/decompiler/java_util.hpp"
#include "java/decompiler/keyed_list.hpp"
#include "java/decompiler/var_type.hpp"

namespace cb::java::decompiler {

class StructMethod;
class StructClass;
class Statement;
class RootStatement;
class DirectGraph;
class SSAConstructorSparseEx;
class VarNamesCollector;
class VarVersionNode;
class VarVersionEdge;

// VarVersionPair.hashCode (var * 3 + version), for the Java-ordered maps and sets; std::hash for the ordinary ones.
struct VarVersionPairHash {
    std::int32_t operator()(const VarVersionPair& p) const { return p.java_hash(); }
};

template <class V>
using VarVersionMap = JavaHashMap<VarVersionPair, V, VarVersionPairHash>;
using VarVersionSet = JavaHashSet<VarVersionPair, VarVersionPairHash>;

}  // namespace cb::java::decompiler

template <>
struct std::hash<cb::java::decompiler::VarVersionPair> {
    std::size_t operator()(const cb::java::decompiler::VarVersionPair& p) const { return static_cast<std::size_t>(p.java_hash()); }
};

namespace cb::java::decompiler {

class VarVersionEdge : public ArenaObject {
public:
    static constexpr int EDGE_GENERAL = 0, EDGE_PHANTOM = 1;

    int type;
    VarVersionNode* source;
    VarVersionNode* dest;

    VarVersionEdge(int type, VarVersionNode* source, VarVersionNode* dest) : type(type), source(source), dest(dest) {}
    // The original's equals: the same type, source and destination.
    bool equals(const VarVersionEdge* o) const { return o == this || (o != nullptr && type == o->type && source == o->source && dest == o->dest); }
    std::string to_string() const;   // "(v_n) ->type-> (v_n)"
};

// The original's HashSet<VarVersionEdge> semantics (equals-based), in insertion order.
struct VarVersionEdgeHash {
    std::size_t operator()(const VarVersionEdge* e) const {
        return std::hash<const void*>()(e->source) ^ (std::hash<const void*>()(e->dest) + static_cast<std::size_t>(e->type));
    }
};
struct VarVersionEdgeEq {
    bool operator()(const VarVersionEdge* a, const VarVersionEdge* b) const { return a->equals(b); }
};
using VarVersionEdgeSet = LinkedSet<VarVersionEdge*, VarVersionEdgeHash, VarVersionEdgeEq>;

class VarVersionNode : public ArenaObject {
public:
    static constexpr int FLAG_PHANTOM_FINEXIT = 2;

    int var;
    int version;
    VarVersionEdgeSet succs;
    VarVersionEdgeSet preds;
    int flags = 0;
    SFormsFastMapDirect* live;   // the live versions at this version's use (a new empty map)

    VarVersionNode(int var, int version);
    VarVersionPair get_var_paar() const { return VarVersionPair(var, version); }
    std::vector<VarVersionNode*> get_predecessors() const;   // the sources of the predecessor edges
    void remove_successor(VarVersionEdge* edge) { succs.remove(edge); }
    void remove_predecessor(VarVersionEdge* edge) { preds.remove(edge); }
    void add_successor(VarVersionEdge* edge) { succs.add(edge); }
    void add_predecessor(VarVersionEdge* edge) { preds.add(edge); }
    std::string to_string() const { return "(" + std::to_string(var) + "_" + std::to_string(version) + ")"; }
};

// The graph of the versions of the SSA-with-memory-dependencies form (the SSAU constructor builds it): which version a version was made from.
class VarVersionsGraph {
public:
    int counter = 0;
    KeyedList<VarVersionNode*, VarVersionPair> nodes;

    VarVersionNode* create_node(const VarVersionPair& ver);
    void add_nodes(const std::vector<VarVersionNode*>& colnodes, const std::vector<VarVersionPair>& colpaars);
    // Whether every path from a root to `node` goes through one of domnodes.
    bool is_dominator_set(VarVersionNode* node, const LinkedSet<VarVersionNode*>& domnodes);
    void init_dominators();   // finds the roots (nodes with no predecessors) and builds the dominator engine

private:
    static std::vector<VarVersionNode*> get_reversed_post_order(const LinkedSet<VarVersionNode*>& roots);
    static void add_to_reverse_post_order_list_iterative(VarVersionNode* root, std::vector<VarVersionNode*>& lst,
                                                         std::unordered_set<VarVersionNode*>& set_visited);

    std::optional<GenericDominatorEngine<VarVersionNode*>> engine_;
};

class VarVersionsProcessor;

// The variables of a method: their names, versions and types.
class VarProcessor : public ArenaObject {
public:
    // SSA (SSAConstructorSparseEx) over the root, the phi versions merged, the types calculated, the new indexes set.
    void set_var_versions(RootStatement* root);
    // Names of the variables (the current method's) and the declaration of each (VarDefinitionHelper).
    void set_var_definitions(Statement* root);
    // The names from the debug information (original variable index -> name); a name met again gets a counter after it.
    void set_debug_var_names(const std::map<int, std::string>& map_debug_var_names);
    void refresh_var_names(VarNamesCollector& vc);   // every name replaced by a free one of the collector
    // The least type of the variable, none while its versions are not set.
    std::optional<VarType> get_var_type(const VarVersionPair& pair) const;
    void set_var_type(const VarVersionPair& pair, const VarType& type);
    std::optional<std::string> get_var_name(const VarVersionPair& pair) const;
    void set_var_name(const VarVersionPair& pair, const std::string& name);
    int get_var_final(const VarVersionPair& pair) const;   // VarTypeProcessor::VAR_FINAL when versions are not set
    void set_var_final(const VarVersionPair& pair, int final_type);
    VarVersionMap<std::string>& get_this_vars() { return this_vars_; }
    VarVersionSet& get_external_vars() { return external_vars_; }

private:
    VarVersionMap<std::string> map_var_names_;
    VarVersionsProcessor* var_versions_ = nullptr;
    VarVersionMap<std::string> this_vars_;   // the variables that are `this` (of the class named)
    VarVersionSet external_vars_;
};

class VarTypeProcessor;

// SSA, merging, typing and renumbering of a method's variables.
class VarVersionsProcessor : public ArenaObject {
public:
    void set_var_versions(RootStatement* root);
    std::optional<VarType> get_var_type(const VarVersionPair& pair) const;
    void set_var_type(const VarVersionPair& pair, const VarType& type);
    int get_var_final(const VarVersionPair& pair) const;
    void set_var_final(const VarVersionPair& pair, int final_type);
    // new variable index -> the index it was made from.
    const std::map<int, int>& get_map_original_var_indices() const { return map_original_var_indices_; }

private:
    static void merge_phi_versions(SSAConstructorSparseEx* ssa, DirectGraph* graph);
    static void update_versions(DirectGraph* graph, const VarVersionMap<int>& versions);
    static void eliminate_non_java_types(VarTypeProcessor* type_processor);
    static void simple_merge(VarTypeProcessor* type_processor, DirectGraph* graph, const StructMethod* mt);
    void set_new_var_indices(VarTypeProcessor* type_processor, DirectGraph* graph);

    std::map<int, int> map_original_var_indices_;
    VarTypeProcessor* type_processor_ = nullptr;
};

// The least (min) and greatest (max) type of each variable version and each integer constant (a VarVersionPair(const's id, -1)), by iterating the
// expressions' type bounds until nothing changes.
class VarTypeProcessor : public ArenaObject {
public:
    static constexpr int VAR_NON_FINAL = 1, VAR_EXPLICIT_FINAL = 2, VAR_FINAL = 3;

    void calculate_var_types(RootStatement* root, DirectGraph* graph);
    // (the max map may hold a none: the original puts a null there)
    VarVersionMap<std::optional<VarType>>& get_map_exprent_max_types() { return map_exprent_max_types_; }
    VarVersionMap<VarType>& get_map_exprent_min_types() { return map_exprent_min_types_; }
    VarVersionMap<int>& get_map_final_vars() { return map_final_vars_; }
    void set_var_type(const VarVersionPair& pair, const VarType& type) { map_exprent_min_types_.put(pair, type); }
    std::optional<VarType> get_var_type(const VarVersionPair& pair) const {
        const VarType* t = map_exprent_min_types_.get(pair);
        return t ? std::optional<VarType>(*t) : std::nullopt;
    }

private:
    void set_init_vars(RootStatement* root);   // `this`, the parameters and the catch variables
    static void reset_exprent_types(DirectGraph* graph);
    bool process_var_types(DirectGraph* graph);   // true when nothing changed
    bool check_type_exprent(Exprent* exprent);
    // minMax: 0 = the least type, 1 = the greatest. false: a min type grew, the pass must run again.
    bool change_exprent_type(Exprent* exprent, VarType new_type, int min_max);

    VarVersionMap<VarType> map_exprent_min_types_;
    VarVersionMap<std::optional<VarType>> map_exprent_max_types_;
    VarVersionMap<int> map_final_vars_;
};

// Where each variable is declared: the statement that contains all its uses, and in it the first assignment or the first block that uses it.
class VarDefinitionHelper {
public:
    // The method's parameters and `this`, and the catch variables, are named here (and are implicitly defined); the variables of the statements are counted.
    VarDefinitionHelper(Statement* root, const StructMethod* mt, VarProcessor* varproc);
    void set_var_definitions();

private:
    Statement* find_first_block(Statement* stat, int varindex);
    IntHashSet init_statement(Statement* stat);   // the variables the statement uses (those used more than once there are declared in it)
    static std::vector<VarExprent*> get_all_vars(const std::vector<Exprent*>& lst);
    static bool set_definition(Exprent* expr, int index);

    IntHashMap<Statement*> map_var_def_statements_;                 // variable index -> the statement it is declared in
    std::unordered_map<int, IntHashSet> map_statement_vars_;        // statement.id -> the variables used in it
    std::set<int> impl_def_vars_;                                   // parameters and catch variables
    VarProcessor* varproc_;
};

}  // namespace cb::java::decompiler
