// dotnet/decompiler/semantics/interpolated_string_resolve_result.hpp -- the result of an interpolated string: the format string and the
// results of the interpolated values.
//
//     auto* rr = make<InterpolatedStringResolveResult>(string_type, "{0} items", std::vector<ResolveResult*>{count});
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: InterpolatedStringResolveResult, from InterpolatedStringResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class InterpolatedStringResolveResult : public ResolveResult {
public:
    InterpolatedStringResolveResult(const IType* string_type, std::string format_string, std::vector<ResolveResult*> arguments)
        : ResolveResult(string_type), format_string(std::move(format_string)), arguments(std::move(arguments)) {}

    std::string format_string;
    std::vector<ResolveResult*> arguments;

    std::vector<ResolveResult*> get_child_results() const override { return arguments; }
    std::string type_name() const override { return "InterpolatedStringResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<InterpolatedStringResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
