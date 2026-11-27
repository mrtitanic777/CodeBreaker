// dotnet/decompiler/csharp/syntax/variable_initializer.cpp -- see variable_initializer.hpp.
#include "dotnet/decompiler/csharp/syntax/variable_initializer.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

VariableInitializer::VariableInitializer() {
}

VariableInitializer::VariableInitializer(const std::string& name_value) {
    set_name(name_value);
}

VariableInitializer::VariableInitializer(const std::string& name_value, Expression* initializer_value) : VariableInitializer(name_value) {
    set_initializer(initializer_value);
}

void VariableInitializer::set_name_token(Identifier* value) {
    set_child_node(name_token_, value, 0);
}

void VariableInitializer::set_initializer(Expression* value) {
    set_child_node(initializer_, value, 1);
}

VariableInitializer* VariableInitializer::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<VariableInitializer, AstNodeKind::VariableInitializerPlaceholder>>(pattern);
}

bool VariableInitializer::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<VariableInitializer>(other);
    return o != nullptr
           && match_string(name(), o->name())
           && match_optional(initializer(), o->initializer(), match);
}

int VariableInitializer::get_child_count() const {
    return 2;
}

AstNode* VariableInitializer::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return name_token_;
    i--;
    if (i == 0) return initializer_;
    throw std::out_of_range("index");
}

void VariableInitializer::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(name_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(initializer_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& VariableInitializer::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return name_token_slot;
    i--;
    if (i == 0) return initializer_slot;
    throw std::out_of_range("index");
}

void VariableInitializer::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<VariableInitializer*>(copy_node);
    if (name_token_ != nullptr) copy->set_name_token(clone_as(name_token_.get()));
    if (initializer_ != nullptr) copy->set_initializer(clone_as(initializer_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
