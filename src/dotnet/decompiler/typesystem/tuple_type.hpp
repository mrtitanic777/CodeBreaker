// dotnet/decompiler/typesystem/tuple_type.hpp -- C# tuple types, (int x, string y), over their System.ValueTuple types (nested in the 8th
// argument past seven elements); and function pointer types, delegate* unmanaged[Cdecl]<int, void>.
//
//     const TupleType* t = make_tuple_type(compilation, {int32, string}, {"x", "y"});
//     t->underlying_type();        // System.ValueTuple`2[[System.Int32],[System.String]]
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: TupleType, TupleTypeExtensions, FunctionPointerType), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler). A function pointer type holds its compilation and whether its module's type
// system has function pointers (upstream holds the module); FunctionPointerType.FromSignature is with MetadataModule, which it reads.
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/metadata/blob_reader.hpp"

namespace cb::dotnet::decompiler::typesystem {

class TupleType final : public AbstractType, public virtual ICompilationProvider {
public:
    static constexpr int kRestPosition = 8;

    const ICompilation& compilation() const override { return *compilation_; }
    const ParameterizedType* underlying_type() const { return underlying_; }
    const std::vector<const IType*>& element_types() const { return element_types_; }
    int cardinality() const { return static_cast<int>(element_types_.size()); }
    const std::vector<std::optional<std::string>>& element_names() const { return element_names_; }

    // A ValueTuple (or a tuple) and how many elements it has.
    static bool is_tuple_compatible(const IType* type, int& tuple_cardinality);
    // The tuple type of a ValueTuple; nullptr for a type that is not one.
    static const TupleType* from_underlying_type(const ICompilation& compilation, const IType* type);
    // The element types of a ValueTuple or tuple (nested past seven); nullopt for a type that is not one.
    static std::optional<std::vector<const IType*>> get_tuple_element_types(const IType* tuple_type);

    TypeKind kind() const override { return TypeKind::Tuple; }
    std::optional<bool> is_reference_type() const override { return underlying_->is_reference_type(); }
    std::vector<const IType*> direct_base_types() const override { return underlying_->direct_base_types(); }
    std::string full_name() const override { return underlying_->full_name(); }
    std::string name() const override { return underlying_->name(); }
    std::string reflection_name() const override { return underlying_->reflection_name(); }
    std::string namespace_() const override { return underlying_->namespace_(); }
    bool equals(const IType* other) const override;
    int hash_code() const override;
    std::string to_string() const override;
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_tuple_type(*this); }
    const IType* visit_children(TypeVisitor& visitor) const override;
    std::vector<const IMethod*> get_accessors(const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return underlying_->get_accessors(f, o); }
    // (none: "new" cannot be used with a tuple type, CS8181)
    std::vector<const IMethod*> get_constructors(const Filter<IMethod>& = {}, GetMemberOptions = GetMemberOptions::IgnoreInheritedMembers) const override { return {}; }
    const ITypeDefinition* get_definition() const override { return underlying_->get_definition(); }
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override { return underlying_->get_definition_or_unknown(); }
    std::vector<const IEvent*> get_events(const Filter<IEvent>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return underlying_->get_events(f, o); }
    std::vector<const IField*> get_fields(const Filter<IField>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return underlying_->get_fields(f, o); }
    std::vector<const IMethod*> get_methods(const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return underlying_->get_methods(f, o); }
    std::vector<const IMethod*> get_methods(const std::vector<const IType*>& a, const Filter<IMethod>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return underlying_->get_methods(a, f, o);
    }
    std::vector<const IType*> get_nested_types(const Filter<ITypeDefinition>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return underlying_->get_nested_types(f, o);
    }
    std::vector<const IType*> get_nested_types(const std::vector<const IType*>& a, const Filter<ITypeDefinition>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override {
        return underlying_->get_nested_types(a, f, o);
    }
    std::vector<const IProperty*> get_properties(const Filter<IProperty>& f = {}, GetMemberOptions o = GetMemberOptions::None) const override { return underlying_->get_properties(f, o); }

    TupleType(const ICompilation& compilation, std::vector<const IType*> element_types, std::vector<std::optional<std::string>> element_names,
              const IModule* value_tuple_assembly);

private:
    const ICompilation* compilation_;
    const ParameterizedType* underlying_;
    std::vector<const IType*> element_types_;
    std::vector<std::optional<std::string>> element_names_;
};

// A tuple type (one object per element types and names). No names (an empty list) leaves every element unnamed.
const TupleType* make_tuple_type(const ICompilation& compilation, std::vector<const IType*> element_types,
                                 std::vector<std::optional<std::string>> element_names = {}, const IModule* value_tuple_assembly = nullptr);
// A tuple's ValueTuple, without nullability (TupleTypeExtensions.TupleUnderlyingTypeOrSelf).
const IType* tuple_underlying_type_or_self(const IType* type);

class FunctionPointerType final : public AbstractType {
public:
    FunctionPointerType(const ICompilation& compilation, bool function_pointers, srm::SignatureCallingConvention calling_convention,
                        std::vector<const IType*> custom_calling_conventions, const IType* return_type, bool return_is_ref_read_only,
                        std::vector<const IType*> parameter_types, std::vector<ReferenceKind> parameter_reference_kinds);

    srm::SignatureCallingConvention calling_convention;
    std::vector<const IType*> custom_calling_conventions;
    const IType* return_type;
    bool return_is_ref_read_only;
    std::vector<const IType*> parameter_types;
    std::vector<ReferenceKind> parameter_reference_kinds;

    std::string name() const override { return "delegate*"; }
    std::optional<bool> is_reference_type() const override { return false; }
    // FunctionPointer when the type system has function pointers; else a struct that acts as UIntPtr.
    TypeKind kind() const override { return function_pointers_ ? TypeKind::FunctionPointer : TypeKind::Struct; }
    const ITypeDefinition* get_definition() const override;
    const ITypeDefinitionOrUnknown* get_definition_or_unknown() const override;
    const IType* accept_visitor(TypeVisitor& visitor) const override { return visitor.visit_function_pointer_type(*this); }
    const IType* visit_children(TypeVisitor& visitor) const override;
    bool equals(const IType* other) const override;
    int hash_code() const override;
    const IType* with_signature(const IType* return_type, std::vector<const IType*> parameter_types) const;
    const ICompilation& compilation() const { return *compilation_; }
    bool function_pointers() const { return function_pointers_; }

private:
    const ICompilation* compilation_;
    bool function_pointers_;
};

// A function pointer type kept by the compilation's arena (one object per signature).
const FunctionPointerType* make_function_pointer_type(const ICompilation& compilation, bool function_pointers, srm::SignatureCallingConvention calling_convention,
                                                      std::vector<const IType*> custom_calling_conventions, const IType* return_type, bool return_is_ref_read_only,
                                                      std::vector<const IType*> parameter_types, std::vector<ReferenceKind> parameter_reference_kinds);

}  // namespace cb::dotnet::decompiler::typesystem
