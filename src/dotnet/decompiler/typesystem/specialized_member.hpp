// dotnet/decompiler/typesystem/specialized_member.hpp -- a member seen through a substitution of its type parameters: List<int>.Add is
// List<T>.Add with T = int, its parameter and return types substituted. Made only through the factories below, which keep one object per
// member and substitution (type_arena.hpp).
//
//     const IMethod* add = SpecializedMethod::create(list_add_definition, list_of_int->get_substitution());
//     add->parameters()[0]->type();     // System.Int32
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem.Implementation: SpecializedMember, SpecializedParameterizedMember, SpecializedMethod
// with its SpecializedTypeParameter, SpecializedField, SpecializedProperty, SpecializedEvent, SpecializedParameter; DefaultParameter's
// ToString), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream sets a specialized member's
// declaring type after constructing it (an optimization of GetMembersHelper); here it is a factory argument, part of the member's key.
#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem {

class SpecializedMember : public virtual IMember {
public:
    const TypeParameterSubstitution& substitution() const override { return substitution_; }
    const IType* declaring_type() const override;
    const IMember* member_definition() const override { return base_member_->member_definition(); }
    const IType* return_type() const override;
    srm::EntityHandle metadata_token() const override { return base_member_->metadata_token(); }
    bool is_virtual() const override { return base_member_->is_virtual(); }
    bool is_override() const override { return base_member_->is_override(); }
    bool is_overridable() const override { return base_member_->is_overridable(); }
    SymbolKind symbol_kind() const override { return base_member_->symbol_kind(); }
    const ITypeDefinition* declaring_type_definition() const override { return base_member_->declaring_type_definition(); }
    std::vector<const IAttribute*> get_attributes() const override { return base_member_->get_attributes(); }
    bool has_attribute(KnownAttribute attribute) const override { return base_member_->has_attribute(attribute); }
    const IAttribute* get_attribute(KnownAttribute attribute) const override { return base_member_->get_attribute(attribute); }
    // The definition's, specialized with this substitution (a generic interface's members are already specialized).
    std::vector<const IMember*> explicitly_implemented_interface_members() const override;
    bool is_explicit_interface_implementation() const override { return base_member_->is_explicit_interface_implementation(); }
    Accessibility accessibility() const override { return base_member_->accessibility(); }
    bool is_static() const override { return base_member_->is_static(); }
    bool is_abstract() const override { return base_member_->is_abstract(); }
    bool is_sealed() const override { return base_member_->is_sealed(); }
    std::string full_name() const override { return base_member_->full_name(); }
    std::string name() const override { return base_member_->name(); }
    std::string namespace_() const override { return base_member_->namespace_(); }
    std::string reflection_name() const override { return base_member_->reflection_name(); }
    const ICompilation& compilation() const override { return base_member_->compilation(); }
    const IModule* parent_module() const override { return base_member_->parent_module(); }
    const IMember* specialize(const TypeParameterSubstitution& new_substitution) const override;
    bool equals(const IMember* other, TypeVisitor& type_normalization) const override;
    bool equals(const IMember* other) const override;
    int hash_code() const override;
    std::string to_string() const override;
    const IMember* base_member() const { return base_member_; }

protected:
    SpecializedMember(const IMember* member_definition, const IType* declaring_type);
    void add_substitution(const TypeParameterSubstitution& new_substitution);
    // An accessor of a property or event, specialized as this member is (made once).
    const IMethod* wrap_accessor(std::atomic<const IMethod*>& cache, const IMethod* accessor_definition) const;
    virtual std::string type_name() const = 0;   // upstream's GetType().Name, for to_string

    const IMember* base_member_;
    TypeParameterSubstitution substitution_;

private:
    mutable std::atomic<const IType*> declaring_type_;
    mutable std::atomic<const IType*> return_type_{nullptr};
};

class SpecializedParameterizedMember : public SpecializedMember, public virtual IParameterizedMember {
public:
    const std::vector<const IParameter*>& parameters() const override;
    std::string to_string() const override;

protected:
    SpecializedParameterizedMember(const IParameterizedMember* member_definition, const IType* declaring_type)
        : SpecializedMember(member_definition, declaring_type) {}

private:
    mutable std::once_flag parameters_once_;
    mutable std::vector<std::unique_ptr<IParameter>> owned_parameters_;
    mutable std::vector<const IParameter*> parameters_;
};

class SpecializedMethod final : public SpecializedParameterizedMember, public virtual IMethod {
public:
    // Upstream's Create: the definition itself where nothing changes (an identity substitution, a non-generic method of a non-generic
    // type), method type arguments dropped for a method without type parameters.
    static const IMethod* create(const IMethod* method_definition, const TypeParameterSubstitution& substitution);
    // Upstream's constructor (a new specialization, even where Create would not make one).
    static const IMethod* make(const IMethod* method_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type = nullptr);

    const std::vector<const IType*>& type_arguments() const override;
    std::vector<const IAttribute*> get_return_type_attributes() const override { return method_definition_->get_return_type_attributes(); }
    bool return_type_is_ref_read_only() const override { return method_definition_->return_type_is_ref_read_only(); }
    bool this_is_ref_read_only() const override { return method_definition_->this_is_ref_read_only(); }
    bool is_init_only() const override { return method_definition_->is_init_only(); }
    const std::vector<const ITypeParameter*>& type_parameters() const override;
    bool is_extension_method() const override { return method_definition_->is_extension_method(); }
    bool is_local_function() const override { return method_definition_->is_local_function(); }
    bool is_constructor() const override { return method_definition_->is_constructor(); }
    bool is_destructor() const override { return method_definition_->is_destructor(); }
    bool is_operator() const override { return method_definition_->is_operator(); }
    bool has_body() const override { return method_definition_->has_body(); }
    bool is_accessor() const override { return method_definition_->is_accessor(); }
    MethodSemanticsAttributes accessor_kind() const override { return method_definition_->accessor_kind(); }
    const IMethod* reduced_from() const override { return nullptr; }
    const IMember* accessor_owner() const override;
    bool equals(const IMember* other, TypeVisitor& type_normalization) const override;
    bool equals(const IMember* other) const override;
    int hash_code() const override;
    const IMember* specialize(const TypeParameterSubstitution& new_substitution) const override;
    const IMethod* specialize_method(const TypeParameterSubstitution& new_substitution) const override;
    std::string to_string() const override;

    SpecializedMethod(const IMethod* method_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type);

protected:
    std::string type_name() const override { return "SpecializedMethod"; }

private:
    class SpecializedTypeParameter;

    const IMethod* method_definition_;
    std::vector<std::unique_ptr<SpecializedTypeParameter>> owned_type_parameters_;
    std::vector<const ITypeParameter*> specialized_type_parameters_;
    bool is_parameterized_;
    TypeParameterSubstitution substitution_without_specialized_type_parameters_;
    mutable std::atomic<const IMember*> accessor_owner_{nullptr};
};

class SpecializedField final : public SpecializedMember, public virtual IField {
public:
    static const IField* create(const IField* field_definition, const TypeParameterSubstitution& substitution);
    static const IField* make(const IField* field_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type = nullptr);

    std::string name() const override { return base_member_->name(); }
    bool is_read_only() const override { return field_definition_->is_read_only(); }
    bool return_type_is_ref_read_only() const override { return field_definition_->return_type_is_ref_read_only(); }
    bool is_volatile() const override { return field_definition_->is_volatile(); }
    const IType* type() const override { return return_type(); }
    bool is_const() const override { return field_definition_->is_const(); }
    ConstantValue get_constant_value(bool throw_on_invalid_metadata = false) const override {
        return field_definition_->get_constant_value(throw_on_invalid_metadata);
    }

    SpecializedField(const IField* field_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type);

protected:
    std::string type_name() const override { return "SpecializedField"; }

private:
    const IField* field_definition_;
};

class SpecializedProperty final : public SpecializedParameterizedMember, public virtual IProperty {
public:
    static const IProperty* create(const IProperty* property_definition, const TypeParameterSubstitution& substitution);
    static const IProperty* make(const IProperty* property_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type = nullptr);

    bool can_get() const override { return property_definition_->can_get(); }
    bool can_set() const override { return property_definition_->can_set(); }
    const IMethod* getter() const override { return wrap_accessor(getter_, property_definition_->getter()); }
    const IMethod* setter() const override { return wrap_accessor(setter_, property_definition_->setter()); }
    bool is_indexer() const override { return property_definition_->is_indexer(); }
    bool return_type_is_ref_read_only() const override { return property_definition_->return_type_is_ref_read_only(); }

    SpecializedProperty(const IProperty* property_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type);

protected:
    std::string type_name() const override { return "SpecializedProperty"; }

private:
    const IProperty* property_definition_;
    mutable std::atomic<const IMethod*> getter_{nullptr}, setter_{nullptr};
};

class SpecializedEvent final : public SpecializedMember, public virtual IEvent {
public:
    static const IEvent* create(const IEvent* event_definition, const TypeParameterSubstitution& substitution);
    static const IEvent* make(const IEvent* event_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type = nullptr);

    bool can_add() const override { return event_definition_->can_add(); }
    bool can_remove() const override { return event_definition_->can_remove(); }
    bool can_invoke() const override { return event_definition_->can_invoke(); }
    const IMethod* add_accessor() const override { return wrap_accessor(add_, event_definition_->add_accessor()); }
    const IMethod* remove_accessor() const override { return wrap_accessor(remove_, event_definition_->remove_accessor()); }
    const IMethod* invoke_accessor() const override { return wrap_accessor(invoke_, event_definition_->invoke_accessor()); }

    SpecializedEvent(const IEvent* event_definition, const TypeParameterSubstitution& substitution, const IType* declaring_type);

protected:
    std::string type_name() const override { return "SpecializedEvent"; }

private:
    const IEvent* event_definition_;
    mutable std::atomic<const IMethod*> add_{nullptr}, remove_{nullptr}, invoke_{nullptr};
};

// A parameter with its type substituted, owned by a specialized member.
class SpecializedParameter final : public virtual IParameter {
public:
    SpecializedParameter(const IParameter* base_parameter, const IType* new_type, const IParameterizedMember* new_owner)
        : base_(base_parameter), type_(new_type), owner_(new_owner) {}
    std::vector<const IAttribute*> get_attributes() const override { return base_->get_attributes(); }
    ReferenceKind reference_kind() const override { return base_->reference_kind(); }
    bool is_params() const override { return base_->is_params(); }
    bool is_optional() const override { return base_->is_optional(); }
    bool has_constant_value_in_signature() const override { return base_->has_constant_value_in_signature(); }
    const IParameterizedMember* owner() const override { return owner_; }
    std::string name() const override { return base_->name(); }
    const IType* type() const override { return type_; }
    bool is_const() const override { return base_->is_const(); }
    ConstantValue get_constant_value(bool throw_on_invalid_metadata = false) const override { return base_->get_constant_value(throw_on_invalid_metadata); }
    SymbolKind symbol_kind() const override { return SymbolKind::Parameter; }
    LifetimeAnnotation lifetime() const override { return base_->lifetime(); }
    std::string to_string() const override;

private:
    const IParameter* base_;
    const IType* type_;
    const IParameterizedMember* owner_;
};

// DefaultParameter.ToString(IParameter): "ref name:System.Int32 = 0".
std::string parameter_to_string(const IParameter& parameter);
// A constant as .NET's object.ToString writes it ("True", "1.5", the string itself; "null" for none).
std::string constant_to_string(const ConstantValue& value);

}  // namespace cb::dotnet::decompiler::typesystem
