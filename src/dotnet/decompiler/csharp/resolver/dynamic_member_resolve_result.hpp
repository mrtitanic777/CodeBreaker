// dotnet/decompiler/csharp/resolver/dynamic_member_resolve_result.hpp -- the result of a member access on a dynamic target; its type is
// dynamic.
//
//     auto* rr = make<DynamicMemberResolveResult>(target, "Length");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: DynamicMemberResolveResult, from DynamicMemberResolveResult.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using il::make;
using semantics::ResolveResult;

class DynamicMemberResolveResult : public ResolveResult {
public:
    DynamicMemberResolveResult(ResolveResult* target, std::string member, const typesystem::IMember* symbol = nullptr);

    ResolveResult* target;
    std::string member;
    // The member the access is bound to when the binder found it (null when not known)
    const typesystem::IMember* symbol;

    std::vector<ResolveResult*> get_child_results() const override { return {target}; }
    std::string type_name() const override { return "DynamicMemberResolveResult"; }
    std::string to_string() const override { return "[Dynamic member '" + member + "']"; }
    ResolveResult* shallow_clone() const override { return make<DynamicMemberResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
