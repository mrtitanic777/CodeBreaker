// dotnet/decompiler/csharp/transforms/normalize_block_statements.hpp -- NormalizeBlockStatements: puts braces around the embedded
// statements of if/while/for/... (or, when the settings do not always use braces, takes them off where a single statement is allowed),
// turns a lone namespace into a file-scoped one, and writes a getter-only property or indexer whose getter just returns an expression
// as an expression-bodied member.
//
//     NormalizeBlockStatements transform;
//     transform.run(root_node, context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Transforms: NormalizeBlockStatements, from NormalizeBlockStatements.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/iast_transform.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

class NormalizeBlockStatements final : public syntax::DepthFirstAstVisitor, public IAstTransform {
public:
    void run(syntax::AstNode* root_node, TransformContext& context) override;

    void visit_syntax_tree(syntax::SyntaxTree& syntax_tree) override;
    void visit_namespace_declaration(syntax::NamespaceDeclaration& namespace_declaration) override;
    void visit_if_else_statement(syntax::IfElseStatement& if_else_statement) override;
    void visit_while_statement(syntax::WhileStatement& while_statement) override;
    void visit_do_while_statement(syntax::DoWhileStatement& do_while_statement) override;
    void visit_foreach_statement(syntax::ForeachStatement& foreach_statement) override;
    void visit_for_statement(syntax::ForStatement& for_statement) override;
    void visit_fixed_statement(syntax::FixedStatement& fixed_statement) override;
    void visit_lock_statement(syntax::LockStatement& lock_statement) override;
    void visit_using_statement(syntax::UsingStatement& using_statement) override;
    void visit_property_declaration(syntax::PropertyDeclaration& property_declaration) override;
    void visit_indexer_declaration(syntax::IndexerDeclaration& indexer_declaration) override;

private:
    void do_transform(syntax::Statement* statement, syntax::Statement* parent);
    static bool is_else_if(syntax::Statement* statement, syntax::Statement* parent);
    void insert_block(syntax::Statement* statement);
    static bool is_allowed_as_embedded_statement(syntax::Statement* statement, syntax::Statement* parent);
    void simplify_property_declaration(syntax::PropertyDeclaration* property_declaration);
    void simplify_indexer_declaration(syntax::IndexerDeclaration* indexer_declaration);

    TransformContext* context_ = nullptr;
    bool has_namespace_ = false;
    syntax::NamespaceDeclaration* single_namespace_declaration_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::csharp::transforms
