// dotnet/decompiler/flow_analysis/dominance.hpp -- the dominator tree of a control flow graph (Cooper, Harvey and Kennedy: "A Simple,
// Fast Dominance Algorithm"), and the nodes with exits that leave their dominated region.
//
//     dominance::compute_dominance(cfg[0], token);
//     util::BitSet exits = dominance::mark_nodes_with_reachable_exits(cfg);
//
// Ported from ILSpy (ICSharpCode.Decompiler.FlowAnalysis: Dominance), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). Upstream's static class is the namespace flow_analysis::dominance.
#pragma once

#include <vector>

#include "dotnet/decompiler/flow_analysis/control_flow_node.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler::flow_analysis::dominance {

// Computes the dominator tree.
//
// Precondition: the dominator tree is not already computed for some nodes reachable from `entry_point` (immediate_dominator and
// dominator_tree_children both unset), and `visited` is false for any node reachable from it.
// Postcondition: a dominator tree is built for all nodes reachable from `entry_point`, and `visited` remains false.
void compute_dominance(ControlFlowNode* entry_point, const util::CancellationToken& cancellation_token = {});

// The common ancestor of a and b in the dominator tree (both must be in the same tree).
ControlFlowNode* find_common_dominator(ControlFlowNode* a, ControlFlowNode* b);

// A set where result[i] is true iff cfg[i] is reachable and some node reachable from cfg[i] is not dominated by cfg[i]. Like "has a
// non-empty dominance frontier", but with non-strict dominance.
// Precondition: dominance was computed for cfg and cfg[i]->user_index == i for all i.
util::BitSet mark_nodes_with_reachable_exits(const std::vector<ControlFlowNode*>& cfg);

}  // namespace cb::dotnet::decompiler::flow_analysis::dominance
