// dotnet/decompiler/csharp/syntax/recursive_pattern_expression.hpp -- a recursive pattern, `T { A: 1 } name` or `T (a, b)`.
//
//     auto* p = make<RecursivePatternExpression>(type, std::vector<Expression*>{sub});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: RecursivePatternExpression, from Expressions/RecursivePatternExpression.cs), Copyright AlphaSierraPapa for the
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

class RecursivePatternExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::RecursivePatternExpression;

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, true};
    static constexpr CSharpSlotInfoT<Expression> sub_patterns_slot{"SubPatterns", true, &Slots::sub_pattern, true};
    static constexpr CSharpSlotInfoT<VariableDesignation> designation_slot{"Designation", false, &Slots::variable_designation, true};

    RecursivePatternExpression() = default;
    RecursivePatternExpression(AstType* type, const std::vector<Expression*>& sub_patterns);
    RecursivePatternExpression(AstType* type, std::initializer_list<Expression*> sub_patterns);
    RecursivePatternExpression(AstType* type, const std::vector<Expression*>& sub_patterns, VariableDesignation* designation);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    AstNodeCollection<Expression>& sub_patterns() const { return sub_patterns_; }
    VariableDesignation* designation() const { return designation_; }
    void set_designation(VariableDesignation* value);
    bool is_positional() const { return is_positional_; }
    void set_is_positional(bool value) { is_positional_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_recursive_pattern_expression(*this); }
    AstNode* shallow_clone() const override { return make<RecursivePatternExpression>(*this); }

private:
    ChildPtr<AstType> type_;
    mutable AstNodeCollection<Expression> sub_patterns_{this, Slots::sub_pattern, 1, false};
    ChildPtr<VariableDesignation> designation_;
    bool is_positional_ = false;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
