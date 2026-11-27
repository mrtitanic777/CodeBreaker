// dotnet/decompiler/flow_analysis/data_flow_visitor.hpp -- the base class of forward data flow analyses over the ILAst: it walks the
// instructions in evaluation order, keeps the current state, merges states where control flow joins (branches, leaves, try/catch/
// finally, conditions, nullable unwrap/rewrap) and repeats block containers until the states stop changing. A concrete analysis gives
// the State type and overrides the visit methods where it reads or writes variables.
//
//     struct MyState {                       // a handle to mutable state: copies share it, as upstream's struct of one BitSet does
//         bool less_than_or_equal(const MyState&) const;  MyState clone() const;  void replace_with(const MyState&);
//         void join_with(const MyState&);  void trigger_finally(const MyState&);
//         bool is_bottom() const;          void replace_with_bottom();
//     };
//     class MyAnalysis : public DataFlowVisitor<MyState> {
//         void handle_match_store(il::MatchInstruction& inst) override { ... }
//         void visit_ld_loc(il::LdLoc& inst) override { ... }
//     };
//
// Ported from ILSpy (ICSharpCode.Decompiler.FlowAnalysis: IDataFlowState<Self>, DataFlowVisitor<State>), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). IDataFlowState is the concept DataFlowState. A state must be copyable
// and default-constructible and behave like a reference to mutable data: the mutating members change what every copy sees. Upstream's
// DEBUG-only record of the input and output state at every instruction is kept, active when NDEBUG is not defined.
#pragma once

#include <cassert>
#include <concepts>
#include <map>
#include <set>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core/error.hpp"
#include "dotnet/decompiler/il/il_visitor.hpp"

namespace cb::dotnet::decompiler::flow_analysis {

// What a state of a data flow analysis offers (upstream's IDataFlowState<Self>). States form a join-semilattice; to handle
// try-finally properly they also implement trigger_finally, so they form a lattice.
template <class S>
concept DataFlowState = std::copyable<S> && std::default_initializable<S> && requires(S s, const S c) {
    // Whether this state is "less than" (or equal to) another one: the partial order of the semilattice, usually "has less
    // information than". After each state change the old state must be less than the new one, so that the analysis terminates.
    { c.less_than_or_equal(c) } -> std::convertible_to<bool>;
    // A new object with a copy of the state.
    { c.clone() } -> std::same_as<S>;
    // Replaces the contents of this state with a copy of those in the argument.
    s.replace_with(c);
    // Joins the incoming state into this one: this becomes the smallest state greater than or equal to both. Used where control
    // flow paths meet.
    s.join_with(c);
    // Merges the end state of a finally block with the end state of a branch leaving the try block. If either input is unreachable
    // the result must be unreachable.
    s.trigger_finally(c);
    // Whether this is the bottom state: no code path from the entry point to this position has been found yet; it is "less than"
    // all other states.
    { c.is_bottom() } -> std::convertible_to<bool>;
    // The same as replace_with(bottom state), possibly done more efficiently.
    s.replace_with_bottom();
};

template <DataFlowState State>
class DataFlowVisitor : public il::ILVisitor {
public:
    // The visitor holds states that point into each other; it is not copied.
    DataFlowVisitor() = default;
    DataFlowVisitor(const DataFlowVisitor&) = delete;
    DataFlowVisitor& operator=(const DataFlowVisitor&) = delete;

protected:
    // The data flow analysis tracks a 'state'. There are many states (one per source position, i.e. instruction), but only these
    // are kept: the current state in the visitor (it corresponds to the instruction being visited and is mutated as the ILAst is
    // traversed), and the input state for each control flow node (mutated as the analysis learns about new control flow edges).

    // The current state. Caution: any state object assigned to it is mutated as the visitor traverses the ILAst.
    State state;

    // The combined state of all possible exceptional control flow paths in the current try block; the input state for catch blocks.
    // Caution: any state object assigned to it is mutated as the visitor meets instructions that may throw. Within a try block,
    // current_state_on_exception == state_on_exception[try block's parent].
    State current_state_on_exception;

    // Derived classes may add to these flags to be sure they do not forget to override an interesting method.
    il::InstructionFlags flags_requiring_manual_impl = il::InstructionFlags::ControlFlow | il::InstructionFlags::MayBranch |
                                                       il::InstructionFlags::MayUnwrapNull | il::InstructionFlags::EndPointUnreachable;

    // Initializes the visitor; must be called once before any visit method. A method and not a constructor, because derived classes
    // may need complex initialization before they can make the initial state.
    void initialize(const State& initial_state) {
        assert(!initialized_);
        initialized_ = true;
        state = initial_state.clone();
        bottom_state_ = initial_state.clone();
        bottom_state_.replace_with_bottom();
        assert(bottom_state_.is_bottom());
        state_on_nullable_rewrap_ = bottom_state_.clone();
        current_state_on_exception = state.clone();
    }

    // Records the input state (start) or output state (end) of an instruction for debugging, in builds with assertions.
    void debug_start_point([[maybe_unused]] il::ILInstruction* inst) {
#ifndef NDEBUG
        debug_point(debug_input_state_, inst);
#endif
    }
    void debug_end_point([[maybe_unused]] il::ILInstruction* inst) {
#ifndef NDEBUG
        debug_point(debug_output_state_, inst);
#endif
    }

    // Handles control flow when the current instruction throws an exception: joins the current state into the "exception state" of
    // the current try block. This should be called for all instructions, not only those that may throw (async exceptions). To avoid
    // redundant calls, every visit may assume the current state is already propagated, and must guarantee the same at its end.
    void propagate_state_on_exception() { current_state_on_exception.join_with(state); }

    // Replaces the current state with the bottom state.
    void mark_unreachable() { state.replace_with_bottom(); }

    // Visits the try block. Returns a new state object for the exceptional control flow transfer out of the try block.
    State handle_try_block(il::TryInstruction* inst) {
        State old_state_on_exception = current_state_on_exception;
        State new_state_on_exception;
        if (auto it = state_on_exception_.find(inst); it != state_on_exception_.end()) {
            new_state_on_exception = it->second;
            new_state_on_exception.join_with(state);
        } else {
            new_state_on_exception = state.clone();
            state_on_exception_.emplace(inst, new_state_on_exception);
        }

        current_state_on_exception = new_state_on_exception;
        inst->try_block()->accept_visitor(*this);
        // swap back to the old object instance
        current_state_on_exception = old_state_on_exception;

        // No matter what kind of try-instruction this is, an async exception may be thrown immediately in the handler block, so
        // propagate the state:
        old_state_on_exception.join_with(new_state_on_exception);

        // Return a copy, so that the caller mutating the returned state does not influence the 'state_on_exception' map
        return new_state_on_exception.clone();
    }

    virtual void begin_try_catch_handler(il::TryCatchHandler&) {}

    // Stores the pattern variable of a match instruction (called while evaluating the match).
    virtual void handle_match_store(il::MatchInstruction& inst) = 0;

    // ---- the visitor

    // Assumes normal control flow and no branches.
    void default_visit(il::ILInstruction& inst) final {
        debug_start_point(&inst);
        if (il::any(inst.direct_flags() & flags_requiring_manual_impl))
            throw InternalError(std::string(typeid(*this).name()) + " is missing implementation for " + typeid(inst).name());

        // Since this instruction has normal control flow, the children are evaluated left-to-right.
        for (il::ILInstruction* child : inst.children()) {
            child->accept_visitor(*this);
            assert(state.is_bottom() || !child->has_flag(il::InstructionFlags::EndPointUnreachable));  // unreachable code is bottom
        }

        debug_end_point(&inst);
    }

    void visit_block_container(il::BlockContainer& container) override {
        debug_start_point(&container);
        std::set<int> worklist;
        // register the work list so that branches within this container can add to it
        work_lists_.emplace(&container, &worklist);
        State state_on_entry = get_block_input_state(container.entry_point());
        if (!state.less_than_or_equal(state_on_entry)) {
            // If there is new information for the container's entry point, add the entry point to the work list.
            state_on_entry.join_with(state);
            worklist.insert(0);
        }

        // To handle loops, the loop body is analyzed before the state for the loop back edge is known, but the input state of the
        // loop body (to which the back edge contributes) is needed before the body can be analyzed. So the analysis of the body is
        // repeated until the state no longer changes. To make that terminate quickly, the control flow nodes are processed in
        // reverse post-order: the work list is a sorted set, assuming the block indices are ordered appropriately (the caller can
        // use BlockContainer::sort_blocks for this).
        while (!worklist.empty()) {
            const int block_index = *worklist.begin();
            worklist.erase(worklist.begin());
            il::Block* block = container.blocks[block_index];
            state.replace_with(state_on_branch_.at(block));
            block->accept_visitor(*this);
        }
        if (auto it = state_on_leave_.find(&container); it != state_on_leave_.end()) {
            state.replace_with(it->second);
        } else {
            mark_unreachable();
        }
        debug_end_point(&container);
        work_lists_.erase(&container);
    }

    void visit_branch(il::Branch& inst) override {
        if (inst.triggers_finally_block()) {
            assert(state.less_than_or_equal(current_state_on_exception));
            branches_triggering_finally_.push_back({&inst, state.clone()});
        } else {
            merge_branch_state_into_target_block(inst, state);
        }
        mark_unreachable();
    }

    void visit_leave(il::Leave& inst) override {
        inst.value()->accept_visitor(*this);
        if (inst.triggers_finally_block()) {
            assert(state.less_than_or_equal(current_state_on_exception));
            branches_triggering_finally_.push_back({&inst, state.clone()});
        } else {
            merge_branch_state_into_state_on_leave(inst, state);
        }
        mark_unreachable();
    }

    void visit_throw(il::Throw& inst) override {
        inst.argument()->accept_visitor(*this);
        mark_unreachable();
    }

    void visit_rethrow(il::Rethrow&) override { mark_unreachable(); }

    void visit_invalid_branch(il::InvalidBranch&) override { mark_unreachable(); }

    void visit_try_catch(il::TryCatch& inst) override {
        debug_start_point(&inst);
        State on_exception = handle_try_block(&inst);
        State endpoint = state.clone();
        for (il::TryCatchHandler* handler : inst.handlers) {
            state.replace_with(on_exception);
            begin_try_catch_handler(*handler);
            handler->filter()->accept_visitor(*this);
            // If the filter returns false, any mutations done by the filter will be visible by the remaining handlers (but it is
            // also possible that the filter did not get executed at all because the exception type does not match).
            on_exception.join_with(state);

            handler->body()->accept_visitor(*this);
            endpoint.join_with(state);
        }
        state = endpoint;
        debug_end_point(&inst);
    }

    // TryCatchHandler is handled directly in visit_try_catch.
    void visit_try_catch_handler(il::TryCatchHandler&) final { throw InternalError("TryCatchHandler is handled by its TryCatch"); }

    void visit_try_finally(il::TryFinally& inst) override {
        debug_start_point(&inst);
        const std::size_t branches_triggering_finally_old_count = branches_triggering_finally_.size();
        // At first, handle 'try { .. } finally { .. }' like 'try { .. } catch {} .. if (?) rethrow; }'
        State on_exception = handle_try_block(&inst);
        State on_success = state.clone();
        state.join_with(on_exception);
        inst.finally_block()->accept_visitor(*this);
        // (a propagate_state_on_exception() to rethrow after the finally block would be redundant)
        assert(state.less_than_or_equal(current_state_on_exception));

        process_branches_leaving_try_finally(inst, branches_triggering_finally_old_count);

        // Use trigger_finally() to ensure points after the try-finally are reachable only if both the try and the finally end points
        // are reachable.
        on_success.trigger_finally(state);
        state = on_success;
        debug_end_point(&inst);
    }

    void visit_try_fault(il::TryFault& inst) override {
        debug_start_point(&inst);
        // try-fault executes the fault block if an exception occurs in try, and always rethrows the exception at the end.
        State on_exception = handle_try_block(&inst);
        State on_success = state;
        state = on_exception;
        inst.fault_block()->accept_visitor(*this);
        assert(state.less_than_or_equal(current_state_on_exception));

        // try-fault exits normally only if no exception occurred
        state = on_success;
        debug_end_point(&inst);
    }

    void visit_if_instruction(il::IfInstruction& inst) override {
        debug_start_point(&inst);
        auto [before_then, before_else] = evaluate_condition(inst.condition());
        state = before_then;
        inst.true_inst()->accept_visitor(*this);
        State after_true_state = state;
        state = before_else;
        inst.false_inst()->accept_visitor(*this);
        state.join_with(after_true_state);
        debug_end_point(&inst);
    }

    void visit_match_instruction(il::MatchInstruction& inst) override {
        auto [on_true, on_false] = evaluate_match(inst);
        state = on_true;
        state.join_with(on_false);
    }

    void visit_null_coalescing_instruction(il::NullCoalescingInstruction& inst) override {
        handle_binary_with_optional_evaluation(&inst, inst.value_inst(), inst.fallback_inst());
    }

    void visit_dynamic_logic_operator_instruction(il::DynamicLogicOperatorInstruction& inst) override {
        handle_binary_with_optional_evaluation(&inst, inst.left(), inst.right());
    }

    void visit_user_defined_logic_operator(il::UserDefinedLogicOperator& inst) override {
        handle_binary_with_optional_evaluation(&inst, inst.left(), inst.right());
    }

    void visit_nullable_rewrap(il::NullableRewrap& inst) override {
        debug_start_point(&inst);
        State old_state = state_on_nullable_rewrap_.clone();
        state_on_nullable_rewrap_.replace_with_bottom();

        inst.argument()->accept_visitor(*this);
        // Join incoming control flow from the NullableUnwraps.
        state.join_with(state_on_nullable_rewrap_);

        state_on_nullable_rewrap_ = old_state;
        debug_end_point(&inst);
    }

    void visit_nullable_unwrap(il::NullableUnwrap& inst) override {
        debug_start_point(&inst);
        inst.argument()->accept_visitor(*this);
        // Conditional control flow edge to the surrounding NullableRewrap.
        state_on_nullable_rewrap_.join_with(state);
        debug_end_point(&inst);
    }

    void visit_switch_instruction(il::SwitchInstruction& inst) override {
        debug_start_point(&inst);
        inst.value()->accept_visitor(*this);
        State before_sections = state.clone();
        inst.sections[0]->accept_visitor(*this);
        State after_sections = state.clone();
        for (int i = 1; i < inst.sections.count(); ++i) {
            state.replace_with(before_sections);
            inst.sections[i]->accept_visitor(*this);
            after_sections.join_with(state);
        }
        state = after_sections;
        debug_end_point(&inst);
    }

    void visit_yield_return(il::YieldReturn& inst) override {
        debug_start_point(&inst);
        inst.value()->accept_visitor(*this);
        debug_end_point(&inst);
    }

    void visit_using_instruction(il::UsingInstruction& inst) override {
        debug_start_point(&inst);
        inst.resource_expression()->accept_visitor(*this);
        inst.body()->accept_visitor(*this);
        debug_end_point(&inst);
    }

    void visit_lock_instruction(il::LockInstruction& inst) override {
        debug_start_point(&inst);
        inst.on_expression()->accept_visitor(*this);
        inst.body()->accept_visitor(*this);
        debug_end_point(&inst);
    }

    void visit_il_function(il::ILFunction&) override { throw InternalError("the data flow visitor does not visit functions"); }

private:
    // A branch or leave that runs a finally block before it reaches its target, with the state it carries.
    struct PendingBranch {
        il::ILInstruction* inst;  // a Branch or a Leave
        State state;
    };

    // The state object that holds the state for incoming branches to the block: a clone of the bottom state on the first call for a
    // block, then the same object. The caller mutates it by calling join_with().
    State get_block_input_state(il::Block* block) {
        if (auto it = state_on_branch_.find(block); it != state_on_branch_.end()) return it->second;
        State s = bottom_state_.clone();
        state_on_branch_.emplace(block, s);
        return s;
    }

    void merge_branch_state_into_target_block(il::Branch& inst, State& branch_state) {
        il::Block* target_block = inst.target_block();
        State target_state = get_block_input_state(target_block);
        if (!branch_state.less_than_or_equal(target_state)) {
            target_state.join_with(branch_state);

            auto* container = static_cast<il::BlockContainer*>(target_block->parent());
            if (auto it = work_lists_.find(container); it != work_lists_.end()) {
                it->second->insert(target_block->child_index());
            } else {
                assert(false && "Failed to find target BlockContainer");
            }
        }
    }

    void merge_branch_state_into_state_on_leave(il::Leave& inst, State& branch_state) {
        if (auto it = state_on_leave_.find(inst.target_container()); it != state_on_leave_.end()) {
            it->second.join_with(branch_state);
        } else {
            state_on_leave_.emplace(inst.target_container(), branch_state.clone());
        }
        // Note: the block container needs no place on the work queue, because it is an ancestor of the leave instruction, and so
        // the visit of the container is running.
    }

    // Processes branches leaving the try-finally: applies trigger_finally to each pending branch, keeps those that will trigger
    // additional finally blocks, and after all finallies are applied merges the branch state into the target block.
    void process_branches_leaving_try_finally(il::TryFinally& try_finally, std::size_t old_count) {
        std::size_t out_pos = old_count;
        for (std::size_t i = old_count; i < branches_triggering_finally_.size(); ++i) {
            PendingBranch pending = branches_triggering_finally_[i];
            il::BlockContainer* target = nullptr;
            if (auto* leave = il::as<il::Leave>(pending.inst))
                target = leave->target_container();
            else
                target = il::as<il::Branch>(pending.inst)->target_container();
            assert(pending.inst->is_descendant_of(&try_finally));
            assert(try_finally.is_descendant_of(target));
            pending.state.trigger_finally(state);
            const bool triggers_another_finally = il::Branch::get_executes_finally_block(&try_finally, target);
            if (triggers_another_finally) {
                branches_triggering_finally_[out_pos++] = pending;
            } else {
                // Merge the state into the target block.
                if (auto* leave = il::as<il::Leave>(pending.inst))
                    merge_branch_state_into_state_on_leave(*leave, pending.state);
                else
                    merge_branch_state_into_target_block(*il::as<il::Branch>(pending.inst), pending.state);
            }
        }
        branches_triggering_finally_.erase(branches_triggering_finally_.begin() + static_cast<std::ptrdiff_t>(out_pos),
                                           branches_triggering_finally_.end());
    }

    // Evaluates the condition of an if. Returns the state after the condition evaluates to true and the state after it evaluates to
    // false. `state` is invalid afterwards and must be overwritten with one of the results.
    std::pair<State, State> evaluate_condition(il::ILInstruction* inst) {
        if (auto* if_inst = il::as<il::IfInstruction>(inst)) {
            // 'if (a?b:c)' or similar. This also includes conditions that are logic.not, logic.and, logic.or.
            debug_start_point(if_inst);
            auto [before_then, before_else] = evaluate_condition(if_inst->condition());
            state = before_then;
            auto [after_then_true, after_then_false] = evaluate_condition(if_inst->true_inst());
            state = before_else;
            auto [after_else_true, after_else_false] = evaluate_condition(if_inst->false_inst());

            State on_true = after_then_true;
            on_true.join_with(after_else_true);
            State on_false = after_then_false;
            on_false.join_with(after_else_false);

            debug_end_point(if_inst);
            return {on_true, on_false};
        } else if (auto* constant = il::as<il::LdcI4>(inst)) {
            if (constant->value == 0) return {bottom_state_.clone(), state};
            return {state, bottom_state_.clone()};
        } else if (auto* match = il::as<il::MatchInstruction>(inst)) {
            return evaluate_match(*match);
        } else {
            // other kind of condition
            inst->accept_visitor(*this);
            return {state, state.clone()};
        }
    }

    // Evaluates a match instruction. Returns the state after the pattern matches and the state after it fails to match. `state` is
    // invalid afterwards and must be overwritten with one of the results.
    std::pair<State, State> evaluate_match(il::MatchInstruction& inst) {
        debug_start_point(&inst);
        inst.tested_operand()->accept_visitor(*this);
        State on_false = state.clone();
        if (!inst.check_not_null && !inst.check_type) on_false.replace_with_bottom();
        handle_match_store(inst);
        for (il::ILInstruction* sub_pattern : inst.sub_patterns()) {
            auto [sub_true, sub_false] = evaluate_condition(sub_pattern);
            on_false.join_with(sub_false);
            state = sub_true;
        }
        debug_end_point(&inst);
        return {state, on_false};
    }

    void handle_binary_with_optional_evaluation(il::ILInstruction* parent, il::ILInstruction* left, il::ILInstruction* right) {
        debug_start_point(parent);
        left->accept_visitor(*this);
        State branch_state = state.clone();
        right->accept_visitor(*this);
        state.join_with(branch_state);
        debug_end_point(parent);
    }

#ifndef NDEBUG
    void debug_point(std::unordered_map<il::ILInstruction*, State>& debug_dict, il::ILInstruction* inst) {
        assert(initialized_ && "initialize() was not called");

        if (auto it = debug_dict.find(inst); it != debug_dict.end()) {
            assert(it->second.less_than_or_equal(state));
            it->second.join_with(state);
        } else if (debug_dict.size() < 1000) {
            // limit the number of tracked instructions to make memory usage in debug builds less horrible
            debug_dict.emplace(inst, state.clone());
        }

        // current_state_on_exception should be all states within the try block joined together: state should already have been
        // joined into it.
        assert(state.less_than_or_equal(current_state_on_exception));
    }
#endif

    // The bottom state. Must not be mutated.
    State bottom_state_;
    bool initialized_ = false;

    // For debugging, the input and output state at every instruction (kept in builds with assertions).
    std::unordered_map<il::ILInstruction*, State> debug_input_state_;
    std::unordered_map<il::ILInstruction*, State> debug_output_state_;

    // The state for incoming branches. Only used for blocks in block containers, not for inline blocks.
    std::unordered_map<il::Block*, State> state_on_branch_;
    // The state at the block container end point (the state for incoming 'leave' instructions).
    std::unordered_map<il::BlockContainer*, State> state_on_leave_;
    // For each block container being visited, the set of blocks (by child index) that had their incoming state changed and were not
    // processed yet.
    std::unordered_map<il::BlockContainer*, std::set<int>*> work_lists_;
    std::vector<PendingBranch> branches_triggering_finally_;
    // The state_on_exception per try instruction.
    std::unordered_map<il::TryInstruction*, State> state_on_exception_;
    State state_on_nullable_rewrap_;
};

}  // namespace cb::dotnet::decompiler::flow_analysis
