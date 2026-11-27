// dotnet/decompiler/csharp/syntax/accessor.cpp -- see accessor.hpp.
#include "dotnet/decompiler/csharp/syntax/accessor.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"
#include "dotnet/decompiler/csharp/syntax/block_statement.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

Accessor::Accessor() {
}

Accessor::Accessor(AccessorKind kind_value) {
    set_accessor_kind(kind_value);
}

Accessor::Accessor(AccessorKind kind_value, const std::vector<AttributeSection*>& attributes_value) : Accessor(kind_value) {
    attributes().add_range(attributes_value);
}

Accessor::Accessor(AccessorKind kind_value, const std::vector<AttributeSection*>& attributes_value, BlockStatement* body_value) : Accessor(kind_value, attributes_value) {
    set_body(body_value);
}

void Accessor::set_body(BlockStatement* value) {
    set_child_node(body_, value);
}

bool Accessor::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<Accessor>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_attributes_and_modifiers(*o, match)
           && match_optional(return_type(), o->return_type(), match)
           && (accessor_kind() == AccessorKind::Any || accessor_kind() == o->accessor_kind())
           && match_optional(body(), o->body(), match);
}

int Accessor::get_child_count() const {
    return 1 + attributes_.count();
}

AstNode* Accessor::get_child_by_index(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return body_;
    throw std::out_of_range("index");
}

void Accessor::set_child_by_index(int index, AstNode* value) {
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
        set_child_node(body_, cast<BlockStatement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& Accessor::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    if (i == 0) return body_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* Accessor::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    return nullptr;
}

void Accessor::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<Accessor*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
