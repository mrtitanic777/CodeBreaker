// dotnet/decompiler/csharp/syntax/identifier_expression.hpp -- a simple name used as an expression, `x` or `F<int>`.
//
//     auto* id = make<IdentifierExpression>("x");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: IdentifierExpression, from Expressions/IdentifierExpression.cs), Copyright AlphaSierraPapa for the
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

class IdentifierExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::IdentifierExpression;

    static constexpr CSharpSlotInfoT<Identifier> identifier_token_slot{"IdentifierToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<AstType> type_arguments_slot{"TypeArguments", true, &Slots::type_argument, true};

    IdentifierExpression() = default;
    IdentifierExpression(const std::string& identifier, TextLocation location);
    explicit IdentifierExpression(const std::string& identifier);
    IdentifierExpression(const std::string& identifier, const std::vector<AstType*>& type_arguments);
    IdentifierExpression(const std::string& identifier, std::initializer_list<AstType*> type_arguments);

    Identifier* identifier_token() const { return identifier_token_; }
    void set_identifier_token(Identifier* value);
    std::string identifier() const { return identifier_token_ != nullptr ? identifier_token_->name() : std::string(); }
    void set_identifier(const std::string& value);
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_identifier_expression(*this); }
    AstNode* shallow_clone() const override { return make<IdentifierExpression>(*this); }

private:
    ChildPtr<Identifier> identifier_token_;
    mutable AstNodeCollection<AstType> type_arguments_{this, Slots::type_argument, 1, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
