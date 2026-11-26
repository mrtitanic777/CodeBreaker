// dotnet/decompiler/typesystem/metadata_entities.hpp -- the type system's view of a module's own definitions: its types, methods,
// parameters, fields, properties, events and type parameters, each read from the metadata row it stands for (eagerly the little that
// says what it is, the rest when first asked), plus the parameter of no row (DefaultParameter) and a vararg call's method
// (VarArgInstanceMethod).
//
//     const ITypeDefinition* t = module.get_definition(type_handle);       // a MetadataTypeDefinition
//     for (const IMethod* m : t->methods()) m->parameters();
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem.Implementation: MetadataTypeDefinition, MetadataMethod, MetadataParameter,
// MetadataField, MetadataProperty, MetadataEvent, MetadataTypeParameter, DefaultParameter; TypeSystem: VarArgInstanceMethod;
// CSharp.Syntax.OperatorDeclaration's operator names), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
// Not yet ported: ExtensionInfo (C# 14 extension blocks: extension_info() is nullptr for now).
//
// An entity belongs to its module (made once per row); its attributes are made the first time they are asked for and kept.
#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/attributes.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/metadata/metadata_reader.hpp"
#include "dotnet/metadata/signature_decoder.hpp"

namespace cb::dotnet::decompiler::typesystem {

class MetadataModule;
class MetadataTypeParameter;
struct GenericContext;

// The made-up parameter (no Param row): a type, a name and what else is given.
class DefaultParameter final : public IParameter {
public:
    DefaultParameter(const IType* type, std::string name, const IParameterizedMember* owner = nullptr, std::vector<const IAttribute*> attributes = {},
                     ReferenceKind reference_kind = ReferenceKind::None, bool is_params = false, bool is_optional = false,
                     ConstantValue default_value = std::monostate());
    SymbolKind symbol_kind() const override { return SymbolKind::Parameter; }
    const IParameterizedMember* owner() const override { return owner_; }
    std::vector<const IAttribute*> get_attributes() const override { return attributes_; }
    ReferenceKind reference_kind() const override { return reference_kind_; }
    bool is_params() const override { return is_params_; }
    bool is_optional() const override { return is_optional_; }
    std::string name() const override { return name_; }
    const IType* type() const override { return type_; }
    bool is_const() const override { return false; }
    bool has_constant_value_in_signature() const override { return is_optional_; }
    LifetimeAnnotation lifetime() const override { return {}; }
    ConstantValue get_constant_value(bool = false) const override { return default_value_; }
    std::string to_string() const override;

private:
    const IType* type_;
    std::string name_;
    std::vector<const IAttribute*> attributes_;
    ReferenceKind reference_kind_;
    bool is_params_, is_optional_;
    ConstantValue default_value_;
    const IParameterizedMember* owner_;
};

// The attributes of an entity, made on the first call and kept.
class LazyAttributeList {
public:
    template <class Build>
    const std::vector<const IAttribute*>& get(Build&& build) const {
        std::call_once(once_, [&] { list_ = build(); });
        return list_.attributes;
    }

private:
    mutable std::once_flag once_;
    mutable AttributeList list_;
};

// ---- types ---------------------------------------------------------------------------------------------------------------

class MetadataTypeDefinition final : public ITypeDefinition {
public:
    MetadataTypeDefinition(const MetadataModule& module, srm::TypeDefinitionHandle handle);
    ~MetadataTypeDefinition() override;

    // ITypeDefinition
    const std::vector<const ITypeDefinition*>& nested_types() const override;
    const std::vector<const IMember*>& members() const override;
    std::vector<const IField*> fields() const override;
    std::vector<const IMethod*> methods() const override;
    std::vector<const IProperty*> properties() const override;
    std::vector<const IEvent*> events() const override;
    KnownTypeCode known_type_code() const override { return known_type_code_; }
    const IType* enum_underlying_type() const override { return enum_underlying_type_; }
    bool is_read_only() const override { return is_read_only_; }
    std::string metadata_name() const override { return metadata_name_; }
    bool has_extensions() const override { return has_extensions_; }
    const ExtensionInfo* extension_info() const override { return nullptr; }
    Nullability nullable_context() const override { return nullable_context_; }
    bool is_record() const override;
    FullTypeName full_type_name() const override { return full_type_name_; }
    // [DefaultMember]'s name (the indexer's name), nullopt for none.
    const std::optional<std::string>& default_member_name() const;
    // The methods a method of this type explicitly implements (its MethodImpl rows).
    std::vector<const IMethod*> get_overrides(srm::MethodDefinitionHandle method) const;
    bool has_overrides(srm::MethodDefinitionHandle method) const;
    srm::TypeDefinitionHandle handle() const { return handle_; }

    // IEntity
    srm::EntityHandle metadata_token() const override { return handle_; }
    const ITypeDefinition* declaring_type_definition() const override { return declaring_type_definition_; }
    const IType* declaring_type() const override { return declaring_type_definition_; }
    const IModule* parent_module() const override;
    std::vector<const IAttribute*> get_attributes() const override;
    bool has_attribute(KnownAttribute attribute) const override;
    const IAttribute* get_attribute(KnownAttribute attribute) const override;
    Accessibility accessibility() const override;
    bool is_static() const override { return (attributes_ & 0x180) == 0x180; }   // Abstract | Sealed
    bool is_abstract() const override { return (attributes_ & 0x80) != 0; }
    bool is_sealed() const override { return (attributes_ & 0x100) != 0; }
    SymbolKind symbol_kind() const override { return SymbolKind::TypeDefinition; }
    const ICompilation& compilation() const override;

    // INamedElement
    std::string name() const override { return full_type_name_.name(); }
    std::string full_name() const override;
    std::string reflection_name() const override { return full_type_name_.reflection_name(); }
    std::string namespace_() const override { return full_type_name_.top_level_type_name().namespace_(); }

    // IType
    TypeKind kind() const override { return kind_; }
    std::optional<bool> is_reference_type() const override;
    bool is_by_ref_like() const override { return is_by_ref_like_; }
    Nullability nullability() const override { return Nullability::Oblivious; }
    const IType* change_nullability(Nullability nullability) const override;
    const ITypeDefinition* get_definition() const override { return this; }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return this; }
    int type_parameter_count() const override { return static_cast<int>(type_parameters_.size()); }
    const std::vector<const ITypeParameter*>& type_parameters() const override { return type_parameters_; }
    const std::vector<const IType*>& type_arguments() const override { return type_arguments_; }
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_type_definition(*this); }
    const IType* visit_children(TypeVisitor&) const override { return this; }
    std::vector<const IType*> direct_base_types() const override;
    TypeParameterSubstitution get_substitution() const override { return TypeParameterSubstitution::identity(); }
    std::vector<const IType*> get_nested_types(const Filter<ITypeDefinition>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IType*> get_nested_types(const std::vector<const IType*>& type_arguments, const Filter<ITypeDefinition>& filter = {},
                                               GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_constructors(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::IgnoreInheritedMembers) const override;
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter = {},
                                            GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IField*> get_fields(const Filter<IField>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IEvent*> get_events(const Filter<IEvent>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMember*> get_members(const Filter<IMember>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    // The same row of the same file (as upstream: across compilations too).
    bool equals(const IType* other) const override;
    int hash_code() const override;
    std::string to_string() const override;   // "02000002 Namespace.Name"

private:
    const MetadataModule* module_;
    srm::TypeDefinitionHandle handle_;
    FullTypeName full_type_name_;
    std::uint32_t attributes_;
    TypeKind kind_ = TypeKind::Class;
    bool is_by_ref_like_ = false, is_read_only_ = false, has_extensions_ = false;
    const ITypeDefinition* declaring_type_definition_ = nullptr;
    std::vector<std::unique_ptr<MetadataTypeParameter>> owned_type_parameters_;
    std::vector<const ITypeParameter*> type_parameters_;
    std::vector<const IType*> type_arguments_;
    KnownTypeCode known_type_code_ = KnownTypeCode::None;
    const IType* enum_underlying_type_ = nullptr;
    Nullability nullable_context_ = Nullability::Oblivious;
    std::string metadata_name_;

    mutable std::once_flag nested_once_, members_once_, fields_once_, properties_once_, events_once_, methods_once_, bases_once_, default_member_once_,
        record_once_;
    mutable std::vector<const ITypeDefinition*> nested_types_;
    mutable std::vector<const IMember*> members_;
    mutable std::vector<const IField*> fields_;
    mutable std::vector<const IProperty*> properties_;
    mutable std::vector<const IEvent*> events_;
    mutable std::vector<const IMethod*> methods_;
    mutable std::vector<const IType*> direct_base_types_;
    mutable std::optional<std::string> default_member_name_;
    mutable bool is_record_ = false;
    LazyAttributeList attributes_list_;
};

class MetadataTypeParameter final : public AbstractTypeParameter {
public:
    // The parameters of a type or method (a nested type's first ones are its outer type's: `copy_from_outer`).
    static std::vector<std::unique_ptr<MetadataTypeParameter>> create(const MetadataModule& module, const IEntity* owner,
                                                                      const std::vector<srm::GenericParameterHandle>& handles,
                                                                      std::size_t first_own = 0);

    MetadataTypeParameter(const MetadataModule& module, const IEntity* owner, int index, std::string name, srm::GenericParameterHandle handle,
                          std::uint16_t attributes);

    srm::GenericParameterHandle metadata_token() const { return handle_; }
    std::vector<const IAttribute*> get_attributes() const override;
    bool has_default_constructor_constraint() const override { return (attr_ & 0x10) != 0; }
    bool has_reference_type_constraint() const override { return (attr_ & 0x04) != 0; }
    bool has_value_type_constraint() const override { return (attr_ & 0x08) != 0; }   // NotNullableValueTypeConstraint
    bool allows_ref_like_type() const override { return (attr_ & 0x20) != 0; }        // AllowByRefLike
    bool has_unmanaged_constraint() const override;
    Nullability nullability_constraint() const override;
    const std::vector<TypeConstraint>& type_constraints() const override;
    bool equals(const IType* other) const override;
    int hash_code() const override;
    std::string to_string() const override;

private:
    Nullability load_nullability_constraint() const;

    const MetadataModule* module_;
    srm::GenericParameterHandle handle_;
    std::uint16_t attr_;
    mutable std::once_flag unmanaged_once_, nullability_once_, constraints_once_;
    mutable bool unmanaged_constraint_ = false;
    mutable Nullability nullability_constraint_ = Nullability::Oblivious;
    mutable std::vector<TypeConstraint> constraints_;
    mutable std::vector<AttributeList> constraint_attributes_;
    LazyAttributeList attributes_list_;
};

// ---- members -------------------------------------------------------------------------------------------------------------

// The parts every Metadata* member has.
class MetadataMemberBase : public virtual IMember {
public:
    const IModule* parent_module() const override;
    const ICompilation& compilation() const override;
    const IMember* member_definition() const override { return this; }
    const TypeParameterSubstitution& substitution() const override { return TypeParameterSubstitution::identity(); }
    std::string full_name() const override;
    std::string reflection_name() const override;
    std::string namespace_() const override;
    using IMember::equals;
    bool equals(const IMember* other, TypeVisitor&) const override { return equals(other); }
    std::string to_string() const override;   // "06000001 Namespace.Type.Name"
    const MetadataModule& module() const { return *module_; }

protected:
    explicit MetadataMemberBase(const MetadataModule& module) : module_(&module) {}
    const MetadataModule* module_;
};

class MetadataMethod final : public MetadataMemberBase, public virtual IMethod {
public:
    MetadataMethod(const MetadataModule& module, srm::MethodDefinitionHandle handle);
    ~MetadataMethod() override;

    srm::EntityHandle metadata_token() const override { return handle_; }
    std::string name() const override;
    const std::vector<const ITypeParameter*>& type_parameters() const override { return type_parameters_; }
    const std::vector<const IType*>& type_arguments() const override { return type_arguments_; }
    SymbolKind symbol_kind() const override { return symbol_kind_; }
    bool is_constructor() const override { return symbol_kind_ == SymbolKind::Constructor; }
    bool is_destructor() const override { return symbol_kind_ == SymbolKind::Destructor; }
    bool is_operator() const override { return symbol_kind_ == SymbolKind::Operator; }
    bool is_accessor() const override { return symbol_kind_ == SymbolKind::Accessor; }
    bool is_extension_method() const override { return is_extension_method_; }
    bool is_local_function() const override { return false; }
    bool has_body() const override;
    const IMember* accessor_owner() const override;
    MethodSemanticsAttributes accessor_kind() const override { return accessor_kind_; }
    const std::vector<const IParameter*>& parameters() const override;
    const IType* return_type() const override;
    bool is_init_only() const override;
    // [NullableContext] of the method, else of its type.
    Nullability nullable_context() const;
    bool is_explicit_interface_implementation() const override;
    std::vector<const IMember*> explicitly_implemented_interface_members() const override;
    const IMethod* reduced_from() const override { return nullptr; }
    const ITypeDefinition* declaring_type_definition() const override;
    const IType* declaring_type() const override { return declaring_type_definition(); }
    std::vector<const IAttribute*> get_attributes() const override;
    bool has_attribute(KnownAttribute attribute) const override;
    const IAttribute* get_attribute(KnownAttribute attribute) const override;
    std::vector<const IAttribute*> get_return_type_attributes() const override;
    bool return_type_is_ref_read_only() const override;
    bool this_is_ref_read_only() const override;
    Accessibility accessibility() const override;
    bool is_static() const override { return (attributes_ & 0x10) != 0; }
    bool is_abstract() const override { return (attributes_ & 0x400) != 0; }
    bool is_sealed() const override { return (attributes_ & (0x400 | 0x20 | 0x100 | 0x10)) == 0x20; }   // Final, not Abstract, NewSlot, Static
    bool is_virtual() const override;
    bool is_override() const override { return (attributes_ & (0x100 | 0x40 | 0x10)) == 0x40; }   // Virtual without NewSlot, not Static
    bool is_overridable() const override { return (attributes_ & (0x400 | 0x40)) != 0 && (attributes_ & 0x20) == 0; }
    bool equals(const IMember* other) const override;
    int hash_code() const override;
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override;
    const IMethod* specialize_method(const TypeParameterSubstitution& substitution) const override;

    // MethodAttributes' access as an accessibility.
    static Accessibility get_accessibility(std::uint16_t method_attributes);

    // The parameters and return type of a method or property signature, with the attributes of its Param rows applied (rows missing from
    // the metadata become DefaultParameters); the return type's outermost modifier, if any.
    struct DecodedSignature {
        const IType* return_type = nullptr;
        std::vector<std::unique_ptr<IParameter>> parameters;
        const ModifiedType* return_type_modifier = nullptr;
    };
    static DecodedSignature decode_signature(const MetadataModule& module, const IParameterizedMember* owner, const srm::MethodSignature<const IType*>& signature,
                                             const srm::HandleList<srm::ParameterHandle>* parameter_handles, Nullability nullable_context,
                                             TypeSystemOptions type_system_options, const std::vector<srm::CustomAttributeHandle>* additional_return_type_attributes = nullptr);

private:
    void decode_signature() const;

    srm::MethodDefinitionHandle handle_;
    std::uint16_t attributes_;
    SymbolKind symbol_kind_ = SymbolKind::Method;
    std::vector<std::unique_ptr<MetadataTypeParameter>> owned_type_parameters_;
    std::vector<const ITypeParameter*> type_parameters_;
    std::vector<const IType*> type_arguments_;
    srm::EntityHandle accessor_owner_;
    MethodSemanticsAttributes accessor_kind_ = MethodSemanticsAttributes::None;
    bool is_extension_method_ = false;

    mutable std::once_flag name_once_, signature_once_, declaring_once_, return_readonly_once_, this_readonly_once_;
    mutable std::string name_;
    mutable const IType* return_type_ = nullptr;
    mutable std::vector<std::unique_ptr<IParameter>> owned_parameters_;
    mutable std::vector<const IParameter*> parameters_;
    mutable bool is_init_only_ = false;
    mutable const ITypeDefinition* declaring_type_ = nullptr;
    mutable bool return_type_is_ref_readonly_ = false, this_is_ref_readonly_ = false;
    LazyAttributeList attributes_list_, return_type_attributes_list_;
};

class MetadataParameter final : public IParameter {
public:
    MetadataParameter(const MetadataModule& module, const IParameterizedMember* owner, const IType* type, srm::ParameterHandle handle);

    srm::EntityHandle metadata_token() const { return handle_; }
    std::vector<const IAttribute*> get_attributes() const override;
    ReferenceKind reference_kind() const override;
    bool is_optional() const override { return (attributes_ & 0x10) != 0; }
    LifetimeAnnotation lifetime() const override;
    bool is_params() const override;
    std::string name() const override;
    bool is_const() const override { return false; }
    const IType* type() const override { return type_; }
    const IParameterizedMember* owner() const override { return owner_; }
    ConstantValue get_constant_value(bool throw_on_invalid_metadata = false) const override;
    bool has_constant_value_in_signature() const override;
    SymbolKind symbol_kind() const override { return SymbolKind::Parameter; }
    std::string to_string() const override;

private:
    bool is_decimal_constant() const;

    const MetadataModule* module_;
    srm::ParameterHandle handle_;
    std::uint16_t attributes_;
    const IType* type_;
    const IParameterizedMember* owner_;
    mutable std::once_flag name_once_, constant_in_signature_once_, decimal_once_;
    mutable std::string name_;
    mutable bool constant_value_in_signature_ = false, decimal_constant_ = false;
    LazyAttributeList attributes_list_;
};

class MetadataField final : public MetadataMemberBase, public virtual IField {
public:
    MetadataField(const MetadataModule& module, srm::FieldDefinitionHandle handle);

    srm::EntityHandle metadata_token() const override { return handle_; }
    std::string name() const override;
    Accessibility accessibility() const override;
    bool is_read_only() const override { return (attributes_ & 0x20) != 0; }   // InitOnly
    bool is_static() const override { return (attributes_ & 0x10) != 0; }
    SymbolKind symbol_kind() const override { return SymbolKind::Field; }
    std::vector<const IMember*> explicitly_implemented_interface_members() const override { return {}; }
    bool is_explicit_interface_implementation() const override { return false; }
    bool is_virtual() const override { return false; }
    bool is_override() const override { return false; }
    bool is_overridable() const override { return false; }
    bool is_abstract() const override { return false; }
    bool is_sealed() const override { return false; }
    const ITypeDefinition* declaring_type_definition() const override;
    const IType* declaring_type() const override { return declaring_type_definition(); }
    std::vector<const IAttribute*> get_attributes() const override;
    bool has_attribute(KnownAttribute attribute) const override;
    const IAttribute* get_attribute(KnownAttribute attribute) const override;
    bool return_type_is_ref_read_only() const override;
    bool is_volatile() const override;
    const IType* return_type() const override { return type(); }
    const IType* type() const override;
    bool is_const() const override;
    ConstantValue get_constant_value(bool throw_on_invalid_metadata = false) const override;
    bool equals(const IMember* other) const override;
    int hash_code() const override;
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override;

private:
    void decode_type_and_volatile_flag() const;
    bool is_decimal_constant() const;

    srm::FieldDefinitionHandle handle_;
    std::uint16_t attributes_;
    mutable std::once_flag name_once_, declaring_once_, type_once_, decimal_once_, constant_once_;
    mutable std::string name_;
    mutable const ITypeDefinition* declaring_type_ = nullptr;
    mutable const IType* type_ = nullptr;
    mutable bool is_volatile_ = false, decimal_constant_ = false;
    mutable ConstantValue constant_value_;
    mutable bool constant_ok_ = false;
    LazyAttributeList attributes_list_;
};

class MetadataProperty final : public MetadataMemberBase, public virtual IProperty {
public:
    MetadataProperty(const MetadataModule& module, srm::PropertyDefinitionHandle handle);
    ~MetadataProperty() override;

    srm::EntityHandle metadata_token() const override { return handle_; }
    std::string name() const override { return name_; }
    bool can_get() const override { return getter_ != nullptr; }
    bool can_set() const override { return setter_ != nullptr; }
    const IMethod* getter() const override { return getter_; }
    const IMethod* setter() const override { return setter_; }
    bool is_indexer() const override { return symbol_kind_ == SymbolKind::Indexer; }
    SymbolKind symbol_kind() const override { return symbol_kind_; }
    const std::vector<const IParameter*>& parameters() const override;
    const IType* return_type() const override;
    bool return_type_is_ref_read_only() const override;
    bool is_explicit_interface_implementation() const override;
    std::vector<const IMember*> explicitly_implemented_interface_members() const override;
    const ITypeDefinition* declaring_type_definition() const override;
    const IType* declaring_type() const override;
    std::vector<const IAttribute*> get_attributes() const override;
    bool has_attribute(KnownAttribute attribute) const override;
    const IAttribute* get_attribute(KnownAttribute attribute) const override;
    Accessibility accessibility() const override;
    bool is_static() const override;
    bool is_abstract() const override;
    bool is_sealed() const override;
    bool is_virtual() const override;
    bool is_override() const override;
    bool is_overridable() const override;
    bool equals(const IMember* other) const override;
    int hash_code() const override;
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override;

    // The property or event members an accessor's explicitly implemented interface members are accessors of.
    static std::vector<const IMember*> get_interface_members_from_accessor(const IMethod* method);

private:
    const IMethod* any_accessor() const { return getter_ != nullptr ? getter_ : setter_; }
    bool determine_is_indexer(const std::string& name) const;
    void decode_signature() const;
    Accessibility compute_accessibility() const;

    srm::PropertyDefinitionHandle handle_;
    const IMethod* getter_;
    const IMethod* setter_;
    std::string name_;
    SymbolKind symbol_kind_ = SymbolKind::Property;
    mutable std::once_flag signature_once_, accessibility_once_;
    mutable const IType* return_type_ = nullptr;
    mutable std::vector<std::unique_ptr<IParameter>> owned_parameters_;
    mutable std::vector<const IParameter*> parameters_;
    mutable Accessibility cached_accessibility_ = Accessibility::None;
    LazyAttributeList attributes_list_;
};

class MetadataEvent final : public MetadataMemberBase, public virtual IEvent {
public:
    MetadataEvent(const MetadataModule& module, srm::EventDefinitionHandle handle);

    srm::EntityHandle metadata_token() const override { return handle_; }
    std::string name() const override { return name_; }
    SymbolKind symbol_kind() const override { return SymbolKind::Event; }
    bool can_add() const override { return !accessors_.adder.is_nil(); }
    bool can_remove() const override { return !accessors_.remover.is_nil(); }
    bool can_invoke() const override { return !accessors_.raiser.is_nil(); }
    const IMethod* add_accessor() const override;
    const IMethod* remove_accessor() const override;
    const IMethod* invoke_accessor() const override;
    const IType* return_type() const override;
    bool is_explicit_interface_implementation() const override;
    std::vector<const IMember*> explicitly_implemented_interface_members() const override;
    const ITypeDefinition* declaring_type_definition() const override;
    const IType* declaring_type() const override;
    std::vector<const IAttribute*> get_attributes() const override;
    bool has_attribute(KnownAttribute attribute) const override;
    const IAttribute* get_attribute(KnownAttribute attribute) const override;
    Accessibility accessibility() const override;
    bool is_static() const override;
    bool is_abstract() const override;
    bool is_sealed() const override;
    bool is_virtual() const override;
    bool is_override() const override;
    bool is_overridable() const override;
    bool equals(const IMember* other) const override;
    int hash_code() const override;
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override;

private:
    const IMethod* any_accessor() const;

    srm::EventDefinitionHandle handle_;
    srm::EventAccessors accessors_;
    std::string name_;
    mutable std::once_flag return_type_once_;
    mutable const IType* return_type_ = nullptr;
    LazyAttributeList attributes_list_;
};

// A vararg method as a call site sees it: the method's fixed parameters, then the call's extra arguments' types.
class VarArgInstanceMethod final : public IMethod {
public:
    VarArgInstanceMethod(const IMethod* base_method, const std::vector<const IType*>& var_arg_types);

    const IMethod* base_method() const { return base_method_; }
    int regular_parameter_count() const { return static_cast<int>(base_method_->parameters().size()) - 1; }
    const std::vector<const IParameter*>& parameters() const override { return parameters_; }
    bool equals(const IMember* other) const override;
    int hash_code() const override { return base_method_->hash_code(); }
    bool equals(const IMember* other, TypeVisitor& type_normalization) const override;
    std::string to_string() const override;   // "[Method Type.Name(A, ..., B):R]"
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override { return specialize_method(substitution); }
    const IMethod* specialize_method(const TypeParameterSubstitution& substitution) const override;

    std::vector<const IAttribute*> get_attributes() const override { return base_method_->get_attributes(); }
    bool has_attribute(KnownAttribute attribute) const override { return base_method_->has_attribute(attribute); }
    const IAttribute* get_attribute(KnownAttribute attribute) const override { return base_method_->get_attribute(attribute); }
    std::vector<const IAttribute*> get_return_type_attributes() const override { return base_method_->get_return_type_attributes(); }
    bool return_type_is_ref_read_only() const override { return base_method_->return_type_is_ref_read_only(); }
    bool this_is_ref_read_only() const override { return base_method_->this_is_ref_read_only(); }
    bool is_init_only() const override { return base_method_->is_init_only(); }
    const std::vector<const ITypeParameter*>& type_parameters() const override { return base_method_->type_parameters(); }
    const std::vector<const IType*>& type_arguments() const override { return base_method_->type_arguments(); }
    srm::EntityHandle metadata_token() const override { return base_method_->metadata_token(); }
    bool is_extension_method() const override { return base_method_->is_extension_method(); }
    bool is_local_function() const override { return base_method_->is_local_function(); }
    bool is_constructor() const override { return base_method_->is_constructor(); }
    bool is_destructor() const override { return base_method_->is_destructor(); }
    bool is_operator() const override { return base_method_->is_operator(); }
    bool has_body() const override { return base_method_->has_body(); }
    bool is_accessor() const override { return base_method_->is_accessor(); }
    const IMember* accessor_owner() const override { return base_method_->accessor_owner(); }
    MethodSemanticsAttributes accessor_kind() const override { return base_method_->accessor_kind(); }
    const IMethod* reduced_from() const override { return base_method_->reduced_from(); }
    const IMember* member_definition() const override { return base_method_->member_definition(); }
    const IType* return_type() const override { return base_method_->return_type(); }
    std::vector<const IMember*> explicitly_implemented_interface_members() const override { return base_method_->explicitly_implemented_interface_members(); }
    bool is_explicit_interface_implementation() const override { return base_method_->is_explicit_interface_implementation(); }
    bool is_virtual() const override { return base_method_->is_virtual(); }
    bool is_override() const override { return base_method_->is_override(); }
    bool is_overridable() const override { return base_method_->is_overridable(); }
    const TypeParameterSubstitution& substitution() const override { return base_method_->substitution(); }
    SymbolKind symbol_kind() const override { return base_method_->symbol_kind(); }
    std::string name() const override { return base_method_->name(); }
    const ITypeDefinition* declaring_type_definition() const override { return base_method_->declaring_type_definition(); }
    const IType* declaring_type() const override { return base_method_->declaring_type(); }
    const IModule* parent_module() const override { return base_method_->parent_module(); }
    bool is_static() const override { return base_method_->is_static(); }
    bool is_abstract() const override { return base_method_->is_abstract(); }
    bool is_sealed() const override { return base_method_->is_sealed(); }
    Accessibility accessibility() const override { return base_method_->accessibility(); }
    std::string full_name() const override { return base_method_->full_name(); }
    std::string reflection_name() const override { return base_method_->reflection_name(); }
    std::string namespace_() const override { return base_method_->namespace_(); }
    const ICompilation& compilation() const override { return base_method_->compilation(); }

private:
    const IMethod* base_method_;
    std::vector<std::unique_ptr<DefaultParameter>> owned_parameters_;
    std::vector<const IParameter*> parameters_;
};

// OperatorDeclaration.GetOperatorType(name) != null: a C# operator method's name (op_Addition, op_Implicit ...).
bool is_operator_method_name(std::string_view name);

}  // namespace cb::dotnet::decompiler::typesystem
