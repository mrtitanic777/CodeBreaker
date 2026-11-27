// dotnet/decompiler/csharp/syntax/fixed_field_declaration.hpp -- FixedFieldDeclaration: `fixed type name[count];` (a fixed-size buffer).
//
//     auto* field = make<FixedFieldDeclaration>(attributes, element_type, variables);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: FixedFieldDeclaration, from TypeMembers/FixedFieldDeclaration.cs with the
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

class FixedFieldDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::FixedFieldDeclaration;
    static constexpr std::string_view fixed_keyword = "fixed";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<FixedVariableInitializer> variables_slot{"Variables", true, &Slots::fixed_variable, true};

    FixedFieldDeclaration();
    FixedFieldDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value);
    FixedFieldDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, const std::vector<FixedVariableInitializer*>& variables_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstType* return_type() const override { return return_type_; }
    void set_return_type(AstType* value) override;
    AstNodeCollection<FixedVariableInitializer>& variables() const { return variables_; }
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_fixed_field_declaration(*this); }
    AstNode* shallow_clone() const override { return make<FixedFieldDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> return_type_;
    mutable AstNodeCollection<FixedVariableInitializer> variables_{this, Slots::fixed_variable, 2, false};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
