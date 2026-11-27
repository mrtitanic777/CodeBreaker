// dotnet/decompiler/il/transforms/local_function_decompiler.cxx -- see local_function_decompiler.hpp.
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"
#include "dotnet/decompiler/il/transforms/local_function_decompiler.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <regex>
#include <unordered_set>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/il/transforms/delegate_construction.hpp"
#include "dotnet/decompiler/il/transforms/transform_display_class_usage.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/local_function_method.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/tree_traversal.hpp"

namespace cb::dotnet::decompiler::il::transforms {

namespace {

// A list of type parameters that lives as long as the ILAst that refers to it.
struct TypeParameterList final : ArenaObject {
    std::vector<const typesystem::ITypeParameter*> items;
};

// (IMethod)method.MemberDefinition
const IMethod* method_definition(const IMethod* method) { return dynamic_cast<const IMethod*>(method->member_definition()); }

// IEntity.IsCompilerGenerated (NRExtensions)
bool is_compiler_generated(const typesystem::IEntity* entity) {
    return entity != nullptr && entity->has_attribute(typesystem::KnownAttribute::CompilerGenerated);
}

// the common ancestor of a and b of type T: the first ancestor of a of that type that is also one of b's
template <class T>
T* find_common_ancestor_instruction(ILInstruction* a, ILInstruction* b) {
    std::unordered_set<T*> ancestors_of_b;
    for (ILInstruction* ancestor : b->ancestors()) {
        if (auto* t = as<T>(ancestor)) ancestors_of_b.insert(t);
    }
    for (ILInstruction* ancestor : a->ancestors()) {
        if (auto* t = as<T>(ancestor); t != nullptr && ancestors_of_b.contains(t)) return t;
    }
    return nullptr;
}

// ILFunction.Variables.SingleOrDefault(VariableKindExtensions.IsThis)
ILVariable* single_this_variable(ILFunction* function) {
    ILVariable* result = nullptr;
    for (ILVariable* v : function->variables()) {
        if (!v->is_this()) continue;
        if (result != nullptr) throw UsageError("Sequence contains more than one matching element");
        result = v;
    }
    return result;
}

// function.Descendants.OfType<ILFunction>(), as a list (nothing of the walks below adds or removes a function)
std::vector<ILFunction*> descendant_functions(ILFunction* function) {
    std::vector<ILFunction*> result;
    for (ILInstruction* inst : function->descendants()) {
        if (auto* f = as<ILFunction>(inst)) result.push_back(f);
    }
    return result;
}

// GenericContextFromTypeArguments(targetMethod, skipCount): the type parameters of the method and its declaring type, the first
// `skipCount` of the method's (after the type's) replaced by the type arguments the use-site passes (which must be type parameters).
// An engaged result with null lists is upstream's default(GenericContext). The lists live in the current ILArena.
std::optional<typesystem::GenericContext> generic_context_from_type_arguments(const IMethod* target_method, int skip_count) {
    if (skip_count < 0 || skip_count > static_cast<int>(target_method->type_parameters().size())) {
        assert(false);
        return std::nullopt;
    }
    int total = static_cast<int>(target_method->declaring_type()->type_parameters().size()) + skip_count;
    if (total == 0) return typesystem::GenericContext();

    auto* class_type_parameters = ILArena::current().make<TypeParameterList>();
    auto* method_type_parameters = ILArena::current().make<TypeParameterList>();
    class_type_parameters->items = target_method->declaring_type()->type_parameters();
    method_type_parameters->items = target_method->type_parameters();
    // targetMethod.DeclaringType.TypeArguments.Concat(targetMethod.TypeArguments).Take(total)
    std::vector<const IType*> skipped_type_arguments = target_method->declaring_type()->type_arguments();
    skipped_type_arguments.insert(skipped_type_arguments.end(), target_method->type_arguments().begin(), target_method->type_arguments().end());
    if (static_cast<int>(skipped_type_arguments.size()) > total) skipped_type_arguments.resize(static_cast<std::size_t>(total));
    int idx = 0;
    for (const IType* skipped_ta : skipped_type_arguments) {
        int cur_idx;
        std::vector<const typesystem::ITypeParameter*>* cur_parameters;
        const std::vector<const IType*>* cur_args;
        if (idx < static_cast<int>(class_type_parameters->items.size())) {
            cur_idx = idx;
            cur_parameters = &class_type_parameters->items;
            cur_args = &target_method->declaring_type()->type_arguments();
        } else {
            cur_idx = idx - static_cast<int>(class_type_parameters->items.size());
            cur_parameters = &method_type_parameters->items;
            cur_args = &target_method->type_arguments();
        }
        if ((*cur_args)[static_cast<std::size_t>(cur_idx)]->kind() != typesystem::TypeKind::TypeParameter) break;
        (*cur_parameters)[static_cast<std::size_t>(cur_idx)] = dynamic_cast<const typesystem::ITypeParameter*>(skipped_ta);
        idx++;
    }
    if (idx != total) {
        assert(false);
        return std::nullopt;
    }

    return typesystem::GenericContext(&class_type_parameters->items, &method_type_parameters->items);
}

// FindTypeDecoder (the form that finds uses of a type-definition handle in the signatures of the module the type is in): a signature
// provider whose type is whether the type was found in it.
class FindTypeDecoder {
public:
    FindTypeDecoder(srm::TypeDefinitionHandle handle, const metadata::MetadataFile& declaring_module) : handle_(handle), declaring_module_(&declaring_module) {}

    bool get_array_type(bool element_type, const srm::ArrayShape&) { return element_type; }
    bool get_by_reference_type(bool element_type) { return element_type; }
    bool get_function_pointer_type(const srm::MethodSignature<bool>& signature) { return any_in_method_signature(signature); }

    static bool any_in_method_signature(const srm::MethodSignature<bool>& signature) {
        if (signature.return_type) return true;
        for (bool type : signature.parameter_types) {
            if (type) return true;
        }
        return false;
    }

    bool get_generic_instantiation(bool generic_type, const std::vector<bool>& type_arguments) {
        if (generic_type) return true;
        for (bool ta : type_arguments) {
            if (ta) return true;
        }
        return false;
    }
    bool get_generic_method_parameter(const int&, int) { return false; }
    bool get_generic_type_parameter(const int&, int) { return false; }
    bool get_modified_type(bool modifier, bool unmodified_type, bool) { return unmodified_type || modifier; }
    bool get_pinned_type(bool element_type) { return element_type; }
    bool get_pointer_type(bool element_type) { return element_type; }
    // typeCode == primitiveType (0: no primitive type, which no type code is)
    bool get_primitive_type(srm::PrimitiveTypeCode) { return false; }
    bool get_sz_array_type(bool element_type) { return element_type; }
    bool get_type_from_definition(const srm::MetadataReader& reader, srm::TypeDefinitionHandle handle, std::uint8_t) {
        return handle_ == handle && &reader == &declaring_module_->metadata();
    }
    // (no current module: a type reference never names the type)
    bool get_type_from_reference(const srm::MetadataReader&, srm::TypeReferenceHandle, std::uint8_t) { return false; }
    bool get_type_from_specification(const srm::MetadataReader& reader, const int& generic_context, srm::TypeSpecificationHandle handle, std::uint8_t) {
        return reader.get_type_specification(handle).decode_signature(*this, generic_context);
    }

private:
    srm::TypeDefinitionHandle handle_;
    const metadata::MetadataFile* declaring_module_;
};

// FindRefStructParameters: collects the type definitions that are the target of a by-reference type in a method signature.
class FindRefStructParameters {
public:
    std::vector<srm::TypeDefinitionHandle> ref_struct_types;

    srm::TypeDefinitionHandle get_array_type(srm::TypeDefinitionHandle, const srm::ArrayShape&) { return {}; }
    srm::TypeDefinitionHandle get_function_pointer_type(const srm::MethodSignature<srm::TypeDefinitionHandle>&) { return {}; }
    srm::TypeDefinitionHandle get_generic_instantiation(srm::TypeDefinitionHandle, const std::vector<srm::TypeDefinitionHandle>&) { return {}; }
    srm::TypeDefinitionHandle get_generic_method_parameter(const int&, int) { return {}; }
    srm::TypeDefinitionHandle get_generic_type_parameter(const int&, int) { return {}; }
    srm::TypeDefinitionHandle get_modified_type(srm::TypeDefinitionHandle, srm::TypeDefinitionHandle, bool) { return {}; }
    srm::TypeDefinitionHandle get_pinned_type(srm::TypeDefinitionHandle) { return {}; }
    srm::TypeDefinitionHandle get_pointer_type(srm::TypeDefinitionHandle) { return {}; }
    srm::TypeDefinitionHandle get_primitive_type(srm::PrimitiveTypeCode) { return {}; }
    srm::TypeDefinitionHandle get_sz_array_type(srm::TypeDefinitionHandle) { return {}; }

    srm::TypeDefinitionHandle get_by_reference_type(srm::TypeDefinitionHandle element_type) {
        if (!element_type.is_nil()) ref_struct_types.push_back(element_type);
        return element_type;
    }

    srm::TypeDefinitionHandle get_type_from_specification(const srm::MetadataReader&, const int&, srm::TypeSpecificationHandle, std::uint8_t) { return {}; }
    srm::TypeDefinitionHandle get_type_from_definition(const srm::MetadataReader&, srm::TypeDefinitionHandle handle, std::uint8_t) { return handle; }
    srm::TypeDefinitionHandle get_type_from_reference(const srm::MetadataReader&, srm::TypeReferenceHandle, std::uint8_t) { return {}; }
};

}  // namespace

void LocalFunctionDecompiler::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().local_functions) return;
    // Disable the transform if we are decompiling a display-class or local function method:
    // This happens if a local function or display class is selected in the ILSpy tree view.
    if (is_local_function_method(function->method, context) ||
        is_local_function_display_class(*function->method->parent_module()->metadata_file(),
                                        srm::TypeDefinitionHandle(function->method->declaring_type_definition()->metadata_token()), &context)) {
        return;
    }

    context_ = &context;
    current_type_definition_ = typesystem::SimpleTypeResolveContext(*function->method).current_type_definition();
    LocalFunctions local_functions;
    // Find all local functions declared inside this method, including nested local functions or local functions declared in lambdas.
    find_use_sites(function, context, local_functions);
    std::vector<std::shared_ptr<LocalFunctionInfo>> infos = local_functions.values();
    replace_references_to_display_class_this(infos);
    determine_capture_and_declaration_scopes(infos);
    propagate_closure_parameter_arguments(local_functions);
    transform_use_sites(infos);
}

void LocalFunctionDecompiler::replace_references_to_display_class_this(const std::vector<std::shared_ptr<LocalFunctionInfo>>& local_functions) {
    for (const std::shared_ptr<LocalFunctionInfo>& info_ptr : local_functions) {
        LocalFunctionInfo& info = *info_ptr;
        ILFunction* local_function = info.definition;
        if (local_function->method->is_static()) continue;
        ILVariable* this_var = single_this_variable(local_function);
        if (this_var == nullptr) continue;
        std::vector<ILInstruction*> use_site_arguments;
        for (ILInstruction* u : info.use_sites) {
            if (auto* call = as<CallInstruction>(u)) use_site_arguments.push_back(call->arguments()[0]);
        }
        ILInstruction* compatible_argument = find_compatible_argument(info, use_site_arguments, true);
        if (compatible_argument == nullptr) continue;
        context_->step("Replace 'this' with " + compatible_argument->to_string(), local_function);
        DelegateConstruction::ReplaceDelegateTargetVisitor visitor(compatible_argument, this_var);
        local_function->accept_visitor(visitor);
        determine_capture_and_declaration_scope(info, -1, compatible_argument);
    }
}

void LocalFunctionDecompiler::determine_capture_and_declaration_scopes(const std::vector<std::shared_ptr<LocalFunctionInfo>>& local_functions) {
    for (const std::shared_ptr<LocalFunctionInfo>& info_ptr : local_functions) {
        LocalFunctionInfo& info = *info_ptr;
        context_->cancellation_token.throw_if_cancellation_requested();
        if (info.definition == nullptr) {
            context_->function()->warnings.push_back("Could not decode local function '" + info.method->to_string() + "'");
            continue;
        }

        context_->step_start_group("Determine and move to declaration scope of " + info.definition->name, info.definition);
        try {
            ILFunction* local_function = info.definition;

            for (ILInstruction* use_site : info.use_sites) {
                determine_capture_and_declaration_scope(info, use_site);

                if (context_->function()->method->is_constructor()) {
                    if (local_function->declaration_scope == nullptr) {
                        local_function->declaration_scope = BlockContainer::find_closest_container(use_site);
                    } else {
                        local_function->declaration_scope = find_common_ancestor_instruction<BlockContainer>(use_site, local_function->declaration_scope);
                        if (local_function->declaration_scope == nullptr) {
                            local_function->declaration_scope = static_cast<BlockContainer*>(context_->function()->body());
                        }
                    }
                }
            }

            if (local_function->declaration_scope == nullptr) {
                local_function->declaration_scope = static_cast<BlockContainer*>(context_->function()->body());
            }

            ILFunction* declaring_function = get_declaring_function(local_function);
            if (declaring_function != context_->function()) {
                context_->step("Move " + local_function->name + " from " + context_->function()->name + " to " + declaring_function->name, local_function);
                context_->function()->local_functions.remove(local_function);
                declaring_function->local_functions.add(local_function);
            }

            int skip_count = 0;
            if (try_validate_skip_count(info, &skip_count) &&
                skip_count != dynamic_cast<const typesystem::LocalFunctionMethod*>(local_function->reduced_method)->number_of_compiler_generated_type_parameters()) {
                assert(false);
                context_->function()->warnings.push_back("Could not decode local function '" + info.method->to_string() + "'");
                if (declaring_function != context_->function()) {
                    declaring_function->local_functions.remove(local_function);
                }
            }
        } catch (...) {
            context_->step_end_group(true);
            throw;
        }
        context_->step_end_group(true);
    }
}

void LocalFunctionDecompiler::transform_use_sites(const std::vector<std::shared_ptr<LocalFunctionInfo>>& local_functions) {
    for (const std::shared_ptr<LocalFunctionInfo>& info_ptr : local_functions) {
        LocalFunctionInfo& info = *info_ptr;
        context_->cancellation_token.throw_if_cancellation_requested();
        if (info.definition == nullptr) continue;
        context_->step_start_group("TransformUseSites of " + info.definition->name, info.definition);
        try {
            for (ILInstruction* use_site : info.use_sites) {
                context_->step("Transform use-site at IL_" + std::format("{:04x}", use_site->start_il_offset()), use_site);
                if (auto* new_obj = as<NewObj>(use_site)) {
                    transform_to_local_function_reference(info.definition, new_obj);
                } else if (auto* call = as<CallInstruction>(use_site)) {
                    Call* call_replacement = transform_to_local_function_invocation(info.definition->reduced_method, call);
                    context_->end_step(call_replacement);
                } else if (auto* fnptr = as<LdFtn>(use_site)) {
                    const IMethod* specialize_method = info.definition->reduced_method->specialize_method(fnptr->method()->substitution());
                    auto* replacement = with_il_range(make<LdFtn>(specialize_method), fnptr);
                    fnptr->replace_with(replacement);
                    context_->end_step(replacement);
                } else {
                    throw UsageError("NotSupportedException");
                }
            }
        } catch (...) {
            context_->step_end_group();
            throw;
        }
        context_->step_end_group();
    }
}

void LocalFunctionDecompiler::propagate_closure_parameter_arguments(LocalFunctions& local_functions) {
    for (ILFunction* local_function : descendant_functions(context_->function())) {
        if (local_function->kind() != ILFunctionKind::LocalFunction) continue;
        context_->cancellation_token.throw_if_cancellation_requested();
        std::uint32_t token = local_function->method->metadata_token().token();
        std::shared_ptr<LocalFunctionInfo>* found = local_functions.find(token);
        if (found == nullptr) throw UsageError("The given key was not present in the dictionary.");
        LocalFunctionInfo& info = **found;

        auto add_as_argument = [&](int index, ILInstruction* argument) {
            switch (argument->op_code) {
            case OpCode::LdLoc:
            case OpCode::LdLoca:
            case OpCode::LdFlda:
            case OpCode::LdObj:
                if (index >= 0 && !is_closure_parameter(info.method->parameters()[static_cast<std::size_t>(index)], current_type_definition_)) return;
                break;
            default:
                if (index >= 0 && is_closure_parameter(info.method->parameters()[static_cast<std::size_t>(index)], current_type_definition_))
                    info.definition->warnings.push_back("Could not transform parameter " + std::to_string(index) + ": unsupported argument pattern");
                return;
            }

            info.local_function_arguments.get_or_add(index).push_back(argument);
        };

        for (ILInstruction* use_site : info.use_sites) {
            if (auto* new_obj = as<NewObj>(use_site)) {
                add_as_argument(-1, new_obj->arguments()[0]);
            } else if (auto* call = as<CallInstruction>(use_site)) {
                int first_argument_index;
                if (info.method->is_static()) {
                    first_argument_index = 0;
                } else {
                    first_argument_index = 1;
                    add_as_argument(-1, call->arguments()[0]);
                }
                for (int i = call->arguments().count() - 1; i >= first_argument_index; i--) {
                    add_as_argument(i - first_argument_index, call->arguments()[i]);
                }
            } else if (is<LdFtn>(use_site)) {
                // &LocalFunction is only possible, if the local function is declared static,
                // this means that the local function can be declared in the top-level scope.
                // Thus, there are no closure parameters that need propagation.
            } else {
                throw UsageError("NotSupportedException");
            }
        }

        context_->step_start_group("PropagateClosureParameterArguments of " + info.definition->name, info.definition);
        try {
            for (const auto& [index, arguments] : std::vector<std::pair<int, std::vector<ILInstruction*>>>(info.local_function_arguments.items())) {
                // info.Definition.Variables.SingleOrDefault(p => p.Kind == VariableKind.Parameter && p.Index == index)
                ILVariable* target_variable = nullptr;
                for (ILVariable* p : info.definition->variables()) {
                    if (p->kind() == VariableKind::Parameter && p->index().has_value() && *p->index() == index) {
                        if (target_variable != nullptr) throw UsageError("Sequence contains more than one matching element");
                        target_variable = p;
                    }
                }
                if (target_variable == nullptr) continue;
                ILInstruction* compatible_argument = find_compatible_argument(info, arguments);
                if (compatible_argument == nullptr) continue;
                context_->step("Replace '" + target_variable->to_string() + "' with '" + compatible_argument->to_string() + "'", info.definition);
                DelegateConstruction::ReplaceDelegateTargetVisitor visitor(compatible_argument, target_variable);
                info.definition->accept_visitor(visitor);
            }
        } catch (...) {
            context_->step_end_group(true);
            throw;
        }
        context_->step_end_group(true);
    }
}

ILInstruction* LocalFunctionDecompiler::find_compatible_argument(LocalFunctionInfo& info, const std::vector<ILInstruction*>& arguments, bool ignore_structure) {
    for (ILInstruction* arg : arguments) {
        if (auto* ld2 = as<IInstructionWithVariableOperand>(arg); ld2 != nullptr && (ignore_structure || info.definition->is_descendant_of(ld2->variable()->function())))
            return arg;
        ILVariable* v = resolve_ancestor_scope_reference(arg);
        if (v != nullptr) return make<LdLoc>(v);
    }
    return nullptr;
}

ILVariable* LocalFunctionDecompiler::resolve_ancestor_scope_reference(ILInstruction* inst) {
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    if (!inst->match_ld_fld(&target, &field)) return nullptr;
    if (field->type()->kind() != typesystem::TypeKind::Class) return nullptr;
    if (!(TransformDisplayClassUsage::is_potential_closure(*context_, field->type()->get_definition()) ||
          typesystem::type_equals(context_->function()->method->declaring_type(), field->type())))
        return nullptr;
    for (ILFunction* f : descendant_functions(context_->function())) {
        for (ILVariable* v : f->variables()) {
            const typesystem::ITypeDefinition* var_type = nullptr;
            ILInstruction* ignored_initializer = nullptr;
            if (!(TransformDisplayClassUsage::is_closure(*context_, v, &var_type, &ignored_initializer) && typesystem::type_equals(var_type, field->type()))) continue;
            return v;
        }
    }
    return nullptr;
}

ILFunction* LocalFunctionDecompiler::get_declaring_function(ILFunction* local_function) {
    if (local_function->declaration_scope == nullptr) return nullptr;
    ILInstruction* inst = local_function->declaration_scope;
    while (inst != nullptr) {
        if (auto* declaring_function = as<ILFunction>(inst)) return declaring_function;
        inst = inst->parent();
    }
    return nullptr;
}

bool LocalFunctionDecompiler::try_validate_skip_count(LocalFunctionInfo& info, int* skip_count) {
    *skip_count = 0;
    ILFunction* local_function = info.definition;
    if (local_function->method->type_parameters().empty()) return true;
    const IMethod* parent_method = static_cast<ILFunction*>(local_function->parent())->method;
    const IMethod* method = local_function->method;
    *skip_count = static_cast<int>(parent_method->declaring_type()->type_parameters().size()) - static_cast<int>(method->declaring_type()->type_parameters().size());

    if (*skip_count > 0) return false;
    *skip_count += static_cast<int>(parent_method->type_parameters().size());
    if (*skip_count < 0 || *skip_count > static_cast<int>(method->type_arguments().size())) return false;

    // (upstream's DEBUG block only asserts that the use-sites pass the same type arguments)
    return true;
}

void LocalFunctionDecompiler::find_use_sites(ILFunction* function, ILTransformContext& context, LocalFunctions& local_functions) {
    for (ILInstruction* inst : function->body()->descendants()) {
        context.cancellation_token.throw_if_cancellation_requested();
        auto* call = as<CallInstruction>(inst);
        auto* ldftn = as<LdFtn>(inst);
        if (call != nullptr && !call->method->is_local_function() && is_local_function_method(call->method, context)) {
            handle_use_site(call->method, call, context, local_functions);
        } else if (ldftn != nullptr && !ldftn->method()->is_local_function() && is_local_function_method(ldftn->method(), context)) {
            const IMethod* ignored_method = nullptr;
            ILInstruction* ignored_target = nullptr;
            const IType* ignored_type = nullptr;
            auto* new_obj = as<NewObj>(ldftn->parent());
            if (new_obj != nullptr && DelegateConstruction::match_delegate_construction(new_obj, &ignored_method, &ignored_target, &ignored_type))
                handle_use_site(ldftn->method(), new_obj, context, local_functions);
            else
                handle_use_site(ldftn->method(), ldftn, context, local_functions);
        }
    }
}

void LocalFunctionDecompiler::handle_use_site(const IMethod* target_method, ILInstruction* inst, ILTransformContext& context, LocalFunctions& local_functions) {
    std::uint32_t token = target_method->metadata_token().token();
    if (std::shared_ptr<LocalFunctionInfo>* existing = local_functions.find(token)) {
        (*existing)->use_sites.push_back(inst);
        return;
    }
    context.step_start_group("Read local function '" + target_method->name() + "'", inst);
    auto info = std::make_shared<LocalFunctionInfo>();
    info->use_sites.push_back(inst);
    info->method = method_definition(target_method);
    ILFunction* root_function = context.function();
    int skip_count = get_skip_count(root_function, target_method);
    info->definition = read_local_function_definition(root_function, target_method, skip_count);
    local_functions.set(token, info);
    if (info->definition != nullptr) {
        find_use_sites(info->definition, context, local_functions);
    }
    context.step_end_group();
}

ILFunction* LocalFunctionDecompiler::read_local_function_definition(ILFunction* root_function, const IMethod* target_method, int skip_count) {
    srm::MethodDefinition method_def = context_->pe_file().metadata().get_method_definition(srm::MethodDefinitionHandle(target_method->metadata_token()));
    std::optional<typesystem::GenericContext> generic_context = generic_context_from_type_arguments(target_method, skip_count);
    if (!generic_context.has_value()) return nullptr;
    ILFunction* function;
    bool has_body_ = has_body(method_def);
    if (!has_body_) {
        function = make<ILFunction>(target_method, 0, typesystem::GenericContext(generic_context->class_type_parameters, generic_context->method_type_parameters),
                                    make<Nop>(), ILFunctionKind::LocalFunction);
    } else {
        std::unique_ptr<ILReader> il_reader = context_->create_il_reader();
        srm::MethodBodyBlock body = context_->pe_file().get_method_body(method_def.relative_virtual_address());

        function = il_reader->read_il(srm::MethodDefinitionHandle(target_method->metadata_token()), body, *generic_context, ILFunctionKind::LocalFunction,
                                      context_->cancellation_token);
    }
    // Embed the local function into the parent function's ILAst, so that "Show steps" can show
    // how the local function body is being transformed.
    root_function->local_functions.add(function);
    if (has_body_) {
        function->declaration_scope = static_cast<BlockContainer*>(root_function->body());
#ifndef NDEBUG
        function->check_invariant(ILPhase::Normal);
#endif
        ILTransformContext nested_context(*context_, function);
        // function.RunTransforms(CSharpDecompiler.GetILTransforms().TakeWhile(t => !(t is LocalFunctionDecompiler)), nestedContext);
        std::vector<std::unique_ptr<IILTransform>> transforms = get_il_transforms();
        auto stop = std::find_if(transforms.begin(), transforms.end(),
                                 [](const auto& t) { return dynamic_cast<LocalFunctionDecompiler*>(t.get()) != nullptr; });
        transforms.erase(stop, transforms.end());
        function->run_transforms(transforms, nested_context);
        function->declaration_scope = nullptr;
    }
    function->reduced_method = reduce_to_local_function(function->method, skip_count);
    return function;
}

int LocalFunctionDecompiler::get_skip_count(ILFunction* root_function, const IMethod* target_method) {
    target_method = method_definition(target_method);
    int skip_count = static_cast<int>(root_function->method->declaring_type()->type_parameters().size()) +
                     static_cast<int>(root_function->method->type_parameters().size()) -
                     static_cast<int>(target_method->declaring_type()->type_parameters().size());
    if (skip_count < 0) {
        skip_count = 0;
    }
    if (!target_method->type_parameters().empty()) {
        // targetMethod.Parameters.Where(IsClosureParameter).SelectMany(p => p.Type.UnwrapByRef().TypeArguments)
        //     .Select(pt => (int?)targetMethod.TypeParameters.IndexOf(pt)).DefaultIfEmpty().Max()
        std::optional<int> last_params;
        for (const typesystem::IParameter* p : target_method->parameters()) {
            if (!is_closure_parameter(p, current_type_definition_)) continue;
            for (const IType* pt : typesystem::unwrap_by_ref(p->type())->type_arguments()) {
                int index = -1;
                const auto& type_parameters = target_method->type_parameters();
                for (std::size_t i = 0; i < type_parameters.size(); i++) {
                    if (typesystem::type_equals(type_parameters[i], pt)) {
                        index = static_cast<int>(i);
                        break;
                    }
                }
                if (!last_params.has_value() || index > *last_params) last_params = index;
            }
        }
        if (last_params.has_value() && *last_params + 1 > skip_count) skip_count = *last_params + 1;
    }
    return skip_count;
}

bool LocalFunctionDecompiler::is_closure_parameter(const typesystem::IParameter* parameter, const typesystem::ITypeResolveContext& context) {
    return is_closure_parameter(parameter, context.current_type_definition());
}

bool LocalFunctionDecompiler::is_closure_parameter(const typesystem::IParameter* parameter, const typesystem::ITypeDefinition* current_type_definition) {
    const auto* brt = dynamic_cast<const typesystem::ByReferenceType*>(parameter->type());
    if (brt == nullptr) return false;
    const typesystem::ITypeDefinition* type = brt->element_type()->get_definition();
    return type != nullptr && type->kind() == typesystem::TypeKind::Struct && TransformDisplayClassUsage::is_potential_closure(current_type_definition, type);
}

const IMethod* LocalFunctionDecompiler::reduce_to_local_function(const IMethod* method, int type_parameters_to_remove) {
    int parameters_to_remove = 0;
    for (int i = static_cast<int>(method->parameters().size()) - 1; i >= 0; i--) {
        if (!is_closure_parameter(method->parameters()[static_cast<std::size_t>(i)], current_type_definition_)) break;
        parameters_to_remove++;
    }
    auto can_be_static_local_function = [&]() {
        if (!context_->settings().static_local_functions) return false;
        // Cannot be static because there are closure parameters that will be removed
        if (parameters_to_remove > 0) return false;
        // no closure parameters, but static:
        // we can safely assume, this local function can be declared static
        if (method->is_static()) return true;
        // the local function is used in conjunction with a lambda, which means,
        // it is defined inside the display-class type
        const typesystem::ITypeDefinition* declaring_type = method->declaring_type_definition();
        if (!is_compiler_generated(declaring_type)) return false;
        // if there are no instance fields, we can make it a static local function
        typesystem::Filter<IField> instance_fields = [](const IField* f) { return !f->is_static(); };
        return declaring_type->get_fields(instance_fields).empty();
    };
    return typesystem::LocalFunctionMethod::create(method, method->name(), can_be_static_local_function(), parameters_to_remove, type_parameters_to_remove);
}

void LocalFunctionDecompiler::transform_to_local_function_reference(ILFunction* function, CallInstruction* use_site) {
    ILInstruction* target = use_site->arguments()[0];
    target->replace_with(with_il_range(make<LdNull>(), target));
    if (auto* with_var = as<IInstructionWithVariableOperand>(target); with_var != nullptr && with_var->variable()->kind() == VariableKind::Local) {
        with_var->variable()->set_kind(VariableKind::DisplayClassLocal);
    }
    auto* fnptr = dynamic_cast<IInstructionWithMethodOperand*>(use_site->arguments()[1]);
    const IMethod* specialize_method = function->reduced_method->specialize_method(fnptr->method()->substitution());
    ILInstruction* fnptr_instruction = use_site->arguments()[1];
    auto* replacement = with_il_range(make<LdFtn>(specialize_method), fnptr_instruction);
    use_site->arguments()[1]->replace_with(replacement);
}

Call* LocalFunctionDecompiler::transform_to_local_function_invocation(const IMethod* reduced_method, CallInstruction* use_site) {
    const IMethod* specialize_method = reduced_method->specialize_method(use_site->method->substitution());
    bool was_instance_call = !use_site->method->is_static();
    auto* replacement = make<Call>(specialize_method);
    int first_argument_index = was_instance_call ? 1 : 0;
    int argument_count = use_site->arguments().count();
    int number_of_compiler_generated_parameters = dynamic_cast<const typesystem::LocalFunctionMethod*>(reduced_method)->number_of_compiler_generated_parameters();
    int reduced_argument_count = argument_count - (number_of_compiler_generated_parameters + first_argument_index);
    // useSite.Arguments.Skip(firstArgumentIndex).Take(reducedArgumentCount)
    std::vector<ILInstruction*> reduced_arguments;
    for (int i = first_argument_index; i < argument_count && static_cast<int>(reduced_arguments.size()) < reduced_argument_count; i++)
        reduced_arguments.push_back(use_site->arguments()[i]);
    replacement->arguments().add_range(reduced_arguments);
    // copy flags
    replacement->constrained_to = use_site->constrained_to;
    replacement->il_stack_was_empty = use_site->il_stack_was_empty;
    replacement->is_tail = use_site->is_tail;
    // copy IL ranges
    replacement->add_il_range(*use_site);
    if (was_instance_call) {
        replacement->add_il_range(*use_site->arguments()[0]);
        ILVariable* variable = nullptr;
        if (use_site->arguments()[0]->match_ld_loc_ref(&variable) && variable->kind() == VariableKind::NamedArgument) {
            // remove the store instruction of the simple load, if it is a named argument.
            ILInstruction* store_inst = variable->store_instructions()[0]->as_instruction();
            static_cast<Block*>(store_inst->parent())->instructions.remove_at(store_inst->child_index());
        }
    }
    for (int i = 0; i < number_of_compiler_generated_parameters; i++) {
        replacement->add_il_range(*use_site->arguments()[argument_count - i - 1]);
    }
    use_site->replace_with(replacement);
    return replacement;
}

void LocalFunctionDecompiler::determine_capture_and_declaration_scope(LocalFunctionInfo& info, ILInstruction* use_site) {
    if (auto* call = as<CallInstruction>(use_site)) {
        const IMethod* ignored_method = nullptr;
        ILInstruction* ignored_target = nullptr;
        const IType* ignored_type = nullptr;
        if (DelegateConstruction::match_delegate_construction(use_site, &ignored_method, &ignored_target, &ignored_type)) {
            // if this is a delegate construction, skip the use-site, because the capture scope
            // was already determined when analyzing "this".
            return;
        }
        int first_argument_index = info.definition->method->is_static() ? 0 : 1;
        for (int i = call->arguments().count() - 1; i >= first_argument_index; i--) {
            if (!determine_capture_and_declaration_scope(info, i - first_argument_index, call->arguments()[i])) break;
        }
        if (first_argument_index > 0) {
            determine_capture_and_declaration_scope(info, -1, call->arguments()[0]);
        }
    } else if (is<LdFtn>(use_site)) {
        // &LocalFunction is only possible, if the local function is declared static,
        // this means that the local function can be declared in the top-level scope.
        // leave info.DeclarationScope null/unassigned.
    } else {
        throw UsageError("NotSupportedException");
    }
}

bool LocalFunctionDecompiler::determine_capture_and_declaration_scope(LocalFunctionInfo& info, int parameter_index, ILInstruction* arg) {
    ILFunction* function = info.definition;
    ILVariable* closure_var = nullptr;
    if (parameter_index >= 0) {
        if (!(parameter_index < static_cast<int>(function->method->parameters().size()) &&
              is_closure_parameter(function->method->parameters()[static_cast<std::size_t>(parameter_index)], current_type_definition_))) {
            return false;
        }
    }
    if (!(arg->match_ld_loc(&closure_var) || arg->match_ld_loca(&closure_var))) {
        closure_var = resolve_ancestor_scope_reference(arg);
        if (closure_var == nullptr) return false;
    }
    if (closure_var->kind() == VariableKind::NamedArgument) return false;

    // GetClosureInitializer
    ILInstruction* initializer = nullptr;
    {
        const typesystem::ITypeDefinition* type = typesystem::unwrap_by_ref(closure_var->type())->get_definition();
        if (type == nullptr) return false;
        if (closure_var->kind() == VariableKind::Parameter) return false;
        if (type->kind() == typesystem::TypeKind::Struct) {
            // variable.AddressInstructions.OrderBy(i => i.StartILOffset).First()
            std::vector<LdLoca*> addresses = closure_var->address_instructions();
            std::stable_sort(addresses.begin(), addresses.end(), [](LdLoca* a, LdLoca* b) { return a->start_il_offset() < b->start_il_offset(); });
            if (addresses.empty()) throw UsageError("Sequence contains no elements");
            initializer = Block::get_containing_statement(addresses.front());
        } else {
            initializer = static_cast<StLoc*>(closure_var->store_instructions()[0]->as_instruction());
        }
    }
    if (initializer == nullptr) return false;
    // determine the capture scope of closureVar and the declaration scope of the function
    BlockContainer* additional_scope = BlockContainer::find_closest_container(initializer);
    if (closure_var->capture_scope == nullptr)
        closure_var->capture_scope = additional_scope;
    else {
        BlockContainer* combined_scope = find_common_ancestor_instruction<BlockContainer>(closure_var->capture_scope, additional_scope);
        assert(combined_scope != nullptr);
        closure_var->capture_scope = combined_scope;
    }
    if (closure_var->kind() == VariableKind::Local) {
        closure_var->set_kind(VariableKind::DisplayClassLocal);
    }
    if (function->declaration_scope == nullptr) {
        function->declaration_scope = closure_var->capture_scope;
    } else {
        // closureVar.CaptureScope.Ancestors.OfType<ILFunction>().First()
        ILFunction* capture_function = nullptr;
        for (ILInstruction* ancestor : closure_var->capture_scope->ancestors()) {
            if (auto* f = as<ILFunction>(ancestor)) {
                capture_function = f;
                break;
            }
        }
        if (!is_in_nested_local_function(function->declaration_scope, capture_function))
            function->declaration_scope = find_common_ancestor_instruction<BlockContainer>(function->declaration_scope, closure_var->capture_scope);
    }
    return true;
}

bool LocalFunctionDecompiler::is_in_nested_local_function(BlockContainer* declaration_scope, ILFunction* function) {
    bool found = false;
    util::pre_order(
        function, [](ILFunction* f) { return std::vector<ILFunction*>(f->local_functions.begin(), f->local_functions.end()); },
        [&](ILFunction* f) {
            if (!found && declaration_scope->is_descendant_of(f->body())) found = true;
        });
    return found;
}

bool LocalFunctionDecompiler::is_local_function_reference(NewObj* inst, ILTransformContext& context) {
    if (inst == nullptr || inst->arguments().count() != 2 || inst->method->declaring_type()->kind() != typesystem::TypeKind::Delegate) return false;
    OpCode op_code = inst->arguments()[1]->op_code;

    return op_code == OpCode::LdFtn && is_local_function_method(dynamic_cast<IInstructionWithMethodOperand*>(inst->arguments()[1])->method(), context);
}

bool LocalFunctionDecompiler::is_local_function_method(const IMethod* method, ILTransformContext& context) {
    if (method->metadata_token().is_nil()) return false;
    return is_local_function_method(*method->parent_module()->metadata_file(), srm::MethodDefinitionHandle(method->metadata_token()), &context);
}

bool LocalFunctionDecompiler::is_local_function_method(const metadata::MetadataFile& module, srm::MethodDefinitionHandle method_handle, ILTransformContext* context) {
    if (context != nullptr && &context->pe_file() != &module) return false;

    const srm::MetadataReader& metadata = module.metadata();
    srm::MethodDefinition method = metadata.get_method_definition(method_handle);
    srm::TypeDefinitionHandle declaring_type = method.get_declaring_type();

    if ((method.attributes() & 0x0003) == 0 /* MethodAttributes.Assembly */ ||
        !(typesystem::is_compiler_generated(method_handle, metadata) || typesystem::is_compiler_generated(declaring_type, metadata)))
        return false;

    std::string ignored_caller_name, ignored_function_name;
    if (!parse_local_function_name(std::string(metadata.get_string(method.name())), &ignored_caller_name, &ignored_function_name)) return false;

    return true;
}

bool LocalFunctionDecompiler::local_function_needs_accessibility_change(const metadata::MetadataFile& module, srm::MethodDefinitionHandle method_handle) {
    if (!is_local_function_method(module, method_handle)) return false;

    const srm::MetadataReader& metadata = module.metadata();
    srm::MethodDefinition method = metadata.get_method_definition(method_handle);

    FindRefStructParameters visitor;
    int generic_context = 0;
    method.decode_signature(visitor, generic_context);

    for (srm::TypeDefinitionHandle h : visitor.ref_struct_types) {
        srm::TypeDefinition td = metadata.get_type_definition(h);
        if (typesystem::is_compiler_generated(h, metadata) && is_value_type(td, metadata)) return true;
    }

    return false;
}

bool LocalFunctionDecompiler::is_local_function_display_class(const metadata::MetadataFile& module, srm::TypeDefinitionHandle type_handle, ILTransformContext* context) {
    if (context != nullptr && &context->pe_file() != &module) return false;

    const srm::MetadataReader& metadata = module.metadata();
    srm::TypeDefinition type = metadata.get_type_definition(type_handle);

    if ((type.attributes() & 0x7) != 0x3 /* TypeAttributes.VisibilityMask != NestedPrivate */) return false;
    if (!has_generated_name(type_handle, metadata)) return false;

    srm::TypeDefinitionHandle declaring_type_handle = type.get_declaring_type();
    srm::TypeDefinition declaring_type = metadata.get_type_definition(declaring_type_handle);

    for (srm::MethodDefinitionHandle method : declaring_type.get_methods()) {
        if (!is_local_function_method(module, method, context)) continue;
        srm::MethodDefinition md = metadata.get_method_definition(method);
        FindTypeDecoder decoder(type_handle, module);
        int generic_context = 0;
        // ...ParameterTypes.Any(): any parameter at all
        if (!md.decode_signature(decoder, generic_context).parameter_types.empty()) return true;
    }

    return false;
}

// Newer Roslyn versions use the format "<callerName>g__functionName|x_y"; older versions use "<callerName>g__functionNamex_y".
bool LocalFunctionDecompiler::parse_local_function_name(const std::string& name, std::string* caller_name, std::string* function_name) {
    static const std::regex function_name_regex(R"(^<(.*)>g__([^|]*)\|?\d+(_\d+)?$)", std::regex::ECMAScript | std::regex::optimize);
    caller_name->clear();
    function_name->clear();
    // string.IsNullOrWhiteSpace
    if (std::all_of(name.begin(), name.end(), [](unsigned char c) { return c == ' ' || (c >= 9 && c <= 13); })) return false;
    std::smatch match;
    bool success = std::regex_match(name, match, function_name_regex);
    if (success) {
        *caller_name = match[1].str();
        *function_name = match[2].str();
    }
    return success;
}

}  // namespace cb::dotnet::decompiler::il::transforms
