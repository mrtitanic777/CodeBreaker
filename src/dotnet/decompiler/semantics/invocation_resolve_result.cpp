#include "dotnet/decompiler/semantics/invocation_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

InvocationResolveResult::InvocationResolveResult(ResolveResult* target_result, const typesystem::IParameterizedMember* member,
                                                 std::vector<ResolveResult*> arguments, std::vector<ResolveResult*> initializer_statements,
                                                 const IType* return_type_override)
    : MemberResolveResult(target_result, member, return_type_override),
      arguments(std::move(arguments)),
      initializer_statements(std::move(initializer_statements)),
      parameterized_member_(member) {}

std::vector<ResolveResult*> InvocationResolveResult::get_child_results() const {
    std::vector<ResolveResult*> results = MemberResolveResult::get_child_results();
    results.insert(results.end(), arguments.begin(), arguments.end());
    results.insert(results.end(), initializer_statements.begin(), initializer_statements.end());
    return results;
}

}  // namespace cb::dotnet::decompiler::semantics
