// dotnet/decompiler/csharp/syntax/stack_alloc_expression.cpp -- see stack_alloc_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/stack_alloc_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

StackAllocExpression::StackAllocExpression(AstType* type, Expression* count_expression, ArrayInitializerExpression* initializer) {
    set_type(type);
    set_count_expression(count_expression);
    set_initializer(initializer);
}

void StackAllocExpression::set_type(AstType* value) { set_child_node(type_, value, 0); }
void StackAllocExpression::set_count_expression(Expression* value) { set_child_node(count_expression_, value, 1); }
void StackAllocExpression::set_initializer(ArrayInitializerExpression* value) { set_child_node(initializer_, value, 2); }

bool StackAllocExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<StackAllocExpression>(other);
    return o != nullptr &&
           match_optional(type_, o->type_, match) &&
           match_optional(count_expression_, o->count_expression_, match) &&
           match_optional(initializer_, o->initializer_, match);
}

int StackAllocExpression::get_child_count() const { return 3; }

AstNode* StackAllocExpression::get_child_by_index(int index) const {
    if (index == 0) return type_;
    if (index == 1) return count_expression_;
    if (index == 2) return initializer_;
    throw std::out_of_range("index");
}

void StackAllocExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    if (index == 1) { set_child_node(count_expression_, cast<Expression>(value), index); return; }
    if (index == 2) { set_child_node(initializer_, cast<ArrayInitializerExpression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& StackAllocExpression::get_child_slot_info(int index) const {
    if (index == 0) return type_slot;
    if (index == 1) return count_expression_slot;
    if (index == 2) return initializer_slot;
    throw std::out_of_range("index");
}

void StackAllocExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<StackAllocExpression*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (count_expression_ != nullptr) copy->set_count_expression(clone_as(count_expression_.get()));
    if (initializer_ != nullptr) copy->set_initializer(clone_as(initializer_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
