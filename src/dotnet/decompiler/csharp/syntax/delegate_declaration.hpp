// dotnet/decompiler/csharp/syntax/delegate_declaration.hpp -- DelegateDeclaration: `delegate return_type Name<type_parameters>(parameters)
// constraints;`.
//
//     auto* d = make<DelegateDeclaration>();
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: DelegateDeclaration, from GeneralScope/DelegateDeclaration.cs with the members
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

class DelegateDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::DelegateDeclaration;
    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<TypeParameterDeclaration> type_parameters_slot{"TypeParameters", true, &Slots::type_parameter, true};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> parameters_slot{"Parameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<Constraint> constraints_slot{"Constraints", true, &Slots::constraint, true};

    DelegateDeclaration();
    DelegateDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value);
    DelegateDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value);
    DelegateDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& parameters_value);
    DelegateDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& parameters_value, const std::vector<Constraint*>& constraints_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstType* return_type() const override { return return_type_; }
    void set_return_type(AstType* value) override;
    Identifier* name_token() const override { return name_token_; }
    void set_name_token(Identifier* value) override;
    AstNodeCollection<TypeParameterDeclaration>& type_parameters() const { return type_parameters_; }
    AstNodeCollection<ParameterDeclaration>& parameters() const { return parameters_; }
    AstNodeCollection<Constraint>& constraints() const { return constraints_; }
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_delegate_declaration(*this); }
    AstNode* shallow_clone() const override { return make<DelegateDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> return_type_;
    ChildPtr<Identifier> name_token_;
    mutable AstNodeCollection<TypeParameterDeclaration> type_parameters_{this, Slots::type_parameter, 3, false};
    mutable AstNodeCollection<ParameterDeclaration> parameters_{this, Slots::parameter, 4, false};
    mutable AstNodeCollection<Constraint> constraints_{this, Slots::constraint, 5, false};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
