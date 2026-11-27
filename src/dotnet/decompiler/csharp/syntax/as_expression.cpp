// dotnet/decompiler/csharp/syntax/as_expression.cpp -- see as_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/as_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

AsExpression::AsExpression(Expression* expression, AstType* type) {
    set_expression(expression);
    set_type(type);
}

void AsExpression::set_expression(Expression* value) { set_child_node(expression_, value, 0); }
void AsExpression::set_type(AstType* value) { set_child_node(type_, value, 1); }

bool AsExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<AsExpression>(other);
    return o != nullptr &&
           match_required(expression_, o->expression_, match) &&
           match_required(type_, o->type_, match);
}

int AsExpression::get_child_count() const { return 2; }

AstNode* AsExpression::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    if (index == 1) return type_;
    throw std::out_of_range("index");
}

void AsExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    if (index == 1) { set_child_node(type_, cast<AstType>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& AsExpression::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    if (index == 1) return type_slot;
    throw std::out_of_range("index");
}

void AsExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<AsExpression*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
