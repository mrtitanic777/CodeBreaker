// dotnet/decompiler/csharp/syntax/out_var_declaration_expression.hpp -- an out variable declaration inside an argument list, `out T name`.
//
//     auto* o = make<OutVarDeclarationExpression>(type, "name");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: OutVarDeclarationExpression, from Expressions/OutVarDeclarationExpression.cs), Copyright AlphaSierraPapa for the
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
#include "dotnet/decompiler/csharp/syntax/direction_expression.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class OutVarDeclarationExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::OutVarDeclarationExpression;

    static constexpr std::string_view out_keyword = "out";

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<VariableInitializer> variable_slot{"Variable", false, &Slots::variable, false};

    OutVarDeclarationExpression() = default;
    OutVarDeclarationExpression(AstType* type, const std::string& name);
    OutVarDeclarationExpression(AstType* type, VariableInitializer* variable);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    VariableInitializer* variable() const { return variable_; }
    void set_variable(VariableInitializer* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_out_var_declaration_expression(*this); }
    AstNode* shallow_clone() const override { return make<OutVarDeclarationExpression>(*this); }

private:
    ChildPtr<AstType> type_;
    ChildPtr<VariableInitializer> variable_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
