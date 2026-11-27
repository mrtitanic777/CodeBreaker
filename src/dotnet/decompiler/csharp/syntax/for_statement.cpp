// dotnet/decompiler/csharp/syntax/for_statement.cpp -- see for_statement.hpp.
#include "dotnet/decompiler/csharp/syntax/for_statement.hpp"

#include <stdexcept>
#include <string>

#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

ForStatement::ForStatement() {
}

ForStatement::ForStatement(const std::vector<Statement*>& initializers_value, Expression* condition_value, const std::vector<Statement*>& iterators_value, Statement* embedded_statement_value) {
    initializers().add_range(initializers_value);
    set_condition(condition_value);
    iterators().add_range(iterators_value);
    set_embedded_statement(embedded_statement_value);
}

void ForStatement::set_condition(Expression* value) {
    set_child_node(condition_, value);
}

void ForStatement::set_embedded_statement(Statement* value) {
    set_child_node(embedded_statement_, value);
}

bool ForStatement::do_match(AstNode* other, const pattern_matching::Match& match) const {
    auto* o = as<ForStatement>(other);
    return o != nullptr
           && initializers().do_match(o->initializers(), match)
           && match_optional(condition(), o->condition(), match)
           && iterators().do_match(o->iterators(), match)
           && match_required(embedded_statement(), o->embedded_statement(), match);
}

int ForStatement::get_child_count() const {
    return 2 + initializers_.count() + iterators_.count();
}

AstNode* ForStatement::get_child_by_index(int index) const {
    int i = index;
    {
        int n = initializers_.count();
        if (i < n) return initializers_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return condition_;
    i--;
    {
        int n = iterators_.count();
        if (i < n) return iterators_[static_cast<std::size_t>(i)];
        i -= n;
    }
    if (i == 0) return embedded_statement_;
    throw std::out_of_range("index");
}

void ForStatement::set_child_by_index(int index, AstNode* value) {
    int i = index;
    {
        int n = initializers_.count();
        if (i < n) {
            initializers_.set(static_cast<std::size_t>(i), cast<Statement>(value));
            return;
        }
        i -= n;
    }
    if (i == 0) {
        set_child_node(condition_, cast<Expression>(value), index);
        return;
    }
    i--;
    {
        int n = iterators_.count();
        if (i < n) {
            iterators_.set(static_cast<std::size_t>(i), cast<Statement>(value));
            return;
        }
        i -= n;
    }
    if (i == 0) {
        set_child_node(embedded_statement_, cast<Statement>(value), index);
        return;
    }
    throw std::out_of_range("index");
}

const CSharpSlotInfo& ForStatement::get_child_slot_info(int index) const {
    int i = index;
    {
        int n = initializers_.count();
        if (i < n) return initializers_slot;
        i -= n;
    }
    if (i == 0) return condition_slot;
    i--;
    {
        int n = iterators_.count();
        if (i < n) return iterators_slot;
        i -= n;
    }
    if (i == 0) return embedded_statement_slot;
    throw std::out_of_range("index");
}

AstNodeCollectionBase* ForStatement::get_collection_by_kind(const CSharpSlotInfo& slot_kind) const {
    if (&slot_kind == &Slots::for_initializer) return &initializers_;
    if (&slot_kind == &Slots::iterator) return &iterators_;
    return nullptr;
}

void ForStatement::clone_children_into(AstNode* copy_node) const {
    auto* copy = static_cast<ForStatement*>(copy_node);
    copy->initializers_.clone_from(copy, initializers_);
    if (condition_ != nullptr) copy->set_condition(clone_as(condition_.get()));
    copy->iterators_.clone_from(copy, iterators_);
    if (embedded_statement_ != nullptr) copy->set_embedded_statement(clone_as(embedded_statement_.get()));
}

}  // namespace cb::dotnet::decompiler::csharp::syntax
