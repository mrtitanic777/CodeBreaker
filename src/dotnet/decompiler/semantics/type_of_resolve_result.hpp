// dotnet/decompiler/semantics/type_of_resolve_result.hpp -- the result of typeof(T): the type, System.Type, and the type it refers to.
//
//     auto* rr = make<TypeOfResolveResult>(system_type, int_type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: TypeOfResolveResult, from TypeOfResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class TypeOfResolveResult : public ResolveResult {
public:
    TypeOfResolveResult(const IType* system_type, const IType* referenced_type);

    const IType* referenced_type() const { return referenced_type_; }

    std::string type_name() const override { return "TypeOfResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<TypeOfResolveResult>(*this); }

private:
    const IType* referenced_type_;
};

}  // namespace cb::dotnet::decompiler::semantics
