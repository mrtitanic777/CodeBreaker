// dotnet/decompiler/csharp/syntax/fixed_variable_initializer.hpp -- FixedVariableInitializer: the `name[count]` of a fixed-size buffer.
//
//     auto* v = make<FixedVariableInitializer>("buffer", count);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: FixedVariableInitializer, from TypeMembers/FixedVariableInitializer.cs with the
// members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class FixedVariableInitializer final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::FixedVariableInitializer;
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<Expression> count_expression_slot{"CountExpression", false, &Slots::expression, false};

    FixedVariableInitializer();
    FixedVariableInitializer(const std::string& name_value, Expression* count_expression_value);

    Identifier* name_token() const { return name_token_; }
    void set_name_token(Identifier* value);
    std::string name() const { return name_token_->name(); }
    void set_name(const std::string& value) { set_name_token(Identifier::create(value)); }
    Expression* count_expression() const { return count_expression_; }
    void set_count_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_fixed_variable_initializer(*this); }
    AstNode* shallow_clone() const override { return make<FixedVariableInitializer>(*this); }

private:
    ChildPtr<Identifier> name_token_;
    ChildPtr<Expression> count_expression_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
