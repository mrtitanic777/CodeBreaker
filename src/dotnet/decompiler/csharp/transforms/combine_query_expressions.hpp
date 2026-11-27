// dotnet/decompiler/csharp/transforms/combine_query_expressions.hpp -- CombineQueryExpressions: combines query expressions and removes
// the transparent identifiers the compiler introduced between their clauses.
//
//     CombineQueryExpressions().run(syntax_tree, context);
//
// A query "from x in (from y in a select new { y, z = f(y) }) where g(x.z) select x.y" is written by the compiler with an anonymous
// type carrying the range variables from clause to clause; this removes it ("from y in a let z = f(y) where g(z) select y"). A query
// whose source is another query that cannot be merged becomes a query continuation ("... into x"); a "from x in e.Cast<T>()" gets the
// type moved into the clause ("from T x in e").
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: CombineQueryExpressions, LetIdentifierAnnotation, from CombineQueryExpressions.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <unordered_map>

#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

// Annotates what a query "let" clause introduced for a transparent identifier member (the identifier it is referred to by).
class LetIdentifierAnnotation final : public il::ArenaObject {};

class CombineQueryExpressions final : public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;

private:
    // The annotation each from or let identifier is referred to with (a variable's, or a LetIdentifierAnnotation; nullptr for none).
    using IdentifierAnnotations = std::unordered_map<std::string, il::ArenaObject*>;

    void combine_queries(syntax::AstNode* node, IdentifierAnnotations& from_or_let_identifiers);
    bool try_remove_transparent_identifier(syntax::QueryExpression* query, syntax::QueryFromClause* from_clause, syntax::QueryExpression* inner_query,
                                           IdentifierAnnotations& let_clauses);
    // Removes all occurrences of transparent identifiers.
    void remove_transparent_identifier_references(syntax::AstNode* node, IdentifierAnnotations& from_or_let_identifiers);

    TransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
