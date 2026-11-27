// dotnet/decompiler/csharp/syntax/lock_statement.cpp -- see lock_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/lock_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

LockStatement::LockStatement() {
}

LockStatement::LockStatement(Expression* expression_value, Statement* embedded_statement_value) {
    set_expression(expression_value);
    set_embedded_statement(embedded_statement_value);
}

void LockStatement::set_expression(Expression* value) {
    set_child_node(expression_, value, 0);
}

void LockStatement::set_embedded_statement(Statement* value) {
    set_child_node(embedded_statement_, value, 1);
}

bool LockStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<LockStatement>(other);
    return o != nullptr
           && match_required(expression(), o->expression(), match)
           && match_required(embedded_statement(), o->embedded_statement(), match);
}

int LockStatement::get_child_count() const {
    return 2;
}

AstNode* LockStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return expression_;
    i--;
    if (i == 0) return embedded_statement_;
    throw std::out_of_range("index");
}

void LockStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(expression_, cast<Expression>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(embedded_statement_, cast<Statement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& LockStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return expression_slot;
    i--;
    if (i == 0) return embedded_statement_slot;
    throw std::out_of_range("index");
}

void LockStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<LockStatement*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
    if (embedded_statement_ != nullptr) copy->set_embedded_statement(clone_as(embedded_statement_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
