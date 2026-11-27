// dotnet/decompiler/csharp/syntax/this_reference_expression.cpp -- see this_reference_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/this_reference_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {


bool ThisReferenceExpression::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<ThisReferenceExpression>(other);
    return o != nullptr;
}


}  // namespace cb::dotnet::decompiler::csharp::syntax
