// dotnet/decompiler/csharp/syntax/array_create_expression.cpp -- see array_create_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/array_create_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ArrayCreateExpression::ArrayCreateExpression(AstType* type, const std::vector<Expression*>& arguments) {
    set_type(type);
    arguments_.add_range(arguments);
}

ArrayCreateExpression::ArrayCreateExpression(AstType* type, std::initializer_list<Expression*> arguments) : ArrayCreateExpression(type, std::vector<Expression*>(arguments)) {}

ArrayCreateExpression::ArrayCreateExpression(AstType* type, const std::vector<Expression*>& arguments, const std::vector<ArraySpecifier*>& additional_array_specifiers) : ArrayCreateExpression(type, arguments) {
    additional_array_specifiers_.add_range(additional_array_specifiers);
}

ArrayCreateExpression::ArrayCreateExpression(AstType* type, const std::vector<Expression*>& arguments, std::initializer_list<ArraySpecifier*> additional_array_specifiers) : ArrayCreateExpression(type, arguments, std::vector<ArraySpecifier*>(additional_array_specifiers)) {}

ArrayCreateExpression::ArrayCreateExpression(AstType* type, const std::vector<Expression*>& arguments, const std::vector<ArraySpecifier*>& additional_array_specifiers, ArrayInitializerExpression* initializer) : ArrayCreateExpression(type, arguments, additional_array_specifiers) {
    set_initializer(initializer);
}

void ArrayCreateExpression::set_type(AstType* value) { set_child_node(type_, value, 0); }
void ArrayCreateExpression::set_initializer(ArrayInitializerExpression* value) { set_child_node(initializer_, value); }

bool ArrayCreateExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ArrayCreateExpression>(other);
    return o != nullptr &&
           match_optional(type_, o->type_, match) &&
           arguments_.do_match(o->arguments_, match) &&
           additional_array_specifiers_.do_match(o->additional_array_specifiers_, match) &&
           match_optional(initializer_, o->initializer_, match);
}

int ArrayCreateExpression::get_child_count() const { return 2 + arguments_.count() + additional_array_specifiers_.count(); }

AstNode* ArrayCreateExpression::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return type_;
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) return arguments_[static_cast<std::size_t>(i)];
    i -= n1;
    int n2 = additional_array_specifiers_.count();
    if (i >= 0 && i < n2) return additional_array_specifiers_[static_cast<std::size_t>(i)];
    i -= n2;
    if (i == 0) return initializer_;
    throw std::out_of_range("index");
}

void ArrayCreateExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) { arguments_.set(static_cast<std::size_t>(i), cast<Expression>(value)); return; }
    i -= n1;
    int n2 = additional_array_specifiers_.count();
    if (i >= 0 && i < n2) { additional_array_specifiers_.set(static_cast<std::size_t>(i), cast<ArraySpecifier>(value)); return; }
    i -= n2;
    if (i == 0) { set_child_node(initializer_, cast<ArrayInitializerExpression>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ArrayCreateExpression::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return type_slot;
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) return arguments_slot;
    i -= n1;
    int n2 = additional_array_specifiers_.count();
    if (i >= 0 && i < n2) return additional_array_specifiers_slot;
    i -= n2;
    if (i == 0) return initializer_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* ArrayCreateExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::argument) return &arguments_;
    if (&slot_kind == &Slots::additional_array_specifier) return &additional_array_specifiers_;
    return nullptr;
}

void ArrayCreateExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ArrayCreateExpression*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    copy->arguments_.clone_from(copy, arguments_);
    copy->additional_array_specifiers_.clone_from(copy, additional_array_specifiers_);
    if (initializer_ != nullptr) copy->set_initializer(clone_as(initializer_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
