// dotnet/decompiler/csharp/syntax/parameter_declaration.hpp -- ParameterDeclaration: one parameter of a method, delegate, indexer or lambda
// (`[attributes] this ref type name = default`).
//
//     auto* p = make<ParameterDeclaration>();
//     p->set_name("count");
//     p->set_type(make<PrimitiveType>("int"));
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: ParameterDeclaration, from TypeMembers/ParameterDeclaration.cs with the members
// ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
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

class ParameterDeclaration : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::ParameterDeclaration;
    static constexpr std::string_view this_modifier = "this";
    static constexpr std::string_view scoped_ref_keyword = "scoped";
    static constexpr std::string_view ref_modifier = "ref";
    static constexpr std::string_view readonly_modifier = "readonly";
    static constexpr std::string_view out_modifier = "out";
    static constexpr std::string_view in_modifier = "in";
    static constexpr std::string_view params_modifier = "params";

    static constexpr CSharpSlotInfoT<AttributeSection> attributes_slot{"Attributes", true, &Slots::attribute_section, true};
    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, true};
    static constexpr CSharpSlotInfoT<Identifier> name_token_slot{"NameToken", false, &Slots::identifier, true};
    static constexpr CSharpSlotInfoT<Expression> default_expression_slot{"DefaultExpression", false, &Slots::expression, true};

    ParameterDeclaration();
    ParameterDeclaration(const std::vector<AttributeSection*>& attributes_value, typesystem::ReferenceKind parameter_modifier_value, AstType* type_value, const std::string& name_value);
    ParameterDeclaration(const std::vector<AttributeSection*>& attributes_value, typesystem::ReferenceKind parameter_modifier_value, AstType* type_value, const std::string& name_value, Expression* default_expression_value);

    AstNodeCollection<AttributeSection>& attributes() const { return attributes_; }
    AstType* type() const { return type_; }
    void set_type(AstType* value);
    Identifier* name_token() const { return name_token_; }
    void set_name_token(Identifier* value);
    std::string name() const { return name_token_ != nullptr ? name_token_->name() : std::string(); }
    void set_name(const std::string& value) { set_name_token(Identifier::create_if_not_empty(value)); }
    Expression* default_expression() const { return default_expression_; }
    void set_default_expression(Expression* value);
    bool has_this_modifier() const { return has_this_modifier_; }
    void set_has_this_modifier(bool value) { has_this_modifier_ = value; }
    bool is_params() const { return is_params_; }
    void set_is_params(bool value) { is_params_ = value; }
    bool is_scoped_ref() const { return is_scoped_ref_; }
    void set_is_scoped_ref(bool value) { is_scoped_ref_ = value; }
    typesystem::ReferenceKind parameter_modifier() const { return parameter_modifier_; }
    void set_parameter_modifier(typesystem::ReferenceKind value) { parameter_modifier_ = value; }

    // A deep copy of this subtree, unattached (AstNode::clone, typed).
    ParameterDeclaration* clone() const { return static_cast<ParameterDeclaration*>(AstNode::clone()); }

    // The node that stands for `pattern` where a ParameterDeclaration goes (upstream's implicit conversion Pattern -> ParameterDeclaration).
    static ParameterDeclaration* from_pattern(pattern_matching::Pattern* pattern);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_parameter_declaration(*this); }
    AstNode* shallow_clone() const override { return make<ParameterDeclaration>(*this); }

private:
    mutable AstNodeCollection<AttributeSection> attributes_{this, Slots::attribute_section, 0, false};
    ChildPtr<AstType> type_;
    ChildPtr<Identifier> name_token_;
    ChildPtr<Expression> default_expression_;
    bool has_this_modifier_{};
    bool is_params_{};
    bool is_scoped_ref_{};
    typesystem::ReferenceKind parameter_modifier_{};
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
