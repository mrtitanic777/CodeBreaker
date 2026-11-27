// dotnet/decompiler/csharp/syntax/field_declaration.hpp -- FieldDeclaration: `[modifiers] type name [= value], ...;` in a type.
//
//     auto* field = make<FieldDeclaration>(attributes, field_type, variables);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: FieldDeclaration, from TypeMembers/FieldDeclaration.cs with the members ILSpy
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

class FieldDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::FieldDeclaration;
    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<VariableInitializer> variables_slot{"Variables", true, &Slots::variable, true};

    FieldDeclaration();
    FieldDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value);
    FieldDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, const std::vector<VariableInitializer*>& variables_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstType* return_type() const override { return return_type_; }
    void set_return_type(AstType* value) override;
    AstNodeCollection<VariableInitializer>& variables() const { return variables_; }
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Field; }

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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_field_declaration(*this); }
    AstNode* shallow_clone() const override { return make<FieldDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> return_type_;
    mutable AstNodeCollection<VariableInitializer> variables_{this, Slots::variable, 2, false};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
