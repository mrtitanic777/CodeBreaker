// dotnet/decompiler/csharp/syntax/attribute_section.cpp -- see attribute_section.hpp.
#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/attribute.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

AttributeSection::AttributeSection(Attribute* attr) {
    attributes().add(attr);
}

AttributeSection::AttributeSection() {
}

AttributeSection::AttributeSection(Identifier* attribute_target_token_value, const std::vector<Attribute*>& attributes_value) {
    set_attribute_target_token(attribute_target_token_value);
    attributes().add_range(attributes_value);
}

void AttributeSection::set_attribute_target_token(Identifier* value) {
    set_child_node(attribute_target_token_, value, 0);
}

AttributeSection* AttributeSection::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<AttributeSection, AstNodeKind::AttributeSectionPlaceholder>>(pattern);
}

bool AttributeSection::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<AttributeSection>(other);
    return o != nullptr
           && match_string(attribute_target(), o->attribute_target())
           && attributes().do_match(o->attributes(), match);
}

int AttributeSection::get_child_count() const {
    return 1 + attributes_.count();
}

AstNode* AttributeSection::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return attribute_target_token_;
    i--;
    {
        int n = attributes_.count();
        if (i < n) return attributes_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void AttributeSection::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(attribute_target_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    {
        int n = attributes_.count();
        if (i < n) {
            attributes_.set(static_cast<std::size_t>(i), cast<Attribute>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& AttributeSection::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return attribute_target_token_slot;
    i--;
    {
        int n = attributes_.count();
        if (i < n) return attributes_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* AttributeSection::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute) return &attributes_;
    return nullptr;
}

void AttributeSection::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<AttributeSection*>(copy_node);
    if (attribute_target_token_ != nullptr) copy->set_attribute_target_token(clone_as(attribute_target_token_.get()));
    copy->attributes_.clone_from(copy, attributes_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
