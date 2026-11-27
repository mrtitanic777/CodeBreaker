// dotnet/decompiler/csharp/syntax/switch_statement.cpp -- see switch_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/switch_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

SwitchStatement::SwitchStatement() {
}

SwitchStatement::SwitchStatement(Expression* expression_value) {
    set_expression(expression_value);
}

SwitchStatement::SwitchStatement(Expression* expression_value, const std::vector<SwitchSection*>& switch_sections_value) : SwitchStatement(expression_value) {
    switch_sections().add_range(switch_sections_value);
}

void SwitchStatement::set_expression(Expression* value) {
    set_child_node(expression_, value, 0);
}

bool SwitchStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<SwitchStatement>(other);
    return o != nullptr
           && match_required(expression(), o->expression(), match)
           && switch_sections().do_match(o->switch_sections(), match);
}

int SwitchStatement::get_child_count() const {
    return 1 + switch_sections_.count();
}

AstNode* SwitchStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return expression_;
    i--;
    {
        int n = switch_sections_.count();
        if (i < n) return switch_sections_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void SwitchStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(expression_, cast<Expression>(value), index);
        return;
    }
    i--;
    {
        int n = switch_sections_.count();
        if (i < n) {
            switch_sections_.set(static_cast<std::size_t>(i), cast<SwitchSection>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& SwitchStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return expression_slot;
    i--;
    {
        int n = switch_sections_.count();
        if (i < n) return switch_sections_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* SwitchStatement::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::switch_section) return &switch_sections_;
    return nullptr;
}

void SwitchStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<SwitchStatement*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
    copy->switch_sections_.clone_from(copy, switch_sections_);
}

SwitchSection::SwitchSection() {
}

SwitchSection::SwitchSection(const std::vector<CaseLabel*>& case_labels_value) {
    case_labels().add_range(case_labels_value);
}

SwitchSection::SwitchSection(const std::vector<CaseLabel*>& case_labels_value, const std::vector<Statement*>& statements_value) : SwitchSection(case_labels_value) {
    statements().add_range(statements_value);
}

SwitchSection* SwitchSection::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<SwitchSection, AstNodeKind::SwitchSectionPlaceholder>>(pattern);
}

bool SwitchSection::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<SwitchSection>(other);
    return o != nullptr
           && case_labels().do_match(o->case_labels(), match)
           && statements().do_match(o->statements(), match);
}

int SwitchSection::get_child_count() const {
    return case_labels_.count() + statements_.count();
}

AstNode* SwitchSection::get_child_by_index(int index) const {
    int i = index;
    {
        int n = case_labels_.count();
        if (i < n) return case_labels_[static_cast<std::size_t>(i)];
        i -= n;
    }
    {
        int n = statements_.count();
        if (i < n) return statements_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void SwitchSection::set_child_by_index(int index, AstNode* value) {
    int i = index;
    {
        int n = case_labels_.count();
        if (i < n) {
            case_labels_.set(static_cast<std::size_t>(i), cast<CaseLabel>(value));
            return;
        }
        i -= n;
    }
    {
        int n = statements_.count();
        if (i < n) {
            statements_.set(static_cast<std::size_t>(i), cast<Statement>(value));
            return;
        }
        i -= n;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& SwitchSection::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = case_labels_.count();
        if (i < n) return case_labels_slot;
        i -= n;
    }
    {
        int n = statements_.count();
        if (i < n) return statements_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* SwitchSection::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::case_label) return &case_labels_;
    if (&slot_kind == &Slots::embedded_statement) return &statements_;
    return nullptr;
}

void SwitchSection::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<SwitchSection*>(copy_node);
    copy->case_labels_.clone_from(copy, case_labels_);
    copy->statements_.clone_from(copy, statements_);
}

CaseLabel::CaseLabel() {
}

CaseLabel::CaseLabel(Expression* expression_value) {
    set_expression(expression_value);
}

void CaseLabel::set_expression(Expression* value) {
    set_child_node(expression_, value, 0);
}

bool CaseLabel::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<CaseLabel>(other);
    return o != nullptr
           && match_optional(expression(), o->expression(), match);
}

int CaseLabel::get_child_count() const {
    return 1;
}

AstNode* CaseLabel::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return expression_;
    throw std::out_of_range("index");
}

void CaseLabel::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(expression_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& CaseLabel::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return expression_slot;
    throw std::out_of_range("index");
}

void CaseLabel::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<CaseLabel*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
