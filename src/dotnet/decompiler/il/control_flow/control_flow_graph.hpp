// dotnet/decompiler/il/control_flow/control_flow_graph.hpp -- the control flow graph of one block container: a node per block, the edges
// of its branches, the dominator tree, and which nodes have exits out of the region they dominate or out of the container.
//
//     ControlFlowGraph cfg(container, token);
//     ControlFlowNode* node = cfg.get_node(block);
//     if (cfg.has_reachable_exit(node)) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: ControlFlowGraph), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). A separate graph is computed for each block container at the start of the block transforms (before
// loop detection). The graph owns its nodes.
#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/flow_analysis/control_flow_node.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

using flow_analysis::ControlFlowNode;

class ControlFlowGraph {
public:
    // Builds the graph for the blocks of `container`. Return statements, exceptions, or branches leaving the container are not
    // modeled by the graph.
    explicit ControlFlowGraph(BlockContainer* container, const util::CancellationToken& cancellation_token = {});
    ControlFlowGraph(const ControlFlowGraph&) = delete;
    ControlFlowGraph& operator=(const ControlFlowGraph&) = delete;

    // The container the graph was created for. It may differ from the container currently holding a block, because a transform
    // could have moved the block since the graph was created.
    BlockContainer* container() const { return container_; }

    // The nodes, indexed by the original block index: originally nodes()[i]->user_data == container->blocks[i], but transforms may
    // move and reorder the ILAst blocks.
    const std::vector<ControlFlowNode*>& nodes() const { return cfg_; }

    // The node of the block. Unlike the index, this finds nodes even after transforms moved the blocks.
    // Precondition: the block belonged to the container when the graph was created.
    ControlFlowNode* get_node(Block* block) const { return dict_.at(block); }

    // Whether there is a control flow path from `node` to a branch or leave out of container(), or to a node of this container that
    // `node` does not dominate. If false, the only way out of the nodes `node` dominates is a return or throw.
    bool has_reachable_exit(const ControlFlowNode* node) const;

    // Whether the node directly contains a branch or leave instruction exiting the container.
    bool has_direct_exit_out_of_container(const ControlFlowNode* node) const;

private:
    void create_edges(const util::CancellationToken& cancellation_token);
    util::BitSet find_nodes_with_exits_out_of_container() const;

    BlockContainer* container_;
    std::vector<std::unique_ptr<ControlFlowNode>> storage_;
    std::vector<ControlFlowNode*> cfg_;
    std::unordered_map<Block*, ControlFlowNode*> dict_;
    // [i] is true iff cfg_[i] directly contains a branch or leave leaving the container
    util::BitSet node_has_direct_exit_out_of_container_;
    // [i] is true iff there is a path from cfg_[i] to a node not dominated by it, or to a branch or leave leaving the container
    util::BitSet node_has_reachable_exit_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
