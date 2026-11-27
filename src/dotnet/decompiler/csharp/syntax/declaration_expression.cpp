// dotnet/decompiler/csharp/syntax/declaration_expression.cpp -- see declaration_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/declaration_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

DeclarationExpression::DeclarationExpression(AstType* type, VariableDesignation* designation) {
    set_type(type);
    set_designation(designation);
}

void DeclarationExpression::set_type(AstType* value) { set_child_node(type_, value, 0); }
void DeclarationExpression::set_designation(VariableDesignation* value) { set_child_node(designation_, value, 1); }

bool DeclarationExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<DeclarationExpression>(other);
    return o != nullptr &&
           match_required(type_, o->type_, match) &&
           match_required(designation_, o->designation_, match);
}

int DeclarationExpression::get_child_count() const { return 2; }

AstNode* DeclarationExpression::get_child_by_index(int index) const {
    if (index == 0) return type_;
    if (index == 1) return designation_;
    throw std::out_of_range("index");
}

void DeclarationExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    if (index == 1) { set_child_node(designation_, cast<VariableDesignation>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& DeclarationExpression::get_child_slot_info(int index) const {
    if (index == 0) return type_slot;
    if (index == 1) return designation_slot;
    throw std::out_of_range("index");
}

void DeclarationExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<DeclarationExpression*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (designation_ != nullptr) copy->set_designation(clone_as(designation_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
