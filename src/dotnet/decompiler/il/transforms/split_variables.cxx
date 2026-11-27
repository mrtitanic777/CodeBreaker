// dotnet/decompiler/il/transforms/split_variables.cxx -- see split_variables.hpp.
#include "dotnet/decompiler/il/transforms/split_variables.hpp"

#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/flow_analysis/reaching_definitions_visitor.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/union_find.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::TypeKind;

bool SplitVariables::is_candidate_variable(ILVariable* v) {
    switch (v->kind()) {
        case VariableKind::Local:
            for (LdLoca* ldloca : v->address_instructions()) {
                if (determine_address_use(ldloca, ldloca->variable()) == AddressUse::Unknown) {
                    // If we don't understand how the address is being used, we can't split the variable.
                    return false;
                }
            }
            return true;
        case VariableKind::StackSlot:
            // stack slots: are already split by construction, except for the locals-turned-stackslots in async functions or stack
            // slots handled by the infeasible path transform
            if (v->function()->is_async() || v->remove_if_redundant) {
                for (LdLoca* ldloca : v->address_instructions()) {
                    if (determine_address_use(ldloca, ldloca->variable()) == AddressUse::Unknown) return false;
                }
                return true;
            }
            return false;
        default:
            // parameters: avoid splitting parameters
            // pinned locals: must not split (doing so would extend the life of the pin to the end of the method)
            return false;
    }
}

SplitVariables::AddressUse SplitVariables::determine_address_use(ILInstruction* address_loading_instruction, ILVariable* target_var) {
    ILInstruction* parent = address_loading_instruction->parent();
    if (is<LdObj>(parent)) return AddressUse::Immediate;
    if (auto* stobj = as<StObj>(parent); stobj != nullptr && stobj->target() == address_loading_instruction) return AddressUse::Immediate;
    if (is<LdObjIfRef>(parent)) {
        // This is either an immediate use, or we need to check how the parent uses the address.
        return determine_address_use(parent, target_var);
    }
    if (auto* ldflda = as<LdFlda>(parent)) return determine_address_use(ldflda, target_var);
    if (is<Await>(parent)) {
        // GetAwaiter() may write to the struct, but shouldn't store the address for later use
        return AddressUse::Immediate;
    }
    if (auto* call = as<CallInstruction>(parent)) return handle_call(address_loading_instruction, target_var, call);
    if (auto* stloc = as<StLoc>(parent); stloc != nullptr && stloc->variable()->is_single_definition()) {
        // Address stored in local variable: also check all uses of that variable.
        if (!(stloc->variable()->kind() == VariableKind::StackSlot || stloc->variable()->kind() == VariableKind::Local)) return AddressUse::Unknown;
        ILInstruction* value = stloc->value();
        while (auto* ld_flda = as<LdFlda>(value)) {
            value = ld_flda->target();
        }
        if (value->op_code != OpCode::LdLoca) {
            // GroupStores only handles ref-locals correctly when they are supported by GetAddressLoadForRefLocalUse(), which only
            // works for ldflda*(ldloca)
            return AddressUse::Unknown;
        }
        for (LdLoc* load : stloc->variable()->load_instructions()) {
            if (determine_address_use(load, target_var) != AddressUse::Immediate) return AddressUse::Unknown;
        }
        return AddressUse::WithSupportedRefLocals;
    }
    return AddressUse::Unknown;
}

SplitVariables::AddressUse SplitVariables::handle_call(ILInstruction* address_loading_instruction, ILVariable* target_var, CallInstruction* call) {
    // Address is passed to method. We'll assume the method only uses the address locally, unless we can see an address being returned
    // from the method:
    const IType* return_type = is<NewObj>(call) ? call->method->declaring_type() : call->method->return_type();
    if (return_type->is_by_ref_like()) {
        // We exclude Span<T>.Item[int index] and ReadOnlySpan<T>.Item[int index], because it is known that this or members of this
        // cannot be returned by the method.
        if (!is_span_of_t_indexer_accessor(call->method)) {
            // If the address is returned from the method, it check whether it's consumed immediately. This can still be fine, as long
            // as we also check the consumer's other arguments for 'stloc targetVar'.
            if (determine_address_use(call, target_var) != AddressUse::Immediate) return AddressUse::Unknown;
        }
    }

    for (const IParameter* p : call->method->parameters()) {
        // catch "out Span<int>" and similar
        const IType* parameter_type = typesystem::skip_modifiers(p->type());
        if (parameter_type->kind() == TypeKind::ByReference &&
            dynamic_cast<const typesystem::ByReferenceType*>(parameter_type)->element_type()->is_by_ref_like())
            return AddressUse::Unknown;
    }
    // ensure there's no 'stloc target' in between the ldloca and the call consuming the address
    for (int i = address_loading_instruction->child_index() + 1; i < call->arguments().count(); i++) {
        for (ILInstruction* inst : call->arguments()[i]->descendants()) {
            auto* store = as<StLoc>(inst);
            if (store != nullptr && store->variable() == target_var) return AddressUse::Unknown;
        }
    }
    return AddressUse::Immediate;
}

bool SplitVariables::is_span_of_t_indexer_accessor(const IMethod* method) {
    const IType* declaring_type = method->declaring_type();
    if (!typesystem::is_known_type(declaring_type, KnownTypeCode::SpanOfT) &&
        !typesystem::is_known_type(declaring_type, KnownTypeCode::ReadOnlySpanOfT))
        return false;
    auto* property = dynamic_cast<const typesystem::IProperty*>(method->accessor_owner());
    if (property == nullptr || !property->is_indexer() || property->name() != "Item" || property->parameters().size() != 1) return false;
    const IType* return_type = property->return_type();
    if (return_type->kind() != TypeKind::ByReference) return false;
    const IType* rt = dynamic_cast<const typesystem::ByReferenceType*>(return_type)->element_type();
    const IParameter* param = property->parameters()[0];
    return typesystem::is_known_type(param->type(), KnownTypeCode::Int32) && rt->equals(declaring_type->type_arguments()[0]);
}

LdLoca* SplitVariables::get_address_load_for_ref_local_use(LdLoc* ldloc) {
    if (!ldloc->variable()->is_single_definition()) return nullptr;  // only single-definition variables can be supported ref locals
    const auto& stores = ldloc->variable()->store_instructions();
    if (stores.size() == 1) {
        if (auto* stloc = as<StLoc>(stores[0]->as_instruction())) {
            ILInstruction* value = stloc->value();
            while (auto* ld_flda = as<LdFlda>(value)) {
                value = ld_flda->target();
            }
            return as<LdLoca>(value);
        }
    }
    return nullptr;
}

// Uses the union-find structure to merge. Instructions in a group are stores to the same variable that must stay together (cannot be
// split).
class SplitVariables::GroupStores final : public flow_analysis::ReachingDefinitionsVisitor {
public:
    GroupStores(ILFunction* scope, const util::CancellationToken& cancellation_token)
        : ReachingDefinitionsVisitor(scope, [](ILVariable* v) { return is_candidate_variable(v); }, cancellation_token) {}


    void visit_ld_loc(LdLoc& inst) override {
        ReachingDefinitionsVisitor::visit_ld_loc(inst);
        handle_load(&inst);
        LdLoca* ref_local_address_load = get_address_load_for_ref_local_use(&inst);
        if (ref_local_address_load != nullptr) {
            // SupportedRefLocal: act as if we copy-propagated the ldloca to the point of use:
            handle_load(ref_local_address_load);
        }
    }

    void visit_ld_loca(LdLoca& inst) override {
        ReachingDefinitionsVisitor::visit_ld_loca(inst);
        handle_load(&inst);
    }

    // Gets the new variable for a LdLoc, StLoc or TryCatchHandler instruction.
    ILVariable* get_new_variable(IInstructionWithVariableOperand* inst) {
        IInstructionWithVariableOperand* representative = union_find_.find(inst);
        ILVariable* v;
        auto it = new_variables_.find(representative);
        if (it == new_variables_.end()) {
            ILVariable* old = inst->variable();
            v = make<ILVariable>(old->kind(), old->type(), old->stack_type, old->index());
            v->name = old->name;
            v->has_generated_name = old->has_generated_name;
            v->state_machine_field = old->state_machine_field;
            v->set_initial_value_is_initialized(old->initial_value_is_initialized());
            v->set_uses_initial_value(false);  // we'll set UsesInitialValue when we encounter an uninit load
            v->remove_if_redundant = old->remove_if_redundant;
            new_variables_.emplace(representative, v);
            old->function()->variables().add(v);
        } else {
            v = it->second;
        }
        if (inst->variable()->uses_initial_value()) {
            auto uninit = uninit_variable_usage_.find(inst->variable());
            if (uninit != uninit_variable_usage_.end() && uninit->second == inst) {
                v->set_uses_initial_value(true);
            }
        }
        return v;
    }

private:
    void handle_load(IInstructionWithVariableOperand* inst) {
        if (is_analyzed_variable(inst->variable())) {
            if (is_potentially_uninitialized(state, inst->variable())) {
                // merge all uninit loads together:
                auto uninit_load = uninit_variable_usage_.find(inst->variable());
                if (uninit_load != uninit_variable_usage_.end()) {
                    union_find_.merge(inst, uninit_load->second);
                } else {
                    uninit_variable_usage_.emplace(inst->variable(), inst);
                }
            }
            for (ILInstruction* store : get_stores(state, inst->variable())) {
                union_find_.merge(inst, as<IInstructionWithVariableOperand>(store));
            }
        }
    }

    util::UnionFind<IInstructionWithVariableOperand*> union_find_;

    // For each uninitialized variable, one representative instruction that potentially observes the uninitialized value of the
    // variable. Used to merge together all such loads of the same uninitialized value.
    std::unordered_map<ILVariable*, IInstructionWithVariableOperand*> uninit_variable_usage_;

    std::unordered_map<IInstructionWithVariableOperand*, ILVariable*> new_variables_;
};

void SplitVariables::run(ILFunction* function, ILTransformContext& context) {
    GroupStores group_stores(function, context.cancellation_token);
    function->body()->accept_visitor(group_stores);
    // Replace analyzed variables with their split versions:
    for (ILInstruction* inst : function->descendants()) {
        auto* with_variable = as<IInstructionWithVariableOperand>(inst);
        if (with_variable == nullptr) continue;
        if (group_stores.is_analyzed_variable(with_variable->variable())) {
            with_variable->set_variable(group_stores.get_new_variable(with_variable));
        }
    }
    function->variables().remove_dead();
}

}  // namespace cb::dotnet::decompiler::il::transforms
