// dotnet/decompiler/csharp/syntax/invocation_expression.cpp -- see invocation_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/invocation_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

InvocationExpression::InvocationExpression(Expression* target) {
    set_target(target);
}

InvocationExpression::InvocationExpression(Expression* target, const std::vector<Expression*>& arguments) : InvocationExpression(target) {
    arguments_.add_range(arguments);
}

InvocationExpression::InvocationExpression(Expression* target, std::initializer_list<Expression*> arguments) : InvocationExpression(target, std::vector<Expression*>(arguments)) {}

void InvocationExpression::set_target(Expression* value) { set_child_node(target_, value, 0); }

bool InvocationExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<InvocationExpression>(other);
    return o != nullptr &&
           match_required(target_, o->target_, match) &&
           arguments_.do_match(o->arguments_, match);
}

int InvocationExpression::get_child_count() const { return 1 + arguments_.count(); }

AstNode* InvocationExpression::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return target_;
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) return arguments_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void InvocationExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) { set_child_node(target_, cast<Expression>(value), index); return; }
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) { arguments_.set(static_cast<std::size_t>(i), cast<Expression>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& InvocationExpression::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return target_slot;
    i--;
    int n1 = arguments_.count();
    if (i >= 0 && i < n1) return arguments_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* InvocationExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::argument) return &arguments_;
    return nullptr;
}

void InvocationExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<InvocationExpression*>(copy_node);
    if (target_ != nullptr) copy->set_target(clone_as(target_.get()));
    copy->arguments_.clone_from(copy, arguments_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
