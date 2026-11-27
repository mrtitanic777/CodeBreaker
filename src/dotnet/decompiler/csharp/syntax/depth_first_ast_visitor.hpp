// dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp -- AST visitors that by default visit every node depth-first: each visit
// method visits the node's children (visit_children). A visitor derives from one and overrides the visit methods of the classes it cares
// about, calling visit_children to go on into the node.
//
//     struct CountCalls : DepthFirstAstVisitor {
//         int calls = 0;
//         void visit_invocation_expression(InvocationExpression& e) override { ++calls; visit_children(e); }
//     };
//     struct Printer : DepthFirstAstVisitorT<std::string> { ... };               // visit methods return a std::string
//     struct Depth : DepthFirstAstVisitorTS<int, int> { ... };                   // take an int, return an int
//
// The children are walked in document order with AstNode::children(), which lets a visit method remove or replace the node it is
// visiting. The typed versions return a default constructed S from visit_children (upstream's default(S)) and ignore what the children
// return.
//
// The visit methods take their node by a type that depends on a template parameter, so these classes can be declared where the node
// classes are not yet defined; a translation unit that makes a visitor needs every node class's header (syntax.hpp).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: DepthFirstAstVisitor, DepthFirstAstVisitor<S>, DepthFirstAstVisitor<T, S>,
// from DepthFirstAstVisitor.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_visitor.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

namespace detail {
// `Class`, spelled so that it depends on the template parameter `Dummy` (see the file comment).
template <class Dummy, class Class>
struct Dependent {
    using type = Class;
};
template <class Dummy, class Class>
using DependentT = typename Dependent<Dummy, Class>::type;
}  // namespace detail

template <class Dummy>
class BasicDepthFirstAstVisitor : public IAstVisitor {
protected:
    // Visits the children of `node` in order.
    virtual void visit_children(AstNode& node) {
        // Children enumerates in document order and tolerates the visitor removing or replacing the current child.
        for (AstNode* child : node.children()) child->accept_visitor(static_cast<IAstVisitor&>(*this));
    }

public:
#define CB_AST_VISIT_DEF(Class, name) \
    void visit_##name(detail::DependentT<Dummy, Class>& node) override { visit_children(node); }
    CB_CSHARP_AST_VISITS(CB_AST_VISIT_DEF)
#undef CB_AST_VISIT_DEF
    void visit_error_node(AstNode& error_node) override { visit_children(error_node); }
    void visit_pattern_placeholder(AstNode& placeholder, pattern_matching::Pattern&) override { visit_children(placeholder); }
};

// The visitor that returns nothing.
using DepthFirstAstVisitor = BasicDepthFirstAstVisitor<void>;

template <class S, class Dummy = void>
class BasicDepthFirstAstVisitorT : public IAstVisitorT<S> {
protected:
    // Visits the children of `node` in order; returns a default constructed S.
    virtual S visit_children(AstNode& node) {
        for (AstNode* child : node.children()) child->accept_visitor(static_cast<IAstVisitorT<S>&>(*this));
        return S{};
    }

public:
#define CB_AST_VISIT_DEF(Class, name) \
    S visit_##name(detail::DependentT<Dummy, Class>& node) override { return visit_children(node); }
    CB_CSHARP_AST_VISITS(CB_AST_VISIT_DEF)
#undef CB_AST_VISIT_DEF
    S visit_error_node(AstNode& error_node) override { return visit_children(error_node); }
    S visit_pattern_placeholder(AstNode& placeholder, pattern_matching::Pattern&) override { return visit_children(placeholder); }
};

template <class S>
using DepthFirstAstVisitorT = BasicDepthFirstAstVisitorT<S>;

template <class T, class S, class Dummy = void>
class BasicDepthFirstAstVisitorTS : public IAstVisitorTS<T, S> {
protected:
    // Visits the children of `node` in order, passing `data`; returns a default constructed S.
    virtual S visit_children(AstNode& node, T data) {
        for (AstNode* child : node.children()) child->accept_visitor(static_cast<IAstVisitorTS<T, S>&>(*this), data);
        return S{};
    }

public:
#define CB_AST_VISIT_DEF(Class, name) \
    S visit_##name(detail::DependentT<Dummy, Class>& node, T data) override { return visit_children(node, std::move(data)); }
    CB_CSHARP_AST_VISITS(CB_AST_VISIT_DEF)
#undef CB_AST_VISIT_DEF
    S visit_error_node(AstNode& error_node, T data) override { return visit_children(error_node, std::move(data)); }
    S visit_pattern_placeholder(AstNode& placeholder, pattern_matching::Pattern&, T data) override {
        return visit_children(placeholder, std::move(data));
    }
};

template <class T, class S>
using DepthFirstAstVisitorTS = BasicDepthFirstAstVisitorTS<T, S>;

}  // namespace cb::dotnet::decompiler::csharp::syntax
