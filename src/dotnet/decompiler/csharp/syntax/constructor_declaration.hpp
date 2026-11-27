// dotnet/decompiler/csharp/syntax/constructor_declaration.hpp -- ConstructorDeclaration and ConstructorInitializer: `[attributes]
// [modifiers] Name(parameters) [: base(arguments)] { body }`.
//
//     auto* ctor = make<ConstructorDeclaration>(attributes, Identifier::create("Point"), parameters);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ConstructorDeclaration, ConstructorInitializer, from
// TypeMembers/ConstructorDeclaration.cs with the members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/entity_declaration.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// Whether a constructor initializer calls `base(...)` or `this(...)` (Any matches both in a pattern).
enum class ConstructorInitializerType {
    Any,
    Base,
    This,
};

class ConstructorDeclaration final : public EntityDeclaration {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ConstructorDeclaration;
    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<ParameterDeclaration> parameters_slot{"Parameters", true, &Slots::parameter, true};
    static constexpr CSharpSlotInfoT<ConstructorInitializer> initializer_slot{"Initializer", false, &Slots::constructor_initializer, true};
    static constexpr CSharpSlotInfoT<BlockStatement> body_slot{"Body", false, &Slots::body, true};

    ConstructorDeclaration();
    ConstructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value);
    ConstructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<ParameterDeclaration*>& parameters_value);
    ConstructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<ParameterDeclaration*>& parameters_value, ConstructorInitializer* initializer_value, BlockStatement* body_value);

    AstNodeCollection<AttributeSection>& attributes() const override { return attributes_; }
    Identifier* name_token() const override { return name_token_; }
    void set_name_token(Identifier* value) override;
    AstNodeCollection<ParameterDeclaration>& parameters() const { return parameters_; }
    ConstructorInitializer* initializer() const { return initializer_; }
    void set_initializer(ConstructorInitializer* value);
    BlockStatement* body() const { return body_; }
    void set_body(BlockStatement* value);
    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Constructor; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_constructor_declaration(*this); }
    AstNode* shallow_clone() const override { return make<ConstructorDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<Identifier> name_token_;
    mutable AstNodeCollection<ParameterDeclaration> parameters_{this, Slots::parameter, 2, false};
    ChildPtr<ConstructorInitializer> initializer_;
    ChildPtr<BlockStatement> body_;
};

class ConstructorInitializer final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ConstructorInitializer;
    static constexpr std::string_view base_keyword = "base";
    static constexpr std::string_view this_keyword = "this";

    static constexpr CSharpSlotInfoT<Expression> arguments_slot{"Arguments", true, &Slots::argument, true};

    ConstructorInitializer();
    explicit ConstructorInitializer(ConstructorInitializerType constructor_initializer_type_value);
    ConstructorInitializer(ConstructorInitializerType constructor_initializer_type_value, const std::vector<Expression*>& arguments_value);

    AstNodeCollection<Expression>& arguments() const { return arguments_; }
    ConstructorInitializerType constructor_initializer_type() const { return constructor_initializer_type_; }
    void set_constructor_initializer_type(ConstructorInitializerType value) { constructor_initializer_type_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_constructor_initializer(*this); }
    AstNode* shallow_clone() const override { return make<ConstructorInitializer>(*this); }

private:
    mutable AstNodeCollection<Expression> arguments_{this, Slots::argument, 0, true};
    ConstructorInitializerType constructor_initializer_type_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
