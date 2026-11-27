// dotnet/decompiler/semantics/array_access_resolve_result.hpp -- the result of array[i, j]: the array and its indexes.
//
//     auto* rr = make<ArrayAccessResolveResult>(element_type, array_result, std::vector<ResolveResult*>{index});
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ArrayAccessResolveResult, from ArrayAccessResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class ArrayAccessResolveResult : public ResolveResult {
public:
    // The type is the element type of the array.
    ArrayAccessResolveResult(const IType* element_type, ResolveResult* array, std::vector<ResolveResult*> indexes);

    ResolveResult* array;
    std::vector<ResolveResult*> indexes;

    std::vector<ResolveResult*> get_child_results() const override;
    std::string type_name() const override { return "ArrayAccessResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<ArrayAccessResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
