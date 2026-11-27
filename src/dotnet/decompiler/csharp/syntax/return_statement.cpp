// dotnet/decompiler/csharp/syntax/return_statement.cpp -- see return_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/return_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ReturnStatement::ReturnStatement() {
}

ReturnStatement::ReturnStatement(Expression* expression_value) {
    set_expression(expression_value);
}

void ReturnStatement::set_expression(Expression* value) {
    set_child_node(expression_, value, 0);
}

bool ReturnStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ReturnStatement>(other);
    return o != nullptr
           && match_optional(expression(), o->expression(), match);
}

int ReturnStatement::get_child_count() const {
    return 1;
}

AstNode* ReturnStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return expression_;
    throw std::out_of_range("index");
}

void ReturnStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(expression_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ReturnStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return expression_slot;
    throw std::out_of_range("index");
}

void ReturnStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ReturnStatement*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
