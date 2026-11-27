// dotnet/decompiler/csharp/syntax/pattern_matching/pattern_extensions.cpp -- see pattern_extensions.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern_extensions.hpp"

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/named_node.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

AstType* to_type(Pattern* pattern) { return AstType::from_pattern(pattern); }
Expression* to_expression(Pattern* pattern) { return Expression::from_pattern(pattern); }
Statement* to_statement(Pattern* pattern) { return Statement::from_pattern(pattern); }

Expression* with_name(Expression* node, const std::string& pattern_group_name) {
    return Expression::from_pattern(il::make<NamedNode>(pattern_group_name, node));
}

Statement* with_name(Statement* node, const std::string& pattern_group_name) {
    return Statement::from_pattern(il::make<NamedNode>(pattern_group_name, node));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
