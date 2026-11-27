// dotnet/decompiler/csharp/output_visitor/error_tolerant_output_visitor.hpp -- ErrorTolerantOutputVisitor: a CSharpOutputVisitor that does
// not stop at a declaration it cannot write. The exception is kept (errors()), the nodes and braces the declaration left open are closed,
// and the error is written as `//` comment lines in its place; the rest of the file is written as usual.
//
//     StringTextWriter text;
//     ErrorTolerantOutputVisitor visitor(text, FormattingOptionsFactory::create_allman());
//     syntax_tree->accept_visitor(visitor);
//     visitor.errors();                              // what went wrong, if anything
//
// Cancellation and I/O errors are not caught. The comment lines are the ones ILSpy writes (the headline and where to report it), then
// the exception's text: .NET's exception text, with its stack trace, has no counterpart here, so what() is written in its place.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: ErrorTolerantOutputVisitor), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler). The comment written in place of a failed declaration
// is CSharpDecompiler::get_error_comment_lines.
#pragma once

#include <exception>
#include <functional>
#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class ErrorTolerantOutputVisitor : public CSharpOutputVisitor {
public:
    ErrorTolerantOutputVisitor(TextWriter& text_writer, const CSharpFormattingOptions& formatting_policy)
        : CSharpOutputVisitor(text_writer, formatting_policy) {}

    // The errors caught so far.
    const std::vector<std::exception_ptr>& errors() const { return errors_; }

    void visit_type_declaration(syntax::TypeDeclaration& type_declaration) override;
    void visit_delegate_declaration(syntax::DelegateDeclaration& delegate_declaration) override;
    void visit_constructor_declaration(syntax::ConstructorDeclaration& constructor_declaration) override;
    void visit_destructor_declaration(syntax::DestructorDeclaration& destructor_declaration) override;
    void visit_enum_member_declaration(syntax::EnumMemberDeclaration& enum_member_declaration) override;
    void visit_extension_declaration(syntax::ExtensionDeclaration& extension_declaration) override;
    void visit_event_declaration(syntax::EventDeclaration& event_declaration) override;
    void visit_custom_event_declaration(syntax::CustomEventDeclaration& custom_event_declaration) override;
    void visit_field_declaration(syntax::FieldDeclaration& field_declaration) override;
    void visit_fixed_field_declaration(syntax::FixedFieldDeclaration& fixed_field_declaration) override;
    void visit_indexer_declaration(syntax::IndexerDeclaration& indexer_declaration) override;
    void visit_method_declaration(syntax::MethodDeclaration& method_declaration) override;
    void visit_operator_declaration(syntax::OperatorDeclaration& operator_declaration) override;
    void visit_property_declaration(syntax::PropertyDeclaration& property_declaration) override;

protected:
    void open_brace(BraceStyle style, bool new_line_after = true) override;
    void close_brace(BraceStyle style, bool unindent = true) override;

private:
    template <class Node>
    void write(Node& node, const std::function<void(Node&)>& write_node);

    std::vector<std::exception_ptr> errors_;
    int brace_depth_ = 0;
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
