// dotnet/decompiler/csharp/syntax/expression_statement.cpp -- see expression_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/expression_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ExpressionStatement::ExpressionStatement() {
}

ExpressionStatement::ExpressionStatement(Expression* expression_value) {
    set_expression(expression_value);
}

void ExpressionStatement::set_expression(Expression* value) {
    set_child_node(expression_, value, 0);
}

bool ExpressionStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ExpressionStatement>(other);
    return o != nullptr
           && match_required(expression(), o->expression(), match);
}

int ExpressionStatement::get_child_count() const {
    return 1;
}

AstNode* ExpressionStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return expression_;
    throw std::out_of_range("index");
}

void ExpressionStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(expression_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ExpressionStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return expression_slot;
    throw std::out_of_range("index");
}

void ExpressionStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ExpressionStatement*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
