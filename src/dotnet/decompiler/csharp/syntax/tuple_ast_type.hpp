// dotnet/decompiler/csharp/syntax/tuple_ast_type.hpp -- TupleAstType: a tuple type `(int a, string)`, and TupleTypeElement, one
// element of it (a type and an optional name).
//
//     auto* tuple = make<TupleAstType>();
//     tuple->elements().add(make<TupleTypeElement>(make<PrimitiveType>("int"), "a"));
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: TupleAstType, TupleTypeElement, from TupleAstType.cs and the members
// generated for them), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// One element of a tuple type: its type, and its name if it has one.
class TupleTypeElement final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::TupleTypeElement;

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, true};

    TupleTypeElement() = default;
    TupleTypeElement(AstType* element_type, const std::string& element_name) {
        set_type(element_type);
        set_name(element_name);
    }

    AstType* type() const { return type_; }
    void set_type(AstType* value) { set_child_node(type_, value, 0); }

    // The element's name, or "" if it has none.
    std::string name() const { return name_token_ != nullptr ? name_token_->name() : std::string(); }
    void set_name(const std::string& value) { set_name_token(Identifier::create_if_not_empty(value)); }

    Identifier* name_token() const { return name_token_; }
    void set_name_token(Identifier* value) { set_child_node(name_token_, value, 1); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return 2; }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_tuple_type_element(*this); }
    AstNode* shallow_clone() const override { return make<TupleTypeElement>(*this); }

private:
    ChildPtr<AstType> type_;
    ChildPtr<Identifier> name_token_;
};

class TupleAstType final : public AstType {
public:
    static constexpr AstNodeKind kind = AstNodeKind::TupleAstType;

    static constexpr CSharpSlotInfoT<TupleTypeElement> elements_slot{"Elements", true, &Slots::element, true};

    TupleAstType() = default;
    explicit TupleAstType(const std::vector<TupleTypeElement*>& element_list) { elements_.add_range(element_list); }

    AstNodeCollection<TupleTypeElement>& elements() const { return elements_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return elements_.count(); }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_tuple_type(*this); }
    AstNode* shallow_clone() const override { return make<TupleAstType>(*this); }

private:
    mutable AstNodeCollection<TupleTypeElement> elements_{this, Slots::element, 0, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
