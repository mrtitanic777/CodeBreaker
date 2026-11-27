// dotnet/decompiler/csharp/syntax/switch_expression.hpp -- a switch expression, `expr switch { pattern => value, ... }`, and its arms (SwitchExpressionSection).
//
//     auto* s = make<SwitchExpression>(expr, std::vector<SwitchExpressionSection*>{arm});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: SwitchExpression, SwitchExpressionSection, from Expressions/SwitchExpression.cs), Copyright AlphaSierraPapa for the
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

class SwitchExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::SwitchExpression;

    static constexpr std::string_view switch_keyword = "switch";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};
    static constexpr CSharpSlotInfoT<SwitchExpressionSection> switch_sections_slot{"SwitchSections", true, &Slots::switch_expression_section, true};

    SwitchExpression() = default;
    explicit SwitchExpression(Expression* expression);
    SwitchExpression(Expression* expression, const std::vector<SwitchExpressionSection*>& switch_sections);
    SwitchExpression(Expression* expression, std::initializer_list<SwitchExpressionSection*> switch_sections);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    AstNodeCollection<SwitchExpressionSection>& switch_sections() const { return switch_sections_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_switch_expression(*this); }
    AstNode* shallow_clone() const override { return make<SwitchExpression>(*this); }

private:
    ChildPtr<Expression> expression_;
    mutable AstNodeCollection<SwitchExpressionSection> switch_sections_{this, Slots::switch_expression_section, 1, true};
};

class SwitchExpressionSection final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::SwitchExpressionSection;

    static constexpr CSharpSlotInfoT<Expression> pattern_slot{"Pattern", false, &Slots::pattern, false};
    static constexpr CSharpSlotInfoT<Expression> body_slot{"Body", false, &Slots::switch_expression_body, false};

    SwitchExpressionSection() = default;
    SwitchExpressionSection(Expression* pattern, Expression* body);

    Expression* pattern() const { return pattern_; }
    void set_pattern(Expression* value);
    Expression* body() const { return body_; }
    void set_body(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_switch_expression_section(*this); }
    AstNode* shallow_clone() const override { return make<SwitchExpressionSection>(*this); }

private:
    ChildPtr<Expression> pattern_;
    ChildPtr<Expression> body_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
