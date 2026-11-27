// dotnet/decompiler/csharp/transforms/add_checked_blocks.cpp -- see add_checked_blocks.hpp.
#include "dotnet/decompiler/csharp/transforms/add_checked_blocks.hpp"

#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/csharp/transforms/pattern_arena.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;

// ---- the annotations --------------------------------------------------------------------------------------------------------------

il::ArenaObject* AddCheckedBlocks::checked_annotation() {
    static il::ArenaObject* const annotation = build_pattern([] { return static_cast<il::ArenaObject*>(make<CheckedUncheckedAnnotation>(true, false)); });
    return annotation;
}

il::ArenaObject* AddCheckedBlocks::unchecked_annotation() {
    static il::ArenaObject* const annotation = build_pattern([] { return static_cast<il::ArenaObject*>(make<CheckedUncheckedAnnotation>(false, false)); });
    return annotation;
}

il::ArenaObject* AddCheckedBlocks::explicit_unchecked_annotation() {
    static il::ArenaObject* const annotation = build_pattern([] { return static_cast<il::ArenaObject*>(make<CheckedUncheckedAnnotation>(false, true)); });
    return annotation;
}

// ---- Cost -------------------------------------------------------------------------------------------------------------------------

AddCheckedBlocks::Cost AddCheckedBlocks::Cost::wrap_in_checked_expr() const {
    if (expressions == 0) {
        return Cost(blocks, 1);
    }
    // hack: penalize multiple layers of nested expressions
    // This doesn't really fit into the original idea of minimizing the number of block+expressions;
    // but tends to produce better-looking results due to less nesting.
    return Cost(blocks, expressions + 2);
}

// ---- the nodes to insert ----------------------------------------------------------------------------------------------------------

class AddCheckedBlocks::InsertedNodeList final : public InsertedNode {
public:
    InsertedNodeList(InsertedNodePtr child1_, InsertedNodePtr child2_) : child1(std::move(child1_)), child2(std::move(child2_)) {}

    void insert(TransformContext& context) override {
        child1->insert(context);
        child2->insert(context);
    }

private:
    InsertedNodePtr child1, child2;
};

class AddCheckedBlocks::InsertedExpression final : public InsertedNode {
public:
    InsertedExpression(Expression* expression_, bool is_checked) : expression(expression_), is_checked_(is_checked) {}

    void insert(TransformContext& context) override {
        context.step(is_checked_ ? "Add checked expression" : "Add unchecked expression", expression);
        AstNode* replacement;
        if (is_checked_) {
            replacement = expression->replace_with([](AstNode* e) -> AstNode* { return make<CheckedExpression>(cast<Expression>(e)); });
        } else {
            replacement = expression->replace_with([](AstNode* e) -> AstNode* { return make<UncheckedExpression>(cast<Expression>(e)); });
        }
        context.end_step(replacement);
    }

private:
    Expression* expression;
    bool is_checked_;
};

class AddCheckedBlocks::InsertedBlock final : public InsertedNode {
public:
    // first_statement_ is inclusive, last_statement_ exclusive.
    InsertedBlock(Statement* first_statement_, Statement* last_statement_, bool is_checked)
        : first_statement(first_statement_), last_statement(last_statement_), is_checked_(is_checked) {}

    void insert(TransformContext& context) override {
        // An InsertedBlock with a null start has infinite cost in the search and is never
        // selected for insertion, so by the time insert runs first_statement is non-null.
        assert(first_statement != nullptr);
        context.step(is_checked_ ? "Add checked block" : "Add unchecked block", first_statement);
        auto* new_block = make<BlockStatement>();
        // Move all statements except for the first
        Statement* next;
        for (Statement* stmt = get_next_statement(first_statement); stmt != nullptr && stmt != last_statement; stmt = next) {
            next = get_next_statement(stmt);
            new_block->add(detach(stmt));
        }
        // Replace the first statement with the new (un)checked block
        Statement* checked_block;
        if (is_checked_)
            checked_block = make<CheckedStatement>(new_block);
        else
            checked_block = make<UncheckedStatement>(new_block);
        first_statement->replace_with(checked_block);
        // now also move the first node into the new block
        new_block->statements().insert_after(nullptr, first_statement);
        context.end_step(checked_block);
    }

private:
    Statement* first_statement;
    Statement* last_statement;
    bool is_checked_;
};

AddCheckedBlocks::InsertedNodePtr AddCheckedBlocks::combine(const InsertedNodePtr& a, const InsertedNodePtr& b) {
    if (a == nullptr) return b;
    if (b == nullptr) return a;
    return std::make_shared<InsertedNodeList>(a, b);
}

// ---- the search -------------------------------------------------------------------------------------------------------------------

void AddCheckedBlocks::run(AstNode* node, TransformContext& context) {
    auto* block = as<BlockStatement>(node);
    if (block == nullptr) {
        for (AstNode* child = node->first_child(); child != nullptr; child = child->next_sibling()) {
            run(child, context);
        }
    } else {
        Result r = get_result_from_block(block);
        if (context.settings.check_for_overflow_underflow) {
            if (r.nodes_to_insert_in_checked_context != nullptr) r.nodes_to_insert_in_checked_context->insert(context);
        } else {
            if (r.nodes_to_insert_in_unchecked_context != nullptr) r.nodes_to_insert_in_unchecked_context->insert(context);
        }
    }
}

AddCheckedBlocks::Result AddCheckedBlocks::get_result_from_block(BlockStatement* block) {
    // For a block, we are tracking 4 possibilities:
    // a) context is checked, no unchecked block open
    Cost cost_checked_context(0, 0);
    InsertedNodePtr nodes_checked_context;
    // b) context is checked, an unchecked block is open
    Cost cost_checked_context_unchecked_block_open = Cost::infinite();
    InsertedNodePtr nodes_checked_context_unchecked_block_open;
    Statement* unchecked_block_start = nullptr;
    // c) context is unchecked, no checked block open
    Cost cost_unchecked_context(0, 0);
    InsertedNodePtr nodes_unchecked_context;
    // d) context is unchecked, a checked block is open
    Cost cost_unchecked_context_checked_block_open = Cost::infinite();
    InsertedNodePtr nodes_unchecked_context_checked_block_open;
    Statement* checked_block_start = nullptr;

    Statement* statement = block->statements().first_or_null();
    while (true) {
        // Blocks can be closed 'for free'. We use '<=' so that blocks are closed as late as possible (goal 4b)
        if (cost_checked_context_unchecked_block_open <= cost_checked_context) {
            cost_checked_context = cost_checked_context_unchecked_block_open;
            nodes_checked_context =
                combine(nodes_checked_context_unchecked_block_open, std::make_shared<InsertedBlock>(unchecked_block_start, statement, false));
        }
        if (cost_unchecked_context_checked_block_open <= cost_unchecked_context) {
            cost_unchecked_context = cost_unchecked_context_checked_block_open;
            nodes_unchecked_context =
                combine(nodes_unchecked_context_checked_block_open, std::make_shared<InsertedBlock>(checked_block_start, statement, true));
        }
        if (statement == nullptr) break;
        // Now try opening blocks. We use '<=' so that blocks are opened as late as possible. (goal 4a)
        if (cost_checked_context + Cost(1, 0) <= cost_checked_context_unchecked_block_open) {
            cost_checked_context_unchecked_block_open = cost_checked_context + Cost(1, 0);
            nodes_checked_context_unchecked_block_open = nodes_checked_context;
            unchecked_block_start = statement;
        }
        if (cost_unchecked_context + Cost(1, 0) <= cost_unchecked_context_checked_block_open) {
            cost_unchecked_context_checked_block_open = cost_unchecked_context + Cost(1, 0);
            nodes_unchecked_context_checked_block_open = nodes_unchecked_context;
            checked_block_start = statement;
        }
        // Now handle the statement
        Result stmt_result = get_result(statement);

        cost_checked_context += stmt_result.cost_in_checked_context;
        nodes_checked_context = combine(nodes_checked_context, stmt_result.nodes_to_insert_in_checked_context);
        cost_checked_context_unchecked_block_open += stmt_result.cost_in_unchecked_context;
        nodes_checked_context_unchecked_block_open =
            combine(nodes_checked_context_unchecked_block_open, stmt_result.nodes_to_insert_in_unchecked_context);
        cost_unchecked_context += stmt_result.cost_in_unchecked_context;
        nodes_unchecked_context = combine(nodes_unchecked_context, stmt_result.nodes_to_insert_in_unchecked_context);
        cost_unchecked_context_checked_block_open += stmt_result.cost_in_checked_context;
        nodes_unchecked_context_checked_block_open =
            combine(nodes_unchecked_context_checked_block_open, stmt_result.nodes_to_insert_in_checked_context);

        if (is<LabelStatement>(statement) || is<LocalFunctionDeclarationStatement>(statement)) {
            // We can't move labels into blocks because that might cause goto-statements
            // to be unable to jump to the labels.
            // Also, we can't move local functions into blocks, because that might cause
            // them to become out of scope from the call-sites.
            cost_checked_context_unchecked_block_open = Cost::infinite();
            cost_unchecked_context_checked_block_open = Cost::infinite();
        }

        statement = get_next_statement(statement);
    }

    Result result;
    result.cost_in_checked_context = cost_checked_context;
    result.nodes_to_insert_in_checked_context = nodes_checked_context;
    result.cost_in_unchecked_context = cost_unchecked_context;
    result.nodes_to_insert_in_unchecked_context = nodes_unchecked_context;
    return result;
}

AddCheckedBlocks::Result AddCheckedBlocks::get_result(AstNode* node) {
    if (auto* block = as<BlockStatement>(node)) return get_result_from_block(block);
    Result result;
    for (AstNode* child = node->first_child(); child != nullptr; child = child->next_sibling()) {
        Result child_result = get_result(child);
        result.cost_in_checked_context += child_result.cost_in_checked_context;
        result.nodes_to_insert_in_checked_context = combine(result.nodes_to_insert_in_checked_context, child_result.nodes_to_insert_in_checked_context);
        result.cost_in_unchecked_context += child_result.cost_in_unchecked_context;
        result.nodes_to_insert_in_unchecked_context =
            combine(result.nodes_to_insert_in_unchecked_context, child_result.nodes_to_insert_in_unchecked_context);
    }
    auto* expr = as<Expression>(node);
    if (expr != nullptr) {
        auto* annotation = expr->annotation<CheckedUncheckedAnnotation>();
        if (annotation != nullptr) {
            if (annotation->is_explicit) {
                // We don't yet support distinguishing CostInUncheckedContext vs. CostInExplicitUncheckedContext,
                // so we always force an unchecked() expression here.
                if (annotation->is_checked) throw std::logic_error("explicit checked");  // should not be needed
                result.cost_in_checked_context = result.cost_in_unchecked_context.wrap_in_checked_expr();
                result.cost_in_unchecked_context = result.cost_in_unchecked_context.wrap_in_checked_expr();
                result.nodes_to_insert_in_unchecked_context =
                    combine(result.nodes_to_insert_in_unchecked_context, std::make_shared<InsertedExpression>(expr, annotation->is_checked));
                result.nodes_to_insert_in_checked_context = result.nodes_to_insert_in_unchecked_context;
            } else {
                // If the annotation requires this node to be in a specific context, add a huge cost to the other context
                // That huge cost gives us the option to ignore a required checked/unchecked expression when there wouldn't be any
                // solution otherwise. (e.g. "for (checked(M().x += 1); true; unchecked(M().x += 2)) {}")
                if (annotation->is_checked)
                    result.cost_in_unchecked_context += Cost(10000, 0);
                else
                    result.cost_in_checked_context += Cost(10000, 0);
            }
        }
        // Embed this node in an checked/unchecked expression:
        const CSharpSlotInfo* slot = expr->slot();
        if (is<ExpressionStatement>(expr->parent())) {
            // We cannot use checked/unchecked for top-level-expressions.
        } else if (slot != nullptr && slot->child_type().first <= kind_of<Expression>::range.first &&
                   slot->child_type().last >= kind_of<Expression>::range.last) {
            // (the slot accepts any Expression)
            // We use '<' so that expressions are introduced on the deepest level possible (goal 3)
            Cost cost_if_wrap_with_checked = result.cost_in_checked_context.wrap_in_checked_expr();
            Cost cost_if_wrap_with_unchecked = result.cost_in_unchecked_context.wrap_in_checked_expr();
            if (cost_if_wrap_with_checked < result.cost_in_unchecked_context) {
                result.cost_in_unchecked_context = cost_if_wrap_with_checked;
                result.nodes_to_insert_in_unchecked_context =
                    combine(result.nodes_to_insert_in_checked_context, std::make_shared<InsertedExpression>(expr, true));
            } else if (cost_if_wrap_with_unchecked < result.cost_in_checked_context) {
                result.cost_in_checked_context = cost_if_wrap_with_unchecked;
                result.nodes_to_insert_in_checked_context =
                    combine(result.nodes_to_insert_in_unchecked_context, std::make_shared<InsertedExpression>(expr, false));
            }
        }
    }
    return result;
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
