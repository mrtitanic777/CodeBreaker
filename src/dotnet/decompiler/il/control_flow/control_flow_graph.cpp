// dotnet/decompiler/il/control_flow/control_flow_graph.cpp -- ControlFlowGraph (ICSharpCode.Decompiler.IL.ControlFlow).
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"

#include <cassert>

#include "dotnet/decompiler/flow_analysis/dominance.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

ControlFlowGraph::ControlFlowGraph(BlockContainer* container, const util::CancellationToken& cancellation_token)
    : container_(container),
      node_has_direct_exit_out_of_container_(container->blocks.count()),
      node_has_reachable_exit_(0) {
    const int count = container->blocks.count();
    cfg_.resize(static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        Block* block = container->blocks[i];
        storage_.push_back(std::make_unique<ControlFlowNode>());
        ControlFlowNode* node = storage_.back().get();
        node->user_index = i;
        node->user_data = block;
        cfg_[static_cast<std::size_t>(i)] = node;
        dict_.emplace(block, node);
    }

    create_edges(cancellation_token);
    flow_analysis::dominance::compute_dominance(cfg_[0], cancellation_token);
    node_has_reachable_exit_ = flow_analysis::dominance::mark_nodes_with_reachable_exits(cfg_);
    node_has_reachable_exit_.union_with(find_nodes_with_exits_out_of_container());
}

void ControlFlowGraph::create_edges(const util::CancellationToken& cancellation_token) {
    for (int i = 0; i < container_->blocks.count(); ++i) {
        cancellation_token.throw_if_cancellation_requested();
        Block* block = container_->blocks[i];
        ControlFlowNode* source_node = cfg_[static_cast<std::size_t>(i)];
        for (ILInstruction* node : block->descendants()) {
            if (auto* branch = as<Branch>(node)) {
                if (branch->target_block()->parent() == container_) {
                    source_node->add_edge_to(cfg_[static_cast<std::size_t>(container_->blocks.index_of(branch->target_block()))]);
                } else if (branch->target_block()->is_descendant_of(container_)) {
                    // Internal control flow within a nested container.
                } else {
                    // Branch out of this container into a parent container. Like return statements and exceptional exits, this is
                    // ignored for the graph and the dominance calculation. However, it is relevant for has_reachable_exit().
                    node_has_direct_exit_out_of_container_.set(i);
                }
            } else if (auto* leave = as<Leave>(node); leave != nullptr && !leave->target_container()->is_descendant_of(block)) {
                // Leave instructions (like other exits out of the container) are ignored for the graph and the dominance, but are
                // relevant for has_reachable_exit(). However, a 'leave' that exits the whole function represents a return, and is
                // not considered a reachable exit.
                if (!leave->is_leaving_function()) node_has_direct_exit_out_of_container_.set(i);
            }
        }
    }
}

util::BitSet ControlFlowGraph::find_nodes_with_exits_out_of_container() const {
    // Also mark the nodes that exit the block container altogether.
    // Invariant: leaving[n.user_index] implies leaving[n.immediate_dominator.user_index]
    util::BitSet leaving(static_cast<int>(cfg_.size()));
    for (ControlFlowNode* node : cfg_) {
        if (leaving[node->user_index]) continue;
        if (node_has_direct_exit_out_of_container_[node->user_index]) {
            for (ControlFlowNode* p = node; p != nullptr; p = p->immediate_dominator) {
                // we can stop marking when we reach an already-marked node
                if (leaving[p->user_index]) break;
                leaving.set(p->user_index);
            }
        }
    }
    return leaving;
}

bool ControlFlowGraph::has_reachable_exit(const ControlFlowNode* node) const {
    assert(cfg_[static_cast<std::size_t>(node->user_index)] == node);
    return node_has_reachable_exit_[node->user_index];
}

bool ControlFlowGraph::has_direct_exit_out_of_container(const ControlFlowNode* node) const {
    assert(cfg_[static_cast<std::size_t>(node->user_index)] == node);
    return node_has_direct_exit_out_of_container_[node->user_index];
}

}  // namespace cb::dotnet::decompiler::il::control_flow
