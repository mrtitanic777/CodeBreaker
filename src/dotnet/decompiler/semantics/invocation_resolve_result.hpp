// dotnet/decompiler/semantics/invocation_resolve_result.hpp -- the result of calling a method, indexer or constructor: the member, its
// target, the arguments and the initializer statements of an object creation.
//
//     auto* rr = make<InvocationResolveResult>(target_result, method, std::vector<ResolveResult*>{arg});
//     rr->get_arguments_for_call();
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: InvocationResolveResult, from InvocationResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/member_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class InvocationResolveResult : public MemberResolveResult {
public:
    InvocationResolveResult(ResolveResult* target_result, const typesystem::IParameterizedMember* member, std::vector<ResolveResult*> arguments = {},
                            std::vector<ResolveResult*> initializer_statements = {}, const IType* return_type_override = nullptr);

    // The arguments as written at the call (named arguments and the expanded params array included).
    std::vector<ResolveResult*> arguments;
    // The statements of an object or collection initializer.
    std::vector<ResolveResult*> initializer_statements;

    // The invoked member, as a parameterized one (upstream hides the base property with a new one).
    const typesystem::IParameterizedMember* member() const { return parameterized_member_; }

    // The arguments in the order of the parameters, as the call passes them (upstream: virtual; a call made from the C# resolver reorders).
    virtual std::vector<ResolveResult*> get_arguments_for_call() const { return arguments; }

    std::vector<ResolveResult*> get_child_results() const override;
    std::string type_name() const override { return "InvocationResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<InvocationResolveResult>(*this); }

private:
    const typesystem::IParameterizedMember* parameterized_member_;
};

}  // namespace cb::dotnet::decompiler::semantics
