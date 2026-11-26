// dotnet/decompiler/il/transforms/il_extraction.cpp -- see il_extraction.hpp.
#include "dotnet/decompiler/il/transforms/il_extraction.hpp"

#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::il {

namespace transforms {

ExtractionContext::ExtractionContext(ILFunction* function, std::function<const IType*(StackType)> find_type)
    : function_(function), find_type_(std::move(find_type)) {
    assert(function != nullptr);
}

void ExtractionContext::register_move(ILInstruction* predecessor) {
    flags_being_moved |= predecessor->flags();
    move_actions_.push_back([this, predecessor]() -> ILInstruction* {
        const IType* type = find_type_(predecessor->result_type());
        ILVariable* v = function_->register_variable(VariableKind::StackSlot, type);
        predecessor->replace_with(make<LdLoc>(v));
        return make<StLoc>(v, predecessor);
    });
}

void ExtractionContext::register_move_if_necessary(ILInstruction* predecessor) {
    if (!can_reorder_with_instructions_being_moved(predecessor)) register_move(predecessor);
}

bool ExtractionContext::can_reorder_with_instructions_being_moved(ILInstruction* predecessor) const {
    // Tracking the instructions being moved would allow fewer moves, but with the few scenarios extraction is used in, it isn't needed.
    return predecessor->flags() == InstructionFlags::None;
}

}  // namespace transforms

// Prepares "extracting" a descendant instruction out of this instruction (the opposite of ILInlining; it may involve re-compiling
// high-level constructs into lower-level ones). True if extraction is possible.
bool ILInstruction::prepare_extract(int child_index, transforms::ExtractionContext& ctx) {
    if (!get_child_slot(child_index).can_inline_into) return false;
    // check whether re-ordering with the predecessors is valid
    for (int i = child_index - 1; i >= 0; --i) {
        ILInstruction* predecessor = get_child(i);
        if (!get_child_slot(i).can_inline_into) return false;
        ctx.register_move_if_necessary(predecessor);
    }
    return true;
}

bool BlockContainer::prepare_extract(int child_index, transforms::ExtractionContext&) {
    // Un-inlining from the entry point is allowed as long as no code moves out of a loop.
    return child_index == 0 && entry_point()->incoming_edge_count() == 1;
}

}  // namespace cb::dotnet::decompiler::il
