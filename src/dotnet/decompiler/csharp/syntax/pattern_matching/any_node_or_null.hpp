// dotnet/decompiler/csharp/syntax/pattern_matching/any_node_or_null.hpp -- matches any node or an absent child, and records what it
// found (a null for an absent child) under a name.
//
//     auto* maybe = make<AnyNodeOrNull>("else");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: AnyNodeOrNull, from AnyNodeOrNull.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class AnyNodeOrNull : public Pattern {
public:
    // An empty group name records nothing.
    explicit AnyNodeOrNull(std::string name = {}) : group_name_(std::move(name)) {}

    const std::string& group_name() const { return group_name_; }

    bool match_one(INode* other, const Match& match) const override;

private:
    std::string group_name_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
