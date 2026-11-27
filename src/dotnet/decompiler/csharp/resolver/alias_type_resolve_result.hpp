// dotnet/decompiler/csharp/resolver/alias_type_resolve_result.hpp -- the result of a using alias that names a type.
//
//     auto* rr = make<AliasTypeResolveResult>("Dict", underlying_type_result);
//     rr->alias();    // "Dict"
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: AliasTypeResolveResult, from AliasTypeResolveResult.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/type_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using il::make;
using semantics::ResolveResult;

class AliasTypeResolveResult : public semantics::TypeResolveResult {
public:
    AliasTypeResolveResult(std::string alias, const semantics::TypeResolveResult& underlying_result)
        : TypeResolveResult(underlying_result.type()), alias_(std::move(alias)) {}

    const std::string& alias() const { return alias_; }

    std::string type_name() const override { return "AliasTypeResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<AliasTypeResolveResult>(*this); }

private:
    std::string alias_;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
