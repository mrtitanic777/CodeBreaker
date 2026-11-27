// dotnet/decompiler/csharp/resolver/csharp_invocation_resolve_result.hpp -- the result of a call as the C# resolver sees it: the
// invocation, with the errors overload resolution found, and how the arguments map to the parameters (extension method call, expanded
// params array, delegate call).
//
//     auto* rr = make<CSharpInvocationResolveResult>(target, method, arguments);
//     rr->get_arguments_for_call();    // one result per parameter, in order
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: CSharpInvocationResolveResult, from CSharpInvocationResolveResult.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <optional>

#include "dotnet/decompiler/csharp/resolver/overload_resolution_errors.hpp"
#include "dotnet/decompiler/semantics/invocation_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using il::make;
using semantics::ResolveResult;

class CSharpInvocationResolveResult : public semantics::InvocationResolveResult {
public:
    CSharpInvocationResolveResult(ResolveResult* target_result, const typesystem::IParameterizedMember* member, std::vector<ResolveResult*> arguments,
                                  OverloadResolutionErrors overload_resolution_errors = OverloadResolutionErrors::None,
                                  bool is_extension_method_invocation = false, bool is_expanded_form = false, bool is_delegate_invocation = false,
                                  std::optional<std::vector<int>> argument_to_parameter_map = std::nullopt,
                                  std::vector<ResolveResult*> initializer_statements = {}, const typesystem::IType* return_type_override = nullptr);

    // The errors overload resolution found (None: none)
    OverloadResolutionErrors overload_resolution_errors;
    // Whether the call is of the form target.Method(...) where Method is an extension method (the target is the first argument).
    bool is_extension_method_invocation;
    // Whether the call is through a delegate.
    bool is_delegate_invocation;
    // Whether the params parameter is passed as its elements instead of an array.
    bool is_expanded_form;

    bool is_error() const override { return overload_resolution_errors != OverloadResolutionErrors::None; }

    // For each argument the index of the parameter it is passed to (nullopt: the arguments are in the order of the parameters).
    const std::optional<std::vector<int>>& get_argument_to_parameter_map() const { return argument_to_parameter_map_; }

    // One result per parameter: named arguments unwrapped, the elements of an expanded params array gathered into an array creation,
    // optional parameters filled with their default value, missing ones with an error result.
    std::vector<ResolveResult*> get_arguments_for_call() const override;

    std::string type_name() const override { return "CSharpInvocationResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<CSharpInvocationResolveResult>(*this); }

private:
    std::optional<std::vector<int>> argument_to_parameter_map_;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
