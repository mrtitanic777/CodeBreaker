// dotnet/decompiler/csharp/syntax/with_initializer_expression.cpp -- see with_initializer_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/with_initializer_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

WithInitializerExpression::WithInitializerExpression(Expression* expression, ArrayInitializerExpression* initializer) {
    set_expression(expression);
    set_initializer(initializer);
}

void WithInitializerExpression::set_expression(Expression* value) { set_child_node(expression_, value, 0); }
void WithInitializerExpression::set_initializer(ArrayInitializerExpression* value) { set_child_node(initializer_, value, 1); }

bool WithInitializerExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<WithInitializerExpression>(other);
    return o != nullptr &&
           match_required(expression_, o->expression_, match) &&
           match_required(initializer_, o->initializer_, match);
}

int WithInitializerExpression::get_child_count() const { return 2; }

AstNode* WithInitializerExpression::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    if (index == 1) return initializer_;
    throw std::out_of_range("index");
}

void WithInitializerExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    if (index == 1) { set_child_node(initializer_, cast<ArrayInitializerExpression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& WithInitializerExpression::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    if (index == 1) return initializer_slot;
    throw std::out_of_range("index");
}

void WithInitializerExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<WithInitializerExpression*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
    if (initializer_ != nullptr) copy->set_initializer(clone_as(initializer_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
