// dotnet/decompiler/csharp/syntax/interpolated_string_expression.hpp -- an interpolated string, `$"a{b}c"`, with its content nodes (InterpolatedStringContent, Interpolation, InterpolatedStringText).
//
//     auto* s = make<InterpolatedStringExpression>(std::vector<InterpolatedStringContent*>{text, make<Interpolation>(expr)});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: InterpolatedStringContent, InterpolatedStringExpression, Interpolation, InterpolatedStringText, from Expressions/InterpolatedStringExpression.cs), Copyright AlphaSierraPapa for the
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

// The two kinds of content of an interpolated string: Interpolation and InterpolatedStringText.
class InterpolatedStringContent : public AstNode {
protected:
    InterpolatedStringContent() = default;
};

class InterpolatedStringExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::InterpolatedStringExpression;

    static constexpr std::string_view open_quote = "$\"";
    static constexpr std::string_view close_quote = "\"";

    static constexpr CSharpSlotInfoT<InterpolatedStringContent> content_slot{"Content", true, &Slots::content, true};

    InterpolatedStringExpression() = default;
    explicit InterpolatedStringExpression(const std::vector<InterpolatedStringContent*>& content);
    explicit InterpolatedStringExpression(std::initializer_list<InterpolatedStringContent*> content);

    AstNodeCollection<InterpolatedStringContent>& content() const { return content_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_interpolated_string_expression(*this); }
    AstNode* shallow_clone() const override { return make<InterpolatedStringExpression>(*this); }

private:
    mutable AstNodeCollection<InterpolatedStringContent> content_{this, Slots::content, 0, true};
};

class Interpolation final : public InterpolatedStringContent {
public:
    static constexpr AstNodeKind kind = AstNodeKind::Interpolation;

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    Interpolation() = default;
    explicit Interpolation(Expression* expression, int alignment = 0, std::optional<std::string> suffix = std::nullopt);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    int alignment() const { return alignment_; }
    const std::optional<std::string>& suffix() const { return suffix_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_interpolation(*this); }
    AstNode* shallow_clone() const override { return make<Interpolation>(*this); }

private:
    ChildPtr<Expression> expression_;
    int alignment_ = 0;
    std::optional<std::string> suffix_;
};

class InterpolatedStringText final : public InterpolatedStringContent {
public:
    static constexpr AstNodeKind kind = AstNodeKind::InterpolatedStringText;

    InterpolatedStringText() = default;
    explicit InterpolatedStringText(const std::string& text);

    std::string text() const { return text_; }
    void set_text(std::string value) { text_ = std::move(value); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;


protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_interpolated_string_text(*this); }
    AstNode* shallow_clone() const override { return make<InterpolatedStringText>(*this); }

private:
    std::string text_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
