// java/decompiler/decompose.hpp -- what the decomposition of a statement graph into structured statements asks of it: the dominators of its statements
// (by statement id), which statements may serve as a head's post (the "extended postdominators", with the exception ranges taken into account), and
// the filter that keeps a head and a post inside the same exception ranges.
//
//     DominatorEngine engine(statement);
//     engine.initialize();
//     engine.is_dominator(stat->id, head->id);
//
//     FastExtendedPostdominanceHelper helper;
//     ExtPostMap posts = helper.get_extended_postdominators(general);   // head id -> the ids that may close it
//
// Ported from Fernflower (de.fernflower.modules.decompiler.decompose: DominatorEngine, DominatorTreeExceptionFilter, FastExtendedPostdominanceHelper),
// Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder). GenericDominatorEngine is in dominators.hpp (the blocks' and the
// exception deobfuscator's); IGraph/IGraphNode, the original's interfaces for it, are not needed: it takes the order and the predecessors directly.
//
// The original's null Integer is 0 here: statement ids start at 1 (the statement counter does, and so do the blocks' ids).
#pragma once

#include <memory>
#include <vector>

#include "java/decompiler/java_util.hpp"
#include "java/decompiler/keyed_list.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

// The original's Set<Integer> of statement ids, in the two kinds the decomposition mixes and iterates: a LinkedHashSet (insertion order: the sets
// FastExtendedPostdominanceHelper returns) or a HashSet (Java's own hash order: a set made while merging statements in DomHelper).
class IdSet {
public:
    enum class Order { LINKED, HASH };

    explicit IdSet(Order order = Order::HASH) : order_(order) {}
    template <class Range>
    static IdSet linked_of(const Range& items) {
        IdSet s(Order::LINKED);
        for (const int x : items) s.add(x);
        return s;
    }

    bool add(int id) { return order_ == Order::LINKED ? linked_.add(id) : hashed_.add(id); }
    bool add_all(const IdSet& other) {
        bool changed = false;
        for (const int x : other.items()) changed |= add(x);
        return changed;
    }
    bool remove(int id) { return order_ == Order::LINKED ? linked_.remove(id) : hashed_.remove(id); }
    bool remove_all(const IdSet& other) {
        bool changed = false;
        for (const int x : other.items()) changed |= remove(x);
        return changed;
    }
    bool contains(int id) const { return order_ == Order::LINKED ? linked_.contains(id) : hashed_.contains(id); }
    std::size_t size() const { return order_ == Order::LINKED ? linked_.size() : hashed_.size(); }
    bool empty() const { return size() == 0; }
    // The ids in the order the original iterates them.
    std::vector<int> items() const { return order_ == Order::LINKED ? linked_.items() : hashed_.items(); }

private:
    Order order_;
    LinkedSet<int> linked_;
    JavaHashSet<int, JavaIntHash> hashed_;
};

using IdSetH = JavaHashSet<int, JavaIntHash>;                      // HashSet<Integer>
template <class V>
using IdMapH = JavaHashMap<int, V, JavaIntHash>;                   // HashMap<Integer, V>
// Statement id -> the ids that may close it (the original's HashMap<Integer, Set<Integer>>). The decomposition hands one map from call to call and
// replaces it with a new one at places, so it is passed as a shared pointer where the original passes the reference.
using ExtPostMap = IdMapH<IdSet>;
using ExtPostMapPtr = std::shared_ptr<ExtPostMap>;

// The immediate dominators of a statement's children, by id, over the reverse post order from its first statement.
class DominatorEngine {
public:
    explicit DominatorEngine(Statement* statement) : statement_(statement) {}

    void initialize() { calc_idoms(); }
    // id -> its immediate dominator's id (0: none yet), in reverse post order.
    KeyedList<int, int>& get_ordered_idoms() { return col_ordered_idoms_; }
    // Whether `dom` dominates `node` (the walk up from node ends at a root: the first statement is its own idom).
    bool is_dominator(int node, int dom) const;

private:
    void order_statements();
    static int get_common_idom(int key1, int key2, const KeyedList<int, int>& ordered_idoms);
    void calc_idoms();

    Statement* statement_;
    KeyedList<int, int> col_ordered_idoms_;   // (the original's VBStyleCollection<Integer, Integer>: value idom, key the statement's id)
};

// For each head, which of the statements dominated by it may close it without leaving an exception range the head is not alone in: the dominator tree
// of the statement and the exception ranges over it.
class DominatorTreeExceptionFilter {
public:
    explicit DominatorTreeExceptionFilter(Statement* statement) : statement_(statement) {}

    void initialize();
    // Whether `exit` may close `head` as far as the exception ranges say (the head's filter must exist: std::runtime_error otherwise).
    bool accept_statement_pair(int head, int exit) const;
    DominatorEngine* get_dom_engine() { return dom_engine_.get(); }

private:
    void build_dominator_tree();
    void build_exception_ranges();
    IdMapH<int> build_exception_doms(int id);
    void build_filter(int id);

    Statement* statement_;
    IdMapH<IdSetH> map_tree_branches_;                 // idom -> the nodes it dominates directly
    IdMapH<IdSetH> map_exception_ranges_;              // handler -> the nodes of its range
    IdMapH<int> map_exception_doms_;                   // handler -> the head dominating its range
    IdMapH<IdMapH<int>> map_exception_range_unique_exit_;   // statement -> handler -> the exit node (-1: more than one)
    std::unique_ptr<DominatorEngine> dom_engine_;
};

// The extended postdominators of a statement's children: for each statement, the statements every path from it passes through (over regular and
// exception edges), then filtered to the ones a regular-edge structure can really close, then by dominance and by exception ranges.
//
// The original keeps its sets as FastFixedSet<Integer> (util/FastFixedSetFactory, area D's fast_sets.hpp), so the state lives in Impl, which the
// .cpp defines against that header. The original's private methods are Impl's: calc_default_reachable_sets, remove_erroneous_nodes,
// calc_reachability_supp_points(edgetype), iterate_reachability(action, edgetype), filter_on_exception_ranges(filter), filter_on_dominance(filter).
class FastExtendedPostdominanceHelper {
public:
    FastExtendedPostdominanceHelper();
    ~FastExtendedPostdominanceHelper();
    FastExtendedPostdominanceHelper(const FastExtendedPostdominanceHelper&) = delete;
    FastExtendedPostdominanceHelper& operator=(const FastExtendedPostdominanceHelper&) = delete;

    // The result, a new map: each id's set sorted ascending ("Order Matters!") and in insertion order (IdSet::Order::LINKED).
    ExtPostMap get_extended_postdominators(Statement* statement);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::java::decompiler
