// dotnet/decompiler/il/control_flow/remove_redundant_return.cpp -- see remove_redundant_return.hpp.
#include "dotnet/decompiler/il/control_flow/remove_redundant_return.hpp"

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

void RemoveRedundantReturn::run(ILFunction* function, transforms::ILTransformContext&) {
    for (ILInstruction* inst : function->descendants()) {
        auto* lambda = as<ILFunction>(inst);
        if (lambda == nullptr) continue;
        auto* c = as<BlockContainer>(lambda->body());
        if (c == nullptr) continue;
        const IType* return_type = lambda->async_return_type != nullptr ? lambda->async_return_type : lambda->return_type;
        if (!(return_type->kind() == typesystem::TypeKind::Void || lambda->is_iterator)) continue;
        Block* last_block = c->blocks.last();
        auto* last_leave = as<Leave>(last_block->instructions.last());
        if (last_leave != nullptr && last_leave->is_leaving_function()) {
            convert_return_to_fallthrough(last_block->instructions.second_to_last_or_default());
        } else {
            if (convert_return_to_fallthrough(last_block->instructions.last())) last_block->instructions.add(make<Leave>(c));
        }
    }
}

bool RemoveRedundantReturn::convert_return_to_fallthrough(ILInstruction* inst) {
    bool result = false;
    if (inst == nullptr) return false;
    if (auto* c = as<BlockContainer>(inst)) {
        if (c->kind() == ContainerKind::Normal) {
            // body of try block, or similar: recurse into the last instruction in the container
            // (no need to handle loops and switches here; DetectExitPoints already did)
            Block* last_block = c->blocks.last();
            auto* leave = as<Leave>(last_block->instructions.last());
            if (leave != nullptr && leave->is_leaving_function() && is<Nop>(leave->value())) {
                leave->set_target_container(c);
                result = true;
            } else if (convert_return_to_fallthrough(last_block->instructions.last())) {
                last_block->instructions.add(make<Leave>(c));
                result = true;
            }
        }
    } else if (auto* try_catch = as<TryCatch>(inst)) {
        result |= convert_return_to_fallthrough(try_catch->try_block());
        for (TryCatchHandler* h : try_catch->handlers) result |= convert_return_to_fallthrough(h->body());
    } else if (auto* try_finally = as<TryFinally>(inst)) {
        result |= convert_return_to_fallthrough(try_finally->try_block());
    } else if (auto* lock_inst = as<LockInstruction>(inst)) {
        result |= convert_return_to_fallthrough(lock_inst->body());
    } else if (auto* using_inst = as<UsingInstruction>(inst)) {
        result |= convert_return_to_fallthrough(using_inst->body());
    } else if (auto* pinned_region = as<PinnedRegion>(inst)) {
        result |= convert_return_to_fallthrough(pinned_region->body());
    } else if (auto* if_instruction = as<IfInstruction>(inst)) {
        result |= convert_return_to_fallthrough(if_instruction->true_inst());
        result |= convert_return_to_fallthrough(if_instruction->false_inst());
    } else if (auto* block = as<Block>(inst)) {
        if (block->kind == BlockKind::ControlFlow) {
            ILInstruction* last_inst = block->instructions.last_or_default();
            auto* last_leave = as<Leave>(last_inst);
            if (last_leave != nullptr && last_leave->is_leaving_function() && is<Nop>(last_leave->value())) {
                block->instructions.remove_at(block->instructions.count() - 1);
                result = true;
                last_inst = block->instructions.last_or_default();
            }
            result |= convert_return_to_fallthrough(last_inst);
        }
    }
    return result;
}

}  // namespace cb::dotnet::decompiler::il::control_flow
