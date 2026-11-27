// dotnet/decompiler/semantics/ambiguous_resolve_result.hpp -- results of a name that has more than one meaning: always an error.
//
//     auto* rr = make<AmbiguousTypeResolveResult>(type);
//     rr->is_error();    // true
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: AmbiguousTypeResolveResult, AmbiguousMemberResolveResult, from AmbiguousResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class AmbiguousTypeResolveResult : public TypeResolveResult {
public:
    explicit AmbiguousTypeResolveResult(const IType* type) : TypeResolveResult(type) {}

    bool is_error() const override { return true; }
    std::string type_name() const override { return "AmbiguousTypeResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<AmbiguousTypeResolveResult>(*this); }
};

class AmbiguousMemberResolveResult : public MemberResolveResult {
public:
    AmbiguousMemberResolveResult(ResolveResult* target_result, const typesystem::IMember* member) : MemberResolveResult(target_result, member) {}

    bool is_error() const override { return true; }
    std::string type_name() const override { return "AmbiguousMemberResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<AmbiguousMemberResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
