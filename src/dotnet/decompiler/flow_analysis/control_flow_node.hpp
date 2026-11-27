// dotnet/decompiler/flow_analysis/control_flow_node.hpp -- a block in a control flow graph: its edges, its place in the dominator tree,
// and flags and slots the algorithms use.
//
//     ControlFlowNode a, b;
//     a.add_edge_to(&b);
//     a.traverse_post_order([](ControlFlowNode* n) { return n->successors; }, [](ControlFlowNode* n) { ... });
//
// Ported from ILSpy (ICSharpCode.Decompiler.FlowAnalysis: ControlFlowNode), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). The graph walks upstream does with GraphTraversal.DepthFirstSearch are written here, in the same
// order. The node does not own its neighbours: the graph that made the nodes does (see il/control_flow/control_flow_graph.hpp).
#pragma once

#include <functional>
#include <vector>

namespace cb::dotnet::decompiler::il {
class Block;
}

namespace cb::dotnet::decompiler::flow_analysis {

class ControlFlowNode {
public:
    // Can be used to look up additional information in an array.
    int user_index = 0;
    // The block this node stands for (upstream's `object UserData`; the graphs built so far are all of ILAst blocks).
    il::Block* user_data = nullptr;
    // Visited flag, used in various algorithms.
    bool visited = false;
    // The node's index in a post-order traversal of the graph from the entry point; computed by the dominance analysis.
    int post_order_number = 0;
    // The parent in the dominator tree; null if dominance has not been calculated, if the node is unreachable, or for the entry point.
    ControlFlowNode* immediate_dominator = nullptr;
    // The children in the dominator tree. Upstream's null (dominance not calculated, or the node is unreachable) is
    // has_dominator_tree_children == false; only the dominance analysis sets either.
    std::vector<ControlFlowNode*> dominator_tree_children;
    bool has_dominator_tree_children = false;
    // Incoming and outgoing control flow edges.
    std::vector<ControlFlowNode*> predecessors;
    std::vector<ControlFlowNode*> successors;

    // Whether this node is reachable. Requires that dominance is computed.
    bool is_reachable() const { return has_dominator_tree_children; }

    void add_edge_to(ControlFlowNode* target);

    using ChildrenFunction = std::function<std::vector<ControlFlowNode*>(ControlFlowNode*)>;
    using VisitAction = std::function<void(ControlFlowNode*)>;
    // Depth-first walks from this node over the nodes that were not visited yet (the walk sets `visited`, it does not reset it).
    void traverse_pre_order(const ChildrenFunction& children, const VisitAction& visit_action);
    void traverse_post_order(const ChildrenFunction& children, const VisitAction& visit_action);

    // Whether this node dominates `node`.
    bool dominates(const ControlFlowNode* node) const;
};

}  // namespace cb::dotnet::decompiler::flow_analysis
