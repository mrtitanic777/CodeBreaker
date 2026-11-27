// dotnet/decompiler/il/transforms/high_level_loop_transform.cpp -- see high_level_loop_transform.hpp.
#include "dotnet/decompiler/il/transforms/high_level_loop_transform.hpp"

#include <algorithm>
#include <cassert>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/condition_detection.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/il/transforms/expression_transforms.hpp"

namespace cb::dotnet::decompiler::il::transforms {

namespace {

// ExpressionTransforms.RunOnSingleStatement
void run_on_single_statement(ILInstruction* statement, ILTransformContext& context) {
    ExpressionTransforms::run_on_single_statement(statement, context);
}

}  // namespace

void HighLevelLoopTransform::run(ILFunction* function, ILTransformContext& context) {
    context_ = &context;

    for (ILInstruction* inst : function->descendants()) {
        auto* loop = as<BlockContainer>(inst);
        if (loop == nullptr) continue;
        if (loop->kind() != ContainerKind::Loop) continue;
        IfInstruction* condition = nullptr;
        Block* loop_body = nullptr;
        if (match_while_loop(loop, &condition, &loop_body)) {
            if (context.settings().for_statement) match_for_loop(loop, condition, loop_body);
            continue;
        }
        if (context.settings().do_while_statement && match_do_while_loop(loop)) continue;
    }
}

bool HighLevelLoopTransform::match_while_loop(BlockContainer* loop, IfInstruction** condition, Block** loop_body) {
    // ConditionDetection favours leave inside if and branch at end of block
    // while-loop:
    // if (!loop-condition) leave loop-container
    // ...
    *condition = nullptr;
    *loop_body = loop->entry_point();
    auto* if_instruction = as<IfInstruction>((*loop_body)->instructions[0]);
    if (if_instruction == nullptr) return false;

    if (!if_instruction->false_inst()->match_nop()) return false;

    if (uses_variable_captured_in_loop(loop, if_instruction->condition())) return false;

    *condition = if_instruction;
    if (!if_instruction->true_inst()->match_leave(static_cast<const BlockContainer*>(loop))) {
        // sometimes the loop-body is nested within the if
        // if (loop-condition) { loop-body }
        // leave loop-container

        if ((*loop_body)->instructions.count() != 2 || !loop->entry_point()->instructions.last()->match_leave(static_cast<const BlockContainer*>(loop)))
            return false;

        if (!if_instruction->true_inst()->has_flag(InstructionFlags::EndPointUnreachable))
            static_cast<Block*>(if_instruction->true_inst())->instructions.add(make<Leave>(loop));

        control_flow::ConditionDetection::invert_if(*loop_body, if_instruction, *context_);
    }

    context_->step("Transform to while (condition) loop: " + loop->entry_point()->label(), loop);
    loop->set_kind(ContainerKind::While);
    // invert comparison
    if_instruction->set_condition(Comp::logic_not(if_instruction->condition()));
    if_instruction->set_false_inst(if_instruction->true_inst());
    // move the rest of the body into a new block
    *loop_body = make<Block>();
    (*loop_body)->add_ref();
    control_flow::ConditionDetection::extract_block(loop->entry_point(), 1, loop->entry_point()->instructions.count(), *loop_body);
    loop->blocks.insert(1, *loop_body);
    (*loop_body)->release_ref();
    if (!(*loop_body)->has_flag(InstructionFlags::EndPointUnreachable)) (*loop_body)->instructions.add(make<Leave>(loop));

    if_instruction->set_true_inst(make<Branch>(*loop_body));
    run_on_single_statement(if_instruction, *context_);

    // Analyze conditions and decide whether to move some of them out of the condition block:
    /*var conditions = new List<ILInstruction>();
    SplitConditions(condition.Condition, conditions);
    // Break apart conditions that could be a MoveNext call followed by a Current accessor call:
    if (MightBeHeaderOfForEach(loop, conditions)) {
        ifInstruction.Condition = conditions[0];
        foreach (var cond in conditions.Skip(1).Reverse()) {
            IfInstruction inst;
            loopBody.Instructions.Insert(0, inst = new IfInstruction(Comp.LogicNot(cond), new Leave(loop)));
            ExpressionTransforms.RunOnSingleStatment(inst, context);
        }
    }*/

    return true;
}

bool HighLevelLoopTransform::might_be_header_of_for_each(BlockContainer* loop, const std::vector<ILInstruction*>& conditions) {
    if (conditions.size() <= 1) return false;
    auto* move_next_call = as<CallInstruction>(conditions[0]);
    if (!(move_next_call != nullptr && move_next_call->method->name() == "MoveNext")) return false;
    bool has_get_current = false;
    for (ILInstruction* inst : conditions[1]->descendants()) {
        auto* getter_call = as<CallInstruction>(inst);
        if (getter_call != nullptr && getter_call->method->is_accessor() && getter_call->method->name() == "get_Current") {
            has_get_current = true;
            break;
        }
    }
    if (!has_get_current) return false;
    ILInstruction* p = loop->parent();
    if (p != nullptr) p = p->parent();
    if (p != nullptr) p = p->parent();
    return as<UsingInstruction>(p) != nullptr;
}

void HighLevelLoopTransform::split_conditions(ILInstruction* expression, std::vector<ILInstruction*>& conditions) {
    ILInstruction* l = nullptr;
    ILInstruction* r = nullptr;
    if (expression->match_logic_and(&l, &r)) {
        split_conditions(l, conditions);
        split_conditions(r, conditions);
    } else {
        conditions.push_back(expression);
    }
}

/// Matches a do-while loop and performs the following transformations:
/// - combine all compatible conditions into one IfInstruction.
/// - extract conditions into a condition block, or move the existing condition block to the end.
bool HighLevelLoopTransform::match_do_while_loop(BlockContainer* loop) {
    DoWhileConditions analysis = analyze_do_while_conditions(loop);
    std::vector<IfInstruction*>& conditions = analysis.conditions;
    ILInstruction* exit = analysis.exit;
    bool swap = analysis.swap;
    bool split = analysis.split;
    bool unwrap = analysis.unwrap;
    // not a do-while loop, exit.
    if (!analysis.found || conditions.empty()) return false;
    context_->step("Transform to do-while loop: " + loop->entry_point()->label(), loop);
    Block* condition_block;
    // first we remove all extracted instructions from the original block.
    auto* original_block = static_cast<Block*>(exit->parent());
    if (unwrap) {
        // we found a condition block nested in a condition that is followed by a return statement:
        // we flip the condition and swap the blocks
        assert(as<IfInstruction>(original_block->parent()) != nullptr);
        auto* return_condition = static_cast<IfInstruction*>(original_block->parent());
        auto* top_level_block = static_cast<Block*>(return_condition->parent());
        assert(top_level_block->parent() == loop);
        ILInstruction* leave_function = top_level_block->instructions[return_condition->child_index() + 1];
        [[maybe_unused]] ILInstruction* return_value = nullptr;
        assert(leave_function->match_return(&return_value));
        return_condition->set_condition(Comp::logic_not(return_condition->condition()));
        return_condition->set_true_inst(leave_function);
        // simplify the condition:
        run_on_single_statement(return_condition, *context_);
        top_level_block->instructions.remove_at(return_condition->child_index() + 1);
        top_level_block->instructions.add_range(std::vector<ILInstruction*>(original_block->instructions.begin(), original_block->instructions.end()));
        original_block = top_level_block;
        split = true;
    }
    original_block->instructions.remove_range(original_block->instructions.count() - static_cast<int>(conditions.size()) - 1,
                                              static_cast<int>(conditions.size()) + 1);
    // we need to split the block:
    if (split) {
        // add a new block at the end and add a branch to the new block.
        condition_block = make<Block>();
        loop->blocks.add(condition_block);
        original_block->instructions.add(make<Branch>(condition_block));
    } else {
        // move the condition block to the end.
        condition_block = original_block;
        loop->blocks.move_element_to_end(original_block);
    }
    // combine all conditions and the exit instruction into one IfInstruction:
    IfInstruction* condition = nullptr;
    condition_block->add_il_range(*exit);
    for (IfInstruction* inst : conditions) {
        condition_block->add_il_range(*inst);
        if (condition == nullptr) {
            condition = inst;
            if (swap) {
                // branches must be swapped and condition negated:
                condition->set_condition(Comp::logic_not(condition->condition()));
                condition->set_false_inst(condition->true_inst());
                condition->set_true_inst(exit);
            } else {
                condition->set_false_inst(exit);
            }
        } else {
            if (swap) {
                condition->set_condition(IfInstruction::logic_and(Comp::logic_not(inst->condition()), condition->condition()));
            } else {
                condition->set_condition(IfInstruction::logic_and(inst->condition(), condition->condition()));
            }
        }
    }
    // insert the combined conditions into the condition block:
    condition_block->instructions.add(condition);
    // simplify the condition:
    run_on_single_statement(condition, *context_);
    // transform complete
    loop->set_kind(ContainerKind::DoWhile);
    return true;
}

HighLevelLoopTransform::DoWhileConditions HighLevelLoopTransform::analyze_do_while_conditions(BlockContainer* loop) {
    // we iterate over all blocks from the bottom, because the entry-point
    // should only be considered as condition block, if there are no other blocks.
    std::vector<Block*> blocks(loop->blocks.begin(), loop->blocks.end());
    for (auto it = blocks.rbegin(); it != blocks.rend(); ++it) {
        Block* block = *it;
        // first we match the end of the block:
        bool swap = false;
        bool unwrap_condition_block = false;
        Block* condition_block = nullptr;
        if (match_do_while_condition_block(loop, block, &swap, &unwrap_condition_block, &condition_block)) {
            // now collect all instructions that are usable as loop conditions
            DoWhileConditions result;
            result.conditions = collect_conditions(loop, condition_block, swap);
            result.found = true;
            // split only if the block is either the entry-point or contains other instructions as well.
            result.split = condition_block == loop->entry_point() ||
                           condition_block->instructions.count() > static_cast<int>(result.conditions.size()) + 1;  // + 1 is the final leave/branch.
            result.exit = condition_block->instructions.last();
            result.swap = swap;
            result.unwrap = unwrap_condition_block;
            return result;
        }
    }
    return DoWhileConditions{};
}

/// Returns a list of all IfInstructions that can be used as loop conditon, i.e.,
/// that have no false-instruction and have leave loop (if swapped) or branch entry-point as true-instruction.
std::vector<IfInstruction*> HighLevelLoopTransform::collect_conditions(BlockContainer* loop, Block* block, bool swap) {
    std::vector<IfInstruction*> list;
    int i = block->instructions.count() - 2;
    while (i >= 0) {
        auto* if_inst = as<IfInstruction>(block->instructions[i]);
        if (if_inst == nullptr) break;
        if (!if_inst->false_inst()->match_nop()) break;
        if (uses_variable_captured_in_loop(loop, if_inst->condition())) break;
        if (swap) {
            if (!if_inst->true_inst()->match_leave(static_cast<const BlockContainer*>(loop))) break;
            list.push_back(if_inst);
        } else {
            if (!if_inst->true_inst()->match_branch(static_cast<const Block*>(loop->entry_point()))) break;
            list.push_back(if_inst);
        }
        i--;
    }

    return list;
}

bool HighLevelLoopTransform::uses_variable_captured_in_loop(BlockContainer* loop, ILInstruction* condition) {
    for (ILInstruction* inst : condition->descendants()) {
        if (auto* with_variable = as<IInstructionWithVariableOperand>(inst)) {
            if (with_variable->variable()->capture_scope == loop) return true;
        }
    }
    return false;
}

bool HighLevelLoopTransform::match_do_while_condition_block(BlockContainer* loop, Block* block, bool* swap_branches, bool* unwrap_condition_block,
                                                            Block** condition_block) {
    // match the end of the block:
    // if (condition) branch entry-point else nop
    // leave loop
    // -or-
    // if (condition) leave loop else nop
    // branch entry-point
    *swap_branches = false;
    *unwrap_condition_block = false;
    *condition_block = block;
    // empty block?
    if (block->instructions.count() < 2) return false;
    ILInstruction* last = block->instructions.last();
    auto* if_instruction = as<IfInstruction>(block->instructions.second_to_last_or_default());
    // no IfInstruction or already transformed?
    if (if_instruction == nullptr || !if_instruction->false_inst()->match_nop()) return false;
    // the block ends in a return statement preceeded by an IfInstruction
    // take a look at the nested block and check if that might be a condition block
    ILInstruction* ignored = nullptr;
    if (last->match_return(&ignored)) {
        if (auto* nested_condition_block = as<Block>(if_instruction->true_inst())) {
            if (nested_condition_block->instructions.count() < 2) return false;
            last = nested_condition_block->instructions.last();
            if_instruction = as<IfInstruction>(nested_condition_block->instructions.second_to_last_or_default());
            if (if_instruction == nullptr || !if_instruction->false_inst()->match_nop()) return false;
            *unwrap_condition_block = true;
            *condition_block = nested_condition_block;
        }
    }
    // if the last instruction is a branch
    // we assume the branch instructions need to be swapped.
    if (last->match_branch(static_cast<const Block*>(loop->entry_point())))
        *swap_branches = true;
    else if (last->match_leave(static_cast<const BlockContainer*>(loop)))
        *swap_branches = false;
    else
        return false;
    // match the IfInstruction
    if (*swap_branches) {
        if (!if_instruction->true_inst()->match_leave(static_cast<const BlockContainer*>(loop))) return false;
    } else {
        if (!if_instruction->true_inst()->match_branch(static_cast<const Block*>(loop->entry_point()))) return false;
    }
    return true;
}

// early match before block containers have been constructed
bool HighLevelLoopTransform::match_do_while_condition_block(Block* block, Block** target1, Block** target2) {
    *target1 = *target2 = nullptr;
    if (block->instructions.count() < 2) return false;

    ILInstruction* last = block->instructions.last();
    auto* if_instruction = as<IfInstruction>(block->instructions.second_to_last_or_default());
    if (if_instruction == nullptr || !if_instruction->false_inst()->match_nop()) return false;

    ILInstruction* ignored = nullptr;
    return (if_instruction->true_inst()->match_branch(target1) || if_instruction->true_inst()->match_return(&ignored)) &&
           (last->match_branch(target2) || last->match_return(&ignored));
}

Block* HighLevelLoopTransform::get_increment_block(BlockContainer* loop, Block* while_loop_body) {
    // loop.Blocks.SingleOrDefault(...): none gives null, more than one is an error
    Block* result = nullptr;
    bool found = false;
    for (Block* b : loop->blocks) {
        if (!(b != while_loop_body && b->instructions.last()->match_branch(static_cast<const Block*>(loop->entry_point())))) continue;
        bool all_simple = true;
        for (int i = 0; i < b->instructions.count() - 1; i++) {
            if (!is_simple_statement(b->instructions[i])) {
                all_simple = false;
                break;
            }
        }
        if (!all_simple) continue;
        if (found) throw UsageError("Sequence contains more than one matching element");
        found = true;
        result = b;
    }
    return result;
}

bool HighLevelLoopTransform::match_increment_block(Block* block, Block** loop_head) {
    if (!block->instructions.last()->match_branch(loop_head)) return false;
    for (int i = 0; i < block->instructions.count() - 1; i++) {
        if (!is_simple_statement(block->instructions[i])) return false;
    }
    return true;
}

bool HighLevelLoopTransform::match_for_loop(BlockContainer* loop, IfInstruction* while_condition, Block* while_loop_body) {
    // for loops have exactly two incoming edges at the entry point.
    if (loop->entry_point()->incoming_edge_count() != 2) return false;
    // try to find an increment block:
    // consists of simple statements only.
    Block* increment_block = get_increment_block(loop, while_loop_body);
    if (increment_block != nullptr) {
        // we found a possible increment block, just make sure, that there are at least three blocks:
        // - condition block
        // - loop body
        // - increment block
        if (increment_block->instructions.count() <= 1 || loop->blocks.count() < 3) return false;
        if (stores_ref_local_used_after_loop(loop, increment_block)) return false;
        context_->step("Transform to for loop: " + loop->entry_point()->label(), loop);
        // move the block to the end of the loop:
        loop->blocks.move_element_to_end(increment_block);
        loop->set_kind(ContainerKind::For);
    } else {
        // we need to move the increment statements into its own block:
        // last must be a branch entry-point
        ILInstruction* last = while_loop_body->instructions.last_or_default();
        ILInstruction* second_to_last = while_loop_body->instructions.second_to_last_or_default();
        if (last == nullptr || second_to_last == nullptr) return false;
        if (!last->match_branch(static_cast<const Block*>(loop->entry_point()))) return false;
        // we only deal with 'numeric' increments
        ILVariable* increment_variable = nullptr;
        if (!match_increment(second_to_last, &increment_variable)) return false;
        // the increment variable must be local/stack variable
        if (increment_variable->kind() == VariableKind::Parameter) return false;
        // split conditions:
        std::vector<ILInstruction*> conditions;
        split_conditions(while_condition->condition(), conditions);
        IfInstruction* for_condition = nullptr;
        int number_of_conditions = 0;
        for (ILInstruction* condition : conditions) {
            // the increment variable must be used in the condition
            bool uses_increment_variable = false;
            for (ILInstruction* inst : condition->descendants()) {
                if (inst->match_ld_loc(static_cast<const ILVariable*>(increment_variable))) {
                    uses_increment_variable = true;
                    break;
                }
            }
            if (!uses_increment_variable) break;
            // condition should not contain an assignment
            bool contains_assignment = false;
            for (ILInstruction* inst : condition->descendants()) {
                if (is_assignment(inst)) {
                    contains_assignment = true;
                    break;
                }
            }
            if (contains_assignment) break;
            if (for_condition == nullptr) {
                for_condition = make<IfInstruction>(condition, while_condition->true_inst(), while_condition->false_inst());
            } else {
                for_condition->set_condition(IfInstruction::logic_and(for_condition->condition(), condition));
            }
            number_of_conditions++;
        }
        if (number_of_conditions == 0) return false;
        context_->step("Transform to for loop: " + loop->entry_point()->label(), loop);
        // split condition block:
        while_condition->replace_with(for_condition);
        run_on_single_statement(for_condition, *context_);
        for (int i = static_cast<int>(conditions.size()) - 1; i >= number_of_conditions; i--) {
            IfInstruction* inst = make<IfInstruction>(Comp::logic_not(conditions[static_cast<std::size_t>(i)]), make<Leave>(loop));
            while_loop_body->instructions.insert(0, inst);
            run_on_single_statement(inst, *context_);
        }
        // create a new increment block and add it at the end:
        int second_to_last_index = second_to_last->child_index();
        auto* new_increment_block = make<Block>();
        loop->blocks.add(new_increment_block);
        // move the increment instruction:
        new_increment_block->instructions.add(second_to_last);
        new_increment_block->instructions.add(last);
        new_increment_block->add_il_range(*second_to_last);
        while_loop_body->instructions.remove_range(second_to_last_index, 2);
        while_loop_body->instructions.add(make<Branch>(new_increment_block));
        // complete transform.
        loop->set_kind(ContainerKind::For);
    }
    return true;
}

bool HighLevelLoopTransform::stores_ref_local_used_after_loop(BlockContainer* loop, Block* increment_block) {
    for (int i = 0; i < increment_block->instructions.count() - 1; i++) {
        for (ILInstruction* inst : increment_block->instructions[i]->descendants()) {
            auto* store = as<StLoc>(inst);
            if (store == nullptr) continue;
            ILVariable* variable = store->variable();
            if (variable->type()->is_by_ref_like() && is_variable_used_after_loop(loop, variable)) return true;
        }
    }
    return false;
}

bool HighLevelLoopTransform::is_variable_used_after_loop(BlockContainer* loop, ILVariable* variable) {
    std::vector<ILInstruction*> uses;
    for (LdLoc* load : variable->load_instructions()) uses.push_back(load);
    for (LdLoca* address : variable->address_instructions()) uses.push_back(address);
    for (IStoreInstruction* store : variable->store_instructions()) uses.push_back(store->as_instruction());
    for (ILInstruction* use : uses) {
        auto* common_parent = as<Block>(loop->get_common_parent(use));
        if (common_parent != nullptr && common_parent->kind == BlockKind::ControlFlow && loop->is_before(use)) return true;
    }
    return false;
}

bool HighLevelLoopTransform::is_assignment(ILInstruction* inst) {
    if (is<StLoc>(inst)) return true;
    if (is<CompoundAssignmentInstruction>(inst)) return true;
    return false;
}

/// Returns true if the instruction is stloc v(add(ldloc v, arg))
/// or compound.assign(ldloca v, arg)
bool HighLevelLoopTransform::match_increment(ILInstruction* inst, ILVariable** variable) {
    ILInstruction* value = nullptr;
    if (inst->match_st_loc(variable, &value)) {
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        if (value->match_binary_numeric_instruction(BinaryNumericOperator::Add, &left, &right)) {
            return left->match_ld_loc(static_cast<const ILVariable*>(*variable));
        }
    } else if (auto* cai = as<CompoundAssignmentInstruction>(inst)) {
        return cai->target_kind == CompoundTargetKind::Address && cai->target()->match_ld_loca(variable);
    }
    return false;
}

/// Gets whether the statement is 'simple' (usable as for loop iterator):
/// Currently we only accept calls and assignments.
bool HighLevelLoopTransform::is_simple_statement(ILInstruction* inst) {
    switch (inst->op_code) {
    case OpCode::Call:
    case OpCode::CallVirt:
    case OpCode::NewObj:
    case OpCode::StLoc:
    case OpCode::StObj:
    case OpCode::NumericCompoundAssign:
    case OpCode::UserDefinedCompoundAssign: return true;
    default: return false;
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
