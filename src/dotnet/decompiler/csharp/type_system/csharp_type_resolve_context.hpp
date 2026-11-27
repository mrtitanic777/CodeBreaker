// dotnet/decompiler/csharp/type_system/csharp_type_resolve_context.hpp -- where C# code is resolved: the module, the using scope, the type
// definition and the member the code is in. It is immutable; the With* members make a changed copy.
//
//     CSharpTypeResolveContext context(compilation.main_module());
//     CSharpTypeResolveContext in_class = context.with_current_type_definition(definition);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.TypeSystem: CSharpTypeResolveContext, from CSharpTypeResolveContext.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream's methodTypeParameterNames field is never set or read
// and is not ported.
#pragma once

#include <stdexcept>

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::csharp::type_system {

class UsingScope;

class CSharpTypeResolveContext final : public typesystem::ITypeResolveContext {
public:
    // The module must not be null (std::invalid_argument).
    explicit CSharpTypeResolveContext(const typesystem::IModule* module, const UsingScope* using_scope = nullptr,
                                      const typesystem::ITypeDefinition* type_definition = nullptr, const typesystem::IMember* member = nullptr)
        : module_(module), current_using_scope_(using_scope), current_type_definition_(type_definition), current_member_(member) {
        if (module == nullptr) throw std::invalid_argument("module");
    }

    const UsingScope* current_using_scope() const { return current_using_scope_; }
    const typesystem::ICompilation& compilation() const override { return module_->compilation(); }
    const typesystem::IModule* current_module() const override { return module_; }
    const typesystem::ITypeDefinition* current_type_definition() const override { return current_type_definition_; }
    const typesystem::IMember* current_member() const override { return current_member_; }

    CSharpTypeResolveContext with_current_type_definition(const typesystem::ITypeDefinition* type_definition) const {
        return CSharpTypeResolveContext(module_, current_using_scope_, type_definition, current_member_);
    }
    CSharpTypeResolveContext with_current_member(const typesystem::IMember* member) const {
        return CSharpTypeResolveContext(module_, current_using_scope_, current_type_definition_, member);
    }
    CSharpTypeResolveContext with_using_scope(const UsingScope* using_scope) const {
        return CSharpTypeResolveContext(module_, using_scope, current_type_definition_, current_member_);
    }

private:
    const typesystem::IModule* module_;
    const UsingScope* current_using_scope_;
    const typesystem::ITypeDefinition* current_type_definition_;
    const typesystem::IMember* current_member_;
};

}  // namespace cb::dotnet::decompiler::csharp::type_system
