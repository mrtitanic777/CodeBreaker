// dotnet/decompiler/flow_analysis/dominance.cxx -- the dominance analysis (ICSharpCode.Decompiler.FlowAnalysis.Dominance).
#include "dotnet/decompiler/flow_analysis/dominance.hpp"

#include <cassert>

namespace cb::dotnet::decompiler::flow_analysis::dominance {

void compute_dominance(ControlFlowNode* entry_point, const util::CancellationToken& cancellation_token) {
    std::vector<ControlFlowNode*> nodes;
    entry_point->traverse_post_order([](ControlFlowNode* n) { return n->successors; }, [&](ControlFlowNode* n) { nodes.push_back(n); });
    assert(nodes.back() == entry_point);
    for (std::size_t i = 0; i < nodes.size(); ++i) nodes[i]->post_order_number = static_cast<int>(i);

    // For the purpose of this algorithm, the entry point is its own dominator. It is reset to null at the end.
    entry_point->immediate_dominator = entry_point;
    bool changed;
    do {
        changed = false;
        cancellation_token.throw_if_cancellation_requested();
        // For all nodes b except the entry point (in reverse post-order)
        for (int i = static_cast<int>(nodes.size()) - 2; i >= 0; --i) {
            ControlFlowNode* b = nodes[static_cast<std::size_t>(i)];
            // Compute the new immediate dominator:
            ControlFlowNode* new_idom = nullptr;
            for (ControlFlowNode* p : b->predecessors) {
                // Ignore predecessors that were not processed yet
                if (p->immediate_dominator != nullptr) {
                    if (new_idom == nullptr)
                        new_idom = p;
                    else
                        new_idom = find_common_dominator(p, new_idom);
                }
            }
            // The reverse post-order ensures at least one predecessor was processed.
            assert(new_idom != nullptr);
            if (new_idom != b->immediate_dominator) {
                b->immediate_dominator = new_idom;
                changed = true;
            }
        }
    } while (changed);
    // Create the dominator tree for all reachable nodes:
    for (ControlFlowNode* node : nodes) {
        if (node->immediate_dominator != nullptr) {
            node->dominator_tree_children.clear();
            node->has_dominator_tree_children = true;
        }
    }
    entry_point->immediate_dominator = nullptr;
    for (ControlFlowNode* node : nodes) {
        // Create the list of children in the dominator tree
        if (node->immediate_dominator != nullptr) node->immediate_dominator->dominator_tree_children.push_back(node);
        // Also reset the visited flag
        node->visited = false;
    }
}

ControlFlowNode* find_common_dominator(ControlFlowNode* a, ControlFlowNode* b) {
    while (a != b) {
        while (a->post_order_number < b->post_order_number) a = a->immediate_dominator;
        while (b->post_order_number < a->post_order_number) b = b->immediate_dominator;
    }
    return a;
}

util::BitSet mark_nodes_with_reachable_exits(const std::vector<ControlFlowNode*>& cfg) {
#ifndef NDEBUG
    for (std::size_t i = 0; i < cfg.size(); ++i) assert(cfg[i]->user_index == static_cast<int>(i));
#endif
    util::BitSet non_empty(static_cast<int>(cfg.size()));
    for (ControlFlowNode* j : cfg) {
        // If j is a join point (more than one incoming edge): `j.is_reachable() && j.immediate_dominator == null` is the root node,
        // which counts as an extra incoming edge
        if (j->is_reachable() && (j->predecessors.size() >= 2 || (j->predecessors.size() >= 1 && j->immediate_dominator == nullptr))) {
            // Add j to the frontier of all predecessors and their dominators up to j's immediate dominator.
            for (ControlFlowNode* p : j->predecessors) {
                for (ControlFlowNode* runner = p; runner != j->immediate_dominator && runner != j && runner != nullptr;
                     runner = runner->immediate_dominator) {
                    non_empty.set(runner->user_index);
                }
            }
        }
    }
    return non_empty;
}

}  // namespace cb::dotnet::decompiler::flow_analysis::dominance
