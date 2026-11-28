// dotnet/decompiler/csharp/transforms/transform_field_and_constructor_initializers.cpp -- see
// transform_field_and_constructor_initializers.hpp.
#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_field_and_constructor_initializers.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/record_decompiler.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/any_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/choice.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/named_node.hpp"
#include "dotnet/decompiler/csharp/syntax/pattern_matching/repeat.hpp"
#include "dotnet/decompiler/csharp/transforms/introduce_unsafe_modifier.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_arena.hpp"
#include "dotnet/decompiler/csharp/transforms/pattern_statement_transform.hpp"
#include "dotnet/decompiler/decompile_run.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/documentation/idocumentation_provider.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/util/decimal.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using il::ILVariable;
using il::VariableKind;
using syntax::pattern_matching::AnyNode;
using syntax::pattern_matching::Choice;
using syntax::pattern_matching::Match;
using syntax::pattern_matching::NamedNode;
using syntax::pattern_matching::Pattern;
using syntax::pattern_matching::Repeat;
using typesystem::Accessibility;
using typesystem::IField;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IParameter;
using typesystem::IProperty;
using typesystem::ITypeDefinition;
using typesystem::TypeKind;

namespace {

// ---- patterns ---------------------------------------------------------------------------------------------------------------------

Expression* named(const std::string& name, Expression* child) { return Expression::from_pattern(make<NamedNode>(name, child)); }


// $this.member = $initializer; or $member = $initializer;
ExpressionStatement* member_initializer_pattern() {
    static ExpressionStatement* const pattern = build_pattern([] {
        auto* this_member = make<MemberReferenceExpression>(make<ThisReferenceExpression>(), std::string(Pattern::any_string));
        auto* choice = make<Choice>();
        choice->add(make<NamedNode>("fieldAccess", this_member));
        choice->add(make<NamedNode>("fieldAccess", make<IdentifierExpression>(std::string(Pattern::any_string))));
        auto* assignment = make<AssignmentExpression>(Expression::from_pattern(choice), Expression::from_pattern(make<AnyNode>("initializer")));
        return make<ExpressionStatement>(assignment);
    });
    return pattern;
}

// ---- keys of the dictionaries -----------------------------------------------------------------------------------------------------

struct MemberHash {
    std::size_t operator()(const IMember* m) const { return static_cast<std::size_t>(m->hash_code()); }
};
struct MemberEqual {
    bool operator()(const IMember* a, const IMember* b) const { return typesystem::member_equals(a, b); }
};

// ---- the analysis -----------------------------------------------------------------------------------------------------------------

enum class InitializerKind { Static, Instance, Primary };

class ConstructorInitializerAnalyzer;

struct InitializerStatement {
    Statement* statement;
    const IMember* member;
    Expression* initializer;
    bool depends_on_constructor_body;
};

class InitializerSequence {
public:
    std::vector<InitializerStatement> statements;

    std::unordered_map<Statement*, std::vector<std::pair<Statement*, Expression*>>> statement_to_other_ctors_map;
    bool has_statement_to_other_ctors_map = false;

    bool has_duplicate_assignments = false;
    bool is_unsafe = false;
    bool covers_full_body = false;

    static std::shared_ptr<InitializerSequence> analyze(ConstructorInitializerAnalyzer& context, ConstructorDeclaration* ctor,
                                                        const IMethod* ctor_method);
    bool is_match(ConstructorDeclaration* ctor);

private:
    static bool can_have_initializer(const IMember* member, ConstructorInitializerAnalyzer& context);
};

class ConstructorInitializerAnalyzer {
public:
    TransformContext& context;
    const ITypeDefinition* type_definition;
    TypeDeclaration* type_declaration;
    const RecordDecompiler* record_decompiler = nullptr;

    std::unordered_map<const IMember*, EntityDeclaration*, MemberHash, MemberEqual> member_to_declaring_syntax_node_map;
    bool has_member_map = false;

    struct BackingStore {
        const IProperty* property;
        const IField* field;
    };
    std::unordered_map<const IParameter*, BackingStore> primary_constructor_parameter_to_backing_store_map;
    bool has_primary_constructor_parameter_map = false;
    std::unordered_map<const IField*, ILVariable*, MemberHash, MemberEqual> backing_field_to_primary_constructor_parameter_variable_map;
    bool has_backing_field_map = false;

    std::shared_ptr<InitializerSequence> instance_initializers;
    std::shared_ptr<InitializerSequence> static_initializers;
    std::shared_ptr<InitializerSequence> primary_constructor_initializers;

    const IMethod* static_constructor = nullptr;
    ConstructorDeclaration* static_constructor_decl = nullptr;

    const IMethod* record_copy_constructor = nullptr;
    ConstructorDeclaration* record_copy_constructor_decl = nullptr;

    const IMethod* primary_constructor = nullptr;
    ConstructorDeclaration* primary_constructor_decl = nullptr;

    std::vector<ConstructorDeclaration*> instance_constructors;

    ConstructorInitializerAnalyzer(TransformContext& context_, const ITypeDefinition* type_definition_, TypeDeclaration* type_declaration_)
        : context(context_), type_definition(type_definition_), type_declaration(type_declaration_) {
        auto record = context.decompile_run.record_decompilers.find(type_definition);
        record_decompiler = record != context.decompile_run.record_decompilers.end() ? record->second.get() : nullptr;
    }

    const metadata::MetadataFile& main_file() const { return *context.type_system.main_metadata_module().metadata_file(); }

    bool is_before_field_init() const {
        if (type_definition == nullptr || type_definition->metadata_token().is_nil()) return false;
        const auto& metadata = main_file().metadata();
        auto td = metadata.get_type_definition(cb::dotnet::metadata::TypeDefinitionHandle(type_definition->metadata_token()));
        constexpr std::uint32_t before_field_init = 0x00100000;
        return (td.attributes() & before_field_init) != 0;
    }

    bool analyze(const std::vector<EntityDeclaration*>& members);
    bool move_constructor_initializer(ConstructorDeclaration* constructor_declaration, const IMethod* ctor_method);
    bool move_field_initializers_to_declarations(InitializerSequence& sequence, InitializerKind kind);
    void remove_implicit_constructor();
};

std::shared_ptr<InitializerSequence> InitializerSequence::analyze(ConstructorInitializerAnalyzer& context, ConstructorDeclaration* ctor,
                                                                  const IMethod* ctor_method) {
    auto sequence = std::make_shared<InitializerSequence>();
    sequence->is_unsafe = ctor->has_modifier(Modifiers::Unsafe);
    std::unordered_set<const IMember*, MemberHash, MemberEqual> initialized_members;
    il::ILFunction* function = ctor->annotation<il::ILFunction>();
    bool is_struct = ctor_method->declaring_type()->kind() == TypeKind::Struct;

    bool only_move_constants = !context.context.settings.always_move_initializer && !context.is_before_field_init() && ctor_method->is_static();
    bool skipped_stmts = false;

    Statement* stmt;
    for (stmt = ctor->body() != nullptr ? ctor->body()->statements().first_or_null() : nullptr; stmt != nullptr; stmt = get_next_statement(stmt)) {
        Match m = pattern_matching::match(member_initializer_pattern(), stmt);
        if (!m.success()) break;
        AstNode* field_access = single<AstNode>(m, "fieldAccess");
        Expression* initializer = single<Expression>(m, "initializer");
        const auto* symbol = dynamic_cast<const IMember*>(get_symbol(field_access));
        const IMember* member = symbol != nullptr ? symbol->member_definition() : nullptr;
        if (member == nullptr || !can_have_initializer(member, context)) break;
        const auto* member_field = dynamic_cast<const IField*>(member);
        if (only_move_constants && !(member_field != nullptr && member_field->is_const())) {
            skipped_stmts = true;
            continue;
        }
        if (!initialized_members.insert(member).second) sequence->has_duplicate_assignments = true;

        bool depends_on_body = false;

        for (il::ArenaObject* annotation : initializer->annotations()) {
            auto* instruction = dynamic_cast<il::ILInstruction*>(annotation);
            if (instruction == nullptr) continue;
            L2Lane* l2 = context.context.l2;
            std::vector<ILVariable*> named;
            if (l2 != nullptr) {
                named = l2->variables_named(*instruction);
            } else {
                for (il::ILInstruction* inst : instruction->descendants())
                    if (auto* with_variable = dynamic_cast<il::IInstructionWithVariableOperand*>(inst)) named.push_back(with_variable->variable());
            }
            for (ILVariable* v : named) {
                if (v->function() == function && variable_of(l2, *v).kind == VariableKind::Parameter) depends_on_body = true;
            }
        }

        sequence->statements.push_back({stmt, member, initializer, depends_on_body});
    }

    if (!skipped_stmts) {
        if (stmt == nullptr) {
            sequence->covers_full_body = true;
        } else {
            Match m = pattern_matching::match(is_struct ? TransformFieldAndConstructorInitializers::this_call_struct_pattern()
                                                        : TransformFieldAndConstructorInitializers::this_call_class_pattern(),
                                              stmt);
            if (m.success()) {
                sequence->covers_full_body = get_next_statement(stmt) == nullptr;
            }
        }
    }

    return sequence;
}

bool InitializerSequence::can_have_initializer(const IMember* member, ConstructorInitializerAnalyzer& context) {
    if (!context.has_member_map) return false;
    auto it = context.member_to_declaring_syntax_node_map.find(member);
    if (it == context.member_to_declaring_syntax_node_map.end()) return true;
    EntityDeclaration* declaring_syntax_node = it->second;
    if (is<FieldDeclaration>(declaring_syntax_node)) return true;
    if (auto* pd = as<PropertyDeclaration>(declaring_syntax_node)) return pd->is_automatic_property();
    return is<EventDeclaration>(declaring_syntax_node);
}

bool InitializerSequence::is_match(ConstructorDeclaration* ctor) {
    if (ctor->body() == nullptr) return false;
    auto& stmts = ctor->body()->statements();
    Statement* other_stmt = stmts.first_or_null();
    for (auto& entry : statements) {
        Match m = pattern_matching::match(member_initializer_pattern(), other_stmt);
        if (!m.success()) return false;
        assert(other_stmt != nullptr);  // because m.success
        AstNode* field_access = single<AstNode>(m, "fieldAccess");
        const auto* symbol = dynamic_cast<const IMember*>(get_symbol(field_access));
        const IMember* other_member = symbol != nullptr ? symbol->member_definition() : nullptr;
        if (!typesystem::member_equals(entry.member, other_member)) return false;
        Expression* other_initializer = single<Expression>(m, "initializer");
        if (!pattern_matching::is_match(entry.initializer, other_initializer)) return false;
        has_statement_to_other_ctors_map = true;
        statement_to_other_ctors_map[entry.statement].push_back({other_stmt, other_initializer});
        other_stmt = get_next_statement(other_stmt);
    }
    return true;
}

// ---- ConstructorInitializerAnalyzer -----------------------------------------------------------------------------------------------

bool ConstructorInitializerAnalyzer::analyze(const std::vector<EntityDeclaration*>& members) {
    member_to_declaring_syntax_node_map.clear();
    for (EntityDeclaration* m : members) {
        const auto* symbol = dynamic_cast<const IMember*>(get_symbol(m));
        if (symbol == nullptr) continue;
        if (!member_to_declaring_syntax_node_map.emplace(symbol, m).second)
            throw std::invalid_argument("An item with the same key has already been added.");
    }
    has_member_map = true;

    std::vector<ConstructorDeclaration*> constructors_not_chained_with_this;
    std::vector<ConstructorDeclaration*> all_ctors;

    // we use the row number of the first method in the metadata table as a heuristic
    // to determine the primary constructor in non-record structs.
    // This is not completely reliable, but works.
    const auto& metadata = main_file().metadata();
    auto type_def = metadata.get_type_definition(cb::dotnet::metadata::TypeDefinitionHandle(type_definition->metadata_token()));
    auto methods = type_def.get_methods();
    std::uint32_t first_method_row_number = methods.empty() ? 0 : cb::dotnet::metadata::get_row_number(methods[0]);

    for (EntityDeclaration* member : members) {
        auto* ctor = as<ConstructorDeclaration>(member);
        if (ctor == nullptr) continue;
        const auto* ctor_method = dynamic_cast<const IMethod*>(get_symbol(ctor));
        assert(ctor_method->is_constructor());
        assert(!ctor_method->metadata_token().is_nil());

        std::uint32_t row_number = cb::dotnet::metadata::get_row_number(ctor_method->metadata_token());
        assert(row_number > 0);
        (void)row_number;

        if (record_decompiler != nullptr && typesystem::member_equals(ctor_method, record_decompiler->primary_constructor())) {
            assert(primary_constructor_decl == nullptr);
            primary_constructor = ctor_method;
            primary_constructor_decl = ctor;
            primary_constructor_parameter_to_backing_store_map.clear();
            for (const auto& entry : record_decompiler->get_parameter_to_backing_store_map())
                primary_constructor_parameter_to_backing_store_map[entry.parameter] = {entry.property, entry.field};
            has_primary_constructor_parameter_map = true;
            constructors_not_chained_with_this.push_back(ctor);
        } else if (ctor_method->is_static()) {
            assert(static_constructor_decl == nullptr);
            static_constructor = ctor_method;
            static_constructor_decl = ctor;
        } else if (record_decompiler != nullptr && record_decompiler->is_copy_constructor(ctor_method)) {
            assert(record_copy_constructor_decl == nullptr);
            record_copy_constructor = ctor_method;
            record_copy_constructor_decl = ctor;
        } else {
            // find this-ctor call
            Statement* stmt = ctor->body() != nullptr ? ctor->body()->statements().first_or_null() : nullptr;
            Match m = pattern_matching::match(ctor_method->declaring_type()->kind() == TypeKind::Struct
                                                  ? TransformFieldAndConstructorInitializers::this_call_struct_pattern()
                                                  : TransformFieldAndConstructorInitializers::this_call_class_pattern(),
                                              stmt);

            all_ctors.push_back(ctor);

            if (m.success() && is<ThisReferenceExpression>(single<Expression>(m, "target"))) continue;

            constructors_not_chained_with_this.push_back(ctor);
        }
    }

    Accessibility expected_ctor_accessibility = type_definition->is_abstract() ? Accessibility::Protected : Accessibility::Public;

    if (context.settings.use_primary_constructor_syntax_for_non_record_types && record_decompiler == nullptr &&
        constructors_not_chained_with_this.size() == 1) {
        assert(primary_constructor == nullptr);

        // this constructor could be converted to a primary constructor
        ConstructorDeclaration* ctor = constructors_not_chained_with_this[0];
        const auto* ctor_method = dynamic_cast<const IMethod*>(get_symbol(constructors_not_chained_with_this[0]));

        auto initializer = InitializerSequence::analyze(*this, ctor, ctor_method);

        if (initializer != nullptr && initializer->covers_full_body && !initializer->has_duplicate_assignments &&
            !initializer->statements.empty() && ctor_method->accessibility() == expected_ctor_accessibility) {
            bool transform_to_primary_constructor = cb::dotnet::metadata::get_row_number(ctor_method->metadata_token()) == first_method_row_number;

            if (ctor_method->parameters().empty()) {
                transform_to_primary_constructor = false;
            }

            for (const auto& entry : initializer->statements) {
                const auto* f = dynamic_cast<const IField*>(entry.member);
                if (f != nullptr && TransformFieldAndConstructorInitializers::is_generated_primary_constructor_backing_field(f)) {
                    auto* rr = entry.initializer->annotation<ILVariableResolveResult>();
                    ILVariable* variable = rr != nullptr ? rr->variable : nullptr;
                    const std::optional<VariableFacts> facts = variable != nullptr ? std::optional<VariableFacts>(variable_of(context.l2, *variable)) : std::nullopt;
                    if (facts && facts->kind == VariableKind::Parameter && facts->index.has_value() && *facts->index >= 0) {
                        const IParameter* p = ctor_method->parameters()[static_cast<std::size_t>(*facts->index)];
                        has_primary_constructor_parameter_map = true;
                        has_backing_field_map = true;
                        primary_constructor_parameter_to_backing_store_map[p] = {nullptr, f};
                        backing_field_to_primary_constructor_parameter_variable_map[f] = variable;
                        transform_to_primary_constructor = true;
                    }
                }
            }

            if (context.settings.show_xml_documentation && context.decompile_run.documentation_provider != nullptr) {
                documentation::IDocumentationProvider* provider = context.decompile_run.documentation_provider;
                std::optional<std::string> class_doc = provider->get_documentation(ctor_method->declaring_type_definition());
                std::optional<std::string> ctor_doc = provider->get_documentation(ctor_method);
                if (ctor_doc.has_value() && ctor_doc != class_doc) transform_to_primary_constructor = false;
            }

            if (transform_to_primary_constructor) {
                has_primary_constructor_parameter_map = true;
                primary_constructor_decl = ctor;
                primary_constructor = ctor_method;
                primary_constructor_initializers = initializer;
            }
        }
    }

    if (static_constructor != nullptr) {
        static_initializers = InitializerSequence::analyze(*this, static_constructor_decl, static_constructor);
    }

    if (primary_constructor != nullptr) {
        assert(primary_constructor_decl != nullptr);

        // if there exists a primary constructor, all other constructors must call it
        assert(constructors_not_chained_with_this.size() == 1);

        if (primary_constructor_initializers == nullptr)
            primary_constructor_initializers = InitializerSequence::analyze(*this, primary_constructor_decl, primary_constructor);
    }

    if (!constructors_not_chained_with_this.empty()) {
        if (type_definition->kind() != TypeKind::Struct ||
            (context.settings.struct_default_constructors_and_field_initializers && !type_definition->is_record())) {
            bool is_primary_ctor = constructors_not_chained_with_this[0] == primary_constructor_decl;
            std::shared_ptr<InitializerSequence> sequence =
                is_primary_ctor ? primary_constructor_initializers
                                : InitializerSequence::analyze(
                                      *this, constructors_not_chained_with_this[0],
                                      dynamic_cast<const IMethod*>(get_symbol(constructors_not_chained_with_this[0])));

            if (sequence == nullptr) return false;

            bool sequence_matches_all_ctors = true;
            for (std::size_t i = 1; i < constructors_not_chained_with_this.size(); i++) {
                if (!sequence->is_match(constructors_not_chained_with_this[i])) {
                    sequence_matches_all_ctors = false;
                    break;
                }
            }

            if (!sequence_matches_all_ctors) {
                // The non-this-chained constructors disagree on their leading field
                // assignments, so there is no shared field-initializer sequence to extract.
                // A primary constructor must extract its initializers (its parameters drive
                // them), so bail; otherwise keep the assignments in the bodies but continue,
                // so the this(...)/base(...) chains still get lifted to initializers.
                if (is_primary_ctor) return false;
            } else if (!is_primary_ctor) {
                bool any_depends_on_body = std::any_of(sequence->statements.begin(), sequence->statements.end(),
                                                       [](const InitializerStatement& s) { return s.depends_on_constructor_body; });
                if (!any_depends_on_body) instance_initializers = sequence;
            }
        }
    }

    instance_constructors = all_ctors;

    return true;
}

bool ConstructorInitializerAnalyzer::move_constructor_initializer(ConstructorDeclaration* constructor_declaration, const IMethod* ctor_method) {
    if (constructor_declaration->body() == nullptr) return false;
    Statement* stmt = constructor_declaration->body()->statements().first_or_null();
    bool is_value_type = ctor_method->declaring_type()->kind() == TypeKind::Struct;

    // value types may omit the constructor initializer completely
    if (stmt == nullptr && is_value_type) {
        return true;
    }

    Match m = pattern_matching::match(is_value_type ? TransformFieldAndConstructorInitializers::this_call_struct_pattern()
                                                    : TransformFieldAndConstructorInitializers::this_call_class_pattern(),
                                      stmt);

    if (!m.success()) return is_value_type;

    assert(stmt != nullptr);  // because m.success

    AstNode* invocation = single<AstNode>(m, "invocation");
    const auto* ctor = dynamic_cast<const IMethod*>(get_symbol(invocation));
    if (ctor == nullptr || !ctor->is_constructor()) return false;

    ConstructorInitializerType type = ctor->declaring_type_definition() == ctor_method->declaring_type_definition()
                                          ? ConstructorInitializerType::This
                                          : ConstructorInitializerType::Base;

    auto* ci = make<ConstructorInitializer>(type);

    context.step("Move constructor call to initializer", stmt);
    // Move arguments from invocation to initializer:
    invocation->get_children(Slots::argument).move_to(ci->arguments());
    // Add the initializer: (unless it is the default 'base()')
    if (!(ci->constructor_initializer_type() == ConstructorInitializerType::Base && ci->arguments().count() == 0))
        constructor_declaration->set_initializer(copy_annotations_from(ci, invocation));

    // Remove the statement
    stmt->remove();
    context.end_step(constructor_declaration->initializer());

    return true;
}

// Evaluates a call to the decimal-ctor.
bool try_evaluate_decimal_constant(semantics::ResolveResult* expression, util::Decimal* value) {
    *value = util::Decimal();
    if (!typesystem::is_known_type(expression->type(), typesystem::KnownTypeCode::Decimal)) {
        return false;
    }
    if (auto* rr = dynamic_cast<resolver::CSharpInvocationResolveResult*>(expression)) {
        const auto* ctor = dynamic_cast<const IMethod*>(rr->member());
        if (ctor == nullptr || ctor->symbol_kind() != typesystem::SymbolKind::Constructor) return false;
        std::vector<semantics::ResolveResult*> args = rr->get_arguments_for_call();
        if (args.size() == 1) {
            typesystem::ConstantValue c = args[0]->constant_value();
            if (const auto* d = std::get_if<double>(&c)) {
                *value = util::Decimal::from_double(*d);
                return true;
            } else if (const auto* f = std::get_if<float>(&c)) {
                *value = util::Decimal::from_float(*f);
                return true;
            } else if (const auto* l = std::get_if<std::int64_t>(&c)) {
                *value = util::Decimal::from_int64(*l);
                return true;
            } else if (const auto* i = std::get_if<std::int32_t>(&c)) {
                *value = util::Decimal::from_int64(*i);
                return true;
            } else if (const auto* ul = std::get_if<std::uint64_t>(&c)) {
                *value = util::Decimal::from_uint64(*ul);
                return true;
            } else if (const auto* ui = std::get_if<std::uint32_t>(&c)) {
                *value = util::Decimal::from_uint64(*ui);
                return true;
            }
            // (upstream also takes an int[4] of the decimal's bits; a constant value here is never an array)
            return false;
        } else if (args.size() == 5) {
            typesystem::ConstantValue c0 = args[0]->constant_value(), c1 = args[1]->constant_value(), c2 = args[2]->constant_value(),
                                      c3 = args[3]->constant_value(), c4 = args[4]->constant_value();
            const auto* lo = std::get_if<std::int32_t>(&c0);
            const auto* mid = std::get_if<std::int32_t>(&c1);
            const auto* hi = std::get_if<std::int32_t>(&c2);
            const auto* is_negative = std::get_if<bool>(&c3);
            const auto* scale = std::get_if<std::uint8_t>(&c4);
            if (lo != nullptr && mid != nullptr && hi != nullptr && is_negative != nullptr && scale != nullptr) {
                *value = util::Decimal(static_cast<std::uint32_t>(*lo), static_cast<std::uint32_t>(*mid), static_cast<std::uint32_t>(*hi),
                                       *is_negative, *scale);
                return true;
            }
        }
        return false;
    }
    typesystem::ConstantValue c = expression->constant_value();
    if (const auto* v = std::get_if<util::Decimal>(&c)) {
        *value = *v;
        return true;
    }
    return false;
}

bool ConstructorInitializerAnalyzer::move_field_initializers_to_declarations(InitializerSequence& sequence, InitializerKind kind) {
    for (const InitializerStatement& entry : sequence.statements) {
        Statement* stmt = entry.statement;
        Expression* initializer = entry.initializer;
        assert(!entry.depends_on_constructor_body || kind == InitializerKind::Primary);

        auto it = member_to_declaring_syntax_node_map.find(entry.member);
        if (it == member_to_declaring_syntax_node_map.end()) {
            // A primary-constructor parameter assignment whose backing member has no separate
            // declaration is redundant and dropped. For static/instance initializers a missing
            // declaration instead means the member is not part of this (partial) syntax tree --
            // e.g. when a single static constructor is decompiled in isolation -- so the
            // assignment must remain in the constructor body.
            if (kind == InitializerKind::Primary) {
                context.step("Remove redundant primary constructor assignment", stmt);
                stmt->remove();
            }
            continue;
        }
        EntityDeclaration* declaring_syntax_node = it->second;

        if (auto* fd = as<FieldDeclaration>(declaring_syntax_node)) {
            VariableInitializer* v = fd->variables().first_or_null();
            if (v->initializer() == nullptr) {
                context.step("Move assignment to field initializer", stmt);
                Expression* moved_initializer = detach(initializer);
                v->set_initializer(moved_initializer);
                context.end_step(moved_initializer);
            } else if (kind == InitializerKind::Static) {
                // decimal constants already have an initializer in the AST at this point,
                // because it was added in CSharpDecompiler.DoDecompile(IField, ...)
                semantics::ResolveResult* constant = get_resolve_result(v->initializer());
                semantics::ResolveResult* expression = get_resolve_result(initializer);
                util::Decimal value;
                if (constant->is_compile_time_constant() && try_evaluate_decimal_constant(expression, &value)) {
                    // decimal values do not match, skip transformation?
                    typesystem::ConstantValue constant_value = constant->constant_value();
                    const auto* constant_decimal = std::get_if<util::Decimal>(&constant_value);
                    if (constant_decimal == nullptr || util::Decimal::compare(value, *constant_decimal) != 0) continue;
                } else {
                    // already has an initializer - do not modify
                    assert(false && "Field already has an initializer");
                }
            } else {
                // already has an initializer - do not modify
                assert(false && "Field already has an initializer");
            }
        } else if (auto* pd = as<PropertyDeclaration>(declaring_syntax_node)) {
            assert(pd->is_automatic_property());
            if (pd->initializer() == nullptr) {
                context.step("Move assignment to property initializer", stmt);
                Expression* moved_initializer = detach(initializer);
                pd->set_initializer(moved_initializer);
                context.end_step(moved_initializer);
            } else {
                // already has an initializer - do not modify
                assert(false && "Property already has an initializer");
            }
        } else if (auto* ev = as<EventDeclaration>(declaring_syntax_node)) {
            VariableInitializer* v = ev->variables().first_or_null();
            if (v->initializer() == nullptr) {
                context.step("Move assignment to event initializer", stmt);
                Expression* moved_initializer = detach(initializer);
                v->set_initializer(moved_initializer);
                context.end_step(moved_initializer);
            } else {
                // already has an initializer - do not modify
                assert(false && "Event already has an initializer");
            }
        } else {
            // cannot move initializer
            continue;
        }
        // Remove the statement from all constructors
        stmt->remove();

        auto other = sequence.statement_to_other_ctors_map.find(stmt);
        if (sequence.has_statement_to_other_ctors_map && other != sequence.statement_to_other_ctors_map.end()) {
            std::vector<Expression*> other_initializers;
            other_initializers.reserve(other->second.size());
            for (auto& [other_stmt, other_initializer] : other->second) {
                other_stmt->remove();
                other_initializers.push_back(other_initializer);
            }
            // Preserve the discarded copies so the breakpoint for this initializer can be
            // emitted in every constructor that runs it, not just the one it was lifted from.
            if (!other_initializers.empty()) {
                initializer->add_annotation(make<MemberInitializerInOtherConstructorsAnnotation>(other_initializers));
            }
        }

        if (sequence.is_unsafe && IntroduceUnsafeModifier::is_unsafe(initializer)) {
            context.step("Add unsafe modifier to initialized member", declaring_syntax_node);
            declaring_syntax_node->set_modifiers(declaring_syntax_node->modifiers() | Modifiers::Unsafe);
        }
    }
    return true;
}

void ConstructorInitializerAnalyzer::remove_implicit_constructor() {
    assert(has_member_map);
    assert(type_definition != nullptr);

    // We do not want to hide the constructor if the user explicitly selected it in the tree view.
    if (type_declaration == nullptr) return;

    // Remove primary constructor body
    if (primary_constructor != nullptr) {
        assert(primary_constructor_decl != nullptr);

        type_declaration->set_has_primary_constructor(!primary_constructor->parameters().empty() ||
                                                      primary_constructor_decl->initializer() != nullptr ||
                                                      type_definition->kind() == TypeKind::Struct);

        // HACK: because our current AST model doesn't allow specifying an explicit ordering across slots,
        // we have to explicitly insert the primary constructor parameters,
        // MoveTo would just append the parameters to the list of children
        if (primary_constructor_decl->parameters().count() > 0) {
            AstNode* insertion_point = type_declaration->type_parameters().last_or_null();
            if (insertion_point == nullptr) insertion_point = type_declaration->name_token();
            for (ParameterDeclaration* param : primary_constructor_decl->parameters().to_vector()) {
                context.step("Move primary constructor parameter to type", param);
                param->remove();
                type_declaration->insert_child_after(insertion_point, param, Slots::parameter);
                insertion_point = param;
            }
        }

        assert(has_primary_constructor_parameter_map);

        for (ParameterDeclaration* pd : type_declaration->primary_constructor_parameters().to_vector()) {
            auto* rr = pd->annotation<ILVariableResolveResult>();
            ILVariable* v = rr != nullptr ? rr->variable : nullptr;
            const std::optional<int> index = v != nullptr ? variable_of(context.l2, *v).index : std::nullopt;
            if (!index.has_value() || *index < 0) continue;
            const IParameter* p = primary_constructor->parameters()[static_cast<std::size_t>(*index)];

            auto found = primary_constructor_parameter_to_backing_store_map.find(p);
            if (found == primary_constructor_parameter_to_backing_store_map.end()) {
                // no backing store, constructor parameter was left unassigned or
                // assigned to a different member in a sub-expression:
                // private int someField = param + param2;
                continue;
            }

            const IProperty* prop = found->second.property;
            const IField* field = found->second.field;

            if (prop != nullptr) {
                std::vector<Attribute*> attributes;
                for (const typesystem::IAttribute* attr : prop->get_attributes())
                    attributes.push_back(context.type_system_ast_builder.convert_attribute(attr));
                if (!attributes.empty()) {
                    auto* section = make<AttributeSection>();
                    section->set_attribute_target("property");
                    section->attributes().add_range(attributes);
                    pd->attributes().add(section);
                }
            }
            if (field != nullptr) {
                std::vector<Attribute*> attributes;
                const auto& removed_types = PatternStatementTransform::attribute_types_to_remove_from_auto_properties();
                for (const typesystem::IAttribute* a : field->get_attributes()) {
                    if (std::find(removed_types.begin(), removed_types.end(), a->attribute_type()->full_name()) != removed_types.end()) continue;
                    attributes.push_back(context.type_system_ast_builder.convert_attribute(a));
                }
                if (!attributes.empty()) {
                    auto* section = make<AttributeSection>();
                    section->set_attribute_target("field");
                    section->attributes().add_range(attributes);
                    pd->attributes().add(section);
                }
            }
        }

        if (primary_constructor_decl->has_modifier(Modifiers::Unsafe)) {
            context.step("Move unsafe modifier from primary constructor to type", type_declaration);
            type_declaration->set_modifiers(type_declaration->modifiers() | Modifiers::Unsafe);
        }

        ConstructorInitializer* initializer = primary_constructor_decl->initializer();
        if (initializer != nullptr && type_declaration->base_types().count() > 0) {
            assert(initializer->constructor_initializer_type() == ConstructorInitializerType::Base);

            AstType* base_type = type_declaration->base_types().first_or_null();
            auto* new_base_type = make<InvocationAstType>();
            context.step("Move primary constructor initializer to base type", base_type);
            base_type->replace_with(new_base_type);
            new_base_type->set_base_type(base_type);
            initializer->arguments().move_to(new_base_type->arguments());
            context.end_step(new_base_type);
        }

        context.step("Remove primary constructor body", primary_constructor_decl);
        primary_constructor_decl->remove();
    }

    // Remove static constructor
    if (static_constructor != nullptr) {
        assert(static_constructor_decl != nullptr);

        if (is_before_field_init() && static_constructor_decl->body() != nullptr && static_constructor_decl->body()->statements().count() == 0) {
            context.step("Remove empty static constructor", static_constructor_decl);
            static_constructor_decl->remove();
        }
    }

    // More than one constructor - do not remove anything
    if (instance_constructors.size() != 1) return;

    ConstructorDeclaration* ctor = instance_constructors[0];
    const auto* ctor_method = dynamic_cast<const IMethod*>(get_symbol(ctor));

    if (type_definition->kind() == TypeKind::Struct && ctor_method->parameters().empty() && instance_initializers != nullptr) {
        // struct constructor with initializers is not optional
        return;
    }

    // dynamically create a pattern of an empty ctor
    auto* empty_ctor_pattern = make<ConstructorDeclaration>();
    empty_ctor_pattern->set_modifiers(type_definition->is_abstract() ? Modifiers::Protected : Modifiers::Public);
    if (ctor->has_modifier(Modifiers::Unsafe)) empty_ctor_pattern->set_modifiers(empty_ctor_pattern->modifiers() | Modifiers::Unsafe);
    empty_ctor_pattern->set_body(make<BlockStatement>());

    if (pattern_matching::is_match(empty_ctor_pattern, ctor)) {
        bool retain_because_of_documentation = context.settings.show_xml_documentation && context.decompile_run.documentation_provider != nullptr &&
                                               context.decompile_run.documentation_provider->get_documentation(ctor_method).has_value();
        if (!retain_because_of_documentation) {
            context.step("Remove implicit constructor", ctor);
            ctor->remove();
        }
    }
}

}  // namespace

// ---- the transform ----------------------------------------------------------------------------------------------------------------

AstNode* TransformFieldAndConstructorInitializers::this_call_class_pattern() {
    static AstNode* const pattern = build_pattern([] {
        auto* cast_choice = make<Choice>();
        cast_choice->add(make<NamedNode>("target", make<ThisReferenceExpression>()));
        cast_choice->add(make<NamedNode>("target", make<BaseReferenceExpression>()));
        auto* cast = make<CastExpression>(AstType::from_pattern(make<AnyNode>()), Expression::from_pattern(cast_choice));
        auto* choice = make<Choice>();
        choice->add(make<NamedNode>("target", make<ThisReferenceExpression>()));
        choice->add(make<NamedNode>("target", make<BaseReferenceExpression>()));
        choice->add(cast);
        auto* target = make<MemberReferenceExpression>(Expression::from_pattern(choice), ".ctor");
        auto* call = make<InvocationExpression>(target, std::initializer_list<Expression*>{Expression::from_pattern(make<Repeat>(make<AnyNode>()))});
        return static_cast<AstNode*>(make<ExpressionStatement>(Expression::from_pattern(make<NamedNode>("invocation", call))));
    });
    return pattern;
}

AstNode* TransformFieldAndConstructorInitializers::this_call_struct_pattern() {
    static AstNode* const pattern = build_pattern([] {
        auto* create = make<ObjectCreateExpression>(AstType::from_pattern(make<AnyNode>()),
                                                    std::initializer_list<Expression*>{Expression::from_pattern(make<Repeat>(make<AnyNode>()))});
        auto* assignment = make<AssignmentExpression>(named("target", make<ThisReferenceExpression>()), named("invocation", create));
        return static_cast<AstNode*>(make<ExpressionStatement>(assignment));
    });
    return pattern;
}

bool TransformFieldAndConstructorInitializers::is_generated_primary_constructor_backing_field(const IField* field) {
    std::string name = field->name();
    return name.starts_with("<") && name.ends_with(">P") && is_compiler_generated(field);
}

void TransformFieldAndConstructorInitializers::run(AstNode* node, TransformContext& context) {
    context_ = &context;

    if (context.current_type_definition() != nullptr) {
        std::vector<EntityDeclaration*> members;
        for (AstNode* child : node->children().to_vector()) {
            if (auto* entity = as<EntityDeclaration>(child)) members.push_back(entity);
        }
        transform_declaration(context.current_type_definition(), node, members);
    }

    for (AstNode* descendant : node->descendants().to_vector()) {
        auto* type_declaration = as<TypeDeclaration>(descendant);
        if (type_declaration == nullptr) continue;
        const auto* current_type_definition = dynamic_cast<const ITypeDefinition*>(get_symbol(type_declaration));
        std::vector<EntityDeclaration*> members = type_declaration->members().to_vector();
        transform_declaration(current_type_definition, type_declaration, members);
    }
}

bool TransformFieldAndConstructorInitializers::transform_declaration(const ITypeDefinition* current_type_definition, AstNode* node,
                                                                     const std::vector<EntityDeclaration*>& members) {
    ConstructorInitializerAnalyzer analyzer(*context_, current_type_definition, as<TypeDeclaration>(node));

    if (!analyzer.analyze(members)) return false;

    if (analyzer.primary_constructor_initializers != nullptr && !analyzer.primary_constructor_initializers->has_duplicate_assignments) {
        analyzer.move_field_initializers_to_declarations(*analyzer.primary_constructor_initializers, InitializerKind::Primary);
    } else if (analyzer.instance_initializers != nullptr && !analyzer.instance_initializers->has_duplicate_assignments) {
        analyzer.move_field_initializers_to_declarations(*analyzer.instance_initializers, InitializerKind::Instance);
    }

    if (analyzer.static_initializers != nullptr && !analyzer.static_initializers->has_duplicate_assignments) {
        analyzer.move_field_initializers_to_declarations(*analyzer.static_initializers, InitializerKind::Static);
    }

    for (EntityDeclaration* member : members) {
        auto* constructor_declaration = as<ConstructorDeclaration>(member);
        if (constructor_declaration == nullptr) continue;
        analyzer.move_constructor_initializer(constructor_declaration, dynamic_cast<const IMethod*>(get_symbol(constructor_declaration)));
    }

    analyzer.remove_implicit_constructor();

    if (!analyzer.has_backing_field_map) {
        return false;
    }

    for (AstNode* descendant : node->descendants().to_vector()) {
        auto* identifier = as<Identifier>(descendant);
        if (identifier == nullptr) continue;
        AstNode* parent = identifier->parent();
        const auto* field = parent != nullptr ? dynamic_cast<const IField*>(get_symbol(parent)) : nullptr;
        if (field == nullptr) {
            continue;
        }
        auto found = analyzer.backing_field_to_primary_constructor_parameter_variable_map.find(
            dynamic_cast<const IField*>(field->member_definition()));
        if (found == analyzer.backing_field_to_primary_constructor_parameter_variable_map.end()) {
            continue;
        }
        ILVariable* v = found->second;
        parent->remove_annotations<semantics::MemberResolveResult>();
        parent->add_annotation(make<ILVariableResolveResult>(v));
        identifier->replace_with(Identifier::create(*variable_of(context_->l2, *v).name));
    }

    return true;
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
