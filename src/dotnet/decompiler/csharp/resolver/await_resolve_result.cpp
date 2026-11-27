#include "dotnet/decompiler/csharp/resolver/await_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

AwaitResolveResult::AwaitResolveResult(const typesystem::IType* result_type, ResolveResult* get_awaiter_invocation, const typesystem::IType* awaiter_type,
                                       const typesystem::IProperty* is_completed_property, const typesystem::IMethod* on_completed_method,
                                       const typesystem::IMethod* get_result_method)
    : ResolveResult(result_type),
      get_awaiter_invocation(get_awaiter_invocation),
      awaiter_type(awaiter_type),
      is_completed_property(is_completed_property),
      on_completed_method(on_completed_method),
      get_result_method(get_result_method) {
    if (awaiter_type == nullptr) throw std::invalid_argument("awaiterType");
    if (get_awaiter_invocation == nullptr) throw std::invalid_argument("getAwaiterInvocation");
}

bool AwaitResolveResult::is_error() const {
    return get_awaiter_invocation->is_error() ||
           (awaiter_type->kind() != typesystem::TypeKind::Dynamic &&
            (is_completed_property == nullptr || on_completed_method == nullptr || get_result_method == nullptr));
}

}  // namespace cb::dotnet::decompiler::csharp::resolver
