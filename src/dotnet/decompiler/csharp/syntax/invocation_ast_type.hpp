// dotnet/decompiler/csharp/syntax/invocation_ast_type.hpp -- InvocationAstType: a type followed by arguments, `Base(args)`, the form a
// primary constructor's base type takes in a record's base list (`record R(int X) : Base(X)`).
//
//     auto* t = make<InvocationAstType>(std::vector<Expression*>{arg}, base);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: InvocationAstType, from InvocationAstType.cs and the members generated for
// it), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class InvocationAstType final : public AstType {
public:
    static constexpr AstNodeKind kind = AstNodeKind::InvocationAstType;

    // The slots: the arguments, then the base type.
    static constexpr CSharpSlotInfoT<Expression> arguments_slot{"Arguments", true, &Slots::expression, true};
    static constexpr CSharpSlotInfoT<AstType> base_type_slot{"BaseType", false, &Slots::type, false};

    InvocationAstType() = default;
    InvocationAstType(const std::vector<Expression*>& argument_list, AstType* base) {
        arguments_.add_range(argument_list);
        set_base_type(base);
    }

    AstNodeCollection<Expression>& arguments() const { return arguments_; }

    AstType* base_type() const { return base_type_; }
    void set_base_type(AstType* value) { set_child_node(base_type_, value); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return arguments_.count() + 1; }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_invocation_type(*this); }
    AstNode* shallow_clone() const override { return make<InvocationAstType>(*this); }

private:
    mutable AstNodeCollection<Expression> arguments_{this, Slots::expression, 0, false};
    ChildPtr<AstType> base_type_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
