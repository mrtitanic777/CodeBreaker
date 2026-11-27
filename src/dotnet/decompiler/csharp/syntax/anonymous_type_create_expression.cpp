// dotnet/decompiler/csharp/syntax/anonymous_type_create_expression.cpp -- see anonymous_type_create_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/anonymous_type_create_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

AnonymousTypeCreateExpression::AnonymousTypeCreateExpression(const std::vector<Expression*>& initializers) {
    initializers_.add_range(initializers);
}

AnonymousTypeCreateExpression::AnonymousTypeCreateExpression(std::initializer_list<Expression*> initializers) : AnonymousTypeCreateExpression(std::vector<Expression*>(initializers)) {}


bool AnonymousTypeCreateExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<AnonymousTypeCreateExpression>(other);
    return o != nullptr &&
           initializers_.do_match(o->initializers_, match);
}

int AnonymousTypeCreateExpression::get_child_count() const { return initializers_.count(); }

AstNode* AnonymousTypeCreateExpression::get_child_by_index(int index) const {
    int i = index;
    int n0 = initializers_.count();
    if (i >= 0 && i < n0) return initializers_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void AnonymousTypeCreateExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int n0 = initializers_.count();
    if (i >= 0 && i < n0) { initializers_.set(static_cast<std::size_t>(i), cast<Expression>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& AnonymousTypeCreateExpression::get_child_slot_info(int index) const {
    int i = index;
    int n0 = initializers_.count();
    if (i >= 0 && i < n0) return initializers_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* AnonymousTypeCreateExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::expression) return &initializers_;
    return nullptr;
}

void AnonymousTypeCreateExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<AnonymousTypeCreateExpression*>(copy_node);
    copy->initializers_.clone_from(copy, initializers_);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
