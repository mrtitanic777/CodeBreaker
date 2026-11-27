// dotnet/decompiler/csharp/output_visitor/insert_missing_tokens_decorator.hpp -- InsertMissingTokensDecorator: a token writer wrapper that
// records where each node's text starts and ends (store_print_start / store_print_end), and re-attaches the identifiers and tokens it sees
// to the node being written, so the syntax tree gets source locations. The start of a node is the position of its first printed token
// (assigned on the next write after start_node), its end the end of its last token.
//
//     TextWriterTokenWriter target(text);
//     InsertMissingTokensDecorator located(target, target);       // target: where the text goes and where the locations come from
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: InsertMissingTokensDecorator), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/csharp/output_visitor/token_writer.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

class InsertMissingTokensDecorator : public DecoratingTokenWriter {
public:
    InsertMissingTokensDecorator(TokenWriter& writer, ILocatable& location_provider);

    void start_node(syntax::AstNode& node) override;
    void end_node(syntax::AstNode& node) override;
    void write_token(std::string_view token) override;
    void write_keyword(std::string_view keyword) override;
    void write_identifier(syntax::Identifier& identifier) override;
    void write_primitive_value(const typesystem::ConstantValue& value, syntax::LiteralFormat format = syntax::LiteralFormat::None) override;
    void write_primitive_type(std::string_view type) override;

private:
    void assign_pending_start_locations();
    // The node the writer is inside of (the last node started at this level), or nullptr.
    syntax::AstNode* current_node() const;

    // upstream's stack of lists: the nodes started at each level, the innermost level in current_list_.
    std::vector<std::vector<syntax::AstNode*>> nodes_;
    std::vector<syntax::AstNode*> current_list_;
    ILocatable& location_provider_;

    // Nodes that have been started but whose first token has not been written yet. The start location of a node is the position of its
    // first printed token (not the start_node position, which precedes any leading newline/indentation), so it is assigned lazily on the
    // next write.
    std::unordered_set<syntax::AstNode*> nodes_awaiting_start_location_;

    // Position immediately after the most recently written token. A node's end location is the end of its last token (not the end_node
    // position, which follows the trailing newline/indentation).
    syntax::TextLocation last_token_end_;
};

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
