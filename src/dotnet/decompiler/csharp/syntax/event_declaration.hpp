// dotnet/decompiler/csharp/syntax/event_declaration.hpp -- EventDeclaration and CustomEventDeclaration: `event type name;` and the `event
// type name { add { } remove { } }` form.
//
//     auto* ev = make<EventDeclaration>(attributes, event_type, variables);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: EventDeclaration, CustomEventDeclaration, from TypeMembers/EventDeclaration.cs
// with the members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class EventDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::EventDeclaration;
    static constexpr std::string_view event_keyword = "event";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<VariableInitializer> variables_slot{"Variables", true, &Slots::variable, true};

    EventDeclaration();
    EventDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value);
    EventDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, const std::vector<VariableInitializer*>& variables_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstType* return_type() const override { return return_type_; }
    void set_return_type(AstType* value) override;
    AstNodeCollection<VariableInitializer>& variables() const { return variables_; }
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Event; }

    // The names are on the variables.
    std::string name() const override { return std::string(); }
    void set_name(const std::string&) override { throw std::logic_error("not supported"); }
    Identifier* name_token() const override { return nullptr; }
    void set_name_token(Identifier*) override { throw std::logic_error("not supported"); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_event_declaration(*this); }
    AstNode* shallow_clone() const override { return make<EventDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> return_type_;
    mutable AstNodeCollection<VariableInitializer> variables_{this, Slots::variable, 2, false};
};

class CustomEventDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::CustomEventDeclaration;
    static constexpr std::string_view event_keyword = "event";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<AstType> private_implementation_type_slot{"PrivateImplementationType", false, &Slots::private_implementation_type, true};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<Accessor> add_accessor_slot{"AddAccessor", false, &Slots::add_accessor, true};
    static constexpr CSharpSlotInfoT<Accessor> remove_accessor_slot{"RemoveAccessor", false, &Slots::remove_accessor, true};

    CustomEventDeclaration();
    CustomEventDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, Identifier* name_token_value);
    CustomEventDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, Identifier* name_token_value, Accessor* add_accessor_value, Accessor* remove_accessor_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstType* return_type() const override { return return_type_; }
    void set_return_type(AstType* value) override;
    AstType* private_implementation_type() const { return private_implementation_type_; }
    void set_private_implementation_type(AstType* value);
    Identifier* name_token() const override { return name_token_; }
    void set_name_token(Identifier* value) override;
    Accessor* add_accessor() const { return add_accessor_; }
    void set_add_accessor(Accessor* value);
    Accessor* remove_accessor() const { return remove_accessor_; }
    void set_remove_accessor(Accessor* value);
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Event; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_custom_event_declaration(*this); }
    AstNode* shallow_clone() const override { return make<CustomEventDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> return_type_;
    ChildPtr<AstType> private_implementation_type_;
    ChildPtr<Identifier> name_token_;
    ChildPtr<Accessor> add_accessor_;
    ChildPtr<Accessor> remove_accessor_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
