// dotnet/decompiler/csharp/syntax/simple_type.hpp -- SimpleType: a type named by one identifier, with type arguments: `int`-like names
// that are not keywords, `List<T>`, `T`. (A dotted name is a MemberType over a SimpleType.)
//
//     auto* t = make<SimpleType>("List", std::vector<AstType*>{make<SimpleType>("T")});    // List<T>
//     t->identifier();   t->type_arguments().add(make<PrimitiveType>("int"));
//
// An absent identifier token and an empty identifier are the same here (std::string, empty = none).
//
// This class is also the model for writing a node class on the core of the syntax tree: see README-port.md.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: SimpleType, from SimpleType.cs and the members generated for it), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class SimpleType final : public AstType {
public:
    static constexpr AstNodeKind kind = AstNodeKind::SimpleType;

    // This node's slots: the identifier token (index 0, may be empty) and the type arguments (from index 1).
    static constexpr CSharpSlotInfoT<Identifier> identifier_token_slot{"IdentifierToken", false, &Slots::identifier, true};
    static constexpr CSharpSlotInfoT<AstType> type_arguments_slot{"TypeArguments", true, &Slots::type_argument, true};

    SimpleType() = default;
    explicit SimpleType(Identifier* identifier_token) { set_identifier_token(identifier_token); }
    explicit SimpleType(const std::string& identifier_text) { set_identifier(identifier_text); }
    SimpleType(const std::string& identifier_text, TextLocation location) {
        set_child(Slots::identifier, Identifier::create(identifier_text, location));
    }
    SimpleType(const std::string& identifier_text, const std::vector<AstType*>& type_arguments) : SimpleType(identifier_text) {
        type_arguments_.add_range(type_arguments);
    }

    // The name, or "" if there is no identifier token.
    std::string identifier() const { return identifier_token_ != nullptr ? identifier_token_->name() : std::string(); }
    void set_identifier(const std::string& value) { set_identifier_token(Identifier::create_if_not_empty(value)); }

    Identifier* identifier_token() const { return identifier_token_; }
    void set_identifier_token(Identifier* value) { set_child_node(identifier_token_, value, 0); }

    AstNodeCollection<AstType>& type_arguments() const { return type_arguments_; }

    // ---- what a node class provides (see README-port.md)
    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override { return 1 + type_arguments_.count(); }
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_simple_type(*this); }
    AstNode* shallow_clone() const override { return make<SimpleType>(*this); }

private:
    ChildPtr<Identifier> identifier_token_;
    mutable AstNodeCollection<AstType> type_arguments_{this, Slots::type_argument, 1, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
