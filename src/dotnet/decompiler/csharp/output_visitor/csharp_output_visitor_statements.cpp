// dotnet/decompiler/csharp/output_visitor/csharp_output_visitor_statements.cpp -- CSharpOutputVisitor: the visits of the statements (and the
// switch expression). See csharp_output_visitor.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: CSharpOutputVisitor), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;

void CSharpOutputVisitor::visit_block_statement(BlockStatement& block_statement) {
    write_block(&block_statement, policy_.statement_brace_style);
    new_line();
}

void CSharpOutputVisitor::write_block(BlockStatement* block_statement, BraceStyle style) {
    start_node(block_statement);
    open_brace(style);
    for (Statement* node : *block_statement) {
        node->accept_visitor(*this);
    }
    close_brace(style);
    end_node(block_statement);
}

void CSharpOutputVisitor::visit_break_statement(BreakStatement& break_statement) {
    start_node(&break_statement);
    write_keyword("break");
    semicolon();
    end_node(&break_statement);
}

void CSharpOutputVisitor::visit_checked_statement(CheckedStatement& checked_statement) {
    start_node(&checked_statement);
    write_keyword(CheckedStatement::checked_keyword);
    checked_statement.body()->accept_visitor(*this);
    end_node(&checked_statement);
}

void CSharpOutputVisitor::visit_continue_statement(ContinueStatement& continue_statement) {
    start_node(&continue_statement);
    write_keyword("continue");
    semicolon();
    end_node(&continue_statement);
}

void CSharpOutputVisitor::visit_do_while_statement(DoWhileStatement& do_while_statement) {
    start_node(&do_while_statement);
    write_keyword(DoWhileStatement::do_keyword);
    write_embedded_statement(do_while_statement.embedded_statement(), policy_.while_new_line_placement);
    write_keyword(DoWhileStatement::while_keyword);
    space(policy_.space_before_while_parentheses);
    l_par();
    space(policy_.spaces_within_while_parentheses);
    do_while_statement.condition()->accept_visitor(*this);
    space(policy_.spaces_within_while_parentheses);
    r_par();
    semicolon();
    end_node(&do_while_statement);
}

void CSharpOutputVisitor::visit_empty_statement(EmptyStatement& empty_statement) {
    start_node(&empty_statement);
    if (empty_statement.leading_trivia().empty() && empty_statement.trailing_trivia().empty()) semicolon();
    end_node(&empty_statement);
}

void CSharpOutputVisitor::visit_expression_statement(ExpressionStatement& expression_statement) {
    start_node(&expression_statement);
    expression_statement.expression()->accept_visitor(*this);
    semicolon();
    end_node(&expression_statement);
}

void CSharpOutputVisitor::visit_fixed_statement(FixedStatement& fixed_statement) {
    start_node(&fixed_statement);
    write_keyword(FixedStatement::fixed_keyword);
    space(policy_.space_before_using_parentheses);
    l_par();
    space(policy_.spaces_within_using_parentheses);
    fixed_statement.type()->accept_visitor(*this);
    space();
    write_comma_separated_list(nodes_of(fixed_statement.variables()));
    space(policy_.spaces_within_using_parentheses);
    r_par();
    write_embedded_statement(fixed_statement.embedded_statement());
    end_node(&fixed_statement);
}

void CSharpOutputVisitor::visit_foreach_statement(ForeachStatement& foreach_statement) {
    start_node(&foreach_statement);
    if (foreach_statement.is_async()) write_keyword(ForeachStatement::await_keyword);
    write_keyword(ForeachStatement::foreach_keyword);
    space(policy_.space_before_foreach_parentheses);
    l_par();
    space(policy_.spaces_within_foreach_parentheses);
    foreach_statement.variable_type()->accept_visitor(*this);
    space();
    foreach_statement.variable_designation()->accept_visitor(*this);
    space();
    write_keyword(ForeachStatement::in_keyword);
    space();
    foreach_statement.in_expression()->accept_visitor(*this);
    space(policy_.spaces_within_foreach_parentheses);
    r_par();
    write_embedded_statement(foreach_statement.embedded_statement());
    end_node(&foreach_statement);
}

void CSharpOutputVisitor::visit_for_statement(ForStatement& for_statement) {
    start_node(&for_statement);
    write_keyword(ForStatement::for_keyword);
    space(policy_.space_before_for_parentheses);
    l_par();
    space(policy_.spaces_within_for_parentheses);

    write_comma_separated_list(nodes_of(for_statement.initializers()));
    space(policy_.space_before_for_semicolon);
    write_token(Tokens::semicolon);
    space(policy_.space_after_for_semicolon);

    accept(for_statement.condition());
    space(policy_.space_before_for_semicolon);
    write_token(Tokens::semicolon);
    if (!for_statement.iterators().empty()) {
        space(policy_.space_after_for_semicolon);
        write_comma_separated_list(nodes_of(for_statement.iterators()));
    }

    space(policy_.spaces_within_for_parentheses);
    r_par();
    write_embedded_statement(for_statement.embedded_statement());
    end_node(&for_statement);
}

void CSharpOutputVisitor::visit_goto_case_statement(GotoCaseStatement& goto_case_statement) {
    start_node(&goto_case_statement);
    write_keyword(GotoCaseStatement::goto_keyword);
    write_keyword(GotoCaseStatement::case_keyword);
    space();
    goto_case_statement.label_expression()->accept_visitor(*this);
    semicolon();
    end_node(&goto_case_statement);
}

void CSharpOutputVisitor::visit_goto_default_statement(GotoDefaultStatement& goto_default_statement) {
    start_node(&goto_default_statement);
    write_keyword(GotoDefaultStatement::goto_keyword);
    write_keyword(GotoDefaultStatement::default_keyword);
    semicolon();
    end_node(&goto_default_statement);
}

void CSharpOutputVisitor::visit_goto_statement(GotoStatement& goto_statement) {
    start_node(&goto_statement);
    write_keyword(GotoStatement::goto_keyword);
    write_identifier(goto_statement.label_token());
    semicolon();
    end_node(&goto_statement);
}

void CSharpOutputVisitor::visit_if_else_statement(IfElseStatement& if_else_statement) {
    start_node(&if_else_statement);
    write_keyword(IfElseStatement::if_keyword);
    space(policy_.space_before_if_parentheses);
    l_par();
    space(policy_.spaces_within_if_parentheses);
    if_else_statement.condition()->accept_visitor(*this);
    space(policy_.spaces_within_if_parentheses);
    r_par();

    if (if_else_statement.false_statement() == nullptr) {
        write_embedded_statement(if_else_statement.true_statement());
    } else {
        write_embedded_statement(if_else_statement.true_statement(), policy_.else_new_line_placement);
        write_keyword(IfElseStatement::else_keyword);
        if (is<IfElseStatement>(if_else_statement.false_statement())) {
            if_else_statement.false_statement()->accept_visitor(*this);
        } else {
            write_embedded_statement(if_else_statement.false_statement());
        }
    }

    end_node(&if_else_statement);
}

void CSharpOutputVisitor::visit_label_statement(LabelStatement& label_statement) {
    start_node(&label_statement);
    write_identifier(label_statement.get_child(Slots::identifier));
    write_token(Tokens::colon);
    bool found_labelled_statement = false;
    for (AstNode* tmp = label_statement.next_sibling(); tmp != nullptr; tmp = tmp->next_sibling()) {
        if (slot_kind_of(tmp) == slot_kind_of(&label_statement)) {
            found_labelled_statement = true;
            break;
        }
    }
    if (!found_labelled_statement) {
        // introduce an EmptyStatement so that the output becomes syntactically valid
        write_token(Tokens::semicolon);
    }
    new_line();
    end_node(&label_statement);
}

void CSharpOutputVisitor::visit_lock_statement(LockStatement& lock_statement) {
    start_node(&lock_statement);
    write_keyword(LockStatement::lock_keyword);
    space(policy_.space_before_lock_parentheses);
    l_par();
    space(policy_.spaces_within_lock_parentheses);
    lock_statement.expression()->accept_visitor(*this);
    space(policy_.spaces_within_lock_parentheses);
    r_par();
    write_embedded_statement(lock_statement.embedded_statement());
    end_node(&lock_statement);
}

void CSharpOutputVisitor::visit_return_statement(ReturnStatement& return_statement) {
    start_node(&return_statement);
    write_keyword(ReturnStatement::return_keyword);
    if (return_statement.expression() != nullptr) {
        space();
        return_statement.expression()->accept_visitor(*this);
    }
    semicolon();
    end_node(&return_statement);
}

void CSharpOutputVisitor::visit_switch_statement(SwitchStatement& switch_statement) {
    start_node(&switch_statement);
    write_keyword(SwitchStatement::switch_keyword);
    space(policy_.space_before_switch_parentheses);
    l_par();
    space(policy_.spaces_within_switch_parentheses);
    switch_statement.expression()->accept_visitor(*this);
    space(policy_.spaces_within_switch_parentheses);
    r_par();
    open_brace(policy_.statement_brace_style);
    if (!policy_.indent_switch_body) {
        writer_.unindent();
    }

    for (SwitchSection* section : switch_statement.switch_sections()) {
        section->accept_visitor(*this);
    }

    if (!policy_.indent_switch_body) {
        writer_.indent();
    }
    close_brace(policy_.statement_brace_style);
    new_line();
    end_node(&switch_statement);
}

void CSharpOutputVisitor::visit_switch_section(SwitchSection& switch_section) {
    start_node(&switch_section);
    bool first = true;
    for (CaseLabel* label : switch_section.case_labels()) {
        if (!first) {
            new_line();
        }
        label->accept_visitor(*this);
        first = false;
    }
    bool is_block = switch_section.statements().count() == 1 && is<BlockStatement>(switch_section.statements()[0]);
    if (policy_.indent_case_body && !is_block) {
        writer_.indent();
    }

    if (!is_block) new_line();

    for (Statement* statement : switch_section.statements()) {
        statement->accept_visitor(*this);
    }

    if (policy_.indent_case_body && !is_block) {
        writer_.unindent();
    }

    end_node(&switch_section);
}

void CSharpOutputVisitor::visit_case_label(CaseLabel& case_label) {
    start_node(&case_label);
    if (case_label.expression() == nullptr) {
        write_keyword(CaseLabel::default_keyword);
    } else {
        write_keyword(CaseLabel::case_keyword);
        space();
        case_label.expression()->accept_visitor(*this);
    }
    write_token(Tokens::colon);
    end_node(&case_label);
}

void CSharpOutputVisitor::visit_switch_expression(SwitchExpression& switch_expression) {
    start_node(&switch_expression);
    switch_expression.expression()->accept_visitor(*this);
    space();
    write_keyword(SwitchExpression::switch_keyword);
    open_brace(policy_.array_initializer_brace_style);
    for (SwitchExpressionSection* node : switch_expression.switch_sections()) {
        node->accept_visitor(*this);
        comma(node);
        new_line();
    }
    close_brace(policy_.array_initializer_brace_style);
    end_node(&switch_expression);
}

void CSharpOutputVisitor::visit_switch_expression_section(SwitchExpressionSection& switch_expression_section) {
    start_node(&switch_expression_section);
    switch_expression_section.pattern()->accept_visitor(*this);
    space();
    write_token(Tokens::arrow);
    space();
    switch_expression_section.body()->accept_visitor(*this);
    end_node(&switch_expression_section);
}

void CSharpOutputVisitor::visit_throw_statement(ThrowStatement& throw_statement) {
    start_node(&throw_statement);
    write_keyword(ThrowStatement::throw_keyword);
    if (throw_statement.expression() != nullptr) {
        space();
        throw_statement.expression()->accept_visitor(*this);
    }
    semicolon();
    end_node(&throw_statement);
}

void CSharpOutputVisitor::visit_try_catch_statement(TryCatchStatement& try_catch_statement) {
    start_node(&try_catch_statement);
    write_keyword(TryCatchStatement::try_keyword);
    write_block(try_catch_statement.try_block(), policy_.statement_brace_style);
    for (CatchClause* catch_clause : try_catch_statement.catch_clauses()) {
        if (policy_.catch_new_line_placement == NewLinePlacement::SameLine)
            space();
        else
            new_line();
        catch_clause->accept_visitor(*this);
    }
    if (try_catch_statement.finally_block() != nullptr) {
        if (policy_.finally_new_line_placement == NewLinePlacement::SameLine)
            space();
        else
            new_line();
        write_keyword(TryCatchStatement::finally_keyword);
        write_block(try_catch_statement.finally_block(), policy_.statement_brace_style);
    }

    new_line();
    end_node(&try_catch_statement);
}

void CSharpOutputVisitor::visit_catch_clause(CatchClause& catch_clause) {
    start_node(&catch_clause);
    write_keyword(CatchClause::catch_keyword);
    if (catch_clause.type() != nullptr) {
        space(policy_.space_before_catch_parentheses);
        l_par();
        space(policy_.spaces_within_catch_parentheses);
        catch_clause.type()->accept_visitor(*this);
        if (!catch_clause.variable_name().empty()) {
            space();
            write_identifier(catch_clause.variable_name_token());
        }
        space(policy_.spaces_within_catch_parentheses);
        r_par();
    }
    if (catch_clause.condition() != nullptr) {
        space();
        write_keyword(CatchClause::when_keyword);
        space(policy_.space_before_if_parentheses);
        write_token(CatchClause::cond_l_par);
        space(policy_.spaces_within_if_parentheses);
        catch_clause.condition()->accept_visitor(*this);
        space(policy_.spaces_within_if_parentheses);
        write_token(CatchClause::cond_r_par);
    }
    write_block(catch_clause.body(), policy_.statement_brace_style);
    end_node(&catch_clause);
}

void CSharpOutputVisitor::visit_unchecked_statement(UncheckedStatement& unchecked_statement) {
    start_node(&unchecked_statement);
    write_keyword(UncheckedStatement::unchecked_keyword);
    unchecked_statement.body()->accept_visitor(*this);
    end_node(&unchecked_statement);
}

void CSharpOutputVisitor::visit_unsafe_statement(UnsafeStatement& unsafe_statement) {
    start_node(&unsafe_statement);
    write_keyword(UnsafeStatement::unsafe_keyword);
    unsafe_statement.body()->accept_visitor(*this);
    end_node(&unsafe_statement);
}

void CSharpOutputVisitor::visit_using_statement(UsingStatement& using_statement) {
    start_node(&using_statement);
    if (using_statement.is_async()) {
        write_keyword(UsingStatement::await_keyword);
    }
    write_keyword(UsingStatement::using_keyword);
    if (using_statement.is_enhanced()) {
        space();
    } else {
        space(policy_.space_before_using_parentheses);
        l_par();
        space(policy_.spaces_within_using_parentheses);
    }

    using_statement.resource_acquisition()->accept_visitor(*this);

    if (using_statement.is_enhanced()) {
        semicolon();
    } else {
        space(policy_.spaces_within_using_parentheses);
        r_par();
    }

    if (using_statement.is_enhanced()) {
        if (auto* block_statement = as<BlockStatement>(using_statement.embedded_statement())) {
            start_node(block_statement);
            for (Statement* node : *block_statement) {
                node->accept_visitor(*this);
            }
            end_node(block_statement);
        } else {
            using_statement.embedded_statement()->accept_visitor(*this);
        }
    } else {
        write_embedded_statement(using_statement.embedded_statement());
    }
    end_node(&using_statement);
}

void CSharpOutputVisitor::visit_variable_declaration_statement(VariableDeclarationStatement& variable_declaration_statement) {
    start_node(&variable_declaration_statement);
    write_modifiers(variable_declaration_statement.modifiers());
    variable_declaration_statement.type()->accept_visitor(*this);
    space();
    write_comma_separated_list(nodes_of(variable_declaration_statement.variables()));
    semicolon();
    end_node(&variable_declaration_statement);
}

void CSharpOutputVisitor::visit_local_function_declaration_statement(LocalFunctionDeclarationStatement& local_function_declaration_statement) {
    start_node(&local_function_declaration_statement);
    local_function_declaration_statement.declaration()->accept_visitor(*this);
    end_node(&local_function_declaration_statement);
}

void CSharpOutputVisitor::visit_while_statement(WhileStatement& while_statement) {
    start_node(&while_statement);
    write_keyword(WhileStatement::while_keyword);
    space(policy_.space_before_while_parentheses);
    l_par();
    space(policy_.spaces_within_while_parentheses);
    while_statement.condition()->accept_visitor(*this);
    space(policy_.spaces_within_while_parentheses);
    r_par();
    write_embedded_statement(while_statement.embedded_statement());
    end_node(&while_statement);
}

void CSharpOutputVisitor::visit_yield_break_statement(YieldBreakStatement& yield_break_statement) {
    start_node(&yield_break_statement);
    write_keyword(YieldBreakStatement::yield_keyword);
    write_keyword(YieldBreakStatement::break_keyword);
    semicolon();
    end_node(&yield_break_statement);
}

void CSharpOutputVisitor::visit_yield_return_statement(YieldReturnStatement& yield_return_statement) {
    start_node(&yield_return_statement);
    write_keyword(YieldReturnStatement::yield_keyword);
    write_keyword(YieldReturnStatement::return_keyword);
    space();
    yield_return_statement.expression()->accept_visitor(*this);
    semicolon();
    end_node(&yield_return_statement);
}

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
