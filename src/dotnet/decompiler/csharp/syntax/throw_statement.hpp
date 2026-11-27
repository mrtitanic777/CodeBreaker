// dotnet/decompiler/csharp/syntax/throw_statement.hpp -- ThrowStatement: `throw [expression];`.
//
//     Statement* stmt = make<ThrowStatement>(exception);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ThrowStatement, from Statements/ThrowStatement.cs with the members ILSpy 11's
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

class ThrowStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ThrowStatement;
    static constexpr std::string_view throw_keyword = "throw";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, true};

    ThrowStatement();
    explicit ThrowStatement(Expression* expression_value);

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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_throw_statement(*this); }
    AstNode* shallow_clone() const override { return make<ThrowStatement>(*this); }

private:
    ChildPtr<Expression> expression_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
