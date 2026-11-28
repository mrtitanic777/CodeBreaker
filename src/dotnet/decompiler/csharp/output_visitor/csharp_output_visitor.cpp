// dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.cpp -- CSharpOutputVisitor: the machinery (nodes, commas, tokens, spaces,
// new lines, braces, keywords) and the pieces the node visits share. The visit methods are in csharp_output_visitor_expressions.cpp,
// csharp_output_visitor_statements.cpp and csharp_output_visitor_members.cpp. See csharp_output_visitor.hpp.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.OutputVisitor: CSharpOutputVisitor), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"

#include <cassert>
#include <string>
#include <unordered_set>

#include "dotnet/decompiler/csharp/output_visitor/formatting_options_factory.hpp"
#include "dotnet/decompiler/csharp/output_visitor/insert_required_spaces_decorator.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"

namespace cb::dotnet::decompiler::csharp::output_visitor {

using namespace syntax;

// ---- AstNode::to_string ------------------------------------------------------------------------------------------------------------

namespace {

// AstNode.ToString(formattingOptions): the node written with a CSharpOutputVisitor (Mono options when none are given).
std::string node_to_string(const AstNode& node, const CSharpFormattingOptions* formatting_options) {
    StringTextWriter w;
    CSharpOutputVisitor visitor(w, formatting_options != nullptr ? *formatting_options : FormattingOptionsFactory::create_mono());
    const_cast<AstNode&>(node).accept_visitor(visitor);
    return w.take_text();
}

}  // namespace

void CSharpOutputVisitor::install_to_string_hook() { AstNode::set_to_string_function(&node_to_string); }

namespace {

// Installs the hook when the program starts, so that to_string works wherever the output visitor is linked in.
const bool to_string_hook_installed = (CSharpOutputVisitor::install_to_string_hook(), true);

}  // namespace

CSharpOutputVisitor::CSharpOutputVisitor(TextWriter& text_writer, const CSharpFormattingOptions& formatting_policy)
    : owned_writer_(TokenWriter::create(text_writer, formatting_policy.indentation_string)),
      writer_(*owned_writer_),
      policy_(formatting_policy) {
    install_to_string_hook();
}

CSharpOutputVisitor::CSharpOutputVisitor(TokenWriter& writer, const CSharpFormattingOptions& formatting_policy)
    : owned_writer_(std::make_unique<InsertRequiredSpacesDecorator>(writer)), writer_(*owned_writer_), policy_(formatting_policy) {
    install_to_string_hook();
}

// ---- StartNode/EndNode ----------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::start_node(AstNode* node) {
    assert(container_stack_.empty() || node->parent() == container_stack_.back() || is<PatternPlaceholderNode>(container_stack_.back()));
    container_stack_.push_back(node);
    writer_.start_node(*node);
    for (Trivia* trivia : node->leading_trivia()) trivia->accept_visitor(*this);
}

void CSharpOutputVisitor::end_node(AstNode* node) {
    for (Trivia* trivia : node->trailing_trivia()) trivia->accept_visitor(*this);
    assert(node == container_stack_.back());
    container_stack_.pop_back();
    writer_.end_node(*node);
}

// ---- Comma ------------------------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::comma(AstNode*, bool no_space_after_comma) {
    space(policy_.space_before_bracket_comma);
    writer_.write_token(",");
    is_after_space_ = false;
    space(!no_space_after_comma && policy_.space_after_bracket_comma);
}

void CSharpOutputVisitor::write_comma_separated_list(const std::vector<AstNode*>& list) {
    bool is_first = true;
    for (AstNode* node : list) {
        if (is_first) {
            is_first = false;
        } else {
            comma(node);
        }
        node->accept_visitor(*this);
    }
}

void CSharpOutputVisitor::write_comma_separated_list_in_parenthesis(const std::vector<AstNode*>& list, bool space_within) {
    l_par();
    if (!list.empty()) {
        space(space_within);
        write_comma_separated_list(list);
        space(space_within);
    }
    r_par();
}

void CSharpOutputVisitor::write_comma_separated_list_in_brackets(const std::vector<AstNode*>& list, bool space_within) {
    write_token(Tokens::l_bracket);
    if (!list.empty()) {
        space(space_within);
        write_comma_separated_list(list);
        space(space_within);
    }
    write_token(Tokens::r_bracket);
}

void CSharpOutputVisitor::write_comma_separated_list_in_brackets(const std::vector<AstNode*>& list) {
    write_token(Tokens::l_bracket);
    if (!list.empty()) {
        space(policy_.spaces_within_brackets);
        write_comma_separated_list(list);
        space(policy_.spaces_within_brackets);
    }
    write_token(Tokens::r_bracket);
}

// ---- Write tokens -------------------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::write_keyword(std::string_view keyword) {
    writer_.write_keyword(keyword);
    is_at_start_of_line_ = false;
    is_after_space_ = false;
}

void CSharpOutputVisitor::write_identifier(Identifier* identifier) {
    writer_.write_identifier(*identifier);
    is_at_start_of_line_ = false;
    is_after_space_ = false;
}

void CSharpOutputVisitor::write_identifier(std::string_view identifier) {
    AstType::create(identifier)->accept_visitor(*this);
    is_at_start_of_line_ = false;
    is_after_space_ = false;
}

void CSharpOutputVisitor::write_token(std::string_view token) {
    writer_.write_token(token);
    is_at_start_of_line_ = false;
    is_after_space_ = false;
}

void CSharpOutputVisitor::l_par() { write_token(Tokens::l_par); }

void CSharpOutputVisitor::r_par() { write_token(Tokens::r_par); }

void CSharpOutputVisitor::semicolon() {
    const CSharpSlotInfo* kind = slot_kind_of(container_stack_.back());

    auto skip_token = [&]() { return kind == &Slots::for_initializer || kind == &Slots::iterator || kind == &Slots::resource_acquisition; };

    auto skip_new_line = [&]() {
        auto* accessor = as<Accessor>(container_stack_.back());
        if (accessor == nullptr) return false;
        if (!(kind == &Slots::getter || kind == &Slots::setter)) return false;
        bool is_auto_property = accessor->body() == nullptr && accessor->attributes().empty() &&
                                policy_.auto_property_formatting == PropertyFormatting::SingleLine;
        return is_auto_property;
    };

    if (!skip_token()) {
        write_token(Tokens::semicolon);
        if (!skip_new_line())
            new_line();
        else
            space();
    }
}

void CSharpOutputVisitor::space(bool add_space) {
    if (add_space && !is_after_space_) {
        writer_.space();
        is_after_space_ = true;
    }
}

void CSharpOutputVisitor::new_line() {
    writer_.new_line();
    is_at_start_of_line_ = true;
    is_after_space_ = false;
}

int CSharpOutputVisitor::get_call_chain_length_limited(MemberReferenceExpression* expr) {
    int call_chain_length = 0;
    MemberReferenceExpression* node = expr;

    while (true) {
        auto* invocation = as<InvocationExpression>(node->target());
        if (invocation == nullptr) break;
        auto* mre = as<MemberReferenceExpression>(invocation->target());
        if (mre == nullptr || call_chain_length >= 4) break;
        node = mre;
        call_chain_length++;
    }
    return call_chain_length;
}

int CSharpOutputVisitor::should_insert_new_line_when_in_method_call_chain(MemberReferenceExpression* expr) {
    int call_chain_length = get_call_chain_length_limited(expr);
    if (call_chain_length < 3) return 0;
    AstNode* enclosing = expr->get_parent([](AstNode* n) { return is<Statement>(n) || is<LambdaExpression>(n) || is<InterpolatedStringContent>(n); });
    if (is<InterpolatedStringContent>(enclosing)) return 0;
    return call_chain_length;
}

bool CSharpOutputVisitor::insert_new_line_when_in_method_call_chain(MemberReferenceExpression* expr) {
    int call_chain_length = should_insert_new_line_when_in_method_call_chain(expr);
    if (call_chain_length == 0) return false;
    if (call_chain_length == 3) writer_.indent();
    writer_.new_line();

    is_at_start_of_line_ = true;
    is_after_space_ = false;
    return true;
}

void CSharpOutputVisitor::open_brace(BraceStyle style, bool new_line_after) {
    switch (style) {
        case BraceStyle::EndOfLine:
        case BraceStyle::BannerStyle:
            if (!is_at_start_of_line_) space();
            write_token("{");
            break;
        case BraceStyle::EndOfLineWithoutSpace:
            write_token("{");
            break;
        case BraceStyle::NextLine:
            if (!is_at_start_of_line_) new_line();
            write_token("{");
            break;
        case BraceStyle::NextLineShifted:
            new_line();
            writer_.indent();
            write_token("{");
            new_line();
            return;
        case BraceStyle::NextLineShifted2:
            new_line();
            writer_.indent();
            write_token("{");
            break;
        default:
            throw std::out_of_range("BraceStyle");
    }
    if (new_line_after) {
        writer_.indent();
        new_line();
    }
}

void CSharpOutputVisitor::close_brace(BraceStyle style, bool unindent) {
    switch (style) {
        case BraceStyle::EndOfLine:
        case BraceStyle::EndOfLineWithoutSpace:
        case BraceStyle::NextLine:
            if (unindent) writer_.unindent();
            write_token("}");
            break;
        case BraceStyle::BannerStyle:
        case BraceStyle::NextLineShifted:
            write_token("}");
            if (unindent) writer_.unindent();
            break;
        case BraceStyle::NextLineShifted2:
            if (unindent) writer_.unindent();
            write_token("}");
            if (unindent) writer_.unindent();
            break;
        default:
            throw std::out_of_range("BraceStyle");
    }
}

// ---- IsKeyword Test -------------------------------------------------------------------------------------------------------------

namespace {

const std::unordered_set<std::string_view>& unconditional_keywords() {
    static const std::unordered_set<std::string_view> set = {
        "abstract", "as",       "base",     "bool",     "break",     "byte",     "case",      "catch",   "char",     "checked",  "class",
        "const",    "continue", "decimal",  "default",  "delegate",  "do",       "double",    "else",    "enum",     "event",    "explicit",
        "extern",   "false",    "finally",  "fixed",    "float",     "for",      "foreach",   "goto",    "if",       "implicit", "in",
        "int",      "interface", "internal", "is",      "lock",      "long",     "namespace", "new",     "null",     "object",   "operator",
        "out",      "override", "params",   "private",  "protected", "public",   "readonly",  "ref",     "return",   "sbyte",    "sealed",
        "short",    "sizeof",   "stackalloc", "static", "string",    "struct",   "switch",    "this",    "throw",    "true",     "try",
        "typeof",   "uint",     "ulong",    "unchecked", "unsafe",   "ushort",   "using",     "virtual", "void",     "volatile", "while"};
    return set;
}

const std::unordered_set<std::string_view>& query_keywords() {
    static const std::unordered_set<std::string_view> set = {"from", "where", "join",  "on",         "equals", "into", "let",
                                                             "orderby", "ascending", "descending", "select", "group", "by"};
    return set;
}

constexpr std::size_t max_keyword_length = 10;  // the longest of the two sets ("stackalloc", "descending")

}  // namespace

bool CSharpOutputVisitor::is_keyword(std::string_view identifier, const AstNode* context) {
    if (identifier.size() > max_keyword_length || identifier.size() < 2 || static_cast<unsigned char>(identifier[0]) < 'a') {
        return false;
    }
    if (unconditional_keywords().contains(identifier)) {
        return true;
    }
    if (query_keywords().contains(identifier)) {
        if (context == nullptr) return true;
        for (AstNode* ancestor : context->ancestors()) {
            if (is<QueryExpression>(ancestor)) return true;
        }
        return false;
    }
    if (identifier == "await") {
        if (context == nullptr) return true;
        for (AstNode* ancestor : context->ancestors()) {
            if (auto* lambda = as<LambdaExpression>(ancestor)) {
                return lambda->is_async();
            }
            if (auto* anonymous_method = as<AnonymousMethodExpression>(ancestor)) {
                return anonymous_method->is_async();
            }
            if (auto* entity = as<EntityDeclaration>(ancestor)) {
                return (entity->modifiers() & Modifiers::Async) == Modifiers::Async;
            }
        }
    }
    return false;
}

// ---- Write constructs -------------------------------------------------------------------------------------------------------------

void CSharpOutputVisitor::write_type_arguments(const std::vector<AstNode*>& type_arguments) {
    if (!type_arguments.empty()) {
        write_token(Tokens::l_chevron);
        write_comma_separated_list(type_arguments);
        write_token(Tokens::r_chevron);
    }
}

void CSharpOutputVisitor::write_type_parameters(const std::vector<AstNode*>& type_parameters) {
    if (!type_parameters.empty()) {
        write_token(Tokens::l_chevron);
        write_comma_separated_list(type_parameters);
        write_token(Tokens::r_chevron);
    }
}

void CSharpOutputVisitor::write_modifiers(Modifiers modifiers) {
    for (Modifiers modifier : CSharpModifiers::all_modifiers()) {
        if ((modifiers & modifier) == modifier) {
            write_keyword(CSharpModifiers::get_modifier_name(modifier));
            space();
        }
    }
}

void CSharpOutputVisitor::write_qualified_identifier(const std::vector<Identifier*>& identifiers) {
    bool first = true;
    for (Identifier* ident : identifiers) {
        if (first) {
            first = false;
        } else {
            writer_.write_token(".");
        }
        writer_.write_identifier(*ident);
    }
}

void CSharpOutputVisitor::write_embedded_statement(Statement* embedded_statement, NewLinePlacement nlp) {
    if (embedded_statement == nullptr) {
        new_line();
        return;
    }
    BlockStatement* block = as<BlockStatement>(embedded_statement);
    if (block != nullptr) {
        write_block(block, policy_.statement_brace_style);
        if (nlp == NewLinePlacement::SameLine) {
            space();  // if not a trailing newline, then at least a trailing space
        } else {
            new_line();
        }
    } else {
        new_line();
        writer_.indent();
        embedded_statement->accept_visitor(*this);
        writer_.unindent();
    }
}

void CSharpOutputVisitor::write_method_body(BlockStatement* body, BraceStyle style, bool) {
    // (not upstream) `=> expr;` for a body of one `return expr;` or one expression statement its declaration asks to be written so
    auto* entity = body != nullptr ? as<EntityDeclaration>(body->parent()) : nullptr;
    Statement* only = entity != nullptr && entity->is_expression_bodied() && body->statements().count() == 1 ? body->statements()[0] : nullptr;
    Expression* value = nullptr;
    if (auto* ret = as<ReturnStatement>(only)) value = ret->expression();
    else if (auto* es = as<ExpressionStatement>(only)) value = es->expression();
    if (value != nullptr) {
        start_node(body);
        start_node(only);
        space();
        write_token(Tokens::arrow);
        space();
        value->accept_visitor(*this);
        semicolon();
        end_node(only);
        end_node(body);
        return;
    }
    if (body == nullptr) {
        semicolon();
    } else {
        write_block(body, style);
        new_line();
    }
}

void CSharpOutputVisitor::write_attributes(const std::vector<AstNode*>& attributes) {
    for (AstNode* attr : attributes) {
        attr->accept_visitor(*this);
    }
}

void CSharpOutputVisitor::write_private_implementation_type(AstType* private_implementation_type) {
    if (private_implementation_type != nullptr) {
        private_implementation_type->accept_visitor(*this);
        write_token(Tokens::dot);
    }
}

// ---- IsPostfixOperator and the initializer predicates ----------------------------------------------------------------------------

bool CSharpOutputVisitor::can_be_confused_with_object_initializer(Expression* expr) {
    auto* ae = as<AssignmentExpression>(expr);
    return ae != nullptr && ae->operator_() == AssignmentOperatorType::Assign;
}

bool CSharpOutputVisitor::is_object_or_collection_initializer(AstNode* node) {
    if (!is<ArrayInitializerExpression>(node)) {
        return false;
    }
    if (is<ObjectCreateExpression>(node->parent())) {
        return slot_kind_of(node) == &Slots::initializer;
    }
    if (is<NamedExpression>(node->parent())) {
        return slot_kind_of(node) == &Slots::expression;
    }
    return false;
}

const CSharpSlotInfo* CSharpOutputVisitor::slot_kind_of(const AstNode* node) {
    const CSharpSlotInfo* slot = node->slot();
    return slot != nullptr ? slot->kind() : nullptr;
}

}  // namespace cb::dotnet::decompiler::csharp::output_visitor
