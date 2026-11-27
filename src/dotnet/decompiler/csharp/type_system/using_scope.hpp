// dotnet/decompiler/csharp/type_system/using_scope.hpp -- a scope that contains "using" statements: the module itself or a namespace
// declaration. It knows its namespace, the namespaces imported into it, its parent scope and keeps two caches for the resolver (the results
// of simple name lookups and the extension methods visible from it).
//
//     auto* scope = make<UsingScope>(context, &module.root_namespace(), std::vector<const INamespace*>{});
//     auto* nested = scope->with_nested_namespace("Collections");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.TypeSystem: UsingScope, from UsingScope.cs), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler). Using aliases and extern aliases are not read from metadata (upstream returns empty lists too).
#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/csharp/type_system/csharp_type_resolve_context.hpp"
#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::type_system {

class UsingScope : public il::ArenaObject {
public:
    using ExtensionMethodGroups = std::vector<std::vector<const typesystem::IMethod*>>;

    // The context is the one outside this scope (its using scope is the parent); neither it nor the namespace may be null.
    UsingScope(const CSharpTypeResolveContext& context, const typesystem::INamespace* ns, std::vector<const typesystem::INamespace*> usings);

    const typesystem::INamespace* namespace_() const { return ns_; }
    const UsingScope* parent() const { return parent_context_.current_using_scope(); }
    const std::vector<const typesystem::INamespace*>& usings() const { return usings_; }
    const std::vector<std::pair<std::string, semantics::ResolveResult*>>& using_aliases() const;
    const std::vector<std::string>& extern_aliases() const;
    // Whether this scope has an alias (either using or extern) with that name.
    bool has_alias(const std::string& identifier) const;

    // The scope of the namespace declaration `simple_name` inside this scope's namespace (a dummy namespace when there is no such one).
    UsingScope* with_nested_namespace(const std::string& simple_name) const;

    // The resolver's cache of simple names looked up in this scope (a cached null result is a result): true and the result when the
    // identifier is cached.
    bool try_get_resolve_cache(const std::string& identifier, semantics::ResolveResult*& result) const;
    // Caches a result unless the identifier is cached already.
    void try_add_resolve_cache(const std::string& identifier, semantics::ResolveResult* result) const;

    // The extension methods visible from this scope grouped by scope, as the resolver found them; null until set.
    const ExtensionMethodGroups* all_extension_methods() const;
    // Keeps the groups unless some were set first; returns the kept ones (upstream's LazyInit.GetOrSet).
    const ExtensionMethodGroups& get_or_set_all_extension_methods(ExtensionMethodGroups groups) const;

private:
    CSharpTypeResolveContext parent_context_;
    const typesystem::INamespace* ns_;
    std::vector<const typesystem::INamespace*> usings_;

    mutable std::mutex mutex_;
    mutable std::unordered_map<std::string, semantics::ResolveResult*> resolve_cache_;
    mutable std::unique_ptr<ExtensionMethodGroups> all_extension_methods_;
};

}  // namespace cb::dotnet::decompiler::csharp::type_system
