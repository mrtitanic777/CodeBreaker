// dotnet/decompiler/csharp/syntax/anonymous_method_expression.cpp -- see anonymous_method_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/anonymous_method_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

AnonymousMethodExpression::AnonymousMethodExpression(const std::vector<ParameterDeclaration*>& parameters, BlockStatement* body) {
    parameters_.add_range(parameters);
    set_body(body);
}

void AnonymousMethodExpression::set_body(BlockStatement* value) { set_child_node(body_, value); }

bool AnonymousMethodExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<AnonymousMethodExpression>(other);
    return o != nullptr &&
           is_async_ == o->is_async_ &&
           parameters_.do_match(o->parameters_, match) &&
           match_required(body_, o->body_, match);
}

int AnonymousMethodExpression::get_child_count() const { return 1 + parameters_.count(); }

AstNode* AnonymousMethodExpression::get_child_by_index(int index) const {
    int i = index;
    int n0 = parameters_.count();
    if (i >= 0 && i < n0) return parameters_[static_cast<std::size_t>(i)];
    i -= n0;
    if (i == 0) return body_;
    throw std::out_of_range("index");
}

void AnonymousMethodExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int n0 = parameters_.count();
    if (i >= 0 && i < n0) { parameters_.set(static_cast<std::size_t>(i), cast<ParameterDeclaration>(value)); return; }
    i -= n0;
    if (i == 0) { set_child_node(body_, cast<BlockStatement>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& AnonymousMethodExpression::get_child_slot_info(int index) const {
    int i = index;
    int n0 = parameters_.count();
    if (i >= 0 && i < n0) return parameters_slot;
    i -= n0;
    if (i == 0) return body_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* AnonymousMethodExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::parameter) return &parameters_;
    return nullptr;
}

void AnonymousMethodExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<AnonymousMethodExpression*>(copy_node);
    copy->parameters_.clone_from(copy, parameters_);
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
