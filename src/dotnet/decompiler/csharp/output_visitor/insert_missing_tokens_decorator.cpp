// dotnet/decompiler/csharp/output_visitor/insert_missing_tokens_decorator.cpp -- InsertMissingTokensDecorator; see
// insert_missing_tokens_decorator.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: InsertMissingTokensDecorator), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/insert_missing_tokens_decorator.hpp"

#include <cassert>

#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;

InsertMissingTokensDecorator::InsertMissingTokensDecorator(TokenWriter& writer, ILocatable& location_provider)
    : DecoratingTokenWriter(writer), location_provider_(location_provider) {}

void InsertMissingTokensDecorator::assign_pending_start_locations() {
    if (nodes_awaiting_start_location_.empty()) return;
    TextLocation location = location_provider_.location();
    for (AstNode* node : nodes_awaiting_start_location_) node->store_print_start(location);
    nodes_awaiting_start_location_.clear();
}

AstNode* InsertMissingTokensDecorator::current_node() const {
    if (nodes_.empty() || nodes_.back().empty()) return nullptr;
    return nodes_.back().back();
}

void InsertMissingTokensDecorator::start_node(AstNode& node) {
    // ignore whitespace: these don't need to be processed.
    // start_node/end_node is only called for them to support folding of comments.
    if (!is<Trivia>(&node)) {
        current_list_.push_back(&node);
        nodes_.push_back(std::move(current_list_));
        current_list_.clear();
        nodes_awaiting_start_location_.insert(&node);
    } else if (auto* comment = as<Comment>(&node)) {
        comment->set_start_location(location_provider_.location());
    }
    if (auto* error = as<ErrorExpression>(&node)) {
        error->set_location(location_provider_.location());
    }
    DecoratingTokenWriter::start_node(node);
}

void InsertMissingTokensDecorator::end_node(AstNode& node) {
    // ignore whitespace: these don't need to be processed.
    // start_node/end_node is only called for them to support folding of comments.
    if (!is<Trivia>(&node)) {
        // A node that printed no tokens of its own collapses to a zero-width span here.
        if (nodes_awaiting_start_location_.erase(&node) > 0) {
            node.store_print_start(last_token_end_);
            // EmptyStatement derives start_location/end_location from its own location field, which is otherwise only set when the
            // semicolon token is written. A comment-only empty statement (e.g. the carrier for a decompiler warning) prints no
            // semicolon, so point its location at the comment it carries -- already printed by now, so the statement lines up with the
            // text the reader sees -- rather than leaving it empty.
            if (auto* empty_statement = as<EmptyStatement>(&node)) {
                TextLocation location = last_token_end_;
                bool found = false;
                for (Trivia* trivia : empty_statement->leading_trivia()) {
                    if (!trivia->start_location().is_empty()) {
                        location = trivia->start_location();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    for (Trivia* trivia : empty_statement->trailing_trivia()) {
                        if (!trivia->start_location().is_empty()) {
                            location = trivia->start_location();
                            break;
                        }
                    }
                }
                empty_statement->set_location(location);
            }
        }
        node.store_print_end(last_token_end_);
        for (AstNode* child : current_list_) {
            assert(child->parent() == nullptr || &node == child->parent());
            // A parentless child is a print-only artifact with no slot in the tree -- e.g. the detached clone of a type's name token that a
            // renamed constructor/destructor prints (CSharpOutputVisitor::visit_constructor_declaration). It cannot be re-attached by
            // kind, and the real name token is already a child, so leave it out rather than route a missing kind into the throwing child
            // setter.
            const CSharpSlotInfo* slot = child->slot();
            if (slot == nullptr) continue;
            // The slot is derived from the child's index in its parent, so it must be read before remove() detaches the child (which
            // would otherwise leave it without a slot).
            const CSharpSlotInfo* kind = slot->kind();
            child->remove();
            node.add_child(child, *kind);
        }
        current_list_ = std::move(nodes_.back());
        nodes_.pop_back();
    } else if (auto* comment = as<Comment>(&node)) {
        comment->set_end_location(location_provider_.location());
    }
    DecoratingTokenWriter::end_node(node);
}

void InsertMissingTokensDecorator::write_token(std::string_view token) {
    assign_pending_start_locations();
    AstNode* last = current_node();
    if (auto* empty_statement = as<EmptyStatement>(last)) {
        empty_statement->set_location(location_provider_.location());
    } else if (auto* error_expression = as<ErrorExpression>(last)) {
        error_expression->set_location(location_provider_.location());
    }
    DecoratingTokenWriter::write_token(token);
    last_token_end_ = location_provider_.location();
}

void InsertMissingTokensDecorator::write_keyword(std::string_view keyword) {
    assign_pending_start_locations();
    TextLocation start = location_provider_.location();
    if (keyword == "this") {
        if (auto* node = as<ThisReferenceExpression>(current_node())) node->store_print_start(start);
    } else if (keyword == "base") {
        if (auto* node = as<BaseReferenceExpression>(current_node())) node->store_print_start(start);
    }
    DecoratingTokenWriter::write_keyword(keyword);
    last_token_end_ = location_provider_.location();
}

void InsertMissingTokensDecorator::write_identifier(Identifier& identifier) {
    assign_pending_start_locations();
    identifier.set_start_location(location_provider_.location());
    current_list_.push_back(&identifier);
    DecoratingTokenWriter::write_identifier(identifier);
    last_token_end_ = location_provider_.location();
}

void InsertMissingTokensDecorator::write_primitive_value(const typesystem::ConstantValue& value, LiteralFormat format) {
    assign_pending_start_locations();
    AstNode* last = current_node();
    Expression* node = as<Expression>(last);
    TextLocation start_location = location_provider_.location();
    DecoratingTokenWriter::write_primitive_value(value, format);
    if (auto* primitive = as<PrimitiveExpression>(node)) {
        primitive->set_location(start_location, location_provider_.location());
    }
    if (auto* null_reference = as<NullReferenceExpression>(node)) {
        null_reference->store_print_start(start_location);
    }
    last_token_end_ = location_provider_.location();
}

void InsertMissingTokensDecorator::write_primitive_type(std::string_view type) {
    assign_pending_start_locations();
    if (auto* node = as<PrimitiveType>(current_node())) node->store_print_start(location_provider_.location());
    DecoratingTokenWriter::write_primitive_type(type);
    last_token_end_ = location_provider_.location();
}

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
