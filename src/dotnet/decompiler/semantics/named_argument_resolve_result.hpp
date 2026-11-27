// dotnet/decompiler/semantics/named_argument_resolve_result.hpp -- the result of a named argument, name: value; the parameter (when it
// is known) and the result of the value.
//
//     auto* rr = make<NamedArgumentResolveResult>(parameter, value_result, method);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: NamedArgumentResolveResult, from NamedArgumentResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class NamedArgumentResolveResult : public ResolveResult {
public:
    NamedArgumentResolveResult(const typesystem::IParameter* parameter, ResolveResult* argument, const typesystem::IParameterizedMember* member = nullptr);
    // A named argument whose parameter is not known.
    NamedArgumentResolveResult(std::string parameter_name, ResolveResult* argument);

    // The member the argument is passed to (null when unknown)
    const typesystem::IParameterizedMember* member = nullptr;
    // The parameter (null when unknown)
    const typesystem::IParameter* parameter = nullptr;
    std::string parameter_name;
    ResolveResult* argument;

    std::vector<ResolveResult*> get_child_results() const override { return {argument}; }
    std::string type_name() const override { return "NamedArgumentResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<NamedArgumentResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
