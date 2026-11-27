// dotnet/decompiler/csharp/syntax/do_while_statement.cpp -- see do_while_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/do_while_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

DoWhileStatement::DoWhileStatement(Expression* condition_value, Statement* embedded_statement_value) {
    set_condition(condition_value);
    set_embedded_statement(embedded_statement_value);
}

DoWhileStatement::DoWhileStatement() {
}

DoWhileStatement::DoWhileStatement(Statement* embedded_statement_value, Expression* condition_value) {
    set_embedded_statement(embedded_statement_value);
    set_condition(condition_value);
}

void DoWhileStatement::set_embedded_statement(Statement* value) {
    set_child_node(embedded_statement_, value, 0);
}

void DoWhileStatement::set_condition(Expression* value) {
    set_child_node(condition_, value, 1);
}

bool DoWhileStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<DoWhileStatement>(other);
    return o != nullptr
           && match_required(embedded_statement(), o->embedded_statement(), match)
           && match_required(condition(), o->condition(), match);
}

int DoWhileStatement::get_child_count() const {
    return 2;
}

AstNode* DoWhileStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return embedded_statement_;
    i--;
    if (i == 0) return condition_;
    throw std::out_of_range("index");
}

void DoWhileStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(embedded_statement_, cast<Statement>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(condition_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& DoWhileStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return embedded_statement_slot;
    i--;
    if (i == 0) return condition_slot;
    throw std::out_of_range("index");
}

void DoWhileStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<DoWhileStatement*>(copy_node);
    if (embedded_statement_ != nullptr) copy->set_embedded_statement(clone_as(embedded_statement_.get()));
    if (condition_ != nullptr) copy->set_condition(clone_as(condition_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
