// dotnet/decompiler/il/transforms/transform_display_class_usage.cxx -- see transform_display_class_usage.hpp.
#include "dotnet/decompiler/il/transforms/transform_display_class_usage.hpp"

#include <algorithm>
#include <cassert>
#include <unordered_set>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/disassembler/il_parser.hpp"
#include "dotnet/decompiler/il/control_flow/async_await_decompiler.hpp"
#include "dotnet/decompiler/il/control_flow/yield_return_decompiler.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/transforms/early_expression_transforms.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/remove_dead_variable_init.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/metadata/metadata_token_helpers.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

namespace srm = cb::dotnet::metadata;

namespace {

// (IField)field.MemberDefinition
const IField* field_definition(const IField* field) { return dynamic_cast<const IField*>(field->member_definition()); }

// IEntity.IsCompilerGeneratedOrIsInCompilerGeneratedClass (NRExtensions)
bool is_compiler_generated_or_is_in_compiler_generated_class(const typesystem::IEntity* entity) {
    for (; entity != nullptr; entity = entity->declaring_type_definition())
        if (entity->has_attribute(typesystem::KnownAttribute::CompilerGenerated)) return true;
    return false;
}

// new SimpleTypeResolveContext(method).CurrentTypeDefinition
const ITypeDefinition* current_type_definition_of(const IMethod* method) {
    if (method == nullptr) return nullptr;
    return typesystem::SimpleTypeResolveContext(*method).current_type_definition();
}

// context.Function.Ancestors.OfType<ILFunction>().Last().Method
const IMethod* root_function_method(ILTransformContext& context) {
    ILFunction* last = nullptr;
    for (ILInstruction* inst : context.function()->ancestors()) {
        if (auto* f = as<ILFunction>(inst)) last = f;
    }
    return last != nullptr ? last->method : nullptr;
}

srm::ILOpCode decode_op_code_skip_nop(srm::BlobReader& reader) {
    srm::ILOpCode code;
    do {
        code = disassembler::decode_op_code(reader);
    } while (code == srm::ILOpCode::Nop);
    return code;
}

}  // namespace

// ---- VariableToDeclare

TransformDisplayClassUsage::VariableToDeclare::VariableToDeclare(DisplayClass* container, const IField* field, ILVariable* declared_variable)
    : container_(container), field_(field), declared_variable_(declared_variable) {
    assert(declared_variable == nullptr || declared_variable->state_machine_field == field);
}

std::string TransformDisplayClassUsage::VariableToDeclare::name() const { return field_->name(); }

bool TransformDisplayClassUsage::VariableToDeclare::has_initializer(ILInstruction* inst) const {
    return std::find(initializers.begin(), initializers.end(), inst) != initializers.end();
}

void TransformDisplayClassUsage::VariableToDeclare::propagate(ILVariable* variable) {
    declared_variable_ = variable;
    can_propagate_ = variable != nullptr;
}

ILVariable* TransformDisplayClassUsage::VariableToDeclare::get_or_declare() {
    if (declared_variable_ != nullptr) return declared_variable_;
    declared_variable_ = container_->variable->function()->register_variable(VariableKind::Local, field_->type(), field_->name());
    declared_variable_->set_initial_value_is_initialized(true);
    declared_variable_->set_uses_initial_value(uses_initial_value);
    declared_variable_->capture_scope = container_->capture_scope;
    return declared_variable_;
}

// ---- the transform

TransformDisplayClassUsage::TransformDisplayClassUsage() = default;
TransformDisplayClassUsage::~TransformDisplayClassUsage() = default;

TransformDisplayClassUsage::DisplayClass* TransformDisplayClassUsage::find_display_class(ILVariable* v) const {
    DisplayClass* const* found = display_classes_.find(v);
    return found != nullptr ? *found : nullptr;
}

TransformDisplayClassUsage::DisplayClass* TransformDisplayClassUsage::make_display_class(ILVariable* variable, const ITypeDefinition* type) {
    owned_display_classes_.push_back(std::make_unique<DisplayClass>(variable, type));
    return owned_display_classes_.back().get();
}

TransformDisplayClassUsage::VariableToDeclare* TransformDisplayClassUsage::make_variable_to_declare(DisplayClass* container, const IField* field,
                                                                                                    ILVariable* declared_variable) {
    owned_variables_.push_back(std::make_unique<VariableToDeclare>(container, field, declared_variable));
    return owned_variables_.back().get();
}

void TransformDisplayClassUsage::run(ILFunction* function, ILTransformContext& context) {
    if (context_ != nullptr) throw UsageError("Reentrancy in TransformDisplayClassUsage");
    try {
        context_ = &context;
        current_type_definition_ = current_type_definition_of(context.function()->method);
        analyze_function(function);
        transform(function);
    } catch (...) {
        clear_state();
        throw;
    }
    clear_state();
}

void TransformDisplayClassUsage::clear_state() {
    display_classes_.clear();
    display_class_copy_map_.clear();
    owned_display_classes_.clear();
    owned_variables_.clear();
    current_type_definition_ = nullptr;
    context_ = nullptr;
}

void TransformDisplayClassUsage::analyze_function(ILFunction* function) {
    // VisitFunction, VisitChildren and Visit of upstream's local functions
    auto visit_function = [&](ILFunction* f) {
        std::vector<ILVariable*> variables(f->variables().begin(), f->variables().end());
        for (ILVariable* v : variables) {
            DisplayClass* result = analyze_variable(v);
            if (result == nullptr || display_classes_.contains(result->variable)) continue;
            context_->step("Detected display-class " + result->variable->to_string(), result->initializer != nullptr ? result->initializer : f->body());
            display_classes_.set(result->variable, result);
        }
    };
    auto visit = [&](auto& self, ILInstruction* inst) -> void {
        if (auto* f = as<ILFunction>(inst)) {
            visit_function(f);
            for (ILInstruction* child : inst->children()) self(self, child);
        } else {
            for (ILInstruction* child : inst->children()) self(self, child);
        }
    };

    visit(visit, function);

    for (const auto& [v, display_class] : std::vector<std::pair<ILVariable*, DisplayClass*>>(display_classes_.items())) {
        if (!validate_display_class_uses(v, display_class)) display_classes_.erase(v);
    }

    for (DisplayClass* display_class : display_classes_.values()) {
        // handle uninitialized fields
        for (const IField* f : display_class->type->fields()) {
            if (display_class->variables_to_declare.contains(f)) continue;
            VariableToDeclare* variable = add_variable(display_class, nullptr, f);
            variable->uses_initial_value = true;
            display_class->variables_to_declare.set(field_definition(f), variable);
        }

        for (VariableToDeclare* v : display_class->variables_to_declare.values()) {
            if (v->can_propagate()) {
                ILVariable* variable_to_propagate = v->get_or_declare();
                if (variable_to_propagate->kind() != VariableKind::Parameter && !display_classes_.contains(variable_to_propagate)) v->propagate(nullptr);
            }
        }
    }
}

bool TransformDisplayClassUsage::validate_display_class_uses(ILVariable* v, DisplayClass* display_class) {
    // (the lists are copied: validating may register variables)
    std::vector<LdLoc*> loads = v->load_instructions();
    for (LdLoc* ldloc : loads) {
        if (!validate_use(v, display_class, ldloc)) return false;
    }
    std::vector<LdLoca*> addresses = v->address_instructions();
    for (LdLoca* ldloca : addresses) {
        if (!validate_use(v, display_class, ldloca)) return false;
    }
    return true;
}

bool TransformDisplayClassUsage::validate_use(ILVariable* v, DisplayClass* container, ILInstruction* use) {
    ILInstruction* parent = use->parent();
    const IField* field = nullptr;
    if (auto* ldflda = as<LdFlda>(parent)) {
        ILInstruction* target = nullptr;
        if (ldflda->match_ld_flda(&target, &field) && target == use) {
            const IField* key_field = field_definition(field);
            VariableToDeclare** found = container->variables_to_declare.find(key_field);
            VariableToDeclare* variable = found != nullptr ? *found : nullptr;
            if (variable == nullptr) {
                variable = add_variable(container, nullptr, field);
            }
            if (variable->can_propagate() && !is_plain_read_or_initializer_store(ldflda, variable)) {
                // The field is mutated after initialization (stored again, or its address
                // escapes). Propagation is only sound if the store can be redirected to
                // the propagated variable: possible for parameters (their remaining uses
                // are restricted in ResolveVariableToPropagate), but not for 'this' and
                // not for a variable that is itself a scalar-replaced display class.
                ILVariable* propagated_variable = variable->get_or_declare();
                if (propagated_variable->is_this() || display_classes_.contains(propagated_variable)) {
                    variable->propagate(nullptr);
                }
            }
            container->variables_to_declare.set(key_field, variable);
            return true;
        }
    }
    if (auto* stobj = as<StObj>(parent)) {
        ILInstruction* target = nullptr;
        ILInstruction* value = nullptr;
        const IType* ignored_type = nullptr;
        if (stobj->match_st_obj(&target, &value, &ignored_type) && value == use) {
            ILInstruction* load = nullptr;
            ILVariable* other_variable = nullptr;
            if (target->match_ld_flda(&load, &field) && load->match_ld_loc_ref(&other_variable)) {
                if (DisplayClass* other_display_class = find_display_class(other_variable)) {
                    if (VariableToDeclare** declared_var = other_display_class->variables_to_declare.find(field_definition(field)))
                        return (*declared_var)->can_propagate();
                }
            }
            return false;
        }
    }
    if (auto* stloc = as<StLoc>(parent)) {
        if (stloc->variable()->is_single_definition() && stloc->value() == use) {
            display_class_copy_map_.set(stloc->variable(), v);
            return validate_display_class_uses(stloc->variable(), container);
        }
    }
    return false;
}

/// True when the given field access is a plain read or one of the recorded
/// initializer stores; false for any other store or for an escaping address.
bool TransformDisplayClassUsage::is_plain_read_or_initializer_store(LdFlda* ldflda, VariableToDeclare* variable) {
    ILInstruction* parent = ldflda->parent();
    if (is<LdObj>(parent)) return true;
    if (auto* store = as<StObj>(parent); store != nullptr && store->target() == ldflda) return variable->has_initializer(store);
    return false;
}

TransformDisplayClassUsage::DisplayClass* TransformDisplayClassUsage::analyze_variable(ILVariable* v) {
    switch (v->kind()) {
    case VariableKind::Parameter:
        if (context_->settings().yield_return && v->function()->state_machine_compiled_with_mono && v->is_this())
            return handle_mono_state_machine(v->function(), v);
        return nullptr;
    case VariableKind::StackSlot:
    case VariableKind::Local:
    case VariableKind::DisplayClassLocal: return detect_display_class(v);
    case VariableKind::InitializerTarget: return detect_display_class_initializer(v);
    default: return nullptr;
    }
}

TransformDisplayClassUsage::DisplayClass* TransformDisplayClassUsage::detect_display_class(ILVariable* v) {
    const ITypeDefinition* definition;
    StLoc* first_store = v->store_instructions().empty() ? nullptr : as<StLoc>(v->store_instructions()[0]->as_instruction());
    if (v->kind() != VariableKind::StackSlot) {
        definition = v->type()->get_definition();
    } else if (!v->store_instructions().empty() && first_store != nullptr) {
        definition = infer_type(first_store->value(), &context_->type_system())->get_definition();
    } else {
        definition = nullptr;
    }
    if (!validate_display_class_definition(definition)) return nullptr;
    DisplayClass* result;
    switch (definition->kind()) {
    case typesystem::TypeKind::Class: {
        if (!v->is_single_definition()) return nullptr;
        StLoc* stloc = nullptr;
        if (v->store_instructions().size() == 1) stloc = as<StLoc>(v->store_instructions()[0]->as_instruction());
        if (stloc == nullptr) return nullptr;
        auto* new_obj = as<NewObj>(stloc->value());
        if (new_obj != nullptr && validate_constructor(*context_, new_obj->method)) {
            result = make_display_class(v, definition);
            result->capture_scope = v->capture_scope;
            result->initializer = stloc;
        } else {
            return nullptr;
        }

        handle_init_block(as<Block>(stloc->parent()), stloc->child_index() + 1, result, result->variable);
        break;
    }
    case typesystem::TypeKind::Struct:
        if (!v->store_instructions().empty()) return nullptr;
        assert(v->store_instructions().empty());
        result = make_display_class(v, definition);
        result->capture_scope = v->capture_scope;
        handle_init_block(find_display_struct_init_block(v), 0, result, result->variable);
        break;
    default: return nullptr;
    }

    if (is_mono_nested_capture_scope(definition)) {
        result->capture_scope = nullptr;
    }
    return result;
}

void TransformDisplayClassUsage::handle_init_block(Block* init_block, int start_index, DisplayClass* result, ILVariable* target_variable) {
    if (init_block == nullptr) return;
    for (int i = start_index; i < init_block->instructions.count(); i++) {
        ILInstruction* init = init_block->instructions[i];
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* ignored_value = nullptr;
        if (!init->match_st_fld(&target, &field, &ignored_value)) break;
        if (!target->match_ld_loc_ref(static_cast<const ILVariable*>(target_variable))) break;
        if (result->variables_to_declare.contains(field_definition(field))) break;
        VariableToDeclare* variable = add_variable(result, static_cast<StObj*>(init), field);
        result->variables_to_declare.set(field_definition(field), variable);
    }
}

namespace {

// Visit of FindDisplayStructInitBlock: a common ancestor of all uses of the variable v.
ILInstruction* find_uses_visit(ILInstruction* inst, ILVariable* v);

// returns null, if v is not used in this sub-tree; returns a descendant of inst, if it is the only use of v in this sub-tree.
ILInstruction* find_uses_visit_children(ILInstruction* inst, ILVariable* v) {
    // Visit all children of the instruction
    ILInstruction* result = nullptr;
    for (ILInstruction* child : inst->children()) {
        ILInstruction* new_result = find_uses_visit(child, v);
        // As soon as there is a second use of v in this sub-tree,
        // we can skip all other children and just return this node.
        if (result == nullptr) {
            result = new_result;
        } else if (new_result != nullptr) {
            return inst;
        }
    }
    return result;
}

ILInstruction* find_uses_visit(ILInstruction* inst, ILVariable* v) {
    if (auto* l = as<LdLoc>(inst); l != nullptr && l->variable() == v) return l;
    if (auto* s = as<StLoc>(inst); s != nullptr && s->variable() == v) return s;
    if (auto* la = as<LdLoca>(inst); la != nullptr && la->variable() == v) return la;
    return find_uses_visit_children(inst, v);
}

}  // namespace

Block* TransformDisplayClassUsage::find_display_struct_init_block(ILVariable* v) {
    ILInstruction* root = v->function()->body();
    ILInstruction* use = find_uses_visit(root, v);
    if (use == nullptr) return nullptr;
    // use?.Ancestors.OfType<Block>().FirstOrDefault()
    for (ILInstruction* ancestor : use->ancestors()) {
        if (auto* block = as<Block>(ancestor)) return block;
    }
    return nullptr;
}

TransformDisplayClassUsage::DisplayClass* TransformDisplayClassUsage::detect_display_class_initializer(ILVariable* v) {
    StLoc* store = v->store_instructions().size() == 1 ? as<StLoc>(v->store_instructions()[0]->as_instruction()) : nullptr;
    if (v->store_instructions().size() != 1 || store == nullptr) return nullptr;
    auto* initializer_block = as<Block>(store->parent());
    if (initializer_block == nullptr || initializer_block->kind != BlockKind::ObjectInitializer) return nullptr;
    auto* new_obj = as<NewObj>(store->value());
    if (new_obj == nullptr) return nullptr;
    const ITypeDefinition* definition = new_obj->method->declaring_type()->get_definition();
    if (!validate_display_class_definition(definition)) return nullptr;
    if (!validate_constructor(*context_, new_obj->method)) return nullptr;
    ILVariable* reference_variable = nullptr;
    if (!initializer_block->parent()->match_st_loc(&reference_variable)) return nullptr;
    if (!reference_variable->is_single_definition()) return nullptr;
    if (!(reference_variable->store_instructions().size() == 1 && is<StLoc>(reference_variable->store_instructions()[0]->as_instruction()))) return nullptr;
    DisplayClass* result = make_display_class(reference_variable, definition);
    result->capture_scope = reference_variable->capture_scope;
    result->initializer = initializer_block->parent();
    handle_init_block(initializer_block, 1, result, v);
    return result;
}

bool TransformDisplayClassUsage::validate_display_class_definition(const ITypeDefinition* definition) {
    if (definition == nullptr) return false;
    if (definition->parent_module()->metadata_file() != &context_->pe_file()) return false;
    // We do not want to accidentially transform state-machines and thus destroy them.
    srm::TypeDefinitionHandle token(definition->metadata_token());
    const srm::MetadataReader& metadata = definition->parent_module()->metadata_file()->metadata();
    if (control_flow::YieldReturnDecompiler::is_compiler_generator_enumerator(token, metadata)) return false;
    if (control_flow::AsyncAwaitDecompiler::is_compiler_generated_state_machine(token, metadata)) return false;
    if (!context_->settings().aggressive_scalar_replacement_of_aggregates) {
        if (definition->declaring_type_definition() == nullptr) return false;
        if (!is_potential_closure(*context_, definition)) return false;
    }
    return true;
}

bool TransformDisplayClassUsage::validate_constructor(ILTransformContext& context, const IMethod* method) {
    try {
        if (!method->parameters().empty()) return false;
        srm::MethodDefinitionHandle handle(method->metadata_token());
        const auto* module = dynamic_cast<const typesystem::MetadataModule*>(method->parent_module());
        const metadata::MetadataFile* file = module->metadata_file();
        if (handle.is_nil() || file != &context.pe_file()) return false;
        srm::MethodDefinition def = file->metadata().get_method_definition(handle);
        if (def.relative_virtual_address() == 0) return false;
        srm::MethodBodyBlock body = file->get_method_body(def.relative_virtual_address());
        // some compilers produce ctors with unused local variables
        // see https://github.com/icsharpcode/ILSpy/issues/2174
        //if (!body.LocalSignature.IsNil)
        //	return false;
        if (!body.exception_regions().empty()) return false;
        srm::BlobReader reader = body.il_reader();
        if (reader.length() < 7) return false;
        // IL_0000: ldarg.0
        // IL_0001: call instance void [mscorlib]System.Object::.ctor()
        // IL_0006: ret
        srm::ILOpCode op_code = decode_op_code_skip_nop(reader);
        switch (op_code) {
        case srm::ILOpCode::Ldarg:
        case srm::ILOpCode::Ldarg_s:
            if (disassembler::decode_index(reader, op_code) != 0) return false;
            break;
        case srm::ILOpCode::Ldarg_0:
            // OK
            break;
        default: return false;
        }
        if (decode_op_code_skip_nop(reader) != srm::ILOpCode::Call) return false;
        srm::EntityHandle base_ctor_handle = metadata::entity_handle_or_nil(reader.read_int32());
        if (base_ctor_handle.is_nil()) return false;
        const IMethod* object_ctor = module->resolve_method(base_ctor_handle, typesystem::GenericContext());
        if (!typesystem::is_known_type(object_ctor->declaring_type(), typesystem::KnownTypeCode::Object)) return false;
        if (!object_ctor->is_constructor() || !object_ctor->parameters().empty()) return false;
        return decode_op_code_skip_nop(reader) == srm::ILOpCode::Ret;
    } catch (const srm::BadImageFormatError&) {
        return false;
    }
}

TransformDisplayClassUsage::VariableToDeclare* TransformDisplayClassUsage::add_variable(DisplayClass* result, StObj* statement, const IField* field) {
    VariableToDeclare* variable = make_variable_to_declare(result, field);
    if (statement != nullptr) {
        variable->propagate(resolve_variable_to_propagate(statement->value(), field->type()));
        variable->initializers.push_back(statement);
    }
    std::optional<bool> is_reference_type = result->type->is_reference_type();
    variable->uses_initial_value = !is_reference_type.has_value() || *is_reference_type || result->variable->uses_initial_value();
    return variable;
}

/// Resolves references to variables that can be propagated.
/// If a value does not match either LdLoc or a LdObj LdLdFlda* LdLoc chain, null is returned.
/// The if any of the variables/fields in the chain cannot be propagated, null is returned.
ILVariable* TransformDisplayClassUsage::resolve_variable_to_propagate(ILInstruction* value, const IType* expected_type) {
    if (auto* load = as<LdLoc>(value)) {
        ILVariable* v = load->variable();
        if (v->kind() == VariableKind::Parameter) {
            if (v->load_count() != 1 && !v->is_this()) {
                // If the variable is a parameter and it is used elsewhere, we cannot propagate it.
                // "dc.field = v; dc.field.mutate(); use(v);" cannot turn to "v.mutate(); use(v)"
                return nullptr;
            }
        } else {
            // Non-parameter propagation will later be checked, and will only be allowed for display classes
            std::optional<bool> is_reference_type = v->type()->is_reference_type();
            if (!(is_reference_type.has_value() && *is_reference_type)) {
                // don't allow propagation for display structs (as used with local functions)
                return nullptr;
            }
        }
        if (!v->is_single_definition()) {
            // "dc.field = v; v = 42; use(dc.field)" cannot turn to "v = 42; use(v);"
            return nullptr;
        }
        if (!(expected_type == nullptr || v->kind() == VariableKind::StackSlot ||
              typesystem::NormalizeTypeVisitor::ignore_nullability().equivalent_types(v->type(), expected_type)))
            return nullptr;
        return v;
    }
    if (auto* ldfld = as<LdObj>(value)) {
        DisplayClass* current_display_class = nullptr;
        for (ILInstruction* item : ldfld->target()->descendants()) {
            ILVariable* v = nullptr;
            if (is_display_class_load(item, &v)) {
                current_display_class = find_display_class(v);
                if (current_display_class == nullptr) return nullptr;
            }
            if (current_display_class == nullptr) return nullptr;
            if (auto* ldf = as<LdFlda>(item)) {
                VariableToDeclare** vd = current_display_class->variables_to_declare.find(field_definition(ldf->field()));
                if (vd != nullptr) {
                    if (!(*vd)->can_propagate()) return nullptr;
                    current_display_class = find_display_class((*vd)->get_or_declare());
                    if (current_display_class == nullptr) return nullptr;
                }
            }
        }
        return current_display_class->variable;
    }
    return nullptr;
}

void TransformDisplayClassUsage::transform(ILFunction* function) {
    visit_il_function(*function);
    context_->step("ResetHasInitialValueFlag", function);
    std::vector<ILFunction*> functions;
    for (ILInstruction* inst : function->descendants()) {
        if (auto* f = as<ILFunction>(inst)) functions.push_back(f);
    }
    for (ILFunction* f : functions) {
        RemoveDeadVariableInit::reset_uses_initial_value_flag(f, *context_);
        for (auto it = f->captured_variables.begin(); it != f->captured_variables.end();) {
            if ((*it)->is_dead())
                it = f->captured_variables.erase(it);
            else
                ++it;
        }
    }
}

bool TransformDisplayClassUsage::is_closure(ILTransformContext& context, ILVariable* variable, const ITypeDefinition** closure_type,
                                            ILInstruction** initializer) {
    *closure_type = nullptr;
    *initializer = nullptr;
    if (variable->is_single_definition() && variable->store_instructions().size() == 1) {
        if (auto* inst = as<StLoc>(variable->store_instructions()[0]->as_instruction())) {
            *initializer = inst;
            if (is_closure_init(context, inst, closure_type)) {
                return true;
            }
        }
    }
    *closure_type = variable->type()->get_definition();
    if (context.settings().local_functions && *closure_type != nullptr && (*closure_type)->kind() == typesystem::TypeKind::Struct &&
        variable->uses_initial_value() && is_potential_closure(context, *closure_type)) {
        // variable.AddressInstructions.OrderBy(i => i.StartILOffset).First()
        std::vector<LdLoca*> addresses = variable->address_instructions();
        std::stable_sort(addresses.begin(), addresses.end(), [](LdLoca* a, LdLoca* b) { return a->start_il_offset() < b->start_il_offset(); });
        if (addresses.empty()) throw UsageError("Sequence contains no elements");
        *initializer = Block::get_containing_statement(addresses.front());
        return true;
    }
    return false;
}

bool TransformDisplayClassUsage::is_closure_init(ILTransformContext& context, StLoc* inst, const ITypeDefinition** closure_type) {
    if (auto* new_obj = as<NewObj>(inst->value())) {
        *closure_type = new_obj->method->declaring_type_definition();
        return *closure_type != nullptr && is_potential_closure(context, new_obj);
    }
    *closure_type = nullptr;
    return false;
}

bool TransformDisplayClassUsage::is_mono_nested_capture_scope(const ITypeDefinition* closure_type) {
    if (closure_type->name().find("AnonStorey") == std::string::npos) return false;
    const ITypeDefinition* decompiled = current_type_definition_of(context_->function()->method);
    for (const IField* f : closure_type->fields()) {
        if (is_potential_closure(decompiled, f->return_type()->get_definition())) return true;
    }
    return false;
}

/// mcs likes to optimize closures in yield state machines away by moving the captured variables' fields into the state machine type,
/// We construct a DisplayClass that spans the whole method body.
TransformDisplayClassUsage::DisplayClass* TransformDisplayClassUsage::handle_mono_state_machine(ILFunction* function, ILVariable* this_variable) {
    if (!(function->state_machine_compiled_with_mono && this_variable->is_this())) return nullptr;
    // Special case for Mono-compiled yield state machines
    const ITypeDefinition* closure_type = this_variable->type()->get_definition();
    if (!(closure_type != current_type_definition_ && is_potential_closure(current_type_definition_, closure_type, true))) return nullptr;

    DisplayClass* display_class = make_display_class(this_variable, this_variable->type()->get_definition());
    display_class->capture_scope = static_cast<BlockContainer*>(function->body());
    for (ILVariable* state_machine_variable : function->variables().items()) {
        if (state_machine_variable->state_machine_field == nullptr || display_class->variables_to_declare.contains(state_machine_variable->state_machine_field))
            continue;
        VariableToDeclare* variable_to_declare = make_variable_to_declare(display_class, state_machine_variable->state_machine_field, state_machine_variable);
        display_class->variables_to_declare.set(state_machine_variable->state_machine_field, variable_to_declare);
    }

    // FindThisField
    const IField* this_field = nullptr;
    {
        typesystem::Filter<IField> filter = [&](const IField* f2) {
            return !f2->is_static() && !display_class->variables_to_declare.contains(f2) && f2->type()->get_definition() == current_type_definition_;
        };
        std::vector<const IField*> fields = closure_type->get_fields(filter);
        if (!fields.empty()) this_field = fields.front();
    }
    if (!function->method->is_static() && this_field != nullptr) {
        ILVariable* this_var = nullptr;
        for (ILVariable* t : function->variables()) {
            if (t->is_this() && t->type()->get_definition() == current_type_definition_) {
                this_var = t;
                break;
            }
        }
        if (this_var == nullptr) {
            this_var = make<ILVariable>(VariableKind::Parameter, current_type_definition_, std::optional<int>(-1));
            this_var->name = "this";
            this_var->state_machine_field = this_field;
            function->variables().add(this_var);
        } else if (this_var->state_machine_field != nullptr && display_class->variables_to_declare.contains(this_var->state_machine_field)) {
            // "this" was already added previously, no need to add it twice.
            return display_class;
        }
        VariableToDeclare* variable_to_declare = make_variable_to_declare(display_class, this_field, this_var);
        display_class->variables_to_declare.set(this_field, variable_to_declare);
    }
    return display_class;
}

bool TransformDisplayClassUsage::is_potential_closure(ILTransformContext& context, NewObj* inst) {
    const ITypeDefinition* decompiled = current_type_definition_of(root_function_method(context));
    return is_potential_closure(decompiled, inst->method->declaring_type_definition());
}

bool TransformDisplayClassUsage::is_potential_closure(ILTransformContext& context, const ITypeDefinition* potential_display_class) {
    const ITypeDefinition* decompiled = current_type_definition_of(root_function_method(context));
    return is_potential_closure(decompiled, potential_display_class);
}

bool TransformDisplayClassUsage::is_potential_closure(const ITypeDefinition* decompiled_type_definition, const ITypeDefinition* potential_display_class,
                                                      bool allow_type_implementing_interfaces) {
    if (potential_display_class == nullptr || !is_compiler_generated_or_is_in_compiler_generated_class(potential_display_class)) return false;
    switch (potential_display_class->kind()) {
    case typesystem::TypeKind::Struct: break;
    case typesystem::TypeKind::Class:
        if (!allow_type_implementing_interfaces) {
            for (const IType* t : potential_display_class->direct_base_types()) {
                if (!typesystem::is_known_type(t, typesystem::KnownTypeCode::Object)) return false;
            }
        }
        break;
    default: return false;
    }

    // Make sure that potentialDisplayCLass and decompiledTypeDefinition are part of the same type tree
    // Either decompiledTypeDefinition is an ancestor type of potentialDisplayClass or both have
    // at least one common ancestor.
    std::unordered_set<const ITypeDefinition*> potential_display_class_ancestors;
    const ITypeDefinition* potential_display_class_parent = potential_display_class->declaring_type_definition();
    while (potential_display_class_parent != nullptr) {
        potential_display_class_ancestors.insert(potential_display_class_parent);
        potential_display_class_parent = potential_display_class_parent->declaring_type_definition();
    }

    const ITypeDefinition* decompiled_type_definition_or_ancestor = decompiled_type_definition;

    while (decompiled_type_definition_or_ancestor != nullptr) {
        if (potential_display_class_ancestors.contains(decompiled_type_definition_or_ancestor)) return true;
        decompiled_type_definition_or_ancestor = decompiled_type_definition_or_ancestor->declaring_type_definition();
    }
    return false;
}

void TransformDisplayClassUsage::visit_il_function(ILFunction& function) {
    context_->step_start_group("Visit " + function.name);
    try {
        current_functions_.push_back(&function);
        ILVisitor::visit_il_function(function);
    } catch (...) {
        current_functions_.pop_back();
        context_->step_end_group(true);
        throw;
    }
    current_functions_.pop_back();
    context_->step_end_group(true);
}

void TransformDisplayClassUsage::default_visit(ILInstruction& inst) {
    ILInstruction* next;
    ILInstruction* child = inst.get_child_count() > 0 ? inst.get_child(0) : nullptr;
    for (; child != nullptr; child = next) {
        next = get_next_sibling(child);
        child->accept_visitor(*this);
    }
}

void TransformDisplayClassUsage::visit_st_loc(StLoc& inst) {
    if (auto* parent_block = as<Block>(inst.parent()); parent_block != nullptr && inst.variable()->is_single_definition()) {
        if ((inst.variable()->kind() == VariableKind::Local || inst.variable()->kind() == VariableKind::StackSlot) && inst.variable()->load_count() == 0) {
            // traverse pre-order, so that we do not have to deal with more special cases afterwards
            ILVisitor::visit_st_loc(inst);
            if (is<StLoc>(inst.value()) || is<CompoundAssignmentInstruction>(inst.value())) {
                context_->step("Remove unused variable assignment " + inst.variable()->to_string(), &inst);
                ILInstruction* replacement = inst.value();
                inst.replace_with(replacement);
                context_->end_step(replacement);
            }
            return;
        }
        DisplayClass* display_class = find_display_class(inst.variable());
        if (display_class != nullptr && display_class->initializer == &inst) {
            // inline contents of object initializer block
            if (auto* init_block = as<Block>(inst.value()); init_block != nullptr && init_block->kind == BlockKind::ObjectInitializer) {
                context_->step("Remove initializer of " + inst.variable()->to_string(), &inst);
                ILInstruction* first_inlined_store = nullptr;
                // Stores are appended after the initializer, in source order; a store that
                // is dropped must not leave a gap, so the position is tracked separately
                // from the loop index.
                int insertion_index = inst.child_index();
                for (int i = 1; i < init_block->instructions.count(); i++) {
                    auto* stobj = static_cast<StObj*>(init_block->instructions[i]);
                    VariableToDeclare** found =
                        display_class->variables_to_declare.find(field_definition(static_cast<LdFlda*>(stobj->target())->field()));
                    if (found == nullptr) throw UsageError("The given key was not present in the dictionary.");
                    VariableToDeclare* variable = *found;
                    // A propagated field is replaced by the variable it was initialized from,
                    // so keeping its initializer would assign that variable to itself - and
                    // where the variable is 'this', the result is not even valid C#. Stores
                    // outside an object-initializer block are dropped in VisitStObj under the
                    // same condition.
                    if (variable->can_propagate() && variable->has_initializer(stobj)) continue;
                    ILVariable* declared = variable->get_or_declare();
                    ILInstruction* stored_value = stobj->value();
                    auto* inlined_store = with_il_range(make<StLoc>(declared, stored_value), stobj);
                    parent_block->instructions.insert(++insertion_index, inlined_store);
                    if (first_inlined_store == nullptr) first_inlined_store = inlined_store;
                }
                context_->end_step(first_inlined_store);
            }
            context_->step("Remove initializer of " + inst.variable()->to_string(), &inst);
            parent_block->instructions.remove(&inst);
            return;
        }
        if (is<LdLoc>(inst.value()) || is<LdObj>(inst.value())) {
            // in some cases (e.g. if inlining fails), there can be a reference to a display class in a stack slot,
            // in that case it is necessary to resolve the reference and iff it can be propagated, replace all loads
            // of the single-definition.
            ILVariable* referenced_display_class = nullptr;
            if (ILVariable** mapped = display_class_copy_map_.find(inst.variable())) {
                referenced_display_class = *mapped;
            } else {
                referenced_display_class = resolve_variable_to_propagate(inst.value());
            }
            if (referenced_display_class != nullptr && display_classes_.contains(referenced_display_class)) {
                context_->step("Propagate reference to " + referenced_display_class->to_string() + " in " + inst.variable()->to_string(), &inst);
                ILInstruction* first_replacement = nullptr;
                std::vector<LdLoc*> loads = inst.variable()->load_instructions();
                for (LdLoc* ld : loads) {
                    auto* new_load = with_il_range(make<LdLoc>(referenced_display_class), ld);
                    ld->replace_with(new_load);
                    if (first_replacement == nullptr) first_replacement = new_load;
                }
                context_->end_step(first_replacement);
                parent_block->instructions.remove(&inst);
                return;
            }
        }
    }
    ILVisitor::visit_st_loc(inst);
}

void TransformDisplayClassUsage::visit_st_obj(StObj& inst) {
    ILVariable* v = nullptr;
    DisplayClass* display_class = nullptr;
    const IField* field = nullptr;
    if (is_display_class_field_access(inst.target(), &v, &display_class, &field)) {
        VariableToDeclare** found = display_class->variables_to_declare.find(field_definition(field));
        if (found == nullptr) throw UsageError("The given key was not present in the dictionary.");
        VariableToDeclare* vd = *found;
        if (vd->can_propagate() && vd->has_initializer(&inst)) {
            if (auto* containing_block = as<Block>(inst.parent())) {
                context_->step("Remove initializer of " + v->to_string() + "." + vd->name() + " due to propagation", &inst);
                containing_block->instructions.remove(&inst);
                return;
            }
        }
        auto* ld_loc = as<LdLoc>(inst.value());
        if (ld_loc != nullptr && ld_loc->variable()->is_single_definition() && ld_loc->variable()->capture_scope == nullptr &&
            !ld_loc->variable()->store_instructions().empty()) {
            auto* stloc = as<StLoc>(ld_loc->variable()->store_instructions()[0]->as_instruction());
            if (stloc != nullptr) {
                if (auto* block = as<Block>(stloc->parent())) {
                    ILInlining::inline_one_if_possible(block, stloc->child_index(), InliningOptions::None, *context_);
                }
            }
        }
    }
    ILVisitor::visit_st_obj(inst);
    EarlyExpressionTransforms::st_obj_to_st_loc(&inst, *context_);
}

void TransformDisplayClassUsage::visit_ld_obj(LdObj& inst) {
    ILVisitor::visit_ld_obj(inst);
    EarlyExpressionTransforms::ld_obj_to_ld_loc(&inst, *context_);
}

bool TransformDisplayClassUsage::is_display_class_load(ILInstruction* target, ILVariable** variable) {
    // We cannot use MatchLdLocRef here because local functions use ref parameters
    if (!target->match_ld_loc(variable) && !target->match_ld_loca(variable)) return false;
    if (ILVariable** other = display_class_copy_map_.find(*variable)) *variable = *other;
    return true;
}

bool TransformDisplayClassUsage::is_display_class_field_access(ILInstruction* inst, ILVariable** display_class_var, DisplayClass** display_class,
                                                               const IField** field) {
    *display_class = nullptr;
    *display_class_var = nullptr;
    *field = nullptr;
    auto* ldflda = as<LdFlda>(inst);
    if (ldflda == nullptr) return false;
    *field = ldflda->field();
    if (!is_display_class_load(ldflda->target(), display_class_var)) return false;
    *display_class = find_display_class(*display_class_var);
    return *display_class != nullptr;
}

void TransformDisplayClassUsage::visit_ld_flda(LdFlda& inst) {
    ILVisitor::visit_ld_flda(inst);
    // Get display class info
    DisplayClass* display_class = nullptr;
    ILVariable* ignored_variable = nullptr;
    const IField* field = nullptr;
    if (!is_display_class_field_access(&inst, &ignored_variable, &display_class, &field)) return;
    const IField* key_field = field_definition(field);
    VariableToDeclare** found = display_class->variables_to_declare.find(key_field);
    if (found == nullptr) throw UsageError("The given key was not present in the dictionary.");
    VariableToDeclare* v = *found;
    context_->step("Replace " + field->name() + " with captured variable " + v->name(), &inst);
    ILVariable* variable = v->get_or_declare();
    auto* replacement = with_il_range(make<LdLoca>(variable), &inst);
    inst.replace_with(replacement);
    context_->end_step(replacement);
    // add captured variable to all descendant functions from the declaring function to this use-site function
    // (a stack enumerates from its top)
    for (auto it = current_functions_.rbegin(); it != current_functions_.rend(); ++it) {
        if (*it == variable->function()) break;
        (*it)->captured_variables.insert(variable);
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
