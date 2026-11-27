// dotnet/decompiler/csharp/syntax/declaration_expression.hpp -- a declaration used as an expression, `var (a, b)` or `T x`.
//
//     auto* d = make<DeclarationExpression>(type, designation);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: DeclarationExpression, from Expressions/DeclarationExpression.cs), Copyright AlphaSierraPapa for the
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

class DeclarationExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::DeclarationExpression;

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<VariableDesignation> designation_slot{"Designation", false, &Slots::variable_designation, false};

    DeclarationExpression() = default;
    DeclarationExpression(AstType* type, VariableDesignation* designation);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    VariableDesignation* designation() const { return designation_; }
    void set_designation(VariableDesignation* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_declaration_expression(*this); }
    AstNode* shallow_clone() const override { return make<DeclarationExpression>(*this); }

private:
    ChildPtr<AstType> type_;
    ChildPtr<VariableDesignation> designation_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
