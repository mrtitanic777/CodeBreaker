// dotnet/decompiler/csharp/syntax/trivia.hpp -- Trivia, the base of the comments (Comment) and preprocessor directives
// (PreProcessorDirective) that are attached to a node, before or after it, instead of being its children.
//
//     node->add_leading_trivia(make<Comment>("generated"));     // AstNode::add_leading_trivia / add_trailing_trivia
//     for (Trivia* t : node->leading_trivia()) ...
//
// Trivia are nodes, with a parent and an index, but they stay out of the owner's slot space: the owner keeps them in two lists (leading
// and trailing, among its annotations), and a trivia's next_sibling / prev_sibling walk the list it is in.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: Trivia, from GeneralScope/Trivia.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class Trivia : public AstNode {
public:
    TextLocation start_location() const override { return trivia_start_; }
    TextLocation end_location() const override { return trivia_end_; }
    void set_start_location(TextLocation value) { trivia_start_ = value; }
    void set_end_location(TextLocation value) { trivia_end_ = value; }

    // Attaches this trivia to `new_parent` as element `index` of `siblings` (the owner's leading or trailing list). For AstNode.
    void set_trivia_parent(AstNode* new_parent, std::vector<Trivia*>* siblings, int index);

protected:
    Trivia() = default;
    Trivia(TextLocation start, TextLocation end) : trivia_start_(start), trivia_end_(end) {}
    // A copy is detached: it is in no owner's list.
    Trivia(const Trivia& other) : AstNode(other), trivia_start_(other.trivia_start_), trivia_end_(other.trivia_end_) {}

private:
    friend class AstNode;
    // The leading or trailing list of the owning node that currently holds this trivia (nullptr while detached). Kept here, not on
    // AstNode, so that plain nodes do not pay for it.
    std::vector<Trivia*>* trivia_siblings_ = nullptr;
    TextLocation trivia_start_;
    TextLocation trivia_end_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
