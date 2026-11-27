// dotnet/decompiler/csharp/syntax/direction_expression.cpp -- see direction_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/direction_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

DirectionExpression::DirectionExpression(FieldDirection field_direction, Expression* expression) {
    set_field_direction(field_direction);
    set_expression(expression);
}

void DirectionExpression::set_expression(Expression* value) { set_child_node(expression_, value, 0); }

bool DirectionExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<DirectionExpression>(other);
    return o != nullptr &&
           field_direction_ == o->field_direction_ &&
           match_required(expression_, o->expression_, match);
}

int DirectionExpression::get_child_count() const { return 1; }

AstNode* DirectionExpression::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    throw std::out_of_range("index");
}

void DirectionExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& DirectionExpression::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    throw std::out_of_range("index");
}

void DirectionExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<DirectionExpression*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
