// dotnet/decompiler/flow_analysis/reaching_definitions_visitor.hpp -- the "reaching definitions" analysis: for each point of a function,
// which stores to its variables may have written the value the variable holds there (https://en.wikipedia.org/wiki/Reaching_definition).
//
//     class FindStores : public ReachingDefinitionsVisitor {
//         using ReachingDefinitionsVisitor::ReachingDefinitionsVisitor;
//         void visit_ld_loc(il::LdLoc& inst) override { for (auto* store : get_stores(state, inst.variable())) ... }
//     };
//     FindStores v(function, [](il::ILVariable* v) { return v->is_single_definition(); }, token);
//     function->body()->accept_visitor(v);
//
// Ported from ILSpy (ICSharpCode.Decompiler.FlowAnalysis: ReachingDefinitionsVisitor), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
//
// The "definitions" are stores to local variables: StLoc, TryCatchHandler (the exception variable), and a special entry for each
// variable that is uninitialized. LdLoca, references and pointers are not tracked, so the analysis is likely wrong or incomplete for
// variables with address_count() != 0. The class does not keep the computed information (that would multiply the states to keep):
// derive from it and override the visit methods at the points of interest (usually the loads).
//
// The state is a class beside the visitor (ReachingDefinitionsState, aliased as ReachingDefinitionsVisitor::State), because C++ needs
// the state type complete where the visitor derives from DataFlowVisitor<State>.
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/flow_analysis/data_flow_visitor.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler::flow_analysis {

// The state during the reaching definitions analysis: either unreachable or reachable.
//   1) unreachable: during the analysis this only means no path from the entry point was found yet. States become reachable as the
//      analysis processes more control flow paths.
//   2) reachable: for each variable, the set of stores that might have written to it before control flow reached the state's source
//      position (not the stores that were definitely overwritten by other stores to the same variable). The sets grow as more code
//      paths are processed.
// To use little memory, every store in the analyzed function gets an integer "store index", and the state is a BitSet with a bit per
// store index. All stores of one variable occupy a contiguous segment of the indices, so the stores a new store overwrites are cleared
// efficiently.
//
// The bit set holds three kinds of bits:
//   Reachable bit (bit 0): this state's position is reachable from the entry point.
//   Reaching uninitialized variable bit (bit si > 0 where all_stores[si] is null): a code path from the scope's entry point to this
//     position does not pass through any store to the variable. first_store_index_for_variable[v.index_in_function()] is the index
//     of that variable's uninitialized bit.
//   Reaching store bit (bit si where all_stores[si] is not null): a code path from the entry point to this position passes through
//     all_stores[si] and through no other store to its variable. A variable's reaching store bits lie between its first store index
//     and the next variable's (both ends exclusive).
// The initial state has the reachable bit and the uninitialized variable bits set and the reaching store bits clear. The bottom
// state has all bits clear. A copy of a state shares the bit set with the original, as a copy of upstream's struct does.
class ReachingDefinitionsState {
public:
    // To tell unreachable from reachable states the first bit is the 'reachable bit'.
    static constexpr int reachable_bit = 0;
    // Because bit 0 is the reachable bit, store indices start at 1.
    static constexpr int first_store_index = 1;

    ReachingDefinitionsState() = default;
    explicit ReachingDefinitionsState(util::BitSet bits) : bits_(std::make_shared<util::BitSet>(std::move(bits))) {}

    bool less_than_or_equal(const ReachingDefinitionsState& other_state) const { return bits_->is_subset_of(*other_state.bits_); }
    ReachingDefinitionsState clone() const { return ReachingDefinitionsState(bits_->clone()); }
    void replace_with(const ReachingDefinitionsState& new_content) { bits_->replace_with(*new_content.bits_); }
    // When control flow is joined together, the bit sets are united (a store reaches iff it reaches through either incoming path).
    void join_with(const ReachingDefinitionsState& incoming_state) { bits_->union_with(*incoming_state.bits_); }
    // Cases: `try { v = 1; } finally { v = 2; }` leaves only store 2 visible after the try-finally; `v = 1; try { v = 2; } finally { }`
    // leaves both. What is wanted is the state after the finally block assuming it was entered without an exception. That is not
    // known (it would take analyzing the finally block twice), so the state after the finally block stands for it. The visitor needs
    // the result to be bottom if the end state of the try block was unreachable, so that is done.
    void trigger_finally(const ReachingDefinitionsState& finally_state) {
        if (is_reachable()) replace_with(finally_state);
    }
    bool is_bottom() const { return !(*bits_)[reachable_bit]; }
    // All bits are cleared, not only the reachable bit, so that the bottom state behaves as expected in joins.
    void replace_with_bottom() { bits_->clear_all(); }
    bool is_reachable() const { return (*bits_)[reachable_bit]; }
    // Clears all store bits from start_store_index (inclusive) to end_store_index (exclusive).
    void kill_stores(int start_store_index, int end_store_index) {
        assert(start_store_index >= first_store_index);
        assert(end_store_index >= start_store_index);
        bits_->clear(start_store_index, end_store_index);
    }
    bool is_reaching_store(int store_index) const { return (*bits_)[store_index]; }
    int next_reaching_store(int start_index, int end_index) const { return bits_->next_set_bit(start_index, end_index); }
    void set_store(int store_index) {
        assert(store_index >= first_store_index);
        bits_->set(store_index);
    }

private:
    std::shared_ptr<util::BitSet> bits_;
};

static_assert(DataFlowState<ReachingDefinitionsState>);

class ReachingDefinitionsVisitor : public DataFlowVisitor<ReachingDefinitionsState> {
public:
    using State = ReachingDefinitionsState;

    // Prepares the analysis for the variables of `scope` for which `pred` returns true ("analyzed variables").
    ReachingDefinitionsVisitor(il::ILFunction* scope, const std::function<bool(il::ILVariable*)>& pred,
                               const util::CancellationToken& cancellation_token);
    // Prepares the analysis for the variables of `scope` with analyzed_variables[v.index_in_function()] set.
    ReachingDefinitionsVisitor(il::ILFunction* scope, util::BitSet analyzed_variables, const util::CancellationToken& cancellation_token);

    bool is_analyzed_variable(il::ILVariable* v) const;

    void visit_st_loc(il::StLoc& inst) override;
    void visit_pinned_region(il::PinnedRegion& inst) override;

protected:
    void handle_match_store(il::MatchInstruction& inst) override;
    void begin_try_catch_handler(il::TryCatchHandler& inst) override;

    // All stores to `v` that reach the state, in store index order. Precondition: v is an analyzed variable.
    std::vector<il::ILInstruction*> get_stores(State at_state, il::ILVariable* v) const;
    // Whether `v` is potentially uninitialized in the state. Precondition: v is an analyzed variable.
    bool is_potentially_uninitialized(State at_state, il::ILVariable* v) const;

    util::CancellationToken cancellation_token;
    // The function being analyzed.
    il::ILFunction* scope;

private:
    void handle_store(il::ILInstruction* inst, il::ILVariable* v);
    static util::BitSet get_active_variable_bit_set(il::ILFunction* scope, const std::function<bool(il::ILVariable*)>& pred);
    // The list of stores of each analyzed variable (null for the others); the first entry is the null "uninitialized" store.
    static std::vector<std::optional<std::vector<il::ILInstruction*>>> find_all_stores_by_variable(
        il::ILFunction* scope, const util::BitSet& active_variables, const util::CancellationToken& cancellation_token);
    // The initial state: reachable bit and uninitialized variable bits set, store bits clear.
    State create_initial_state() const;

    // All stores of all variables of the scope: state[store_index] is true iff all_stores_[store_index] is a reaching definition.
    // all_stores_[firstStoreIndexForVariable[v]] is the null "uninitialized" entry of v, followed by v's stores.
    std::vector<il::ILInstruction*> all_stores_;
    // Maps the instructions in all_stores_ to their index (not the null entries, which have one index per variable).
    std::unordered_map<il::ILInstruction*, int> store_index_map_;
    // first_store_index_for_variable_[scope.variables.count()] == all_stores_.size()
    std::vector<int> first_store_index_for_variable_;
    // analyzed_variables_[v.index_in_function()] is true iff the analysis is enabled for the variable.
    util::BitSet analyzed_variables_;
};

}  // namespace cb::dotnet::decompiler::flow_analysis
