// dotnet/decompiler/csharp/syntax/while_statement.cpp -- see while_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/while_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

WhileStatement::WhileStatement() {
}

WhileStatement::WhileStatement(Expression* condition_value, Statement* embedded_statement_value) {
    set_condition(condition_value);
    set_embedded_statement(embedded_statement_value);
}

void WhileStatement::set_condition(Expression* value) {
    set_child_node(condition_, value, 0);
}

void WhileStatement::set_embedded_statement(Statement* value) {
    set_child_node(embedded_statement_, value, 1);
}

bool WhileStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<WhileStatement>(other);
    return o != nullptr
           && match_required(condition(), o->condition(), match)
           && match_required(embedded_statement(), o->embedded_statement(), match);
}

int WhileStatement::get_child_count() const {
    return 2;
}

AstNode* WhileStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return condition_;
    i--;
    if (i == 0) return embedded_statement_;
    throw std::out_of_range("index");
}

void WhileStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(condition_, cast<Expression>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(embedded_statement_, cast<Statement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& WhileStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return condition_slot;
    i--;
    if (i == 0) return embedded_statement_slot;
    throw std::out_of_range("index");
}

void WhileStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<WhileStatement*>(copy_node);
    if (condition_ != nullptr) copy->set_condition(clone_as(condition_.get()));
    if (embedded_statement_ != nullptr) copy->set_embedded_statement(clone_as(embedded_statement_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
