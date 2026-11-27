// dotnet/decompiler/csharp/syntax/pattern_matching/optional_node.cpp -- see optional_node.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/optional_node.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/named_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

OptionalNode::OptionalNode(INode* child) : child_node_(child) {
    if (child == nullptr) throw std::invalid_argument("child");
}

OptionalNode::OptionalNode(const std::string& name, INode* child) : OptionalNode(il::make<NamedNode>(name, child)) {}

bool OptionalNode::match_in_collection(const INodeList& other, std::size_t pos, const Match& match,
                                       BacktrackingInfo& backtracking_info) const {
    // Push the "absent" alternative (resume the following pattern node at the same position), then try to consume the element here.
    backtracking_info.backtracking_stack.push_back(PossibleMatch{pos, match.check_point()});
    return child_node_->match_one(pos < other.size() ? other.node_at(pos) : nullptr, match);
}

bool OptionalNode::match_one(INode* other, const Match& match) const {
    if (other == nullptr) return true;
    return child_node_->match_one(other, match);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
