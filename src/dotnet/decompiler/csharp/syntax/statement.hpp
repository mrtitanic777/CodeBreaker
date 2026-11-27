// dotnet/decompiler/csharp/syntax/statement.hpp -- Statement, the base of every C# statement node (BlockStatement, IfElseStatement,
// ReturnStatement ...). Each concrete statement class is in its own header.
//
//     Statement* copy = stmt->clone();                                      // typed, unlike AstNode::clone
//     stmt->replace_with([](Statement* s) { return make<BlockStatement>(s); });
//     Statement* any = Statement::from_pattern(make<AnyNode>("body"));      // a pattern in the place of a statement
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Statement, from Statements/Statement.cs and the pattern placeholder
// generated into it), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <concepts>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class Statement : public AstNode {
public:
    // A deep copy of this subtree, unattached (AstNode::clone, typed).
    Statement* clone() const { return static_cast<Statement*>(AstNode::clone()); }

    // Replaces this statement by `new_node` (nullptr: removes it); see AstNode::replace_with.
    void replace_with(AstNode* new_node) { AstNode::replace_with(new_node); }
    // Replaces this statement by what the function makes of it: the statement is removed first, the function gets it and returns the
    // root of a tree, which is put in its place. Returns the replacement.
    template <class F>
        requires std::invocable<F, Statement*>
    Statement* replace_with(F&& replace_function) {
        AstNode* replacement = AstNode::replace_with(
            [&replace_function](AstNode* node) -> AstNode* { return replace_function(static_cast<Statement*>(node)); });
        return static_cast<Statement*>(replacement);
    }

    // The node that stands for `pattern` where a statement goes (upstream's implicit conversion Pattern -> Statement).
    static Statement* from_pattern(pattern_matching::Pattern* pattern);

protected:
    Statement() = default;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
