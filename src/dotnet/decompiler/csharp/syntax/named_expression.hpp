// dotnet/decompiler/csharp/syntax/named_expression.hpp -- a member initializer inside an object initializer, `Name = expr`.
//
//     auto* n = make<NamedExpression>("Name", expr);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: NamedExpression, from Expressions/NamedExpression.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/syntax/tokens.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class NamedExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::NamedExpression;

    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    NamedExpression() = default;
    NamedExpression(const std::string& name, Expression* expression);

    Identifier* name_token() const { return name_token_; }
    void set_name_token(Identifier* value);
    std::string name() const { return name_token_ != nullptr ? name_token_->name() : std::string(); }
    void set_name(const std::string& value);
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_named_expression(*this); }
    AstNode* shallow_clone() const override { return make<NamedExpression>(*this); }

private:
    ChildPtr<Identifier> name_token_;
    ChildPtr<Expression> expression_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
