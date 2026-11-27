// dotnet/decompiler/csharp/syntax/enum_member_declaration.cpp -- see enum_member_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/enum_member_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

EnumMemberDeclaration::EnumMemberDeclaration() {
}

EnumMemberDeclaration::EnumMemberDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value) {
    attributes().add_range(attributes_value);
    set_name_token(name_token_value);
}

EnumMemberDeclaration::EnumMemberDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, Expression* initializer_value) : EnumMemberDeclaration(attributes_value, name_token_value) {
    set_initializer(initializer_value);
}

void EnumMemberDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

void EnumMemberDeclaration::set_initializer(Expression* value) {
    set_child_node(initializer_, value);
}

bool EnumMemberDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<EnumMemberDeclaration>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && match_optional(initializer(), o->initializer(), match);
}

int EnumMemberDeclaration::get_child_count() const {
    return 2 + attributes_.count();
}

AstNode* EnumMemberDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return name_token_;
    i--;
    if (i == 0) return initializer_;
    throw std::out_of_range("index");
}

void EnumMemberDeclaration::set_child_by_index(int index, AstNode* value) {
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
    if (i == 0) {
        set_child_node(initializer_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& EnumMemberDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return name_token_slot;
    i--;
    if (i == 0) return initializer_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* EnumMemberDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    return nullptr;
}

void EnumMemberDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<EnumMemberDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (initializer_ != nullptr) copy->set_initializer(clone_as(initializer_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
