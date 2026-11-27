// dotnet/decompiler/csharp/syntax/parenthesized_expression.cpp -- see parenthesized_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/parenthesized_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ParenthesizedExpression::ParenthesizedExpression(Expression* expression) {
    set_expression(expression);
}

void ParenthesizedExpression::set_expression(Expression* value) { set_child_node(expression_, value, 0); }

bool ParenthesizedExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ParenthesizedExpression>(other);
    return o != nullptr &&
           match_required(expression_, o->expression_, match);
}

int ParenthesizedExpression::get_child_count() const { return 1; }

AstNode* ParenthesizedExpression::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    throw std::out_of_range("index");
}

void ParenthesizedExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ParenthesizedExpression::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    throw std::out_of_range("index");
}

void ParenthesizedExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ParenthesizedExpression*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
