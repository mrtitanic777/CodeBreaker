// dotnet/decompiler/csharp/syntax/try_catch_statement.hpp -- TryCatchStatement and CatchClause: `try { } catch (type name) when (condition)
// { } finally { }`.
//
//     Statement* stmt = make<TryCatchStatement>(try_block, std::vector<CatchClause*>{clause}, finally_block);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Syntax: TryCatchStatement, CatchClause, from Statements/TryCatchStatement.cs with the
// members ILSpy 11's source generator adds), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/statement.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

class TryCatchStatement final : public Statement {
public:
    static constexpr AstNodeKind kind = AstNodeKind::TryCatchStatement;
    static constexpr std::string_view try_keyword = "try";
    static constexpr std::string_view finally_keyword = "finally";

    static constexpr CSharpSlotInfoT<BlockStatement> try_block_slot{"TryBlock", false, &Slots::try_block, false};
    static constexpr CSharpSlotInfoT<CatchClause> catch_clauses_slot{"CatchClauses", true, &Slots::catch_clause, true};
    static constexpr CSharpSlotInfoT<BlockStatement> finally_block_slot{"FinallyBlock", false, &Slots::finally_block, true};

    TryCatchStatement();
    explicit TryCatchStatement(BlockStatement* try_block_value);
    TryCatchStatement(BlockStatement* try_block_value, const std::vector<CatchClause*>& catch_clauses_value);
    TryCatchStatement(BlockStatement* try_block_value, const std::vector<CatchClause*>& catch_clauses_value, BlockStatement* finally_block_value);

    BlockStatement* try_block() const { return try_block_; }
    void set_try_block(BlockStatement* value);
    AstNodeCollection<CatchClause>& catch_clauses() const { return catch_clauses_; }
    BlockStatement* finally_block() const { return finally_block_; }
    void set_finally_block(BlockStatement* value);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    AstNodeCollectionBase* get_collection_by_kind(const CSharpSlotInfo& slot_kind) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_try_catch_statement(*this); }
    AstNode* shallow_clone() const override { return make<TryCatchStatement>(*this); }

private:
    ChildPtr<BlockStatement> try_block_;
    mutable AstNodeCollection<CatchClause> catch_clauses_{this, Slots::catch_clause, 1, false};
    ChildPtr<BlockStatement> finally_block_;
};

class CatchClause : public AstNode {
public:
    static constexpr AstNodeKind kind = AstNodeKind::CatchClause;
    static constexpr std::string_view catch_keyword = "catch";
    static constexpr std::string_view when_keyword = "when";
    static constexpr std::string_view cond_l_par = "(";
    static constexpr std::string_view cond_r_par = ")";

    static constexpr CSharpSlotInfoT<AstType> type_slot{"Type", false, &Slots::type, true};
    static constexpr CSharpSlotInfoT<Identifier> variable_name_token_slot{"VariableNameToken", false, &Slots::identifier, true};
    static constexpr CSharpSlotInfoT<Expression> condition_slot{"Condition", false, &Slots::condition, true};
    static constexpr CSharpSlotInfoT<BlockStatement> body_slot{"Body", false, &Slots::body, false};

    CatchClause();
    CatchClause(AstType* type_value, const std::string& variable_name_value, Expression* condition_value, BlockStatement* body_value);

    AstType* type() const { return type_; }
    void set_type(AstType* value);
    Identifier* variable_name_token() const { return variable_name_token_; }
    void set_variable_name_token(Identifier* value);
    std::string variable_name() const { return variable_name_token_ != nullptr ? variable_name_token_->name() : std::string(); }
    void set_variable_name(const std::string& value) { set_variable_name_token(Identifier::create_if_not_empty(value)); }
    Expression* condition() const { return condition_; }
    void set_condition(Expression* value);
    BlockStatement* body() const { return body_; }
    void set_body(BlockStatement* value);

    // The node that stands for `pattern` where a CatchClause goes (upstream's implicit conversion Pattern -> CatchClause).
    static CatchClause* from_pattern(pattern_matching::Pattern* pattern);

    AstNodeKind node_kind() const override { return kind; }
    bool do_match(AstNode* other, const pattern_matching::Match& match) const override;

    int get_child_count() const override;
    AstNode* get_child_by_index(int index) const override;
    void set_child_by_index(int index, AstNode* value) override;
    const CSharpSlotInfo& get_child_slot_info(int index) const override;
    void clone_children_into(AstNode* copy) const override;

protected:
    void dispatch_to(IAstVisitor& visitor) override { visitor.visit_catch_clause(*this); }
    AstNode* shallow_clone() const override { return make<CatchClause>(*this); }

private:
    ChildPtr<AstType> type_;
    ChildPtr<Identifier> variable_name_token_;
    ChildPtr<Expression> condition_;
    ChildPtr<BlockStatement> body_;
};

}  // namespace cb::dotnet::decompiler::csharp::syntax
