// dotnet/decompiler/il/control_flow/loop_detection.cpp -- see loop_detection.hpp.
#include "dotnet/decompiler/il/control_flow/loop_detection.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <optional>

#include "core/error.hpp"
#include "dotnet/decompiler/flow_analysis/dominance.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/util/tree_traversal.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

namespace {

// A special control flow node (not part of any graph) that signifies that we want to construct a loop without any exit point.
ControlFlowNode* no_exit_point() {
    static ControlFlowNode node;
    return &node;
}

bool is_return_leave(ILInstruction* inst) {
    auto* leave = as<Leave>(inst);
    return leave != nullptr && leave->is_leaving_function() && is<Nop>(leave->value());
}

}  // namespace

void LoopDetection::run(Block* block, transforms::BlockTransformContext& context) {
    context_ = &context;
    // LoopDetection runs early enough so that block should still be in the original container at this point.
    assert(block->parent() == context.control_flow_graph->container());
    current_block_container_ = context.control_flow_graph->container();

    // Because this is a post-order block transform, we can assume that any nested loops within this loop have already been
    // constructed.

    if (auto* switch_inst = as<SwitchInstruction>(block->instructions.last())) {
        // Switch instructions support "break;" just like loops
        detect_switch_body(block, switch_inst);
    }

    ControlFlowNode* h = context.control_flow_node;  // CFG node for our potential loop head
    assert(h->user_data == block);

    std::optional<NodeList> loop;
    for (ControlFlowNode* t : h->predecessors) {
        if (h->dominates(t)) {
            // h->t is a back edge, and h is a loop header
            // Add the natural loop of t->h to the loop.

            // Definitions:
            // * A back edge is an edge t->h so that h dominates t.
            // * The natural loop of the back edge is the smallest set of nodes that includes the back edge and has no predecessors
            //   outside the set except for the predecessor of the header.

            if (!loop) {
                loop.emplace();
                loop->push_back(h);
                // Mark loop header as visited so that the pre-order traversal stops at the loop header.
                h->visited = true;
            }
            t->traverse_pre_order([](ControlFlowNode* n) { return n->predecessors; }, [&](ControlFlowNode* n) { loop->push_back(n); });
        }
    }
    if (loop) {
        Block* head_block = h->user_data;
        context.step("Construct loop with head " + head_block->label(), head_block);
        // loop now is the union of all natural loops with loop head h.

        // Ensure any block included into nested loops is also considered part of this loop:
        include_nested_containers(*loop);
        // Try to extend the loop to reduce the number of exit points:
        ControlFlowNode* exit_point = nullptr;
        extend_loop(h, *loop, &exit_point);

        // Sort blocks in the loop in reverse post-order to make the output look a bit nicer.
        // (if the loop doesn't contain nested loops, this is a topological sort)
        std::sort(loop->begin(), loop->end(), [](ControlFlowNode* a, ControlFlowNode* b) { return a->post_order_number > b->post_order_number; });
        assert((*loop)[0] == h);
        for (ControlFlowNode* node : *loop) {
            node->visited = false;  // reset visited flag so that we can find outer loops
            assert(h->dominates(node));
        }
        construct_loop(*loop, exit_point);
    }
}

// For each block in the input loop that is the head of a nested loop or switch, include all blocks from the nested container into
// the loop.
//
// This ensures that all blocks that were included into inner loops are also included into the outer loop, thus keeping our loops
// well-nested. (More details: https://github.com/icsharpcode/ILSpy/issues/915)
//
// Pre+Post-Condition: node.Visited iff loop.Contains(node)
void LoopDetection::include_nested_containers(NodeList& loop) {
    for (std::size_t i = 0; i < loop.size(); i++) include_block(loop[i]->user_data, loop);
}

void LoopDetection::include_block(Block* block, NodeList& loop) {
    for (int k = 0; k < block->instructions.count(); k++) {
        auto* nested_container = as<BlockContainer>(block->instructions[k]);
        if (nested_container == nullptr) continue;
        // Just in case the block has multiple nested containers (e.g. due to loop and switch), also check the entry point:
        include_block(nested_container->entry_point(), loop);
        // Use normal processing for all non-entry-point blocks
        // (the entry-point itself doesn't have a CFG node, because it's newly created by this transform)
        for (int i = 1; i < nested_container->blocks.count(); i++) {
            ControlFlowNode* node = context_->control_flow_graph->get_node(nested_container->blocks[i]);
            assert(loop[0]->dominates(node));
            if (!node->visited) {
                node->visited = true;
                loop.push_back(node);
                // note: this block will be re-visited when the "i < loop.Count" gets around to the new entry
            }
        }
    }
}

// Given a natural loop, add additional CFG nodes to the loop in order to reduce the number of exit points out of the loop.
// We do this because C# only allows reaching a single exit point (with 'break' statements or when the loop condition evaluates to
// false), so we'd have to introduce 'goto' statements for any additional exit points.
//
// Definition:
// A "reachable exit" is a branch/leave target that is reachable from the loop, but not dominated by the loop head. A reachable exit
// may or may not have a corresponding CFG node (depending on whether it is a block in the current block container).
//   -> reachable exits are leaving the code region dominated by the loop
//
// Definition:
// A loop "exit point" is a CFG node that is not itself part of the loop, but has at least one predecessor which is part of the loop.
//   -> exit points are leaving the loop itself
//
// Nodes can only be added to the loop if they are dominated by the loop head.
// When adding a node to the loop, we must also add all of that node's predecessors to the loop.
// (this ensures that the loop keeps its single entry point)
//
// Goal: If possible, find a set of nodes that can be added to the loop so that there remains only a single exit point.
// Add as little code as possible to the loop to reach this goal.
//
// This means we need to partition the set of nodes dominated by the loop entry point into two sets (in-loop and out-of-loop).
// Constraints:
//  * the loop head itself is in-loop
//  * there must not be any edge from an out-of-loop node to an in-loop node
//    -> all predecessors of in-loop nodes are also in-loop
//    -> all nodes in a cycle are part of the same partition
// Optimize:
//  * use only a single exit point if at all possible
//  * minimize the amount of code in the in-loop partition (thus: maximize the amount of code in the out-of-loop partition)
//
// Observations:
//  * If a node is in-loop, so are all its ancestors in the dominator tree (up to the loop entry point)
//  * If there are no exits reachable from a node (i.e. all paths from that node lead to a return/throw instruction), it is valid to
//    put the group of nodes dominated by that node into either partition independently of any other nodes except for the ancestors
//    in the dominator tree. (exception: the loop head itself must always be in-loop)
//
// There are two different cases we need to consider:
// 1) There are no exits reachable at all from the loop head.
//    ->  it is possible to create a loop with zero exit points by adding all nodes dominated by the loop to the loop.
//    -> the only way to exit the loop is by "return;" or "throw;"
// 2) There are some exits reachable from the loop head.
//
// In case 1, we can pick a single exit point freely by picking any node that has no reachable exits (other than the loop head).
// All nodes dominated by the exit point are out-of-loop, all other nodes are in-loop.
// See pick_exit_point() for the heuristic that picks the exit point in this case.
//
// In case 2, we need to pick our exit point so that all paths from the loop head to the reachable exits run through that exit point.
//
// This is a form of postdominance where the reachable exits are considered exit nodes, while "return;" or "throw;" instructions are
// not considered exit nodes.
//
// Using this form of postdominance, we are looking for an exit point that post-dominates all nodes in the natural loop.
// --> a common ancestor in post-dominator tree.
// To minimize the amount of code in-loop, we pick the lowest common ancestor.
// All nodes dominated by the exit point are out-of-loop, all other nodes are in-loop.
// (using normal dominance as in case 1, not post-dominance!)
//
// If it is impossible to use a single exit point for the loop, the lowest common ancestor will be the fake "exit node" used by the
// post-dominance analysis. In this case, we fall back to the old heuristic algorithm.
//
// Requires and maintains the invariant that a node is marked as visited iff it is contained in the loop.
void LoopDetection::extend_loop(ControlFlowNode* loop_head, NodeList& loop, ControlFlowNode** exit_point) {
    *exit_point = find_exit_point(loop_head, loop);
    assert(std::find(loop.begin(), loop.end(), *exit_point) == loop.end());
    if (*exit_point != nullptr) {
        // Either we are in case 1 and just picked an exit that maximizes the amount of code outside the loop, or we are in case 2 and
        // found an exit point via post-dominance.
        // Note that if exitPoint == NoExitPoint, we end up adding all dominated blocks to the loop.
        ControlFlowNode* ep = *exit_point;
        util::pre_order(
            loop_head, [&](ControlFlowNode* n) { return dominator_tree_children(n, ep); },
            [&](ControlFlowNode* node) {
                if (!node->visited) {
                    node->visited = true;
                    loop.push_back(node);
                }
            });
        // The loop/switch can only be entered through the entry point.
        if (is_switch_) {
            // In the case of a switch, false positives in the "continue;" detection logic can lead to falsely excludes some blocks
            // from the body. Fix that by including all predecessors of included blocks.
            assert(loop[0] == loop_head);
            for (std::size_t i = 1; i < loop.size(); i++) {
                for (ControlFlowNode* p : loop[i]->predecessors) {
                    if (!p->visited) {
                        p->visited = true;
                        loop.push_back(p);
                    }
                }
            }
        }
    } else {
        // We are in case 2, but could not find a suitable exit point.
        // Heuristically try to minimize the number of exit points
        // (but we'll always end up with more than 1 exit and will require goto statements).
        extend_loop_heuristic(loop_head, loop, loop_head);
    }
}

// Finds a suitable single exit point for the specified loop.
// 1) If a suitable exit point was found: the control flow block that should be reached when breaking from the loop
// 2) If the loop should not have any exit point (extend by all dominated blocks): NoExitPoint
// 3) otherwise (exit point unknown, heuristically extend loop): null
// This method must not write to the Visited flags on the CFG.
ControlFlowNode* LoopDetection::find_exit_point(ControlFlowNode* loop_head, const NodeList& natural_loop) {
    bool has_reachable_exit_ = has_reachable_exit(loop_head);
    if (!has_reachable_exit_) {
        // Case 1:
        // There are no nodes n so that loopHead dominates a predecessor of n but not n itself
        // -> we could build a loop with zero exit points.
        Branch* exit_branch = nullptr;
        if (is_possible_foreach_loop(loop_head->user_data, &exit_branch)) {
            if (exit_branch != nullptr) {
                // let's see if the target of the exit branch is a suitable exit point
                ControlFlowNode* cfg_node = nullptr;
                for (ControlFlowNode* n : loop_head->successors) {
                    if (n->user_data == exit_branch->target_block()) {
                        cfg_node = n;
                        break;
                    }
                }
                if (cfg_node != nullptr && loop_head->dominates(cfg_node) && !context_->control_flow_graph->has_reachable_exit(cfg_node)) {
                    return cfg_node;
                }
            }
            return no_exit_point();
        }
        ControlFlowNode* exit_point = nullptr;
        int exit_point_il_offset = -1;
        consider_return_as_exit_point(loop_head->user_data, &exit_point, &exit_point_il_offset);
        for (ControlFlowNode* node : loop_head->dominator_tree_children) pick_exit_point(node, &exit_point, &exit_point_il_offset);
        return exit_point;
    } else {
        // Case 2:
        // We need to pick our exit point so that all paths from the loop head to the reachable exits run through that exit point.
        const NodeList& cfg = context_->control_flow_graph->nodes();
        int exit_node_arity = 0;
        ReverseCfg rev_cfg = prepare_reverse_cfg(loop_head, &exit_node_arity);
        ControlFlowNode* common_ancestor = rev_cfg.nodes[static_cast<std::size_t>(loop_head->user_index)];
        assert(common_ancestor->is_reachable());
        for (ControlFlowNode* cfg_node : natural_loop) {
            ControlFlowNode* rev_node = rev_cfg.nodes[static_cast<std::size_t>(cfg_node->user_index)];
            if (rev_node->is_reachable()) {
                common_ancestor = flow_analysis::dominance::find_common_dominator(common_ancestor, rev_node);
            }
        }
        // All paths from within the loop to a reachable exit run through 'commonAncestor'.
        // However, this doesn't mean that 'commonAncestor' is valid as an exit point.
        // We walk up the post-dominator tree until we've got a valid exit point:
        while (common_ancestor->user_index >= 0) {
            ControlFlowNode* exit_point = cfg[static_cast<std::size_t>(common_ancestor->user_index)];
            assert(exit_point->visited == (std::find(natural_loop.begin(), natural_loop.end(), exit_point) != natural_loop.end()));
            // It's possible that 'commonAncestor' is itself part of the natural loop.
            // If so, it's not a valid exit point.
            if (!exit_point->visited && validate_exit_point(loop_head, exit_point)) {
                // we found an exit point
                return exit_point;
            }
            common_ancestor = common_ancestor->immediate_dominator;
        }
        // least common post-dominator is the artificial exit node
        // This means we're in one of two cases:
        // * The loop might have multiple exit points.
        //     -> we should return null
        // * The loop has a single exit point that wasn't considered during post-dominance analysis.
        //        (which means the single exit isn't dominated by the loop head)
        //     -> we should return NoExitPoint so that all code dominated by the loop head is included into the loop
        if (exit_node_arity > 1) return nullptr;

        // Exit node is on the very edge of the tree, and isn't important for determining inclusion
        // Still necessary for switch detection to insert correct leave statements
        if (exit_node_arity == 1 && is_switch_) {
            NodeList distinct;
            for (ControlFlowNode* target : loop_context_->get_break_targets(loop_head))
                if (std::find(distinct.begin(), distinct.end(), target) == distinct.end()) distinct.push_back(target);
            if (distinct.size() != 1) throw UsageError("Sequence does not contain exactly one element");
            return distinct.front();
        }

        // If exitNodeArity == 0, we should maybe look test if our exits out of the block container are all compatible?
        // but I don't think it hurts to have a bit too much code inside the loop in this rare case.
        return no_exit_point();
    }
}

// Validates an exit point.
//
// An exit point is invalid iff there is a node reachable from the exit point that is dominated by the loop head, but not by the
// exit point. (i.e. this method returns false iff the exit point's dominance frontier contains a node dominated by the loop head.
// but we implement this the slow way because we don't have dominance frontiers precomputed)
//
// We need this because it's possible that there's a return block (thus reverse-unreachable node ignored by post-dominance) that is
// reachable both directly from the loop, and from the exit point.
bool LoopDetection::validate_exit_point(ControlFlowNode* loop_head, ControlFlowNode* exit_point) {
    ControlFlowGraph& cfg = *context_->control_flow_graph;
    std::function<bool(ControlFlowNode*)> is_valid = [&](ControlFlowNode* node) {
        if (!cfg.has_reachable_exit(node)) {
            // Optimization: if the dominance frontier is empty, we don't need to check every node.
            return true;
        }
        for (ControlFlowNode* succ : node->successors) {
            if (loop_head != succ && loop_head->dominates(succ) && !exit_point->dominates(succ)) return false;
        }
        for (ControlFlowNode* child : node->dominator_tree_children) {
            if (!is_valid(child)) return false;
        }
        return true;
    };
    return is_valid(exit_point);
}

// Extension of ControlFlowGraph.HasReachableExit
// Uses loopContext.GetBreakTargets().Any() when analyzing switches to avoid classifying continue blocks as reachable exits.
bool LoopDetection::has_reachable_exit(ControlFlowNode* node) {
    return is_switch_ ? !loop_context_->get_break_targets(node).empty() : context_->control_flow_graph->has_reachable_exit(node);
}

// Returns the children in a loop dominator tree, with an optional exit point
// Avoids returning continue statements when analysing switches (because increment blocks can be dominated)
LoopDetection::NodeList LoopDetection::dominator_tree_children(ControlFlowNode* n, ControlFlowNode* exit_point) {
    NodeList result;
    for (ControlFlowNode* c : n->dominator_tree_children)
        if (c != exit_point && (!is_switch_ || !loop_context_->match_continue(c))) result.push_back(c);
    return result;
}

// Pick exit point by picking any node that has no reachable exits.
//
// In the common case where the code was compiled with a compiler that emits IL code in source order (like the C# compiler), we can
// find the "real" exit point by simply picking the block with the highest IL offset.
// So let's do that instead of maximizing amount of code.
// This method must not write to the Visited flags on the CFG.
void LoopDetection::pick_exit_point(ControlFlowNode* node, ControlFlowNode** exit_point, int* exit_point_il_offset) {
    if (is_switch_ && loop_context_->match_continue(node)) return;

    Block* block = node->user_data;
    if (block->start_il_offset() > *exit_point_il_offset && !has_reachable_exit(node) && node->user_data->parent() == current_block_container_) {
        // HasReachableExit(node) == false
        // -> there are no nodes n so that `node` dominates a predecessor of n but not n itself
        // -> there is no control flow out of `node` back into the loop, so it's usable as exit point

        // Additionally, we require that the block wasn't already moved into a nested loop,
        // since there's no way to jump into the middle of that loop when we need to exit.
        // NB: this is the only reason why we detect nested loops before outer loops:
        // If we detected the outer loop first, the outer loop might pick an exit point
        // that prevents us from finding a nice exit for the inner loops, causing
        // unnecessary gotos.
        *exit_point = node;
        *exit_point_il_offset = block->start_il_offset();
        return;  // don't visit children, they are likely to have even later IL offsets and we'd end up moving almost all of the code
                 // into the loop.
    }
    consider_return_as_exit_point(block, exit_point, exit_point_il_offset);
    for (ControlFlowNode* child : node->dominator_tree_children) pick_exit_point(child, exit_point, exit_point_il_offset);
}

void LoopDetection::consider_return_as_exit_point(Block* block, ControlFlowNode** exit_point, int* exit_point_il_offset) {
    // It's possible that the real exit point of the loop is a "return;" that has been combined (by ControlFlowSimplification) with
    // the condition block.
    ILInstruction* ignored_condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!block->match_if_at_end_of_block(&ignored_condition, &true_inst, &false_inst)) return;
    if (true_inst->start_il_offset() > *exit_point_il_offset && is_return_leave(true_inst)) {
        // By using NoExitPoint, everything (including the "return;") becomes part of the loop body
        // Then DetectExitPoint will move the "return;" out of the loop body.
        *exit_point = no_exit_point();
        *exit_point_il_offset = true_inst->start_il_offset();
    }
    if (false_inst->start_il_offset() > *exit_point_il_offset && is_return_leave(false_inst)) {
        *exit_point = no_exit_point();
        *exit_point_il_offset = false_inst->start_il_offset();
    }
}

// Constructs a new control flow graph.
// Each node cfg[i] has a corresponding node rev[i].
// Edges are only created for nodes dominated by loopHead, and are in reverse from their direction in the primary CFG.
// An artificial exit node is used for edges that leave the set of nodes dominated by loopHead, or that leave the block Container.
LoopDetection::ReverseCfg LoopDetection::prepare_reverse_cfg(ControlFlowNode* loop_head, int* exit_node_arity) {
    const NodeList& cfg = context_->control_flow_graph->nodes();
    ReverseCfg result;
    result.nodes.resize(cfg.size() + 1);
    for (std::size_t i = 0; i < cfg.size(); i++) {
        auto node = std::make_unique<ControlFlowNode>();
        node->user_index = static_cast<int>(i);
        node->user_data = cfg[i]->user_data;
        result.nodes[i] = node.get();
        result.storage.push_back(std::move(node));
    }
    ControlFlowNode* node_treated_as_exit_node = nullptr;
    bool multiple_nodes_treated_as_exit_nodes = false;
    auto exit_node_owner = std::make_unique<ControlFlowNode>();
    ControlFlowNode* exit_node = exit_node_owner.get();
    exit_node->user_index = -1;
    result.nodes[cfg.size()] = exit_node;
    result.storage.push_back(std::move(exit_node_owner));
    for (std::size_t i = 0; i < cfg.size(); i++) {
        if (!loop_head->dominates(cfg[i]) || (is_switch_ && cfg[i] != loop_head && loop_context_->match_continue(cfg[i]))) continue;

        // Add reverse edges for all edges in cfg
        for (ControlFlowNode* succ : cfg[i]->successors) {
            // edges to outer loops still count as exits (labelled continue not implemented)
            if (is_switch_ && loop_context_->match_continue(succ, 1)) continue;

            if (loop_head->dominates(succ)) {
                result.nodes[static_cast<std::size_t>(succ->user_index)]->add_edge_to(result.nodes[i]);
            } else {
                if (node_treated_as_exit_node == nullptr) node_treated_as_exit_node = succ;
                if (node_treated_as_exit_node != succ) multiple_nodes_treated_as_exit_nodes = true;
                exit_node->add_edge_to(result.nodes[i]);
            }
        }
        if (context_->control_flow_graph->has_direct_exit_out_of_container(cfg[i])) {
            exit_node->add_edge_to(result.nodes[i]);
        }
    }
    if (multiple_nodes_treated_as_exit_nodes)
        *exit_node_arity = 2;  // more than 1
    else if (node_treated_as_exit_node != nullptr)
        *exit_node_arity = 1;
    else
        *exit_node_arity = 0;
    flow_analysis::dominance::compute_dominance(exit_node, context_->cancellation_token);
    return result;
}

bool LoopDetection::is_possible_foreach_loop(Block* loop_head, Branch** exit_branch) {
    *exit_branch = nullptr;
    auto* container = static_cast<BlockContainer*>(loop_head->parent());
    if (!(container->slot_info() == &TryInstruction::try_block_slot && is<TryFinally>(container->parent()))) return false;
    if (loop_head->instructions.count() != 2) return false;
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!loop_head->instructions[0]->match_if_instruction(&condition, &true_inst)) return false;
    ILInstruction* false_inst = loop_head->instructions[1];
    ILInstruction* arg = nullptr;
    while (condition->match_logic_not(&arg)) {
        condition = arg;
        std::swap(true_inst, false_inst);
    }
    auto* call = as<CallInstruction>(condition);
    if (!(call != nullptr && call->method->name() == "MoveNext")) return false;
    ILVariable* enumerator_var = nullptr;
    if (!(call->arguments().count() == 1 && call->arguments()[0]->match_ld_loc_ref(&enumerator_var))) return false;
    *exit_branch = as<Branch>(false_inst);

    // Check that loopHead is entry-point of try-block:
    Block* entry_point = container->entry_point();
    Block* target_block = nullptr;
    while (entry_point->incoming_edge_count() == 1 && entry_point->instructions.count() == 1 && entry_point->instructions[0]->match_branch(&target_block)) {
        // skip blocks that only branch to another block
        entry_point = target_block;
    }
    return entry_point == loop_head;
}

// This function implements a heuristic algorithm that tries to reduce the number of exit points. It is only used as fall-back when it
// is impossible to use a single exit point.
//
// This heuristic loop extension algorithm traverses the loop head's dominator tree in pre-order.
// For each candidate node, we detect whether adding it to the loop reduces the number of exit points.
// If it does, the candidate is added to the loop.
//
// Adding a node to the loop has two effects on the the number of exit points:
// * exit points that were added to the loop are no longer exit points, thus reducing the total number of exit points
// * successors of the newly added nodes might be new, additional exit points
//
// Requires and maintains the invariant that a node is marked as visited iff it is contained in the loop.
void LoopDetection::extend_loop_heuristic(ControlFlowNode* loop_head, NodeList& loop, ControlFlowNode* candidate) {
    assert(candidate->visited == (std::find(loop.begin(), loop.end(), candidate) != loop.end()));
    if (!candidate->visited) {
        // This node not yet part of the loop, but might be added
        NodeList additional_nodes;
        // Find additionalNodes nodes and mark them as visited.
        candidate->traverse_pre_order([](ControlFlowNode* n) { return n->predecessors; }, [&](ControlFlowNode* n) { additional_nodes.push_back(n); });
        // This means Visited now represents the candidate extended loop.
        // Determine new exit points that are reachable from the additional nodes
        // (note: some of these might have previously been exit points, too)
        NodeList new_exit_points;
        for (ControlFlowNode* n : additional_nodes) {
            for (ControlFlowNode* successor : n->successors) {
                if (!successor->visited && std::find(new_exit_points.begin(), new_exit_points.end(), successor) == new_exit_points.end())
                    new_exit_points.push_back(successor);
            }
        }
        // Make visited represent the unextended loop, so that we can measure the exit points in the old state.
        for (ControlFlowNode* node : additional_nodes) node->visited = false;
        // Measure number of added and removed exit points
        int removed_exit_points = static_cast<int>(std::count_if(additional_nodes.begin(), additional_nodes.end(), is_exit_point));
        int added_exit_points = static_cast<int>(std::count_if(new_exit_points.begin(), new_exit_points.end(), [](ControlFlowNode* n) { return !is_exit_point(n); }));
        if (removed_exit_points > added_exit_points) {
            // We can reduce the number of exit points by adding the candidate node to the loop.
            candidate->traverse_pre_order([](ControlFlowNode* n) { return n->predecessors; }, [&](ControlFlowNode* n) { loop.push_back(n); });
        }
    }
    // Pre-order traversal of dominator tree
    for (ControlFlowNode* node : candidate->dominator_tree_children) extend_loop_heuristic(loop_head, loop, node);
}

// Gets whether 'node' is an exit point for the loop marked by the Visited flag.
bool LoopDetection::is_exit_point(ControlFlowNode* node) {
    if (node->visited) return false;  // nodes in the loop are not exit points
    for (ControlFlowNode* pred : node->predecessors) {
        if (pred->visited) return true;
    }
    return false;
}

// Move the blocks associated with the loop into a new block container.
void LoopDetection::construct_loop(NodeList& loop, ControlFlowNode* exit_point) {
    Block* old_entry_point = loop[0]->user_data;
    Block* exit_target_block = exit_point != nullptr ? exit_point->user_data : nullptr;

    BlockContainer* loop_container = make<BlockContainer>(ContainerKind::Loop);
    Block* new_entry_point = make<Block>();
    loop_container->blocks.add(new_entry_point);
    // Move contents of oldEntryPoint to newEntryPoint
    // (we can't move the block itself because it might be the target of branch instructions outside the loop)
    new_entry_point->instructions.replace_list(old_entry_point->instructions);
    new_entry_point->add_il_range(*old_entry_point);
    old_entry_point->instructions.replace_list(std::vector<ILInstruction*>{loop_container});
    if (exit_target_block != nullptr) old_entry_point->instructions.add(make<Branch>(exit_target_block));

    loop_container->add_il_range(*new_entry_point);
    move_blocks_into_container(loop, loop_container);

    // Rewrite branches within the loop from oldEntryPoint to newEntryPoint:
    for (ILInstruction* descendant : loop_container->descendants()) {
        auto* branch = as<Branch>(descendant);
        if (branch == nullptr) continue;
        if (branch->target_block() == old_entry_point) {
            branch->set_target_block(new_entry_point);
        } else if (branch->target_block() == exit_target_block) {
            branch->replace_with(with_il_range(make<Leave>(loop_container), branch));
        }
    }
}

void LoopDetection::move_blocks_into_container(NodeList& loop, BlockContainer* loop_container) {
    // Move other blocks into the loop body: they're all dominated by the loop header, and thus cannot be the target of branch
    // instructions outside the loop.
    for (std::size_t i = 1; i < loop.size(); i++) {
        Block* block = loop[i]->user_data;
        // some blocks might already be in use by nested loops that were detected earlier;
        // don't move those (they'll be implicitly moved when the block containing the nested loop container is moved).
        if (block->parent() == current_block_container_) {
            assert(block->child_index() != 0);
            int old_child_index = block->child_index();
            loop_container->blocks.add(block);
            current_block_container_->blocks.swap_remove_at(old_child_index);
        }
    }
    for (std::size_t i = 1; i < loop.size(); i++) {
        // Verify that we moved all loop blocks into the loop container.
        // If we wanted to move any blocks already in use by a nested loop, this means we check that the whole nested loop got moved.
        assert(loop[i]->user_data->is_descendant_of(loop_container));
    }
}

void LoopDetection::detect_switch_body(Block* block, SwitchInstruction* switch_inst) {
    assert(block->instructions.last() == switch_inst);
    ControlFlowNode* h = context_->control_flow_node;  // CFG node for our switch head
    assert(h->user_data == block);

    is_switch_ = true;
    loop_context_ = std::make_unique<SwitchDetection::LoopContext>(*context_->control_flow_graph, h);

    NodeList nodes_in_switch;
    nodes_in_switch.push_back(h);
    h->visited = true;
    ControlFlowNode* exit_point = nullptr;
    extend_loop(h, nodes_in_switch, &exit_point);
    if (exit_point != nullptr && h->dominates(exit_point) && exit_point->predecessors.size() == 1 && !has_reachable_exit(exit_point)) {
        // If the exit point is reachable from just one single "break;", it's better to move the code into the switch.
        // (unlike loops which should not be nested unless necessary, nesting switches makes it clearer in which cases a piece of
        // code is reachable)
        util::pre_order(
            exit_point, [](ControlFlowNode* p) { return p->dominator_tree_children; }, [&](ControlFlowNode* p) { nodes_in_switch.push_back(p); });
        for (ControlFlowNode* node : nodes_in_switch) node->visited = true;
        exit_point = nullptr;
    }

    context_->step("Create BlockContainer for switch", switch_inst);
    // Sort blocks in the loop in reverse post-order to make the output look a bit nicer.
    // (if the loop doesn't contain nested loops, this is a topological sort)
    std::sort(nodes_in_switch.begin(), nodes_in_switch.end(),
              [](ControlFlowNode* a, ControlFlowNode* b) { return a->post_order_number > b->post_order_number; });
    assert(nodes_in_switch[0] == h);
    for (ControlFlowNode* node : nodes_in_switch) {
        node->visited = false;  // reset visited flag so that we can find outer loops
        assert(h->dominates(node));
    }

    BlockContainer* switch_container = make<BlockContainer>(ContainerKind::Switch);
    Block* new_entry_point = make<Block>();
    new_entry_point->add_il_range(*switch_inst);
    switch_container->blocks.add(new_entry_point);
    new_entry_point->instructions.add(switch_inst);
    block->instructions.set(block->instructions.count() - 1, switch_container);

    Block* exit_target_block = exit_point != nullptr ? exit_point->user_data : nullptr;
    if (exit_target_block != nullptr) {
        block->instructions.add(make<Branch>(exit_target_block));
    }

    switch_container->add_il_range(*new_entry_point);
    move_blocks_into_container(nodes_in_switch, switch_container);

    // Rewrite branches within the loop from oldEntryPoint to newEntryPoint:
    for (ILInstruction* descendant : switch_container->descendants()) {
        auto* branch = as<Branch>(descendant);
        if (branch == nullptr) continue;
        if (branch->target_block() == exit_target_block) {
            branch->replace_with(with_il_range(make<Leave>(switch_container), branch));
        }
    }

    is_switch_ = false;
}

}  // namespace cb::dotnet::decompiler::il::control_flow
