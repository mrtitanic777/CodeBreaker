// dotnet/decompiler/csharp/syntax/named_argument_expression.cpp -- see named_argument_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/named_argument_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

NamedArgumentExpression::NamedArgumentExpression(const std::string& name, Expression* expression) {
    set_name(name);
    set_expression(expression);
}

void NamedArgumentExpression::set_name_token(Identifier* value) { set_child_node(name_token_, value, 0); }
void NamedArgumentExpression::set_name(const std::string& value) { set_name_token(Identifier::create(value)); }
void NamedArgumentExpression::set_expression(Expression* value) { set_child_node(expression_, value, 1); }

bool NamedArgumentExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<NamedArgumentExpression>(other);
    return o != nullptr &&
           match_string(name(), o->name()) &&
           match_required(expression_, o->expression_, match);
}

int NamedArgumentExpression::get_child_count() const { return 2; }

AstNode* NamedArgumentExpression::get_child_by_index(int index) const {
    if (index == 0) return name_token_;
    if (index == 1) return expression_;
    throw std::out_of_range("index");
}

void NamedArgumentExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(name_token_, cast<Identifier>(value), index); return; }
    if (index == 1) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& NamedArgumentExpression::get_child_slot_info(int index) const {
    if (index == 0) return name_token_slot;
    if (index == 1) return expression_slot;
    throw std::out_of_range("index");
}

void NamedArgumentExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<NamedArgumentExpression*>(copy_node);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
