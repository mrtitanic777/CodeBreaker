// dotnet/decompiler/csharp/syntax/constructor_declaration.cpp -- see constructor_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/constructor_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/block_statement.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/parameter_declaration.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ConstructorDeclaration::ConstructorDeclaration() {
}

ConstructorDeclaration::ConstructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value) {
    attributes().add_range(attributes_value);
    set_name_token(name_token_value);
}

ConstructorDeclaration::ConstructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<ParameterDeclaration*>& parameters_value) : ConstructorDeclaration(attributes_value, name_token_value) {
    parameters().add_range(parameters_value);
}

ConstructorDeclaration::ConstructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, const std::vector<ParameterDeclaration*>& parameters_value, ConstructorInitializer* initializer_value, BlockStatement* body_value) : ConstructorDeclaration(attributes_value, name_token_value, parameters_value) {
    set_initializer(initializer_value);
    set_body(body_value);
}

void ConstructorDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

void ConstructorDeclaration::set_initializer(ConstructorInitializer* value) {
    set_child_node(initializer_, value);
}

void ConstructorDeclaration::set_body(BlockStatement* value) {
    set_child_node(body_, value);
}

bool ConstructorDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ConstructorDeclaration>(other);
    return o != nullptr
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && parameters().do_match(o->parameters(), match)
           && match_optional(initializer(), o->initializer(), match)
           && match_optional(body(), o->body(), match);
}

int ConstructorDeclaration::get_child_count() const {
    return 3 + attributes_.count() + parameters_.count();
}

AstNode* ConstructorDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return name_token_;
    i--;
    {
        int n = parameters_.count();
        if (i < n) return parameters_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return initializer_;
    i--;
    if (i == 0) return body_;
    throw std::out_of_range("index");
}

void ConstructorDeclaration::set_child_by_index(int index, AstNode* value) {
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
        set_child_node(name_token_, cast<Identifier>(value), index);
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
        set_child_node(initializer_, cast<ConstructorInitializer>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(body_, cast<BlockStatement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ConstructorDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return name_token_slot;
    i--;
    {
        int n = parameters_.count();
        if (i < n) return parameters_slot;
        i -= n;
    }
    if (i == 0) return initializer_slot;
    i--;
    if (i == 0) return body_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* ConstructorDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::parameter) return &parameters_;
    return nullptr;
}

void ConstructorDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ConstructorDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    copy->parameters_.clone_from(copy, parameters_);
    if (initializer_ != nullptr) copy->set_initializer(clone_as(initializer_.get()));
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

ConstructorInitializer::ConstructorInitializer() {
}

ConstructorInitializer::ConstructorInitializer(ConstructorInitializerType constructor_initializer_type_value) {
    set_constructor_initializer_type(constructor_initializer_type_value);
}

ConstructorInitializer::ConstructorInitializer(ConstructorInitializerType constructor_initializer_type_value, const std::vector<Expression*>& arguments_value) : ConstructorInitializer(constructor_initializer_type_value) {
    arguments().add_range(arguments_value);
}

bool ConstructorInitializer::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ConstructorInitializer>(other);
    return o != nullptr
           && (constructor_initializer_type() == ConstructorInitializerType::Any || constructor_initializer_type() == o->constructor_initializer_type())
           && arguments().do_match(o->arguments(), match);
}

int ConstructorInitializer::get_child_count() const {
    return arguments_.count();
}

AstNode* ConstructorInitializer::get_child_by_index(int index) const {
    int i = index;
    {
        int n = arguments_.count();
        if (i < n) return arguments_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void ConstructorInitializer::set_child_by_index(int index, AstNode* value) {
    int i = index;
    {
        int n = arguments_.count();
        if (i < n) {
            arguments_.set(static_cast<std::size_t>(i), cast<Expression>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ConstructorInitializer::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = arguments_.count();
        if (i < n) return arguments_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* ConstructorInitializer::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::argument) return &arguments_;
    return nullptr;
}

void ConstructorInitializer::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ConstructorInitializer*>(copy_node);
    copy->arguments_.clone_from(copy, arguments_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
