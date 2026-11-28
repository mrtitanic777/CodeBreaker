// dotnet/decompiler/csharp/transforms/pattern_statement_transform.hpp -- PatternStatementTransform: finds the expanded forms of C#
// statements and members with pattern matching and replaces them with the construct they came from: a while loop with an iterator
// becomes `for`, loops over arrays become `foreach`, properties that only wrap a compiler-generated field become automatic properties,
// a Finalize method becomes a destructor, and nested try-catch-finally, cascading else-if, a && (b && c), `!(a == b)`, pattern-based
// `fixed` and enhanced `using` are tidied.
//
//     PatternStatementTransform transform;
//     transform.run(root_node, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: PatternStatementTransform, from PatternStatementTransform.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/match.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/context_tracking_visitor.hpp"
#include "dotnet/decompiler/csharp/transforms/declare_variables.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// Finds the expanded form of using statements using pattern matching and replaces it with a UsingStatement.
class PatternStatementTransform final : public ContextTrackingVisitor<syntax::AstNode*>, public IAstTransform {
public:
    // std::logic_error when called again while it runs.
    void run(syntax::AstNode* root_node, TransformContext& context) override;

    // The attribute types that are dropped from a backing field when it becomes an automatic property.
    static const std::vector<std::string>& attribute_types_to_remove_from_auto_properties();

    // Whether the field is the compiler-generated backing field of an automatic property (and the property it belongs to).
    static bool is_backing_field_of_automatic_property(const typesystem::IField* field, const typesystem::IProperty** property);

    // ---- visitor overrides ----
    syntax::AstNode* visit_expression_statement(syntax::ExpressionStatement& expression_statement) override;
    syntax::AstNode* visit_for_statement(syntax::ForStatement& for_statement) override;
    syntax::AstNode* visit_if_else_statement(syntax::IfElseStatement& if_else_statement) override;
    syntax::AstNode* visit_property_declaration(syntax::PropertyDeclaration& property_declaration) override;
    syntax::AstNode* visit_event_declaration(syntax::EventDeclaration& event_declaration) override;
    syntax::AstNode* visit_method_declaration(syntax::MethodDeclaration& method_declaration) override;
    syntax::AstNode* visit_destructor_declaration(syntax::DestructorDeclaration& destructor_declaration) override;
    syntax::AstNode* visit_try_catch_statement(syntax::TryCatchStatement& try_catch_statement) override;
    syntax::AstNode* visit_identifier(syntax::Identifier& identifier) override;
    syntax::AstNode* visit_binary_operator_expression(syntax::BinaryOperatorExpression& expr) override;
    syntax::AstNode* visit_unary_operator_expression(syntax::UnaryOperatorExpression& expr) override;
    syntax::AstNode* visit_fixed_statement(syntax::FixedStatement& fixed_statement) override;
    syntax::AstNode* visit_using_statement(syntax::UsingStatement& using_statement) override;

protected:
    // Go through the children, and keep visiting a node as long as it changes.
    syntax::AstNode* visit_children(syntax::AstNode& node) override;

private:
    using Base = ContextTrackingVisitor<syntax::AstNode*>;

    // for
    syntax::ForStatement* transform_for(syntax::ExpressionStatement* node);
    static bool descend_into_statement(syntax::AstNode* node);
    static bool for_statement_uses_variable(syntax::ForStatement* statement, il::ILVariable* variable);
    static bool is_variable_used_after(syntax::Statement* loop, il::ILVariable* variable);
    bool iterator_variables_declared_inside_loop_body(syntax::Statement* iterator_statement);

    // foreach
    bool variable_can_be_used_as_foreach_local(il::ILVariable* item_var, syntax::Statement* loop);
    bool address_used_for_single_call(il::ILVariable* v, il::BlockContainer* loop) const;
    syntax::Statement* transform_foreach_on_array(syntax::ForStatement* for_statement);
    syntax::Statement* transform_foreach_on_inline_array(syntax::ForStatement* for_statement);
    bool match_lower_bound(int index_num, il::ILVariable** index, il::ILVariable* collection, syntax::Statement* statement);
    bool match_foreach_on_multi_dim_array(const std::vector<il::ILVariable*>& upper_bounds, il::ILVariable* collection,
                                          syntax::Statement* first_initializer_statement, syntax::IdentifierExpression** foreach_variable,
                                          std::vector<syntax::Statement*>* statements, std::vector<il::ILVariable*>* lower_bounds);
    syntax::Statement* transform_foreach_on_multi_dim_array(syntax::ExpressionStatement* expression_statement);
    syntax::AstType* convert_variable_type(il::ILVariable* variable);

    // automatic properties
    static bool can_transform_to_automatic_property(const typesystem::IProperty* property, bool accessors_must_be_compiler_generated);
    syntax::PropertyDeclaration* transform_automatic_property(syntax::PropertyDeclaration* property_declaration);
    syntax::Identifier* replace_backing_field_usage(syntax::Identifier* identifier);

    // destructors, try-catch-finally, else-if
    syntax::DestructorDeclaration* transform_destructor(syntax::MethodDeclaration* method_def);
    syntax::DestructorDeclaration* transform_destructor_body(syntax::DestructorDeclaration* dtor_def);
    syntax::TryCatchStatement* transform_try_catch_finally(syntax::TryCatchStatement* try_finally);
    syntax::AstNode* simplify_cascading_if_else_statements(syntax::IfElseStatement* node);

    DeclareVariables declare_variables_;
    TransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
