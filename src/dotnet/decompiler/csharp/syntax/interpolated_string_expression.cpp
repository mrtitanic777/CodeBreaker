// dotnet/decompiler/csharp/syntax/interpolated_string_expression.cpp -- see interpolated_string_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/interpolated_string_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {



InterpolatedStringExpression::InterpolatedStringExpression(const std::vector<InterpolatedStringContent*>& content) {
    content_.add_range(content);
}

InterpolatedStringExpression::InterpolatedStringExpression(std::initializer_list<InterpolatedStringContent*> content) : InterpolatedStringExpression(std::vector<InterpolatedStringContent*>(content)) {}


bool InterpolatedStringExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<InterpolatedStringExpression>(other);
    return o != nullptr &&
           content_.do_match(o->content_, match);
}

int InterpolatedStringExpression::get_child_count() const { return content_.count(); }

AstNode* InterpolatedStringExpression::get_child_by_index(int index) const {
    int i = index;
    int n0 = content_.count();
    if (i >= 0 && i < n0) return content_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void InterpolatedStringExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int n0 = content_.count();
    if (i >= 0 && i < n0) { content_.set(static_cast<std::size_t>(i), cast<InterpolatedStringContent>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& InterpolatedStringExpression::get_child_slot_info(int index) const {
    int i = index;
    int n0 = content_.count();
    if (i >= 0 && i < n0) return content_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* InterpolatedStringExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::content) return &content_;
    return nullptr;
}

void InterpolatedStringExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<InterpolatedStringExpression*>(copy_node);
    copy->content_.clone_from(copy, content_);
}

Interpolation::Interpolation(Expression* expression, int alignment, std::optional<std::string> suffix) : alignment_(alignment), suffix_(std::move(suffix)) {
    set_expression(expression);
}

void Interpolation::set_expression(Expression* value) { set_child_node(expression_, value, 0); }

bool Interpolation::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<Interpolation>(other);
    return o != nullptr &&
           match_required(expression_, o->expression_, match) &&
           alignment_ == o->alignment_ &&
           match_string(suffix_.value_or(""), o->suffix_.value_or(""));
}

int Interpolation::get_child_count() const { return 1; }

AstNode* Interpolation::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    throw std::out_of_range("index");
}

void Interpolation::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& Interpolation::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    throw std::out_of_range("index");
}

void Interpolation::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<Interpolation*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

InterpolatedStringText::InterpolatedStringText(const std::string& text) {
    set_text(text);
}


bool InterpolatedStringText::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<InterpolatedStringText>(other);
    return o != nullptr &&
           match_string(text(), o->text());
}


}  // namespace cb::dotnet::decompiler::csharp::syntax
