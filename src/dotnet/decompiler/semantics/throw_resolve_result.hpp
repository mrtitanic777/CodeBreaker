// dotnet/decompiler/semantics/throw_resolve_result.hpp -- the result of a throw expression (it has no type).
//
//     auto* rr = make<ThrowResolveResult>();
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ThrowResolveResult, from ThrowResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class ThrowResolveResult : public ResolveResult {
public:
    ThrowResolveResult();

    std::string type_name() const override { return "ThrowResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<ThrowResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
