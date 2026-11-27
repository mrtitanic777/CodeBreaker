// dotnet/decompiler/semantics/size_of_resolve_result.hpp -- the result of sizeof(T): the type, and the size when it is a constant.
//
//     auto* rr = make<SizeOfResolveResult>(int32_type, int_type, 4);
//     rr->is_compile_time_constant();    // true
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: SizeOfResolveResult, from SizeOfResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <optional>

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class SizeOfResolveResult : public ResolveResult {
public:
    SizeOfResolveResult(const IType* int32, const IType* referenced_type, std::optional<int> constant_value);

    const IType* referenced_type() const { return referenced_type_; }

    bool is_compile_time_constant() const override { return constant_value_.has_value(); }
    ConstantValue constant_value() const override;
    // An error unless the type is known to be a value type.
    bool is_error() const override;
    std::string type_name() const override { return "SizeOfResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<SizeOfResolveResult>(*this); }

private:
    const IType* referenced_type_;
    std::optional<int> constant_value_;
};

}  // namespace cb::dotnet::decompiler::semantics
