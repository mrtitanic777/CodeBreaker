// dotnet/decompiler/semantics/for_each_resolve_result.hpp -- the result of a foreach statement: the GetEnumerator call and the types and
// members the loop uses (Current, MoveNext).
//
//     auto* rr = make<ForEachResolveResult>(get_enumerator_call, collection_type, enumerator_type, element_type, current, move_next, void_type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: ForEachResolveResult, from ForEachResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class ForEachResolveResult : public ResolveResult {
public:
    ForEachResolveResult(ResolveResult* get_enumerator_call, const IType* collection_type, const IType* enumerator_type, const IType* element_type,
                         const typesystem::IProperty* current_property, const typesystem::IMethod* move_next_method, const IType* void_type);

    // The call of GetEnumerator
    ResolveResult* get_enumerator_call;
    // The collection type that was explicitly chosen for the enumeration
    const IType* collection_type;
    // The type of the enumerator
    const IType* enumerator_type;
    // The type of the loop variable
    const IType* element_type;
    // The Current property of the enumerator (null when the enumerator is dynamic)
    const typesystem::IProperty* current_property;
    // The MoveNext method of the enumerator (null when the enumerator is dynamic)
    const typesystem::IMethod* move_next_method;

    std::string type_name() const override { return "ForEachResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<ForEachResolveResult>(*this); }
};

}  // namespace cb::dotnet::decompiler::semantics
