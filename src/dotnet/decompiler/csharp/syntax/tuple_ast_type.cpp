// dotnet/decompiler/csharp/syntax/tuple_ast_type.cpp -- see tuple_ast_type.hpp.
#include "dotnet/decompiler/csharp/syntax/tuple_ast_type.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::syntax {

// ---- TupleTypeElement ----------------------------------------------------------------------------------------------------------------

bool TupleTypeElement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* element = as<TupleTypeElement>(other);
    return element != nullptr && match_required(type_, element->type_, match) && match_string(name(), element->name());
}

AstNode* TupleTypeElement::get_child_by_index(int index) const {
    if (index == 0) return type_;
    if (index == 1) return name_token_;
    throw std::out_of_range("index");
}

void TupleTypeElement::set_child_by_index(int index, AstNode* value) {
    if (index == 0) {
        set_child_node(type_, cast<AstType>(value), index);
        return;
    }
    if (index == 1) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& TupleTypeElement::get_child_slot_info(int index) const {
    if (index == 0) return type_slot;
    if (index == 1) return name_token_slot;
    throw std::out_of_range("index");
}

void TupleTypeElement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<TupleTypeElement*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
}

// ---- TupleAstType --------------------------------------------------------------------------------------------------------------------

bool TupleAstType::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* tuple = as<TupleAstType>(other);
    return tuple != nullptr && elements_.do_match(tuple->elements_, match);
}

AstNode* TupleAstType::get_child_by_index(int index) const {
    if (index >= 0 && index < elements_.count()) return elements_[static_cast<std::size_t>(index)];
    throw std::out_of_range("index");
}

void TupleAstType::set_child_by_index(int index, AstNode* value) {
    if (index >= 0 && index < elements_.count()) {
        elements_.set(static_cast<std::size_t>(index), cast<TupleTypeElement>(value));
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& TupleAstType::get_child_slot_info(int index) const {
    if (index >= 0 && index < elements_.count()) return elements_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* TupleAstType::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::element) return &elements_;
    return nullptr;
}

void TupleAstType::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<TupleAstType*>(copy_node);
    copy->elements_.clone_from(copy, elements_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
