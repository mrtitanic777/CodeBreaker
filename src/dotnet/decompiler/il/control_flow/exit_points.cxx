// dotnet/decompiler/il/control_flow/exit_points.cxx -- see exit_points.hpp.
#include "dotnet/decompiler/il/control_flow/exit_points.hpp"

#include <cassert>

#include "dotnet/decompiler/il/il_instruction_extensions.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

namespace {

// The markers get_exit returns (two nops with a comment, compared by address).
Nop* exit_not_yet_determined() {
    static Nop* marker = [] {
        static Nop nop;
        nop.comment = "ExitNotYetDetermined";
        return &nop;
    }();
    return marker;
}

Nop* no_exit() {
    static Nop* marker = [] {
        static Nop nop;
        nop.comment = "NoExit";
        return &nop;
    }();
    return marker;
}

}  // namespace

ILInstruction* DetectExitPoints::get_exit(ILInstruction* inst) {
    const SlotInfo* slot = inst->slot_info();
    if (slot == &Block::instruction_slot) {
        auto* block = static_cast<Block*>(inst->parent());
        ILInstruction* next = block->instructions.element_at_or_default(inst->child_index() + 1);
        return next != nullptr ? next : exit_not_yet_determined();
    } else if (slot == &TryInstruction::try_block_slot || slot == &TryCatchHandler::body_slot || slot == &TryCatch::handler_slot ||
               slot == &PinnedRegion::body_slot || slot == &UsingInstruction::body_slot || slot == &LockInstruction::body_slot) {
        return get_exit(inst->parent());
    }
    return no_exit();
}

bool DetectExitPoints::compatible_exit_instruction(ILInstruction* exit1, ILInstruction* exit2) {
    if (exit1 == nullptr || exit2 == nullptr || exit1->op_code != exit2->op_code) return false;
    switch (exit1->op_code) {
        case OpCode::Branch: {
            auto* br1 = static_cast<Branch*>(exit1);
            auto* br2 = static_cast<Branch*>(exit2);
            return br1->target_block() == br2->target_block();
        }
        case OpCode::Leave: {
            auto* leave1 = static_cast<Leave*>(exit1);
            auto* leave2 = static_cast<Leave*>(exit2);
            return leave1->target_container() == leave2->target_container() && leave1->value()->match_nop() && leave2->value()->match_nop();
        }
        default:
            return false;
    }
}

DetectExitPoints::ContainerContext::ContainerContext(BlockContainer* container_, ILInstruction* current_exit_)
    : container(container_), current_exit(current_exit_) {
    if (current_exit == exit_not_yet_determined()) potential_exits.emplace();
}

void DetectExitPoints::ContainerContext::handle_exit(ILInstruction* inst) {
    if (current_exit == exit_not_yet_determined() && container->leave_count() == 0) {
        potential_exits->push_back(inst);
    } else if (compatible_exit_instruction(inst, current_exit)) {
        inst->replace_with(with_il_range(make<Leave>(container), inst));
    }
}

void DetectExitPoints::run(ILFunction* function, transforms::ILTransformContext& context) {
    cancellation_token_ = context.cancellation_token;
    blocks_potentially_made_unreachable_.clear();
    container_stack_.clear();
    function->accept_visitor(*this);
    // It's possible that there are unreachable code blocks which we only detect as such during exit point detection.
    // Clean them up.
    for (Block* block : blocks_potentially_made_unreachable_) {
        if (block->incoming_edge_count() == 0 || (block->incoming_edge_count() == 1 && is_infinite_loop(block))) block->remove();
    }
    blocks_potentially_made_unreachable_.clear();
    container_stack_.clear();
}

bool DetectExitPoints::is_infinite_loop(Block* block) {
    if (block->instructions.count() != 1) return false;
    auto* b = as<Branch>(block->instructions[0]);
    return b != nullptr && b->target_block() == block;
}

void DetectExitPoints::default_visit(ILInstruction& inst) {
    for (ILInstruction* child : inst.children()) child->accept_visitor(*this);
}

void DetectExitPoints::visit_block_container(BlockContainer& container) {
    ILInstruction* this_exit = get_exit(&container);
    container_stack_.push_back(std::make_unique<ContainerContext>(&container, this_exit));
    ContainerContext* stack_entry = container_stack_.back().get();
    default_visit(container);
    bool any_connected = false;
    if (stack_entry->potential_exits) {
        for (ILInstruction* i : *stack_entry->potential_exits) {
            if (i->is_connected()) {
                any_connected = true;
                break;
            }
        }
    }
    if (any_connected) {
        // This transform determined an exit point.
        std::vector<ILInstruction*> connected_exits;
        for (ILInstruction* i : *stack_entry->potential_exits)
            if (i->is_connected()) connected_exits.push_back(i);
        ILInstruction* new_exit = choose_exit(connected_exits);
        assert(!new_exit->match_leave(&container));
        for (ILInstruction* exit : *stack_entry->potential_exits) {
            if (exit->is_connected() && compatible_exit_instruction(new_exit, exit)) {
                exit->replace_with(with_il_range(make<Leave>(&container), exit));
            }
        }
        ILInstruction* inst = &container;
        // traverse up to the block (we always find one because get_exit only returns "not yet determined" if there is a block)
        while (inst->parent()->op_code != OpCode::Block) inst = inst->parent();
        auto* block = static_cast<Block*>(inst->parent());
        if (block->has_flag(InstructionFlags::EndPointUnreachable)) {
            // Special case: despite replacing the exits with leave(currentContainer), we still have an unreachable endpoint.
            // The appended currentExit instruction would not be reachable!
            // This happens in test case ExceptionHandling.ThrowInFinally()
            if (auto* b = as<Branch>(new_exit)) blocks_potentially_made_unreachable_.push_back(b->target_block());
        } else {
            block->instructions.add(new_exit);
        }
    }
    assert(container_stack_.back().get() == stack_entry);
    container_stack_.pop_back();
}

ILInstruction* DetectExitPoints::choose_exit(const std::vector<ILInstruction*>& potential_exits) {
    ILInstruction* first = potential_exits.front();
    auto* first_leave = as<Leave>(first);
    if (first_leave != nullptr && first_leave->is_leaving_function()) {
        for (std::size_t i = 1; i < potential_exits.size(); ++i) {
            ILInstruction* exit = potential_exits[i];
            auto* leave = as<Leave>(exit);
            if (!(leave != nullptr && leave->is_leaving_function())) return exit;
        }
    }
    return first;
}

void DetectExitPoints::visit_block(Block& block) {
    cancellation_token_.throw_if_cancellation_requested();
    // Not a range-for: the children might add to the block.
    for (int i = 0; i < block.instructions.count(); i++) block.instructions[i]->accept_visitor(*this);
}

void DetectExitPoints::visit_branch(Branch& inst) {
    for (auto it = container_stack_.rbegin(); it != container_stack_.rend(); ++it) {
        ContainerContext& entry = **it;
        if (inst.target_block()->is_descendant_of(entry.container)) break;
        entry.handle_exit(&inst);
    }
}

void DetectExitPoints::visit_leave(Leave& inst) {
    default_visit(inst);
    if (!inst.value()->match_nop()) return;
    for (auto it = container_stack_.rbegin(); it != container_stack_.rend(); ++it) {
        ContainerContext& entry = **it;
        if (inst.target_container() == entry.container) break;
        if (inst.is_leaving_function() || inst.target_container()->kind() != ContainerKind::Normal) {
            if (entry.container->kind() == ContainerKind::Normal) {
                // Don't transform a `return`/`break` into a leave for try-block containers (or similar).
                // It's possible that those can be turned into fallthrough later, but it might not work out and then we would be left
                // with a `goto`. But continue searching the container stack, it might be possible to turn the `return` into a
                // `break` instead.
            } else {
                // return; could turn to break;
                entry.handle_exit(&inst);
                break;  // but only for the innermost loop/switch
            }
        } else {
            entry.handle_exit(&inst);
        }
    }
}

}  // namespace cb::dotnet::decompiler::il::control_flow
