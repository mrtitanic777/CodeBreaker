// dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp -- the node that stands for a Pattern inside a syntax tree, so that a pattern
// (AnyNode, Choice, Repeat ...) can be put where a node of the base class goes (a statement, an expression, a type ...) and matching
// the tree hands the decision to the pattern.
//
//     Expression* e = Expression::from_pattern(make<AnyNode>("x"));    // a PatternPlaceholderNode<Expression, ...>
//
// Upstream generates a private nested PatternPlaceholder class in each base marked [DecompilerAstNode(hasPatternPlaceholder: true)];
// this one template is all of them. A class that wants one declares, with its kind pair in ast_node_kind.hpp (the placeholder's kind
// right after the class's own):
//
//     static X* from_pattern(pattern_matching::Pattern* pattern) {
//         return pattern ? make<PatternPlaceholderNode<X, AstNodeKind::XPlaceholder>>(pattern) : nullptr;
//     }
//
// Base must be default constructible (all node classes are) and, if it is abstract, gets its pure virtuals from here (dispatch_to,
// do_match, shallow_clone, node_kind).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: the PatternPlaceholder classes generated into AstNode, Expression,
// Statement, AstType and the classes marked hasPatternPlaceholder), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/ipattern_placeholder.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

template <class Base, AstNodeKind PlaceholderKind>
class PatternPlaceholderNode final : public Base, public pattern_matching::IPatternPlaceholder {
public:
    explicit PatternPlaceholderNode(pattern_matching::Pattern* child) : child_(child) {}

    pattern_matching::Pattern* child() const { return child_; }

    AstNodeKind node_kind() const override { return PlaceholderKind; }

    bool do_match(AstNode* other, const pattern_matching::Match& match) const override { return child_->match_one(other, match); }
    bool match_in_collection(const pattern_matching::INodeList& other, std::size_t pos, const pattern_matching::Match& match,
                             pattern_matching::BacktrackingInfo& backtracking_info) const override {
        return child_->match_in_collection(other, pos, match, backtracking_info);
    }

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_pattern_placeholder(*this, *child_); }
    AstNode* shallow_clone() const override { return make<PatternPlaceholderNode>(*this); }

private:
    pattern_matching::Pattern* child_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
