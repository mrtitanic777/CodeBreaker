// dotnet/decompiler/il/control_flow/loop_detection.hpp -- detects loops (and the bodies of switches) in the ILAst.
//
// A block is a loop head if a block it dominates branches back to it. The natural loop is extended to reduce the number of exit
// points (C# only allows reaching a single exit, with `break` or when the loop condition turns false, so any more exits would need
// goto), and the blocks of the loop are moved into a new BlockContainer of kind Loop. A switch instruction gets the same
// treatment: its body becomes a container of kind Switch, so `break;` works in it.
//
//     // a post-order block transform, run early: before the other control flow structures are detected, on basic blocks
//     // (not extended basic blocks), and after the return block is duplicated (ControlFlowSimplification)
//     LoopDetection transform;
//     transform.run(block, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: LoopDetection, from ControlFlow/LoopDetection.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "dotnet/decompiler/flow_analysis/control_flow_node.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/control_flow/switch_detection.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class LoopDetection final : public transforms::IBlockTransform {
public:
    // Checks whether `block` is a loop head; if it is, constructs a loop instruction (a nested BlockContainer).
    void run(Block* block, transforms::BlockTransformContext& context) override;
    std::string name() const override { return "LoopDetection"; }

private:
    using NodeList = std::vector<ControlFlowNode*>;

    // For each block of the loop that is the head of a nested loop or switch, includes all blocks of the nested container, so the
    // loops stay well-nested. Precondition and postcondition: node.visited iff the loop contains the node.
    void include_nested_containers(NodeList& loop);
    void include_block(Block* block, NodeList& loop);

    // Given a natural loop, adds more nodes to reduce the number of exit points. Requires and keeps the invariant that a node is
    // marked visited iff it is in the loop. `exit_point` is the node reached when leaving the loop: a graph node, the "no exit
    // point" marker (everything dominated by the head goes into the loop), or null.
    void extend_loop(ControlFlowNode* loop_head, NodeList& loop, ControlFlowNode** exit_point);

    // Finds a suitable single exit point for the loop: a node of the graph, the no-exit-point marker, or null (exit point unknown:
    // extend the loop heuristically). Must not write to the visited flags.
    ControlFlowNode* find_exit_point(ControlFlowNode* loop_head, const NodeList& natural_loop);
    // An exit point is invalid iff a node reachable from it is dominated by the loop head but not by the exit point.
    bool validate_exit_point(ControlFlowNode* loop_head, ControlFlowNode* exit_point);
    // ControlFlowGraph::has_reachable_exit, but for switches continue blocks are not counted as reachable exits.
    bool has_reachable_exit(ControlFlowNode* node);
    // The children in a loop dominator tree, without the exit point (and without continue statements when analyzing switches).
    NodeList dominator_tree_children(ControlFlowNode* n, ControlFlowNode* exit_point);
    // Picks an exit point: the node with the highest IL offset that has no reachable exits.
    void pick_exit_point(ControlFlowNode* node, ControlFlowNode** exit_point, int* exit_point_il_offset);
    static void consider_return_as_exit_point(Block* block, ControlFlowNode** exit_point, int* exit_point_il_offset);

    // A reverse control flow graph for the nodes dominated by the loop head, with an artificial exit node (see prepare_reverse_cfg).
    struct ReverseCfg {
        std::vector<std::unique_ptr<ControlFlowNode>> storage;
        NodeList nodes;  // [i] is the reverse node of the graph's node i; the last one is the exit node
    };
    // Each node cfg[i] has a node rev[i]; edges exist only for the nodes dominated by the loop head, reversed, and an artificial exit
    // node gets those leaving the set of dominated nodes or the container. `exit_node_arity`: 0 = no graph nodes used as exit nodes,
    // 1 = a single node (not dominated by the loop head), 2 = more than one.
    ReverseCfg prepare_reverse_cfg(ControlFlowNode* loop_head, int* exit_node_arity);
    static bool is_possible_foreach_loop(Block* loop_head, Branch** exit_branch);

    // The fall-back heuristic, used when a single exit point is impossible: walks the head's dominator tree in pre-order and adds
    // each candidate that reduces the number of exit points.
    void extend_loop_heuristic(ControlFlowNode* loop_head, NodeList& loop, ControlFlowNode* candidate);
    // Whether `node` is an exit point of the loop marked by the visited flag.
    static bool is_exit_point(ControlFlowNode* node);

    // Moves the blocks associated with the loop into a new block container.
    void construct_loop(NodeList& loop, ControlFlowNode* exit_point);
    void move_blocks_into_container(NodeList& loop, BlockContainer* loop_container);
    void detect_switch_body(Block* block, SwitchInstruction* switch_inst);

    transforms::BlockTransformContext* context_ = nullptr;
    // The block container of the current control flow graph.
    BlockContainer* current_block_container_ = nullptr;
    // Set during detect_switch_body, used by extend_loop and its children.
    bool is_switch_ = false;
    // Used when is_switch_, to determine the appropriate exit points within loops.
    std::unique_ptr<SwitchDetection::LoopContext> loop_context_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
