// dotnet/decompiler/il/transforms/il_extraction.hpp -- the context of "extracting" an instruction out of the ILAst: moving it (and what
// is evaluated before it) into stores to fresh temporary variables, the opposite of inlining. Instructions answer through
// prepare_extract whether extraction through them is possible and what has to move along.
//
//     // inside an instruction's prepare_extract:
//     ctx.register_move_if_necessary(predecessor);       // a predecessor that cannot be reordered moves along
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: ExtractionContext, from ILExtraction.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). The static extract(instruction, ILTransformContext) is ILInstruction::extract.
#pragma once

#include <functional>
#include <vector>

#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"

namespace cb::dotnet::decompiler::il::transforms {

// ICompilation.FindType(StackType, Sign): the type of a stack type (unknown for Unknown, a reference to unknown for Ref).
const IType* find_type_of_stack_type(const typesystem::ICompilation& compilation, StackType stack_type, typesystem::Sign sign = typesystem::Sign::None);

// Context object for the extract operation.
class ExtractionContext {
public:
    // `function` is the nearest function, where the temporaries are registered; `find_type` gives the type of a stack type.
    ExtractionContext(ILFunction* function, std::function<const IType*(StackType)> find_type);

    // The combined flags of all instructions being moved.
    InstructionFlags flags_being_moved = InstructionFlags::None;

    // Moves `predecessor` (evaluated before the instructions being moved) too, so the evaluation order stays unchanged.
    void register_move(ILInstruction* predecessor);
    void register_move_if_necessary(ILInstruction* predecessor);
    // Whether `predecessor`, currently evaluated before the instructions being moved, can stay where it is although the move
    // changes the evaluation order.
    bool can_reorder_with_instructions_being_moved(ILInstruction* predecessor) const;

    // The actions to run when extracting: each replaces its instruction with a load of a fresh temporary and returns the store to the
    // temporary, which goes in at block level.
    const std::vector<std::function<ILInstruction*()>>& move_actions() const { return move_actions_; }
    ILFunction* function() const { return function_; }
    const std::function<const IType*(StackType)>& find_type() const { return find_type_; }

    // Extracts the instruction: it is replaced with a load of a new temporary variable and moved to a store to that variable at block
    // level (with whatever has to move along to keep the evaluation order). Returns the variable, or nullptr if extraction is not
    // possible.
    static ILVariable* extract(ILInstruction* inst_to_extract, ILTransformContext& context);

private:
    ILFunction* function_;
    std::function<const IType*(StackType)> find_type_;
    std::vector<std::function<ILInstruction*()>> move_actions_;
};

}  // namespace cb::dotnet::decompiler::il::transforms
