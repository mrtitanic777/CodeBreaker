// dotnet/decompiler/flow_analysis/definite_assignment_visitor.cpp -- the definite assignment analysis
// (ICSharpCode.Decompiler.FlowAnalysis.DefiniteAssignmentVisitor).
#include "dotnet/decompiler/flow_analysis/definite_assignment_visitor.hpp"

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::flow_analysis {

using namespace il;

DefiniteAssignmentVisitor::DefiniteAssignmentVisitor(ILFunction* scope, const util::CancellationToken& cancellation_token)
    : cancellation_token_(cancellation_token), scope_(scope), variables_with_uninitialized_usage_(scope->variables().count()) {
    cancellation_token.throw_if_cancellation_requested();
    flags_requiring_manual_impl |= InstructionFlags::MayReadLocals | InstructionFlags::MayWriteLocals;
    initialize(State(scope->variables().count()));
}

bool DefiniteAssignmentVisitor::is_potentially_used_uninitialized(ILVariable* v) const {
    assert(v->function() == scope_);
    return variables_with_uninitialized_usage_[v->index_in_function()];
}

void DefiniteAssignmentVisitor::handle_store(ILVariable* v) {
    cancellation_token_.throw_if_cancellation_requested();
    if (v->function() == scope_) {
        // Mark the variable as initialized:
        state.mark_variable_initialized(v->index_in_function());
        // Note that this gets called even if the store is in unreachable code, but that is fine because
        // bottom_state.mark_variable_initialized() has no effect.

        // After the state change, propagate_state_on_exception() would have to be called, but because
        // mark_variable_initialized() only clears a bit, that is guaranteed to be a no-op.
    }
}

void DefiniteAssignmentVisitor::ensure_initialized(ILVariable* v) {
    if (v->function() == scope_ && state.is_potentially_uninitialized(v->index_in_function()))
        variables_with_uninitialized_usage_.set(v->index_in_function());
}

void DefiniteAssignmentVisitor::visit_st_loc(StLoc& inst) {
    inst.value()->accept_visitor(*this);
    handle_store(inst.variable());
}

void DefiniteAssignmentVisitor::handle_match_store(MatchInstruction& inst) { handle_store(inst.variable()); }

void DefiniteAssignmentVisitor::begin_try_catch_handler(TryCatchHandler& inst) {
    handle_store(inst.variable());
    DataFlowVisitor::begin_try_catch_handler(inst);
}

void DefiniteAssignmentVisitor::visit_pinned_region(PinnedRegion& inst) {
    inst.init()->accept_visitor(*this);
    handle_store(inst.variable());
    inst.body()->accept_visitor(*this);
}

void DefiniteAssignmentVisitor::visit_ld_loc(LdLoc& inst) { ensure_initialized(inst.variable()); }

void DefiniteAssignmentVisitor::visit_ld_loca(LdLoca& inst) {
    // A variable needs to be initialized before it can be taken by reference. The exception is if the variable is passed to an out
    // parameter (handled in visit_call).
    ensure_initialized(inst.variable());
}

void DefiniteAssignmentVisitor::visit_call(Call& inst) { handle_call(inst); }

void DefiniteAssignmentVisitor::visit_call_virt(CallVirt& inst) { handle_call(inst); }

void DefiniteAssignmentVisitor::visit_new_obj(NewObj& inst) { handle_call(inst); }

void DefiniteAssignmentVisitor::visit_il_function(ILFunction& inst) {
    debug_start_point(&inst);
    State state_before_function = state.clone();
    State state_on_exception_before_function = current_state_on_exception.clone();
    // Note: lambdas are handled at their point of declaration. Their body is visited immediately, because captured variables need to
    // be definitely initialized at this point. The state after the lambda body is ignored (by resetting to the state before),
    // because it is not known when the lambda will be invoked. This also makes this logic unsuitable for reaching definitions, as
    // the effect of stores in lambdas would not be seen. Only the simpler case of definite assignment can support lambdas.
    inst.body()->accept_visitor(*this);

    // For local functions the situation is similar to lambdas. However, the state of the declaration site is not used when visiting
    // local functions, but the state(s) of their points of use. Because more points of use may be found within the local
    // functions, a fixed-point iteration is used.
    bool changed;
    do {
        changed = false;
        for (ILFunction* nested_function : inst.local_functions) {
            if (!local_functions_needing_analysis_.contains(nested_function->reduced_method)) continue;
            local_functions_needing_analysis_.erase(nested_function->reduced_method);
            State state_on_entry = state_of_local_function_use_.at(nested_function->reduced_method);
            state.replace_with(state_on_entry);
            current_state_on_exception.replace_with(state_on_entry);
            nested_function->accept_visitor(*this);
            changed = true;
        }
    } while (changed);
    current_state_on_exception = state_on_exception_before_function;
    state = state_before_function;
    debug_end_point(&inst);
}

void DefiniteAssignmentVisitor::handle_call(CallInstruction& call) {
    debug_start_point(&call);
    bool has_out_args = false;
    for (ILInstruction* arg : call.arguments()) {
        ILVariable* v = nullptr;
        const IParameter* parameter = nullptr;
        if (arg->match_ld_loca(&v) && (parameter = call.get_parameter(arg->child_index())) != nullptr &&
            parameter->reference_kind() == typesystem::ReferenceKind::Out) {
            // Visiting ldloca would require the variable to be initialized, but out arguments need not be initialized.
            has_out_args = true;
        } else {
            arg->accept_visitor(*this);
        }
    }
    // Mark out arguments as initialized, but only after the whole call:
    if (has_out_args) {
        for (ILInstruction* arg : call.arguments()) {
            ILVariable* v = nullptr;
            const IParameter* parameter = nullptr;
            if (arg->match_ld_loca(&v) && (parameter = call.get_parameter(arg->child_index())) != nullptr &&
                parameter->reference_kind() == typesystem::ReferenceKind::Out)
                handle_store(v);
        }
    }
    handle_local_function_use(call.method);
    debug_end_point(&call);
}

void DefiniteAssignmentVisitor::handle_local_function_use(const IMethod* method) {
    if (method->is_local_function()) {
        if (auto it = state_of_local_function_use_.find(method); it != state_of_local_function_use_.end()) {
            State state_on_entry = it->second;
            if (!state.less_than_or_equal(state_on_entry)) {
                state_on_entry.join_with(state);
                local_functions_needing_analysis_.insert(method);
            }
        } else {
            state_of_local_function_use_.emplace(method, state.clone());
            local_functions_needing_analysis_.insert(method);
        }
    }
}

void DefiniteAssignmentVisitor::visit_ld_ftn(LdFtn& inst) {
    debug_start_point(&inst);
    handle_local_function_use(inst.method());
    debug_end_point(&inst);
}

}  // namespace cb::dotnet::decompiler::flow_analysis
