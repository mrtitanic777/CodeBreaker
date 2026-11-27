// dotnet/decompiler/il/transforms/remove_unconstrained_generic_reference_type_check.cxx -- see
// remove_unconstrained_generic_reference_type_check.hpp.
#include "dotnet/decompiler/il/transforms/remove_unconstrained_generic_reference_type_check.hpp"

#include <utility>
#include <vector>

#include "core/error.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void RemoveUnconstrainedGenericReferenceTypeCheck::run(Block* block, int pos, StatementTransformContext& context) {
    int start_pos = pos;
    // stloc temp(default.value ``0)
    ILVariable* temp;
    ILInstruction* default_value;
    if (!block->instructions[pos]->match_st_loc(&temp, &default_value)) return;
    const IType* type;
    if (!default_value->match_default_value(&type)) return;
    if (temp->store_count() != 2 || temp->load_count() != 1 || temp->address_count() != 1) return;
    pos++;
    // if (comp.o(ldloc temp == ldnull)) Block IL_002a {
    //     stloc temp(ldobj ``0(ldloc address))
    //     stloc address(ldloca temp)
    // }
    auto* if_inst = as<IfInstruction>(block->instructions.element_at_or_default(pos));
    if (if_inst == nullptr) return;
    ILInstruction* temp_load_for_null_check;
    if (!if_inst->condition()->match_comp_equals_null(&temp_load_for_null_check)) return;
    if (!temp_load_for_null_check->match_ld_loc(temp)) return;
    auto* dereference_block = as<Block>(if_inst->true_inst());
    if (dereference_block == nullptr) return;
    if (!is<Nop>(if_inst->false_inst())) return;
    if (dereference_block->instructions.count() != 2) return;
    auto* temp_store = as<StLoc>(dereference_block->instructions[0]);
    auto* address_reassign = as<StLoc>(dereference_block->instructions[1]);
    if (temp_store == nullptr || address_reassign == nullptr) return;
    if (temp_store->variable() != temp) return;
    ILInstruction* address_load_for_ld_obj;
    if (!temp_store->value()->match_ld_obj(&address_load_for_ld_obj, type)) return;
    if (!address_load_for_ld_obj->match_ld_loc(address_reassign->variable())) return;
    if (!address_reassign->value()->match_ld_loca(temp)) return;
    ILVariable* address = address_reassign->variable();
    if (address->store_count() != 2 || address->load_count() != 2 || address->address_count() != 0) return;
    pos++;
    // pos now is the first store to V_i
    // ...(constrained[``0].callvirt Method(ldobj.if.ref ``0(ldloc address), ldloc V_i ...))...
    ILInstruction* call_target = nullptr;
    int matching_loads = 0;
    for (LdLoc* load : address->load_instructions()) {
        if (address_load_for_ld_obj != load) {
            call_target = load;
            ++matching_loads;
        }
    }
    if (matching_loads != 1) throw UsageError("InvalidOperationException: Sequence does not contain exactly one matching element");
    auto* ld_obj_if_ref = as<LdObjIfRef>(call_target->parent());
    auto* call = ld_obj_if_ref != nullptr ? as<CallVirt>(ld_obj_if_ref->parent()) : nullptr;
    if (call == nullptr) return;
    if (call->arguments().count() == 0 || call->arguments()[0] != ld_obj_if_ref || call->constrained_to == nullptr || !type->equals(call->constrained_to))
        return;
    ILInstruction* containing_stmt = call;
    while (containing_stmt->parent() != block) {
        if (containing_stmt->parent() == nullptr) return;
        containing_stmt = containing_stmt->parent();
    }
    if (containing_stmt->child_index() < pos) return;
    // check if we can inline all temporaries used in the call:
    int temporary_init_index = containing_stmt->child_index() - 1;
    std::vector<std::pair<ILInstruction*, ILInstruction*>> replacements;
    for (int arg_index = call->arguments().count() - 1; arg_index > 0 && temporary_init_index >= pos; arg_index--) {
        ILInstruction* argument = call->arguments()[arg_index];
        if (auto* load = as<LdLoc>(argument)) {
            ILInstruction* expr;
            if (block->instructions[temporary_init_index]->match_st_loc(load->variable(), &expr) &&
                ILInlining::variable_can_be_used_for_inlining(load->variable())) {
                if (!ILInlining::can_move_into_call_virt(expr, containing_stmt, call, argument)) {
                    return;
                }
                replacements.emplace_back(argument, expr);
                temporary_init_index--;
            }
        }
    }
    // all stores to V_i processed?
    if (temporary_init_index != pos - 1) {
        return;
    }
    context.step("RemoveUnconstrainedGenericReferenceTypeCheck", block->instructions[start_pos]);
    for (const auto& [argument, expr] : replacements) {
        argument->replace_with(expr);
    }
    block->instructions.remove_range(start_pos, containing_stmt->child_index() - start_pos);
    context.end_step(containing_stmt);
}

}  // namespace cb::dotnet::decompiler::il::transforms
