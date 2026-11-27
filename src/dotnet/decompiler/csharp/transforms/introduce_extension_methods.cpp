// dotnet/decompiler/csharp/transforms/introduce_extension_methods.cpp -- see introduce_extension_methods.hpp.
#include "dotnet/decompiler/csharp/transforms/introduce_extension_methods.hpp"

#include <cassert>
#include <optional>
#include <string>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/csharp/type_system/csharp_type_resolve_context.hpp"
#include "dotnet/decompiler/csharp/type_system/using_scope.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/semantics/conversion.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"
#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using resolver::CSharpResolver;
using semantics::ResolveResult;
using type_system::CSharpTypeResolveContext;
using type_system::UsingScope;

void IntroduceExtensionMethods::run(AstNode* root_node, TransformContext& context) {
    context_ = &context;
    // The decompiler attaches a UsingScope annotation to the syntax-tree root.
    initialize_context(root_node->annotation<UsingScope>());
    root_node->accept_visitor(static_cast<IAstVisitor&>(*this));
}

void IntroduceExtensionMethods::initialize_context(const UsingScope* using_scope) {
    std::string ns = context_->current_type_definition() != nullptr ? context_->current_type_definition()->namespace_() : std::string();
    if (!ns.empty()) {
        std::size_t start = 0;
        while (true) {
            std::size_t dot = ns.find('.', start);
            std::string part = ns.substr(start, dot == std::string::npos ? std::string::npos : dot - start);
            using_scope = using_scope->with_nested_namespace(part);
            if (dot == std::string::npos) break;
            start = dot + 1;
        }
    }
    CSharpTypeResolveContext current_context(&context_->type_system.main_module(), using_scope, context_->current_type_definition());
    resolver_ = make<CSharpResolver>(current_context);
}

void IntroduceExtensionMethods::visit_namespace_declaration(NamespaceDeclaration& namespace_declaration) {
    const UsingScope* using_scope = resolver_->current_using_scope();
    for (const std::string& ident : namespace_declaration.identifiers()) using_scope = using_scope->with_nested_namespace(ident);
    CSharpResolver* previous_resolver = resolver_;
    try {
        resolver_ = make<CSharpResolver>(resolver_->with_current_using_scope(using_scope));
        DepthFirstAstVisitor::visit_namespace_declaration(namespace_declaration);
    } catch (...) {
        resolver_ = previous_resolver;
        throw;
    }
    resolver_ = previous_resolver;
}

void IntroduceExtensionMethods::visit_type_declaration(TypeDeclaration& type_declaration) {
    CSharpResolver* previous_resolver = resolver_;
    resolver_ = make<CSharpResolver>(
        resolver_->with_current_type_definition(dynamic_cast<const typesystem::ITypeDefinition*>(get_symbol(&type_declaration))));
    try {
        DepthFirstAstVisitor::visit_type_declaration(type_declaration);
    } catch (...) {
        resolver_ = previous_resolver;
        throw;
    }
    resolver_ = previous_resolver;
}

void IntroduceExtensionMethods::visit_invocation_expression(InvocationExpression& invocation_ref) {
    InvocationExpression* invocation_expression = &invocation_ref;
    DepthFirstAstVisitor::visit_invocation_expression(invocation_ref);
    MemberReferenceExpression* member_ref_expr = nullptr;
    ResolveResult* target = nullptr;
    Expression* first_argument = nullptr;
    if (!can_transform_to_extension_method_call(invocation_expression, member_ref_expr, target, first_argument)) return;
    const auto* method = dynamic_cast<const typesystem::IMethod*>(get_symbol(invocation_expression));
    bool stepped = false;
    if (auto* dir_expr = as<DirectionExpression>(first_argument)) {
        if (!context_->settings.ref_extension_methods || dir_expr->field_direction() == FieldDirection::Out) return;
        context_->step("Introduce extension method call", invocation_expression);
        stepped = true;
        // A ref/out direction expression always wraps an operand.
        first_argument = dir_expr->expression();
        target = get_resolve_result(first_argument);
        detach(dir_expr);
    } else if (is<NullReferenceExpression>(first_argument)) {
        assert(context_->required_namespaces_superset().contains(method->parameters()[0]->type()->namespace_()));
        context_->step("Introduce extension method call", invocation_expression);
        stepped = true;
        // The replacement is a freshly created CastExpression, so the result is non-null.
        first_argument = first_argument->replace_with([&](Expression* expr) -> Expression* {
            return make<CastExpression>(context_->type_system_ast_builder.convert_type(method->parameters()[0]->type()), detach(expr));
        });
    }
    if (auto* identifier_expression = as<IdentifierExpression>(invocation_expression->target())) {
        if (!stepped) context_->step("Introduce extension method call", invocation_expression);
        detach(identifier_expression);
        member_ref_expr = make<MemberReferenceExpression>(detach(first_argument), method->name(), identifier_expression->type_arguments().detach());
        invocation_expression->set_target(member_ref_expr);
    } else {
        if (!stepped) context_->step("Introduce extension method call", invocation_expression);
        // The target is not an IdentifierExpression, so can_transform_to_extension_method_call matched the MemberReferenceExpression case and
        // member_ref_expr is non-null.
        member_ref_expr->set_target(detach(first_argument));
    }
    if (auto* irr = dynamic_cast<resolver::CSharpInvocationResolveResult*>(get_resolve_result(invocation_expression))) {
        // do not forget to update the CSharpInvocationResolveResult => set IsExtensionMethodInvocation == true
        invocation_expression->remove_annotations<resolver::CSharpInvocationResolveResult>();
        auto* new_resolve_result = make<resolver::CSharpInvocationResolveResult>(
            irr->target_result(), irr->member(), irr->arguments, irr->overload_resolution_errors, /*is_extension_method_invocation*/ true,
            irr->is_expanded_form, irr->is_delegate_invocation, irr->get_argument_to_parameter_map(), irr->initializer_statements);
        invocation_expression->add_annotation(new_resolve_result);
    }
}

bool IntroduceExtensionMethods::can_transform_to_extension_method_call(InvocationExpression* invocation_expression,
                                                                       MemberReferenceExpression*& member_ref_expr, ResolveResult*& target,
                                                                       Expression*& first_argument) {
    const auto* method = dynamic_cast<const typesystem::IMethod*>(get_symbol(invocation_expression));
    member_ref_expr = nullptr;
    target = nullptr;
    first_argument = nullptr;
    if (method == nullptr || !method->is_extension_method() || invocation_expression->arguments().size() == 0) return false;
    std::vector<const typesystem::IType*> type_arguments;
    if (auto* mre = as<MemberReferenceExpression>(invocation_expression->target())) {
        if (mre->type_arguments().size() != 0) type_arguments = method->type_arguments();
        member_ref_expr = mre;
    } else if (auto* ide = as<IdentifierExpression>(invocation_expression->target())) {
        if (ide->type_arguments().size() != 0) type_arguments = method->type_arguments();
        member_ref_expr = nullptr;
    } else {
        return false;
    }
    first_argument = invocation_expression->arguments()[0];
    if (is<NamedArgumentExpression>(first_argument)) return false;
    target = get_resolve_result(first_argument);
    if (auto* crr = dynamic_cast<semantics::ConstantResolveResult*>(target); crr != nullptr && std::holds_alternative<std::monostate>(crr->constant_value())) {
        target = make<semantics::ConversionResolveResult>(method->parameters()[0]->type(), crr, semantics::Conversion::null_literal_conversion());
    } else if (auto* de = as<DirectionExpression>(first_argument)) {
        target = get_resolve_result(de->expression());
    }
    assert(target != nullptr);
    std::vector<ResolveResult*> args(invocation_expression->arguments().size() - 1);
    resolver::ArgumentNames arg_names;
    int pos = 0;
    for (std::size_t i = 1; i < invocation_expression->arguments().size(); i++) {
        Expression* arg = invocation_expression->arguments()[i];
        if (auto* nae = as<NamedArgumentExpression>(arg)) {
            if (arg_names.empty()) arg_names.assign(args.size(), std::nullopt);
            arg_names[pos] = nae->name();
            args[pos] = get_resolve_result(nae->expression());
        } else {
            args[pos] = get_resolve_result(arg);
        }
        pos++;
    }
    return resolver_->can_transform_to_extension_method_call(method, type_arguments, target, args, arg_names);
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
