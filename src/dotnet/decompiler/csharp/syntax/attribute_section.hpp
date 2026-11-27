// dotnet/decompiler/csharp/syntax/attribute_section.hpp -- AttributeSection: `[target: Attribute, Attribute]`.
//
//     auto* section = make<AttributeSection>(attribute);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: AttributeSection, from GeneralScope/AttributeSection.cs with the members ILSpy
// 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class AttributeSection : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::AttributeSection;
    static constexpr CSharpSlotInfoT<Identifier> attribute_target_token_slot{"AttributeTargetToken", false, &Slots::identifier, true};
    static constexpr CSharpSlotInfoT<Attribute> attributes_slot{"Attributes", true, &Slots::attribute, true};

    explicit AttributeSection(Attribute* attr);
    AttributeSection();
    AttributeSection(Identifier* attribute_target_token_value, const std::vector<Attribute*>& attributes_value);

    Identifier* attribute_target_token() const { return attribute_target_token_; }
    void set_attribute_target_token(Identifier* value);
    AstNodeCollection<Attribute>& attributes() const { return attributes_; }

    // The attribute target ("assembly", "return"), or "" if there is none.
    std::string attribute_target() const {
        Identifier* token = get_child(Slots::identifier);
        return token != nullptr ? token->name() : std::string();
    }
    void set_attribute_target(const std::string& value) { set_child(Slots::identifier, Identifier::create(value)); }

    // The node that stands for `pattern` where a AttributeSection goes (upstream's implicit conversion Pattern -> AttributeSection).
    static AttributeSection* from_pattern(pattern_matching::Pattern* pattern);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_attribute_section(*this); }
    AstNode* shallow_clone() const override { return make<AttributeSection>(*this); }

private:
    ChildPtr<Identifier> attribute_target_token_;
    mutable AstNodeCollection<Attribute> attributes_{this, Slots::attribute, 1, true};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
