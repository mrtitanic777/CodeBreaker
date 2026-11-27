// dotnet/decompiler/il/transforms/introduce_ref_read_only_modifier_on_locals.cpp -- see introduce_ref_read_only_modifier_on_locals.hpp.
#include "dotnet/decompiler/il/transforms/introduce_ref_read_only_modifier_on_locals.hpp"

#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void IntroduceRefReadOnlyModifierOnLocals::run(ILFunction* function, ILTransformContext&) {
    for (ILVariable* variable : function->variables()) {
        if (variable->type()->kind() != typesystem::TypeKind::ByReference || variable->kind() == VariableKind::Parameter) continue;
        // ref readonly
        if (is_used_as_ref_readonly(variable)) {
            variable->is_ref_read_only = true;
            continue;
        }
    }
}

bool IntroduceRefReadOnlyModifierOnLocals::is_used_as_ref_readonly(ILVariable* variable) {
    for (IStoreInstruction* store_inst : variable->store_instructions()) {
        auto* store = as<StLoc>(store_inst->as_instruction());
        if (store == nullptr) continue;
        // C# requires the local to be ref readonly for this store:
        if (ILInlining::is_readonly_reference(store->value())) return true;
        // or it must be, to keep the meaning of a readonly.ldelema:
        ILInstruction* val = store->value();
        while (auto* ldflda = as<LdFlda>(val)) val = ldflda->target();
        auto* ldelema = as<LdElema>(val);
        if (ldelema != nullptr && ldelema->is_read_only) return true;
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::il::transforms
