// dotnet/decompiler/semantics/conversion_resolve_result.hpp -- the result of converting an expression to another type: the input and the
// conversion applied.
//
//     auto* rr = make<ConversionResolveResult>(target_type, input, Conversion::boxing_conversion());
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ConversionResolveResult, from ConversionResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/conversion.hpp"
#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class ConversionResolveResult : public ResolveResult {
public:
    ConversionResolveResult(const IType* target_type, ResolveResult* input, const Conversion* conversion);
    ConversionResolveResult(const IType* target_type, ResolveResult* input, const Conversion* conversion, bool check_for_overflow);

    ResolveResult* input;
    const Conversion* conversion;
    bool check_for_overflow = false;

    bool is_error() const override { return !conversion->is_valid(); }
    std::vector<ResolveResult*> get_child_results() const override { return {input}; }
    std::string type_name() const override { return "ConversionResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<ConversionResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
