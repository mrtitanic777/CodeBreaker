// dotnet/decompiler/csharp/syntax/fixed_variable_initializer.cpp -- see fixed_variable_initializer.hpp.
#include "dotnet/decompiler/csharp/syntax/fixed_variable_initializer.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

FixedVariableInitializer::FixedVariableInitializer() {
}

FixedVariableInitializer::FixedVariableInitializer(const std::string& name_value, Expression* count_expression_value) {
    set_name(name_value);
    set_count_expression(count_expression_value);
}

void FixedVariableInitializer::set_name_token(Identifier* value) {
    set_child_node(name_token_, value, 0);
}

void FixedVariableInitializer::set_count_expression(Expression* value) {
    set_child_node(count_expression_, value, 1);
}

bool FixedVariableInitializer::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<FixedVariableInitializer>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_required(count_expression(), o->count_expression(), match);
}

int FixedVariableInitializer::get_child_count() const {
    return 2;
}

AstNode* FixedVariableInitializer::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return name_token_;
    i--;
    if (i == 0) return count_expression_;
    throw std::out_of_range("index");
}

void FixedVariableInitializer::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(count_expression_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& FixedVariableInitializer::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return name_token_slot;
    i--;
    if (i == 0) return count_expression_slot;
    throw std::out_of_range("index");
}

void FixedVariableInitializer::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<FixedVariableInitializer*>(copy_node);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (count_expression_ != nullptr) copy->set_count_expression(clone_as(count_expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
