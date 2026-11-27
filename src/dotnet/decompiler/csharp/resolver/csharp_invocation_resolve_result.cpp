#include "dotnet/decompiler/csharp/resolver/csharp_invocation_resolve_result.hpp"

#include <algorithm>
#include <cstdint>

#include "dotnet/decompiler/semantics/array_create_resolve_result.hpp"
#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"
#include "dotnet/decompiler/semantics/error_resolve_result.hpp"
#include "dotnet/decompiler/semantics/named_argument_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using semantics::ArrayCreateResolveResult;
using semantics::ConstantResolveResult;
using semantics::ErrorResolveResult;
using semantics::NamedArgumentResolveResult;

CSharpInvocationResolveResult::CSharpInvocationResolveResult(ResolveResult* target_result, const typesystem::IParameterizedMember* member,
                                                             std::vector<ResolveResult*> arguments, OverloadResolutionErrors overload_resolution_errors,
                                                             bool is_extension_method_invocation, bool is_expanded_form, bool is_delegate_invocation,
                                                             std::optional<std::vector<int>> argument_to_parameter_map,
                                                             std::vector<ResolveResult*> initializer_statements,
                                                             const typesystem::IType* return_type_override)
    : InvocationResolveResult(target_result, member, std::move(arguments), std::move(initializer_statements), return_type_override),
      overload_resolution_errors(overload_resolution_errors),
      is_extension_method_invocation(is_extension_method_invocation),
      is_delegate_invocation(is_delegate_invocation),
      is_expanded_form(is_expanded_form),
      argument_to_parameter_map_(std::move(argument_to_parameter_map)) {}

std::vector<ResolveResult*> CSharpInvocationResolveResult::get_arguments_for_call() const {
    const auto& parameters = member()->parameters();
    std::vector<ResolveResult*> results(parameters.size(), nullptr);
    std::vector<ResolveResult*> params_arguments;  // (used only in the expanded form)
    for (int i = 0; i < static_cast<int>(arguments.size()); i++) {
        int mapped_to;
        if (argument_to_parameter_map_)
            mapped_to = (*argument_to_parameter_map_)[i];
        else
            mapped_to = is_expanded_form ? std::min(i, static_cast<int>(results.size()) - 1) : i;
        if (mapped_to >= 0 && mapped_to < static_cast<int>(results.size())) {
            if (is_expanded_form && mapped_to == static_cast<int>(results.size()) - 1) {
                params_arguments.push_back(arguments[i]);
            } else {
                auto* narr = dynamic_cast<NamedArgumentResolveResult*>(arguments[i]);
                if (narr != nullptr)
                    results[mapped_to] = narr->argument;
                else
                    results[mapped_to] = arguments[i];
            }
        }
    }
    if (is_expanded_form) {
        const typesystem::IType* array_type = parameters.back()->type();
        const typesystem::IType* int32 = member()->compilation().find_type(typesystem::KnownTypeCode::Int32);
        std::vector<ResolveResult*> size_arguments{
            make<ConstantResolveResult>(int32, typesystem::ConstantValue{static_cast<std::int32_t>(params_arguments.size())})};
        results.back() = make<ArrayCreateResolveResult>(array_type, std::move(size_arguments), std::move(params_arguments));
    }
    for (std::size_t i = 0; i < results.size(); i++) {
        if (results[i] == nullptr) {
            if (parameters[i]->is_optional())
                results[i] = make<ConstantResolveResult>(parameters[i]->type(), parameters[i]->get_constant_value());
            else
                results[i] = ErrorResolveResult::unknown_error();
        }
    }
    return results;
}

}  // namespace cb::dotnet::decompiler::csharp::resolver
