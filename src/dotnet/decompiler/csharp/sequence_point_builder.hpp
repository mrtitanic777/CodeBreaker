// dotnet/decompiler/csharp/sequence_point_builder.hpp -- the sequence points of a decompiled syntax tree: for each ILFunction the list of
// IL ranges and the C# text range each one belongs to (what a debugger uses to step through the written C#).
//
//     SequencePointBuilder builder;
//     syntax_tree->accept_visitor(builder);                       // after the tree is written (its nodes have locations)
//     auto points = builder.get_sequence_points();                // ILFunction -> sorted points, hidden ones filling the gaps
//
// How it works: every statement and expression node is annotated with the ILInstruction(s) it was made from, each instruction has the
// IL ranges it came from, and the instructions form a tree. The builder visits the nodes, starting a sequence point for each statement
// (start_sequence_point), adding the IL ranges of the node's instructions and of their descendants (add_to_sequence_point; never into
// a nested ILFunction, which gets its own points) and ending it with the statement's text range (end_sequence_point). The debugger
// does not support breakpoints on single expressions, so a statement's sub-expressions share its sequence point; a lambda's body is
// the exception. Only the points are computed; writing them as a PDB is not ported.
//
// The columns of tokens that are not nodes (a header's brace or parenthesis, a keyword) are the neighbouring node's column plus a
// fixed offset, which assumes the decompiler's own formatting (CreateAllman with its overrides); with any other formatting the columns
// are off.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: SequencePointBuilder, from SequencePointBuilder.cs), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not ported here: the PDB writer (DebugInfo).
#pragma once

#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>

#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/text_location.hpp"
#include "dotnet/decompiler/debug_info/sequence_point.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/util/insertion_ordered_map.hpp"

namespace cb::dotnet::decompiler::csharp {

class SequencePointBuilder final : public syntax::DepthFirstAstVisitor {
public:
    using Interval = util::Interval;
    using SequencePointsByFunction = util::InsertionOrderedMap<il::ILFunction*, std::vector<debug_info::SequencePoint>>;

    SequencePointBuilder() = default;
    SequencePointBuilder(const SequencePointBuilder&) = delete;
    SequencePointBuilder& operator=(const SequencePointBuilder&) = delete;

    // Called after the visitor is done: each function's points sorted by IL offset, overlaps removed, ends moved to the sequence point
    // candidates, and hidden points over the gaps and the function's epilog.
    SequencePointsByFunction get_sequence_points();

    // Whether the instruction's IL range can go into a sequence point (not empty, in a tree, not a block or container).
    static bool has_usable_il_range(const il::ILInstruction* inst);

    void visit_block_statement(syntax::BlockStatement& block_statement) override;
    void visit_field_declaration(syntax::FieldDeclaration& field_declaration) override;
    void visit_property_declaration(syntax::PropertyDeclaration& property_declaration) override;
    void visit_indexer_declaration(syntax::IndexerDeclaration& indexer_declaration) override;
    void visit_for_statement(syntax::ForStatement& for_statement) override;
    void visit_event_declaration(syntax::EventDeclaration& event_declaration) override;
    void visit_switch_statement(syntax::SwitchStatement& switch_statement) override;
    void visit_switch_section(syntax::SwitchSection& switch_section) override;
    void visit_lambda_expression(syntax::LambdaExpression& lambda_expression) override;
    void visit_query_continuation_clause(syntax::QueryContinuationClause& query_continuation_clause) override;
    void visit_query_from_clause(syntax::QueryFromClause& query_from_clause) override;
    void visit_query_group_clause(syntax::QueryGroupClause& query_group_clause) override;
    void visit_query_join_clause(syntax::QueryJoinClause& query_join_clause) override;
    void visit_query_let_clause(syntax::QueryLetClause& query_let_clause) override;
    void visit_query_ordering(syntax::QueryOrdering& query_ordering) override;
    void visit_query_select_clause(syntax::QuerySelectClause& query_select_clause) override;
    void visit_query_where_clause(syntax::QueryWhereClause& query_where_clause) override;
    void visit_using_statement(syntax::UsingStatement& using_statement) override;
    void visit_foreach_statement(syntax::ForeachStatement& foreach_statement) override;
    void visit_lock_statement(syntax::LockStatement& lock_statement) override;
    void visit_if_else_statement(syntax::IfElseStatement& if_else_statement) override;
    void visit_while_statement(syntax::WhileStatement& while_statement) override;
    void visit_do_while_statement(syntax::DoWhileStatement& do_while_statement) override;
    void visit_fixed_statement(syntax::FixedStatement& fixed_statement) override;
    void visit_try_catch_statement(syntax::TryCatchStatement& try_catch_statement) override;
    void visit_catch_clause(syntax::CatchClause& catch_clause) override;

protected:
    void visit_children(syntax::AstNode& node) override;

private:
    using TextLocation = syntax::TextLocation;

    // What is recorded for one sequence point.
    struct StatePerSequencePoint {
        // The main AST node of the sequence point.
        syntax::AstNode* primary_node = nullptr;
        // The IL intervals of the sequence point (nullopt: no sequence point is being collected, upstream's default struct).
        std::optional<std::vector<Interval>> intervals;
        // The function containing the sequence point.
        il::ILFunction* function = nullptr;
    };

    void visit_as_sequence_point(syntax::AstNode* node);
    void visit_member_initializer(syntax::Expression* initializer);
    // Start a new C# statement = new sequence point.
    void start_sequence_point(syntax::AstNode* primary_node);
    void end_sequence_point(TextLocation start_location, TextLocation end_location);
    void add_to_sequence_point_raw(il::ILFunction* function, const std::vector<Interval>& ranges);
    // Adds the ILAst instructions the node is annotated with (and their sub-instructions, unless another sequence point has them).
    void add_to_sequence_point(syntax::AstNode* node);
    void add_to_sequence_point(il::ILInstruction* inst);

    static TextLocation offset(TextLocation location, int columns);
    static TextLocation catch_header_end(syntax::CatchClause& catch_clause);

    std::vector<std::pair<il::ILFunction*, debug_info::SequencePoint>> sequence_points_;
    std::unordered_set<il::ILInstruction*> mapped_instructions_;
    // The state of the outer statements.
    std::vector<StatePerSequencePoint> outer_states_;
    // The state of the current sequence point.
    StatePerSequencePoint current_;
};

}  // namespace cb::dotnet::decompiler::csharp
