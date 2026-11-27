// dotnet/decompiler/csharp/output_visitor/insert_required_spaces_decorator.cpp -- InsertRequiredSpacesDecorator; see
// insert_required_spaces_decorator.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: InsertRequiredSpacesDecorator), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/insert_required_spaces_decorator.hpp"

#include <cmath>
#include <variant>

#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using syntax::CommentType;
using syntax::LiteralFormat;
using syntax::PreProcessorDirectiveType;

void InsertRequiredSpacesDecorator::write_identifier(syntax::Identifier& identifier) {
    if (identifier.is_verbatim() || CSharpOutputVisitor::is_keyword(identifier.name(), &identifier)) {
        if (last_written_ == LastWritten::KeywordOrIdentifier) {
            // this space is not strictly required, so we call space()
            space();
        }
    } else if (last_written_ == LastWritten::KeywordOrIdentifier) {
        // this space is strictly required, so we directly call the formatter
        DecoratingTokenWriter::space();
    }
    DecoratingTokenWriter::write_identifier(identifier);
    last_written_ = LastWritten::KeywordOrIdentifier;
}

void InsertRequiredSpacesDecorator::write_keyword(std::string_view keyword) {
    if (last_written_ == LastWritten::KeywordOrIdentifier) {
        space();
    }
    DecoratingTokenWriter::write_keyword(keyword);
    last_written_ = LastWritten::KeywordOrIdentifier;
}

void InsertRequiredSpacesDecorator::write_token(std::string_view token) {
    // Avoid that two +, - or ? tokens are combined into a ++, -- or ?? token.
    // Note that we don't need to handle tokens like = because there's no valid C# program that contains the single token twice in a row.
    // (for +, - and &, this can happen with unary operators; for ?, this can happen in "a is int? ? b : c" or "a as int? ?? 0"; and for
    // /, this can happen with "1/ *ptr" or "1/ //comment".)
    if ((last_written_ == LastWritten::Plus && token[0] == '+') || (last_written_ == LastWritten::Minus && token[0] == '-') ||
        (last_written_ == LastWritten::Ampersand && token[0] == '&') || (last_written_ == LastWritten::QuestionMark && token[0] == '?') ||
        (last_written_ == LastWritten::Division && token[0] == '*')) {
        DecoratingTokenWriter::space();
    }
    DecoratingTokenWriter::write_token(token);
    if (token == "+") {
        last_written_ = LastWritten::Plus;
    } else if (token == "-") {
        last_written_ = LastWritten::Minus;
    } else if (token == "&") {
        last_written_ = LastWritten::Ampersand;
    } else if (token == "?") {
        last_written_ = LastWritten::QuestionMark;
    } else if (token == "/") {
        last_written_ = LastWritten::Division;
    } else {
        last_written_ = LastWritten::Other;
    }
}

void InsertRequiredSpacesDecorator::space() {
    DecoratingTokenWriter::space();
    last_written_ = LastWritten::Whitespace;
}

void InsertRequiredSpacesDecorator::new_line() {
    DecoratingTokenWriter::new_line();
    last_written_ = LastWritten::Whitespace;
}

void InsertRequiredSpacesDecorator::write_comment(CommentType comment_type, std::string_view content) {
    if (last_written_ == LastWritten::Division) {
        // When there's a comment starting after a division operator "1.0 / /*comment*/a", then we need to insert a space in front of the
        // comment.
        DecoratingTokenWriter::space();
    }
    DecoratingTokenWriter::write_comment(comment_type, content);
    last_written_ = LastWritten::Whitespace;
}

void InsertRequiredSpacesDecorator::write_pre_processor_directive(PreProcessorDirectiveType type, std::string_view argument) {
    DecoratingTokenWriter::write_pre_processor_directive(type, argument);
    last_written_ = LastWritten::Whitespace;
}

void InsertRequiredSpacesDecorator::write_primitive_value(const typesystem::ConstantValue& value, LiteralFormat format) {
    if (last_written_ == LastWritten::KeywordOrIdentifier) {
        space();
    }
    DecoratingTokenWriter::write_primitive_value(value, format);
    if (std::holds_alternative<std::monostate>(value) || std::holds_alternative<bool>(value)) return;
    if (std::holds_alternative<std::u16string>(value)) {
        if (format == LiteralFormat::VerbatimStringLiteral)
            last_written_ = LastWritten::KeywordOrIdentifier;
        else
            last_written_ = LastWritten::Other;
    } else if (std::holds_alternative<char16_t>(value)) {
        last_written_ = LastWritten::Other;
    } else if (std::holds_alternative<util::Decimal>(value)) {
        last_written_ = LastWritten::Other;
    } else if (const float* f = std::get_if<float>(&value)) {
        if (std::isinf(*f) || std::isnan(*f)) return;
        last_written_ = LastWritten::Other;
    } else if (const double* d = std::get_if<double>(&value)) {
        if (std::isinf(*d) || std::isnan(*d)) return;
        // needs space if identifier follows number; this avoids mistaking the following identifier as type suffix
        last_written_ = LastWritten::KeywordOrIdentifier;
    } else {
        // the integers (IFormattable): needs space if identifier follows number; this avoids mistaking the following identifier as type
        // suffix
        last_written_ = LastWritten::KeywordOrIdentifier;
    }
}

void InsertRequiredSpacesDecorator::write_primitive_type(std::string_view type) {
    if (last_written_ == LastWritten::KeywordOrIdentifier) {
        space();
    }
    DecoratingTokenWriter::write_primitive_type(type);
    if (type == "new") {
        last_written_ = LastWritten::Other;
    } else {
        last_written_ = LastWritten::KeywordOrIdentifier;
    }
}

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
