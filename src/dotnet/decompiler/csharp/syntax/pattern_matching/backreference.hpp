// dotnet/decompiler/csharp/syntax/pattern_matching/backreference.hpp -- matches what an earlier named group matched again: a node
// that matches the group's last captured node (or an absent child, if the group captured nothing).
//
//     auto* same = make<Backreference>("x");       // after a NamedNode("x", ...) earlier in the pattern
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: Backreference, from Backreference.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class Backreference : public Pattern {
public:
    explicit Backreference(std::string name) : referenced_group_name_(std::move(name)) {}

    const std::string& referenced_group_name() const { return referenced_group_name_; }

    bool match_one(INode* other, const Match& match) const override;

private:
    std::string referenced_group_name_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
