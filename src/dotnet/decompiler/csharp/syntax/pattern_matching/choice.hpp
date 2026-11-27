// dotnet/decompiler/csharp/syntax/pattern_matching/choice.hpp -- matches if any one of its alternatives does (the first that does wins;
// what a failed alternative recorded is undone).
//
//     auto* choice = make<Choice>();
//     choice->add(first_pattern);
//     choice->add("name", second_pattern);          // an alternative that records the node it matched under a name
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: Choice, from Choice.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class Choice : public Pattern {
public:
    // Adds an alternative recorded under `name` (a NamedNode around it). Null is std::invalid_argument.
    void add(const std::string& name, INode* alternative);
    // Adds an alternative. Null is std::invalid_argument.
    void add(INode* alternative);

    bool match_one(INode* other, const Match& match) const override;

    // The alternatives in the order added (upstream: IEnumerable<INode>).
    auto begin() const { return alternatives_.begin(); }
    auto end() const { return alternatives_.end(); }

private:
    std::vector<INode*> alternatives_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
