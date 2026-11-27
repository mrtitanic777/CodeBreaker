// dotnet/decompiler/csharp/syntax/goto_statement.hpp -- GotoStatement, GotoCaseStatement and GotoDefaultStatement: `goto label;`, `goto
// case value;` and `goto default;`.
//
//     Statement* stmt = make<GotoStatement>("loop_start");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: GotoStatement, GotoCaseStatement, GotoDefaultStatement, from
// Statements/GotoStatement.cs with the members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class GotoStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::GotoStatement;
    static constexpr std::string_view goto_keyword = "goto";

    static constexpr CSharpSlotInfoT<Identifier> label_token_slot{"LabelToken", false, &Slots::identifier, true};

    GotoStatement();
    explicit GotoStatement(const std::string& label_value);

    Identifier* label_token() const { return label_token_; }
    void set_label_token(Identifier* value);
    std::string label() const { return label_token_ != nullptr ? label_token_->name() : std::string(); }
    void set_label(const std::string& value) { set_label_token(Identifier::create_if_not_empty(value)); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_goto_statement(*this); }
    AstNode* shallow_clone() const override { return make<GotoStatement>(*this); }

private:
    ChildPtr<Identifier> label_token_;
};

class GotoCaseStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::GotoCaseStatement;
    static constexpr std::string_view goto_keyword = "goto";
    static constexpr std::string_view case_keyword = "case";

    static constexpr CSharpSlotInfoT<Expression> label_expression_slot{"LabelExpression", false, &Slots::expression, false};

    GotoCaseStatement();
    explicit GotoCaseStatement(Expression* label_expression_value);

    Expression* label_expression() const { return label_expression_; }
    void set_label_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_goto_case_statement(*this); }
    AstNode* shallow_clone() const override { return make<GotoCaseStatement>(*this); }

private:
    ChildPtr<Expression> label_expression_;
};

class GotoDefaultStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::GotoDefaultStatement;
    static constexpr std::string_view goto_keyword = "goto";
    static constexpr std::string_view default_keyword = "default";



    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_goto_default_statement(*this); }
    AstNode* shallow_clone() const override { return make<GotoDefaultStatement>(*this); }

private:
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
