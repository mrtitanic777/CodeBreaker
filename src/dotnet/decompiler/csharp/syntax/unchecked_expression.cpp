// dotnet/decompiler/csharp/syntax/unchecked_expression.cpp -- see unchecked_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/unchecked_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

UncheckedExpression::UncheckedExpression(Expression* expression) {
    set_expression(expression);
}

void UncheckedExpression::set_expression(Expression* value) { set_child_node(expression_, value, 0); }

bool UncheckedExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<UncheckedExpression>(other);
    return o != nullptr &&
           match_required(expression_, o->expression_, match);
}

int UncheckedExpression::get_child_count() const { return 1; }

AstNode* UncheckedExpression::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    throw std::out_of_range("index");
}

void UncheckedExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& UncheckedExpression::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    throw std::out_of_range("index");
}

void UncheckedExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<UncheckedExpression*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
