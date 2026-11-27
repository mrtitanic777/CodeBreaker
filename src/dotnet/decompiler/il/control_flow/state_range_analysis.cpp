// dotnet/decompiler/il/control_flow/state_range_analysis.cpp -- see state_range_analysis.hpp.
#include "dotnet/decompiler/il/control_flow/state_range_analysis.hpp"

#include <cassert>
#include <stdexcept>

namespace cb::dotnet::decompiler::il::control_flow {

namespace {

// C#'s unchecked(-x) on int.
int unchecked_neg(int x) { return static_cast<int>(0u - static_cast<std::uint32_t>(x)); }

}  // namespace

StateRangeAnalysis::StateRangeAnalysis(StateRangeAnalysisMode mode, const IField* state_field, ILVariable* cached_state_var, bool legacy_visual_basic)
    : mode_(mode), state_field_(state_field), legacy_visual_basic_(legacy_visual_basic), eval_context_(state_field, legacy_visual_basic) {
    if (mode == StateRangeAnalysisMode::IteratorDispose) finally_method_to_state_range.emplace();
    if (mode == StateRangeAnalysisMode::AwaitInFinally) ranges_for_leave_.emplace();
    if (cached_state_var != nullptr) eval_context_.add_state_variable(cached_state_var);
}

StateRangeAnalysis StateRangeAnalysis::create_nested_analysis() const {
    StateRangeAnalysis sra(mode_, state_field_, nullptr, legacy_visual_basic_);
    sra.do_finally_bodies = do_finally_bodies;
    sra.skip_finally_bodies = skip_finally_bodies;
    for (ILVariable* v : eval_context_.state_variables()) sra.eval_context_.add_state_variable(v);
    return sra;
}

util::LongSet StateRangeAnalysis::assign_state_ranges(ILInstruction* inst, util::LongSet state_range) {
    cancellation_token.throw_if_cancellation_requested();

    // User code - abort analysis
    auto user_code = [&]() -> util::LongSet {
        auto* leave = as<Leave>(inst);
        if (mode_ == StateRangeAnalysisMode::IteratorDispose && !(leave != nullptr && leave->is_leaving_function()))
            throw SymbolicAnalysisFailedException("Unexpected instruction in Iterator.Dispose()");
        return util::LongSet::empty_set();
    };

    SymbolicValue val;
    if (auto* block_container = as<BlockContainer>(inst)) {
        add_state_range(block_container->entry_point(), state_range);
        for (Block* block : block_container->blocks) {
            // We assume that there are no jumps to blocks already processed.
            // TODO: is SortBlocks() guaranteeing this, even if the user code has loops?
            if (const util::LongSet* range = ranges_.find(block)) {
                util::LongSet block_range = *range;
                assign_state_ranges(block, std::move(block_range));
            }
        }
        // Since we don't track 'leave' edges, we can only conservatively return LongSet.Empty.
        return util::LongSet::empty_set();
    }
    if (auto* block = as<Block>(inst)) {
        for (ILInstruction* inst_in_block : block->instructions) {
            if (state_range.is_empty()) break;
            util::LongSet old_state_range = state_range;
            state_range = assign_state_ranges(inst_in_block, state_range);
            // End-point can only be reachable in a subset of the states where the start-point is reachable.
            assert(state_range.is_subset_of(old_state_range));
            // If the end-point is unreachable, it must be reachable in no states.
            assert(state_range.is_empty() || !inst_in_block->has_flag(InstructionFlags::EndPointUnreachable));
        }
        return state_range;
    }
    if (auto* try_finally = as<TryFinally>(inst); try_finally != nullptr && mode_ == StateRangeAnalysisMode::IteratorDispose) {
        util::LongSet after_try = assign_state_ranges(try_finally->try_block(), state_range);
        // really finally should start with 'stateRange.UnionWith(afterTry)', but that's equal to 'stateRange'.
        assert(after_try.is_subset_of(state_range));
        util::LongSet after_finally = assign_state_ranges(try_finally->finally_block(), state_range);
        return after_try.intersect_with(after_finally);
    }
    if (auto* switch_inst = as<SwitchInstruction>(inst)) {
        val = eval_context_.eval(switch_inst->value());
        if (val.type != SymbolicValueType::State) return user_code();
        std::vector<util::LongInterval> exit_intervals;
        for (SwitchSection* section : switch_inst->sections) {
            // switch (state + Constant)
            // matches 'case VALUE:'
            // iff (state + Constant == value)
            // iff (state == value - Constant)
            util::LongSet effective_labels = section->labels().add_offset(static_cast<std::int64_t>(unchecked_neg(val.constant)));
            util::LongSet result = assign_state_ranges(section->body(), state_range.intersect_with(effective_labels));
            exit_intervals.insert(exit_intervals.end(), result.intervals().begin(), result.intervals().end());
        }
        // exitIntervals = union of exits of all sections
        return util::LongSet(exit_intervals);
    }
    if (auto* if_inst = as<IfInstruction>(inst)) {
        val = eval_context_.eval(if_inst->condition()).as_bool();
        if (val.type != SymbolicValueType::StateInSet) return user_code();
        util::LongSet true_ranges = val.value_set;
        util::LongSet after_true = assign_state_ranges(if_inst->true_inst(), state_range.intersect_with(true_ranges));
        util::LongSet after_false = assign_state_ranges(if_inst->false_inst(), state_range.except_with(true_ranges));
        return after_true.union_with(after_false);
    }
    if (auto* br = as<Branch>(inst)) {
        add_state_range(br->target_block(), state_range);
        return util::LongSet::empty_set();
    }
    if (auto* leave = as<Leave>(inst); leave != nullptr && mode_ == StateRangeAnalysisMode::AwaitInFinally) {
        add_state_range_for_leave(leave->target_container(), state_range);
        return util::LongSet::empty_set();
    }
    if (is<Nop>(inst)) return state_range;
    if (auto* stloc = as<StLoc>(inst)) {
        if (stloc->variable() == do_finally_bodies || stloc->variable() == skip_finally_bodies) {
            // pre-roslyn async/await uses a generated 'bool doFinallyBodies';
            // do not treat this as user code.
            // Mono also does this for yield-return.
            return state_range;
        }
        val = eval_context_.eval(stloc->value());
        if (val.type == SymbolicValueType::State && val.constant == 0) {
            eval_context_.add_state_variable(stloc->variable());
            return state_range;
        } else {
            return user_code();  // user code
        }
    }
    if (auto* call = as<Call>(inst); call != nullptr && mode_ == StateRangeAnalysisMode::IteratorDispose) {
        // Call to finally method.
        // Usually these are in finally blocks, but sometimes (e.g. foreach over array),
        // the C# compiler puts the call to a finally method outside the try-finally block.
        const IMethod* finally_method = dynamic_cast<const IMethod*>(call->method->member_definition());
        if (!finally_method_to_state_range->try_add(finally_method, state_range))
            throw std::invalid_argument("An item with the same key has already been added.");
        return util::LongSet::empty_set();  // return Empty since we executed user code (the finally method)
    }
    if (auto* stobj = as<StObj>(inst); stobj != nullptr && mode_ == StateRangeAnalysisMode::IteratorMoveNext) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        if (stobj->match_st_fld(&target, &field, &value) && target->match_ld_this() && field->member_definition() == state_field_ &&
            value->match_ldc_i4(-1)) {
            // Mono resets the state field during MoveNext();
            // don't consider this user code.
            return state_range;
        }
        return user_code();
    }
    if (auto* stobj = as<StObj>(inst); stobj != nullptr && mode_ == StateRangeAnalysisMode::IteratorDispose) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        if (stobj->match_st_fld(&target, &field, &value) && target->match_ld_this()) {
            if (field->member_definition() == state_field_ && value->match_ldc_i4(-2)) {
                // Roslyn 4.13 sets the state field in Dispose() to mark the iterator as disposed,
                // don't consider this user code.
                return state_range;
            } else if (value->match_default_or_null_or_zero()) {
                // Roslyn 4.13 clears any local hoisted local variables in Dispose(),
                // don't consider this user code.
                return state_range;
            } else {
                return user_code();
            }
        }
        return user_code();
    }
    return user_code();
}

void StateRangeAnalysis::add_state_range(Block* block, util::LongSet state_range) {
    if (util::LongSet* existing_range = ranges_.find(block))
        ranges_.set(block, state_range.union_with(*existing_range));
    else
        ranges_.try_add(block, std::move(state_range));
}

void StateRangeAnalysis::add_state_range_for_leave(BlockContainer* target, util::LongSet state_range) {
    if (util::LongSet* existing_range = ranges_for_leave_->find(target))
        ranges_for_leave_->set(target, state_range.union_with(*existing_range));
    else
        ranges_for_leave_->try_add(target, std::move(state_range));
}

util::LongDict<Block*> StateRangeAnalysis::get_block_state_set_mapping(BlockContainer* container) const {
    std::vector<std::pair<util::LongSet, Block*>> mapping;
    // First, consider container exits:
    for (const auto& [block, states] : ranges_) {
        if (block->parent() != container) mapping.emplace_back(states, block);
    }
    // Then blocks within the container:
    for (int i = container->blocks.count() - 1; i >= 0; --i) {
        Block* block = container->blocks[i];
        if (const util::LongSet* states = ranges_.find(block)) mapping.emplace_back(*states, block);
    }
    return util::LongDict<Block*>::create(mapping);
}

util::LongDict<BlockContainer*> StateRangeAnalysis::get_block_state_set_mapping_for_leave() const {
    assert(mode_ == StateRangeAnalysisMode::AwaitInFinally);
    std::vector<std::pair<util::LongSet, BlockContainer*>> mapping;
    for (const auto& [container, states] : *ranges_for_leave_) mapping.emplace_back(states, container);
    return util::LongDict<BlockContainer*>::create(mapping);
}

}  // namespace cb::dotnet::decompiler::il::control_flow
