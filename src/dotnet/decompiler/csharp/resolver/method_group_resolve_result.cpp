#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

MethodGroupResolveResult::MethodGroupResolveResult(ResolveResult* target_result, std::string method_name, std::vector<MethodListWithDeclaringType> methods,
                                                   std::vector<const typesystem::IType*> type_arguments)
    : ResolveResult(&typesystem::SpecialType::no_type()),
      method_lists_(std::move(methods)),
      type_arguments_(std::move(type_arguments)),
      target_result_(target_result),
      method_name_(std::move(method_name)) {}

const typesystem::IType* MethodGroupResolveResult::target_type() const {
    return target_result_ != nullptr ? target_result_->type() : &typesystem::SpecialType::unknown_type();
}

std::vector<const typesystem::IMethod*> MethodGroupResolveResult::methods() const {
    std::vector<const typesystem::IMethod*> result;
    for (const MethodListWithDeclaringType& list : method_lists_)
        for (const typesystem::IParameterizedMember* m : list) result.push_back(dynamic_cast<const typesystem::IMethod*>(m));
    return result;
}

MethodGroupResolveResult* MethodGroupResolveResult::with_chosen_method(const typesystem::IMethod* method) const {
    auto* result = make<MethodGroupResolveResult>(*this);
    result->chosen_method_ = method;
    return result;
}

std::vector<ResolveResult*> MethodGroupResolveResult::get_child_results() const {
    if (target_result_ != nullptr) return {target_result_};
    return {};
}

std::string MethodGroupResolveResult::to_string() const {
    return "[" + type_name() + " with " + std::to_string(methods().size()) + " method(s)]";
}

}  // namespace cb::dotnet::decompiler::csharp::resolver
