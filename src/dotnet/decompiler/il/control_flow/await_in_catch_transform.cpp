// dotnet/decompiler/il/control_flow/await_in_catch_transform.cpp -- see await_in_catch_transform.hpp.
#include "dotnet/decompiler/il/control_flow/await_in_catch_transform.hpp"

#include <algorithm>
#include <cassert>
#include <typeinfo>
#include <unordered_set>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_graph.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

using transforms::ILTransformContext;
using typesystem::KnownTypeCode;

namespace {

// C#'s `(T)inst`: an exception when the instruction is not a T.
template <class T>
T* cast_to(ILInstruction* inst) {
    T* result = as<T>(inst);
    if (inst != nullptr && result == nullptr) throw std::bad_cast();
    return result;
}

}  // namespace

void AwaitInCatchTransform::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().await_in_catch_finally) return;
    std::vector<BlockContainer*> changed_containers;
    std::unordered_set<Block*> removed_blocks;

    // analyze all try-catch statements in the function
    std::vector<TryCatch*> try_catches;
    for (ILInstruction* d : function->descendants())
        if (auto* try_catch = as<TryCatch>(d)) try_catches.push_back(try_catch);
    for (TryCatch* try_catch : try_catches) {
        BlockContainer* container = try_catch->parent() != nullptr ? as<BlockContainer>(try_catch->parent()->parent()) : nullptr;
        if (container == nullptr) continue;
        // Detect all handlers that contain an await expression
        ILVariable* catch_handler_identifier = nullptr;
        std::vector<CatchBlockInfo> transformable_catch_blocks;
        analyze_handlers(try_catch->handlers, &catch_handler_identifier, &transformable_catch_blocks);
        ControlFlowGraph cfg(container, context.cancellation_token);
        if (!transformable_catch_blocks.empty() && std::find(changed_containers.begin(), changed_containers.end(), container) == changed_containers.end())
            changed_containers.push_back(container);
        SwitchInstruction* switch_instruction_opt = nullptr;
        for (const CatchBlockInfo& result : transformable_catch_blocks) {
            removed_blocks.clear();
            ControlFlowNode* node = cfg.get_node(result.real_catch_block_entry_point);

            context.step_start_group("Inline catch block with await (at " + result.handler->variable()->to_string() + ")", result.handler);

            // Remove the IfInstruction from the jump table and eliminate all branches to the block.
            if (auto* jump_table_entry = as<IfInstruction>(result.jump_table_entry)) {
                auto* jump_table_block = cast_to<Block>(jump_table_entry->parent());
                context.step("Remove jump-table entry", result.jump_table_entry);
                jump_table_block->instructions.remove_at(result.jump_table_entry->child_index());

                for (ILInstruction* d : try_catch->descendants()) {
                    auto* branch = as<Branch>(d);
                    if (branch != nullptr && branch->target_block() == jump_table_block) {
                        if (auto* exit_container = as<BlockContainer>(result.next_block_or_exit_container)) {
                            context.step("branch jumpTableBlock => leave exitContainer", branch);
                            branch->replace_with(make<Leave>(exit_container));
                        } else {
                            context.step("branch jumpTableBlock => branch nextBlock", branch);
                            branch->replace_with(make<Branch>(cast_to<Block>(result.next_block_or_exit_container)));
                        }
                    }
                }
            } else if (auto* section = as<SwitchSection>(result.jump_table_entry)) {
                assert(switch_instruction_opt == nullptr || section->parent() == switch_instruction_opt);
                switch_instruction_opt = cast_to<SwitchInstruction>(section->parent());
            }

            // Add the real catch block entry-point to the block container
            auto* handler_body = static_cast<BlockContainer*>(result.handler->body());
            Block* catch_block_head = handler_body->blocks.last();

            result.real_catch_block_entry_point->remove();
            handler_body->blocks.insert(0, result.real_catch_block_entry_point);

            // Remove the generated catch block
            catch_block_head->remove();

            transform_async_throw_to_throw(context, removed_blocks, result.real_catch_block_entry_point);

            // Inline all blocks that are dominated by the entrypoint of the real catch block
            for (ControlFlowNode* n : cfg.nodes()) {
                Block* block = n->user_data;

                if (node->dominates(n)) {
                    transform_async_throw_to_throw(context, removed_blocks, block);

                    if (block->parent() == result.handler->body()) continue;

                    if (!removed_blocks.contains(block)) {
                        context.step("Move block", result.handler->body());
                        move_block(block, static_cast<BlockContainer*>(result.handler->body()));
                    }
                }
            }

            // Remove unreachable pattern blocks
            // TODO : sanity check
            if (auto* next_block = as<Block>(result.next_block_or_exit_container); next_block != nullptr && next_block->incoming_edge_count() == 0) {
                std::vector<Block*> dependent_blocks;
                Block* current = next_block;

                do {
                    for (ILInstruction* d : current->descendants()) {
                        if (auto* branch = as<Branch>(d)) dependent_blocks.push_back(branch->target_block());
                    }

                    current->remove();
                    if (auto it = std::find(dependent_blocks.begin(), dependent_blocks.end(), current); it != dependent_blocks.end()) dependent_blocks.erase(it);
                    current = nullptr;
                    for (Block* b : dependent_blocks) {
                        if (b->incoming_edge_count() == 0) {
                            current = b;
                            break;
                        }
                    }
                } while (current != nullptr);
            }

            // Remove all assignments to the common object variable that stores the exception object.
            if (result.object_variable != result.handler->variable()) {
                std::vector<LdLoc*> loads = result.object_variable->load_instructions();
                for (LdLoc* load : loads) {
                    if (!load->is_descendant_of(result.handler)) continue;

                    if (auto* cc = as<CastClass>(load->parent()); cc != nullptr && typesystem::type_equals(cc->type(), result.handler->variable()->type())) {
                        cc->replace_with(with_il_range(with_il_range(make<LdLoc>(result.handler->variable()), cc), load));
                    } else {
                        load->replace_with(with_il_range(make<LdLoc>(result.handler->variable()), load));
                    }
                }
            }

            context.step_end_group(true);
        }

        if (switch_instruction_opt != nullptr) {
            auto* b = as<Block>(switch_instruction_opt->parent());
            if (b != nullptr && b->incoming_edge_count() > 0) {
                SwitchSection* default_section = switch_instruction_opt->get_default_section();

                for (ILInstruction* d : container->descendants()) {
                    auto* branch = as<Branch>(d);
                    if (branch == nullptr || branch->target_block() != b) continue;
                    branch->replace_with(default_section->body()->clone());
                }
            }
        }
    }

    // clean up all modified containers
    for (BlockContainer* container : changed_containers) container->sort_blocks(true);
}

void AwaitInCatchTransform::transform_async_throw_to_throw(ILTransformContext& context, std::unordered_set<Block*>& removed_blocks, Block* block) {
    ILVariable* v = nullptr;
    StLoc* typed_exception_variable_store = nullptr;
    Block* capture_block = nullptr;
    Block* throw_block = nullptr;
    if (match_exception_capture_block(context, block, &v, &typed_exception_variable_store, &capture_block, &throw_block)) {
        context.step("ExceptionDispatchInfo.Capture(" + v->to_string() + ").Throw() => throw;", typed_exception_variable_store);
        block->instructions.remove_range(typed_exception_variable_store->child_index() + 1, 2);
        capture_block->remove();
        throw_block->remove();
        removed_blocks.insert(capture_block);
        removed_blocks.insert(throw_block);
        typed_exception_variable_store->replace_with(make<Rethrow>());
    }
}

void AwaitInCatchTransform::move_block(Block* block, BlockContainer* target) {
    block->remove();
    target->blocks.add(block);
}

bool AwaitInCatchTransform::analyze_handlers(InstructionCollection<TryCatchHandler>& handlers, ILVariable** catch_handler_identifier,
                                             std::vector<CatchBlockInfo>* transformable_catch_blocks) {
    transformable_catch_blocks->clear();
    *catch_handler_identifier = nullptr;
    for (TryCatchHandler* handler : handlers) {
        int id = 0;
        ILVariable* identifier_variable = nullptr;
        Block* real_entry_point = nullptr;
        ILInstruction* next_block_or_exit_container = nullptr;
        ILInstruction* jump_table_entry = nullptr;
        ILVariable* object_variable = nullptr;
        if (!match_await_catch_handler(handler, &id, &identifier_variable, &real_entry_point, &next_block_or_exit_container, &jump_table_entry,
                                       &object_variable)) {
            continue;
        }

        if (id < 1 || (*catch_handler_identifier != nullptr && identifier_variable != *catch_handler_identifier)) {
            continue;
        }

        *catch_handler_identifier = identifier_variable;
        transformable_catch_blocks->push_back(CatchBlockInfo{id, handler, real_entry_point, next_block_or_exit_container, jump_table_entry,
                                                              object_variable != nullptr ? object_variable : handler->variable()});
    }
    return !transformable_catch_blocks->empty();
}

bool AwaitInCatchTransform::match_await_catch_handler(TryCatchHandler* handler, int* id, ILVariable** identifier_variable, Block** real_entry_point,
                                                      ILInstruction** next_block_or_exit_container, ILInstruction** jump_table_entry,
                                                      ILVariable** object_variable) {
    *id = 0;
    *identifier_variable = nullptr;
    *real_entry_point = nullptr;
    *jump_table_entry = nullptr;
    *object_variable = nullptr;
    *next_block_or_exit_container = nullptr;
    ILVariable* exception_variable = handler->variable();
    Block* catch_block = static_cast<BlockContainer*>(handler->body())->entry_point();
    ILInstruction* value = nullptr;
    switch (catch_block->instructions.count()) {
        case 3:
            if (!catch_block->instructions[0]->match_st_loc(object_variable, &value)) return false;
            if (!value->match_ld_loc(exception_variable)) return false;
            break;
        case 4: {
            ILVariable* temporary_variable = nullptr;
            if (!catch_block->instructions[0]->match_st_loc(&temporary_variable, &value)) return false;
            if (!value->match_ld_loc(exception_variable)) return false;
            if (!catch_block->instructions[1]->match_st_loc(object_variable, &value)) return false;
            if (!value->match_ld_loc(temporary_variable)) return false;
            break;
        }
        default:
            // if the exception variable is not used at all (e.g., catch (Exception))
            // the "exception-variable-assignment" is omitted completely.
            // This can happen in optimized code.
            break;
    }
    Block* jump_table_start_block = nullptr;
    if (!catch_block->instructions.last()->match_branch(&jump_table_start_block)) return false;
    ILInstruction* identifier_variable_assignment = catch_block->instructions.second_to_last_or_default();
    if (identifier_variable_assignment == nullptr) return false;
    if (!identifier_variable_assignment->match_st_loc(identifier_variable, &value) || !value->match_ldc_i4(id)) return false;
    // analyze jump table:
    switch (jump_table_start_block->instructions.count()) {
        case 3: {
            // stloc identifierVariableCopy(identifierVariable)
            // if (comp(identifierVariable == id)) br realEntryPoint
            // br jumpTableEntryBlock
            ILVariable* identifier_variable_copy = nullptr;
            ILInstruction* identifier_variable_load = nullptr;
            if (!jump_table_start_block->instructions[0]->match_st_loc(&identifier_variable_copy, &identifier_variable_load) ||
                !identifier_variable_load->match_ld_loc(*identifier_variable)) {
                return false;
            }
            return parse_if_jump_table(*id, jump_table_start_block, identifier_variable_copy, real_entry_point, next_block_or_exit_container, jump_table_entry);
        }
        case 2:
            // if (comp(identifierVariable == id)) br realEntryPoint
            // br jumpTableEntryBlock
            return parse_if_jump_table(*id, jump_table_start_block, *identifier_variable, real_entry_point, next_block_or_exit_container, jump_table_entry);
        case 1: {
            auto* switch_inst = as<SwitchInstruction>(jump_table_start_block->instructions[0]);
            if (switch_inst == nullptr) return false;
            return parse_switch_jump_table(*id, switch_inst, *identifier_variable, real_entry_point, next_block_or_exit_container, jump_table_entry);
        }
        default:
            return false;
    }
}

bool AwaitInCatchTransform::parse_switch_jump_table(int id, SwitchInstruction* jump_table, ILVariable* identifier_variable, Block** real_entry_point,
                                                    ILInstruction** next_block_or_exit_container, ILInstruction** jump_table_entry) {
    *real_entry_point = nullptr;
    *next_block_or_exit_container = nullptr;
    *jump_table_entry = nullptr;

    if (!jump_table->value()->match_ld_loc(identifier_variable)) return false;

    SwitchSection* default_section = jump_table->get_default_section();

    for (SwitchSection* section : jump_table->sections) {
        if (!section->labels().contains(id)) continue;
        if (!section->body()->match_branch(real_entry_point)) return false;
        Block* t = nullptr;
        BlockContainer* t2 = nullptr;
        if (default_section->body()->match_branch(&t))
            *next_block_or_exit_container = t;
        else if (default_section->body()->match_leave(&t2))
            *next_block_or_exit_container = t2;
        *jump_table_entry = section;
        return true;
    }

    return false;
}

bool AwaitInCatchTransform::parse_if_jump_table(int id, Block* jump_table_entry_block, ILVariable* identifier_variable, Block** real_entry_point,
                                                ILInstruction** next_block_or_exit_container, ILInstruction** jump_table_entry) {
    *real_entry_point = nullptr;
    *next_block_or_exit_container = nullptr;
    *jump_table_entry = nullptr;
    do {
        auto* if_inst = as<IfInstruction>(jump_table_entry_block->instructions.second_to_last_or_default());
        if (if_inst == nullptr) return false;
        ILInstruction* last_inst = jump_table_entry_block->instructions.last();
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        if (if_inst->condition()->match_comp_equals(&left, &right)) {
            if (!if_inst->true_inst()->match_branch(real_entry_point)) return false;
            if (!last_inst->match_branch(&jump_table_entry_block)) {
                if (!last_inst->match_leave(cast_to<BlockContainer>(last_inst->parent()->parent()))) return false;
            }
        } else if (if_inst->condition()->match_comp_not_equals(&left, &right)) {
            if (!last_inst->match_branch(real_entry_point)) return false;
            if (!if_inst->true_inst()->match_branch(&jump_table_entry_block)) {
                if (!if_inst->true_inst()->match_leave(cast_to<BlockContainer>(last_inst->parent()->parent()))) return false;
            }
        } else {
            return false;
        }
        if (!left->match_ld_loc(identifier_variable)) return false;
        if (right->match_ldc_i4(id)) {
            *next_block_or_exit_container = jump_table_entry_block != nullptr ? static_cast<ILInstruction*>(jump_table_entry_block) : last_inst->parent()->parent();
            *jump_table_entry = if_inst;
            return true;
        }
    } while (jump_table_entry_block != nullptr && jump_table_entry_block->instructions.count() == 2);
    return false;
}

// Block beforeThrowBlock {
// 	[before throw]
// 	stloc typedExceptionVariable(isinst System.Exception(ldloc objectVariable))
// 	if (comp.o(ldloc typedExceptionVariable != ldnull)) br captureBlock
// 	br throwBlock
// }
//
// Block throwBlock {
// 	throw(ldloc objectVariable)
// }
//
// Block captureBlock {
// 	callvirt Throw(call Capture(ldloc typedExceptionVariable))
// 	br nextBlock
// }
// =>
// throw(ldloc result.Handler.Variable)
bool AwaitInCatchTransform::match_exception_capture_block(ILTransformContext& context, Block* block, ILVariable** object_variable,
                                                          StLoc** typed_exception_variable_store, Block** capture_block, Block** throw_block) {
    auto derives_from_exception = [](const IType* t) {
        for (const IType* ty : typesystem::get_all_base_types(t))
            if (typesystem::is_known_type(ty, KnownTypeCode::Exception)) return true;
        return false;
    };

    *capture_block = nullptr;
    *throw_block = nullptr;
    *typed_exception_variable_store = nullptr;

    auto* typed_exception_variable_st_loc = as<StLoc>(block->instructions.element_at_or_default(block->instructions.count() - 3));

    ILInstruction* arg = nullptr;
    const IType* type = nullptr;
    ILVariable* v = nullptr;
    if (typed_exception_variable_st_loc == nullptr || !typed_exception_variable_st_loc->value()->match_is_inst(&arg, &type) || !derives_from_exception(type) ||
        !arg->match_ld_loc(&v)) {
        return false;
    }

    if (*object_variable == nullptr) {
        *object_variable = v;
    } else if (*object_variable != v) {
        return false;
    }

    *typed_exception_variable_store = typed_exception_variable_st_loc;

    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!block->instructions[block->instructions.count() - 2]->match_if_instruction(&condition, &true_inst)) return false;

    ILInstruction* last_instr = block->instructions.last();
    if (!last_instr->match_branch(throw_block)) return false;

    if (auto* branch_to_capture = as<Branch>(true_inst); condition->match_comp_not_equals_null(&arg) && branch_to_capture != nullptr) {
        if (!arg->match_ld_loc((*typed_exception_variable_store)->variable())) return false;
        *capture_block = branch_to_capture->target_block();
    } else {
        return false;
    }

    ILInstruction* ov = nullptr;
    if ((*throw_block)->incoming_edge_count() != 1 || (*throw_block)->instructions.count() != 1 ||
        !((*throw_block)->instructions[0]->match_throw(&ov) && ov->match_ld_loc(*object_variable))) {
        return false;
    }

    auto match_capture_throw_calls = [&](ILInstruction* inst) {
        const IType* exception_dispatch_info_type = typesystem::find_type(
            context.type_system(), typesystem::FullTypeName(typesystem::TopLevelTypeName("System.Runtime.ExceptionServices", "ExceptionDispatchInfo")));
        auto* call_virt = as<CallVirt>(inst);
        if (call_virt == nullptr || call_virt->arguments().count() != 1) return false;

        auto* call = as<Call>(call_virt->arguments()[0]);
        if (call == nullptr || call->arguments().count() != 1 || !call->arguments()[0]->match_ld_loc(typed_exception_variable_st_loc->variable())) {
            return false;
        }

        return call_virt->method->name() == "Throw" && typesystem::type_equals(call_virt->method->declaring_type(), exception_dispatch_info_type) &&
               call->method->name() == "Capture" && typesystem::type_equals(call->method->declaring_type(), exception_dispatch_info_type);
    };

    if ((*capture_block)->incoming_edge_count() != 1 || (*capture_block)->instructions.count() != 2 ||
        !match_capture_throw_calls((*capture_block)->instructions[0])) {
        return false;
    }

    return true;
}

}  // namespace cb::dotnet::decompiler::il::control_flow
