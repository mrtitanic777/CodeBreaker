// dotnet/decompiler/csharp/transforms/introduce_unsafe_modifier.cpp -- see introduce_unsafe_modifier.hpp.
#include "dotnet/decompiler/csharp/transforms/introduce_unsafe_modifier.hpp"

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/transforms/transform_context.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/operator_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::transforms {

using namespace syntax;
using typesystem::IType;
using typesystem::TypeKind;

void IntroduceUnsafeModifier::run(AstNode* compilation_unit, TransformContext& context) {
    context_ = &context;
    try {
        compilation_unit->accept_visitor(static_cast<IAstVisitorT<bool>&>(*this));
    } catch (...) {
        context_ = nullptr;
        throw;
    }
    context_ = nullptr;
}

bool IntroduceUnsafeModifier::is_unsafe(AstNode* node) {
    IntroduceUnsafeModifier visitor;
    return node->accept_visitor(static_cast<IAstVisitorT<bool>&>(visitor));
}

void IntroduceUnsafeModifier::step(const std::string& description, AstNode* node) {
    if (context_ != nullptr) context_->step(description, node);
}

void IntroduceUnsafeModifier::end_step(AstNode* node) {
    if (context_ != nullptr) context_->end_step(node);
}

bool IntroduceUnsafeModifier::visit_children(AstNode& node) {
    bool result = false;
    AstNode* next = nullptr;
    for (AstNode* child = node.first_child(); child != nullptr; child = next) {
        // Store next to allow the loop to continue if the visitor removes/replaces child.
        next = child->next_sibling();
        result |= child->accept_visitor(static_cast<IAstVisitorT<bool>&>(*this));
    }
    if (result && is<EntityDeclaration>(&node) && !is<Accessor>(&node)) {
        step("Add unsafe modifier", &node);
        auto* entity = static_cast<EntityDeclaration*>(&node);
        entity->set_modifiers(entity->modifiers() | Modifiers::Unsafe);
        return false;
    }
    return result;
}

bool IntroduceUnsafeModifier::visit_pointer_reference_expression(PointerReferenceExpression& pointer_reference_expression) {
    DepthFirstAstVisitorT<bool>::visit_pointer_reference_expression(pointer_reference_expression);
    return true;
}

bool IntroduceUnsafeModifier::visit_size_of_expression(SizeOfExpression& size_of_expression) {
    // C# sizeof(MyStruct) requires unsafe{}
    // (not for sizeof(int), but that gets constant-folded and thus decompiled to 4)
    DepthFirstAstVisitorT<bool>::visit_size_of_expression(size_of_expression);
    return true;
}

bool IntroduceUnsafeModifier::visit_composed_type(ComposedType& composed_type) {
    if (composed_type.pointer_rank() > 0) return true;
    return DepthFirstAstVisitorT<bool>::visit_composed_type(composed_type);
}

bool IntroduceUnsafeModifier::visit_function_pointer_type(FunctionPointerAstType&) { return true; }

bool IntroduceUnsafeModifier::visit_unary_operator_expression(UnaryOperatorExpression& unary_operator_expression) {
    bool result = DepthFirstAstVisitorT<bool>::visit_unary_operator_expression(unary_operator_expression);
    if (unary_operator_expression.operator_() == UnaryOperatorType::Dereference) {
        auto* bop = as<BinaryOperatorExpression>(unary_operator_expression.expression());
        if (bop != nullptr && bop->operator_() == BinaryOperatorType::Add) {
            auto* orr = dynamic_cast<semantics::OperatorResolveResult*>(get_resolve_result(bop));
            if (orr != nullptr && !orr->operands().empty() && orr->operands()[0]->type()->kind() == TypeKind::Pointer) {
                step("Replace pointer addition with indexer", &unary_operator_expression);
                // transform "*(ptr + int)" to "ptr[int]"
                IndexerExpression* indexer = make<IndexerExpression>();
                indexer->set_target(detach(bop->left()));
                indexer->arguments().add(detach(bop->right()));
                copy_annotations_from(indexer, &unary_operator_expression);
                copy_annotations_from(indexer, bop);
                unary_operator_expression.replace_with(indexer);
                end_step(indexer);
            }
        }
        return true;
    }
    if (unary_operator_expression.operator_() == UnaryOperatorType::AddressOf) return true;
    return result;
}

bool IntroduceUnsafeModifier::visit_member_reference_expression(MemberReferenceExpression& member_reference_expression) {
    bool result = DepthFirstAstVisitorT<bool>::visit_member_reference_expression(member_reference_expression);
    auto* uoe = as<UnaryOperatorExpression>(member_reference_expression.target());
    if (uoe != nullptr && uoe->operator_() == UnaryOperatorType::Dereference) {
        step("Replace pointer member access", &member_reference_expression);
        PointerReferenceExpression* pre = make<PointerReferenceExpression>();
        pre->set_target(detach(uoe->expression()));
        pre->set_member_name(member_reference_expression.member_name());
        member_reference_expression.type_arguments().move_to(pre->type_arguments());
        copy_annotations_from(pre, uoe);
        pre->remove_annotations<semantics::ResolveResult>();  // only copy the ResolveResult from the MRE
        copy_annotations_from(pre, &member_reference_expression);
        member_reference_expression.replace_with(pre);
        end_step(pre);
    }
    if (has_unsafe_resolve_result(&member_reference_expression)) return true;
    return result;
}

bool IntroduceUnsafeModifier::visit_identifier_expression(IdentifierExpression& identifier_expression) {
    bool result = DepthFirstAstVisitorT<bool>::visit_identifier_expression(identifier_expression);
    if (has_unsafe_resolve_result(&identifier_expression)) return true;
    return result;
}

bool IntroduceUnsafeModifier::visit_stack_alloc_expression(StackAllocExpression& stack_alloc_expression) {
    bool result = DepthFirstAstVisitorT<bool>::visit_stack_alloc_expression(stack_alloc_expression);
    if (has_unsafe_resolve_result(&stack_alloc_expression)) return true;
    return result;
}

bool IntroduceUnsafeModifier::visit_invocation_expression(InvocationExpression& invocation_expression) {
    bool result = DepthFirstAstVisitorT<bool>::visit_invocation_expression(invocation_expression);
    if (has_unsafe_resolve_result(&invocation_expression)) return true;
    return result;
}

bool IntroduceUnsafeModifier::visit_object_create_expression(ObjectCreateExpression& object_create_expression) {
    bool result = DepthFirstAstVisitorT<bool>::visit_object_create_expression(object_create_expression);
    if (has_unsafe_resolve_result(&object_create_expression)) return true;
    return result;
}

bool IntroduceUnsafeModifier::visit_fixed_variable_initializer(FixedVariableInitializer& fixed_variable_initializer) {
    DepthFirstAstVisitorT<bool>::visit_fixed_variable_initializer(fixed_variable_initializer);
    return true;
}

bool IntroduceUnsafeModifier::has_unsafe_resolve_result(const AstNode* node) {
    semantics::ResolveResult* rr = get_resolve_result(node);
    if (rr == nullptr) return false;
    if (is_unsafe_type(rr->type())) return true;
    auto* member_rr = dynamic_cast<semantics::MemberResolveResult*>(rr);
    if (const auto* pm = member_rr != nullptr ? dynamic_cast<const typesystem::IParameterizedMember*>(member_rr->member()) : nullptr) {
        for (const typesystem::IParameter* p : pm->parameters())
            if (is_unsafe_type(p->type())) return true;
    } else if (dynamic_cast<resolver::MethodGroupResolveResult*>(rr) != nullptr) {
        const typesystem::ISymbol* chosen_method = get_symbol(node);
        if (const auto* pm2 = dynamic_cast<const typesystem::IParameterizedMember*>(chosen_method)) {
            if (is_unsafe_type(pm2->return_type())) return true;
            for (const typesystem::IParameter* p : pm2->parameters())
                if (is_unsafe_type(p->type())) return true;
        }
    }
    return false;
}

bool IntroduceUnsafeModifier::is_unsafe_type(const IType* type) {
    if (type == nullptr) return false;
    switch (type->kind()) {
        case TypeKind::Pointer:
        case TypeKind::FunctionPointer:
            return true;
        case TypeKind::ByReference:
        case TypeKind::Array:
            return is_unsafe_type(dynamic_cast<const typesystem::TypeWithElementType*>(type)->element_type());
        default:
            return false;
    }
}

}  // namespace cb::dotnet::decompiler::csharp::transforms
