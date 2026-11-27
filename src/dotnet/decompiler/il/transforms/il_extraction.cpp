// dotnet/decompiler/il/transforms/il_extraction.cpp -- see il_extraction.hpp.
#include "dotnet/decompiler/il/transforms/il_extraction.hpp"

#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il {

namespace transforms {

const IType* find_type_of_stack_type(const typesystem::ICompilation& compilation, StackType stack_type, typesystem::Sign sign) {
    switch (stack_type) {
        case StackType::Unknown: return &typesystem::SpecialType::unknown_type();
        case StackType::Ref: return typesystem::make_by_reference_type(&typesystem::SpecialType::unknown_type());
        default: return compilation.find_type(typesystem::to_known_type_code(stack_type, sign));
    }
}

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

ILVariable* ExtractionContext::extract(ILInstruction* inst_to_extract, ILTransformContext& context) {
    ILFunction* function = nullptr;
    for (ILInstruction* ancestor : inst_to_extract->ancestors()) {
        if (auto* f = as<ILFunction>(ancestor)) {
            function = f;
            break;
        }
    }
    if (function == nullptr) throw std::out_of_range("the instruction is not in a function");
    ExtractionContext ctx(function, [&context](StackType stack_type) { return find_type_of_stack_type(context.type_system(), stack_type); });
    ctx.flags_being_moved = inst_to_extract->flags();
    ILInstruction* inst = inst_to_extract;
    while (inst != nullptr) {
        auto* if_inst = as<IfInstruction>(inst->parent());
        if (if_inst != nullptr && inst->slot_info() != &IfInstruction::condition_slot) {
            // this context doesn't support extraction, but maybe we can create a block here?
            if (if_inst->result_type() == StackType::Void) {
                Block* new_block = make<Block>();
                inst->replace_with(new_block);
                new_block->instructions.add(inst);
            }
        }
        auto* block = as<Block>(inst->parent());
        if (block != nullptr && block->kind == BlockKind::ControlFlow) {
            // We've reached a target block, and extraction is possible all the way.
            // Check if the parent BlockContainer allows extraction:
            if (auto* container = as<BlockContainer>(block->parent())) {
                switch (container->kind()) {
                    case ContainerKind::Normal:
                    case ContainerKind::Loop:
                        // extraction is always possible
                        break;
                    case ContainerKind::Switch:
                        // extraction is possible, unless in the entry-point (i.e., the switch head)
                        if (block == container->entry_point() && inst->child_index() == 0) {
                            // try to extract to the container's parent block, if it's a valid location
                            inst = container;
                            continue;
                        }
                        break;
                    case ContainerKind::While:
                        // extraction is possible, unless in the entry-point (i.e., the condition block)
                        if (block == container->entry_point()) {
                            return nullptr;
                        }
                        break;
                    case ContainerKind::DoWhile:
                        // extraction is possible, unless in the last block (i.e., the condition block)
                        if (block == container->blocks.last()) {
                            return nullptr;
                        }
                        break;
                    case ContainerKind::For:
                        // extraction is possible, unless in the first or last block (i.e., the condition block or increment block)
                        if (block == container->entry_point() || block == container->blocks.last()) {
                            return nullptr;
                        }
                        break;
                }
            }
            int insert_index = inst->child_index();
            const IType* type = find_type_of_stack_type(context.type_system(), inst_to_extract->result_type());
            // Move instToExtract itself:
            ILVariable* v = function->register_variable(VariableKind::StackSlot, type);
            inst_to_extract->replace_with(make<LdLoc>(v));
            block->instructions.insert(insert_index, make<StLoc>(v, inst_to_extract));
            // Apply the other move actions:
            for (const auto& move_action : ctx.move_actions()) {
                ILInstruction* store = move_action();
                block->instructions.insert(insert_index, store);
            }
            return v;
        }
        if (inst->parent() != nullptr && !inst->parent()->prepare_extract(inst->child_index(), ctx)) return nullptr;
        inst = inst->parent();
    }
    return nullptr;
}

}  // namespace transforms

ILVariable* ILInstruction::extract(transforms::ILTransformContext& context) {
    return transforms::ExtractionContext::extract(this, context);
}

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
