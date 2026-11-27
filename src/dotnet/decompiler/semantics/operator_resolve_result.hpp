// dotnet/decompiler/semantics/operator_resolve_result.hpp -- the result of applying an operator: the operator (as a
// System.Linq.Expressions.ExpressionType), its operands, and the user-defined operator method when there is one.
//
//     auto* rr = make<OperatorResolveResult>(bool_type, il::ExpressionType::Equal, std::vector<ResolveResult*>{a, b});
//
// Ported from ILSpy (ICSharpCode.Decompiler.Semantics: OperatorResolveResult, from OperatorResolveResult.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/il/advanced_instructions.hpp"
#include "dotnet/decompiler/semantics/resolve_result.hpp"

namespace cb::dotnet::decompiler::semantics {

class OperatorResolveResult : public ResolveResult {
public:
    OperatorResolveResult(const IType* result_type, il::ExpressionType operator_type, std::vector<ResolveResult*> operands);
    OperatorResolveResult(const IType* result_type, il::ExpressionType operator_type, const typesystem::IMethod* user_defined_operator_method,
                          bool is_lifted_operator, std::vector<ResolveResult*> operands);

    il::ExpressionType operator_type() const { return operator_type_; }
    const std::vector<ResolveResult*>& operands() const { return operands_; }
    // null for a built-in operator
    const typesystem::IMethod* user_defined_operator_method() const { return user_defined_operator_method_; }
    bool is_lifted_operator() const { return is_lifted_operator_; }

    std::vector<ResolveResult*> get_child_results() const override { return operands_; }
    std::string type_name() const override { return "OperatorResolveResult"; }
    ResolveResult* shallow_clone() const override { return make<OperatorResolveResult>(*this); }

private:
    il::ExpressionType operator_type_;
    const typesystem::IMethod* user_defined_operator_method_ = nullptr;
    std::vector<ResolveResult*> operands_;
    bool is_lifted_operator_ = false;
};

}  // namespace cb::dotnet::decompiler::semantics
