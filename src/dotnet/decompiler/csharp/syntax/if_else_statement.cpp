// dotnet/decompiler/csharp/syntax/if_else_statement.cpp -- see if_else_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/if_else_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

IfElseStatement::IfElseStatement() {
}

IfElseStatement::IfElseStatement(Expression* condition_value, Statement* true_statement_value) {
    set_condition(condition_value);
    set_true_statement(true_statement_value);
}

IfElseStatement::IfElseStatement(Expression* condition_value, Statement* true_statement_value, Statement* false_statement_value) : IfElseStatement(condition_value, true_statement_value) {
    set_false_statement(false_statement_value);
}

void IfElseStatement::set_condition(Expression* value) {
    set_child_node(condition_, value, 0);
}

void IfElseStatement::set_true_statement(Statement* value) {
    set_child_node(true_statement_, value, 1);
}

void IfElseStatement::set_false_statement(Statement* value) {
    set_child_node(false_statement_, value, 2);
}

bool IfElseStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<IfElseStatement>(other);
    return o != nullptr
           && match_required(condition(), o->condition(), match)
           && match_required(true_statement(), o->true_statement(), match)
           && match_optional(false_statement(), o->false_statement(), match);
}

int IfElseStatement::get_child_count() const {
    return 3;
}

AstNode* IfElseStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return condition_;
    i--;
    if (i == 0) return true_statement_;
    i--;
    if (i == 0) return false_statement_;
    throw std::out_of_range("index");
}

void IfElseStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(condition_, cast<Expression>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(true_statement_, cast<Statement>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(false_statement_, cast<Statement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& IfElseStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return condition_slot;
    i--;
    if (i == 0) return true_statement_slot;
    i--;
    if (i == 0) return false_statement_slot;
    throw std::out_of_range("index");
}

void IfElseStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<IfElseStatement*>(copy_node);
    if (condition_ != nullptr) copy->set_condition(clone_as(condition_.get()));
    if (true_statement_ != nullptr) copy->set_true_statement(clone_as(true_statement_.get()));
    if (false_statement_ != nullptr) copy->set_false_statement(clone_as(false_statement_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
