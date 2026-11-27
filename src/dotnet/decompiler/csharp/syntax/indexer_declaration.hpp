// dotnet/decompiler/csharp/syntax/indexer_declaration.hpp -- IndexerDeclaration: `type this[parameters] { get; set; }`.
//
//     auto* indexer = make<IndexerDeclaration>(attributes, value_type, nullptr, parameters);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: IndexerDeclaration, from TypeMembers/IndexerDeclaration.cs with the members
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

class IndexerDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::IndexerDeclaration;
    static constexpr std::string_view this_keyword = "this";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> return_type_slot{"ReturnType", false, &Slots::type, false};
    static constexpr CSharpSlotInfoT<AstType> private_implementation_type_slot{"PrivateImplementationType", false, &Slots::private_implementation_type, true};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> parameters_slot{"Parameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<Accessor> getter_slot{"Getter", false, &Slots::getter, true};
    static constexpr CSharpSlotInfoT<Accessor> setter_slot{"Setter", false, &Slots::setter, true};
    static constexpr CSharpSlotInfoT<Expression> expression_body_slot{"ExpressionBody", false, &Slots::expression_body, true};

    IndexerDeclaration();
    IndexerDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value);
    IndexerDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, const std::vector<ParameterDeclaration*>& parameters_value);
    IndexerDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, const std::vector<ParameterDeclaration*>& parameters_value, Accessor* getter_value, Accessor* setter_value, Expression* expression_body_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    AstType* return_type() const override { return return_type_; }
    void set_return_type(AstType* value) override;
    AstType* private_implementation_type() const { return private_implementation_type_; }
    void set_private_implementation_type(AstType* value);
    AstNodeCollection<ParameterDeclaration>& parameters() const { return parameters_; }
    Accessor* getter() const { return getter_; }
    void set_getter(Accessor* value);
    Accessor* setter() const { return setter_; }
    void set_setter(Accessor* value);
    Expression* expression_body() const { return expression_body_; }
    void set_expression_body(Expression* value);
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Indexer; }

    // An indexer is always called Item.
    std::string name() const override { return "Item"; }
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
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_indexer_declaration(*this); }
    AstNode* shallow_clone() const override { return make<IndexerDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> return_type_;
    ChildPtr<AstType> private_implementation_type_;
    mutable AstNodeCollection<ParameterDeclaration> parameters_{this, Slots::parameter, 3, false};
    ChildPtr<Accessor> getter_;
    ChildPtr<Accessor> setter_;
    ChildPtr<Expression> expression_body_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
