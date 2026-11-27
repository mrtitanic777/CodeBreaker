// dotnet/decompiler/csharp/syntax/composed_type.cpp -- see composed_type.hpp.
#include "dotnet/decompiler/csharp/syntax/composed_type.hpp"

#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/attribute_section.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

// ---- ArraySpecifier ------------------------------------------------------------------------------------------------------------------

std::string ArraySpecifier::to_string(const output_visitor::CSharpFormattingOptions*) const {
    return "[" + std::string(static_cast<std::size_t>(dimensions_ > 0 ? dimensions_ - 1 : 0), ',') + "]";
}

void ArraySpecifier::check_invariant() {
    AstNode::check_invariant();
    // A rank specifier always has at least one dimension (`[]` is rank 1); the output relies on it.
    assert(dimensions_ >= 1 && "ArraySpecifier::dimensions must be at least 1");
}

bool ArraySpecifier::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* specifier = as<ArraySpecifier>(other);
    return specifier != nullptr && dimensions_ == specifier->dimensions_;
}

// ---- ComposedType --------------------------------------------------------------------------------------------------------------------

ComposedType::ComposedType(const std::vector<AttributeSection*>& attribute_sections, AstType* base) {
    attributes_.add_range(attribute_sections);
    set_base_type(base);
}

ComposedType::ComposedType(const std::vector<AttributeSection*>& attribute_sections, AstType* base,
                           const std::vector<ArraySpecifier*>& specifiers)
    : ComposedType(attribute_sections, base) {
    array_specifiers_.add_range(specifiers);
}

std::string ComposedType::to_string(const output_visitor::CSharpFormattingOptions*) const {
    std::string b;
    if (has_ref_specifier_) b += "ref ";
    if (has_read_only_specifier_) b += "readonly ";
    b += base_type_->to_string();
    if (has_nullable_specifier_) b += '?';
    b.append(static_cast<std::size_t>(pointer_rank_), '*');
    for (ArraySpecifier* array_specifier : array_specifiers_) {
        b += '[';
        b.append(static_cast<std::size_t>(array_specifier->dimensions() - 1), ',');
        b += ']';
    }
    return b;
}

AstType* ComposedType::make_pointer_type() {
    if (array_specifiers_.count() > 0) return AstType::make_pointer_type();
    pointer_rank_++;
    return this;
}

AstType* ComposedType::make_array_type(int dimensions) {
    insert_child_before(array_specifiers_.first_or_null(), make<ArraySpecifier>(dimensions), Slots::array_specifier);
    return this;
}

AstType* ComposedType::make_ref_type() {
    has_ref_specifier_ = true;
    return this;
}

void ComposedType::check_invariant() {
    AstNode::check_invariant();
    // pointer_rank is the number of '*' specifiers; a transform that corrupts it (an unbalanced decrement) is caught here, at the
    // offending transform.
    assert(pointer_rank_ >= 0 && "ComposedType::pointer_rank must not be negative");
}

bool ComposedType::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* composed_type = as<ComposedType>(other);
    return composed_type != nullptr && attributes_.do_match(composed_type->attributes_, match) &&
           has_ref_specifier_ == composed_type->has_ref_specifier_ && has_read_only_specifier_ == composed_type->has_read_only_specifier_ &&
           match_required(base_type_, composed_type->base_type_, match) && has_nullable_specifier_ == composed_type->has_nullable_specifier_ &&
           pointer_rank_ == composed_type->pointer_rank_ && array_specifiers_.do_match(composed_type->array_specifiers_, match);
}

// The collections are used through their untyped operations where only AstNode is needed, so this file does not depend on
// AttributeSection being defined.
AstNode* ComposedType::get_child_by_index(int index) const {
    int i = index;
    int attribute_count = attributes_.count();
    if (i >= 0 && i < attribute_count) return attributes_.nodes()[static_cast<std::size_t>(i)];
    i -= attribute_count;
    if (i == 0) return base_type_;
    i--;
    if (i >= 0 && i < array_specifiers_.count()) return array_specifiers_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void ComposedType::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int attribute_count = attributes_.count();
    if (i >= 0 && i < attribute_count) {
        attributes_.set_node(static_cast<std::size_t>(i), value);
        return;
    }
    i -= attribute_count;
    if (i == 0) {
        set_child_node(base_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i >= 0 && i < array_specifiers_.count()) {
        array_specifiers_.set(static_cast<std::size_t>(i), cast<ArraySpecifier>(value));
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ComposedType::get_child_slot_info(int index) const {
    int i = index;
    int attribute_count = attributes_.count();
    if (i >= 0 && i < attribute_count) return attributes_slot;
    i -= attribute_count;
    if (i == 0) return base_type_slot;
    i--;
    if (i >= 0 && i < array_specifiers_.count()) return array_specifiers_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* ComposedType::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::attribute_section) return &attributes_;
    if (&slot_kind == &Slots::array_specifier) return &array_specifiers_;
    return nullptr;
}

void ComposedType::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ComposedType*>(copy_node);
    copy->attributes_.clone_from(copy, attributes_);
    if (base_type_ != nullptr) copy->set_base_type(clone_as(base_type_.get()));
    copy->array_specifiers_.clone_from(copy, array_specifiers_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
