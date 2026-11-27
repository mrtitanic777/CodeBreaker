#include "dotnet/decompiler/semantics/operator_resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

OperatorResolveResult::OperatorResolveResult(const IType* result_type, il::ExpressionType operator_type, std::vector<ResolveResult*> operands)
    : ResolveResult(result_type), operator_type_(operator_type), operands_(std::move(operands)) {}

OperatorResolveResult::OperatorResolveResult(const IType* result_type, il::ExpressionType operator_type,
                                             const typesystem::IMethod* user_defined_operator_method, bool is_lifted_operator,
                                             std::vector<ResolveResult*> operands)
    : ResolveResult(result_type),
      operator_type_(operator_type),
      user_defined_operator_method_(user_defined_operator_method),
      operands_(std::move(operands)),
      is_lifted_operator_(is_lifted_operator) {}

}  // namespace cb::dotnet::decompiler::semantics
