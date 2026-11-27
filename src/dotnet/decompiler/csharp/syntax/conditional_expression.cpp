// dotnet/decompiler/csharp/syntax/conditional_expression.cpp -- see conditional_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/conditional_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ConditionalExpression::ConditionalExpression(Expression* condition, Expression* true_expression, Expression* false_expression) {
    set_condition(condition);
    set_true_expression(true_expression);
    set_false_expression(false_expression);
}

void ConditionalExpression::set_condition(Expression* value) { set_child_node(condition_, value, 0); }
void ConditionalExpression::set_true_expression(Expression* value) { set_child_node(true_expression_, value, 1); }
void ConditionalExpression::set_false_expression(Expression* value) { set_child_node(false_expression_, value, 2); }

bool ConditionalExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ConditionalExpression>(other);
    return o != nullptr &&
           match_required(condition_, o->condition_, match) &&
           match_required(true_expression_, o->true_expression_, match) &&
           match_required(false_expression_, o->false_expression_, match);
}

int ConditionalExpression::get_child_count() const { return 3; }

AstNode* ConditionalExpression::get_child_by_index(int index) const {
    if (index == 0) return condition_;
    if (index == 1) return true_expression_;
    if (index == 2) return false_expression_;
    throw std::out_of_range("index");
}

void ConditionalExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(condition_, cast<Expression>(value), index); return; }
    if (index == 1) { set_child_node(true_expression_, cast<Expression>(value), index); return; }
    if (index == 2) { set_child_node(false_expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ConditionalExpression::get_child_slot_info(int index) const {
    if (index == 0) return condition_slot;
    if (index == 1) return true_expression_slot;
    if (index == 2) return false_expression_slot;
    throw std::out_of_range("index");
}

void ConditionalExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ConditionalExpression*>(copy_node);
    if (condition_ != nullptr) copy->set_condition(clone_as(condition_.get()));
    if (true_expression_ != nullptr) copy->set_true_expression(clone_as(true_expression_.get()));
    if (false_expression_ != nullptr) copy->set_false_expression(clone_as(false_expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
