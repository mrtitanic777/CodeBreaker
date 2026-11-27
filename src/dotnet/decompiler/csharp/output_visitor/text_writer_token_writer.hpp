// dotnet/decompiler/csharp/output_visitor/text_writer_token_writer.hpp -- TextWriterTokenWriter, the token writer that lays the C# out as
// text: indentation, spaces, line breaks, comments, preprocessor directives, and the literals (numbers as .NET prints them, strings and
// characters escaped as ILSpy escapes them). It keeps the line, column and length of what it wrote (ILocatable).
//
//     StringTextWriter text;
//     TextWriterTokenWriter writer(text);
//     writer.write_keyword("class");  writer.space();  ...
//     TextWriterTokenWriter::print_primitive_value(typesystem::ConstantValue(std::int32_t(5)));   // "5"
//     TextWriterTokenWriter::convert_string(u"a\"b\n");                                           // a\"b\n
//
// Columns and lengths count UTF-16 code units, as .NET's string.Length does. A character is escaped by its Unicode category as
// ILSpy does (marks, separators, control, format, surrogates, private use, connector punctuation, modifier symbols, unassigned and
// space separators other than ' ' as \uXXXX). Escaping an identifier looks at supplementary characters as a pair: the tables of util/char_info
// classify code units only, so a supplementary character is taken as printable except in the private use planes, the emoji
// skin tone modifiers and the tag characters.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: TextWriterTokenWriter), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#pragma once

#include <optional>
#include <string>
#include <string_view>

#include "dotnet/decompiler/csharp/output_visitor/text_writer.hpp"
#include "dotnet/decompiler/csharp/output_visitor/token_writer.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class TextWriterTokenWriter : public TokenWriter, public ILocatable {
public:
    explicit TextWriterTokenWriter(TextWriter& text_writer) : text_writer_(text_writer) {}

    int indentation() const { return indentation_; }
    void set_indentation(int value) { indentation_ = value; }
    const std::string& indentation_string() const { return indentation_string_; }
    void set_indentation_string(std::string value) { indentation_string_ = std::move(value); }

    // ILocatable: the place the next character goes to (after the indentation a pending new line will write).
    syntax::TextLocation location() const override;
    int length() const override { return length_; }

    void write_identifier(syntax::Identifier& identifier) override;
    void write_keyword(std::string_view keyword) override;
    void write_token(std::string_view token) override;
    void space() override;
    void new_line() override;
    void indent() override { ++indentation_; }
    void unindent() override { --indentation_; }
    void write_comment(syntax::CommentType comment_type, std::string_view content) override;
    void write_pre_processor_directive(syntax::PreProcessorDirectiveType type, std::string_view argument) override;
    void write_primitive_value(const typesystem::ConstantValue& value, syntax::LiteralFormat format = syntax::LiteralFormat::None) override;
    void write_interpolated_text(std::string_view text) override;
    void write_primitive_type(std::string_view type) override;
    void start_node(syntax::AstNode& node) override;
    void end_node(syntax::AstNode& node) override;

    // The text a primitive value is written as (a fresh writer's write_primitive_value).
    static std::string print_primitive_value(const typesystem::ConstantValue& value);

    // The escape sequence for the character within a char literal, without the single quotes.
    static std::string convert_char_literal(char16_t ch);
    // Converts special characters to escape sequences within the string (a double quote is escaped too).
    static std::u16string convert_string(std::u16string_view str);
    static std::string convert_string(std::string_view utf8);

    // The identifier with characters that can not be written as they are replaced by \uXXXX (\UXXXXXXXX for a supplementary one).
    static std::string escape_identifier(std::string_view identifier);
    static bool contains_non_printable_identifier_char(std::string_view identifier);

    // The lower-case name of a directive ("region", "endif"): what follows the # (upstream's type.ToString().ToLowerInvariant()).
    static std::string_view pre_processor_directive_name(syntax::PreProcessorDirectiveType type);

protected:
    void write_indentation();

private:
    TextWriter& text_writer_;
    bool needs_indent_ = true;
    bool is_at_start_of_line_ = true;
    int line_ = 1;
    int column_ = 1;
    int length_ = 0;
    int indentation_ = 0;
    std::string indentation_string_ = "\t";
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
