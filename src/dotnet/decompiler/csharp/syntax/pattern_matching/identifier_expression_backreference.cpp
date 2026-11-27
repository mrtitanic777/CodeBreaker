// dotnet/decompiler/csharp/syntax/pattern_matching/identifier_expression_backreference.cpp -- see
// identifier_expression_backreference.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/identifier_expression_backreference.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

#include "dotnet/decompiler/csharp/syntax/identifier_expression.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

bool IdentifierExpressionBackreference::match_one(INode* other, const Match& match) const {
    auto* ident = syntax::as<IdentifierExpression>(other);
    if (ident == nullptr || ident->type_arguments().count() > 0) return false;
    std::vector<INode*> found = match.get(referenced_group_name_);
    if (found.empty()) throw std::logic_error("no node was recorded under the referenced group");  // upstream: Last() of nothing throws
    auto* referenced = syntax::as<AstNode>(found.back());
    if (referenced == nullptr) return false;
    Identifier* referenced_identifier = referenced->get_child(Slots::identifier);
    if (referenced_identifier == nullptr) return false;
    return ident->identifier() == referenced_identifier->name();
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
