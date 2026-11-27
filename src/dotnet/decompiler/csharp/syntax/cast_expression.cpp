// dotnet/decompiler/csharp/syntax/cast_expression.cpp -- see cast_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/cast_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

CastExpression::CastExpression(AstType* type, Expression* expression) {
    set_type(type);
    set_expression(expression);
}

void CastExpression::set_type(AstType* value) { set_child_node(type_, value, 0); }
void CastExpression::set_expression(Expression* value) { set_child_node(expression_, value, 1); }

bool CastExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<CastExpression>(other);
    return o != nullptr &&
           match_required(type_, o->type_, match) &&
           match_required(expression_, o->expression_, match);
}

int CastExpression::get_child_count() const { return 2; }

AstNode* CastExpression::get_child_by_index(int index) const {
    if (index == 0) return type_;
    if (index == 1) return expression_;
    throw std::out_of_range("index");
}

void CastExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    if (index == 1) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& CastExpression::get_child_slot_info(int index) const {
    if (index == 0) return type_slot;
    if (index == 1) return expression_slot;
    throw std::out_of_range("index");
}

void CastExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<CastExpression*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
