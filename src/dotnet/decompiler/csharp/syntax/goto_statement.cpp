// dotnet/decompiler/csharp/syntax/goto_statement.cpp -- see goto_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/goto_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

GotoStatement::GotoStatement() {
}

GotoStatement::GotoStatement(const std::string& label_value) {
    set_label(label_value);
}

void GotoStatement::set_label_token(Identifier* value) {
    set_child_node(label_token_, value, 0);
}

bool GotoStatement::do_match(AstNode* other, const pattern_matching::Match&) const {
    auto* o = as<GotoStatement>(other);
    return o != nullptr
           && match_string(label(), o->label());
}

int GotoStatement::get_child_count() const {
    return 1;
}

AstNode* GotoStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return label_token_;
    throw std::out_of_range("index");
}

void GotoStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(label_token_, cast<Identifier>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& GotoStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return label_token_slot;
    throw std::out_of_range("index");
}

void GotoStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<GotoStatement*>(copy_node);
    if (label_token_ != nullptr) copy->set_label_token(clone_as(label_token_.get()));
}

GotoCaseStatement::GotoCaseStatement() {
}

GotoCaseStatement::GotoCaseStatement(Expression* label_expression_value) {
    set_label_expression(label_expression_value);
}

void GotoCaseStatement::set_label_expression(Expression* value) {
    set_child_node(label_expression_, value, 0);
}

bool GotoCaseStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<GotoCaseStatement>(other);
    return o != nullptr
           && match_required(label_expression(), o->label_expression(), match);
}

int GotoCaseStatement::get_child_count() const {
    return 1;
}

AstNode* GotoCaseStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return label_expression_;
    throw std::out_of_range("index");
}

void GotoCaseStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(label_expression_, cast<Expression>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& GotoCaseStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return label_expression_slot;
    throw std::out_of_range("index");
}

void GotoCaseStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<GotoCaseStatement*>(copy_node);
    if (label_expression_ != nullptr) copy->set_label_expression(clone_as(label_expression_.get()));
}

bool GotoDefaultStatement::do_match(AstNode* other, const pattern_matching::Match&) const {
    return as<GotoDefaultStatement>(other) != nullptr;
}

int GotoDefaultStatement::get_child_count() const {
    return 0;
}

AstNode* GotoDefaultStatement::get_child_by_index(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void GotoDefaultStatement::set_child_by_index(int index, AstNode*) {
    throw std::out_of_range(std::to_string(index));
}

const CSharpSlotInfo& GotoDefaultStatement::get_child_slot_info(int index) const {
    throw std::out_of_range(std::to_string(index));
}

void GotoDefaultStatement::clone_children_into(AstNode*) const {

}

}  // namespace cb::dotnet::decompiler::csharp::syntax
