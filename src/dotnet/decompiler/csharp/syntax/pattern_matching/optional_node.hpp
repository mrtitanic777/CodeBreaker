// dotnet/decompiler/csharp/syntax/pattern_matching/optional_node.hpp -- matches its child pattern, or nothing at all: an absent child
// matches, and in a list of children it may consume one element or none.
//
//     auto* optional = make<OptionalNode>(child_pattern);
//     auto* named = make<OptionalNode>("x", child_pattern);       // records the matched node as "x"
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: OptionalNode, from OptionalNode.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class OptionalNode : public Pattern {
public:
    // `child_node` must not be null (std::invalid_argument).
    explicit OptionalNode(INode* child);
    OptionalNode(const std::string& name, INode* child);

    INode* child_node() const { return child_node_; }

    bool match_in_collection(const INodeList& other, std::size_t pos, const Match& match,
                             BacktrackingInfo& backtracking_info) const override;
    bool match_one(INode* other, const Match& match) const override;

private:
    INode* child_node_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
