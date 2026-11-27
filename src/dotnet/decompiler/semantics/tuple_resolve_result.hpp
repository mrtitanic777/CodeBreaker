// dotnet/decompiler/semantics/tuple_resolve_result.hpp -- the result of a tuple expression, (a, b): the results of the elements; the type is
// the tuple type of their types (no type when an element has none).
//
//     auto* rr = make<TupleResolveResult>(compilation, std::vector<ResolveResult*>{a, b});
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: TupleResolveResult, from TupleResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <optional>

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class TupleResolveResult : public ResolveResult {
public:
    // element_names: empty leaves every element unnamed.
    TupleResolveResult(const typesystem::ICompilation& compilation, std::vector<ResolveResult*> elements,
                       std::vector<std::optional<std::string>> element_names = {}, const typesystem::IModule* value_tuple_assembly = nullptr);

    const std::vector<ResolveResult*>& elements() const { return elements_; }

    std::vector<ResolveResult*> get_child_results() const override { return elements_; }
    std::string type_name() const override { return "TupleResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<TupleResolveResult>(*this); }

private:
    static const IType* get_tuple_type(const typesystem::ICompilation& compilation, const std::vector<ResolveResult*>& elements,
                                       std::vector<std::optional<std::string>> element_names, const typesystem::IModule* value_tuple_assembly);

    std::vector<ResolveResult*> elements_;
};

}  // namespace cb::dotnet::decompiler::semantics
