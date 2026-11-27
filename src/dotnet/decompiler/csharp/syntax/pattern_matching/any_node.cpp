// dotnet/decompiler/csharp/syntax/pattern_matching/any_node.cpp -- see any_node.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/any_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

bool AnyNode::match_one(INode* other, const Match& match) const {
    match.add(group_name_, other);
    return other != nullptr;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
