// dotnet/decompiler/csharp/record_decompiler.cpp -- see record_decompiler.hpp.
#include "dotnet/decompiler/csharp/record_decompiler.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <typeinfo>

#include "core/error.hpp"
#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/transform_expression_trees.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/char_info.hpp"

namespace cb::dotnet::decompiler::csharp {

using il::AddressOf;
using il::BinaryNumericInstruction;
using il::BinaryNumericOperator;
using il::Block;
using il::Box;
using il::Call;
using il::CallInstruction;
using il::CallVirt;
using il::ILFunction;
using il::ILInstruction;
using il::ILVariable;
using il::LdcI4;
using il::Leave;
using il::NewObj;
using il::StObj;
using il::VariableKind;
using typesystem::Accessibility;
using typesystem::GetMemberOptions;
using typesystem::IAttribute;
using typesystem::IField;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IParameter;
using typesystem::IProperty;
using typesystem::IType;
using typesystem::ITypeDefinition;
using typesystem::KnownAttribute;
using typesystem::KnownTypeCode;
using typesystem::NormalizeTypeVisitor;
using typesystem::ReferenceKind;
using typesystem::SymbolKind;
using typesystem::TypeKind;
using typesystem::member_equals;
using typesystem::type_equals;

namespace {

// `body.Ancestors.OfType<ILFunction>().Single()`.
ILFunction* single_function_ancestor(ILInstruction* body) {
    ILFunction* result = nullptr;
    for (ILInstruction* node = body; node != nullptr; node = node->parent()) {
        if (auto* function = il::as<ILFunction>(node)) {
            if (result != nullptr) throw UsageError("Sequence contains more than one matching element");
            result = function;
        }
    }
    if (result == nullptr) throw UsageError("Sequence contains no matching element");
    return result;
}

// `variables.Single(v => v.Kind == VariableKind.Parameter && v.Index == 0)`.
ILVariable* single_parameter_zero(ILFunction* function) {
    ILVariable* result = nullptr;
    for (ILVariable* v : function->variables()) {
        if (v->kind() == VariableKind::Parameter && v->index() == 0) {
            if (result != nullptr) throw UsageError("Sequence contains more than one matching element");
            result = v;
        }
    }
    if (result == nullptr) throw UsageError("Sequence contains no matching element");
    return result;
}

bool any_not_allowed(const std::vector<const IAttribute*>& attributes, bool (*is_allowed)(const IAttribute*)) {
    return std::any_of(attributes.begin(), attributes.end(), [&](const IAttribute* a) { return !is_allowed(a); });
}

void unpack_logic_and_chain_into(ILInstruction* inst, std::vector<ILInstruction*>& result) {
    ILInstruction* lhs = nullptr;
    ILInstruction* rhs = nullptr;
    if (inst->match_logic_and(&lhs, &rhs)) {
        unpack_logic_and_chain_into(lhs, result);
        unpack_logic_and_chain_into(rhs, result);
    } else {
        result.push_back(inst);
    }
}

}  // namespace

RecordDecompiler::RecordDecompiler(const typesystem::DecompilerTypeSystem& dts, const ITypeDefinition* record_type_def, const DecompilerSettings& settings,
                                   const util::CancellationToken& cancellation_token)
    : type_system_(dts), record_type_def_(record_type_def), settings_(settings), cancellation_token_(cancellation_token) {
    for (const IType* b : record_type_def->direct_base_types()) {
        if (b->kind() == TypeKind::Class) {
            base_class_ = b;
            break;
        }
    }
    is_struct_ = base_class_ != nullptr && typesystem::is_known_type(base_class_, KnownTypeCode::ValueType);
    is_inherited_record_ = !is_struct_ && !(base_class_ != nullptr && typesystem::is_known_type(base_class_, KnownTypeCode::Object));
    is_sealed_ = record_type_def->is_sealed();
    detect_automatic_properties();
    ordered_members_ = detect_member_order(record_type_def, backing_field_to_auto_property_);
    primary_ctor_ = detect_primary_constructor();
}

void RecordDecompiler::detect_automatic_properties() {
    auto subst = typesystem::as_parameterized_type(record_type_def_)->get_substitution();
    for (const IProperty* property : record_type_def_->properties()) {
        cancellation_token_.throw_if_cancellation_requested();
        const auto* p = dynamic_cast<const IProperty*>(property->specialize(subst));
        const IField* field = nullptr;
        if (is_auto_property(p, &field)) {
            backing_field_to_auto_property_.add(field, p);
            auto_property_to_backing_field_.add(p, field);
        }
    }
}

bool RecordDecompiler::is_auto_property(const IProperty* p, const IField** field) const {
    *field = nullptr;
    if (p->is_static()) return false;
    if (!p->parameters().empty()) return false;
    if (p->getter() != nullptr) {
        if (!is_auto_getter(p->getter(), field)) return false;
    }
    if (p->setter() != nullptr) {
        const IField* field2 = nullptr;
        if (!is_auto_setter(p->setter(), &field2)) return false;
        if (*field != nullptr) {
            if (!member_equals(*field, field2)) return false;
        } else {
            *field = field2;
        }
    }
    if (*field == nullptr) return false;
    if (!is_record_type((*field)->declaring_type())) return false;
    return (*field)->name() == "<" + p->name() + ">k__BackingField";
}

bool RecordDecompiler::is_auto_getter(const IMethod* method, const IField** field) const {
    *field = nullptr;
    Block* body = decompile_body(method);
    if (body == nullptr) return false;
    // return this.field;
    ILInstruction* ret_val = nullptr;
    if (!body->instructions[0]->match_return(&ret_val)) return false;
    if (method->is_static()) {
        return ret_val->match_lds_fld(field);
    }
    ILInstruction* target = nullptr;
    if (!ret_val->match_ld_fld(&target, field)) return false;
    return target->match_ld_this();
}

bool RecordDecompiler::is_auto_setter(const IMethod* method, const IField** field) const {
    *field = nullptr;
    assert(!method->is_static());
    Block* body = decompile_body(method);
    if (body == nullptr) return false;
    // this.field = value;
    ILInstruction* value_inst = nullptr;
    if (method->is_static()) {
        if (!body->instructions[0]->match_sts_fld(field, &value_inst)) return false;
    } else {
        ILInstruction* target = nullptr;
        if (!body->instructions[0]->match_st_fld(&target, field, &value_inst)) return false;
        if (!target->match_ld_this()) return false;
    }
    ILVariable* value = nullptr;
    if (!value_inst->match_ld_loc(&value)) return false;
    if (!(value->kind() == VariableKind::Parameter && value->index() == 0)) return false;
    ILInstruction* ret_val = nullptr;
    return body->instructions[1]->match_return(&ret_val) && ret_val->match_nop();
}

const IMethod* RecordDecompiler::detect_primary_constructor() {
    assert(record_type_def_->is_record());
    if (!settings_.use_primary_constructor_syntax) return nullptr;

    auto subst = typesystem::as_parameterized_type(record_type_def_)->get_substitution();

    std::unordered_map<std::string, const IMember*> name_to_member_map;
    // The constructors in the order they were found, with the constructor each one chains to (nullptr: none).
    std::vector<std::pair<const IMethod*, const IMethod*>> ctor_chain_map;

    for (const IMember* m : record_type_def_->get_members(
             [](const IMember* member) { return member->symbol_kind() == SymbolKind::Property || member->symbol_kind() == SymbolKind::Field; },
             GetMemberOptions::ReturnMemberDefinitions)) {
        name_to_member_map[m->name()] = m;
    }

    const IMethod* guessed_primary_ctor = nullptr;
    Accessibility expected_ctor_accessibility = record_type_def_->is_abstract() ? Accessibility::Protected : Accessibility::Public;

    for (const IMethod* method : record_type_def_->methods()) {
        cancellation_token_.throw_if_cancellation_requested();
        if (method->is_static() || !method->is_constructor()) continue;
        if (method->accessibility() != expected_ctor_accessibility) continue;
        if (is_copy_constructor(method)) continue;
        Block* body = decompile_body(method);
        if (body == nullptr) continue;
        const IMethod* chained_ctor = nullptr;
        if (const IMethod* found = find_chained_ctor(body)) chained_ctor = dynamic_cast<const IMethod*>(found->member_definition());
        ctor_chain_map.emplace_back(method, chained_ctor);

        if (chained_ctor != nullptr && chained_ctor->declaring_type_definition() == record_type_def_) continue;

        const IMethod* m = method->specialize_method(subst);
        if (is_primary_constructor(body, m, method, name_to_member_map)) {
            if (guessed_primary_ctor == nullptr) {
                guessed_primary_ctor = method;
            } else {
                // Multiple primary constructor candidates found - give up
                guessed_primary_ctor = nullptr;
                break;
            }
        }
    }

    if (guessed_primary_ctor != nullptr) {
        for (const auto& [source, target] : ctor_chain_map) {
            if (guessed_primary_ctor == source) continue;
            // In a record with a primary ctor every other ctor must call the primary ctor at the end of the ctor chain. If the target is
            // null or a ctor of another type, we found a ctor that does not follow this rule. The full chain need not be checked,
            // because the C# compiler enforces that there are no loops in the ctor call graph.
            if (target == nullptr || target->declaring_type_definition() != record_type_def_) {
                guessed_primary_ctor = nullptr;
                break;
            }
        }
    }

    if (guessed_primary_ctor == nullptr) primary_ctor_parameter_to_auto_property_.clear();

    std::vector<std::pair<const IParameter*, const IProperty*>> remaining;
    for (const auto& [parameter, property] : primary_ctor_parameter_to_auto_property_) {
        if (!member_equals(parameter->owner(), guessed_primary_ctor)) continue;
        remaining.emplace_back(parameter, property);
        auto_property_to_primary_ctor_parameter_.add(property, parameter);
    }
    primary_ctor_parameter_to_auto_property_ = std::move(remaining);

    return guessed_primary_ctor;
}

bool RecordDecompiler::validate_parameter(ILVariable* v, int expected_minimum_index, const IMethod* unspecialized_method) const {
    if (v->kind() != VariableKind::Parameter) return false;
    assert(v->index().has_value());
    if (*v->index() < 0 || *v->index() >= static_cast<int>(unspecialized_method->parameters().size())) return false;
    const IParameter* parameter = unspecialized_method->parameters()[static_cast<std::size_t>(*v->index())];
    for (const auto& entry : primary_ctor_parameter_to_auto_property_)
        if (entry.first == parameter) return true;
    return *v->index() >= expected_minimum_index;
}

bool RecordDecompiler::is_primary_constructor(Block* body, const IMethod* method, const IMethod* unspecialized_method,
                                              const std::unordered_map<std::string, const IMember*>& name_to_member_map) {
    for (const IParameter* p : unspecialized_method->parameters()) {
        // ref and out are not valid modifiers in this context
        if (p->reference_kind() == ReferenceKind::Ref || p->reference_kind() == ReferenceKind::Out) return false;

        // for each positional parameter there must be a field or property of the same name and type
        auto member = name_to_member_map.find(p->name());
        if (member == name_to_member_map.end()) return false;

        const IType* param_type = p->reference_kind() != ReferenceKind::None ? typesystem::unwrap_by_ref(p->type()) : p->type();
        if (!NormalizeTypeVisitor::type_erasure().equivalent_types(param_type, member->second->return_type())) return false;
    }

    if (!is_struct_) {
        const int count = body->instructions.count();
        auto* base_ctor_call = count >= 2 ? il::as<CallInstruction>(body->instructions[count - 2]) : nullptr;
        if (base_ctor_call == nullptr) return false;
        const IMethod* base_ctor = base_ctor_call->method;
        if (!base_ctor->is_constructor()) return false;
        if (type_equals(base_ctor->declaring_type(), method->declaring_type())) return false;
    }

    const int addon_inst = is_struct_ ? 1 : 2;

    if (body->instructions.count() < addon_inst) return false;

    int parameter_index = 0;

    for (int i = 0; i < body->instructions.count() - addon_inst; i++) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value_inst = nullptr;
        if (!body->instructions[i]->match_st_fld(&target, &field, &value_inst)) return false;
        if (!target->match_ld_this()) return false;
        // allow assignments to fields that are not backing fields of auto-properties
        const IProperty* const* found_property = backing_field_to_auto_property_.find(field);
        if (found_property == nullptr) continue;
        const IProperty* property = *found_property;
        ILVariable* v = nullptr;
        ILInstruction* loaded = nullptr;
        const IType* loaded_type = nullptr;
        if (value_inst->match_ld_loc(&v)) {
            if (!validate_parameter(v, parameter_index, unspecialized_method)) return false;
            parameter_index = *v->index();
        } else if (value_inst->match_ld_obj(&loaded, &loaded_type) && loaded->match_ld_loc(&v)) {
            if (!validate_parameter(v, parameter_index, unspecialized_method)) return false;
            parameter_index = *v->index();
            if (method->parameters()[static_cast<std::size_t>(parameter_index)]->reference_kind() == ReferenceKind::None) return false;
        } else {
            continue;
        }
        const IParameter* parameter = unspecialized_method->parameters()[static_cast<std::size_t>(parameter_index)];
        bool already_added = false;
        for (const auto& entry : primary_ctor_parameter_to_auto_property_)
            if (entry.first == parameter) already_added = true;
        if (already_added) continue;

        if (record_type_def_->kind() != TypeKind::Struct) {
            if (!(property->can_set() && property->setter()->is_init_only())) continue;
        }
        primary_ctor_parameter_to_auto_property_.emplace_back(parameter, property);
    }

    ILInstruction* return_inst = body->instructions.empty() ? nullptr : body->instructions[body->instructions.count() - 1];
    ILInstruction* ret_val = nullptr;
    return return_inst != nullptr && return_inst->match_return(&ret_val) && ret_val->match_nop();
}

const IMethod* RecordDecompiler::find_chained_ctor(Block* body) {
    // look for a call instruction or assignment to a chained constructor
    for (ILInstruction* inst : body->instructions) {
        if (auto* call = il::as<Call>(inst)) {
            if (call->method->is_constructor()) return call->method;
        } else if (auto* st_obj = il::as<StObj>(inst)) {
            if (auto* new_obj = il::as<NewObj>(st_obj->value())) {
                if (st_obj->target()->match_ld_this()) return new_obj->method;
            }
        }
    }
    return nullptr;
}

std::vector<const IMember*> RecordDecompiler::detect_member_order(const ITypeDefinition* record_type_def,
                                                                   const MemberMap<IField, const IProperty*>& backing_field_to_auto_property) {
    // For records, the order of members is important: Equals/GetHashCode/PrintMembers must agree on an order of fields+properties. The IL
    // metadata has the order of fields and the order of properties, but we need to detect the correct interleaving. We could try to
    // detect this from the PrintMembers body, but let's initially restrict ourselves to the common case where the record only uses
    // properties.
    auto subst = typesystem::as_parameterized_type(record_type_def)->get_substitution();
    std::vector<const IMember*> result;
    for (const IProperty* p : record_type_def->properties()) result.push_back(p->specialize(subst));
    for (const IField* f : record_type_def->fields()) {
        const auto* specialized = dynamic_cast<const IField*>(f->specialize(subst));
        if (!backing_field_to_auto_property.contains(specialized)) result.push_back(specialized);
    }
    return result;
}

bool RecordDecompiler::is_record_type(const IType* type) const {
    if (type->get_definition() != record_type_def_) return false;
    const auto& args = type->type_arguments();
    const auto& params = record_type_def_->type_parameters();
    if (args.size() != params.size()) return false;
    for (std::size_t i = 0; i < args.size(); ++i)
        if (!type_equals(args[i], params[i])) return false;
    return true;
}

bool RecordDecompiler::method_is_generated(const IMethod* method) const {
    if (is_copy_constructor(method)) return is_generated_copy_constructor(method);

    const std::string name = method->name();
    // Some members in records are always compiler-generated and lead to a "duplicate definition" error if we emit the generated code.
    if (name == "op_Equality" || name == "op_Inequality") {
        // Don't emit comparison operators into C# record definition. Note: the user can declare additional operator== as long as they
        // have different parameter types.
        return method->parameters().size() == 2 &&
               std::all_of(method->parameters().begin(), method->parameters().end(), [&](const IParameter* p) { return is_record_type(p->type()); });
    }
    if (name == "Equals" && method->parameters().size() == 1) {
        const IType* param_type = method->parameters()[0]->type();
        if (typesystem::is_known_type(param_type, KnownTypeCode::Object) && method->is_override()) {
            // override bool Equals(object? obj): always generated
            return true;
        } else if (is_record_type(param_type)) {
            // virtual bool Equals(R? other): generated unless user-declared
            return is_generated_equals(method);
        } else if (is_inherited_record_ && base_class_ != nullptr &&
                   NormalizeTypeVisitor::type_erasure().equivalent_types(param_type, base_class_) && method->is_override()) {
            // override bool Equals(BaseClass? obj): always generated
            return true;
        }
        return false;
    }
    if (name == "GetHashCode") return is_generated_get_hash_code(method);
    if (name == "<Clone>$" && method->parameters().empty()) {
        // Always generated; the method name cannot be expressed in C#
        return true;
    }
    if (name == "PrintMembers") return is_generated_print_members(method);
    if (name == "ToString" && method->parameters().empty()) return is_generated_to_string(method);
    if (name == "Deconstruct" && primary_ctor_ != nullptr && method->parameters().size() == primary_ctor_->parameters().size())
        return is_generated_deconstruct(method);
    return false;
}

bool RecordDecompiler::property_is_generated(const IProperty* property) const {
    if (!record_type_def_->is_record()) return false;
    if (property->name() == "EqualityContract" && !is_struct_) return is_generated_equality_contract(property);
    return is_property_declared_by_primary_constructor(property);
}

bool RecordDecompiler::is_property_declared_by_primary_constructor(const IProperty* property) const {
    auto subst = typesystem::as_parameterized_type(record_type_def_)->get_substitution();
    return primary_ctor_ != nullptr &&
           auto_property_to_primary_ctor_parameter_.contains(dynamic_cast<const IProperty*>(property->specialize(subst)));
}

std::vector<RecordDecompiler::BackingStore> RecordDecompiler::get_parameter_to_backing_store_map() const {
    std::vector<BackingStore> result;
    for (const auto& [parameter, property] : primary_ctor_parameter_to_auto_property_) {
        const IField* const* field = auto_property_to_backing_field_.find(property);
        if (field == nullptr) throw std::out_of_range("The given key was not present in the dictionary.");
        result.push_back({parameter, property, *field});
    }
    return result;
}

bool RecordDecompiler::is_copy_constructor(const IMethod* method) const {
    if (method == nullptr) return false;

    assert(method->declaring_type_definition() == record_type_def_);

    // Sealing a record after it was compiled leaves the copy constructor protected where a sealed record would have got a private one.
    // Accept either, matching what is_generated_copy_constructor already allows.
    return method->is_constructor() && method->parameters().size() == 1 &&
           (method->accessibility() == Accessibility::Protected || (is_sealed_ && method->accessibility() == Accessibility::Private)) &&
           is_record_type(method->parameters()[0]->type());
}

bool RecordDecompiler::is_allowed_attribute(const IAttribute* attribute) {
    return attribute->attribute_type()->reflection_name() == "System.Runtime.CompilerServices.CompilerGeneratedAttribute";
}

bool RecordDecompiler::is_generated_copy_constructor(const IMethod* method) const {
    /*
        call BaseClass..ctor(ldloc this, ldloc original)
        stfld <X>k__BackingField(ldloc this, ldfld <X>k__BackingField(ldloc original))
        leave IL_0000 (nop)
     */
    assert(method->is_constructor() && method->parameters().size() == 1);
    if (any_not_allowed(method->get_attributes(), &is_allowed_attribute) || !method->get_return_type_attributes().empty()) return false;
    if (method->accessibility() != Accessibility::Protected && (!is_sealed_ || method->accessibility() != Accessibility::Private)) return false;
    Block* body = decompile_body(method);
    if (body == nullptr) return false;
    ILFunction* function = single_function_ancestor(body);
    ILVariable* other = single_parameter_zero(function);
    assert(is_record_type(other->type()));
    int pos = 0;
    // First instruction is the base constructor call
    auto* base_ctor_call = il::as<Call>(body->instructions[pos]);
    if (base_ctor_call == nullptr || !base_ctor_call->method->is_constructor()) return false;
    if (!type_equals(base_ctor_call->method->declaring_type(), base_class_)) return false;
    if (base_ctor_call->arguments().count() != (is_inherited_record_ ? 2 : 1)) return false;
    if (!base_ctor_call->arguments()[0]->match_ld_this()) return false;
    if (is_inherited_record_) {
        if (!base_ctor_call->arguments()[1]->match_ld_loc(other)) return false;
    }
    pos++;
    // Then all the fields are copied over
    for (const IMember* member : ordered_members_) {
        if (member->is_static()) continue;
        const IField* field = dynamic_cast<const IField*>(member);
        if (field == nullptr) {
            const IField* const* backing = auto_property_to_backing_field_.find(dynamic_cast<const IProperty*>(member));
            if (backing == nullptr) continue;
            field = *backing;
        }
        if (pos >= body->instructions.count()) return false;
        ILInstruction* lhs_target = nullptr;
        const IField* lhs_field = nullptr;
        ILInstruction* value_inst = nullptr;
        if (!body->instructions[pos]->match_st_fld(&lhs_target, &lhs_field, &value_inst)) return false;
        if (!lhs_target->match_ld_this()) return false;
        if (!member_equals(lhs_field, field)) return false;

        ILInstruction* rhs_target = nullptr;
        const IField* rhs_field = nullptr;
        if (!value_inst->match_ld_fld(&rhs_target, &rhs_field)) return false;
        if (!rhs_target->match_ld_loc(other)) return false;
        if (!member_equals(rhs_field, field)) return false;
        pos++;
    }
    return il::is<Leave>(body->instructions[pos]);
}

bool RecordDecompiler::is_generated_equality_contract(const IProperty* property) const {
    // Generated member:
    // protected virtual Type EqualityContract {
    //    [CompilerGenerated] get => typeof(R);
    // }
    assert(!is_struct_ && property->name() == "EqualityContract");
    if (property->accessibility() != Accessibility::Protected && (!is_sealed_ || property->accessibility() != Accessibility::Private)) return false;
    if (!(is_sealed_ || property->is_virtual() || property->is_override())) return false;
    if (property->is_sealed()) return false;
    const IMethod* getter = property->getter();
    if (!(getter != nullptr && !property->can_set())) return false;
    std::vector<const IAttribute*> attrs = property->get_attributes();
    switch (attrs.size()) {
    case 0:
        // Roslyn 3.x does not emit a CompilerGeneratedAttribute on the property itself.
        break;
    case 1:
        // Roslyn 4.4 started doing so.
        if (!typesystem::is_known_type(attrs[0]->attribute_type(), KnownAttribute::CompilerGenerated)) return false;
        break;
    default:
        return false;
    }
    if (!getter->get_return_type_attributes().empty()) return false;
    attrs = getter->get_attributes();
    if (attrs.size() != 1) return false;
    if (!typesystem::is_known_type(attrs[0]->attribute_type(), KnownAttribute::CompilerGenerated)) return false;
    Block* body = decompile_body(getter);
    if (body == nullptr || body->instructions.count() != 1) return false;
    auto* leave = il::as<Leave>(body->instructions[0]);
    if (leave == nullptr) return false;
    // leave IL_0000 (call GetTypeFromHandle(ldtypetoken R))
    const IType* ty = nullptr;
    if (!il::transforms::TransformExpressionTrees::match_get_type_from_handle(leave->value(), &ty)) return false;
    return is_record_type(ty);
}

bool RecordDecompiler::match_string_builder_append(ILInstruction* inst, const ILVariable* sb, ILInstruction** val) const {
    *val = nullptr;
    auto* call = il::as<CallVirt>(inst);
    if (call == nullptr) return false;
    const IType* declaring_type = call->method->declaring_type();
    if (!(call->method->name() == "Append" && declaring_type->namespace_() == "System.Text" && declaring_type->name() == "StringBuilder")) return false;
    if (call->arguments().count() != 2) return false;
    if (!call->arguments()[0]->match_ld_loc(sb)) return false;
    *val = call->arguments()[1];
    return true;
}

bool RecordDecompiler::is_generated_print_members(const IMethod* method) const {
    assert(method->name() == "PrintMembers");
    if (method->parameters().size() != 1) return false;
    if (!is_sealed_ && !method->is_overridable()) return false;
    if (any_not_allowed(method->get_attributes(), &is_allowed_attribute) || !method->get_return_type_attributes().empty()) return false;
    if (method->accessibility() != Accessibility::Protected && (!is_sealed_ || method->accessibility() != Accessibility::Private)) return false;
    Block* body = decompile_body(method);
    if (body == nullptr) return false;
    ILFunction* function = single_function_ancestor(body);
    ILVariable* builder = single_parameter_zero(function);
    if (builder->type()->reflection_name() != "System.Text.StringBuilder") return false;
    int pos = 0;
    // Roslyn 4.0.0-3.final starts to insert a call to RuntimeHelpers.EnsureSufficientExecutionStack()
    if (!is_struct_ && !is_inherited_record_) {
        auto* ensure_call = il::as<Call>(body->instructions[pos]);
        if (ensure_call != nullptr && ensure_call->arguments().count() == 0 && ensure_call->method->name() == "EnsureSufficientExecutionStack" &&
            ensure_call->method->declaring_type()->namespace_() == "System.Runtime.CompilerServices" &&
            ensure_call->method->declaring_type()->name() == "RuntimeHelpers") {
            pos++;
        }
    }

    auto is_printed_member = [&](const IMember* member) {
        if (member->is_static()) return false;   // static fields/properties are not printed
        if (member->accessibility() != Accessibility::Public) return false;   // non-public fields/properties are not printed
        if (!is_struct_ && member->name() == "EqualityContract") return false;   // EqualityContract is never printed
        if (member->is_explicit_interface_implementation()) return false;   // explicit interface impls are not printed
        if (member->is_override()) return false;   // override is not printed (again), the virtual base property was already printed
        return true;
    };

    if (is_inherited_record_) {
        auto is_base_call = [&](ILInstruction* inst) {
            auto* call = il::as<CallInstruction>(inst);
            if (call == nullptr || call->method->name() != "PrintMembers") return false;
            if (call->arguments().count() != 2) return false;
            if (!call->arguments()[0]->match_ld_this()) return false;
            if (!call->arguments()[1]->match_ld_loc(builder)) return false;
            return true;
        };
        // Special case: inherited record adding no new members
        ILInstruction* return_value = nullptr;
        if (body->instructions[pos]->match_return(&return_value) && is_base_call(return_value) &&
            !std::any_of(ordered_members_.begin(), ordered_members_.end(), is_printed_member)) {
            return true;
        }
        // if (call PrintMembers(ldloc this, ldloc builder)) Block IL_000f {
        //   callvirt Append(ldloc builder, ldstr ", ")
        // }
        ILInstruction* condition = nullptr;
        ILInstruction* true_inst = nullptr;
        if (!body->instructions[pos]->match_if_instruction(&condition, &true_inst)) return false;
        if (!is_base_call(condition)) return false;
        // trueInst = callvirt Append(ldloc builder, ldstr ", ")
        true_inst = Block::unwrap(true_inst);
        ILInstruction* val = nullptr;
        if (!match_string_builder_append(true_inst, builder, &val)) return false;
        std::string text;
        if (!(val->match_ld_str(&text) && text == ", ")) return false;
        pos++;
    }

    // Matches the appends of constant strings and joins their text (nullopt: not one).
    auto match_string_builder_append_constant = [&]() -> std::optional<std::string> {
        std::optional<std::string> text;
        ILInstruction* val = nullptr;
        std::string val_text;
        while (match_string_builder_append(body->instructions[pos], builder, &val) && val->match_ld_str(&val_text)) {
            text = text.value_or(std::string()) + val_text;
            pos++;
        }
        return text;
    };

    bool needs_comma = false;
    for (const IMember* member : ordered_members_) {
        if (!is_printed_member(member)) continue;
        cancellation_token_.throw_if_cancellation_requested();
        /*
        callvirt Append(ldloc builder, ldstr "A")
        callvirt Append(ldloc builder, ldstr " = ")
        callvirt Append(ldloc builder, constrained[System.Int32].callvirt ToString(addressof System.Int32(call get_A(ldloc this))))
        callvirt Append(ldloc builder, ldstr ", ")
        callvirt Append(ldloc builder, ldstr "B")
        callvirt Append(ldloc builder, ldstr " = ")
        callvirt Append(ldloc builder, constrained[System.Int32].callvirt ToString(ldflda B(ldloc this)))
        leave IL_0000 (ldc.i4 1) */
        std::optional<std::string> text = match_string_builder_append_constant();
        if (!text.has_value()) return false;
        std::string expected_text = (needs_comma ? ", " : "") + member->name() + " = ";
        if (*text != expected_text) return false;
        ILInstruction* val = nullptr;
        if (!match_string_builder_append(body->instructions[pos], builder, &val)) return false;
        auto* to_string_call = il::as<CallInstruction>(val);
        if (to_string_call != nullptr && to_string_call->method->name() == "ToString" && !to_string_call->method->is_static()) {
            if (to_string_call->arguments().count() != 1) return false;
            val = to_string_call->arguments()[0];
            if (auto* address_of = il::as<AddressOf>(val)) val = address_of->value();
        } else if (auto* box = il::as<Box>(val)) {
            if (!NormalizeTypeVisitor::type_erasure().equivalent_types(box->type(), member->return_type())) return false;
            val = box->argument();
        }
        auto* getter_call = il::as<CallInstruction>(val);
        const auto* property = dynamic_cast<const IProperty*>(member);
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        if (getter_call != nullptr && property != nullptr) {
            if (!member_equals(getter_call->method, property->getter())) return false;
            if (getter_call->arguments().count() != 1) return false;
            if (!getter_call->arguments()[0]->match_ld_this()) return false;
        } else if (val->match_ld_fld(&target, &field) || val->match_ld_flda(&target, &field)) {
            if (!target->match_ld_this()) return false;
            if (!member_equals(field, member)) return false;
        } else {
            return false;
        }
        pos++;
        needs_comma = true;
    }
    // leave IL_0000 (ldc.i4 1)
    ILInstruction* ret_val = nullptr;
    return body->instructions[pos]->match_return(&ret_val) && ret_val->match_ldc_i4(needs_comma ? 1 : 0);
}

bool RecordDecompiler::is_generated_to_string(const IMethod* method) const {
    assert(method->name() == "ToString" && method->parameters().empty());
    if (!method->is_override()) return false;
    if (method->is_sealed()) return false;
    if (any_not_allowed(method->get_attributes(), &is_allowed_attribute) || !method->get_return_type_attributes().empty()) return false;
    Block* body = decompile_body(method);
    if (body == nullptr) return false;
    // stloc stringBuilder(newobj StringBuilder..ctor())
    ILVariable* string_builder = nullptr;
    ILInstruction* string_builder_init = nullptr;
    if (!body->instructions[0]->match_st_loc(&string_builder, &string_builder_init)) return false;
    auto* new_obj = il::as<NewObj>(string_builder_init);
    if (new_obj == nullptr || new_obj->arguments().count() != 0) return false;
    const ITypeDefinition* ctor_type = new_obj->method->declaring_type_definition();
    if (ctor_type == nullptr || ctor_type->name() != "StringBuilder" || ctor_type->namespace_() != "System.Text") return false;

    auto match_append_call_with_value = [&](ILInstruction* inst, const std::string& val) {
        auto* call = il::as<CallVirt>(inst);
        if (call == nullptr || call->method->name() != "Append") return false;
        if (call->arguments().count() != 2) return false;
        if (!call->arguments()[0]->match_ld_loc(string_builder)) return false;
        // Roslyn 4.0.0-3.final starts to use char for 1 length string
        if (typesystem::is_known_type(call->method->parameters()[0]->type(), KnownTypeCode::Char)) {
            std::u16string units = util::utf16(val);
            return units.size() == 1 && call->arguments()[1]->match_ldc_i4(static_cast<std::int32_t>(units[0]));
        }
        std::string val1;
        return call->arguments()[1]->match_ld_str(&val1) && val1 == val;
    };

    // callvirt Append(ldloc stringBuilder, ldstr "R")
    if (!match_append_call_with_value(body->instructions[1], record_type_def_->name())) return false;
    // callvirt Append(ldloc stringBuilder, ldstr " { ")
    if (!match_append_call_with_value(body->instructions[2], " { ")) return false;
    // if (callvirt PrintMembers(ldloc this, ldloc stringBuilder)) { trueInst }
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!body->instructions[3]->match_if_instruction(&condition, &true_inst)) return true;
    auto* print_members_call = il::as<CallInstruction>(condition);
    if (!(print_members_call != nullptr && print_members_call->method->name() == "PrintMembers" &&
          (il::is<CallVirt>(condition) || (is_sealed_ && il::is<Call>(condition)))))
        return false;
    if (print_members_call->arguments().count() != 2) return false;
    if (!print_members_call->arguments()[0]->match_ld_this()) return false;
    if (!print_members_call->arguments()[1]->match_ld_loc(string_builder)) return false;
    // trueInst: callvirt Append(ldloc stringBuilder, ldstr " ")
    if (!match_append_call_with_value(Block::unwrap(true_inst), " ")) return false;
    // callvirt Append(ldloc stringBuilder, ldstr "}")
    if (!match_append_call_with_value(body->instructions[4], "}")) return false;
    // leave IL_0000 (callvirt ToString(ldloc stringBuilder))
    auto* leave = il::as<Leave>(body->instructions[5]);
    if (leave == nullptr) return false;
    auto* to_string_call = il::as<CallVirt>(leave->value());
    if (to_string_call == nullptr || to_string_call->method->name() != "ToString") return false;
    if (to_string_call->arguments().count() != 1) return false;
    return to_string_call->arguments()[0]->match_ld_loc(string_builder);
}

bool RecordDecompiler::is_generated_equals(const IMethod* method) const {
    // virtual bool Equals(R? other) {
    //    return other != null && EqualityContract == other.EqualityContract && EqualityComparer<int>.Default.Equals(A, other.A) && ...;
    // }
    // Starting with Roslyn 3.10, it's:
    // virtual bool Equals(R? other) {
    //    return this == other || other != null && EqualityContract == other.EqualityContract && EqualityComparer<int>.Default.Equals(A, other.A) && ...;
    // }
    assert(method->name() == "Equals" && method->parameters().size() == 1);
    if (method->parameters().size() != 1) return false;
    if (!is_sealed_ && !method->is_overridable()) return false;
    if (any_not_allowed(method->get_attributes(), &is_allowed_attribute) || !method->get_return_type_attributes().empty()) return false;
    Block* body = decompile_body(method);
    if (body == nullptr) return false;
    ILInstruction* return_value = nullptr;
    if (!body->instructions[0]->match_return(&return_value)) return false;
    // special case for empty record struct; always returns true;
    if (return_value->match_ldc_i4(1)) return true;
    ILFunction* function = single_function_ancestor(body);
    ILVariable* other = single_parameter_zero(function);
    assert(is_record_type(other->type()));
    ILInstruction* lhs = nullptr;
    ILInstruction* rhs = nullptr;
    if (return_value->match_logic_or(&lhs, &rhs)) {
        // this == other || ...
        ILInstruction* comp_left = nullptr;
        ILInstruction* comp_right = nullptr;
        if (!lhs->match_comp_equals(&comp_left, &comp_right)) return false;
        if (!comp_left->match_ld_this()) return false;
        if (!comp_right->match_ld_loc(other)) return false;
        return_value = rhs;
    }
    std::vector<ILInstruction*> conditions = unpack_logic_and_chain(return_value);
    assert(!conditions.empty());
    std::size_t pos = 0;
    if (!is_struct_) {
        if (is_inherited_record_) {
            // call BaseClass::Equals(ldloc this, ldloc other)
            if (pos >= conditions.size()) return false;
            auto* call = il::as<Call>(conditions[pos]);
            if (call == nullptr || call->method->name() != "Equals") return false;
            if (base_class_ != nullptr && !NormalizeTypeVisitor::type_erasure().equivalent_types(call->method->declaring_type(), base_class_)) return false;
            if (call->arguments().count() != 2) return false;
            if (!call->arguments()[0]->match_ld_this()) return false;
            if (!call->arguments()[1]->match_ld_loc(other)) return false;
            pos++;
        } else {
            // comp.o(ldloc other != ldnull)
            if (pos >= conditions.size()) return false;
            ILInstruction* arg = nullptr;
            if (!conditions[pos]->match_comp_not_equals_null(&arg)) return false;
            if (!arg->match_ld_loc(other)) return false;
            pos++;
            // call op_Equality(callvirt get_EqualityContract(ldloc this), callvirt get_EqualityContract(ldloc other))
            // Special-cased because Roslyn isn't using EqualityComparer<T> here.
            if (pos >= conditions.size()) return false;
            auto* op_equality_call = il::as<Call>(conditions[pos]);
            if (op_equality_call == nullptr || !op_equality_call->method->is_operator() || op_equality_call->method->name() != "op_Equality") return false;
            if (!typesystem::is_known_type(op_equality_call->method->declaring_type(), KnownTypeCode::Type)) return false;
            if (op_equality_call->arguments().count() != 2) return false;
            ILInstruction* target1 = nullptr;
            ILInstruction* target2 = nullptr;
            if (!match_get_equality_contract(op_equality_call->arguments()[0], &target1)) return false;
            if (!match_get_equality_contract(op_equality_call->arguments()[1], &target2)) return false;
            if (!target1->match_ld_this()) return false;
            if (!target2->match_ld_loc(other)) return false;
            pos++;
        }
    }
    for (const IMember* member : ordered_members_) {
        if (!member_considered_for_equality(member)) continue;
        if (!is_struct_ && member->name() == "EqualityContract") continue;   // already special-cased
        // EqualityComparer<int>.Default.Equals(A, other.A)
        // callvirt Equals(call get_Default(), ldfld <A>k__BackingField(ldloc this), ldfld <A>k__BackingField(ldloc other))
        if (pos >= conditions.size()) return false;
        auto* equals_call = il::as<CallVirt>(conditions[pos]);
        if (equals_call == nullptr || equals_call->method->name() != "Equals") return false;
        if (equals_call->arguments().count() != 3) return false;
        if (!is_equality_comparer_get_default_call(equals_call->arguments()[0], member->return_type())) return false;
        ILInstruction* target1 = nullptr;
        const IMember* member1 = nullptr;
        if (!match_member_access(equals_call->arguments()[1], &target1, &member1)) return false;
        ILInstruction* target2 = nullptr;
        const IMember* member2 = nullptr;
        if (!match_member_access(equals_call->arguments()[2], &target2, &member2)) return false;
        if (!target1->match_ld_this()) return false;
        if (!member_equals(member1, member)) return false;
        if (!(is_struct_ ? target2->match_ld_loca(other) : target2->match_ld_loc(other))) return false;
        if (!member_equals(member2, member)) return false;
        pos++;
    }
    return pos == conditions.size();
}

std::vector<ILInstruction*> RecordDecompiler::unpack_logic_and_chain(ILInstruction* root_of_chain) {
    std::vector<ILInstruction*> result;
    unpack_logic_and_chain_into(root_of_chain, result);
    return result;
}

bool RecordDecompiler::match_get_equality_contract(ILInstruction* inst, ILInstruction** target) const {
    *target = nullptr;
    auto* call = il::as<CallInstruction>(inst);
    if (call == nullptr || call->method->name() != "get_EqualityContract") return false;
    if (!(il::is<CallVirt>(inst) || (is_sealed_ && il::is<Call>(inst)))) return false;
    if (call->arguments().count() != 1) return false;
    *target = call->arguments()[0];
    return true;
}

bool RecordDecompiler::is_equality_comparer_get_default_call(ILInstruction* inst, const IType* type) {
    auto* call = il::as<Call>(inst);
    if (call == nullptr || call->method->name() != "get_Default" || !call->method->is_static()) return false;
    const IType* declaring_type = call->method->declaring_type();
    if (!(declaring_type != nullptr && declaring_type->name() == "EqualityComparer" && declaring_type->namespace_() == "System.Collections.Generic")) return false;
    if (declaring_type->type_arguments().size() != 1) return false;
    if (!NormalizeTypeVisitor::type_erasure().equivalent_types(declaring_type->type_arguments()[0], type)) return false;
    return call->arguments().count() == 0;
}

bool RecordDecompiler::member_considered_for_equality(const IMember* member) const {
    if (member->is_static()) return false;
    if (const auto* property = dynamic_cast<const IProperty*>(member)) {
        if (!is_struct_ && property->name() == "EqualityContract") return !is_inherited_record_;
        return auto_property_to_backing_field_.contains(property);
    }
    return dynamic_cast<const IField*>(member) != nullptr;
}

bool RecordDecompiler::is_generated_get_hash_code(const IMethod* method) const {
    /*
     return (
        (
            EqualityComparer<Type>.Default.GetHashCode(EqualityContract) * -1521134295 + EqualityComparer<int>.Default.GetHashCode(A)
        ) * -1521134295 + EqualityComparer<int>.Default.GetHashCode(B)
     ) * -1521134295 + EqualityComparer<object>.Default.GetHashCode(C);
    */
    assert(method->name() == "GetHashCode");
    if (!method->parameters().empty()) return false;
    if (!method->is_override() || method->is_sealed()) return false;
    if (any_not_allowed(method->get_attributes(), &is_allowed_attribute) || !method->get_return_type_attributes().empty()) return false;
    Block* body = decompile_body(method);
    if (body == nullptr) return false;
    ILInstruction* return_value = nullptr;
    if (!body->instructions[0]->match_return(&return_value)) return false;
    // special case for empty record struct; always returns false;
    if (return_value->match_ldc_i4(0)) return true;
    std::vector<const IMember*> hashed_members;
    bool found_base_class_hash = false;

    auto process_individual_hash_code = [&](ILInstruction* inst) {
        // base.GetHashCode(): call GetHashCode(ldloc this)
        if (auto* base_hash_code_call = il::as<Call>(inst); base_hash_code_call != nullptr && base_hash_code_call->method->name() == "GetHashCode") {
            if (base_hash_code_call->arguments().count() != 1) return false;
            if (!base_hash_code_call->arguments()[0]->match_ld_this()) return false;
            if (found_base_class_hash || !hashed_members.empty()) return false;   // must be first
            found_base_class_hash = true;
            return type_equals(base_hash_code_call->method->declaring_type(), base_class_);
        }
        // callvirt GetHashCode(call get_Default(), callvirt get_EqualityContract(ldloc this))
        // callvirt GetHashCode(call get_Default(), ldfld <A>k__BackingField(ldloc this)))
        auto* get_hash_code_call = il::as<CallVirt>(inst);
        if (get_hash_code_call == nullptr || get_hash_code_call->method->name() != "GetHashCode") return false;
        if (get_hash_code_call->arguments().count() != 2) return false;
        // getHashCodeCall.Arguments[0] is checked later
        ILInstruction* target = nullptr;
        const IMember* member = nullptr;
        if (!match_member_access(get_hash_code_call->arguments()[1], &target, &member)) return false;
        if (!target->match_ld_this()) return false;
        if (!is_equality_comparer_get_default_call(get_hash_code_call->arguments()[0], member->return_type())) return false;
        hashed_members.push_back(member);
        return true;
    };

    std::function<bool(ILInstruction*)> visit = [&](ILInstruction* inst) -> bool {
        auto* add = il::as<BinaryNumericInstruction>(inst);
        if (add != nullptr && add->op == BinaryNumericOperator::Add && !add->check_for_overflow) {
            auto* mul = il::as<BinaryNumericInstruction>(add->left());
            if (mul != nullptr && mul->op == BinaryNumericOperator::Mul && !mul->check_for_overflow) {
                auto* constant = il::as<LdcI4>(mul->right());
                if (constant != nullptr && constant->value == -1521134295) {
                    if (!visit(mul->left())) return false;
                    return process_individual_hash_code(add->right());
                }
            }
        }
        return process_individual_hash_code(inst);
    };

    if (!visit(return_value)) return false;
    if (found_base_class_hash != is_inherited_record_) return false;
    std::vector<const IMember*> considered;
    for (const IMember* m : ordered_members_)
        if (member_considered_for_equality(m)) considered.push_back(m);
    if (considered.size() != hashed_members.size()) return false;
    for (std::size_t i = 0; i < considered.size(); ++i)
        if (!member_equals(considered[i], hashed_members[i])) return false;
    return true;
}

bool RecordDecompiler::is_generated_deconstruct(const IMethod* method) const {
    assert(method->name() == "Deconstruct" && primary_ctor_ != nullptr && method->parameters().size() == primary_ctor_->parameters().size());

    if (!typesystem::is_known_type(method->return_type(), KnownTypeCode::Void)) return false;

    for (std::size_t i = 0; i < method->parameters().size(); i++) {
        const IParameter* deconstruct = method->parameters()[i];
        const IParameter* ctor = primary_ctor_->parameters()[i];

        if (deconstruct->reference_kind() != ReferenceKind::Out) return false;

        const IType* ctor_type = ctor->type();
        if (ctor->reference_kind() == ReferenceKind::In || ctor->reference_kind() == ReferenceKind::RefReadOnly) {
            const auto* by_ref = dynamic_cast<const typesystem::ByReferenceType*>(ctor_type);
            if (by_ref == nullptr) throw std::bad_cast();
            ctor_type = by_ref->element_type();
        }
        const auto* deconstruct_by_ref = dynamic_cast<const typesystem::ByReferenceType*>(deconstruct->type());
        if (deconstruct_by_ref == nullptr) throw std::bad_cast();
        if (!type_equals(ctor_type, deconstruct_by_ref->element_type())) return false;

        if (ctor->name() != deconstruct->name()) return false;
    }

    Block* body = decompile_body(method);
    if (body == nullptr || body->instructions.count() != static_cast<int>(method->parameters().size()) + 1) return false;

    for (int i = 0; i < body->instructions.count() - 1; i++) {
        // stobj T(ldloc parameter, call getter(ldloc this))
        ILInstruction* target_inst = nullptr;
        ILInstruction* getter = nullptr;
        const IType* stobj_type = nullptr;
        if (!body->instructions[i]->match_st_obj(&target_inst, &getter, &stobj_type)) return false;
        ILVariable* target = nullptr;
        if (!target_inst->match_ld_loc(&target)) return false;
        if (!(target->kind() == VariableKind::Parameter && target->index() == i)) return false;

        auto* call = il::as<Call>(getter);
        if (call == nullptr || call->arguments().count() != 1) return false;
        if (!call->arguments()[0]->match_ld_this()) return false;

        if (!call->method->is_accessor()) return false;
        const auto* auto_property = dynamic_cast<const IProperty*>(call->method->accessor_owner());
        if (auto_property == nullptr) throw std::bad_cast();
        if (!auto_property_to_backing_field_.contains(auto_property)) {
            if (auto_property->declaring_type_definition() == record_type_def_) return false;
        }
    }

    ILInstruction* return_inst = body->instructions.empty() ? nullptr : body->instructions[body->instructions.count() - 1];
    ILInstruction* ret_val = nullptr;
    return return_inst != nullptr && return_inst->match_return(&ret_val) && ret_val->match_nop();
}

bool RecordDecompiler::match_member_access(ILInstruction* inst, ILInstruction** target, const IMember** member) const {
    *target = nullptr;
    *member = nullptr;
    auto* call = il::as<CallInstruction>(inst);
    const IProperty* property = nullptr;
    if (call != nullptr && call->method->accessor_kind() == typesystem::MethodSemanticsAttributes::Getter) {
        property = dynamic_cast<const IProperty*>(call->method->accessor_owner());
    }
    if (property != nullptr && (il::is<CallVirt>(call) || (is_sealed_ && il::is<Call>(call)))) {
        if (call->arguments().count() != 1) return false;
        *target = call->arguments()[0];
        *member = property;
        return true;
    }
    const IField* field = nullptr;
    if (inst->match_ld_fld(target, &field)) {
        const IProperty* const* auto_property = backing_field_to_auto_property_.find(field);
        if (auto_property != nullptr)
            *member = *auto_property;
        else
            *member = field;
        return true;
    }
    return false;
}

Block* RecordDecompiler::decompile_body(const IMethod* method) const {
    if (method == nullptr) return nullptr;
    return CSharpDecompiler::decompile_body_for_analysis(method, type_system_, cancellation_token_);
}

}  // namespace cb::dotnet::decompiler::csharp
