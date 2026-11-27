// dotnet/decompiler/csharp/transforms/normalize_block_statements.cpp -- see normalize_block_statements.hpp.
#include "dotnet/decompiler/csharp/transforms/normalize_block_statements.hpp"

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/any_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/any_node_or_null.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/repeat.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_arena.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using syntax::pattern_matching::AnyNode;
using syntax::pattern_matching::AnyNodeOrNull;
using syntax::pattern_matching::Pattern;
using syntax::pattern_matching::Repeat;

namespace {

// Modifiers that are emitted on accessors, but can be moved to the property declaration.
constexpr Modifiers movable_modifiers = Modifiers::Readonly;

Accessor* make_calculated_getter() {
    auto* getter = make<Accessor>();
    getter->set_modifiers(Modifiers::Any);
    auto* body = make<BlockStatement>();
    body->add(make<ReturnStatement>(Expression::from_pattern(make<AnyNode>("expression"))));
    getter->set_body(body);
    return getter;
}

PropertyDeclaration* calculated_getter_only_property_pattern() {
    static PropertyDeclaration* const pattern = build_pattern([] {
        auto* p = make<PropertyDeclaration>();
        p->attributes().add(AttributeSection::from_pattern(make<Repeat>(make<AnyNode>())));
        p->set_modifiers(Modifiers::Any);
        p->set_name(std::string(Pattern::any_string));
        p->set_private_implementation_type(AstType::from_pattern(make<AnyNodeOrNull>()));
        p->set_return_type(AstType::from_pattern(make<AnyNode>()));
        p->set_getter(make_calculated_getter());
        return p;
    });
    return pattern;
}

IndexerDeclaration* calculated_getter_only_indexer_pattern() {
    static IndexerDeclaration* const pattern = build_pattern([] {
        auto* p = make<IndexerDeclaration>();
        p->attributes().add(AttributeSection::from_pattern(make<Repeat>(make<AnyNode>())));
        p->set_modifiers(Modifiers::Any);
        p->set_private_implementation_type(AstType::from_pattern(make<AnyNodeOrNull>()));
        p->parameters().add(ParameterDeclaration::from_pattern(make<Repeat>(make<AnyNode>())));
        p->set_return_type(AstType::from_pattern(make<AnyNode>()));
        p->set_getter(make_calculated_getter());
        return p;
    });
    return pattern;
}

}  // namespace

void NormalizeBlockStatements::run(AstNode* root_node, TransformContext& context) {
    context_ = &context;
    root_node->accept_visitor(static_cast<IAstVisitor&>(*this));
}

void NormalizeBlockStatements::visit_syntax_tree(SyntaxTree& syntax_tree) {
    single_namespace_declaration_ = nullptr;
    has_namespace_ = false;
    DepthFirstAstVisitor::visit_syntax_tree(syntax_tree);
    if (context_->settings.file_scoped_namespaces && single_namespace_declaration_ != nullptr) {
        context_->step("Use file-scoped namespace", single_namespace_declaration_);
        single_namespace_declaration_->set_is_file_scoped(true);
    }
}

void NormalizeBlockStatements::visit_namespace_declaration(NamespaceDeclaration& namespace_declaration) {
    single_namespace_declaration_ = nullptr;
    if (!has_namespace_) {
        has_namespace_ = true;
        single_namespace_declaration_ = &namespace_declaration;
    }
    DepthFirstAstVisitor::visit_namespace_declaration(namespace_declaration);
}

void NormalizeBlockStatements::visit_if_else_statement(IfElseStatement& if_else_statement) {
    DepthFirstAstVisitor::visit_if_else_statement(if_else_statement);
    do_transform(if_else_statement.true_statement(), &if_else_statement);
    do_transform(if_else_statement.false_statement(), &if_else_statement);
}

void NormalizeBlockStatements::visit_while_statement(WhileStatement& while_statement) {
    DepthFirstAstVisitor::visit_while_statement(while_statement);
    insert_block(while_statement.embedded_statement());
}

void NormalizeBlockStatements::visit_do_while_statement(DoWhileStatement& do_while_statement) {
    DepthFirstAstVisitor::visit_do_while_statement(do_while_statement);
    insert_block(do_while_statement.embedded_statement());
}

void NormalizeBlockStatements::visit_foreach_statement(ForeachStatement& foreach_statement) {
    DepthFirstAstVisitor::visit_foreach_statement(foreach_statement);
    insert_block(foreach_statement.embedded_statement());
}

void NormalizeBlockStatements::visit_for_statement(ForStatement& for_statement) {
    DepthFirstAstVisitor::visit_for_statement(for_statement);
    insert_block(for_statement.embedded_statement());
}

void NormalizeBlockStatements::visit_fixed_statement(FixedStatement& fixed_statement) {
    DepthFirstAstVisitor::visit_fixed_statement(fixed_statement);
    insert_block(fixed_statement.embedded_statement());
}

void NormalizeBlockStatements::visit_lock_statement(LockStatement& lock_statement) {
    DepthFirstAstVisitor::visit_lock_statement(lock_statement);
    insert_block(lock_statement.embedded_statement());
}

void NormalizeBlockStatements::visit_using_statement(UsingStatement& using_statement) {
    DepthFirstAstVisitor::visit_using_statement(using_statement);
    do_transform(using_statement.embedded_statement(), &using_statement);
}

void NormalizeBlockStatements::do_transform(Statement* statement, Statement* parent) {
    if (statement == nullptr) return;
    if (context_->settings.always_use_braces) {
        if (!is_else_if(statement, parent)) {
            insert_block(statement);
        }
    } else {
        auto* b = as<BlockStatement>(statement);
        if (b != nullptr && b->statements().count() == 1 && is_allowed_as_embedded_statement(b->statements().first_or_null(), parent)) {
            context_->step("Remove redundant block statement", statement);
            Statement* inner_statement = detach(b->statements().first_or_null());
            statement->replace_with(inner_statement);
            context_->end_step(inner_statement);
        } else if (!is_allowed_as_embedded_statement(statement, parent)) {
            insert_block(statement);
        }
    }
}

bool NormalizeBlockStatements::is_else_if(Statement* statement, Statement* parent) {
    const CSharpSlotInfo* slot = statement->slot();
    return is<IfElseStatement>(parent) && slot != nullptr && slot->kind() == &Slots::false_statement;
}

void NormalizeBlockStatements::insert_block(Statement* statement) {
    if (!is<BlockStatement>(statement)) {
        auto* b = make<BlockStatement>();
        context_->step("Add block statement", statement);
        statement->replace_with(b);
        if (is<EmptyStatement>(statement) && !statement->has_children()) {
            copy_annotations_from(b, statement);
        } else {
            b->add(statement);
        }
        context_->end_step(b);
    }
}

bool NormalizeBlockStatements::is_allowed_as_embedded_statement(Statement* statement, Statement* parent) {
    if (auto* ies = as<IfElseStatement>(statement)) {
        const CSharpSlotInfo* slot = ies->slot();
        return is<IfElseStatement>(parent) && slot != nullptr && slot->kind() == &Slots::false_statement;
    }
    if (is<VariableDeclarationStatement>(statement) || is<WhileStatement>(statement) || is<DoWhileStatement>(statement) ||
        is<SwitchStatement>(statement) || is<ForeachStatement>(statement) || is<ForStatement>(statement) || is<LockStatement>(statement) ||
        is<FixedStatement>(statement))
        return false;
    if (auto* us = as<UsingStatement>(statement)) return is<UsingStatement>(parent) && !us->is_enhanced();
    return !(parent != nullptr && is<IfElseStatement>(parent->parent()));
}

void NormalizeBlockStatements::visit_property_declaration(PropertyDeclaration& property_declaration) {
    if (context_->settings.use_expression_body_for_calculated_getter_only_properties) {
        simplify_property_declaration(&property_declaration);
    }
    DepthFirstAstVisitor::visit_property_declaration(property_declaration);
}

void NormalizeBlockStatements::visit_indexer_declaration(IndexerDeclaration& indexer_declaration) {
    if (context_->settings.use_expression_body_for_calculated_getter_only_properties) {
        simplify_indexer_declaration(&indexer_declaration);
    }
    DepthFirstAstVisitor::visit_indexer_declaration(indexer_declaration);
}

void NormalizeBlockStatements::simplify_property_declaration(PropertyDeclaration* property_declaration) {
    pattern_matching::Match m = pattern_matching::match(calculated_getter_only_property_pattern(), property_declaration);
    if (!m.success()) return;
    Accessor* getter = property_declaration->getter();
    if (getter == nullptr) return;
    if ((getter->modifiers() & ~movable_modifiers) != Modifiers::None) return;
    context_->step("Use expression-bodied property", property_declaration);
    property_declaration->set_modifiers(property_declaration->modifiers() | getter->modifiers());
    property_declaration->set_expression_body(detach(single<Expression>(m, "expression")));
    copy_annotations_from(property_declaration, getter);
    getter->remove();
}

void NormalizeBlockStatements::simplify_indexer_declaration(IndexerDeclaration* indexer_declaration) {
    pattern_matching::Match m = pattern_matching::match(calculated_getter_only_indexer_pattern(), indexer_declaration);
    if (!m.success()) return;
    Accessor* getter = indexer_declaration->getter();
    if (getter == nullptr) return;
    if ((getter->modifiers() & ~movable_modifiers) != Modifiers::None) return;
    context_->step("Use expression-bodied indexer", indexer_declaration);
    indexer_declaration->set_modifiers(indexer_declaration->modifiers() | getter->modifiers());
    indexer_declaration->set_expression_body(detach(single<Expression>(m, "expression")));
    copy_annotations_from(indexer_declaration, getter);
    getter->remove();
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
