// dotnet/decompiler/csharp/syntax/anonymous_method_expression.hpp -- an anonymous method, `delegate (int x) { ... }`.
//
//     auto* m = make<AnonymousMethodExpression>(params, body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: AnonymousMethodExpression, from Expressions/AnonymousMethodExpression.cs), Copyright AlphaSierraPapa for the
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
#include "dotnet/decompiler/csharp/syntax/lambda_expression.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class AnonymousMethodExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::AnonymousMethodExpression;

    static constexpr std::string_view delegate_keyword = "delegate";
    static constexpr std::string_view async_modifier = "async";

    static constexpr CSharpSlotInfoT<ParameterDeclaration> parameters_slot{"Parameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<BlockStatement> body_slot{"Body", false, &Slots::body, false};

    AnonymousMethodExpression() = default;
    AnonymousMethodExpression(const std::vector<ParameterDeclaration*>& parameters, BlockStatement* body);

    AstNodeCollection<ParameterDeclaration>& parameters() const { return parameters_; }
    BlockStatement* body() const { return body_; }
    void set_body(BlockStatement* value);
    bool is_async() const { return is_async_; }
    void set_is_async(bool value) { is_async_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_anonymous_method_expression(*this); }
    AstNode* shallow_clone() const override { return make<AnonymousMethodExpression>(*this); }

private:
    mutable AstNodeCollection<ParameterDeclaration> parameters_{this, Slots::parameter, 0, false};
    ChildPtr<BlockStatement> body_;
    bool is_async_ = false;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
