// dotnet/decompiler/typesystem/types.hpp -- the kinds of type that are not definitions: the special types (unknown, null, dynamic, nint ...),
// an unknown type by name, arrays, pointers, references, modified types, generic instances, nullability-annotated types, intersections,
// type parameters (abstract, default, dummy); the visitor that rebuilds a type part by part, and the substitution of type arguments for
// type parameters.
//
//     const IType* list_of_int = make_parameterized_type(list_definition, {int32});
//     list_of_int->accept_visitor(substitution);                 // a TypeVisitor rebuilds the type it visits
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: TypeVisitor, TypeParameterSubstitution, SpecialType, ArrayType, PointerType,
// ByReferenceType, ParameterizedType, ModifiedType, IntersectionType, NullableType, NormalizeTypeVisitor; TypeSystem.Implementation:
// AbstractType, UnknownType, TypeWithElementType, DecoratedType, NullabilityAnnotatedType, NullabilityAnnotatedTypeParameter,
// AbstractTypeParameter, DefaultTypeParameter, DummyTypeParameter; Util.BusyManager), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler). Upstream's `new ArrayType(...)` and the like are the make_* functions (type_arena.hpp says why); the
// ITypeReference classes of these types (ArrayTypeReference ...) come with their users.
#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/type_arena.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::typesystem {

class ParameterizedType;
class ArrayType;
class PointerType;
class ByReferenceType;
class ModifiedType;
class NullabilityAnnotatedType;
class TupleType;
class FunctionPointerType;

// ---- the visitor and the substitution ------------------------------------------------------------------------------------

class TypeVisitor {
public:
    virtual ~TypeVisitor() = default;
    virtual const IType* visit_type_definition(const ITypeDefinition& type);
    virtual const IType* visit_type_parameter(const ITypeParameter& type);
    virtual const IType* visit_parameterized_type(const ParameterizedType& type);
    virtual const IType* visit_array_type(const ArrayType& type);
    virtual const IType* visit_pointer_type(const PointerType& type);
    virtual const IType* visit_by_reference_type(const ByReferenceType& type);
    virtual const IType* visit_tuple_type(const TupleType& type);
    virtual const IType* visit_other_type(const IType& type);
    virtual const IType* visit_mod_req(const ModifiedType& type);
    virtual const IType* visit_mod_opt(const ModifiedType& type);
    virtual const IType* visit_nullability_annotated_type(const NullabilityAnnotatedType& type);
    virtual const IType* visit_function_pointer_type(const FunctionPointerType& type);
};

// Type arguments for the class's and the method's type parameters (nullopt: that kind of parameter stays as it is).
class TypeParameterSubstitution : public TypeVisitor {
public:
    TypeParameterSubstitution() = default;   // Identity
    TypeParameterSubstitution(std::optional<std::vector<const IType*>> class_type_arguments, std::optional<std::vector<const IType*>> method_type_arguments)
        : class_type_arguments_(std::move(class_type_arguments)), method_type_arguments_(std::move(method_type_arguments)) {}
    static const TypeParameterSubstitution& identity();

    const std::optional<std::vector<const IType*>>& class_type_arguments() const { return class_type_arguments_; }
    const std::optional<std::vector<const IType*>>& method_type_arguments() const { return method_type_arguments_; }
    bool is_identity() const { return !class_type_arguments_ && !method_type_arguments_; }
    // g after f: f's type arguments with g applied to them.
    static TypeParameterSubstitution compose(const TypeParameterSubstitution* g, const TypeParameterSubstitution* f);
    bool equals(const TypeParameterSubstitution& other, TypeVisitor& normalization) const;
    bool equals(const TypeParameterSubstitution& other) const;
    int hash_code() const;
    std::string to_string() const;

    const IType* visit_type_parameter(const ITypeParameter& type) override;
    const IType* visit_nullability_annotated_type(const NullabilityAnnotatedType& type) override;

private:
    std::optional<std::vector<const IType*>> class_type_arguments_, method_type_arguments_;
};

// ---- the abstract bases --------------------------------------------------------------------------------------------------

class AbstractType : public virtual IType {
public:
    std::string full_name() const override;
    std::string namespace_() const override { return {}; }
    std::string reflection_name() const override { return full_name(); }
    bool is_by_ref_like() const override { return false; }
    Nullability nullability() const override { return Nullability::Oblivious; }
    const IType* change_nullability(Nullability) const override { return this; }
    int type_parameter_count() const override { return 0; }
    const std::vector<const ITypeParameter*>& type_parameters() const override;
    const std::vector<const IType*>& type_arguments() const override;
    const IType* declaring_type() const override { return nullptr; }
    const ITypeDefinition* get_definition() const override { return nullptr; }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return nullptr; }
    std::vector<const IType*> direct_base_types() const override { return {}; }
    std::vector<const IType*> get_nested_types(const Filter<ITypeDefinition>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IType*> get_nested_types(const std::vector<const IType*>&, const Filter<ITypeDefinition>& = {},
                                               GetMemberOptions = GetMemberOptions::None) const override {
        return {};
    }
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>&, const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::None) const override {
        return {};
    }
    std::vector<const IMethod*> get_constructors(const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::IgnoreInheritedMembers) const override {
        return {};
    }
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IField*> get_fields(const Filter<IField>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IEvent*> get_events(const Filter<IEvent>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    // Methods, properties, fields, then events (as upstream's default).
    std::vector<const IMember*> get_members(const Filter<IMember>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    TypeParameterSubstitution get_substitution() const override;
    bool equals(const IType* other) const override { return this == other; }
    int hash_code() const override;
    std::string to_string() const override { return reflection_name(); }
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_other_type(*this); }
    const IType* visit_children(TypeVisitor&) const override { return this; }
};

// ---- special and unknown types -------------------------------------------------------------------------------------------

class SpecialType final : public AbstractType {
public:
    static const SpecialType& unknown_type();          // "?"
    static const SpecialType& null_type();             // the type of null
    static const SpecialType& no_type();               // no type at all ("?")
    static const SpecialType& dynamic();
    static const SpecialType& nint();
    static const SpecialType& nuint();
    static const SpecialType& arg_list();              // __arglist
    static const SpecialType& unbound_type_argument();  // the missing argument of an unbound generic type ("")

    std::string name() const override { return name_; }
    TypeKind kind() const override { return kind_; }
    std::optional<bool> is_reference_type() const override { return is_reference_type_; }
    // Special types are equal when their kinds are.
    bool equals(const IType* other) const override;
    int hash_code() const override { return 81625621 ^ static_cast<int>(kind_); }
    const IType* change_nullability(Nullability nullability) const override;

    SpecialType(TypeKind kind, std::string name, std::optional<bool> is_reference_type)
        : kind_(kind), name_(std::move(name)), is_reference_type_(is_reference_type) {}

private:
    TypeKind kind_;
    std::string name_;
    std::optional<bool> is_reference_type_;
};

// A type the compilation does not have: its name, as far as it is known.
class UnknownType final : public AbstractType, public virtual ITypeDefinitionOrUnknown {
public:
    // A null namespace (nullopt) is not known.
    UnknownType(std::optional<std::string> namespace_name, std::string name, int type_parameter_count = 0, std::optional<bool> is_reference_type = std::nullopt);
    explicit UnknownType(const FullTypeName& full_type_name, std::optional<bool> is_reference_type = std::nullopt);

    TypeKind kind() const override { return TypeKind::Unknown; }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return this; }
    std::string name() const override { return full_type_name_.name(); }
    std::string namespace_() const override { return full_type_name_.top_level_type_name().namespace_(); }
    std::string reflection_name() const override { return namespace_known_ ? full_type_name_.reflection_name() : "?"; }
    std::string full_name() const override { return namespace_known_ ? full_type_name_.full_name() : "?"; }
    FullTypeName full_type_name() const override { return full_type_name_; }
    int type_parameter_count() const override { return full_type_name_.type_parameter_count(); }
    const std::vector<const ITypeParameter*>& type_parameters() const override;
    const std::vector<const IType*>& type_arguments() const override;
    std::optional<bool> is_reference_type() const override { return is_reference_type_; }
    const IType* change_nullability(Nullability nullability) const override;
    int hash_code() const override;
    bool equals(const IType* other) const override;
    std::string to_string() const override { return "[UnknownType " + full_type_name_.reflection_name() + "]"; }

private:
    bool namespace_known_;
    FullTypeName full_type_name_;
    std::optional<bool> is_reference_type_;
};

// ---- types with an element type ------------------------------------------------------------------------------------------

class TypeWithElementType : public AbstractType {
public:
    std::string name() const override { return element_type_->name() + name_suffix(); }
    std::string namespace_() const override { return element_type_->namespace_(); }
    std::string full_name() const override { return element_type_->full_name() + name_suffix(); }
    std::string reflection_name() const override { return element_type_->reflection_name() + name_suffix(); }
    std::string to_string() const override { return element_type_->to_string() + name_suffix(); }
    virtual std::string name_suffix() const = 0;
    const IType* element_type() const { return element_type_; }

protected:
    explicit TypeWithElementType(const IType* element_type);
    const IType* element_type_;
};

class ArrayType final : public TypeWithElementType, public virtual ICompilationProvider {
public:
    ArrayType(const ICompilation& compilation, const IType* element_type, int dimensions, Nullability nullability);
    TypeKind kind() const override { return TypeKind::Array; }
    const ICompilation& compilation() const override { return *compilation_; }
    int dimensions() const { return dimensions_; }
    Nullability nullability() const override { return nullability_; }
    const IType* change_nullability(Nullability nullability) const override;
    std::string name_suffix() const override { return "[" + std::string(static_cast<std::size_t>(dimensions_ - 1), ',') + "]"; }
    std::optional<bool> is_reference_type() const override { return true; }
    int hash_code() const override;
    bool equals(const IType* other) const override;
    std::string to_string() const override;
    // System.Array, and IList<T> and IReadOnlyList<T> for a one-dimensional array of a non-pointer.
    std::vector<const IType*> direct_base_types() const override;
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter = {},
                                            GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_array_type(*this); }
    const IType* visit_children(TypeVisitor& visitor) const override;

private:
    const ICompilation* compilation_;
    int dimensions_;
    Nullability nullability_;
};

class PointerType final : public TypeWithElementType {
public:
    explicit PointerType(const IType* element_type) : TypeWithElementType(element_type) {}
    TypeKind kind() const override { return TypeKind::Pointer; }
    std::string name_suffix() const override { return "*"; }
    std::optional<bool> is_reference_type() const override { return std::nullopt; }
    int hash_code() const override { return element_type_->hash_code() ^ 91725811; }
    bool equals(const IType* other) const override;
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_pointer_type(*this); }
    const IType* visit_children(TypeVisitor& visitor) const override;
};

class ByReferenceType final : public TypeWithElementType {
public:
    explicit ByReferenceType(const IType* element_type) : TypeWithElementType(element_type) {}
    TypeKind kind() const override { return TypeKind::ByReference; }
    std::string name_suffix() const override { return "&"; }
    std::optional<bool> is_reference_type() const override { return std::nullopt; }
    bool is_by_ref_like() const override { return true; }
    int hash_code() const override { return element_type_->hash_code() ^ 91725813; }
    bool equals(const IType* other) const override;
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_by_reference_type(*this); }
    const IType* visit_children(TypeVisitor& visitor) const override;
};

// modreq(...) / modopt(...): a type with a custom modifier, which otherwise behaves as the type it modifies.
class ModifiedType final : public TypeWithElementType {
public:
    ModifiedType(const IType* modifier, const IType* unmodified, bool is_required);
    const IType* modifier() const { return modifier_; }
    TypeKind kind() const override { return kind_; }
    std::string name_suffix() const override { return std::string(kind_ == TypeKind::ModReq ? " modreq" : " modopt") + "(" + modifier_->full_name() + ")"; }
    std::optional<bool> is_reference_type() const override { return element_type_->is_reference_type(); }
    bool is_by_ref_like() const override { return element_type_->is_by_ref_like(); }
    Nullability nullability() const override { return element_type_->nullability(); }
    const IType* change_nullability(Nullability nullability) const override;
    const ITypeDefinition* get_definition() const override { return element_type_->get_definition(); }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return element_type_->get_definition_or_unknown(); }
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return element_type_->get_accessors(f, o); }
    std::vector<const IMethod*> get_constructors(const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::IgnoreInheritedMembers) const override {
        return element_type_->get_constructors(f, o);
    }
    std::vector<const IEvent*> get_events(const Filter<IEvent>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return element_type_->get_events(f, o); }
    std::vector<const IField*> get_fields(const Filter<IField>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return element_type_->get_fields(f, o); }
    std::vector<const IMember*> get_members(const Filter<IMember>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return element_type_->get_members(f, o); }
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>& a, const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return element_type_->get_methods(a, f, o);
    }
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return element_type_->get_methods(f, o); }
    std::vector<const IType*> get_nested_types(const std::vector<const IType*>& a, const Filter<ITypeDefinition>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return element_type_->get_nested_types(a, f, o);
    }
    std::vector<const IType*> get_nested_types(const Filter<ITypeDefinition>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return element_type_->get_nested_types(f, o);
    }
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return element_type_->get_properties(f, o); }
    const IType* visit_children(TypeVisitor& visitor) const override;
    const IType* accept_visitor(TypeVisitor& visitor) const override { return kind_ == TypeKind::ModReq ? visitor.visit_mod_req(*this) : visitor.visit_mod_opt(*this); }
    bool equals(const IType* other) const override;
    int hash_code() const override;

private:
    TypeKind kind_;
    const IType* modifier_;
};

// ---- generic instances ---------------------------------------------------------------------------------------------------

class ParameterizedType final : public virtual IType {
public:
    // UsageError for no type arguments, or a count that is not the generic type's.
    ParameterizedType(const IType* generic_type, std::vector<const IType*> type_arguments);
    TypeKind kind() const override { return generic_type_->kind(); }
    const IType* generic_type() const { return generic_type_; }
    std::optional<bool> is_reference_type() const override { return generic_type_->is_reference_type(); }
    bool is_by_ref_like() const override { return generic_type_->is_by_ref_like(); }
    Nullability nullability() const override { return generic_type_->nullability(); }
    const IType* change_nullability(Nullability nullability) const override;
    // The declaring type, with the leading type arguments when it is generic.
    const IType* declaring_type() const override;
    int type_parameter_count() const override { return static_cast<int>(type_arguments_.size()); }
    std::string full_name() const override { return generic_type_->full_name(); }
    std::string name() const override { return generic_type_->name(); }
    std::string namespace_() const override { return generic_type_->namespace_(); }
    std::string reflection_name() const override;   // List`1[[System.Int32]]
    std::string to_string() const override;
    const std::vector<const IType*>& type_arguments() const override { return type_arguments_; }
    const IType* get_type_argument(int index) const { return type_arguments_.at(static_cast<std::size_t>(index)); }
    const std::vector<const ITypeParameter*>& type_parameters() const override { return generic_type_->type_parameters(); }
    const ITypeDefinition* get_definition() const override { return generic_type_->get_definition(); }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return generic_type_->get_definition_or_unknown(); }
    TypeParameterSubstitution get_substitution() const override;
    TypeParameterSubstitution get_substitution(const std::optional<std::vector<const IType*>>& method_type_arguments) const;
    std::vector<const IType*> direct_base_types() const override;
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
    bool equals(const IType* other) const override;
    int hash_code() const override;
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_parameterized_type(*this); }
    const IType* visit_children(TypeVisitor& visitor) const override;

private:
    const IType* generic_type_;
    std::vector<const IType*> type_arguments_;
};

// ---- decorated types -----------------------------------------------------------------------------------------------------

// A type that behaves as another one, but for what it changes.
class DecoratedType : public virtual IType {
public:
    TypeKind kind() const override { return base_type_->kind(); }
    std::optional<bool> is_reference_type() const override { return base_type_->is_reference_type(); }
    bool is_by_ref_like() const override { return base_type_->is_by_ref_like(); }
    Nullability nullability() const override { return base_type_->nullability(); }
    const IType* declaring_type() const override { return base_type_->declaring_type(); }
    int type_parameter_count() const override { return base_type_->type_parameter_count(); }
    const std::vector<const ITypeParameter*>& type_parameters() const override { return base_type_->type_parameters(); }
    const std::vector<const IType*>& type_arguments() const override { return base_type_->type_arguments(); }
    std::vector<const IType*> direct_base_types() const override { return base_type_->direct_base_types(); }
    std::string full_name() const override { return base_type_->full_name(); }
    std::string name() const override { return base_type_->name(); }
    std::string reflection_name() const override { return base_type_->reflection_name(); }
    std::string namespace_() const override { return base_type_->namespace_(); }
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return base_type_->get_accessors(f, o); }
    std::vector<const IMethod*> get_constructors(const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::IgnoreInheritedMembers) const override {
        return base_type_->get_constructors(f, o);
    }
    const ITypeDefinition* get_definition() const override { return base_type_->get_definition(); }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return base_type_->get_definition_or_unknown(); }
    std::vector<const IEvent*> get_events(const Filter<IEvent>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return base_type_->get_events(f, o); }
    std::vector<const IField*> get_fields(const Filter<IField>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return base_type_->get_fields(f, o); }
    std::vector<const IMember*> get_members(const Filter<IMember>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return base_type_->get_members(f, o); }
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return base_type_->get_methods(f, o); }
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>& a, const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return base_type_->get_methods(a, f, o);
    }
    std::vector<const IType*> get_nested_types(const Filter<ITypeDefinition>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return base_type_->get_nested_types(f, o);
    }
    std::vector<const IType*> get_nested_types(const std::vector<const IType*>& a, const Filter<ITypeDefinition>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return base_type_->get_nested_types(a, f, o);
    }
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return base_type_->get_properties(f, o); }
    TypeParameterSubstitution get_substitution() const override;
    int hash_code() const override;

protected:
    explicit DecoratedType(const IType* base_type) : base_type_(base_type) {}
    const IType* base_type_;
};

// T? or T! of a reference type (or dynamic, an unknown type, or a type parameter).
class NullabilityAnnotatedType : public DecoratedType {
public:
    NullabilityAnnotatedType(const IType* type, Nullability nullability);
    Nullability nullability() const override { return nullability_; }
    const IType* type_without_annotation() const { return base_type_; }
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_nullability_annotated_type(*this); }
    bool equals(const IType* other) const override;
    const IType* change_nullability(Nullability nullability) const override;
    const IType* visit_children(TypeVisitor& visitor) const override;
    std::string to_string() const override;

private:
    Nullability nullability_;
};

class NullabilityAnnotatedTypeParameter final : public NullabilityAnnotatedType, public virtual ITypeParameter {
public:
    NullabilityAnnotatedTypeParameter(const ITypeParameter* type, Nullability nullability);
    const ITypeParameter* original_type_parameter() const { return type_parameter_; }
    std::string name() const override { return type_parameter_->name(); }
    SymbolKind symbol_kind() const override { return SymbolKind::TypeParameter; }
    SymbolKind owner_type() const override { return type_parameter_->owner_type(); }
    const IEntity* owner() const override { return type_parameter_->owner(); }
    int index() const override { return type_parameter_->index(); }
    std::vector<const IAttribute*> get_attributes() const override { return type_parameter_->get_attributes(); }
    VarianceModifier variance() const override { return type_parameter_->variance(); }
    const IType* effective_base_class() const override { return type_parameter_->effective_base_class(); }
    const std::vector<const IType*>& effective_interface_set() const override { return type_parameter_->effective_interface_set(); }
    bool has_default_constructor_constraint() const override { return type_parameter_->has_default_constructor_constraint(); }
    bool has_reference_type_constraint() const override { return type_parameter_->has_reference_type_constraint(); }
    bool has_value_type_constraint() const override { return type_parameter_->has_value_type_constraint(); }
    bool has_unmanaged_constraint() const override { return type_parameter_->has_unmanaged_constraint(); }
    bool allows_ref_like_type() const override { return type_parameter_->allows_ref_like_type(); }
    Nullability nullability_constraint() const override { return type_parameter_->nullability_constraint(); }
    const std::vector<TypeConstraint>& type_constraints() const override { return type_parameter_->type_constraints(); }

private:
    const ITypeParameter* type_parameter_;
};

// ---- type parameters -----------------------------------------------------------------------------------------------------

class AbstractTypeParameter : public virtual ITypeParameter, public virtual ICompilationProvider {
public:
    SymbolKind symbol_kind() const override { return SymbolKind::TypeParameter; }
    SymbolKind owner_type() const override { return owner_type_; }
    const IEntity* owner() const override { return owner_; }
    int index() const override { return index_; }
    VarianceModifier variance() const override { return variance_; }
    const ICompilation& compilation() const override { return *compilation_; }
    const IType* effective_base_class() const override;
    const std::vector<const IType*>& effective_interface_set() const override;
    TypeKind kind() const override { return TypeKind::TypeParameter; }
    std::optional<bool> is_reference_type() const override;
    bool is_by_ref_like() const override { return false; }
    Nullability nullability() const override { return Nullability::Oblivious; }
    const IType* change_nullability(Nullability nullability) const override;
    const IType* declaring_type() const override { return nullptr; }
    int type_parameter_count() const override { return 0; }
    const std::vector<const ITypeParameter*>& type_parameters() const override;
    const std::vector<const IType*>& type_arguments() const override;
    std::vector<const IType*> direct_base_types() const override;
    std::string name() const override { return name_; }
    std::string namespace_() const override { return {}; }
    std::string full_name() const override { return name_; }
    std::string reflection_name() const override;   // `0 or ``0
    const ITypeDefinition* get_definition() const override { return nullptr; }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return nullptr; }
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_type_parameter(*this); }
    const IType* visit_children(TypeVisitor&) const override { return this; }
    std::vector<const IType*> get_nested_types(const Filter<ITypeDefinition>& = {}, GetMemberOptions = GetMemberOptions::None) const override { return {}; }
    std::vector<const IType*> get_nested_types(const std::vector<const IType*>&, const Filter<ITypeDefinition>& = {},
                                               GetMemberOptions = GetMemberOptions::None) const override {
        return {};
    }
    std::vector<const IMethod*> get_constructors(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::IgnoreInheritedMembers) const override;
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter = {},
                                            GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IField*> get_fields(const Filter<IField>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IEvent*> get_events(const Filter<IEvent>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMember*> get_members(const Filter<IMember>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    TypeParameterSubstitution get_substitution() const override;
    bool equals(const IType* other) const override { return this == other; }   // type parameters compare by reference
    int hash_code() const override;
    std::string to_string() const override { return reflection_name(); }

protected:
    AbstractTypeParameter(const IEntity* owner, int index, std::optional<std::string> name, VarianceModifier variance);
    AbstractTypeParameter(const ICompilation& compilation, SymbolKind owner_type, int index, std::optional<std::string> name, VarianceModifier variance);

private:
    const IType* calculate_effective_base_class() const;
    std::vector<const IType*> calculate_effective_interface_set() const;

    const ICompilation* compilation_;
    SymbolKind owner_type_;
    const IEntity* owner_ = nullptr;
    int index_;
    std::string name_;
    VarianceModifier variance_;
    mutable std::atomic<const IType*> effective_base_class_{nullptr};
    mutable std::once_flag interfaces_once_;
    mutable std::vector<const IType*> effective_interface_set_;
    mutable std::atomic<bool> interfaces_ready_{false};
};

class DefaultTypeParameter final : public AbstractTypeParameter {
public:
    DefaultTypeParameter(const IEntity* owner, int index, std::optional<std::string> name = std::nullopt, VarianceModifier variance = VarianceModifier::Invariant,
                         std::vector<const IAttribute*> attributes = {}, bool has_value_type_constraint = false, bool has_reference_type_constraint = false,
                         bool has_default_constructor_constraint = false, const std::vector<const IType*>& constraints = {},
                         Nullability nullability_constraint = Nullability::Oblivious);
    DefaultTypeParameter(const ICompilation& compilation, SymbolKind owner_type, int index, std::optional<std::string> name = std::nullopt,
                         VarianceModifier variance = VarianceModifier::Invariant, std::vector<const IAttribute*> attributes = {},
                         bool has_value_type_constraint = false, bool has_reference_type_constraint = false, bool has_default_constructor_constraint = false,
                         const std::vector<const IType*>& constraints = {}, Nullability nullability_constraint = Nullability::Oblivious);
    std::vector<const IAttribute*> get_attributes() const override { return attributes_; }
    bool has_value_type_constraint() const override { return has_value_type_constraint_; }
    bool has_reference_type_constraint() const override { return has_reference_type_constraint_; }
    bool has_default_constructor_constraint() const override { return has_default_constructor_constraint_; }
    bool has_unmanaged_constraint() const override { return false; }
    bool allows_ref_like_type() const override { return false; }
    Nullability nullability_constraint() const override { return nullability_constraint_; }
    const std::vector<TypeConstraint>& type_constraints() const override { return type_constraints_; }

private:
    void make_constraints(const std::vector<const IType*>& constraints);
    bool has_value_type_constraint_, has_reference_type_constraint_, has_default_constructor_constraint_;
    Nullability nullability_constraint_;
    std::vector<const IAttribute*> attributes_;
    std::vector<TypeConstraint> type_constraints_;
};

// The type parameters of no entity, by position (!0, !!0 ...): what normalized and unknown types use.
class DummyTypeParameter final : public AbstractType, public virtual ITypeParameter {
public:
    static const ITypeParameter* get_method_type_parameter(int index);
    static const ITypeParameter* get_class_type_parameter(int index);
    static const std::vector<const ITypeParameter*>& get_class_type_parameter_list(int length);

    SymbolKind symbol_kind() const override { return SymbolKind::TypeParameter; }
    std::string name() const override;
    std::string reflection_name() const override;
    std::string to_string() const override { return reflection_name() + " (dummy)"; }
    std::optional<bool> is_reference_type() const override { return std::nullopt; }
    TypeKind kind() const override { return TypeKind::TypeParameter; }
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_type_parameter(*this); }
    int index() const override { return index_; }
    std::vector<const IAttribute*> get_attributes() const override { return {}; }
    SymbolKind owner_type() const override { return owner_type_; }
    VarianceModifier variance() const override { return VarianceModifier::Invariant; }
    const IEntity* owner() const override { return nullptr; }
    const IType* effective_base_class() const override;
    const std::vector<const IType*>& effective_interface_set() const override;
    bool has_default_constructor_constraint() const override { return false; }
    bool has_reference_type_constraint() const override { return false; }
    bool has_value_type_constraint() const override { return false; }
    bool has_unmanaged_constraint() const override { return false; }
    bool allows_ref_like_type() const override { return false; }
    Nullability nullability_constraint() const override { return Nullability::Oblivious; }
    const std::vector<TypeConstraint>& type_constraints() const override;
    const IType* change_nullability(Nullability nullability) const override;

    DummyTypeParameter(SymbolKind owner_type, int index) : owner_type_(owner_type), index_(index) {}

private:
    SymbolKind owner_type_;
    int index_;
};

// A & B: the type a value of several constraint types has.
class IntersectionType final : public AbstractType {
public:
    // The distinct types; Unknown for none, the type itself for one.
    static const IType* create(const std::vector<const IType*>& types);
    const std::vector<const IType*>& types() const { return types_; }
    TypeKind kind() const override { return TypeKind::Intersection; }
    std::string name() const override;
    std::string reflection_name() const override;
    std::optional<bool> is_reference_type() const override;
    int hash_code() const override;
    bool equals(const IType* other) const override;
    std::vector<const IType*> direct_base_types() const override { return types_; }
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>& type_arguments, const Filter<IMethod>& filter = {},
                                            GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IField*> get_fields(const Filter<IField>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IEvent*> get_events(const Filter<IEvent>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMember*> get_members(const Filter<IMember>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& filter = {}, GetMemberOptions options = GetMemberOptions::None) const override;

    explicit IntersectionType(std::vector<const IType*> types) : types_(std::move(types)) {}

private:
    std::vector<const IType*> types_;
};

// ---- the factories (upstream's constructors) ------------------------------------------------------------------------------

// The compilation a type belongs to (nullptr for one made only of special types).
const ICompilation* compilation_of(const IType* type);
// The arena a type made of these parts lives in.
TypeArena& arena_for(std::initializer_list<const IType*> parts);

const IType* make_array_type(const ICompilation& compilation, const IType* element_type, int dimensions = 1, Nullability nullability = Nullability::Oblivious);
const IType* make_pointer_type(const IType* element_type);
const IType* make_by_reference_type(const IType* element_type);
const IType* make_modified_type(const IType* modifier, const IType* unmodified, bool is_required);
const IType* make_parameterized_type(const IType* generic_type, std::vector<const IType*> type_arguments);
const IType* make_nullability_annotated_type(const IType* type, Nullability nullability);

// ---- helpers (NullableType, NormalizeTypeVisitor) ------------------------------------------------------------------------

namespace nullable_type {
bool is_nullable(const IType* type);   // Nullable<T>
bool is_non_nullable_value_type(const IType* type);
const IType* get_underlying_type(const IType* type);
const IType* create(const ICompilation& compilation, const IType* element_type);
}

class NormalizeTypeVisitor final : public TypeVisitor {
public:
    static NormalizeTypeVisitor& type_erasure();
    static NormalizeTypeVisitor& ignore_nullability_and_tuples();
    static NormalizeTypeVisitor& ignore_nullability();

    bool equivalent_types(const IType* a, const IType* b);
    bool remove_mod_opt = true;
    bool remove_mod_req = true;
    bool replace_class_type_parameters_with_dummy = true;
    bool replace_method_type_parameters_with_dummy = true;
    bool dynamic_and_object = true;
    bool int_ptr_to_nint = true;
    bool tuple_to_underlying_type = true;
    bool remove_nullability = true;

    const IType* visit_type_parameter(const ITypeParameter& type) override;
    const IType* visit_type_definition(const ITypeDefinition& type) override;
    const IType* visit_tuple_type(const TupleType& type) override;
    const IType* visit_nullability_annotated_type(const NullabilityAnnotatedType& type) override;
    const IType* visit_array_type(const ArrayType& type) override;
    const IType* visit_mod_opt(const ModifiedType& type) override;
    const IType* visit_mod_req(const ModifiedType& type) override;
};

// Upstream's BusyManager: a guard against recursing into the same object (cyclic constraints, cyclic base types) on one thread.
class BusyLock {
public:
    explicit BusyLock(const void* object);
    ~BusyLock();
    BusyLock(const BusyLock&) = delete;
    BusyLock& operator=(const BusyLock&) = delete;
    bool success() const { return success_; }

private:
    bool success_;
};

}  // namespace cb::dotnet::decompiler::typesystem
