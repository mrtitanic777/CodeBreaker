// dotnet/decompiler/csharp/syntax/foreach_statement.cpp -- see foreach_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/foreach_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"
#include "dotnet/decompiler/csharp/syntax/variable_designation.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ForeachStatement::ForeachStatement() {
}

ForeachStatement::ForeachStatement(AstType* variable_type_value, VariableDesignation* variable_designation_value, Expression* in_expression_value, Statement* embedded_statement_value) {
    set_variable_type(variable_type_value);
    set_variable_designation(variable_designation_value);
    set_in_expression(in_expression_value);
    set_embedded_statement(embedded_statement_value);
}

void ForeachStatement::set_variable_type(AstType* value) {
    set_child_node(variable_type_, value, 0);
}

void ForeachStatement::set_variable_designation(VariableDesignation* value) {
    set_child_node(variable_designation_, value, 1);
}

void ForeachStatement::set_in_expression(Expression* value) {
    set_child_node(in_expression_, value, 2);
}

void ForeachStatement::set_embedded_statement(Statement* value) {
    set_child_node(embedded_statement_, value, 3);
}

bool ForeachStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ForeachStatement>(other);
    return o != nullptr
           && is_async() == o->is_async()
           && match_required(variable_type(), o->variable_type(), match)
           && match_required(variable_designation(), o->variable_designation(), match)
           && match_required(in_expression(), o->in_expression(), match)
           && match_required(embedded_statement(), o->embedded_statement(), match);
}

int ForeachStatement::get_child_count() const {
    return 4;
}

AstNode* ForeachStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return variable_type_;
    i--;
    if (i == 0) return variable_designation_;
    i--;
    if (i == 0) return in_expression_;
    i--;
    if (i == 0) return embedded_statement_;
    throw std::out_of_range("index");
}

void ForeachStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(variable_type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(variable_designation_, cast<VariableDesignation>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(in_expression_, cast<Expression>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(embedded_statement_, cast<Statement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ForeachStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return variable_type_slot;
    i--;
    if (i == 0) return variable_designation_slot;
    i--;
    if (i == 0) return in_expression_slot;
    i--;
    if (i == 0) return embedded_statement_slot;
    throw std::out_of_range("index");
}

void ForeachStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ForeachStatement*>(copy_node);
    if (variable_type_ != nullptr) copy->set_variable_type(clone_as(variable_type_.get()));
    if (variable_designation_ != nullptr) copy->set_variable_designation(clone_as(variable_designation_.get()));
    if (in_expression_ != nullptr) copy->set_in_expression(clone_as(in_expression_.get()));
    if (embedded_statement_ != nullptr) copy->set_embedded_statement(clone_as(embedded_statement_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
