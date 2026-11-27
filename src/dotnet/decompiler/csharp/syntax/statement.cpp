// dotnet/decompiler/csharp/syntax/statement.cpp -- see statement.hpp.
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

Statement* Statement::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<Statement, AstNodeKind::StatementPlaceholder>>(pattern);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
