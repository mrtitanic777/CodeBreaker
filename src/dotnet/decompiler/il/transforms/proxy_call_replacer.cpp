// dotnet/decompiler/il/transforms/proxy_call_replacer.cpp -- see proxy_call_replacer.hpp.
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"
#include "dotnet/decompiler/il/transforms/proxy_call_replacer.hpp"

#include <memory>
#include <vector>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void ProxyCallReplacer::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().async_await) return;
    for (ILInstruction* descendant : function->descendants()) {
        if (auto* inst = as<CallInstruction>(descendant)) {
            run_on_call(inst, context);
        }
    }
}

void ProxyCallReplacer::run_on_call(CallInstruction* inst, ILTransformContext& context) {
    if (inst->method->is_static()) return;
    srm::EntityHandle token = inst->method->metadata_token();
    if (token.is_nil() || token.kind() != srm::HandleKind::MethodDefinition) return;
    srm::MethodDefinitionHandle handle(token);
    if (!is_defined_in_current_or_outer_class(inst->method, context.function()->method->declaring_type_definition())) return;
    if (!is_compiler_generated_or_is_in_compiler_generated_class(inst->method)) return;
    const srm::MetadataReader& metadata = context.pe_file().metadata();
    srm::MethodDefinition method_def = metadata.get_method_definition(handle);
    if (!has_body(method_def)) return;
    // Use the callee's generic context
    typesystem::GenericContext generic_context = typesystem::GenericContext::of(*inst->method);
    // partially copied from CSharpDecompiler
    std::unique_ptr<ILReader> il_reader = context.create_il_reader();
    srm::MethodBodyBlock body = context.pe_file().get_method_body(method_def.relative_virtual_address());
    ILFunction* proxy_function = il_reader->read_il(handle, body, generic_context, ILFunctionKind::TopLevelFunction, context.cancellation_token);
    ILTransformContext transform_context(context, proxy_function);
    proxy_function->run_transforms(early_il_transforms(), transform_context);
    auto* block_container = as<BlockContainer>(proxy_function->body());
    if (block_container == nullptr) return;
    if (block_container->blocks.count() != 1) return;
    Block* block = block_container->blocks[0];
    Call* call;
    ILInstruction* return_value;
    switch (block->instructions.count()) {
        case 1:
            // leave IL_0000 (call Test(ldloc this, ldloc A_1))
            if (!block->instructions[0]->match_leave(block_container, &return_value)) return;
            call = as<Call>(return_value);
            break;
        case 2:
            // call Test(ldloc this, ldloc A_1)
            // leave IL_0000(nop)
            call = as<Call>(block->instructions[0]);
            if (!block->instructions[1]->match_leave(block_container, &return_value)) return;
            if (!return_value->match_nop()) return;
            break;
        default: return;
    }
    if (call == nullptr || call->method->is_constructor()) {
        return;
    }
    if (call->method->is_static() || call->method->parameters().size() != inst->method->parameters().size()) {
        return;
    }
    // check if original arguments are only correct ldloc calls
    for (int i = 0; i < call->arguments().count(); i++) {
        ILInstruction* original_arg = call->arguments()[i];
        ILVariable* var;
        if (!original_arg->match_ld_loc(&var) || var->kind() != VariableKind::Parameter || var->index() != i - 1) {
            return;
        }
    }
    context.step("Replace proxy: " + inst->method->name() + " with " + call->method->name(), inst);
    // Apply the wrapper call's substitution to the actual method call.
    auto* new_inst = make<Call>(call->method->specialize_method(inst->method->substitution()));
    // copy flags
    new_inst->constrained_to = call->constrained_to;
    new_inst->il_stack_was_empty = inst->il_stack_was_empty;
    new_inst->is_tail = call->is_tail && inst->is_tail;
    // copy IL ranges
    new_inst->add_il_range(*inst);
    new_inst->arguments().replace_list(inst->arguments());
    inst->replace_with(new_inst);
    context.end_step(new_inst);
}

bool ProxyCallReplacer::is_defined_in_current_or_outer_class(const IMethod* method, const typesystem::ITypeDefinition* declaring_type_definition) {
    while (declaring_type_definition != nullptr) {
        if (method->declaring_type_definition() == declaring_type_definition) return true;
        declaring_type_definition = declaring_type_definition->declaring_type_definition();
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::il::transforms
