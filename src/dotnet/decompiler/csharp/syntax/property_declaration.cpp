// dotnet/decompiler/csharp/syntax/property_declaration.cpp -- see property_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/property_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/accessor.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

PropertyDeclaration::PropertyDeclaration() {
}

PropertyDeclaration::PropertyDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value) {
    attributes().add_range(attributes_value);
    set_return_type(return_type_value);
    set_name_token(name_token_value);
}

PropertyDeclaration::PropertyDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, Identifier* name_token_value, AstType* private_implementation_type_value, Accessor* getter_value, Accessor* setter_value, Expression* initializer_value, Expression* expression_body_value) : PropertyDeclaration(attributes_value, return_type_value, name_token_value) {
    set_private_implementation_type(private_implementation_type_value);
    set_getter(getter_value);
    set_setter(setter_value);
    set_initializer(initializer_value);
    set_expression_body(expression_body_value);
}

void PropertyDeclaration::set_return_type(AstType* value) {
    set_child_node(return_type_, value);
}

void PropertyDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

void PropertyDeclaration::set_private_implementation_type(AstType* value) {
    set_child_node(private_implementation_type_, value);
}

void PropertyDeclaration::set_getter(Accessor* value) {
    set_child_node(getter_, value);
}

void PropertyDeclaration::set_setter(Accessor* value) {
    set_child_node(setter_, value);
}

void PropertyDeclaration::set_initializer(Expression* value) {
    set_child_node(initializer_, value);
}

void PropertyDeclaration::set_expression_body(Expression* value) {
    set_child_node(expression_body_, value);
}

bool PropertyDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<PropertyDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && match_optional(private_implementation_type(), o->private_implementation_type(), match)
           && match_optional(getter(), o->getter(), match)
           && match_optional(setter(), o->setter(), match)
           && match_optional(initializer(), o->initializer(), match)
           && match_optional(expression_body(), o->expression_body(), match)
           && is_automatic_property() == o->is_automatic_property();
}

int PropertyDeclaration::get_child_count() const {
    return 7 + attributes_.count();
}

AstNode* PropertyDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return return_type_;
    i--;
    if (i == 0) return name_token_;
    i--;
    if (i == 0) return private_implementation_type_;
    i--;
    if (i == 0) return getter_;
    i--;
    if (i == 0) return setter_;
    i--;
    if (i == 0) return initializer_;
    i--;
    if (i == 0) return expression_body_;
    throw std::out_of_range("index");
}

void PropertyDeclaration::set_child_by_index(int index, AstNode* value) {
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
        set_child_node(return_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(private_implementation_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(getter_, cast<Accessor>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(setter_, cast<Accessor>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(initializer_, cast<Expression>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(expression_body_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& PropertyDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return return_type_slot;
    i--;
    if (i == 0) return name_token_slot;
    i--;
    if (i == 0) return private_implementation_type_slot;
    i--;
    if (i == 0) return getter_slot;
    i--;
    if (i == 0) return setter_slot;
    i--;
    if (i == 0) return initializer_slot;
    i--;
    if (i == 0) return expression_body_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* PropertyDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    return nullptr;
}

void PropertyDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<PropertyDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (return_type_ != nullptr) copy->set_return_type(clone_as(return_type_.get()));
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (private_implementation_type_ != nullptr) copy->set_private_implementation_type(clone_as(private_implementation_type_.get()));
    if (getter_ != nullptr) copy->set_getter(clone_as(getter_.get()));
    if (setter_ != nullptr) copy->set_setter(clone_as(setter_.get()));
    if (initializer_ != nullptr) copy->set_initializer(clone_as(initializer_.get()));
    if (expression_body_ != nullptr) copy->set_expression_body(clone_as(expression_body_.get()));
}

bool PropertyDeclaration::is_automatic_property() const {
    if (getter() != nullptr && getter()->body() != nullptr) return false;
    if (setter() != nullptr && setter()->body() != nullptr) return false;
    return true;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
