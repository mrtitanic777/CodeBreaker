// dotnet/decompiler/semantics/namespace_resolve_result.hpp -- the result of a name that means a namespace.
//
//     auto* rr = make<NamespaceResolveResult>(ns);
//     rr->namespace_name();    // "System.Collections"
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: NamespaceResolveResult, from NamespaceResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class NamespaceResolveResult : public ResolveResult {
public:
    explicit NamespaceResolveResult(const typesystem::INamespace* ns);

    const typesystem::INamespace* namespace_() const { return ns_; }
    std::string namespace_name() const { return ns_->full_name(); }

    std::string type_name() const override { return "NamespaceResolveResult"; }
    std::string to_string() const override;
    ResolveResult* shallow_clone() const override { return make<NamespaceResolveResult>(*this); }

private:
    const typesystem::INamespace* ns_;
};

}  // namespace cb::dotnet::decompiler::semantics
