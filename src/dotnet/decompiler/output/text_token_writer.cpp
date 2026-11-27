// dotnet/decompiler/output/text_token_writer.cpp -- TextTokenWriter; see text_token_writer.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler: TextTokenWriter, from Output/TextTokenWriter.cs), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/output/text_token_writer.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/text_writer.hpp"
#include "dotnet/decompiler/csharp/output_visitor/text_writer_token_writer.hpp"
#include "dotnet/decompiler/csharp/resolver/dynamic_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/dynamic_member_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/combine_query_expressions.hpp"
#include "dotnet/decompiler/il/advanced_instructions.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/inheritance_helper.hpp"
#include "dotnet/decompiler/typesystem/local_function_method.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler {

using namespace csharp::syntax;
using csharp::output_visitor::TextOutputWriter;
using csharp::output_visitor::TextWriterTokenWriter;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::ISymbol;
using typesystem::IType;

const IMethod* TextTokenWriter::enclosing_method() const {
    // The first method symbol found going outwards from the node being written.
    for (auto it = node_stack_.rbegin(); it != node_stack_.rend(); ++it) {
        const IMethod* mr = dynamic_cast<const IMethod*>(csharp::get_symbol(*it));
        if (mr != nullptr) return mr;
    }
    return nullptr;
}

void TextTokenWriter::write_identifier(Identifier& identifier) {
    if (identifier.is_verbatim() || csharp::output_visitor::CSharpOutputVisitor::is_keyword(identifier.name(), &identifier)) {
        output_.write('@');
    }

    const ISymbol* definition = get_current_definition();
    std::string name = TextWriterTokenWriter::escape_identifier(identifier.name());
    if (definition != nullptr) {
        if (auto* t = dynamic_cast<const IType*>(definition)) {
            output_.write_reference(*t, name, true);
            return;
        }
        if (auto* m = dynamic_cast<const IMember*>(definition)) {
            output_.write_reference(*m, name, true);
            return;
        }
    }

    const ISymbol* member = get_current_member_reference();
    if (member != nullptr) {
        if (auto* t = dynamic_cast<const IType*>(member)) {
            output_.write_reference(*t, name, false);
            return;
        }
        if (auto* m = dynamic_cast<const IMember*>(member)) {
            if (is_dynamic_member_reference(node_stack_.back())) {
                // A member synthesized for a dynamic access: show its signature on hover, but do not make it a navigation target (there is
                // no real member to jump to), and no occurrence highlight either - it is a distinct synthetic member at every use.
                output_.write_local_reference(name, std::any(m), false, true);
            } else {
                output_.write_reference(*m, name, false);
            }
            return;
        }
    }

    std::any local_definition = get_current_local_definition(&identifier);
    if (local_definition.has_value()) {
        output_.write_local_reference(name, local_definition, true);
        return;
    }

    std::any local_ref = get_current_local_reference();
    if (local_ref.has_value()) {
        output_.write_local_reference(name, local_ref);
        return;
    }

    if (first_using_declaration_ && !last_using_declaration_) {
        output_.mark_fold_start("...", !settings_.expand_using_declarations);
        first_using_declaration_ = false;
    }

    output_.write(name);
}

const ISymbol* TextTokenWriter::get_current_member_reference() {
    AstNode* node = node_stack_.back();
    const ISymbol* symbol = csharp::get_symbol(node);
    const CSharpSlotInfo* node_slot = node->slot();
    const CSharpSlotInfo* slot_kind = node_slot != nullptr ? node_slot->kind() : nullptr;
    if (symbol == nullptr && slot_kind == &Slots::target_expression && is<InvocationExpression>(node->parent())) {
        symbol = csharp::get_symbol(node->parent());
    }
    if (symbol != nullptr && slot_kind == &Slots::type && is<ObjectCreateExpression>(node->parent())) {
        const ISymbol* ctor_symbol = csharp::get_symbol(node->parent());
        if (ctor_symbol != nullptr) symbol = ctor_symbol;
    }

    if (is<IdentifierExpression>(node) && slot_kind == &Slots::target_expression && is<InvocationExpression>(node->parent())) {
        if (auto* member = dynamic_cast<const IMember*>(symbol)) {
            const IType* declaring_type = member->declaring_type();
            if (declaring_type != nullptr && declaring_type->kind() == typesystem::TypeKind::Delegate) return nullptr;
        }
    }
    return filter_member(symbol);
}

const ISymbol* TextTokenWriter::filter_member(const ISymbol* symbol) {
    if (symbol == nullptr) return nullptr;
    if (dynamic_cast<const typesystem::LocalFunctionMethod*>(symbol) != nullptr) return nullptr;
    return symbol;
}

bool TextTokenWriter::is_dynamic_member_reference(AstNode* node) {
    // True if the member reference at this node was synthesized for a dynamic member access/invocation. Such members carry a hover tooltip
    // but must not be navigation targets, since they do not exist in metadata.
    auto* rr = node->annotation<semantics::ResolveResult>();
    if (dynamic_cast<csharp::resolver::DynamicMemberResolveResult*>(rr) != nullptr) return true;
    // The node itself is a dynamic invocation/indexing (a.Method(b), a[b]): its parentheses/brackets carry the synthesized member.
    if (dynamic_cast<csharp::resolver::DynamicInvocationResolveResult*>(rr) != nullptr) return true;
    const CSharpSlotInfo* node_slot = node->slot();
    const CSharpSlotInfo* slot_kind = node_slot != nullptr ? node_slot->kind() : nullptr;
    if (slot_kind == &Slots::target_expression && is<InvocationExpression>(node->parent()) &&
        dynamic_cast<csharp::resolver::DynamicInvocationResolveResult*>(node->parent()->annotation<semantics::ResolveResult>()) != nullptr)
        return true;
    // new T(dynamicArg): the object creation (or its type-name slot) is backed by a dynamic newobj, whose synthesized constructor has no
    // metadata to navigate to.
    if (node->annotation<il::DynamicInvokeConstructorInstruction>() != nullptr) return true;
    if (slot_kind == &Slots::type && node->parent() != nullptr && node->parent()->annotation<il::DynamicInvokeConstructorInstruction>() != nullptr)
        return true;
    return false;
}

std::any TextTokenWriter::get_current_local_reference() {
    AstNode* node = node_stack_.back();
    if (auto* rr = node->annotation<csharp::ILVariableResolveResult>()) {
        if (rr->variable != nullptr) return std::any(rr->variable);
    }

    if (auto* let_clause_variable = node->annotation<csharp::transforms::LetIdentifierAnnotation>()) return std::any(let_clause_variable);

    if (auto* goto_statement = as<GotoStatement>(node)) {
        const IMethod* method = enclosing_method();
        if (method != nullptr) return std::any(method->to_string() + goto_statement->label());
    }

    // Local-function references are recorded with the unspecialized definition, so that generic use sites (which carry specialized methods)
    // and the declaration all share equal reference objects and can be matched for highlighting.
    const CSharpSlotInfo* node_slot = node->slot();
    if (node_slot != nullptr && node_slot->kind() == &Slots::target_expression && is<InvocationExpression>(node->parent())) {
        const ISymbol* symbol = csharp::get_symbol(node->parent());
        if (auto* local_function = dynamic_cast<const typesystem::LocalFunctionMethod*>(symbol)) {
            return std::any(local_function->member_definition());
        }
    }

    // Method-group references to local functions (delegate conversions) are annotated with a MethodGroupResolveResult, which get_symbol
    // does not surface. A local function cannot be overloaded, so such a group contains exactly one method.
    if (auto* method_group = dynamic_cast<csharp::resolver::MethodGroupResolveResult*>(csharp::get_resolve_result(node))) {
        auto methods = method_group->methods();
        if (!methods.empty()) {
            if (auto* local_function = dynamic_cast<const typesystem::LocalFunctionMethod*>(methods.front())) {
                return std::any(local_function->member_definition());
            }
        }
    }

    return std::any();
}

std::any TextTokenWriter::get_current_local_definition(Identifier* id) {
    AstNode* node = node_stack_.back();
    if (is<Identifier>(node) && node->parent() != nullptr) node = node->parent();

    if (is<ParameterDeclaration>(node) || is<VariableInitializer>(node) || is<CatchClause>(node) || is<VariableDesignation>(node)) {
        auto* rr = node->annotation<csharp::ILVariableResolveResult>();
        if (rr != nullptr && rr->variable != nullptr) return std::any(rr->variable);
    }

    const CSharpSlotInfo* id_slot = id->slot();
    const CSharpSlotInfo* id_slot_kind = id_slot != nullptr ? id_slot->kind() : nullptr;
    if (id_slot_kind == &Slots::into_identifier || id_slot_kind == &Slots::join_identifier) {
        auto* rr = id->annotation<csharp::ILVariableResolveResult>();
        if (rr != nullptr && rr->variable != nullptr) return std::any(rr->variable);
    }

    if (is<QueryLetClause>(node)) {
        auto* variable = node->annotation<csharp::transforms::LetIdentifierAnnotation>();
        if (variable != nullptr) return std::any(variable);
    }

    if (auto* label = as<LabelStatement>(node)) {
        const IMethod* method = enclosing_method();
        if (method != nullptr) return std::any(method->to_string() + label->label());
    }

    if (is<MethodDeclaration>(node) && is<LocalFunctionDeclarationStatement>(node->parent())) {
        auto* local_function = dynamic_cast<semantics::MemberResolveResult*>(csharp::get_resolve_result(node->parent()));
        if (local_function != nullptr) return std::any(local_function->member()->member_definition());
    }

    return std::any();
}

const ISymbol* TextTokenWriter::get_current_definition() {
    if (node_stack_.empty()) return nullptr;

    AstNode* node = node_stack_.back();
    if (is<Identifier>(node)) node = node->parent();
    if (is_definition(node)) return csharp::get_symbol(node);

    return nullptr;
}

void TextTokenWriter::write_keyword(std::string_view keyword) {
    // To make reference for 'this' and 'base' keywords in the ClassName():this() expression
    if (keyword == "this" || keyword == "base") {
        if (auto* initializer = as<ConstructorInitializer>(node_stack_.back())) {
            if (auto* member = dynamic_cast<const IMember*>(csharp::get_symbol(initializer))) {
                output_.write_reference(*member, keyword);
                return;
            }
        }
        // As primary expressions, 'this' and 'base' reference the current and the base type respectively, matching IDE go-to-definition
        // behavior.
        if (is<ThisReferenceExpression>(node_stack_.back()) || is<BaseReferenceExpression>(node_stack_.back())) {
            const IType* type = csharp::get_resolve_result(node_stack_.back())->type();
            if (type->kind() != typesystem::TypeKind::Unknown) {
                output_.write_reference(*type, keyword);
                return;
            }
        }
    }
    // Make the 'override' modifier a reference to the nearest overridden member, so that go-to-definition on 'override' navigates to the
    // base member. Modifier tokens are written without being pushed onto the node stack, so the top of the stack is the declaration the
    // modifier belongs to.
    if (keyword == "override") {
        if (auto* entity_declaration = as<EntityDeclaration>(node_stack_.back())) {
            auto* override_member = dynamic_cast<const IMember*>(csharp::get_symbol(entity_declaration));
            if (override_member != nullptr && override_member->is_override()) {
                const IMember* base_member = typesystem::inheritance_helper::get_base_member(override_member);
                if (base_member != nullptr) {
                    output_.write_reference(*base_member, keyword);
                    return;
                }
            }
        }
    }
    output_.write(keyword);
}

bool TextTokenWriter::needs_fold(AstNode* node) {
    if (node == nullptr) return false;

    if (is<EntityDeclaration>(node)) return true;

    if (is<BlockStatement>(node)) {
        AstNode* parent = node->parent();
        if (is<EntityDeclaration>(parent) || is<LocalFunctionDeclarationStatement>(parent) || is<AnonymousMethodExpression>(parent) ||
            is<LambdaExpression>(parent))
            return true;
    }

    return false;
}

void TextTokenWriter::write_token(std::string_view token) {
    if (token == "{") {
        // Interpolation braces ("{" inside an interpolated string) are not structural braces: they neither fold nor affect the within-type
        // brace level.
        if (is<Interpolation>(peek_or_null())) {
            output_.write("{");
            return;
        }
        if (brace_level_within_type_ >= 0 || is<TypeDeclaration>(node_stack_.back())) brace_level_within_type_++;
        if (needs_fold(peek_or_null()) || settings_.fold_braces)
            output_.mark_fold_start("...", !settings_.expand_member_definitions && brace_level_within_type_ == 1, brace_level_within_type_ == 1);
        output_.write("{");
    } else if (token == "}") {
        output_.write('}');
        if (is<Interpolation>(peek_or_null())) return;
        if (needs_fold(peek_or_null()) || settings_.fold_braces) output_.mark_fold_end();
        if (brace_level_within_type_ >= 0) brace_level_within_type_--;
    } else {
        // Attach member reference to token only if there's no identifier in the current node.
        const ISymbol* member = get_current_member_reference();
        AstNode* node = node_stack_.back();
        if (member != nullptr && node->get_child(Slots::identifier) == nullptr) {
            if (auto* t = dynamic_cast<const IType*>(member)) {
                output_.write_reference(*t, token, false);
                return;
            }
            if (auto* m = dynamic_cast<const IMember*>(member)) {
                if (is_dynamic_member_reference(node))
                    output_.write_local_reference(token, std::any(m), false, true);
                else
                    output_.write_reference(*m, token, false);
                return;
            }
        } else {
            output_.write(token);
        }
    }
}

void TextTokenWriter::new_line() {
    if (!first_using_declaration_ && last_using_declaration_) {
        output_.mark_fold_end();
        last_using_declaration_ = false;
    }
    output_.write_line();
}

void TextTokenWriter::write_comment(CommentType comment_type, std::string_view content) {
    switch (comment_type) {
        case CommentType::SingleLine:
            output_.write("//");
            output_.write_line(content);
            break;
        case CommentType::MultiLine:
            output_.write("/*");
            output_.write(content);
            output_.write("*/");
            break;
        case CommentType::Documentation: {
            // Only a following documentation comment continues the fold: a regular comment in the same trivia list must not keep the fold
            // open (it never calls MarkFoldEnd).
            AstNode* next = node_stack_.back()->next_sibling();
            auto* next_comment = as<Comment>(next);
            bool is_last_line = !(next_comment != nullptr && next_comment->comment_type() == CommentType::Documentation);
            if (!in_documentation_comment_ && !is_last_line) {
                in_documentation_comment_ = true;
                output_.mark_fold_start("///" + std::string(content), !settings_.expand_xml_documentation_comments);
            }
            output_.write("///");
            output_.write(content);
            if (in_documentation_comment_ && is_last_line) {
                in_documentation_comment_ = false;
                output_.mark_fold_end();
            }
            output_.write_line();
            break;
        }
        default:
            output_.write(content);
            break;
    }
}

void TextTokenWriter::write_pre_processor_directive(PreProcessorDirectiveType type, std::string_view argument) {
    // pre-processor directive must start on its own line
    output_.write('#');
    output_.write(TextWriterTokenWriter::pre_processor_directive_name(type));
    if (!argument.empty()) {
        output_.write(' ');
        output_.write(argument);
    }
    output_.write_line();
}

void TextTokenWriter::write_primitive_value(const typesystem::ConstantValue& value, LiteralFormat format) {
    TextOutputWriter text_writer(output_);
    TextWriterTokenWriter(text_writer).write_primitive_value(value, format);
}

void TextTokenWriter::write_interpolated_text(std::string_view text) { output_.write(TextWriterTokenWriter::convert_string(text)); }

void TextTokenWriter::write_primitive_type(std::string_view type) {
    if (type == "new") {
        output_.write(type);
        output_.write("()");
    } else if (type == "dynamic") {
        // dynamic has no metadata type definition; emit it as a hover-only reference (no navigation target, no occurrence highlight) so it
        // can still carry a tooltip.
        output_.write_local_reference(type, std::any(static_cast<const IType*>(&typesystem::SpecialType::dynamic())), false, true);
    } else if (type == "bool" || type == "byte" || type == "sbyte" || type == "short" || type == "ushort" || type == "int" || type == "uint" ||
               type == "long" || type == "ulong" || type == "float" || type == "double" || type == "decimal" || type == "char" ||
               type == "string" || type == "object") {
        AstNode* node = node_stack_.back();
        const CSharpSlotInfo* node_slot = node->slot();
        const ISymbol* symbol;
        if (node_slot != nullptr && node_slot->kind() == &Slots::type && is<ObjectCreateExpression>(node->parent())) {
            symbol = csharp::get_symbol(node->parent());
        } else {
            symbol = csharp::get_symbol(node_stack_.back());
        }
        if (symbol == nullptr) {
            output_.write(type);
            return;
        }
        if (auto* t = dynamic_cast<const IType*>(symbol)) {
            output_.write_reference(*t, type, false);
            return;
        }
        if (auto* m = dynamic_cast<const IMember*>(symbol)) {
            output_.write_reference(*m, type, false);
            return;
        }
    } else {
        output_.write(type);
    }
}

bool TextTokenWriter::is_using_declaration(AstNode* node) { return is<UsingDeclaration>(node) || is<UsingAliasDeclaration>(node); }

void TextTokenWriter::start_node(AstNode& node) {
    if (node_stack_.empty()) {
        if (is_using_declaration(&node)) {
            first_using_declaration_ = !is_using_declaration(node.prev_sibling());
            last_using_declaration_ = !is_using_declaration(node.next_sibling());
        } else {
            first_using_declaration_ = false;
            last_using_declaration_ = false;
        }
    }
    if (is<EntityDeclaration>(&node)) {
        // The declaration's logical region starts here, before its documentation comments and attributes are written; the body fold marked
        // later refers back to this position for group toggling in the UI.
        output_.mark_definition_start();
    }
    node_stack_.push_back(&node);
}

void TextTokenWriter::end_node(AstNode& node) {
    AstNode* popped = node_stack_.back();
    node_stack_.pop_back();
    if (popped != &node) throw std::logic_error("end_node: not the node last started");
}

bool TextTokenWriter::is_definition(AstNode*& node) {
    if (is<EntityDeclaration>(node) && !is<LocalFunctionDeclarationStatement>(node->parent())) return true;
    if (is<VariableInitializer>(node) && (is<FieldDeclaration>(node->parent()) || is<EventDeclaration>(node->parent()))) {
        node = node->parent();
        return true;
    }
    if (is<FixedVariableInitializer>(node) && is<FixedFieldDeclaration>(node->parent())) {
        node = node->parent();
        return true;
    }
    return false;
}

}  // namespace cb::dotnet::decompiler
