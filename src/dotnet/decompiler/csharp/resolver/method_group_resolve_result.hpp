// dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp -- the result of a name that means a group of methods (before overload
// resolution picks one): the methods grouped by declaring type, the target, the explicit type arguments, and the chosen method once
// overload resolution has picked one.
//
//     auto* rr = make<MethodGroupResolveResult>(target, "Add", method_lists, type_arguments);
//     for (const IMethod* m : rr->methods()) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: MethodListWithDeclaringType, MethodGroupResolveResult, from
// MethodGroupResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Not ported: GetExtensionMethods, GetEligibleExtensionMethods (code completion; the decompiler never calls them) and PerformOverloadResolution,
// which is resolver::perform_overload_resolution (overload_resolution.hpp). The members they use (extension_methods, resolver) are here.
#pragma once

#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using il::make;
using semantics::ResolveResult;

class CSharpResolver;

// The methods of one declaring type (the type they are found on, which may be a more derived one than the declaring type of each).
class MethodListWithDeclaringType : public std::vector<const typesystem::IParameterizedMember*> {
public:
    explicit MethodListWithDeclaringType(const typesystem::IType* declaring_type) : declaring_type_(declaring_type) {}
    MethodListWithDeclaringType(const typesystem::IType* declaring_type, const std::vector<const typesystem::IParameterizedMember*>& methods)
        : std::vector<const typesystem::IParameterizedMember*>(methods), declaring_type_(declaring_type) {}

    const typesystem::IType* declaring_type() const { return declaring_type_; }

private:
    const typesystem::IType* declaring_type_;
};

class MethodGroupResolveResult : public ResolveResult {
public:
    // target_result is null for a static method group; type_arguments the explicit ones (may be empty).
    MethodGroupResolveResult(ResolveResult* target_result, std::string method_name, std::vector<MethodListWithDeclaringType> methods,
                             std::vector<const typesystem::IType*> type_arguments);

    ResolveResult* target_result() const { return target_result_; }
    // The type of the target (unknown without one).
    const typesystem::IType* target_type() const;
    const std::string& method_name() const { return method_name_; }
    // Every method of the group.
    std::vector<const typesystem::IMethod*> methods() const;
    const std::vector<MethodListWithDeclaringType>& methods_grouped_by_declaring_type() const { return method_lists_; }
    const std::vector<const typesystem::IType*>& type_arguments() const { return type_arguments_; }

    // The extension methods found by the resolver, grouped by the namespace they come from (filled lazily from resolver).
    std::vector<std::vector<const typesystem::IMethod*>> extension_methods;
    CSharpResolver* resolver = nullptr;

    // The method overload resolution chose (null while none has been).
    const typesystem::IMethod* chosen_method() const { return chosen_method_; }
    // A copy of this result with the chosen method set.
    MethodGroupResolveResult* with_chosen_method(const typesystem::IMethod* method) const;

    std::vector<ResolveResult*> get_child_results() const override;
    std::string type_name() const override { return "MethodGroupResolveResult"; }
    std::string to_string() const override;
    ResolveResult* shallow_clone() const override { return make<MethodGroupResolveResult>(*this); }

private:
    std::vector<MethodListWithDeclaringType> method_lists_;
    std::vector<const typesystem::IType*> type_arguments_;
    ResolveResult* target_result_;
    std::string method_name_;
    const typesystem::IMethod* chosen_method_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
