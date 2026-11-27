// dotnet/decompiler/csharp/syntax/tuple_expression.cpp -- see tuple_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/tuple_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

TupleExpression::TupleExpression(const std::vector<Expression*>& elements) {
    elements_.add_range(elements);
}

TupleExpression::TupleExpression(std::initializer_list<Expression*> elements) : TupleExpression(std::vector<Expression*>(elements)) {}


bool TupleExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<TupleExpression>(other);
    return o != nullptr &&
           elements_.do_match(o->elements_, match);
}

int TupleExpression::get_child_count() const { return elements_.count(); }

AstNode* TupleExpression::get_child_by_index(int index) const {
    int i = index;
    int n0 = elements_.count();
    if (i >= 0 && i < n0) return elements_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void TupleExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int n0 = elements_.count();
    if (i >= 0 && i < n0) { elements_.set(static_cast<std::size_t>(i), cast<Expression>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& TupleExpression::get_child_slot_info(int index) const {
    int i = index;
    int n0 = elements_.count();
    if (i >= 0 && i < n0) return elements_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* TupleExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::expression) return &elements_;
    return nullptr;
}

void TupleExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<TupleExpression*>(copy_node);
    copy->elements_.clone_from(copy, elements_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
