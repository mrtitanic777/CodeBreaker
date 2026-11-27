// dotnet/decompiler/il/transforms/introduce_dynamic_type_on_locals.cxx -- see introduce_dynamic_type_on_locals.hpp.
#include "dotnet/decompiler/il/transforms/introduce_dynamic_type_on_locals.hpp"

#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void IntroduceDynamicTypeOnLocals::run(ILFunction* function, ILTransformContext&) {
    // function.Descendants.OfType<ILFunction>() (nothing here changes the tree)
    std::vector<ILFunction*> nested_functions;
    for (ILInstruction* inst : function->descendants()) {
        if (auto* f = as<ILFunction>(inst)) nested_functions.push_back(f);
    }
    for (ILFunction* nested_function : nested_functions) {
        std::unordered_set<int> dynamic_variables;
        for (ILVariable* variable : nested_function->variables()) {
            if (variable->kind() != VariableKind::Local && variable->kind() != VariableKind::StackSlot &&
                variable->kind() != VariableKind::ForeachLocal && variable->kind() != VariableKind::UsingLocal) {
                continue;
            }
            if (!typesystem::is_known_type(variable->type(), typesystem::KnownTypeCode::Object) || variable->load_count() == 0) continue;
            for (LdLoc* load : variable->load_instructions()) {
                bool is_dynamic_use;
                if (auto* dynamic_instruction = as<DynamicInstruction>(load->parent())) {
                    CSharpArgumentInfo argument_info = dynamic_instruction->get_argument_info_of_child(load->child_index());
                    is_dynamic_use = !argument_info.has_flag(CSharpArgumentInfoFlags::UseCompileTimeType);
                } else {
                    // awaiting a dynamic value: the awaited operand has type dynamic
                    auto* await = as<Await>(load->parent());
                    is_dynamic_use = await != nullptr && await->get_result_method != nullptr && await->get_result_method->declaring_type() != nullptr &&
                                     await->get_result_method->declaring_type()->kind() == typesystem::TypeKind::Dynamic;
                }
                if (is_dynamic_use) {
                    variable->set_type(&typesystem::SpecialType::dynamic());
                    if (variable->index().has_value() && variable->kind() == VariableKind::Local) {
                        dynamic_variables.insert(*variable->index());
                    }
                    break;
                }
            }
        }
        for (ILVariable* variable : nested_function->variables()) {
            if (variable->index().has_value() && variable->kind() == VariableKind::Local && dynamic_variables.contains(*variable->index())) {
                variable->set_type(&typesystem::SpecialType::dynamic());
                continue;
            }
        }
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
