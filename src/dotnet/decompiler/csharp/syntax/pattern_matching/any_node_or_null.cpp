// dotnet/decompiler/csharp/syntax/pattern_matching/any_node_or_null.cpp -- see any_node_or_null.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/any_node_or_null.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

bool AnyNodeOrNull::match_one(INode* other, const Match& match) const {
    if (other == nullptr)
        match.add_null(group_name_);
    else
        match.add(group_name_, other);
    return true;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
