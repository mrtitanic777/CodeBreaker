// dotnet/decompiler/csharp/output_visitor/generic_grammar_ambiguity_visitor.hpp -- GenericGrammarAmbiguityVisitor: finds a `<` comparison that
// the C# parser would read as the start of a generic argument list (`a < b > (c)` is a call of the generic method a<b>(c)), and puts the
// comparison in parentheses.
//
//     GenericGrammarAmbiguityVisitor::resolve_ambiguities(syntax_tree);
//     bool bad = GenericGrammarAmbiguityVisitor::causes_ambiguity_with_generics(binary_operator_expression);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: GenericGrammarAmbiguityVisitor), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class GenericGrammarAmbiguityVisitor : public syntax::DepthFirstAstVisitorT<bool> {
public:
    // Parenthesizes every `<` comparison under the node that would be read as a generic argument list.
    static void resolve_ambiguities(syntax::AstNode* root_node);
    static bool causes_ambiguity_with_generics(syntax::BinaryOperatorExpression* binary_operator_expression);

    bool visit_binary_operator_expression(syntax::BinaryOperatorExpression& binary_operator_expression) override;
    bool visit_identifier_expression(syntax::IdentifierExpression& identifier_expression) override;
    bool visit_type_reference_expression(syntax::TypeReferenceExpression& type_reference_expression) override;
    bool visit_member_reference_expression(syntax::MemberReferenceExpression& member_reference_expression) override;

protected:
    bool visit_children(syntax::AstNode& node) override;

private:
    int generic_nesting_level_ = 0;
    bool ambiguity_found_ = false;
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
