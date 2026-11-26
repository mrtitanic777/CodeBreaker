// dotnet/decompiler/il/semantic_helper.cpp -- see semantic_helper.hpp.
#include "dotnet/decompiler/il/semantic_helper.hpp"

#include <unordered_set>

#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::il {

InstructionFlags SemanticHelper::combine_branches(InstructionFlags true_flags, InstructionFlags false_flags) {
    // the endpoint of the 'if' is only unreachable if both branches have an unreachable endpoint
    constexpr InstructionFlags combine_with_and = InstructionFlags::EndPointUnreachable;
    return (true_flags & false_flags) | ((true_flags | false_flags) & ~combine_with_and);
}

bool SemanticHelper::is_pure(InstructionFlags inst) {
    // ControlFlow is fine: internal control flow is pure as long as it's not an infinite loop,
    // and infinite loops are impossible without MayBranch.
    constexpr InstructionFlags pure_flags = InstructionFlags::MayReadLocals | InstructionFlags::ControlFlow;
    return !any(inst & ~pure_flags);
}

bool SemanticHelper::may_reorder(ILInstruction* inst1, ILInstruction* inst2) {
    // If both instructions perform an impure action, we cannot reorder them
    if (!is_pure(inst1->flags()) && !is_pure(inst2->flags())) return false;
    // We cannot reorder if inst2 might write what inst1 looks at
    if (inst2_might_write_to_variable_read_by_inst1(inst1, inst2)) return false;
    // and the same in reverse:
    if (inst2_might_write_to_variable_read_by_inst1(inst2, inst1)) return false;
    return true;
}

bool SemanticHelper::inst2_might_write_to_variable_read_by_inst1(ILInstruction* inst1, ILInstruction* inst2) {
    if (!inst1->has_flag(InstructionFlags::MayReadLocals)) {
        // quick exit if inst1 doesn't read any variables
        return false;
    }
    std::unordered_set<ILVariable*> variables;
    for (ILInstruction* inst : inst1->descendants())
        if (auto* load = as<LdLoc>(inst)) variables.insert(load->variable());
    if (inst2->has_flag(InstructionFlags::SideEffect)) {
        // If inst2 might have indirect writes, we cannot reorder with any loads of variables that have their address taken.
        for (ILVariable* v : variables)
            if (v->address_count() > 0) return true;
    }
    for (ILInstruction* inst : inst2->descendants()) {
        if (inst->has_direct_flag(InstructionFlags::MayWriteLocals)) {
            ILVariable* v = dynamic_cast<IInstructionWithVariableOperand*>(inst)->variable();
            if (variables.contains(v)) return true;
        }
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::il
