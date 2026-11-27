// dotnet/decompiler/csharp/sequence_point_builder.cpp -- see sequence_point_builder.hpp.
#include "dotnet/decompiler/csharp/sequence_point_builder.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler::csharp {

using debug_info::SequencePoint;
using il::ILFunction;
using il::ILInstruction;
using syntax::AstNode;
using syntax::Expression;

namespace {

// The closest ILFunction among the instruction and its ancestors (nullptr for none).
ILFunction* closest_function(ILInstruction* inst) {
    for (ILInstruction* node = inst; node != nullptr; node = node->parent())
        if (auto* function = il::as<ILFunction>(node)) return function;
    return nullptr;
}

}  // namespace

void SequencePointBuilder::visit_as_sequence_point(AstNode* node) {
    if (node == nullptr) return;
    start_sequence_point(node);
    node->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    end_sequence_point(node->start_location(), node->end_location());
}

void SequencePointBuilder::visit_children(AstNode& node) {
    syntax::DepthFirstAstVisitor::visit_children(node);
    add_to_sequence_point(&node);
}

void SequencePointBuilder::visit_block_statement(syntax::BlockStatement& block_statement) {
    // Enhanced using variables need special-casing here, because the block syntax is omitted from the text output, so the positions of
    // the opening and closing braces cannot be used.
    auto* using_parent = syntax::as<syntax::UsingStatement>(block_statement.parent());
    bool is_enhanced_using = using_parent != nullptr && using_parent->is_enhanced();
    if (!is_enhanced_using) {
        auto* block_container = block_statement.annotation<il::BlockContainer>();
        if (block_container != nullptr) {
            start_sequence_point(&block_statement);
            int interval_start;
            auto* handler = il::as<il::TryCatchHandler>(block_container->parent());
            if (handler != nullptr && !handler->exception_specifier_il_range().is_empty()) {
                // A block container that is part of a TryCatchHandler does not steal the exception-specifier IL range.
                interval_start = handler->exception_specifier_il_range().end;
            } else {
                interval_start = block_container->start_il_offset();
            }
            // The end will be set to the first sequence point candidate location before the first statement of the function when the
            // sequence point is adjusted.
            int interval_end = interval_start + 1;

            current_.intervals->push_back(Interval(interval_start, interval_end));
            current_.function = closest_function(block_container);
            end_sequence_point(block_statement.start_location(), offset(block_statement.start_location(), 1));
        }
        // Blocks that are not the top-level function block have no ILInstruction annotations, so there is no IL range to give their
        // opening brace; no sequence point is made for it.
    }

    for (syntax::Statement* stmt : block_statement.statements()) visit_as_sequence_point(stmt);
    auto* implicit_return = block_statement.annotation<ImplicitReturnAnnotation>();
    if (implicit_return != nullptr && !is_enhanced_using) {
        start_sequence_point(&block_statement);
        add_to_sequence_point(implicit_return->leave);
        end_sequence_point(offset(block_statement.end_location(), -1), block_statement.end_location());
    }
}

void SequencePointBuilder::visit_field_declaration(syntax::FieldDeclaration& field_declaration) {
    for (syntax::VariableInitializer* variable : field_declaration.variables()) visit_member_initializer(variable->initializer());
    syntax::DepthFirstAstVisitor::visit_field_declaration(field_declaration);
}

void SequencePointBuilder::visit_property_declaration(syntax::PropertyDeclaration& property_declaration) {
    if (property_declaration.expression_body() != nullptr) {
        visit_as_sequence_point(property_declaration.expression_body());
    } else if (property_declaration.initializer() != nullptr) {
        visit_member_initializer(property_declaration.initializer());
    } else {
        syntax::DepthFirstAstVisitor::visit_property_declaration(property_declaration);
    }
}

// A field, auto-property or event initializer is written once at the declaration but, in IL, runs in every instance constructor that
// does not chain to this(...) (and static initializers run in the static constructor). The primary sequence point covers the
// constructor the initializer was lifted from; MemberInitializerInOtherConstructorsAnnotation carries the initializer's copies from the
// remaining constructors, so a matching sequence point is made, at the same source location but with the other constructor's IL, for
// each of them.
void SequencePointBuilder::visit_member_initializer(Expression* initializer) {
    if (initializer == nullptr) return;
    visit_as_sequence_point(initializer);
    auto* annotation = initializer->annotation<MemberInitializerInOtherConstructorsAnnotation>();
    if (annotation == nullptr) return;
    for (Expression* other : annotation->initializers) {
        start_sequence_point(initializer);
        other->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
        end_sequence_point(initializer->start_location(), initializer->end_location());
    }
}

void SequencePointBuilder::visit_indexer_declaration(syntax::IndexerDeclaration& indexer_declaration) {
    if (indexer_declaration.expression_body() != nullptr) {
        visit_as_sequence_point(indexer_declaration.expression_body());
    } else {
        syntax::DepthFirstAstVisitor::visit_indexer_declaration(indexer_declaration);
    }
}

void SequencePointBuilder::visit_for_statement(syntax::ForStatement& for_statement) {
    // Every element of a for-statement is its own sequence point.
    for (syntax::Statement* init : for_statement.initializers()) visit_as_sequence_point(init);
    visit_as_sequence_point(for_statement.condition());
    for (syntax::Statement* inc : for_statement.iterators()) visit_as_sequence_point(inc);
    visit_as_sequence_point(for_statement.embedded_statement());
}

void SequencePointBuilder::visit_event_declaration(syntax::EventDeclaration& event_declaration) {
    for (syntax::VariableInitializer* variable : event_declaration.variables()) visit_member_initializer(variable->initializer());
    syntax::DepthFirstAstVisitor::visit_event_declaration(event_declaration);
}

void SequencePointBuilder::visit_switch_statement(syntax::SwitchStatement& switch_statement) {
    start_sequence_point(&switch_statement);
    switch_statement.expression()->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    for (syntax::SwitchSection* section : switch_statement.switch_sections()) {
        // The sections do not contribute to the current sequence point.
        section->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    }
    // Add the switch statement itself to the sequence point (only after the sections are visited).
    add_to_sequence_point(&switch_statement);
    end_sequence_point(switch_statement.start_location(), offset(switch_statement.expression()->end_location(), 1));
}

void SequencePointBuilder::visit_switch_section(syntax::SwitchSection& switch_section) {
    // Every statement in the switch section is its own sequence point.
    for (syntax::Statement* stmt : switch_section.statements()) visit_as_sequence_point(stmt);
}

void SequencePointBuilder::visit_lambda_expression(syntax::LambdaExpression& lambda_expression) {
    add_to_sequence_point(&lambda_expression);
    visit_as_sequence_point(lambda_expression.body());
}

void SequencePointBuilder::visit_query_continuation_clause(syntax::QueryContinuationClause& query_continuation_clause) {
    add_to_sequence_point(&query_continuation_clause);
    visit_as_sequence_point(query_continuation_clause.preceding_query());
}

void SequencePointBuilder::visit_query_from_clause(syntax::QueryFromClause& query_from_clause) {
    if (query_from_clause.parent()->first_child() != &query_from_clause) {
        add_to_sequence_point(&query_from_clause);
        visit_as_sequence_point(query_from_clause.expression());
    } else {
        syntax::DepthFirstAstVisitor::visit_query_from_clause(query_from_clause);
    }
}

void SequencePointBuilder::visit_query_group_clause(syntax::QueryGroupClause& query_group_clause) {
    add_to_sequence_point(&query_group_clause);
    visit_as_sequence_point(query_group_clause.projection());
    visit_as_sequence_point(query_group_clause.key());
}

void SequencePointBuilder::visit_query_join_clause(syntax::QueryJoinClause& query_join_clause) {
    add_to_sequence_point(&query_join_clause);
    visit_as_sequence_point(query_join_clause.on_expression());
    visit_as_sequence_point(query_join_clause.equals_expression());
}

void SequencePointBuilder::visit_query_let_clause(syntax::QueryLetClause& query_let_clause) {
    add_to_sequence_point(&query_let_clause);
    visit_as_sequence_point(query_let_clause.expression());
}

void SequencePointBuilder::visit_query_ordering(syntax::QueryOrdering& query_ordering) {
    add_to_sequence_point(&query_ordering);
    visit_as_sequence_point(query_ordering.expression());
}

void SequencePointBuilder::visit_query_select_clause(syntax::QuerySelectClause& query_select_clause) {
    add_to_sequence_point(&query_select_clause);
    visit_as_sequence_point(query_select_clause.expression());
}

void SequencePointBuilder::visit_query_where_clause(syntax::QueryWhereClause& query_where_clause) {
    add_to_sequence_point(&query_where_clause);
    visit_as_sequence_point(query_where_clause.condition());
}

void SequencePointBuilder::visit_using_statement(syntax::UsingStatement& using_statement) {
    start_sequence_point(&using_statement);
    using_statement.resource_acquisition()->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    visit_as_sequence_point(using_statement.embedded_statement());
    add_to_sequence_point(&using_statement);
    if (using_statement.is_enhanced())
        end_sequence_point(using_statement.start_location(), using_statement.resource_acquisition()->end_location());
    else
        end_sequence_point(using_statement.start_location(), offset(using_statement.resource_acquisition()->end_location(), 1));
}

void SequencePointBuilder::visit_foreach_statement(syntax::ForeachStatement& foreach_statement) {
    auto* foreach_info = foreach_statement.annotation<ForeachAnnotation>();
    if (foreach_info == nullptr) {
        syntax::DepthFirstAstVisitor::visit_foreach_statement(foreach_statement);
        return;
    }
    // TODO upstream: add a sequence point on the foreach token (mapped to the nop before the using instruction).
    start_sequence_point(&foreach_statement);
    foreach_statement.in_expression()->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    add_to_sequence_point(foreach_info->get_enumerator_call);
    end_sequence_point(foreach_statement.in_expression()->start_location(), foreach_statement.in_expression()->end_location());

    start_sequence_point(&foreach_statement);
    add_to_sequence_point(foreach_info->move_next_call);
    constexpr int in_keyword_length = 2;   // "in"
    end_sequence_point(offset(foreach_statement.variable_designation()->end_location(), 1),
                       offset(foreach_statement.variable_designation()->end_location(), 1 + in_keyword_length));

    start_sequence_point(&foreach_statement);
    add_to_sequence_point(foreach_info->get_current_call);
    end_sequence_point(foreach_statement.variable_type()->start_location(), foreach_statement.variable_designation()->end_location());

    visit_as_sequence_point(foreach_statement.embedded_statement());
}

void SequencePointBuilder::visit_lock_statement(syntax::LockStatement& lock_statement) {
    start_sequence_point(&lock_statement);
    lock_statement.expression()->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    visit_as_sequence_point(lock_statement.embedded_statement());
    add_to_sequence_point(&lock_statement);
    end_sequence_point(lock_statement.start_location(), offset(lock_statement.expression()->end_location(), 1));
}

void SequencePointBuilder::visit_if_else_statement(syntax::IfElseStatement& if_else_statement) {
    start_sequence_point(&if_else_statement);
    if_else_statement.condition()->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    visit_as_sequence_point(if_else_statement.true_statement());
    visit_as_sequence_point(if_else_statement.false_statement());
    add_to_sequence_point(&if_else_statement);
    end_sequence_point(if_else_statement.start_location(), offset(if_else_statement.condition()->end_location(), 1));
}

void SequencePointBuilder::visit_while_statement(syntax::WhileStatement& while_statement) {
    start_sequence_point(&while_statement);
    while_statement.condition()->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    visit_as_sequence_point(while_statement.embedded_statement());
    add_to_sequence_point(&while_statement);
    end_sequence_point(while_statement.start_location(), offset(while_statement.condition()->end_location(), 1));
}

void SequencePointBuilder::visit_do_while_statement(syntax::DoWhileStatement& do_while_statement) {
    start_sequence_point(&do_while_statement);
    visit_as_sequence_point(do_while_statement.embedded_statement());
    do_while_statement.condition()->accept_visitor(static_cast<syntax::IAstVisitor&>(*this));
    add_to_sequence_point(&do_while_statement);
    // The trailing 'while (' precedes the condition: "while" + ' ' + '('.
    constexpr int while_keyword_length = 5;
    end_sequence_point(offset(do_while_statement.condition()->start_location(), -(while_keyword_length + 2)),
                       offset(do_while_statement.condition()->end_location(), 1));
}

void SequencePointBuilder::visit_fixed_statement(syntax::FixedStatement& fixed_statement) {
    for (syntax::VariableInitializer* v : fixed_statement.variables()) visit_as_sequence_point(v);
    visit_as_sequence_point(fixed_statement.embedded_statement());
}

void SequencePointBuilder::visit_try_catch_statement(syntax::TryCatchStatement& try_catch_statement) {
    visit_as_sequence_point(try_catch_statement.try_block());
    for (syntax::CatchClause* c : try_catch_statement.catch_clauses()) visit_as_sequence_point(c);
    visit_as_sequence_point(try_catch_statement.finally_block());
}

void SequencePointBuilder::visit_catch_clause(syntax::CatchClause& catch_clause) {
    if (catch_clause.condition() == nullptr) {
        auto* try_catch_handler = catch_clause.annotation<il::TryCatchHandler>();
        if (try_catch_handler != nullptr && !try_catch_handler->exception_specifier_il_range().is_empty()) {
            start_sequence_point(&catch_clause);
            ILFunction* function = closest_function(try_catch_handler);
            add_to_sequence_point_raw(function, {try_catch_handler->exception_specifier_il_range()});
            end_sequence_point(catch_clause.start_location(), catch_header_end(catch_clause));
        }
    } else {
        start_sequence_point(&catch_clause);
        add_to_sequence_point(catch_clause.condition());
        // The 'when (' precedes the condition: "when" + ' ' + '('.
        constexpr int when_keyword_length = 4;
        end_sequence_point(offset(catch_clause.condition()->start_location(), -(when_keyword_length + 2)),
                           offset(catch_clause.condition()->end_location(), 1));
    }
    visit_as_sequence_point(catch_clause.body());
}

void SequencePointBuilder::start_sequence_point(AstNode* primary_node) {
    outer_states_.push_back(current_);
    current_ = StatePerSequencePoint();
    current_.primary_node = primary_node;
    current_.intervals.emplace();
}

// Token nodes are not part of the AST, so the brace, parenthesis and keyword positions that statement-header sequence points address
// are derived from the surrounding real nodes plus a fixed column offset. The offsets assume the decompiler's own formatting: one space
// before a header's '(' and none within it, single spaces around keywords. That holds because output is always printed with
// DecompilerSettings' CSharpFormattingOptions (Allman plus its overrides). Sequence points are only valid under that formatting:
// printing the tree with other options before generating them would shift these columns and produce wrong line/column mappings.
SequencePointBuilder::TextLocation SequencePointBuilder::offset(TextLocation location, int columns) {
    return TextLocation(location.line(), location.column() + columns);
}

// The end of a 'catch' header: the ')' after the variable (or the type, if unnamed), or just past the 'catch' keyword when there is no
// exception specifier.
SequencePointBuilder::TextLocation SequencePointBuilder::catch_header_end(syntax::CatchClause& catch_clause) {
    constexpr int catch_keyword_length = 5;
    if (catch_clause.type() == nullptr) return offset(catch_clause.start_location(), catch_keyword_length);
    AstNode* before_r_paren = catch_clause.variable_name_token() == nullptr ? static_cast<AstNode*>(catch_clause.type())
                                                                            : static_cast<AstNode*>(catch_clause.variable_name_token());
    return offset(before_r_paren->end_location(), 1);
}

void SequencePointBuilder::end_sequence_point(TextLocation start_location, TextLocation end_location) {
    assert(!start_location.is_empty());   // missing start location
    assert(!end_location.is_empty());     // missing end location
    if (current_.intervals.has_value() && !current_.intervals->empty() && current_.function != nullptr) {
        // A LongSet deduplicates and merges the intervals.
        std::vector<util::LongInterval> long_intervals;
        long_intervals.reserve(current_.intervals->size());
        for (const Interval& i : *current_.intervals) long_intervals.emplace_back(i.start, i.end);
        util::LongSet long_set(long_intervals);
        assert(!long_set.is_empty());
        SequencePoint point;
        point.offset = static_cast<int>(long_set.intervals().front().start);
        point.end_offset = static_cast<int>(long_set.intervals().front().end);
        point.start_line = start_location.line();
        point.start_column = start_location.column();
        point.end_line = end_location.line();
        point.end_column = end_location.column();
        sequence_points_.emplace_back(current_.function, point);
    }
    current_ = outer_states_.back();
    outer_states_.pop_back();
}

void SequencePointBuilder::add_to_sequence_point_raw(ILFunction* function, const std::vector<Interval>& ranges) {
    current_.intervals->insert(current_.intervals->end(), ranges.begin(), ranges.end());
    assert(current_.function == nullptr || current_.function == function);
    current_.function = function;
}

void SequencePointBuilder::add_to_sequence_point(AstNode* node) {
    for (il::ArenaObject* annotation : node->annotations())
        if (auto* inst = dynamic_cast<ILInstruction*>(annotation)) add_to_sequence_point(inst);
}

void SequencePointBuilder::add_to_sequence_point(ILInstruction* inst) {
    if (!mapped_instructions_.insert(inst).second) {
        // inst was already used by a nested sequence point within this sequence point.
        return;
    }
    // Add the IL range of this instruction to the current sequence point.
    if (has_usable_il_range(inst) && current_.intervals.has_value()) {
        for (const Interval& range : inst->il_ranges()) current_.intervals->push_back(range);
        ILFunction* function = closest_function(inst->parent());
        assert(current_.function == nullptr || current_.function == function);
        current_.function = function;
    }

    // Do not add the instructions of lambdas and delegates.
    if (il::is<ILFunction>(inst)) return;

    // Also add the child IL instructions, unless another C# expression has processed them already.
    for (int i = 0; i < inst->get_child_count(); ++i) add_to_sequence_point(inst->get_child(i));
}

bool SequencePointBuilder::has_usable_il_range(const ILInstruction* inst) {
    if (inst->il_range_is_empty()) return false;
    if (inst->parent() == nullptr) return false;
    return !(il::is<il::BlockContainer>(inst) || il::is<il::Block>(inst));
}

SequencePointBuilder::SequencePointsByFunction SequencePointBuilder::get_sequence_points() {
    SequencePointsByFunction dict;
    for (const auto& [function, sequence_point] : sequence_points_) dict.get_or_add(function).push_back(sequence_point);

    for (auto& entry : dict.items()) {
        ILFunction* function = entry.first;
        // For each function, sort the sequence points and fix the overlaps.
        std::vector<SequencePoint> new_list;
        int pos = 0;
        std::vector<SequencePoint> curr_function_sequence_points = entry.second;
        std::stable_sort(curr_function_sequence_points.begin(), curr_function_sequence_points.end(),
                         [](const SequencePoint& a, const SequencePoint& b) {
                             if (a.offset != b.offset) return a.offset < b.offset;
                             return a.end_offset < b.end_offset;
                         });
        for (const SequencePoint& sequence_point : curr_function_sequence_points) {
            if (sequence_point.offset < pos) {
                // An overlapping sequence point: delete the previous sequence points that are after sequence_point.offset.
                while (!new_list.empty() && new_list.back().end_offset > sequence_point.offset) {
                    SequencePoint& last = new_list.back();
                    if (last.offset >= sequence_point.offset)
                        new_list.pop_back();
                    else
                        last.end_offset = sequence_point.offset;
                }
            }

            new_list.push_back(sequence_point);
            pos = sequence_point.end_offset;
        }
        // Add a hidden sequence point to account for the epilog of the function.
        if (pos < function->code_size) {
            SequencePoint hidden;
            hidden.offset = pos;
            hidden.end_offset = function->code_size;
            hidden.set_hidden();
            new_list.push_back(hidden);
        }

        const std::vector<int>& sequence_point_candidates = function->sequence_point_candidates.value();
        const auto candidate = [&](int index) { return sequence_point_candidates.at(static_cast<std::size_t>(index)); };
        const int candidate_count = static_cast<int>(sequence_point_candidates.size());
        int curr_sp_candidate_index = 0;

        for (int i = 0; i < static_cast<int>(new_list.size()) - 1; i++) {
            const int curr_end_offset_before = new_list[static_cast<std::size_t>(i)].end_offset;
            const int next_offset_before = new_list[static_cast<std::size_t>(i + 1)].offset;

            // Adjust the end offset of the current sequence point to the closest sequence point candidate but do not create an
            // overlapping sequence point. Moving the start of the current sequence point is not required as it is 0 for the first
            // sequence point and is moved during the last iteration for all others.
            while (curr_sp_candidate_index < candidate_count && candidate(curr_sp_candidate_index) < curr_end_offset_before)
                curr_sp_candidate_index++;
            if (curr_sp_candidate_index < candidate_count && candidate(curr_sp_candidate_index) <= next_offset_before)
                new_list[static_cast<std::size_t>(i)].end_offset = candidate(curr_sp_candidate_index);

            // Adjust the start offset of the next sequence point to the closest previous sequence point candidate but do not create an
            // overlapping sequence point.
            while (curr_sp_candidate_index < candidate_count && candidate(curr_sp_candidate_index) < next_offset_before)
                curr_sp_candidate_index++;
            if (curr_sp_candidate_index < candidate_count &&
                candidate(curr_sp_candidate_index - 1) >= new_list[static_cast<std::size_t>(i)].end_offset) {
                new_list[static_cast<std::size_t>(i + 1)].offset = candidate(curr_sp_candidate_index - 1);
                curr_sp_candidate_index--;
            }

            // Fill in any gaps with a hidden sequence point.
            if (new_list[static_cast<std::size_t>(i)].end_offset != new_list[static_cast<std::size_t>(i + 1)].offset) {
                SequencePoint new_sp;
                new_sp.offset = new_list[static_cast<std::size_t>(i)].end_offset;
                new_sp.end_offset = new_list[static_cast<std::size_t>(i + 1)].offset;
                new_sp.set_hidden();
                new_list.insert(new_list.begin() + (++i), new_sp);
            }
        }
        entry.second = std::move(new_list);
    }

    return dict;
}

}  // namespace cb::dotnet::decompiler::csharp
