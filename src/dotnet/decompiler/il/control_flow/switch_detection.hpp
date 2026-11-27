// dotnet/decompiler/il/control_flow/switch_detection.hpp -- turns sequences of conditional branches that test one integer value into
// an ILAst switch instruction.
//
// C# switch statements are not necessarily compiled into IL switch instructions (e.g. when the integer values are not contiguous).
// This finds such sequences (with SwitchAnalysis) and simplifies them into a switch instruction, which like C#'s does not require
// contiguous values; it also combines the sections of an existing IL switch that branch to one target, and marks the compiler's
// implicit default case of a switch expression.
//
//     SwitchDetection transform;
//     transform.run(function, context);
//     SwitchDetection::simplify_switch_instruction(block, context);      // what ControlFlowSimplification runs per block
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: SwitchDetection and its LoopContext, from ControlFlow/SwitchDetection.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The uses of HighLevelLoopTransform,
// SwitchOnStringTransform and NullableLiftingTransform wait for those headers.
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/flow_analysis/control_flow_node.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/control_flow/switch_analysis.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class SwitchDetection final : public transforms::IILTransform {
public:
    // When detecting a switch, it is important to tell apart the branches that will eventually decompile to `continue;` statements.
    // A LoopContext is made for a node and its dominator tree: for a branch to be a `continue;` it must be contained in the target
    // loop. It also gives the depth of the loop a `continue;` targets relative to the context node, to avoid (or eventually
    // support) labelled continues to outer loops.
    class LoopContext {
    public:
        LoopContext(ControlFlowGraph& cfg, ControlFlowNode* context_node);

        bool match_continue(ControlFlowNode* node) const;
        bool match_continue(ControlFlowNode* node, int depth) const;
        bool match_continue(ControlFlowNode* node, int* depth) const;
        int get_continue_depth(ControlFlowNode* node) const;

        // All potential targets for `break;` statements from a dominator tree, assuming the tree must be exited by break or continue:
        // the successors, not in the tree, of the nodes of the dominator tree (leaving out the continue nodes). A node is returned
        // once for each outgoing edge. Labelled continue statements (depth > 1) count as break targets.
        std::vector<ControlFlowNode*> get_break_targets(ControlFlowNode* dominator) const;

    private:
        static ControlFlowNode* find_continue(ControlFlowNode* loop_head);

        std::unordered_map<ControlFlowNode*, int> continue_depth_;
    };

    void run(ILFunction* function, transforms::ILTransformContext& context) override;
    std::string name() const override { return "SwitchDetection"; }

    // switch (...) { default: throw helper-call }  ->  the helper call's throw (for the default case of a switch expression).
    static void inline_switch_expression_default_case_throw_helper(Block* block, transforms::ILTransformContext& context);
    // Whether the block is only the compiler-generated default case of a switch expression: a throw helper call followed by the
    // unreachable branch or leave, or a directly inlined `throw new SwitchExpressionException(...)`.
    static bool is_switch_expression_throw_helper_block(Block* block);
    // Combines the sections that branch to the same target, moves an offset into the labels, and sorts the sections.
    static void simplify_switch_instruction(Block* block, transforms::ILTransformContext& context);

private:
    void process_block(Block* block, bool* block_container_needs_cleanup);
    static void sort_switch_sections(SwitchInstruction* sw, transforms::ILTransformContext& context);
    static void adjust_labels(SwitchInstruction* sw, transforms::ILTransformContext& context);
    // Whether a switch statement is preferred over an if statement.
    bool use_csharp_switch();
    bool match_roslyn_switch_on_string();
    // Builds the control flow graph of the current container (if necessary), sets loop_context_ and returns the nodes of the inner
    // flow and of the case blocks of the potential switch.
    std::pair<std::vector<ControlFlowNode*>, std::vector<ControlFlowNode*>> analyze_control_flow();
    // Whether the switch can be built without any goto.
    bool switch_uses_goto(const std::vector<ControlFlowNode*>& flow_nodes, const std::vector<ControlFlowNode*>& case_nodes,
                          Block** break_block);
    // Adds the null case of a switch on a nullable to the flow nodes (some of SwitchOnNullableTransform's analysis).
    void add_null_case(std::vector<ControlFlowNode*>& flow_nodes, std::vector<ControlFlowNode*>& case_nodes);
    static bool match_switch_expression_throw_helper_call(ILInstruction* inst, ILInstruction** value);
    static bool is_short_circuit(ControlFlowNode* parent, int side);
    static bool is_flow_node(ControlFlowNode* n);
    bool is_single_condition(const std::vector<ControlFlowNode*>& flow_nodes, const std::vector<ControlFlowNode*>& case_nodes);
    static void reset_visited(const std::vector<ControlFlowNode*>& nodes);

    SwitchAnalysis analysis_;
    transforms::ILTransformContext* context_ = nullptr;
    BlockContainer* current_container_ = nullptr;
    std::unique_ptr<ControlFlowGraph> control_flow_graph_;
    std::unique_ptr<LoopContext> loop_context_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
