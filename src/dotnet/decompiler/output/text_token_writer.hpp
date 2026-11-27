// dotnet/decompiler/output/text_token_writer.hpp -- TextTokenWriter: the token writer that sends the C# the output visitor writes to an
// ITextOutput, with what a viewer needs: identifiers that are references to types, members and local variables (or the definition of one),
// folds around members, blocks and documentation comments, and the using declarations folded as a group.
//
//     PlainTextOutput output;
//     TextTokenWriter writer(output, settings);
//     CSharpOutputVisitor visitor(writer, FormattingOptionsFactory::create_allman());
//     syntax_tree->accept_visitor(visitor);
//     output.text();
//
// What an identifier refers to is read from the node's annotations (the symbol of GetSymbol, an ILVariableResolveResult, a
// LetIdentifierAnnotation): a local variable's reference is the ILVariable itself, a label's the text of its method plus the label's name,
// a local function's its member definition. Text is UTF-8; literals are written by a TextWriterTokenWriter over the output.
//
// Ported from ILSpy (ICSharpCode.Decompiler: TextTokenWriter, from Output/TextTokenWriter.cs), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <any>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/output_visitor/token_writer.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/output/text_output.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler {

class TextTokenWriter final : public csharp::output_visitor::TokenWriter {
public:
    // The output and the settings must outlive the writer.
    TextTokenWriter(ITextOutput& output, const DecompilerSettings& settings) : output_(output), settings_(settings) {}

    void write_identifier(csharp::syntax::Identifier& identifier) override;
    void write_keyword(std::string_view keyword) override;
    void write_token(std::string_view token) override;
    void space() override { output_.write(' '); }
    void indent() override { output_.indent(); }
    void unindent() override { output_.unindent(); }
    void new_line() override;
    void write_comment(csharp::syntax::CommentType comment_type, std::string_view content) override;
    void write_pre_processor_directive(csharp::syntax::PreProcessorDirectiveType type, std::string_view argument) override;
    void write_primitive_value(const typesystem::ConstantValue& value, csharp::syntax::LiteralFormat format = csharp::syntax::LiteralFormat::None) override;
    void write_interpolated_text(std::string_view text) override;
    void write_primitive_type(std::string_view type) override;
    void start_node(csharp::syntax::AstNode& node) override;
    // std::logic_error (upstream's InvalidOperationException) if `node` is not the node last started.
    void end_node(csharp::syntax::AstNode& node) override;

    // Whether the node is a definition (a declaration of an entity, not a local function's; a field's or event's variable initializer,
    // for which `node` becomes the declaration; a fixed buffer's variable).
    static bool is_definition(csharp::syntax::AstNode*& node);

private:
    const typesystem::ISymbol* get_current_member_reference();
    static const typesystem::ISymbol* filter_member(const typesystem::ISymbol* symbol);
    static bool is_dynamic_member_reference(csharp::syntax::AstNode* node);
    std::any get_current_local_reference();
    std::any get_current_local_definition(csharp::syntax::Identifier* id);
    const typesystem::ISymbol* get_current_definition();
    // The text of the innermost method on the node stack, or none.
    const typesystem::IMethod* enclosing_method() const;
    static bool needs_fold(csharp::syntax::AstNode* node);
    static bool is_using_declaration(csharp::syntax::AstNode* node);
    csharp::syntax::AstNode* peek_or_null() const { return node_stack_.empty() ? nullptr : node_stack_.back(); }

    ITextOutput& output_;
    const DecompilerSettings& settings_;
    std::vector<csharp::syntax::AstNode*> node_stack_;
    int brace_level_within_type_ = -1;
    bool in_documentation_comment_ = false;
    bool first_using_declaration_ = false;
    bool last_using_declaration_ = false;
};

}  // namespace cb::dotnet::decompiler
