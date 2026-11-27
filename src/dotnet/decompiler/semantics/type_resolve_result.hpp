// dotnet/decompiler/semantics/type_resolve_result.hpp -- the result of a name that means a type.
//
//     auto* rr = make<TypeResolveResult>(string_type);
//     rr->is_error();    // false (true only for an unknown type)
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: TypeResolveResult, from TypeResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class TypeResolveResult : public ResolveResult {
public:
    explicit TypeResolveResult(const IType* type) : ResolveResult(type) {}

    bool is_error() const override;
    std::string type_name() const override { return "TypeResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<TypeResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
