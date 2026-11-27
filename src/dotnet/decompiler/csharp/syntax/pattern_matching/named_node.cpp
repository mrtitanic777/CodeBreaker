// dotnet/decompiler/csharp/syntax/pattern_matching/named_node.cpp -- see named_node.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/named_node.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

NamedNode::NamedNode(std::string name, INode* child) : group_name_(std::move(name)), child_node_(child) {
    if (child == nullptr) throw std::invalid_argument("child");
}

bool NamedNode::match_one(INode* other, const Match& match) const {
    match.add(group_name_, other);
    return child_node_->match_one(other, match);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
