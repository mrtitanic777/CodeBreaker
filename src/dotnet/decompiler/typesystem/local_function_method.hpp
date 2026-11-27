// dotnet/decompiler/typesystem/local_function_method.hpp -- the view of a compiler-generated method as the C# 7 local function it was:
// the same method without the compiler-generated closure parameters at the end (and without the type parameters the compiler added in
// front), under the name the local function is declared with. Always static (a local function has no `this`; a local function in an
// instance method captures it).
//
//     const LocalFunctionMethod* reduced = LocalFunctionMethod::create(method, "Add", false, 1, 0);
//     reduced->parameters();                 // method's parameters without the last one
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem.Implementation: LocalFunctionMethod, from LocalFunctionMethod.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The made method is kept by the base method's compilation's
// arena (upstream's `new` is garbage collected); its name stays settable, as upstream's.
#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem {

class LocalFunctionMethod final : public virtual IMethod {
public:
    // The constructor, with the method kept by the arena of the base method's compilation.
    static LocalFunctionMethod* create(const IMethod* base_method, std::string name, bool is_static_local_function,
                                       int number_of_compiler_generated_parameters, int number_of_compiler_generated_type_parameters);
    LocalFunctionMethod(const IMethod* base_method, std::string name, bool is_static_local_function, int number_of_compiler_generated_parameters,
                        int number_of_compiler_generated_type_parameters);

    int number_of_compiler_generated_parameters() const { return number_of_compiler_generated_parameters_; }
    int number_of_compiler_generated_type_parameters() const { return number_of_compiler_generated_type_parameters_; }
    bool is_static_local_function() const { return is_static_local_function_; }
    // Upstream's `Name { set; }`.
    void set_name(std::string value) const { name_ = std::move(value); }

    // INamedElement, ISymbol
    std::string full_name() const override { return name_; }
    std::string name() const override { return name_; }
    std::string reflection_name() const override { return base_method_->reflection_name(); }
    std::string namespace_() const override { return base_method_->namespace_(); }
    SymbolKind symbol_kind() const override { return base_method_->symbol_kind(); }
    const ICompilation& compilation() const override { return base_method_->compilation(); }

    // IEntity
    srm::EntityHandle metadata_token() const override { return base_method_->metadata_token(); }
    const ITypeDefinition* declaring_type_definition() const override { return base_method_->declaring_type_definition(); }
    const IType* declaring_type() const override { return base_method_->declaring_type(); }
    const IModule* parent_module() const override { return base_method_->parent_module(); }
    std::vector<const IAttribute*> get_attributes() const override { return base_method_->get_attributes(); }
    bool has_attribute(KnownAttribute attribute) const override { return base_method_->has_attribute(attribute); }
    const IAttribute* get_attribute(KnownAttribute attribute) const override { return base_method_->get_attribute(attribute); }
    Accessibility accessibility() const override { return base_method_->accessibility(); }
    // We consider local functions as always static, because they do not have a "this parameter". Even local functions in instance
    // methods capture this.
    bool is_static() const override { return true; }
    bool is_abstract() const override { return base_method_->is_abstract(); }
    bool is_sealed() const override { return base_method_->is_sealed(); }

    // IMember
    const IMember* member_definition() const override;
    const IType* return_type() const override { return base_method_->return_type(); }
    std::vector<const IMember*> explicitly_implemented_interface_members() const override { return base_method_->explicitly_implemented_interface_members(); }
    bool is_explicit_interface_implementation() const override { return base_method_->is_explicit_interface_implementation(); }
    bool is_virtual() const override { return base_method_->is_virtual(); }
    bool is_override() const override { return base_method_->is_override(); }
    bool is_overridable() const override { return base_method_->is_overridable(); }
    const TypeParameterSubstitution& substitution() const override { return base_method_->substitution(); }
    const IMember* specialize(const TypeParameterSubstitution& substitution) const override { return specialize_method(substitution); }
    bool equals(const IMember* other, TypeVisitor& type_normalization) const override;
    bool equals(const IMember* other) const override;
    int hash_code() const override { return base_method_->hash_code(); }
    std::string to_string() const override;

    // IParameterizedMember
    const std::vector<const IParameter*>& parameters() const override;

    // IMethod
    std::vector<const IAttribute*> get_return_type_attributes() const override { return base_method_->get_return_type_attributes(); }
    bool return_type_is_ref_read_only() const override { return base_method_->return_type_is_ref_read_only(); }
    bool is_init_only() const override { return base_method_->is_init_only(); }
    bool this_is_ref_read_only() const override { return base_method_->this_is_ref_read_only(); }
    const std::vector<const ITypeParameter*>& type_parameters() const override;
    const std::vector<const IType*>& type_arguments() const override;
    bool is_extension_method() const override { return base_method_->is_extension_method(); }
    bool is_local_function() const override { return true; }
    bool is_constructor() const override { return base_method_->is_constructor(); }
    bool is_destructor() const override { return base_method_->is_destructor(); }
    bool is_operator() const override { return base_method_->is_operator(); }
    bool has_body() const override { return base_method_->has_body(); }
    bool is_accessor() const override { return base_method_->is_accessor(); }
    const IMember* accessor_owner() const override { return base_method_->accessor_owner(); }
    MethodSemanticsAttributes accessor_kind() const override { return base_method_->accessor_kind(); }
    const IMethod* reduced_from() const override { return base_method_; }
    const IMethod* specialize_method(const TypeParameterSubstitution& substitution) const override;

private:
    const IMethod* base_method_;
    mutable std::string name_;
    bool is_static_local_function_;
    int number_of_compiler_generated_parameters_;
    int number_of_compiler_generated_type_parameters_;

    mutable std::once_flag type_parameters_once_;
    mutable std::vector<const ITypeParameter*> type_parameters_;
    mutable std::once_flag type_arguments_once_;
    mutable std::vector<const IType*> type_arguments_;
    mutable std::once_flag parameters_once_;
    mutable std::vector<const IParameter*> parameters_;
};

}  // namespace cb::dotnet::decompiler::typesystem
