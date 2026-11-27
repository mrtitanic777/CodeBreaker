// dotnet/decompiler/csharp/syntax/pattern_matching/inode.hpp -- what can take part in a pattern match: a syntax node (AstNode) or a
// pattern (Pattern). A pattern is a tree of nodes and patterns; matching it against a tree of nodes either fails or records the named
// nodes in a Match.
//
//     Match m = match(pattern, node);
//     bool ok = is_match(pattern, node);
//
// `match_one` is upstream's DoMatch(INode, Match): does this node or pattern match `other` (null: the absent child)? A node matches
// a node of the same class whose children match; a pattern decides for itself. `match_in_collection` is DoMatchCollection: match
// starting at element `pos` of a list of children, where one pattern (Repeat, OptionalNode) can stand for several elements or none.
// A node class's own comparison of two nodes is AstNode::do_match(AstNode*, const Match&).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax.PatternMatching: INode and PatternExtensions.Match / IsMatch, from INode.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The extensions that build nodes (ToType,
// ToExpression, ToStatement, WithName) are in pattern_extensions.hpp.
#pragma once

#include <cstddef>

#include "dotnet/decompiler/csharp/syntax/pattern_matching/backtracking_info.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/match.hpp"

namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching {

class INode;

// A read-only list of INode: the children of one collection slot (AstNodeCollectionBase implements it).
class INodeList {
public:
    virtual ~INodeList() = default;
    virtual std::size_t size() const = 0;
    virtual INode* node_at(std::size_t index) const = 0;
};

class INode {
public:
    virtual ~INode() = default;

    // Whether this matches `other` (nullptr: the child is absent), recording named nodes in `match`.
    virtual bool match_one(INode* other, const Match& match) const = 0;

    // Matches starting at element `pos` of `other`, which may be past the end; true when this consumed exactly element `pos` (a
    // pattern that stands for several elements pushes the other possibilities on `backtracking_info` and returns false).
    virtual bool match_in_collection(const INodeList& other, std::size_t pos, const Match& match,
                                     BacktrackingInfo& backtracking_info) const = 0;
};

// Matches `pattern` against `other`: the Match of what it recorded, or the failure (upstream's `pattern.Match(other)`).
inline Match match(const INode* pattern, INode* other) {
    Match m = Match::create_new();
    if (pattern->match_one(other, m)) return m;
    return Match();
}

// Whether `pattern` matches `other` (upstream's `pattern.IsMatch(other)`).
inline bool is_match(const INode* pattern, INode* other) { return pattern->match_one(other, Match::create_new()); }

}  // namespace cb::dotnet::decompiler::csharp::syntax::pattern_matching
