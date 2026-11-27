// dotnet/decompiler/csharp/syntax/type_declaration.hpp -- TypeDeclaration: a class, struct, interface, enum or record declaration with its
// members.
//
//     auto* type = make<TypeDeclaration>();
//     type->set_class_type(ClassType::Struct);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: TypeDeclaration, from GeneralScope/TypeDeclaration.cs with the members ILSpy
// 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// The kind of type a TypeDeclaration declares.
enum class ClassType {
    Class,
    Struct,
    Interface,
    Enum,
    RecordClass,
    RecordStruct,
};

class TypeDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::TypeDeclaration;
    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<TypeParameterDeclaration> type_parameters_slot{"TypeParameters", true, &Slots::type_parameter, true};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> primary_constructor_parameters_slot{"PrimaryConstructorParameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<AstType> base_types_slot{"BaseTypes", true, &Slots::base_type, true};
    static constexpr CSharpSlotInfoT<Constraint> constraints_slot{"Constraints", true, &Slots::constraint, true};
    static constexpr CSharpSlotInfoT<EntityDeclaration> members_slot{"Members", true, &Slots::type_member, true};

    TypeDeclaration();
    TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value);
    TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value);
    TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& primary_constructor_parameters_value);
    TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& primary_constructor_parameters_value, const std::vector<AstType*>& base_types_value);
    TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& primary_constructor_parameters_value, const std::vector<AstType*>& base_types_value, const std::vector<Constraint*>& constraints_value);
    TypeDeclaration(ClassType class_type_value, const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<TypeParameterDeclaration*>& type_parameters_value, const std::vector<ParameterDeclaration*>& primary_constructor_parameters_value, const std::vector<AstType*>& base_types_value, const std::vector<Constraint*>& constraints_value, const std::vector<EntityDeclaration*>& members_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    Identifier* name_token() const override { return name_token_; }
    void set_name_token(Identifier* value) override;
    AstNodeCollection<TypeParameterDeclaration>& type_parameters() const { return type_parameters_; }
    AstNodeCollection<ParameterDeclaration>& primary_constructor_parameters() const { return primary_constructor_parameters_; }
    AstNodeCollection<AstType>& base_types() const { return base_types_; }
    AstNodeCollection<Constraint>& constraints() const { return constraints_; }
    AstNodeCollection<EntityDeclaration>& members() const { return members_; }
    ClassType class_type() const { return class_type_; }
    void set_class_type(ClassType value) { class_type_ = value; }
    bool has_primary_constructor() const { return has_primary_constructor_; }
    void set_has_primary_constructor(bool value) { has_primary_constructor_ = value; }
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_type_declaration(*this); }
    AstNode* shallow_clone() const override { return make<TypeDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<Identifier> name_token_;
    mutable AstNodeCollection<TypeParameterDeclaration> type_parameters_{this, Slots::type_parameter, 2, false};
    mutable AstNodeCollection<ParameterDeclaration> primary_constructor_parameters_{this, Slots::parameter, 3, false};
    mutable AstNodeCollection<AstType> base_types_{this, Slots::base_type, 4, false};
    mutable AstNodeCollection<Constraint> constraints_{this, Slots::constraint, 5, false};
    mutable AstNodeCollection<EntityDeclaration> members_{this, Slots::type_member, 6, false};
    ClassType class_type_{};
    bool has_primary_constructor_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
