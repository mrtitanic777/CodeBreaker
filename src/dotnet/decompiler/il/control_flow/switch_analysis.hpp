// dotnet/decompiler/il/control_flow/switch_analysis.hpp -- reconstructs a C# switch from the code the compiler wrote for it.
//
// C# switch statements are not necessarily compiled into IL switch instructions: when the label values are not contiguous, the
// compiler generates if statements similar to a binary search. This analysis reads such a tree of conditions (and IL switches) at
// the end of a block and finds the sections: which set of values of one variable leads to which instruction. It expects basic blocks,
// not extended basic blocks.
//
//     SwitchAnalysis analysis;
//     if (analysis.analyze_block(block)) { for (auto& [labels, inst] : analysis.sections) ...; analysis.switch_variable(); }
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: SwitchAnalysis, from ControlFlow/SwitchAnalysis.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

class SwitchAnalysis {
public:
    // The variable to be used as the argument of the switch instruction.
    ILVariable* switch_variable() const { return switch_var_; }
    // Whether at least one of the analyzed blocks contained an IL switch instruction.
    bool contains_il_switch() const { return contains_il_switch_; }
    // The sections found by the previous analyze_block call.
    std::vector<std::pair<util::LongSet, ILInstruction*>> sections;
    // Blocks that can be deleted if the tail of the initial block is replaced with a switch instruction.
    std::vector<Block*> inner_blocks;
    Block* root_block() const { return root_block_; }
    // Whether to allow unreachable cases in switch instructions.
    bool allow_unreachable_cases = false;

    // Analyzes the last two statements in the block and sees if they can be turned into a switch instruction.
    bool analyze_block(Block* block);

    // The set of values x for which `x compared with val` is true (for the comparison kind and sign).
    static util::LongSet make_set_where_comparison_is_true(ComparisonKind kind, std::int64_t val, Sign sign);

private:
    bool analyze_block(Block* block, util::LongSet input_values, bool tail_only);
    bool analyze_switch(SwitchInstruction* inst, const util::LongSet& input_values);
    // Adds a section; a branch (or a leave of a container) to the target of an existing section is merged into that section.
    void add_section(util::LongSet values, ILInstruction* inst);
    bool match_switch_var(ILInstruction* inst);
    bool match_switch_var(ILInstruction* inst, std::int64_t* sub);
    // The set of values of the interesting variable for which the condition is true.
    bool analyze_condition(ILInstruction* condition, util::LongSet* true_values);
    static util::LongSet make_greater_than_or_equal_set(std::int64_t val, Sign sign);
    static util::LongSet make_less_than_or_equal_set(std::int64_t val, Sign sign);

    // The variable that represents the switch expression; null while analyzing the first block.
    ILVariable* switch_var_ = nullptr;
    bool contains_il_switch_ = false;
    Block* root_block_ = nullptr;
    // Used to de-duplicate sections with a branch (or a value-less leave): the index into `sections` by target.
    std::unordered_map<Block*, int> target_block_to_section_index_;
    std::unordered_map<BlockContainer*, int> target_container_to_section_index_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow
