// dotnet/decompiler/flow_analysis/control_flow_node.cpp -- ControlFlowNode (ICSharpCode.Decompiler.FlowAnalysis).
#include "dotnet/decompiler/flow_analysis/control_flow_node.hpp"

#include <algorithm>
#include <utility>

namespace cb::dotnet::decompiler::flow_analysis {

namespace {

// ILSpy's GraphTraversal.DepthFirstSearch for one start node and no reversed successors: a stack of "visit" and "post-order"
// continuations; `visit` returns whether the node is new (called in pre-order), the successors are asked for in pre-order, and
// `post_order` runs when everything below a node is done.
void depth_first_search(ControlFlowNode* start, const std::function<bool(ControlFlowNode*)>& visit,
                        const ControlFlowNode::ChildrenFunction& successors, const ControlFlowNode::VisitAction& post_order) {
    struct Item {
        ControlFlowNode* node;
        bool is_post_order_continuation;
    };
    std::vector<Item> worklist;
    worklist.push_back({start, false});
    while (!worklist.empty()) {
        const Item item = worklist.back();
        if (item.is_post_order_continuation) {
            if (post_order) post_order(item.node);
            worklist.pop_back();
            continue;
        }
        if (!visit(item.node)) {
            worklist.pop_back();
            continue;
        }
        const std::size_t old_size = worklist.size();
        worklist.back().is_post_order_continuation = true;
        for (ControlFlowNode* child : successors(item.node)) worklist.push_back({child, false});
        // The stack reverses the children; reversing them again restores their order.
        std::reverse(worklist.begin() + static_cast<std::ptrdiff_t>(old_size), worklist.end());
    }
}

}  // namespace

void ControlFlowNode::add_edge_to(ControlFlowNode* target) {
    successors.push_back(target);
    target->predecessors.push_back(this);
}

void ControlFlowNode::traverse_pre_order(const ChildrenFunction& children, const VisitAction& visit_action) {
    depth_first_search(
        this,
        [&](ControlFlowNode* node) {
            if (node->visited) return false;
            node->visited = true;
            visit_action(node);
            return true;
        },
        children, nullptr);
}

void ControlFlowNode::traverse_post_order(const ChildrenFunction& children, const VisitAction& visit_action) {
    depth_first_search(
        this,
        [](ControlFlowNode* node) {
            if (node->visited) return false;
            node->visited = true;
            return true;
        },
        children, visit_action);
}

bool ControlFlowNode::dominates(const ControlFlowNode* node) const {
    // (upstream's TODO: this can be made O(1) by numbering the dominator tree)
    const ControlFlowNode* tmp = node;
    while (tmp != nullptr) {
        if (tmp == this) return true;
        tmp = tmp->immediate_dominator;
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::flow_analysis
