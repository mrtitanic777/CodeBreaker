// dotnet/decompiler/csharp/resolver/alias_namespace_resolve_result.hpp -- the result of a using alias that names a namespace.
//
//     auto* rr = make<AliasNamespaceResolveResult>("Coll", underlying_namespace_result);
//     rr->alias();    // "Coll"
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: AliasNamespaceResolveResult, from AliasNamespaceResolveResult.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/namespace_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using il::make;
using semantics::ResolveResult;

class AliasNamespaceResolveResult : public semantics::NamespaceResolveResult {
public:
    AliasNamespaceResolveResult(std::string alias, const semantics::NamespaceResolveResult& underlying_result)
        : NamespaceResolveResult(underlying_result.namespace_()), alias_(std::move(alias)) {}

    const std::string& alias() const { return alias_; }

    std::string type_name() const override { return "AliasNamespaceResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<AliasNamespaceResolveResult>(*this); }

private:
    std::string alias_;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
