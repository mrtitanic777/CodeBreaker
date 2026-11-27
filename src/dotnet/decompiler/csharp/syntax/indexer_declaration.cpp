// dotnet/decompiler/csharp/syntax/indexer_declaration.cpp -- see indexer_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/indexer_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/accessor.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

IndexerDeclaration::IndexerDeclaration() {
}

IndexerDeclaration::IndexerDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value) {
    attributes().add_range(attributes_value);
    set_return_type(return_type_value);
}

IndexerDeclaration::IndexerDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, const std::vector<ParameterDeclaration*>& parameters_value) : IndexerDeclaration(attributes_value, return_type_value) {
    set_private_implementation_type(private_implementation_type_value);
    parameters().add_range(parameters_value);
}

IndexerDeclaration::IndexerDeclaration(const std::vector<AttributeSection*>& attributes_value, AstType* return_type_value, AstType* private_implementation_type_value, const std::vector<ParameterDeclaration*>& parameters_value, Accessor* getter_value, Accessor* setter_value, Expression* expression_body_value) : IndexerDeclaration(attributes_value, return_type_value, private_implementation_type_value, parameters_value) {
    set_getter(getter_value);
    set_setter(setter_value);
    set_expression_body(expression_body_value);
}

void IndexerDeclaration::set_return_type(AstType* value) {
    set_child_node(return_type_, value);
}

void IndexerDeclaration::set_private_implementation_type(AstType* value) {
    set_child_node(private_implementation_type_, value);
}

void IndexerDeclaration::set_getter(Accessor* value) {
    set_child_node(getter_, value);
}

void IndexerDeclaration::set_setter(Accessor* value) {
    set_child_node(setter_, value);
}

void IndexerDeclaration::set_expression_body(Expression* value) {
    set_child_node(expression_body_, value);
}

bool IndexerDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<IndexerDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && match_optional(private_implementation_type(), o->private_implementation_type(), match)
           && parameters().do_match(o->parameters(), match)
           && match_optional(getter(), o->getter(), match)
           && match_optional(setter(), o->setter(), match)
           && match_optional(expression_body(), o->expression_body(), match);
}

int IndexerDeclaration::get_child_count() const {
    return 5 + attributes_.count() + parameters_.count();
}

AstNode* IndexerDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return return_type_;
    i--;
    if (i == 0) return private_implementation_type_;
    i--;
    {
        int n = parameters_.count();
        if (i < n) return parameters_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return getter_;
    i--;
    if (i == 0) return setter_;
    i--;
    if (i == 0) return expression_body_;
    throw std::out_of_range("index");
}

void IndexerDeclaration::set_child_by_index(int index, AstNode* value) {
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
        set_child_node(private_implementation_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    {
        int n = parameters_.count();
        if (i < n) {
            parameters_.set(static_cast<std::size_t>(i), cast<ParameterDeclaration>(value));
            return;
        }
        i -= n;
    }
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
        set_child_node(expression_body_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& IndexerDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return return_type_slot;
    i--;
    if (i == 0) return private_implementation_type_slot;
    i--;
    {
        int n = parameters_.count();
        if (i < n) return parameters_slot;
        i -= n;
    }
    if (i == 0) return getter_slot;
    i--;
    if (i == 0) return setter_slot;
    i--;
    if (i == 0) return expression_body_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* IndexerDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::parameter) return &parameters_;
    return nullptr;
}

void IndexerDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<IndexerDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (return_type_ != nullptr) copy->set_return_type(clone_as(return_type_.get()));
    if (private_implementation_type_ != nullptr) copy->set_private_implementation_type(clone_as(private_implementation_type_.get()));
    copy->parameters_.clone_from(copy, parameters_);
    if (getter_ != nullptr) copy->set_getter(clone_as(getter_.get()));
    if (setter_ != nullptr) copy->set_setter(clone_as(setter_.get()));
    if (expression_body_ != nullptr) copy->set_expression_body(clone_as(expression_body_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
