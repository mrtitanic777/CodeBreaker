// dotnet/decompiler/typesystem/synthetic_range_index_accessor.hpp -- a compiler-generated indexer accessor as C# 8 sees it: the method
// `get_Item(System.Index)` / `get_Item(System.Range)` (or a setter) that stands for the real `get_Item(int)`, `set_Item(int, T)` or
// `Slice(int, int)` of "implicit index support" and "implicit range support".
//
//     const IMethod* accessor = SyntheticRangeIndexAccessor::create(get_item, index_type, false);
//     accessor->parameters()[0]->type();      // System.Index
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem.Implementation: SyntheticRangeIndexAccessor, from SyntheticRangeIndexer.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The made method is kept by the underlying method's
// compilation's arena (upstream's `new` is garbage collected).
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem {

class SyntheticRangeIndexAccessor final : public virtual IMethod {
public:
    // The constructor, with the method kept by the arena of the underlying method's compilation.
    static SyntheticRangeIndexAccessor* create(const IMethod* underlying_method, const IType* index_or_range_type, bool slicing);
    SyntheticRangeIndexAccessor(const IMethod* underlying_method, const IType* index_or_range_type, bool slicing);

    bool is_slicing() const { return slicing_; }

    // INamedElement, ISymbol
    std::string full_name() const override { return underlying_method_->full_name(); }
    std::string name() const override { return underlying_method_->name(); }
    std::string reflection_name() const override { return underlying_method_->reflection_name(); }
    std::string namespace_() const override { return underlying_method_->namespace_(); }
    SymbolKind symbol_kind() const override { return SymbolKind::Method; }
    const ICompilation& compilation() const override { return underlying_method_->compilation(); }

    // IEntity
    srm::EntityHandle metadata_token() const override { return underlying_method_->metadata_token(); }
    const ITypeDefinition* declaring_type_definition() const override { return underlying_method_->declaring_type_definition(); }
    const IType* declaring_type() const override { return underlying_method_->declaring_type(); }
    const IModule* parent_module() const override { return underlying_method_->parent_module(); }
    std::vector<const IAttribute*> get_attributes() const override { return underlying_method_->get_attributes(); }
    bool has_attribute(KnownAttribute attribute) const override { return underlying_method_->has_attribute(attribute); }
    const IAttribute* get_attribute(KnownAttribute attribute) const override { return underlying_method_->get_attribute(attribute); }
    Accessibility accessibility() const override { return underlying_method_->accessibility(); }
    bool is_static() const override { return underlying_method_->is_static(); }
    bool is_abstract() const override { return underlying_method_->is_abstract(); }
    bool is_sealed() const override { return underlying_method_->is_sealed(); }

    // IMember
    const IMember* member_definition() const override { return underlying_method_->member_definition(); }
    const IType* return_type() const override { return underlying_method_->return_type(); }
    std::vector<const IMember*> explicitly_implemented_interface_members() const override { return {}; }
    bool is_explicit_interface_implementation() const override { return false; }
    bool is_virtual() const override { return underlying_method_->is_virtual(); }
    bool is_override() const override { return underlying_method_->is_override(); }
    bool is_overridable() const override { return underlying_method_->is_overridable(); }
    const TypeParameterSubstitution& substitution() const override { return underlying_method_->substitution(); }
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override { return specialize_method(substitution); }
    bool equals(const IMember* other, TypeVisitor& type_normalization) const override;
    bool equals(const IMember* other) const override;
    int hash_code() const override { return underlying_method_->hash_code() ^ index_or_range_type_->hash_code(); }
    std::string to_string() const override { return underlying_method_->to_string(); }

    // IParameterizedMember
    const std::vector<const IParameter*>& parameters() const override { return parameters_; }

    // IMethod
    std::vector<const IAttribute*> get_return_type_attributes() const override { return underlying_method_->get_return_type_attributes(); }
    bool return_type_is_ref_read_only() const override { return underlying_method_->return_type_is_ref_read_only(); }
    bool is_init_only() const override { return underlying_method_->is_init_only(); }
    bool this_is_ref_read_only() const override { return underlying_method_->this_is_ref_read_only(); }
    const std::vector<const ITypeParameter*>& type_parameters() const override { return no_type_parameters_; }
    const std::vector<const IType*>& type_arguments() const override { return no_type_arguments_; }
    bool is_extension_method() const override { return false; }
    bool is_local_function() const override { return false; }
    bool is_constructor() const override { return false; }
    bool is_destructor() const override { return false; }
    bool is_operator() const override { return false; }
    bool has_body() const override { return underlying_method_->has_body(); }
    bool is_accessor() const override { return underlying_method_->is_accessor(); }
    const IMember* accessor_owner() const override { return underlying_method_->accessor_owner(); }
    MethodSemanticsAttributes accessor_kind() const override { return underlying_method_->accessor_kind(); }
    const IMethod* reduced_from() const override { return underlying_method_->reduced_from(); }
    const IMethod* specialize_method(const TypeParameterSubstitution& substitution) const override;

private:
    // The underlying method: `get_Item(int)`, `set_Item(int, T)` or `Slice(int, int)`.
    const IMethod* underlying_method_;
    const IType* index_or_range_type_;
    bool slicing_;
    std::vector<std::unique_ptr<DefaultParameter>> own_parameters_;
    std::vector<const IParameter*> parameters_;
    std::vector<const ITypeParameter*> no_type_parameters_;
    std::vector<const IType*> no_type_arguments_;
};

}  // namespace cb::dotnet::decompiler::typesystem
