// dotnet/decompiler/csharp/syntax/pattern_matching/repeat.hpp -- in a list of children, matches the child pattern between min_count and
// max_count times in a row (backtracking over how many); alone, matches one node (or nothing if min_count is 0).
//
//     auto* any_statements = make<Repeat>(make<AnyNode>());      // zero or more of anything
//     any_statements->set_min_count(1);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: Repeat, from Repeat.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <climits>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class Repeat : public Pattern {
public:
    // `child_node` must not be null (std::invalid_argument). Matches 0 to INT_MAX times until changed.
    explicit Repeat(INode* child);

    int min_count() const { return min_count_; }
    void set_min_count(int value) { min_count_ = value; }
    int max_count() const { return max_count_; }
    void set_max_count(int value) { max_count_ = value; }
    INode* child_node() const { return child_node_; }

    bool match_in_collection(const INodeList& other, std::size_t pos, const Match& match,
                             BacktrackingInfo& backtracking_info) const override;
    bool match_one(INode* other, const Match& match) const override;

private:
    INode* child_node_;
    int min_count_ = 0;
    int max_count_ = INT_MAX;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
