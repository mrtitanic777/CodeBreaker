// dotnet/decompiler/csharp/syntax/pattern_matching/pattern_extensions.hpp -- the helpers that put patterns into a tree of nodes: a
// pattern as a type, an expression or a statement, and an expression or statement that records what it matches under a name.
//
//     Expression* any = to_expression(make<AnyNode>());
//     Expression* named = with_name(some_expression_pattern, "call");        // match.get<Expression>("call") after a match
//
// (Upstream's `Match` and `IsMatch` extensions on INode are `match` and `is_match` in inode.hpp.)
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: PatternExtensions ToType, ToExpression, ToStatement and
// WithName, from INode.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_fwd.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

// The pattern as a node that stands for it where a type / expression / statement goes (nullptr for nullptr).
AstType* to_type(Pattern* pattern);
Expression* to_expression(Pattern* pattern);
Statement* to_statement(Pattern* pattern);

// `node` recorded under `pattern_group_name` when matched (a NamedNode around it), as an expression / statement again.
Expression* with_name(Expression* node, const std::string& pattern_group_name);
Statement* with_name(Statement* node, const std::string& pattern_group_name);

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
