// dotnet/decompiler/csharp/syntax/query_expression.hpp -- a query expression, `from x in xs where c select x`, and its clauses.
//
//     auto* q = make<QueryExpression>(std::vector<QueryClause*>{from, select});
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: QueryClause, QueryExpression, QueryContinuationClause, QueryFromClause, QueryLetClause, QueryWhereClause, QueryJoinClause, QueryOrderClause, QueryOrdering, QuerySelectClause, QueryGroupClause, from Expressions/QueryExpression.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/expression.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"
#include "dotnet/decompiler/csharp/syntax/tokens.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

enum class QueryOrderingDirection : std::uint8_t {
    None,
    Ascending,
    Descending,
};

// The base of the clauses of a query expression (from, let, where, join, orderby, select, group, into).
class QueryClause : public AstNode {
protected:
    QueryClause() = default;
};

class QueryExpression final : public Expression {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryExpression;

    static constexpr CSharpSlotInfoT<QueryClause> clauses_slot{"Clauses", true, &Slots::clause, true};

    QueryExpression() = default;
    explicit QueryExpression(const std::vector<QueryClause*>& clauses);
    explicit QueryExpression(std::initializer_list<QueryClause*> clauses);

    AstNodeCollection<QueryClause>& clauses() const { return clauses_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_expression(*this); }
    AstNode* shallow_clone() const override { return make<QueryExpression>(*this); }

private:
    mutable AstNodeCollection<QueryClause> clauses_{this, Slots::clause, 0, true};
};

class QueryContinuationClause final : public QueryClause {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryContinuationClause;

    static constexpr std::string_view into_keyword = "into";

    static constexpr CSharpSlotInfoT<QueryExpression> preceding_query_slot{"PrecedingQuery", false, &Slots::preceding_query, false};
    static constexpr CSharpSlotInfoT<Identifier> identifier_token_slot{"IdentifierToken", false, &Slots::identifier, false};

    QueryContinuationClause() = default;
    QueryContinuationClause(QueryExpression* preceding_query, const std::string& identifier);

    QueryExpression* preceding_query() const { return preceding_query_; }
    void set_preceding_query(QueryExpression* value);
    Identifier* identifier_token() const { return identifier_token_; }
    void set_identifier_token(Identifier* value);
    std::string identifier() const { return identifier_token_ != nullptr ? identifier_token_->name() : std::string(); }
    void set_identifier(const std::string& value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_continuation_clause(*this); }
    AstNode* shallow_clone() const override { return make<QueryContinuationClause>(*this); }

private:
    ChildPtr<QueryExpression> preceding_query_;
    ChildPtr<Identifier> identifier_token_;
};

class QueryFromClause final : public QueryClause {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryFromClause;

    static constexpr std::string_view from_keyword = "from";
    static constexpr std::string_view in_keyword = "in";

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, true};
    static constexpr CSharpSlotInfoT<Identifier> identifier_token_slot{"IdentifierToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    QueryFromClause() = default;
    QueryFromClause(AstType* type, const std::string& identifier, Expression* expression);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    Identifier* identifier_token() const { return identifier_token_; }
    void set_identifier_token(Identifier* value);
    std::string identifier() const { return identifier_token_ != nullptr ? identifier_token_->name() : std::string(); }
    void set_identifier(const std::string& value);
    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_from_clause(*this); }
    AstNode* shallow_clone() const override { return make<QueryFromClause>(*this); }

private:
    ChildPtr<AstType> type_;
    ChildPtr<Identifier> identifier_token_;
    ChildPtr<Expression> expression_;
};

class QueryLetClause final : public QueryClause {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryLetClause;

    static constexpr std::string_view let_keyword = "let";

    static constexpr CSharpSlotInfoT<Identifier> identifier_token_slot{"IdentifierToken", false, &Slots::identifier, false};
    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    QueryLetClause() = default;
    QueryLetClause(const std::string& identifier, Expression* expression);

    Identifier* identifier_token() const { return identifier_token_; }
    void set_identifier_token(Identifier* value);
    std::string identifier() const { return identifier_token_ != nullptr ? identifier_token_->name() : std::string(); }
    void set_identifier(const std::string& value);
    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_let_clause(*this); }
    AstNode* shallow_clone() const override { return make<QueryLetClause>(*this); }

private:
    ChildPtr<Identifier> identifier_token_;
    ChildPtr<Expression> expression_;
};

class QueryWhereClause final : public QueryClause {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryWhereClause;

    static constexpr std::string_view where_keyword = "where";

    static constexpr CSharpSlotInfoT<Expression> condition_slot{"Condition", false, &Slots::condition, false};

    QueryWhereClause() = default;
    explicit QueryWhereClause(Expression* condition);

    Expression* condition() const { return condition_; }
    void set_condition(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_where_clause(*this); }
    AstNode* shallow_clone() const override { return make<QueryWhereClause>(*this); }

private:
    ChildPtr<Expression> condition_;
};

class QueryJoinClause final : public QueryClause {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryJoinClause;

    static constexpr std::string_view join_keyword = "join";
    static constexpr std::string_view in_keyword = "in";
    static constexpr std::string_view on_keyword = "on";
    static constexpr std::string_view equals_keyword = "equals";
    static constexpr std::string_view into_keyword = "into";

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, true};
    static constexpr CSharpSlotInfoT<Identifier> join_identifier_token_slot{"JoinIdentifierToken", false, &Slots::join_identifier, false};
    static constexpr CSharpSlotInfoT<Expression> in_expression_slot{"InExpression", false, &Slots::in_expression, false};
    static constexpr CSharpSlotInfoT<Expression> on_expression_slot{"OnExpression", false, &Slots::on_expression, false};
    static constexpr CSharpSlotInfoT<Expression> equals_expression_slot{"EqualsExpression", false, &Slots::equals_expression, false};
    static constexpr CSharpSlotInfoT<Identifier> into_identifier_token_slot{"IntoIdentifierToken", false, &Slots::into_identifier, true};

    QueryJoinClause() = default;
    QueryJoinClause(AstType* type, const std::string& join_identifier, Expression* in_expression, Expression* on_expression, Expression* equals_expression, const std::string& into_identifier);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    Identifier* join_identifier_token() const { return join_identifier_token_; }
    void set_join_identifier_token(Identifier* value);
    std::string join_identifier() const { return join_identifier_token_ != nullptr ? join_identifier_token_->name() : std::string(); }
    void set_join_identifier(const std::string& value);
    Expression* in_expression() const { return in_expression_; }
    void set_in_expression(Expression* value);
    Expression* on_expression() const { return on_expression_; }
    void set_on_expression(Expression* value);
    Expression* equals_expression() const { return equals_expression_; }
    void set_equals_expression(Expression* value);
    Identifier* into_identifier_token() const { return into_identifier_token_; }
    void set_into_identifier_token(Identifier* value);
    std::string into_identifier() const { return into_identifier_token_ != nullptr ? into_identifier_token_->name() : std::string(); }
    void set_into_identifier(const std::string& value);

    // Whether the join has an `into` identifier (derived, so not compared when matching).
    bool is_group_join() const { return !into_identifier().empty(); }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_join_clause(*this); }
    AstNode* shallow_clone() const override { return make<QueryJoinClause>(*this); }

private:
    ChildPtr<AstType> type_;
    ChildPtr<Identifier> join_identifier_token_;
    ChildPtr<Expression> in_expression_;
    ChildPtr<Expression> on_expression_;
    ChildPtr<Expression> equals_expression_;
    ChildPtr<Identifier> into_identifier_token_;
};

class QueryOrderClause final : public QueryClause {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryOrderClause;

    static constexpr std::string_view orderby_keyword = "orderby";

    static constexpr CSharpSlotInfoT<QueryOrdering> orderings_slot{"Orderings", true, &Slots::ordering, true};

    QueryOrderClause() = default;
    explicit QueryOrderClause(const std::vector<QueryOrdering*>& orderings);
    explicit QueryOrderClause(std::initializer_list<QueryOrdering*> orderings);

    AstNodeCollection<QueryOrdering>& orderings() const { return orderings_; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_order_clause(*this); }
    AstNode* shallow_clone() const override { return make<QueryOrderClause>(*this); }

private:
    mutable AstNodeCollection<QueryOrdering> orderings_{this, Slots::ordering, 0, true};
};

class QueryOrdering final : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryOrdering;

    static constexpr std::string_view ascending_keyword = "ascending";
    static constexpr std::string_view descending_keyword = "descending";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    QueryOrdering() = default;
    QueryOrdering(Expression* expression, QueryOrderingDirection direction);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);
    QueryOrderingDirection direction() const { return direction_; }
    void set_direction(QueryOrderingDirection value) { direction_ = value; }

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_ordering(*this); }
    AstNode* shallow_clone() const override { return make<QueryOrdering>(*this); }

private:
    ChildPtr<Expression> expression_;
    QueryOrderingDirection direction_ = QueryOrderingDirection{};
};

class QuerySelectClause final : public QueryClause {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QuerySelectClause;

    static constexpr std::string_view select_keyword = "select";

    static constexpr CSharpSlotInfoT<Expression> expression_slot{"Expression", false, &Slots::expression, false};

    QuerySelectClause() = default;
    explicit QuerySelectClause(Expression* expression);

    Expression* expression() const { return expression_; }
    void set_expression(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_select_clause(*this); }
    AstNode* shallow_clone() const override { return make<QuerySelectClause>(*this); }

private:
    ChildPtr<Expression> expression_;
};

class QueryGroupClause final : public QueryClause {
public:
    static constexpr AstNodeKind kind = AstNodeKind::QueryGroupClause;

    static constexpr std::string_view group_keyword = "group";
    static constexpr std::string_view by_keyword = "by";

    static constexpr CSharpSlotInfoT<Expression> projection_slot{"Projection", false, &Slots::projection, false};
    static constexpr CSharpSlotInfoT<Expression> key_slot{"Key", false, &Slots::key, false};

    QueryGroupClause() = default;
    QueryGroupClause(Expression* projection, Expression* key);

    Expression* projection() const { return projection_; }
    void set_projection(Expression* value);
    Expression* key() const { return key_; }
    void set_key(Expression* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_query_group_clause(*this); }
    AstNode* shallow_clone() const override { return make<QueryGroupClause>(*this); }

private:
    ChildPtr<Expression> projection_;
    ChildPtr<Expression> key_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
