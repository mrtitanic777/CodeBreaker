// dotnet/decompiler/csharp/output_visitor/formatting_options_factory.cpp -- the predefined formatting styles; see formatting_options_factory.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: FormattingOptionsFactory), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/formatting_options_factory.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

CSharpFormattingOptions FormattingOptionsFactory::create_empty() {

    return CSharpFormattingOptions();
}

CSharpFormattingOptions FormattingOptionsFactory::create_mono() {

    CSharpFormattingOptions o;
    o.indent_namespace_body = true;
    o.indent_class_body = true;
    o.indent_interface_body = true;
    o.indent_struct_body = true;
    o.indent_enum_body = true;
    o.indent_method_body = true;
    o.indent_property_body = true;
    o.indent_event_body = true;
    o.indent_blocks = true;
    o.indent_switch_body = false;
    o.indent_case_body = true;
    o.indent_break_statements = true;
    o.indent_preprocessor_directives = true;
    o.indent_blocks_inside_expressions = false;
    o.namespace_brace_style = BraceStyle::NextLine;
    o.class_brace_style = BraceStyle::NextLine;
    o.interface_brace_style = BraceStyle::NextLine;
    o.struct_brace_style = BraceStyle::NextLine;
    o.enum_brace_style = BraceStyle::NextLine;
    o.method_brace_style = BraceStyle::NextLine;
    o.constructor_brace_style = BraceStyle::NextLine;
    o.destructor_brace_style = BraceStyle::NextLine;
    o.anonymous_method_brace_style = BraceStyle::EndOfLine;

    o.property_brace_style = BraceStyle::EndOfLine;
    o.property_get_brace_style = BraceStyle::EndOfLine;
    o.property_set_brace_style = BraceStyle::EndOfLine;
    o.simple_get_block_formatting = PropertyFormatting::SingleLine;
    o.simple_set_block_formatting = PropertyFormatting::SingleLine;

    o.event_brace_style = BraceStyle::EndOfLine;
    o.event_add_brace_style = BraceStyle::EndOfLine;
    o.event_remove_brace_style = BraceStyle::EndOfLine;
    o.allow_event_add_block_inline = true;
    o.allow_event_remove_block_inline = true;
    o.statement_brace_style = BraceStyle::EndOfLine;

    o.else_new_line_placement = NewLinePlacement::SameLine;
    o.else_if_new_line_placement = NewLinePlacement::SameLine;
    o.catch_new_line_placement = NewLinePlacement::SameLine;
    o.finally_new_line_placement = NewLinePlacement::SameLine;
    o.while_new_line_placement = NewLinePlacement::SameLine;
    o.array_initializer_wrapping = Wrapping::WrapIfTooLong;
    o.array_initializer_brace_style = BraceStyle::EndOfLine;
    o.allow_one_lined_array_initialziers = true;

    o.space_before_method_call_parentheses = true;
    o.space_before_method_declaration_parentheses = true;
    o.space_before_constructor_declaration_parentheses = true;
    o.space_before_delegate_declaration_parentheses = true;
    o.space_after_method_call_parameter_comma = true;
    o.space_after_constructor_declaration_parameter_comma = true;

    o.space_before_new_parentheses = true;
    o.spaces_within_new_parentheses = false;
    o.spaces_between_empty_new_parentheses = false;
    o.space_before_new_parameter_comma = false;
    o.space_after_new_parameter_comma = true;

    o.space_before_if_parentheses = true;
    o.space_before_while_parentheses = true;
    o.space_before_for_parentheses = true;
    o.space_before_foreach_parentheses = true;
    o.space_before_catch_parentheses = true;
    o.space_before_switch_parentheses = true;
    o.space_before_lock_parentheses = true;
    o.space_before_using_parentheses = true;
    o.space_around_assignment = true;
    o.space_around_logical_operator = true;
    o.space_around_equality_operator = true;
    o.space_around_relational_operator = true;
    o.space_around_bitwise_operator = true;
    o.space_around_additive_operator = true;
    o.space_around_multiplicative_operator = true;
    o.space_around_shift_operator = true;
    o.space_around_null_coalescing_operator = true;
    o.spaces_within_parentheses = false;
    o.space_within_method_call_parentheses = false;
    o.space_within_method_declaration_parentheses = false;
    o.spaces_within_if_parentheses = false;
    o.spaces_within_while_parentheses = false;
    o.spaces_within_for_parentheses = false;
    o.spaces_within_foreach_parentheses = false;
    o.spaces_within_catch_parentheses = false;
    o.spaces_within_switch_parentheses = false;
    o.spaces_within_lock_parentheses = false;
    o.spaces_within_using_parentheses = false;
    o.spaces_within_cast_parentheses = false;
    o.spaces_within_size_of_parentheses = false;
    o.spaces_within_type_of_parentheses = false;
    o.spaces_within_checked_expression_parantheses = false;
    o.space_before_conditional_operator_condition = true;
    o.space_after_conditional_operator_condition = true;
    o.space_before_conditional_operator_separator = true;
    o.space_after_conditional_operator_separator = true;

    o.spaces_within_brackets = false;
    o.spaces_before_brackets = true;
    o.space_before_bracket_comma = false;
    o.space_after_bracket_comma = true;

    o.space_before_for_semicolon = false;
    o.space_after_for_semicolon = true;
    o.space_after_typecast = false;

    o.align_embedded_statements = true;
    o.simple_property_formatting = PropertyFormatting::SingleLine;
    o.auto_property_formatting = PropertyFormatting::SingleLine;
    o.empty_line_formatting = EmptyLineFormatting::DoNotIndent;
    o.space_before_method_declaration_parameter_comma = false;
    o.space_after_method_declaration_parameter_comma = true;
    o.space_after_delegate_declaration_parameter_comma = true;
    o.space_before_field_declaration_comma = false;
    o.space_after_field_declaration_comma = true;
    o.space_before_local_variable_declaration_comma = false;
    o.space_after_local_variable_declaration_comma = true;

    o.space_before_indexer_declaration_bracket = true;
    o.space_within_indexer_declaration_bracket = false;
    o.space_before_indexer_declaration_parameter_comma = false;
    o.space_in_named_argument_after_double_colon = true;
    o.remove_end_of_line_white_space = true;

    o.space_after_indexer_declaration_parameter_comma = true;

    o.minimum_blank_lines_before_usings = 0;
    o.minimum_blank_lines_after_usings = 1;
    o.using_placement = UsingPlacement::TopOfFile;

    o.minimum_blank_lines_before_first_declaration = 0;
    o.minimum_blank_lines_between_types = 1;
    o.minimum_blank_lines_between_fields = 0;
    o.minimum_blank_lines_between_event_fields = 0;
    o.minimum_blank_lines_between_members = 1;
    o.minimum_blank_lines_around_region = 1;
    o.minimum_blank_lines_inside_region = 1;
    o.align_to_first_indexer_argument = false;
    o.align_to_first_indexer_declaration_parameter = true;
    o.align_to_first_method_call_argument = false;
    o.align_to_first_method_declaration_parameter = true;
    o.keep_comments_at_first_column = true;
    o.chained_method_call_wrapping = Wrapping::DoNotWrap;
    o.method_call_argument_wrapping = Wrapping::DoNotWrap;
    o.new_line_afer_method_call_open_parentheses = NewLinePlacement::DoNotCare;
    o.method_call_closing_parentheses_on_new_line = NewLinePlacement::DoNotCare;

    o.indexer_argument_wrapping = Wrapping::DoNotWrap;
    o.new_line_afer_indexer_open_bracket = NewLinePlacement::DoNotCare;
    o.indexer_closing_bracket_on_new_line = NewLinePlacement::DoNotCare;

    o.new_line_before_new_query_clause = NewLinePlacement::NewLine;
    return o;
}

CSharpFormattingOptions FormattingOptionsFactory::create_sharp_develop() {

    CSharpFormattingOptions base_options = create_kr_style();
    return base_options;
}

CSharpFormattingOptions FormattingOptionsFactory::create_kr_style() {

    CSharpFormattingOptions o;
    o.indent_namespace_body = true;
    o.indent_class_body = true;
    o.indent_interface_body = true;
    o.indent_struct_body = true;
    o.indent_enum_body = true;
    o.indent_method_body = true;
    o.indent_property_body = true;
    o.indent_event_body = true;
    o.indent_blocks = true;
    o.indent_switch_body = true;
    o.indent_case_body = true;
    o.indent_break_statements = true;
    o.indent_preprocessor_directives = true;
    o.namespace_brace_style = BraceStyle::NextLine;
    o.class_brace_style = BraceStyle::NextLine;
    o.interface_brace_style = BraceStyle::NextLine;
    o.struct_brace_style = BraceStyle::NextLine;
    o.enum_brace_style = BraceStyle::NextLine;
    o.method_brace_style = BraceStyle::NextLine;
    o.constructor_brace_style = BraceStyle::NextLine;
    o.destructor_brace_style = BraceStyle::NextLine;
    o.anonymous_method_brace_style = BraceStyle::EndOfLine;
    o.property_brace_style = BraceStyle::EndOfLine;
    o.property_get_brace_style = BraceStyle::EndOfLine;
    o.property_set_brace_style = BraceStyle::EndOfLine;
    o.simple_get_block_formatting = PropertyFormatting::SingleLine;
    o.simple_set_block_formatting = PropertyFormatting::SingleLine;

    o.event_brace_style = BraceStyle::EndOfLine;
    o.event_add_brace_style = BraceStyle::EndOfLine;
    o.event_remove_brace_style = BraceStyle::EndOfLine;
    o.allow_event_add_block_inline = true;
    o.allow_event_remove_block_inline = true;
    o.statement_brace_style = BraceStyle::EndOfLine;

    o.else_new_line_placement = NewLinePlacement::SameLine;
    o.else_if_new_line_placement = NewLinePlacement::SameLine;
    o.catch_new_line_placement = NewLinePlacement::SameLine;
    o.finally_new_line_placement = NewLinePlacement::SameLine;
    o.while_new_line_placement = NewLinePlacement::SameLine;
    o.array_initializer_wrapping = Wrapping::WrapIfTooLong;
    o.array_initializer_brace_style = BraceStyle::EndOfLine;

    o.space_before_method_call_parentheses = false;
    o.space_before_method_declaration_parentheses = false;
    o.space_before_constructor_declaration_parentheses = false;
    o.space_before_delegate_declaration_parentheses = false;
    o.space_before_indexer_declaration_bracket = false;
    o.space_after_method_call_parameter_comma = true;
    o.space_after_constructor_declaration_parameter_comma = true;
    o.new_line_before_constructor_initializer_colon = NewLinePlacement::NewLine;
    o.new_line_after_constructor_initializer_colon = NewLinePlacement::SameLine;

    o.space_before_new_parentheses = false;
    o.spaces_within_new_parentheses = false;
    o.spaces_between_empty_new_parentheses = false;
    o.space_before_new_parameter_comma = false;
    o.space_after_new_parameter_comma = true;

    o.space_before_if_parentheses = true;
    o.space_before_while_parentheses = true;
    o.space_before_for_parentheses = true;
    o.space_before_foreach_parentheses = true;
    o.space_before_catch_parentheses = true;
    o.space_before_switch_parentheses = true;
    o.space_before_lock_parentheses = true;
    o.space_before_using_parentheses = true;

    o.space_around_assignment = true;
    o.space_around_logical_operator = true;
    o.space_around_equality_operator = true;
    o.space_around_relational_operator = true;
    o.space_around_bitwise_operator = true;
    o.space_around_additive_operator = true;
    o.space_around_multiplicative_operator = true;
    o.space_around_shift_operator = true;
    o.space_around_null_coalescing_operator = true;
    o.spaces_within_parentheses = false;
    o.space_within_method_call_parentheses = false;
    o.space_within_method_declaration_parentheses = false;
    o.spaces_within_if_parentheses = false;
    o.spaces_within_while_parentheses = false;
    o.spaces_within_for_parentheses = false;
    o.spaces_within_foreach_parentheses = false;
    o.spaces_within_catch_parentheses = false;
    o.spaces_within_switch_parentheses = false;
    o.spaces_within_lock_parentheses = false;
    o.spaces_within_using_parentheses = false;
    o.spaces_within_cast_parentheses = false;
    o.spaces_within_size_of_parentheses = false;
    o.spaces_within_type_of_parentheses = false;
    o.spaces_within_checked_expression_parantheses = false;
    o.space_before_conditional_operator_condition = true;
    o.space_after_conditional_operator_condition = true;
    o.space_before_conditional_operator_separator = true;
    o.space_after_conditional_operator_separator = true;
    o.space_before_array_declaration_brackets = false;

    o.spaces_within_brackets = false;
    o.spaces_before_brackets = false;
    o.space_before_bracket_comma = false;
    o.space_after_bracket_comma = true;

    o.space_before_for_semicolon = false;
    o.space_after_for_semicolon = true;
    o.space_after_typecast = false;

    o.align_embedded_statements = true;
    o.simple_property_formatting = PropertyFormatting::SingleLine;
    o.auto_property_formatting = PropertyFormatting::SingleLine;
    o.empty_line_formatting = EmptyLineFormatting::DoNotIndent;
    o.space_before_method_declaration_parameter_comma = false;
    o.space_after_method_declaration_parameter_comma = true;
    o.space_after_delegate_declaration_parameter_comma = true;
    o.space_before_field_declaration_comma = false;
    o.space_after_field_declaration_comma = true;
    o.space_before_local_variable_declaration_comma = false;
    o.space_after_local_variable_declaration_comma = true;

    o.space_within_indexer_declaration_bracket = false;
    o.space_before_indexer_declaration_parameter_comma = false;
    o.space_in_named_argument_after_double_colon = true;

    o.space_after_indexer_declaration_parameter_comma = true;
    o.remove_end_of_line_white_space = true;

    o.minimum_blank_lines_before_usings = 0;
    o.minimum_blank_lines_after_usings = 1;

    o.minimum_blank_lines_before_first_declaration = 0;
    o.minimum_blank_lines_between_types = 1;
    o.minimum_blank_lines_between_fields = 0;
    o.minimum_blank_lines_between_event_fields = 0;
    o.minimum_blank_lines_between_members = 1;
    o.minimum_blank_lines_around_region = 1;
    o.minimum_blank_lines_inside_region = 1;

    o.keep_comments_at_first_column = true;
    o.chained_method_call_wrapping = Wrapping::DoNotWrap;
    o.method_call_argument_wrapping = Wrapping::DoNotWrap;
    o.new_line_afer_method_call_open_parentheses = NewLinePlacement::DoNotCare;
    o.method_call_closing_parentheses_on_new_line = NewLinePlacement::DoNotCare;

    o.indexer_argument_wrapping = Wrapping::DoNotWrap;
    o.new_line_afer_indexer_open_bracket = NewLinePlacement::DoNotCare;
    o.indexer_closing_bracket_on_new_line = NewLinePlacement::DoNotCare;

    o.new_line_before_new_query_clause = NewLinePlacement::NewLine;
    return o;
}

CSharpFormattingOptions FormattingOptionsFactory::create_allman() {

    CSharpFormattingOptions base_options = create_kr_style();
    base_options.anonymous_method_brace_style = BraceStyle::NextLine;
    base_options.property_brace_style = BraceStyle::NextLine;
    base_options.property_get_brace_style = BraceStyle::NextLine;
    base_options.property_set_brace_style = BraceStyle::NextLine;

    base_options.event_brace_style = BraceStyle::NextLine;
    base_options.event_add_brace_style = BraceStyle::NextLine;
    base_options.event_remove_brace_style = BraceStyle::NextLine;
    base_options.statement_brace_style = BraceStyle::NextLine;
    base_options.array_initializer_brace_style = BraceStyle::NextLine;

    base_options.catch_new_line_placement = NewLinePlacement::NewLine;
    base_options.else_new_line_placement = NewLinePlacement::NewLine;
    base_options.else_if_new_line_placement = NewLinePlacement::SameLine;

    base_options.finally_new_line_placement = NewLinePlacement::NewLine;
    base_options.while_new_line_placement = NewLinePlacement::DoNotCare;
    base_options.array_initializer_wrapping = Wrapping::DoNotWrap;
    base_options.indent_blocks_inside_expressions = true;

    return base_options;
}

CSharpFormattingOptions FormattingOptionsFactory::create_whitesmiths() {

    CSharpFormattingOptions base_options = create_kr_style();

    base_options.namespace_brace_style = BraceStyle::NextLineShifted;
    base_options.class_brace_style = BraceStyle::NextLineShifted;
    base_options.interface_brace_style = BraceStyle::NextLineShifted;
    base_options.struct_brace_style = BraceStyle::NextLineShifted;
    base_options.enum_brace_style = BraceStyle::NextLineShifted;
    base_options.method_brace_style = BraceStyle::NextLineShifted;
    base_options.constructor_brace_style = BraceStyle::NextLineShifted;
    base_options.destructor_brace_style = BraceStyle::NextLineShifted;
    base_options.anonymous_method_brace_style = BraceStyle::NextLineShifted;
    base_options.property_brace_style = BraceStyle::NextLineShifted;
    base_options.property_get_brace_style = BraceStyle::NextLineShifted;
    base_options.property_set_brace_style = BraceStyle::NextLineShifted;

    base_options.event_brace_style = BraceStyle::NextLineShifted;
    base_options.event_add_brace_style = BraceStyle::NextLineShifted;
    base_options.event_remove_brace_style = BraceStyle::NextLineShifted;
    base_options.statement_brace_style = BraceStyle::NextLineShifted;
    base_options.indent_blocks_inside_expressions = true;
    return base_options;
}

CSharpFormattingOptions FormattingOptionsFactory::create_gnu() {

    CSharpFormattingOptions base_options = create_allman();
    base_options.statement_brace_style = BraceStyle::NextLineShifted2;
    return base_options;
}
}  // namespace cb::dotnet::decompiler::csharp::output_visitor
