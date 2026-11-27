// dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp -- CSharpOutputVisitor: writes a C# syntax tree as C# text, through a
// TokenWriter, laid out by a CSharpFormattingOptions. It is a syntax-tree visitor with one visit method per node class; each starts the node
// (so the writer knows what it is writing), writes the node's keywords, tokens, spaces and children in the order C# has them, and ends it.
//
//     StringTextWriter text;
//     CSharpOutputVisitor visitor(text, FormattingOptionsFactory::create_allman());
//     syntax_tree->accept_visitor(visitor);
//     text.text();                                  // the C#
//
//     CSharpOutputVisitor::is_keyword("class");     // true: such a name is written with a leading @
//
// The visit methods and the helpers are virtual, as upstream's are, so a subclass can change what is written. The formatting options are
// copied in. Writing a node installs AstNode::to_string (set_to_string_function, below): a node's to_string() writes it with this visitor.
//
// The visit methods are in csharp_output_visitor.cpp (machinery), csharp_output_visitor_expressions.cpp, csharp_output_visitor_statements.cpp
// and csharp_output_visitor_members.cpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: CSharpOutputVisitor), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler). Differences: collections are passed to the list-writing helpers as std::vector<AstNode*> (nodes_of);
// the text of a pattern node that is none of the pattern classes (upstream writes its ToString) is its type name.
#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "dotnet/decompiler/csharp/output_visitor/csharp_formatting_options.hpp"
#include "dotnet/decompiler/csharp/output_visitor/token_writer.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_node.hpp"
#include "dotnet/decompiler/csharp/syntax/ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/modifiers.hpp"
#include "dotnet/decompiler/csharp/syntax/operator_type.hpp"

namespace cb::dotnet::decompiler::csharp {
namespace syntax::pattern_matching {
class AnyNode;
class Backreference;
class Choice;
class IdentifierExpressionBackreference;
class NamedNode;
class Repeat;
class OptionalNode;
class INode;
}  // namespace syntax::pattern_matching

namespace output_visitor {

class CSharpOutputVisitor : public syntax::IAstVisitor {
public:
    // Writes to a text writer, through a TokenWriter made by TokenWriter::create with the policy's indentation string.
    CSharpOutputVisitor(TextWriter& text_writer, const CSharpFormattingOptions& formatting_policy);
    // Writes to a token writer (the spaces the lexer needs are added around it). The writer must outlive the visitor.
    CSharpOutputVisitor(TokenWriter& writer, const CSharpFormattingOptions& formatting_policy);
    ~CSharpOutputVisitor() override = default;

    // Whether the name must be written with a leading @ (a C# keyword; a query keyword or `await` only where the context makes it one).
    static bool is_keyword(std::string_view identifier, const syntax::AstNode* context = nullptr);
    // Makes AstNode::to_string write the node with this visitor (done at program start and by the constructors; calling it again is harmless).
    static void install_to_string_hook();
    // TextWriterTokenWriter::convert_string.
    static std::string convert_string(std::string_view text);

    virtual void write_type_parameters(const std::vector<syntax::AstNode*>& type_parameters);

    // IAstVisitor: one visit method per node class.
#define CB_OV_VISIT_DECL(Class, name) void visit_##name(syntax::Class& node) override;
    CB_CSHARP_AST_VISITS(CB_OV_VISIT_DECL)
#undef CB_OV_VISIT_DECL
    void visit_error_node(syntax::AstNode& error_node) override;
    void visit_pattern_placeholder(syntax::AstNode& placeholder, syntax::pattern_matching::Pattern& pattern) override;

    // Writes a block with the given brace style (no new line after it).
    virtual void write_block(syntax::BlockStatement* block_statement, BraceStyle style);

protected:
    // ---- StartNode/EndNode
    virtual void start_node(syntax::AstNode* node);
    virtual void end_node(syntax::AstNode* node);

    // ---- Comma and lists
    virtual void comma(syntax::AstNode* next_node, bool no_space_after_comma = false);
    virtual void write_comma_separated_list(const std::vector<syntax::AstNode*>& list);
    virtual void write_comma_separated_list_in_parenthesis(const std::vector<syntax::AstNode*>& list, bool space_within);
    // `[a, b]` with the parameter spacing of the caller.
    virtual void write_comma_separated_list_in_brackets(const std::vector<syntax::AstNode*>& list, bool space_within);
    // `[a, b]` with the policy's brackets spacing.
    virtual void write_comma_separated_list_in_brackets(const std::vector<syntax::AstNode*>& list);

    // ---- Write tokens
    virtual void write_keyword(std::string_view keyword);
    virtual void write_identifier(syntax::Identifier* identifier);
    virtual void write_identifier(std::string_view identifier);
    virtual void write_token(std::string_view token);
    virtual void l_par();
    virtual void r_par();
    virtual void semicolon();
    virtual void space(bool add_space = true);
    virtual void new_line();
    // Whether a call chain this long gets a new line before each dot, and the new line itself.
    virtual bool insert_new_line_when_in_method_call_chain(syntax::MemberReferenceExpression* expr);
    virtual void open_brace(BraceStyle style, bool new_line_after = true);
    virtual void close_brace(BraceStyle style, bool unindent = true);

    // ---- Write constructs
    virtual void write_type_arguments(const std::vector<syntax::AstNode*>& type_arguments);
    virtual void write_modifiers(syntax::Modifiers modifiers);
    virtual void write_qualified_identifier(const std::vector<syntax::Identifier*>& identifiers);
    virtual void write_embedded_statement(syntax::Statement* embedded_statement, NewLinePlacement nlp = NewLinePlacement::NewLine);
    virtual void write_method_body(syntax::BlockStatement* body, BraceStyle style, bool new_line_after = true);
    virtual void write_attributes(const std::vector<syntax::AstNode*>& attributes);
    virtual void write_private_implementation_type(syntax::AstType* private_implementation_type);
    virtual void print_initializer_elements(syntax::AstNodeCollection<syntax::Expression>& elements);

    bool can_be_confused_with_object_initializer(syntax::Expression* expr);
    bool is_object_or_collection_initializer(syntax::AstNode* node);
    bool lambda_needs_parenthesis(syntax::LambdaExpression* lambda_expression);

    // The shared slot kind of a node's position (nullptr when it has no parent): compare with `&Slots::x`.
    static const syntax::CSharpSlotInfo* slot_kind_of(const syntax::AstNode* node);

    // ---- helpers
    // The nodes of a collection (or any range of nodes) as a vector.
    template <class Range>
    static std::vector<syntax::AstNode*> nodes_of(const Range& range) {
        std::vector<syntax::AstNode*> result;
        for (auto* node : range) result.push_back(node);
        return result;
    }
    // The only element of a collection (upstream's Single()); std::logic_error if it has none or several.
    template <class Collection>
    static auto single(const Collection& collection) {
        if (collection.count() != 1) throw std::logic_error("Sequence does not contain exactly one element");
        return collection[0];
    }
    // Visits the node if there is one (upstream's `node?.AcceptVisitor(this)`).
    void accept(syntax::AstNode* node) {
        if (node != nullptr) node->accept_visitor(*this);
    }

    TokenWriter& writer() const { return writer_; }

    // The writer the visitor writes to (what is passed in, wrapped in the decorator that adds the spaces the lexer needs), the options, and
    // the nodes being written (the innermost last).
    std::unique_ptr<TokenWriter> owned_writer_;
    TokenWriter& writer_;
    CSharpFormattingOptions policy_;
    std::vector<syntax::AstNode*> container_stack_;

    bool is_at_start_of_line_ = true;
    bool is_after_space_ = false;

private:
    int get_call_chain_length_limited(syntax::MemberReferenceExpression* expr);
    int should_insert_new_line_when_in_method_call_chain(syntax::MemberReferenceExpression* expr);
    static bool is_postfix_operator(syntax::UnaryOperatorType op);
    void maybe_new_lines_after_usings(syntax::AstNode* node);

    // Pattern nodes
    void visit_any_node(syntax::pattern_matching::AnyNode& any_node);
    void visit_backreference(syntax::pattern_matching::Backreference& backreference);
    void visit_identifier_expression_backreference(syntax::pattern_matching::IdentifierExpressionBackreference& backreference);
    void visit_choice(syntax::pattern_matching::Choice& choice);
    void visit_named_node(syntax::pattern_matching::NamedNode& named_node);
    void visit_repeat(syntax::pattern_matching::Repeat& repeat);
    void visit_optional_node(syntax::pattern_matching::OptionalNode& optional_node);
    void visit_node_in_pattern(syntax::pattern_matching::INode* child_node);
};

}  // namespace output_visitor
}  // namespace cb::dotnet::decompiler::csharp
