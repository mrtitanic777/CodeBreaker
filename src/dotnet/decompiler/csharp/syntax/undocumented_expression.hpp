// dotnet/decompiler/csharp/syntax/undocumented_expression.hpp -- the undocumented `__arglist`, `__refvalue`, `__reftype` and `__makeref` expressions.
//
//     auto* u = make<UndocumentedExpression>(UndocumentedExpressionType::MakeRef, std::vector<Expression*>{x});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: UndocumentedExpression, from Expressions/UndocumentedExpression.cs), Copyright AlphaSierraPapa for the
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

enum class UndocumentedExpressionType : std::uint8_t {
    ArgListAccess,  // __arglist
    ArgList,        // __arglist (a1, a2, ..., an)
    RefValue,       // __refvalue (expr , type)
    RefType,        // __reftype (expr)
    MakeRef,        // __makeref (expr)
};

class UndocumentedExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::UndocumentedExpression;

    static constexpr std::string_view arglist_keyword = "__arglist";
    static constexpr std::string_view refvalue_keyword = "__refvalue";
    static constexpr std::string_view reftype_keyword = "__reftype";
    static constexpr std::string_view makeref_keyword = "__makeref";

    static constexpr CSharpSlotInfoT<Expression> arguments_slot{"Arguments", true, &Slots::argument, true};

    UndocumentedExpression() = default;
    explicit UndocumentedExpression(UndocumentedExpressionType undocumented_expression_type);
    UndocumentedExpression(UndocumentedExpressionType undocumented_expression_type, const std::vector<Expression*>& arguments);
    UndocumentedExpression(UndocumentedExpressionType undocumented_expression_type, std::initializer_list<Expression*> arguments);

    AstNodeCollection<Expression>& arguments() const { return arguments_; }
    UndocumentedExpressionType undocumented_expression_type() const { return undocumented_expression_type_; }
    void set_undocumented_expression_type(UndocumentedExpressionType value) { undocumented_expression_type_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_undocumented_expression(*this); }
    AstNode* shallow_clone() const override { return make<UndocumentedExpression>(*this); }

private:
    mutable AstNodeCollection<Expression> arguments_{this, Slots::argument, 0, true};
    UndocumentedExpressionType undocumented_expression_type_ = UndocumentedExpressionType{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
