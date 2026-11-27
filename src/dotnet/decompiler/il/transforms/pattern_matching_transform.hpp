// dotnet/decompiler/il/transforms/pattern_matching_transform.hpp -- recognizes C# type patterns (`x is T v`) in the isinst/unbox.any
// code the compiler emits, for reference types and for value types, and turns the test and the store into one `match.type`
// instruction in the if condition; then moves the property tests that follow into the pattern as sub patterns (`x is { P: 1 }`).
//
//     stloc V(isinst T(x))                                if (match.type[T].notnull(V = x)) br trueBlock
//     if (comp.o(ldloc V == ldnull)) br falseBlock  =>    br falseBlock
//     br trueBlock
//
//     PatternMatchingTransform transform;
//     transform.run(function, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: PatternMatchingTransform, from PatternMatchingTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <memory>
#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::control_flow {
class ControlFlowGraph;
}

namespace cb::dotnet::decompiler::il::transforms {

class PatternMatchingTransform final : public IILTransform {
public:
    // The graph is made when the first check needs it (upstream's `ref ControlFlowGraph? cfg`).
    using GraphPtr = std::unique_ptr<control_flow::ControlFlowGraph>;

    void run(ILFunction* function, ILTransformContext& context) override;
    std::string name() const override { return "PatternMatchingTransform"; }

private:
    bool pattern_match_ref_types(Block* block, BlockContainer* container, ILTransformContext& context, GraphPtr& cfg);
    static ILInstruction* detect_property_sub_patterns(MatchInstruction* parent_pattern, ILInstruction* true_inst, ILInstruction* parent_false_inst,
                                                       BlockContainer* container, ILTransformContext& context, GraphPtr& cfg);
    static ILInstruction* detect_property_sub_pattern(MatchInstruction* parent_pattern, Block* block, ILInstruction* parent_false_inst,
                                                      ILTransformContext& context, GraphPtr& cfg);
    static ILInstruction* match_null_check_pattern(Block* block, MatchInstruction* var_pattern, ILInstruction* parent_false_inst,
                                                   ILTransformContext& context);
    static ILInstruction* match_nullable_has_value_check_pattern(Block* block, MatchInstruction* var_pattern, ILInstruction* parent_false_inst,
                                                                 ILTransformContext& context, GraphPtr& cfg);
    static bool property_or_field_access(ILInstruction* operand, ILInstruction** target, const IMember** member);
    static bool match_block_containing_one_condition(Block* block, ILInstruction** condition, ILInstruction** true_inst, ILInstruction** false_inst);
    static bool check_all_uses_dominated_by(ILVariable* v, BlockContainer* container, ILInstruction* true_inst, ILInstruction* store_to_v,
                                            ILInstruction* load_in_null_check, ILTransformContext& context, GraphPtr& cfg);
    bool pattern_match_value_types(Block* block, BlockContainer* container, ILTransformContext& context, GraphPtr& cfg);
    bool match_is_inst_block(Block* block, const IType** type, ILInstruction** tested_operand, ILVariable** tested_variable, const IType** box_type,
                             Block** unbox_block, ILInstruction** false_inst);
    bool match_unbox_block(Block* unbox_block, const IType* type, ILVariable** tested_variable, const IType** box_type, StLoc** store_to_v);
};

}  // namespace cb::dotnet::decompiler::il::transforms
