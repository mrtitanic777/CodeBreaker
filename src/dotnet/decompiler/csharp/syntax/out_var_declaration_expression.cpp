// dotnet/decompiler/csharp/syntax/out_var_declaration_expression.cpp -- see out_var_declaration_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/out_var_declaration_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

OutVarDeclarationExpression::OutVarDeclarationExpression(AstType* type, const std::string& name) {
    set_type(type);
    set_variable(make<VariableInitializer>(name));
}

OutVarDeclarationExpression::OutVarDeclarationExpression(AstType* type, VariableInitializer* variable) {
    set_type(type);
    set_variable(variable);
}

void OutVarDeclarationExpression::set_type(AstType* value) { set_child_node(type_, value, 0); }
void OutVarDeclarationExpression::set_variable(VariableInitializer* value) { set_child_node(variable_, value, 1); }

bool OutVarDeclarationExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<OutVarDeclarationExpression>(other);
    return o != nullptr &&
           match_required(type_, o->type_, match) &&
           match_required(variable_, o->variable_, match);
}

int OutVarDeclarationExpression::get_child_count() const { return 2; }

AstNode* OutVarDeclarationExpression::get_child_by_index(int index) const {
    if (index == 0) return type_;
    if (index == 1) return variable_;
    throw std::out_of_range("index");
}

void OutVarDeclarationExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    if (index == 1) { set_child_node(variable_, cast<VariableInitializer>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& OutVarDeclarationExpression::get_child_slot_info(int index) const {
    if (index == 0) return type_slot;
    if (index == 1) return variable_slot;
    throw std::out_of_range("index");
}

void OutVarDeclarationExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<OutVarDeclarationExpression*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (variable_ != nullptr) copy->set_variable(clone_as(variable_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
