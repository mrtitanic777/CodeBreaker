// dotnet/decompiler/csharp/output_visitor/csharp_formatting_options.hpp -- CSharpFormattingOptions: how the output visitor lays C# out
// (indentation, brace styles, where new lines go, which spaces are written, blank lines, wrapping). The options come from
// FormattingOptionsFactory (formatting_options_factory.hpp): ILSpy writes with its Allman style (create_allman).
//
//     CSharpFormattingOptions options = FormattingOptionsFactory::create_allman();
//     options.space_before_method_call_parentheses = true;
//
// Every upstream property is a public data member with its name in snake_case, and has upstream's default (false, 0, the first enumerator;
// allow_one_lined_array_initialziers true and embedded_statement_placement NewLine). Upstream's `Clone` is the copy constructor.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: CSharpFormattingOptions and its enums), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not ported: the commented-out XML load/save code.
#pragma once

#include <cstdint>
#include <string>

namespace cb::dotnet::decompiler::csharp::output_visitor {

enum class BraceStyle { EndOfLine, EndOfLineWithoutSpace, NextLine, NextLineShifted, NextLineShifted2, BannerStyle };
enum class PropertyFormatting { SingleLine, MultipleLines };
enum class Wrapping { DoNotWrap, WrapAlways, WrapIfTooLong };
enum class NewLinePlacement { DoNotCare, NewLine, SameLine };
enum class UsingPlacement { TopOfFile, InsideNamespace };
enum class EmptyLineFormatting { DoNotChange, Indent, DoNotIndent };

class CSharpFormattingOptions {
public:
    std::string name;
    bool is_built_in = false;
    std::string indentation_string = "\t";

    // ---- Indentation
    bool indent_namespace_body = false;
    bool indent_class_body = false;
    bool indent_interface_body = false;
    bool indent_struct_body = false;
    bool indent_enum_body = false;
    bool indent_method_body = false;
    bool indent_property_body = false;
    bool indent_event_body = false;
    bool indent_blocks = false;
    bool indent_switch_body = false;
    bool indent_case_body = false;
    bool indent_break_statements = false;
    bool align_embedded_statements = false;
    bool align_else_in_if_statements = false;
    PropertyFormatting auto_property_formatting = PropertyFormatting{};
    PropertyFormatting simple_property_formatting = PropertyFormatting{};
    EmptyLineFormatting empty_line_formatting = EmptyLineFormatting{};
    bool indent_preprocessor_directives = false;
    bool align_to_member_reference_dot = false;
    bool indent_blocks_inside_expressions = false;

    // ---- Braces
    BraceStyle namespace_brace_style = BraceStyle{};
    BraceStyle class_brace_style = BraceStyle{};
    BraceStyle interface_brace_style = BraceStyle{};
    BraceStyle struct_brace_style = BraceStyle{};
    BraceStyle enum_brace_style = BraceStyle{};
    BraceStyle method_brace_style = BraceStyle{};
    BraceStyle anonymous_method_brace_style = BraceStyle{};
    BraceStyle constructor_brace_style = BraceStyle{};
    BraceStyle destructor_brace_style = BraceStyle{};
    BraceStyle property_brace_style = BraceStyle{};
    BraceStyle property_get_brace_style = BraceStyle{};
    BraceStyle property_set_brace_style = BraceStyle{};
    PropertyFormatting simple_get_block_formatting = PropertyFormatting{};
    PropertyFormatting simple_set_block_formatting = PropertyFormatting{};
    BraceStyle event_brace_style = BraceStyle{};
    BraceStyle event_add_brace_style = BraceStyle{};
    BraceStyle event_remove_brace_style = BraceStyle{};
    bool allow_event_add_block_inline = false;
    bool allow_event_remove_block_inline = false;
    BraceStyle statement_brace_style = BraceStyle{};
    bool allow_if_block_inline = false;
    bool allow_one_lined_array_initialziers = true;

    // ---- NewLines
    NewLinePlacement else_new_line_placement = NewLinePlacement{};
    NewLinePlacement else_if_new_line_placement = NewLinePlacement{};
    NewLinePlacement catch_new_line_placement = NewLinePlacement{};
    NewLinePlacement finally_new_line_placement = NewLinePlacement{};
    NewLinePlacement while_new_line_placement = NewLinePlacement{};
    NewLinePlacement embedded_statement_placement = NewLinePlacement::NewLine;

    // ---- Spaces
    bool space_between_parameter_attribute_sections = false;
    bool space_before_method_declaration_parentheses = false;
    bool space_between_empty_method_declaration_parentheses = false;
    bool space_before_method_declaration_parameter_comma = false;
    bool space_after_method_declaration_parameter_comma = false;
    bool space_within_method_declaration_parentheses = false;
    bool space_before_method_call_parentheses = false;
    bool space_between_empty_method_call_parentheses = false;
    bool space_before_method_call_parameter_comma = false;
    bool space_after_method_call_parameter_comma = false;
    bool space_within_method_call_parentheses = false;
    bool space_before_field_declaration_comma = false;
    bool space_after_field_declaration_comma = false;
    bool space_before_local_variable_declaration_comma = false;
    bool space_after_local_variable_declaration_comma = false;
    bool space_before_constructor_declaration_parentheses = false;
    bool space_between_empty_constructor_declaration_parentheses = false;
    bool space_before_constructor_declaration_parameter_comma = false;
    bool space_after_constructor_declaration_parameter_comma = false;
    bool space_within_constructor_declaration_parentheses = false;
    NewLinePlacement new_line_before_constructor_initializer_colon = NewLinePlacement{};
    NewLinePlacement new_line_after_constructor_initializer_colon = NewLinePlacement{};
    bool space_before_indexer_declaration_bracket = false;
    bool space_within_indexer_declaration_bracket = false;
    bool space_before_indexer_declaration_parameter_comma = false;
    bool space_after_indexer_declaration_parameter_comma = false;
    bool space_before_delegate_declaration_parentheses = false;
    bool space_between_empty_delegate_declaration_parentheses = false;
    bool space_before_delegate_declaration_parameter_comma = false;
    bool space_after_delegate_declaration_parameter_comma = false;
    bool space_within_delegate_declaration_parentheses = false;
    bool space_before_new_parentheses = false;
    bool space_before_if_parentheses = false;
    bool space_before_while_parentheses = false;
    bool space_before_for_parentheses = false;
    bool space_before_foreach_parentheses = false;
    bool space_before_catch_parentheses = false;
    bool space_before_switch_parentheses = false;
    bool space_before_lock_parentheses = false;
    bool space_before_using_parentheses = false;
    bool space_around_assignment = false;
    bool space_around_logical_operator = false;
    bool space_around_equality_operator = false;
    bool space_around_relational_operator = false;
    bool space_around_bitwise_operator = false;
    bool space_around_additive_operator = false;
    bool space_around_multiplicative_operator = false;
    bool space_around_shift_operator = false;
    bool space_around_null_coalescing_operator = false;
    bool space_after_unsafe_address_of_operator = false;
    bool space_after_unsafe_asterisk_of_operator = false;
    bool space_around_unsafe_arrow_operator = false;
    bool spaces_within_parentheses = false;
    bool spaces_within_if_parentheses = false;
    bool spaces_within_while_parentheses = false;
    bool spaces_within_for_parentheses = false;
    bool spaces_within_foreach_parentheses = false;
    bool spaces_within_catch_parentheses = false;
    bool spaces_within_switch_parentheses = false;
    bool spaces_within_lock_parentheses = false;
    bool spaces_within_using_parentheses = false;
    bool spaces_within_cast_parentheses = false;
    bool spaces_within_size_of_parentheses = false;
    bool space_before_size_of_parentheses = false;
    bool spaces_within_type_of_parentheses = false;
    bool spaces_within_new_parentheses = false;
    bool spaces_between_empty_new_parentheses = false;
    bool space_before_new_parameter_comma = false;
    bool space_after_new_parameter_comma = false;
    bool space_before_type_of_parentheses = false;
    bool spaces_within_checked_expression_parantheses = false;
    bool space_before_conditional_operator_condition = false;
    bool space_after_conditional_operator_condition = false;
    bool space_before_conditional_operator_separator = false;
    bool space_after_conditional_operator_separator = false;
    bool space_before_anonymous_method_parentheses = false;
    bool space_within_anonymous_method_parentheses = false;
    bool spaces_within_brackets = false;
    bool spaces_before_brackets = false;
    bool space_before_bracket_comma = false;
    bool space_after_bracket_comma = false;
    bool space_before_for_semicolon = false;
    bool space_after_for_semicolon = false;
    bool space_after_typecast = false;
    bool space_before_array_declaration_brackets = false;
    bool space_in_named_argument_after_double_colon = false;
    bool remove_end_of_line_white_space = false;
    bool space_before_semicolon = false;

    // ---- Blank Lines
    int minimum_blank_lines_before_usings = 0;
    int minimum_blank_lines_after_usings = 0;
    int minimum_blank_lines_before_first_declaration = 0;
    int minimum_blank_lines_between_types = 0;
    int minimum_blank_lines_between_fields = 0;
    int minimum_blank_lines_between_event_fields = 0;
    int minimum_blank_lines_between_members = 0;
    int minimum_blank_lines_around_region = 0;
    int minimum_blank_lines_inside_region = 0;

    // ---- Keep formatting
    bool keep_comments_at_first_column = false;

    // ---- Wrapping
    Wrapping array_initializer_wrapping = Wrapping{};
    BraceStyle array_initializer_brace_style = BraceStyle{};
    Wrapping chained_method_call_wrapping = Wrapping{};
    Wrapping method_call_argument_wrapping = Wrapping{};
    NewLinePlacement new_line_afer_method_call_open_parentheses = NewLinePlacement{};
    NewLinePlacement method_call_closing_parentheses_on_new_line = NewLinePlacement{};
    Wrapping indexer_argument_wrapping = Wrapping{};
    NewLinePlacement new_line_afer_indexer_open_bracket = NewLinePlacement{};
    NewLinePlacement indexer_closing_bracket_on_new_line = NewLinePlacement{};
    Wrapping method_declaration_parameter_wrapping = Wrapping{};
    NewLinePlacement new_line_afer_method_declaration_open_parentheses = NewLinePlacement{};
    NewLinePlacement method_declaration_closing_parentheses_on_new_line = NewLinePlacement{};
    Wrapping indexer_declaration_parameter_wrapping = Wrapping{};
    NewLinePlacement new_line_afer_indexer_declaration_open_bracket = NewLinePlacement{};
    NewLinePlacement indexer_declaration_closing_bracket_on_new_line = NewLinePlacement{};
    bool align_to_first_indexer_argument = false;
    bool align_to_first_indexer_declaration_parameter = false;
    bool align_to_first_method_call_argument = false;
    bool align_to_first_method_declaration_parameter = false;
    NewLinePlacement new_line_before_new_query_clause = NewLinePlacement{};

    // ---- Using Declarations
    UsingPlacement using_placement = UsingPlacement{};
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
