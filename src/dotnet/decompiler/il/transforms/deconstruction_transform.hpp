// dotnet/decompiler/il/transforms/deconstruction_transform.hpp -- detects that a run of statements is a lowered deconstruction assignment
// (`var (a, b) = expr;`, `(x, (y, z)) = t;`), rooted in a Deconstruct call or in reads of a tuple's elements and including nested
// designations, and folds it into one DeconstructInstruction.
//
//     stloc tuple(call MakeIntIntTuple(ldloc this))             deconstruct {
//     stloc myInt(call op_Implicit(ldfld Item2(ldloca tuple)))     init: <empty>
//     stloc a(ldfld Item1(ldloca tuple))                    =>     deconstruct: match.deconstruct(temp = ldloca tuple) { match(result0 = ...), ... }
//     stloc b(ldloc myInt)                                         conversions: { stloc conv2(call op_Implicit(ldloc result1)) }
//                                                                  assignments: { stloc a(ldloc result0); stloc b(ldloc conv2) } }
//
//     DeconstructionTransform transform;
//     transform.run(block, pos, context);
//
// A nested designation over Deconstruct calls (`var (x, (a, b)) = o;`) chains the calls, with a defensive copy for struct elements; one
// over tuples is lowered to one temporary per nested designation followed by the element reads in depth-first leaf order. The matching
// does not change the block: every rewrite is collected as a delayed action and run once the whole sequence is known to match.
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: DeconstructionTransform, from DeconstructionTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::typesystem {
class TupleType;
}

namespace cb::dotnet::decompiler::il::transforms {

class DeconstructionTransform final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "DeconstructionTransform"; }

private:
    using TupleType = typesystem::TupleType;
    // Upstream's Action<DeconstructInstruction> (a multicast delegate): the actions run in the order they were added.
    using DelayedAction = std::function<void(DeconstructInstruction*)>;
    using DelayedActions = std::vector<DelayedAction>;

    // A matched Deconstruct call: one node of the (possibly nested) deconstruction pattern.
    struct DeconstructionCall {
        const IMethod* method = nullptr;
        // Pattern variable of this match node; null for the root (which gets a fresh temp).
        ILVariable* receiver = nullptr;
        // The out-argument variable per element.
        std::vector<ILVariable*> results;
        // Nested deconstruction per element; null = leaf element.
        std::vector<DeconstructionCall*> nested_calls;
    };

    // A tuple variable being deconstructed: one node of the (possibly nested) designation. Nested nodes are the temporaries a nested
    // tuple designation is lowered to.
    struct TupleNode {
        TupleNode(ILVariable* variable, const TupleType* type);
        ILVariable* variable;
        const TupleType* type;
        // Nested designation per element; null = leaf element.
        std::vector<TupleNode*> nested_elements;
        // Flat leaf index (depth-first) of each element.
        std::vector<int> element_flat_index;
        // Number of element reads of `variable` consumed by the pattern.
        int matched_access_count = 0;
    };

    struct ConversionInfo {
        const IType* input_type = nullptr;
        Conv* conv = nullptr;
    };

    void reset();
    bool transform_deconstruction(Block* block, int pos);
    bool match_deconstruction_sequence(Block* block, int start_pos, int* end_pos, DeconstructionCall** root_call, ILInstruction** root_tested_operand,
                                       std::vector<StLoc*>* conversion_st_locs, DelayedActions* delayed_actions);
    bool inline_deconstruction_initializer(Block* block, int pos);
    bool is_consumable_by_enclosing_deconstruction(Block* block, int pos);
    void match_deconstruction(Block* block, int* pos, DeconstructionCall** root_call, ILInstruction** tested_operand);
    DeconstructionCall* match_deconstruction_call(ILInstruction* inst, ILInstruction** tested_operand);
    void match_nested_deconstructions(Block* block, int* pos, DeconstructionCall* root_call);
    void match_nested_tuple_designations(Block* block, int* pos, const std::unordered_set<ILVariable*>* do_not_nest);
    TupleNode* resolve_tuple_container(ILVariable* container, const IType* container_type);
    void initialize_flat_leaf_indices();
    TupleNode* make_tuple_node(ILVariable* variable, const TupleType* type);
    TupleNode* find_tuple_node(ILVariable* variable) const;
    bool match_conversions(Block* block, int* pos, std::vector<std::pair<ILVariable*, ConversionInfo>>* conversions, std::vector<StLoc*>* conversion_st_locs,
                           DelayedActions* delayed_actions);
    bool match_conversion(ILInstruction* inst, ILInstruction** input_instruction, ILVariable** output_variable, ConversionInfo* info);
    bool match_assignments(Block* block, int* pos, const std::vector<std::pair<ILVariable*, ConversionInfo>>& conversions,
                           const std::vector<StLoc*>& conversion_st_locs, DelayedActions* delayed_actions, bool allow_unrelated_assignments,
                           bool* any_assignments);
    bool match_assignment(ILInstruction* inst, const IType** target_type, ILInstruction** value_inst, DelayedAction* add_assignment);
    int find_index(ILInstruction* inst, DelayedActions* delayed_actions);
    bool is_compatible_implicit_conversion(const IType* target_type, const ConversionInfo& conversion_info);
    MatchInstruction* build_pattern_match(DeconstructionCall* call, ILVariable* match_variable, ILInstruction* tested_operand);
    MatchInstruction* build_tuple_pattern_match(TupleNode* node, ILVariable* match_variable, ILInstruction* tested_operand);

    static bool try_find_enclosing_deconstruction_call(Block* block, int pos, int* enclosing_pos);
    static bool has_enclosing_tuple_designation(Block* block, int pos);
    static bool match_tuple_element_read(ILInstruction* inst, ILVariable** container, const IType** container_type, int* index);
    static bool match_tuple_element_store(ILInstruction* inst, ILVariable** temp, ILVariable** container, const IType** container_type, int* index);
    static bool match_ld_loc_or_ld_loca(ILInstruction* inst, ILVariable** variable);

    StatementTransformContext* context_ = nullptr;
    std::vector<std::pair<ILVariable*, int>> deconstruction_results_lookup_;
    std::vector<std::pair<ILVariable*, TupleNode*>> tuple_nodes_;
    std::vector<ILVariable*> deconstruction_results_;
    TupleNode* tuple_root_ = nullptr;
    bool rooted_in_deconstruct_call_ = false;
    // The nodes of the current attempt (the pointers above and in the delayed actions refer to them).
    std::deque<DeconstructionCall> calls_;
    std::deque<TupleNode> nodes_;

    bool lookup_deconstruction_result(ILVariable* variable, int* index) const;
    void add_deconstruction_result(ILVariable* variable, int index);
};

}  // namespace cb::dotnet::decompiler::il::transforms
