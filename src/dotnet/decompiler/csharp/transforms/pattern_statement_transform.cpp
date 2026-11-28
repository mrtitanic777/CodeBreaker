// dotnet/decompiler/csharp/transforms/pattern_statement_transform.cpp -- see pattern_statement_transform.hpp.
#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_statement_transform.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/any_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/backreference.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/choice.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/identifier_expression_backreference.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/named_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/optional_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/repeat.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_arena.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/metadata/method_semantics_lookup.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using il::ILVariable;
using il::VariableKind;
using syntax::pattern_matching::AnyNode;
using syntax::pattern_matching::Backreference;
using syntax::pattern_matching::Choice;
using syntax::pattern_matching::IdentifierExpressionBackreference;
using syntax::pattern_matching::Match;
using syntax::pattern_matching::NamedNode;
using syntax::pattern_matching::OptionalNode;
using syntax::pattern_matching::Pattern;
using syntax::pattern_matching::Repeat;
using typesystem::IEvent;
using typesystem::IField;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IProperty;
using typesystem::IType;
using typesystem::KnownAttribute;
using typesystem::KnownTypeCode;
using typesystem::TypeKind;

namespace {

// ---- small helpers ----------------------------------------------------------------------------------------------------------------

Expression* any_node(const std::string& name = {}) { return Expression::from_pattern(make<AnyNode>(name)); }
Statement* any_statement(const std::string& name = {}) { return Statement::from_pattern(make<AnyNode>(name)); }
Expression* any_identifier() { return make<IdentifierExpression>(std::string(Pattern::any_string)); }
Expression* named(const std::string& name, Expression* child) { return Expression::from_pattern(make<NamedNode>(name, child)); }
Expression* int_literal(std::int32_t value) { return make<PrimitiveExpression>(typesystem::ConstantValue(value)); }
Expression* backref(const std::string& name) { return Expression::from_pattern(make<IdentifierExpressionBackreference>(name)); }

// Whether the (possibly null) type is the given known type.
bool is_known(const IType* type, KnownTypeCode code) { return type != nullptr && typesystem::is_known_type(type, code); }

// upstream's int.TryParse(value.ToString(), out i) for a literal's value (an integer or a string of digits).
bool try_parse_int_value(const typesystem::ConstantValue& value, int* result) {
    std::string text;
    if (const auto* s = std::get_if<std::u16string>(&value)) {
        for (char16_t c : *s) {
            if (c > 0x7f) return false;
            text.push_back(static_cast<char>(c));
        }
    } else if (std::holds_alternative<std::int8_t>(value)) {
        text = std::to_string(std::get<std::int8_t>(value));
    } else if (std::holds_alternative<std::uint8_t>(value)) {
        text = std::to_string(std::get<std::uint8_t>(value));
    } else if (std::holds_alternative<std::int16_t>(value)) {
        text = std::to_string(std::get<std::int16_t>(value));
    } else if (std::holds_alternative<std::uint16_t>(value)) {
        text = std::to_string(std::get<std::uint16_t>(value));
    } else if (std::holds_alternative<std::int32_t>(value)) {
        text = std::to_string(std::get<std::int32_t>(value));
    } else if (std::holds_alternative<std::uint32_t>(value)) {
        text = std::to_string(std::get<std::uint32_t>(value));
    } else if (std::holds_alternative<std::int64_t>(value)) {
        text = std::to_string(std::get<std::int64_t>(value));
    } else if (std::holds_alternative<std::uint64_t>(value)) {
        text = std::to_string(std::get<std::uint64_t>(value));
    } else {
        return false;
    }
    if (text.empty()) return false;
    std::size_t pos = 0;
    try {
        long long parsed = std::stoll(text, &pos);
        if (pos != text.size() || parsed < INT32_MIN || parsed > INT32_MAX) return false;
        *result = static_cast<int>(parsed);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

void remove_compiler_generated_attribute(AstNodeCollection<AttributeSection>& attribute_sections,
                                         const std::vector<std::string>& attributes_to_remove) {
    for (AttributeSection* section : attribute_sections.to_vector()) {
        for (Attribute* attr : section->attributes().to_vector()) {
            const auto* tr = dynamic_cast<const IType*>(get_symbol(attr->type()));
            if (tr != nullptr && std::find(attributes_to_remove.begin(), attributes_to_remove.end(), tr->full_name()) != attributes_to_remove.end()) {
                attr->remove();
            }
        }
        if (section->attributes().count() == 0) section->remove();
    }
}

// This matches the following patterns: <Property>k__BackingField (used by C#), _Property (used by VB).
bool name_could_be_backing_field_of_automatic_property(const std::string& name, std::string* property_name) {
    static const std::string prefix = "<";
    static const std::string suffix = ">k__BackingField";
    if (name.size() > prefix.size() + suffix.size() && name.compare(0, prefix.size(), prefix) == 0 &&
        name.compare(name.size() - suffix.size(), suffix.size(), suffix) == 0) {
        *property_name = name.substr(prefix.size(), name.size() - prefix.size() - suffix.size());
        return true;
    }
    if (name.size() > 1 && name[0] == '_') {
        *property_name = name.substr(1);
        return true;
    }
    return false;
}

bool is_event_backing_field_declaration(FieldDeclaration* fd, const IEvent* ev) {
    if (fd->variables().count() > 1) return false;
    const auto* f = dynamic_cast<const IField*>(get_symbol(fd));
    if (f == nullptr) return false;
    const auto* module = dynamic_cast<const typesystem::MetadataModule*>(f->parent_module());
    if (module == nullptr) return false;
    if (f->accessibility() != typesystem::Accessibility::Private) return false;
    if (!typesystem::type_equals(ev->return_type(), f->return_type())) return false;
    cb::dotnet::metadata::EventDefinitionHandle event_handle;
    return module->metadata_file()->property_and_event_backing_field_lookup().is_event_backing_field(
        cb::dotnet::metadata::FieldDefinitionHandle(f->metadata_token()), event_handle);
}

// ---- the patterns -----------------------------------------------------------------------------------------------------------------

// $variable = $initializer;
AstNode* variable_assign_pattern() {
    static AstNode* const pattern = build_pattern([] {
        return static_cast<AstNode*>(make<ExpressionStatement>(
            make<AssignmentExpression>(named("variable", any_identifier()), any_node("initializer"))));
    });
    return pattern;
}

WhileStatement* for_pattern() {
    static WhileStatement* const pattern = build_pattern([] {
        auto* condition = make<BinaryOperatorExpression>(named("ident", any_identifier()), BinaryOperatorType::Any, any_node("endExpr"));
        auto* body = make<BlockStatement>();
        body->add(Statement::from_pattern(make<Repeat>(make<AnyNode>("statement"))));
        auto* iterator_assignment = make<AssignmentExpression>(Expression::from_pattern(make<Backreference>("ident")), AssignmentOperatorType::Any, any_node());
        body->add(Statement::from_pattern(make<NamedNode>("iterator", make<ExpressionStatement>(iterator_assignment))));
        return make<WhileStatement>(condition, body);
    });
    return pattern;
}

ForStatement* for_on_array_pattern() {
    static ForStatement* const pattern = build_pattern([] {
        auto* p = make<ForStatement>();
        p->initializers().add(make<ExpressionStatement>(make<AssignmentExpression>(named("indexVariable", any_identifier()), int_literal(0))));
        p->set_condition(make<BinaryOperatorExpression>(
            backref("indexVariable"), BinaryOperatorType::LessThan,
            make<MemberReferenceExpression>(named("arrayVariable", any_identifier()), "Length")));
        p->iterators().add(make<ExpressionStatement>(make<AssignmentExpression>(
            backref("indexVariable"), make<BinaryOperatorExpression>(backref("indexVariable"), BinaryOperatorType::Add, int_literal(1)))));
        auto* body = make<BlockStatement>();
        body->add(make<ExpressionStatement>(make<AssignmentExpression>(
            named("itemVariable", any_identifier()),
            make<IndexerExpression>(backref("arrayVariable"), std::initializer_list<Expression*>{backref("indexVariable")}))));
        body->add(Statement::from_pattern(make<Repeat>(make<AnyNode>("statements"))));
        p->set_embedded_statement(body);
        return p;
    });
    return pattern;
}

ForStatement* for_on_inline_array_pattern() {
    static ForStatement* const pattern = build_pattern([] {
        auto* p = make<ForStatement>();
        p->initializers().add(make<ExpressionStatement>(make<AssignmentExpression>(named("indexVariable", any_identifier()), int_literal(0))));
        p->set_condition(make<BinaryOperatorExpression>(
            backref("indexVariable"), BinaryOperatorType::LessThan,
            named("length", make<PrimitiveExpression>(PrimitiveExpression::any_value))));
        p->iterators().add(make<ExpressionStatement>(make<AssignmentExpression>(
            backref("indexVariable"), make<BinaryOperatorExpression>(backref("indexVariable"), BinaryOperatorType::Add, int_literal(1)))));
        auto* body = make<BlockStatement>();
        body->add(make<ExpressionStatement>(make<AssignmentExpression>(named("itemVariable", any_identifier()), Expression::from_pattern(make<NamedNode>("elementAccess", make<AnyNode>())))));
        body->add(Statement::from_pattern(make<Repeat>(make<AnyNode>("statements"))));
        p->set_embedded_statement(body);
        return p;
    });
    return pattern;
}

ForStatement* for_on_array_multi_dim_pattern() {
    static ForStatement* const pattern = build_pattern([] {
        auto* p = make<ForStatement>();
        p->set_condition(make<BinaryOperatorExpression>(named("indexVariable", any_identifier()), BinaryOperatorType::LessThanOrEqual,
                                                         named("upperBoundVariable", any_identifier())));
        p->iterators().add(make<ExpressionStatement>(make<AssignmentExpression>(
            backref("indexVariable"), make<BinaryOperatorExpression>(backref("indexVariable"), BinaryOperatorType::Add, int_literal(1)))));
        auto* body = make<BlockStatement>();
        body->add(any_statement("lowerBoundAssign"));
        body->add(Statement::from_pattern(make<Repeat>(make<AnyNode>("statements"))));
        p->set_embedded_statement(body);
        return p;
    });
    return pattern;
}

// $variable = $collection.GetUpperBound($index);
AstNode* variable_assign_upper_bound_pattern() {
    static AstNode* const pattern = build_pattern([] {
        auto* call = make<InvocationExpression>(make<MemberReferenceExpression>(named("collection", any_identifier()), "GetUpperBound"),
                                                std::initializer_list<Expression*>{named("index", make<PrimitiveExpression>(PrimitiveExpression::any_value))});
        return static_cast<AstNode*>(make<ExpressionStatement>(make<AssignmentExpression>(named("variable", any_identifier()), call)));
    });
    return pattern;
}

// $variable = $collection.GetLowerBound($index);
ExpressionStatement* variable_assign_lower_bound_pattern() {
    static ExpressionStatement* const pattern = build_pattern([] {
        auto* call = make<InvocationExpression>(make<MemberReferenceExpression>(named("collection", any_identifier()), "GetLowerBound"),
                                                std::initializer_list<Expression*>{named("index", make<PrimitiveExpression>(PrimitiveExpression::any_value))});
        return make<ExpressionStatement>(make<AssignmentExpression>(named("variable", any_identifier()), call));
    });
    return pattern;
}

// $variable = $collection[$index1, $index2, ...];
ExpressionStatement* foreach_variable_on_mult_array_assign_pattern() {
    static ExpressionStatement* const pattern = build_pattern([] {
        auto* indexer = make<IndexerExpression>(
            named("collection", any_identifier()),
            std::initializer_list<Expression*>{Expression::from_pattern(make<Repeat>(make<NamedNode>("index", any_identifier())))});
        return make<ExpressionStatement>(make<AssignmentExpression>(named("variable", any_identifier()), indexer));
    });
    return pattern;
}

Accessor* make_property_accessor(bool is_getter) {
    auto* accessor = make<Accessor>();
    accessor->attributes().add(AttributeSection::from_pattern(make<Repeat>(make<AnyNode>())));
    accessor->set_modifiers(Modifiers::Any);
    auto* body = make<BlockStatement>();
    if (is_getter) {
        auto* ret = make<ReturnStatement>();
        ret->set_expression(any_node("fieldReference"));
        body->add(ret);
    } else {
        auto* assignment = make<AssignmentExpression>(Expression::from_pattern(make<Backreference>("fieldReference")), make<IdentifierExpression>("value"));
        body->add(static_cast<Expression*>(assignment));
    }
    accessor->set_body(body);
    return accessor;
}

PropertyDeclaration* make_automatic_property_pattern(bool with_setter) {
    auto* p = make<PropertyDeclaration>();
    p->attributes().add(AttributeSection::from_pattern(make<Repeat>(make<AnyNode>())));
    p->set_modifiers(Modifiers::Any);
    p->set_return_type(AstType::from_pattern(make<AnyNode>()));
    p->set_private_implementation_type(AstType::from_pattern(make<OptionalNode>(make<AnyNode>())));
    p->set_name(std::string(Pattern::any_string));
    p->set_getter(make_property_accessor(true));
    if (with_setter) p->set_setter(make_property_accessor(false));
    return p;
}

PropertyDeclaration* automatic_property_pattern() {
    static PropertyDeclaration* const pattern = build_pattern([] { return make_automatic_property_pattern(true); });
    return pattern;
}

PropertyDeclaration* automatic_readonly_property_pattern() {
    static PropertyDeclaration* const pattern = build_pattern([] { return make_automatic_property_pattern(false); });
    return pattern;
}

BlockStatement* destructor_body_pattern() {
    static BlockStatement* const pattern = build_pattern([] {
        auto* finally_block = make<BlockStatement>();
        finally_block->add(static_cast<Expression*>(
            make<InvocationExpression>(make<MemberReferenceExpression>(make<BaseReferenceExpression>(), "Finalize"))));
        auto* try_catch = make<TryCatchStatement>();
        try_catch->set_try_block(BlockStatement::from_pattern(make<AnyNode>("body")));
        try_catch->set_finally_block(finally_block);
        auto* body = make<BlockStatement>();
        body->add(try_catch);
        return body;
    });
    return pattern;
}

MethodDeclaration* destructor_pattern() {
    static MethodDeclaration* const pattern = build_pattern([] {
        auto* p = make<MethodDeclaration>();
        p->attributes().add(AttributeSection::from_pattern(make<Repeat>(make<AnyNode>())));
        p->set_modifiers(Modifiers::Any);
        p->set_return_type(make<PrimitiveType>("void"));
        p->set_name("Finalize");
        // (not destructor_body_pattern(): a node has one parent, and the body pattern is also matched on its own)
        auto* finally_block = make<BlockStatement>();
        finally_block->add(static_cast<Expression*>(
            make<InvocationExpression>(make<MemberReferenceExpression>(make<BaseReferenceExpression>(), "Finalize"))));
        auto* try_catch = make<TryCatchStatement>();
        try_catch->set_try_block(BlockStatement::from_pattern(make<AnyNode>("body")));
        try_catch->set_finally_block(finally_block);
        auto* body = make<BlockStatement>();
        body->add(try_catch);
        p->set_body(body);
        return p;
    });
    return pattern;
}

TryCatchStatement* try_catch_finally_pattern() {
    static TryCatchStatement* const pattern = build_pattern([] {
        auto* inner = make<TryCatchStatement>();
        inner->set_try_block(BlockStatement::from_pattern(make<AnyNode>()));
        inner->catch_clauses().add(CatchClause::from_pattern(make<Repeat>(make<AnyNode>())));
        auto* try_block = make<BlockStatement>();
        try_block->add(inner);
        auto* p = make<TryCatchStatement>();
        p->set_try_block(try_block);
        p->set_finally_block(BlockStatement::from_pattern(make<AnyNode>()));
        return p;
    });
    return pattern;
}

IfElseStatement* cascading_if_else_pattern() {
    static IfElseStatement* const pattern = build_pattern([] {
        auto* nested = make<IfElseStatement>(any_node(), any_statement(), Statement::from_pattern(make<OptionalNode>(make<AnyNode>())));
        auto* false_block = make<BlockStatement>();
        false_block->add(Statement::from_pattern(make<NamedNode>("nestedIfStatement", nested)));
        return make<IfElseStatement>(any_node(), any_statement(), false_block);
    });
    return pattern;
}

// C# 7.3 pattern based fixed (for value types): reference types are handled by DetectPinnedRegions.IsCustomRefPinPattern
Expression* address_of_pinnable_reference() {
    static Expression* const pattern = build_pattern([] {
        auto* call = make<InvocationExpression>(make<MemberReferenceExpression>(any_node("target"), "GetPinnableReference"));
        return static_cast<Expression*>(make<UnaryOperatorExpression>(UnaryOperatorType::AddressOf, call));
    });
    return pattern;
}

}  // namespace

// ---- Run ------------------------------------------------------------------------------------------------------------------------

const std::vector<std::string>& PatternStatementTransform::attribute_types_to_remove_from_auto_properties() {
    static const std::vector<std::string> types = {"System.Runtime.CompilerServices.CompilerGeneratedAttribute",
                                                   "System.Diagnostics.DebuggerBrowsableAttribute"};
    return types;
}

void PatternStatementTransform::run(AstNode* root_node, TransformContext& context) {
    if (context_ != nullptr) throw std::logic_error("Reentrancy in PatternStatementTransform.Run?");
    struct Cleanup {
        PatternStatementTransform* self;
        ~Cleanup() {
            self->context_ = nullptr;
            self->uninitialize();
            self->declare_variables_.clear_analysis_results();
        }
    } cleanup{this};
    context_ = &context;
    initialize(context);
    declare_variables_.analyze(root_node, *context_);
    root_node->accept_visitor(static_cast<IAstVisitorT<AstNode*>&>(*this));
}

// ---- visitor overrides ------------------------------------------------------------------------------------------------------------

AstNode* PatternStatementTransform::visit_children(AstNode& node) {
    // Go through the children, and keep visiting a node as long as it changes.
    // Because some transforms delete/replace nodes before and after the node being transformed, we rely
    // on the transform's return value to know where we need to keep iterating.
    for (AstNode* child = node.first_child(); child != nullptr; child = child->next_sibling()) {
        AstNode* old_child;
        do {
            old_child = child;
            child = child->accept_visitor(static_cast<IAstVisitorT<AstNode*>&>(*this));
            assert(child != nullptr && child->parent() == &node);
        } while (child != old_child);
    }
    return &node;
}

AstNode* PatternStatementTransform::visit_expression_statement(ExpressionStatement& expression_statement) {
    AstNode* result = transform_foreach_on_multi_dim_array(&expression_statement);
    if (result != nullptr) return result;
    result = transform_for(&expression_statement);
    if (result != nullptr) return result;
    return Base::visit_expression_statement(expression_statement);
}

AstNode* PatternStatementTransform::visit_for_statement(ForStatement& for_statement) {
    AstNode* result = transform_foreach_on_array(&for_statement);
    if (result != nullptr) return result;
    result = transform_foreach_on_inline_array(&for_statement);
    if (result != nullptr) return result;
    return Base::visit_for_statement(for_statement);
}

AstNode* PatternStatementTransform::visit_if_else_statement(IfElseStatement& if_else_statement) {
    AstNode* simplified_if_else = simplify_cascading_if_else_statements(&if_else_statement);
    if (simplified_if_else != nullptr) return simplified_if_else;
    return Base::visit_if_else_statement(if_else_statement);
}

AstNode* PatternStatementTransform::visit_property_declaration(PropertyDeclaration& property_declaration) {
    if (context_->settings.automatic_properties &&
        (property_declaration.setter() != nullptr || context_->settings.getter_only_automatic_properties)) {
        AstNode* result = transform_automatic_property(&property_declaration);
        if (result != nullptr) return result;
    }
    return Base::visit_property_declaration(property_declaration);
}

AstNode* PatternStatementTransform::visit_event_declaration(EventDeclaration& event_declaration) {
    // A field-like event declaration hides its backing field; remove the field declaration
    // if it was emitted because other members reference it. (This happens for events that
    // CSharpDecompiler.DoDecompile already recognized as automatic: the backing field is
    // hidden from the normal member list, but re-emitted via the work list when referenced.)
    const auto* symbol = dynamic_cast<const IEvent*>(get_symbol(&event_declaration));
    if (context_->settings.automatic_events && symbol != nullptr) {
        FieldDeclaration* field_decl = nullptr;
        if (AstNode* parent = event_declaration.parent()) {
            for (AstNode* child : parent->children().to_vector()) {
                auto* fd = as<FieldDeclaration>(child);
                if (fd != nullptr && is_event_backing_field_declaration(fd, symbol)) {
                    field_decl = fd;
                    break;
                }
            }
        }
        if (field_decl != nullptr) field_decl->remove();
    }
    return Base::visit_event_declaration(event_declaration);
}

AstNode* PatternStatementTransform::visit_method_declaration(MethodDeclaration& method_declaration) {
    AstNode* result = transform_destructor(&method_declaration);
    return result != nullptr ? result : Base::visit_method_declaration(method_declaration);
}

AstNode* PatternStatementTransform::visit_destructor_declaration(DestructorDeclaration& destructor_declaration) {
    AstNode* result = transform_destructor_body(&destructor_declaration);
    return result != nullptr ? result : Base::visit_destructor_declaration(destructor_declaration);
}

AstNode* PatternStatementTransform::visit_try_catch_statement(TryCatchStatement& try_catch_statement) {
    AstNode* result = transform_try_catch_finally(&try_catch_statement);
    return result != nullptr ? result : Base::visit_try_catch_statement(try_catch_statement);
}

// ---- for --------------------------------------------------------------------------------------------------------------------------

ForStatement* PatternStatementTransform::transform_for(ExpressionStatement* node) {
    if (!context_->settings.for_statement) return nullptr;
    Match m1 = pattern_matching::match(variable_assign_pattern(), node);
    if (!m1.success()) return nullptr;
    ILVariable* variable = get_il_variable(single<IdentifierExpression>(m1, "variable"));
    AstNode* next = node->next_sibling();
    if (next == nullptr) return nullptr;
    auto* next_for = as<ForStatement>(next);
    if (next_for != nullptr && for_statement_uses_variable(next_for, variable)) {
        context_->step("Move declaration into for initializer", node);
        node->remove();
        next->insert_child_after(nullptr, node, Slots::for_initializer);
        return next_for;
    }
    Match m3 = pattern_matching::match(for_pattern(), next);
    if (!m3.success()) return nullptr;
    // ensure the variable in the for pattern is the same as in the declaration
    if (variable != get_il_variable(single<IdentifierExpression>(m3, "ident"))) return nullptr;
    auto* loop = cast<WhileStatement>(next);
    // Cannot convert to for loop, if the iteration variable is a ref local used after the loop: its
    // declaration is hoisted in front, leaving a headless `for (; cond; v = ref ...)` whose only
    // initialization is the for-initializer ref-assignment -- which can't be split from a ref local
    // (CS8174). Keeping it a while-loop matches the source and keeps the initializer on the decl.
    if (variable != nullptr && variable_of(context_->l2, *variable).type->is_by_ref_like() && is_variable_used_after(loop, variable)) return nullptr;
    // Cannot convert to for loop, if any variable that is used in the "iterator" part of the pattern,
    // will be declared in the body of the while-loop.
    Statement* iterator_statement = single<Statement>(m3, "iterator");
    if (iterator_variables_declared_inside_loop_body(iterator_statement)) return nullptr;
    // Cannot convert to for loop, because that would change the semantics of the program.
    // continue in while jumps to the condition block.
    // Whereas continue in for jumps to the increment block.
    for (AstNode* d : loop->descendant_nodes(&PatternStatementTransform::descend_into_statement)) {
        if (is<ContinueStatement>(d)) return nullptr;
    }
    context_->step("Transform while loop to for", loop);
    node->remove();
    auto* new_body = make<BlockStatement>();
    for (Statement* stmt : m3.get<Statement>("statement")) new_body->add(detach(stmt));
    auto* for_statement = make<ForStatement>();
    copy_annotations_from(for_statement, loop);
    for_statement->initializers().add(node);
    for_statement->set_condition(detach(loop->condition()));
    for_statement->iterators().add(detach(iterator_statement));
    for_statement->set_embedded_statement(new_body);
    loop->replace_with(for_statement);
    context_->end_step(for_statement);
    return for_statement;
}

bool PatternStatementTransform::descend_into_statement(AstNode* node) {
    if (is<Expression>(node) || is<ExpressionStatement>(node)) return false;
    if (is<WhileStatement>(node) || is<ForeachStatement>(node) || is<DoWhileStatement>(node) || is<ForStatement>(node)) return false;
    return true;
}

bool PatternStatementTransform::for_statement_uses_variable(ForStatement* statement, ILVariable* variable) {
    auto uses = [&](AstNode* root) {
        for (AstNode* node : root->descendants_and_self()) {
            auto* ie = as<IdentifierExpression>(node);
            if (ie != nullptr && get_il_variable(ie) == variable) return true;
        }
        return false;
    };
    if (statement->condition() != nullptr && uses(statement->condition())) return true;
    for (Statement* i : statement->iterators()) {
        if (uses(i)) return true;
    }
    return false;
}

bool PatternStatementTransform::is_variable_used_after(Statement* loop, ILVariable* variable) {
    for (AstNode* sibling = loop->next_sibling(); sibling != nullptr; sibling = sibling->next_sibling()) {
        for (AstNode* node : sibling->descendants_and_self()) {
            auto* ie = as<IdentifierExpression>(node);
            if (ie != nullptr && get_il_variable(ie) == variable) return true;
        }
    }
    return false;
}

bool PatternStatementTransform::iterator_variables_declared_inside_loop_body(Statement* iterator_statement) {
    for (AstNode* node : iterator_statement->descendants_and_self().to_vector()) {
        auto* id = as<IdentifierExpression>(node);
        if (id == nullptr) continue;
        ILVariable* v = get_il_variable(id);
        if (v == nullptr || !DeclareVariables::variable_needs_declaration(variable_of(context_->l2, *v).kind)) continue;
        if (declare_variables_.get_declaration_point(v)->parent() == iterator_statement->parent()) return true;
    }
    return false;
}

// ---- foreach ----------------------------------------------------------------------------------------------------------------------

AstType* PatternStatementTransform::convert_variable_type(ILVariable* variable) {
    const typesystem::IType* type = variable_of(context_->l2, *variable).type;
    if (context_->settings.anonymous_types && contains_anonymous_type(type)) return make<SimpleType>("var");
    return context_->type_system_ast_builder.convert_type(type);
}

bool PatternStatementTransform::variable_can_be_used_as_foreach_local(ILVariable* item_var, Statement* loop) {
    if (item_var == nullptr) return false;
    const VariableFacts item = variable_of(context_->l2, *item_var);
    if (!(item.kind == VariableKind::Local || item.kind == VariableKind::StackSlot)) {
        // only locals/temporaries can be converted into foreach loop variable
        return false;
    }

    il::BlockContainer* block_container = loop->annotation<il::BlockContainer>();

    if (!item.is_single_definition()) {
        // foreach variable cannot be assigned to.
        // As a special case, we accept taking the address for a method call,
        // but only if the call is the only use, so that any mutation by the call
        // cannot be observed.
        if (!address_used_for_single_call(item_var, block_container)) {
            return false;
        }
    }

    if (item.capture_scope != nullptr && item.capture_scope != block_container) {
        // captured variables cannot be declared in the loop unless the loop is their capture scope
        return false;
    }

    AstNode* decl_point = declare_variables_.get_declaration_point(item_var);
    bool in_loop = false;
    for (AstNode* ancestor : decl_point->ancestors()) {
        if (ancestor == loop) {
            in_loop = true;
            break;
        }
    }
    return in_loop && !declare_variables_.was_merged(item_var);
}

bool PatternStatementTransform::address_used_for_single_call(ILVariable* v, il::BlockContainer* loop) const {
    const VariableFacts facts = variable_of(context_->l2, *v);
    if (facts.store_count == 1 && facts.address_count == 1 && facts.load_count == 0 && facts.type->is_reference_type() == std::optional<bool>(false)) {
        // used as this pointer for a method call
        // this is OK iff the call is not within a nested loop (the first container above the call is the loop)
        std::optional<std::vector<il::BlockContainer*>> containers;
        if (context_->l2 != nullptr) {
            containers = context_->l2->single_this_call_containers(*v);
        } else {
            il::LdLoca* address = v->address_instructions()[0];
            auto* call = il::as<il::Call>(address->parent());
            if (call != nullptr && address->child_index() == 0 && !call->method->is_static()) {
                containers.emplace();
                for (il::ILInstruction* node = call->parent(); node != nullptr; node = node->parent())
                    if (auto* c = il::as<il::BlockContainer>(node)) {
                        containers->push_back(c);
                        break;
                    }
            }
        }
        return containers && !containers->empty() && containers->front() == loop;
    }
    return false;
}

Statement* PatternStatementTransform::transform_foreach_on_array(ForStatement* for_statement) {
    if (!context_->settings.for_each_statement) return nullptr;
    Match m = pattern_matching::match(for_on_array_pattern(), for_statement);
    if (!m.success()) return nullptr;
    ILVariable* item_variable = get_il_variable(single<IdentifierExpression>(m, "itemVariable"));
    ILVariable* index_variable = get_il_variable(single<IdentifierExpression>(m, "indexVariable"));
    ILVariable* array_variable = get_il_variable(single<IdentifierExpression>(m, "arrayVariable"));
    if (item_variable == nullptr || index_variable == nullptr || array_variable == nullptr) return nullptr;
    const typesystem::IType* array_type = variable_of(context_->l2, *array_variable).type;
    if (array_type->kind() != TypeKind::Array && !is_known(array_type, KnownTypeCode::String)) return nullptr;
    if (!variable_can_be_used_as_foreach_local(item_variable, for_statement)) return nullptr;
    if (const VariableFacts index = variable_of(context_->l2, *index_variable); index.store_count != 2 || index.load_count != 3 || index.address_count != 0)
        return nullptr;
    context_->step("Introduce foreach over array", for_statement);
    auto* body = make<BlockStatement>();
    for (Statement* statement : m.get<Statement>("statements")) body->statements().add(detach(statement));
    AstType* variable_type = convert_variable_type(item_variable);
    auto* designation = make<SingleVariableDesignation>(*variable_of(context_->l2, *item_variable).name);
    Expression* in_expression = detach(single<IdentifierExpression>(m, "arrayVariable"));
    auto* foreach_stmt = make<ForeachStatement>(variable_type, designation, in_expression, body);
    copy_annotations_from(foreach_stmt, for_statement);
    item_variable->set_kind(VariableKind::ForeachLocal);
    if (context_->l2 != nullptr) context_->l2->variable_changed(item_variable);
    // Add the variable annotation for highlighting (TokenTextWriter expects it directly on the ForeachStatement).
    foreach_stmt->variable_designation()->add_annotation(make<ILVariableResolveResult>(item_variable, variable_of(context_->l2, *item_variable).type));
    // TODO : add ForeachAnnotation
    for_statement->replace_with(foreach_stmt);
    context_->end_step(foreach_stmt);
    return foreach_stmt;
}

// Reconstructs a `foreach` over an inline array from the `for` loop the compiler lowers it to:
// `for (i = 0; i < N; i++) { item = <PrivateImplementationDetails>.InlineArrayElementRef(ref buffer, i); ... }`.
// The rewrite is only sound because the loop bound `N` equals the inline array length, which proves the index is always in range:
// `InlineArrayElementRef` is the compiler's unchecked element accessor, whereas the C# inline-array indexer `buffer[i]` is bounds-checked,
// so the two only agree when the index is provably in-bounds. A loop that does not match this exact shape keeps the (unnameable but
// faithful) helper call.
Statement* PatternStatementTransform::transform_foreach_on_inline_array(ForStatement* for_statement) {
    if (!context_->settings.for_each_statement || !context_->settings.inline_arrays) return nullptr;
    Match m = pattern_matching::match(for_on_inline_array_pattern(), for_statement);
    if (!m.success()) return nullptr;
    ILVariable* item_variable = get_il_variable(single<IdentifierExpression>(m, "itemVariable"));
    ILVariable* index_variable = get_il_variable(single<IdentifierExpression>(m, "indexVariable"));
    if (item_variable == nullptr || index_variable == nullptr) return nullptr;

    // The loop body must start with `item = InlineArrayElementRef(ref buffer, index)`.
    auto* element_access = as<InvocationExpression>(single<Expression>(m, "elementAccess"));
    if (element_access == nullptr) return nullptr;
    const auto* helper = dynamic_cast<const IMethod*>(get_symbol(element_access));
    if (helper == nullptr || helper->declaring_type() == nullptr || helper->declaring_type()->full_name() != "<PrivateImplementationDetails>")
        return nullptr;
    if (!(helper->name() == "InlineArrayElementRef" || helper->name() == "InlineArrayElementRefReadOnly")) return nullptr;
    if (element_access->arguments().count() != 2) return nullptr;
    // arg0: `ref buffer`, arg1: the loop index.
    auto* direction = as<DirectionExpression>(element_access->arguments().first_or_null());
    auto* buffer_identifier = direction != nullptr ? as<IdentifierExpression>(direction->expression()) : nullptr;
    if (buffer_identifier == nullptr) return nullptr;
    ILVariable* buffer_variable = get_il_variable(buffer_identifier);
    if (buffer_variable == nullptr) return nullptr;
    auto* index_identifier = as<IdentifierExpression>(element_access->arguments().last_or_null());
    if (index_identifier == nullptr || get_il_variable(index_identifier) != index_variable) return nullptr;

    // Soundness: the loop counts 0..length-1 over exactly the inline array's length, so the
    // index is provably in range. Any other bound (or a non-inline-array buffer) is rejected.
    std::optional<int> array_length = get_inline_array_length(variable_of(context_->l2, *buffer_variable).type);
    if (!array_length) return nullptr;
    const auto* loop_bound = std::get_if<std::int32_t>(&single<PrimitiveExpression>(m, "length")->value());
    if (loop_bound == nullptr || *loop_bound != *array_length) return nullptr;

    if (!variable_can_be_used_as_foreach_local(item_variable, for_statement)) return nullptr;
    // The index is a pure counter: stored at init + increment, loaded at the condition,
    // the increment, and the element access; never captured by address.
    if (const VariableFacts index = variable_of(context_->l2, *index_variable); index.store_count != 2 || index.load_count != 3 || index.address_count != 0)
        return nullptr;

    context_->step("Introduce foreach over inline array", for_statement);
    // Take the buffer reference for the `in` expression before dropping the element access.
    Expression* in_expression = detach(buffer_identifier);
    // Reuse the loop body (preserving its annotations) after removing its leading
    // `item = <PrivateImplementationDetails>.InlineArrayElementRef(ref buffer, i)` statement.
    auto* body = cast<BlockStatement>(for_statement->embedded_statement());
    body->statements().first_or_null()->remove();
    AstType* variable_type = convert_variable_type(item_variable);
    auto* designation = make<SingleVariableDesignation>(*variable_of(context_->l2, *item_variable).name);
    auto* foreach_stmt = make<ForeachStatement>(variable_type, designation, in_expression, detach(body));
    copy_annotations_from(foreach_stmt, for_statement);
    item_variable->set_kind(VariableKind::ForeachLocal);
    if (context_->l2 != nullptr) context_->l2->variable_changed(item_variable);
    foreach_stmt->variable_designation()->add_annotation(make<ILVariableResolveResult>(item_variable, variable_of(context_->l2, *item_variable).type));
    for_statement->replace_with(foreach_stmt);
    context_->end_step(foreach_stmt);
    return foreach_stmt;
}

bool PatternStatementTransform::match_lower_bound(int index_num, ILVariable** index, ILVariable* collection, Statement* statement) {
    *index = nullptr;
    Match m = pattern_matching::match(variable_assign_lower_bound_pattern(), statement);
    if (!m.success()) return false;
    int i;
    if (!try_parse_int_value(single<PrimitiveExpression>(m, "index")->value(), &i) || index_num != i) return false;
    *index = get_il_variable(single<IdentifierExpression>(m, "variable"));
    return get_il_variable(single<IdentifierExpression>(m, "collection")) == collection;
}

bool PatternStatementTransform::match_foreach_on_multi_dim_array(const std::vector<ILVariable*>& upper_bounds, ILVariable* collection,
                                                                 Statement* first_initializer_statement, IdentifierExpression** foreach_variable,
                                                                 std::vector<Statement*>* statements, std::vector<ILVariable*>* lower_bounds) {
    std::size_t i = 0;
    *foreach_variable = nullptr;
    statements->clear();
    lower_bounds->assign(upper_bounds.size(), nullptr);
    Statement* stmt = first_initializer_statement;
    Match m;
    ILVariable* index_variable = nullptr;
    while (i < upper_bounds.size() && match_lower_bound(static_cast<int>(i), &index_variable, collection, stmt)) {
        m = pattern_matching::match(for_on_array_multi_dim_pattern(), get_next_statement(stmt));
        if (!m.success()) return false;
        ILVariable* upper_bound = get_il_variable(single<IdentifierExpression>(m, "upperBoundVariable"));
        if (upper_bounds[i] != upper_bound) return false;
        stmt = single<Statement>(m, "lowerBoundAssign");
        (*lower_bounds)[i] = index_variable;
        i++;
    }
    if (collection->type()->kind() != TypeKind::Array) return false;
    Match m2 = pattern_matching::match(foreach_variable_on_mult_array_assign_pattern(), stmt);
    if (!m2.success()) return false;
    ILVariable* collection2 = get_il_variable(single<IdentifierExpression>(m2, "collection"));
    if (collection2 != collection) return false;
    *foreach_variable = single<IdentifierExpression>(m2, "variable");
    // (upstream reads the statements from `m`, which is empty when no lower bound matched: the loop is not a foreach then)
    if (!m.success()) throw std::logic_error("Sequence contains no elements");
    *statements = m.get<Statement>("statements");
    return true;
}

Statement* PatternStatementTransform::transform_foreach_on_multi_dim_array(ExpressionStatement* expression_statement) {
    if (!context_->settings.for_each_statement) return nullptr;
    Match m;
    Statement* stmt = expression_statement;
    ILVariable* collection = nullptr;
    std::vector<ILVariable*> upper_bounds;
    bool have_upper_bounds = false;
    std::vector<Statement*> statements_to_delete;
    int i = 0;
    // first we look for all the upper bound initializations
    do {
        m = pattern_matching::match(variable_assign_upper_bound_pattern(), stmt);
        if (!m.success()) break;
        if (!have_upper_bounds) {
            collection = get_il_variable(single<IdentifierExpression>(m, "collection"));
            const auto* array_type = collection != nullptr ? dynamic_cast<const typesystem::ArrayType*>(collection->type()) : nullptr;
            if (array_type == nullptr) break;
            upper_bounds.assign(static_cast<std::size_t>(array_type->dimensions()), nullptr);
            have_upper_bounds = true;
        } else {
            statements_to_delete.push_back(stmt);
        }
        ILVariable* next_collection = get_il_variable(single<IdentifierExpression>(m, "collection"));
        if (next_collection != collection) break;
        int index;
        if (!try_parse_int_value(single<PrimitiveExpression>(m, "index")->value(), &index) || index != i) break;
        upper_bounds[static_cast<std::size_t>(i)] = get_il_variable(single<IdentifierExpression>(m, "variable"));
        stmt = get_next_statement(stmt);
        i++;
    } while (stmt != nullptr && have_upper_bounds && i < static_cast<int>(upper_bounds.size()));

    if (!have_upper_bounds || upper_bounds.empty() || upper_bounds.back() == nullptr || collection == nullptr || stmt == nullptr) return nullptr;
    IdentifierExpression* foreach_variable;
    std::vector<Statement*> statements;
    std::vector<ILVariable*> lower_bounds;
    if (!match_foreach_on_multi_dim_array(upper_bounds, collection, stmt, &foreach_variable, &statements, &lower_bounds)) return nullptr;
    statements_to_delete.push_back(stmt);
    // The matched multi-dimensional foreach pattern guarantees a statement after stmt.
    statements_to_delete.push_back(get_next_statement(stmt));
    ILVariable* item_variable = get_il_variable(foreach_variable);
    if (item_variable == nullptr || !variable_of(context_->l2, *item_variable).is_single_definition() ||
        (variable_of(context_->l2, *item_variable).kind != VariableKind::Local && variable_of(context_->l2, *item_variable).kind != VariableKind::StackSlot))
        return nullptr;
    for (ILVariable* ub : upper_bounds) {
        if (!(ub->is_single_definition() && ub->load_count() == 1)) return nullptr;
    }
    for (ILVariable* lb : lower_bounds) {
        if (lb == nullptr || !(lb->store_count() == 2 && lb->load_count() == 3 && lb->address_count() == 0)) return nullptr;
    }
    context_->step("Introduce foreach over multidimensional array", expression_statement);
    auto* body = make<BlockStatement>();
    for (Statement* statement : statements) body->statements().add(detach(statement));
    AstType* variable_type = convert_variable_type(item_variable);
    auto* designation = make<SingleVariableDesignation>(*variable_of(context_->l2, *item_variable).name);
    Expression* in_expression = detach(single<IdentifierExpression>(m, "collection"));
    auto* foreach_stmt = make<ForeachStatement>(variable_type, designation, in_expression, body);
    for (Statement* statement : statements_to_delete) detach(statement);
    // foreach_stmt->copy_annotations_from(for_statement);
    item_variable->set_kind(VariableKind::ForeachLocal);
    if (context_->l2 != nullptr) context_->l2->variable_changed(item_variable);
    // Add the variable annotation for highlighting (TokenTextWriter expects it directly on the ForeachStatement).
    foreach_stmt->variable_designation()->add_annotation(make<ILVariableResolveResult>(item_variable, variable_of(context_->l2, *item_variable).type));
    // TODO : add ForeachAnnotation
    expression_statement->replace_with(foreach_stmt);
    context_->end_step(foreach_stmt);
    return foreach_stmt;
}

// ---- automatic properties ---------------------------------------------------------------------------------------------------------

bool PatternStatementTransform::can_transform_to_automatic_property(const IProperty* property, bool accessors_must_be_compiler_generated) {
    if (!property->can_get()) return false;
    if (accessors_must_be_compiler_generated && !is_compiler_generated(property->getter())) return false;
    if (const IMethod* setter = property->setter()) {
        if (accessors_must_be_compiler_generated && !is_compiler_generated(setter)) return false;
        if (typesystem::has_readonly_modifier(setter)) return false;
    }
    return true;
}

PropertyDeclaration* PatternStatementTransform::transform_automatic_property(PropertyDeclaration* property_declaration) {
    const auto* property = dynamic_cast<const IProperty*>(get_symbol(property_declaration));
    if (property == nullptr) return nullptr;
    bool has_generated_field = false;
    if (const auto* declaring = property->declaring_type_definition()) {
        for (const IField* f : declaring->fields()) {
            if (f->name() == "_" + property->name() && is_compiler_generated(f)) {
                has_generated_field = true;
                break;
            }
        }
    }
    if (!can_transform_to_automatic_property(property, !has_generated_field)) return nullptr;
    const IField* field = nullptr;
    Match m = pattern_matching::match(automatic_property_pattern(), property_declaration);
    if (m.success()) {
        field = dynamic_cast<const IField*>(get_symbol(single<AstNode>(m, "fieldReference")));
    } else {
        Match m2 = pattern_matching::match(automatic_readonly_property_pattern(), property_declaration);
        if (m2.success()) {
            field = dynamic_cast<const IField*>(get_symbol(single<AstNode>(m2, "fieldReference")));
        }
    }
    std::string ignored_name;
    if (field == nullptr || !name_could_be_backing_field_of_automatic_property(field->name(), &ignored_name)) return nullptr;
    if ((property_declaration->setter() != nullptr && property_declaration->setter()->has_modifier(Modifiers::Readonly)) ||
        (property_declaration->has_modifier(Modifiers::Readonly) && property_declaration->setter() != nullptr))
        return nullptr;
    if (is_compiler_generated(field) && field->declaring_type_definition() == property->declaring_type_definition()) {
        context_->step("Convert property to auto-property", property_declaration);
        // Clearing the accessor body turns it into an auto-property accessor.
        Accessor* getter = property_declaration->getter();
        Accessor* setter = property_declaration->setter();
        if (getter != nullptr) {
            remove_compiler_generated_attribute(getter->attributes(), {"System.Runtime.CompilerServices.CompilerGeneratedAttribute"});
            getter->set_body(nullptr);
        }
        if (setter != nullptr) {
            remove_compiler_generated_attribute(setter->attributes(), {"System.Runtime.CompilerServices.CompilerGeneratedAttribute"});
            setter->set_body(nullptr);
        }
        property_declaration->set_modifiers(property_declaration->modifiers() & ~Modifiers::Readonly);
        if (getter != nullptr) getter->set_modifiers(getter->modifiers() & ~Modifiers::Readonly);

        FieldDeclaration* field_decl = nullptr;
        if (AstNode* parent = property_declaration->parent()) {
            for (AstNode* child : parent->children().to_vector()) {
                auto* fd = as<FieldDeclaration>(child);
                if (fd == nullptr) continue;
                const auto* member = dynamic_cast<const IMember*>(get_symbol(fd));
                if (member != nullptr && field->equals(member)) {
                    field_decl = fd;
                    break;
                }
            }
        }
        if (field_decl != nullptr) {
            field_decl->remove();
            // Add C# 7.3 attributes on backing field:
            CSharpDecompiler::remove_attribute(field_decl, KnownAttribute::CompilerGenerated);
            CSharpDecompiler::remove_attribute(field_decl, KnownAttribute::DebuggerBrowsable);
            for (AttributeSection* section : field_decl->attributes().to_vector()) {
                section->set_attribute_target("field");
                property_declaration->attributes().add(detach(section));
            }
        }
    }
    // Since the property instance is not changed, we can continue in the visitor as usual, so return null
    return nullptr;
}

AstNode* PatternStatementTransform::visit_identifier(Identifier& identifier) {
    if (context_->settings.automatic_properties) {
        Identifier* new_identifier = replace_backing_field_usage(&identifier);
        if (new_identifier != nullptr) {
            identifier.replace_with(new_identifier);
            context_->end_step(new_identifier);
            return new_identifier;
        }
    }
    return Base::visit_identifier(identifier);
}

bool PatternStatementTransform::is_backing_field_of_automatic_property(const IField* field, const IProperty** property) {
    *property = nullptr;
    std::string property_name;
    if (!name_could_be_backing_field_of_automatic_property(field->name(), &property_name)) return false;
    if (!is_compiler_generated(field)) return false;
    if (const auto* declaring = field->declaring_type_definition()) {
        auto properties = declaring->get_properties([&](const IProperty* p) { return p->name() == property_name; },
                                                    typesystem::GetMemberOptions::IgnoreInheritedMembers);
        if (!properties.empty()) *property = properties[0];
    }
    return *property != nullptr;
}

Identifier* PatternStatementTransform::replace_backing_field_usage(Identifier* identifier) {
    std::string ignored_name;
    if (name_could_be_backing_field_of_automatic_property(identifier->name(), &ignored_name)) {
        AstNode* parent = identifier->parent();
        if (parent == nullptr) return nullptr;
        auto* mrr = parent->annotation<semantics::MemberResolveResult>();
        const auto* field = mrr != nullptr ? dynamic_cast<const IField*>(mrr->member()) : nullptr;
        const IProperty* property = nullptr;
        if (field != nullptr && is_backing_field_of_automatic_property(field, &property) &&
            can_transform_to_automatic_property(property, !(is_compiler_generated(field) && field->name() == "_" + property->name())) &&
            (current_method_ == nullptr || current_method_->accessor_owner() != static_cast<const IMember*>(property))) {
            if (!property->can_set() && !context_->settings.getter_only_automatic_properties) return nullptr;
            context_->step("Replace backing field use with property", identifier);
            parent->remove_annotations<semantics::MemberResolveResult>();
            parent->add_annotation(make<semantics::MemberResolveResult>(mrr->target_result(), property));
            return Identifier::create(property->name());
        }
    }
    return nullptr;
}


// ---- destructors ------------------------------------------------------------------------------------------------------------------

DestructorDeclaration* PatternStatementTransform::transform_destructor(MethodDeclaration* method_def) {
    Match m = pattern_matching::match(destructor_pattern(), method_def);
    if (m.success()) {
        context_->step("Convert Finalize method to destructor", method_def);
        auto* dd = make<DestructorDeclaration>();
        method_def->attributes().move_to(dd->attributes());
        copy_annotations_from(dd, method_def);
        dd->set_modifiers(method_def->modifiers() & ~(Modifiers::Protected | Modifiers::Override));
        dd->set_body(detach(single<BlockStatement>(m, "body")));
        // A destructor only appears inside a type declaration, so the context tracker
        // has an enclosing type at this point.
        dd->set_name(current_type_definition_->name());
        method_def->replace_with(dd);
        context_->end_step(dd);
        return dd;
    }
    return nullptr;
}

DestructorDeclaration* PatternStatementTransform::transform_destructor_body(DestructorDeclaration* dtor_def) {
    Match m = pattern_matching::match(destructor_body_pattern(), dtor_def->body());
    if (m.success()) {
        context_->step("Simplify destructor body", dtor_def);
        dtor_def->set_body(detach(single<BlockStatement>(m, "body")));
        return dtor_def;
    }
    return nullptr;
}

// ---- try-catch-finally ------------------------------------------------------------------------------------------------------------

// Simplify nested 'try { try {} catch {} } finally {}'. This transformation must run after the using/lock tranformations.
TryCatchStatement* PatternStatementTransform::transform_try_catch_finally(TryCatchStatement* try_finally) {
    if (pattern_matching::is_match(try_catch_finally_pattern(), try_finally)) {
        context_->step("Merge nested try-catch-finally", try_finally);
        auto* try_catch = cast<TryCatchStatement>(try_finally->try_block()->statements().first_or_null());
        try_finally->set_try_block(detach(try_catch->try_block()));
        try_catch->catch_clauses().move_to(try_finally->catch_clauses());
    }
    // Since the try_finally instance is not changed, we can continue in the visitor as usual, so return null
    return nullptr;
}

// ---- simplify cascading if-else-if statements -------------------------------------------------------------------------------------

AstNode* PatternStatementTransform::simplify_cascading_if_else_statements(IfElseStatement* node) {
    Match m = pattern_matching::match(cascading_if_else_pattern(), node);
    if (m.success()) {
        context_->step("Simplify cascading if-else", node);
        IfElseStatement* else_if = single<IfElseStatement>(m, "nestedIfStatement");
        node->set_false_statement(detach(else_if));
    }
    return nullptr;
}

// Use associativity of logic operators to avoid parentheses.
AstNode* PatternStatementTransform::visit_binary_operator_expression(BinaryOperatorExpression& expr) {
    switch (expr.operator_()) {
        case BinaryOperatorType::ConditionalAnd:
        case BinaryOperatorType::ConditionalOr: {
            // a && (b && c) ==> (a && b) && c
            auto* b_and_c = as<BinaryOperatorExpression>(expr.right());
            if (b_and_c != nullptr && b_and_c->operator_() == expr.operator_()) {
                context_->step("Reassociate conditional logic", &expr);
                // make b_and_c the parent and expr the child.
                // A conditional-and/or operator always has both operands present.
                Expression* b = detach(b_and_c->left());
                Expression* c = detach(b_and_c->right());
                expr.replace_with(detach(b_and_c));
                b_and_c->set_left(&expr);
                b_and_c->set_right(c);
                expr.set_right(b);
                context_->end_step(b_and_c);
                return Base::visit_binary_operator_expression(*b_and_c);
            }
            break;
        }
        default:
            break;
    }
    return Base::visit_binary_operator_expression(expr);
}

AstNode* PatternStatementTransform::visit_unary_operator_expression(UnaryOperatorExpression& expr) {
    auto* binary = as<BinaryOperatorExpression>(expr.expression());
    if (expr.operator_() == UnaryOperatorType::Not && binary != nullptr && binary->operator_() == BinaryOperatorType::Equality) {
        context_->step("Replace negated equality with inequality", &expr);
        binary->set_operator_(BinaryOperatorType::InEquality);
        expr.replace_with(detach(binary));
        context_->end_step(binary);
        return visit_binary_operator_expression(*binary);
    }
    return Base::visit_unary_operator_expression(expr);
}

// ---- C# 7.3 pattern based fixed (for value types) ---------------------------------------------------------------------------------

AstNode* PatternStatementTransform::visit_fixed_statement(FixedStatement& fixed_statement) {
    if (context_->settings.pattern_based_fixed_statement) {
        for (VariableInitializer* v : fixed_statement.variables().to_vector()) {
            Match m = pattern_matching::match(address_of_pinnable_reference(), v->initializer());
            if (m.success()) {
                Expression* target = single<Expression>(m, "target");
                if (get_resolve_result(target)->type()->is_reference_type() == std::optional<bool>(false)) {
                    context_->step("Use pattern-based fixed statement", &fixed_statement);
                    v->set_initializer(detach(target));
                }
            }
        }
    }
    return Base::visit_fixed_statement(fixed_statement);
}

// ---- C# 8.0 using variables -------------------------------------------------------------------------------------------------------

AstNode* PatternStatementTransform::visit_using_statement(UsingStatement& using_statement_in) {
    auto* using_statement = static_cast<UsingStatement*>(Base::visit_using_statement(using_statement_in));
    if (!context_->settings.use_enhanced_using) return using_statement;

    if (get_next_statement(using_statement) != nullptr || !is<BlockStatement>(using_statement->parent())) return using_statement;

    if (!is<VariableDeclarationStatement>(using_statement->resource_acquisition())) return using_statement;

    context_->step("Use enhanced using statement", using_statement);
    using_statement->set_is_enhanced(true);
    return using_statement;
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
