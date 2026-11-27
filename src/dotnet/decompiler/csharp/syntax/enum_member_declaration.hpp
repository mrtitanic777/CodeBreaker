// dotnet/decompiler/csharp/syntax/enum_member_declaration.hpp -- EnumMemberDeclaration: one member of an enum, `Name [= value]`.
//
//     auto* member = make<EnumMemberDeclaration>(attributes, Identifier::create("Red"), value);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: EnumMemberDeclaration, from TypeMembers/EnumMemberDeclaration.cs with the
// members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class EnumMemberDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::EnumMemberDeclaration;
    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<Expression> initializer_slot{"Initializer", false, &Slots::enum_member_initializer, true};

    EnumMemberDeclaration();
    EnumMemberDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value);
    EnumMemberDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, Expression* initializer_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    Identifier* name_token() const override { return name_token_; }
    void set_name_token(Identifier* value) override;
    Expression* initializer() const { return initializer_; }
    void set_initializer(Expression* value);
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Field; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_enum_member_declaration(*this); }
    AstNode* shallow_clone() const override { return make<EnumMemberDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<Identifier> name_token_;
    ChildPtr<Expression> initializer_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
