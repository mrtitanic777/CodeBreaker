#include "dotnet/decompiler/csharp/resolver/dynamic_member_resolve_result.hpp"

#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

DynamicMemberResolveResult::DynamicMemberResolveResult(ResolveResult* target, std::string member, const typesystem::IMember* symbol)
    : ResolveResult(&typesystem::SpecialType::dynamic()), target(target), member(std::move(member)), symbol(symbol) {}

}  // namespace cb::dotnet::decompiler::csharp::resolver
