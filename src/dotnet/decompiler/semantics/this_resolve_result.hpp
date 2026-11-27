// dotnet/decompiler/semantics/this_resolve_result.hpp -- the result of "this" or "base".
//
//     auto* rr = make<ThisResolveResult>(type, /*causes_non_virtual_invocation=*/true);    // base
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ThisResolveResult, from ThisResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class ThisResolveResult : public ResolveResult {
public:
    explicit ThisResolveResult(const IType* type, bool causes_non_virtual_invocation = false)
        : ResolveResult(type), causes_non_virtual_invocation_(causes_non_virtual_invocation) {}

    // Whether a call on this result is not virtual (a base access).
    bool causes_non_virtual_invocation() const { return causes_non_virtual_invocation_; }

    std::string type_name() const override { return "ThisResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<ThisResolveResult>(*this); }

private:
    bool causes_non_virtual_invocation_;
};

}  // namespace cb::dotnet::decompiler::semantics
