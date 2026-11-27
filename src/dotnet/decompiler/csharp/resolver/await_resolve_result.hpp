// dotnet/decompiler/csharp/resolver/await_resolve_result.hpp -- the result of an await expression: the GetAwaiter call and the awaiter
// members the await pattern needs (IsCompleted, OnCompleted, GetResult).
//
//     auto* rr = make<AwaitResolveResult>(result_type, get_awaiter_call, awaiter_type, is_completed, on_completed, get_result);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: AwaitResolveResult, from AwaitResolveResult.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using il::make;
using semantics::ResolveResult;

class AwaitResolveResult : public ResolveResult {
public:
    AwaitResolveResult(const typesystem::IType* result_type, ResolveResult* get_awaiter_invocation, const typesystem::IType* awaiter_type,
                       const typesystem::IProperty* is_completed_property, const typesystem::IMethod* on_completed_method,
                       const typesystem::IMethod* get_result_method);

    // The call of GetAwaiter
    ResolveResult* get_awaiter_invocation;
    const typesystem::IType* awaiter_type;
    // The IsCompleted property of the awaiter (null when missing)
    const typesystem::IProperty* is_completed_property;
    const typesystem::IMethod* on_completed_method;
    const typesystem::IMethod* get_result_method;

    bool is_error() const override;
    std::vector<ResolveResult*> get_child_results() const override { return {get_awaiter_invocation}; }
    std::string type_name() const override { return "AwaitResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<AwaitResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
