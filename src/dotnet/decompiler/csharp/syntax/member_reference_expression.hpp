// dotnet/decompiler/csharp/syntax/member_reference_expression.hpp -- a member access, `target.Name` or `target.Name<T>`.
//
//     auto* m = make<MemberReferenceExpression>(target, "Name");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: MemberReferenceExpression, from Expressions/MemberReferenceExpression.cs), Copyright AlphaSierraPapa for the
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

class MemberReferenceExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::MemberReferenceExpression;

    static constexpr CSharpSlotInfoT<Expression> target_slot{"Target", false, &Slots::target_expression, false};
    static constexpr CSharpSlotInfoT<Identifier> member_name_token_slot{"MemberNameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<AstType> type_arguments_slot{"TypeArguments", true, &Slots::type_argument, true};

    MemberReferenceExpression() = default;
    MemberReferenceExpression(Expression* target, const std::string& member_name);
    MemberReferenceExpression(Expression* target, const std::string& member_name, const std::vector<AstType*>& type_arguments);
    MemberReferenceExpression(Expression* target, const std::string& member_name, std::initializer_list<AstType*> type_arguments);

    Expression* target() const { return target_; }
    void set_target(Expression* value);
    Identifier* member_name_token() const { return member_name_token_; }
    void set_member_name_token(Identifier* value);
    std::string member_name() const { return member_name_token_ != nullptr ? member_name_token_->name() : std::string(); }
    void set_member_name(const std::string& value);
    AstNodeCollection<AstType>& type_arguments() const { return type_arguments_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_member_reference_expression(*this); }
    AstNode* shallow_clone() const override { return make<MemberReferenceExpression>(*this); }

private:
    ChildPtr<Expression> target_;
    ChildPtr<Identifier> member_name_token_;
    mutable AstNodeCollection<AstType> type_arguments_{this, Slots::type_argument, 2, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
