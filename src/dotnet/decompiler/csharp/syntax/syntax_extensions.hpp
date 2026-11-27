// dotnet/decompiler/csharp/syntax/syntax_extensions.hpp -- small helpers over the syntax tree that upstream writes as extension methods.
//
//     Statement* next = get_next_statement(statement);             // the next sibling that is a statement
//     bool arglist = is_arg_list(type);                            // `__arglist`
//     Expression* inner = unwrap_in_direction_expression(arg);     // `in x` -> x
//     detach(node);                                                // node->remove(), returning the node
//
// is_comparison_operator (OperatorType, from operator_declaration.hpp), is_bitwise (BinaryOperatorType, from operator_type.hpp) and
// unwrap_in_direction_expression (DirectionExpression, from direction_expression.hpp) are declared and defined once those headers exist
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: SyntaxExtensions, from SyntaxExtensions.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

#include "dotnet/decompiler/csharp/syntax/operator_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/syntax/direction_expression.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// Whether the operator is one of ==, !=, >, <, >=, <=.
bool is_comparison_operator(OperatorType operator_type);

// Whether the operator is &, | or ^.
bool is_bitwise(BinaryOperatorType operator_type);

// The next sibling of the statement that is a statement, or nullptr.
Statement* get_next_statement(Statement* statement);

// Whether the type is the `__arglist` pseudo type (nullptr: false).
bool is_arg_list(const AstType* type);

// Adds `name = argument` to the attribute's arguments (a named attribute argument).
void add_named_argument(Attribute* attribute, const std::string& name, Expression* argument);

// Removes the node from its parent and returns it.
template <class T>
T* detach(T* node) {
    node->remove();
    return node;
}

// `in x` as `x` (taken out of its direction expression); any other expression is returned unchanged.
Expression* unwrap_in_direction_expression(Expression* expr);

}  // namespace cb::dotnet::decompiler::csharp::syntax
