// dotnet/decompiler/csharp/syntax/extension_declaration.hpp -- ExtensionDeclaration: C# 14's `extension<T>(receiver) { members }` block.
//
//     auto* ext = make<ExtensionDeclaration>(attributes, type_parameters, receiver_parameters);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ExtensionDeclaration, from TypeMembers/ExtensionDeclaration.cs with the members
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

class ExtensionDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ExtensionDeclaration;
    static constexpr std::string_view extension_keyword = "extension";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<TypeParameterDeclaration> type_parameters_slot{"TypeParameters", true, &Slots::type_parameter, true};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> receiver_parameters_slot{"ReceiverParameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<Constraint> constraints_slot{"Constraints", true, &Slots::constraint, true};
    static constexpr CSharpSlotInfoT<EntityDeclaration> members_slot{"Members", true, &Slots::type_member, true};

    ExtensionDeclaration();
    explicit ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value);
    ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value);
    ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& receiver_parameters_value);
    ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& receiver_parameters_value, const std::vector<Constraint*>& constraints_value);
    ExtensionDeclaration(const std::vector<AttributeSection*>& attributes_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& receiver_parameters_value, const std::vector<Constraint*>& constraints_value, const std::vector<EntityDeclaration*>& members_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstNodeCollection<TypeParameterDeclaration>& type_parameters() const { return type_parameters_; }
    AstNodeCollection<ParameterDeclaration>& receiver_parameters() const { return receiver_parameters_; }
    AstNodeCollection<Constraint>& constraints() const { return constraints_; }
    AstNodeCollection<EntityDeclaration>& members() const { return members_; }
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::TypeDefinition; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_extension_declaration(*this); }
    AstNode* shallow_clone() const override { return make<ExtensionDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    mutable AstNodeCollection<TypeParameterDeclaration> type_parameters_{this, Slots::type_parameter, 1, false};
    mutable AstNodeCollection<ParameterDeclaration> receiver_parameters_{this, Slots::parameter, 2, false};
    mutable AstNodeCollection<Constraint> constraints_{this, Slots::constraint, 3, false};
    mutable AstNodeCollection<EntityDeclaration> members_{this, Slots::type_member, 4, false};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
