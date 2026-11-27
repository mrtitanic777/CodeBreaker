// dotnet/decompiler/csharp/syntax/pattern_matching/named_node.hpp -- records the node a child pattern is matched against under a name,
// then matches the child pattern.
//
//     auto* named = make<NamedNode>("call", child);      // or `with_name(expression, "call")` (pattern_extensions.hpp)
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: NamedNode, from NamedNode.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class NamedNode : public Pattern {
public:
    // `child_node` must not be null (std::invalid_argument).
    NamedNode(std::string name, INode* child);

    const std::string& group_name() const { return group_name_; }
    INode* child_node() const { return child_node_; }

    bool match_one(INode* other, const Match& match) const override;

private:
    std::string group_name_;
    INode* child_node_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
