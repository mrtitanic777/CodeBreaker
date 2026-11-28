// dotnet/decompiler/csharp/output_visitor/csharp_output_visitor_expressions.cpp -- CSharpOutputVisitor: the visits of the expressions and of
// the query expressions. See csharp_output_visitor.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: CSharpOutputVisitor), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;

void CSharpOutputVisitor::visit_anonymous_method_expression(AnonymousMethodExpression& anonymous_method_expression) {
    start_node(&anonymous_method_expression);
    if (anonymous_method_expression.is_async()) {
        write_keyword(AnonymousMethodExpression::async_modifier);
        space();
    }
    write_keyword(AnonymousMethodExpression::delegate_keyword);
    if (!anonymous_method_expression.parameters().empty()) {
        space(policy_.space_before_anonymous_method_parentheses);
        write_comma_separated_list_in_parenthesis(nodes_of(anonymous_method_expression.parameters()),
                                                  policy_.space_within_anonymous_method_parentheses);
    }
    write_block(anonymous_method_expression.body(), policy_.anonymous_method_brace_style);
    end_node(&anonymous_method_expression);
}

void CSharpOutputVisitor::visit_undocumented_expression(UndocumentedExpression& undocumented_expression) {
    start_node(&undocumented_expression);
    switch (undocumented_expression.undocumented_expression_type()) {
        case UndocumentedExpressionType::ArgList:
        case UndocumentedExpressionType::ArgListAccess:
            write_keyword(UndocumentedExpression::arglist_keyword);
            break;
        case UndocumentedExpressionType::MakeRef:
            write_keyword(UndocumentedExpression::makeref_keyword);
            break;
        case UndocumentedExpressionType::RefType:
            write_keyword(UndocumentedExpression::reftype_keyword);
            break;
        case UndocumentedExpressionType::RefValue:
            write_keyword(UndocumentedExpression::refvalue_keyword);
            break;
    }
    if (undocumented_expression.undocumented_expression_type() != UndocumentedExpressionType::ArgListAccess) {
        space(policy_.space_before_method_call_parentheses);
        write_comma_separated_list_in_parenthesis(nodes_of(undocumented_expression.arguments()), policy_.space_within_method_call_parentheses);
    }
    end_node(&undocumented_expression);
}

void CSharpOutputVisitor::visit_array_create_expression(ArrayCreateExpression& array_create_expression) {
    start_node(&array_create_expression);
    write_keyword(ArrayCreateExpression::new_keyword);
    accept(array_create_expression.type());
    if (array_create_expression.arguments().count() > 0) {
        write_comma_separated_list_in_brackets(nodes_of(array_create_expression.arguments()));
    }
    for (ArraySpecifier* specifier : array_create_expression.additional_array_specifiers()) {
        specifier->accept_visitor(*this);
    }
    accept(array_create_expression.initializer());
    end_node(&array_create_expression);
}

void CSharpOutputVisitor::visit_array_initializer_expression(ArrayInitializerExpression& array_initializer_expression) {
    start_node(&array_initializer_expression);
    bool braces_are_optional = array_initializer_expression.elements().count() == 1 &&
                               is_object_or_collection_initializer(array_initializer_expression.parent()) &&
                               !can_be_confused_with_object_initializer(array_initializer_expression.elements()[0]);
    if (braces_are_optional) {
        array_initializer_expression.elements()[0]->accept_visitor(*this);
    } else {
        print_initializer_elements(array_initializer_expression.elements());
    }
    end_node(&array_initializer_expression);
}

namespace {

bool is_simple_expression(Expression* ex) {
    if (is<NullReferenceExpression>(ex) || is<ThisReferenceExpression>(ex) || is<PrimitiveExpression>(ex) || is<IdentifierExpression>(ex)) return true;
    if (auto* mre = as<MemberReferenceExpression>(ex)) {
        Expression* target = mre->target();
        return is<ThisReferenceExpression>(target) || is<IdentifierExpression>(target) || is<BaseReferenceExpression>(target);
    }
    return false;
}

bool is_complex_expression(Expression* ex) {
    return is<AnonymousMethodExpression>(ex) || is<LambdaExpression>(ex) || is<AnonymousTypeCreateExpression>(ex) || is<ObjectCreateExpression>(ex) ||
           is<NamedExpression>(ex);
}

}  // namespace

void CSharpOutputVisitor::print_initializer_elements(AstNodeCollection<Expression>& elements) {
    bool any_not_simple = false;
    bool any_complex = false;
    for (Expression* e : elements) {
        if (!is_simple_expression(e)) any_not_simple = true;
        if (is_complex_expression(e)) any_complex = true;
    }
    bool wrap_always = policy_.array_initializer_wrapping == Wrapping::WrapAlways || (elements.count() > 1 && any_not_simple) || any_complex;
    bool wrap = wrap_always || elements.count() > 10;
    open_brace(wrap ? policy_.array_initializer_brace_style : BraceStyle::EndOfLine, wrap);
    if (!wrap) space();
    int idx = 0;
    for (Expression* node : elements) {
        if (idx > 0) {
            comma(node, true);
            if (wrap_always || idx % 10 == 0)
                new_line();
            else
                space();
        }
        node->accept_visitor(*this);
        idx++;
    }
    if (wrap)
        new_line();
    else
        space();
    close_brace(wrap ? policy_.array_initializer_brace_style : BraceStyle::EndOfLine, wrap);
}

void CSharpOutputVisitor::visit_as_expression(AsExpression& as_expression) {
    start_node(&as_expression);
    as_expression.expression()->accept_visitor(*this);
    space();
    write_keyword(AsExpression::as_keyword);
    space();
    as_expression.type()->accept_visitor(*this);
    end_node(&as_expression);
}

void CSharpOutputVisitor::visit_assignment_expression(AssignmentExpression& assignment_expression) {
    start_node(&assignment_expression);
    assignment_expression.left()->accept_visitor(*this);
    space(policy_.space_around_assignment);
    write_token(AssignmentExpression::get_operator_token(assignment_expression.operator_()));
    space(policy_.space_around_assignment);
    assignment_expression.right()->accept_visitor(*this);
    end_node(&assignment_expression);
}

void CSharpOutputVisitor::visit_base_reference_expression(BaseReferenceExpression& base_reference_expression) {
    start_node(&base_reference_expression);
    write_keyword("base");
    end_node(&base_reference_expression);
}

void CSharpOutputVisitor::visit_binary_operator_expression(BinaryOperatorExpression& binary_operator_expression) {
    start_node(&binary_operator_expression);
    accept(binary_operator_expression.left());
    bool space_policy;
    switch (binary_operator_expression.operator_()) {
        case BinaryOperatorType::BitwiseAnd:
        case BinaryOperatorType::BitwiseOr:
        case BinaryOperatorType::ExclusiveOr:
            space_policy = policy_.space_around_bitwise_operator;
            break;
        case BinaryOperatorType::ConditionalAnd:
        case BinaryOperatorType::ConditionalOr:
            space_policy = policy_.space_around_logical_operator;
            break;
        case BinaryOperatorType::GreaterThan:
        case BinaryOperatorType::GreaterThanOrEqual:
        case BinaryOperatorType::LessThanOrEqual:
        case BinaryOperatorType::LessThan:
            space_policy = policy_.space_around_relational_operator;
            break;
        case BinaryOperatorType::Equality:
        case BinaryOperatorType::InEquality:
            space_policy = policy_.space_around_equality_operator;
            break;
        case BinaryOperatorType::Add:
        case BinaryOperatorType::Subtract:
            space_policy = policy_.space_around_additive_operator;
            break;
        case BinaryOperatorType::Multiply:
        case BinaryOperatorType::Divide:
        case BinaryOperatorType::Modulus:
            space_policy = policy_.space_around_multiplicative_operator;
            break;
        case BinaryOperatorType::ShiftLeft:
        case BinaryOperatorType::ShiftRight:
        case BinaryOperatorType::UnsignedShiftRight:
            space_policy = policy_.space_around_shift_operator;
            break;
        case BinaryOperatorType::NullCoalescing:
        case BinaryOperatorType::IsPattern:
        case BinaryOperatorType::PatternOr:
            space_policy = true;
            break;
        case BinaryOperatorType::Range:
            space_policy = false;
            break;
        default:
            throw std::logic_error("Invalid value for BinaryOperatorType");
    }
    space(space_policy);
    std::string_view operator_token = BinaryOperatorExpression::get_operator_token(binary_operator_expression.operator_());
    if (operator_token == BinaryOperatorExpression::is_keyword || operator_token == BinaryOperatorExpression::pattern_or_keyword) {
        write_keyword(operator_token);
    } else {
        write_token(operator_token);
    }
    space(space_policy);
    accept(binary_operator_expression.right());
    end_node(&binary_operator_expression);
}

void CSharpOutputVisitor::visit_cast_expression(CastExpression& cast_expression) {
    start_node(&cast_expression);
    l_par();
    space(policy_.spaces_within_cast_parentheses);
    cast_expression.type()->accept_visitor(*this);
    space(policy_.spaces_within_cast_parentheses);
    r_par();
    space(policy_.space_after_typecast);
    cast_expression.expression()->accept_visitor(*this);
    end_node(&cast_expression);
}

void CSharpOutputVisitor::visit_checked_expression(CheckedExpression& checked_expression) {
    start_node(&checked_expression);
    write_keyword(CheckedExpression::checked_keyword);
    l_par();
    space(policy_.spaces_within_checked_expression_parantheses);
    checked_expression.expression()->accept_visitor(*this);
    space(policy_.spaces_within_checked_expression_parantheses);
    r_par();
    end_node(&checked_expression);
}

void CSharpOutputVisitor::visit_conditional_expression(ConditionalExpression& conditional_expression) {
    start_node(&conditional_expression);
    conditional_expression.condition()->accept_visitor(*this);

    space(policy_.space_before_conditional_operator_condition);
    write_token(ConditionalExpression::question_mark_token);
    space(policy_.space_after_conditional_operator_condition);

    conditional_expression.true_expression()->accept_visitor(*this);

    space(policy_.space_before_conditional_operator_separator);
    write_token(ConditionalExpression::colon_token);
    space(policy_.space_after_conditional_operator_separator);
    conditional_expression.false_expression()->accept_visitor(*this);

    end_node(&conditional_expression);
}

void CSharpOutputVisitor::visit_default_value_expression(DefaultValueExpression& default_value_expression) {
    start_node(&default_value_expression);

    write_keyword(DefaultValueExpression::default_keyword);
    l_par();
    space(policy_.spaces_within_type_of_parentheses);
    default_value_expression.type()->accept_visitor(*this);
    space(policy_.spaces_within_type_of_parentheses);
    r_par();

    end_node(&default_value_expression);
}

void CSharpOutputVisitor::visit_direction_expression(DirectionExpression& direction_expression) {
    start_node(&direction_expression);

    switch (direction_expression.field_direction()) {
        case FieldDirection::Out:
            write_keyword(DirectionExpression::out_keyword);
            break;
        case FieldDirection::Ref:
            write_keyword(DirectionExpression::ref_keyword);
            break;
        case FieldDirection::In:
            write_keyword(DirectionExpression::in_keyword);
            break;
        default:
            throw std::logic_error("Invalid value for FieldDirection");
    }
    space();
    direction_expression.expression()->accept_visitor(*this);

    end_node(&direction_expression);
}

void CSharpOutputVisitor::visit_declaration_expression(DeclarationExpression& declaration_expression) {
    start_node(&declaration_expression);

    declaration_expression.type()->accept_visitor(*this);
    space();
    declaration_expression.designation()->accept_visitor(*this);

    end_node(&declaration_expression);
}

void CSharpOutputVisitor::visit_recursive_pattern_expression(RecursivePatternExpression& recursive_pattern_expression) {
    start_node(&recursive_pattern_expression);

    accept(recursive_pattern_expression.type());
    space();
    if (recursive_pattern_expression.is_positional()) {
        write_token(Tokens::l_par);
    } else {
        write_token(Tokens::l_brace);
    }
    space();
    write_comma_separated_list(nodes_of(recursive_pattern_expression.sub_patterns()));
    space();
    if (recursive_pattern_expression.is_positional()) {
        write_token(Tokens::r_par);
    } else {
        write_token(Tokens::r_brace);
    }
    if (recursive_pattern_expression.designation() != nullptr) {
        space();
        recursive_pattern_expression.designation()->accept_visitor(*this);
    }

    end_node(&recursive_pattern_expression);
}

void CSharpOutputVisitor::visit_out_var_declaration_expression(OutVarDeclarationExpression& out_var_declaration_expression) {
    start_node(&out_var_declaration_expression);

    write_keyword(OutVarDeclarationExpression::out_keyword);
    space();
    out_var_declaration_expression.type()->accept_visitor(*this);
    space();
    out_var_declaration_expression.variable()->accept_visitor(*this);

    end_node(&out_var_declaration_expression);
}

void CSharpOutputVisitor::visit_identifier_expression(IdentifierExpression& identifier_expression) {
    start_node(&identifier_expression);
    write_identifier(identifier_expression.identifier_token());
    write_type_arguments(nodes_of(identifier_expression.type_arguments()));
    end_node(&identifier_expression);
}

void CSharpOutputVisitor::visit_indexer_expression(IndexerExpression& indexer_expression) {
    start_node(&indexer_expression);
    accept(indexer_expression.target());
    space(policy_.space_before_method_call_parentheses);
    write_comma_separated_list_in_brackets(nodes_of(indexer_expression.arguments()));
    end_node(&indexer_expression);
}

void CSharpOutputVisitor::visit_invocation_expression(InvocationExpression& invocation_expression) {
    start_node(&invocation_expression);
    invocation_expression.target()->accept_visitor(*this);
    space(policy_.space_before_method_call_parentheses);
    write_comma_separated_list_in_parenthesis(nodes_of(invocation_expression.arguments()), policy_.space_within_method_call_parentheses);
    if (!is<MemberReferenceExpression>(invocation_expression.parent())) {
        if (auto* mre = as<MemberReferenceExpression>(invocation_expression.target())) {
            if (should_insert_new_line_when_in_method_call_chain(mre) >= 3) writer_.unindent();
        }
    }
    end_node(&invocation_expression);
}

void CSharpOutputVisitor::visit_is_expression(IsExpression& is_expression) {
    start_node(&is_expression);
    is_expression.expression()->accept_visitor(*this);
    space();
    write_keyword(IsExpression::is_keyword);
    is_expression.type()->accept_visitor(*this);
    end_node(&is_expression);
}

void CSharpOutputVisitor::visit_lambda_expression(LambdaExpression& lambda_expression) {
    start_node(&lambda_expression);
    write_attributes(nodes_of(lambda_expression.attributes()));
    if (lambda_expression.is_async()) {
        write_keyword(LambdaExpression::async_modifier);
        space();
    }
    if (lambda_needs_parenthesis(&lambda_expression)) {
        write_comma_separated_list_in_parenthesis(nodes_of(lambda_expression.parameters()), policy_.space_within_method_declaration_parentheses);
    } else {
        single(lambda_expression.parameters())->accept_visitor(*this);
    }
    space();
    write_token(Tokens::arrow);
    if (auto* block = as<BlockStatement>(lambda_expression.body())) {
        write_block(block, policy_.anonymous_method_brace_style);
    } else {
        space();
        lambda_expression.body()->accept_visitor(*this);
    }
    end_node(&lambda_expression);
}

bool CSharpOutputVisitor::lambda_needs_parenthesis(LambdaExpression* lambda_expression) {
    if (lambda_expression->parameters().count() != 1) {
        return true;
    }
    ParameterDeclaration* p = single(lambda_expression->parameters());
    return !(p->type() == nullptr && p->parameter_modifier() == typesystem::ReferenceKind::None && !p->is_params());
}

void CSharpOutputVisitor::visit_member_reference_expression(MemberReferenceExpression& member_reference_expression) {
    start_node(&member_reference_expression);
    member_reference_expression.target()->accept_visitor(*this);
    bool inserted_new_line = insert_new_line_when_in_method_call_chain(&member_reference_expression);
    write_token(Tokens::dot);
    write_identifier(member_reference_expression.member_name_token());
    write_type_arguments(nodes_of(member_reference_expression.type_arguments()));
    if (inserted_new_line && !is<InvocationExpression>(member_reference_expression.parent())) {
        writer_.unindent();
    }
    end_node(&member_reference_expression);
}

void CSharpOutputVisitor::visit_named_argument_expression(NamedArgumentExpression& named_argument_expression) {
    start_node(&named_argument_expression);
    write_identifier(named_argument_expression.name_token());
    write_token(Tokens::colon);
    space();
    named_argument_expression.expression()->accept_visitor(*this);
    end_node(&named_argument_expression);
}

void CSharpOutputVisitor::visit_named_expression(NamedExpression& named_expression) {
    start_node(&named_expression);
    write_identifier(named_expression.name_token());
    space();
    write_token(Tokens::assign);
    space();
    named_expression.expression()->accept_visitor(*this);
    end_node(&named_expression);
}

void CSharpOutputVisitor::visit_null_reference_expression(NullReferenceExpression& null_reference_expression) {
    start_node(&null_reference_expression);
    writer_.write_primitive_value(typesystem::ConstantValue());
    is_after_space_ = false;
    end_node(&null_reference_expression);
}

void CSharpOutputVisitor::visit_object_create_expression(ObjectCreateExpression& object_create_expression) {
    start_node(&object_create_expression);
    write_keyword(ObjectCreateExpression::new_keyword);
    object_create_expression.type()->accept_visitor(*this);
    // also use parenthesis if there is an '(' token and this isn't an anonymous type
    bool use_parenthesis = !object_create_expression.arguments().empty() || object_create_expression.initializer() == nullptr;
    if (use_parenthesis) {
        space(policy_.space_before_method_call_parentheses);
        write_comma_separated_list_in_parenthesis(nodes_of(object_create_expression.arguments()), policy_.space_within_method_call_parentheses);
    }
    accept(object_create_expression.initializer());
    end_node(&object_create_expression);
}

void CSharpOutputVisitor::visit_anonymous_type_create_expression(AnonymousTypeCreateExpression& anonymous_type_create_expression) {
    start_node(&anonymous_type_create_expression);
    write_keyword(AnonymousTypeCreateExpression::new_keyword);
    print_initializer_elements(anonymous_type_create_expression.initializers());
    end_node(&anonymous_type_create_expression);
}

void CSharpOutputVisitor::visit_parenthesized_expression(ParenthesizedExpression& parenthesized_expression) {
    start_node(&parenthesized_expression);
    l_par();
    space(policy_.spaces_within_parentheses);
    parenthesized_expression.expression()->accept_visitor(*this);
    space(policy_.spaces_within_parentheses);
    r_par();
    end_node(&parenthesized_expression);
}

void CSharpOutputVisitor::visit_pointer_reference_expression(PointerReferenceExpression& pointer_reference_expression) {
    start_node(&pointer_reference_expression);
    pointer_reference_expression.target()->accept_visitor(*this);
    write_token(PointerReferenceExpression::arrow_token);
    write_identifier(pointer_reference_expression.member_name_token());
    write_type_arguments(nodes_of(pointer_reference_expression.type_arguments()));
    end_node(&pointer_reference_expression);
}

void CSharpOutputVisitor::visit_primitive_expression(PrimitiveExpression& primitive_expression) {
    start_node(&primitive_expression);
    writer_.write_primitive_value(primitive_expression.value(), primitive_expression.format());
    is_after_space_ = false;
    end_node(&primitive_expression);
}

void CSharpOutputVisitor::visit_interpolated_string_expression(InterpolatedStringExpression& interpolated_string_expression) {
    start_node(&interpolated_string_expression);

    writer_.write_token("$\"");
    for (InterpolatedStringContent* element : interpolated_string_expression.content()) {
        element->accept_visitor(*this);
    }
    writer_.write_token("\"");
    is_after_space_ = false;

    end_node(&interpolated_string_expression);
}

void CSharpOutputVisitor::visit_interpolation(Interpolation& interpolation) {
    start_node(&interpolation);

    writer_.write_token("{");
    interpolation.expression()->accept_visitor(*this);
    if (interpolation.alignment() != 0) {
        writer_.write_token(",");
        writer_.write_primitive_value(typesystem::ConstantValue(static_cast<std::int32_t>(interpolation.alignment())));
    }
    if (interpolation.suffix().has_value()) {
        writer_.write_token(":");
        writer_.write_interpolated_text(*interpolation.suffix());
    }
    writer_.write_token("}");

    end_node(&interpolation);
}

void CSharpOutputVisitor::visit_interpolated_string_text(InterpolatedStringText& interpolated_string_text) {
    start_node(&interpolated_string_text);
    writer_.write_interpolated_text(interpolated_string_text.text());
    end_node(&interpolated_string_text);
}

void CSharpOutputVisitor::visit_size_of_expression(SizeOfExpression& size_of_expression) {
    start_node(&size_of_expression);

    write_keyword(SizeOfExpression::sizeof_keyword);
    l_par();
    space(policy_.spaces_within_size_of_parentheses);
    size_of_expression.type()->accept_visitor(*this);
    space(policy_.spaces_within_size_of_parentheses);
    r_par();

    end_node(&size_of_expression);
}

void CSharpOutputVisitor::visit_stack_alloc_expression(StackAllocExpression& stack_alloc_expression) {
    start_node(&stack_alloc_expression);
    write_keyword(StackAllocExpression::stackalloc_keyword);
    accept(stack_alloc_expression.type());
    std::vector<AstNode*> count;
    if (stack_alloc_expression.count_expression() != nullptr) count.push_back(stack_alloc_expression.count_expression());
    write_comma_separated_list_in_brackets(count);
    accept(stack_alloc_expression.initializer());
    end_node(&stack_alloc_expression);
}

void CSharpOutputVisitor::visit_this_reference_expression(ThisReferenceExpression& this_reference_expression) {
    start_node(&this_reference_expression);
    write_keyword("this");
    end_node(&this_reference_expression);
}

void CSharpOutputVisitor::visit_throw_expression(ThrowExpression& throw_expression) {
    start_node(&throw_expression);
    write_keyword(ThrowExpression::throw_keyword);
    space();
    throw_expression.expression()->accept_visitor(*this);
    end_node(&throw_expression);
}

void CSharpOutputVisitor::visit_tuple_expression(TupleExpression& tuple_expression) {
    assert(tuple_expression.elements().count() >= 2);
    start_node(&tuple_expression);
    l_par();
    write_comma_separated_list(nodes_of(tuple_expression.elements()));
    r_par();
    end_node(&tuple_expression);
}

void CSharpOutputVisitor::visit_type_of_expression(TypeOfExpression& type_of_expression) {
    start_node(&type_of_expression);

    write_keyword(TypeOfExpression::typeof_keyword);
    l_par();
    space(policy_.spaces_within_type_of_parentheses);
    type_of_expression.type()->accept_visitor(*this);
    space(policy_.spaces_within_type_of_parentheses);
    r_par();

    end_node(&type_of_expression);
}

void CSharpOutputVisitor::visit_type_reference_expression(TypeReferenceExpression& type_reference_expression) {
    start_node(&type_reference_expression);
    type_reference_expression.type()->accept_visitor(*this);
    end_node(&type_reference_expression);
}

void CSharpOutputVisitor::visit_unary_operator_expression(UnaryOperatorExpression& unary_operator_expression) {
    start_node(&unary_operator_expression);
    UnaryOperatorType op_type = unary_operator_expression.operator_();
    std::optional<std::string_view> op_symbol = UnaryOperatorExpression::get_operator_token(op_type);
    if (op_type == UnaryOperatorType::Await || op_type == UnaryOperatorType::PatternNot) {
        write_keyword(*op_symbol);
        space();
    } else if (!is_postfix_operator(op_type) && op_symbol) {
        write_token(*op_symbol);
    }
    unary_operator_expression.expression()->accept_visitor(*this);
    if (is_postfix_operator(op_type)) {
        write_token(*op_symbol);
    }
    end_node(&unary_operator_expression);
}

bool CSharpOutputVisitor::is_postfix_operator(UnaryOperatorType op) {
    return op == UnaryOperatorType::PostIncrement || op == UnaryOperatorType::PostDecrement || op == UnaryOperatorType::NullConditional ||
           op == UnaryOperatorType::SuppressNullableWarning;
}

void CSharpOutputVisitor::visit_unchecked_expression(UncheckedExpression& unchecked_expression) {
    start_node(&unchecked_expression);
    write_keyword(UncheckedExpression::unchecked_keyword);
    l_par();
    space(policy_.spaces_within_checked_expression_parantheses);
    unchecked_expression.expression()->accept_visitor(*this);
    space(policy_.spaces_within_checked_expression_parantheses);
    r_par();
    end_node(&unchecked_expression);
}

void CSharpOutputVisitor::visit_with_initializer_expression(WithInitializerExpression& with_initializer_expression) {
    start_node(&with_initializer_expression);
    with_initializer_expression.expression()->accept_visitor(*this);
    write_keyword("with");
    with_initializer_expression.initializer()->accept_visitor(*this);
    end_node(&with_initializer_expression);
}

// ---- Query Expressions --------------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::visit_query_expression(QueryExpression& query_expression) {
    start_node(&query_expression);
    if (slot_kind_of(&query_expression) != &Slots::preceding_query) writer_.indent();
    bool first = true;
    for (QueryClause* clause : query_expression.clauses()) {
        if (first) {
            first = false;
        } else {
            if (!is<QueryContinuationClause>(clause)) {
                new_line();
            }
        }
        clause->accept_visitor(*this);
    }
    if (slot_kind_of(&query_expression) != &Slots::preceding_query) writer_.unindent();
    end_node(&query_expression);
}

void CSharpOutputVisitor::visit_query_continuation_clause(QueryContinuationClause& query_continuation_clause) {
    start_node(&query_continuation_clause);
    query_continuation_clause.preceding_query()->accept_visitor(*this);
    space();
    write_keyword(QueryContinuationClause::into_keyword);
    space();
    write_identifier(query_continuation_clause.identifier_token());
    end_node(&query_continuation_clause);
}

void CSharpOutputVisitor::visit_query_from_clause(QueryFromClause& query_from_clause) {
    start_node(&query_from_clause);
    write_keyword(QueryFromClause::from_keyword);
    space();
    accept(query_from_clause.type());
    space();
    write_identifier(query_from_clause.identifier_token());
    space();
    write_keyword(QueryFromClause::in_keyword);
    space();
    query_from_clause.expression()->accept_visitor(*this);
    end_node(&query_from_clause);
}

void CSharpOutputVisitor::visit_query_let_clause(QueryLetClause& query_let_clause) {
    start_node(&query_let_clause);
    write_keyword(QueryLetClause::let_keyword);
    space();
    write_identifier(query_let_clause.identifier_token());
    space(policy_.space_around_assignment);
    write_token(Tokens::assign);
    space(policy_.space_around_assignment);
    query_let_clause.expression()->accept_visitor(*this);
    end_node(&query_let_clause);
}

void CSharpOutputVisitor::visit_query_where_clause(QueryWhereClause& query_where_clause) {
    start_node(&query_where_clause);
    write_keyword(QueryWhereClause::where_keyword);
    space();
    query_where_clause.condition()->accept_visitor(*this);
    end_node(&query_where_clause);
}

void CSharpOutputVisitor::visit_query_join_clause(QueryJoinClause& query_join_clause) {
    start_node(&query_join_clause);
    write_keyword(QueryJoinClause::join_keyword);
    accept(query_join_clause.type());
    space();
    write_identifier(query_join_clause.join_identifier_token());
    space();
    write_keyword(QueryJoinClause::in_keyword);
    space();
    query_join_clause.in_expression()->accept_visitor(*this);
    space();
    write_keyword(QueryJoinClause::on_keyword);
    space();
    query_join_clause.on_expression()->accept_visitor(*this);
    space();
    write_keyword(QueryJoinClause::equals_keyword);
    space();
    query_join_clause.equals_expression()->accept_visitor(*this);
    if (query_join_clause.is_group_join()) {
        space();
        write_keyword(QueryJoinClause::into_keyword);
        write_identifier(query_join_clause.into_identifier_token());
    }
    end_node(&query_join_clause);
}

void CSharpOutputVisitor::visit_query_order_clause(QueryOrderClause& query_order_clause) {
    start_node(&query_order_clause);
    write_keyword(QueryOrderClause::orderby_keyword);
    space();
    write_comma_separated_list(nodes_of(query_order_clause.orderings()));
    end_node(&query_order_clause);
}

void CSharpOutputVisitor::visit_query_ordering(QueryOrdering& query_ordering) {
    start_node(&query_ordering);
    query_ordering.expression()->accept_visitor(*this);
    switch (query_ordering.direction()) {
        case QueryOrderingDirection::Ascending:
            space();
            write_keyword(QueryOrdering::ascending_keyword);
            break;
        case QueryOrderingDirection::Descending:
            space();
            write_keyword(QueryOrdering::descending_keyword);
            break;
        default:
            break;
    }
    end_node(&query_ordering);
}

void CSharpOutputVisitor::visit_query_select_clause(QuerySelectClause& query_select_clause) {
    start_node(&query_select_clause);
    write_keyword(QuerySelectClause::select_keyword);
    space();
    query_select_clause.expression()->accept_visitor(*this);
    end_node(&query_select_clause);
}

void CSharpOutputVisitor::visit_query_group_clause(QueryGroupClause& query_group_clause) {
    start_node(&query_group_clause);
    write_keyword(QueryGroupClause::group_keyword);
    space();
    query_group_clause.projection()->accept_visitor(*this);
    space();
    write_keyword(QueryGroupClause::by_keyword);
    space();
    query_group_clause.key()->accept_visitor(*this);
    end_node(&query_group_clause);
}

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
