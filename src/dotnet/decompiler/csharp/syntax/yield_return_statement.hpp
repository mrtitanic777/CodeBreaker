// dotnet/decompiler/csharp/syntax/yield_return_statement.hpp -- YieldReturnStatement: `yield return expression;`.
//
//     Statement* stmt = make<YieldReturnStatement>(value);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: YieldReturnStatement, from Statements/YieldReturnStatement.cs with the members
// ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class YieldReturnStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::YieldReturnStatement;
    static constexpr std::string_view yield_keyword = "yield";
    static constexpr std::string_view return_keyword = "return";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    YieldReturnStatement();
    explicit YieldReturnStatement(Expression* expression_value);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_yield_return_statement(*this); }
    AstNode* shallow_clone() const override { return make<YieldReturnStatement>(*this); }

private:
    ChildPtr<Expression> expression_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
