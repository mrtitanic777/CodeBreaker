// dotnet/decompiler/csharp/syntax/pattern_matching/any_node.hpp -- matches any node (not an absent child) and records it under a name.
//
//     auto* any = make<AnyNode>("operand");        // match->get<Expression>("operand") after a match
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: AnyNode, from AnyNode.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class AnyNode : public Pattern {
public:
    // An empty group name records nothing.
    explicit AnyNode(std::string name = {}) : group_name_(std::move(name)) {}

    const std::string& group_name() const { return group_name_; }

    bool match_one(INode* other, const Match& match) const override;

private:
    std::string group_name_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
