// dotnet/decompiler/csharp/transforms/combine_query_expressions.cpp -- see combine_query_expressions.hpp.
#include "dotnet/decompiler/csharp/transforms/combine_query_expressions.hpp"

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_arena.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using namespace syntax::pattern_matching;

namespace {

// a.Cast<T>()
Expression* cast_pattern() {
    static Expression* const pattern = build_pattern([] {
        return static_cast<Expression*>(make<InvocationExpression>(make<MemberReferenceExpression>(
            Expression::from_pattern(make<AnyNode>("inExpr")), "Cast", std::vector<AstType*>{AstType::from_pattern(make<AnyNode>("targetType"))})));
    });
    return pattern;
}

// select new { x, a.y, z = f(x) }
QuerySelectClause* select_transparent_identifier_pattern() {
    static QuerySelectClause* const pattern = build_pattern([] {
        Choice* choice = make<Choice>();
        // name is equivalent to name = name
        choice->add(with_name(make<IdentifierExpression>(std::string(Pattern::any_string)), "expr"));
        // expr.name is equivalent to name = expr.name
        choice->add(with_name(make<MemberReferenceExpression>(Expression::from_pattern(make<AnyNode>()), std::string(Pattern::any_string)), "expr"));
        choice->add(with_name(make<NamedExpression>(std::string(Pattern::any_string), Expression::from_pattern(make<AnyNode>())), "expr"));
        Repeat* repeat = make<Repeat>(choice);
        repeat->set_min_count(1);
        return make<QuerySelectClause>(make<AnonymousTypeCreateExpression>(std::vector<Expression*>{Expression::from_pattern(repeat)}));
    });
    return pattern;
}

}  // namespace

void CombineQueryExpressions::run(AstNode* root_node, TransformContext& context) {
    if (!context.settings.query_expressions) return;
    context_ = &context;
    IdentifierAnnotations from_or_let_identifiers;
    try {
        combine_queries(root_node, from_or_let_identifiers);
    } catch (...) {
        context_ = nullptr;
        throw;
    }
    context_ = nullptr;
}

void CombineQueryExpressions::combine_queries(AstNode* node, IdentifierAnnotations& from_or_let_identifiers) {
    AstNode* next = nullptr;
    for (AstNode* child = node->first_child(); child != nullptr; child = next) {
        // store reference to next child before transformation
        next = child->next_sibling();
        combine_queries(child, from_or_let_identifiers);
    }
    if (auto* query = as<QueryExpression>(node)) {
        auto* from_clause = cast<QueryFromClause>(query->clauses()[0]);
        if (auto* inner_query = as<QueryExpression>(from_clause->expression())) {
            if (try_remove_transparent_identifier(query, from_clause, inner_query, from_or_let_identifiers)) {
                remove_transparent_identifier_references(query, from_or_let_identifiers);
            } else {
                QueryContinuationClause* continuation = make<QueryContinuationClause>();
                context_->step("Introduce query continuation", from_clause);
                continuation->set_preceding_query(detach(inner_query));
                continuation->set_identifier(from_clause->identifier());
                copy_annotations_from(continuation, from_clause);
                from_clause->replace_with(continuation);
                context_->end_step(continuation);
            }
        } else {
            Match m = match(cast_pattern(), from_clause->expression());
            if (m.success()) {
                context_->step("Move Cast type into from clause", from_clause);
                from_clause->set_type(detach(single<AstType>(m, "targetType")));
                from_clause->set_expression(detach(single<Expression>(m, "inExpr")));
            }
        }
    }
}

bool CombineQueryExpressions::try_remove_transparent_identifier(QueryExpression* query, QueryFromClause* from_clause, QueryExpression* inner_query,
                                                                IdentifierAnnotations& let_clauses) {
    if (!CSharpDecompiler::is_transparent_identifier(from_clause->identifier())) return false;
    auto* select_clause = as<QuerySelectClause>(inner_query->clauses().last_or_null());
    if (select_clause == nullptr) return false;
    Match m = match(select_transparent_identifier_pattern(), select_clause);
    if (!m.success()) return false;
    // from * in (from x in ... select new { members of anonymous type }) ...
    // =>
    // from x in ... { let x = ... } ...
    context_->step("Remove transparent query identifier", from_clause);
    from_clause->remove();
    select_clause->remove();
    // Move clauses from innerQuery to query
    QueryClause* insertion_pos = nullptr;
    for (QueryClause* clause : inner_query->clauses().to_vector()) {
        QueryClause* detached = detach(clause);
        query->clauses().insert_after(insertion_pos, detached);
        insertion_pos = detached;
    }
    context_->end_step(query->clauses()[0]);

    auto add_query_let_clause = [&](const std::string& name, Expression* expression) {
        QueryLetClause* let_clause = make<QueryLetClause>(name, detach(expression));
        auto* annotation = make<LetIdentifierAnnotation>();
        let_clause->add_annotation(annotation);
        let_clauses[name] = annotation;
        query->clauses().insert_after(insertion_pos, let_clause);
    };

    for (Expression* expr : m.get<Expression>("expr")) {
        if (auto* identifier = as<IdentifierExpression>(expr)) {
            let_clauses[identifier->identifier()] = identifier->annotation<ILVariableResolveResult>();
        } else if (auto* member = as<MemberReferenceExpression>(expr)) {
            add_query_let_clause(member->member_name(), member);
        } else if (auto* named_expression = as<NamedExpression>(expr)) {
            auto* identifier_expression = as<IdentifierExpression>(named_expression->expression());
            if (identifier_expression != nullptr && named_expression->name() == identifier_expression->identifier()) {
                let_clauses[named_expression->name()] = identifier_expression->annotation<ILVariableResolveResult>();
                continue;
            }
            add_query_let_clause(named_expression->name(), named_expression->expression());
        }
    }
    return true;
}

void CombineQueryExpressions::remove_transparent_identifier_references(AstNode* node, IdentifierAnnotations& from_or_let_identifiers) {
    for (AstNode* child : node->children()) remove_transparent_identifier_references(child, from_or_let_identifiers);
    auto* mre = as<MemberReferenceExpression>(node);
    if (mre == nullptr) return;
    auto* ident = as<IdentifierExpression>(mre->target());
    if (ident == nullptr || !CSharpDecompiler::is_transparent_identifier(ident->identifier())) return;
    IdentifierExpression* new_ident = make<IdentifierExpression>(mre->member_name());
    mre->type_arguments().move_to(new_ident->type_arguments());
    copy_annotations_from(new_ident, mre);
    new_ident->remove_annotations<semantics::MemberResolveResult>();  // remove the reference to the property of the anonymous type
    auto it = from_or_let_identifiers.find(mre->member_name());
    if (it != from_or_let_identifiers.end() && it->second != nullptr) new_ident->add_annotation(it->second);
    context_->step("Replace transparent query identifier reference", mre);
    mre->replace_with(new_ident);
    context_->end_step(new_ident);
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
