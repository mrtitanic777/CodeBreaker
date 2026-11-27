// dotnet/decompiler/csharp/syntax/named_expression.cpp -- see named_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/named_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

NamedExpression::NamedExpression(const std::string& name, Expression* expression) {
    set_name(name);
    set_expression(expression);
}

void NamedExpression::set_name_token(Identifier* value) { set_child_node(name_token_, value, 0); }
void NamedExpression::set_name(const std::string& value) { set_name_token(Identifier::create(value)); }
void NamedExpression::set_expression(Expression* value) { set_child_node(expression_, value, 1); }

bool NamedExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<NamedExpression>(other);
    return o != nullptr &&
           match_string(name(), o->name()) &&
           match_required(expression_, o->expression_, match);
}

int NamedExpression::get_child_count() const { return 2; }

AstNode* NamedExpression::get_child_by_index(int index) const {
    if (index == 0) return name_token_;
    if (index == 1) return expression_;
    throw std::out_of_range("index");
}

void NamedExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(name_token_, cast<Identifier>(value), index); return; }
    if (index == 1) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& NamedExpression::get_child_slot_info(int index) const {
    if (index == 0) return name_token_slot;
    if (index == 1) return expression_slot;
    throw std::out_of_range("index");
}

void NamedExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<NamedExpression*>(copy_node);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
