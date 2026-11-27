// dotnet/decompiler/semantics/initialized_object_resolve_result.hpp -- the result that stands for the object being initialized inside an
// object or collection initializer.
//
//     auto* rr = make<InitializedObjectResolveResult>(type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: InitializedObjectResolveResult, from InitializedObjectResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class InitializedObjectResolveResult : public ResolveResult {
public:
    explicit InitializedObjectResolveResult(const IType* type) : ResolveResult(type) {}

    std::string type_name() const override { return "InitializedObjectResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<InitializedObjectResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
