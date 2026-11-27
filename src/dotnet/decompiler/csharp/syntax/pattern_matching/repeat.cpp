// dotnet/decompiler/csharp/syntax/pattern_matching/repeat.cpp -- see repeat.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/repeat.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

Repeat::Repeat(INode* child) : child_node_(child) {
    if (child == nullptr) throw std::invalid_argument("child");
}

bool Repeat::match_in_collection(const INodeList& other, std::size_t pos, const Match& match, BacktrackingInfo& backtracking_info) const {
    std::vector<PossibleMatch>& backtracking_stack = backtracking_info.backtracking_stack;
    int match_count = 0;
    if (min_count_ <= 0) backtracking_stack.push_back(PossibleMatch{pos, match.check_point()});
    while (match_count < max_count_ && pos < other.size() && child_node_->match_one(other.node_at(pos), match)) {
        match_count++;
        pos++;
        if (match_count >= min_count_) backtracking_stack.push_back(PossibleMatch{pos, match.check_point()});
    }
    // Never a normal (single-element) match: the caller always looks at the results on the backtracking stack.
    return false;
}

bool Repeat::match_one(INode* other, const Match& match) const {
    if (other == nullptr) return min_count_ <= 0;
    return max_count_ >= 1 && child_node_->match_one(other, match);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
