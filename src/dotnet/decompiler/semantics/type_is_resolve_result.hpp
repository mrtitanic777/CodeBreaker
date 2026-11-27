// dotnet/decompiler/semantics/type_is_resolve_result.hpp -- the result of "x is T": the input and the type tested for; the type is bool.
//
//     auto* rr = make<TypeIsResolveResult>(input, target_type, boolean_type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: TypeIsResolveResult, from TypeIsResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class TypeIsResolveResult : public ResolveResult {
public:
    TypeIsResolveResult(ResolveResult* input, const IType* target_type, const IType* boolean_type);

    ResolveResult* input;
    const IType* target_type;

    std::string type_name() const override { return "TypeIsResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<TypeIsResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
