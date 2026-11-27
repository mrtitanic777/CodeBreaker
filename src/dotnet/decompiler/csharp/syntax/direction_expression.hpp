// dotnet/decompiler/csharp/syntax/direction_expression.hpp -- an argument passed by `ref`, `out` or `in`, and the FieldDirection of such an argument.
//
//     auto* d = make<DirectionExpression>(FieldDirection::Ref, expr);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: DirectionExpression, from Expressions/DirectionExpression.cs), Copyright AlphaSierraPapa for the
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

enum class FieldDirection : std::uint8_t {
    None,
    Out,
    Ref,
    In,
};

const char* to_string(FieldDirection direction);

class DirectionExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::DirectionExpression;

    static constexpr std::string_view ref_keyword = "ref";
    static constexpr std::string_view out_keyword = "out";
    static constexpr std::string_view in_keyword = "in";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    DirectionExpression() = default;
    DirectionExpression(FieldDirection field_direction, Expression* expression);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    FieldDirection field_direction() const { return field_direction_; }
    void set_field_direction(FieldDirection value) { field_direction_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_direction_expression(*this); }
    AstNode* shallow_clone() const override { return make<DirectionExpression>(*this); }

private:
    ChildPtr<Expression> expression_;
    FieldDirection field_direction_ = FieldDirection{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
