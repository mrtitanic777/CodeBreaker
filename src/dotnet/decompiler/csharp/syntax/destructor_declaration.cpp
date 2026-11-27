// dotnet/decompiler/csharp/syntax/destructor_declaration.cpp -- see destructor_declaration.hpp.
#include "dotnet/decompiler/csharp/syntax/destructor_declaration.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/block_statement.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

DestructorDeclaration::DestructorDeclaration() {
}

DestructorDeclaration::DestructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value) {
    attributes().add_range(attributes_value);
    set_name_token(name_token_value);
}

DestructorDeclaration::DestructorDeclaration(const std::vector<AttributeSection*>& attributes_value, Identifier* name_token_value, BlockStatement* body_value) : DestructorDeclaration(attributes_value, name_token_value) {
    set_body(body_value);
}

void DestructorDeclaration::set_name_token(Identifier* value) {
    set_child_node(name_token_, value);
}

void DestructorDeclaration::set_body(BlockStatement* value) {
    set_child_node(body_, value);
}

bool DestructorDeclaration::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<DestructorDeclaration>(other);
    return o != nullptr
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && match_optional(body(), o->body(), match);
}

int DestructorDeclaration::get_child_count() const {
    return 2 + attributes_.count();
}

AstNode* DestructorDeclaration::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return name_token_;
    i--;
    if (i == 0) return body_;
    throw std::out_of_range("index");
}

void DestructorDeclaration::set_child_by_index(int index, AstNode* value) {
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
        set_child_node(body_, cast<BlockStatement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& DestructorDeclaration::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return name_token_slot;
    i--;
    if (i == 0) return body_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* DestructorDeclaration::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    return nullptr;
}

void DestructorDeclaration::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<DestructorDeclaration*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
