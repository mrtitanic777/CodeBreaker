// dotnet/decompiler/csharp/syntax/type_parameter_declaration.hpp -- TypeParameterDeclaration: `[attributes] in/out Name` in a generic
// declaration.
//
//     auto* tp = make<TypeParameterDeclaration>("T");
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: TypeParameterDeclaration, from GeneralScope/TypeParameterDeclaration.cs with the
// members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class TypeParameterDeclaration final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::TypeParameterDeclaration;
    static constexpr std::string_view out_variance_keyword = "out";
    static constexpr std::string_view in_variance_keyword = "in";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};

    explicit TypeParameterDeclaration(const std::string& name_value);
    TypeParameterDeclaration();
    TypeParameterDeclaration(const std::vector<AttributeSection*>& attributes_value, typesystem::VarianceModifier variance_value, const std::string& name_value);

    AstNodeCollection<AttributeSection>& attributes() const { return attributes_; }
    Identifier* name_token() const { return name_token_; }
    void set_name_token(Identifier* value);
    std::string name() const { return name_token_->name(); }
    void set_name(const std::string& value) { set_name_token(Identifier::create(value)); }
    typesystem::VarianceModifier variance() const { return variance_; }
    void set_variance(typesystem::VarianceModifier value) { variance_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_type_parameter_declaration(*this); }
    AstNode* shallow_clone() const override { return make<TypeParameterDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<Identifier> name_token_;
    typesystem::VarianceModifier variance_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
