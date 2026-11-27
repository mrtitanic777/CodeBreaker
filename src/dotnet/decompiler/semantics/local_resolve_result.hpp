// dotnet/decompiler/semantics/local_resolve_result.hpp -- the result of a reference to a local variable or parameter (the type of a ref
// parameter is the type it points to).
//
//     auto* rr = make<LocalResolveResult>(variable);
//     rr->is_parameter();
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: LocalResolveResult, from LocalResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class LocalResolveResult : public ResolveResult {
public:
    explicit LocalResolveResult(const typesystem::IVariable* variable);

    const typesystem::IVariable* variable() const { return variable_; }
    bool is_parameter() const;

    bool is_compile_time_constant() const override { return variable_->is_const(); }
    ConstantValue constant_value() const override;
    std::string type_name() const override { return "LocalResolveResult"; }
    std::string to_string() const override;
    ResolveResult* shallow_clone() const override { return make<LocalResolveResult>(*this); }

private:
    static const IType* unpack_type_if_by_ref_parameter(const typesystem::IVariable* variable);

    const typesystem::IVariable* variable_;
};

}  // namespace cb::dotnet::decompiler::semantics
