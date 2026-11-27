// dotnet/decompiler/flow_analysis/reaching_definitions_visitor.cpp -- the reaching definitions analysis
// (ICSharpCode.Decompiler.FlowAnalysis.ReachingDefinitionsVisitor).
#include "dotnet/decompiler/flow_analysis/reaching_definitions_visitor.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::flow_analysis {

using namespace il;

ReachingDefinitionsVisitor::ReachingDefinitionsVisitor(ILFunction* scope, const std::function<bool(ILVariable*)>& pred,
                                                       const util::CancellationToken& cancellation_token)
    : ReachingDefinitionsVisitor(scope, get_active_variable_bit_set(scope, pred), cancellation_token) {
    this->cancellation_token = cancellation_token;
}

util::BitSet ReachingDefinitionsVisitor::get_active_variable_bit_set(ILFunction* scope, const std::function<bool(ILVariable*)>& pred) {
    if (scope == nullptr) throw std::invalid_argument("scope");
    util::BitSet active_variables(scope->variables().count());
    for (int vi = 0; vi < scope->variables().count(); ++vi) active_variables.assign(vi, pred(scope->variables()[vi]));
    return active_variables;
}

ReachingDefinitionsVisitor::ReachingDefinitionsVisitor(ILFunction* scope_, util::BitSet analyzed_variables,
                                                       const util::CancellationToken& token)
    : scope(scope_), analyzed_variables_(std::move(analyzed_variables)) {
    if (scope_ == nullptr) throw std::invalid_argument("scope");
    flags_requiring_manual_impl |= InstructionFlags::MayWriteLocals;

    // Fill `all_stores_` and `store_index_map_` and `first_store_index_for_variable_`.
    auto stores_by_var = find_all_stores_by_variable(scope, analyzed_variables_, token);
    std::size_t total = State::first_store_index;
    for (const auto& l : stores_by_var)
        if (l) total += l->size();
    all_stores_.assign(total, nullptr);
    first_store_index_for_variable_.assign(static_cast<std::size_t>(scope->variables().count()) + 1, 0);
    int si = State::first_store_index;
    for (std::size_t vi = 0; vi < stores_by_var.size(); ++vi) {
        token.throw_if_cancellation_requested();
        first_store_index_for_variable_[vi] = si;
        auto& stores = stores_by_var[vi];
        if (stores) {
            [[maybe_unused]] int expected_store_count = static_cast<int>(scope->variables()[static_cast<int>(vi)]->store_instructions().size());
            // Extra store for the uninitialized state.
            expected_store_count += 1;
            assert(static_cast<int>(stores->size()) == expected_store_count);
            for (std::size_t i = 0; i < stores->size(); ++i) all_stores_[static_cast<std::size_t>(si) + i] = (*stores)[i];
            // Add all stores except for the first (representing the uninitialized state) to store_index_map_.
            for (std::size_t i = 1; i < stores->size(); ++i) store_index_map_.emplace((*stores)[i], si + static_cast<int>(i));
            si += static_cast<int>(stores->size());
        }
    }
    first_store_index_for_variable_[static_cast<std::size_t>(scope->variables().count())] = si;
    assert(static_cast<std::size_t>(si) == all_stores_.size());

    initialize(create_initial_state());
}

std::vector<std::optional<std::vector<ILInstruction*>>> ReachingDefinitionsVisitor::find_all_stores_by_variable(
    ILFunction* scope, const util::BitSet& active_variables, const util::CancellationToken& cancellation_token) {
    // For each variable, find the list of instructions storing to that variable
    std::vector<std::optional<std::vector<ILInstruction*>>> stores_by_var(static_cast<std::size_t>(scope->variables().count()));
    for (int vi = 0; vi < static_cast<int>(stores_by_var.size()); ++vi) {
        if (active_variables[vi]) stores_by_var[static_cast<std::size_t>(vi)] = std::vector<ILInstruction*>{nullptr};
    }
    for (ILInstruction* inst : scope->descendants()) {
        if (inst->has_direct_flag(InstructionFlags::MayWriteLocals)) {
            cancellation_token.throw_if_cancellation_requested();
            ILVariable* v = dynamic_cast<IInstructionWithVariableOperand*>(inst)->variable();
            if (v->function() == scope && active_variables[v->index_in_function()])
                stores_by_var[static_cast<std::size_t>(v->index_in_function())]->push_back(inst);
        }
    }
    return stores_by_var;
}

ReachingDefinitionsVisitor::State ReachingDefinitionsVisitor::create_initial_state() const {
    util::BitSet initial_state(static_cast<int>(all_stores_.size()));
    initial_state.set(State::reachable_bit);
    for (int vi = 0; vi < scope->variables().count(); ++vi) {
        if (analyzed_variables_[vi]) {
            assert(all_stores_[static_cast<std::size_t>(first_store_index_for_variable_[static_cast<std::size_t>(vi)])] == nullptr);
            initial_state.set(first_store_index_for_variable_[static_cast<std::size_t>(vi)]);
        }
    }
    return State(std::move(initial_state));
}

void ReachingDefinitionsVisitor::handle_store(ILInstruction* inst, ILVariable* v) {
    cancellation_token.throw_if_cancellation_requested();
    if (v->function() == scope && analyzed_variables_[v->index_in_function()] && state.is_reachable()) {
        const auto vi = static_cast<std::size_t>(v->index_in_function());
        // Clear the set of stores for this variable:
        state.kill_stores(first_store_index_for_variable_[vi], first_store_index_for_variable_[vi + 1]);
        // And replace it with this store:
        const int si = store_index_map_.at(inst);
        state.set_store(si);

        // propagate_state_on_exception() should be called here because the state changed. That is equal to
        // current_state_on_exception.join_with(state). Because state <= current_state_on_exception is guaranteed on entering,
        // adding the single additional store to the exceptional state does what propagate_state_on_exception() does.
        current_state_on_exception.set_store(si);
    }
}

void ReachingDefinitionsVisitor::visit_st_loc(StLoc& inst) {
    inst.value()->accept_visitor(*this);
    handle_store(&inst, inst.variable());
}

void ReachingDefinitionsVisitor::handle_match_store(MatchInstruction& inst) { handle_store(&inst, inst.variable()); }

void ReachingDefinitionsVisitor::begin_try_catch_handler(TryCatchHandler& inst) {
    DataFlowVisitor::begin_try_catch_handler(inst);
    handle_store(&inst, inst.variable());
}

void ReachingDefinitionsVisitor::visit_pinned_region(PinnedRegion& inst) {
    inst.init()->accept_visitor(*this);
    handle_store(&inst, inst.variable());
    inst.body()->accept_visitor(*this);
}

bool ReachingDefinitionsVisitor::is_analyzed_variable(ILVariable* v) const {
    return v->function() == scope && analyzed_variables_[v->index_in_function()];
}

std::vector<ILInstruction*> ReachingDefinitionsVisitor::get_stores(State at_state, ILVariable* v) const {
    assert(v->function() == scope && analyzed_variables_[v->index_in_function()]);
    const auto vi = static_cast<std::size_t>(v->index_in_function());
    int start_index = first_store_index_for_variable_[vi] + 1;
    const int end_index = first_store_index_for_variable_[vi + 1];
    std::vector<ILInstruction*> result;
    while (start_index < end_index) {
        const int next_reaching_store = at_state.next_reaching_store(start_index, end_index);
        if (next_reaching_store == -1) break;
        assert(dynamic_cast<IInstructionWithVariableOperand*>(all_stores_[static_cast<std::size_t>(next_reaching_store)])->variable() == v);
        result.push_back(all_stores_[static_cast<std::size_t>(next_reaching_store)]);
        start_index = next_reaching_store + 1;
    }
    return result;
}

bool ReachingDefinitionsVisitor::is_potentially_uninitialized(State at_state, ILVariable* v) const {
    assert(v->function() == scope && analyzed_variables_[v->index_in_function()]);
    return at_state.is_reaching_store(first_store_index_for_variable_[static_cast<std::size_t>(v->index_in_function())]);
}

}  // namespace cb::dotnet::decompiler::flow_analysis
