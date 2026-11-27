// dotnet/decompiler/csharp/syntax/pattern_matching/pattern.cpp -- see pattern.hpp.
#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

#include <cassert>
#include <vector>

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

bool Pattern::match_in_collection(const INodeList& other, std::size_t pos, const Match& match, BacktrackingInfo&) const {
    return match_one(pos < other.size() ? other.node_at(pos) : nullptr, match);
}

bool Pattern::match_collections(const INodeList& pattern_children, const INodeList& other_children, const Match& match) {
    BacktrackingInfo backtracking_info;
    std::vector<std::size_t> pattern_stack;
    std::vector<PossibleMatch>& stack = backtracking_info.backtracking_stack;
    pattern_stack.push_back(0);
    stack.push_back(PossibleMatch{0, match.check_point()});
    while (!stack.empty()) {
        std::size_t pattern_pos = pattern_stack.back();
        pattern_stack.pop_back();
        std::size_t other_pos = stack.back().next_other_index;
        match.restore_check_point(stack.back().checkpoint);
        stack.pop_back();
        bool success = true;
        while (pattern_pos < pattern_children.size() && success) {
            INode* cur1 = pattern_children.node_at(pattern_pos);
            assert(stack.size() == pattern_stack.size());
            success = cur1->match_in_collection(other_children, other_pos, match, backtracking_info);
            assert(stack.size() >= pattern_stack.size());
            // For every alternative the pattern node pushed, queue the next pattern node so the continuation resumes after it.
            while (stack.size() > pattern_stack.size()) pattern_stack.push_back(pattern_pos + 1);
            pattern_pos++;
            other_pos++;
        }
        if (success && other_pos >= other_children.size()) return true;
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
