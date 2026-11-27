// dotnet/decompiler/csharp/syntax/expression.cpp -- see expression.hpp.
#include "dotnet/decompiler/csharp/syntax/expression.hpp"

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

Expression* Expression::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<Expression, AstNodeKind::ExpressionPlaceholder>>(pattern);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
