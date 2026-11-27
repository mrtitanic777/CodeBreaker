// dotnet/decompiler/csharp/syntax/variable_declaration_statement.cpp -- see variable_declaration_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/variable_declaration_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/variable_initializer.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

VariableDeclarationStatement::VariableDeclarationStatement(AstType* type_value, const std::string& name_value, Expression* initializer) {
    set_type(type_value);
    variables().add(make<VariableInitializer>(name_value, initializer));
}

VariableDeclarationStatement::VariableDeclarationStatement() {
}

VariableDeclarationStatement::VariableDeclarationStatement(Modifiers modifiers_value, AstType* type_value) {
    set_modifiers(modifiers_value);
    set_type(type_value);
}

VariableDeclarationStatement::VariableDeclarationStatement(Modifiers modifiers_value, AstType* type_value, const std::vector<VariableInitializer*>& variables_value) : VariableDeclarationStatement(modifiers_value, type_value) {
    variables().add_range(variables_value);
}

void VariableDeclarationStatement::set_type(AstType* value) {
    set_child_node(type_, value, 0);
}

bool VariableDeclarationStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<VariableDeclarationStatement>(other);
    return o != nullptr
           && (modifiers() == Modifiers::Any || modifiers() == o->modifiers())
           && match_required(type(), o->type(), match)
           && variables().do_match(o->variables(), match);
}

int VariableDeclarationStatement::get_child_count() const {
    return 1 + variables_.count();
}

AstNode* VariableDeclarationStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return type_;
    i--;
    {
        int n = variables_.count();
        if (i < n) return variables_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void VariableDeclarationStatement::set_child_by_index(int index, AstNode* value) {
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
    throw std::out_of_range("index");
}

const CSharpSlotInfo& VariableDeclarationStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return type_slot;
    i--;
    {
        int n = variables_.count();
        if (i < n) return variables_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* VariableDeclarationStatement::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::variable) return &variables_;
    return nullptr;
}

void VariableDeclarationStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<VariableDeclarationStatement*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    copy->variables_.clone_from(copy, variables_);
}

VariableInitializer* VariableDeclarationStatement::get_variable(const std::string& name) const {
    return variables_.first_or_null([&name](VariableInitializer* vi) { return vi->name() == name; });
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
