// dotnet/decompiler/csharp/syntax/default_value_expression.cpp -- see default_value_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/default_value_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

DefaultValueExpression::DefaultValueExpression(AstType* type) {
    set_type(type);
}

void DefaultValueExpression::set_type(AstType* value) { set_child_node(type_, value, 0); }

bool DefaultValueExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<DefaultValueExpression>(other);
    return o != nullptr &&
           match_required(type_, o->type_, match);
}

int DefaultValueExpression::get_child_count() const { return 1; }

AstNode* DefaultValueExpression::get_child_by_index(int index) const {
    if (index == 0) return type_;
    throw std::out_of_range("index");
}

void DefaultValueExpression::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& DefaultValueExpression::get_child_slot_info(int index) const {
    if (index == 0) return type_slot;
    throw std::out_of_range("index");
}

void DefaultValueExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<DefaultValueExpression*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
