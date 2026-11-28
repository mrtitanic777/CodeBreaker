// dotnet/decompiler/csharp/transforms/introduce_using_declarations.cpp -- see introduce_using_declarations.hpp.
#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "dotnet/decompiler/csharp/transforms/introduce_using_declarations.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/csharp/csharp_decompiler.hpp"
#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/syntax/depth_first_ast_visitor.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/csharp/type_system/csharp_type_resolve_context.hpp"
#include "dotnet/decompiler/csharp/type_system/using_scope.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/util/culture_compare.hpp"
#include "dotnet/decompiler/util/insertion_ordered_map.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using resolver::CSharpResolver;
using type_system::CSharpTypeResolveContext;
using type_system::UsingScope;
using typesystem::IType;

namespace {

// ICSharpCode.Decompiler.TypeSystem.Implementation.DefaultVariable: a variable known by name and type, to put in the resolver's scope.
class DefaultVariable final : public il::ArenaObject, public typesystem::IVariable {
public:
    DefaultVariable(const IType* type, std::string name) : type_(type), name_(std::move(name)) {}

    typesystem::SymbolKind symbol_kind() const override { return typesystem::SymbolKind::Variable; }
    std::string name() const override { return name_; }
    const IType* type() const override { return type_; }
    bool is_const() const override { return false; }
    typesystem::ConstantValue get_constant_value(bool) const override { return {}; }

private:
    const IType* type_;
    std::string name_;
};

// .NET's culture-sensitive string comparison (en-US, ICU), which upstream's OrderBy/ThenByDescending on namespace names sorts by.
using util::culture_compare;

// Finds the namespaces the types the tree refers to are in.
class FindRequiredImports final : public DepthFirstAstVisitor {
public:
    explicit FindRequiredImports(const TransformContext& context) : l2_(context.l2) {
        current_namespace_ = context.current_type_definition() != nullptr ? context.current_type_definition()->namespace_() : std::string();
    }

    L2Lane* l2_;
    std::unordered_set<std::string> declared_namespaces = {std::string()};
    std::unordered_set<std::string> imported_namespaces;

    void visit_simple_type(SimpleType& simple_type) override {
        auto* trr = simple_type.annotation<semantics::TypeResolveResult>();
        add_imported_namespace(trr != nullptr ? trr->type() : nullptr);
        DepthFirstAstVisitor::visit_simple_type(simple_type);  // also visit type arguments
    }

    void visit_namespace_declaration(NamespaceDeclaration& namespace_declaration) override {
        std::string old_namespace = current_namespace_;
        for (const std::string& ident : namespace_declaration.identifiers()) {
            current_namespace_ = NamespaceDeclaration::build_qualified_name(current_namespace_, ident);
            declared_namespaces.insert(current_namespace_);
        }
        DepthFirstAstVisitor::visit_namespace_declaration(namespace_declaration);
        current_namespace_ = old_namespace;
    }

    void visit_foreach_statement(ForeachStatement& foreach_statement) override {
        auto* annotation = foreach_statement.annotation<ForeachAnnotation>();
        if (annotation != nullptr) {
            auto* call = dynamic_cast<il::CallInstruction*>(annotation->get_enumerator_call);
            const IMethod* method = call != nullptr ? (l2_ != nullptr ? l2_->call_method(*call) : call->method) : nullptr;
            if (method != nullptr && method->is_extension_method()) add_imported_namespace(method->declaring_type());
        }
        DepthFirstAstVisitor::visit_foreach_statement(foreach_statement);
    }

    void visit_parenthesized_variable_designation(ParenthesizedVariableDesignation& designation) override {
        auto* annotation = designation.annotation<il::MatchInstruction>();
        const IMethod* method = annotation != nullptr ? (l2_ != nullptr ? l2_->match_method(*annotation) : annotation->method()) : nullptr;
        if (method != nullptr) add_imported_namespace(method->declaring_type());
        DepthFirstAstVisitor::visit_parenthesized_variable_designation(designation);
    }

    void visit_tuple_expression(TupleExpression& tuple_expression) override {
        auto* annotation = tuple_expression.annotation<il::MatchInstruction>();
        const IMethod* method = annotation != nullptr ? (l2_ != nullptr ? l2_->match_method(*annotation) : annotation->method()) : nullptr;
        if (method != nullptr) add_imported_namespace(method->declaring_type());
        DepthFirstAstVisitor::visit_tuple_expression(tuple_expression);
    }

    void visit_array_initializer_expression(ArrayInitializerExpression& array_initializer_expression) override {
        for (Expression* item : array_initializer_expression.elements()) {
            auto* optional_call = item->annotation<il::CallInstruction>();
            const IMethod* method = optional_call != nullptr ? (l2_ != nullptr ? l2_->call_method(*optional_call) : optional_call->method) : nullptr;
            if (method != nullptr && method->is_extension_method() && method->name() == "Add") add_imported_namespace(method->declaring_type());
        }
        DepthFirstAstVisitor::visit_array_initializer_expression(array_initializer_expression);
    }

private:
    bool is_parent_of_current_namespace(const std::string& ns) const {
        if (ns.empty()) return true;
        if (current_namespace_.starts_with(ns)) {
            if (current_namespace_.size() == ns.size()) return true;
            if (current_namespace_[ns.size()] == '.') return true;
        }
        return false;
    }

    void add_imported_namespace(const IType* type) {
        if (type != nullptr && !is_parent_of_current_namespace(type->namespace_())) imported_namespaces.insert(type->namespace_());
    }

    std::string current_namespace_;
};

// Replaces the type names that do not resolve to their type in the scope they are written in (ambiguous names) with qualified ones.
class FullyQualifyAmbiguousTypeNamesVisitor final : public DepthFirstAstVisitor {
public:
    FullyQualifyAmbiguousTypeNamesVisitor(TransformContext& context, const UsingScope* using_scope)
        : context_(context), ignore_using_scope_(!context.settings.using_declarations), settings_(context.settings) {
        resolver_ = make<CSharpResolver>(CSharpTypeResolveContext(&context.type_system.main_module()));
        if (!ignore_using_scope_) {
            std::string ns = context.current_type_definition() != nullptr ? context.current_type_definition()->namespace_() : std::string();
            if (!ns.empty()) {
                std::size_t start = 0;
                while (true) {
                    std::size_t dot = ns.find('.', start);
                    using_scope = using_scope->with_nested_namespace(ns.substr(start, dot == std::string::npos ? std::string::npos : dot - start));
                    if (dot == std::string::npos) break;
                    start = dot + 1;
                }
            }
            resolver_ = make<CSharpResolver>(resolver_->with_current_using_scope(using_scope).with_current_type_definition(context.current_type_definition()));
        }
        ast_builder_ = create_ast_builder(resolver_);
    }

    void visit_namespace_declaration(NamespaceDeclaration& namespace_declaration) override {
        if (ignore_using_scope_) {
            DepthFirstAstVisitor::visit_namespace_declaration(namespace_declaration);
            return;
        }
        CSharpResolver* previous_resolver = resolver_;
        std::shared_ptr<syntax::TypeSystemAstBuilder> previous_ast_builder = ast_builder_;
        const UsingScope* using_scope = resolver_->current_using_scope();
        for (const std::string& ident : namespace_declaration.identifiers()) using_scope = using_scope->with_nested_namespace(ident);
        resolver_ = make<CSharpResolver>(resolver_->with_current_using_scope(using_scope));
        try {
            ast_builder_ = create_ast_builder(resolver_);
            DepthFirstAstVisitor::visit_namespace_declaration(namespace_declaration);
        } catch (...) {
            ast_builder_ = previous_ast_builder;
            resolver_ = previous_resolver;
            throw;
        }
        ast_builder_ = previous_ast_builder;
        resolver_ = previous_resolver;
    }

    void visit_type_declaration(TypeDeclaration& type_declaration) override {
        if (ignore_using_scope_) {
            DepthFirstAstVisitor::visit_type_declaration(type_declaration);
            return;
        }
        if (type_declaration.has_primary_constructor()) {
            in_primary_constructor_ = true;
            try {
                for (ParameterDeclaration* parameter : type_declaration.primary_constructor_parameters().to_vector())
                    parameter->accept_visitor(static_cast<IAstVisitor&>(*this));
            } catch (...) {
                in_primary_constructor_ = false;
                throw;
            }
            in_primary_constructor_ = false;
        }
        CSharpResolver* previous_resolver = resolver_;
        std::shared_ptr<syntax::TypeSystemAstBuilder> previous_ast_builder = ast_builder_;
        resolver_ = make<CSharpResolver>(resolver_->with_current_type_definition(dynamic_cast<const typesystem::ITypeDefinition*>(get_symbol(&type_declaration))));
        try {
            ast_builder_ = create_ast_builder(resolver_);
            DepthFirstAstVisitor::visit_type_declaration(type_declaration);
        } catch (...) {
            ast_builder_ = previous_ast_builder;
            resolver_ = previous_resolver;
            throw;
        }
        ast_builder_ = previous_ast_builder;
        resolver_ = previous_resolver;
    }

    void visit_parameter_declaration(ParameterDeclaration& parameter_declaration) override {
        // Parameters of primary constructors are visited separately from the rest of the type declaration since their types are at the same
        // scope as the type declaration and so need to use the outer resolver. This check ensures that the visitor only runs once per
        // parameter since their AstNodes will get revisited by the call to the base visit_type_declaration in visit_type_declaration above.
        if (in_primary_constructor_ || !is<TypeDeclaration>(parameter_declaration.parent()))
            DepthFirstAstVisitor::visit_parameter_declaration(parameter_declaration);
    }

    void visit_method_declaration(MethodDeclaration& declaration) override {
        visit_entity(declaration, [&] { DepthFirstAstVisitor::visit_method_declaration(declaration); });
    }
    void visit_accessor(Accessor& accessor) override {
        visit_entity(accessor, [&] { DepthFirstAstVisitor::visit_accessor(accessor); });
    }
    void visit_constructor_declaration(ConstructorDeclaration& declaration) override {
        visit_entity(declaration, [&] { DepthFirstAstVisitor::visit_constructor_declaration(declaration); });
    }
    void visit_destructor_declaration(DestructorDeclaration& declaration) override {
        visit_entity(declaration, [&] { DepthFirstAstVisitor::visit_destructor_declaration(declaration); });
    }
    void visit_operator_declaration(OperatorDeclaration& declaration) override {
        visit_entity(declaration, [&] { DepthFirstAstVisitor::visit_operator_declaration(declaration); });
    }

    void visit_simple_type(SimpleType& simple_type) override {
        auto* rr = simple_type.annotation<semantics::TypeResolveResult>();
        if (rr == nullptr) {
            DepthFirstAstVisitor::visit_simple_type(simple_type);
            return;
        }
        ast_builder_->name_lookup_mode = simple_type.get_name_lookup_mode();
        if (ast_builder_->name_lookup_mode == resolver::NameLookupMode::Type) {
            AstType* outermost_type = &simple_type;
            while (is<AstType>(outermost_type->parent())) outermost_type = cast<AstType>(outermost_type->parent());
            if (is<TypeReferenceExpression>(outermost_type->parent())) {
                // ILSpy uses TypeReferenceExpression in expression context even when the C# parser wouldn't know that it's a type reference.
                // Fall back to expression-mode lookup in these cases:
                ast_builder_->name_lookup_mode = resolver::NameLookupMode::Expression;
            }
        }
        if (is<Attribute>(simple_type.parent())) {
            replace_and_record_step("Qualify ambiguous attribute type", &simple_type, ast_builder_->convert_attribute_type(rr->type()));
        } else {
            replace_and_record_step("Qualify ambiguous type", &simple_type, ast_builder_->convert_type(rr->type()));
        }
    }

private:
    std::shared_ptr<syntax::TypeSystemAstBuilder> create_ast_builder(CSharpResolver* resolver, il::ILFunction* function = nullptr) {
        if (function != nullptr) {
            auto variables = std::make_shared<resolver::VariableDictionary>();
            for (il::ILVariable* variable : context_.l2 != nullptr ? context_.l2->function_variables(*function) : function->variables().items()) {
                const VariableFacts v = variable_of(context_.l2, *variable);
                if (v.kind != il::VariableKind::Parameter && v.name && !variables->contains(*v.name))
                    variables->set(*v.name, make<DefaultVariable>(v.type, *v.name));
            }
            resolver = make<CSharpResolver>(resolver->add_variables(variables));
        }
        auto builder = std::make_shared<syntax::TypeSystemAstBuilder>(resolver);
        builder->use_nullable_specifier_for_value_types = settings_.lift_nullables;
        builder->always_use_global = settings_.always_use_global;
        builder->add_resolve_result_annotations = true;
        builder->use_aliases = true;
        return builder;
    }

    void visit_entity(EntityDeclaration& entity_declaration, const std::function<void()>& base_call) {
        if (ignore_using_scope_) {
            base_call();
            return;
        }
        const auto* method = dynamic_cast<const typesystem::IMethod*>(get_symbol(&entity_declaration));
        if (method == nullptr) {
            base_call();
            return;
        }
        CSharpResolver* previous_resolver = resolver_;
        std::shared_ptr<syntax::TypeSystemAstBuilder> previous_ast_builder = ast_builder_;
        if (CSharpDecompiler::is_windows_forms_initialize_component_method(method)) {
            CSharpTypeResolveContext current_context(&previous_resolver->compilation().main_module());
            resolver_ = make<CSharpResolver>(current_context);
        } else {
            resolver_ = make<CSharpResolver>(resolver_->with_current_member(method));
        }
        try {
            auto* function = entity_declaration.annotation<il::ILFunction>();
            ast_builder_ = create_ast_builder(resolver_, function);
            base_call();
        } catch (...) {
            resolver_ = previous_resolver;
            ast_builder_ = previous_ast_builder;
            throw;
        }
        resolver_ = previous_resolver;
        ast_builder_ = previous_ast_builder;
    }

    // Upstream records a debug step here only when the converted type differs from the original (a structural comparison that is
    // debug-only); the step log is off, so this just replaces.
    void replace_and_record_step(const char*, SimpleType* simple_type, AstType* replacement) { simple_type->replace_with(replacement); }

    TransformContext& context_;
    bool ignore_using_scope_;
    const DecompilerSettings& settings_;
    CSharpResolver* resolver_ = nullptr;
    std::shared_ptr<syntax::TypeSystemAstBuilder> ast_builder_;
    bool in_primary_constructor_ = false;
};

}  // namespace

void IntroduceUsingDeclarations::run(AstNode* root_node, TransformContext& context) {
    // First determine all the namespaces that need to be imported:
    FindRequiredImports required_imports(context);
    root_node->accept_visitor(static_cast<IAstVisitor&>(required_imports));
    std::vector<const typesystem::INamespace*> resolved_namespaces;
    if (context.settings.using_declarations) {
        // #define directives are leading trivia on the SyntaxTree, not children, so using declarations go at the very start of the member list.
        AstNode* insertion_point = nullptr;
        // Now add using declarations for those namespaces:
        std::vector<std::string> sorted_imports(required_imports.imported_namespaces.begin(), required_imports.imported_namespaces.end());
        std::stable_sort(sorted_imports.begin(), sorted_imports.end(), [](const std::string& a, const std::string& b) {
            bool a_system = a.starts_with("System");
            bool b_system = b.starts_with("System");
            if (a_system != b_system) return !a_system;
            return culture_compare(a, b) > 0;
        });
        for (const std::string& ns : sorted_imports) {
            assert(context.required_namespaces_superset().contains(ns));
            // we go backwards (OrderByDescending) through the list of namespaces because we insert them backwards
            // (always inserting at the start of the list)
            std::vector<std::string> parts;
            std::size_t start = 0;
            while (true) {
                std::size_t dot = ns.find('.', start);
                parts.push_back(ns.substr(start, dot == std::string::npos ? std::string::npos : dot - start));
                if (dot == std::string::npos) break;
                start = dot + 1;
            }
            AstType* ns_type = make<SimpleType>(parts[0]);
            for (std::size_t i = 1; i < parts.size(); i++) ns_type = make<MemberType>(ns_type, parts[i]);
            const typesystem::INamespace* resolved_namespace = typesystem::get_namespace_by_full_name(context.type_system, ns);
            if (resolved_namespace != nullptr) resolved_namespaces.push_back(resolved_namespace);
            context.step("Add using declaration", root_node);
            auto* node = make<UsingDeclaration>(ns_type);
            root_node->insert_child_after(insertion_point, node, Slots::member);
            context.end_step(node);
        }
    }
    auto* using_scope = make<UsingScope>(CSharpTypeResolveContext(&context.type_system.main_module()), &context.type_system.root_namespace(), resolved_namespaces);
    root_node->add_annotation(using_scope);
    // verify that the SimpleTypes refer to the correct type (no ambiguities)
    FullyQualifyAmbiguousTypeNamesVisitor visitor(context, using_scope);
    root_node->accept_visitor(static_cast<IAstVisitor&>(visitor));
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
