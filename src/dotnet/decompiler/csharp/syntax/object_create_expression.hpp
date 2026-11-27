// dotnet/decompiler/csharp/syntax/object_create_expression.hpp -- an object creation, `new T(args) { ... }`.
//
//     auto* n = make<ObjectCreateExpression>(type, std::vector<Expression*>{a});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ObjectCreateExpression, from Expressions/ObjectCreateExpression.cs), Copyright AlphaSierraPapa for the
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

class ObjectCreateExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ObjectCreateExpression;

    static constexpr std::string_view new_keyword = "new";

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<Expression> arguments_slot{"Arguments", true, &Slots::argument, true};
    static constexpr CSharpSlotInfoT<ArrayInitializerExpression> initializer_slot{"Initializer", false, &Slots::initializer, true};

    ObjectCreateExpression() = default;
    explicit ObjectCreateExpression(AstType* type);
    ObjectCreateExpression(AstType* type, const std::vector<Expression*>& arguments);
    ObjectCreateExpression(AstType* type, std::initializer_list<Expression*> arguments);
    ObjectCreateExpression(AstType* type, const std::vector<Expression*>& arguments, ArrayInitializerExpression* initializer);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    AstNodeCollection<Expression>& arguments() const { return arguments_; }
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_object_create_expression(*this); }
    AstNode* shallow_clone() const override { return make<ObjectCreateExpression>(*this); }

private:
    ChildPtr<AstType> type_;
    mutable AstNodeCollection<Expression> arguments_{this, Slots::argument, 1, false};
    ChildPtr<ArrayInitializerExpression> initializer_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
