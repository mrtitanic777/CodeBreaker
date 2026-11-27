#include "dotnet/decompiler/csharp/resolver/dynamic_invocation_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

DynamicInvocationResolveResult::DynamicInvocationResolveResult(ResolveResult* target, DynamicInvocationType invocation_type,
                                                               std::vector<ResolveResult*> arguments,
                                                               std::vector<ResolveResult*> initializer_statements, const typesystem::IMember* symbol)
    : ResolveResult(&typesystem::SpecialType::dynamic()),
      target(target),
      invocation_type(invocation_type),
      arguments(std::move(arguments)),
      initializer_statements(std::move(initializer_statements)),
      symbol(symbol) {}

}  // namespace cb::dotnet::decompiler::csharp::resolver
