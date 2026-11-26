// dotnet/decompiler/typesystem/fake_member.hpp -- members the decompiler makes up where the metadata has none: the default constructor a
// type parameter with a new() or struct constraint implies, and the fields, methods, properties and events transforms create.
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem.Implementation: FakeMember, FakeField, FakeMethod, FakeProperty, FakeEvent), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream sets their properties after construction; here they
// are public members set before the member is handed out. A fake member is kept by its compilation's arena.
#pragma once

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem {

class FakeMember : public virtual IMember {
public:
    const IMember* member_definition() const override { return this; }
    const IType* return_type() const override { return return_type_value; }
    std::vector<const IMember*> explicitly_implemented_interface_members() const override { return {}; }
    bool is_explicit_interface_implementation() const override { return false; }
    bool is_virtual() const override { return false; }
    bool is_override() const override { return false; }
    bool is_overridable() const override { return false; }
    const TypeParameterSubstitution& substitution() const override { return TypeParameterSubstitution::identity(); }
    srm::EntityHandle metadata_token() const override { return {}; }
    std::string name() const override { return name_value; }
    const ITypeDefinition* declaring_type_definition() const override { return declaring_type_value != nullptr ? declaring_type_value->get_definition() : nullptr; }
    const IType* declaring_type() const override { return declaring_type_value; }
    const IModule* parent_module() const override;
    std::vector<const IAttribute*> get_attributes() const override { return {}; }
    bool has_attribute(KnownAttribute) const override { return false; }
    const IAttribute* get_attribute(KnownAttribute) const override { return nullptr; }
    Accessibility accessibility() const override { return accessibility_value; }
    bool is_static() const override { return is_static_value; }
    bool is_abstract() const override { return false; }
    bool is_sealed() const override { return false; }
    const ICompilation& compilation() const override { return *compilation_; }
    std::string full_name() const override;
    std::string reflection_name() const override;
    std::string namespace_() const override { return declaring_type_value != nullptr ? declaring_type_value->namespace_() : std::string(); }
    bool equals(const IMember* other, TypeVisitor&) const override { return equals(other); }
    bool equals(const IMember* other) const override { return this == other; }
    int hash_code() const override;
    std::string to_string() const override { return full_name(); }

    const IType* return_type_value = &SpecialType::unknown_type();
    std::string name_value;
    const IType* declaring_type_value = nullptr;
    Accessibility accessibility_value = Accessibility::Public;
    bool is_static_value = false;

protected:
    explicit FakeMember(const ICompilation& compilation) : compilation_(&compilation) {}

private:
    const ICompilation* compilation_;
};

class FakeField final : public FakeMember, public virtual IField {
public:
    explicit FakeField(const ICompilation& compilation) : FakeMember(compilation) {}
    std::string name() const override { return name_value; }
    bool is_read_only() const override { return false; }
    bool return_type_is_ref_read_only() const override { return false; }
    bool is_volatile() const override { return false; }
    bool is_const() const override { return false; }
    ConstantValue get_constant_value(bool = false) const override { return std::monostate{}; }
    const IType* type() const override { return return_type_value; }
    SymbolKind symbol_kind() const override { return SymbolKind::Field; }
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override;
};

class FakeMethod final : public FakeMember, public virtual IMethod {
public:
    FakeMethod(const ICompilation& compilation, SymbolKind symbol_kind) : FakeMember(compilation), symbol_kind_(symbol_kind) {}
    SymbolKind symbol_kind() const override { return symbol_kind_; }
    std::vector<const IAttribute*> get_return_type_attributes() const override { return {}; }
    bool return_type_is_ref_read_only() const override { return false; }
    bool this_is_ref_read_only() const override { return false; }
    bool is_init_only() const override { return false; }
    const std::vector<const ITypeParameter*>& type_parameters() const override { return type_parameters_value; }
    const std::vector<const IType*>& type_arguments() const override;
    bool is_extension_method() const override { return false; }
    bool is_local_function() const override { return false; }
    bool is_constructor() const override { return symbol_kind_ == SymbolKind::Constructor; }
    bool is_destructor() const override { return symbol_kind_ == SymbolKind::Destructor; }
    bool is_operator() const override { return symbol_kind_ == SymbolKind::Operator; }
    bool has_body() const override { return false; }
    bool is_accessor() const override { return accessor_owner_value != nullptr; }
    const IMember* accessor_owner() const override { return accessor_owner_value; }
    MethodSemanticsAttributes accessor_kind() const override { return accessor_kind_value; }
    const IMethod* reduced_from() const override { return nullptr; }
    const std::vector<const IParameter*>& parameters() const override { return parameters_value; }
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override;
    const IMethod* specialize_method(const TypeParameterSubstitution& substitution) const override;

    std::vector<const ITypeParameter*> type_parameters_value;
    const IMember* accessor_owner_value = nullptr;
    MethodSemanticsAttributes accessor_kind_value = MethodSemanticsAttributes::None;
    std::vector<const IParameter*> parameters_value;

private:
    SymbolKind symbol_kind_;
    mutable std::once_flag type_arguments_once_;
    mutable std::vector<const IType*> type_arguments_;
};

class FakeProperty final : public FakeMember, public virtual IProperty {
public:
    explicit FakeProperty(const ICompilation& compilation) : FakeMember(compilation) {}
    SymbolKind symbol_kind() const override { return is_indexer_value ? SymbolKind::Indexer : SymbolKind::Property; }
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override;
    bool can_get() const override { return getter_value != nullptr; }
    bool can_set() const override { return setter_value != nullptr; }
    const IMethod* getter() const override { return getter_value; }
    const IMethod* setter() const override { return setter_value; }
    bool is_indexer() const override { return is_indexer_value; }
    bool return_type_is_ref_read_only() const override { return false; }
    const std::vector<const IParameter*>& parameters() const override { return parameters_value; }
    std::string to_string() const override;

    const IMethod* getter_value = nullptr;
    const IMethod* setter_value = nullptr;
    bool is_indexer_value = false;
    std::vector<const IParameter*> parameters_value;
};

class FakeEvent final : public FakeMember, public virtual IEvent {
public:
    explicit FakeEvent(const ICompilation& compilation) : FakeMember(compilation) {}
    SymbolKind symbol_kind() const override { return SymbolKind::Event; }
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override;
    bool can_add() const override { return add_accessor_value != nullptr; }
    bool can_remove() const override { return remove_accessor_value != nullptr; }
    bool can_invoke() const override { return invoke_accessor_value != nullptr; }
    const IMethod* add_accessor() const override { return add_accessor_value; }
    const IMethod* remove_accessor() const override { return remove_accessor_value; }
    const IMethod* invoke_accessor() const override { return invoke_accessor_value; }

    const IMethod* add_accessor_value = nullptr;
    const IMethod* remove_accessor_value = nullptr;
    const IMethod* invoke_accessor_value = nullptr;
};

// FakeMethod.CreateDummyConstructor: a public parameterless .ctor of `declaring_type`, kept by the compilation's arena.
const IMethod* create_dummy_constructor(const ICompilation& compilation, const IType* declaring_type, Accessibility accessibility = Accessibility::Public);

}  // namespace cb::dotnet::decompiler::typesystem
