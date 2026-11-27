// dotnet/decompiler/csharp/transforms/replace_method_calls_with_operators.hpp -- ReplaceMethodCallsWithOperators: replaces calls of operator
// methods, string concatenation and a few runtime helpers with the C# syntax they stand for: String.Concat(a, b) -> a + b,
// op_Addition(a, b) -> a + b, op_Explicit(x) -> (T)x, Type.GetTypeFromHandle(typeof(T).TypeHandle) -> typeof(T), Activator.CreateInstance<T>()
// -> new T(), RuntimeHelpers.GetSubArray(a, r) -> a[r] and the methodof pattern.
//
//     ReplaceMethodCallsWithOperators().run(syntax_tree, context);
//     Expression* shorter = ReplaceMethodCallsWithOperators::remove_redundant_to_string_in_concat(arg, concat_method, /*is_last_argument*/ true);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: ReplaceMethodCallsWithOperators, from ReplaceMethodCallsWithOperators.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <optional>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
}

namespace cb::dotnet::decompiler::csharp::transforms {

class ReplaceMethodCallsWithOperators final : public syntax::DepthFirstAstVisitor, public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;

    void visit_invocation_expression(syntax::InvocationExpression& invocation_expression) override;
    void visit_cast_expression(syntax::CastExpression& cast_expression) override;

    // Whether the operator's checked form (op_CheckedAddition for op_Addition ...) exists in the method's declaring type.
    static bool has_checked_equivalent(const typesystem::IMethod* method);

    // "x.ToString()" as "x" where the + operator calls ToString() itself and the result is the same.
    static syntax::Expression* remove_redundant_to_string_in_concat(syntax::Expression* expr, const typesystem::IMethod* concat_method,
                                                                    bool is_last_argument);

private:
    void process_invocation_expression(syntax::InvocationExpression* invocation_expression);
    static bool is_instantiable_type_parameter(const typesystem::IType* type);
    static bool check_arguments_for_string_concat(const std::vector<syntax::Expression*>& arguments);
    static bool is_string_concat(const typesystem::IParameterizedMember* member);
    static bool to_string_is_known_effect_free(const typesystem::IType* type);
    static std::optional<syntax::BinaryOperatorType> get_binary_operator_type_from_metadata_name(const std::string& name, bool& is_checked,
                                                                                                 const DecompilerSettings& settings);
    static std::optional<syntax::UnaryOperatorType> get_unary_operator_type_from_metadata_name(const std::string& name, bool& is_checked,
                                                                                               const DecompilerSettings& settings);

    TransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
