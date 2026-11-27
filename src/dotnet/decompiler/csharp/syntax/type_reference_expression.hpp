// dotnet/decompiler/csharp/syntax/type_reference_expression.hpp -- a type used where an expression is expected, as the target of a static member access.
//
//     auto* t = make<TypeReferenceExpression>(type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: TypeReferenceExpression, from Expressions/TypeReferenceExpression.cs), Copyright AlphaSierraPapa for the
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

class TypeReferenceExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::TypeReferenceExpression;

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};

    TypeReferenceExpression() = default;
    explicit TypeReferenceExpression(AstType* type);

    AstType* type() const { return type_; }
    void set_type(AstType* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_type_reference_expression(*this); }
    AstNode* shallow_clone() const override { return make<TypeReferenceExpression>(*this); }

private:
    ChildPtr<AstType> type_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
