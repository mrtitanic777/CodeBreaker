// dotnet/decompiler/semantics/by_reference_resolve_result.hpp -- the result of "ref x", "out x" or "in x": the result of the element and
// the kind of reference; the type is the by-reference type of the element type.
//
//     auto* rr = make<ByReferenceResolveResult>(local_result, typesystem::ReferenceKind::Ref);
//     rr->element_type();
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ByReferenceResolveResult, from ByReferenceResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class ByReferenceResolveResult : public ResolveResult {
public:
    ByReferenceResolveResult(ResolveResult* element_result, typesystem::ReferenceKind kind);
    // A reference to a type, without a result for the element (internal upstream).
    ByReferenceResolveResult(const IType* element_type, typesystem::ReferenceKind kind);

    typesystem::ReferenceKind reference_kind() const { return reference_kind_; }
    // null when made from a type
    ResolveResult* element_result = nullptr;

    const IType* element_type() const;

    std::vector<ResolveResult*> get_child_results() const override;
    std::string type_name() const override { return "ByReferenceResolveResult"; }
    std::string to_string() const override;
    ResolveResult* shallow_clone() const override { return make<ByReferenceResolveResult>(*this); }

private:
    typesystem::ReferenceKind reference_kind_;
};

}  // namespace cb::dotnet::decompiler::semantics
