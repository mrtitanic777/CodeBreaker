// dotnet/decompiler/semantics/array_create_resolve_result.hpp -- the result of "new T[n] { ... }": the size arguments and the initializer
// elements (none when there is no initializer).
//
//     auto* rr = make<ArrayCreateResolveResult>(array_type, std::vector<ResolveResult*>{size}, std::nullopt);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ArrayCreateResolveResult, from ArrayCreateResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <optional>

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class ArrayCreateResolveResult : public ResolveResult {
public:
    ArrayCreateResolveResult(const IType* array_type, std::vector<ResolveResult*> size_arguments,
                             std::optional<std::vector<ResolveResult*>> initializer_elements);

    std::vector<ResolveResult*> size_arguments;
    // nullopt: the creation has no initializer
    std::optional<std::vector<ResolveResult*>> initializer_elements;

    std::vector<ResolveResult*> get_child_results() const override;
    std::string type_name() const override { return "ArrayCreateResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<ArrayCreateResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
