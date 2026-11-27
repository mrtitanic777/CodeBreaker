// dotnet/decompiler/il/control_flow/detect_pinned_regions.cpp -- see detect_pinned_regions.hpp.
#include "dotnet/decompiler/il/control_flow/detect_pinned_regions.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <format>
#include <string>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

using transforms::ILInlining;
using transforms::InliningOptions;

void DetectPinnedRegions::run(ILFunction* function, transforms::ILTransformContext& context) {
    context_ = &context;
    for (ILInstruction* descendant : function->descendants()) {
        auto* container = as<BlockContainer>(descendant);
        if (container == nullptr) continue;
        context.cancellation_token.throw_if_cancellation_requested();
        detect_null_safe_array_to_pointer_or_custom_ref_pin(container);
        split_blocks_at_writes_to_pinned_locals(container);
        for (Block* block : container->blocks) detect_pinned_region(block);
        container->blocks.remove_all([](Block* b) { return b->instructions.count() == 0; });  // remove dummy blocks
    }
    // Sometimes there's leftover writes to the original pinned locals
    for (ILInstruction* descendant : function->descendants()) {
        auto* block = as<Block>(descendant);
        if (block == nullptr) continue;
        context.cancellation_token.throw_if_cancellation_requested();
        for (int i = 0; i < block->instructions.count(); i++) {
            auto* stloc = as<StLoc>(block->instructions[i]);
            if (stloc != nullptr && stloc->variable()->kind() == VariableKind::PinnedLocal && stloc->variable()->load_count() == 0 &&
                stloc->variable()->address_count() == 0) {
                if (SemanticHelper::is_pure(stloc->value()->flags())) {
                    block->instructions.remove_at(i--);
                } else {
                    stloc->replace_with(stloc->value());
                }
            }
        }
    }
    context_ = nullptr;
}

// Ensures that every write to a pinned local is followed by a branch instruction.
// This ensures the 'pinning region' does not involve any half blocks, which makes it easier to extract.
void DetectPinnedRegions::split_blocks_at_writes_to_pinned_locals(BlockContainer* container) {
    for (int i = 0; i < container->blocks.count(); i++) {
        Block* block = container->blocks[i];
        for (int j = 0; j < block->instructions.count() - 1; j++) {
            ILInstruction* inst = block->instructions[j];
            ILVariable* v = nullptr;
            ILInstruction* value = nullptr;
            if (inst->match_st_loc(&v, &value) && v->kind() == VariableKind::PinnedLocal) {
                if (block->instructions[j + 1]->op_code != OpCode::Branch) {
                    // split block after j:
                    context_->step("Split block after pinned local write", inst);
                    Block* new_block = make<Block>();
                    for (int k = j + 1; k < block->instructions.count(); k++) new_block->instructions.add(block->instructions[k]);
                    new_block->add_il_range(*new_block->instructions[0]);
                    block->instructions.remove_range(j + 1, new_block->instructions.count());
                    block->instructions.add(make<Branch>(new_block));
                    container->blocks.insert(i + 1, new_block);
                }
                // in case of re-pinning (e.g. C++/CLI assignment to pin_ptr variable),
                // it's possible for the new value to be dependent on the old.
                if (v->is_used_within(value)) {
                    // In this case, we need to un-inline the uses of the pinned local
                    // so that they are split off into the block prior to the pinned local write
                    ILVariable* temp = context_->function()->register_variable(VariableKind::StackSlot, v->type());
                    int insert_index = j++;
                    block->instructions.insert(insert_index, make<StLoc>(temp, make<LdLoc>(v)));
                    for (ILInstruction* descendant : value->descendants()) {
                        if (descendant->match_ld_loc(v)) {
                            descendant->replace_with(with_il_range(make<LdLoc>(temp), descendant));
                        }
                    }
                }
                if (j > 0) {
                    // split block before j:
                    context_->step("Split block before pinned local write", inst);
                    Block* new_block = make<Block>();
                    new_block->instructions.add(block->instructions[j]);
                    new_block->instructions.add(block->instructions[j + 1]);
                    new_block->add_il_range(*new_block->instructions[0]);
                    assert(block->instructions.count() == j + 2);
                    block->instructions.remove_range(j, 2);
                    block->instructions.insert(j, make<Branch>(new_block));
                    container->blocks.insert(i + 1, new_block);
                }
            }
        }
    }
}

// ---- null-safe array to pointer -----------------------------------------------------------------------------------------------------

void DetectPinnedRegions::detect_null_safe_array_to_pointer_or_custom_ref_pin(BlockContainer* container) {
    bool modified = false;
    for (int i = 0; i < container->blocks.count(); i++) {
        Block* block = container->blocks[i];
        ILVariable* v = nullptr;
        ILVariable* p = nullptr;
        Block* target_block = nullptr;
        ILInstruction* ldloc_mem = nullptr;
        CallInstruction* call_gpr = nullptr;
        StLoc* stloc_ptr = nullptr;
        Block* null_block = nullptr;
        Block* not_null_block = nullptr;
        if (is_null_safe_array_to_pointer_pattern(block, &v, &p, &target_block)) {
            context_->step("NullSafeArrayToPointerPattern", block);
            ILInstruction* array_to_pointer = make<GetPinnableReference>(make<LdLoc>(v), nullptr);
            if (p->stack_type != StackType::Ref) {
                array_to_pointer = make<Conv>(array_to_pointer, typesystem::to_primitive_type(p->stack_type), false, Sign::None);
            }
            int index = block->instructions.count() - 2;
            ILInstruction* old_inst = block->instructions[index];
            block->instructions.set(index, with_il_range(make<StLoc>(p, array_to_pointer), old_inst));
            static_cast<Branch*>(block->instructions.last())->set_target_block(target_block);
            modified = true;
        } else if (is_custom_ref_pin_pattern(block, &ldloc_mem, &call_gpr, &v, &stloc_ptr, &target_block, &null_block, &not_null_block)) {
            context_->step("CustomRefPinPattern", block);
            ILInstruction* gpr;
            if (context_->settings().pattern_based_fixed_statement) {
                gpr = make<GetPinnableReference>(ldloc_mem, call_gpr->method);
            } else {
                Comp* condition = make<Comp>(ComparisonKind::Inequality, Sign::None, ldloc_mem, make<LdNull>());
                Conv* false_inst = make<Conv>(make<LdcI4>(0), PrimitiveType::Ref, false, Sign::None);
                gpr = make<IfInstruction>(condition, call_gpr, false_inst);
            }
            int index = block->instructions.count() - 2;
            ILInstruction* old_inst = block->instructions[index];
            block->instructions.set(index, with_il_range(make<StLoc>(v, gpr), old_inst));
            if (stloc_ptr != nullptr) block->instructions.insert(block->instructions.count() - 1, stloc_ptr);
            static_cast<Branch*>(block->instructions.last())->set_target_block(target_block);
            // clear out internal blocks that are now unreachable, so that
            // targetBlock.IncomingEdgeCount is accurate at this point.
            if (null_block != nullptr) null_block->instructions.clear();
            not_null_block->instructions.clear();
            if (target_block->incoming_edge_count() == 1 && target_block->parent() == block->parent()) {
                block->instructions.remove_at(block->instructions.count() - 1);
                block->instructions.add_range(target_block->instructions);
                target_block->instructions.clear();
                if (stloc_ptr != nullptr) {
                    ILInlining::inline_one_if_possible(block, stloc_ptr->child_index(), InliningOptions::None, *context_);
                }
            }
            modified = true;
        }
    }
    if (modified) {
        container->blocks.remove_all([](Block* b) { return b->incoming_edge_count() == 0; });  // remove blocks made unreachable
    }
}

// Detect the following pattern:
//      if (comp.o(ldloc mem != ldnull)) br notNullBlock
//      br nullBlock
//  }
//
//  Block nullBlock (incoming: 1) {
//      stloc ptr(conv i4->u <zero extend>(ldc.i4 0))
//      br targetBlock
//  }
//
//  Block notNullBlock (incoming: 1) {
//      stloc V_1(call GetPinnableReference(ldloc mem))
//      stloc ptr(conv ref->u (ldloc V_1))
//      br targetBlock
//  }
// It will be replaced with:
//      stloc V_1(get.pinnable.reference(ldloc mem))
//      stloc ptr(conv ref->u (ldloc V_1))
//      br targetBlock
bool DetectPinnedRegions::is_custom_ref_pin_pattern(Block* block, ILInstruction** ldloc_mem, CallInstruction** call_gpr, ILVariable** v,
                                                    StLoc** ptr_assign, Block** target_block, Block** null_block, Block** not_null_block) {
    *ldloc_mem = nullptr;
    *call_gpr = nullptr;
    *v = nullptr;
    *ptr_assign = nullptr;
    *target_block = nullptr;
    *null_block = nullptr;
    *not_null_block = nullptr;
    //      if (comp.o(ldloc mem != ldnull)) br on_not_null
    //      br on_null
    ILInstruction* if_condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!block->match_if_at_end_of_block(&if_condition, &true_inst, &false_inst)) return false;
    if (!if_condition->match_comp_not_equals_null(ldloc_mem)) {
        if (if_condition->match_comp_equals_null(ldloc_mem)) {
            std::swap(true_inst, false_inst);
        } else {
            return false;
        }
    }
    if (!SemanticHelper::is_pure((*ldloc_mem)->flags())) return false;
    if (!true_inst->match_branch(not_null_block) || (*not_null_block)->parent() != block->parent()) return false;
    if (!false_inst->match_branch(null_block) || (*null_block)->parent() != block->parent()) return false;

    //  Block notNullBlock (incoming: 1) {
    //      stloc V_1(call GetPinnableReference(ldloc mem))
    //      stloc ptr(conv ref->u (ldloc V_1))
    //      br targetBlock
    //  }
    if ((*not_null_block)->incoming_edge_count() != 1) return false;
    if ((*not_null_block)->instructions.count() < 2) return false;
    // stloc V_1(call GetPinnableReference(ldloc mem))
    ILInstruction* value = nullptr;
    if (!(*not_null_block)->instructions[0]->match_st_loc(v, &value)) return false;
    if ((*v)->kind() != VariableKind::PinnedLocal) return false;
    *call_gpr = as<CallInstruction>(value);
    if (*call_gpr == nullptr || (*call_gpr)->arguments().count() != 1) return false;
    if ((*call_gpr)->method->name() != "GetPinnableReference") return false;
    if (!(*ldloc_mem)->match((*call_gpr)->arguments()[0]).success()) return false;
    // stloc ptr(conv ref->u (ldloc V_1))
    *ptr_assign = as<StLoc>((*not_null_block)->instructions[1]);
    if (*ptr_assign != nullptr) {
        if (!(*ptr_assign)->value()->unwrap_conv(ConversionKind::StopGCTracking)->match_ld_loc(*v)) return false;
        // br targetBlock
        if (!(*not_null_block)->instructions[2]->match_branch(target_block)) return false;

        //  Block nullBlock (incoming: 1) {
        //      stloc ptr(conv i4->u <zero extend>(ldc.i4 0))
        //      br targetBlock
        //  }
        if ((*null_block)->incoming_edge_count() != 1) return false;
        if ((*null_block)->instructions.count() != 2) return false;
        ILInstruction* null_pointer_inst = nullptr;
        if (!(*null_block)->instructions[0]->match_st_loc((*ptr_assign)->variable(), &null_pointer_inst)) return false;
        if (!null_pointer_inst->match_ldc_i(std::int64_t{0})) return false;
        if (!(*null_block)->instructions[1]->match_branch(*target_block)) return false;
    } else {
        // br targetBlock
        if (!(*not_null_block)->instructions[1]->match_branch(target_block)) return false;

        if (*target_block != *null_block) return false;
        // nullBlock must be set to null, so that we do not clear out targetBlock in the caller.
        *null_block = nullptr;
    }
    return true;
}

// Detect the following pattern:
//   ...
//   stloc V(ldloc S)
//   if (comp(ldloc S == ldnull)) br B_null_or_empty
//   br B_not_null
// }
// Block B_not_null {
//   if (conv i->i4 (ldlen(ldloc V))) br B_not_null_and_not_empty
//   br B_null_or_empty
// }
// Block B_not_null_and_not_empty {
//   stloc P(ldelema(ldloc V, ldc.i4 0, ...))
//   br B_target
// }
// Block B_null_or_empty {
//   stloc P(conv i4->u(ldc.i4 0))
//   br B_target
// }
// And convert the whole thing into:
//   ...
//   stloc P(array.to.pointer(V))
//   br B_target
bool DetectPinnedRegions::is_null_safe_array_to_pointer_pattern(Block* block, ILVariable** v, ILVariable** p, Block** target_block) {
    *v = nullptr;
    *p = nullptr;
    *target_block = nullptr;
    // ...
    // if (comp(ldloc V == ldnull)) br B_null_or_empty
    // br B_not_null
    auto* if_inst = as<IfInstruction>(block->instructions.second_to_last_or_default());
    if (if_inst == nullptr) return false;
    auto* condition = as<Comp>(if_inst->condition());
    if (!(condition != nullptr && condition->kind() == ComparisonKind::Equality && condition->left()->match_ld_loc(v) &&
          condition->right()->match_ld_null()))
        return false;
    bool using_previous_var = false;
    if ((*v)->kind() == VariableKind::StackSlot) {
        // If the variable is a stack slot, that might be due to an inline assignment, so check the previous instruction:
        auto* previous = as<StLoc>(block->instructions.element_at_or_default(block->instructions.count() - 3));
        if (previous != nullptr && previous->value()->match_ld_loc(*v)) {
            // stloc V(ldloc S)
            // if (comp(ldloc S == ldnull)) ...
            *v = previous->variable();
            using_previous_var = true;
        }
    }
    Block* null_or_empty_block = nullptr;
    if (!if_inst->true_inst()->match_branch(&null_or_empty_block)) return false;
    if (!if_inst->false_inst()->match_nop()) return false;
    if (null_or_empty_block->parent() != block->parent()) return false;
    if (!is_null_safe_array_to_pointer_null_or_empty_block(null_or_empty_block, p, target_block)) return false;
    if (!((*p)->kind() == VariableKind::PinnedLocal || (using_previous_var && (*v)->kind() == VariableKind::PinnedLocal))) return false;
    Block* not_null_block = nullptr;
    if (!block->instructions.last()->match_branch(&not_null_block)) return false;
    if (not_null_block->parent() != block->parent()) return false;
    return is_null_safe_array_to_pointer_not_null_block(not_null_block, *v, *p, null_or_empty_block, *target_block);
}

bool DetectPinnedRegions::is_null_safe_array_to_pointer_not_null_block(Block* block, ILVariable* v, ILVariable* p, Block* null_or_empty_block,
                                                                       Block* target_block) {
    // Block B_not_null {
    //   if (conv i->i4 (ldlen(ldloc V))) br B_not_null_and_not_empty
    //   br B_null_or_empty
    // }
    if (block->instructions.count() != 2) return false;
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!block->instructions[0]->match_if_instruction(&condition, &true_inst)) return false;
    ILInstruction* false_inst = block->instructions[1];
    if (auto* comp = as<Comp>(condition); comp != nullptr && comp->right()->match_ldc_i(std::int64_t{0})) {
        if (comp->kind() == ComparisonKind::Equality) {
            // if (len == 0): effectively negates the condition
            condition = comp->left();
            std::swap(true_inst, false_inst);
        } else if (comp->kind() == ComparisonKind::Inequality) {
            // if (len != 0): comparison is redundant (equivalent to implicit non-zero check)
            condition = comp->left();
        } else {
            return false;
        }
    }
    condition = condition->unwrap_conv(ConversionKind::Truncate);
    ILInstruction* array = nullptr;
    CallInstruction* call = as<CallInstruction>(condition);
    if (condition->match_ld_len(StackType::I, &array)) {
        // OK
    } else if (call != nullptr && call->method->name() == "get_Length") {
        // Used instead of ldlen for multi-dimensional arrays
        if (!typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::Array)) return false;
        if (call->arguments().count() != 1) return false;
        array = call->arguments()[0];
    } else {
        return false;
    }
    if (!array->match_ld_loc(v)) return false;
    Block* not_null_and_not_empty_block = nullptr;
    if (!true_inst->match_branch(&not_null_and_not_empty_block)) return false;
    if (not_null_and_not_empty_block->parent() != block->parent()) return false;
    if (!is_null_safe_array_to_pointer_not_null_and_not_empty_block(not_null_and_not_empty_block, v, p, target_block)) return false;
    return false_inst->match_branch(null_or_empty_block);
}

bool DetectPinnedRegions::is_null_safe_array_to_pointer_not_null_and_not_empty_block(Block* block, ILVariable* v, ILVariable* p,
                                                                                     Block* target_block) {
    // Block B_not_null_and_not_empty {
    //   stloc P(ldelema(ldloc V, ldc.i4 0, ...))
    //   br B_target
    // }
    if (block->instructions.count() != 2) return false;
    ILVariable* p2 = nullptr;
    ILInstruction* value = nullptr;
    if (!block->instructions[0]->match_st_loc(&p2, &value)) return false;
    if (p != p2) {
        // If the pointer is unused, the variable P might have been split.
        if (p->load_count() == 0 && p->address_count() == 0 && p2->load_count() == 0 && p2->address_count() == 0) {
            if (!ILVariableEqualityComparer{}(p, p2)) return false;
        } else {
            return false;
        }
    }
    if (v->kind() == VariableKind::PinnedLocal) {
        value = value->unwrap_conv(ConversionKind::StopGCTracking);
    }
    auto* ldelema = as<LdElema>(value);
    if (ldelema == nullptr) return false;
    if (!ldelema->array()->match_ld_loc(v)) return false;
    for (ILInstruction* index : ldelema->indices())
        if (!index->match_ldc_i4(std::int32_t{0})) return false;
    return block->instructions[1]->match_branch(target_block);
}

bool DetectPinnedRegions::is_null_safe_array_to_pointer_null_or_empty_block(Block* block, ILVariable** p, Block** target_block) {
    *p = nullptr;
    *target_block = nullptr;
    // Block B_null_or_empty {
    //   stloc P(conv i4->u(ldc.i4 0))
    //   br B_target
    // }
    ILInstruction* value = nullptr;
    return block->instructions.count() == 2 && block->instructions[0]->match_st_loc(p, &value) &&
           ((*p)->kind() == VariableKind::PinnedLocal || (*p)->kind() == VariableKind::Local) && is_null_or_zero(value) &&
           block->instructions[1]->match_branch(target_block);
}

// ---- CreatePinnedRegion -------------------------------------------------------------------------------------------------------------

bool DetectPinnedRegions::detect_pinned_region(Block* block) {
    // After split_blocks_at_writes_to_pinned_locals(), only the second-to-last instruction in each block can be a write to a pinned local.
    auto* st_loc = as<StLoc>(block->instructions.second_to_last_or_default());
    if (st_loc == nullptr || st_loc->variable()->kind() != VariableKind::PinnedLocal) return false;
    // stLoc is a store to a pinned local.
    if (is_null_or_zero(st_loc->value())) {
        return false;  // ignore unpin instructions
    }
    if (st_loc->variable()->type()->is_reference_type() == std::optional<bool>(false)) {
        // `pinned` flag has no effect on value types (#2148)
        return false;
    }
    // stLoc is a store that starts a new pinned region

    context_->step_start_group("DetectPinnedRegion " + st_loc->variable()->name.value_or(""), block);
    bool result = create_pinned_region(block, st_loc);
    context_->step_end_group(true);
    return result;
}

bool DetectPinnedRegions::create_pinned_region(Block* block, StLoc* st_loc) {
    // Collect the blocks to be moved into the region:
    auto* source_container = static_cast<BlockContainer*>(block->parent());
    std::vector<int> reached_edges_per_block(static_cast<std::size_t>(source_container->blocks.count()), 0);
    std::deque<Block*> work_list;
    Block* entry_block = static_cast<Branch*>(block->instructions.last())->target_block();
    if (entry_block->parent() != source_container) {
        // we didn't find a single block to be added to the pinned region
        return false;
    }
    ILInstruction* ignored = nullptr;
    if (entry_block->instructions[0]->match_st_loc(st_loc->variable(), &ignored)) {
        // pinned region has empty body
    } else {
        reached_edges_per_block[static_cast<std::size_t>(entry_block->child_index())]++;
        work_list.push_back(entry_block);
    }
    while (!work_list.empty()) {
        Block* work_item = work_list.front();
        work_list.pop_front();
        for (ILInstruction* descendant : work_item->descendants()) {
            auto* branch = as<Branch>(descendant);
            if (branch == nullptr) continue;
            if (branch->target_block()->parent() == source_container) {
                if (branch->target_block()->instructions[0]->match_st_loc(st_loc->variable(), &ignored)) {
                    // Found unpin instruction
                    continue;
                }
                assert(branch->target_block() != block);
                if (reached_edges_per_block[static_cast<std::size_t>(branch->target_block()->child_index())]++ == 0) {
                    // detected first edge to that block: add block as work item
                    work_list.push_back(branch->target_block());
                }
            }
        }
    }

    // Validate that all uses of a block consistently are inside or outside the pinned region.
    // (we cannot do this anymore after we start moving blocks around)
    bool clone_blocks = false;
    for (int i = 0; i < source_container->blocks.count(); i++) {
        if (reached_edges_per_block[static_cast<std::size_t>(i)] != 0 &&
            reached_edges_per_block[static_cast<std::size_t>(i)] != source_container->blocks[i]->incoming_edge_count()) {
            // Don't abort in this case, we still need to somehow represent the pinned variable with a fixed statement.
            // We'll duplicate the code so that it can be both inside and outside the pinned region.
            clone_blocks = true;
            break;
        }
    }

    context_->step("CreatePinnedRegion", block);
    BlockContainer* body = make<BlockContainer>();
    std::vector<Block*> cloned_blocks;
    if (clone_blocks) cloned_blocks.assign(static_cast<std::size_t>(source_container->blocks.count()), nullptr);
    for (int i = 0; i < source_container->blocks.count(); i++) {
        if (reached_edges_per_block[static_cast<std::size_t>(i)] > 0) {
            Block* inner_block = source_container->blocks[i];
            if (clone_blocks) {
                inner_block = static_cast<Block*>(inner_block->clone());
                cloned_blocks[static_cast<std::size_t>(i)] = inner_block;
            }
            ILInstruction* ignored_condition = nullptr;
            ILInstruction* true_inst = nullptr;
            ILInstruction* false_inst = nullptr;
            if (inner_block->match_if_at_end_of_block(&ignored_condition, &true_inst, &false_inst)) {
                handle_branch_leaving_pinned_region(true_inst, reached_edges_per_block, source_container, st_loc->variable());
                handle_branch_leaving_pinned_region(false_inst, reached_edges_per_block, source_container, st_loc->variable());
            } else {
                handle_branch_leaving_pinned_region(inner_block->instructions.last_or_default(), reached_edges_per_block, source_container,
                                                    st_loc->variable());
            }

            // move block into body
            if (source_container->blocks[i] == entry_block) {
                // ensure entry point comes first
                body->blocks.insert(0, inner_block);
            } else {
                body->blocks.add(inner_block);
            }
            if (!clone_blocks) {
                source_container->blocks.set(i, make<Block>());  // replace with dummy block; we'll delete the dummy block later
            }
        }
    }
    if (body->blocks.count() == 0) {
        // empty body, the entryBlock itself doesn't belong into the pinned region
        assert(reached_edges_per_block[static_cast<std::size_t>(entry_block->child_index())] == 0);
        Block* body_block = make<Block>();
        body_block->set_il_range(*st_loc);
        body_block->instructions.add(make<Branch>(entry_block));
        body->blocks.add(body_block);
    }

    PinnedRegion* pinned_region = with_il_range(make<PinnedRegion>(st_loc->variable(), st_loc->value(), body), st_loc);
    st_loc->replace_with(pinned_region);
    block->instructions.remove_at(block->instructions.count() - 1);  // remove branch into body

    if (clone_blocks) {
        // Adjust branches between cloned blocks.
        for (ILInstruction* descendant : body->descendants()) {
            auto* branch = as<Branch>(descendant);
            if (branch == nullptr) continue;
            if (branch->target_container() == source_container) {
                int i = branch->target_block()->child_index();
                if (cloned_blocks[static_cast<std::size_t>(i)] != nullptr) {
                    branch->set_target_block(cloned_blocks[static_cast<std::size_t>(i)]);
                }
            }
        }
        // Replace unreachable blocks in sourceContainer with dummy blocks:
        std::vector<bool> is_alive(static_cast<std::size_t>(source_container->blocks.count()), false);
        std::vector<int> duplicated_block_start_offsets;
        for (Block* remaining_block : source_container->topological_sort(true)) {
            is_alive[static_cast<std::size_t>(remaining_block->child_index())] = true;
            if (cloned_blocks[static_cast<std::size_t>(remaining_block->child_index())] != nullptr) {
                duplicated_block_start_offsets.push_back(remaining_block->start_il_offset());
            }
        }
        for (std::size_t i = 0; i < is_alive.size(); i++) {
            if (!is_alive[i]) source_container->blocks.set(static_cast<int>(i), make<Block>());
        }
        // we'll delete the dummy blocks later
        assert(!duplicated_block_start_offsets.empty());
        std::sort(duplicated_block_start_offsets.begin(), duplicated_block_start_offsets.end());
        std::string message = "The blocks ";
        for (std::size_t i = 0; i < duplicated_block_start_offsets.size(); i++) {
            if (i > 0) message += ", ";
            message += std::format("IL_{:04x}", static_cast<unsigned>(duplicated_block_start_offsets[i]));
        }
        message += std::format(" are reachable both inside and outside the pinned region starting at IL_{:04x}.",
                               static_cast<unsigned>(st_loc->start_il_offset()));
        message += " ILSpy has duplicated these blocks in order to place them both within and outside the `fixed` statement.";
        context_->function()->warnings.push_back(std::move(message));
    }

    process_pinned_region(pinned_region);
    return true;
}

void DetectPinnedRegions::handle_branch_leaving_pinned_region(ILInstruction* potential_branch, const std::vector<int>& reached_edges_per_block,
                                                              BlockContainer* source_container, ILVariable* pinned_region_var) {
    auto* branch = as<Branch>(potential_branch);
    if (branch != nullptr && branch->target_block()->incoming_edge_count() == 1 && branch->target_container() == source_container &&
        reached_edges_per_block[static_cast<std::size_t>(branch->target_block()->child_index())] == 0) {
        // branch that leaves body.
        // The target block should have an instruction that resets the pin; delete that instruction:
        auto* unpin = as<StLoc>(branch->target_block()->instructions.first());
        if (unpin != nullptr && unpin->variable() == pinned_region_var && is_null_or_zero(unpin->value())) {
            branch->target_block()->instructions.remove_at(0);
        }
    }
}

bool DetectPinnedRegions::is_null_or_zero(ILInstruction* inst) {
    while (auto* conv = as<Conv>(inst)) inst = conv->argument();
    return inst->match_ldc_i4(std::int32_t{0}) || inst->match_ld_null();
}

// ---- ProcessPinnedRegion ------------------------------------------------------------------------------------------------------------

void DetectPinnedRegions::process_pinned_region(PinnedRegion* pinned_region) {
    if (pinned_region->variable()->type()->kind() == typesystem::TypeKind::ByReference) {
        // C# doesn't support a "by reference" variable, so replace it with a native pointer
        context_->step("Replace pinned ref-local with native pointer", pinned_region);
        ILVariable* old_var = pinned_region->variable();
        const IType* element_type = dynamic_cast<const typesystem::ByReferenceType*>(old_var->type())->element_type();
        ILInstruction* field_target = nullptr;
        const IField* field = nullptr;
        if (element_type->kind() == typesystem::TypeKind::Pointer && pinned_region->init()->match_ld_flda(&field_target, &field) &&
            typesystem::type_equals(dynamic_cast<const typesystem::PointerType*>(element_type)->element_type(), field->type())) {
            // Roslyn 2.6 (C# 7.2) uses type "int*&" for the pinned local referring to a fixed field of type "int".
            // Remove the extra level of indirection.
            element_type = dynamic_cast<const typesystem::PointerType*>(element_type)->element_type();
        }
        ILVariable* new_var = make<ILVariable>(VariableKind::PinnedRegionLocal, typesystem::make_pointer_type(element_type), old_var->index());
        new_var->name = old_var->name;
        new_var->has_generated_name = old_var->has_generated_name;
        old_var->function()->variables().add(new_var);
        replace_pinned_var(old_var, new_var, pinned_region);
        use_existing_variable_for_pinned_region(pinned_region);
    } else if (pinned_region->variable()->type()->kind() == typesystem::TypeKind::Array) {
        context_->step("Replace pinned array with native pointer", pinned_region);
        move_array_to_pointer_to_pinned_region_init(pinned_region);
        use_existing_variable_for_pinned_region(pinned_region);
    } else if (typesystem::is_known_type(pinned_region->variable()->type(), KnownTypeCode::String)) {
        // fixing a string
        handle_string_to_pointer(pinned_region);
    } else if (auto* init_conv = as<Conv>(pinned_region->init()); init_conv != nullptr && init_conv->kind == ConversionKind::StopGCTracking) {
        // If pinnedRegion.Variable was already a pointer type, the input IL has a StopGCTracking conversion.
        // We can simply remove this conversion, as it is not needed.
        context_->step("Remove StopGCTracking conversion", pinned_region);
        pinned_region->set_init(init_conv->argument());
    }
    // Detect nested pinned regions:
    auto* body = static_cast<BlockContainer*>(pinned_region->body());
    for (Block* block : body->blocks) detect_pinned_region(block);
    body->blocks.remove_all([](Block* b) { return b->instructions.count() == 0; });  // remove dummy blocks
    body->set_il_range(*body->entry_point());
    if (pinned_region->variable()->kind() != VariableKind::PinnedRegionLocal) {
        assert(pinned_region->variable()->kind() == VariableKind::PinnedLocal);
        pinned_region->variable()->set_kind(VariableKind::PinnedRegionLocal);
    }
}

void DetectPinnedRegions::move_array_to_pointer_to_pinned_region_init(PinnedRegion* pinned_region) {
    // Roslyn started marking the array variable as pinned, and then uses array.to.pointer immediately within the region.
    assert(pinned_region->variable()->type()->kind() == typesystem::TypeKind::Array);
    // Find the single load of the variable within the pinnedRegion:
    LdLoc* ldloc = nullptr;
    for (ILInstruction* descendant : pinned_region->descendants()) {
        auto* inst = dynamic_cast<IInstructionWithVariableOperand*>(descendant);
        if (inst == nullptr) continue;
        if (inst->variable() == pinned_region->variable() && descendant != pinned_region) {
            if (ldloc != nullptr) return;  // more than 1 variable access
            ldloc = as<LdLoc>(descendant);
            if (ldloc == nullptr) return;  // variable access that is not LdLoc
        }
    }
    if (ldloc == nullptr) return;
    auto* array_to_pointer = as<GetPinnableReference>(ldloc->parent());
    if (array_to_pointer == nullptr) return;
    auto* conv = as<Conv>(array_to_pointer->parent());
    if (!(conv != nullptr && conv->kind == ConversionKind::StopGCTracking)) return;
    assert(array_to_pointer->is_descendant_of(pinned_region));
    ILVariable* old_var = pinned_region->variable();
    ILVariable* new_var = make<ILVariable>(
        VariableKind::PinnedRegionLocal,
        typesystem::make_pointer_type(dynamic_cast<const typesystem::ArrayType*>(old_var->type())->element_type()), old_var->index());
    new_var->name = old_var->name;
    new_var->has_generated_name = old_var->has_generated_name;
    old_var->function()->variables().add(new_var);
    pinned_region->set_variable(new_var);
    pinned_region->set_init(with_il_range(make<GetPinnableReference>(pinned_region->init(), array_to_pointer->method()), array_to_pointer));
    conv->replace_with(with_il_range(make<LdLoc>(new_var), conv));
}

void DetectPinnedRegions::replace_pinned_var(ILVariable* old_var, ILVariable* new_var, ILInstruction* inst) {
    assert(new_var->stack_type == StackType::I);
    auto* conv = as<Conv>(inst);
    if (conv != nullptr && conv->kind == ConversionKind::StopGCTracking && conv->argument()->match_ld_loc(old_var) &&
        conv->result_type() == new_var->stack_type) {
        // conv ref->i (ldloc oldVar)
        //  => ldloc newVar
        conv->add_il_range(*conv->argument());
        conv->replace_with(with_il_range(make<LdLoc>(new_var), conv));
        return;
    }
    auto* iwvo = dynamic_cast<IInstructionWithVariableOperand*>(inst);
    std::string val;
    if (iwvo != nullptr && iwvo->variable() == old_var) {
        iwvo->set_variable(new_var);
        if (auto* stloc = as<StLoc>(inst); stloc != nullptr && old_var->type()->kind() == typesystem::TypeKind::ByReference) {
            stloc->set_value(make<Conv>(stloc->value(), PrimitiveType::I, false, Sign::None));
        }
        if ((is<LdLoc>(inst) || is<StLoc>(inst)) && !is_slot_accepting_both_managed_and_unmanaged_pointers(inst->slot_info()) &&
            old_var->stack_type != StackType::I) {
            // wrap inst in Conv, so that the stack types match up
            ILInstruction* parent = inst->parent();
            int child_index = inst->child_index();
            parent->children().set(child_index, make<Conv>(inst, typesystem::to_primitive_type(old_var->stack_type), false, Sign::None));
        }
    } else if (inst->match_ld_str(&val) && val == "Is this ILSpy?") {
        inst->replace_with(make<LdStr>("This is ILSpy!"));  // easter egg ;)
        return;
    }
    for (ILInstruction* child : inst->children()) replace_pinned_var(old_var, new_var, child);
}

bool DetectPinnedRegions::is_slot_accepting_both_managed_and_unmanaged_pointers(const SlotInfo* slot_info) {
    return slot_info == &Block::instruction_slot || slot_info == &LdObj::target_slot || slot_info == &StObj::target_slot;
}

bool DetectPinnedRegions::is_branch_on_null(ILInstruction* cond_branch, ILVariable* native_var, Block** target_block) {
    *target_block = nullptr;
    // if (comp(ldloc nativeVar == conv i4->i <sign extend>(ldc.i4 0))) br targetBlock
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    return cond_branch->match_if_instruction(&condition, &true_inst) && condition->match_comp_equals(&left, &right) &&
           left->match_ld_loc(native_var) && is_null_or_zero(right) && true_inst->match_branch(target_block);
}

void DetectPinnedRegions::handle_string_to_pointer(PinnedRegion* pinned_region) {
    assert(typesystem::is_known_type(pinned_region->variable()->type(), KnownTypeCode::String));
    auto* body = static_cast<BlockContainer*>(pinned_region->body());
    if (body->entry_point()->incoming_edge_count() != 1) return;
    // stloc nativeVar(conv o->i (ldloc pinnedVar))
    // if (comp(ldloc nativeVar == conv i4->i <sign extend>(ldc.i4 0))) br targetBlock
    // br adjustOffsetToStringData
    ILVariable* new_var;
    ILVariable* native_var = nullptr;
    ILInstruction* init_inst = nullptr;
    if (!body->entry_point()->instructions[0]->match_st_loc(&native_var, &init_inst)) {
        // potentially a special case with legacy csc and an unused pinned variable:
        if (pinned_region->variable()->address_count() == 0 && pinned_region->variable()->load_count() == 0) {
            const IType* char_ptr = typesystem::make_pointer_type(context_->type_system().find_type(KnownTypeCode::Char));
            new_var = make<ILVariable>(VariableKind::PinnedRegionLocal, char_ptr, pinned_region->variable()->index());
            new_var->name = pinned_region->variable()->name;
            new_var->has_generated_name = pinned_region->variable()->has_generated_name;
            pinned_region->variable()->function()->variables().add(new_var);
            pinned_region->set_variable(new_var);
            pinned_region->set_init(make<GetPinnableReference>(pinned_region->init(), nullptr));
        }
        return;
    }
    if (typesystem::get_stack_type(*native_var->type()) != StackType::I) return;

    Block* target_block = nullptr;
    Block* adjust_offset_to_string_data = nullptr;
    if (body->entry_point()->instructions.count() == 2) {
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        if (!init_inst->match_binary_numeric_instruction(BinaryNumericOperator::Add, &left, &right)) return;
        if (!left->unwrap_conv(ConversionKind::StopGCTracking)->match_ld_loc(pinned_region->variable())) return;
        if (!is_offset_to_string_data_call(right)) return;
        if (!body->entry_point()->instructions[1]->match_branch(&target_block)) return;
    } else if (body->entry_point()->instructions.count() == 3) {
        if (!init_inst->unwrap_conv(ConversionKind::StopGCTracking)->match_ld_loc(pinned_region->variable())) return;
        if (!is_branch_on_null(body->entry_point()->instructions[1], native_var, &target_block)) return;
        if (!body->entry_point()->instructions[2]->match_branch(&adjust_offset_to_string_data)) return;
        if (!(adjust_offset_to_string_data->parent() == body && adjust_offset_to_string_data->incoming_edge_count() == 1 &&
              is_offset_to_string_data_block(adjust_offset_to_string_data, native_var, target_block)))
            return;
    } else {
        return;
    }

    context_->step("Handle pinned string (with adjustOffsetToStringData)", pinned_region);
    if (target_block->parent() == body) {
        // remove old entry point
        body->blocks.remove_at(0);
        if (adjust_offset_to_string_data != nullptr) body->blocks.remove_at(adjust_offset_to_string_data->child_index());
        // make targetBlock the new entry point
        body->blocks.remove_at(target_block->child_index());
        body->blocks.insert(0, target_block);
    } else {
        // pinned region has empty body, immediately jumps to targetBlock which is outside
        body->blocks[0]->instructions.clear();
        body->blocks.remove_range(1, body->blocks.count() - 1);
        body->blocks[0]->instructions.add(make<Branch>(target_block));
    }
    pinned_region->set_init(make<GetPinnableReference>(pinned_region->init(), nullptr));

    ILVariable* other_var = nullptr;
    ILInstruction* other_var_init = nullptr;
    // In optimized builds, the 'nativeVar' may end up being a stack slot,
    // and only gets assigned to a real variable after the offset adjustment.
    if (native_var->kind() == VariableKind::StackSlot && native_var->load_count() == 1 &&
        body->entry_point()->instructions[0]->match_st_loc(&other_var, &other_var_init) && other_var_init->match_ld_loc(native_var) &&
        other_var->is_single_definition()) {
        body->entry_point()->instructions.remove_at(0);
        native_var = other_var;
    }
    if (native_var->kind() == VariableKind::Local) {
        new_var = make<ILVariable>(VariableKind::PinnedRegionLocal, native_var->type(), native_var->index());
        new_var->name = native_var->name;
        new_var->has_generated_name = native_var->has_generated_name;
        native_var->function()->variables().add(new_var);
        replace_pinned_var(native_var, new_var, pinned_region);
    } else {
        new_var = native_var;
    }
    replace_pinned_var(pinned_region->variable(), new_var, pinned_region);
}

bool DetectPinnedRegions::is_offset_to_string_data_block(Block* block, ILVariable* native_var, Block* target_block) {
    // stloc nativeVar(add(ldloc nativeVar, conv i4->i <sign extend>(call [Accessor System.Runtime.CompilerServices.RuntimeHelpers.get_OffsetToStringData():System.Int32]())))
    // br IL_0011
    if (block->instructions.count() != 2) return false;
    ILInstruction* value = nullptr;
    if (native_var->is_single_definition() && native_var->load_count() == 2) {
        // If there are no loads (except for the two in the string-to-pointer pattern), then we might have split nativeVar:
        ILVariable* other_var = nullptr;
        if (!block->instructions[0]->match_st_loc(&other_var, &value)) return false;
        if (!(other_var->is_single_definition() && other_var->load_count() == 0)) return false;
    } else if (native_var->store_count() == 2) {
        // normal case with non-split variable
        if (!block->instructions[0]->match_st_loc(native_var, &value)) return false;
    } else {
        return false;
    }
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    if (!value->match_binary_numeric_instruction(BinaryNumericOperator::Add, &left, &right)) return false;
    if (!left->match_ld_loc(native_var)) return false;
    if (!is_offset_to_string_data_call(right)) return false;
    return block->instructions[1]->match_branch(target_block);
}

bool DetectPinnedRegions::is_offset_to_string_data_call(ILInstruction* inst) {
    auto* call = as<Call>(inst->unwrap_conv(ConversionKind::SignExtend));
    return call != nullptr && call->method->full_name() == "System.Runtime.CompilerServices.RuntimeHelpers.get_OffsetToStringData";
}

// Modifies a pinned region to eliminate an extra local variable that roslyn tends to generate.
void DetectPinnedRegions::use_existing_variable_for_pinned_region(PinnedRegion* pinned_region) {
    // PinnedRegion V_1(..., BlockContainer {
    // Block IL_0000(incoming: 1) {
    //    stloc V_0(ldloc V_1)
    //    ...
    auto* body = as<BlockContainer>(pinned_region->body());
    if (body == nullptr) return;
    if (pinned_region->variable()->load_count() != 1) return;
    ILVariable* v = nullptr;
    ILInstruction* init = nullptr;
    if (!body->entry_point()->instructions[0]->match_st_loc(&v, &init)) return;
    if (!init->match_ld_loc(pinned_region->variable())) return;
    if (!(v->is_single_definition() && typesystem::type_equals(v->type(), pinned_region->variable()->type()))) return;
    if (v->kind() != VariableKind::Local) return;
    // replace V_1 with V_0
    v->set_kind(VariableKind::PinnedRegionLocal);
    pinned_region->set_variable(v);
    body->entry_point()->instructions.remove_at(0);
}

}  // namespace cb::dotnet::decompiler::il::control_flow
