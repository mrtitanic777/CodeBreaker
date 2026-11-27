// dotnet/decompiler/csharp/syntax/expression.hpp -- Expression, the base of every C# expression node (BinaryOperatorExpression,
// InvocationExpression, IdentifierExpression ...). Each concrete expression class is in its own header.
//
//     Expression* copy = expr->clone();                                    // typed, unlike AstNode::clone
//     expr->replace_with([](Expression* e) { return make<ParenthesizedExpression>(e); });
//     Expression* any = Expression::from_pattern(make<AnyNode>("x"));      // a pattern in the place of an expression
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Expression, from Expressions/Expression.cs and the pattern placeholder
// generated into it), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <concepts>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class Expression : public AstNode {
public:
    // A deep copy of this subtree, unattached (AstNode::clone, typed).
    Expression* clone() const { return static_cast<Expression*>(AstNode::clone()); }

    // Replaces this expression by `new_node` (nullptr: removes it); see AstNode::replace_with.
    void replace_with(AstNode* new_node) { AstNode::replace_with(new_node); }
    // Replaces this expression by what the function makes of it: the expression is removed first, the function gets it and returns
    // the root of a tree, which is put in its place. Returns the replacement.
    template <class F>
        requires std::invocable<F, Expression*>
    Expression* replace_with(F&& replace_function) {
        AstNode* replacement = AstNode::replace_with(
            [&replace_function](AstNode* node) -> AstNode* { return replace_function(static_cast<Expression*>(node)); });
        return static_cast<Expression*>(replacement);
    }

    // The node that stands for `pattern` where an expression goes (upstream's implicit conversion Pattern -> Expression).
    static Expression* from_pattern(pattern_matching::Pattern* pattern);

protected:
    Expression() = default;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
