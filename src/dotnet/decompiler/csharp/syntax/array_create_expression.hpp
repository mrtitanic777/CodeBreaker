// dotnet/decompiler/csharp/syntax/array_create_expression.hpp -- an array creation, `new int[5][]` or `new[] { 1, 2 }`.
//
//     auto* n = make<ArrayCreateExpression>(type, std::vector<Expression*>{size});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ArrayCreateExpression, from Expressions/ArrayCreateExpression.cs), Copyright AlphaSierraPapa for the
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

class ArrayCreateExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ArrayCreateExpression;

    static constexpr std::string_view new_keyword = "new";

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, true};
    static constexpr CSharpSlotInfoT<Expression> arguments_slot{"Arguments", true, &Slots::argument, true};
    static constexpr CSharpSlotInfoT<ArraySpecifier> additional_array_specifiers_slot{"AdditionalArraySpecifiers", true, &Slots::additional_array_specifier, true};
    static constexpr CSharpSlotInfoT<ArrayInitializerExpression> initializer_slot{"Initializer", false, &Slots::initializer, true};

    ArrayCreateExpression() = default;
    ArrayCreateExpression(AstType* type, const std::vector<Expression*>& arguments);
    ArrayCreateExpression(AstType* type, std::initializer_list<Expression*> arguments);
    ArrayCreateExpression(AstType* type, const std::vector<Expression*>& arguments, const std::vector<ArraySpecifier*>& additional_array_specifiers);
    ArrayCreateExpression(AstType* type, const std::vector<Expression*>& arguments, std::initializer_list<ArraySpecifier*> additional_array_specifiers);
    ArrayCreateExpression(AstType* type, const std::vector<Expression*>& arguments, const std::vector<ArraySpecifier*>& additional_array_specifiers, ArrayInitializerExpression* initializer);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    AstNodeCollection<Expression>& arguments() const { return arguments_; }
    AstNodeCollection<ArraySpecifier>& additional_array_specifiers() const { return additional_array_specifiers_; }
    ArrayInitializerExpression* initializer() const { return initializer_; }
    void set_initializer(ArrayInitializerExpression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_array_create_expression(*this); }
    AstNode* shallow_clone() const override { return make<ArrayCreateExpression>(*this); }

private:
    ChildPtr<AstType> type_;
    mutable AstNodeCollection<Expression> arguments_{this, Slots::argument, 1, false};
    mutable AstNodeCollection<ArraySpecifier> additional_array_specifiers_{this, Slots::additional_array_specifier, 2, false};
    ChildPtr<ArrayInitializerExpression> initializer_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
