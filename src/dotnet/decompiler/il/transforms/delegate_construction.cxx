// dotnet/decompiler/il/transforms/delegate_construction.cxx -- see delegate_construction.hpp.
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"
#include "dotnet/decompiler/il/transforms/delegate_construction.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <optional>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/il/transforms/combine_exits_transform.hpp"
#include "dotnet/decompiler/il/transforms/local_function_decompiler.hpp"
#include "dotnet/decompiler/il/transforms/transform_display_class_usage.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

namespace srm = cb::dotnet::metadata;

namespace {

// A list of type parameters that lives as long as the ILAst that refers to it.
struct TypeParameterList final : ArenaObject {
    std::vector<const typesystem::ITypeParameter*> items;
};

// IMember.HasGeneratedName (NRExtensions)
bool has_generated_name(const typesystem::IMember* member) { return member->name().starts_with("<"); }

// IEntity.IsCompilerGenerated (NRExtensions)
bool is_compiler_generated(const typesystem::IEntity* entity) {
    return entity != nullptr && entity->has_attribute(typesystem::KnownAttribute::CompilerGenerated);
}

bool has_only_read_only_properties(const typesystem::ITypeDefinition* type) {
    for (const typesystem::IProperty* property : type->properties()) {
        if (property->can_set()) return false;
    }
    return true;
}

// IType.IsAnonymousType (NRExtensions)
bool is_anonymous_type(const IType* type) {
    if (type == nullptr) return false;
    if (type->namespace_().empty() && is_generated_name(type->name()) &&
        (type->name().find("AnonType") != std::string::npos || type->name().find("AnonymousType") != std::string::npos)) {
        const typesystem::ITypeDefinition* td = type->get_definition();
        return td != nullptr && is_compiler_generated(td) && has_only_read_only_properties(td);
    }
    return false;
}

class ContainsAnonTypeVisitor final : public typesystem::TypeVisitor {
public:
    bool contains_anon_type = false;

    const IType* visit_other_type(const IType& type) override {
        if (is_anonymous_type(&type)) contains_anon_type = true;
        return TypeVisitor::visit_other_type(type);
    }

    const IType* visit_type_definition(const typesystem::ITypeDefinition& type) override {
        if (is_anonymous_type(&type)) contains_anon_type = true;
        return TypeVisitor::visit_type_definition(type);
    }
};

// IType.ContainsAnonymousType (NRExtensions)
bool contains_anonymous_type(const IType* type) {
    ContainsAnonTypeVisitor visitor;
    type->accept_visitor(visitor);
    return visitor.contains_anon_type;
}

bool contains_anonymous_type(const IMethod* method) {
    if (contains_anonymous_type(method->return_type())) return true;
    for (const typesystem::IParameter* p : method->parameters()) {
        if (contains_anonymous_type(p->type())) return true;
    }
    return false;
}

// function.Variables.SingleOrDefault(VariableKindExtensions.IsThis)
ILVariable* single_this_variable(ILFunction* function) {
    ILVariable* result = nullptr;
    for (ILVariable* v : function->variables()) {
        if (!v->is_this()) continue;
        if (result != nullptr) throw UsageError("Sequence contains more than one matching element");
        result = v;
    }
    return result;
}

// GenericContextFromTypeArguments(subst): the type parameters the substitution maps to, or none when an argument is not one. The
// lists live in the current ILArena.
std::optional<typesystem::GenericContext> generic_context_from_type_arguments(const typesystem::TypeParameterSubstitution& subst) {
    auto* class_type_parameters = ILArena::current().make<TypeParameterList>();
    auto* method_type_parameters = ILArena::current().make<TypeParameterList>();
    if (subst.class_type_arguments().has_value()) {
        for (const IType* t : *subst.class_type_arguments()) {
            const auto* tp = dynamic_cast<const typesystem::ITypeParameter*>(t);
            if (tp != nullptr)
                class_type_parameters->items.push_back(tp);
            else
                return std::nullopt;
        }
    }
    if (subst.method_type_arguments().has_value()) {
        for (const IType* t : *subst.method_type_arguments()) {
            const auto* tp = dynamic_cast<const typesystem::ITypeParameter*>(t);
            if (tp != nullptr)
                method_type_parameters->items.push_back(tp);
            else
                return std::nullopt;
        }
    }
    return typesystem::GenericContext(&class_type_parameters->items, &method_type_parameters->items);
}

}  // namespace

void DelegateConstruction::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().anonymous_methods) return;
    // upstream's try/finally: the previous context is back, and the method is off the stack, however this ends
    struct Restore {
        DelegateConstruction* self;
        ILTransformContext* prev_context;
        const typesystem::ITypeDefinition* prev_type_definition;
        ~Restore() {
            self->context_ = prev_context;
            self->current_type_definition_ = prev_type_definition;
            self->active_methods_.pop_back();
        }
    };
    ILTransformContext* prev_context = context_;
    const typesystem::ITypeDefinition* prev_type_definition = current_type_definition_;
    active_methods_.push_back(function->method->metadata_token());
    Restore restore{this, prev_context, prev_type_definition};
    context_ = &context;
    current_type_definition_ = typesystem::SimpleTypeResolveContext(*function->method).current_type_definition();
    const util::CancellationToken& cancellation_token = context.cancellation_token;
    for (ILInstruction* inst : function->descendants()) {
        cancellation_token.throw_if_cancellation_requested();
        const IMethod* target_method = nullptr;
        ILInstruction* target = nullptr;
        const IType* delegate_type = nullptr;
        if (!match_delegate_construction(inst, &target_method, &target, &delegate_type, false)) continue;
        context.step_start_group("TransformDelegateConstruction " + std::to_string(inst->start_il_offset()), inst);
        ILFunction* f = transform_delegate_construction(inst, target_method, target, delegate_type);
        if (auto* inst_with_var = f != nullptr ? as<IInstructionWithVariableOperand>(target) : nullptr) {
            ILVariable* v = inst_with_var->variable();
            if (v->kind() == VariableKind::Local) {
                v->set_kind(VariableKind::DisplayClassLocal);
            }
            if (v->is_single_definition() && v->store_instructions().size() == 1) {
                auto* store = as<StLoc>(v->store_instructions()[0]->as_instruction());
                if (store != nullptr && is<NewObj>(store->value())) {
                    v->capture_scope = BlockContainer::find_closest_container(store);
                }
            }
        }
        if (f != nullptr) {
            context.end_step(f);
        }
        context.step_end_group();
    }
}

bool DelegateConstruction::match_delegate_construction(ILInstruction* inst, const IMethod** target_method, ILInstruction** target,
                                                       const IType** delegate_type, bool allow_transformed) {
    *target_method = nullptr;
    *target = nullptr;
    *delegate_type = nullptr;
    if (auto* call = as<NewObj>(inst)) {
        if (call->arguments().count() != 2) return false;
        *target = call->arguments()[0];
        OpCode op_code = call->arguments()[1]->op_code;
        *delegate_type = call->method->declaring_type();
        if (!(op_code == OpCode::LdFtn || op_code == OpCode::LdVirtFtn || (allow_transformed && op_code == OpCode::ILFunction))) return false;
        *target_method = dynamic_cast<IInstructionWithMethodOperand*>(call->arguments()[1])->method();
    } else if (auto* ld_virt_delegate = as<LdVirtDelegate>(inst)) {
        *target = ld_virt_delegate->argument();
        *target_method = ld_virt_delegate->method();
        *delegate_type = ld_virt_delegate->type();
    } else {
        return false;
    }
    return (*delegate_type)->kind() == typesystem::TypeKind::Delegate || (*delegate_type)->kind() == typesystem::TypeKind::Unknown;
}

bool DelegateConstruction::is_anonymous_method(const typesystem::ITypeDefinition* decompiled_type_definition, const IMethod* method) {
    if (method == nullptr) return false;
    if (!(has_generated_name(method) || method->name().find('$') != std::string::npos || is_compiler_generated(method) ||
          TransformDisplayClassUsage::is_potential_closure(decompiled_type_definition, method->declaring_type_definition()) ||
          contains_anonymous_type(method))) {
        return false;
    }
    return true;
}

ILFunction* DelegateConstruction::transform_delegate_construction(ILInstruction* value, const IMethod* target_method, ILInstruction* target,
                                                                  const IType* delegate_type) {
    if (!is_anonymous_method(current_type_definition_, target_method)) return nullptr;
    if (target_method->metadata_token().is_nil()) return nullptr;
    if (LocalFunctionDecompiler::is_local_function_method(target_method, *context_)) return nullptr;
    if (!validate_delegate_target(target)) return nullptr;
    srm::MethodDefinitionHandle handle(target_method->metadata_token());
    for (const srm::EntityHandle& active : active_methods_) {
        if (active == srm::EntityHandle(handle)) {
            context_->function()->warnings.push_back(" Found self-referencing delegate construction. Abort transformation to avoid stack overflow.");
            return nullptr;
        }
    }
    srm::MethodDefinition method_definition = context_->pe_file().metadata().get_method_definition(handle);
    if (!has_body(method_definition)) return nullptr;
    std::optional<typesystem::GenericContext> generic_context = generic_context_from_type_arguments(target_method->substitution());
    if (!generic_context.has_value()) return nullptr;
    std::unique_ptr<ILReader> il_reader = context_->create_il_reader();
    srm::MethodBodyBlock body = context_->pe_file().get_method_body(method_definition.relative_virtual_address());
    ILFunction* function = il_reader->read_il(handle, body, *generic_context, ILFunctionKind::Delegate, context_->cancellation_token);
    function->delegate_type = delegate_type;
    // Embed the lambda into the parent function's ILAst, so that "Show steps" can show
    // how the lambda body is being transformed.
    value->replace_with(function);
#ifndef NDEBUG
    function->check_invariant(ILPhase::Normal);
#endif

    const std::string context_prefix = target_method->name();
    for (ILVariable* v : function->variables()) {
        if (v->kind() != VariableKind::Parameter) v->name = context_prefix + v->name.value_or("");
    }

    ILTransformContext nested_context(*context_, function);
    // function.RunTransforms(CSharpDecompiler.GetILTransforms().TakeWhile(t => !(t is DelegateConstruction)).Concat(GetTransforms()), nestedContext);
    std::vector<std::unique_ptr<IILTransform>> transforms = get_il_transforms();
    auto stop = std::find_if(transforms.begin(), transforms.end(), [](const auto& t) { return dynamic_cast<DelegateConstruction*>(t.get()) != nullptr; });
    transforms.erase(stop, transforms.end());
    transforms.push_back(std::make_unique<CombineExitsTransform>());  // GetTransforms()
    function->run_transforms(transforms, nested_context);
    nested_context.step("DelegateConstruction (ReplaceDelegateTargetVisitor)", function);
    ReplaceDelegateTargetVisitor replace_visitor(target, single_this_variable(function));
    function->accept_visitor(replace_visitor);
    // handle nested lambdas
    nested_context.step_start_group("DelegateConstruction (nested lambdas)", function);
    run(function, nested_context);
    nested_context.step_end_group();
    function->add_il_range(*target);
    function->add_il_range(*value);
    if (auto* call = as<Call>(value)) function->add_il_range(*call->arguments()[1]);
    return function;
}

bool DelegateConstruction::validate_delegate_target(ILInstruction* inst) {
    switch (inst->op_code) {
    case OpCode::LdNull: return true;
    case OpCode::LdLoc: return static_cast<LdLoc*>(inst)->variable()->is_single_definition();
    case OpCode::LdObj: {
        auto* ldobj = static_cast<LdObj*>(inst);
        // TODO : should make sure that the display-class 'this' is unused,
        // if the delegate target is ldobj(ldsflda field).
        if (is<LdsFlda>(ldobj->target())) return true;
        // TODO : ldfld chains must be validated more thoroughly, i.e., we should make sure
        // that the value of the field is never changed.
        ILInstruction* target = ldobj;
        while (is<LdObj>(target) || is<LdFlda>(target)) {
            if (auto* o = as<LdObj>(target)) {
                target = o->target();
                continue;
            }
            if (auto* f = as<LdFlda>(target)) {
                target = f->target();
                continue;
            }
        }
        return is<LdLoc>(target);
    }
    default: return false;
    }
}

// ---- ReplaceDelegateTargetVisitor

void DelegateConstruction::ReplaceDelegateTargetVisitor::default_visit(ILInstruction& inst) {
    for (ILInstruction* child : inst.children()) {
        child->accept_visitor(*this);
    }
}

void DelegateConstruction::ReplaceDelegateTargetVisitor::visit_il_function(ILFunction& function) {
    if (this_variable_ != nullptr && &function == this_variable_->function()) {
        ILVariable* v = nullptr;
        if (auto* l = as<LdLoc>(target_)) {
            v = l->variable();
        } else if (auto* lo = as<LdObj>(target_)) {
            ILInstruction* inner = lo->target();
            while (auto* ldf = as<LdFlda>(inner)) {
                inner = ldf->target();
            }
            if (auto* l2 = as<LdLoc>(inner)) v = l2->variable();
        }
        if (v != nullptr) function.captured_variables.insert(v);
    }
    ILVisitor::visit_il_function(function);
}

void DelegateConstruction::ReplaceDelegateTargetVisitor::visit_ld_loc(LdLoc& inst) {
    if (inst.variable() == this_variable_) {
        inst.replace_with(target_->clone());
        return;
    }
    ILVisitor::visit_ld_loc(inst);
}

void DelegateConstruction::ReplaceDelegateTargetVisitor::visit_ld_obj(LdObj& inst) {
    if (this_variable_ != nullptr && inst.target()->match_ld_loca(static_cast<const ILVariable*>(this_variable_))) {
        inst.replace_with(target_->clone());
        return;
    }
    ILVisitor::visit_ld_obj(inst);
}

}  // namespace cb::dotnet::decompiler::il::transforms
