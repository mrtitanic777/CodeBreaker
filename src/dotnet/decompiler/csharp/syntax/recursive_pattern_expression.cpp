// dotnet/decompiler/csharp/syntax/recursive_pattern_expression.cpp -- see recursive_pattern_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/recursive_pattern_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

RecursivePatternExpression::RecursivePatternExpression(AstType* type, const std::vector<Expression*>& sub_patterns) {
    set_type(type);
    sub_patterns_.add_range(sub_patterns);
}

RecursivePatternExpression::RecursivePatternExpression(AstType* type, std::initializer_list<Expression*> sub_patterns) : RecursivePatternExpression(type, std::vector<Expression*>(sub_patterns)) {}

RecursivePatternExpression::RecursivePatternExpression(AstType* type, const std::vector<Expression*>& sub_patterns, VariableDesignation* designation) : RecursivePatternExpression(type, sub_patterns) {
    set_designation(designation);
}

void RecursivePatternExpression::set_type(AstType* value) { set_child_node(type_, value, 0); }
void RecursivePatternExpression::set_designation(VariableDesignation* value) { set_child_node(designation_, value); }

bool RecursivePatternExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<RecursivePatternExpression>(other);
    return o != nullptr &&
           match_optional(type_, o->type_, match) &&
           sub_patterns_.do_match(o->sub_patterns_, match) &&
           match_optional(designation_, o->designation_, match) &&
           is_positional_ == o->is_positional_;
}

int RecursivePatternExpression::get_child_count() const { return 2 + sub_patterns_.count(); }

AstNode* RecursivePatternExpression::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return type_;
    i--;
    int n1 = sub_patterns_.count();
    if (i >= 0 && i < n1) return sub_patterns_[static_cast<std::size_t>(i)];
    i -= n1;
    if (i == 0) return designation_;
    throw std::out_of_range("index");
}

void RecursivePatternExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    i--;
    int n1 = sub_patterns_.count();
    if (i >= 0 && i < n1) { sub_patterns_.set(static_cast<std::size_t>(i), cast<Expression>(value)); return; }
    i -= n1;
    if (i == 0) { set_child_node(designation_, cast<VariableDesignation>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& RecursivePatternExpression::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return type_slot;
    i--;
    int n1 = sub_patterns_.count();
    if (i >= 0 && i < n1) return sub_patterns_slot;
    i -= n1;
    if (i == 0) return designation_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* RecursivePatternExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::sub_pattern) return &sub_patterns_;
    return nullptr;
}

void RecursivePatternExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<RecursivePatternExpression*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    copy->sub_patterns_.clone_from(copy, sub_patterns_);
    if (designation_ != nullptr) copy->set_designation(clone_as(designation_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
