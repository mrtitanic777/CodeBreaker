// dotnet/decompiler/flow_analysis/definite_assignment_visitor.hpp -- the definite assignment analysis: which variables of a function may
// be read (or have their address taken) before every path to the read has stored to them.
//
//     DefiniteAssignmentVisitor visitor(function, token);
//     function->body()->accept_visitor(visitor);
//     if (visitor.is_potentially_used_uninitialized(v)) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.FlowAnalysis: DefiniteAssignmentVisitor), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler). Lambdas are visited where they are declared (captured variables must be assigned there; the state
// after the lambda is ignored because it is unknown when it runs); local functions are visited with the state(s) of their points of
// use, by a fixed-point iteration. The state is a class beside the visitor (DefiniteAssignmentState, aliased as
// DefiniteAssignmentVisitor::State), because C++ needs the state type complete where the visitor derives from DataFlowVisitor<State>.
#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "dotnet/decompiler/flow_analysis/data_flow_visitor.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler::flow_analysis {

// The state for definite assignment analysis. bits[i]: some code path from the entry point to this state's position does not write
// to function.variables[i] (the variable is not definitely assigned there). The initial state has all bits set (nothing is definitely
// assigned); the bottom state has all bits clear. A copy shares the bit set with the original, as a copy of upstream's struct does.
class DefiniteAssignmentState {
public:
    DefiniteAssignmentState() = default;
    // The initial state.
    explicit DefiniteAssignmentState(int variable_count) : bits_(std::make_shared<util::BitSet>(variable_count)) {
        bits_->set(0, variable_count);
    }

    bool less_than_or_equal(const DefiniteAssignmentState& other_state) const { return bits_->is_subset_of(*other_state.bits_); }
    DefiniteAssignmentState clone() const { return DefiniteAssignmentState(bits_->clone(), 0); }
    void replace_with(const DefiniteAssignmentState& new_content) { bits_->replace_with(*new_content.bits_); }
    void join_with(const DefiniteAssignmentState& incoming_state) { bits_->union_with(*incoming_state.bits_); }
    // If no path to the end of the try block leaves a variable v uninitialized, there is no such path to the end of the whole
    // try-finally either (it cannot complete unless the try block does). That is: a variable is definitely assigned after the
    // try-finally if it is definitely assigned in either the try or the finally block. As the bits are the opposite of definite
    // assignment, the output bit is the AND of both.
    void trigger_finally(const DefiniteAssignmentState& finally_state) { bits_->intersect_with(*finally_state.bits_); }
    void replace_with_bottom() { bits_->clear_all(); }
    bool is_bottom() const { return !bits_->any(); }
    void mark_variable_initialized(int variable_index) { bits_->clear(variable_index); }
    bool is_potentially_uninitialized(int variable_index) const { return (*bits_)[variable_index]; }

private:
    DefiniteAssignmentState(util::BitSet bits, int) : bits_(std::make_shared<util::BitSet>(std::move(bits))) {}

    std::shared_ptr<util::BitSet> bits_;
};

static_assert(DataFlowState<DefiniteAssignmentState>);

class DefiniteAssignmentVisitor : public DataFlowVisitor<DefiniteAssignmentState> {
public:
    using State = DefiniteAssignmentState;

    DefiniteAssignmentVisitor(il::ILFunction* scope, const util::CancellationToken& cancellation_token);

    bool is_potentially_used_uninitialized(il::ILVariable* v) const;

    void visit_st_loc(il::StLoc& inst) override;
    void visit_ld_loc(il::LdLoc& inst) override;
    void visit_ld_loca(il::LdLoca& inst) override;
    void visit_call(il::Call& inst) override;
    void visit_call_virt(il::CallVirt& inst) override;
    void visit_new_obj(il::NewObj& inst) override;
    void visit_il_function(il::ILFunction& inst) override;
    void visit_pinned_region(il::PinnedRegion& inst) override;
    void visit_ld_ftn(il::LdFtn& inst) override;

protected:
    void handle_match_store(il::MatchInstruction& inst) override;
    void begin_try_catch_handler(il::TryCatchHandler& inst) override;

private:
    void handle_store(il::ILVariable* v);
    void ensure_initialized(il::ILVariable* v);
    void handle_call(il::CallInstruction& call);
    // For a use of a local function, remembers the current state as the state on entry for processing the function body later.
    void handle_local_function_use(const il::IMethod* method);

    util::CancellationToken cancellation_token_;
    il::ILFunction* scope_;
    util::BitSet variables_with_uninitialized_usage_;

    std::unordered_map<const il::IMethod*, State> state_of_local_function_use_;
    std::unordered_set<const il::IMethod*> local_functions_needing_analysis_;
};

}  // namespace cb::dotnet::decompiler::flow_analysis
