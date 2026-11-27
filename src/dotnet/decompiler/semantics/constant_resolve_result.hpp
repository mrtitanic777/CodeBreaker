// dotnet/decompiler/semantics/constant_resolve_result.hpp -- the result of a compile-time constant: its type and value.
//
//     auto* rr = make<ConstantResolveResult>(int32_type, ConstantValue{std::int32_t{42}});
//     rr->to_string();    // "[ConstantResolveResult System.Int32 = 42]"
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ConstantResolveResult, from ConstantResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class ConstantResolveResult : public ResolveResult {
public:
    ConstantResolveResult(const IType* type, ConstantValue constant_value) : ResolveResult(type), constant_value_(std::move(constant_value)) {}

    bool is_compile_time_constant() const override { return true; }
    ConstantValue constant_value() const override { return constant_value_; }
    std::string type_name() const override { return "ConstantResolveResult"; }
    std::string to_string() const override;
    ResolveResult* shallow_clone() const override { return make<ConstantResolveResult>(*this); }

private:
    ConstantValue constant_value_;
};

}  // namespace cb::dotnet::decompiler::semantics
