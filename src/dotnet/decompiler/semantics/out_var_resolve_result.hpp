// dotnet/decompiler/semantics/out_var_resolve_result.hpp -- the result of an out variable declaration (out var x): it has no type of its
// own, the declared variable type is kept.
//
//     auto* rr = make<OutVarResolveResult>(variable_type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: OutVarResolveResult, from OutVarResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class OutVarResolveResult : public ResolveResult {
public:
    explicit OutVarResolveResult(const IType* original_variable_type);

    const IType* original_variable_type;

    std::string type_name() const override { return "OutVarResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<OutVarResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
