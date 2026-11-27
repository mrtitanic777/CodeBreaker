// dotnet/decompiler/csharp/syntax/for_statement.hpp -- ForStatement: `for (initializers; condition; iterators) embedded`.
//
//     Statement* stmt = make<ForStatement>(initializers, condition, iterators, body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ForStatement, from Statements/ForStatement.cs with the members ILSpy 11's source
// generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class ForStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ForStatement;
    static constexpr std::string_view for_keyword = "for";

    static constexpr CSharpSlotInfoT<Statement> initializers_slot{"Initializers", true, &Slots::for_initializer, true};
    static constexpr CSharpSlotInfoT<Expression> condition_slot{"Condition", false, &Slots::condition, true};
    static constexpr CSharpSlotInfoT<Statement> iterators_slot{"Iterators", true, &Slots::iterator, true};
    static constexpr CSharpSlotInfoT<Statement> embedded_statement_slot{"EmbeddedStatement", false, &Slots::embedded_statement, false};

    ForStatement();
    ForStatement(const std::vector<Statement*>& initializers_value, Expression* condition_value, const std::vector<Statement*>& iterators_value, Statement* embedded_statement_value);

    AstNodeCollection<Statement>& initializers() const { return initializers_; }
    Expression* condition() const { return condition_; }
    void set_condition(Expression* value);
    AstNodeCollection<Statement>& iterators() const { return iterators_; }
    Statement* embedded_statement() const { return embedded_statement_; }
    void set_embedded_statement(Statement* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_for_statement(*this); }
    AstNode* shallow_clone() const override { return make<ForStatement>(*this); }

private:
    mutable AstNodeCollection<Statement> initializers_{this, Slots::for_initializer, 0, false};
    ChildPtr<Expression> condition_;
    mutable AstNodeCollection<Statement> iterators_{this, Slots::iterator, 2, false};
    ChildPtr<Statement> embedded_statement_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
