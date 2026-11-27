// dotnet/decompiler/csharp/resolver/dynamic_invocation_resolve_result.hpp -- the result of a dynamic invocation, indexing or object
// creation: the target, the arguments and the member the call is bound to when it is known; its type is dynamic.
//
//     auto* rr = make<DynamicInvocationResolveResult>(target, DynamicInvocationType::Invocation, arguments);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: DynamicInvocationType, DynamicInvocationResolveResult, from
// DynamicInvocationResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using il::make;
using semantics::ResolveResult;

enum class DynamicInvocationType : std::uint8_t { Invocation, Indexing, ObjectCreation };

class DynamicInvocationResolveResult : public ResolveResult {
public:
    DynamicInvocationResolveResult(ResolveResult* target, DynamicInvocationType invocation_type, std::vector<ResolveResult*> arguments,
                                   std::vector<ResolveResult*> initializer_statements = {}, const typesystem::IMember* symbol = nullptr);

    // The target of the invocation (null for an object creation)
    ResolveResult* target;
    DynamicInvocationType invocation_type;
    std::vector<ResolveResult*> arguments;
    // The statements of an object or collection initializer
    std::vector<ResolveResult*> initializer_statements;
    // The member the call is bound to when the binder found it (null when not known)
    const typesystem::IMember* symbol;

    std::string type_name() const override { return "DynamicInvocationResolveResult"; }
    std::string to_string() const override { return "[Dynamic invocation ]"; }
    ResolveResult* shallow_clone() const override { return make<DynamicInvocationResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
