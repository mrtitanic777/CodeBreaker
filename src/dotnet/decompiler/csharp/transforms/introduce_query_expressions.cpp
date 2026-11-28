// dotnet/decompiler/csharp/transforms/introduce_query_expressions.cpp -- see introduce_query_expressions.hpp.
#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "dotnet/decompiler/csharp/transforms/introduce_query_expressions.hpp"

#include <cassert>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using il::ILFunction;
using il::ILVariable;

void IntroduceQueryExpressions::run(AstNode* root_node, TransformContext& context) {
    if (!context.settings.query_expressions) return;
    context_ = &context;
    decompile_queries(root_node);
    // After all queries were decompiled, detect degenerate queries (queries not property terminated with 'select' or 'group') and fix them,
    // either by adding a degenerate select, or by combining them with another query.
    for (AstNode* node : root_node->descendants()) {
        auto* query = as<QueryExpression>(node);
        if (query == nullptr) continue;
        auto* from_clause = cast<QueryFromClause>(query->clauses()[0]);
        if (is_degenerate_query(query)) {
            // introduce select for degenerate query
            context_->step("Add degenerate query select clause", query);
            query->clauses().add(make<QuerySelectClause>(copy_annotations_from(make<IdentifierExpression>(from_clause->identifier()), from_clause)));
        }
        // See if the data source of this query is a degenerate query, and combine the queries if possible.
        QueryExpression* inner_query = as<QueryExpression>(from_clause->expression());
        while (inner_query != nullptr && is_degenerate_query(inner_query)) {
            auto* inner_from_clause = cast<QueryFromClause>(inner_query->clauses()[0]);
            auto* inner_resolve_result = inner_from_clause->annotation<ILVariableResolveResult>();
            auto* range_resolve_result = from_clause->annotation<ILVariableResolveResult>();
            ILVariable* inner_variable = inner_resolve_result != nullptr ? inner_resolve_result->variable : nullptr;
            ILVariable* range_variable = range_resolve_result != nullptr ? range_resolve_result->variable : nullptr;
            context_->step("Combine nested query clauses", from_clause);
            // Replace the fromClause with all clauses from the inner query
            from_clause->remove();
            QueryClause* insertion_pos = nullptr;
            for (QueryClause* clause : inner_query->clauses().to_vector()) {
                combine_range_variables(clause, inner_variable, range_variable);
                QueryClause* detached = detach(clause);
                query->clauses().insert_after(insertion_pos, detached);
                insertion_pos = detached;
            }
            context_->end_step(inner_from_clause);
            from_clause = inner_from_clause;
            inner_query = as<QueryExpression>(from_clause->expression());
        }
    }
}

void IntroduceQueryExpressions::combine_range_variables(QueryClause* clause, ILVariable* old_variable, ILVariable* new_variable) {
    if (old_variable == nullptr || new_variable == nullptr) return;
    for (AstNode* node : clause->descendant_nodes()) {
        auto* identifier = as<Identifier>(node);
        if (identifier == nullptr) continue;
        AstNode* parent = identifier->parent();
        if (parent == nullptr) continue;
        auto* resolve_result = parent->annotation<ILVariableResolveResult>();
        ILVariable* variable = resolve_result != nullptr ? resolve_result->variable : nullptr;
        if (variable == old_variable) {
            context_->step("Combine query range variables", identifier);
            parent->remove_annotations<ILVariableResolveResult>();
            parent->add_annotation(make<ILVariableResolveResult>(new_variable));
            Identifier* new_identifier = Identifier::create(*variable_of(context_->l2, *new_variable).name);
            identifier->replace_with(new_identifier);
            context_->end_step(new_identifier);
        }
    }
}

bool IntroduceQueryExpressions::is_degenerate_query(QueryExpression* query) {
    if (query == nullptr) return false;
    QueryClause* last_clause = query->clauses().last_or_null();
    return !(is<QuerySelectClause>(last_clause) || is<QueryGroupClause>(last_clause));
}

void IntroduceQueryExpressions::decompile_queries(AstNode* node) {
    Expression* query = decompile_query(as<InvocationExpression>(node));
    if (query != nullptr) {
        if (is<ExpressionStatement>(node->parent()) && can_use_discard_assignment()) query = make<AssignmentExpression>(make<IdentifierExpression>("_"), query);
        node->replace_with(query);
        context_->end_step(query);
    }
    AstNode* next = nullptr;
    for (AstNode* child = (query != nullptr ? static_cast<AstNode*>(query) : node)->first_child(); child != nullptr; child = next) {
        // store reference to next child before transformation
        next = child->next_sibling();
        decompile_queries(child);
    }
}

bool IntroduceQueryExpressions::can_use_discard_assignment() const {
    // TODO : check whether there exists a variable named '_' in scope.
    return context_->settings.discards;
}

QueryExpression* IntroduceQueryExpressions::decompile_query(InvocationExpression* invocation) {
    if (invocation == nullptr) return nullptr;
    auto* mre = as<MemberReferenceExpression>(invocation->target());
    if (mre == nullptr || is_null_conditional(mre->target())) return nullptr;
    const std::string member_name = mre->member_name();
    if (member_name == "Select") {
        if (invocation->arguments().size() != 1) return nullptr;
        if (!is_complex_query(mre)) return nullptr;
        Expression* expr = invocation->arguments()[0];
        ParameterDeclaration* parameter = nullptr;
        Expression* body = nullptr;
        if (match_simple_lambda(expr, parameter, body)) {
            context_->step("Build select query", invocation);
            QueryExpression* query = make<QueryExpression>();
            query->clauses().add(make_from_clause(parameter, detach(mre->target())));
            query->clauses().add(copy_annotations_from(make<QuerySelectClause>(wrap_expression_in_parentheses_if_necessary(detach(body), parameter->name())), expr));
            return query;
        }
        return nullptr;
    }
    if (member_name == "GroupBy") {
        if (invocation->arguments().size() == 2) {
            Expression* key_lambda = invocation->arguments()[0];
            Expression* projection_lambda = invocation->arguments()[1];
            ParameterDeclaration* parameter1 = nullptr;
            Expression* key_selector = nullptr;
            ParameterDeclaration* parameter2 = nullptr;
            Expression* element_selector = nullptr;
            if (match_simple_lambda(key_lambda, parameter1, key_selector) && match_simple_lambda(projection_lambda, parameter2, element_selector) &&
                parameter1->name() == parameter2->name()) {
                context_->step("Build group query", invocation);
                QueryExpression* query = make<QueryExpression>();
                query->clauses().add(make_from_clause(parameter1, detach(mre->target())));
                Expression* projection = detach(element_selector);
                Expression* key = detach(key_selector);
                auto* query_group_clause = make<QueryGroupClause>(projection, key);
                // The matched group-by lambdas always carry their ILFunction annotation.
                query_group_clause->add_annotation(
                    make<QueryGroupClauseAnnotation>(key_lambda->annotation<ILFunction>(), projection_lambda->annotation<ILFunction>()));
                query->clauses().add(query_group_clause);
                return query;
            }
        } else if (invocation->arguments().size() == 1) {
            Expression* lambda = invocation->arguments()[0];
            ParameterDeclaration* parameter = nullptr;
            Expression* key_selector = nullptr;
            if (match_simple_lambda(lambda, parameter, key_selector)) {
                context_->step("Build group query", invocation);
                QueryExpression* query = make<QueryExpression>();
                query->clauses().add(make_from_clause(parameter, detach(mre->target())));
                Expression* projection = copy_annotations_from(make<IdentifierExpression>(parameter->name()), parameter);
                query->clauses().add(make<QueryGroupClause>(projection, detach(key_selector)));
                return query;
            }
        }
        return nullptr;
    }
    if (member_name == "SelectMany") {
        if (invocation->arguments().size() != 2) return nullptr;
        Expression* from_expression_lambda = invocation->arguments()[0];
        ParameterDeclaration* parameter = nullptr;
        Expression* collection_selector = nullptr;
        if (!match_simple_lambda(from_expression_lambda, parameter, collection_selector)) return nullptr;
        if (is_null_conditional(collection_selector)) return nullptr;
        auto* lambda = as<LambdaExpression>(invocation->arguments()[1]);
        if (lambda != nullptr && lambda->parameters().size() == 2 && is<Expression>(lambda->body())) {
            ParameterDeclaration* p1 = lambda->parameters()[0];
            ParameterDeclaration* p2 = lambda->parameters()[1];
            if (p1->name() == parameter->name()) {
                context_->step("Build select-many query", invocation);
                QueryExpression* query = make<QueryExpression>();
                query->clauses().add(make_from_clause(p1, detach(mre->target())));
                query->clauses().add(copy_annotations_from(make_from_clause(p2, detach(collection_selector)), from_expression_lambda));
                query->clauses().add(
                    make<QuerySelectClause>(wrap_expression_in_parentheses_if_necessary(detach(static_cast<Expression*>(lambda->body())), parameter->name())));
                return query;
            }
        }
        return nullptr;
    }
    if (member_name == "Where") {
        if (invocation->arguments().size() != 1) return nullptr;
        if (!is_complex_query(mre)) return nullptr;
        Expression* expr = invocation->arguments()[0];
        ParameterDeclaration* parameter = nullptr;
        Expression* body = nullptr;
        if (match_simple_lambda(expr, parameter, body)) {
            context_->step("Build where query", invocation);
            QueryExpression* query = make<QueryExpression>();
            query->clauses().add(make_from_clause(parameter, detach(mre->target())));
            query->clauses().add(copy_annotations_from(make<QueryWhereClause>(detach(body)), expr));
            return query;
        }
        return nullptr;
    }
    if (member_name == "OrderBy" || member_name == "OrderByDescending" || member_name == "ThenBy" || member_name == "ThenByDescending") {
        if (invocation->arguments().size() != 1) return nullptr;
        if (!is_complex_query(mre)) return nullptr;
        Expression* lambda = invocation->arguments()[0];
        ParameterDeclaration* parameter = nullptr;
        Expression* order_expression = nullptr;
        if (match_simple_lambda(lambda, parameter, order_expression)) {
            if (validate_then_by_chain(invocation, parameter->name())) {
                context_->step("Build order query", invocation);
                QueryOrderClause* order_clause = make<QueryOrderClause>();
                while (mre->member_name() == "ThenBy" || mre->member_name() == "ThenByDescending") {
                    // insert new ordering at beginning
                    Expression* ordering_expression = detach(order_expression);
                    QueryOrderingDirection direction = (mre->member_name() == "ThenBy" ? QueryOrderingDirection::None : QueryOrderingDirection::Descending);
                    order_clause->orderings().insert_after(nullptr, copy_annotations_from(make<QueryOrdering>(ordering_expression, direction), lambda));
                    auto* tmp = cast<InvocationExpression>(mre->target());
                    mre = cast<MemberReferenceExpression>(tmp->target());
                    lambda = tmp->arguments()[0];
                    // validate_then_by_chain already confirmed every clause in the chain is a simple lambda, so this match always succeeds.
                    bool matched = match_simple_lambda(lambda, parameter, order_expression);
                    assert(matched);
                    (void)matched;
                    assert(order_expression != nullptr);
                }
                // The chain was validated and every iteration re-matched a simple lambda, so the final range-variable parameter is set.
                assert(parameter != nullptr);
                // insert new ordering at beginning
                Expression* ordering_expression = detach(order_expression);
                QueryOrderingDirection direction = (mre->member_name() == "OrderBy" ? QueryOrderingDirection::None : QueryOrderingDirection::Descending);
                order_clause->orderings().insert_after(nullptr, copy_annotations_from(make<QueryOrdering>(ordering_expression, direction), lambda));
                QueryExpression* query = make<QueryExpression>();
                query->clauses().add(make_from_clause(parameter, detach(mre->target())));
                query->clauses().add(order_clause);
                return query;
            }
        }
        return nullptr;
    }
    if (member_name == "Join" || member_name == "GroupJoin") {
        if (invocation->arguments().size() != 4) return nullptr;
        Expression* source1 = mre->target();
        Expression* source2 = invocation->arguments()[0];
        if (is_null_conditional(source2)) return nullptr;
        Expression* outer_lambda = invocation->arguments()[1];
        ParameterDeclaration* element1 = nullptr;
        Expression* key1 = nullptr;
        if (!match_simple_lambda(outer_lambda, element1, key1)) return nullptr;
        Expression* inner_lambda = invocation->arguments()[2];
        ParameterDeclaration* element2 = nullptr;
        Expression* key2 = nullptr;
        if (!match_simple_lambda(inner_lambda, element2, key2)) return nullptr;
        auto* lambda = as<LambdaExpression>(invocation->arguments()[3]);
        if (lambda != nullptr && lambda->parameters().size() == 2 && is<Expression>(lambda->body())) {
            ParameterDeclaration* p1 = lambda->parameters()[0];
            ParameterDeclaration* p2 = lambda->parameters()[1];
            if (validate_parameter(p1) && validate_parameter(p2) && p1->name() == element1->name() &&
                (p2->name() == element2->name() || mre->member_name() == "GroupJoin")) {
                context_->step(mre->member_name() == "GroupJoin" ? "Build group join query" : "Build join query", invocation);
                QueryExpression* query = make<QueryExpression>();
                query->clauses().add(make_from_clause(element1, detach(source1)));
                QueryJoinClause* join_clause = make<QueryJoinClause>();
                join_clause->set_join_identifier(element2->name());  // join elementName2
                copy_annotations_from(join_clause->join_identifier_token(), element2);
                join_clause->set_in_expression(detach(source2));   // in source2
                join_clause->set_on_expression(detach(key1));      // on key1
                join_clause->set_equals_expression(detach(key2));  // equals key2
                if (mre->member_name() == "GroupJoin") {
                    join_clause->set_into_identifier(p2->name());  // into p2.Name
                    copy_annotations_from(join_clause->into_identifier_token(), p2);
                }
                // The matched join key-selector lambdas always carry their ILFunction annotation.
                join_clause->add_annotation(make<QueryJoinClauseAnnotation>(outer_lambda->annotation<ILFunction>(), inner_lambda->annotation<ILFunction>()));
                query->clauses().add(join_clause);
                query->clauses().add(copy_annotations_from(make<QuerySelectClause>(detach(static_cast<Expression*>(lambda->body()))), lambda));
                return query;
            }
        }
        return nullptr;
    }
    return nullptr;
}

bool IntroduceQueryExpressions::is_complex_query(MemberReferenceExpression* mre) {
    return (is<InvocationExpression>(mre->target()) && is<InvocationExpression>(mre->parent())) ||
           (mre->parent() != nullptr && is<QueryClause>(mre->parent()->parent()));
}

QueryFromClause* IntroduceQueryExpressions::make_from_clause(ParameterDeclaration* parameter, Expression* body) {
    QueryFromClause* from_clause = make<QueryFromClause>();
    from_clause->set_identifier(parameter->name());
    from_clause->set_expression(body);
    copy_annotations_from(from_clause, parameter);
    return from_clause;
}

AstNode* IntroduceQueryExpressions::ApplyAnnotationVisitor::visit_identifier(Identifier& identifier) {
    if (identifier.name() == identifier_) identifier.add_annotation(annotation_);
    return &identifier;
}

bool IntroduceQueryExpressions::is_null_conditional(Expression* target) {
    if (auto* unary = as<UnaryOperatorExpression>(target)) return unary->operator_() == UnaryOperatorType::NullConditional;
    if (auto* member = as<MemberReferenceExpression>(target)) return is_null_conditional(member->target());
    if (auto* invocation = as<InvocationExpression>(target)) return is_null_conditional(invocation->target());
    if (auto* indexer = as<IndexerExpression>(target); indexer != nullptr && indexer->target() != nullptr) return is_null_conditional(indexer->target());
    return false;
}

Expression* IntroduceQueryExpressions::wrap_expression_in_parentheses_if_necessary(Expression* expression, const std::string& parameter_name) {
    auto* ident = as<IdentifierExpression>(expression);
    if (ident != nullptr && parameter_name == ident->identifier()) return make<ParenthesizedExpression>(expression);
    return expression;
}

bool IntroduceQueryExpressions::validate_then_by_chain(InvocationExpression* invocation, const std::string& expected_parameter_name) {
    if (invocation == nullptr || invocation->arguments().size() != 1) return false;
    auto* mre = as<MemberReferenceExpression>(invocation->target());
    if (mre == nullptr) return false;
    ParameterDeclaration* parameter = nullptr;
    Expression* body = nullptr;
    if (!match_simple_lambda(invocation->arguments()[0], parameter, body)) return false;
    if (parameter->name() != expected_parameter_name) return false;
    if (mre->member_name() == "OrderBy" || mre->member_name() == "OrderByDescending") return !is_null_conditional(mre->target());
    if (mre->member_name() == "ThenBy" || mre->member_name() == "ThenByDescending")
        return validate_then_by_chain(as<InvocationExpression>(mre->target()), expected_parameter_name);
    return false;
}

bool IntroduceQueryExpressions::match_simple_lambda(Expression* expr, ParameterDeclaration*& parameter, Expression*& body) {
    auto* lambda = as<LambdaExpression>(expr);
    if (lambda != nullptr && lambda->parameters().size() == 1 && is<Expression>(lambda->body())) {
        ParameterDeclaration* p = lambda->parameters()[0];
        if (validate_parameter(p)) {
            parameter = p;
            body = static_cast<Expression*>(lambda->body());
            return true;
        }
    }
    parameter = nullptr;
    body = nullptr;
    return false;
}

bool IntroduceQueryExpressions::validate_parameter(ParameterDeclaration* p) {
    return p->parameter_modifier() == typesystem::ReferenceKind::None && p->attributes().size() == 0;
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
