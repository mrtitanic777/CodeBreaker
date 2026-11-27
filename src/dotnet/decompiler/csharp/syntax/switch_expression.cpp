// dotnet/decompiler/csharp/syntax/switch_expression.cpp -- see switch_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/switch_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

SwitchExpression::SwitchExpression(Expression* expression) {
    set_expression(expression);
}

SwitchExpression::SwitchExpression(Expression* expression, const std::vector<SwitchExpressionSection*>& switch_sections) : SwitchExpression(expression) {
    switch_sections_.add_range(switch_sections);
}

SwitchExpression::SwitchExpression(Expression* expression, std::initializer_list<SwitchExpressionSection*> switch_sections) : SwitchExpression(expression, std::vector<SwitchExpressionSection*>(switch_sections)) {}

void SwitchExpression::set_expression(Expression* value) { set_child_node(expression_, value, 0); }

bool SwitchExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<SwitchExpression>(other);
    return o != nullptr &&
           match_required(expression_, o->expression_, match) &&
           switch_sections_.do_match(o->switch_sections_, match);
}

int SwitchExpression::get_child_count() const { return 1 + switch_sections_.count(); }

AstNode* SwitchExpression::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return expression_;
    i--;
    int n1 = switch_sections_.count();
    if (i >= 0 && i < n1) return switch_sections_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void SwitchExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    i--;
    int n1 = switch_sections_.count();
    if (i >= 0 && i < n1) { switch_sections_.set(static_cast<std::size_t>(i), cast<SwitchExpressionSection>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& SwitchExpression::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return expression_slot;
    i--;
    int n1 = switch_sections_.count();
    if (i >= 0 && i < n1) return switch_sections_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* SwitchExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::switch_expression_section) return &switch_sections_;
    return nullptr;
}

void SwitchExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<SwitchExpression*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
    copy->switch_sections_.clone_from(copy, switch_sections_);
}

SwitchExpressionSection::SwitchExpressionSection(Expression* pattern, Expression* body) {
    set_pattern(pattern);
    set_body(body);
}

void SwitchExpressionSection::set_pattern(Expression* value) { set_child_node(pattern_, value, 0); }
void SwitchExpressionSection::set_body(Expression* value) { set_child_node(body_, value, 1); }

bool SwitchExpressionSection::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<SwitchExpressionSection>(other);
    return o != nullptr &&
           match_required(pattern_, o->pattern_, match) &&
           match_required(body_, o->body_, match);
}

int SwitchExpressionSection::get_child_count() const { return 2; }

AstNode* SwitchExpressionSection::get_child_by_index(int index) const {
    if (index == 0) return pattern_;
    if (index == 1) return body_;
    throw std::out_of_range("index");
}

void SwitchExpressionSection::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(pattern_, cast<Expression>(value), index); return; }
    if (index == 1) { set_child_node(body_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& SwitchExpressionSection::get_child_slot_info(int index) const {
    if (index == 0) return pattern_slot;
    if (index == 1) return body_slot;
    throw std::out_of_range("index");
}

void SwitchExpressionSection::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<SwitchExpressionSection*>(copy_node);
    if (pattern_ != nullptr) copy->set_pattern(clone_as(pattern_.get()));
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
