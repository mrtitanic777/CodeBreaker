// dotnet/decompiler/csharp/resolver/lambda_resolve_result.hpp -- the result of a lambda or anonymous method: its parameters, return type
// and whether it converts to a given delegate signature; DecompiledLambdaResolveResult is the one made from a decompiled ILFunction.
//
//     auto* rr = make<DecompiledLambdaResolveResult>(function, delegate_type, inferred_return_type, true, false, false);
//     rr->is_async();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: LambdaResolveResult, DecompiledLambdaResolveResult, LambdaConversion, from
// LambdaResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/semantics/conversion.hpp"
#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::il {
class ILFunction;
}

namespace cb::dotnet::decompiler::csharp::resolver {

using il::make;
using semantics::Conversion;
using semantics::ResolveResult;

class CSharpConversions;

class LambdaResolveResult : public ResolveResult {
public:
    // Whether the lambda has a parameter list: (int x) => ... or delegate(int x) { }, not delegate { }.
    virtual bool has_parameter_list() const = 0;
    virtual bool is_anonymous_method() const = 0;
    // Whether the parameter types are inferred: (x) => ...
    virtual bool is_implicitly_typed() const = 0;
    virtual bool is_async() const = 0;

    // The return type the lambda has when its parameters have these types.
    virtual const typesystem::IType* get_inferred_return_type(const std::vector<const typesystem::IType*>& parameter_types) const = 0;
    virtual const std::vector<const typesystem::IParameter*>& parameters() const = 0;
    virtual const typesystem::IType* return_type() const = 0;

    // Whether the lambda converts to a delegate with these parameter types and this return type: the conversion, or Conversion::none().
    virtual const Conversion* is_valid(const std::vector<const typesystem::IType*>& parameter_types, const typesystem::IType* return_type,
                                       CSharpConversions* conversions) const = 0;

    virtual ResolveResult* body() const = 0;

    std::vector<ResolveResult*> get_child_results() const override { return {body()}; }
    std::string type_name() const override { return "LambdaResolveResult"; }

protected:
    LambdaResolveResult();
};

class DecompiledLambdaResolveResult final : public LambdaResolveResult {
public:
    DecompiledLambdaResolveResult(il::ILFunction* function, const typesystem::IType* delegate_type, const typesystem::IType* inferred_return_type,
                                  bool has_parameter_list, bool is_anonymous_method, bool is_implicitly_typed);

    const typesystem::IType* delegate_type;
    const typesystem::IType* inferred_return_type;

    bool has_parameter_list() const override { return has_parameter_list_; }
    bool is_anonymous_method() const override { return is_anonymous_method_; }
    bool is_implicitly_typed() const override { return is_implicitly_typed_; }
    bool is_async() const override;
    const std::vector<const typesystem::IParameter*>& parameters() const override;
    const typesystem::IType* return_type() const override;
    ResolveResult* body() const override { return body_; }
    const typesystem::IType* get_inferred_return_type(const std::vector<const typesystem::IType*>&) const override { return inferred_return_type; }
    const Conversion* is_valid(const std::vector<const typesystem::IType*>& parameter_types, const typesystem::IType* return_type,
                               CSharpConversions* conversions) const override;

    std::string type_name() const override { return "DecompiledLambdaResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<DecompiledLambdaResolveResult>(*this); }

private:
    il::ILFunction* function_;
    bool has_parameter_list_;
    bool is_anonymous_method_;
    bool is_implicitly_typed_;
    ResolveResult* body_;
};

// The conversion of an anonymous function to a delegate type.
class LambdaConversion : public Conversion {
public:
    static const LambdaConversion* instance();

    bool is_anonymous_function_conversion() const override { return true; }
    bool is_implicit() const override { return true; }
    std::string to_string() const override { return "LambdaConversion"; }

protected:
    LambdaConversion() = default;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver
