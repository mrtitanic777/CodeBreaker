// dotnet/decompiler/csharp/syntax/object_create_expression.cpp -- see object_create_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/object_create_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ObjectCreateExpression::ObjectCreateExpression(AstType* type) {
    set_type(type);
}

ObjectCreateExpression::ObjectCreateExpression(AstType* type, const std::vector<Expression*>& arguments) : ObjectCreateExpression(type) {
    arguments_.add_range(arguments);
}

ObjectCreateExpression::ObjectCreateExpression(AstType* type, std::initializer_list<Expression*> arguments) : ObjectCreateExpression(type, std::vector<Expression*>(arguments)) {}

ObjectCreateExpression::ObjectCreateExpression(AstType* type, const std::vector<Expression*>& arguments, ArrayInitializerExpression* initializer) : ObjectCreateExpression(type, arguments) {
    set_initializer(initializer);
}

void ObjectCreateExpression::set_type(AstType* value) { set_child_node(type_, value, 0); }
void ObjectCreateExpression::set_initializer(ArrayInitializerExpression* value) { set_child_node(initializer_, value); }

bool ObjectCreateExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ObjectCreateExpression>(other);
    return o != nullptr &&
           match_required(type_, o->type_, match) &&
           arguments_.do_match(o->arguments_, match) &&
           match_optional(initializer_, o->initializer_, match);
}

int ObjectCreateExpression::get_child_count() const { return 2 + arguments_.count(); }

AstNode* ObjectCreateExpression::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return type_;
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) return arguments_[static_cast<std::size_t>(i)];
    i -= n1;
    if (i == 0) return initializer_;
    throw std::out_of_range("index");
}

void ObjectCreateExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) { arguments_.set(static_cast<std::size_t>(i), cast<Expression>(value)); return; }
    i -= n1;
    if (i == 0) { set_child_node(initializer_, cast<ArrayInitializerExpression>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ObjectCreateExpression::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return type_slot;
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) return arguments_slot;
    i -= n1;
    if (i == 0) return initializer_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* ObjectCreateExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::argument) return &arguments_;
    return nullptr;
}

void ObjectCreateExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ObjectCreateExpression*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    copy->arguments_.clone_from(copy, arguments_);
    if (initializer_ != nullptr) copy->set_initializer(clone_as(initializer_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
