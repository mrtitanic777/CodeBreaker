// dotnet/decompiler/il/semantic_helper.hpp -- what instruction flags say about an instruction's behavior: how the flags of two branches
// combine, whether an instruction is pure, and whether two instructions may swap places.
//
//     InstructionFlags f = SemanticHelper::combine_branches(true_inst->flags(), false_inst->flags());
//     if (SemanticHelper::may_reorder(first, second)) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: SemanticHelper, from SemanticHelper.cs), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::il {

class SemanticHelper {
public:
    // The flags of an `if`: the end point is unreachable only if it is in both branches; every other flag of either branch counts.
    static InstructionFlags combine_branches(InstructionFlags true_flags, InstructionFlags false_flags);

    // Whether an instruction with these flags is pure: no side effects, no exceptions, no branching (reading locals and internal
    // control flow are fine: an infinite loop needs MayBranch).
    static bool is_pure(InstructionFlags inst);

    // Whether the sequence 'inst1; inst2;' may be ordered 'inst2; inst1;'.
    static bool may_reorder(ILInstruction* inst1, ILInstruction* inst2);

private:
    static bool inst2_might_write_to_variable_read_by_inst1(ILInstruction* inst1, ILInstruction* inst2);
};

}  // namespace cb::dotnet::decompiler::il
