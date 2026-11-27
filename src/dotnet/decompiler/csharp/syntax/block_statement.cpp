// dotnet/decompiler/csharp/syntax/block_statement.cpp -- see block_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/block_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression_statement.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_placeholder.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

BlockStatement::BlockStatement() {
}

BlockStatement::BlockStatement(const std::vector<Statement*>& statements_value) {
    statements().add_range(statements_value);
}

BlockStatement* BlockStatement::from_pattern(pattern_matching::Pattern* pattern) {
    if (pattern == nullptr) return nullptr;
    return make<PatternPlaceholderNode<BlockStatement, AstNodeKind::BlockStatementPlaceholder>>(pattern);
}

bool BlockStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<BlockStatement>(other);
    return o != nullptr
           && statements().do_match(o->statements(), match);
}

int BlockStatement::get_child_count() const {
    return statements_.count();
}

AstNode* BlockStatement::get_child_by_index(int index) const {
    int i = index;
    {
        int n = statements_.count();
        if (i < n) return statements_[static_cast<std::size_t>(i)];
        i -= n;
    }
    throw std::out_of_range("index");
}

void BlockStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
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

const CSharpSlotInfo& BlockStatement::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = statements_.count();
        if (i < n) return statements_slot;
        i -= n;
    }
    throw std::out_of_range("index");
}

AstNodeCollectionBase* BlockStatement::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::statement) return &statements_;
    return nullptr;
}

void BlockStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<BlockStatement*>(copy_node);
    copy->statements_.clone_from(copy, statements_);
}

void BlockStatement::add(Statement* statement) {
    add_child(statement, Slots::statement);
}

void BlockStatement::add(Expression* expression) {
    add_child(make<ExpressionStatement>(expression), Slots::statement);
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
