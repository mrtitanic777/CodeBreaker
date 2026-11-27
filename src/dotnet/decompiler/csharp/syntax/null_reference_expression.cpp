// dotnet/decompiler/csharp/syntax/null_reference_expression.cpp -- see null_reference_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/null_reference_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {


bool NullReferenceExpression::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<NullReferenceExpression>(other);
    return o != nullptr;
}


}  // namespace cb::dotnet::decompiler::csharp::syntax
