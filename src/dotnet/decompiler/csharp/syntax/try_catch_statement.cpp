// dotnet/decompiler/csharp/syntax/try_catch_statement.cpp -- see try_catch_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/try_catch_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/ast_type.hpp"
#include "dotnet/decompiler/csharp/syntax/block_statement.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

TryCatchStatement::TryCatchStatement() {
}

TryCatchStatement::TryCatchStatement(BlockStatement* try_block_value) {
    set_try_block(try_block_value);
}

TryCatchStatement::TryCatchStatement(BlockStatement* try_block_value, const std::vector<CatchClause*>& catch_clauses_value) : TryCatchStatement(try_block_value) {
    catch_clauses().add_range(catch_clauses_value);
}

TryCatchStatement::TryCatchStatement(BlockStatement* try_block_value, const std::vector<CatchClause*>& catch_clauses_value, BlockStatement* finally_block_value) : TryCatchStatement(try_block_value, catch_clauses_value) {
    set_finally_block(finally_block_value);
}

void TryCatchStatement::set_try_block(BlockStatement* value) {
    set_child_node(try_block_, value, 0);
}

void TryCatchStatement::set_finally_block(BlockStatement* value) {
    set_child_node(finally_block_, value);
}

bool TryCatchStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<TryCatchStatement>(other);
    return o != nullptr
           && match_required(try_block(), o->try_block(), match)
           && catch_clauses().do_match(o->catch_clauses(), match)
           && match_optional(finally_block(), o->finally_block(), match);
}

int TryCatchStatement::get_child_count() const {
    return 2 + catch_clauses_.count();
}

AstNode* TryCatchStatement::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return try_block_;
    i--;
    {
        int n = catch_clauses_.count();
        if (i < n) return catch_clauses_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return finally_block_;
    throw std::out_of_range("index");
}

void TryCatchStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(try_block_, cast<BlockStatement>(value), index);
        return;
    }
    i--;
    {
        int n = catch_clauses_.count();
        if (i < n) {
            catch_clauses_.set(static_cast<std::size_t>(i), cast<CatchClause>(value));
            return;
        }
        i -= n;
    }
    if (i == 0) {
        set_child_node(finally_block_, cast<BlockStatement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& TryCatchStatement::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return try_block_slot;
    i--;
    {
        int n = catch_clauses_.count();
        if (i < n) return catch_clauses_slot;
        i -= n;
    }
    if (i == 0) return finally_block_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* TryCatchStatement::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::catch_clause) return &catch_clauses_;
    return nullptr;
}

void TryCatchStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<TryCatchStatement*>(copy_node);
    if (try_block_ != nullptr) copy->set_try_block(clone_as(try_block_.get()));
    copy->catch_clauses_.clone_from(copy, catch_clauses_);
    if (finally_block_ != nullptr) copy->set_finally_block(clone_as(finally_block_.get()));
}

CatchClause::CatchClause() {
}

CatchClause::CatchClause(AstType* type_value, const std::string& variable_name_value, Expression* condition_value, BlockStatement* body_value) {
    set_type(type_value);
    set_variable_name(variable_name_value);
    set_condition(condition_value);
    set_body(body_value);
}

void CatchClause::set_type(AstType* value) {
    set_child_node(type_, value, 0);
}

void CatchClause::set_variable_name_token(Identifier* value) {
    set_child_node(variable_name_token_, value, 1);
}

void CatchClause::set_condition(Expression* value) {
    set_child_node(condition_, value, 2);
}

void CatchClause::set_body(BlockStatement* value) {
    set_child_node(body_, value, 3);
}

CatchClause* CatchClause::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<CatchClause, AstNodeKind::CatchClausePlaceholder>>(pattern);
}

bool CatchClause::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<CatchClause>(other);
    return o != nullptr
           && match_optional(type(), o->type(), match)
           && match_string(variable_name(), o->variable_name())
           && match_optional(condition(), o->condition(), match)
           && match_required(body(), o->body(), match);
}

int CatchClause::get_child_count() const {
    return 4;
}

AstNode* CatchClause::get_child_by_index(int index) const {
    int i = index;
    if (i == 0) return type_;
    i--;
    if (i == 0) return variable_name_token_;
    i--;
    if (i == 0) return condition_;
    i--;
    if (i == 0) return body_;
    throw std::out_of_range("index");
}

void CatchClause::set_child_by_index(int index, AstNode* value) {
    int i = index;
    if (i == 0) {
        set_child_node(type_, cast<AstType>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(variable_name_token_, cast<Identifier>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(condition_, cast<Expression>(value), index);
        return;
    }
    i--;
    if (i == 0) {
        set_child_node(body_, cast<BlockStatement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& CatchClause::get_child_slot_info(int index) const {
    int i = index;
    if (i == 0) return type_slot;
    i--;
    if (i == 0) return variable_name_token_slot;
    i--;
    if (i == 0) return condition_slot;
    i--;
    if (i == 0) return body_slot;
    throw std::out_of_range("index");
}

void CatchClause::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<CatchClause*>(copy_node);
    if (type_ != nullptr) copy->set_type(clone_as(type_.get()));
    if (variable_name_token_ != nullptr) copy->set_variable_name_token(clone_as(variable_name_token_.get()));
    if (condition_ != nullptr) copy->set_condition(clone_as(condition_.get()));
    if (body_ != nullptr) copy->set_body(clone_as(body_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
