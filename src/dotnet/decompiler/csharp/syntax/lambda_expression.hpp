// dotnet/decompiler/csharp/syntax/lambda_expression.hpp -- a lambda, `(x) => body`; the body is an expression or a block.
//
//     auto* l = make<LambdaExpression>(attributes, parameters, body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: LambdaExpression, from Expressions/LambdaExpression.cs), Copyright AlphaSierraPapa for the
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

class LambdaExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::LambdaExpression;

    static constexpr std::string_view async_modifier = "async";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> parameters_slot{"Parameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<AstNode> body_slot{"Body", false, &Slots::lambda_body, false};

    LambdaExpression() = default;
    LambdaExpression(const std::vector<AttributeSection*>& attributes, const std::vector<ParameterDeclaration*>& parameters, AstNode* body);

    AstNodeCollection<AttributeSection>& attributes() const { return attributes_; }
    AstNodeCollection<ParameterDeclaration>& parameters() const { return parameters_; }
    AstNode* body() const { return body_; }
    void set_body(AstNode* value);
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_lambda_expression(*this); }
    AstNode* shallow_clone() const override { return make<LambdaExpression>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    mutable AstNodeCollection<ParameterDeclaration> parameters_{this, Slots::parameter, 1, false};
    ChildPtr<AstNode> body_;
    bool is_async_ = false;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
