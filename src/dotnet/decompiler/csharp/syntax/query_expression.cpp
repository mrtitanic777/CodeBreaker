// dotnet/decompiler/csharp/syntax/query_expression.cpp -- see query_expression.hpp.
#include "dotnet/decompiler/csharp/syntax/query_expression.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {



QueryExpression::QueryExpression(const std::vector<QueryClause*>& clauses) {
    clauses_.add_range(clauses);
}

QueryExpression::QueryExpression(std::initializer_list<QueryClause*> clauses) : QueryExpression(std::vector<QueryClause*>(clauses)) {}


bool QueryExpression::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryExpression>(other);
    return o != nullptr &&
           clauses_.do_match(o->clauses_, match);
}

int QueryExpression::get_child_count() const { return clauses_.count(); }

AstNode* QueryExpression::get_child_by_index(int index) const {
    int i = index;
    int n0 = clauses_.count();
    if (i >= 0 && i < n0) return clauses_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void QueryExpression::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int n0 = clauses_.count();
    if (i >= 0 && i < n0) { clauses_.set(static_cast<std::size_t>(i), cast<QueryClause>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryExpression::get_child_slot_info(int index) const {
    int i = index;
    int n0 = clauses_.count();
    if (i >= 0 && i < n0) return clauses_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* QueryExpression::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::clause) return &clauses_;
    return nullptr;
}

void QueryExpression::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryExpression*>(copy_node);
    copy->clauses_.clone_from(copy, clauses_);
}

QueryContinuationClause::QueryContinuationClause(QueryExpression* preceding_query, const std::string& identifier) {
    set_preceding_query(preceding_query);
    set_identifier(identifier);
}

void QueryContinuationClause::set_preceding_query(QueryExpression* value) { set_child_node(preceding_query_, value, 0); }
void QueryContinuationClause::set_identifier_token(Identifier* value) { set_child_node(identifier_token_, value, 1); }
void QueryContinuationClause::set_identifier(const std::string& value) { set_identifier_token(Identifier::create(value)); }

bool QueryContinuationClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryContinuationClause>(other);
    return o != nullptr &&
           match_required(preceding_query_, o->preceding_query_, match) &&
           match_string(identifier(), o->identifier());
}

int QueryContinuationClause::get_child_count() const { return 2; }

AstNode* QueryContinuationClause::get_child_by_index(int index) const {
    if (index == 0) return preceding_query_;
    if (index == 1) return identifier_token_;
    throw std::out_of_range("index");
}

void QueryContinuationClause::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(preceding_query_, cast<QueryExpression>(value), index); return; }
    if (index == 1) { set_child_node(identifier_token_, cast<Identifier>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryContinuationClause::get_child_slot_info(int index) const {
    if (index == 0) return preceding_query_slot;
    if (index == 1) return identifier_token_slot;
    throw std::out_of_range("index");
}

void QueryContinuationClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryContinuationClause*>(copy_node);
    if (preceding_query_ != nullptr) copy->set_preceding_query(clone_as(preceding_query_.get()));
    if (identifier_token_ != nullptr) copy->set_identifier_token(clone_as(identifier_token_.get()));
}

QueryFromClause::QueryFromClause(AstType* type, const std::string& identifier, Expression* expression) {
    set_type(type);
    set_identifier(identifier);
    set_expression(expression);
}

void QueryFromClause::set_type(AstType* value) { set_child_node(type_, value, 0); }
void QueryFromClause::set_identifier_token(Identifier* value) { set_child_node(identifier_token_, value, 1); }
void QueryFromClause::set_identifier(const std::string& value) { set_identifier_token(Identifier::create(value)); }
void QueryFromClause::set_expression(Expression* value) { set_child_node(expression_, value, 2); }

bool QueryFromClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryFromClause>(other);
    return o != nullptr &&
           match_optional(type_, o->type_, match) &&
           match_string(identifier(), o->identifier()) &&
           match_required(expression_, o->expression_, match);
}

int QueryFromClause::get_child_count() const { return 3; }

AstNode* QueryFromClause::get_child_by_index(int index) const {
    if (index == 0) return type_;
    if (index == 1) return identifier_token_;
    if (index == 2) return expression_;
    throw std::out_of_range("index");
}

void QueryFromClause::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    if (index == 1) { set_child_node(identifier_token_, cast<Identifier>(value), index); return; }
    if (index == 2) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryFromClause::get_child_slot_info(int index) const {
    if (index == 0) return type_slot;
    if (index == 1) return identifier_token_slot;
    if (index == 2) return expression_slot;
    throw std::out_of_range("index");
}

void QueryFromClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryFromClause*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (identifier_token_ != nullptr) copy->set_identifier_token(clone_as(identifier_token_.get()));
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

QueryLetClause::QueryLetClause(const std::string& identifier, Expression* expression) {
    set_identifier(identifier);
    set_expression(expression);
}

void QueryLetClause::set_identifier_token(Identifier* value) { set_child_node(identifier_token_, value, 0); }
void QueryLetClause::set_identifier(const std::string& value) { set_identifier_token(Identifier::create(value)); }
void QueryLetClause::set_expression(Expression* value) { set_child_node(expression_, value, 1); }

bool QueryLetClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryLetClause>(other);
    return o != nullptr &&
           match_string(identifier(), o->identifier()) &&
           match_required(expression_, o->expression_, match);
}

int QueryLetClause::get_child_count() const { return 2; }

AstNode* QueryLetClause::get_child_by_index(int index) const {
    if (index == 0) return identifier_token_;
    if (index == 1) return expression_;
    throw std::out_of_range("index");
}

void QueryLetClause::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(identifier_token_, cast<Identifier>(value), index); return; }
    if (index == 1) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryLetClause::get_child_slot_info(int index) const {
    if (index == 0) return identifier_token_slot;
    if (index == 1) return expression_slot;
    throw std::out_of_range("index");
}

void QueryLetClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryLetClause*>(copy_node);
    if (identifier_token_ != nullptr) copy->set_identifier_token(clone_as(identifier_token_.get()));
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

QueryWhereClause::QueryWhereClause(Expression* condition) {
    set_condition(condition);
}

void QueryWhereClause::set_condition(Expression* value) { set_child_node(condition_, value, 0); }

bool QueryWhereClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryWhereClause>(other);
    return o != nullptr &&
           match_required(condition_, o->condition_, match);
}

int QueryWhereClause::get_child_count() const { return 1; }

AstNode* QueryWhereClause::get_child_by_index(int index) const {
    if (index == 0) return condition_;
    throw std::out_of_range("index");
}

void QueryWhereClause::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(condition_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryWhereClause::get_child_slot_info(int index) const {
    if (index == 0) return condition_slot;
    throw std::out_of_range("index");
}

void QueryWhereClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryWhereClause*>(copy_node);
    if (condition_ != nullptr) copy->set_condition(clone_as(condition_.get()));
}

QueryJoinClause::QueryJoinClause(AstType* type, const std::string& join_identifier, Expression* in_expression, Expression* on_expression, Expression* equals_expression, const std::string& into_identifier) {
    set_type(type);
    set_join_identifier(join_identifier);
    set_in_expression(in_expression);
    set_on_expression(on_expression);
    set_equals_expression(equals_expression);
    set_into_identifier(into_identifier);
}

void QueryJoinClause::set_type(AstType* value) { set_child_node(type_, value, 0); }
void QueryJoinClause::set_join_identifier_token(Identifier* value) { set_child_node(join_identifier_token_, value, 1); }
void QueryJoinClause::set_join_identifier(const std::string& value) { set_join_identifier_token(Identifier::create(value)); }
void QueryJoinClause::set_in_expression(Expression* value) { set_child_node(in_expression_, value, 2); }
void QueryJoinClause::set_on_expression(Expression* value) { set_child_node(on_expression_, value, 3); }
void QueryJoinClause::set_equals_expression(Expression* value) { set_child_node(equals_expression_, value, 4); }
void QueryJoinClause::set_into_identifier_token(Identifier* value) { set_child_node(into_identifier_token_, value, 5); }
void QueryJoinClause::set_into_identifier(const std::string& value) { set_into_identifier_token(Identifier::create_if_not_empty(value)); }

bool QueryJoinClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryJoinClause>(other);
    return o != nullptr &&
           match_optional(type_, o->type_, match) &&
           match_string(join_identifier(), o->join_identifier()) &&
           match_required(in_expression_, o->in_expression_, match) &&
           match_required(on_expression_, o->on_expression_, match) &&
           match_required(equals_expression_, o->equals_expression_, match) &&
           match_string(into_identifier(), o->into_identifier());
}

int QueryJoinClause::get_child_count() const { return 6; }

AstNode* QueryJoinClause::get_child_by_index(int index) const {
    if (index == 0) return type_;
    if (index == 1) return join_identifier_token_;
    if (index == 2) return in_expression_;
    if (index == 3) return on_expression_;
    if (index == 4) return equals_expression_;
    if (index == 5) return into_identifier_token_;
    throw std::out_of_range("index");
}

void QueryJoinClause::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(type_, cast<AstType>(value), index); return; }
    if (index == 1) { set_child_node(join_identifier_token_, cast<Identifier>(value), index); return; }
    if (index == 2) { set_child_node(in_expression_, cast<Expression>(value), index); return; }
    if (index == 3) { set_child_node(on_expression_, cast<Expression>(value), index); return; }
    if (index == 4) { set_child_node(equals_expression_, cast<Expression>(value), index); return; }
    if (index == 5) { set_child_node(into_identifier_token_, cast<Identifier>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryJoinClause::get_child_slot_info(int index) const {
    if (index == 0) return type_slot;
    if (index == 1) return join_identifier_token_slot;
    if (index == 2) return in_expression_slot;
    if (index == 3) return on_expression_slot;
    if (index == 4) return equals_expression_slot;
    if (index == 5) return into_identifier_token_slot;
    throw std::out_of_range("index");
}

void QueryJoinClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryJoinClause*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (join_identifier_token_ != nullptr) copy->set_join_identifier_token(clone_as(join_identifier_token_.get()));
    if (in_expression_ != nullptr) copy->set_in_expression(clone_as(in_expression_.get()));
    if (on_expression_ != nullptr) copy->set_on_expression(clone_as(on_expression_.get()));
    if (equals_expression_ != nullptr) copy->set_equals_expression(clone_as(equals_expression_.get()));
    if (into_identifier_token_ != nullptr) copy->set_into_identifier_token(clone_as(into_identifier_token_.get()));
}

QueryOrderClause::QueryOrderClause(const std::vector<QueryOrdering*>& orderings) {
    orderings_.add_range(orderings);
}

QueryOrderClause::QueryOrderClause(std::initializer_list<QueryOrdering*> orderings) : QueryOrderClause(std::vector<QueryOrdering*>(orderings)) {}


bool QueryOrderClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryOrderClause>(other);
    return o != nullptr &&
           orderings_.do_match(o->orderings_, match);
}

int QueryOrderClause::get_child_count() const { return orderings_.count(); }

AstNode* QueryOrderClause::get_child_by_index(int index) const {
    int i = index;
    int n0 = orderings_.count();
    if (i >= 0 && i < n0) return orderings_[static_cast<std::size_t>(i)];
    throw std::out_of_range("index");
}

void QueryOrderClause::set_child_by_index(int index, AstNode* value) {
    int i = index;
    int n0 = orderings_.count();
    if (i >= 0 && i < n0) { orderings_.set(static_cast<std::size_t>(i), cast<QueryOrdering>(value)); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryOrderClause::get_child_slot_info(int index) const {
    int i = index;
    int n0 = orderings_.count();
    if (i >= 0 && i < n0) return orderings_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* QueryOrderClause::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::ordering) return &orderings_;
    return nullptr;
}

void QueryOrderClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryOrderClause*>(copy_node);
    copy->orderings_.clone_from(copy, orderings_);
}

QueryOrdering::QueryOrdering(Expression* expression, QueryOrderingDirection direction) {
    set_expression(expression);
    set_direction(direction);
}

void QueryOrdering::set_expression(Expression* value) { set_child_node(expression_, value, 0); }

bool QueryOrdering::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryOrdering>(other);
    return o != nullptr &&
           match_required(expression_, o->expression_, match) &&
           direction_ == o->direction_;
}

int QueryOrdering::get_child_count() const { return 1; }

AstNode* QueryOrdering::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    throw std::out_of_range("index");
}

void QueryOrdering::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryOrdering::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    throw std::out_of_range("index");
}

void QueryOrdering::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryOrdering*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

QuerySelectClause::QuerySelectClause(Expression* expression) {
    set_expression(expression);
}

void QuerySelectClause::set_expression(Expression* value) { set_child_node(expression_, value, 0); }

bool QuerySelectClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QuerySelectClause>(other);
    return o != nullptr &&
           match_required(expression_, o->expression_, match);
}

int QuerySelectClause::get_child_count() const { return 1; }

AstNode* QuerySelectClause::get_child_by_index(int index) const {
    if (index == 0) return expression_;
    throw std::out_of_range("index");
}

void QuerySelectClause::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(expression_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QuerySelectClause::get_child_slot_info(int index) const {
    if (index == 0) return expression_slot;
    throw std::out_of_range("index");
}

void QuerySelectClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QuerySelectClause*>(copy_node);
    if (expression_ != nullptr) copy->set_expression(clone_as(expression_.get()));
}

QueryGroupClause::QueryGroupClause(Expression* projection, Expression* key) {
    set_projection(projection);
    set_key(key);
}

void QueryGroupClause::set_projection(Expression* value) { set_child_node(projection_, value, 0); }
void QueryGroupClause::set_key(Expression* value) { set_child_node(key_, value, 1); }

bool QueryGroupClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<QueryGroupClause>(other);
    return o != nullptr &&
           match_required(projection_, o->projection_, match) &&
           match_required(key_, o->key_, match);
}

int QueryGroupClause::get_child_count() const { return 2; }

AstNode* QueryGroupClause::get_child_by_index(int index) const {
    if (index == 0) return projection_;
    if (index == 1) return key_;
    throw std::out_of_range("index");
}

void QueryGroupClause::set_child_by_index(int index, AstNode* value) {
    if (index == 0) { set_child_node(projection_, cast<Expression>(value), index); return; }
    if (index == 1) { set_child_node(key_, cast<Expression>(value), index); return; }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& QueryGroupClause::get_child_slot_info(int index) const {
    if (index == 0) return projection_slot;
    if (index == 1) return key_slot;
    throw std::out_of_range("index");
}

void QueryGroupClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<QueryGroupClause*>(copy_node);
    if (projection_ != nullptr) copy->set_projection(clone_as(projection_.get()));
    if (key_ != nullptr) copy->set_key(clone_as(key_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
