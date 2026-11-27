// dotnet/decompiler/csharp/output_visitor/insert_required_spaces_decorator.hpp -- InsertRequiredSpacesDecorator: a token writer wrapper that
// writes the minimal number of spaces so that the C# lexer recognizes the tokens that were written (two names, a name and a keyword or a
// number side by side; `+` after `+`; `/` before `*` ...). A space the formatting policy asks for is written by the visitor; this adds
// only the ones the text would otherwise be wrong without.
//
//     InsertRequiredSpacesDecorator writer(inner);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: InsertRequiredSpacesDecorator), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/csharp/output_visitor/token_writer.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class InsertRequiredSpacesDecorator : public DecoratingTokenWriter {
public:
    explicit InsertRequiredSpacesDecorator(TokenWriter& writer) : DecoratingTokenWriter(writer) {}

    void write_identifier(syntax::Identifier& identifier) override;
    void write_keyword(std::string_view keyword) override;
    void write_token(std::string_view token) override;
    void space() override;
    void new_line() override;
    void write_comment(syntax::CommentType comment_type, std::string_view content) override;
    void write_pre_processor_directive(syntax::PreProcessorDirectiveType type, std::string_view argument) override;
    void write_primitive_value(const typesystem::ConstantValue& value, syntax::LiteralFormat format = syntax::LiteralFormat::None) override;
    void write_primitive_type(std::string_view type) override;

private:
    // Used to insert the minimal amount of spaces so that the lexer recognizes the tokens that were written.
    enum class LastWritten { Whitespace, Other, KeywordOrIdentifier, Plus, Minus, Ampersand, QuestionMark, Division };

    LastWritten last_written_ = LastWritten::Whitespace;
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
