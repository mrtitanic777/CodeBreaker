// dotnet/decompiler/typesystem/type_system.hpp -- the decompiler's model of .NET types and members: what a type is (a definition, a type
// parameter, an array, a generic instance ...), what a member is (method, field, property, event), and the module and compilation they
// belong to. Every part of the decompiler past the metadata reads programs through these interfaces.
//
//     const ITypeDefinition* t = compilation.find_type(KnownTypeCode::String)->get_definition();
//     for (const IMethod* m : t->get_methods()) m->name();
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: INamedElement, ISymbol, SymbolKind, IType, GetMemberOptions, IEntity, IMember,
// IMethod, IField, IProperty, IEvent, IParameter, ReferenceKind, LifetimeAnnotation, IParameterizedMember, ITypeParameter, TypeConstraint,
// VarianceModifier, IVariable, IModule, IModuleReference, INamespace, IAttribute, ICompilation, ICompilationProvider, ITypeDefinition,
// ITypeDefinitionOrUnknown, ITypeReference, ITypeResolveContext, Accessibility (and AccessibilityExtensions), TypeKind (with
// known_type_reference.hpp), Nullability), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// How the port holds them: a type system object is immutable once made (lazy caches aside) and handed out as a raw pointer; definitions
// and members belong to their module, types made from others (arrays, generic instances, pointers ...) to a TypeArena (type_arena.hpp)
// that keeps one object per distinct type. Upstream's IEnumerable results are std::vector; a Predicate is a std::function (empty: none);
// bool? is std::optional<bool>. ITypeDefinition is both an IType and an IEntity, so the shared bases are virtual.
#pragma once

// MSVC warns (C4250) wherever a class gets an interface member through the class that implements it rather than the interface: that is
// how the virtual interface inheritance here is meant to work.
#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(disable : 4250)
#endif

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "dotnet/decompiler/typesystem/full_type_name.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/util/decimal.hpp"
#include "dotnet/metadata/custom_attribute_decoder.hpp"
#include "dotnet/metadata/handles.hpp"

namespace cb::dotnet::decompiler::metadata {
class MetadataFile;
struct AssemblyVersion;
}

namespace cb::dotnet::decompiler::typesystem {

namespace srm = cb::dotnet::metadata;

class IType;
class ITypeDefinition;
class ITypeDefinitionOrUnknown;
class ITypeParameter;
class IEntity;
class IMember;
class IMethod;
class IField;
class IProperty;
class IEvent;
class IParameter;
class IParameterizedMember;
class IModule;
class INamespace;
class IAttribute;
class ICompilation;
class TypeVisitor;
class TypeParameterSubstitution;
class ExtensionInfo;
enum class KnownAttribute : std::uint8_t;

template <class T>
using Filter = std::function<bool(const T*)>;

enum class SymbolKind : std::uint8_t {
    None, Module, TypeDefinition, Field, Property, Indexer, Event, Method, Operator, Constructor, Destructor, Accessor, Namespace, Variable,
    Parameter, TypeParameter, Constraint, ReturnType,
};

enum class Accessibility : std::uint8_t { None, Private, ProtectedAndInternal, Protected, Internal, ProtectedOrInternal, Public };
// The accessibilities ordered as "visible to a subset of": a <= b unless a is protected and b internal.
bool less_than_or_equal(Accessibility a, Accessibility b);
Accessibility intersect(Accessibility a, Accessibility b);
Accessibility union_of(Accessibility a, Accessibility b);
// An entity's accessibility intersected with that of every type it is nested in.
Accessibility effective_accessibility(const IEntity& entity);

enum class Nullability : std::uint8_t { Oblivious = 0, NotNullable = 1, Nullable = 2 };

enum class GetMemberOptions : std::uint8_t { None = 0x00, ReturnMemberDefinitions = 0x01, IgnoreInheritedMembers = 0x02 };
constexpr GetMemberOptions operator|(GetMemberOptions a, GetMemberOptions b) {
    return static_cast<GetMemberOptions>(static_cast<int>(a) | static_cast<int>(b));
}
constexpr bool has(GetMemberOptions o, GetMemberOptions flag) { return (static_cast<int>(o) & static_cast<int>(flag)) == static_cast<int>(flag); }

enum class ReferenceKind : std::uint8_t { None, Out, Ref, In, RefReadOnly };
enum class VarianceModifier : std::uint8_t { Invariant, Covariant, Contravariant };

// MethodSemanticsAttributes (System.Reflection): what an accessor is to its property or event.
enum class MethodSemanticsAttributes : std::uint16_t { None = 0, Setter = 0x1, Getter = 0x2, Other = 0x4, Adder = 0x8, Remover = 0x10, Raiser = 0x20 };

struct LifetimeAnnotation {
    bool scoped_ref = false;
};

// A constant's value (IVariable.GetConstantValue's object): null, a primitive, a string, or a decimal (a DecimalConstantAttribute's).
using ConstantValue = std::variant<std::monostate, bool, char16_t, std::int8_t, std::uint8_t, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t,
                                   std::int64_t, std::uint64_t, float, double, std::u16string, util::Decimal>;
// A metadata constant as a type system constant.
ConstantValue to_constant_value(const srm::ConstantValue& value);

// ---- the common bases ----------------------------------------------------------------------------------------------------

class INamedElement {
public:
    virtual ~INamedElement() = default;
    // "System.Collections.Generic.List", "List", "System.Collections.Generic.List`1", "System.Collections.Generic"
    virtual std::string full_name() const = 0;
    virtual std::string name() const = 0;
    virtual std::string reflection_name() const = 0;
    virtual std::string namespace_() const = 0;
};

class ISymbol {
public:
    virtual ~ISymbol() = default;
    virtual SymbolKind symbol_kind() const = 0;
    virtual std::string name() const = 0;
};

class ICompilationProvider {
public:
    virtual ~ICompilationProvider() = default;
    virtual const ICompilation& compilation() const = 0;
};

// ---- types ---------------------------------------------------------------------------------------------------------------

class IType : public virtual INamedElement {
public:
    virtual TypeKind kind() const = 0;
    // true: a reference type; false: a value type; nullopt: not known (a type parameter without constraint, an unknown type)
    virtual std::optional<bool> is_reference_type() const = 0;
    virtual bool is_by_ref_like() const = 0;
    virtual Nullability nullability() const = 0;
    virtual const IType* change_nullability(Nullability nullability) const = 0;
    // The definition behind the type (a generic instance's generic type, an array's nothing): nullptr when there is none.
    virtual const ITypeDefinition* get_definition() const = 0;
    virtual const ITypeDefinitionOrUnknown* get_definition_or_unknown() const = 0;
    virtual const IType* declaring_type() const = 0;
    virtual int type_parameter_count() const = 0;
    virtual const std::vector<const ITypeParameter*>& type_parameters() const = 0;
    virtual const std::vector<const IType*>& type_arguments() const = 0;
    virtual const IType* accept_visitor(TypeVisitor& visitor) const = 0;
    virtual const IType* visit_children(TypeVisitor& visitor) const = 0;
    virtual std::vector<const IType*> direct_base_types() const = 0;
    virtual TypeParameterSubstitution get_substitution() const = 0;
    virtual std::vector<const IType*> get_nested_types(const Filter<ITypeDefinition>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const = 0;
    // Nested types with type arguments for their own type parameters (see upstream's note on why the extra parameter cannot leak).
    virtual std::vector<const IType*> get_nested_types(const std::vector<const IType*>& type_arguments, const Filter<ITypeDefinition>& filter = {},
                                                       GetMemberOptions options = GetMemberOptions::None) const = 0;
    virtual std::vector<const IMethod*> get_constructors(const Filter<IMethod>& filter = {},
                                                         GetMemberOptions options = GetMemberOptions::IgnoreInheritedMembers) const = 0;
    virtual std::vector<const IMethod*> get_methods(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const = 0;
    virtual std::vector<const IMethod*> get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter = {},
                                                    GetMemberOptions options = GetMemberOptions::None) const = 0;
    virtual std::vector<const IProperty*> get_properties(const Filter<IProperty>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const = 0;
    virtual std::vector<const IField*> get_fields(const Filter<IField>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const = 0;
    virtual std::vector<const IEvent*> get_events(const Filter<IEvent>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const = 0;
    virtual std::vector<const IMember*> get_members(const Filter<IMember>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const = 0;
    virtual std::vector<const IMethod*> get_accessors(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const = 0;

    // IEquatable<IType>: reference equality unless a type says otherwise (generic instances, arrays ... compare by structure).
    virtual bool equals(const IType* other) const = 0;
    virtual int hash_code() const = 0;
    // object.ToString: the reflection name, with nullability marks (?, !) where a type has them.
    virtual std::string to_string() const = 0;
};

// Two types are the same type (upstream's a.Equals(b), with null equal to null).
inline bool type_equals(const IType* a, const IType* b) { return a == b || (a != nullptr && a->equals(b)); }

class ITypeDefinitionOrUnknown : public virtual IType {
public:
    virtual FullTypeName full_type_name() const = 0;
};

// ---- entities and members ------------------------------------------------------------------------------------------------

class IEntity : public virtual ISymbol, public virtual ICompilationProvider, public virtual INamedElement {
public:
    std::string name() const override = 0;   // (ISymbol's and INamedElement's name are the same)
    virtual srm::EntityHandle metadata_token() const = 0;
    virtual const ITypeDefinition* declaring_type_definition() const = 0;
    virtual const IType* declaring_type() const = 0;
    virtual const IModule* parent_module() const = 0;
    virtual std::vector<const IAttribute*> get_attributes() const = 0;
    virtual bool has_attribute(KnownAttribute attribute) const = 0;
    virtual const IAttribute* get_attribute(KnownAttribute attribute) const = 0;
    virtual Accessibility accessibility() const = 0;
    virtual bool is_static() const = 0;
    virtual bool is_abstract() const = 0;
    virtual bool is_sealed() const = 0;
};

class IMember : public virtual IEntity {
public:
    // The member before any substitution (itself for a definition).
    virtual const IMember* member_definition() const = 0;
    virtual const IType* return_type() const = 0;
    const IType* declaring_type() const override = 0;
    virtual std::vector<const IMember*> explicitly_implemented_interface_members() const = 0;
    virtual bool is_explicit_interface_implementation() const = 0;
    virtual bool is_virtual() const = 0;
    virtual bool is_override() const = 0;
    virtual bool is_overridable() const = 0;
    virtual const TypeParameterSubstitution& substitution() const = 0;
    virtual const IMember* specialize(const TypeParameterSubstitution& substitution) const = 0;
    // Equal after normalizing the declaring types and substitutions with `type_normalization`.
    virtual bool equals(const IMember* other, TypeVisitor& type_normalization) const = 0;
    // object.Equals and GetHashCode: a definition is equal only to itself, a specialized member to one of the same definition and
    // substitution.
    virtual bool equals(const IMember* other) const = 0;
    virtual int hash_code() const = 0;
    virtual std::string to_string() const = 0;
};

// Two members are the same member (upstream's a.Equals(b), with null equal to null).
inline bool member_equals(const IMember* a, const IMember* b) { return a == b || (a != nullptr && a->equals(b)); }

class IVariable : public virtual ISymbol {
public:
    virtual const IType* type() const = 0;
    virtual bool is_const() const = 0;
    // The constant's value (null for none); with throw_on_invalid_metadata, BadImageFormatError for an unreadable one.
    virtual ConstantValue get_constant_value(bool throw_on_invalid_metadata = false) const = 0;
};

class IParameter : public virtual IVariable {
public:
    virtual std::vector<const IAttribute*> get_attributes() const = 0;
    virtual ReferenceKind reference_kind() const = 0;
    virtual LifetimeAnnotation lifetime() const = 0;
    virtual bool is_params() const = 0;
    virtual bool is_optional() const = 0;
    virtual bool has_constant_value_in_signature() const = 0;
    virtual const IParameterizedMember* owner() const = 0;
    // "ref name:System.Int32 = 0" (DefaultParameter.ToString)
    virtual std::string to_string() const = 0;
};

class IParameterizedMember : public virtual IMember {
public:
    virtual const std::vector<const IParameter*>& parameters() const = 0;
};

class IMethod : public virtual IParameterizedMember {
public:
    virtual std::vector<const IAttribute*> get_return_type_attributes() const = 0;
    virtual bool return_type_is_ref_read_only() const = 0;
    virtual bool is_init_only() const = 0;
    virtual bool this_is_ref_read_only() const = 0;
    virtual const std::vector<const ITypeParameter*>& type_parameters() const = 0;
    virtual const std::vector<const IType*>& type_arguments() const = 0;
    virtual bool is_extension_method() const = 0;
    virtual bool is_local_function() const = 0;
    virtual bool is_constructor() const = 0;
    virtual bool is_destructor() const = 0;
    virtual bool is_operator() const = 0;
    virtual bool has_body() const = 0;
    virtual bool is_accessor() const = 0;
    virtual const IMember* accessor_owner() const = 0;
    virtual MethodSemanticsAttributes accessor_kind() const = 0;
    // The static method an extension method was reduced from (nullptr when it was not).
    virtual const IMethod* reduced_from() const = 0;
    virtual const IMethod* specialize_method(const TypeParameterSubstitution& substitution) const = 0;
};

class IField : public virtual IMember, public virtual IVariable {
public:
    std::string name() const override = 0;
    virtual bool is_read_only() const = 0;
    virtual bool return_type_is_ref_read_only() const = 0;
    virtual bool is_volatile() const = 0;
};

class IProperty : public virtual IParameterizedMember {
public:
    virtual bool can_get() const = 0;
    virtual bool can_set() const = 0;
    virtual const IMethod* getter() const = 0;
    virtual const IMethod* setter() const = 0;
    virtual bool is_indexer() const = 0;
    virtual bool return_type_is_ref_read_only() const = 0;
};

class IEvent : public virtual IMember {
public:
    virtual bool can_add() const = 0;
    virtual bool can_remove() const = 0;
    virtual bool can_invoke() const = 0;
    virtual const IMethod* add_accessor() const = 0;
    virtual const IMethod* remove_accessor() const = 0;
    virtual const IMethod* invoke_accessor() const = 0;
};

struct TypeConstraint {
    const IType* type = nullptr;
    std::vector<const IAttribute*> attributes;
    static constexpr SymbolKind kSymbolKind = SymbolKind::Constraint;
};

class ITypeParameter : public virtual IType, public virtual ISymbol {
public:
    std::string name() const override = 0;
    virtual SymbolKind owner_type() const = 0;   // TypeDefinition or Method
    virtual const IEntity* owner() const = 0;
    virtual int index() const = 0;
    virtual std::vector<const IAttribute*> get_attributes() const = 0;
    virtual VarianceModifier variance() const = 0;
    virtual const IType* effective_base_class() const = 0;
    virtual const std::vector<const IType*>& effective_interface_set() const = 0;
    virtual bool has_default_constructor_constraint() const = 0;
    virtual bool has_reference_type_constraint() const = 0;
    virtual bool has_value_type_constraint() const = 0;
    virtual bool has_unmanaged_constraint() const = 0;
    virtual bool allows_ref_like_type() const = 0;
    virtual Nullability nullability_constraint() const = 0;
    virtual const std::vector<TypeConstraint>& type_constraints() const = 0;
};

class ITypeDefinition : public virtual ITypeDefinitionOrUnknown, public virtual IEntity {
public:
    std::string name() const override = 0;
    const IType* declaring_type() const override = 0;
    virtual const std::vector<const ITypeDefinition*>& nested_types() const = 0;
    virtual const std::vector<const IMember*>& members() const = 0;
    virtual std::vector<const IField*> fields() const = 0;
    virtual std::vector<const IMethod*> methods() const = 0;
    virtual std::vector<const IProperty*> properties() const = 0;
    virtual std::vector<const IEvent*> events() const = 0;
    virtual KnownTypeCode known_type_code() const = 0;
    virtual const IType* enum_underlying_type() const = 0;
    virtual bool is_read_only() const = 0;
    // The name as the metadata has it (with the `N of a generic type).
    virtual std::string metadata_name() const = 0;
    virtual bool has_extensions() const = 0;
    virtual const ExtensionInfo* extension_info() const = 0;
    virtual Nullability nullable_context() const = 0;
    virtual bool is_record() const = 0;
};

// ---- attributes, namespaces, modules, the compilation --------------------------------------------------------------------

class IAttribute {
public:
    virtual ~IAttribute() = default;
    virtual const IType* attribute_type() const = 0;
    virtual const IMethod* constructor() const = 0;
    virtual bool has_decode_errors() const = 0;
    virtual const std::vector<srm::CustomAttributeTypedArgument<const IType*>>& fixed_arguments() const = 0;
    virtual const std::vector<srm::CustomAttributeNamedArgument<const IType*>>& named_arguments() const = 0;
};

class INamespace : public virtual ISymbol, public virtual ICompilationProvider {
public:
    virtual std::string extern_alias() const = 0;
    virtual std::string full_name() const = 0;
    virtual const INamespace* parent_namespace() const = 0;
    virtual std::vector<const INamespace*> child_namespaces() const = 0;
    virtual std::vector<const ITypeDefinition*> types() const = 0;
    virtual std::vector<const IModule*> contributing_modules() const = 0;
    virtual const INamespace* get_child_namespace(std::string_view name) const = 0;
    virtual const ITypeDefinition* get_type_definition(std::string_view name, int type_parameter_count) const = 0;
};

class IModule : public virtual ISymbol, public virtual ICompilationProvider {
public:
    virtual const metadata::MetadataFile* metadata_file() const = 0;
    virtual bool is_main_module() const = 0;
    virtual std::string assembly_name() const = 0;
    virtual srm::Version assembly_version() const = 0;
    virtual std::string full_assembly_name() const = 0;
    virtual std::vector<const IAttribute*> get_assembly_attributes() const = 0;
    virtual std::vector<const IAttribute*> get_module_attributes() const = 0;
    virtual bool internals_visible_to(const IModule& module) const = 0;
    virtual const INamespace& root_namespace() const = 0;
    virtual const ITypeDefinition* get_type_definition(const TopLevelTypeName& top_level_type_name) const = 0;
    virtual std::vector<const ITypeDefinition*> top_level_type_definitions() const = 0;
    virtual std::vector<const ITypeDefinition*> type_definitions() const = 0;
};

class TypeArena;
enum class TypeSystemOptions : std::uint32_t;

class ICompilation {
public:
    virtual ~ICompilation() = default;
    virtual const IModule& main_module() const = 0;
    virtual const std::vector<const IModule*>& modules() const = 0;
    virtual const std::vector<const IModule*>& referenced_modules() const = 0;
    virtual const INamespace& root_namespace() const = 0;
    virtual const INamespace* get_namespace_for_extern_alias(std::string_view alias) const = 0;
    // A known type of the compilation, or an UnknownType when its corlib does not have it.
    virtual const IType* find_type(KnownTypeCode type_code) const = 0;
    virtual TypeSystemOptions type_system_options() const = 0;
    // Where the types this compilation makes are kept.
    virtual TypeArena& arena() const = 0;
};

// ---- references (types named but not yet resolved) ---------------------------------------------------------------------

class ITypeResolveContext : public virtual ICompilationProvider {
public:
    virtual const IModule* current_module() const = 0;
    virtual const ITypeDefinition* current_type_definition() const = 0;
    virtual const IMember* current_member() const = 0;
};

class ITypeReference {
public:
    virtual ~ITypeReference() = default;
    virtual const IType* resolve(const ITypeResolveContext& context) const = 0;
};

class IModuleReference {
public:
    virtual ~IModuleReference() = default;
    virtual const IModule* resolve(const ITypeResolveContext& context) const = 0;
};

}  // namespace cb::dotnet::decompiler::typesystem
