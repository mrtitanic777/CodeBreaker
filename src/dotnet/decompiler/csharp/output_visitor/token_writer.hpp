// dotnet/decompiler/csharp/output_visitor/token_writer.hpp -- TokenWriter, what the C# output visitor writes its tokens (identifiers,
// keywords, punctuation, literals, spaces, line breaks, comments) to, and the writers that wrap one: DecoratingTokenWriter passes
// everything on to the writer it wraps.
//
//     StringTextWriter text;
//     auto writer = TokenWriter::create(text);                 // layout writer + the spaces the lexer needs
//     CSharpOutputVisitor visitor(*writer, FormattingOptionsFactory::create_allman());
//     tree->accept_visitor(visitor);
//
// TokenWriter::create_writer_that_sets_locations_in_ast also records, on each node, where its text starts and ends (TextLocation).
// The decorators own the writer they wrap when they are made by these factories (own()).
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: TokenWriter, ILocatable, DecoratingTokenWriter, from ITokenWriter.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). An object value is a typesystem::ConstantValue
// (null is its empty alternative), as PrimitiveExpression holds it.
#pragma once

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/output_visitor/text_writer.hpp"
#include "dotnet/decompiler/csharp/syntax/comment.hpp"
#include "dotnet/decompiler/csharp/syntax/identifier.hpp"
#include "dotnet/decompiler/csharp/syntax/pre_processor_directive.hpp"
#include "dotnet/decompiler/csharp/syntax/primitive_expression.hpp"
#include "dotnet/decompiler/csharp/syntax/text_location.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class TokenWriter {
public:
    virtual ~TokenWriter() = default;

    virtual void start_node(syntax::AstNode& node) = 0;
    virtual void end_node(syntax::AstNode& node) = 0;

    // Writes an identifier.
    virtual void write_identifier(syntax::Identifier& identifier) = 0;
    // Writes a keyword to the output.
    virtual void write_keyword(std::string_view keyword) = 0;
    // Writes a token to the output.
    virtual void write_token(std::string_view token) = 0;
    // Writes a primitive/literal value.
    virtual void write_primitive_value(const typesystem::ConstantValue& value, syntax::LiteralFormat format = syntax::LiteralFormat::None) = 0;
    virtual void write_primitive_type(std::string_view type) = 0;
    // Writes a piece of text in an interpolated string literal.
    virtual void write_interpolated_text(std::string_view text) = 0;

    virtual void space() = 0;
    virtual void indent() = 0;
    virtual void unindent() = 0;
    virtual void new_line() = 0;

    virtual void write_comment(syntax::CommentType comment_type, std::string_view content) = 0;
    // `argument` empty: none.
    virtual void write_pre_processor_directive(syntax::PreProcessorDirectiveType type, std::string_view argument) = 0;

    // A TextWriterTokenWriter with the spaces the lexer needs added.
    static std::unique_ptr<TokenWriter> create(TextWriter& writer, const std::string& indentation = "\t");
    // The same, also setting each node's location in the syntax tree.
    static std::unique_ptr<TokenWriter> create_writer_that_sets_locations_in_ast(TextWriter& writer, const std::string& indentation = "\t");
    static std::unique_ptr<TokenWriter> insert_required_spaces(TokenWriter& writer);
    // Wraps a writer that provides locations (an ILocatable); std::logic_error (upstream's InvalidOperationException) if it does not.
    static std::unique_ptr<TokenWriter> wrap_in_writer_that_sets_locations_in_ast(TokenWriter& writer);
};

// A writer that knows where the next character goes and how much it has written.
class ILocatable {
public:
    virtual ~ILocatable() = default;
    virtual syntax::TextLocation location() const = 0;
    virtual int length() const = 0;
};

class DecoratingTokenWriter : public TokenWriter {
public:
    explicit DecoratingTokenWriter(TokenWriter& decorated_writer) : decorated_writer_(decorated_writer) {}

    // Makes this writer the owner of `writer` (the one it wraps, when the factories built the chain).
    void own(std::unique_ptr<TokenWriter> writer) { owned_.push_back(std::move(writer)); }

    void start_node(syntax::AstNode& node) override { decorated_writer_.start_node(node); }
    void end_node(syntax::AstNode& node) override { decorated_writer_.end_node(node); }
    void write_identifier(syntax::Identifier& identifier) override { decorated_writer_.write_identifier(identifier); }
    void write_keyword(std::string_view keyword) override { decorated_writer_.write_keyword(keyword); }
    void write_token(std::string_view token) override { decorated_writer_.write_token(token); }
    void write_primitive_value(const typesystem::ConstantValue& value, syntax::LiteralFormat format = syntax::LiteralFormat::None) override {
        decorated_writer_.write_primitive_value(value, format);
    }
    void write_primitive_type(std::string_view type) override { decorated_writer_.write_primitive_type(type); }
    void write_interpolated_text(std::string_view text) override { decorated_writer_.write_interpolated_text(text); }
    void space() override { decorated_writer_.space(); }
    void indent() override { decorated_writer_.indent(); }
    void unindent() override { decorated_writer_.unindent(); }
    void new_line() override { decorated_writer_.new_line(); }
    void write_comment(syntax::CommentType comment_type, std::string_view content) override { decorated_writer_.write_comment(comment_type, content); }
    void write_pre_processor_directive(syntax::PreProcessorDirectiveType type, std::string_view argument) override {
        decorated_writer_.write_pre_processor_directive(type, argument);
    }

protected:
    TokenWriter& decorated_writer() const { return decorated_writer_; }

private:
    TokenWriter& decorated_writer_;
    std::vector<std::unique_ptr<TokenWriter>> owned_;
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
