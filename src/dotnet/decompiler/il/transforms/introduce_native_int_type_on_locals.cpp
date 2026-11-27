// dotnet/decompiler/il/transforms/introduce_native_int_type_on_locals.cpp -- see introduce_native_int_type_on_locals.hpp.
#include "dotnet/decompiler/il/transforms/introduce_native_int_type_on_locals.hpp"

#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void IntroduceNativeIntTypeOnLocals::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().native_integers) return;
    std::vector<ILFunction*> nested_functions;
    for (ILInstruction* inst : function->descendants()) {
        if (auto* f = as<ILFunction>(inst)) nested_functions.push_back(f);
    }
    for (ILFunction* nested_function : nested_functions) {
        std::unordered_map<int, const IType*> variable_type_mapping;
        for (ILVariable* variable : nested_function->variables()) {
            if (variable->kind() != VariableKind::Local && variable->kind() != VariableKind::StackSlot &&
                variable->kind() != VariableKind::PatternLocal && variable->kind() != VariableKind::ForeachLocal &&
                variable->kind() != VariableKind::UsingLocal) {
                continue;
            }
            if (!(typesystem::is_known_type(variable->type(), KnownTypeCode::IntPtr) ||
                  typesystem::is_known_type(variable->type(), KnownTypeCode::UIntPtr)))
                continue;
            bool is_used_as_native = false;
            for (LdLoc* load : variable->load_instructions()) {
                if (is_used_as_native_int(load)) {
                    is_used_as_native = true;
                    break;
                }
            }
            bool is_assigned_native_int = false;
            for (IStoreInstruction* store : variable->store_instructions()) {
                if (is_native_int_store(store, context.type_system())) {
                    is_assigned_native_int = true;
                    break;
                }
            }
            if (is_used_as_native || is_assigned_native_int) {
                variable->set_type(get_sign(*variable->type()) == Sign::Unsigned ? &typesystem::SpecialType::nuint()
                                                                                  : &typesystem::SpecialType::nint());
                if (variable->kind() == VariableKind::Local && variable->index().has_value())
                    variable_type_mapping[*variable->index()] = variable->type();
            }
        }
        for (ILVariable* variable : nested_function->variables()) {
            if (variable->kind() == VariableKind::Local && variable->index().has_value()) {
                auto it = variable_type_mapping.find(*variable->index());
                if (it != variable_type_mapping.end()) variable->set_type(it->second);
            }
        }
    }
}

bool IntroduceNativeIntTypeOnLocals::is_used_as_native_int(LdLoc* load) {
    ILInstruction* parent = load->parent();
    if (auto* binary = as<BinaryNumericInstruction>(parent)) return binary->underlying_result_type() == StackType::I;
    if (auto* bit_not = as<BitNot>(parent)) return bit_not->underlying_result_type() == StackType::I;
    if (auto* call = as<CallInstruction>(parent)) {
        const IParameter* parameter = call->get_parameter(load->child_index());
        return parameter != nullptr && is_csharp_native_integer_type(*parameter->type());
    }
    return false;
}

bool IntroduceNativeIntTypeOnLocals::is_native_int_store(IStoreInstruction* store, const typesystem::ICompilation& compilation) {
    if (auto* stloc = as<StLoc>(store->as_instruction())) {
        ILInstruction* value = stloc->value();
        if (auto* binary = as<BinaryNumericInstruction>(value); binary != nullptr && binary->underlying_result_type() == StackType::I)
            return true;
        if (auto* conv = as<Conv>(value); conv != nullptr && conv->result_type() == StackType::I) return true;
        const IType* inferred_type = infer_type(value, &compilation);
        return is_csharp_native_integer_type(*inferred_type);
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::il::transforms
