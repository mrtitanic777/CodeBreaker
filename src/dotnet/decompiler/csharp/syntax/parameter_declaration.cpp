// dotnet/decompiler/csharp/syntax/parameter_declaration.cpp -- see parameter_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ParameterDeclaration::ParameterDeclaration() {
}

ParameterDeclaration::ParameterDeclaration(const std::vector<AttributeSection*>& attributes_value, typesystem::ReferenceKind parameter_modifier_value, AstType* type_value, const std::string& name_value) {
    attributes().add_range(attributes_value);
    set_parameter_modifier(parameter_modifier_value);
    set_type(type_value);
    set_name(name_value);
}

ParameterDeclaration::ParameterDeclaration(const std::vector<AttributeSection*>& attributes_value, typesystem::ReferenceKind parameter_modifier_value, AstType* type_value, const std::string& name_value, Expression* default_expression_value) : ParameterDeclaration(attributes_value, parameter_modifier_value, type_value, name_value) {
    set_default_expression(default_expression_value);
}

void ParameterDeclaration::set_type(AstType* value) {
    set_child_node(type_, value);
}

void ParameterDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

void ParameterDeclaration::set_default_expression(Expression* value) {
    set_child_node(default_expression_, value);
}

ParameterDeclaration* ParameterDeclaration::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<ParameterDeclaration, AstNodeKind::ParameterDeclarationPlaceholder>>(pattern);
}

bool ParameterDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ParameterDeclaration>(other);
    return o != nullptr
           && attributes().do_match(o->attributes(), match)
           && has_this_modifier() == o->has_this_modifier()
           && is_params() == o->is_params()
           && is_scoped_ref() == o->is_scoped_ref()
           && parameter_modifier() == o->parameter_modifier()
           && match_optional(type(), o->type(), match)
           && match_string(name(), o->name())
           && match_optional(default_expression(), o->default_expression(), match);
}

int ParameterDeclaration::get_child_count() const {
    return 3 + attributes_.count();
}

AstNode* ParameterDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return type_;
    i--;
    if (i == 0) return name_token_;
    i--;
    if (i == 0) return default_expression_;
    throw std::out_of_range("index");
}

void ParameterDeclaration::set_child_by_index(int index, AstNode* value) {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) {
            attributes_.set(static_cast<std::size_t>(i), cast<AttributeSection>(value));
            return;
        }
        i -= n;
    }
    if (i == 0) {
        set_child_node(type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(default_expression_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ParameterDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return type_slot;
    i--;
    if (i == 0) return name_token_slot;
    i--;
    if (i == 0) return default_expression_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* ParameterDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    return nullptr;
}

void ParameterDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ParameterDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (default_expression_ != nullptr) copy->set_default_expression(clone_as(default_expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
