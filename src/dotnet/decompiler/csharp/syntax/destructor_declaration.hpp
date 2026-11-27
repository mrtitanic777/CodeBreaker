// dotnet/decompiler/csharp/syntax/destructor_declaration.hpp -- DestructorDeclaration: `~Name() { body }`.
//
//     auto* dtor = make<DestructorDeclaration>(attributes, Identifier::create("Point"), body);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: DestructorDeclaration, from TypeMembers/DestructorDeclaration.cs with the
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

class DestructorDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::DestructorDeclaration;
    static constexpr std::string_view tilde_token = "~";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<BlockStatement> body_slot{"Body", false, &Slots::body, true};

    DestructorDeclaration();
    DestructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value);
    DestructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, BlockStatement* body_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    Identifier* name_token() const override { return name_token_; }
    void set_name_token(Identifier* value) override;
    BlockStatement* body() const { return body_; }
    void set_body(BlockStatement* value);
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Destructor; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_destructor_declaration(*this); }
    AstNode* shallow_clone() const override { return make<DestructorDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<Identifier> name_token_;
    ChildPtr<BlockStatement> body_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
