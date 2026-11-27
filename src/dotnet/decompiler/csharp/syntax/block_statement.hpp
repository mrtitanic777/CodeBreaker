// dotnet/decompiler/csharp/syntax/block_statement.hpp -- BlockStatement: `{ statement; ... }`, a list of statements in braces.
//
//     auto* block = make<BlockStatement>();
//     block->add(make<ReturnStatement>());
//     for (Statement* s : *block) ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: BlockStatement, from Statements/BlockStatement.cs with the members ILSpy 11's
// source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class BlockStatement : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::BlockStatement;
    static constexpr CSharpSlotInfoT<Statement> statements_slot{"Statements", true, &Slots::statement, true};

    BlockStatement();
    explicit BlockStatement(const std::vector<Statement*>& statements_value);

    AstNodeCollection<Statement>& statements() const { return statements_; }

    // Adds `statement` at the end; an expression is added as an ExpressionStatement.
    void add(Statement* statement);
    void add(Expression* expression);

    // The statements as a range (upstream's IEnumerable<Statement>).
    AstNodeIterator<Statement> begin() const { return statements_.begin(); }
    std::default_sentinel_t end() const { return std::default_sentinel; }

    // The node that stands for `pattern` where a BlockStatement goes (upstream's implicit conversion Pattern -> BlockStatement).
    static BlockStatement* from_pattern(pattern_matching::Pattern* pattern);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_block_statement(*this); }
    AstNode* shallow_clone() const override { return make<BlockStatement>(*this); }

private:
    mutable AstNodeCollection<Statement> statements_{this, Slots::statement, 0, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
