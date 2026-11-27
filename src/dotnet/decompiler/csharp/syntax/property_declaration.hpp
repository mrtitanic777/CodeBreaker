// dotnet/decompiler/csharp/syntax/property_declaration.hpp -- PropertyDeclaration: `[modifiers] type [interface.]Name { get; set; } [=
// initializer]`.
//
//     auto* prop = make<PropertyDeclaration>();
//     bool auto_property = prop->is_automatic_property();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: PropertyDeclaration, from TypeMembers/PropertyDeclaration.cs with the members
// ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class PropertyDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::PropertyDeclaration;
    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<AstType> private_implementation_type_slot{"PrivateImplementationType", false, &Slots::private_implementation_type, true};
    static constexpr CSharpSlotInfoT<Accessor> getter_slot{"Getter", false, &Slots::getter, true};
    static constexpr CSharpSlotInfoT<Accessor> setter_slot{"Setter", false, &Slots::setter, true};
    static constexpr CSharpSlotInfoT<Expression> initializer_slot{"Initializer", false, &Slots::expression, true};
    static constexpr CSharpSlotInfoT<Expression> expression_body_slot{"ExpressionBody", false, &Slots::expression_body, true};

    PropertyDeclaration();
    PropertyDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value);
    PropertyDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value, AstType* private_implementation_type_value, Accessor* getter_value, Accessor* setter_value, Expression* initializer_value, Expression* expression_body_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstType* return_type() const override { return return_type_; }
    void set_return_type(AstType* value) override;
    Identifier* name_token() const override { return name_token_; }
    void set_name_token(Identifier* value) override;
    AstType* private_implementation_type() const { return private_implementation_type_; }
    void set_private_implementation_type(AstType* value);
    Accessor* getter() const { return getter_; }
    void set_getter(Accessor* value);
    Accessor* setter() const { return setter_; }
    void set_setter(Accessor* value);
    Expression* initializer() const { return initializer_; }
    void set_initializer(Expression* value);
    Expression* expression_body() const { return expression_body_; }
    void set_expression_body(Expression* value);
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Property; }

    // Whether neither accessor has a body (`{ get; set; }`).
    bool is_automatic_property() const;

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_property_declaration(*this); }
    AstNode* shallow_clone() const override { return make<PropertyDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> return_type_;
    ChildPtr<Identifier> name_token_;
    ChildPtr<AstType> private_implementation_type_;
    ChildPtr<Accessor> getter_;
    ChildPtr<Accessor> setter_;
    ChildPtr<Expression> initializer_;
    ChildPtr<Expression> expression_body_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
