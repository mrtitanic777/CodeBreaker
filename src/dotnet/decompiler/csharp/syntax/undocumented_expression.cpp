// dotnet/decompiler/csharp/syntax/undocumented_expression.cpp -- see undocumented_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/undocumented_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

UndocumentedExpression::UndocumentedExpression(UndocumentedExpressionType undocumented_expression_type) {
    set_undocumented_expression_type(undocumented_expression_type);
}

UndocumentedExpression::UndocumentedExpression(UndocumentedExpressionType undocumented_expression_type, const std::vector<Expression*>& arguments) : UndocumentedExpression(undocumented_expression_type) {
    arguments_.add_range(arguments);
}

UndocumentedExpression::UndocumentedExpression(UndocumentedExpressionType undocumented_expression_type, std::initializer_list<Expression*> arguments) : UndocumentedExpression(undocumented_expression_type, std::vector<Expression*>(arguments)) {}


bool UndocumentedExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<UndocumentedExpression>(other);
    return o != nullptr &&
           undocumented_expression_type_ == o->undocumented_expression_type_ &&
           arguments_.do_match(o->arguments_, match);
}

int UndocumentedExpression::get_child_count() const { return arguments_.count(); }

AstNode* UndocumentedExpression::get_child_by_index(int index) const {
    int i = index;
    int n0 = arguments_.count();
    if (i >= 0 && i < n0) return arguments_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void UndocumentedExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int n0 = arguments_.count();
    if (i >= 0 && i < n0) { arguments_.set(static_cast<std::size_t>(i), cast<Expression>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& UndocumentedExpression::get_child_slot_info(int index) const {
    int i = index;
    int n0 = arguments_.count();
    if (i >= 0 && i < n0) return arguments_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* UndocumentedExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::argument) return &arguments_;
    return nullptr;
}

void UndocumentedExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<UndocumentedExpression*>(copy_node);
    copy->arguments_.clone_from(copy, arguments_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
