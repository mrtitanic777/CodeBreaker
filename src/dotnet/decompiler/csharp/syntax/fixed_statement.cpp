// dotnet/decompiler/csharp/syntax/fixed_statement.cpp -- see fixed_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/fixed_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"
#include "dotnet/decompiler/csharp/syntax/variable_initializer.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

FixedStatement::FixedStatement() {
}

FixedStatement::FixedStatement(AstType* type_value, const std::vector<VariableInitializer*>& variables_value, Statement* embedded_statement_value) {
    set_type(type_value);
    variables().add_range(variables_value);
    set_embedded_statement(embedded_statement_value);
}

void FixedStatement::set_type(AstType* value) {
    set_child_node(type_, value, 0);
}

void FixedStatement::set_embedded_statement(Statement* value) {
    set_child_node(embedded_statement_, value);
}

bool FixedStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<FixedStatement>(other);
    return o != nullptr
           && match_required(type(), o->type(), match)
           && variables().do_match(o->variables(), match)
           && match_required(embedded_statement(), o->embedded_statement(), match);
}

int FixedStatement::get_child_count() const {
    return 2 + variables_.count();
}

AstNode* FixedStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return type_;
    i--;
    {
        int n = variables_.count();
        if (i < n) return variables_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return embedded_statement_;
    throw std::out_of_range("index");
}

void FixedStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(type_, cast<AstType>(value), index);
        return;
    }
    i--;
    {
        int n = variables_.count();
        if (i < n) {
            variables_.set(static_cast<std::size_t>(i), cast<VariableInitializer>(value));
            return;
        }
        i -= n;
    }
    if (i == 0) {
        set_child_node(embedded_statement_, cast<Statement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& FixedStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return type_slot;
    i--;
    {
        int n = variables_.count();
        if (i < n) return variables_slot;
        i -= n;
    }
    if (i == 0) return embedded_statement_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* FixedStatement::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::variable) return &variables_;
    return nullptr;
}

void FixedStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<FixedStatement*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    copy->variables_.clone_from(copy, variables_);
    if (embedded_statement_ != nullptr) copy->set_embedded_statement(clone_as(embedded_statement_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
