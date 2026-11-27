// dotnet/decompiler/csharp/syntax/error_expression.cpp -- see error_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/error_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ErrorExpression::ErrorExpression(const std::string& error) {
    add_trailing_trivia(make<Comment>(error, CommentType::MultiLine));
}


bool ErrorExpression::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<ErrorExpression>(other);
    return o != nullptr;
}


}  // namespace cb::dotnet::decompiler::csharp::syntax
