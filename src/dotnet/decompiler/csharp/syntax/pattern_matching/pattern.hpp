// dotnet/decompiler/csharp/syntax/pattern_matching/pattern.hpp -- the base of the pattern classes (AnyNode, NamedNode, Choice, Repeat,
// OptionalNode, Backreference ...): INodes that are not syntax nodes but decide for themselves what they match. A pattern goes into a
// tree through a placeholder node (AstNode::from_pattern, Expression::from_pattern ...), so a pattern tree is a tree of nodes with
// patterns at the leaves it wants to be free at.
//
//     auto* pattern = make<NamedNode>("x", make<AnyNode>());              // all patterns are made in the current ILArena
//     Pattern::match_string(pattern_text, text)                          // "$any$" matches any text
//
// Patterns live in an ILArena like nodes do; a pattern that is built once and used by every run (upstream keeps them in static
// readonly fields) belongs to an arena that outlives them all.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: Pattern, from Pattern.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string_view>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/inode.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class Pattern : public il::ArenaObject, public INode {
public:
    // The text that matches any text in match_string.
    static constexpr std::string_view any_string = "$any$";

    // Whether the pattern text is any_string or equals the text. (An absent name and an empty one are the same here.)
    static bool match_string(std::string_view pattern, std::string_view text) { return pattern == any_string || pattern == text; }

    // A pattern matches element `pos` alone, unless it overrides this to stand for several (Repeat, OptionalNode).
    bool match_in_collection(const INodeList& other, std::size_t pos, const Match& match,
                             BacktrackingInfo& backtracking_info) const override;

    // Matches a whole list of pattern children against a list of nodes, backtracking over the alternatives the children pushed on the
    // stack; true when every element of `other_children` was consumed.
    static bool match_collections(const INodeList& pattern_children, const INodeList& other_children, const Match& match);

protected:
    Pattern() = default;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
